# Coffee Vending Machine - C++ Makefile

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2

TARGET = coffee_vending_machine
SRCS = main.cpp
HEADERS = Coffee.hpp CoffeeFactory.hpp PaymentStrategy.hpp Observer.hpp \
          Inventory.hpp MachineState.hpp CoffeeMachine.hpp User.hpp Operator.hpp

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: clean $(TARGET)
