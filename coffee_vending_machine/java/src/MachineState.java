// State Pattern - Allows machine to alter behavior when internal state changes
// Demonstrates Abstraction and Polymorphism (OOP)
public interface MachineState {
    void selectCoffee(CoffeeMachine machine, int choice);
    void insertPayment(CoffeeMachine machine, PaymentStrategy payment);
    void dispense(CoffeeMachine machine);
    void cancel(CoffeeMachine machine);
    String getStateName();
}

// Concrete State - Idle State
class IdleState implements MachineState {
    @Override
    public void selectCoffee(CoffeeMachine machine, int choice) {
        try {
            CoffeeFactory.CoffeeType type = CoffeeFactory.CoffeeType.values()[choice - 1];
            if (machine.getInventory().checkAvailability(type)) {
                Coffee coffee = CoffeeFactory.createCoffee(type);
                machine.setSelectedCoffee(coffee);
                machine.setSelectedCoffeeType(type);
                System.out.println("Selected: " + coffee.getName());
                System.out.printf("Price: $%.2f%n", coffee.getPrice());
                machine.setState(new SelectingState());
            } else {
                System.out.println("Sorry, " + type + " is currently unavailable due to low ingredients.");
            }
        } catch (ArrayIndexOutOfBoundsException e) {
            System.out.println("Invalid selection. Please try again.");
        }
    }

    @Override
    public void insertPayment(CoffeeMachine machine, PaymentStrategy payment) {
        System.out.println("Please select a coffee first.");
    }

    @Override
    public void dispense(CoffeeMachine machine) {
        System.out.println("Please select a coffee and make payment first.");
    }

    @Override
    public void cancel(CoffeeMachine machine) {
        System.out.println("Nothing to cancel.");
    }

    @Override
    public String getStateName() {
        return "Idle";
    }
}

// Concrete State - Selecting State (Coffee selected, awaiting payment)
class SelectingState implements MachineState {
    @Override
    public void selectCoffee(CoffeeMachine machine, int choice) {
        System.out.println("Coffee already selected. Please make payment or cancel.");
    }

    @Override
    public void insertPayment(CoffeeMachine machine, PaymentStrategy payment) {
        Coffee coffee = machine.getSelectedCoffee();
        if (payment.pay(coffee.getPrice())) {
            machine.setState(new ProcessingState());
            machine.getCurrentState().dispense(machine);
        } else {
            System.out.println("Payment failed. Please try again or cancel.");
        }
    }

    @Override
    public void dispense(CoffeeMachine machine) {
        System.out.println("Please complete payment first.");
    }

    @Override
    public void cancel(CoffeeMachine machine) {
        System.out.println("Order cancelled.");
        machine.setSelectedCoffee(null);
        machine.setSelectedCoffeeType(null);
        machine.setState(new IdleState());
    }

    @Override
    public String getStateName() {
        return "Selecting";
    }
}

// Concrete State - Processing State
class ProcessingState implements MachineState {
    @Override
    public void selectCoffee(CoffeeMachine machine, int choice) {
        System.out.println("Machine is processing. Please wait.");
    }

    @Override
    public void insertPayment(CoffeeMachine machine, PaymentStrategy payment) {
        System.out.println("Payment already received. Processing order.");
    }

    @Override
    public void dispense(CoffeeMachine machine) {
        Coffee coffee = machine.getSelectedCoffee();
        System.out.println("\nProcessing your order...");
        coffee.prepare();

        // Simulate preparation time
        System.out.printf("Please wait %d seconds...%n", coffee.getPreparationTime());

        // Consume ingredients from inventory
        machine.getInventory().consumeIngredients(machine.getSelectedCoffeeType());

        machine.setState(new DispensingState());
        machine.getCurrentState().dispense(machine);
    }

    @Override
    public void cancel(CoffeeMachine machine) {
        System.out.println("Cannot cancel. Order is being processed.");
    }

    @Override
    public String getStateName() {
        return "Processing";
    }
}

// Concrete State - Dispensing State
class DispensingState implements MachineState {
    @Override
    public void selectCoffee(CoffeeMachine machine, int choice) {
        System.out.println("Please collect your coffee first.");
    }

    @Override
    public void insertPayment(CoffeeMachine machine, PaymentStrategy payment) {
        System.out.println("Please collect your coffee first.");
    }

    @Override
    public void dispense(CoffeeMachine machine) {
        Coffee coffee = machine.getSelectedCoffee();
        System.out.println("\n*** Your " + coffee.getName() + " is ready! ***");
        System.out.println("Please collect your coffee from the dispenser.");
        System.out.println("Thank you for your purchase!\n");

        // Reset machine state
        machine.setSelectedCoffee(null);
        machine.setSelectedCoffeeType(null);
        machine.setState(new IdleState());
    }

    @Override
    public void cancel(CoffeeMachine machine) {
        System.out.println("Cannot cancel. Coffee is being dispensed.");
    }

    @Override
    public String getStateName() {
        return "Dispensing";
    }
}
