// Singleton Pattern - Ensures only one instance of CoffeeMachine exists
// Demonstrates Encapsulation (OOP)
public class CoffeeMachine {
    // Singleton instance with thread-safe lazy initialization
    private static volatile CoffeeMachine instance;

    private MachineState currentState;
    private Inventory inventory;
    private Coffee selectedCoffee;
    private CoffeeFactory.CoffeeType selectedCoffeeType;
    private boolean isOperational;

    // Private constructor for Singleton
    private CoffeeMachine() {
        this.inventory = new Inventory();
        this.currentState = new IdleState();
        this.isOperational = true;
    }

    // Thread-safe Singleton getInstance with double-checked locking
    public static CoffeeMachine getInstance() {
        if (instance == null) {
            synchronized (CoffeeMachine.class) {
                if (instance == null) {
                    instance = new CoffeeMachine();
                }
            }
        }
        return instance;
    }

    // State Pattern - Delegate actions to current state
    public void selectCoffee(int choice) {
        if (!isOperational) {
            System.out.println("Machine is under maintenance. Please try later.");
            return;
        }
        currentState.selectCoffee(this, choice);
    }

    public void makePayment(PaymentStrategy payment) {
        if (!isOperational) {
            System.out.println("Machine is under maintenance. Please try later.");
            return;
        }
        currentState.insertPayment(this, payment);
    }

    public void cancelOrder() {
        currentState.cancel(this);
    }

    public void displayMenu() {
        CoffeeFactory.displayMenu();
    }

    public void displayStatus() {
        System.out.println("\n===== MACHINE STATUS =====");
        System.out.println("State: " + currentState.getStateName());
        System.out.println("Operational: " + (isOperational ? "Yes" : "No (Maintenance)"));
        if (selectedCoffee != null) {
            System.out.println("Selected: " + selectedCoffee.getName());
        }
        System.out.println("==========================");
    }

    // Getters and Setters
    public MachineState getCurrentState() { return currentState; }
    public void setState(MachineState state) { this.currentState = state; }

    public Inventory getInventory() { return inventory; }

    public Coffee getSelectedCoffee() { return selectedCoffee; }
    public void setSelectedCoffee(Coffee coffee) { this.selectedCoffee = coffee; }

    public CoffeeFactory.CoffeeType getSelectedCoffeeType() { return selectedCoffeeType; }
    public void setSelectedCoffeeType(CoffeeFactory.CoffeeType type) { this.selectedCoffeeType = type; }

    public boolean isOperational() { return isOperational; }
    public void setOperational(boolean operational) { this.isOperational = operational; }

    // Observer registration helper
    public void registerObserver(InventoryObserver observer) {
        inventory.addObserver(observer);
    }

    public void removeObserver(InventoryObserver observer) {
        inventory.removeObserver(observer);
    }
}
