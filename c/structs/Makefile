CC=gcc
INCLUDE=.
CFLAGS= -Wall
BINS = ll stack

stack: stack.o
	$(CC) $(CFLAGS) -o $@.out $<

stack.o: stack.c stack.h
	$(CC) $(CFLAGS) -c $<
ll: ll.o
	$(CC) $(CFLAGS) -o $@.out $<

ll.o: ll.c ll.h node.h
	$(CC) $(CFLAGS) -c $<

dll: dll.o
	$(CC) $(CFLAGS) -o $@.out $<

dll.o: dll.c dll.h node.h
	$(CC) $(CFLAGS) -c $<

cll: cll.o
	$(CC) $(CFLAGS) -o $@.out $<

cll.o: cll.c cll.h node.h
	$(CC) $(CFLAGS) -c $<


clean:
	rm *.o *.out
