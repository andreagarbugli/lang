#ifndef _GX_PARSER_H
#define _GX_PARSER_H

#include "common.h"
#include "darray.h"
#include "lexer.h"
#include "str.h"

/*===---------------------------------*/
/*    AST   						  */
/*===---------------------------------*/
typedef enum ast_node_type {
	AST_LET			,
	AST_NUM			,
	AST_IDENT		,
	AST_BINOP		,
} ast_node_type_t;

typedef struct ast_node {
	ast_node_type_t	type;
	token_t      	tok;
} ast_node_t;

typedef struct ast_ident {
	ast_node_type_t	type;
    token_t     	tok;
    string_t    	val;
} ast_ident_t;

typedef struct ast_binop {
	ast_node_type_t	type;
	token_t      	tok;
	ast_node_t		*lhs;
	ast_node_t		*rhs;
	char			op;
} ast_binop_t;

// LetExpr ::= 
//		let IdentExpr = Expr;
typedef struct ast_let {
	ast_node_type_t	type;
	token_t      	tok;
    ast_ident_t     *ident;
	ast_node_t		*expr;
} ast_let_t;

typedef struct program {
	ast_node_t	**nodes;
	size_t		len;
} program_t;


/*===---------------------------------*/
/*    PARSER   						  */
/*===---------------------------------*/
typedef struct parser {
	lexer_t 	*lex;
	token_t 	tok_cur;
	token_t 	tok_peek;
	string_t	*errors;
} parser_t;

#endif // _GX_PARSER_H