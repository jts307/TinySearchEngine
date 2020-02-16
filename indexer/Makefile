# Makefile for 'crawler' module
#
# Jacob Werzinsky 

PROG = indexer
OBJS = indexer.o 
LIBS = ../common/common.a ../libcs50/libcs50.a
CFLAGS = -Wall -pedantic -std=c11 -ggdb -lm $(TESTING) -I../common -I../libcs50 
CC = gcc
MAKE = make

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# Dependencies: 
indexer.o: ../libcs50/webpage.h ../libcs50/memory.h ../common/pagedir.h ../common/index.h 

.PHONY: test clean

test: $(PROG) testing.sh
	bash -v testing.sh

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
	rm -f stocks
