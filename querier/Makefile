# Makefile for 'indexer' module
#
# Jacob Werzinsky 

PROG1 = querier
OBJS1 = querier.o  
PROG2 = fuzzquery
OBJS2 = fuzzquery.o
LIBS = ../common/common.a ../libcs50/libcs50.a
CFLAGS = -Wall -pedantic -std=c11 -ggdb -lm $(TESTING) -I../common -I../libcs50 
CFLAG2 = -Wall -pedantic -std=gnu11 -ggdb -lm $(TESTING) -I../common -I../libcs50
CC = gcc
MAKE = make

all: $(PROG1) $(PROG2)

$(PROG1): $(OBJS1) $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

$(PROG2): $(OBJS2) $(LIBS)
	$(CC) $(CFLAG2) $^ -o $@

# Dependencies: 
querier.o: ../libcs50/memory.h ../libcs50/file.h ../libcs50/counters.h ../common/pagedir.h ../common/index.h ../common/word.h

fuzzquery.o: ../libcs50/file.h ../libcs50/memory.h

../common/common.a:
	cd ../common;make

../libcs50/libcs50.a:
	cd ../libcs50;make

.PHONY: test clean

test: $(PROG1) $(PROG2) testing.sh
	bash -v testing.sh

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG1)
	rm -f $(PROG2)
	rm -f stocks
	rm -frd testing
