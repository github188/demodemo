<%@ page contentType="text/html; charset=gb2312" language="java"%>
<%@ page import="java.util.*"%>
<%@ page import="java.text.*"%>
<jsp:useBean id="MD5" scope="request" class="beartool.MD5"/>
<%

String v_mid,key,v_url,v_oid,v_amount,v_moneytype,v_md5info;  

    v_mid = "20000400";                                   
    v_url = "http://localhost/chinabank/Receive.jsp";     
                                                          
      key = "test";                                       
                                                           
                                                          
//****************************************

        
    // v_oid=request.getParameter("v_oid");
      if(request.getParameter("v_oid")!=null && !request.getParameter("v_oid").equals(""))  //ÅÐ¶ÏÊÇ·ñÓÐ´«µÝ¶©µ¥ºÅ
      {
          v_oid=request.getParameter("v_oid");
      }
        else                         
      {
          Date currTime = new Date();
          SimpleDateFormat sf = new SimpleDateFormat("yyyyMMdd-"+v_mid+"-hhmmss",Locale.US);
          v_oid=sf.format(currTime);                      
      }                                  
    v_amount=request.getParameter("v_amount");                               
        v_moneytype ="CNY";                              
        v_md5info = "";                                  

String text = v_amount+v_moneytype+v_oid+v_mid+v_url+key; 
v_md5info = MD5.getMD5ofStr(text);                        

String remark1,remark2;

%>
