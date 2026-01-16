import java.util.ArrayList;
import java.util.List;

// Observer Pattern - For notifying operators about inventory levels
// Demonstrates Abstraction (OOP)

// Observer Interface
interface InventoryObserver {
    void update(String ingredient, int currentLevel, int threshold);
}

// Subject Interface
interface InventorySubject {
    void addObserver(InventoryObserver observer);
    void removeObserver(InventoryObserver observer);
    void notifyObservers(String ingredient, int currentLevel, int threshold);
}
