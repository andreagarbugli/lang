#include "parser.h"

/*===---------------------------------*/
/*    AST							  */
/*===---------------------------------*/
void ast_node__delete(ast_node_t *node) {
	switch (node->type) {
		case AST_LET: {
				ast_let_t *let_node = (ast_let_t *)node;
				ast_node__delete(let_node->expr);
			} break;
		case AST_BINOP: {
				ast_binop_t *binop_node = (ast_binop_t *)node;
				ast_node__delete(binop_node->rhs);
				ast_node__delete(binop_node->lhs);
			} break;
		default:
			break;
	}

	free(node);
}

int ast_node__to_string(ast_node_t *node, char *str)  {
	int n_write = 0;
	switch (node->type) {
	case AST_LET: {
			ast_let_t *let = (ast_let_t *)node;
			n_write += snprintf(str, 8192, "let = ");
			n_write += ast_node__to_string((ast_node_t *)let->expr, str + n_write);
		} break;
	case AST_BINOP: {
			ast_binop_t *binop = (ast_binop_t *)node;
			n_write += ast_node__to_string((ast_node_t *)binop->lhs, str);
			n_write += snprintf(str, 8192, " %c ", binop->op);
			n_write += ast_node__to_string((ast_node_t *)binop->rhs, str + n_write);
		} break;
	default:
		break;
	}

	return n_write;
}

/*===---------------------------------*/
/*    PARSER						  */
/*===---------------------------------*/
inline static void parser__next_token(parser_t *p) {
	p->tok_cur = p->tok_peek;
	p->tok_peek = lexer__next_token(p->lex);
}

static bool __parser__current_token_is(parser_t *p, token_type_t tok_type) {
	return p->tok_cur.type == tok_type;
}

static bool __parser__peek_token_is(parser_t *p, token_type_t tok_type) {
	return p->tok_peek.type == tok_type;
}

static bool __parser__expect_peek(parser_t *p, token_type_t tok_type) {
	if (__parser__peek_token_is(p, tok_type)) {
		parser__next_token(p);
		return true;
	} 

	return false;
}

static ast_node_t *parser__parse_let(parser_t *p) {
	ast_let_t *node = (ast_let_t *)malloc(sizeof(ast_let_t));
	node->type = AST_LET;
	node->tok = p->tok_cur;

	if (!__parser__expect_peek(p, TOK_IDENT))
		return NULL;

	node->ident = (ast_ident_t *)malloc(sizeof(ast_ident_t));
	node->ident->type = AST_IDENT;
	node->ident->tok = p->tok_cur;
	node->ident->val = p->tok_cur.literal; 

	if (!__parser__expect_peek(p, TOK_ASSIGN))
		return NULL;

	// TODO(garbu): We're skipping the expression until we 
	// 	encounter a semicolon
	while (!__parser__current_token_is(p, TOK_SEMICOLON))
		parser__next_token(p);
	
	return (ast_node_t *)node;
}

static ast_node_t *parser__parse_node(parser_t *p) {
	switch (p->tok_cur.type)
	{
	case TOK_KEY_LET:
		return parser__parse_let(p);
	default:
		return NULL;
	}
}

parser_t parser__new(lexer_t *lex) {
	parser_t p = {0};
	p.lex = lex;
	vector_define_new(string_t*, errors);

	p.errors = errors;

	parser__next_token(&p);
	parser__next_token(&p);

	return p;
}

program_t parser__parse_program(parser_t *p) {
	vector_define_new(ast_node_t*, nodes);

	program_t prog;
	prog.nodes = nodes;

	while (p->tok_cur.type != TOK_EOF) {
		ast_node_t *node = parser__parse_node(p);
		if (node != NULL) {
			vector__push(nodes, node);
		}

		parser__next_token(p);
	}

	return prog;
}