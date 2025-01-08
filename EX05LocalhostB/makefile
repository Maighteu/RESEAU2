all:	TCPEchoServer	TCPEchoClient	LibSer.o	HandleTCPClient.o
LibSer.o:	LibSer.c	LibSer.h
	echo "Compilation de  LibSer.o"
	gcc -c LibSer.c

HandleTCPClient.o:	HandleTCPClient.c	HandleTCPClient.h
	echo "Compilation de HandleTCPClient.o"
	gcc -c HandleTCPClient.c
	
TCPEchoServer:	TCPEchoServer.c	HandleTCPClient.o	LibSer.o
	echo "Compilation de  TCPEchoServer"
	gcc -o	TCPEchoServer  TCPEchoServer.c  HandleTCPClient.o	LibSer.o
	
TCPEchoClient: TCPEchoClient.c		
	echo "Compilation de TCPEchoServer"
	gcc -o TCPEchoClient	TCPEchoClient.c	
	



