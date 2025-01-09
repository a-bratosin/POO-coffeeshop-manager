
#ifndef EVENTS_H
#define EVENTS_H

#include <string>
#include <vector>
#include <utility>
#include "handler.h"

using namespace std;
class Event {
private:
    string event_name;
    string event_date;
    // variabilă care stochează ce produse vor fi disponibile la eveniment, și în ce cantități
    vector<pair<string,int>> products;
    float marketing_costs;
    float misc_costs;
    float ticket_price;
    int ticket_count;
public:
    Event(const string &event_name_in, const string &event_date_in, const vector<pair<string,int>> &products_in, const float marketing_costs_in, const float misc_costs_in, const float ticket_price_in, const int ticket_count_in);

    void display_information();

    vector<string> event_to_data();
    vector<pair<string,int>> get_products();
    float get_ticket_price();
    int get_ticket_count();
    float get_misc_costs();
    float get_marketing_costs();
};

class EventHandler: public Handler {
private:
    string event_file_path;
    vector<Event> events;
public:
    EventHandler(const string &file_path_in);
    ~EventHandler() override;

    Event parse_data_element(const vector<string> &data_el);
    void parse_data();

    vector<vector<string>> get_data();

    void write_to_file();

    void create_event();
    Event search_event(string const &name, bool const display=false);

    vector<Event> get_events_by_date(string date);

};

#endif //EVENTS_H
