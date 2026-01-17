package dao;

import db.DBConnection;
import java.sql.*;

public class OperatorDAO {

    public boolean login(String username, String password) {

        String sql =
            "SELECT * FROM operators WHERE username=? AND password=?";

        try (Connection con = DBConnection.getConnection();
             PreparedStatement ps = con.prepareStatement(sql)) {

            ps.setString(1, username);
            ps.setString(2, password);

            ResultSet rs = ps.executeQuery();
            return rs.next();

        } catch (Exception e) {
            return false;
        }
    }
}

