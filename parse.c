/*
Author: Steven George
file: parse.c
Version: Assignment 1 Milestone 2
Date: Wednesday September 17 2025
Description: Implements a G0 parsers for CSC 453. Recursive descent parsing. 
Did not need to use follow sets but does 
*/

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

/*
Take an expected token and see if the next token returned by get_token() returns the expected 
token, erroring if not. This is how the parser "consumes" a token and a new one is loaded 
into curr_token
*/
void match(int expected){
    if (curr_token == expected){
        curr_token = get_token();
    } else{
        fprintf(stderr, "ERROR LINE %d: expected token %s, but found %s\n", line_number, token_name[expected], token_name[curr_token]);
        exit(1);
    }
}

/*
Parses for types, which right now is only kwInt
*/
void parse_type(){
    // Don't call match here in case I need to add more later down the line
    if (curr_token == kwINT){
        match(kwINT);
        
    } else{
        fprintf(stderr, "ERROR LINE %d: expected token %s, but found %s\n", line_number, token_name[kwINT], token_name[curr_token]);
        exit(1);
    }
}

/*
Parses optional formals which is the empty set right. Checks the follows set and errors 
if does not find what is expected
*/
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

/*
Parses optional variable declarations which is the empty set right now. Check follows 
set and errors if it does not find the expected result.
*/
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

/*
Parses the optional expression list. Empty set right now so checks the follows set and 
errors if it does not find the expected token.
*/
void parse_opt_expr_list(){
    // return;
    if (curr_token == RPAREN){
        return;
    } else{
        fprintf(stderr, "ERROR LINE %d: expected token %s, but found %s\n", line_number, token_name[RPAREN], token_name[curr_token]);
        exit(1);    
    }
}

/*
Parses a function call. Expects an ID, LPAREN, then matches option expression list, then
expects a parenthesis. Errors if not perfect matching 
*/
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


/*
Parses statements. Expected a function call then a semicolon. Start of program right now
*/
void parse_stmt(){
    if (curr_token == ID){
        parse_fn_call();
        match(SEMI);
    } else{
        fprintf(stderr, "ERROR LINE %d: expected token %s, but found %s\n", line_number, token_name[ID], token_name[curr_token]);
        exit(1);
    }
}

/*
Parses options statment list. Right now that can have a stmt qnd then another possible 
statement list. If RBRACE that is the follows set and we return. Error if none of the above
found. 
*/
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


/*
Parses a function defintion. See the spec.
*/
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


/*
Start of recursion. 
Expects EOF or function definition and recursive call to parse helper again
*/
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


/*
Parse function. Call recursive helper and gets first token. Matches EOF after all recursion completed
*/
int parse(){
    curr_token = get_token();
    parse_helper();
    match(EOF);
    return 0;
}