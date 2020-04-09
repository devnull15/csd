CC=gcc

main: main.c 
	$(CC) -o main main.c
tcp_run: tcp.c
	$(CC) -pthread -o tcp tcp.c; ./tcp
udp_run: udp.c
	$(CC) -pthread -o udp udp.c; ./udp
clean:
	rm *~ tcp udp main
