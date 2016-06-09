#include <string>
#include <vector>

#include "../Tokenizer/include/Application.hpp"
#include "../../types/types.h"

using namespace CppJieba;
using std::string;
using std::vector;

const char* const DICT_PATH = "../Tokenizer/dict/jieba.dict.utf8";
const char* const HMM_PATH = "../Tokenizer/dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "../Tokenizer/dict/user.dict.utf8";
const char* const IDF_PATH = "../Tokenizer/dict/idf.utf8";
const char* const STOP_WORD_PATH = "../Tokenizer/dict/stop_words.utf8";

string
strToUTF8Str(const string query_content) {
	string s = query_content;

	return s;
}

void
search_pages(vector<res_item> &res_items, vector<string> words);

void
printSearchResults(const vector<res_item> res_items);

void
tokenize(string query, vector<string> &words) {
	CppJieba::Application app(DICT_PATH,
                            HMM_PATH,
                            USER_DICT_PATH,
                            IDF_PATH,
                            STOP_WORD_PATH);
  	string result;
  	string s = query;

  	app.cut(s, words, METHOD_QUERY);
  	cout << join(words.begin(), words.end(), "/") << endl;
}

response query(const string query_content, vector<res_item> res_items) {
	response res;

	string s = strToUTF8Str(query_content);
	
	/**
	 *  Tokenizer Part.
	 */
	vector<string> words;
	tokenize(s, words);

	/**
	 *  Use the cut words to query database, store the result in res_items.
	 */
	 // search_pages(res_items, words);

	//debug dump output.
	// printSearchResults(res_items);

	//make res.
	res.number = res_items.size() > 100 ? 100 : res_items.size();
	res.OutputOffset = 5;
	res.MaxOutputs = 100;
	res.res_items = res_items;

	return res;
}

// test demo
int main(int argc, char** argv) {

  vector<res_item> res_items;
  vector<string> words;

  const string s = "我是拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上CEO，走上人生巅峰。";

  query(s, res_items);

  return 0;
}
