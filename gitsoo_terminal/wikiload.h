//
// Created by P. Chu on 6/10/16.
//

#ifndef __WIKILOAD_H__
#define __WIKILOAD_H__

#include "gitsoo.h"

typedef void (*add_document_callback)(gitsoo_env *env,
                                      const char *title,
                                      const char *body);

int load_wikipedia_dump(gitsoo_env *env, const char *path,
                        add_document_callback func, int max_article_count);

#endif /* __WIKILOAD_H__ */
