#include <string>
#include <chrono>
#include <iostream>

using namespace std;
// verific dacă data a fost introdusă/citită conform tiparului dat
bool is_valid_date(std::string date)
{
    std::istringstream in{std::move(date)};
    std::chrono::year_month_day ymd{};
    in >> std::chrono::parse("%d/%m/%Y", ymd);
    return !in.fail();
}


// verific dacă data e din viitor (utilă la unele )
bool is_future_date(const string &date) {

    if (!is_valid_date(date)) {
        cout<<"Dată invalidă!"<<endl;
        throw 1;
    }

    istringstream in(date);
    chrono::year_month_day ymd;

    in >> chrono::parse("%d/%m/%Y", ymd);


    // obțin data curentă
    auto current_date = chrono::year_month_day{chrono::floor<chrono::days>(chrono::system_clock::now())};

    // compar data introdusă cu data curentă
    // aici e un operator overloaded, care compară două obiecte de tip year_month_day
    if (ymd > current_date) {
        return true;
    }
    return false;
}
