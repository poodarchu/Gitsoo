#include "../Tokenizer/include/Application.hpp"

using namespace CppJieba;


const char* const DICT_PATH = "../Tokenizer/dict/jieba.dict.utf8";
const char* const HMM_PATH = "../Tokenizer/dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "../Tokenizer/dict/user.dict.utf8";
const char* const IDF_PATH = "../Tokenizer/dict/idf.utf8";
const char* const STOP_WORD_PATH = "../Tokenizer/dict/stop_words.utf8";

int main(int argc, char** argv) {
  CppJieba::Application app(DICT_PATH,
                            HMM_PATH,
                            USER_DICT_PATH,
                            IDF_PATH,
                            STOP_WORD_PATH);
  vector<string> words;
  string result;
  string s = "我是拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上CEO，走上人生巅峰。";

  cout << "\n" << "[demo] METHOD_QUERY" << "\n"; // colorful
    app.cut(s, words, METHOD_QUERY);
    cout << join(words.begin(), words.end(), "/") << endl;

  cout << "\n" << "[demo] TAGGING" << "\n"; // colorful
  vector<pair<string, string> > tagres;
  app.tag(s, tagres);
  cout << s << endl;
  cout << tagres << endl;;

  cout << "\n" << "[demo] KEYWORD" << "\n"; // colorful
  vector<pair<string, double> > keywordres;
  app.extract(s, keywordres, 5);
  cout << s << endl;
  cout << keywordres << endl;

  return EXIT_SUCCESS;
}
