// Abstract Product - Part of Factory Pattern
// Demonstrates Abstraction (OOP)
public abstract class Coffee {
    protected String name;
    protected double price;
    protected int preparationTime; // in seconds

    public abstract void prepare();

    public String getName() { return name; }
    public double getPrice() { return price; }
    public int getPreparationTime() { return preparationTime; }

    @Override
    public String toString() {
        return name + " - $" + price;
    }
}

// Concrete Products - Inheritance (OOP)
class Espresso extends Coffee {
    public Espresso() {
        this.name = "Espresso";
        this.price = 2.50;
        this.preparationTime = 30;
    }

    @Override
    public void prepare() {
        System.out.println("Preparing Espresso: Grinding beans, extracting shot...");
    }
}

class Cappuccino extends Coffee {
    public Cappuccino() {
        this.name = "Cappuccino";
        this.price = 3.50;
        this.preparationTime = 45;
    }

    @Override
    public void prepare() {
        System.out.println("Preparing Cappuccino: Extracting espresso, steaming milk, adding foam...");
    }
}

class Latte extends Coffee {
    public Latte() {
        this.name = "Latte";
        this.price = 3.00;
        this.preparationTime = 40;
    }

    @Override
    public void prepare() {
        System.out.println("Preparing Latte: Extracting espresso, adding steamed milk...");
    }
}

class Americano extends Coffee {
    public Americano() {
        this.name = "Americano";
        this.price = 2.00;
        this.preparationTime = 25;
    }

    @Override
    public void prepare() {
        System.out.println("Preparing Americano: Extracting espresso, adding hot water...");
    }
}

class Mocha extends Coffee {
    public Mocha() {
        this.name = "Mocha";
        this.price = 4.00;
        this.preparationTime = 50;
    }

    @Override
    public void prepare() {
        System.out.println("Preparing Mocha: Adding chocolate, extracting espresso, steaming milk...");
    }
}
