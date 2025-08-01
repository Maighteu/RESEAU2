#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <netdb.h>

#define MAXPENDING 5    /* Maximum outstanding connection requests */

#include "LibSerHV.h"
#include "HandleTCPClient.h"   /* TCP client handling function */

int main(int argc, char *argv[])
{
    int servSock;                    /* Socket descriptor for server */
    int clntSock;                    /* Socket descriptor for client */
    char *servIP;  
    struct sockaddr_in echoServAddr; /* Local address */
    struct sockaddr_in echoClntAddr; /* Client address */
    unsigned short echoServPort;     /* Server port */
    unsigned int clntLen;            /* Length of client address data structure */

    char host[NI_MAXHOST];
    char port[NI_MAXSERV];
    struct addrinfo hints;           /* Structure containing additional address informations */
    struct addrinfo *results;        /* Structure containing the results of the address resolution */
    struct addrinfo *info;           /* Structure used to display the results of the address resolution */


    if (argc != 3)     /* Test for correct number of arguments */
    {
        fprintf(stderr, "Usage:  %s <Server Port>\n", argv[0]);
        exit(1);
    }
    /* Address resolution for the address given in the parameters */
    memset(&hints, 0, sizeof(struct addrinfo)); /* Initialisation to 0 */
    /* Add some information about the address */
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV; /* Ask to not resolve port number */

    if (getaddrinfo(argv[1], argv[2], &hints, &results) != 0)
    {
        printf("(Error) getaddrinfo() failed\n");
    }
    else
    {
        /* Display host name and port number */
        for (info = results; info != NULL; info = info->ai_next)
        {
          getnameinfo(info->ai_addr, info->ai_addrlen, host, NI_MAXHOST, port, NI_MAXSERV, 0);
          printf("Host: %s --- Service: %s\n", host, port);
        }
        freeaddrinfo(results);
    }
    /* Set variables */
    struct sockaddr_in *internet_addr = (struct sockaddr_in *)results->ai_addr;
    servIP = inet_ntoa(internet_addr->sin_addr);           /* First arg: server IP address (dotted quad) */
    echoServPort = atoi(argv[2]); /* Second arg: server Port */

    /* Create socket for incoming connections */
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");
    else
       printf("socket() Ok\n") ;
      
    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP); /* Server Ip address String to Ipv4 */
    echoServAddr.sin_port = htons(echoServPort);      /* Local port */

    /* Bind to the local address */
    if (bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("bind() failed");
    else
       printf("bind() Ok\n") ;    

    /* Mark the socket so it will listen for incoming connections */
    if (listen(servSock, MAXPENDING) < 0)
        DieWithError("listen() failed");
    else
       printf("listen() Ok\n") ;     

    while(1) /* Run forever */
    {
        /* Set the size of the in-out parameter */
        clntLen = sizeof(echoClntAddr);

        /* Wait for a client to connect */
        if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, 
                               &clntLen)) < 0)
            DieWithError("(Error) accept() failed\n");
        else
           printf("(Success) accept() OK\n") ;     

        /* clntSock is connected to a client! */
        /* Getting client informations */
        if (getpeername(clntSock, (struct sockaddr *)&echoClntAddr, &clntLen) == 0)
            if (getnameinfo((struct sockaddr *)&echoClntAddr, clntLen, host, sizeof(host), port, sizeof(port), NI_NUMERICHOST | NI_NUMERICSERV) == 0)
                printf("(Info) Client connected --> IP address : %s, Port : %s\n", host, port);
            else
                printf("(Error) Impossible to get precise client informations\n");
        else
            printf("(Error) Impossible to get client informations\n");

        printf("(Process) Handling client %s...\n", inet_ntoa(echoClntAddr.sin_addr));

        HandleTCPClient(clntSock);
    }
    /* NOT REACHED */
}
