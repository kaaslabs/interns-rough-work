package payment;

import java.util.Scanner;

public class CashPayment implements Payment {

    public boolean pay(int amount) {
        Scanner sc = new Scanner(System.in);
        System.out.print("Insert ₹" + amount + ": ");
        int paid = sc.nextInt();
        return paid >= amount;
    }
}

