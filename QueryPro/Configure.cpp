#include <cstdlib>

#include "Configure.h"

using namespace std;

Configure::Configure(const char * name) {
	Process(name);
}

void Configure::Process(const char * name) {
	ifstream in(name);

	if(!in.is_open()) {
		cerr<<"Error Conf File!\n";
		exit(1);
	}

	char buf[1025];

	while(!in.eof()) {
		in.getline(buf,1024);
		if(buf[0]!='#') Insert(buf);
	}

	in.close();
}

void Configure::Insert(const char * buf) {
	char name[1025],val[1025];
	sscanf(buf,"%s %s",name,val);
	rem.insert(pair<string,string>((string)name,(string)val));
}

const char * Configure::getString(const char * str) {
	map<string,string>::iterator iter=rem.find(string(str));

	if(iter==rem.end()) return NULL;
	else return iter->second.c_str();
}

int Configure::getInt(const char * str) {
	const char *buf=getString(str);
	int ret;
	if(buf==NULL) return 0;
	sscanf(buf,"%d",&ret);
	return ret;
}
