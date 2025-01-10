#include <iostream>
#include <string>
#include <vector>

#include "employees.h"

// creez o funcție de template care să se ocupe de traducerea unui document CSV

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


void test() {
    cout<<"Test to_english"<<endl;
}