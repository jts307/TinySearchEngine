# Makefile for common library
#   Builds and compiles all these utility functions into a library.
#
# Jacob Werzinsky, Winter 2020 CS50 

# object files, and the target library
OBJS = pagedir.o
LIB = common.a
FLAGS = 
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I ../libcs50 $(FLAGS)
CC = gcc
MAKE = make

# Build the library by archiving object files
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# Dependencies: 
pagedir.o: pagedir.h ../libcs50/memory.h ../libcs50/webpage.h
memory.o: ../libcs50/memory.h
webpage.o: ../libcs50/webpage.h ../libcs50/memory.h ../libcs50/file.h
file.o: ../libcs50/file.h
 
.PHONY: clean test 

# clean up
clean:
	rm -f core
	rm -f $(LIB) *~ *.o

