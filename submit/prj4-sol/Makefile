PROJECT =	y86-sim
#CFLAGS =        -g -Wall -std=c11
COURSE =	cs220
CPPFLAGS = 	-I $$HOME/$(COURSE)/include
LDFLAGS =	-L $$HOME/$(COURSE)/lib -l cs220 -l y86

$(PROJECT):	main.o ysim.o
	        $(CC) ysim.o main.o $(LDFLAGS) -o $@

ysim.o:		ysim.c ysim.h
main.o:		main.c ysim.h

clean:
		rm -f $(TARGETS) *.o *~
