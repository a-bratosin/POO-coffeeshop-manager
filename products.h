
#ifndef PRODUCTS_H
#define PRODUCTS_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "handler.h"
#include "input_handling.h"
using namespace std;

class Product{
private:
    string name;
    // pe lângă nume, un produs are și un cod unic
    string code;
    // prețurile sunt în RON
    float price;
    // costul produsului pentru cafenea
    // pentru chestii gen cafea, ciocolată caldă, etc. costul este unul mediu pentru produsul respectiv
    float cost;
    // aici împart cam toate produsele în două categorii:
    // - cele pentru care e un stoc numărabil (prăjituri, biscuiți, sucuri, etc.)
    // - cele care sunt făcut pe loc din ingreditente care nu pot fi clar cuantificate (cafea, ciocolată caldă, etc.)
    bool is_stockable;
    int stock;
public:
    Product(const string &name_in, const string &code_in, const float price_in, const float cost_in, const bool is_stockable_in, const int stock_in);
    Product(const string &name_in, const string &code_in, const float price_in, const float &cost_in, const bool is_stockable_in);
    
    string get_name();
    float get_price();
    float get_cost();

    int get_stock();
    void set_stock(int new_stock);

    bool can_be_purchased();
    bool get_is_stockable();
    void decrement_stock();

    
    vector<string> product_to_data();
    void display_information();
};





// clasă care citește și stochează datele despre produse, și care gestionează stocurile
class ProductHandler : public Handler{
private:
    string product_file_path;
    vector<Product> products;
public:

    explicit ProductHandler(const string &file_path_in);
    ~ProductHandler() override;

    Product parse_data_element(const vector<string> &data_el);
    void parse_data() override;

    vector<vector<string>> get_data() override;

    void write_to_file() override;

    // funcție de căutare după nume care retunează indexul produsului în vector
    // funcția asta poate și să afișeze produsul găsit, în funcție de variabila display
    int search_product(const string &name, const bool display=false);

    void add_product();

    void remove_product(const string &name);

    Product get_product(int index);

    // aici returnează prețul produsului, în scopul calculării totalului în clasa comenzii
    float purchase_product(const string &name);

    float search_cost(const string &name);
    void reset_stock(const string &name, int stock);
};

#endif //PRODUCTS_H