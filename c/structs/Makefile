CC=gcc
INCLUDE=.
CFLAGS= -Wall
BINS = ll stack

stack: stack.o
	$(CC) $(CFLAGS) -o $@ $<

stack.o: stack.c stack.h
	$(CC) $(CFLAGS) -c $<
ll: ll.o
	$(CC) $(CFLAGS) -o $@ $<

ll.o: ll.c ll.h node.h
	$(CC) $(CFLAGS) -c $<
clean:
	rm *.o $(BINS)
