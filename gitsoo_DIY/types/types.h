/*
   Include the struct.
*/

#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <string>
using std::string;
using std::vector;

// For main and query.
struct res_item {
    string title;
    string author;
    long long timestamp;
    string abstract;
    string url;
};


// For main and query.
struct response {
	int number;
	int OutputOffset;
	int MaxOutputs;
    vector<res_item> res_items;
};


#endif
