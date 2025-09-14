
#include "scanner.h"
extern int get_token();
extern char* lexeme;
char* token_name[] = {
  "UNDEF",
  "ID",
  "INTCON",
  "LPAREN",
  "RPAREN",
  "LBRACE",
  "RBRACE",
  "COMMA",
  "SEMI",
  "kwINT",
  "kwIF",
  "kwELSE",
  "kwWHILE",
  "kwRETURN",
  "opASSG",
  "opADD",
  "opSUB",
  "opMUL",
  "opDIV",
  "opEQ",
  "opNE",
  "opGT",
  "opGE",
  "opLT",
  "opLE",
  "opAND",
  "opOR",
  "opNOT",
};


void print_token(Token tok, char* lexeme) {
  if (tok < UNDEF || tok > opNOT) {
    printf("TOKEN VALUE OUT OF BOUNDS: %d\n", tok);
  }
  else {
    printf("%s : %s\n", token_name[tok], lexeme);
  }
}

int parse(){
    int tok;
    printf("parse called\n");

    while ((tok = get_token()) != EOF) {
        print_token(tok, lexeme);
    }
    return 0;
}