//
// Created by abratosin on 12/29/24.
//
#include "input_handling.h"
#include "handler.h"
#include "check_date.h"
#include "employees.h"
#include "products.h"
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <fstream>
#include <random>
#include <utility>
#include <array>

using namespace std;





//              CLIENȚI ȘI GESTIONAREA COMENZILOR
class Customer {
private:
    string name;
    // numărul de comenzi date de client, pentru a putea aplica reduceri
    int previous_orders;

public:
    explicit Customer(const string &name_in){
        name = name_in;
        previous_orders = 0;
    }
    Customer(const string &name_in, const int previous_orders_in){
        name = name_in;
        previous_orders = previous_orders_in;
    }
    void set_previous_orders(const int previous_orders_in){
        previous_orders = previous_orders_in;
    }
    string get_name() {
        return name;
    }
    int get_previous_orders(){
        return previous_orders;
    }
    void increment_orders(){
        previous_orders++;
    }

    vector<string> customer_to_data() {
        vector<string> data;
        data.push_back(name);
        data.push_back(to_string(previous_orders));

        return data;
    }

    void display_information() {
        cout<<"Nume: "<<name<<endl;
        cout<<"Nr. comenzi anterioare: "<<previous_orders<<endl;
    }
};
class CustomerHandler: public Handler {
private:
    string customer_file_path;
    vector<Customer> customers;
public:
    CustomerHandler(const string &file_path_in): Handler(file_path_in) {
        customer_file_path = file_path+"/customers.csv";
        CustomerHandler::parse_data();
    }
    ~CustomerHandler() override{
        CustomerHandler::write_to_file();
    }
    Customer parse_data_element(const vector<string> &data_el){
        if(data_el.size() != 2){
            cout<<"Prea multe câmpuri pentru client. Datele au fost introduse incorect."<<endl;
            throw 1;
        }
        const string name = data_el[0];
        const int previous_orders = stoi(data_el[1]);
        return Customer(name, previous_orders);
    }

    void parse_data() override{
        CSVInputHandler csv_handler(customer_file_path);
        vector<vector<string>> data = csv_handler.get_data();
        for(int i=0; i<data.size(); i++){
            customers.push_back(parse_data_element(data[i]));
        }
    }

    vector<vector<string>> get_data() override {
        CSVInputHandler csv_handler(customer_file_path);
        vector<vector<string>> data_in;

        for (int i=0; i<customers.size(); i++) {
            data_in.push_back(customers[i].customer_to_data());
        }

        return data_in;
    }
    void write_to_file() override {
        vector<vector<string>> data_in = this->get_data();

        CSVInputHandler csv_handler(customer_file_path);
        csv_handler.lines_from_data(data_in);
    }

    int search_customer(string const &name, bool const display=false) {
        for (int i=0; i<customers.size(); i++) {
            if (customers[i].get_name() == name) {
                cout<<customers[i].get_name()<<endl;
                cout<<name<<endl;
                if (display) {
                    cout<<"Clientul a fost găsit!"<<endl;
                    customers[i].display_information();
                }
                return i;
            }
        }
        if (display) {
            cout<<"Eroare! Clientul nu a putut fi găsit!"<<endl;
        }
        return -1;
    }

    void add_customer(string const &name) {
        // în mod normal, nu ar trb să se poată ajunge la if-ul ăsta
        if (search_customer(name) != -1) {
            cout<<"Clientul există deja în baza de date!"<<endl;
            throw 2;
        }
        // aici codul e puțin sloppy: aici pun 1 deoarece funcția este apelată doar când un client își adaugă prim
        customers.push_back(Customer(name));
    }

    float get_discount(string const &name) {
        int customer_index = search_customer(name);
        if (customer_index==-1) {
            cout<<"Clientul \""<<name<<"\" nu a putut fi găsit!"<<endl;
            throw 6;
        }
        // 10% reducere pentru >= comenzi, 15% pentru >= 20
        if (customers[customer_index].get_previous_orders()>=20) {
            return 0.2;
        }
        if (customers[customer_index].get_previous_orders()>=10) {
            return 0.1;
        }

        return 0.0;
    }

    void add_order(string const &name) {
        int customer_index = search_customer(name);
        if (customer_index==-1) {
            cout<<"Clientul \""<<name<<"\" nu a putut fi găsit!"<<endl;
            throw 6;
        }

        customers[customer_index].increment_orders();
    }
};

class Order {
private:
    int order_id;
    string customer_name;
    string order_date;
    int product_count;
    vector<string> products;
    float gross_price;
    // discountul de loialitate; float între 0 și 1
    float loyalty_discount;
    // prețul după discount
    float net_price;

public:
    Order(int order_id_in, const string &customer_name_in, const string &order_date_in, int product_count_in, const vector<string> &products_in, const float gross_price_in, const float loyalty_discount_in){
        order_id = order_id_in;
        customer_name = customer_name_in;
        order_date = order_date_in;
        product_count = product_count_in;
        products = products_in;
        gross_price = gross_price_in;
        loyalty_discount = loyalty_discount_in;
        net_price = gross_price - loyalty_discount*gross_price;
    }
    Order(int order_id_in, const string &customer_name_in, const string &order_date_in, int product_count_in, const vector<string> &products_in, const float gross_price_in, const float loyalty_discount_in, const float net_price_in){
        order_id = order_id_in;
        customer_name = customer_name_in;
        order_date = order_date_in;
        product_count = product_count_in;
        products = products_in;
        gross_price = gross_price_in;
        loyalty_discount = loyalty_discount_in;
        net_price = net_price_in;
    }
    string get_date() {
        return this->order_date;
    }
    void display_information() {
        cout<<"Order ID: "<<order_id<<endl;
        cout<<"Customer: "<<customer_name<<endl;
        cout<<"Order date: "<<order_date<<endl;
        cout<<"Products: ";
        for (int i=0; i<products.size(); i++) {
            cout<<products[i]<<", ";
        }
        cout<<endl;
        cout<<"Gross price: "<<gross_price<<endl;
        cout<<"Loyalty discount: "<<loyalty_discount*100<<"%"<<endl;
        cout<<"Net price: "<<net_price<<endl;
    }

    vector<string> order_to_data() {
        vector<string>data;
        data.push_back(to_string(order_id));
        data.push_back(customer_name);
        data.push_back(order_date);
        data.push_back(to_string(product_count));
        string product_str;
        for (int i=0; i<products.size(); i++) {
            product_str+=products[i];
            if (i!=products.size()-1) {
                product_str+="-";
            }
        }
        data.push_back(product_str);
        data.push_back(to_string(gross_price));
        data.push_back(to_string(loyalty_discount));
        data.push_back(to_string(net_price));

        return data;

    }
    vector<string> get_product_names() {
        return products;
    }

};

class OrderHandler: public Handler {
private:
    string order_file_path;
    vector<Order> orders;
public:
    OrderHandler(const string &file_path_in): Handler(file_path_in) {
        order_file_path = file_path+"/orders.csv";
        OrderHandler::parse_data();
    }
    ~OrderHandler() override{
        OrderHandler::write_to_file();
    }
    Order parse_data_from_element(const vector<string> &data_el){
        if(data_el.size() != 8){
            cout<<"Prea multe câmpuri pentru comandă. Datele au fost introduse incorect."<<endl;
            throw 1;
        }
        const int order_id = stoi(data_el[0]);
        const string customer_name = data_el[1];
        const string order_date = data_el[2];
        const int product_count = stoi(data_el[3]);
        vector<string> products;
        stringstream ss(data_el[4]);
        string product;
        while(getline(ss, product, '-')){
            products.push_back(product);
        }
        const float gross_price = stof(data_el[5]);
        const float loyalty_discount = stof(data_el[6]);
        const float net_price = stof(data_el[7]);
        return Order(order_id, customer_name, order_date, product_count, products, gross_price, loyalty_discount, net_price);
    }

    void parse_data() override {
        CSVInputHandler csv_handler(order_file_path);
        vector<vector<string>> data = csv_handler.get_data();
        for(int i=0; i<data.size(); i++){
            orders.push_back(parse_data_from_element(data[i]));
        }
    }

    vector<vector<string>> get_data() override {
        CSVInputHandler csv_handler(order_file_path);
        vector<vector<string>> data_in;

        for (int i=0; i<orders.size(); i++) {
            data_in.push_back(orders[i].order_to_data());
        }

        return data_in;
    }
    void write_to_file() override {
        vector<vector<string>> data_in = this->get_data();

        CSVInputHandler csv_handler(order_file_path);
        csv_handler.lines_from_data(data_in);
    }


    void create_order() {
        ProductHandler product_handler(file_path);
        CustomerHandler customer_handler(file_path);
        // seed pentru generarea unui ID de comandă
        unsigned id_seed = std::chrono::system_clock::now().time_since_epoch().count();
        default_random_engine generator(id_seed);

        int order_id = generator();
        string customer_name;
        string order_date;
        int product_count;
        vector<string> products;
        float gross_price = 0;
        float loyalty_discount;
        float net_price;
        cout<<"Introduceți data comenzii: (dd/mm/yyyy)"<<endl;
        cin>>order_date;
        if (!is_valid_date(order_date)) {
            cout<<"Dată invalidă!"<<endl;
            throw 1;
        }
        if (is_future_date(order_date)) {
            cout<<"Comanda introdusă este din viitor!"<<endl;
            throw 2;
        }
        cout<<"Introduceți numărul de produse: "<<endl;
        cin>>product_count;
        // pentru a putea citi după cu getline()
        cin.ignore();
        for (int i=0; i<product_count; i++) {
            string product_name;
            cout<<"Introduceți numele produsului: ";
            getline(cin, product_name);
            products.push_back(product_name);
        }
        for (int i=0; i<product_count; i++) {
            gross_price+=product_handler.purchase_product(products[i]);
        }
        cout<<"Costul total al comenzii este: "<<gross_price<<"RON"<<endl;
        cout<<"Introduceți numele clientului: "<<endl;
        getline(cin, customer_name);
        if (customer_handler.search_customer(customer_name) == -1) {
            cout<<"Client nou; adăugare în baza de date..."<<endl;
            customer_handler.add_customer(customer_name);
        }
        loyalty_discount = customer_handler.get_discount(customer_name);
        if (loyalty_discount>0) {
            cout<<"Datorită fidelității sale, clientul beneficiază de o reducere de "<<loyalty_discount*100<<"%!"<<endl;
        }
        net_price = gross_price - loyalty_discount*gross_price;
        cout<<"Prețul final al comenzii este: "<<net_price<<"RON"<<endl;

        customer_handler.add_order(customer_name);
        orders.push_back(Order(order_id, customer_name, order_date, product_count, products, gross_price, loyalty_discount, net_price));
    }

    // funcție care returnează toate comenzile făcute într-o anumită zi
    vector<Order> get_orders_by_date(string date) {
        vector<Order> orders;
        for (int i=0; i<orders.size(); i++) {
            if (orders[i].get_date() == date) {
                orders.push_back(orders[i]);
            }
        }

        return orders;
    }
};

class Event {
private:
    string event_name;
    string event_date;
    // variabilă care stochează ce produse vor fi disponibile la eveniment, și în ce cantități
    vector<pair<string,int>> products;
    float marketing_costs;
    float misc_costs;
    float ticket_price;
    int ticket_count;
public:
    Event(const string &event_name_in, const string &event_date_in, const vector<pair<string,int>> &products_in, const float marketing_costs_in, const float misc_costs_in, const float ticket_price_in, const int ticket_count_in){
        event_name = event_name_in;
        event_date = event_date_in;
        products = products_in;
        marketing_costs = marketing_costs_in;
        misc_costs = misc_costs_in;
        ticket_price = ticket_price_in;
        ticket_count = ticket_count_in;
    }

    void display_information(){
        cout<<"Nume eveniment: "<<event_name<<endl;
        cout<<"Data evenimentului: "<<event_date<<endl;
        cout<<"Produsele oferite: "<<endl;
        for (int i=0; i<products.size(); i++) {
            cout<<products[i].first<<": "<<products[i].second<<endl;
        }
        cout<<"Costuri de marketing: "<<marketing_costs<<endl;
        cout<<"Costuri diverse (postere, catering, etc.): "<<misc_costs<<endl;
        cout<<"Prețul unui bilet: "<<ticket_price<<endl;
        cout<<"Numărul de bilete vândute: "<<ticket_count<<endl;
    }

    vector<string> event_to_data() {
        vector<string> data;
        data.push_back(event_name);
        data.push_back(event_date);
        string products_str;
        for (int i=0; i<products.size(); i++) {
            products_str+=products[i].first;
            products_str+="-";
            products_str+=to_string(products[i].second);
            if (i!=products.size()-1) {
                products_str+="/";
            }
        }
        data.push_back(products_str);
        data.push_back(to_string(marketing_costs));
        data.push_back(to_string(misc_costs));
        data.push_back(to_string(ticket_price));
        data.push_back(to_string(ticket_count));

        return data;
    }

    vector<pair<string,int>> get_products() {
        return products;
    }
    float get_ticket_price() {
        return ticket_price;
    }
    int get_ticket_count() {
        return ticket_count;
    }
    float get_misc_costs() {
        return misc_costs;
    }
    float get_marketing_costs() {
        return marketing_costs;
    }
};

class EventHandler: public Handler {
private:
    string event_file_path;
    vector<Event> events;
public:
    EventHandler(const string &file_path_in): Handler(file_path_in) {
        event_file_path = file_path+"/events.csv";
        EventHandler::parse_data();
    }
    ~EventHandler() override{
        EventHandler::write_to_file();
    }
    Event parse_data_element(const vector<string> &data_el){
        if(data_el.size() != 7){
            cout<<"Prea multe câmpuri pentru eveniment. Datele au fost introduse incorect."<<endl;
            throw 1;
        }
        const string event_name = data_el[0];
        const string event_date = data_el[1];
        vector<pair<string,int>> products;
        stringstream ss(data_el[2]);
        string product;
        while(getline(ss, product, '/')){
            stringstream ss_product(product);
            string product_name;
            string product_count;
            getline(ss_product, product_name, '-');
            getline(ss_product, product_count, '-');
            products.push_back(make_pair(product_name, stoi(product_count)));
        }
        const float marketing_costs = stof(data_el[3]);
        const float misc_costs = stof(data_el[4]);
        const float ticket_price = stof(data_el[5]);
        const int ticket_count = stoi(data_el[6]);
        return Event(event_name, event_date, products, marketing_costs, misc_costs, ticket_price, ticket_count);
    }

    void parse_data() override {
        CSVInputHandler csv_handler(event_file_path);
        vector<vector<string>> data = csv_handler.get_data();
        for(int i=0; i<data.size(); i++){
            events.push_back(parse_data_element(data[i]));
        }
    }

    vector<vector<string>> get_data() override {
        CSVInputHandler csv_handler(event_file_path);
        vector<vector<string>> data_in;

        for (int i=0; i<events.size(); i++) {
            data_in.push_back(events[i].event_to_data());
        }

        return data_in;
    }

    void write_to_file() override {

        vector<vector<string>> data_in = this->get_data();

        CSVInputHandler csv_handler(event_file_path);
        csv_handler.lines_from_data(data_in);
    }

    void create_event() {
        ProductHandler product_handler(file_path);


        string event_name;
        string event_date;
        vector<pair<string,int>> products;
        float marketing_costs;
        float misc_costs;
        float ticket_price;
        int ticket_count;
        cout<<"Introduceți numele evenimentului: "<<endl;
        getline(cin, event_name);
        cout<<"Introduceți data evenimentului: (dd/mm/yyyy)"<<endl;
        cin>>event_date;
        if (!is_valid_date(event_date)) {
            cout<<"Dată invalidă!"<<endl;
            throw 1;
        }

        int product_count;
        cout<<"Introduceți numărul de produse oferite: "<<endl;
        cin>>product_count;
        cin.ignore();
        for (int i=0; i<product_count; i++) {
            string product_name;
            int product_number;
            cout<<"Introduceți numele produsului #: "<<i+1<<endl;
            getline(cin, product_name);
            if(product_handler.search_product(product_name)==-1){
                cout<<"Produsul nu se află în gama magazinului!"<<endl;
            }

            cout<<"Introduceți cantitatea produsului: ";
            cin>>product_number;
            products.push_back(make_pair(product_name, product_number));
        }
        cout<<"Introduceți costurile de marketing: "<<endl;
        cin>>marketing_costs;
        cout<<"Introduceți costurile diverse (postere, catering, etc.): "<<endl;
        cin>>misc_costs;
        cout<<"Introduceți prețul unui bilet: "<<endl;
        cin>>ticket_price;
        cout<<"Introduceți numărul de bilete vândute: "<<endl;
        cin>>ticket_count;
        events.push_back(Event(event_name, event_date, products, marketing_costs, misc_costs, ticket_price, ticket_count));
    }

    Event search_event(string const &name, bool const display=false) {
        for (int i=0; i<events.size(); i++) {
            if (events[i].get_products()[0].first == name) {
                if (display) {
                    cout<<"Evenimentul a fost găsit!"<<endl;
                    events[i].display_information();
                }
                return events[i];
            }
        }
        if (display) {
            cout<<"Eroare! Evenimentul nu a putut fi găsit!"<<endl;
        }
        throw 1;
    }

    vector<Event> get_events_by_date(string date){
        vector<Event> events_by_date;
        for (int i=0; i<events.size(); i++) {
            if (events[i].get_products()[0].first == date) {
                events_by_date.push_back(events[i]);
            }
        }
        return events_by_date;
    }

};

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