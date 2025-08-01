all:	admin TCPEchoServer TCPEchoClient HandleTCPClient.o LibSerHV.o

LibSerHV.o:	LibSerHV.c LibSerHV.h data.h
	cc -c LibSerHV.c
	echo "Compilation de LibSerHV"

admin:	data.h admin.c LibSerHV.o
	cc -o admin admin.c LibSerHV.o
	echo "Compilation de admin"


HandleTCPClient.o:	HandleTCPClient.c HandleTCPClient.h Requete.h data.h LibSerHV.h 
		echo "Compilation de HandleTCPClient.o" 
		gcc -c HandleTCPClient.c 

TCPEchoServer:	TCPEchoServer.c HandleTCPClient.o LibSerHV.o 
		echo "Compilation de  TCPEchoServer" 
		gcc -o TCPEchoServer  TCPEchoServer.c  HandleTCPClient.o LibSerHV.o 

TCPEchoClient:	TCPEchoClient.c Requete.h 
		echo "Compilation de TCPEchoServer" 
		gcc -o TCPEchoClient TCPEchoClient.c