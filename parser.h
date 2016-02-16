/**********************************************************************
    Java parser and pretty-printer.
 **********************************************************************/

#ifndef PARSER_H
#define PARSER_H
#include <stdbool.h>

typedef struct packing_heat {
	token_t op;
	float num;
	float post_inc_num;

	bool post_flag; // = false;
	// if seen a left parenthese before the expression started, signaling possible post operations

	struct packing_heat *next; // can form a linked list in order to properly evaluate like-precedence ops left to right

} *pack_t;

void parse();
    /* Scan source, identify structure, and print appropriately. */

#endif
