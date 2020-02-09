# Makefile for common library
#   Builds and compiles all pageDirectory based utility modules
# Jacob Werzinsky, Winter 2020 CS50 

# object files, archive files, and the target library
OBJS = pagedir.o
ARCS = libcs50.a
LIB = common.a
FLAGS = 
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I ../libcs50 $(FLAGS)
CC = gcc
MAKE = make

# Build the library by archiving object files
$(LIB): $(OBJS) $(ARVS)
	ar cr $(LIB) $(OBJS)

# Dependencies: 
pagedir.o: pagedir.h ../libcs50/memory.h ../libcs50/webpage.h
 
.PHONY: clean test 

# clean up
clean:
	rm -f core
	rm -f $(LIB) *~ *.o

