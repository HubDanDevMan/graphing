PROG=Graphing
CC=gcc
CFLAGS=-Wall -c -O3 
LD=gcc
LDFLAGS=-l SDL2 -l SDL2_ttf -lm -lc
CFILES:= $(wildcard *.c)
OBJS := $(patsubst %.c, %.o, $(CFILES))

all: link

%.o: %.c 
	$(CC) $(CFLAGS) $<


link: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(PROG)

run: all
	./$(PROG)

clean:
	-rm *.o 
