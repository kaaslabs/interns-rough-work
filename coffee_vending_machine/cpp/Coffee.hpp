#ifndef COFFEE_HPP
#define COFFEE_HPP

#include <string>
#include <iostream>

// Abstract Product - Part of Factory Pattern
// Demonstrates Abstraction (OOP)
class Coffee {
protected:
    std::string name;
    double price;
    int preparationTime; // in seconds

public:
    virtual ~Coffee() = default;
    virtual void prepare() = 0;

    std::string getName() const { return name; }
    double getPrice() const { return price; }
    int getPreparationTime() const { return preparationTime; }

    friend std::ostream& operator<<(std::ostream& os, const Coffee& coffee) {
        os << coffee.name << " - $" << coffee.price;
        return os;
    }
};

// Concrete Products - Inheritance (OOP)
class Espresso : public Coffee {
public:
    Espresso() {
        name = "Espresso";
        price = 2.50;
        preparationTime = 30;
    }

    void prepare() override {
        std::cout << "Preparing Espresso: Grinding beans, extracting shot...\n";
    }
};

class Cappuccino : public Coffee {
public:
    Cappuccino() {
        name = "Cappuccino";
        price = 3.50;
        preparationTime = 45;
    }

    void prepare() override {
        std::cout << "Preparing Cappuccino: Extracting espresso, steaming milk, adding foam...\n";
    }
};

class Latte : public Coffee {
public:
    Latte() {
        name = "Latte";
        price = 3.00;
        preparationTime = 40;
    }

    void prepare() override {
        std::cout << "Preparing Latte: Extracting espresso, adding steamed milk...\n";
    }
};

class Americano : public Coffee {
public:
    Americano() {
        name = "Americano";
        price = 2.00;
        preparationTime = 25;
    }

    void prepare() override {
        std::cout << "Preparing Americano: Extracting espresso, adding hot water...\n";
    }
};

class Mocha : public Coffee {
public:
    Mocha() {
        name = "Mocha";
        price = 4.00;
        preparationTime = 50;
    }

    void prepare() override {
        std::cout << "Preparing Mocha: Adding chocolate, extracting espresso, steaming milk...\n";
    }
};

#endif // COFFEE_HPP
