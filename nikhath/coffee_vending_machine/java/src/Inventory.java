import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

// Inventory - Implements Observer Pattern (Subject)
// Demonstrates Encapsulation (OOP)
public class Inventory implements InventorySubject {
    private Map<String, Integer> ingredients;
    private Map<String, Integer> thresholds;
    private List<InventoryObserver> observers;

    // Ingredient requirements for each coffee
    private static final Map<CoffeeFactory.CoffeeType, Map<String, Integer>> RECIPES;

    static {
        RECIPES = new HashMap<>();

        Map<String, Integer> espressoRecipe = new HashMap<>();
        espressoRecipe.put("Coffee Beans", 20);
        espressoRecipe.put("Water", 30);
        RECIPES.put(CoffeeFactory.CoffeeType.ESPRESSO, espressoRecipe);

        Map<String, Integer> cappuccinoRecipe = new HashMap<>();
        cappuccinoRecipe.put("Coffee Beans", 20);
        cappuccinoRecipe.put("Water", 30);
        cappuccinoRecipe.put("Milk", 100);
        RECIPES.put(CoffeeFactory.CoffeeType.CAPPUCCINO, cappuccinoRecipe);

        Map<String, Integer> latteRecipe = new HashMap<>();
        latteRecipe.put("Coffee Beans", 20);
        latteRecipe.put("Water", 30);
        latteRecipe.put("Milk", 150);
        RECIPES.put(CoffeeFactory.CoffeeType.LATTE, latteRecipe);

        Map<String, Integer> americanoRecipe = new HashMap<>();
        americanoRecipe.put("Coffee Beans", 20);
        americanoRecipe.put("Water", 150);
        RECIPES.put(CoffeeFactory.CoffeeType.AMERICANO, americanoRecipe);

        Map<String, Integer> mochaRecipe = new HashMap<>();
        mochaRecipe.put("Coffee Beans", 20);
        mochaRecipe.put("Water", 30);
        mochaRecipe.put("Milk", 100);
        mochaRecipe.put("Chocolate", 30);
        RECIPES.put(CoffeeFactory.CoffeeType.MOCHA, mochaRecipe);
    }

    public Inventory() {
        ingredients = new HashMap<>();
        thresholds = new HashMap<>();
        observers = new ArrayList<>();
        initializeInventory();
    }

    private void initializeInventory() {
        // Initialize with default quantities (in grams/ml)
        ingredients.put("Coffee Beans", 500);
        ingredients.put("Water", 2000);
        ingredients.put("Milk", 1000);
        ingredients.put("Chocolate", 200);
        ingredients.put("Cups", 50);

        // Set low-level thresholds
        thresholds.put("Coffee Beans", 100);
        thresholds.put("Water", 500);
        thresholds.put("Milk", 200);
        thresholds.put("Chocolate", 50);
        thresholds.put("Cups", 10);
    }

    public boolean checkAvailability(CoffeeFactory.CoffeeType coffeeType) {
        Map<String, Integer> recipe = RECIPES.get(coffeeType);
        if (recipe == null) return false;

        for (Map.Entry<String, Integer> entry : recipe.entrySet()) {
            Integer available = ingredients.get(entry.getKey());
            if (available == null || available < entry.getValue()) {
                return false;
            }
        }
        return ingredients.get("Cups") > 0;
    }

    public void consumeIngredients(CoffeeFactory.CoffeeType coffeeType) {
        Map<String, Integer> recipe = RECIPES.get(coffeeType);
        if (recipe == null) return;

        for (Map.Entry<String, Integer> entry : recipe.entrySet()) {
            String ingredient = entry.getKey();
            int required = entry.getValue();
            int current = ingredients.get(ingredient) - required;
            ingredients.put(ingredient, current);

            // Check if below threshold and notify observers
            if (current <= thresholds.get(ingredient)) {
                notifyObservers(ingredient, current, thresholds.get(ingredient));
            }
        }
        // Consume a cup
        int cups = ingredients.get("Cups") - 1;
        ingredients.put("Cups", cups);
        if (cups <= thresholds.get("Cups")) {
            notifyObservers("Cups", cups, thresholds.get("Cups"));
        }
    }

    public void refillIngredient(String ingredient, int amount) {
        if (ingredients.containsKey(ingredient)) {
            int current = ingredients.get(ingredient);
            ingredients.put(ingredient, current + amount);
            System.out.printf("Refilled %s: %d + %d = %d%n",
                              ingredient, current, amount, current + amount);
        } else {
            System.out.println("Unknown ingredient: " + ingredient);
        }
    }

    public void displayInventory() {
        System.out.println("\n========== INVENTORY STATUS ==========");
        for (Map.Entry<String, Integer> entry : ingredients.entrySet()) {
            String status = entry.getValue() <= thresholds.get(entry.getKey()) ? " [LOW]" : "";
            System.out.printf("%-15s: %d%s%n", entry.getKey(), entry.getValue(), status);
        }
        System.out.println("=======================================");
    }

    // Observer Pattern methods
    @Override
    public void addObserver(InventoryObserver observer) {
        observers.add(observer);
    }

    @Override
    public void removeObserver(InventoryObserver observer) {
        observers.remove(observer);
    }

    @Override
    public void notifyObservers(String ingredient, int currentLevel, int threshold) {
        for (InventoryObserver observer : observers) {
            observer.update(ingredient, currentLevel, threshold);
        }
    }

    public Map<String, Integer> getIngredients() {
        return new HashMap<>(ingredients);
    }
}
