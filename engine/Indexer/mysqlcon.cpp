#include"MysqlCon.h"
#include<iostream>
using namespace std;
MysqlCon::MysqlCon(char *service, char *USER, char *passwd, char *dbname, int port)
{
	ConnectDB(service, USER,passwd, dbname, port);
}
MysqlCon::~MysqlCon()
{
	FreeConnect();
}
bool MysqlCon::ConnectDB(char *host,char* USER, char* passwd, char* dbname, int port) //连接数据库
{
	mysql_init(&mysql);
	if (!(mysql_real_connect(&mysql, host, USER, passwd, dbname, port, 0, NULL)))
	{
		printf("Error Connecting to Database:%s\n", mysql_error(&mysql));
		return false;
	}
	else
	{
		printf("Connecting...");
		return true;
	}
}

void MysqlCon::FreeConnect() //释放数据库
{
	mysql_free_result(res);
	mysql_close(&mysql);
}

//数据库操作

MYSQL_RES* MysqlCon::QueryDB(char* query) //执行访问语句，将取出的数据放入结果集中
{
	mysql_query(&mysql, "SET NAMES UTF8");
	if (mysql_query(&mysql, query))
	{
		printf("Query Failed:%s\n", mysql_error(&mysql));
		return NULL;
	}
	else
	{
		if (!(res = mysql_store_result(&mysql)))
		{
			printf("Couldn't get result:%s", mysql_error(&mysql));
			return false;
		}
		else
		{
			return res;
		}
	}
	return NULL;
}

bool MysqlCon::DealData(char* query)  // 处理数据
{
	mysql_query(&mysql, "SET NAMES UTF8" );
	if ((mysql_query(&mysql, query)))
	{
		printf("Deal Failed:%s\n", mysql_error(&mysql));
		return false;
	}
	else return true;
}

bool MysqlCon::DealBinary(char * sql, unsigned int n) // 处理二进制的
{
	mysql_query(&mysql, "SET NAMES UTF8");
	if (mysql_real_query(&mysql, sql, n))
	{
		printf("Failed in dealing binary:%s\n", mysql_error(&mysql));
		return false;
	}
	return true;
}

bool MysqlCon::ChangeUserDB(char* user, char *passwd, char*dbname) // 改变用户和数据库
{
	if (mysql_change_user(&mysql, user, passwd, dbname))
	{
		printf("Failed to change user. Error:%s\n", mysql_error(&mysql));
		return false;
	}
	else
		return true;
}