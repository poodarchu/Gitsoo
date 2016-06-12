/*
   The main pro for Gitsoo.
   Use the lib jsoncpp and mongoose.

   Complie command:
   g++ main.cpp jsoncpp/jsoncpp.cpp mongoose/mongoose.c -o main -lpthread
*/

#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>

#include "mongoose/mongoose.h"
#include "jsoncpp/json/json.h"

#include "../../types/types.h"

using namespace std;

// Just for test.
response query(string a,res_item * & b) {
	cout<<"Query:"<<a<<endl;

	response ret;

	ret.number=2;
	ret.OutputOffset=123;
	ret.MaxOutputs=321;

	b=new res_item [2];

	b[0].title=string("title 1");
	b[0].author="author 1";
	b[0].timestamp=201606091234LL;
	b[0].abstract="aaaaaa 1";
	b[0].url="url 1";

	b[1].title="title 2";
	b[1].author="author 2";
	b[1].timestamp=201606094321LL;
	b[1].abstract="aaaaaa 2";
	b[1].url="url 2";

	return ret;
}

// Change to Json and then change to string.
string ansToJson(response res,res_item * ans) {
	Json::Value array,ret,temp;

	ret["jsonrpc"]="2.0";
	ret["number"]=res.number;
	ret["OutputOffset"]=res.OutputOffset;
	ret["MaxOutputs"]=res.MaxOutputs;
	ret["id"]=1;

	for(int i=0;i<res.number;++i) {
		temp.clear();
		temp["title"]=ans[i].title;
		temp["author"]=ans[i].author.data();
		temp["timestamp"]=ans[i].timestamp;
		temp["abstract"]=ans[i].abstract.data();
		temp["url"]=ans[i].url.data();

		array.append(temp);
	}

	ret["result"]=array;

	return ret.toStyledString();
}

// Execute this function to get the answer of search query.
string getData(const char * que_data) {
	Json::Reader reader;
	Json::Value que_json;

	if(reader.parse(que_data,que_json) && \
			0==strcmp(que_json["method"].asString().data(),"query")) {

		res_item * ans;
		response res=query(string(que_json["params"].asString()),ans);		// !!!

		if(res.number==-1) return string("Server Error");
		string ret=ansToJson(res,ans);

		// Should I delete this one ?
		// delete [] ans;

		return ret;
	}
	else {
		// Wrong for the format of query string.
		return string("Wrong Query");
	}
}

// Execute this function when a http request get.
static int index_html(mg_connection * conn) {
	mg_send_header(conn,"Content-Type","application/json");

	string data=getData(conn->content);
	mg_send_data(conn,data.data(),data.length());

	return 0;
}

int main() {
	mg_server * server=mg_create_server(NULL);
	mg_set_option(server,"listening_port","80");
	mg_add_uri_handler(server,"/",index_html);

	cout<<"Start\n";

	while(1) {
		mg_poll_server(server,1000);
	}

	mg_destroy_server(&server);

	return 0;
}
