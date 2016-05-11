/*
   Solve the Query.conf.
*/

#ifndef CONFIGURE_H
#define CONFIGURE_H

#include <string>
#include <cstdio>
#include <map>
#include <iostream>
#include <fstream>

using namespace std;

class Configure {
	private:
		std::map<std::string,std::string> rem;

		void Process(const char *);
		void Insert(const char *);
	public:
		Configure(const char *);

		const char * getString(const char *);
		int getInt(const char *);
};

#endif
