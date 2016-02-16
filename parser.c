/**********************************************************************
    Uses a recursive-descent parser that accepts a superset of the
    language.
    At the moment it's an unreasonably large superset: basically any
    string of tokens with balanced curvy braces is accepted inside a
    class definition.  You have to fix that by fleshing out the routines
    that parse a compound_stmt.  You also have to
        - find all declared identifiers, so you can print them red
        - indent things appropriately
        - fix inter-token spaces
 **********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "reader.h"
#include "scanner.h"
#include "parser.h"
#include <math.h>
/* forward declarations: */

static token_t     *tok; // current token to scan into     // use a pointer?
static location_t  *loc; // current location to scan from

static pack_t E(bool seen_left_paren);
static pack_t EP();
static pack_t T(bool seen_left_paren);
static pack_t TP();
static pack_t F(bool seen_left_paren);
static pack_t FP(bool seen_left_paren);
static pack_t FPP(pack_t pack);

// static void debug(int n) { // for debugging
//     printf("DEBUG %d\n", n);
// }

//    Get next token from the scanner.  Filter out white space, tabs, and newlines.
static void get_token() {
    do {
        scan(loc, tok);   // scanner.h        
    } while (tok->tc == T_SPACE || tok->tc == T_NEWLINE || tok->tc == T_TAB); // while whitespace
}

//    A parse error has occurred.  Print error message and halt.
static void parse_error() {
    fprintf(stderr, "Syntax error in parser\n");
    // fprintf(stderr, " at line %d, column %d\n",
    //     tok.location.line->line_num, tok.location.column);
    exit(1);
}

//    A token of class tc is expected from the scanner.  Verify and print.
static void match(token_class tc) {
    if (tc != tok->tc) {
        parse_error(); // this will exit
    }
    //printf("matching %s\n", printallthethings(tc));
    get_token(); // get a token after matched the current one, pretty much updates the current token
}

// a function I created to get the numeric value of a number in a token, at that token's location
static float get_token_numeric_value(token_t *token) {
    float value;

    int column = token->location.column;

    value = atof(token->location.line->data+column); // all to float

    //printf("get_token_numeric_value returned %f\n", value);

    return value;
}


// where the magic happens
void parse() {

    // just in case...
    tok = (token_t *) malloc(sizeof(token_t));
    loc = (location_t *) malloc(sizeof(location_t));

    set_to_beginning(loc); // put the location at the start of the file
    
    get_token(); // set tok to the first token

    while (tok->tc != T_EOF) {

        printf("%s", tok->location.line->data);
        printf("==%f\n\n", E(false)->num);
        match(T_SEMICOLON);

    }

}

static pack_t E(bool seen_left_paren) { // evaluate the given list of +/- ops here

    pack_t ret_pack = (pack_t) malloc(sizeof(struct packing_heat));
    pack_t pack = (pack_t) malloc(sizeof(struct packing_heat));
    pack_t tpack = (pack_t) malloc(sizeof(struct packing_heat));

    //printf("ENTERED E(%d)\n", seen_left_paren);

    tpack = T(seen_left_paren);

    float left = tpack->num;
    float right;
    //printf("E left: %f\n", left);

    pack = EP();
    if (pack == NULL) { // POSSIBLE POST OP ALLOWED
        //printf("LEAVING E(%d)\n", seen_left_paren);
        ret_pack = tpack;
        return ret_pack;
    }

    while (pack != NULL) { // evaluate the linked list one pack at a time

        right = pack->num;

        if (pack->op.tc == T_PLUS) {
            //printf("adding %f plus %f\n", left, right);
            left = left + right;
        } else if (pack->op.tc == T_MINUS) {
            //printf("subtracting %f minus %f\n", left, right);
            left = left - right;
        } else {
            parse_error();
        }

        pack = pack->next;

    }

    //printf("LEAVING E(%d)\n", seen_left_paren);
    ret_pack->num = left;
    return ret_pack;

}

static pack_t EP() {
    //printf("ENTERED EP()\n");

    pack_t ret_pack = (pack_t) malloc(sizeof(struct packing_heat));
    pack_t tpack = (pack_t) malloc(sizeof(struct packing_heat));

    if (tok->tc == T_PLUS) {
        ret_pack->op.tc = T_PLUS;
        match(T_PLUS);
    } else if (tok->tc == T_MINUS) {
        ret_pack->op.tc = T_MINUS;
        match(T_MINUS);
    } else {
        //printf("LEAVING EP()\n");
        return NULL; // epsilon
    }

    tpack = T(false); // false b/c seen an operation

    float left = tpack->num;
    //printf("EP left: %f\n", left);
    ret_pack->num = left;
    pack_t pack = EP();
    if (pack == NULL) {
        //printf("LEAVING EP()\n");
        return ret_pack;
    } else {
        ret_pack->next = pack; // add to linked list for +/-
    }

    //printf("LEAVING EP()\n");
    return ret_pack;

}

static pack_t T(bool seen_left_paren) { // evaluate the given list of *,/,% ops here

    //printf("ENTERED T(%d)\n", seen_left_paren);

    pack_t fpack = (pack_t) malloc(sizeof(struct packing_heat));

    fpack = F(seen_left_paren);
    float left = fpack->num;
    float right;
    //printf("T left: %f\n", left);

    pack_t pack = TP();

    if (pack == NULL) { // POSSIBLE POST OP ALLOWED
        //printf("LEAVING T(%d)\n", seen_left_paren);
        return fpack;
    }

    //printf("T right: %f\n", right);

    while (pack != NULL) {

        right = pack->num;

        if (pack->op.tc == T_MULTIPLY) {

            //printf("doing %f * %f\n", left, right);

            left = left * right;
        
        } else if (pack->op.tc == T_DIVIDE) {

            //printf("doing %f / %f\n", left, right);

            left = left / right;

        } else if (pack->op.tc == T_MODULUS) {

            if (floorf(left) == left && floorf(right) == right) {
                int l = left;
                int r = right;
                left = l % r;
            } else {
                parse_error(); // can't mod non-floats!
            }

        } else {
            parse_error();
        }

        pack = pack->next;

    }

    fpack->num = left;
    //printf("LEAVING T(%d)\n", seen_left_paren);
    return fpack;

}

static pack_t TP() {
    //printf("ENTERED TP()\n");

    pack_t ret_pack = (pack_t) malloc(sizeof(struct packing_heat));

    // set the operation of the return pack
    if (tok->tc == T_MULTIPLY) {
        ret_pack->op.tc = T_MULTIPLY;
        match(T_MULTIPLY);
    } else if (tok->tc == T_DIVIDE) {
        ret_pack->op.tc = T_DIVIDE;
        match(T_DIVIDE);
    } else if (tok->tc == T_MODULUS) {
        ret_pack->op.tc = T_MODULUS;
        match(T_MODULUS);
    } else {

        //printf("LEAVING TP()\n");
        return NULL; // epsilon
    }

    pack_t fpack = F(false); // no post ops here cause seen a + or -
    float left = fpack->num;
    //printf("TP left: %f\n", left);
    ret_pack->num = left;
    pack_t pack = TP();
    if (pack == NULL) {
        //printf("LEAVING TP()\n");
        return ret_pack;
    } else {
        ret_pack->next = pack;
    }

    //printf("LEAVING TP()\n");
    return ret_pack;
}

static pack_t F(bool seen_left_paren) {
    //printf("ENTERED F(%d)\n", seen_left_paren);

    pack_t ret_pack = (pack_t) malloc(sizeof(struct packing_heat));
    pack_t pack = (pack_t) malloc(sizeof(struct packing_heat));

    if (tok->tc == T_UNARY_PLUS) {
        match(T_UNARY_PLUS);
        ret_pack = F(seen_left_paren);

        // check if (+)-4 = 4   if so:
        // if (ret_pack->num < 0) {
        //      ret_pack->num = -ret_pack->num;
        // }

        //printf("LEAVING F(%d)\n", seen_left_paren);
        return ret_pack;

    } else if (tok->tc == T_UNARY_MINUS) {
        match(T_UNARY_MINUS);
        ret_pack = F(seen_left_paren);
        ret_pack->num = -ret_pack->num;

        //printf("LEAVING F(%d)\n", seen_left_paren);
        return ret_pack;

    } else if (tok->tc == T_INCREMENT) {
        match(T_INCREMENT);
        ret_pack = FP(seen_left_paren);
        ret_pack->num++;
        ret_pack->post_inc_num++;

        //printf("LEAVING F(%d)\n", seen_left_paren);
        return ret_pack;

    } else if (tok->tc == T_DECREMENT) {
        match(T_DECREMENT);
        ret_pack = FP(seen_left_paren);
        ret_pack->num--;
        ret_pack->post_inc_num--;

        //printf("LEAVING F(%d)\n", seen_left_paren);
        return ret_pack;

    } else if (tok->tc == T_INTEGER) {
        pack->num = get_token_numeric_value(tok);
        //printf("F value is %f\n", pack->num);
        match(T_INTEGER);
        ret_pack = FPP(pack);
        //printf("FPP value is %f\n", ret_pack->num);
        //printf("LEAVING F(%d)\n", seen_left_paren);
        return ret_pack;

    } else if (tok->tc == T_FLOAT) {
        pack->num = get_token_numeric_value(tok);
        //printf("F value is %f\n", pack->num);
        match(T_FLOAT);
        ret_pack = FPP(pack);
        //printf("FPP value is %f\n", ret_pack->num);
        //printf("LEAVING F(%d)\n", seen_left_paren);
        return ret_pack;

    } else if (tok->tc == T_MINUS) { // must be a neg number *********************** WORKS!!!!
        match(T_MINUS);
        if (tok->tc == T_INTEGER) {
            pack->num = -get_token_numeric_value(tok); // get the actual number from the line
            match(T_INTEGER);
        } else if (tok->tc == T_FLOAT) {
            pack->num = -get_token_numeric_value(tok);
            match(T_FLOAT);
        }

        ret_pack = FPP(pack);
        //printf("LEAVING F(%d)\n", seen_left_paren);
        return ret_pack;

    } else if (tok->tc == T_LEFT_PAREN) {
        match(T_LEFT_PAREN);
        //printf("setting seen paren to true\n");
        pack = E(true); // true for seen a left parenthese // need E to return a pack?
        match(T_RIGHT_PAREN);

        if (pack->post_flag == true) { // TESTING **********************
            //printf("in F setting the post_inc_num %f to the num %f\n", pack->num, pack->post_inc_num);
            pack->num = pack->post_inc_num;
        }

        ret_pack = FPP(pack);
        //printf("LEAVING F(%d)\n", seen_left_paren);
        return ret_pack;

    } else {

        parse_error(); // did not find anything that was expected
        exit(-1);
    }

}

static pack_t FP(bool seen_left_paren) { // deals with pre increment/decrement and also a number or expression
    //printf("ENTERED FP(%d)\n", seen_left_paren);

    pack_t ret_pack = (pack_t) malloc(sizeof(struct packing_heat));
    pack_t pack = (pack_t) malloc(sizeof(struct packing_heat));

    if (tok->tc == T_INCREMENT) {
        match(T_INCREMENT);
        ret_pack = FP(seen_left_paren);
        ret_pack->num++;
        ret_pack->post_inc_num++;
        //printf("FP incrementing to %f and to %f\n", ret_pack->num, ret_pack->post_inc_num);
        //printf("LEAVING FP(%d)\n", seen_left_paren);
        return ret_pack;

    } else if (tok->tc == T_DECREMENT) {
        match(T_DECREMENT);
        ret_pack = FP(seen_left_paren);
        ret_pack->num--;
        ret_pack->post_inc_num--;
        //printf("LEAVING FP(%d)\n", seen_left_paren);
        return ret_pack;

    } else if (tok->tc == T_INTEGER) { // saw a pre increment/decrement

        pack->num = get_token_numeric_value(tok);
        match(T_INTEGER);
        ret_pack = FPP(pack);
        //printf("LEAVING FP(%d)\n", seen_left_paren);
        return ret_pack;

    } else if (tok->tc == T_FLOAT) {

        pack->num = get_token_numeric_value(tok);
        match(T_FLOAT);
        ret_pack = FPP(pack);

        //printf("LEAVING FP(%d)\n", seen_left_paren);
        return ret_pack;

    } else if (tok->tc == T_LEFT_PAREN) {

        match(T_LEFT_PAREN);
        //printf("setting seen paren to true\n");
        pack = E(true); // true for seen a left parenthese
        match(T_RIGHT_PAREN);

        if (pack->post_flag == true) { // TESTING **********************
            //printf("in FP setting the post_inc_num %f to the num %f\n", pack->num, pack->post_inc_num);
            pack->num = pack->post_inc_num;
        }

        ret_pack = FPP(pack);
        //printf("LEAVING FP(%d)\n", seen_left_paren);
        return ret_pack;
    }

    parse_error(); // did not find anything that was expected
    exit(-1);

}

static pack_t FPP(pack_t pack) {                // to deal with POST increment/decrement operators

    //printf("ENTERED FPP()\n");

    pack_t ret_pack = (pack_t) malloc(sizeof(struct packing_heat));
    
    ret_pack->num = pack->num;

    if (pack->post_flag == true) { // if this is not the first post increment/decrement we've seen so far

        ret_pack->post_inc_num = pack->post_inc_num;

        if (tok->tc == T_INCREMENT) {
            match(T_INCREMENT);
            ret_pack->op.tc = T_INCREMENT;
            ret_pack->post_inc_num = pack->post_inc_num + 1;
            ret_pack->post_flag = true;
            ret_pack = FPP(ret_pack);
        } else if (tok->tc == T_DECREMENT) {
            match(T_DECREMENT);
            ret_pack->op.tc = T_DECREMENT;
            ret_pack->post_inc_num = pack->post_inc_num - 1;
            ret_pack->post_flag = true;
            ret_pack = FPP(ret_pack);
        } else {
            //printf("value given to FPP(): %f\n", pack->num);
            //printf("LEAVING FPP() on epsilon\n");
            return pack; // epsilon ******************************
        }
    
        //printf("LEAVING FPP(), incremented %f to %f\n", pack->post_inc_num, ret_pack->post_inc_num);
        ret_pack->num = pack->num;
        return ret_pack;

    }

    // this is the frist post increment/decrement we've seen so far

    if (tok->tc == T_INCREMENT) {
        match(T_INCREMENT);
        ret_pack->op.tc = T_INCREMENT;
        ret_pack->post_inc_num = pack->num + 1;
        ret_pack->post_flag = true;
        ret_pack = FPP(ret_pack);
    } else if (tok->tc == T_DECREMENT) {
        match(T_DECREMENT);
        ret_pack->op.tc = T_DECREMENT;
        ret_pack->post_inc_num = pack->num - 1;
        ret_pack->post_flag = true;
        ret_pack = FPP(ret_pack);
    } else {
        //printf("value given to FPP(): %f\n", pack->num);
        //printf("LEAVING FPP() on epsilon\n");
        return pack; // epsilon ******************************
    }
    
    //printf("LEAVING FPP(), incremented %f to %f\n", pack->num, ret_pack->post_inc_num);
    ret_pack->num = pack->num;
    return ret_pack;

}

// evaluate the post increment/decrement ONLY if you find a right paren after all of them AND you had a left 
// paren BEFORE the thing to be incremented/decremented
// flag a possible post_inc/dec expression when see a left paren in front of it???
