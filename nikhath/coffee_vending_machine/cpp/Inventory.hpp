#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include "Observer.hpp"
#include "CoffeeFactory.hpp"
#include <map>
#include <string>
#include <iostream>
#include <iomanip>

// Inventory - Implements Observer Pattern (Subject)
// Demonstrates Encapsulation (OOP)
class Inventory : public InventorySubject {
private:
    std::map<std::string, int> ingredients;
    std::map<std::string, int> thresholds;
    std::vector<InventoryObserver*> observers;

    // Recipe definitions
    static std::map<CoffeeType, std::map<std::string, int>> RECIPES;

    static void initializeRecipes() {
        static bool initialized = false;
        if (initialized) return;

        RECIPES[CoffeeType::ESPRESSO] = {
            {"Coffee Beans", 20}, {"Water", 30}
        };

        RECIPES[CoffeeType::CAPPUCCINO] = {
            {"Coffee Beans", 20}, {"Water", 30}, {"Milk", 100}
        };

        RECIPES[CoffeeType::LATTE] = {
            {"Coffee Beans", 20}, {"Water", 30}, {"Milk", 150}
        };

        RECIPES[CoffeeType::AMERICANO] = {
            {"Coffee Beans", 20}, {"Water", 150}
        };

        RECIPES[CoffeeType::MOCHA] = {
            {"Coffee Beans", 20}, {"Water", 30}, {"Milk", 100}, {"Chocolate", 30}
        };

        initialized = true;
    }

    void initializeInventory() {
        // Initialize with default quantities (in grams/ml)
        ingredients["Coffee Beans"] = 500;
        ingredients["Water"] = 2000;
        ingredients["Milk"] = 1000;
        ingredients["Chocolate"] = 200;
        ingredients["Cups"] = 50;

        // Set low-level thresholds
        thresholds["Coffee Beans"] = 100;
        thresholds["Water"] = 500;
        thresholds["Milk"] = 200;
        thresholds["Chocolate"] = 50;
        thresholds["Cups"] = 10;
    }

public:
    Inventory() {
        initializeRecipes();
        initializeInventory();
    }

    bool checkAvailability(CoffeeType coffeeType) {
        auto recipeIt = RECIPES.find(coffeeType);
        if (recipeIt == RECIPES.end()) return false;

        for (const auto& [ingredient, required] : recipeIt->second) {
            auto it = ingredients.find(ingredient);
            if (it == ingredients.end() || it->second < required) {
                return false;
            }
        }
        return ingredients["Cups"] > 0;
    }

    void consumeIngredients(CoffeeType coffeeType) {
        auto recipeIt = RECIPES.find(coffeeType);
        if (recipeIt == RECIPES.end()) return;

        for (const auto& [ingredient, required] : recipeIt->second) {
            ingredients[ingredient] -= required;
            int current = ingredients[ingredient];

            // Check if below threshold and notify observers
            if (current <= thresholds[ingredient]) {
                notifyObservers(ingredient, current, thresholds[ingredient]);
            }
        }

        // Consume a cup
        ingredients["Cups"]--;
        if (ingredients["Cups"] <= thresholds["Cups"]) {
            notifyObservers("Cups", ingredients["Cups"], thresholds["Cups"]);
        }
    }

    void refillIngredient(const std::string& ingredient, int amount) {
        auto it = ingredients.find(ingredient);
        if (it != ingredients.end()) {
            int current = it->second;
            it->second += amount;
            std::cout << "Refilled " << ingredient << ": " << current
                      << " + " << amount << " = " << it->second << "\n";
        } else {
            std::cout << "Unknown ingredient: " << ingredient << "\n";
        }
    }

    void displayInventory() const {
        std::cout << "\n========== INVENTORY STATUS ==========\n";
        for (const auto& [ingredient, quantity] : ingredients) {
            auto threshIt = thresholds.find(ingredient);
            std::string status = (threshIt != thresholds.end() &&
                                  quantity <= threshIt->second) ? " [LOW]" : "";
            std::cout << std::left << std::setw(15) << ingredient
                      << ": " << quantity << status << "\n";
        }
        std::cout << "=======================================\n";
    }

    // Observer Pattern methods
    void addObserver(InventoryObserver* observer) override {
        observers.push_back(observer);
    }

    void removeObserver(InventoryObserver* observer) override {
        observers.erase(
            std::remove(observers.begin(), observers.end(), observer),
            observers.end()
        );
    }

    void notifyObservers(const std::string& ingredient, int currentLevel, int threshold) override {
        for (auto* observer : observers) {
            observer->update(ingredient, currentLevel, threshold);
        }
    }

    const std::map<std::string, int>& getIngredients() const {
        return ingredients;
    }
};

// Static member definition
std::map<CoffeeType, std::map<std::string, int>> Inventory::RECIPES;

#endif // INVENTORY_HPP
