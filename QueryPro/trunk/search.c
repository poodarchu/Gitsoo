#include <math.h>
#include <stdio.h>

#include "util.h"
#include "token.h"

//将inverted_index_hash和inverted_index_value和postings_list也用于检索
typedef inverted_index_hash query_token_hash;
typedef inverted_index_value query_token_value:
typedef postings_list token_positions_list;

typedef struct {
  token_positions_list *documents;   //文档编号的序列
  token_positions_list *current;　　　//当前文档的编号
} doc_search_cursor;

typedef struct {
  const UT_array *positions;    //位置信息
  int base;                     //词元在查询中的位置
  int current;　　　　　　　　　　 //当前的位置信息
} phrase_searcher_cursor;

typedef struct {
  int document_id;　　　　　　　　//检索出的文档编号
  double score;
  UT_hash_handle hh;　　　　　　　//用于将该结构提转化为哈希表
} search_results;

/**
 *　检索主函数
 */
void search(gitsoo_env *env, const char *query) {
  int query32_len;
  UTF32Char *query32;

  if(!utf8toutf32(query, strlen(query), &query32, &query32_len)) {
    search_results *results = NULL;

    if(query32_len < env->token_len) {
      print_error("too short query.");
    } else {
      query_token_hash *query_tokens = NULL;
      spilt_query_to_tokens(
        env, query32, query32_len, env->token_len, &query_tokens
      );
      search_docs(env, &results, query_tokens);

      free(query32);
    }
  }
}

void search_docs(gitsoo_env, search_results **results, query_token_hash *tokens) {

}
