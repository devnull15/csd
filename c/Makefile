CC=gcc

main: main.c 
	$(CC) -o main main.c
throckets_old: throckets.c
	$(CC) -D_GNU_SOURCE -Wall -o throckets throckets.c -lpthread; ./throckets
tcp_run: tcp.c
	$(CC) -pthread -o tcp tcp.c; ./tcp
udp_run: udp.c
	$(CC) -pthread -o udp udp.c; ./udp

