//
// Created by abratosin on 12/29/24.
//
#include "input_handling.cpp"

#include <iostream>
#include <string>

using namespace std;

// gestionarea produselor

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
class ProductHandler {
private:
    string file_path;
    vector<Product> products;
public:

    explicit ProductHandler(const string &file_path_in){
        this->file_path = file_path_in;
        parse_data();
    }
    ~ProductHandler(){
        write_to_file();
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

    void parse_data() {
        CSVInputHandler csv_handler(file_path);
        vector<vector<string>> data = csv_handler.get_data();
        for(int i=0; i<data.size(); i++){
            products.push_back(parse_data_element(data[i]));
        }
    }

    void write_to_file() {
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