/**********************************************************************
    Tokenizer for Java source.
    Allows unicode escapes only within strings and comments.  Otherwise,
    accepts all and only those tokens defined by the Java language
    specification.  (Note that this is significantly more than you were
    required to do for assignment 3; this solution would have received a
    lot of extra credit.)
    Tokens are classified as listed in scanner.h.
 **********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "reader.h"
#include "char_classes.h"
#include "scanner.h"

// static void print_location (token_t *tok)
// {
//     fprintf(stderr, " at line %d, column %d\n",
//         tok->location.line->line_num, tok->location.column);
// }

static void debug(int n) { // for debugging
    printf("DEBUG %d\n", n);
}

static void scan_error() {
    debug(-1);
    fprintf(stderr, "scan error\n");
    // fprintf(stderr, " at line %d, column %d\n",
    //     tok.location.line->line_num, tok.location.column);
    exit(1);
}

// for debugging. print a string of the type of token given
char *printallthethings(token_class tc) {
    switch (tc) {
        case T_PLUS:
            return "T_PLUS";
        case T_MINUS:
            return "T_MINUS";
        case T_MULTIPLY:
            return "T_MULTIPLY";
        case T_DIVIDE:
            return "T_DIVIDE";
        case T_INTEGER:
            return "T_INTEGER";
        case T_FLOAT:
            return "T_FLOAT";
        case T_LEFT_PAREN:
            return "T_LEFT_PAREN";
        case T_RIGHT_PAREN:
            return "T_RIGHT_PAREN";
        case T_INCREMENT:
            return "T_INCREMENT";
        case T_SEMICOLON:
            return "T_SEMICOLON";
        default:
            return "other...";
    }
}

/********
    Modify tok to describe next token of input.
    Update loc to refer to location immediately after tok.
 ********/

void scan(location_t *loc, token_t *tok) {

    //printf("entered scan\n");

    // list of states for the DFA
    enum {
            start,
            done,
            got_space,
            got_newline,
            got_tab,
            got_semicolon,
            got_eof,
            got_dot,
            got_digit,
            got_float,
            got_integer,
            got_neg,
            got_neg_float,
            got_neg_integer,
            got_left_paren,
            got_right_paren,
            got_other,
            got_post_dot_zero_chain,
        
        /* operators: */
            got_plus,
            got_minus,
            got_star,
            got_divide,
            got_mod,
            got_increment,
            got_decrement,
            got_unary_plus,
            got_unary_minus

    } state = start;

    location_t loc_save;

// start of the in-function macros:
/* Standard way to recognize a token: put back lookahead character that isn't part of current token: */
// #define is global scope no matter what. WHITE SPACE AFTER THE BACKSLASH OF THE MACRO CAUSED ERROR
// take the token up to the previous char
#define ACCEPT_REUSE(t) \
    *loc = loc_save;    \
    tok->length--;      \
    tok->tc = t;        \
    state = done;

/* Shortcut to eliminate final states with no out transitions: go
    ahead and accept token in previous state, but don't put back the
    lookahead: it's actually part of the token: */
// take the token up to the current char
#define ACCEPT(t) \
    tok->tc = t;  \
    state = done;
// end of the macros

    // start the scanning!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    tok->location = *loc; // dereference to set
    tok->length = 0;

    while (state != done) {

        //printf("entered while loop in scan\n");

        loc_save = *loc;

        // returns the ascii value of the char at location loc, then INCREMENTS THE LOCATION, from reader.c
        // this will also increment the line number if needed
        int c = get_character(loc);

        tok->length++;

        // start DFA state traversing

        switch (state) {

            // begin start case
            case start:
                switch (char_classes[c]) {

                    // whitespace/newlines
                    case WHITE:
                        state = got_space;
                        break;                  // how much does this break?
                    case TAB:
                        state = got_tab;
                        break;
                    case EOLN:
                        state = got_newline;
                        break;

                    // numbers
                    case DOT:
                        state = got_dot;
                        break;
                    CASE_DIGIT:
                        state = got_digit;
                        break;
                    
                    // non-paren, single operators
                    case PLUS:
                        ACCEPT(T_PLUS); // start -> +
                        break;
                    case MINUS:
                        ACCEPT(T_MINUS); // start -> -
                        break;
                    case STAR:
                        ACCEPT(T_MULTIPLY); // start -> *
                        break;
                    case SLASH:
                        ACCEPT(T_DIVIDE); // start -> /
                        break;
                    case PCT:
                        ACCEPT(T_MODULUS); // start -> %
                        break;
                    
                    // parentheses
                    case LPAREN:
                        state = got_left_paren;
                        //continue;
                        //ACCEPT(got_left_paren);
                        break;
                    case RPAREN:
                        state = got_right_paren;
                        ACCEPT(T_RIGHT_PAREN);
                        break;
                    
                    // semicolon / end
                    case SEMIC:
                        ACCEPT(T_SEMICOLON); // state -> ;
                        break;
                    case END:

                        ACCEPT_REUSE(T_EOF); // ?
                        break;
                    default:
                        scan_error();
                }
                break;
            // end start case


            // begin whitespace, tab, newline character strings case
            case got_space: // start -> (space)
                switch (char_classes[c]) {
                    case WHITE: // if another whitespace, next char.
                        state = got_space;
                        break;
                    default:
                    // if this char is not whitespace then accept the whitespace as a token, reusing the current char
                        ACCEPT_REUSE(T_SPACE); // start -> (1 or more spaces)
                        break;
                }
                break;
            case got_tab: // start -> /t
                switch (char_classes[c]) {
                    case TAB:
                        state = got_tab;
                        break;
                    default:
                        ACCEPT_REUSE(T_TAB); // start -> (1 or more tabs)
                        break;
                }
                break;
            case got_newline: // start -> \n
                switch (char_classes[c]) {
                    case EOLN:
                        state = got_newline;
                        break;  /* stay put */
                    default:
                        ACCEPT_REUSE(T_NEWLINE); // start -> (1 or more newlines)
                        break;
                }
                break;
            // end whitespace, tab, newline character strings



            /* operators: */

            case got_left_paren: // start -> (
                switch (char_classes[c]) {
                    case PLUS:
                        state = got_plus;
                        break;
                    case MINUS:
                        state = got_minus;
                        break;
                    default:
                        // found a new expression
                        ACCEPT_REUSE(T_LEFT_PAREN); // start -> (
                        break;
                }
                break;

            case got_plus: // start -> ( -> +
                switch (char_classes[c]) {
                    case PLUS:
                        state = got_increment;
                        break;
                    case RPAREN:
                        state = got_unary_plus;
                        ACCEPT(T_UNARY_PLUS); // start -> ( -> + -> )
                        break;
                    default:
                        scan_error();
                }
                break;

            case got_minus: // start -> ( -> -
                switch (char_classes[c]) {
                    case MINUS:
                        state = got_decrement;
                        break;
                    case RPAREN:
                        state = got_unary_minus;
                        ACCEPT(T_UNARY_MINUS); // start -> ( -> - -> )
                        break;
                    CASE_DIGIT: // start -> ( -> - -> n
                        // accept the parenthese and start from the minus
                        // my custom ACCEPT function for dealing with NEGATIVE NUMBERS
                        *loc = loc_save;
                        loc->column--;
                        tok->length -= 2;
                        tok->tc = T_LEFT_PAREN;
                        state = done;
                        // printf("********************* saw the minus sign\n");                        
                        break;
                    default:
                        scan_error();
                }
                break;

            case got_increment: // start -> ( -> + -> +
                switch (char_classes[c]) {
                    case RPAREN:
                        ACCEPT(T_INCREMENT); // start -> ( -> + -> + -> )
                        break;
                    default:
                        scan_error();
                }
                break;

            case got_decrement: // start -> ( -> - -> -
                switch (char_classes[c]) {
                    case RPAREN:
                        ACCEPT(T_DECREMENT); // start -> ( -> - -> - -> )
                        break;
                    default:
                        scan_error();
                }
                break;

            /* end operators */

            /* numeric literals: */

            case got_digit: // start -> n
                switch (char_classes[c]) {
                    CASE_DIGIT:
                        state = got_digit;
                        break;
                    case DOT:
                        state = got_dot;
                        break;
                    default:
                        ACCEPT_REUSE(T_INTEGER); // start -> (1 or more n's) followed by a non-dot
                        break;
                }
                break;

            case got_dot:   // start -> (0 or more n's) -> .
                switch (char_classes[c]) {
                    CASE_DIGIT_NONZERO:
                        state = got_float;
                        break;
                    case DIG_0:
                        state = got_post_dot_zero_chain;
                        break;
                    default:
                        ACCEPT_REUSE(T_INTEGER); // start -> (0 or more n's) -> .
                        // assume there has already been a number before the dot if there are no numbers after the dot
                        break;
                }
                break;

            case got_post_dot_zero_chain:
                switch (char_classes[c]) {
                    case DIG_0:
                        state = got_post_dot_zero_chain;
                        break;
                    CASE_DIGIT_NONZERO:
                        state = got_float;
                        break;
                    default:
                        ACCEPT_REUSE(T_INTEGER); // start -> (0 or more n's) -> . -> (1 or more zeros)
                        break;
                }
                break;
            
            case got_float: // start -> (0 or more n's) -> . -> (1 or more n's)
                switch (char_classes[c]) {
                    CASE_DIGIT:
                        state = got_float;
                        break;
                    default:
                        ACCEPT_REUSE(T_FLOAT); // start -> (0 or more n's) -> . -> (1 or more n's)
                        break;
                }
                break;

            /* end numeric literals */

            default:
                scan_error();

                
            
        }
    }
}







