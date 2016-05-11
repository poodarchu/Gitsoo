package com;
import java.sql.*;

public class JDBCcon {

	final static String hostname = "localhost";
	final static String port = "3306";
	public static Connection con(String userid, String passwd, String dbname){
		if(dbname == null||dbname.equals(""))
			return null;
		try{
			String url = "jdbc:mysql://"+hostname+":"+port+"/"+dbname+"?user=" +userid
					+ "&password=" + passwd + "&useUnicode=true&character Encoding = UTF8&useSSL=true";
			Class.forName("com.mysql.jdbc.Driver");
			Connection conn = DriverManager.getConnection(url);
			return conn;
		}catch (Exception e){
			e.printStackTrace();
			return null;
		}
	}
	
}
