/*
   Include the struct.
*/

#ifndef TYPES_H
#define TYPES_H

#include <string>

using std::string;

// For main and query.
struct response {
	int number;
	int OutputOffset;
	int MaxOutputs;
};

// For main and query.
struct res_item {
	string title;
	string author;
	long long timestamp;
	string abstract;
	string url;
};

#endif
