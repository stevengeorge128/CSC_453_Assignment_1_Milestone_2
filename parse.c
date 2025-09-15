
#include "scanner.h"
extern int get_token();
extern char* lexeme;
extern int line_number;
int curr_token; 

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


// void print_token(Token tok, char* lexeme) {
//   if (tok < UNDEF || tok > opNOT) {
//     printf("TOKEN VALUE OUT OF BOUNDS: LINE %d\n", tok);
//   }
//   else {
//     printf("%s : %s\n", token_name[tok], lexeme);
//   }
// }

void match(int expected){
    if (curr_token == expected){
        curr_token = get_token();
    } else{
        fprintf(stderr, "ERROR LINE %d: expected token %s, but found %s\n", line_number, token_name[expected], token_name[curr_token]);
        exit(1);
    }
}

void parse_type(){
    // Don't call match here in case I need to add more later down the line
    if (curr_token == kwINT){
        match(kwINT);
        
    } else{
        fprintf(stderr, "ERROR LINE %d: expected token %s, but found %s\n", line_number, token_name[kwINT], token_name[curr_token]);
        exit(1);
    }
}

void parse_opt_formals(){
    // return;
    // Would have an if here if anything in the first set
    if (curr_token == RPAREN){
        // Looking at the follow set right now to make this decision
        return;
    }else{
        fprintf(stderr, "ERROR LINE %d: expected token %s, but found %s\n", line_number, token_name[RPAREN], token_name[curr_token]);
        exit(1);
    }

}

void parse_opt_var_decls(){
    // return;
    // Would have an if here if anything in the first set
    if (curr_token == ID || curr_token == RBRACE){
        // Looking at the follow set right now to make this decision
        return;
    }else{
        fprintf(stderr, "ERROR LINE %d: expected token %s | %s, but found %s\n", line_number, token_name[ID],token_name[RBRACE], token_name[curr_token]);
        exit(1);
    }

}
void parse_opt_expr_list(){
    // return;
    if (curr_token == RPAREN){
        return;
    } else{
        fprintf(stderr, "ERROR LINE %d: expected token %s, but found %s\n", line_number, token_name[RPAREN], token_name[curr_token]);
        exit(1);    
    }
}
void parse_fn_call(){
    if (curr_token == ID){
        match(ID);
        match(LPAREN);
        parse_opt_expr_list();
        match(RPAREN);
    } else{
        fprintf(stderr, "ERROR LINE %d: expected token %s, but found %s\n", line_number, token_name[ID], token_name[curr_token]);
        exit(1);
    }
}



void parse_stmt(){
    if (curr_token == ID){
        parse_fn_call();
        match(SEMI);
    } else{
        fprintf(stderr, "ERROR LINE %d: expected token %s, but found %s\n", line_number, token_name[ID], token_name[curr_token]);
        exit(1);
    }
}

void parse_opt_stmt_list(){


    if (curr_token == ID){
        parse_stmt();
        parse_opt_stmt_list();
        
    } 
    // return; 
    else if (curr_token == RBRACE) {
        // Using follows set here
        return;
        
    } else{
        fprintf(stderr, "ERROR LINE %d: expected token %s | %s, but found %s\n", line_number, token_name[ID],token_name[RBRACE], token_name[curr_token]);
        exit(1);
    }
}


void parse_func_defn(){
    parse_type();
    match(ID);
    match(LPAREN);
    parse_opt_formals();
    match(RPAREN);
    match(LBRACE);
    parse_opt_var_decls();
    parse_opt_stmt_list();
    match(RBRACE);
}

void parse_helper(){
    if (curr_token == EOF){
        return;
    } else if (curr_token == kwINT){
        parse_func_defn();
        parse_helper();
        
    } else{
        fprintf(stderr, "ERROR LINE %d\n",line_number);
        exit(1);
    }

}


int parse(){
    // printf("parse called\n");

    curr_token = get_token();
    parse_helper();
    match(EOF);

    


    return 0;
}