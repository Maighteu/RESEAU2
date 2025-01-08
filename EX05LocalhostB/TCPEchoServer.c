#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#include <netdb.h>      /* For Getinfo */


#define MAXPENDING 5    /* Maximum outstanding connection requests */

#include "LibSer.h"  	/* Error handling function */
#include "HandleTCPClient.h"   /* TCP client handling function */

int main(int argc, char *argv[])
{
    int servSock;                    /* Socket descriptor for server */
    int clntSock;                    /* Socket descriptor for client */
    char* hostName ;                   /* Hostname to resolve */
    char *servIP;                    /* Server IP address (dotted quad) */
    struct sockaddr_in *echoServAddr; /*  Pointer to Local address */
    struct sockaddr_in echoClntAddr; /* Client address */
    unsigned short echoServPort;     /* Server port */
    unsigned int clntLen;            /* Length of client address data structure */
                                     /* Hostname resolution */
                                     // Pour la recherche 
                                     

    if (argc != 3)     /* Test for correct number of arguments */
    {
        fprintf(stderr, "Usage:  %s <server Ip address> <Server Port>\n", argv[0]);
        exit(1);
    }
    
    
    hostName = argv[1] ;       /* First argument server Ip address */  	
    echoServPort = atoi(argv[2]);  /* Second argument local port */

    /* resolve hostName */  
   
	struct addrinfo* addr;
  	int result = getaddrinfo(hostName, NULL, NULL, &addr); /* String to sockaddr_in */
 	 if (result != 0) {
    		printf("Error from getaddrinfo: %s\n", gai_strerror(result));
 	      exit(0) ;
  	}
 	 else {

 	   /* memset(&echoServAddr, 0, sizeof(echoServAddr));    Zero out structure */
 	                  
	      echoServAddr  = (struct sockaddr_in*) addr->ai_addr; /* Pointer to sockaddr_in Local address IP */
 	      echoServAddr->sin_family = AF_INET; /* Interenet address family */
	      servIP = inet_ntoa(echoServAddr->sin_addr) ;
  	    printf("getaddrinfo: %s\n", servIP);
  	    echoServAddr->sin_port = htons(echoServPort);      /* We add Local port */ 
      }

    /* Create socket for incoming connections */
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");
    else
       printf("socket() Ok\n") ;
       

    /* Bind to the local address */
    if (bind(servSock, (struct sockaddr *) echoServAddr, sizeof(struct sockaddr_in)) < 0) /* Now echoserver is a pointer */
        DieWithError("bind() failed");
    else
       printf("bind() Ok\n") ;    

    /* Mark the socket so it will listen for incoming connections */
    if (listen(servSock, MAXPENDING) < 0)
        DieWithError("listen() failed");
    else
       printf("listen() Ok\n") ;     

    for (;;) /* Run forever */
    {
        /* Set the size of the in-out parameter */
        clntLen = sizeof(echoClntAddr);

        /* Wait for a client to connect */
        if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, 
                               &clntLen)) < 0)
            DieWithError("accept() failed");
        else
           printf("accept() Ok\n") ;     


        /* clntSock is connected to a client! */

        printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

        HandleTCPClient(clntSock);
    }
    /* NOT REACHED */
}
