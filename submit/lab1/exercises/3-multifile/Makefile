PROJECT = quadr
SRC_FILES = main.c quadr.c quadr.h Makefile README test.data
CFLAGS = -g -Wall -std=c11
LDFLAGS = -lm

$(PROJECT):	quadr.o main.o
		$(CC) quadr.o main.o -o $@ $(LDFLAGS)

clean:
		rm -f *.o $(PROJECT) $(PROJECT).tar.gz
dist:		$(PROJECT).tar.gz

$(PROJECT).tar.gz: $(SRC_FILES)
		tar -cf $(PROJECT).tar $(SCR_FILES)
		gzip -f $(PROJECT).tar

depend:
		$(CC) -MM $(CPPFLAGS) *.c
