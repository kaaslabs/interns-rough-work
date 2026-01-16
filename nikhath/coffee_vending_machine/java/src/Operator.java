import java.util.ArrayList;
import java.util.List;

// Operator Actor - Manages and maintains the Coffee Machine
// Implements Observer Pattern to receive inventory alerts
// Demonstrates Encapsulation and Inheritance concepts (OOP)
public class Operator implements InventoryObserver {
    private String operatorId;
    private String name;
    private CoffeeMachine machine;
    private List<String> alerts;

    public Operator(String operatorId, String name) {
        this.operatorId = operatorId;
        this.name = name;
        this.machine = CoffeeMachine.getInstance();
        this.alerts = new ArrayList<>();

        // Register this operator as an observer for inventory updates
        machine.registerObserver(this);
    }

    // Observer Pattern - Receive notifications about low inventory
    @Override
    public void update(String ingredient, int currentLevel, int threshold) {
        String alert = String.format("[ALERT] Low inventory: %s at %d (threshold: %d)",
                                      ingredient, currentLevel, threshold);
        alerts.add(alert);
        System.out.println("\n*** OPERATOR NOTIFICATION ***");
        System.out.println("Operator " + name + " received alert:");
        System.out.println(alert);
        System.out.println("*****************************\n");
    }

    // Maintenance operations
    public void checkInventory() {
        System.out.println("\nOperator " + name + " checking inventory...");
        machine.getInventory().displayInventory();
    }

    public void refillIngredient(String ingredient, int amount) {
        System.out.println("\nOperator " + name + " refilling " + ingredient + "...");
        machine.getInventory().refillIngredient(ingredient, amount);
    }

    public void performMaintenance() {
        System.out.println("\nOperator " + name + " performing maintenance...");
        machine.setOperational(false);
        System.out.println("Machine is now in maintenance mode.");
    }

    public void completeMaintenance() {
        System.out.println("\nOperator " + name + " completing maintenance...");
        machine.setOperational(true);
        System.out.println("Machine is back online.");
    }

    public void viewMachineStatus() {
        System.out.println("\nOperator " + name + " checking machine status...");
        machine.displayStatus();
    }

    public void viewAlerts() {
        System.out.println("\n===== OPERATOR ALERTS =====");
        if (alerts.isEmpty()) {
            System.out.println("No alerts.");
        } else {
            for (String alert : alerts) {
                System.out.println(alert);
            }
        }
        System.out.println("===========================");
    }

    public void clearAlerts() {
        alerts.clear();
        System.out.println("Alerts cleared.");
    }

    // Refill all ingredients to maximum
    public void refillAll() {
        System.out.println("\nOperator " + name + " refilling all ingredients...");
        refillIngredient("Coffee Beans", 400);
        refillIngredient("Water", 1500);
        refillIngredient("Milk", 800);
        refillIngredient("Chocolate", 150);
        refillIngredient("Cups", 40);
        System.out.println("All ingredients refilled.");
    }

    // Unregister from notifications
    public void unregister() {
        machine.removeObserver(this);
    }

    // Getters
    public String getOperatorId() { return operatorId; }
    public String getName() { return name; }
    public List<String> getAlerts() { return new ArrayList<>(alerts); }
}
