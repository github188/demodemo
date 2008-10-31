<%@ page language="java" contentType="text/html;charset=utf-8" %>
<jsp:useBean id="db" scope="application" class="sales.DatabaseHelp"/>
<%@ page import="java.sql.*;"%>
<%
    Connection c = db.getConnection();
    PreparedStatement prep = c.prepareStatement("SELECT * FROM sales_user");
    ResultSet rs = prep.executeQuery();
    ResultSetMetaData meta = rs.getMetaData();
    int col = meta.getColumnCount();
    out.print("<table>");
    out.print("<tr>");
    for(int i = 1; i <= col; i++){
        out.print("<th>");
        out.print(meta.getColumnName(i));
        out.print("</th>");
    }
    out.print("</tr>");
    while(rs.next()){
        out.print("<tr>");
        for(int i = 1; i <= col; i++){
            out.print("<td>");
            out.print(rs.getString(i));
            out.print("</td>");
        }
        out.print("</tr>");
    }
    out.print("</table>");
    rs.close();
    prep.close();
    c.close();
%>
