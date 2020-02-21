# Makefile for CS50 Tiny Search Engine
#
# David Kotz - April 2016, 2017
# Updated by Temi Prioleau, January 2020

MAKE = make
.PHONY: all valgrind clean

############## default: make all libs and programs ##########
all: 
	$(MAKE) -C libcs50
	$(MAKE) -C common
	$(MAKE) -C crawler
	$(MAKE) -C indexer
	$(MAKE) -C querier

############## valgrind all programs ##########
valgrind: all
	$(MAKE) -C crawler valgrind
	$(MAKE) -C indexer valgrind
	$(MAKE) -C querier valgrind

############### TAGS for emacs users ##########
TAGS:  Makefile */Makefile */*.c */*.h */*.md */*.sh
	etags $^

############## clean  ##########
clean:
	rm -f *~
	rm -f TAGS
	$(MAKE) -C libcs50 clean
	$(MAKE) -C common clean
	$(MAKE) -C crawler clean
	$(MAKE) -C indexer clean
	$(MAKE) -C querier clean
