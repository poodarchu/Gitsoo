// #include "db_opt" 导入需要的db操作函数


/**
 * 设定应用程序的运行环境
 * ii_buffer_update_threshold ,Flush倒排索引缓冲区的阈值
 * enable_phrase_search 是否启用短语检索
 * db_path 数据库的路径
 * return 0 成功
 *
 * int init_database(gitsoo_env *env, const char *db_path);
 * fin_database() is also in charge of the db oprations group.--YJH,ZH
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

//释放应用程序的运行环境
//fin_database() is in charge of the db oprations group.--YJH,ZH
static void fin_env(gitsoo_env *env) {
  fin_database(env);
}
