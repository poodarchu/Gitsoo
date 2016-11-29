#ifndef INVERTTABLE_H_
#define INVERTTABLE_H_

#include"invertNode.h"
#include"CHashAlgo.h"
class invertTable{
public:
	vector<string> textToToken(int docId,string str);		//针对一篇文章分词
	vector<vector<invertNode>> docToNode(vector<int> docId,vector<string> str);
	vector<string> getToken();								//获取token
	vector<string> split(const string &s, const string &seperator);
private:
	vector<string> token;									//存放倒排表的token
	vector<vector<invertNode>> invertList;					//存放 每一个token对应vector的vector依据二者下标相等建立一一对应关系
	vector<vector<invertNode>> invertListVec;

};
#endif