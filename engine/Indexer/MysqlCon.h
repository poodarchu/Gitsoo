#pragma once
#ifndef MYSQLCON_H_
#define MYSQLCON_H_
#include<stdio.h>
#include<winsock.h>
#include"mysql.h"
#include<Windows.h>
//#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "../libs/libmysql.lib")


class MysqlCon
{
public:
	MYSQL mysql;					//mysql 连接
	MYSQL_FIELD *fd;				// 字段列数组
	char field[32][32];				// 存字段名二维数组
	MYSQL_RES *res;					// 返回行的一个查询结果集
	MYSQL_ROW column;				//一个行数据的类型安全(type-safe)的表示，表示数据行的列  
	//char query[150];
	MysqlCon() {}
	MysqlCon(char *service, char *USER, char *passwd, char *dbname, int port);
	~MysqlCon();
	bool ConnectDB(char *service, char *USER, char *passwd, char *dbname, int port);//连接数据库
	void FreeConnect();				//释放数据库
	MYSQL_RES* QueryDB(char* query);//执行访问语句，将取出的数据放入结果集中
	bool DealData(char* query);		// 处理数据
	bool ChangeUserDB(char* user, char *passwd, char*dbname);						// 改变用户和数据库
	bool DealBinary(char * sql, unsigned int n);									// 处理二进制的
};



#endif

