#ifndef FINANCIAL_H
#define FINANCIAL_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include <array>
#include "events.h"
#include "employees.h"
#include "products.h"
#include "customers_orders.h"
#include "handler.h"

using namespace std;
// clasă care conține raportul financiar calculat pentru o anumită zi
class FinancialReport {
private:
    string date;
    float wages;
    float product_costs;
    float product_revenues;
    float event_revenues;
    float event_costs;


public:
    // aici am doi constructori diferiți, în funcție de argumentele primite
    // un exemplu de polimorfism
    FinancialReport(const string& date, const float wages_in, const float product_costs_in, const float product_revenues_in, const float event_revenues_in, const float event_costs_in);

    FinancialReport(string date, string root_folder);

    pair<float,float> calculate_product_revenue_cost(string date, string root_folder);

    float calculate_wages(string date, string root_folder);

    pair<float,float> calculate_event_revenue_cost(string date, string root_folder);

    void display_report();
    vector<string> report_to_data();

    string get_date();

};

class FinancialReportHandler: public Handler {
private:
    string report_file_path;
    vector<FinancialReport> reports;
public:
    FinancialReportHandler(const string &file_path_in);
    ~FinancialReportHandler() override;
    FinancialReport parse_data_element(const vector<string> &data_el);

    void parse_data() override;

    vector<vector<string>> get_data() override;

    void write_to_file() override;

    void generate_report(string date);

    FinancialReport get_report_by_date(string date);
};

#endif // FINANCIAL_H