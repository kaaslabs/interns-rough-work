package dao;

import db.DBConnection;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;

public class InventoryDAO {

    // Check if enough ingredients are available
    public boolean hasEnough(int m, int c, int s) throws Exception {

        String sql = "SELECT milk, coffee, sugar FROM inventory WHERE id = 1";

        try (Connection con = DBConnection.getConnection();
             PreparedStatement ps = con.prepareStatement(sql);
             ResultSet rs = ps.executeQuery()) {

            rs.next();
            return rs.getInt("milk") >= m &&
                   rs.getInt("coffee") >= c &&
                   rs.getInt("sugar") >= s;
        }
    }

    // Reduce inventory after coffee preparation
    public void use(int m, int c, int s) throws Exception {

        String sql =
            "UPDATE inventory " +
            "SET milk = milk - ?, coffee = coffee - ?, sugar = sugar - ? " +
            "WHERE id = 1";

        try (Connection con = DBConnection.getConnection();
             PreparedStatement ps = con.prepareStatement(sql)) {

            ps.setInt(1, m);
            ps.setInt(2, c);
            ps.setInt(3, s);

            int rows = ps.executeUpdate();

       
        }
    }

    // Refill inventory (operator operation)
    public void refill(int m, int c, int s) throws Exception {

        String sql =
            "UPDATE inventory " +
            "SET milk = milk + ?, coffee = coffee + ?, sugar = sugar + ? " +
            "WHERE id = 1";

        try (Connection con = DBConnection.getConnection();
             PreparedStatement ps = con.prepareStatement(sql)) {

            ps.setInt(1, m);
            ps.setInt(2, c);
            ps.setInt(3, s);

            int rows = ps.executeUpdate();

            if (rows == 1) {
                System.out.println("Inventory refilled successfully ✔");
            } else {
                System.out.println("Inventory refill failed ❌");
            }
        }
    }

    // Show current inventory from database
    public void show() throws Exception {

        String sql = "SELECT * FROM inventory WHERE id = 1";

        try (Connection con = DBConnection.getConnection();
             PreparedStatement ps = con.prepareStatement(sql);
             ResultSet rs = ps.executeQuery()) {

            rs.next();
            System.out.println("\n--- INVENTORY STATUS (DB) ---");
            System.out.println("Milk   : " + rs.getInt("milk") + " ml");
            System.out.println("Coffee : " + rs.getInt("coffee") + " g");
            System.out.println("Sugar  : " + rs.getInt("sugar") + " g");
        }
    }
}

