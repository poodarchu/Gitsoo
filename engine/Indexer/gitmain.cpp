#include"SolveData.h"
#include"invertTable.h"



int main()
{
	SolveData solve;
	char* c;
	invertTable inverttable;

	//solve.ConnectMysql("localhost", "root", "may1024","gitsoo",3306);
	solve.ConnectMysql("localhost", "root", "root","gitsoo",3306);
	
	solve.GetData_Solve();
	
	vector<vector<invertNode>> invertTableRes = inverttable.docToNode(solve.getDocID(),solve.getText());
	
	for ( int i=0; i<(solve.getDocID()).size(); i++ ) {

		vector<string> tokenRes = inverttable.getToken();

		const int len = (tokenRes[i]).length();
		c = new char[len+1];
		strcpy_s(c,strlen(c) + strlen(tokenRes[i].c_str()) + 1, tokenRes[i].c_str());
		solve.SaveData(c, invertTableRes[i]);	
	}
	system("pause");
	return 0;
	


}
		