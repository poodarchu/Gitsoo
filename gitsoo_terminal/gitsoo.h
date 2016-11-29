//
// Created by P. Chu on 6/10/16.
//

#ifndef GITSOO_OFFLINE_GITSOO_H
#define GITSOO_OFFLINE_GITSOO_H

#include "utlist.h"
#include "uthash.h"
#include "utarray.h"
#include "sqlite3.h"

#define N_GRAM 2

//倒排列表
typedef struct _postings_list {
    int document_id;   //文档编号
    UT_array* positions;  //文档位置信息数组
    int positions_count;    //位置信息的条数
    struct _postings_list* next;   //下一个到道排列表的指针
} postings_list;

//倒排索引
typedef struct {
    int token_id;
    postings_list* postings_list;//指向包含该词元的道排列表的指针
    int docs_count;  //出现过该词元的文档数目
    int positions_count;    //该词元在所有文档中出现的次数之和
    UT_hash_handle hh;     //用于将该结构体转化为hashtable
} inverted_index_hash, inverted_index_value;

typedef enum {
    compress_none,   //不压缩
    compress_golomb  //使用golomb压缩
} compress_method;

typedef struct _gitsoo_env {
    const char* db_path;  //数据库的路径

    int token_len;   //N-GRAM中N的取值
    compress_method compress;
    int enable_phrase_search;  //是否启用短语搜索

    inverted_index_hash* ii_buffer; //用于更新倒排索引的 缓冲区
    int ii_buffer_count;            //docs number in buffer.
    int ii_buffer_update_threshold; //
    int indexed_count;              //indexed docs' number

    //cfg related to sqlite3
    sqlite3* db;

    //sqlite3的准备语句
    sqlite3_stmt* get_document_id_st;
    sqlite3_stmt* get_document_title_st;
    sqlite3_stmt* insert_document_st;
    sqlite3_stmt* update_document_st;
    sqlite3_stmt* get_token_id_st;
    sqlite3_stmt* get_token_st;
    sqlite3_stmt* store_token_st;
    sqlite3_stmt* get_postings_st;
    sqlite3_stmt* update_postings_st;
    sqlite3_stmt* get_settings_st;
    sqlite3_stmt* replace_settings_st;
    sqlite3_stmt* get_document_count_st;
    sqlite3_stmt* begin_st;
    sqlite3_stmt* commit_st;
    sqlite3_stmt* rollback_st;
} gitsoo_env;

#ifndef FALSE
#define FALSE 0
#define TRUE 1
#endif

#define DEFAULT_II_BUFFER_UPDATE_THRESHOLD 8191


#endif //GITSOO_OFFLINE_GITSOO_H
