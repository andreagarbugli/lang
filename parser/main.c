#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>

typedef int8_t	 i8;
typedef int16_t	 i16;
typedef int32_t	 i32;
typedef int64_t	 i64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float    f32;
typedef double   f64;

typedef enum ast_node_type {
	AST_LET,
	AST_NUM,
	AST_BINOP,
} ast_node_type_t;

typedef struct ast_node {
	ast_node_type_t	type;
} ast_node_t;

typedef struct ast_num {
	ast_node_type_t	type;
	float			value;
} ast_num_t;

typedef struct ast_binop {
	ast_node_type_t type;
	ast_node_t		*lhs;
	ast_node_t	   	*rhs;
	char		   	op;
} ast_binop_t;

// LetExpr ::= 
//		let IdentExpr = Expr;
typedef struct ast_let {
	ast_node_type_t	type;
	char			ident[256];
	ast_node_t		*rhs;
} ast_let_t;

typedef struct program {
	ast_node_t *root;
	size_t	   len;
} program_t;

void ast_node__delete(ast_node_t *node) {
	switch (node->type) {
		case AST_LET:
			ast_let_t *let_node = (ast_let_t *)node;
			ast_node__delete(let_node->rhs);
			break;
		case AST_BINOP:
			ast_binop_t *binop_node = (ast_binop_t *)node;
			ast_node__delete(binop_node->rhs);
			ast_node__delete(binop_node->lhs);
			break;
		default:
			break;
	}

	free(node);
}

static char debug_str[8192];

int ast_node__to_string(ast_node_t *node, char *str)  {
	int n_write = 0;
	switch (node->type) {
		case AST_LET:
			ast_let_t *let = (ast_let_t *)node;
			n_write += snprintf(str, 8192, "let = ");
			n_write += ast_node__to_string((ast_node_t *)let->rhs, str + n_write);
			break;
		case AST_NUM:
			ast_num_t *num = (ast_num_t *)node;
			n_write += snprintf(str, 8192, "%f", num->value);
			break;
		case AST_BINOP:
			ast_binop_t *binop = (ast_binop_t *)node;
			n_write += ast_node__to_string((ast_node_t *)binop->lhs, str);
			n_write += snprintf(str + n_write, 8192, " %c ", binop->op);
			n_write += ast_node__to_string((ast_node_t *)binop->rhs, str + n_write);
			break;
		default:
			break;
	}

	return n_write;
}

int main() {
	// simple num test
//	ast_num_t *num = (ast_num_t *)malloc(sizeof(ast_num_t)); 
//	num->type = AST_NUM;
//	num->value = 20.0;

	// binop test
	ast_num_t *num_lhs = (ast_num_t *)malloc(sizeof(ast_num_t)); 
	num_lhs->type = AST_NUM;
	num_lhs->value = 5.0;

	ast_num_t *num_rhs = (ast_num_t *)malloc(sizeof(ast_num_t)); 
	num_rhs->type = AST_NUM;
	num_rhs->value = 10.0;

	ast_binop_t *binop = (ast_binop_t *)malloc(sizeof(ast_binop_t)); 
	binop->type = AST_BINOP;
	binop->lhs = (ast_node_t *)num_lhs;
	binop->rhs = (ast_node_t *)num_rhs;
	binop->op = '+';

	ast_let_t *let = (ast_let_t *)malloc(sizeof(ast_let_t));
	let->type = AST_LET;
	let->rhs = (ast_node_t *)binop;
	strncpy(let->ident, "num", 256);

	program_t prog = {0};
	prog.root = (ast_node_t *)let;

	ast_node__to_string(prog.root, debug_str);
	fprintf(stderr, "%s\n", debug_str);

	ast_node__delete(prog.root);

	return 0;
}

