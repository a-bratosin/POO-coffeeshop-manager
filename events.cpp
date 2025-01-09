

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include "events.h"
#include "input_handling.h"
#include "products.h"
#include "handler.h"
#include "check_date.h"
using namespace std;

// funcțiile aferente clasei Event
Event::Event(const string &event_name_in, const string &event_date_in, const vector<pair<string,int>> &products_in, const float marketing_costs_in, const float misc_costs_in, const float ticket_price_in, const int ticket_count_in){
    event_name = event_name_in;
    event_date = event_date_in;
    products = products_in;
    marketing_costs = marketing_costs_in;
    misc_costs = misc_costs_in;
    ticket_price = ticket_price_in;
    ticket_count = ticket_count_in;
}

void Event::display_information(){
    cout<<"Nume eveniment: "<<event_name<<endl;
    cout<<"Data evenimentului: "<<event_date<<endl;
    cout<<"Produsele oferite: "<<endl;
    for (int i=0; i<products.size(); i++) {
        cout<<products[i].first<<": "<<products[i].second<<endl;
    }
    std::cout<<"Costuri de marketing: "<<marketing_costs<<endl;
    cout<<"Costuri diverse (postere, catering, etc.): "<<misc_costs<<endl;
    cout<<"Prețul unui bilet: "<<ticket_price<<endl;
    cout<<"Numărul de bilete vândute: "<<ticket_count<<endl;
}

vector<string> Event::event_to_data() {
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

vector<pair<string,int>> Event::get_products() {
    return products;
}
float Event::get_ticket_price() {
    return ticket_price;
}
int Event::get_ticket_count() {
    return ticket_count;
}
float Event::get_misc_costs() {
    return misc_costs;
}
float Event::get_marketing_costs() {
    return marketing_costs;
}


// funcțiile aferente clasei EventHandler


EventHandler::EventHandler(const string &file_path_in): Handler(file_path_in) {
    event_file_path = file_path+"/events.csv";
    EventHandler::parse_data();
}
EventHandler::~EventHandler(){
    EventHandler::write_to_file();
}


Event EventHandler::parse_data_element(const vector<string> &data_el){
    if(data_el.size() != 7){
        std::cout<<"Prea multe câmpuri pentru eveniment. Datele au fost introduse incorect."<<std::endl;
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

void EventHandler::parse_data() {
    CSVInputHandler csv_handler(event_file_path);
    vector<vector<string>> data = csv_handler.get_data();
    for(int i=0; i<data.size(); i++){
        events.push_back(parse_data_element(data[i]));
    }
}

vector<vector<string>> EventHandler::get_data() {
    CSVInputHandler csv_handler(event_file_path);
    vector<vector<string>> data_in;

    for (int i=0; i<events.size(); i++) {
        data_in.push_back(events[i].event_to_data());
    }

    return data_in;
}

void EventHandler::write_to_file() {

    vector<vector<string>> data_in = this->get_data();

    CSVInputHandler csv_handler(event_file_path);
    csv_handler.lines_from_data(data_in);
}

void EventHandler::create_event() {
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
    std::cin>>product_count;
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

Event EventHandler::search_event(string const &name, bool const display) {
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

vector<Event> EventHandler::get_events_by_date(string date){
    vector<Event> events_by_date;
    for (int i=0; i<events.size(); i++) {
        if (events[i].get_products()[0].first == date) {
            events_by_date.push_back(events[i]);
        }
    }
    return events_by_date;
}
