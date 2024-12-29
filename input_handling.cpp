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
class CSVInputHandler{
        private:
            string file_path;
            string delimiter;
            // în obiectul de input în sine păstrez liniile din fișier, nu datele
            // așa (mi se pare că) am mai mult control asupra modului în care datele sunt scrise în fișier
            vector<string> lines;

        // în principiu, clasa CSVInputHandler este singura care citește din fișier în mod direct, și stochează datele sub formă de vector al liniilor
        // apoi clasa oferă această informație restului programului fie ca atare, fie sub formă de vector de vectori de stringuri
        // deci majoritatea claselor vor primi acel vector de date, vor face procesările lor, îl vor returna obiectului de CSV, care va scrie datele în fișierul dorit
        public:

        CSVInputHandler(const string &file_path_in, const string &delimiter_in){
            this->file_path = file_path_in;
            this->delimiter = delimiter_in;

            read_lines();
        }
        ~CSVInputHandler(){
            write_lines();
        }
        void read_lines(){
            ifstream file(file_path);
            vector<string> lines_temp;
            if(file.is_open()){
                string line;
                while(getline(file, line)){
                    lines_temp.push_back(line);
                }
            }

            lines = lines_temp;
        }

        void write_lines(){
            ofstream file;
            file.open(file_path);
            if(file.is_open()){
                for(int i=0; i<lines.size(); i++){
                    file<<lines[i]<<endl;
                }
            }
        }

        void print_lines(){
            for(int i=0; i<lines.size(); i++){
                cout<<lines[i]<<endl;
            }
        }

        vector<string> get_lines(){
            return lines;
        }


        // este mai ușor și ceva mai safe doar să comunic cu alte clase de parsing prin data, cum fiecare clasă în parte nu ar trebui să gestioneze decât transpunerea fiecărui câmp al clasei în string
        // așa ar trebui să nu fiu nevoit să refolosesc cod
        vector<vector<string>> get_data(){
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

        // asta e utilă pentru a pune mai multe date deodată
        void lines_from_data(vector<vector<string>> data){
            vector<string> lines_temp;
            for(int i=0; i<data.size(); i++){
                lines_temp.push_back(line_from_data(data[i]));
            }
            lines = lines_temp;
        }

        string line_from_data(const vector<string> &data){
            string line = "";
            for(int i=0; i<data.size(); i++){
                line += data[i];
                if(i<data.size()-1){
                    line += ",";
                }
            }
            return line;
        }

        void append_line(const string& line){
            lines.push_back(line);
        }

};
