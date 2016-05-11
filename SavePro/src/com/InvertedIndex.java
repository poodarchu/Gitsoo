package com;
import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.ObjectInputStream;
import java.sql.*;
import java.util.ArrayList;

public class InvertedIndex{
	
	public static boolean saveURL(String url, String date, String[]title) //保存第二张表
	{
		System.out.println("The method saveURL is execute!");
		Connection conn = null;
		PreparedStatement stmt = null;
		try{
			conn = JDBCcon.con("root", "may1024", "testfun"); //连接数据库
			conn.setAutoCommit(false);
			stmt = conn.prepareStatement("insert into url_db(url, date, title) values (?,?,?)");
			stmt.setString(1, url);
			stmt.setString(2, date);
			stmt.setObject(3, title);
			stmt.executeUpdate();
			conn.commit();
		}catch(SQLException e){
			e.printStackTrace();
			return false;
		}finally{
			if(stmt != null){
				try {
					stmt.close();
				} catch (SQLException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}else if(conn != null){
				try {
					conn.close();
				} catch (SQLException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
		return true;
	}
	

	public static int getID(String url) //获取第二张表的ID
	{
		//System.out.println(" the method save in InvertedIndex is execute!");
		Statement stmt = null;
		ResultSet rst = null;
		Connection conn = null;
		Integer ID = null;
		try{
			conn = JDBCcon.con("root", "may1024", "testfun");
			stmt = conn.createStatement();
			String sql = "select id from url_db where URL = '" + url + "';";
			rst = stmt.executeQuery(sql);
			while (rst.next()){
			ID = rst.getInt(1);
			}
		}catch(Exception e){
			e.printStackTrace();
					return ID ;
		}finally
		{
			if(rst != null)
			{
				try {
					rst.close();
				} catch (SQLException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
			else if(stmt != null)
			{
				try{
					stmt.close();
				}catch(SQLException e){
					e.printStackTrace();
				}
			}
			else if(conn != null){
				try{
					conn.close();
				}catch(SQLException e){
					e.printStackTrace();
				}
			}
		}
		return ID;
	}
	
	public static void Insert(String path) throws IOException // 插入数据到数据库
	{
		File fin = new File(path);
		BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream(fin),"UTF-8"));
		String line = null;
		int textlong = Integer.parseInt( (line = br.readLine()).trim());
		while((line = br.readLine())!= null)
		{
			String []arg = line.split(",", 6);
			String URL = arg[0];
			String date = arg[1];
			String titlelength = arg[2];
			String datalength = arg[3];
			String[] title = arg[4].split(" ", Integer.parseInt(titlelength));
			String []letter = arg[5].split(" ", Integer.parseInt(datalength));
			saveURL(URL,date, title); //插入到第二张表
			for(int i = 0; i < Integer.parseInt(datalength); i++)
			{
				invertedIndex(URL,letter[i]); //实现插入数据到第一张表
			}
		}
		br.close();
	}
	
	public static void invertedIndex(String url, String letter) // 实现倒排
	{
		int ID = getID(url);
		solve_letter(letter,ID);
	}
	
	public static boolean solve_letter(String letter, int id) // 实现对第二张表插入到数据库的数据的处理
	{
		System.out.println("method solve_letter is execute");
		Statement stmt = null;
		ResultSet rst = null;
		try{
			Connection conn = JDBCcon.con("root", "may1024", "testfun");
			conn.setAutoCommit(false);
			stmt = conn.createStatement();
			String sql = "Select count(0) count, id idbyte from data_db where letter = '" + letter + "'"; //判断letter在不在数据库中
			rst = stmt.executeQuery(sql);
			while(rst.next())
			{
				int count = rst.getInt("count");
				System.out.println("solve: count = " + count );
				if(count > 0) //letter在数据库中
				{
					Blob inblob = (Blob)rst.getBlob("idbyte");
					InputStream is = inblob.getBinaryStream();
					BufferedInputStream input = new BufferedInputStream(is);
					byte[]buff = new byte[(int) inblob.length()];
					while(-1 != (input.read(buff,0,buff.length)));
					ObjectInputStream in = new ObjectInputStream(new ByteArrayInputStream(buff));
					ArrayList<Integer> ID= (ArrayList<Integer>) in.readObject();
					System.out.println(ID);
					if(ID.contains(id) == false) //数据库中的ID里面没有要插入的id
					{
						ID.add(id);
						PreparedStatement pstmt = conn.prepareStatement("update data_db set id = ? where letter = ?");
						pstmt.setObject(1, ID);
						pstmt.setString(2, letter);
						pstmt.executeUpdate();
						conn.commit();
					}
				}
				else{ //letter不在原来的数据库中
					ArrayList<Integer> ID = new ArrayList<Integer>();
					ID.add(id);
					System.out.println("solve: id = " + id + "  ID = " + ID);
					PreparedStatement pstmt = conn.prepareStatement("insert into data_db(letter, id) values (?,?)");
					pstmt.setString(1, letter);
					pstmt.setObject(2, ID);
					pstmt.executeUpdate();
					conn.commit();
				}
		}
		}catch (Exception ex){
			ex.printStackTrace();
			return false;
		}
		return true;
	}
}