CC=gcc
INCLUDE=./structs/

main: main.c 
	$(CC) -o main main.c
tcp_run: tcp.c
	$(CC) -pthread -o tcp tcp.c; ./tcp
udp_run: udp.c
	$(CC) -pthread -o udp udp.c; ./udp
syncd: syncd.o $(INCLUDE)stack.o
	$(CC) -o $@ &<
syncd.o: syncd.c $(INCLUDE)stack.h
	$(CC) -o $@ $<


clean:
	rm *~ tcp udp main
