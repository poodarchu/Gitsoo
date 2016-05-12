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

//检索主函数
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

//tokens: 从查询中提取出的词元信息
void search_docs(gitsoo_env *env, search_results **results, query_token_hash *tokens)
{
  int n_tokens;
  doc_search_cursor *cursors;

  if (!tokens) { return; }

  //按照文档频率的升序对tokens排序
  HASH_SORT(tokens, query_token_value_docs_count_desc_sort);

  //初始化
  n_tokens = HASH_COUNT(tokens);
  if (n_tokens &&
      (cursors = (doc_search_cursor *)calloc(
                   sizeof(doc_search_cursor), n_tokens))) {
    int i;
    doc_search_cursor *cur;
    query_token_value *token;
    for (i = 0, token = tokens; token; i++, token = token->hh.next) {
      if (!token->token_id) {
        //当前的token在构建索引的过程中从未出现过
        goto exit;
      }
      if (fetch_postings(env, token->token_id,
                         &cursors[i].documents, NULL)) {
        print_error("decode postings error!: %d\n", token->token_id);
        goto exit;
      }
      if (!cursors[i].documents) {
        //虽然当前的token存在，但是由于更新或删除导致其倒排列表为空
        goto exit;
      }
      cursors[i].current = cursors[i].documents;
    }
    while (cursors[0].current) {
      int doc_id, next_doc_id = 0;
      //将拥有文档最少的词元称作A
      doc_id = cursors[0].current->document_id;
      // 对于除词元A以外的词元，不断获取其下一个document_id，直到当前的document_id不小于词元A的document_id为止
      for (cur = cursors + 1, i = 1; i < n_tokens; cur++, i++) {
        while (cur->current && cur->current->document_id < doc_id) {
          cur->current = cur->current->next;
        }
        if (!cur->current) { goto exit; }
        // 对于除词元A以外的词元，如果其document_id不等于词元A的document_id,那么就将这个document_id设定为next_doc_id
        if (cur->current->document_id != doc_id) {
          next_doc_id = cur->current->document_id;
          break;
        }
      }
      if (next_doc_id > 0) {
        // 不断获取A的下一个document_id，直到其当前的document_id不小于next_doc_id为止
        while (cursors[0].current
               && cursors[0].current->document_id < next_doc_id) {
          cursors[0].current = cursors[0].current->next;
        }
      } else {
        int phrase_count = -1;
        if (env->enable_phrase_search) {
          phrase_count = search_phrase(tokens, cursors);
        }
        if (phrase_count) {
          double score = calc_tf_idf(tokens, cursors, n_tokens,
                                     env->indexed_count);
          add_search_result(results, doc_id, score);
        }
        cursors[0].current = cursors[0].current->next;
      }
    }
exit:
    for (i = 0; i < n_tokens; i++) {
      if (cursors[i].documents) {
        free_token_positions_list(cursors[i].documents);
      }
    }
    free(cursors);
  }
  free_inverted_index(tokens);

  HASH_SORT(*results, search_results_score_desc_sort);
}

//进行短语检索
//query_tokens 从查询中提取出的词元信息
//doc_cursors 用于检索文档的游标的集合
//return 检索出的短语数
static int search_phrase(const query_token_hash *query_tokens, doc_search_cursor *doc_cursors) {
  int n_positions = 0;
  const query_token_value *qt;
  phrase_search_cursor *cursors;

  /* 获取查询中词元的总数 */
  for (qt = query_tokens; qt; qt = qt->hh.next) {
    n_positions += qt->positions_count;
  }

  if ((cursors = (phrase_search_cursor *)malloc(sizeof(
                   phrase_search_cursor) * n_positions))) {
    int i, phrase_count = 0;
    phrase_search_cursor *cur;
    /* 初始化游标 */
    for (i = 0, cur = cursors, qt = query_tokens; qt;
         i++, qt = qt->hh.next) {
      int *pos = NULL;
      while ((pos = (int *)utarray_next(qt->postings_list->positions,
                                        pos))) {
        cur->base = *pos;
        cur->positions = doc_cursors[i].current->positions;
        cur->current = (int *)utarray_front(cur->positions);
        cur++;
      }
    }
    /* 检索短语 */
    while (cursors[0].current) {
      int rel_position, next_rel_position;
      rel_position = next_rel_position = *cursors[0].current -
                                         cursors[0].base;
      /* 对于除词元A以外的词元，不断地向后读取其出现位置，直到其偏移量不小于词元A的偏移量为止 */
      for (cur = cursors + 1, i = 1; i < n_positions; cur++, i++) {
        for (; cur->current
             && (*cur->current - cur->base) < rel_position;
             cur->current = (int *)utarray_next(cur->positions, cur->current)) {}
        if (!cur->current) { goto exit; }

        /* 对于除词元A以外的词元，若其偏移量不等于A的偏移量，就退出循环 */
        if ((*cur->current - cur->base) != rel_position) {
          next_rel_position = *cur->current - cur->base;
          break;
        }
      }
      if (next_rel_position > rel_position) {
        /* 不断向后读取，直到词元A的偏移量不小于next_rel_position为止 */
        while (cursors[0].current &&
               (*cursors[0].current - cursors[0].base) < next_rel_position) {
          cursors[0].current = (int *)utarray_next(
                                 cursors[0].positions, cursors[0].current);
        }
      } else {
        /* 找到了短语 */
        phrase_count++;
        cursors->current = (int *)utarray_next(
                             cursors->positions, cursors->current);
      }
    }
exit:
    free(cursors);
    return phrase_count;
  }
  return 0;
}

//释放词元的出现位置列表
void free_token_positions_list(token_positions_list *list) {
  free_postings_list((postings_list *)list);
}

//将文档添加到检索结果中
//results 指向检索结果的指针
//document_id 要天剑的文档编号
//score 得分
static void add_search_result(search_results **results, const int document_id, const double score) {
  search_results *r;
  if (*results) {
    HASH_FIND_INT(*results, &document_id, r);
  } else {
    r = NULL;
  }
  if (!r) {
    if ((r = malloc(sizeof(search_results)))) {
      r->document_id = document_id;
      r->score = 0;
      HASH_ADD_INT(*results, document_id, r);
    }
  }
  if (r) {
    r->score += score;
  }
}

//根据得分比较两条检索结果
//返回得分的大小关系
static int search_results_score_desc_sort(search_results *a, search_results *b) {
  return (b->score > a->score) ? 1 : (b->score < a->score) ? -1 : 0;
}

//比较出现过词元A和词元B的文档数
static int query_token_value_docs_count_desc_sort(query_token_value *a, query_token_value *b) {
  return b->docs_count - a->docs_count;
}

//使用TF-IDF计算得分
//query_tokens: 查询
//doc_cursors: 用于文档检索的游标集合
//查询中的词元数
//建立过索引的文档总数
static double calc_tf_idf(
  const query_token_hash *query_tokens,
  doc_search_cursor *doc_cursors, const int n_query_tokens,
  const int indexed_count)
{
  int i;
  const query_token_value *qt;
  doc_search_cursor *dcur;
  double score = 0;
  for (qt = query_tokens, dcur = doc_cursors, i = 0;
       i < n_query_tokens;
       qt = qt->hh.next, dcur++, i++) {
    double idf = log2((double)indexed_count / qt->docs_count);
    score += (double)dcur->current->positions_count * idf;
  }
  return score;
}



//text: 查询字符串
//text_len: 查询字符串的长度
//n: N-gram
//query_tokens: 按词元编号存储位置信息序列的关联数组，若传入的是指向NULL的指针，则新建一个关联数组
int split_query_to_tokens(gitsoo_env *env,
                      const UTF32Char *text,
                      const unsigned int text_len,
                      const int n, query_token_hash **query_tokens) {
  return text_to_postings_lists(env,
                                0, /* 将document_id设为0 */
                                text, text_len, n,
                                (inverted_index_hash **)query_tokens);
}

void print_search_results(gitsoo_env *env, search_results *results) {
  int num_search_results;

  if (!results) { return; }
  num_search_results = HASH_COUNT(results);

  while (results) {
    int title_len;
    const char *title;
    search_results *r;

    r = results;
    HASH_DEL(results, r);
    db_get_document_title(env, r->document_id, &title, &title_len);
    printf("document_id: %d title: %.*s score: %lf\n",
           r->document_id, title_len, title, r->score);
    free(r);
  }

  printf("Total %u documents are found!\n", num_search_results);
}
