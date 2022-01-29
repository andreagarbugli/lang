#ifndef _LEXER_H
#define _LEXER_H

#include "str.h"

/*===---------------------------------*/
/*    TOKEN						  */
/*===---------------------------------*/
typedef enum token_type {
	TOK_ILLEGAL 	,
	TOK_EOF 		,

	/* Identifier + literals */
	TOK_IDENT  		, // add, foobar, x, y, ...
	TOK_INT			, // 123456 TODO(garbu): float, hex, etc. ??

	/* Operators */
	TOK_ASSIGN		, // '='
	TOK_PLUS		, // '+'
	TOK_MINUS    	, // '-'
	TOK_BANG		, // '!'
	TOK_ASTERISK	, // '*'
	TOK_SLASH		, // '/'
	TOK_LT			, // '<'
	TOK_GT			, // '>'
	TOK_EQ			, // '=='
	TOK_NOT_EQ		, // '!='
	
	/* Delimiters */
	TOK_COMMA		,
	TOK_SEMICOLON	,
	TOK_LPAREN		,
	TOK_RPAREN		,
	TOK_LBRACE		,
	TOK_RBRACE		,

	/* Keywords */
	TOK_KEY_FUNC	,
	TOK_KEY_LET		,
	TOK_KEY_RETURN	,
	TOK_KEY_TRUE	,
	TOK_KEY_FALSE	,
	TOK_KEY_IF		,
	TOK_KEY_ELSE	,

	__TOK_MAX
} token_type_t;

typedef struct token {
	token_type_t 	type;
	string_t		literal;
} token_t;

/**
 * @brief Return the string representation of a token type
 * 
 * @param tok The token
 * @return char* The string value of the token type
 */
char *token__get_type_def(token_t *tok);

/*===---------------------------------*/
/*    LEXER						  */
/*===---------------------------------*/
typedef struct lexer {
	string_t 	input;
	size_t 		pos;
	size_t 		pos_read;
	char 		ch;
} lexer_t;


lexer_t lexer__init(string_t *input);

void lexer__destroy(lexer_t *lex);

/**
 * @brief Get the next token
 * 
 * @param lex The Lexer
 * @return token_t Return the token read
 */
token_t lexer__next_token(lexer_t *lex);

#endif // _LEXER_H