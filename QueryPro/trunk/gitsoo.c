

/* 判断从地址t开始的、长度为l的二进制序列是否与字符串c一致 */
#define MEMSTRCMP(t,l,c) (l == (sizeof(c) - 1) && !memcmp(t, c, l))

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
