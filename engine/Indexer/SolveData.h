#pragma once
#ifndef SOLVEDATA_H_
#define SOLVEDATA_H_
#include"MysqlCon.h"
#include"invertNode.h"

class SolveData
{
public:
	SolveData(){}
	SolveData(char *service, char *USER, char *passwd, char *dbname, int port)
	{
		_mc.ConnectDB(service, USER, passwd, dbname, port);
	}
	// 连接数据库
	void ConnectMysql(char *service, char *USER, char *passwd, char *dbname, int port)
	{
		_mc.ConnectDB(service, USER, passwd, dbname, port);
	}
	~SolveData() { }
	vector<invertNode>  selFromInvtable(char* token);//从倒排表取出token的item
	void GetData_Solve();//从数据库将update=1数据取出，并且将update置0，处理data
	int tableCount(char *table);// 数表里面有多少行
	void SaveData(char* token, vector<invertNode> item);	//保存数据到数据库
	vector<int> getDocID() { return _docID; }
	vector<string> getText() { return _text; }
protected:
	vector<invertNode> charToVector(char *p, int n);// 把char*变回vector 
	// 序列化存入数据库 
	void inserthelp(const vector<invertNode>& a1, char* token);
	void updatehelp(const vector<invertNode>& a1, char* token);
	vector<invertNode> Merge50(const vector<invertNode> &a, const vector<invertNode>& b);// 归并算法，找出词频为前50个的词 

	void Comparison(char*token, const vector<invertNode>&a);// 比对,更新
	void buildTo50(vector<invertNode>&item);// 如果新保存vector里面的东西不满50个，则（docID = 0，tokenFreq = 0，index = 0）填满
private:
	MysqlCon _mc;
	MYSQL_RES *buffres;
	vector<int> _docID;
	vector<string> _text;
	//MYSQL_ROW buffcol;
};


#endif
