CC=gcc
CFLAGS=-I.

all: tcp_server tcp_client udp_server udp_client

tcp_server:
	$(CC) -o tcpserver tcpserver.c $(CFLAGS)

tcp_client:
	$(CC) -o tcpclient tcpclient.c $(CFLAGS)

udp_server:
	$(CC) -o udpserver udpserver.c $(CFLAGS)

udp_client:
	$(CC) -o udpclient udpclient.c $(CFLAGS)

clean:
	rm tcpclient tcpserver udpclient udpserver