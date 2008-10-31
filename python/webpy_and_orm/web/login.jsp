<%@ page language="java" contentType="text/html;charset=utf-8" %>
<jsp:useBean id="db" scope="application" class="sales.DatabaseHelp"/>
<%
    String error = null;
    session.setAttribute("user_name", null);
    session.setAttribute("user_id", null);
    if(request.getMethod().toUpperCase().equals("POST")){
        String user = request.getParameter("name");
        String password = request.getParameter("password");
        int user_id = db.login(user, password);
        if(user_id > 0){
            session.setAttribute("user_id", user_id + "");
            session.setAttribute("user_name", user);
            response.sendRedirect("/index.jsp");
        }else  {
            if(user_id == -1){
                error = "密码错误!";
            }else if(user_id == -2){
                error = "用户不存在!";
            }
        }
    }
    if(request.getParameter("logout") != null){
        response.sendRedirect("/index.jsp");
    }
%>
<%@include file="/common_header.jsp" %>
<form method='POST' action="?">
<table>
    <tr>
        <td colspan='2'>推销员登录</td>
    </tr>
    <%
        if(error != null){
    %>
    <tr>
        <td colspan='2'>错误信息:<%=error %></td>
    </tr>
    <%} %>
    <tr>
        <td>账号*:</td>
        <td><input type='text' name='name' size='20'/></td>
    </tr>
    <tr>
        <td>密码*:</td>
        <td><input type='password' name='password' size='20'/></td>
    </tr>
    <tr>
        <td></td>
        <td><input type='submit' value='登录'/>
        &nbsp;<a href="/reg.jsp">注册</a>
        </td>
    </tr>               
</table>
</form>
<%@include file="/common_footer.jsp" %>

