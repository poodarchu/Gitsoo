#ifndef __TOKEN_H__
#define __TOKEN_H__

#include "gitsoo.h"

int text_to_postings_lists(gitsoo_env *env,
                           const int document_id, const UTF32Char *text,
                           const unsigned int text_len,
                           const int n, inverted_index_hash **postings);

int token_to_postings_list(gitsoo_env *env,
                           const int document_id, const char *token,
                           const unsigned int token_size,
                           const int position,
                           inverted_index_hash **postings);

#endif
