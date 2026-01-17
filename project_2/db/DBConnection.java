package db;

import java.sql.Connection;
import java.sql.DriverManager;

public class DBConnection {

    private static final String URL =
        "jdbc:mysql://localhost:3306/coffee_vending?useSSL=false&serverTimezone=UTC";
    private static final String USER = "coffeeuser";
    private static final String PASSWORD = "Coffee@123"; 

    public static Connection getConnection() {
        try {
            return DriverManager.getConnection(URL, USER, PASSWORD);
        } catch (Exception e) {
            System.out.println("Database connection failed:");
            e.printStackTrace();   // 🔥 THIS WILL SHOW THE REAL ERROR
            return null;
        }
    }
}

