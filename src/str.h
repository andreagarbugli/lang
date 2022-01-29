#ifndef _DSTRING_H
#define _DSTRING_H

#include "common.h"

typedef struct string {
	char *data;
	size_t len;
	size_t cap;
} string_t;

/**
 * @brief Create a new string_t
 * 
 * @param str The 
 */
string_t string__create();

void string__append_char(string_t *str, char c);

void string__append_str(string_t *str, const char *s);

void string__append(string_t *str, string_t *s);

int32_t string__get_char_at(string_t *str, size_t index, char *c);

void string__copy(string_t *dst, const string_t *src);

string_t string__from(const char *str);

void string__clear(string_t *str);

void string__delete(string_t *str);

#endif // _DSTRING_H
