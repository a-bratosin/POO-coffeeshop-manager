#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "handler.h"
#include "input_handling.h"
#include "products.h"
using namespace std;


Product::Product(const string &name_in, const string &code_in, const float price_in, const float cost_in, const bool is_stockable_in, const int stock_in){
    name = name_in;
    code = code_in;
    price = price_in;
    cost = cost_in;
    is_stockable = is_stockable_in;
    //cout<<cost<<"  "<<cost_in<<endl;

    if ((is_stockable_in == false) && (stock_in != -1)) {
        cout<<"Ați introdus un stoc pentru produs, cu toate că produsul nu are stoc definit. Acesta va fi ignorat."<<endl;
        throw 3; // aici dau throw ptc nu ar trebui să se ajungă la codul ăsta în mod normal1
    }
    else {
        stock = stock_in;
    }
}

Product::Product(const string &name_in, const string &code_in, const float price_in, const float &cost_in, const bool is_stockable_in){
    name = name_in;
    code = code_in;
    price = price_in;
    cost = cost_in;
    // dacă utilizatorul introduce un produs care poate avea stoc, dar îl omite, acesta este inițializat cu 0
    is_stockable = is_stockable_in;
    if (is_stockable_in == true) {
        stock = 0;
    }
    else {
        // -1 - produsul nu are stoc
        stock = -1;
    }
}

string Product::get_name() {
    return this->name;
}
float Product::get_price() {
    return this->price;
}
float Product::get_cost() {
    return this->cost;
}
int Product::get_stock() {
    return this->stock;
}

void Product::set_stock(int new_stock) {
    this->stock = new_stock;
}

bool Product::can_be_purchased() {
    if (this->is_stockable == false) {
        return true;
    }
    if (this->stock > 0) {
        return true;
    }
    return false;
}

bool Product::get_is_stockable() {
    return this->is_stockable;
}

// teoretic, la funcția asta n-ar trebui să poți ajunge dacă produsul respectiv nu e în stoc
void Product::decrement_stock() {
    if (this->is_stockable == false) {
        cout<<"Produsul nu are stoc"<<endl;
        return;
    }
    if (this->stock==0) {
        cout<<"Eroare! Produsul nu mai este în stoc!"<<endl;
    }
    this->stock--;
}

// converts the element
vector<string> Product::product_to_data() {
    vector<string> data_el;
    data_el.push_back(name);
    data_el.push_back(code);
    data_el.push_back(to_string(price));
    data_el.push_back(to_string(cost));
    data_el.push_back(to_string(is_stockable));
    data_el.push_back(to_string(stock));
    return data_el;
}

void Product::display_information() {
    cout<<"Nume: "<<name<<endl;
    cout<<"Cod: "<<code<<endl;
    cout<<"Prețul produsului: "<<price<<endl;
    cout<<"Costul pentru cafenea: "<<cost<<endl;
    if (is_stockable) {
        cout<<"Stoc: "<<stock<<endl;
    }
}



// funcțiile aferente clasei ProductHandler

ProductHandler::ProductHandler(const string &file_path_in): Handler(file_path_in) {
    product_file_path = file_path+"/ro/products.csv";
    ProductHandler::parse_data();
}

ProductHandler::~ProductHandler(){
    ProductHandler::write_to_file();
}

Product ProductHandler::parse_data_element(const vector<string> &data_el){
    if(data_el.size() != 6){
        cout<<"Prea multe câmpuri pentru produs. Datele au fost introduse incorect."<<endl;
        throw 1;
    }
    const string name = data_el[0];
    const string code = data_el[1];
    const float price = stof(data_el[2]);
    const float cost = stof(data_el[3]);
    const bool is_stockable = stoi(data_el[4]);
    const int stock = stoi(data_el[5]);
    return Product(name, code, price, cost, is_stockable, stock);
}

void ProductHandler::parse_data() {
    CSVInputHandler csv_handler(product_file_path);
    vector<vector<string>> data = csv_handler.get_data();
    for(int i=0; i<data.size(); i++){
        products.push_back(parse_data_element(data[i]));
    }
}

vector<vector<string>> ProductHandler::get_data() {
    CSVInputHandler csv_handler(product_file_path);
    vector<vector<string>> data_in;

    for (int i=0; i<products.size(); i++) {
        data_in.push_back(products[i].product_to_data());
    }

    return data_in;
}

void ProductHandler::write_to_file() {

    vector<vector<string>> data_in = get_data();

    CSVInputHandler csv_handler(product_file_path);
    csv_handler.lines_from_data(data_in);
}

// funcție de căutare după nume care retunează indexul produsului în vector
// funcția asta poate și să afișeze produsul găsit, în funcție de variabila display
int ProductHandler::search_product(const string &name, const bool display) {
    //display = false;
    for (int i=0; i<products.size(); i++) {
        if (products[i].get_name() == name) {
            if (display) {
                cout<<"Produsul a fost găsit!"<<endl;
                products[i].display_information();
            }
            return i;
        }
    }
    if (display) {
        cout<<"Eroare! Produsul nu a putut fi găsit!"<<endl;
    }
    return -1;
}

void ProductHandler::add_product() {
    string name;
    string code;
    float price;
    float cost;
    bool is_stockable;
    int stock;
    cout<<"Introduceți numele produsului: ";
    getline(cin, name);
    if (search_product(name) != -1) {
        cout<<"Produsul există deja în baza de date!"<<endl;
        throw 2;
    }
    cout<<"Introduceți codul produsului: ";
    cin>>code;
    cout<<"Introduceți prețul produsului: ";
    cin>>price;
    cout<<"Introduceți costul produsului pentru cafenea: ";
    cin>>cost;
    cout<<"Produsul are stoc? (1 - da, 0 - nu): ";
    cin>>is_stockable;
    if (is_stockable) {
        cout<<"Introduceți stocul produsului: ";
        cin>>stock;
        products.push_back(Product(name,code,price, cost, is_stockable, stock));
        return;
    }
    products.push_back(Product(name, code, price, cost, is_stockable));
}


void ProductHandler::remove_product(const string &name) {
    int index = search_product(name);
    if (index == -1) {
        cout<<"Produsul nu a putut fi găsit!"<<endl;
        throw 5;
    }
    products.erase(products.begin() + index);

    cout<<"Produs șters!"<<endl;
}

Product ProductHandler::get_product(int index) {
    return this->products[index];
}
// aici returnează prețul produsului, în scopul calculării totalului în clasa comenzii
float ProductHandler::purchase_product(const string &name) {
    int product_index = search_product(name);
    if (product_index==-1) {
        cout<<"Produsul \""<<name<<"\" nu a putut fi găsit!"<<endl;
        throw 6;
    }

    if (products[product_index].can_be_purchased()) {
        products[product_index].decrement_stock();
        return products[product_index].get_price();
    }
    else {
        cout<<"Produsul nu mai este în stoc!"<<endl;
        throw 7;
    }
}

float ProductHandler::search_cost(const string &name) {
    int product_index = search_product(name);
    if (product_index==-1) {
        cout<<"Produsul \""<<name<<"\" nu a putut fi găsit!"<<endl;
        throw 6;
    }
    return products[product_index].get_price();
}

void ProductHandler::reset_stock(const string &name, int stock) {
    int product_index = search_product(name);
    if (product_index==-1) {
        cout<<"Produsul \""<<name<<"\" nu a putut fi găsit!"<<endl;
        throw 6;
    }

    if (products[product_index].get_is_stockable()) {
        products[product_index].set_stock(stock);
    }
    else {
        cout<<"Produsul nu are stoc!"<<endl;
        throw 8;
    }
}