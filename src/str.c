#include <string.h>

#include "str.h"

string_t string__create() {
	string_t str;
	str.len = 0;
	str.cap = 15;
	str.data = (char *)calloc(16, sizeof(char));

	return str;
}

void string__append_char(string_t *str, char c) {
	if (str->len >= str->cap - 1) {
		str->cap *= 2;
		str->data = (char *)realloc(str->data, str->cap);
	}

	str->data[str->len++] = c;
}

void string__append_str(string_t *str, const char *s) {
	size_t tmp_len = str->len + strlen(s);
	if (tmp_len >= str->cap - 1) {
		do {
			str->cap *= 2;
		} while (tmp_len >= str->cap - 1);

		str->data = (char*)realloc(str->data, str->cap);
	}

	strncpy(&(str->data[str->len]), s, strlen(s));
	str->len = tmp_len;
}

void string__append(string_t *str, string_t *s) {
	size_t tmp_len = str->len + s->len; 
	if (tmp_len >= str->cap - 1) {
		do {
			str->cap *= 2;
		} while (tmp_len >= str->cap - 1);

		str->data = realloc(str->data, str->cap);
	}

	strncpy(&(str->data[str->len]), s->data, s->len);
	str->len = tmp_len;
}

int32_t string__get_char_at(string_t *str, size_t index, char *c) {
    if (index > str->len)
        return -1;

    *c = str->data[index];
    return 0;
}

void string__copy(string_t *dst, const string_t *src) {
	dst->len = src->len;
	dst->cap = src->cap;
	dst->data = (char *)malloc(src->len + 1);
	memcpy(dst->data, src->data, src->len + 1);
}

string_t string__from(const char *str) {
	string_t res;
	res.len = strlen(str);
	res.cap = res.len + 1;
	res.data = (char *)malloc(res.cap);
	strncpy(res.data, str, res.len);
	res.data[res.len] = '\0';

	return res;
}

void string__clear(string_t *str) {
	memset(str->data, 0, str->len);
	str->len = 0;
}

void string__delete(string_t *str) {
	str->len = 0;
	str->cap = 0;
	free(str->data);
}
