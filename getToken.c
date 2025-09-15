
/*
Author: Steven George
file: getToken.c
Version: Assignment 1 Milestone 1
Date: Wednesday September 10 2025
Description: Implements a scanner to find all tokens in the G0 language spec for CSC 453.
Program functions by skipping all white space and comments at the start of each get token 
call, then checks for all single character tokens, then all double character tokens, then 
all identifiers (and inside this functionality keywords), then integer constants. 
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "scanner.h"

// Function defintions 
int skip_whitespace_and_comments();
Token make_single_char_token(int c, Token t);
Token make_possible_two_char_token(int c, int next, Token twoCharOp, Token oneCharOp);
Token categorize_identifier(int c);
Token categorize_integers(int c);

// Variable definitions
char* lexeme = NULL; // Global pointer to hold lexeme that will be printed
char buffer[4096]; // Buffer that each character will be added to
int i = 0; // Pointer to location in buffer that each getchar should be added to, if added
int loopCondition = 1;
int c; // Integer to store characters
int line_number = 1;

int get_token() {
    /*
    Returns the next token in the G0 language spec read from stdin
    */
    
    i = 0;  // Reset the buffer pointer at each start of get_token
    if (lexeme){ 
        // printf("Value of lexeme is %s\n", lexeme);
        // Free lexeme always before calling strdup on it 
        free(lexeme);
        lexeme = NULL;
    }

    c = skip_whitespace_and_comments(); // STILL NEED TO IMPLEMENT THIS

    /*
    First we can do characters that are not followed by anything else, for example left paren
    
    Single character means we can skip duplicating from the buffer
    */
    if (c == '('){return make_single_char_token(c, LPAREN);}
    if (c == ')'){return make_single_char_token(c, RPAREN);}
    if (c == '{'){return make_single_char_token(c, LBRACE);}
    if (c == '}'){return make_single_char_token(c, RBRACE);}
    if (c == ','){return make_single_char_token(c, COMMA);}
    if (c == ';'){return make_single_char_token(c, SEMI);}
    if (c == '+'){return make_single_char_token(c, opADD);}
    if (c == '-'){return make_single_char_token(c, opSUB);}
    if (c == '*'){return make_single_char_token(c, opMUL);}
    if (c == '/'){return make_single_char_token(c, opDIV);}

    /*
    Next we do characters that could be single chars or multiple
    */
    if (c == '=') {return make_possible_two_char_token(c, '=', opASSG, opEQ);}
    if (c == '>') {return make_possible_two_char_token(c, '=', opGT, opGE);}
    if (c == '<') {return make_possible_two_char_token(c, '=', opLT, opLE);}
    if (c == '!') {return make_possible_two_char_token(c, '=', opNOT, opNE);}
    if (c == '&') {return make_possible_two_char_token(c, '&', UNDEF, opAND);}
    if (c == '|') {return make_possible_two_char_token(c, '|', UNDEF, opOR);}

    /*
    Next we will check for identifiers
    */
    if (isalpha(c)){ // Identifier must start with a letter
        return categorize_identifier(c);
    }

    /*
    Next we need to check for integer constants
    */
    if (isdigit(c)){
        return categorize_integers(c);
    }

    // DOUBLE CHECK THAT THIS IS HOW YOU WANT TO DO THIS
    if (c == EOF){
            return -1;
    }

    
    return -1; // DOUBLE CHECK THAT THIS SHOULD BE HERE
}






/*
Take a single character and its token, add an empty string 
to the buffer, and copy it into lexeme and return the token
*/
Token make_single_char_token(int c, Token t){
        i = 0;
        buffer[i++] = c;
        buffer[i] = '\0';
        lexeme = strdup(buffer);
        return t;
}

/*
Takes a character that could be a single character token
or a two character token. Reads another char from stdin 
and compares that to the given int next. If equal 
then return the twoCharOp token, otherwise return the oneCharOp
token and put the read char back into the input stream
*/
Token make_possible_two_char_token(int c, int next, Token oneCharOp, Token twoCharOp){
    i = 0;
    buffer[i++] = c;
    c = getchar();
    if (c == next){
        buffer[i++] = c;
        buffer[i] = '\0';
        lexeme = strdup(buffer);
        return twoCharOp;
    }
    ungetc(c, stdin);
    buffer[i] = '\0';
    lexeme = strdup(buffer);
    return oneCharOp;

}

/*
Takes a character representing the start of an identifier and iterates through stdin until 
the end of that idenfiers is found. Compare the lexeme to keywords to check and return keyword 
before returning the token as a general identifier. 
*/
Token categorize_identifier(int c){
    // Reset position in buffer and add first character
    i = 0;
    buffer[i++] = c;
    c = getchar();

    // While number or digit or underscore get the character
    while (isalnum(c) || c == '_'){
        buffer[i++] = c;
        c = getchar();
    }
    // return ID;

    // Unget the last character
    if (c != EOF){
        ungetc(c, stdin);
    }
    
    // End the buffer string
    buffer[i] = '\0';
    lexeme = strdup(buffer);
    
    // Check for keywords
    if (strcmp(buffer, "int") == 0){return kwINT;}
    if (strcmp(buffer, "if") == 0){return kwIF;}
    if (strcmp(buffer, "else") == 0){return kwELSE;}
    if (strcmp(buffer, "while") == 0){return kwWHILE;}
    if (strcmp(buffer, "return") == 0){return kwRETURN;}

    return ID;



    // Next we either see if it is a valid identifier because if the loop broke for
    // something like a comment thats find because you dont have to have whitespace before 
    // comments, but if it broke for something like a & in the variable name we have an issue.
    // return -1;
}

/*
Take an integer c representing a digit character and iterate through stdin until the end
of the integer constant made up by this first digit is found. Return the INTCON Token
and place the lexeme into the buffer
*/
Token categorize_integers(int c){

    i = 0;
    buffer[i++] = c;
    c = getchar();
    while (isdigit(c)){
        buffer[i++] = c;
        c = getchar();
    }

    if (c != EOF){
        ungetc(c, stdin);
    }
    
    buffer[i] = '\0';
    lexeme = strdup(buffer);
    
    return INTCON;

}

// NEED TO CONSIDER CASE WHERE COMMENT STUFF SHOWS UP INSIDE QUOTES
// NEED TO CHECK ABOUT USING THE CYTPE LIBRARY
/**/

/*
First thing called by getToken. Skips all whitespace and comments before the next token. 
Returns the next character after the whitespace or comment in stdin. I made this design
decision because ungetc can only be guaranteed to unget a single character, not two. So, 
if I getchar a '/' and then dont get a '*' I unget the asterisk but cannot unget the slash 
so I return it. 
*/
int skip_whitespace_and_comments(){
    int c;
    int prev;
    // Loop until all comments and whitespace have been removed
    while (1){
        c = getchar();

        // If end of file then return. EOF can be continously returned so no need to ungetc
        if ( c == EOF){ // Check for immediate EOF
            return EOF;
        }

        if (isspace((unsigned char)c)){ // Check if whitespace and restart loop until no more
            if (c == '\n'){
                line_number++;
            }
            continue;
        }

        // If not a * then unget both characters 
        // If yes then loop until you get the sequence */ or EOF


        if (c == '/'){ // Check for backslash, ungetting if not 
            prev = c; // Store first character then read the next so we have references to 2 chars
            c = getchar();

            if ( c == EOF){ // Check EOF
                return EOF;
            }

            if (c == '*'){ // If comment 
                prev = getchar(); // Skip both prev and c ahead by two chars
                c = getchar();

                while (1){ // Loop until EOF or end of block comment
                    if (c == '\n'){
                        line_number++;
                    }
                    if ( prev == EOF || c == EOF){ // SHOULD WE ERROR ON THIS?
                        fprintf(stderr, "Unterminated comment\n");
                        exit(1);
                        // return;
                    }

                    if (prev == '*' && c == '/'){
                        // Found end of comment 
                        // printf("Removed comment\n");
                        break;
                    }
                    prev = c;
                    c = getchar();
                    
                }

                continue; // Restart outer loop in case more whitespace after comment

            } else{
                // ungetc(c, stdin); 
                // Return an opDiv instead of ungetting both because the library only 
                // guarantees returning 1 to stdin
                ungetc(c, stdin);
                return '/';
            }
    

        } else {
            // ungetc(c, stdin);
            return c;
        }
    }
}
