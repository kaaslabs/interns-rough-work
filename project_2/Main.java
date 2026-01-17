import roles.User;
import roles.Operator;
import machine.VendingMachine;
import java.util.Scanner;

public class Main {

    public static void main(String[] args) {

        Scanner sc = new Scanner(System.in);
        VendingMachine machine = new VendingMachine();

        int ch;
        do {
            System.out.println("\n1.User\n2.Operator\n3.Exit");
            ch = sc.nextInt();

            if (ch == 1) new User().start(machine);
            if (ch == 2) new Operator().start();

        } while (ch != 3);
    }
}

