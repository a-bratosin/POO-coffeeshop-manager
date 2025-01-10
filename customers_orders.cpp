#include <string>
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include "input_handling.h"
#include "handler.h"
#include "products.h"
#include "check_date.h"

#include "customers_orders.h"

// funcțiile aferente clasei Customer
Customer::Customer(const string &name_in){
    name = name_in;
    previous_orders = 0;
}
Customer::Customer(const string &name_in, const int previous_orders_in){
    name = name_in;
    previous_orders = previous_orders_in;
}
void Customer::set_previous_orders(const int previous_orders_in){
    previous_orders = previous_orders_in;
}


string Customer::get_name() {
    return name;
}
int Customer::get_previous_orders(){
    return previous_orders;
}
void Customer::increment_orders(){
    previous_orders++;
}

vector<string> Customer::customer_to_data() {
    vector<string> data;
    data.push_back(name);
    data.push_back(to_string(previous_orders));

    return data;
}

void Customer::display_information() {
    cout<<"Nume: "<<name<<endl;
    cout<<"Nr. comenzi anterioare: "<<previous_orders<<endl;
}


// funcțiile aferente clasei CustomerHandler
CustomerHandler::CustomerHandler(const string &file_path_in): Handler(file_path_in) {
    customer_file_path = file_path+"/ro/customers.csv";
    CustomerHandler::parse_data();
}
CustomerHandler::~CustomerHandler() {
    CustomerHandler::write_to_file();
}
Customer CustomerHandler::parse_data_element(const vector<string> &data_el){
    if(data_el.size() != 2){
        cout<<"Prea multe câmpuri pentru client. Datele au fost introduse incorect."<<endl;
        throw 1;
    }
    const string name = data_el[0];
    const int previous_orders = stoi(data_el[1]);
    return Customer(name, previous_orders);
}

void CustomerHandler::parse_data(){
    CSVInputHandler csv_handler(customer_file_path);
    vector<vector<string>> data = csv_handler.get_data();
    for(int i=0; i<data.size(); i++){
        customers.push_back(parse_data_element(data[i]));
    }
}

vector<vector<string>> CustomerHandler::get_data() {
    CSVInputHandler csv_handler(customer_file_path);
    vector<vector<string>> data_in;

    for (int i=0; i<customers.size(); i++) {
        data_in.push_back(customers[i].customer_to_data());
    }

    return data_in;
}
void CustomerHandler::write_to_file(){
    vector<vector<string>> data_in = this->get_data();

    CSVInputHandler csv_handler(customer_file_path);
    csv_handler.lines_from_data(data_in);
}

int CustomerHandler::search_customer(string const &name, bool const display) {
    for (int i=0; i<customers.size(); i++) {
        if (customers[i].get_name() == name) {
            //cout<<customers[i].get_name()<<endl;
            //cout<<name<<endl;
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

void CustomerHandler::add_customer(string const &name) {
    // în mod normal, nu ar trb să se poată ajunge la if-ul ăsta
    if (search_customer(name) != -1) {
        cout<<"Clientul există deja în baza de date!"<<endl;
        throw 11;
    }
    // aici codul e puțin sloppy: aici pun 1 deoarece funcția este apelată doar când un client își adaugă prim
    customers.push_back(Customer(name));
}

float CustomerHandler::get_discount(string const &name) {
    int customer_index = search_customer(name);
    if (customer_index==-1) {
        cout<<"Clientul \""<<name<<"\" nu a putut fi găsit!"<<endl;
        throw 11;
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

void CustomerHandler::add_order(string const &name) {
    int customer_index = search_customer(name);
    if (customer_index==-1) {
        cout<<"Clientul \""<<name<<"\" nu a putut fi găsit!"<<endl;
        throw 11;
    }

    customers[customer_index].increment_orders();
}


// funcțiile aferente clasei Order
Order::Order(int order_id_in, const string &customer_name_in, const string &order_date_in, int product_count_in, const vector<string> &products_in, const float gross_price_in, const float loyalty_discount_in){
    order_id = order_id_in;
    customer_name = customer_name_in;
    order_date = order_date_in;
    product_count = product_count_in;
    products = products_in;
    gross_price = gross_price_in;
    loyalty_discount = loyalty_discount_in;
    net_price = gross_price - loyalty_discount*gross_price;
}
Order::Order(int order_id_in, const string &customer_name_in, const string &order_date_in, int product_count_in, const vector<string> &products_in, const float gross_price_in, const float loyalty_discount_in, const float net_price_in){
    order_id = order_id_in;
    customer_name = customer_name_in;
    order_date = order_date_in;
    product_count = product_count_in;
    products = products_in;
    gross_price = gross_price_in;
    loyalty_discount = loyalty_discount_in;
    net_price = net_price_in;
}
string Order::get_date() {
    return this->order_date;
}
void Order::display_information() {
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

vector<string> Order::order_to_data() {
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
vector<string> Order::get_product_names() {
    return products;
}


// funcțiile aferente clasei OrderHandler

OrderHandler::OrderHandler(const string &file_path_in): Handler(file_path_in) {
    order_file_path = file_path+"/ro/orders.csv";
    OrderHandler::parse_data();
}
OrderHandler::~OrderHandler(){
    OrderHandler::write_to_file();
}
Order OrderHandler::parse_data_from_element(const vector<string> &data_el){
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

void OrderHandler::parse_data() {
    CSVInputHandler csv_handler(order_file_path);
    vector<vector<string>> data = csv_handler.get_data();
    for(int i=0; i<data.size(); i++){
        orders.push_back(parse_data_from_element(data[i]));
    }
}

vector<vector<string>> OrderHandler::get_data() {
    CSVInputHandler csv_handler(order_file_path);
    vector<vector<string>> data_in;

    for (int i=0; i<orders.size(); i++) {
        data_in.push_back(orders[i].order_to_data());
    }

    return data_in;
}
void OrderHandler::write_to_file(){
    vector<vector<string>> data_in = this->get_data();

    CSVInputHandler csv_handler(order_file_path);
    csv_handler.lines_from_data(data_in);
}


void OrderHandler::create_order() {
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
        throw 3;
    }
    if (is_future_date(order_date)) {
        cout<<"Comanda introdusă este din viitor!"<<endl;
        throw 3;
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
    try{
        for (int i=0; i<product_count; i++) {
            gross_price+=product_handler.purchase_product(products[i]);
        }
    }catch(int e){
        throw 3; // 3 care corespunde unei erori la crearea comenzii
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
vector<Order> OrderHandler::get_orders_by_date(string date) {
    vector<Order> new_orders;
    for (int i=0; i<orders.size(); i++) {
        if (orders[i].get_date() == date) {
            new_orders.push_back(orders[i]);
        }
    }
    cout<<new_orders.size();
    return new_orders;
}
