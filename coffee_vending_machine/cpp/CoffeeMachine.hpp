#ifndef COFFEE_MACHINE_HPP
#define COFFEE_MACHINE_HPP

#include "Coffee.hpp"
#include "CoffeeFactory.hpp"
#include "PaymentStrategy.hpp"
#include "Inventory.hpp"
#include "MachineState.hpp"
#include <memory>
#include <mutex>
#include <iostream>

// Forward declarations for state classes
class IdleState;
class SelectingState;
class ProcessingState;
class DispensingState;

// Singleton Pattern - Ensures only one instance of CoffeeMachine exists
// Demonstrates Encapsulation (OOP)
class CoffeeMachine {
private:
    std::unique_ptr<MachineState> currentState;
    std::unique_ptr<Inventory> inventory;
    std::unique_ptr<Coffee> selectedCoffee;
    CoffeeType selectedCoffeeType;
    bool isOperational;

    // Singleton instance
    static CoffeeMachine* instance;
    static std::mutex mutex_;

    // Private constructor for Singleton
    CoffeeMachine();

public:
    // Delete copy constructor and assignment operator
    CoffeeMachine(const CoffeeMachine&) = delete;
    CoffeeMachine& operator=(const CoffeeMachine&) = delete;

    // Thread-safe Singleton getInstance
    static CoffeeMachine* getInstance();

    // Cleanup singleton (for proper resource management)
    static void destroyInstance();

    // State Pattern - Delegate actions to current state
    void selectCoffee(int choice);
    void makePayment(std::unique_ptr<PaymentStrategy> payment);
    void cancelOrder();
    void displayMenu();
    void displayStatus();

    // Getters and Setters
    MachineState* getCurrentState() { return currentState.get(); }
    void setState(std::unique_ptr<MachineState> state);

    Inventory* getInventory() { return inventory.get(); }

    Coffee* getSelectedCoffee() { return selectedCoffee.get(); }
    void setSelectedCoffee(std::unique_ptr<Coffee> coffee);

    CoffeeType getSelectedCoffeeType() const { return selectedCoffeeType; }
    void setSelectedCoffeeType(CoffeeType type) { selectedCoffeeType = type; }

    bool getIsOperational() const { return isOperational; }
    void setOperational(bool operational) { isOperational = operational; }

    // Observer registration helper
    void registerObserver(InventoryObserver* observer);
    void removeObserver(InventoryObserver* observer);
};

// Concrete State - Idle State
class IdleState : public MachineState {
public:
    void selectCoffee(CoffeeMachine* machine, int choice) override;
    void insertPayment(CoffeeMachine* machine, std::unique_ptr<PaymentStrategy> payment) override;
    void dispense(CoffeeMachine* machine) override;
    void cancel(CoffeeMachine* machine) override;
    std::string getStateName() const override { return "Idle"; }
};

// Concrete State - Selecting State
class SelectingState : public MachineState {
public:
    void selectCoffee(CoffeeMachine* machine, int choice) override;
    void insertPayment(CoffeeMachine* machine, std::unique_ptr<PaymentStrategy> payment) override;
    void dispense(CoffeeMachine* machine) override;
    void cancel(CoffeeMachine* machine) override;
    std::string getStateName() const override { return "Selecting"; }
};

// Concrete State - Processing State
class ProcessingState : public MachineState {
public:
    void selectCoffee(CoffeeMachine* machine, int choice) override;
    void insertPayment(CoffeeMachine* machine, std::unique_ptr<PaymentStrategy> payment) override;
    void dispense(CoffeeMachine* machine) override;
    void cancel(CoffeeMachine* machine) override;
    std::string getStateName() const override { return "Processing"; }
};

// Concrete State - Dispensing State
class DispensingState : public MachineState {
public:
    void selectCoffee(CoffeeMachine* machine, int choice) override;
    void insertPayment(CoffeeMachine* machine, std::unique_ptr<PaymentStrategy> payment) override;
    void dispense(CoffeeMachine* machine) override;
    void cancel(CoffeeMachine* machine) override;
    std::string getStateName() const override { return "Dispensing"; }
};

// Static member definitions
CoffeeMachine* CoffeeMachine::instance = nullptr;
std::mutex CoffeeMachine::mutex_;

// CoffeeMachine Implementation
CoffeeMachine::CoffeeMachine()
    : inventory(std::make_unique<Inventory>()),
      currentState(std::make_unique<IdleState>()),
      isOperational(true),
      selectedCoffeeType(CoffeeType::ESPRESSO) {}

CoffeeMachine* CoffeeMachine::getInstance() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (instance == nullptr) {
        instance = new CoffeeMachine();
    }
    return instance;
}

void CoffeeMachine::destroyInstance() {
    std::lock_guard<std::mutex> lock(mutex_);
    delete instance;
    instance = nullptr;
}

void CoffeeMachine::selectCoffee(int choice) {
    if (!isOperational) {
        std::cout << "Machine is under maintenance. Please try later.\n";
        return;
    }
    currentState->selectCoffee(this, choice);
}

void CoffeeMachine::makePayment(std::unique_ptr<PaymentStrategy> payment) {
    if (!isOperational) {
        std::cout << "Machine is under maintenance. Please try later.\n";
        return;
    }
    currentState->insertPayment(this, std::move(payment));
}

void CoffeeMachine::cancelOrder() {
    currentState->cancel(this);
}

void CoffeeMachine::displayMenu() {
    CoffeeFactory::displayMenu();
}

void CoffeeMachine::displayStatus() {
    std::cout << "\n===== MACHINE STATUS =====\n";
    std::cout << "State: " << currentState->getStateName() << "\n";
    std::cout << "Operational: " << (isOperational ? "Yes" : "No (Maintenance)") << "\n";
    if (selectedCoffee) {
        std::cout << "Selected: " << selectedCoffee->getName() << "\n";
    }
    std::cout << "==========================\n";
}

void CoffeeMachine::setState(std::unique_ptr<MachineState> state) {
    currentState = std::move(state);
}

void CoffeeMachine::setSelectedCoffee(std::unique_ptr<Coffee> coffee) {
    selectedCoffee = std::move(coffee);
}

void CoffeeMachine::registerObserver(InventoryObserver* observer) {
    inventory->addObserver(observer);
}

void CoffeeMachine::removeObserver(InventoryObserver* observer) {
    inventory->removeObserver(observer);
}

// State Implementations
// IdleState
void IdleState::selectCoffee(CoffeeMachine* machine, int choice) {
    try {
        CoffeeType type = static_cast<CoffeeType>(choice - 1);
        if (machine->getInventory()->checkAvailability(type)) {
            auto coffee = CoffeeFactory::createCoffee(type);
            std::cout << "Selected: " << coffee->getName() << "\n";
            std::cout << std::fixed << std::setprecision(2);
            std::cout << "Price: $" << coffee->getPrice() << "\n";
            machine->setSelectedCoffeeType(type);
            machine->setSelectedCoffee(std::move(coffee));
            machine->setState(std::make_unique<SelectingState>());
        } else {
            std::cout << "Sorry, " << CoffeeFactory::getCoffeeTypeName(type)
                      << " is currently unavailable due to low ingredients.\n";
        }
    } catch (...) {
        std::cout << "Invalid selection. Please try again.\n";
    }
}

void IdleState::insertPayment(CoffeeMachine* machine, std::unique_ptr<PaymentStrategy> payment) {
    std::cout << "Please select a coffee first.\n";
}

void IdleState::dispense(CoffeeMachine* machine) {
    std::cout << "Please select a coffee and make payment first.\n";
}

void IdleState::cancel(CoffeeMachine* machine) {
    std::cout << "Nothing to cancel.\n";
}

// SelectingState
void SelectingState::selectCoffee(CoffeeMachine* machine, int choice) {
    std::cout << "Coffee already selected. Please make payment or cancel.\n";
}

void SelectingState::insertPayment(CoffeeMachine* machine, std::unique_ptr<PaymentStrategy> payment) {
    Coffee* coffee = machine->getSelectedCoffee();
    if (payment->pay(coffee->getPrice())) {
        machine->setState(std::make_unique<ProcessingState>());
        machine->getCurrentState()->dispense(machine);
    } else {
        std::cout << "Payment failed. Please try again or cancel.\n";
    }
}

void SelectingState::dispense(CoffeeMachine* machine) {
    std::cout << "Please complete payment first.\n";
}

void SelectingState::cancel(CoffeeMachine* machine) {
    std::cout << "Order cancelled.\n";
    machine->setSelectedCoffee(nullptr);
    machine->setState(std::make_unique<IdleState>());
}

// ProcessingState
void ProcessingState::selectCoffee(CoffeeMachine* machine, int choice) {
    std::cout << "Machine is processing. Please wait.\n";
}

void ProcessingState::insertPayment(CoffeeMachine* machine, std::unique_ptr<PaymentStrategy> payment) {
    std::cout << "Payment already received. Processing order.\n";
}

void ProcessingState::dispense(CoffeeMachine* machine) {
    Coffee* coffee = machine->getSelectedCoffee();
    std::cout << "\nProcessing your order...\n";
    coffee->prepare();

    std::cout << "Please wait " << coffee->getPreparationTime() << " seconds...\n";

    // Consume ingredients from inventory
    machine->getInventory()->consumeIngredients(machine->getSelectedCoffeeType());

    machine->setState(std::make_unique<DispensingState>());
    machine->getCurrentState()->dispense(machine);
}

void ProcessingState::cancel(CoffeeMachine* machine) {
    std::cout << "Cannot cancel. Order is being processed.\n";
}

// DispensingState
void DispensingState::selectCoffee(CoffeeMachine* machine, int choice) {
    std::cout << "Please collect your coffee first.\n";
}

void DispensingState::insertPayment(CoffeeMachine* machine, std::unique_ptr<PaymentStrategy> payment) {
    std::cout << "Please collect your coffee first.\n";
}

void DispensingState::dispense(CoffeeMachine* machine) {
    Coffee* coffee = machine->getSelectedCoffee();
    std::cout << "\n*** Your " << coffee->getName() << " is ready! ***\n";
    std::cout << "Please collect your coffee from the dispenser.\n";
    std::cout << "Thank you for your purchase!\n\n";

    // Reset machine state
    machine->setSelectedCoffee(nullptr);
    machine->setState(std::make_unique<IdleState>());
}

void DispensingState::cancel(CoffeeMachine* machine) {
    std::cout << "Cannot cancel. Coffee is being dispensed.\n";
}

#endif // COFFEE_MACHINE_HPP
