<%@ page language="java" contentType="text/html;charset=utf-8" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<title>世界上最伟大的推销员--羊皮卷的实践</title>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link rel="stylesheet" type="text/css" href="/statics/main.css"/>
</head>
<body>
<div class="wrapper">
	<div class='header'>
		<h2 style="float:left;">世界上最伟大的推销员</h2>
		<div class="head-menu">
			<a href="/index.jsp">首页</a>
			<a href="/story_list.jsp">羊皮卷的故事</a>
			<a href="/pay.jsp">羊皮卷的实践</a>
			<a href="#">羊皮卷的启示</a>
			<a href="#">成功的美德</a>
			<a href="#">评论摘录</a>
		</div>
		<form method='POST' action="/login.jsp">
        <div class="head-menu">
        <%
            String user = (String)session.getAttribute("user_name");
            if(user != null){
        %>
            欢迎<span><%=user %></span>
          <a href="/login.jsp?logout=1">退出</a>
        <% }else{ %>
            用户名：<input class="login" size="8" type="text" name="name" />
            密码:<input  class="login"  size="8" type="password" name="password" />
            <input type='submit' value='登录'/>
            <a href="/reg.jsp">注册</a>
       <% } %>
            <a href="#">我的羊皮卷</a>
        </div>		
        </form>
	</div>
	<hr style="clear:both;height:1px;" />
	<div class='content' style="clear:both">
		
