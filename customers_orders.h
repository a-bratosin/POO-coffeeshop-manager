
#ifndef CUSTOMERS_ORDERS_H
#define CUSTOMERS_ORDERS_H

#include <string>
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include "input_handling.h"
#include "handler.h"
#include "products.h"
#include "check_date.h"

using namespace std;
//              CLIENȚI ȘI GESTIONAREA COMENZILOR
class Customer {
private:
    string name;
    // numărul de comenzi date de client, pentru a putea aplica reduceri
    int previous_orders;

public:
    explicit Customer(const string &name_in);
    Customer(const string &name_in, const int previous_orders_in);


    void set_previous_orders(const int previous_orders_in);

    string get_name();
    int get_previous_orders();
    void increment_orders();

    vector<string> customer_to_data();
    void display_information();
};


class CustomerHandler: public Handler {
private:
    string customer_file_path;
    vector<Customer> customers;
public:
    CustomerHandler(const string &file_path_in);
    ~CustomerHandler() override;

    Customer parse_data_element(const vector<string> &data_el);
    void parse_data() override;

    vector<vector<string>> get_data() override ;
    void write_to_file() override;
    int search_customer(string const &name, bool const display=false);

    void add_customer(string const &name);

    float get_discount(string const &name);
    void add_order(string const &name);
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
    Order(int order_id_in, const string &customer_name_in, const string &order_date_in, int product_count_in, const vector<string> &products_in, const float gross_price_in, const float loyalty_discount_in);
    Order(int order_id_in, const string &customer_name_in, const string &order_date_in, int product_count_in, const vector<string> &products_in, const float gross_price_in, const float loyalty_discount_in, const float net_price_in);

    string get_date();
    void display_information();

    vector<string> order_to_data();
    vector<string> get_product_names();

};

class OrderHandler: public Handler {
private:
    string order_file_path;
    vector<Order> orders;
public:
    OrderHandler(const string &file_path_in);
    ~OrderHandler() override;
    Order parse_data_from_element(const vector<string> &data_el);

    void parse_data() override;

    vector<vector<string>> get_data() override;
    void write_to_file() override;


    void create_order();
    // funcție care returnează toate comenzile făcute într-o anumită zi
    vector<Order> get_orders_by_date(string date);
};

#endif //CUSTOMERS_ORDERS_H