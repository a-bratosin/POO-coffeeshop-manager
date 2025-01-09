//
// Created by abratosin on 12/29/24.
//
#include "input_handling.h"
#include "handler.h"
#include "check_date.h"
#include "employees.h"
#include "products.h"
#include "customers_orders.h"
#include "events.h"
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <fstream>
#include <random>
#include <utility>
#include <array>

using namespace std;



// clasă care conține raportul financiar calculat pentru o anumită zi
class FinancialReport {
private:
    float wages;
    float product_costs;
    float product_revenues;
    float event_revenues;
    float event_costs;


public:
    // aici am doi constructori diferiți, în funcție de argumentele primite
    // un exemplu de polimorfism
    FinancialReport(const float wages_in, const float product_costs_in, const float product_revenues_in, const float event_revenues_in, const float event_costs_in) {
        wages = wages_in;
        product_costs = product_costs_in;
        product_revenues = product_revenues_in;
        event_revenues = event_revenues_in;
        event_costs = event_costs_in;
    }

    FinancialReport(string date, string root_folder) {

        EmployeeHandler employees(root_folder);
        EventHandler events(root_folder);

        // verific dacă data introdusă este validă
        if (!is_valid_date(date)) {
            cout<<"Dată invalidă!"<<endl;
            throw 1;
        }
        if (is_future_date(date)) {
            cout<<"Rapoartele financiare nu pot fi făcute pentru zile din viitor!"<<endl;
        }

        // mai întâi mă uit pe toate comenzile din ziua respectivă
        // adun prețurile și respectiv costurile produselor

        // operațiile astea le fac în funcții separate ca să fiu sigur că obiectele respective se distrug
        // astfel, mă asigur că nu accesează mai multe obiecte aceleași fișiere
        pair<float,float> revenue_cost = calculate_product_revenue_cost(date, root_folder);
        product_revenues = revenue_cost.first;
        product_costs = revenue_cost.second;

        pair<float,float> event_revenue_cost = calculate_event_revenue_cost(date, root_folder);
        event_revenues = event_revenue_cost.first;
        event_costs = event_revenue_cost.second;

        wages = calculate_wages(date, root_folder);




    }

    pair<float,float> calculate_product_revenue_cost(string date, string root_folder) {
        OrderHandler orders(root_folder);
        ProductHandler product_list(root_folder);

        vector<Order> orders_by_date = orders.get_orders_by_date(date);
        vector<Product> products;

        float revenue = 0;
        float cost = 0;
        for (int i=0; i<orders_by_date.size(); i++) {
            vector<string> product_names = orders_by_date[i].get_product_names();
            for (int j=0; j<product_names.size(); j++) {
                // cursed
                // caută produsul după nume, apoi returnează obiectul Product
                Product current_product = product_list.get_product(product_list.search_product(product_names[j]));
                revenue += current_product.get_price();
                cost += current_product.get_cost();
            }
        }
        return make_pair(revenue, cost);
    }

    float calculate_wages(string date, string root_folder) {
        EmployeeHandler employees(root_folder);
        float wages= 0;

        // TODO: să văd de ce îmi dă memory leak funcția aia de copiere și să o implementez aici
        for (int i=0; i<employees.employees.size(); i++) {
            wages += employees.employees[i]->get_daily_pay(date);
        }


        return wages;
    }

    pair<float,float> calculate_event_revenue_cost(string date, string root_folder) {
        EventHandler events(root_folder);
        vector<Event> events_by_date = events.get_events_by_date(date);
        float revenue = 0;
        float cost = 0;
        for (int i=0; i<events_by_date.size(); i++) {
            revenue += events_by_date[i].get_ticket_price()*events_by_date[i].get_ticket_count();
            // costurile evenimentelor sunt doar costurile de marketing și diverse
            cost += events_by_date[i].get_misc_costs()+events_by_date[i].get_marketing_costs();
        }
        return make_pair(revenue, cost);
    }

    void display_report() {
        cout<<"Veniturile magazinului: "<<product_revenues<<endl;
        cout<<" - Venituri din vânzări: "<<product_revenues<<endl;
        cout<<" - Venituri din evenimente: "<<event_revenues<<endl;
        cout<<"Costurile magazinului: "<<product_costs<<endl;
        cout<<" - Salariile pentru ziua aceasta: "<<wages<<endl;
        cout<<" - Costuri aferente produselor: "<<product_costs<<endl;
        cout<<" - Costuri aferente evenimentelor: "<<event_costs<<endl;
    }

    vector<string> report_to_data() {
        vector<string> data;
        data.push_back(to_string(wages));
        data.push_back(to_string(product_costs));
        data.push_back(to_string(product_revenues));
        data.push_back(to_string(event_revenues));
        data.push_back(to_string(event_costs));

        return data;
    }
};

class FinancialReportHandler: public Handler {
private:
    string report_file_path;
    vector<FinancialReport> reports;
public:
    FinancialReportHandler(const string &file_path_in): Handler(file_path_in) {
        report_file_path = file_path+"/reports.csv";
        FinancialReportHandler::parse_data();
    }
    ~FinancialReportHandler() override{
        FinancialReportHandler::write_to_file();
    }
    FinancialReport parse_data_element(const vector<string> &data_el){
        if(data_el.size() != 5){
            cout<<"Prea multe câmpuri pentru raport. Datele au fost introduse incorect."<<endl;
            throw 1;
        }
        const float wages = stof(data_el[0]);
        const float product_costs = stof(data_el[1]);
        const float product_revenues = stof(data_el[2]);
        const float event_revenues = stof(data_el[3]);
        const float event_costs = stof(data_el[4]);
        return FinancialReport(wages, product_costs, product_revenues, event_revenues, event_costs);
    }

    void parse_data() override{
        CSVInputHandler csv_handler(report_file_path);
        vector<vector<string>> data = csv_handler.get_data();
        for(int i=0; i<data.size(); i++){
            reports.push_back(parse_data_element(data[i]));
        }
    }

    vector<vector<string>> get_data() override {
        CSVInputHandler csv_handler(report_file_path);
        vector<vector<string>> data_in;

        for (int i=0; i<reports.size(); i++) {
            data_in.push_back(reports[i].report_to_data());
        }

        return data_in;
    }

    void write_to_file() override {

        vector<vector<string>> data_in = this->get_data();

        CSVInputHandler csv_handler(report_file_path);
        csv_handler.lines_from_data(data_in);
    }
};

// creez o funcție de template care să se ocupe de traducerea unui document CSV
template <typename T>
void convert_to_english(T handler) {

}

template<>
void convert_to_english(EmployeeHandler &handler) {
    string header_file = "Position/Name/Pay/Shifts";
}

int main(){
    string data_path = "./data/";

    char selection='0';
    while (selection<'1' || selection>'5'){

        cout<<"Selectați localitatea pentru care doriți să faceți operații:"<<endl;
        cout<<"1) București"<<endl;
        cout<<"2) Cluj-Napoca"<<endl;
        cout<<"3) Timișoara"<<endl;
        cout<<"4) Iași"<<endl;
        cout<<"5. Brașov"<<endl;
        cin>>selection;

        switch (selection) {
            case '1':
                data_path+="bucuresti";;
                break;
            case '2':
                data_path+="cluj";
                break;
            case '3':
                data_path+="timisoara";
                break;
            case '4':
                data_path+="iasi";
                break;
            case '5':
                data_path+="brasov";
                break;
            default:
                cout<<"Selectați o opțiune validă!\n"<<endl;
                break;
        }

        cin.clear();
        cin.ignore(100, '\n');
    }

    /*
    ProductHandler product_handler(data_path);
    //product_handler.add_product();
    product_handler.search_product("Sugar Cookie", true);
    product_handler.reset_stock("Sugar Cookie", 20);
    cout<<product_handler.purchase_product("Sugar Cookie")<<endl;
    */

    /*
    OrderHandler order_handler(data_path);
    order_handler.create_order();

    cout<<"end"<<endl;
    */

    EmployeeHandler employee_handler(data_path);
    employee_handler.add_employee();
    cout<<"end"<<endl;
    return 0;
}