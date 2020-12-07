PROJECT =	stall-sim
#CFLAGS =        -g -Wall -std=c11
COURSE =	cs220
CPPFLAGS = 	-I $$HOME/$(COURSE)/include
LDFLAGS =	-L $$HOME/$(COURSE)/lib -l cs220 -l y86

$(PROJECT):	main.o stall-sim.o
	        $(CC) stall-sim.o main.o $(LDFLAGS) -o $@

stall-sim.o:	stall-sim.c stall-sim.h
main.o:		main.c stall-sim.h

clean:
		rm -f $(TARGETS) *.o *~
