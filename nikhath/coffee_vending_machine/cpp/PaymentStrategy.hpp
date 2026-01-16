#ifndef PAYMENT_STRATEGY_HPP
#define PAYMENT_STRATEGY_HPP

#include <string>
#include <iostream>
#include <iomanip>

// Strategy Pattern - Defines a family of algorithms (payment methods)
// Demonstrates Abstraction and Polymorphism (OOP)
class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;
    virtual bool pay(double amount) = 0;
    virtual std::string getPaymentMethod() const = 0;
};

// Concrete Strategy - Cash Payment
class CashPayment : public PaymentStrategy {
private:
    double cashInserted;

public:
    explicit CashPayment(double cash) : cashInserted(cash) {}

    bool pay(double amount) override {
        if (cashInserted >= amount) {
            double change = cashInserted - amount;
            std::cout << std::fixed << std::setprecision(2);
            std::cout << "Payment of $" << amount << " accepted via Cash.\n";
            if (change > 0) {
                std::cout << "Change returned: $" << change << "\n";
            }
            return true;
        }
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Insufficient cash. Required: $" << amount
                  << ", Inserted: $" << cashInserted << "\n";
        return false;
    }

    std::string getPaymentMethod() const override {
        return "Cash";
    }
};

// Concrete Strategy - Card Payment
class CardPayment : public PaymentStrategy {
private:
    std::string cardNumber;
    std::string pin;

    bool validateCard() const {
        return cardNumber.length() >= 16 && pin.length() == 4;
    }

public:
    CardPayment(const std::string& cardNum, const std::string& pinCode)
        : cardNumber(cardNum), pin(pinCode) {}

    bool pay(double amount) override {
        if (validateCard()) {
            std::cout << std::fixed << std::setprecision(2);
            std::cout << "Payment of $" << amount << " accepted via Card (**** "
                      << cardNumber.substr(cardNumber.length() - 4) << ").\n";
            return true;
        }
        std::cout << "Card payment failed. Invalid card or PIN.\n";
        return false;
    }

    std::string getPaymentMethod() const override {
        return "Card";
    }
};

// Concrete Strategy - UPI Payment
class UPIPayment : public PaymentStrategy {
private:
    std::string upiId;

    bool validateUPI() const {
        return upiId.find('@') != std::string::npos;
    }

public:
    explicit UPIPayment(const std::string& upi) : upiId(upi) {}

    bool pay(double amount) override {
        if (validateUPI()) {
            std::cout << std::fixed << std::setprecision(2);
            std::cout << "Payment of $" << amount << " accepted via UPI ("
                      << upiId << ").\n";
            return true;
        }
        std::cout << "UPI payment failed. Invalid UPI ID.\n";
        return false;
    }

    std::string getPaymentMethod() const override {
        return "UPI";
    }
};

#endif // PAYMENT_STRATEGY_HPP
