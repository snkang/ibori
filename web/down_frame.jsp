<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ page import = "java.net.*, java.util.*, java.sql.*, java.lang.String" %>

<%
	String type = request.getParameter("type");
	if ( type == null ) type = "id_pw";
	String s_url = request.getParameter("s_url");
	String s_cookie = request.getParameter("cookie");

	Cookie[] cookies  = request.getCookies();
//	CookieUtils cu = new CookieUtils();
	
	if( s_cookie != null) {

// 		Cookie cookie = new Cookie("level",level);
// 	  cookie.setMaxAge(-1);
// 	  cookie.setPath("http://"+s_url);

// 	  response.addCookie(cookie);
// 	  response.sendRedirect("http://​localhost:8080/saml/someservices.jsp");
	 }	
%>


<html>
<head>
<title>down_frame</title>
<meta http-equiv="Content-Type" content="text/html; charset=euc-kr">
<style type="text/css">
body {	background-color: #FFFFCC; }
td,th { color: #000000; }
</style>
</head>

<body leftmargin="0" topmargin="0" marginwidth="0" marginheight="0">

<textarea style="height: 100%; width: 100%; font-size: 14px; padding: 12px 20px; margin: 8px 0; box-sizing: border-box; border: 1px solid #555; outline: none; :focue background-color: lightblue;" >
-----------------<Headers>-----------------
<%
Enumeration headerNames = request.getHeaderNames();
while (headerNames.hasMoreElements()) {
	String key = (String) headerNames.nextElement();
	String value = request.getHeader(key);
%>
&nbsp;* <%=key%>&nbsp;:&nbsp;<%=value%>
<%
}
%>			
-----------------<Cookies>-----------------
<%
	if (cookies != null){
		for (int i = 0; i < cookies.length; i++){		%>
&nbsp;* <%=cookies[i].getName()%>&nbsp;:&nbsp;<%=cookies[i].getValue()%>
<%}
}
%>
</textarea>
</body>

</html>