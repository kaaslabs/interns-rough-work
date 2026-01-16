#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <string>
#include <vector>
#include <algorithm>

// Observer Pattern - For notifying operators about inventory levels
// Demonstrates Abstraction (OOP)

// Observer Interface
class InventoryObserver {
public:
    virtual ~InventoryObserver() = default;
    virtual void update(const std::string& ingredient, int currentLevel, int threshold) = 0;
};

// Subject Interface
class InventorySubject {
public:
    virtual ~InventorySubject() = default;
    virtual void addObserver(InventoryObserver* observer) = 0;
    virtual void removeObserver(InventoryObserver* observer) = 0;
    virtual void notifyObservers(const std::string& ingredient, int currentLevel, int threshold) = 0;
};

#endif // OBSERVER_HPP
