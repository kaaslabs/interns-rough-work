/**
 * Coffee Vending Machine - Main Demo (C++)
 *
 * Design Patterns Used:
 * 1. Singleton Pattern - CoffeeMachine (single instance)
 * 2. Factory Pattern - CoffeeFactory (creates coffee objects)
 * 3. State Pattern - MachineState (Idle, Selecting, Processing, Dispensing)
 * 4. Strategy Pattern - PaymentStrategy (Cash, Card, UPI)
 * 5. Observer Pattern - Operator observes Inventory for low-level alerts
 *
 * OOP Concepts Applied:
 * 1. Encapsulation - Private members with getters/setters
 * 2. Abstraction - Abstract classes and interfaces
 * 3. Inheritance - Coffee hierarchy (Espresso, Cappuccino, etc.)
 * 4. Polymorphism - Different payment methods and coffee types
 *
 * Actors:
 * 1. User - Orders coffee, makes payment
 * 2. Operator - Maintains machine, refills inventory
 * 3. Machine - Central system (Singleton)
 * 4. Payment - Handles transactions (Strategy Pattern)
 * 5. Inventory - Manages ingredients (Observer Subject)
 */

#include <iostream>
#include <string>
#include <memory>
#include <limits>

#include "Coffee.hpp"
#include "CoffeeFactory.hpp"
#include "PaymentStrategy.hpp"
#include "Observer.hpp"
#include "Inventory.hpp"
#include "CoffeeMachine.hpp"
#include "User.hpp"
#include "Operator.hpp"

void runInteractiveMode(CoffeeMachine* machine, Operator* op);
void orderCoffeeInteractive(User& user);

int main() {
    std::cout << "╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║       COFFEE VENDING MACHINE - SYSTEM DEMONSTRATION      ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n";

    // Get the singleton machine instance
    CoffeeMachine* machine = CoffeeMachine::getInstance();

    // Create actors
    User user1("U001", "Alice");
    User user2("U002", "Bob");
    Operator op("OP001", "Charlie");

    // Demonstration scenarios
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "SCENARIO 1: Normal Coffee Purchase with Cash\n";
    std::cout << std::string(60, '=') << "\n";

    user1.viewMenu();
    user1.selectCoffee(2); // Cappuccino
    user1.makePayment(std::make_unique<CashPayment>(5.00));

    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "SCENARIO 2: Coffee Purchase with Card Payment\n";
    std::cout << std::string(60, '=') << "\n";

    user2.selectCoffee(3); // Latte
    user2.makePayment(std::make_unique<CardPayment>("1234567890123456", "1234"));

    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "SCENARIO 3: Coffee Purchase with UPI\n";
    std::cout << std::string(60, '=') << "\n";

    user1.selectCoffee(5); // Mocha
    user1.makePayment(std::make_unique<UPIPayment>("alice@upi"));

    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "SCENARIO 4: Failed Payment (Insufficient Cash)\n";
    std::cout << std::string(60, '=') << "\n";

    user2.selectCoffee(1); // Espresso ($2.50)
    user2.makePayment(std::make_unique<CashPayment>(1.00)); // Only $1
    user2.cancelOrder();

    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "SCENARIO 5: Operator Checks Inventory\n";
    std::cout << std::string(60, '=') << "\n";

    op.checkInventory();
    op.viewMachineStatus();

    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "SCENARIO 6: Multiple Orders to Trigger Low Inventory Alert\n";
    std::cout << std::string(60, '=') << "\n";

    // Make several orders to deplete inventory
    for (int i = 0; i < 5; ++i) {
        user1.selectCoffee(2); // Cappuccino (uses lots of milk)
        user1.makePayment(std::make_unique<CashPayment>(10.00));
    }

    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "SCENARIO 7: Operator Views Alerts and Refills\n";
    std::cout << std::string(60, '=') << "\n";

    op.viewAlerts();
    op.checkInventory();
    op.refillAll();
    op.checkInventory();

    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "SCENARIO 8: Machine Maintenance\n";
    std::cout << std::string(60, '=') << "\n";

    op.performMaintenance();
    user1.selectCoffee(1); // Should fail - machine under maintenance
    op.completeMaintenance();
    user1.selectCoffee(1); // Should work now
    user1.makePayment(std::make_unique<CashPayment>(5.00));

    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "DEMONSTRATION COMPLETE\n";
    std::cout << std::string(60, '=') << "\n";

    // Interactive mode
    runInteractiveMode(machine, &op);

    // Cleanup singleton
    CoffeeMachine::destroyInstance();

    return 0;
}

void runInteractiveMode(CoffeeMachine* machine, Operator* op) {
    std::cout << "\n\nWould you like to try the interactive mode? (y/n): ";
    std::string choice;
    std::getline(std::cin, choice);

    if (choice != "y" && choice != "Y") {
        std::cout << "Thank you for using the Coffee Vending Machine!\n";
        return;
    }

    User interactiveUser("U003", "Guest");
    bool running = true;

    while (running) {
        std::cout << "\n========== MAIN MENU ==========\n";
        std::cout << "1. View Coffee Menu\n";
        std::cout << "2. Order Coffee\n";
        std::cout << "3. View Machine Status\n";
        std::cout << "4. Operator: Check Inventory\n";
        std::cout << "5. Operator: Refill All\n";
        std::cout << "6. Exit\n";
        std::cout << "================================\n";
        std::cout << "Enter choice: ";

        int menuChoice;
        if (!(std::cin >> menuChoice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Please enter a valid number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (menuChoice) {
            case 1:
                machine->displayMenu();
                break;

            case 2:
                orderCoffeeInteractive(interactiveUser);
                break;

            case 3:
                machine->displayStatus();
                break;

            case 4:
                op->checkInventory();
                break;

            case 5:
                op->refillAll();
                break;

            case 6:
                running = false;
                std::cout << "Thank you for using the Coffee Vending Machine!\n";
                break;

            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

void orderCoffeeInteractive(User& user) {
    CoffeeMachine::getInstance()->displayMenu();
    std::cout << "Select coffee (1-5): ";

    int coffeeChoice;
    if (!(std::cin >> coffeeChoice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Order cancelled.\n";
        return;
    }

    user.selectCoffee(coffeeChoice);

    std::cout << "\nPayment Methods:\n";
    std::cout << "1. Cash\n";
    std::cout << "2. Card\n";
    std::cout << "3. UPI\n";
    std::cout << "Select payment method: ";

    int paymentChoice;
    if (!(std::cin >> paymentChoice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Order cancelled.\n";
        user.cancelOrder();
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::unique_ptr<PaymentStrategy> payment;

    switch (paymentChoice) {
        case 1: {
            std::cout << "Enter cash amount: $";
            double amount;
            if (!(std::cin >> amount)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid amount. Order cancelled.\n";
                user.cancelOrder();
                return;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            payment = std::make_unique<CashPayment>(amount);
            break;
        }

        case 2: {
            std::cout << "Enter card number (16 digits): ";
            std::string cardNum;
            std::getline(std::cin, cardNum);
            std::cout << "Enter PIN (4 digits): ";
            std::string pin;
            std::getline(std::cin, pin);
            payment = std::make_unique<CardPayment>(cardNum, pin);
            break;
        }

        case 3: {
            std::cout << "Enter UPI ID: ";
            std::string upiId;
            std::getline(std::cin, upiId);
            payment = std::make_unique<UPIPayment>(upiId);
            break;
        }

        default:
            std::cout << "Invalid payment method. Order cancelled.\n";
            user.cancelOrder();
            return;
    }

    user.makePayment(std::move(payment));
}
