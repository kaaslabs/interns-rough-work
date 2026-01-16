#ifndef USER_HPP
#define USER_HPP

#include "CoffeeMachine.hpp"
#include <string>
#include <iostream>

// User Actor - Interacts with the Coffee Machine
// Demonstrates Encapsulation (OOP)
class User {
private:
    std::string userId;
    std::string name;
    CoffeeMachine* machine;

public:
    User(const std::string& id, const std::string& userName)
        : userId(id), name(userName), machine(CoffeeMachine::getInstance()) {}

    void viewMenu() {
        std::cout << "\nUser " << name << " viewing menu...\n";
        machine->displayMenu();
    }

    void selectCoffee(int choice) {
        std::cout << "\nUser " << name << " selecting coffee option " << choice << "...\n";
        machine->selectCoffee(choice);
    }

    void makePayment(std::unique_ptr<PaymentStrategy> payment) {
        std::cout << "\nUser " << name << " making payment via "
                  << payment->getPaymentMethod() << "...\n";
        machine->makePayment(std::move(payment));
    }

    void cancelOrder() {
        std::cout << "\nUser " << name << " cancelling order...\n";
        machine->cancelOrder();
    }

    // Convenience method for complete transaction
    void orderCoffee(int choice, std::unique_ptr<PaymentStrategy> payment) {
        viewMenu();
        selectCoffee(choice);
        makePayment(std::move(payment));
    }

    // Getters
    const std::string& getUserId() const { return userId; }
    const std::string& getName() const { return name; }
};

#endif // USER_HPP
