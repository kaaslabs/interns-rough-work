#ifndef OPERATOR_HPP
#define OPERATOR_HPP

#include "CoffeeMachine.hpp"
#include "Observer.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

// Operator Actor - Manages and maintains the Coffee Machine
// Implements Observer Pattern to receive inventory alerts
// Demonstrates Encapsulation and Inheritance concepts (OOP)
class Operator : public InventoryObserver {
private:
    std::string operatorId;
    std::string name;
    CoffeeMachine* machine;
    std::vector<std::string> alerts;

public:
    Operator(const std::string& id, const std::string& operatorName)
        : operatorId(id), name(operatorName), machine(CoffeeMachine::getInstance()) {
        // Register this operator as an observer for inventory updates
        machine->registerObserver(this);
    }

    ~Operator() {
        // Unregister from notifications
        machine->removeObserver(this);
    }

    // Observer Pattern - Receive notifications about low inventory
    void update(const std::string& ingredient, int currentLevel, int threshold) override {
        std::ostringstream oss;
        oss << "[ALERT] Low inventory: " << ingredient
            << " at " << currentLevel << " (threshold: " << threshold << ")";
        alerts.push_back(oss.str());

        std::cout << "\n*** OPERATOR NOTIFICATION ***\n";
        std::cout << "Operator " << name << " received alert:\n";
        std::cout << oss.str() << "\n";
        std::cout << "*****************************\n\n";
    }

    // Maintenance operations
    void checkInventory() {
        std::cout << "\nOperator " << name << " checking inventory...\n";
        machine->getInventory()->displayInventory();
    }

    void refillIngredient(const std::string& ingredient, int amount) {
        std::cout << "\nOperator " << name << " refilling " << ingredient << "...\n";
        machine->getInventory()->refillIngredient(ingredient, amount);
    }

    void performMaintenance() {
        std::cout << "\nOperator " << name << " performing maintenance...\n";
        machine->setOperational(false);
        std::cout << "Machine is now in maintenance mode.\n";
    }

    void completeMaintenance() {
        std::cout << "\nOperator " << name << " completing maintenance...\n";
        machine->setOperational(true);
        std::cout << "Machine is back online.\n";
    }

    void viewMachineStatus() {
        std::cout << "\nOperator " << name << " checking machine status...\n";
        machine->displayStatus();
    }

    void viewAlerts() {
        std::cout << "\n===== OPERATOR ALERTS =====\n";
        if (alerts.empty()) {
            std::cout << "No alerts.\n";
        } else {
            for (const auto& alert : alerts) {
                std::cout << alert << "\n";
            }
        }
        std::cout << "===========================\n";
    }

    void clearAlerts() {
        alerts.clear();
        std::cout << "Alerts cleared.\n";
    }

    // Refill all ingredients to maximum
    void refillAll() {
        std::cout << "\nOperator " << name << " refilling all ingredients...\n";
        refillIngredient("Coffee Beans", 400);
        refillIngredient("Water", 1500);
        refillIngredient("Milk", 800);
        refillIngredient("Chocolate", 150);
        refillIngredient("Cups", 40);
        std::cout << "All ingredients refilled.\n";
    }

    // Getters
    const std::string& getOperatorId() const { return operatorId; }
    const std::string& getName() const { return name; }
    const std::vector<std::string>& getAlerts() const { return alerts; }
};

#endif // OPERATOR_HPP
