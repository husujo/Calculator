/**********************************************************************
    Maintain input as a linked list of lines.
    Return characters on demand.
 **********************************************************************/

#ifndef READER_H
#define READER_H

// line_t struct
typedef struct line {
    char *data; // the actual line
    int line_num; // line/row #
    int length; /* number of non-NUL characters == index of trailing NUL */
    struct line * next;
} line_t;

// location_t struct
typedef struct {
    line_t *line;
    int column; // column/char #
} location_t;

void initialize_reader();
    /* read input into memory */
void set_to_beginning(location_t *loc);
    /* make location_t point to beginning of program */
int get_character(location_t *loc);
    /* Return EOF if there are no more characters.
        Else return character at specified location, and advance loc to
        next character of input. */
void finalize_reader();
    /* discard buffered input */

#endif
