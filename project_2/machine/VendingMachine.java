package machine;

import dao.InventoryDAO;
import model.Coffee;
import payment.Payment;

public class VendingMachine {

    private InventoryDAO inventoryDAO;

    public VendingMachine() {
        inventoryDAO = new InventoryDAO();
    }

    public void prepare(Coffee coffee, Payment payment) {

        try {
            // 1️⃣ Check inventory from database
            if (!inventoryDAO.hasEnough(
                    coffee.getMilk(),
                    coffee.getCoffee(),
                    coffee.getSugar())) {
                System.out.println("❌ Insufficient ingredients in inventory");
                return;
            }

            // 2️⃣ Payment
            if (!payment.pay(coffee.getPrice())) {
                System.out.println("❌ Payment failed");
                return;
            }

            // 3️⃣ Realistic coffee preparation steps
            System.out.println("\nPreparing " + coffee.getName() + "...");

            System.out.println("Grinding coffee beans...");
            pause();

            System.out.println("Tamping coffee powder...");
            pause();

            System.out.println("Heating water to 92–96°C...");
            pause();

            System.out.println("Extracting espresso shot...");
            pause();

            // Milk-based drinks
            if (coffee.getMilk() > 0) {
                System.out.println("Heating milk...");
                pause();

                System.out.println("Frothing milk...");
                pause();

                System.out.println("Mixing espresso with milk...");
                pause();
            }

            // Sugar step
            if (coffee.getSugar() > 0) {
                System.out.println("Adding sugar...");
                pause();
            }

            // 4️⃣ Update inventory in database
            inventoryDAO.use(
                    coffee.getMilk(),
                    coffee.getCoffee(),
                    coffee.getSugar());

            // 5️⃣ Final output
            System.out.println(coffee.getName() + " is ready ☕");
            System.out.println("Enjoy your drink\n");

        } catch (Exception e) {
            System.out.println("❌ Error while preparing coffee");
            e.printStackTrace();
        }
    }

    // Small delay to simulate machine operation
    private void pause() {
        try {
            Thread.sleep(800); // 0.8 second delay
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}

