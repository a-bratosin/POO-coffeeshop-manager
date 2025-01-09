//
// Created by abratosin on 12/29/24.
//

#ifndef INPUT_HANDLING_H
#define INPUT_HANDLING_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class CSVInputHandler{
        private:
            string file_path;
            // în obiectul de input în sine păstrez liniile din fișier, nu datele
            // așa (mi se pare că) am mai mult control asupra modului în care datele sunt scrise în fișier
            vector<string> lines;
            //header-ul fișierului CSV care definește câmpurile
            string csv_header;

        // în principiu, clasa CSVInputHandler este singura care citește din fișier în mod direct, și stochează datele sub formă de vector al liniilor
        // apoi clasa oferă această informație restului programului fie ca atare, fie sub formă de vector de vectori de stringuri
        // deci majoritatea claselor vor primi acel vector de date, vor face procesările lor, îl vor returna obiectului de CSV, care va scrie datele în fișierul dorit
        public:

        CSVInputHandler(const string &file_path_in);
        ~CSVInputHandler();

        void read_lines();

        void write_lines();

        void print_lines();

        vector<string> get_lines();



        // este mai ușor și ceva mai safe doar să comunic cu alte clase de parsing prin data, cum fiecare clasă în parte nu ar trebui să gestioneze decât transpunerea fiecărui câmp al clasei în string
        // așa ar trebui să nu fiu nevoit să refolosesc cod
        vector<vector<string>> get_data();

        // am nevoie de funcția asta pentru a putea scrie datele în fișier
        void lines_from_data(vector<vector<string>> data);

        string line_from_data(const vector<string> &data);

        void append_line(const string& line);

};

#endif //INPUT_HANDLING_H
