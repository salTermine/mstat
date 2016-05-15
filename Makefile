SHELL = /bin/sh 

.SUFFIXES:
.SUFFIXES: .c .o

CFLAGS = -Wall -Werror
BINS = mstat

all: $(BINS)

mstat: 
	gcc $(CFLAGS) hw1.c -o $@

clean:
	rm -f *.o $(BINS)