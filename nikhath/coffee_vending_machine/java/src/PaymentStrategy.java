// Strategy Pattern - Defines a family of algorithms (payment methods)
// Demonstrates Abstraction and Polymorphism (OOP)
public interface PaymentStrategy {
    boolean pay(double amount);
    String getPaymentMethod();
}

// Concrete Strategy - Cash Payment
class CashPayment implements PaymentStrategy {
    private double cashInserted;

    public CashPayment(double cash) {
        this.cashInserted = cash;
    }

    @Override
    public boolean pay(double amount) {
        if (cashInserted >= amount) {
            double change = cashInserted - amount;
            System.out.printf("Payment of $%.2f accepted via Cash.%n", amount);
            if (change > 0) {
                System.out.printf("Change returned: $%.2f%n", change);
            }
            return true;
        }
        System.out.printf("Insufficient cash. Required: $%.2f, Inserted: $%.2f%n",
                          amount, cashInserted);
        return false;
    }

    @Override
    public String getPaymentMethod() {
        return "Cash";
    }
}

// Concrete Strategy - Card Payment
class CardPayment implements PaymentStrategy {
    private String cardNumber;
    private String pin;

    public CardPayment(String cardNumber, String pin) {
        this.cardNumber = cardNumber;
        this.pin = pin;
    }

    @Override
    public boolean pay(double amount) {
        // Simulate card validation
        if (validateCard()) {
            System.out.printf("Payment of $%.2f accepted via Card (**** %s).%n",
                              amount, cardNumber.substring(cardNumber.length() - 4));
            return true;
        }
        System.out.println("Card payment failed. Invalid card or PIN.");
        return false;
    }

    private boolean validateCard() {
        return cardNumber != null && cardNumber.length() >= 16 &&
               pin != null && pin.length() == 4;
    }

    @Override
    public String getPaymentMethod() {
        return "Card";
    }
}

// Concrete Strategy - UPI Payment
class UPIPayment implements PaymentStrategy {
    private String upiId;

    public UPIPayment(String upiId) {
        this.upiId = upiId;
    }

    @Override
    public boolean pay(double amount) {
        if (validateUPI()) {
            System.out.printf("Payment of $%.2f accepted via UPI (%s).%n", amount, upiId);
            return true;
        }
        System.out.println("UPI payment failed. Invalid UPI ID.");
        return false;
    }

    private boolean validateUPI() {
        return upiId != null && upiId.contains("@");
    }

    @Override
    public String getPaymentMethod() {
        return "UPI";
    }
}
