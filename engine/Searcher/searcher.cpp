#include "cppjieba/Jieba.hpp"
#include "cppjieba/KeywordExtractor.hpp"
#include "../deps/limonp/*.hpp"

#include <vector>
#include <string>

using namespace std;

const char* const DICT_PATH = "../dict/jieba.dict.utf8";
const char* const HMM_PATH = "../dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "../dict/user.dict.utf8";
const char* const IDF_PATH = "../dict/idf.utf8";
const char* const STOP_WORD_PATH = "../dict/stop_words.utf8";

string 
strToUTF8Str(const string query_content, string utf8Str);

void 
searchWithOffset(res_item* res, vector<cppjieba::Word> jiebaWords);

void
printSearchResults(res_item* res);

response query(const string query_content,res_item* res) {
	
	string s = strToUTF8Str(query_content);
	string result;
	
	/**
	 *  Tokenizer Part.
	 */
	cppjieba::Jieba jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH);
	
	//store the cut words in words : vector<string>
	vector<string> words;
	//words with loc offset info.
	vector<cppjieba::Word> jiebaWords;
	
	jieba.CutForSearch(s, words);
	//debug dump output.
	result = limonp::Join(words.begin(), words.end(), "/");
	cout << result << endl;
	
	cout << "CutForSearch Word With Offset" << endl;
	jieba.CutForSearch(s, jiebaWords, true);
	cout << jiebaWords << endl;

	
	
	/** 
	 *  Use the cut words to query database, store the result in res.
	 */
	searchWithOffset(res, jiebaWords);
	//debug dump output.
	printSearchResults(res);
	
	return res;
}