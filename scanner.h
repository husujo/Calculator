/**********************************************************************
    Tokenizer
 **********************************************************************/

#ifndef SCANNER_H
#define SCANNER_H
#include "reader.h"

typedef enum {

    T_SPACE, // any number of only spaces
    T_NEWLINE, // any number of only newlines  (/n, /r)
    T_TAB,
    T_SEMICOLON,
    T_EOF,
    T_DOT,
    T_DIGIT,
    T_FLOAT,
    T_INTEGER,
    T_neg,
    T_NEG_FLOAT,
    T_NEG_INTEGER,
    T_LEFT_PAREN,
    T_RIGHT_PAREN,
    T_OTHER,
        
    /* operators: */
    T_PLUS,
    T_MINUS,
    T_MULTIPLY,
    T_DIVIDE,
    T_MODULUS,
    T_INCREMENT,
    T_DECREMENT,
    T_UNARY_PLUS,
    T_UNARY_MINUS

} token_class;

// token_t struct
typedef struct {
    token_class tc; // enumerated type
    location_t location; // what spot in the input start at?
    int length;     /* length of token in characters (may span lines) */
} token_t;

char *printallthethings(token_class tc); // for debugging. print a string of the type of token given

void scan(location_t * loc, token_t * tok);
    /* Modify tok to describe next token of input.
        Update loc to refer to location immediately after tok. */

#endif
