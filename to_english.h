#ifndef ENGLISH_H
#define ENGLISH_H

#include "employees.h"
#include "events.h"
#include "customers_orders.h"
#include "financial.h"
#include "products.h"

template <typename T>
void convert_to_english(T& handler);

template<>
void convert_to_english(EmployeeHandler& handler);

template<>
void convert_to_english(EventHandler& handler);

template<>
void convert_to_english(CustomerHandler& handler);


template<>
void convert_to_english(OrderHandler& handler);

template<>
void convert_to_english(ProductHandler& handler);

template<>
void convert_to_english(FinancialReportHandler& handler);

#endif //ENGLISH_H