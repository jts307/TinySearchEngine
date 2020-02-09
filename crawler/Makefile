# Makefile for 'crawler' module
#
# Jacob Werzinsky 

PROG = crawler
OBJS = crawler.o ../libcs50/libcs50.a ../common/common.a 
LIBS =
CFLAGS = -Wall -pedantic -std=c11 -ggdb -lm $(TESTING) -I../common -I../libcs50
CC = gcc
MAKE = make

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# Dependencies: 
crawler.o: ../libcs50/bag.h ../libcs50/hashtable.h ../libcs50/webpage.h ../libcs50/memory.h ../common/pagedir.h

.PHONY: test clean

test: $(PROG) test_names
	./$(PROG) < test_names

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
	rm -f stocks

