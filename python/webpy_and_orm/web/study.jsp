<%@ page language="java" contentType="text/html;charset=utf-8" %>
<%@include file="/common_header.jsp" %>
<%
	String error = null;
%>
<table>
    <tr>
        <td colspan='2'>羊皮卷的实践</td>
    </tr>
    <%
        if(error != null){
    %>
    <tr>
        <td colspan='2'>错误信息:<%=error %></td>
    </tr>
    <%} %>
    <tr>
        <td>今天阅读羊皮卷的提示:</td>
        <td>
我不再为的昨日的成绩自吹自擂。将要做的 日常行为与之相

比已经完成的定会更好。我要不断改进

自己的仪态和风度。我要展示自己独一无二

的个性。
        </td>
    </tr>
    <tr>
        <td>今天阅读羊皮卷的次数:</td>
        <td><select name="times">
				<option value="1">1</option>
				<option value="2">2</option>
				<option value="3">3</option>
			</select>
		</td>
    </tr>
    <tr>
        <td>今天我的表现</td>
        <td>
			<select name="status">
                <option value="1">没有记住羊皮卷的忠告</option>
                <option value="2">记住了羊皮卷--但行动不一致</option>
				<option value="3">大部分做到了羊皮卷上所说的</option>
                <option value="4">做到了羊皮卷上所说的一切</option>
            </select>
		</td>
    </tr>
    <tr>
        <td>今天工作记录</td>
        <td>
			<textarea name='work' style="width:290pt;height:90pt">
			</textarea>
        </td>
    </tr>
    <tr>
        <td>今天的感悟：</td>
        <td>
            <textarea name='feel' style="width:290pt;height:90pt">
            </textarea>
        </td>
    </tr>
    <tr>
        <td></td>
        <td><input type='submit' value='保存我的羊皮卷记录'/>
        </td>
    </tr>               
</table>
<table>
    <tr>
        <td colspan='2'>羊皮卷的羊皮卷记录</td>
    </tr>
    <tr>
        <td>星期一<br/>
			阅读次数:3<br/>
			实践得分:3<br/>
			总分:3
		</td>
        <td>
			<div class='title'>工作记录</div>
			<div class='context'>很累，很累，很累，很累，很累，很累，</div>
			<div class='title'>羊皮卷的感悟</div>
			<div class='context'>很累，很累，很累，很累，很累，很累，</div>
		</td>
    </tr>
    <tr>
        <td>星期一<br/>
            阅读次数:3<br/>
            实践得分:3<br/>
            总分:3
        </td>
        <td>
            <div class='title'>工作记录</div>
            <div class='context'>很累，很累，很累，很累，很累，很累，</div>
            <div class='title'>羊皮卷的感悟</div>
            <div class='context'>很累，很累，很累，很累，很累，很累，</div>
        </td>
    </tr>
    <tr>
        <td>星期一<br/>
            阅读次数:3<br/>
            实践得分:3<br/>
            总分:3
        </td>
        <td>
            <div class='title'>工作记录</div>
            <div class='context'>很累，很累，很累，很累，很累，很累，</div>
            <div class='title'>羊皮卷的感悟</div>
            <div class='context'>很累，很累，很累，很累，很累，很累，</div>
        </td>
    </tr>
    <tr>
        <td>星期一<br/>
            阅读次数:3<br/>
            实践得分:3<br/>
            总分:3
        </td>
        <td>
            <div class='title'>工作记录</div>
            <div class='context'>很累，很累，很累，很累，很累，很累，</div>
            <div class='title'>羊皮卷的感悟</div>
            <div class='context'>很累，很累，很累，很累，很累，很累，</div>
        </td>
    </tr>
</table>

<%@include file="/common_footer.jsp" %>
