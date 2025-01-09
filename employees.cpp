#include "employees.h" 
#include "input_handling.h"
#include "handler.h"
#include "check_date.h"
#include <iostream>
#include <string>
#include <utility>
#include <array>
#include <vector>
#include <cmath>
#include <chrono>


// definiția funcțiilor aferente clasei Employee

const string Employee::get_name() {
        return this->name;
}

const float Employee::get_pay() {
    return this->pay;
}

array<pair<int,int>,7> Employee::get_hours() {
    return hours;
}

Employee::Employee() {
    name = "Necunoscut";
    pay = 0.0;
    for (int i=0; i<7; i++) {
        hours[i] = make_pair(0, 0);
    }
}

Employee::Employee(const string &name_in, float pay_in, array<pair<int, int>,7> hours_in){
    name = name_in;
    pay = pay_in;
    for (int i=0; i<7; i++) {
        hours[i] = hours_in[i];
    }
}

Employee::Employee( const Employee & other) {
    this->name = other.name;
    this->pay = other.pay;
    for (int i=0; i<7; i++) {
        this->hours[i] = other.hours[i];
    }

}

void Employee::display_information() {
    cout<<"Nume: "<<name<<endl;
    cout<<"Salariu brut: "<<pay<<endl;
    cout<<"Orele de lucru:"<<endl;
    for (int i=0; i<7; i++) {
        cout<<"Ziua "<<i+1<<": "<<hours[i].first<<" - "<<hours[i].second<<endl;
    }
}

void Employee::display_full_information() {
    display_information();
    display_role();
}

vector<string> Employee::partial_employee_to_data() {
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
}

float Employee::get_hourly_pay() {
    return pay;
}

float Employee::get_weekly_pay() {
    float pay = 0.0;
    for (int i=0; i<7; i++) {
        pay += (hours[i].second - hours[i].first)*pay;
    }
    return pay;
}

float Employee::get_daily_pay(string date) { // weekday - 1-7
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


// definiția funcțiilor aferente clasei Barista
void Barista::display_role() {
    cout<<"Rol: Barista"<<endl;
}

string Barista::get_role() {
    return "barista";
}

vector<string> Barista::employee_to_data() {
    vector<string> data = partial_employee_to_data();

    data.insert(data.begin(), "Barista");

    return data;
}


// funcțiile clasei Manager

void Manager::display_role() {
    cout<<"Rol: Manager"<<endl;
}

string Manager::get_role() {
    return "manager";
}

vector<string> Manager::employee_to_data() {
    vector<string> data = partial_employee_to_data();

    data.insert(data.begin(), "Manager");

    return data;
}


// funcțiile clasei Waiter

void Waiter::display_role()  {
    cout<<"Rol: Ospătar"<<endl;
}

string Waiter::get_role() {
    return "Ospatar";
}
vector<string> Waiter::employee_to_data() {
    vector<string> data = partial_employee_to_data();

    data.insert(data.begin(), "Ospatar");

    return data;
}


// funcțiile clasei EmployeeHandler

Employee* EmployeeHandler::parse_data_element(const vector<string> &data_el){
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

void EmployeeHandler::parse_data() {
    CSVInputHandler csv_handler(employee_file_path);
    vector<vector<string>> data = csv_handler.get_data();
    for(int i=0; i<data.size(); i++){
        employees.push_back(parse_data_element(data[i]));
    }
}

vector<vector<string>> EmployeeHandler::get_data() {
    CSVInputHandler csv_handler(employee_file_path);
    vector<vector<string>> data_in;

    for (int i=0; i<employees.size(); i++) {
        data_in.push_back(employees[i]->employee_to_data());
    }

    return data_in;
}
void EmployeeHandler::write_to_file() {
    vector<vector<string>> data_in = get_data();

    CSVInputHandler csv_handler(employee_file_path);
    csv_handler.lines_from_data(data_in);
}

void EmployeeHandler::add_employee() {
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
vector<Employee *> EmployeeHandler::get_employees() {
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

Employee* EmployeeHandler::search_employee(string name, bool display) {
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

float EmployeeHandler::get_weekly_pay(string name) {
    Employee *employee = search_employee(name, false);
    return employee->get_weekly_pay();
}

float EmployeeHandler::get_salary(string name, unsigned int hours_worked, bool display) {
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