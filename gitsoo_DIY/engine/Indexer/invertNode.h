#ifndef INVERTNODE_H_
#define INVERTNODE_H_

#include"util.h"
#define MAXPOSSIZE 10

class invertNode{
public:
	int docId;				//文档编号
	int tokenFreq;			//文档频率
	vector<int> pos;		//文档出现的位置	

	invertNode()
	{
		docId = 0;
		tokenFreq = 0;
		vector<int>pos(MAXPOSSIZE,0);//对于pos初始化
		
	}
	void createInvertNode(int _docId, int _tokenFreq, int _perPos)//vector做参数  同为vector直接=即可
	{
		docId =  _docId;
		tokenFreq = _tokenFreq;
		pos.push_back(_perPos);
	
	}
	void refNode(int _docId, int _tokenFreq, int _perPos){
		docId =  _docId;
		tokenFreq = _tokenFreq;
		pos.push_back(_perPos);
	
	}


};


#endif