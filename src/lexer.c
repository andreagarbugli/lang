#include "lexer.h"
#include "str.h"

/*===---------------------------------*/
/*    TOKEN						  */
/*===---------------------------------*/
char *token__get_type_def(token_t *tok) {

	switch (tok->type) {
		case TOK_EOF:
			return "TOK_EOF";
	
		/* Identifier + literals */
		case TOK_IDENT:
			return "TOK_IDENT";
		case TOK_INT:
			return "TOK_INT";
	
		/* Operators */
		case TOK_ASSIGN:
			return "TOK_ASSIGN";
		case TOK_PLUS:
			return "TOK_PLUS";
		case TOK_MINUS:
			return "TOK_MINUS";
		case TOK_BANG:
			return "TOK_BANG";
		case TOK_SLASH:
			return "TOK_SLASH";
		case TOK_ASTERISK:
			return "TOK_ASTERISK";
		case TOK_LT:
			return "TOK_LT";
		case TOK_GT:
    		return "TOK_GT";
    	case TOK_EQ:
			return "TOK_EQ";
		case TOK_NOT_EQ:
    		return "TOK_NOT_EQ";

		/* Delimiters */
		case TOK_COMMA:
			return "TOK_COMMA";
		case TOK_SEMICOLON:
			return "TOK_SEMICOLON";
		case TOK_LPAREN:
			return "TOK_LPAREN";
		case TOK_RPAREN:
			return "TOK_RPAREN";
		case TOK_LBRACE:
			return "TOK_LBRACE";
		case TOK_RBRACE:
			return "TOK_RBRACE";
	
		/* Keywords */
		case TOK_KEY_FUNC:
			return "TOK_KEY_FUNC";
		case TOK_KEY_LET:
			return "TOK_KEY_LET";
		case TOK_KEY_RETURN:
			return "TOK_KEY_RETURN";
		case TOK_KEY_TRUE:
			return "TOK_KEY_TRUE";
		case TOK_KEY_FALSE:
			return "TOK_KEY_FALSE";
		case TOK_KEY_IF:
			return "TOK_KEY_IF";
		case TOK_KEY_ELSE:
			return "TOK_KEY_ELSE";
			
		default: // TOK_ILLEGAL
			return "TOK_ILLEGAL";
	}
}

/*===---------------------------------*/
/*    LEXER						  */
/*===---------------------------------*/

inline static bool lexer__is_ident_char(char ch) {
	return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z')
		   || ch == '_';
}

inline static bool lexer__is_digit(char ch) {
	return ('0' <= ch && ch <= '9');
}

inline static void lexer__read_char(lexer_t *lex) {
	if (lex->pos_read >= lex->input.len)
		lex->ch = 0;
	else
		string__get_char_at(&lex->input, lex->pos_read, &lex->ch);

	lex->pos = lex->pos_read;
	lex->pos_read++;
}

inline static void lexer__consume_whitespace(lexer_t *lex) {
	while (lex->ch == ' ' || lex->ch == '\t'
		   || lex->ch == '\n' || lex->ch == '\r')
		lexer__read_char(lex);
}

/**
 * @brief Peek the next char in the input without moving the current position of the cursor 
 * 
 * @param lex The Lexer
 * @return char The character read
 */
inline static char lexer__peek_char(lexer_t *lex) {
	if (lex->pos_read >= lex->input.len)
		return 0;

	return lex->input.data[lex->pos_read];
}

static void lexer__read_ident(lexer_t *lex, token_t *tok) {
	size_t start_pos = lex->pos;

	while (lexer__is_ident_char(lex->ch) || lexer__is_digit(lex->ch))
		lexer__read_char(lex);

	for (size_t i = start_pos; i < lex->pos; i++)
		string__append_char(&tok->literal, lex->input.data[i]);
}

static void lexer__lookup_ident(token_t *tok) {
	string_t *lit = &tok->literal;
	
	if (strcmp(lit->data, "fn") == 0)
		tok->type = TOK_KEY_FUNC;
	else if (strcmp(lit->data, "let") == 0)
		tok->type = TOK_KEY_LET;
	else if (strcmp(lit->data, "return") == 0)
		tok->type = TOK_KEY_RETURN;
	else if (strcmp(lit->data, "true") == 0)
		tok->type = TOK_KEY_TRUE;
	else if (strcmp(lit->data, "false") == 0)
		tok->type = TOK_KEY_FALSE;
	else if (strcmp(lit->data, "if") == 0)
		tok->type = TOK_KEY_IF;
	else if (strcmp(lit->data, "else") == 0)
		tok->type = TOK_KEY_ELSE;
	else
		tok->type = TOK_IDENT;
}

static void lexer__read_number(lexer_t *lex, token_t *tok) {
	size_t start_pos = lex->pos;

	while (lexer__is_digit(lex->ch))
		lexer__read_char(lex);

	for (size_t i = start_pos; i < lex->pos; i++) 
		string__append_char(&tok->literal, lex->input.data[i]);
}

static void lexer__handle_equal_sign(lexer_t *lex, token_t *tok) {
    size_t start_pos = lex->pos;

    char c = lexer__peek_char(lex);
    if (c == '=') {
        lexer__read_char(lex);

		tok->type = TOK_EQ;
    } else {
        tok->type = TOK_ASSIGN;
    }

	for (size_t i = start_pos; i < lex->pos_read; i++) {
		string__append_char(&tok->literal, lex->input.data[i]);
	}
}

static void lexer__handle_bang_sign(lexer_t *lex, token_t *tok) {
    size_t start_pos = lex->pos;

    char c = lexer__peek_char(lex);
    if (c == '=') {
        lexer__read_char(lex);

		tok->type = TOK_NOT_EQ;
    } else {
        tok->type = TOK_BANG;
    }

	for (size_t i = start_pos; i < lex->pos_read; i++) 
		string__append_char(&tok->literal, lex->input.data[i]);
}

lexer_t lexer__init(string_t *input) {
    lexer_t lex = {
        .ch = 0,
        .pos = 0,
        .pos_read = 0,
        .input = *input,
    };

	lexer__read_char(&lex);

    return lex;
}

void lexer__destroy(lexer_t *lex) {
	(void)lex;
	// TODO(garbu)
}

token_t lexer__next_token(lexer_t *lex) {
	token_t res = {
		.type = TOK_ILLEGAL,
		.literal = string__create()
	};

	token_t *tok = &res;
	lexer__consume_whitespace(lex);

	switch (lex->ch) {
		case '=': 
            lexer__handle_equal_sign(lex, tok);
			goto exit_consume;

		/* Delimiters */
		case ',':
			tok->type = TOK_COMMA;
			break;
		case ';':
			tok->type = TOK_SEMICOLON;
			break;
		case '(':
			tok->type = TOK_LPAREN;
			break;
		case ')':
			tok->type = TOK_RPAREN;
			break;
		case '{':
			tok->type = TOK_LBRACE;
			break;
		case '}':
			tok->type = TOK_RBRACE;
			break;

		/* Operator */
		case '+':
			tok->type = TOK_PLUS;
			break;
		case '-':
			tok->type = TOK_MINUS;
			break;
		case '!':
			lexer__handle_bang_sign(lex, tok);
			goto exit_consume;
		case '/':
			tok->type = TOK_SLASH;
			break;
		case '*':
			tok->type = TOK_ASTERISK;
			break;
		case '<':
			tok->type = TOK_LT;
			break;
		case '>':
			tok->type = TOK_GT;
			break;

		/* EOF */
		case 0:
			tok->type = TOK_EOF;
			break;

		/* Identifier and others */
		default: // TODO(garbu): Handle new token
			if (lexer__is_ident_char(lex->ch)) {
				lexer__read_ident(lex, tok);
				lexer__lookup_ident(tok);
				goto exit_no_consume;
			} else if (lexer__is_digit(lex->ch)) {
				tok->type = TOK_INT;
				lexer__read_number(lex, tok);
				goto exit_no_consume;
			} else {
				tok->type = TOK_ILLEGAL;
			}
			break;
	}
	
	string__append_char(&tok->literal, lex->ch);

exit_consume:

	lexer__read_char(lex);

exit_no_consume:

	return res;
}