#include "mpc.h"

#ifdef _WIN32
#include <string.h>

static char buffer[2048];

char* readline(char* prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char* cpy = malloc(strlen(buffer)+1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy)-1] = '\0';
  return cpy;
}

void add_history(char* unused) {}

#else
#include <editline/readline.h>
#endif

int main(int argc, char** argv) {

/*Parser*/
  mpc_parser_t* Number   = mpc_new("Number");
  mpc_parser_t* Operator = mpc_new("operator");
  mpc_parser_t* Expr     = mpc_new("expr");
  mpc_parser_t* Lisper    = mpc_new("lisper");

  mpca_lang(MPCA_LANG_DEFAULT,
    "                                                    \
　　  number   : /-?[0-9]+/ ;                            \
      operator : '+' | '-' | '*' | '/' ;                 \
      expr     : <number> | '(' <operator> <expr>+ ')' ; \
      lisper   : /^/ <operator> <expr>+ /$/ ;            \
    ",
    Number, Operator, Expr, Lisper);

	puts("Lisper Version 0.2");
	puts("Press Ctrl+c to Exit\n");

	while (1) {

    char*input=readline("lisper> ");
    add_history(input);

    mpc_result_t r;
    if (mac_parse("<stdin>", input, Lispy, &r)) {
      /* On success print delete the AST */
      mpc_ast_print(r.output);
      mpc_ast_delete(r.output);
    } else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
	  }

    free(input);
  }

  mpc_cleanup(4, Number, Operator, Expr, Lispy);

	return 0;
}
