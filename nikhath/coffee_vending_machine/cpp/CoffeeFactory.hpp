#ifndef COFFEE_FACTORY_HPP
#define COFFEE_FACTORY_HPP

#include "Coffee.hpp"
#include <memory>
#include <stdexcept>
#include <iomanip>

// Factory Pattern - Creates coffee objects without exposing creation logic
enum class CoffeeType {
    ESPRESSO,
    CAPPUCCINO,
    LATTE,
    AMERICANO,
    MOCHA,
    COUNT // Helper for iteration
};

class CoffeeFactory {
public:
    // Factory Method - Polymorphism (OOP)
    static std::unique_ptr<Coffee> createCoffee(CoffeeType type) {
        switch (type) {
            case CoffeeType::ESPRESSO:
                return std::make_unique<Espresso>();
            case CoffeeType::CAPPUCCINO:
                return std::make_unique<Cappuccino>();
            case CoffeeType::LATTE:
                return std::make_unique<Latte>();
            case CoffeeType::AMERICANO:
                return std::make_unique<Americano>();
            case CoffeeType::MOCHA:
                return std::make_unique<Mocha>();
            default:
                throw std::invalid_argument("Unknown coffee type");
        }
    }

    static std::unique_ptr<Coffee> createCoffee(int choice) {
        if (choice >= 0 && choice < static_cast<int>(CoffeeType::COUNT)) {
            return createCoffee(static_cast<CoffeeType>(choice));
        }
        throw std::invalid_argument("Invalid coffee choice");
    }

    static void displayMenu() {
        std::cout << "\n========== COFFEE MENU ==========\n";
        for (int i = 0; i < static_cast<int>(CoffeeType::COUNT); ++i) {
            auto coffee = createCoffee(static_cast<CoffeeType>(i));
            std::cout << (i + 1) << ". " << *coffee << "\n";
        }
        std::cout << "==================================\n";
    }

    static std::string getCoffeeTypeName(CoffeeType type) {
        switch (type) {
            case CoffeeType::ESPRESSO: return "Espresso";
            case CoffeeType::CAPPUCCINO: return "Cappuccino";
            case CoffeeType::LATTE: return "Latte";
            case CoffeeType::AMERICANO: return "Americano";
            case CoffeeType::MOCHA: return "Mocha";
            default: return "Unknown";
        }
    }
};

#endif // COFFEE_FACTORY_HPP
