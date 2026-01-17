package roles;

import machine.VendingMachine;
import model.Coffee;
import payment.CashPayment;
import java.util.Scanner;

public class User {

    public void start(VendingMachine machine) {

        Scanner sc = new Scanner(System.in);

        Coffee latte = new Coffee("Latte", 80, 200, 20, 10);
        Coffee espresso = new Coffee("Espresso", 70, 0, 30, 5);

        System.out.println("1. Latte");
        System.out.println("2. Espresso");
        int ch = sc.nextInt();

        Coffee selected = (ch == 1) ? latte : espresso;
        machine.prepare(selected, new CashPayment());
    }
}

