# Makefile for 'crawler' module
#
# Jacob Werzinsky 

PROG = querier 
OBJS = querier.o 
LIBS = ../common/common.a ../libcs50/libcs50.a
CFLAGS = -Wall -pedantic -std=c11 -ggdb -lm $(TESTING) -I../common -I../libcs50 
CC = gcc
MAKE = make

$(PROG): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

# Dependencies: 
querier.o: ../libcs50/memory.h ../libcs50/file.h ../common/pagedir.h ../common/index.h ../common/word.h

../common/common.a:
	cd ../common;make

../libcs50/libcs50.a:
	cd ../libcs50;make

.PHONY: test clean

test: $(PROG) testing.sh
	bash -v testing.sh

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
	rm -f stocks
