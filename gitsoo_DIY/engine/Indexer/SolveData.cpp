#include"MysqlCon.h"
#include"SolveData.h"
#include<cstring>
#include<stdlib.h>
#include<iostream>
#include<vector>
using namespace std;

//从数据库将update=1数据取出，并且将update置0，处理data
void SolveData:: GetData_Solve()
{
	char * query = "select pageID,text from tbname where update_ = 1";  // 取出update_为1的数据
	//mysql_free_result(_mc.res);
	_mc.res = _mc.QueryDB(query);
	while (_mc.column = mysql_fetch_row(_mc.res))
	{
		int docid = atoi(_mc.column[0]);
		_docID.push_back(docid);
		_text.push_back(_mc.column[1]);
	}

	query = "update tbname set update_ =  1 where update_ = 1";  //取完数据直接置零
	_mc.DealData(query);
}



// 把char*变回vector 
vector<invertNode> SolveData::charToVector(char *p, int n)
{
	invertNode *temp = (invertNode*)p;
	vector<invertNode> q(temp, temp + n);
	return q;
}

// 序列化存入数据库 
void SolveData::inserthelp( const vector<invertNode>& a1, char* token)
{
	//mysql_free_result(_mc.res);
	char sql[300], *end;
	sprintf_s(sql, 300, "insert into invtable(token, item) values ('%s',", token);  /********要改表名。******/
	end = sql + strlen(sql);
	*end++ = '\'';
	end += mysql_real_escape_string(&_mc.mysql, end, (char*)&a1[0], sizeof(a1));
	*end++ = '\'';
	*end++ = ')';
	_mc.DealBinary(sql, (unsigned int)(end - sql));
}

void SolveData::updatehelp(const vector<invertNode>& a1, char* token)
{
	//mysql_free_result(_mc.res);
	char sql[300] = "update invtable set item = '";
	char * end;
	end = sql + strlen(sql);
	end += mysql_real_escape_string(&_mc.mysql, end, (char*)&a1[0], sizeof(a1));
	*end++ = '\'';
	char * p = " where token = '";
	strcat_s(sql, strlen(sql) + strlen(p) + 1, p);
	strcat_s(sql, strlen(sql) + strlen(token) + 1, token);
	end = sql + strlen(sql);
	*end++ = '\'';
	_mc.DealBinary(sql, (unsigned int)(end - sql));
}


// 归并算法，找出词频为前50个的词 /*******记得改类型********/
vector<invertNode> SolveData::Merge50(const vector<invertNode> &a, const vector<invertNode>& b)
{
	vector<invertNode> result;
	result.resize(50);
	vector<invertNode>::iterator ri;
	vector<invertNode>::const_iterator ai, bi;
	for (ai = a.begin(), bi = b.begin(), ri = result.begin(); ri != result.end(); ri++)
	{
		if (ai == a.end() && bi == b.end()) break;
		else if (ai == a.end())
		{
			(*ri) = (*bi);
			bi++;
		}
		else if (bi == b.end())
		{
			(*ri) = (*ai);
			ai++;
		}
		else if ((*ai).tokenFreq > (*bi).tokenFreq) /*记得改类型！！！！*/
		{
			(*ri) = (*ai);
			ai++;
		}
		else if ((*ai).tokenFreq == (*bi).tokenFreq)
		{
			if ((*ai).docId == (*bi).docId)
			{
				(*ri) = (*bi);
				ai++;
				bi++;
			}
			else
			{
				(*ri) = (*bi);
				ri++;
				(*ri) = (*ai);
				ai++;
				bi++;
			}
		}
		else
		{
			(*ri) = (*bi);
			bi++;
		}
	}
	return result;
}

// 数表里面有多少行 /**********记得改类型***********/
int SolveData::tableCount( char *table)
{
	//mysql_free_result(_mc.res);
	char *sql = "select count(0) from ";
	size_t len = strlen(sql) + strlen(table) + 1;
	char* query = (char*)malloc(len);
	sprintf_s(query, len, "%s%s", sql, table);
	_mc.res = _mc.QueryDB(query);
	_mc.column = mysql_fetch_row(_mc.res);
	int count = atoi(_mc.column[0]);
	return count;
}


/***********************这个先不管它暂时没用****************************/
////每次取出一个buffer的结果集,buffer从ni开始，count为表里的长度,返回 buffer到哪里
//int SolveData::buffer(int ni, const int& count)
//{
//	char query[85];
//	const int n = 50;// 记得改
//	ni += n;
//	if (ni > count)
//		ni = count;
//	//sprintf_s(query, 85 ,"select token from invtable where tokenID between %d and %d", ni - n, ni);  //记得改
//	sprintf_s(query, 85, "select token from invtable where tokenID between %d and %d", ni - n, ni);  //记得改
//	buffres = _mc.QueryDB(query);
//	return ni;
//}
//
////比对助手！！！God！！！好烦这东西好烦好烦
//bool SolveData::comparisonhelp(char*token)
//{
//	while (buffcol = mysql_fetch_row(buffres))
//		if (strcmp(buffcol[0], token) == 0)
//			return true;
//	return false;
//
//}
/***********************这个先不管它暂时没用****************************/

// 比对
void SolveData::Comparison(char*token, const vector<invertNode>&a)
{
	char * sql = "select item from invtable where token = '";
	size_t len = strlen(sql) + strlen(token) + 2;
	char* query = (char*)malloc(len);
	sprintf_s(query, len, "%s%s'", sql, token);
//	mysql_free_result(_mc.res);
	_mc.res = _mc.QueryDB(query);
	_mc.column = mysql_fetch_row(_mc.res);
	char *p = _mc.column[0];
	const int n = 50; //个数
	vector<invertNode> temp =	Merge50(charToVector(p, n), a);
	updatehelp(temp, token);
}

// 如果新保存vector里面的东西不满50个，则填满
void SolveData::buildTo50(vector<invertNode>&item)
{
	//if (item.size() >= 50) return;
	if (item.size() >= 10) return;
	//invertNode temp(0, 0, 0);//杰华改成了invertNode temp;
	invertNode temp;
	//for (int i = item.size(); i <= 50; i++)
	for (int i = item.size(); i <= 10; i++)
		item.push_back(temp);
}

void SolveData::SaveData(char* token, vector<invertNode> item)
{
	char query[85];
	//sprintf_s(query, 85 ,"select token from invtable where tokenID between %d and %d", ni - n, ni);  //记得改
	sprintf_s(query, 85, "select item from invtable where token = '%s'",token);  
	buffres = _mc.QueryDB(query);
	if (mysql_affected_rows(&_mc.mysql) == 0)
	{
		buildTo50(item);
		inserthelp(item, token);
	}
	else
	{
		Comparison(token, item);
	}
	
}


vector<invertNode> SolveData::selFromInvtable(char* token)//从倒排表取出token的item
{
	char* sql = "select item from invtable where token = '";  //记得改
	size_t len = strlen(sql) + strlen(token) + 2;
	char* query = (char*)malloc(len);
	sprintf_s(query, len, "%s%s'", sql, token);
	mysql_free_result(_mc.res);
	_mc.res = _mc.QueryDB(query);
	_mc.column = mysql_fetch_row(_mc.res);
	char *p = _mc.column[0];
	const int n = 50; //个数
	return charToVector(p, n);
}
