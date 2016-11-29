CC = gcc
CFLAGS = -Wall -std=c99 -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -O3 -g -I ./include
OBJS = gitsoo.o util.o token.o search.o postings.o database.o wikiload.o
DATE=$(shell date "+%Y%m%d")
DIR_NAME=gitsoo-${DATE}

gitsoo: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -l sqlite3 -l expat -l m

.c.o:
	$(CC) $(CFLAGS) -c $<

gitsoo.o: gitsoo.h util.h token.h search.h postings.h database.h wikiload.h
util.o: util.h
token.o: gitsoo.h token.h
search.o: gitsoo.h util.h token.h search.h postings.h
postings.o: gitsoo.h util.h postings.h database.h
database.o: gitsoo.h util.h database.h
wikipedia.o: gitsoo.h wikiload.h

.PHONY: clean
clean:
	rm *.o

dist:
	rm -rf $(DIR_NAME)
	mkdir $(DIR_NAME)
	cp -R *.c *.h include Makefile README $(DIR_NAME)
	tar cvfz $(DIR_NAME).tar.gz $(DIR_NAME)
	rm -rf $(DIR_NAME)