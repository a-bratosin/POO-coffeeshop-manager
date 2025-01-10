//
// Created by abratosin on 12/29/24.
//
#include "input_handling.h"
#include "handler.h"
#include "check_date.h"
#include "employees.h"
#include "products.h"
#include "customers_orders.h"
#include "events.h"
#include "financial.h"
#include "to_english.h"
#include "events.h"
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <fstream>
#include <random>
#include <utility>
#include <array>

using namespace std;

// funcție care îmi verifică dacă stringul dat este număr întreg
bool has_only_digits(const string s)
{
    return s.find_first_not_of("0123456789") == string::npos;
}

int main()
{
    string data_path = "./data/";

    char selection = '0';
    while (selection < '1' || selection > '5')
    {

        cout << "Selectați localitatea pentru care doriți să faceți operații:" << endl;
        cout << "1) București" << endl;
        cout << "2) Cluj-Napoca" << endl;
        cout << "3) Timișoara" << endl;
        cout << "4) Iași" << endl;
        cout << "5) Brașov" << endl;
        cin >> selection;

        switch (selection)
        {
        case '1':
            data_path += "bucuresti";
            ;
            break;
        case '2':
            data_path += "cluj";
            break;
        case '3':
            data_path += "timisoara";
            break;
        case '4':
            data_path += "iasi";
            break;
        case '5':
            data_path += "brasov";
            break;
        default:
            cout << "Selectați o opțiune validă!\n"
                 << endl;
            break;
        }

        cin.clear();
        cin.ignore(100, '\n');
    }

    selection = '0';
    while (selection < '1' || selection > '5')
    {

        cout << "Selectați categoria de operațiune pe care doriți să o realizați:" << endl;
        cout << "1) Gestionarea angajaților" << endl;
        cout << "2) Gestionarea produselor" << endl;
        cout << "3) Gestionarea comenzilor" << endl;
        cout << "4) Gestionarea evenimentelor" << endl;
        cout << "5) Gestionarea rapoartelor financiare" << endl;
        cin >> selection;

        switch (selection)
        {
        case '1':
            // am nevoie să cuprind întregul switch case într-un try/catch pentru a putea prelua excepțiile care ar putea apărea
            try
            {
                EmployeeHandler employee_handler(data_path);

                selection = '0';
                while (selection < '1' || selection > '4')
                {

                    cout << "Selectați operația dorită:" << endl;
                    cout << "1) Adăugarea unui angajat" << endl;
                    cout << "2) Căutarea unui angajat" << endl;
                    cout << "3) Ștergerea unui angajat" << endl;
                    cout << "4) Vizualizarea salariului" << endl;
                    cin >> selection;

                    switch (selection)
                    {
                    case '1':
                        cin.clear();
                        cin.ignore(100, '\n');
                        employee_handler.add_employee();
                        break;
                    case '2':
                    {
                        cout << "Introduceți numele angajatului căutat:" << endl;
                        cin.clear();
                        cin.ignore(100, '\n');

                        string employee_name;
                        getline(cin, employee_name);
                        employee_handler.search_employee(employee_name, true);
                        break;
                    }
                    case '3':
                    {
                        cout << "Introduceți numele angajatului care trebuie șters:" << endl;
                        cin.clear();
                        cin.ignore(100, '\n');

                        string employee_name;
                        getline(cin, employee_name);
                        employee_handler.remove_employee(employee_name);
                        break;
                    }
                    case '4':
                    {
                        cin.clear();
                        cin.ignore(100, '\n');
                        cout << "Introduceți numele angajatului pentru care vreți să calculați salariul:" << endl;
                        string employee_name;
                        getline(cin, employee_name);
                        cout << "Introduceți nr. de ore lucrate:" << endl;
                        string hours_worked;
                        cin >> hours_worked;

                        // verific dacă stringul conține doar cifre
                        if (has_only_digits(hours_worked))
                        {
                            int new_stock = stoi(hours_worked);

                            employee_handler.get_salary(employee_name, 40, true);
                        }
                        else
                        {
                            cout << "Numărul nu a fost bine introdus!" << endl;
                            throw(5);
                        }

                        break;
                    }

                    default:
                        cout << "Selectați o opțiune validă!\n"
                             << endl;
                        break;
                    }
                }
                cin.clear();
                cin.ignore(100, '\n');

                cout << "Doriți să exportați CSV-ul și în engleză? (d/n)";
                char choice;
                cin >> choice;
                if (choice == 'd')
                {
                    convert_to_english(employee_handler);
                }
            }
            catch (int e)
            {
                switch (e)
                {
                case 1:
                    cout << "Eroare la deschiderea fișierului de produse!" << endl;
                    break;
                case 2:
                    cout << "Eroare la căutarea angajatului!" << endl;
                    break;
                case 3:
                    cout << "Eroare la adăugarea unui angajat!" << endl;
                    break;

                default:
                    cout << "Eroare (foarte) tehnică! Ceva a mers foarte prost!" << endl;
                    break;
                }

                break;
            }
            break;

        case '2':

            // am nevoie să cuprind întregul switch case într-un try/catch pentru a putea prelua excepțiile care ar putea apărea

            // pentru a putea declara variabile în interiorul unui switch case, trebuie să definim un scop separat pt acesta
            // în cazul de față, acest lucru este realizat de blocul din try
            try
            {
                ProductHandler product_handler(data_path);

                selection = '0';
                while (selection < '1' || selection > '4')
                {

                    cout << "Selectați operația dorită:" << endl;
                    cout << "1) Adăugare de produse" << endl;
                    cout << "2) Căutare de produse" << endl;
                    cout << "3) Ștergere de produse" << endl;
                    cout << "4) Resetarea stocului" << endl;
                    cin >> selection;

                    switch (selection)
                    {
                    case '1':
                        cin.clear();
                        cin.ignore(100, '\n');
                        product_handler.add_product();
                        break;
                    case '2':
                    {
                        cout << "Introduceți numele produsului căutat:" << endl;
                        cin.clear();
                        cin.ignore(100, '\n');

                        string product_name;
                        getline(cin, product_name);
                        product_handler.search_product(product_name, true);
                        break;
                    }
                    case '3':
                    {
                        cout << "Introduceți numele produsului care trebuie șters:" << endl;
                        cin.clear();
                        cin.ignore(100, '\n');

                        string product_name;
                        getline(cin, product_name);
                        product_handler.remove_product(product_name);
                        break;
                    }
                    case '4':
                    {
                        cin.clear();
                        cin.ignore(100, '\n');
                        cout << "Introduceți numele produsului pentru care vreți să resetați stocul:" << endl;
                        string product_name;
                        getline(cin, product_name);
                        cout << "Introduceți stocul nou:" << endl;
                        string new_stock_buf;
                        cin >> new_stock_buf;

                        // verific dacă stringul conține doar cifre
                        if (has_only_digits(new_stock_buf))
                        {
                            int new_stock = stoi(new_stock_buf);

                            product_handler.reset_stock(product_name, new_stock);
                        }
                        else
                        {
                            cout << "Stoc invalid!" << endl;
                            throw(5);
                        }

                        break;
                    }

                    default:
                        cout << "Selectați o opțiune validă!\n"
                             << endl;
                        break;
                    }
                }
                cin.clear();
                cin.ignore(100, '\n');

                cout << "Doriți să exportați CSV-ul și în engleză? (d/n)";
                char choice;
                cin >> choice;
                if (choice == 'd')
                {
                    convert_to_english(product_handler);
                }
            }
            catch (int e)
            {
                switch (e)
                {
                case 1:
                    cout << "Eroare la deschiderea fișierului de produse!" << endl;
                    break;
                case 2:
                    cout << "Eroare la căutarea produsului!" << endl;
                    break;
                case 7:
                    cout << "Eroare la cumpărarea unui produs!" << endl;
                    break;
                case 8:
                    cout << "Eroare la gestionarea stocului!" << endl;
                    break;

                case 11:
                    cout << "Eroare (foarte) tehnică! Ceva a mers foarte prost!" << endl;
                    break;
                }

                break;
            }

            break;

        case '3':
            // pentru a putea declara variabile în interiorul unui switch case, trebuie să definim un scop separat pt acesta
            // în cazul de față, acest lucru este realizat de blocul din try
            try
            {
                OrderHandler order_handler(data_path);

                selection = '0';
                while (selection < '1' || selection > '2')
                {

                    cout << "Selectați operația dorită:" << endl;
                    cout << "1) Adăugare de comandă" << endl;
                    cout << "2) Căutarea comenzilor dintr-o zi" << endl;
                    cin >> selection;

                    switch (selection)
                    {
                    case '1':
                        cin.clear();
                        cin.ignore(100, '\n');
                        order_handler.create_order();
                        break;
                    case '2':
                    {
                        cout << "Introduceți data comenzilor:" << endl;
                        cin.clear();
                        cin.ignore(100, '\n');

                        string date;
                        getline(cin, date);
                        vector<Order> order_list = order_handler.get_orders_by_date(date);

                        //cout<<"size:"<<order_list.size()<<endl;
                        if(order_list.size()==0){
                            cout<<"Nu au fost înregistrate comenzi în acea dată!"<<endl;
                            break;
                        }

                        for (int i = 0; i < order_list.size(); i++)
                        {
                            order_list[i].display_information();
                        }
                        break;
                    }

                    default:
                        cout << "Selectați o opțiune validă!\n"
                             << endl;
                        break;
                    }
                }
                cin.clear();
                cin.ignore(100, '\n');

                cout << "Doriți să exportați CSV-urile și în engleză? (d/n)";
                char choice;
                cin >> choice;
                if (choice == 'd')
                {
                    convert_to_english(order_handler);
                    CustomerHandler customers(data_path);
                    convert_to_english(customers);
                }
            }
            catch (int e)
            {
                switch (e)
                {
                case 1:
                    cout << "Eroare la deschiderea fișierelor!" << endl;
                    break;
                case 2:
                    cout << "Eroare la adăugarea unui client nou!" << endl;
                    break;
                case 3:
                    cout << "Eroare la crearea unui comenzi!" << endl;
                    break;

                case 11:
                    cout << "Eroare (foarte) tehnică! Ceva a mers foarte prost!" << endl;
                    break;
                }

                break;
            }

            break;

        case '4':
        
            // pentru a putea declara variabile în interiorul unui switch case, trebuie să definim un scop separat pt acesta
            // în cazul de față, acest lucru este realizat de blocul din try
            try
            {
                EventHandler event_handler(data_path);

                selection = '0';
                while (selection < '1' || selection > '3')
                {

                    cout << "Selectați operația dorită:" << endl;
                    cout << "1) Adăugarea unui eveniment" << endl;
                    cout << "2) Căutarea unui eveniment" << endl;
                    cout << "3) Căutarea evenimentelor dintr-o anume zi" << endl;
                    cin >> selection;

                    switch (selection)
                    {
                    case '1':
                        cin.clear();
                        cin.ignore(100, '\n');
                        event_handler.create_event();
                        break;
                    case '2':
                    {
                        cout << "Introduceți numele evenimentului căutat:" << endl;
                        cin.clear();
                        cin.ignore(100, '\n');

                        string event_name;
                        getline(cin, event_name);
                        Event searched_event = event_handler.search_event(event_name, true);
                        //searched_event.display_information();
                        break;
                    }
                    
                    case '3':
                    {
                        cout << "Introduceți data:" << endl;
                        cin.clear();
                        cin.ignore(100, '\n');

                        string date;
                        getline(cin, date);
                        vector<Event> searched_events = event_handler.get_events_by_date(date);
                        
                        cout<<"size:"<<searched_events.size()<<endl;
                        if(searched_events.size()==0){
                            cout<<"Nu au fost înregistrate comenzi în acea dată!"<<endl;
                            break;
                        }

                        for(int i=0; i<searched_events.size(); i++){
                            searched_events[i].display_information();
                            cout<<endl;
                        }

                        break;
                    }
                    
                    default:
                        cout << "Selectați o opțiune validă!\n"
                             << endl;
                        break;
                    }
                }
                cin.clear();
                cin.ignore(100, '\n');

                cout << "Doriți să exportați CSV-ul și în engleză? (d/n)";
                char choice;
                cin >> choice;
                if (choice == 'd')
                {
                    convert_to_english(event_handler);
                }
            }
            catch (int e)
            {
                switch (e)
                {
                case 1:
                    cout << "Eroare la deschiderea fișierului de evenimente!" << endl;
                    break;
                case 2:
                    cout << "Eroare la crearea evenimentului" << endl;
                    break;
                case 7:
                    cout << "Eroare la căutarea evenimentului!" << endl;
                    break;
                
                default:
                    cout << "Eroare (foarte) tehnică! Ceva a mers foarte prost!" << endl;
                    break;
                }

                break;
            }
    

            break;
        case '5':
            
            // pentru a putea declara variabile în interiorul unui switch case, trebuie să definim un scop separat pt acesta
            // în cazul de față, acest lucru este realizat de blocul din try
            try
            {
                FinancialReportHandler financial_handler(data_path);

                selection = '0';
                while (selection < '1' || selection > '2')
                {

                    cout << "Selectați operația dorită:" << endl;
                    cout << "1) Generare de raport"<<endl;
                    cout << "2) Căutare de raport generat" << endl;
                    
                    cin >> selection;

                    switch (selection)
                    {
                    case '1':
                    {   
                        cout << "Introduceți ziua pentru care vreți să generați raportul:" << endl;
                        cin.clear();
                        cin.ignore(100, '\n');

                        string date;
                        getline(cin, date);
                        financial_handler.generate_report(date);
                        break;
                    }
                    case '2':
                    {
                        cout << "Introduceți ziua pentru care vreți să căutați raportul:" << endl;
                        cin.clear();
                        cin.ignore(100, '\n');

                        string date;
                        getline(cin, date);
                        FinancialReport report = financial_handler.get_report_by_date(date);
                        
                        report.display_report();
                        break;
                    }
                    
                    default:
                        cout << "Selectați o opțiune validă!\n"
                             << endl;
                        break;
                    }
                }
                cin.clear();
                cin.ignore(100, '\n');

                cout << "Doriți să exportați CSV-ul și în engleză? (d/n)";
                char choice;
                cin >> choice;
                if (choice == 'd')
                {
                    convert_to_english(financial_handler);
                }
            }
            catch (int e)
            {
                switch (e)
                {
                case 1:
                    cout << "Eroare la deschiderea fișierului de rapoarte generate!" << endl;
                    break;
                case 2:
                    cout << "Eroare la generarea raportului!" << endl;
                    break;
                case 5:
                    cout << "Eroare la căutarea raportului!" << endl;
                    break;
                case 3:
                    cout << "Eroare la inserarea datei!"<<endl;
                    break;
                default:
                    cout << "Eroare (foarte) tehnică! Ceva a mers foarte prost!" << endl;
                    break;
                }

                break;
            }


            break;
        default:
            cout << "Selectați o opțiune validă!\n"
                 << endl;
            break;
        }

        cin.clear();
        cin.ignore(100, '\n');
    }
    return 0;
}