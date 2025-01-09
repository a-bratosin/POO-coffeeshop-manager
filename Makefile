CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -fPIC
TARGET = build/coffeeshop-manager
SRCS = main.cpp input_handling.cpp employees.cpp
OBJS = $(SRCS:.cpp=.o)


all: $(TARGET)
$(TARGET): $(OBJS)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


