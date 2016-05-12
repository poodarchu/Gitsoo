#ifndef __GITSOO_H__
#define __GITSOO_H__

#include <utlist.h>
#include <uthash.h>
#include <utarray.h>
#include <mysql.h>
// 2-gram
#define N_GRAM 2

//倒排列表=文档编号+位置
typedef struct _postings_list {
  int document_id;             // 文档编号
  UT_array *positions;         // 位置信息的数组
  int positions_count;         // 位置信息的条数
  struct _postings_list *next; // 指向下一个倒排列表的指针
} postings_list;

// 倒排索引=词元编号为键+以倒排列表为值的关联数组
typedef struct {
  int token_id;                 // 词元编号
  postings_list *postings_list; // 指向包含该词元的倒排列表的指针
  int docs_count;               // 出现过该词元的文档数
  int positions_count;          // 该词元在所有文档中的出现次数之和
  UT_hash_handle hh;            // 用于将该结构体转化为哈希表
} inverted_index_hash, inverted_index_value;

// 压缩方法
typedef enum {
  compress_none
} compress_method;

// 应用程序的全局配置环境
typedef struct _gitsoo_env {
  const char *db_path;

  int token_len;                  //词元的长度。N-gram中N的取值
  int enable_phrase_search;       // 是否进行短语检索
  compress_method compress;       // 压缩倒排列表等数据的方法

  inverted_index_hash *ii_buffer; //用于更新倒排索引的缓冲区
  int ii_buffer_count;            //用于更新倒排索引的缓冲区中的文档数
  int ii_buffer_update_threshold; //缓冲区中文档数的阈值
  int indexed_count;              //建立了索引的文档数


  mysql *db; // mysql的实例
  mysql_stmt *...;   //all related stmts， 供数据库操作使用

} gitsoo_env;

static int init_env(gitsoo_env *env, int ii_buffer_update_threshold, int enable_phrase_search, const char *db_path);

static void fin_env(gitsoo_env *env);

#ifndef FALSE
#define FALSE 0
#define TRUE 1
#endif

#define DEFAULT_II_BUFFER_UPDATE_THRESHOLD 2048

#endif
