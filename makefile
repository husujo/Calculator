#######################################################################
#	Makefile for Assignment 2 (Java scanner and formatter)
#	CSC 173, Fall 2000
#
#
#
#	ADDED -lm after the objects for GCC with floorf()
#######################################################################

.SUFFIXES: .o .c
# Don't use default rules for other suffixes.

CC =		gcc
CFLAGS =	-g -Wall
GOAL =		calculator	
CFILES =	reader.c scanner.c char_classes.c parser.c main.c
HFILES =	reader.h scanner.h char_classes.h parser.h
OBJECTS =	reader.o scanner.o char_classes.o parser.o main.o

.c.o:
	$(CC) -c $(CFLAGS) $*.c

$(GOAL): $(OBJECTS)
	$(CC) -o $(GOAL) $(OBJECTS) -lm

clean:
	/bin/rm $(OBJECTS) $(GOAL)

sources:
	@echo $(CFILES) $(HFILES)

depend:
	echo '# DO NOT EDIT THIS FILE -- it is automatically generated' \
		> makefile.dep
	echo '' >> makefile.dep
	$(CC) -M $(CFILES) >> makefile.dep
