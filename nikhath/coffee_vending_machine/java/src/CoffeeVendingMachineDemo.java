import java.util.Scanner;

/**
 * Coffee Vending Machine - Main Demo Class
 *
 * Design Patterns Used:
 * 1. Singleton Pattern - CoffeeMachine (single instance)
 * 2. Factory Pattern - CoffeeFactory (creates coffee objects)
 * 3. State Pattern - MachineState (Idle, Selecting, Processing, Dispensing)
 * 4. Strategy Pattern - PaymentStrategy (Cash, Card, UPI)
 * 5. Observer Pattern - Operator observes Inventory for low-level alerts
 *
 * OOP Concepts Applied:
 * 1. Encapsulation - Private fields with getters/setters
 * 2. Abstraction - Interfaces (PaymentStrategy, MachineState, Observer)
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
public class CoffeeVendingMachineDemo {

    public static void main(String[] args) {
        System.out.println("╔══════════════════════════════════════════════════════════╗");
        System.out.println("║       COFFEE VENDING MACHINE - SYSTEM DEMONSTRATION      ║");
        System.out.println("╚══════════════════════════════════════════════════════════╝");

        // Get the singleton machine instance
        CoffeeMachine machine = CoffeeMachine.getInstance();

        // Create actors
        User user1 = new User("U001", "Alice");
        User user2 = new User("U002", "Bob");
        Operator operator = new Operator("OP001", "Charlie");

        // Demonstration scenarios
        System.out.println("\n" + "=".repeat(60));
        System.out.println("SCENARIO 1: Normal Coffee Purchase with Cash");
        System.out.println("=".repeat(60));

        user1.viewMenu();
        user1.selectCoffee(2); // Cappuccino
        user1.makePayment(new CashPayment(5.00));

        System.out.println("\n" + "=".repeat(60));
        System.out.println("SCENARIO 2: Coffee Purchase with Card Payment");
        System.out.println("=".repeat(60));

        user2.selectCoffee(3); // Latte
        user2.makePayment(new CardPayment("1234567890123456", "1234"));

        System.out.println("\n" + "=".repeat(60));
        System.out.println("SCENARIO 3: Coffee Purchase with UPI");
        System.out.println("=".repeat(60));

        user1.selectCoffee(5); // Mocha
        user1.makePayment(new UPIPayment("alice@upi"));

        System.out.println("\n" + "=".repeat(60));
        System.out.println("SCENARIO 4: Failed Payment (Insufficient Cash)");
        System.out.println("=".repeat(60));

        user2.selectCoffee(1); // Espresso ($2.50)
        user2.makePayment(new CashPayment(1.00)); // Only $1
        user2.cancelOrder();

        System.out.println("\n" + "=".repeat(60));
        System.out.println("SCENARIO 5: Operator Checks Inventory");
        System.out.println("=".repeat(60));

        operator.checkInventory();
        operator.viewMachineStatus();

        System.out.println("\n" + "=".repeat(60));
        System.out.println("SCENARIO 6: Multiple Orders to Trigger Low Inventory Alert");
        System.out.println("=".repeat(60));

        // Make several orders to deplete inventory
        for (int i = 0; i < 5; i++) {
            user1.selectCoffee(2); // Cappuccino (uses lots of milk)
            user1.makePayment(new CashPayment(10.00));
        }

        System.out.println("\n" + "=".repeat(60));
        System.out.println("SCENARIO 7: Operator Views Alerts and Refills");
        System.out.println("=".repeat(60));

        operator.viewAlerts();
        operator.checkInventory();
        operator.refillAll();
        operator.checkInventory();

        System.out.println("\n" + "=".repeat(60));
        System.out.println("SCENARIO 8: Machine Maintenance");
        System.out.println("=".repeat(60));

        operator.performMaintenance();
        user1.selectCoffee(1); // Should fail - machine under maintenance
        operator.completeMaintenance();
        user1.selectCoffee(1); // Should work now
        user1.makePayment(new CashPayment(5.00));

        System.out.println("\n" + "=".repeat(60));
        System.out.println("DEMONSTRATION COMPLETE");
        System.out.println("=".repeat(60));

        // Interactive mode
        runInteractiveMode(machine, operator);
    }

    private static void runInteractiveMode(CoffeeMachine machine, Operator operator) {
        Scanner scanner = new Scanner(System.in);

        System.out.println("\n\nWould you like to try the interactive mode? (y/n): ");
        String choice = scanner.nextLine().trim().toLowerCase();

        if (!choice.equals("y")) {
            System.out.println("Thank you for using the Coffee Vending Machine!");
            return;
        }

        User interactiveUser = new User("U003", "Guest");
        boolean running = true;

        while (running) {
            System.out.println("\n========== MAIN MENU ==========");
            System.out.println("1. View Coffee Menu");
            System.out.println("2. Order Coffee");
            System.out.println("3. View Machine Status");
            System.out.println("4. Operator: Check Inventory");
            System.out.println("5. Operator: Refill All");
            System.out.println("6. Exit");
            System.out.println("================================");
            System.out.print("Enter choice: ");

            try {
                int menuChoice = Integer.parseInt(scanner.nextLine().trim());

                switch (menuChoice) {
                    case 1:
                        machine.displayMenu();
                        break;

                    case 2:
                        orderCoffeeInteractive(scanner, interactiveUser);
                        break;

                    case 3:
                        machine.displayStatus();
                        break;

                    case 4:
                        operator.checkInventory();
                        break;

                    case 5:
                        operator.refillAll();
                        break;

                    case 6:
                        running = false;
                        System.out.println("Thank you for using the Coffee Vending Machine!");
                        break;

                    default:
                        System.out.println("Invalid choice. Please try again.");
                }
            } catch (NumberFormatException e) {
                System.out.println("Please enter a valid number.");
            }
        }
        scanner.close();
    }

    private static void orderCoffeeInteractive(Scanner scanner, User user) {
        CoffeeMachine.getInstance().displayMenu();
        System.out.print("Select coffee (1-5): ");

        try {
            int coffeeChoice = Integer.parseInt(scanner.nextLine().trim());
            user.selectCoffee(coffeeChoice);

            System.out.println("\nPayment Methods:");
            System.out.println("1. Cash");
            System.out.println("2. Card");
            System.out.println("3. UPI");
            System.out.print("Select payment method: ");

            int paymentChoice = Integer.parseInt(scanner.nextLine().trim());
            PaymentStrategy payment = null;

            switch (paymentChoice) {
                case 1:
                    System.out.print("Enter cash amount: $");
                    double amount = Double.parseDouble(scanner.nextLine().trim());
                    payment = new CashPayment(amount);
                    break;

                case 2:
                    System.out.print("Enter card number (16 digits): ");
                    String cardNum = scanner.nextLine().trim();
                    System.out.print("Enter PIN (4 digits): ");
                    String pin = scanner.nextLine().trim();
                    payment = new CardPayment(cardNum, pin);
                    break;

                case 3:
                    System.out.print("Enter UPI ID: ");
                    String upiId = scanner.nextLine().trim();
                    payment = new UPIPayment(upiId);
                    break;

                default:
                    System.out.println("Invalid payment method. Order cancelled.");
                    user.cancelOrder();
                    return;
            }

            user.makePayment(payment);

        } catch (NumberFormatException e) {
            System.out.println("Invalid input. Order cancelled.");
            user.cancelOrder();
        }
    }
}
