<%@ page language="java" contentType="text/html;charset=utf-8" %>
<jsp:useBean id="db" scope="request" class="sales.DatabaseHelp"/>
<%
	if(request.getMethod().toUpperCase().equals("POST")){
		String user = request.getParameter("name");
		String password = request.getParameter("password");
		String email = request.getParameter("email");
		int user_id = db.createUser(user, password, email);
		if(user_id > 0){
			session.setAttribute("user_id", user_id + "");
			session.setAttribute("user_name", user);
			response.sendRedirect("/index.jsp");
		}
	}
%>
<%@include file="/common_header.jsp" %>
<form method='POST' action="?">
<table>
	<tr>
		<td colspan='2'>推销员注册:填写注册信息(带*为必填) </td>
	</tr>
    <tr>
        <td>账号*:</td>
		<td><input type='text' name='name' size='20'/></td>
    </tr>
    <tr>
        <td>密码*:</td>
        <td><input type='password' name='password' size='20'/></td>
    </tr>
    <tr>
        <td>确认密码*:</td>
        <td><input type='password' name='password1' size='20'/></td>
    </tr>
    <tr>
        <td>Email*:</td>
        <td><input type='text' name='email' size='20'/></td>
    </tr>
    <tr>
        <td></td>
        <td>继续注册前请阅读以下条款:<br/>
			<textarea name='aa' style="width:290pt;height:90pt">
在这个网站是免费注册！我们希望您遵守以下的网站规定。请仔细阅读下列条款，您必须接受以下的条款才可以注册！

1. 遵守中华人民共和国的各项有关法律法规。
2. 不得在发布任何色情非法，以及危害国家安全的言论。
3. 严禁链接有关政治，色情，宗教，迷信等违法信息。
4. 承担一切因您的行为而直接或间接导致的民事或刑事法律责任。
5. 互相尊重，遵守互联网络道德，严禁互相恶意攻击，漫骂。
6. 管理员及版主有权保留或删除网站中的任意内容。
7. 本网站实践课程是自愿付费参加, 由于课程的特殊性. 无法实现退款, 付费前请谨慎考虑.
8. 本站管理员拥有一切管理权力。</textarea>
			<br />
			<input type="checkbox" name="agree" value="Y">我已阅读并遵守以上条款!			
		</td>
    </tr>
    <tr>
        <td></td>
        <td><input type='submit' value='注册'/>
			<input type='submit' value='重填'/>
		</td>
    </tr>				
</table>
</form>
<%@include file="/common_footer.jsp" %>

