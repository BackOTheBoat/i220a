PROJECT = hamming-encode

CFLAGS = -g -Wall -std=c11
LDFLAGS = -lm

$(PROJECT): hamming.o main.o
	$(CC) hamming.o main.o $(LDFLAGS) -o $@

hamming-decode: hamming-encode
		ln -s -f $< $@

SRC_FILES = main.c \
	main.o \
	hamming.c \
	hamming.o \
	hamming-encode.o \
	hamming-decode \
	Makefile \
	README

clean:
	rm -f *.o $(PROJECT) $(PROJECT).tar.gz

depend:
	$(CC) -MM $(CPPFLAGS) *.c

dist:	$(PROJECT).tar.gz

hamming.o:	hamming.c hamming.h

main.o:		hamming.h main.c
