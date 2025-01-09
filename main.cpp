//
// Created by abratosin on 12/29/24.
//
#include "input_handling.h"
#include "handler.h"
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <fstream>
#include <random>
#include <utility>
#include <array>

using namespace std;


// funcție pentru verificarea validității unei date introduse sub formă de string, cu formatul dd/mm/yyyy
/*
bool is_valid_date(string date) {

    istringstream in(move(date));
    chrono::year_month_day ymd{};
    string test;
    in>>test;
    cout<<"test: "<<test<<endl;

    in = istringstream(date);
    // converteste data din string dd/mm/yyyy într-un obiect de tip year_month_day
    in >> chrono::parse("%d/%m/%Y", ymd);

    // verific mai întâi dacă data a fost convertită corect
    if (in.fail()) {
        cout<<"Dată invalidă!"<<endl;
        return false;
    }
    cout<<ymd.ok()<<endl;
    cout<<ymd.year()<<" "<<ymd.month()<<" "<<ymd.day()<<endl;
    return ymd.ok();

}
*/

bool
is_valid_date(std::string date)
{
    std::istringstream in{std::move(date)};
    std::chrono::year_month_day ymd{};
    in >> std::chrono::parse("%d/%m/%Y", ymd);
    return !in.fail();
}


bool is_future_date(const string &date) {

    if (!is_valid_date(date)) {
        cout<<"Dată invalidă!"<<endl;
        throw 1;
    }

    istringstream in(date);
    chrono::year_month_day ymd;

    in >> chrono::parse("%d/%m/%Y", ymd);


    // obțin data curentă
    auto current_date = chrono::year_month_day{chrono::floor<chrono::days>(chrono::system_clock::now())};

    // compar data introdusă cu data curentă
    // aici e un operator overloaded, care compară două obiecte de tip year_month_day
    if (ymd > current_date) {
        return true;
    }
    return false;
}


//              CLASA DE ANGAJAȚI
// pentru implementarea funcționalității angajaților, implementez aici un design pattern-ul de Factory
// astfel, generez clasa abstractă Employee, care este moștenită de clasele Barista, Manager, și Waiter
// creez și o clasă EmployeeCreator, care la rândul ei este moștenită de către BaristaCreator, ManagerCreator, și WaiterCreator
class Employee{
private:
    // numele complet al angajatului
    string name;
    // salariul brut al angajatului/oră
    float pay = 0.0;
    // vector de orele de lucru ale angajatului, pentru fiecare zi a săptămânii
    // perechea reprezintă intervalul de ore de lucru
    array<pair<int, int>,7> hours;



public:
    const string get_name() {
        return this->name;
    }
    const float get_pay() {
        return this->pay;
    }
    array<pair<int,int>,7> get_hours() {
        return hours;
    }
    Employee() {
        name = "Necunoscut";
        pay = 0.0;
        for (int i=0; i<7; i++) {
            hours[i] = make_pair(0, 0);
        }
    }
    Employee(const string &name_in, float pay_in, array<pair<int, int>,7> hours_in){
        name = name_in;
        pay = pay_in;
        for (int i=0; i<7; i++) {
            hours[i] = hours_in[i];
        }
    }

    Employee( const Employee & other) {
        this->name = other.name;
        this->pay = other.pay;
        for (int i=0; i<7; i++) {
            this->hours[i] = other.hours[i];
        }

    }
    virtual ~Employee() = default;

    void display_information() {
        cout<<"Nume: "<<name<<endl;
        cout<<"Salariu brut: "<<pay<<endl;
        cout<<"Orele de lucru:"<<endl;
        for (int i=0; i<7; i++) {
            cout<<"Ziua "<<i+1<<": "<<hours[i].first<<" - "<<hours[i].second<<endl;
        }
    }
    virtual void display_role() = 0;
    virtual string get_role() = 0;

    void display_full_information() {
        display_information();
        display_role();
    }

    vector<string> partial_employee_to_data() {
        vector<string> data;
        data.push_back(name);
        data.push_back(to_string(pay));
        string hours_string="";
        for (int i=0; i<7; i++) {
            hours_string.append(to_string(hours[i].first));
            hours_string.append("-");
            hours_string.append(to_string(hours[i].first));
            if (i<6) {
                hours_string.append("/");
            }
        }
        data.push_back(hours_string);
        return data;
    };
    virtual vector<string> employee_to_data() = 0;


    float get_hourly_pay() {
        return pay;
    }

    float get_weekly_pay() {
        float pay = 0.0;
        for (int i=0; i<7; i++) {
            pay += (hours[i].second - hours[i].first)*pay;
        }
        return pay;
    }

    float get_daily_pay(string date) { // weekday - 1-7
        // săptămânile încep lunea
        if (!is_valid_date(date)) {
            cout<<"Dată invalidă!"<<endl;
            throw 1;
        }
        std::istringstream in{std::move(date)};
        std::chrono::year_month_day ymd;
        in >> std::chrono::parse("%d/%m/%Y", ymd);

        chrono::weekday wd = chrono::weekday{ymd};
        int weekday = wd.iso_encoding(); // ISO encoding deoarece săptămânile încep lunea

        return (hours[weekday-1].second - hours[weekday-1].first)*pay;
    }


};

class Barista : public Employee {
private:
    string role = "barista";
public:
    Barista(const string &name_in, float pay_in, array<pair<int, int>,7> hours_in) : Employee(name_in, pay_in, hours_in) {

    }
    Barista (const Barista & other) : Employee(other) {

    }
    void display_role() override {
        cout<<"Rol: Barista"<<endl;
    }
    string get_role() {
        return "barista";
    }
    vector<string> employee_to_data() override {
        vector<string> data = partial_employee_to_data();

        data.insert(data.begin(), "Barista");

        return data;
    }
};

class Manager : public Employee {
public:
    Manager(const string &name_in, float pay_in, array<pair<int, int>,7> hours_in) : Employee(name_in, pay_in, hours_in) {

    }
    Manager(const Manager &other) : Employee(other){}
    void display_role() override {
        cout<<"Rol: Manager"<<endl;
    }

    string get_role() {
        return "manager";
    }
    vector<string> employee_to_data() override {
        vector<string> data = partial_employee_to_data();

        data.insert(data.begin(), "Manager");

        return data;
    }
};

class Waiter : public Employee {
public:
    Waiter(const string &name_in, float pay_in, array<pair<int, int>,7> hours_in) : Employee(name_in, pay_in, hours_in) {

    }
    Waiter(const Waiter &other) : Employee(other){}

    void display_role() override {
        cout<<"Rol: Ospătar"<<endl;
    }
    string get_role() {
        return "Ospatar";
    }
    vector<string> employee_to_data() override {
        vector<string> data = partial_employee_to_data();

        data.insert(data.begin(), "Ospatar");

        return data;
    }
};

class EmployeeCreator {
public:
    virtual ~EmployeeCreator() = default;

    virtual Employee* create_employee(const string &name_in, float pay_in, array<pair<int, int>,7> hours_in) = 0;

};

class BaristaCreator : public EmployeeCreator {
public:
    Employee* create_employee(const string &name_in, float pay_in, array<pair<int, int>,7> hours_in) override {
        return new Barista(name_in, pay_in, hours_in);
    }

    Employee* create_employee(Employee& employee) {
        string name = employee.get_name();
        const Barista temp = Barista(employee.get_name(),employee.get_pay(), employee.get_hours());
        return new Barista(temp);
    }
};

class ManagerCreator : public EmployeeCreator {
public:
    Employee* create_employee(const string &name_in, float pay_in, array<pair<int, int>,7> hours_in) override {
        return new Manager(name_in, pay_in, hours_in);
    }
    Employee* create_employee(Employee& employee) {
        string name = employee.get_name();
        const Manager temp(employee.get_name(),employee.get_pay(), employee.get_hours());
        return new Manager(temp);
    }
};

class WaiterCreator : public EmployeeCreator {
public:
    Employee* create_employee(const string &name_in, float pay_in, array<pair<int, int>,7> hours_in) override {
        return new Waiter(name_in, pay_in, hours_in);
    }
    Employee* create_employee(Employee& employee) {
        string name = employee.get_name();
        Waiter temp(employee.get_name(),employee.get_pay(), employee.get_hours());
        return new Waiter(temp);
    }
};

class EmployeeHandler : public Handler {
private:
    string employee_file_path;


public:
    vector<Employee*> employees;
    explicit EmployeeHandler(const string &file_path_in): Handler(file_path_in) {
        employee_file_path = file_path+"/employees.csv";
        EmployeeHandler::parse_data();
    }

    ~EmployeeHandler() override{
        EmployeeHandler::write_to_file();

        for (int i=0; i <employees.size(); i++) {
            delete employees[i];
        }
    }

    Employee* parse_data_element(const vector<string> &data_el){
        cout<<data_el[0]<<endl;
        if(data_el.size() != 4){
            cout<<"Prea multe câmpuri pentru angajat. Datele au fost introduse incorect."<<endl;
            throw 1;
        }
        const string name = data_el[1];
        const float pay = stof(data_el[2]);
        array<pair<int, int>,7> hours;
        string hours_string = data_el[3];
        istringstream in(hours_string);
        string hour;
        int i = 0;
        while (getline(in, hour, '/')) {
            istringstream hour_in(hour);
            string start_hour;
            string end_hour;
            getline(hour_in, start_hour, '-');
            getline(hour_in, end_hour, '-');
            hours[i] = make_pair(stoi(start_hour), stoi(end_hour));
            i++;
        }

        if (data_el[0] == "Barista") {
            BaristaCreator creator;
            return creator.create_employee(name, pay, hours);
        }
        if (data_el[0] == "Manager") {
            ManagerCreator creator;
            return creator.create_employee(name, pay, hours);
        }
        if (data_el[0] == "Waiter") {
            WaiterCreator creator;
            return creator.create_employee(name, pay, hours);
        }
        cout<<"Rol necunoscut!"<<endl;
        throw 2;
    }

    void parse_data() override {
        CSVInputHandler csv_handler(employee_file_path);
        vector<vector<string>> data = csv_handler.get_data();
        for(int i=0; i<data.size(); i++){
            employees.push_back(parse_data_element(data[i]));
        }
    }

    vector<vector<string>> get_data() override {
        CSVInputHandler csv_handler(employee_file_path);
        vector<vector<string>> data_in;

        for (int i=0; i<employees.size(); i++) {
            data_in.push_back(employees[i]->employee_to_data());
        }

        return data_in;
    }
    void write_to_file() override {
        vector<vector<string>> data_in = get_data();

        CSVInputHandler csv_handler(employee_file_path);
        csv_handler.lines_from_data(data_in);
    }

    void add_employee() {
        cout<<"Ce categorie de angajat doriți să adăugați? (Barista, Manager, Waiter): "<<endl;
        cout<<"1) Barista"<<endl;
        cout<<"2) Manager"<<endl;
        cout<<"3) Ospătar"<<endl;
        int choice;
        cin>>choice;
        cin.ignore();

        cout<<"Introduceți numele angajatului: "<<endl;
        string name;
        getline(cin, name);
        cout<<"Introduceți salariul brut pe oră al angajatului: "<<endl;
        float pay;
        cin>>pay;
        cin.ignore();
        array<pair<int, int>,7> hours;
        cout<<"Introduceți orele de lucru ale angajatului, sub forma de interval de ore, pentru fiecare zi a săptămânii: "<<endl;
        for (int i=0; i<7; i++) {
            cout<<"Ziua "<<i+1<<": ";
            int start_hour;
            int end_hour;

            cin>>start_hour;
            cin>>end_hour;

            if (start_hour > end_hour) {
                cout<<"Interval orar invalid!"<<endl;
                throw 2;
            }

            hours[i] = make_pair(start_hour, end_hour);
        }
        BaristaCreator barista_creator;
        ManagerCreator manager_creator;
        WaiterCreator waiter_creator;

        switch (choice) {
            case 1:
                employees.push_back(barista_creator.create_employee(name, pay, hours));
                break;
            case 2:
                employees.push_back(manager_creator.create_employee(name, pay, hours));
                break;
            case 3:
                employees.push_back(waiter_creator.create_employee(name, pay, hours));
                break;
            default:
                //cout<<"Selectați o opțiune validă!"<<endl;
                throw 3;
        }



    }
    // aici returnează o copie a vectorului
    // are memory leak
    /*
    vector<Employee *> get_employees() {
        vector<Employee*> new_vector;

        BaristaCreator barista_creator;
        ManagerCreator manager_creator;
        WaiterCreator waiter_creator;

        for (int i=0; i<this->employees.size(); i++) {
            if (this->employees[i]->get_role()=="barista") {
                // TODO: fix memory leak
                new_vector.push_back(barista_creator.create_employee(this->employees[i]));
            }   else if (this->employees[i]->get_role()=="manager") {
                new_vector.push_back(manager_creator.create_employee(*this->employees[i]));
            }   else if (this->employees[i]->get_role()=="waiter") {
                new_vector.push_back(waiter_creator.create_employee(*this->employees[i]));
            }

        }
        return new_vector;
    }
    */

    Employee *search_employee(string name, bool display) {
        for (int i=0; i<employees.size(); i++) {
            if (employees[i]->get_name() == name) {
                if (display) {
                    cout<<"Angajatul a fost găsit!"<<endl;
                    employees[i]->display_full_information();
                }
                return employees[i];
            }
        }
        if (display) {
            cout<<"Angajatul nu a putut fi găsit!"<<endl;
        }
        throw 1;
    }

    float get_weekly_pay(string name) {
        Employee *employee = search_employee(name, false);
        return employee->get_weekly_pay();
    }

    float get_salary(string name, unsigned int hours_worked, bool display=false) {
        Employee *employee = search_employee(name, false);
        float net_salary = floor(employee->get_hourly_pay()*hours_worked*0.45);
        if (display) {
            cout<<"Pentru "<<hours_worked<<"ore lucrate, angajatul are un salariu brut de "<<floor(employee->get_hourly_pay()*hours_worked)<<" RON."<<endl;
            cout<<"Din aceștia, se extrage: "<<endl;
            cout<<"\tAsigurări sociale (CAS) 25%: "<<floor(employee->get_hourly_pay()*hours_worked*0.25)<<" RON"<<endl;
            cout<<"\tAsigurări sociale de sănătate (CASS) 10%: "<<floor(employee->get_hourly_pay()*hours_worked*0.1)<<" RON"<<endl;
            cout<<"\tImpozit pe venit 10%:"<<floor(employee->get_hourly_pay()*hours_worked*0.1)<<endl;
            cout<<"Salariu net: "<<net_salary<<" RON"<<endl;
        }

        return net_salary;

    }
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
        //cout<<cost<<"  "<<cost_in<<endl;

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
    string product_file_path;
    vector<Product> products;
public:

    explicit ProductHandler(const string &file_path_in): Handler(file_path_in) {
        product_file_path = file_path+"/products.csv";
        ProductHandler::parse_data();
    }

    ~ProductHandler() override{
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
        CSVInputHandler csv_handler(product_file_path);
        vector<vector<string>> data = csv_handler.get_data();
        for(int i=0; i<data.size(); i++){
            products.push_back(parse_data_element(data[i]));
        }
    }

    vector<vector<string>> get_data() override {
        CSVInputHandler csv_handler(product_file_path);
        vector<vector<string>> data_in;

        for (int i=0; i<products.size(); i++) {
            data_in.push_back(products[i].product_to_data());
        }

        return data_in;
    }

    void write_to_file() override {

        vector<vector<string>> data_in = get_data();

        CSVInputHandler csv_handler(product_file_path);
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

    Product get_product(int index) {
        return this->products[index];
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
            return products[product_index].get_price();
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
        return products[product_index].get_price();
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