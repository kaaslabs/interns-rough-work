// Factory Pattern - Creates coffee objects without exposing creation logic
public class CoffeeFactory {

    public enum CoffeeType {
        ESPRESSO, CAPPUCCINO, LATTE, AMERICANO, MOCHA
    }

    // Factory Method - Polymorphism (OOP)
    public static Coffee createCoffee(CoffeeType type) {
        switch (type) {
            case ESPRESSO:
                return new Espresso();
            case CAPPUCCINO:
                return new Cappuccino();
            case LATTE:
                return new Latte();
            case AMERICANO:
                return new Americano();
            case MOCHA:
                return new Mocha();
            default:
                throw new IllegalArgumentException("Unknown coffee type: " + type);
        }
    }

    public static Coffee createCoffee(int choice) {
        CoffeeType[] types = CoffeeType.values();
        if (choice >= 0 && choice < types.length) {
            return createCoffee(types[choice]);
        }
        throw new IllegalArgumentException("Invalid coffee choice: " + choice);
    }

    public static void displayMenu() {
        System.out.println("\n========== COFFEE MENU ==========");
        CoffeeType[] types = CoffeeType.values();
        for (int i = 0; i < types.length; i++) {
            Coffee coffee = createCoffee(types[i]);
            System.out.printf("%d. %s%n", i + 1, coffee);
        }
        System.out.println("==================================");
    }
}
