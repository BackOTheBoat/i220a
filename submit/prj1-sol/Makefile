PROJECT = hamming-encode

CFLAGS = -g -Wall -std=c11
LDFLAGS = -lm

$(PROJECT): hamming.o main.o
	gcc hamming.o main.o -o $@

hamming-decode: hamming-encode
		ln -s -f $< $@

SRC_FILES = main.c \
	main.o \
	hamming.c \
	hamming.h \
	hamming-encode.o \
	hamming-decode.o \
	Makefile \
	README

depend:
	gcc -MM $(CPPFLAGS) *.c

main.o: main.c hamming.h
hamming-encode.o: hamming.c haimming.h
