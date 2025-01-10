#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include <array>
#include "events.h"
#include "employees.h"
#include "products.h"
#include "customers_orders.h"
#include "handler.h"
#include "check_date.h"


#include "financial.h"

// funcții aferente clasei FinancialReport

// aici am doi constructori diferiți, în funcție de argumentele primite
// un exemplu de polimorfism
FinancialReport::FinancialReport(const string& date_in ,const float wages_in, const float product_costs_in, const float product_revenues_in, const float event_revenues_in, const float event_costs_in, const float total_expenses_in, const float total_revenue_in, const float net_total_in) {
    date = date_in;
    wages = wages_in;
    product_costs = product_costs_in;
    product_revenues = product_revenues_in;
    event_revenues = event_revenues_in;
    event_costs = event_costs_in;
    total_expenses = total_expenses_in;
    total_revenue = total_revenue_in;
    net_total = net_total_in;
}

FinancialReport::FinancialReport(string date_in, string root_folder) {
    
    date = date_in;

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


    total_expenses = product_costs + event_costs + wages;
    total_revenue = product_revenues + event_revenues;
    net_total = total_revenue - total_expenses;

}

pair<float,float> FinancialReport::calculate_product_revenue_cost(string date, string root_folder) {
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

float FinancialReport::calculate_wages(string date, string root_folder) {
    EmployeeHandler employees(root_folder);
    float wages= 0;

    // TODO: să văd de ce îmi dă memory leak funcția aia de copiere și să o implementez aici
    for (int i=0; i<employees.employees.size(); i++) {
        wages += employees.employees[i]->get_daily_pay(date);
    }


    return wages;
}

pair<float,float> FinancialReport::calculate_event_revenue_cost(string date, string root_folder) {
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

void FinancialReport::display_report() {
    cout<<"Veniturile magazinului: "<<total_revenue<<endl;
    cout<<" - Venituri din vânzări: "<<product_revenues<<endl;
    cout<<" - Venituri din evenimente: "<<event_revenues<<endl;
    cout<<"Costurile magazinului: "<<total_expenses<<endl;
    cout<<" - Salariile pentru ziua aceasta: "<<wages<<endl;
    cout<<" - Costuri aferente produselor: "<<product_costs<<endl;
    cout<<" - Costuri aferente evenimentelor: "<<event_costs<<endl;
    cout<<"Venitul net al magazinului: "<<net_total<<endl;
}

vector<string> FinancialReport::report_to_data() {
    vector<string> data;
    data.push_back(date);
    data.push_back(to_string(wages));
    data.push_back(to_string(product_costs));
    data.push_back(to_string(product_revenues));
    data.push_back(to_string(event_revenues));
    data.push_back(to_string(event_costs));
    data.push_back(to_string(total_expenses));
    data.push_back(to_string(total_revenue));
    data.push_back(to_string(net_total));
    return data;
}

string FinancialReport::get_date(){
    return this->date;
}

// funcții aferente clasei FinancialReportHandler

FinancialReportHandler::FinancialReportHandler(const string &file_path_in): Handler(file_path_in) {
    report_file_path = file_path+"/ro/reports.csv";
    FinancialReportHandler::parse_data();
}
FinancialReportHandler::~FinancialReportHandler(){
    FinancialReportHandler::write_to_file();
}
FinancialReport FinancialReportHandler::parse_data_element(const vector<string> &data_el){
    if(data_el.size() != 6){
        cout<<"Prea multe câmpuri pentru raport. Datele au fost introduse incorect."<<endl;
        throw 1;
    }
    const string date = data_el[0];
    const float wages = stof(data_el[1]);
    const float product_costs = stof(data_el[2]);
    const float product_revenues = stof(data_el[3]);
    const float event_revenues = stof(data_el[4]);
    const float event_costs = stof(data_el[5]);
    const float total_expenses = stof(data_el[6]);
    const float total_revenue = stof(data_el[7]);
    const float net_total = stof(data_el[8]);
    return FinancialReport(date, wages, product_costs, product_revenues, event_revenues, event_costs,total_expenses,total_revenue,net_total);
}

void FinancialReportHandler::parse_data(){
    CSVInputHandler csv_handler(report_file_path);
    vector<vector<string>> data = csv_handler.get_data();
    for(int i=0; i<data.size(); i++){
        reports.push_back(parse_data_element(data[i]));
    }
}

vector<vector<string>> FinancialReportHandler::get_data() {
    CSVInputHandler csv_handler(report_file_path);
    vector<vector<string>> data_in;

    for (int i=0; i<reports.size(); i++) {
        data_in.push_back(reports[i].report_to_data());
    }

    return data_in;
}

void FinancialReportHandler::write_to_file(){

    vector<vector<string>> data_in = this->get_data();

    CSVInputHandler csv_handler(report_file_path);
    csv_handler.lines_from_data(data_in);
}

void FinancialReportHandler::generate_report(string date){
    FinancialReport new_report(date, file_path);
    new_report.display_report();

    int previous_report_index = report_date_index(date);
    if(previous_report_index!=-1){
        cout<<"Deja a fost generat un raport. Se suprascrie..."<<endl;
        reports[previous_report_index] = new_report;
    }else{
        reports.push_back(new_report);
    }
    
}
//TODO: try/catch pt funcțiile din generarea de raport
FinancialReport FinancialReportHandler::get_report_by_date(string date){
    if(is_future_date(date)){
        cout<<"Data introdusă este din viitor!"<<endl;
        throw 3;
    }

    for(int i=0; i<reports.size(); i++){
        if(date == reports[i].get_date()){
            return reports[i];
        }
    }

    cout<<"Nu a fost generat un raport pentru data introdusă!"<<endl;
    throw 5;
}

int FinancialReportHandler::report_date_index(string date){
     if(is_future_date(date)){
        cout<<"Data introdusă este din viitor!"<<endl;
        throw 3;
    }

    for(int i=0; i<reports.size(); i++){
        if(date == reports[i].get_date()){
            return i;
        }
    }

    cout<<"Nu a fost generat un raport pentru data introdusă!"<<endl;
    return -1;
}
