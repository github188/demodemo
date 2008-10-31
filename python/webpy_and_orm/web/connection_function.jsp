<%@ page import="java.sql.*;"%>
<%

    Connection getConnection() throws Exception{
          Class.forName("org.sqlite.JDBC");
          return DriverManager.getConnection("jdbc:sqlite:sales.db");
    }
    
    public int createUser(String name, String password, String email) {
        Connection conn = null;
        PreparedStatement prep = null;
        try {
            conn = getConnection();
            prep = conn.prepareStatement("insert into sales_user" +
                    "(user_id, username, password, email) values (null, ?, ?, ?);");
            conn.setAutoCommit(true);
            
            prep.setString(1, name.trim());
            prep.setString(2, password.trim());
            prep.setString(2, email.trim());
            //conn.commit();
            if(prep.executeUpdate() == 1){
                return getLastUserId(conn);
            }
            return 0;
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (prep != null)
                try {
                    prep.close();
                } catch (SQLException e) {
                    e.printStackTrace();
                }
            if(conn != null)
                try {
                    conn.close();
                } catch (SQLException e) {
                    e.printStackTrace();
                }
        }
        return 0;
    }
    
    public int getLastUserId(Connection con) throws SQLException{
        PreparedStatement prep = con.prepareStatement("SELECT max(user_id) FROM sales_user");
        ResultSet rs = prep.executeQuery();
        rs.next();
        int userId = rs.getInt(1);
        rs.close();
        prep.close();
        return userId;      
    }
    
    public String getField(String table, String pk, int id, String field){
        Connection conn = null;
        PreparedStatement prep = null;
        ResultSet rs = null;
        try {
            conn = getConnection();
            
            prep = conn.prepareStatement("SELECT "+field+" FROM " + table
                            + " where " + pk + "=?");
            prep.setString(1, pk);
            rs = prep.executeQuery();
            if (rs.next()) {
                return rs.getString(1);
            } else {
                return "-2"; // not found user;
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (rs != null)
                try {
                    rs.close();
                } catch (SQLException e1) {
                    e1.printStackTrace();
                }
            if (prep != null)
                try {
                    prep.close();
                } catch (SQLException e) {
                    e.printStackTrace();
                }
            if (conn != null)
                try {
                    conn.close();
                } catch (SQLException e) {
                    e.printStackTrace();
                }
        }
        return "-3"; //error        
    }

    public int setField(String table, String pk, int id, String field, String value){
        Connection conn = null;
        PreparedStatement prep = null;
        try {
            conn = getConnection();
            prep = conn.prepareStatement("update "+table+" set " + field + "=?"
                            + " where " + pk + "=?");
            prep.setString(1, value);
            prep.setInt(2, id);
            return prep.executeUpdate();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (prep != null)
                try {
                    prep.close();
                } catch (SQLException e) {
                    e.printStackTrace();
                }
            if (conn != null)
                try {
                    conn.close();
                } catch (SQLException e) {
                    e.printStackTrace();
                }
        }
        return -3; //error      
    }   
    
    public int login(String user, String password){
        Connection conn = null;
        PreparedStatement prep = null;
        try {
            prep = conn.prepareStatement("SELECT user_id, password FROM sales_user"
                            + " where username = ? or email= ?");
            prep.setString(1, user.trim());
            prep.setString(2, user.trim());

            ResultSet rs = prep.executeQuery();
            if (rs.next()) {
                String org_password = rs.getString(2);
                if (org_password.equals(password)) {
                    return rs.getInt(1);
                } else {
                    return -1; // error password;
                }
            } else {
                return -2; // not found user;
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (prep != null)
                try {
                    prep.close();
                } catch (SQLException e) {
                    e.printStackTrace();
                }
            if (conn != null)
                try {
                    conn.close();
                } catch (SQLException e) {
                    e.printStackTrace();
                }
        }
        return -3; //error
    }
    
%>