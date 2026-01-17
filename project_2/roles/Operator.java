package roles;

import dao.InventoryDAO;
import dao.OperatorDAO;
import java.util.Scanner;

public class Operator {

    public void start() {

        Scanner sc = new Scanner(System.in);
        OperatorDAO dao = new OperatorDAO();
        InventoryDAO inv = new InventoryDAO();

        System.out.print("Username: ");
        String u = sc.next();
        System.out.print("Password: ");
        String p = sc.next();

        if (!dao.login(u, p)) {
            System.out.println("Invalid login");
            return;
        }

        int ch;
        do {
            System.out.println("\n1.View Inventory");
            System.out.println("2.Refill");
            System.out.println("3.Exit");
            ch = sc.nextInt();

            try {
                if (ch == 1) inv.show();
                if (ch == 2) {
                    System.out.print("Milk: ");
                    int m = sc.nextInt();
                    System.out.print("Coffee: ");
                    int c = sc.nextInt();
                    System.out.print("Sugar: ");
                    int s = sc.nextInt();
                    inv.refill(m, c, s);
                }
            } catch (Exception e) {
                System.out.println("Error");
            }
        } while (ch != 3);
    }
}

