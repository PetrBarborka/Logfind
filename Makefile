CFLAGS = -Wall -g

all: 
	make logfind

options:
	cc $(CFLAGS) -c $@.c -o $@.o

args:
	cc $(CFLAGS) -c $@.c -o $@.o

lines:
	cc $(CFLAGS) -c $@.c -o $@.o

files:
	cc $(CFLAGS) -c $@.c -o $@.o

logfind: options args lines files
	cc $(CFLAGS) options.o args.o files.o lines.o $@.c -o $@

clean:
	rm logfind *.o
