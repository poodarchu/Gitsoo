/**
 * 将文档添加到数据库中，建立倒排索引
 * env 存储着应用程序运行环境的结构体
 * title 文档标题，为NULL时将会清空缓冲区
 * body 文档正文
 */
static void add_document(gitsoo_env *env, const char *title, const char *body) {
  if (title && body) {
    UTF32Char *body32;
    int body32_len, document_id;
    unsigned int title_size, body_size;

    title_size = strlen(title);
    body_size = strlen(body);

    /* 将文档存储到数据库中并获取该文档对应的文档编号 */
    db_add_document(env, title, title_size, body, body_size);
    document_id = db_get_document_id(env, title, title_size);

    /* 转换文档正文的字符编码 */
    if (!utf8toutf32(body, body_size, &body32, &body32_len)) {
      /* 为文档创建倒排列表 */
      text_to_postings_lists(env, document_id, body32, body32_len,
                             env->token_len, &env->ii_buffer);
      env->ii_buffer_count++;
      free(body32);
    }
    env->indexed_count++;
    print_error("count:%d title: %s", env->indexed_count, title);
  }

  /* 存储在缓冲区中的文档数量达到了指定的阈值时，更新存储器上的倒排索引 */
  if (env->ii_buffer &&
      (env->ii_buffer_count > env->ii_buffer_update_threshold || !title)) {
    inverted_index_hash *p;

    print_time_diff();

    /* 更新所有词元对应的倒排项 */
    for (p = env->ii_buffer; p != NULL; p = p->hh.next) {
      update_postings(env, p);
    }
    free_inverted_index(env->ii_buffer);
    print_error("index flushed.");
    env->ii_buffer = NULL;
    env->ii_buffer_count = 0;

    print_time_diff();
  }
}

/**
 * 设定应用程序的运行环境
 * ii_buffer_update_threshold ,Flush倒排索引缓冲区的阈值
 * enable_phrase_search 是否启用短语检索
 * db_path 数据库的路径
 * return 0 成功
 */
static int init_env(gitsoo_env *env, int ii_buffer_update_threshold, int enable_phrase_search, const char *db_path) {
  int rc;
  memset(env, 0, sizeof(gitsoo_env));
  rc = init_database(env, db_path);
  if (!rc) {
    env->token_len = N_GRAM;
    env->ii_buffer_update_threshold = ii_buffer_update_threshold;
    env->enable_phrase_search = enable_phrase_search;
  }
  return rc;
}

/**
 * 释放应用程序的运行环境
 */
static void fin_env(gitsoo_env *env) {
  fin_database(env);
}

/* 判断从地址t开始的、长度为l的二进制序列是否与字符串c一致 */
#define MEMSTRCMP(t,l,c) (l == (sizeof(c) - 1) && !memcmp(t, c, l))


int main(int argc, char *argv[]) {

}
