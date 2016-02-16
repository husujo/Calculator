// no main.h file?

#include <stdlib.h>
#include <stdio.h>
#include "reader.h"
#include "scanner.h"
#include "parser.h"

int main (int argc, char *argv[]) {
    FILE *fp;   // file type variable from stdio
    
    fp = fopen(argv[1], "r"); // argv[1] = argc = # args = second arg. argv[1] = second arg !!!!
    if (fp == NULL) {
        printf("Error: infile.txt does not exist. YOU DONE GOOFED\n");
        exit(1); // "Syntax error..."
    }
  
    initialize_reader(fp); // from reader.h
    
    // Do scan, parse, evaluate here
    parse(); // from parse.h
    
    // free all the memory taken by the linked list of lines. idk why but ok
    finalize_reader(); // reader.h
    
    exit(0);
}
