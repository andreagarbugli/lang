#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>

#ifdef _WIN32
	#include <Windows.h>
#elif LINUX
	#include <unistd.h>
#endif

#include "common.h"
#include "darray.h"
#include "lexer.h"
#include "string.h"

void DEBUG_print_token(token_t *tok) {
	fprintf(stderr, "%-20s\t\t'%s'\n",
			token__get_type_def(tok), tok->literal.data);
}

#define NSEC_PER_SEC 1000000000

inline static u64 timespec_to_ns(struct timespec *ts) {
	return ts->tv_sec * NSEC_PER_SEC + ts->tv_nsec;
}

static bool g_running = true;

void handle_ctrlc(i32 signum) {
	(void)signum;
	g_running = false;
}

void create_and_exec_lexer(string_t *input) {
	lexer_t lex = lexer__init(input);

	vector_define_new(token_t, tokens); // = token_array__new();

	token_t tok;
	do {
		tok = lexer__next_token(&lex);

		vector__push(tokens, tok);
	} while (tok.type != TOK_EOF);
	
	for (token_t *tok = vector__begin(tokens); tok != vector__end(tokens); tok++) {
		DEBUG_print_token(tok);
		string__delete(&tok->literal);
	}

	// token_array__delete(tokens);
	vector__delete(tokens);

	lexer__destroy(&lex);
}

void repl__start() {
#ifdef LINUX
	struct sigaction sa = {0};
	sa.sa_flags = 0;
	sa.sa_handler = handle_ctrlc;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
#endif

	string_t input = string__create();
	while (true) {
		char c = getchar();

		if (!g_running) {
			break;
		}

		if (c == '\n') {
			create_and_exec_lexer(&input);
			string__clear(&input);
			fprintf(stdout, ">>> ");
		} else {
			string__append_char(&input, c);
		}
	}

	string__delete(&input);

	fprintf(stdout, "\n");
	fprintf(stdout, "Bye!\n");
}

void compile(char *script_name) {
	i32 script_file = open(script_name, O_RDONLY);
	if (script_file < 0) {
		fprintf(stderr, "[error] cannot open the script '%s': %s\n",
				script_name, strerror(errno));
	}
	
	string_t input = string__create();
	char buf[BUFSIZ] = {0};
	size_t bytes_read = 0; 
	while ((bytes_read = read(script_file, buf, BUFSIZ)) > 0) {
		string__append_str(&input, buf);
	}

	create_and_exec_lexer(&input);

	string__delete(&input);
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stdout, "Welcome to GX Language\n");
		fprintf(stdout, ">>> ");

		repl__start();
	} else {
#ifdef LINUX
		struct timespec start_ts, end_ts;
		clock_gettime(CLOCK_MONOTONIC, &start_ts);
#endif
		compile(argv[1]);

#ifdef LINUX
		clock_gettime(CLOCK_MONOTONIC, &end_ts);
		f64 elapsed = timespec_to_ns(&end_ts) - timespec_to_ns(&start_ts);

		fprintf(stderr, "\n*** EXEC TIME: %fs\n", elapsed / NSEC_PER_SEC);
#endif
	}
	
	return 0;
}
