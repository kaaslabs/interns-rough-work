// User Actor - Interacts with the Coffee Machine
// Demonstrates Encapsulation (OOP)
public class User {
    private String userId;
    private String name;
    private CoffeeMachine machine;

    public User(String userId, String name) {
        this.userId = userId;
        this.name = name;
        this.machine = CoffeeMachine.getInstance();
    }

    public void viewMenu() {
        System.out.println("\nUser " + name + " viewing menu...");
        machine.displayMenu();
    }

    public void selectCoffee(int choice) {
        System.out.println("\nUser " + name + " selecting coffee option " + choice + "...");
        machine.selectCoffee(choice);
    }

    public void makePayment(PaymentStrategy payment) {
        System.out.println("\nUser " + name + " making payment via " + payment.getPaymentMethod() + "...");
        machine.makePayment(payment);
    }

    public void cancelOrder() {
        System.out.println("\nUser " + name + " cancelling order...");
        machine.cancelOrder();
    }

    // Convenience method for complete transaction
    public void orderCoffee(int choice, PaymentStrategy payment) {
        viewMenu();
        selectCoffee(choice);
        makePayment(payment);
    }

    // Getters
    public String getUserId() { return userId; }
    public String getName() { return name; }
}
