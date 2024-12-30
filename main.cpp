//
// Created by abratosin on 12/29/24.
//
#include "input_handling.cpp"

#include <iostream>
#include <string>

using namespace std;

// clasă abstractă care îmi definește interacțiunile cu CSV-urile
class Handler {
private:
    string file_path;
public:
    virtual ~Handler() = default;

    Handler(const string& file_path_in){
        file_path = file_path_in;
    }
    virtual void parse_data() = 0;
    virtual void write_to_file() = 0;

};

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
    Product(const string &name_in, const string &code_in, const float price_in, const float cost_in, const bool is_stockable_in, const int stock_in){
        name = name_in;
        code = code_in;
        price = price_in;
        cost = cost_in;
        is_stockable = is_stockable_in;
        cout<<cost<<"  "<<cost_in<<endl;

        if ((is_stockable_in == false) && (stock_in != -1)) {
            cout<<"Ați introdus un stoc pentru produs, cu toate că produsul nu are stoc definit. Acesta va fi ignorat."<<endl;
            throw 3; // aici dau throw ptc nu ar trebui să se ajungă la codul ăsta în mod normal1
        }
        else {
            stock = stock_in;
        }
    }
    Product(const string &name_in, const string &code_in, const float price_in, const float &cost_in, const bool is_stockable_in){
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

    string get_name() {
        return this->name;
    }
    float get_price() {
        return this->price;
    }
    float get_cost() {
        return this->cost;
    }
    int get_stock() {
        return this->stock;
    }

    void set_stock(int new_stock) {
        this->stock = new_stock;
    }

    bool can_be_purchased() {
        if (this->is_stockable == false) {
            return true;
        }
        if (this->stock > 0) {
            return true;
        }
        return false;
    }

    bool get_is_stockable() {
        return this->is_stockable;
    }

    // teoretic, la funcția asta n-ar trebui să poți ajunge dacă produsul respectiv nu e în stoc
    void decrement_stock() {
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
    vector<string> product_to_data() {
        vector<string> data_el;
        data_el.push_back(name);
        data_el.push_back(code);
        data_el.push_back(to_string(price));
        data_el.push_back(to_string(cost));
        data_el.push_back(to_string(is_stockable));
        data_el.push_back(to_string(stock));
        return data_el;
    }

    void display_information() {
        cout<<"Nume: "<<name<<endl;
        cout<<"Cod: "<<code<<endl;
        cout<<"Prețul produsului: "<<price<<endl;
        cout<<"Costul pentru cafenea: "<<cost<<endl;
        if (is_stockable) {
            cout<<"Stoc: "<<stock<<endl;
        }
    }
};
// clasă care citește și stochează datele despre produse, și care gestionează stocurile
class ProductHandler : public Handler{
private:
    string file_path;
    vector<Product> products;
public:

    explicit ProductHandler(const string &file_path_in): Handler(file_path_in) {
        this->file_path = file_path_in;
        ProductHandler::parse_data();
    }

    ~ProductHandler(){
        ProductHandler::write_to_file();
    }

    Product parse_data_element(const vector<string> &data_el){
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

    void parse_data() override {
        CSVInputHandler csv_handler(file_path);
        vector<vector<string>> data = csv_handler.get_data();
        for(int i=0; i<data.size(); i++){
            products.push_back(parse_data_element(data[i]));
        }
    }

    void write_to_file() override {
        CSVInputHandler csv_handler(file_path);
        vector<vector<string>> data_in;

        for (int i=0; i<products.size(); i++) {
            data_in.push_back(products[i].product_to_data());
        }

        csv_handler.lines_from_data(data_in);
    }

    // funcție de căutare după nume care retunează indexul produsului în vector
    // funcția asta poate și să afișeze produsul găsit, în funcție de variabila display
    int search_product(const string &name, const bool display=false) {
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

    void add_product() {
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
            throw 1;
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

    void remove_product(const string &name) {
        int index = search_product(name);
        if (index == -1) {
            cout<<"Produsul nu a putut fi găsit!"<<endl;
            throw 5;
        }
        products.erase(products.begin() + index);
    }

    // aici returnează prețul produsului, în scopul calculării totalului în clasa comenzii
    float purchase_product(const string &name) {
        int product_index = search_product(name);
        if (product_index==-1) {
            cout<<"Produsul \""<<name<<"\" nu a putut fi găsit!"<<endl;
            throw 6;
        }

        if (products[product_index].can_be_purchased()) {
            products[product_index].decrement_stock();
            return products[product_index].get_cost();
        }
        else {
            cout<<"Produsul nu mai este în stoc!"<<endl;
            throw 7;
        }
    }

    float search_cost(const string &name) {
        int product_index = search_product(name);
        if (product_index==-1) {
            cout<<"Produsul \""<<name<<"\" nu a putut fi găsit!"<<endl;
            throw 6;
        }
        return products[product_index].get_cost();
    }

    void reset_stock(const string &name, int stock) {
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
};


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
    string file_path;
    vector<Customer> customers;
public:
    CustomerHandler(const string &file_path_in): Handler(file_path_in) {
        this->file_path = file_path_in;
        CustomerHandler::parse_data();
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
        CSVInputHandler csv_handler(file_path);
        vector<vector<string>> data = csv_handler.get_data();
        for(int i=0; i<data.size(); i++){
            customers.push_back(parse_data_element(data[i]));
        }
    }
    void write_to_file() override {
        CSVInputHandler csv_handler(file_path);
        vector<vector<string>> data_in;

        for (int i=0; i<customers.size(); i++) {
            data_in.push_back(customers[i].customer_to_data());
        }

        csv_handler.lines_from_data(data_in);
    }

    int search_customer(string const &name, bool const display=false) {
        for (int i=0; i<customers.size(); i++) {
            if (customers[i].get_name() == name) {
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
    vector<string> products;
    float gross_price;
    // discountul de loialitate; float între 0 și 1
    float loyalty_discount;
    // prețul după discount
    float net_price;

public:
    Order(int order_id_in, const string &customer_name_in, const string &order_date_in, const vector<string> &products_in, const float gross_price_in, const float loyalty_discount_in){
        order_id = order_id_in;
        customer_name = customer_name_in;
        order_date = order_date_in;
        products = products_in;
        gross_price = gross_price_in;
        loyalty_discount = loyalty_discount_in;
        net_price = gross_price - loyalty_discount*gross_price;
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

};
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

    ProductHandler product_handler(data_path+"/products.csv");
    //product_handler.add_product();
    product_handler.search_product("Sugar Cookie", true);
    product_handler.reset_stock("Sugar Cookie", 20);
    cout<<product_handler.purchase_product("Sugar Cookie")<<endl;


    return 0;
}