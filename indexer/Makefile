# Makefile for 'crawler' module
#
# Jacob Werzinsky 

PROG1 = indexer
OBJS1 = indexer.o 
PROG2 = indextest
OBJS2 = indextest.o
LIBS = ../common/common.a ../libcs50/libcs50.a
CFLAGS = -Wall -pedantic -std=c11 -ggdb -lm $(TESTING) -I../common -I../libcs50 
CC = gcc
MAKE = make

all: $(PROG1) $(PROG2)

$(PROG1): $(OBJS1)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

$(PROG2): $(OBJS2)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# Dependencies: 
indexer.o: ../libcs50/webpage.h ../libcs50/memory.h ../common/pagedir.h ../common/index.h 

.PHONY: test clean

test: $(PROG1) $(PROG2) testing.sh
	bash -v testing.sh

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG1)
	rm -f $(PROG2)
	rm -f stocks
