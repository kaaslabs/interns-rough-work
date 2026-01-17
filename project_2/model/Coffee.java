package model;

public class Coffee {

    private String name;
    private int price;
    private int milk;
    private int coffee;
    private int sugar;

    public Coffee(String name, int price, int milk, int coffee, int sugar) {
        this.name = name;
        this.price = price;
        this.milk = milk;
        this.coffee = coffee;
        this.sugar = sugar;
    }

    public String getName() { return name; }
    public int getPrice() { return price; }
    public int getMilk() { return milk; }
    public int getCoffee() { return coffee; }
    public int getSugar() { return sugar; }
}

