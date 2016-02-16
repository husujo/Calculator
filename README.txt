Hunter Johnston
Calculator
10/11/15

I AM USING THE NEW SPECS, details below.

Description of Problem

Our task was to put together a DFA, and LL1 grammar to read in and parse a mathematical expression with order of operations, and also some weird incrementing/decrementing notation.

Description of Implementation

	Execute with: ./calculator input.txt

I used legacy the code given to us, and modified it heavily, mostly removing things. I wrote a DFA for scanning and a LL1 grammar for parsing. I used a pack_t struct I made to help with the parsing of same-precedence operators. Enumerated lists and switch case statements made checking the numerous possibilities of tokens easier to do. The input expression goes in the input.txt file, and you end your expression with a semicolon.

note: my incrementing syntax is using (++),(--) as incrementing/decrementing and you can put it before a number/expression or after, and it will always evaluate the pre operations, but only evaluate the post operations if there is another set of parentheses all around the incrementations. Ex: (++)3(++) = 4, but ((++)3(++)) = 5. and you can stack as many increments and decrements as you want, as long as you put unary symbols first. so (-)(++)(++)5(--)(--) is legal and evaluates to -7.

List of Files

README:
	This file.

makefile:
	the makefile for compiling. I use clang as my compiler, but I switched it back to gcc for you. I also had to add -lm flag for gcc because gcc does not like the floorf() function.

calculator:
	The executable file. Execute with: ./calculator input.txt
	
input.txt:
	The file that calculator reads from to get an expression. For nice output, make sure there is a \n (enter) character at the end of the input.

sample_input.txt:
	Some test cases to show of my calculators bad-ass self
sample_output.txt:
	The results of the sample_input.txt run with the calculator

char_classes.c/h:
	contains the enumerated list of char_classes corresponding with the ASCII table numbers. for use in the scanner determining what character it has read. Also contains some switch case macros for numbers to condense the code in the scanner.

main.c:
	check the command line arguments and call the reader and the parser. Close the reader nicely when finished.

reader.c/h:
	Form and store the linked list of lines, has the line_t struct and the location_t struct. Can return a character at a particular location (which is a line and a column of that line).

parser.c/h:
	Contains the LL1 Grammar for the "parse tree" which is really just a tree of function calls resembling the grammar. I named the functions the same thing my grammar non-terminals were named for convenience and readability. I also implemented my own pack_t struct (packing_heat) which pretty much let me return a number and an operator from the same function return call. I later expanded it's use and had it also hold the number to be used if a post inc/dec operation was carried out. I had a bug of reading same-precedence operators from right to left, which I fixed by linking pack_t's together and not evaluating them until I had to, which I could then traverse the linked list and evaluate them in order.

scanner.c/h:
	Contains the token_t struct and the enumerated token type list. The code traverses the DFA I made for reading tokens and then spits out a token, given a starting location to read from.

Extra Credit

I used git.

I got my calculator to accept a number if that number has a negative symbol in front of it without parentheses, as part of the definition of a number. For example would accept: -5, because -5 is a number, but not -(5) because if you want to negate an expression you use the (-) token. See the sample_input.txt/sample_output.txt for negative number examples.

I know the grading might be a bit more slack about the incrementing and decrementing tokens, but I would just like to say that I think mine work exactly how you envisioned: pre-inc/dec always execute, but post only execute if there is outer parentheses around the number/expression to be incremented. they work if you use both pre and post incrementing on an expression/number. Any extra credit would be appreciated. See the sample_input.txt/sample_output.txt for examples of incrementing

Does removing unused legacy code count as extra? Cause I trashed a whole lot of that.

I think I handle errors well, exiting the program if I see non-syntax. Extra credit would be appreciated.

