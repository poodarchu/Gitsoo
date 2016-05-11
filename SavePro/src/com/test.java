package com;

import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public class test {

	public static void main(String[]args) throws IOException{
		
		InvertedIndex.Insert("G:\\gitsoo\\data_small.txt");
	}
}
