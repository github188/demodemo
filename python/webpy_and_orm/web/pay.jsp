<%@ page language="java" contentType="text/html;charset=utf-8" %>
<%@include file="/common_header.jsp" %>

<%@ page import="java.util.*"%>
<%@ page import="java.text.*"%>
<jsp:useBean id="MD5" scope="request" class="beartool.MD5"/>
<%

	String v_mid,key,v_url,v_oid,v_amount,v_moneytype,v_md5info;  

    v_mid = "20452979";                                   
    v_url = "http://www.greatestsalesman.cn/pay_ok.jsp";     
                                                          
	key = "myww028.comtest";                                       
 
	Date currTime = new Date();
	SimpleDateFormat sf = new SimpleDateFormat("yyyyMMdd-"+v_mid+"-#hhmmss",Locale.US);
	v_oid=sf.format(currTime);                      
                                        
    v_amount=request.getParameter("t");    
    if (v_amount == null || v_amount.equals("")){
    	v_amount = "100";
    }                           
	v_moneytype ="CNY";                              
	v_md5info = "";                                  

	String text = v_amount+v_moneytype+v_oid+v_mid+v_url+key; 
	v_md5info = MD5.getMD5ofStr(text);                        
	
%>


<div>
<div>&lt;羊皮卷实践的摘录&gt;:</div>
<p>
....<br/>

我不是希腊神话中的先知，但是我敢打赌，尝试了这本书中的计划，一年内你的收入增加一倍或者两倍的机率是七十五比一。<br/>

“但是我和别人不一样！”你提醒我。<br/>

真的不一样吗？新年伊始时，你订的计划有几样完成了？你说过要减肥、戒烟、戒酒……结果呢？<br/>

你要自欺欺人到何时呢？<br/>

....<br/>

从阅读别人成功的事迹中得到快感，就好像有人通过阅读色情书籍得到快感一样。遗憾的是，他们从来不能身临其境，
而只是在想象中参与别人的生活和行为，他们像看客一样，只看不做。<br/>
....
</p>
</div>

<div>
	羊皮卷实践的内容很简单， 只要一点就是坚持。 如果你已经开始想放弃了，那很遗憾。你和世上的95%的平凡人一样。
	在本网站也有 200 位推销员选择了放弃。<br/><br/>
	
	为了让你能真正的坚持, 你需要为实践课程付100RMB的实践费. 而且一旦开始就不能中止或退出. 直到完成50周, 350天的实践课程.
	这100RMB为了让你体会到坚持的重要性. 如果你再次放弃了, 这100元就白花了. 如果你坚持完成了50周的实践. 你每天只花了4毛钱就
	养成了一个成功最重要的习惯.<br/><br/>
	
	不要再从别人成功故事中体会快感, 现在开始行动把.<br/><br/>
	<form method="POST" action="http://pay3.chinabank.com.cn/PayGate">
		<input type="hidden" name="v_mid" value="<%=v_mid %>"/>
		<input type="hidden" name="v_oid" value="<%=v_oid %>"/>
		<input type="hidden" name="v_amount" value="<%=v_amount %>"/>
		<input type="hidden" name="v_url" value="<%=v_url %>"/>
		<input type="hidden" name="v_md5info" value="<%=v_md5info %>"/>
		<input type="hidden" name="v_moneytype" value="CNY"/>
	点击 <input type="submit" value='支付<%=v_amount %>元' /> 开始你的成功之旅.
	</form>
</div>
			
<%@include file="/common_footer.jsp" %>
