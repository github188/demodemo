<%@ page language="java" contentType="text/html;charset=utf-8" %>
<%@include file="/common_header.jsp" %>
<jsp:useBean id="db" scope="application" class="sales.DatabaseHelp"/>
<jsp:useBean id="MD5" scope="request" class="beartool.MD5"/>
<%
String key = "myww028.comtest";  

       String v_oid = request.getParameter("v_oid");       
     String v_pmode = request.getParameter("v_pmode");      
   String v_pstatus = request.getParameter("v_pstatus");    
   String v_pstring = request.getParameter("v_pstring");    
    String v_amount = request.getParameter("v_amount");     
 String v_moneytype = request.getParameter("v_moneytype"); 
    String v_md5str = request.getParameter("v_md5str");     


String text = v_oid+v_pstatus+v_amount+v_moneytype+key;
String v_md5 = MD5.getMD5ofStr(text).toUpperCase();
        
if (v_md5str.equals(v_md5))
{
    if ("30".equals(v_pstatus))
    {
        out.print("支付失败");
    }else if ("20".equals(v_pstatus)){
        session.setAttribute("pay_ok", v_oid);
        String user_id = (String)session.getAttribute("user_id");
        if(user_id != null){
            db.setField("sales_user", "user_id", user_id, "order_id", v_oid)
        }
        %>
            支付成功!<br/>
        <%
            if(user_id != null){
        %>
            <a href="/pay.jsp">开始实践成功的羊皮卷</a>
        <% }else { %>
            <a href="/login.jsp">登录</a>才能后开始实践成功的羊皮卷
        <% } %>
        <%
    }
}else{
    out.print("校验码未通过，不是银行传递回来的参数");
}
%>

<%@include file="/common_footer.jsp" %>
