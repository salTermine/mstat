SHELL = /bin/sh 

.SUFFIXES:
.SUFFIXES: .c .o

CFLAGS = -Wall -Werror
BINS = mstat

all: $(BINS)

mstat: 
	gcc $(CFLAGS) reg_stats.c -o $@

clean:
	rm -f *.o $(BINS)