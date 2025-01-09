CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -fPIC
TARGET = build/coffeeshop-manager
SRCS = main.cpp input_handling.cpp employees.cpp products.cpp financial.cpp customers_orders.cpp check_date.cpp events.cpp
OBJS = $(SRCS:.cpp=.o)


all: $(TARGET)
$(TARGET): $(OBJS)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


