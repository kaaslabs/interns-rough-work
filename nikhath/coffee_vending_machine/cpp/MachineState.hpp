#ifndef MACHINE_STATE_HPP
#define MACHINE_STATE_HPP

#include <string>
#include <memory>
#include <iostream>

// Forward declaration
class CoffeeMachine;

// State Pattern - Allows machine to alter behavior when internal state changes
// Demonstrates Abstraction and Polymorphism (OOP)
class MachineState {
public:
    virtual ~MachineState() = default;
    virtual void selectCoffee(CoffeeMachine* machine, int choice) = 0;
    virtual void insertPayment(CoffeeMachine* machine, std::unique_ptr<PaymentStrategy> payment) = 0;
    virtual void dispense(CoffeeMachine* machine) = 0;
    virtual void cancel(CoffeeMachine* machine) = 0;
    virtual std::string getStateName() const = 0;
};

#endif // MACHINE_STATE_HPP
