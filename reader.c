/**********************************************************************
    Maintain input as a linked list of lines.
    Return characters on demand.
 **********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "reader.h"

#define MAX_LINE_LEN 512

static line_t head = {0, 0, 0}; // supposed to be 4 members?

// static void debug(int n) { // helpful debug function
//     printf("DEBUG %d\n", n);
// }

/********
    Read source program into a linked list of line_t structures,
    with (empty) head node "head".
 ********/
void initialize_reader(FILE *fp) {
    //printf("entered initialize_reader()\n");
    char s[MAX_LINE_LEN];
    line_t *tail = &head; // set tail to head

    // go through and make the linked list of lines
    while (1) {
        line_t *old_tail;

        // nothing else to read in, if fgets returned NULL pointer. this also reads a line and stores it in s
        if (!fgets(s, MAX_LINE_LEN, fp)) { 
            break;
        }
        
//set the adress of oldtail to the address of tail. changing the address of tail will not change the address of oldtail
        old_tail = tail; 
        tail = (line_t *) malloc(sizeof(line_t)); // does this reset the address of oldtail too? probably not.
        tail->data = strdup(s); // stringduplicate, returns pointer to the duplication
        tail->line_num = old_tail->line_num + 1;
        tail->length = strlen(s); // stringlength
        if (tail->length == MAX_LINE_LEN-1 && tail->data[MAX_LINE_LEN]) { // if line too long
            fprintf(stderr, "input line %d too long (%d max)\n", tail->line_num, MAX_LINE_LEN);
            exit(-1);
        }
        tail->next = 0;
        old_tail->next = tail; 
    }
}

// set to begining of linked list, right after the head aka the first line
void set_to_beginning(location_t *loc) {
    loc->line = head.next;
    loc->column = 0;
}

// returns the ascii value of the char at location loc, then increments the location
int get_character(location_t *loc) {

    //printf("entered get_character\n");

    int rtn;

    if (loc->column >= loc->line->length) { // error
        return 0;
    }
    
    //rtn = loc->line->data[loc->column++]; // original code, modifyed below for readability
    rtn = loc->line->data[loc->column];
    loc->column++;

// if location is now greater or equal line length, AND line->next != NULL
    if (loc->column >= loc->line->length && loc->line->next) { 
        loc->line = loc->line->next; // go to next line, first column
        loc->column = 0;
    }
    return rtn;
}


// free all the memory of the linked list at the end
void finalize_reader() {
    line_t * l = head.next;

// while l != NULL
    while (l) {
        line_t * t = l;
        if (l->data) {
            free(l->data);
        }
            
        l = l->next;
        free(t);
    }
}
