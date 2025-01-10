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

    /*
    EmployeeHandler employee_handler(data_path);
    employee_handler.add_employee();
    cout<<"end"<<endl;

    test();

    convert_to_english(employee_handler);
    */

    EventHandler event_handler(data_path);
    event_handler.create_event();
    cout<<"end"<<endl;

    convert_to_english(event_handler);
    return 0;
}