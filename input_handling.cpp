//
// Created by abratosin on 12/29/24.
//

#include "input_handling.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;
CSVInputHandler::CSVInputHandler(const string &file_path_in){
    this->file_path = file_path_in;

    read_lines();
}

CSVInputHandler::~CSVInputHandler(){
    write_lines();
}
void CSVInputHandler::read_lines(){
    ifstream file(file_path);
    getline(file, csv_header);
    vector<string> lines_temp;
    if(file.is_open()){
        string line;
        while(getline(file, line)){
            lines_temp.push_back(line);
        }
    }

    lines = lines_temp;
}

void CSVInputHandler::write_lines(){
    ofstream file;
    file.open(file_path);
    file<<csv_header<<endl;
    cout<<csv_header<<endl;
    if(file.is_open()){
        for(int i=0; i<lines.size(); i++){
            file<<lines[i]<<endl;
            cout<<lines[i]<<endl;
        }
    }
}

void CSVInputHandler::print_lines(){
    for(int i=0; i<lines.size(); i++){
        cout<<lines[i]<<endl;
    }
}

vector<string> CSVInputHandler::get_lines(){
    return lines;
}


// este mai ușor și ceva mai safe doar să comunic cu alte clase de parsing prin data, cum fiecare clasă în parte nu ar trebui să gestioneze decât transpunerea fiecărui câmp al clasei în string
// așa ar trebui să nu fiu nevoit să refolosesc cod
vector<vector<string>> CSVInputHandler::get_data(){
    vector<vector<string>> data;
    for(int i=0; i<lines.size(); i++){
        vector<string> row;
        stringstream ss(lines[i]);
        string item;
        while(getline(ss, item, ',')){
            row.push_back(item);
        }
        data.push_back(row);
    }
    return data;
}

// am nevoie de funcția asta pentru a putea scrie datele în fișier
void CSVInputHandler::lines_from_data(vector<vector<string>> data){
    vector<string> lines_temp;
    for(int i=0; i<data.size(); i++){
        lines_temp.push_back(line_from_data(data[i]));
    }
    lines = lines_temp;
}

string CSVInputHandler::line_from_data(const vector<string> &data){
    string line = "";
    for(int i=0; i<data.size(); i++){
        line += data[i];
        if(i<data.size()-1){
            line += ",";
        }
    }
    return line;
}

void CSVInputHandler::append_line(const string& line){
    lines.push_back(line);
}