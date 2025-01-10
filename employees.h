
#ifndef EMPLOYEES_H
#define EMPLOYEES_H


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

using namespace std;

//                            CLASA DE ANGAJAȚI

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
    const string get_name();
    const float get_pay();
    array<pair<int,int>,7> get_hours();

    Employee();
    Employee(const string &name_in, float pay_in, array<pair<int, int>,7> hours_in);
    Employee( const Employee & other);
    virtual ~Employee() = default;

    void display_information();
    virtual void display_role() = 0;
    virtual string get_role() = 0;
    void display_full_information() ;

    vector<string> partial_employee_to_data();
    virtual vector<string> employee_to_data() = 0;


    float get_hourly_pay();
    float get_weekly_pay();
    float get_daily_pay(string date);

};

class Barista : public Employee {
private:
    string role = "barista";
public:
    Barista(const string &name_in, float pay_in, array<pair<int, int>,7> hours_in) : Employee(name_in, pay_in, hours_in) {}
    Barista (const Barista & other) : Employee(other) {}
    
    void display_role() override;
    string get_role();

    vector<string> employee_to_data() override;
};


class Manager : public Employee {
public:
    Manager(const string &name_in, float pay_in, array<pair<int, int>,7> hours_in) : Employee(name_in, pay_in, hours_in) {}
    Manager(const Manager &other) : Employee(other){}
    
    void display_role() override;
    string get_role();

    vector<string> employee_to_data() override;
};

class Waiter : public Employee {
public:
    Waiter(const string &name_in, float pay_in, array<pair<int, int>,7> hours_in) : Employee(name_in, pay_in, hours_in) {}
    Waiter(const Waiter &other) : Employee(other){}

    void display_role() override;
    string get_role();

    vector<string> employee_to_data() override;
};



// clasele Creator le las în header

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
        employee_file_path = file_path+"/ro/employees.csv";
        EmployeeHandler::parse_data();
    }

    ~EmployeeHandler() override{
        cout<<"destructor"<<endl;
        EmployeeHandler::write_to_file();

        
        for (int i=0; i <employees.size(); i++) {
            delete employees[i];
        }
        
    }

    Employee* parse_data_element(const vector<string> &data_el);
    void parse_data() override;
    vector<vector<string>> get_data() override;

    void write_to_file() override;

    void add_employee();
    int search_employee(string name, bool display);
    // funcție pe care n-am mai pus-o deoarece dădea memory leak
    // vector<Employee *> EmployeeHandler::get_employees();
    void remove_employee(string name);

    float get_weekly_pay(string name);
    float get_salary(string name, unsigned int hours_worked, bool display=false);

};

#endif