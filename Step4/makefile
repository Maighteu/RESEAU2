all:	admin TCPEchoServer TCPEchoClient HandleTCPClient.o LibSerHV.o 
 
admin:	data.h admin.c LibSerHV.o   
		cc -o admin admin.c  LibSerHV.o 
LibSerHV.o: LibSerHV.c LibSerHV.h data.h  
		cc -c LibSerHV.c 
HandleTCPClient.o: HandleTCPClient.c HandleTCPClient.h Requete.h data.h LibSerHV.h 
		gcc -c HandleTCPClient.c 
TCPEchoServer:	TCPEchoServer.c HandleTCPClient.o LibSerHV.o 
		gcc -o TCPEchoServer  TCPEchoServer.c  HandleTCPClient.o  LibSerHV.o 
TCPEchoClient:	TCPEchoClient.c Requete.h 
		gcc -o TCPEchoClient TCPEchoClient.c 