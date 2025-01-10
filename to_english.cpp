#include <iostream>
#include <string>
#include <vector>

#include "employees.h"
#include "customers_orders.h"
#include "events.h"
#include "financial.h"
#include "products.h"

// creez o funcție de template care să se ocupe de traducerea unui document CSV
// specializez funcția asta pentru fiecare tip de Handler pe care îl am în codebase
template <typename T>
void convert_to_english(T &handler) {

}

// pentru toate CSV-urile, va trebui să schimbăm header-ul fișierului
// În rest, sunt mai multe tipuri de fișiere care includ cuvinte care pot fi traduse

// pentru angajați, singurele cuvinte sunt ”Barista”, ”Manager”, și ”Ospătar”
// dintre care doar ”Ospătar” trebuie tradus
template<>
void convert_to_english(EmployeeHandler& handler) {
    string header = "Position,Name,Pay,Shifts";

    vector<vector<string>> data = handler.get_data();

    
    for (int i = 0; i < data.size(); i++) {
        for(int j = 0; j < data[i].size(); j++) {
            if (data[i][j] == "Ospatar") {
                data[i][j] = "Waiter";
            }
        }
    }
    
    string file_folder = handler.get_file_path();

    string file_path = file_folder + "/en/employees.csv";
    cout<<"File path: "<<file_path<<endl;
    
    
    CSVInputHandler csv_handler(file_path);
    csv_handler.set_header(header);
    csv_handler.lines_from_data(data);
    csv_handler.write_lines();
    
}


// la Customers nu e vreo altă schimbare în afară de header
template<>
void convert_to_english(CustomerHandler& handler) {
    string header = "Name,Order count";

    vector<vector<string>> data = handler.get_data();
    
    string file_folder = handler.get_file_path();

    string file_path = file_folder + "/en/customers.csv";
    cout<<"File path: "<<file_path<<endl;
    
    
    CSVInputHandler csv_handler(file_path);
    csv_handler.set_header(header);
    csv_handler.lines_from_data(data);
    csv_handler.write_lines();
}

template<>
void convert_to_english(EventHandler& handler) {
    string header = "Name,Date,Products,Marketing costs,Misc costs,Ticket price,Ticket count";

    vector<vector<string>> data = handler.get_data();
    
    string file_folder = handler.get_file_path();

    string file_path = file_folder + "/en/events.csv";
    cout<<"File path: "<<file_path<<endl;
    
    
    CSVInputHandler csv_handler(file_path);
    csv_handler.set_header(header);
    csv_handler.lines_from_data(data);
    csv_handler.write_lines();
}

template<>
void convert_to_english(ProductHandler& handler) {
    string header = "Name,Code,Price,Cost,Stockable,Stock";

    vector<vector<string>> data = handler.get_data();
    
    string file_folder = handler.get_file_path();

    string file_path = file_folder + "/en/products.csv";
    cout<<"File path: "<<file_path<<endl;
    
    
    CSVInputHandler csv_handler(file_path);
    csv_handler.set_header(header);
    csv_handler.lines_from_data(data);
    csv_handler.write_lines();
}

template<>
void convert_to_english(OrderHandler& handler) {
    string header = "ID,Name,Date,Number of products,Products,Gross price,Loyalty discount,Net price";

    vector<vector<string>> data = handler.get_data();
    
    string file_folder = handler.get_file_path();

    string file_path = file_folder + "/en/orders.csv";
    cout<<"File path: "<<file_path<<endl;
    
    
    CSVInputHandler csv_handler(file_path);
    csv_handler.set_header(header);
    csv_handler.lines_from_data(data);
    csv_handler.write_lines();
}

template<>
void convert_to_english(FinancialReportHandler& handler) {
    string header = "Date,Wages,Product costs,Product revenues,Event revenues,Event costs,Total expenses,Total Revenues,Net Revenue";

    vector<vector<string>> data = handler.get_data();
    
    string file_folder = handler.get_file_path();

    string file_path = file_folder + "/en/reports.csv";
    cout<<"File path: "<<file_path<<endl;
    
    
    CSVInputHandler csv_handler(file_path);
    csv_handler.set_header(header);
    csv_handler.lines_from_data(data);
    csv_handler.write_lines();
}

void test() {
    cout<<"Test to_english"<<endl;
}