#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#include <netdb.h>      /* For Getinfo */

#define RCVBUFSIZE 32   /* Size of receive buffer */

void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sock;                        /* Socket descriptor */
    unsigned short echoServPort;     /* Echo server port */
    char *hostName ;	              /* hostName Server */
    struct sockaddr_in *echoServAddr; /*  Pointer to Local address */
    char *servIP;                    /* Server IP address (dotted quad) */
    char *echoString;                /* String to send to echo server */
    char echoBuffer[RCVBUFSIZE];     /* Buffer for echo string */
    unsigned int echoStringLen;      /* Length of string to echo */
    int bytesRcvd, totalBytesRcvd;   /* Bytes read in single recv() 
                                        and total bytes read */

    if ((argc < 4) || (argc > 4))    /* Test for correct number of arguments */
    {
       fprintf(stderr, "Usage: %s <Server IP> <Echo Port>] <Echo Word> \n",
               argv[0]);
       exit(1);
    }

    hostName= argv[1];             /* First arg: server IP address (dotted quad) */
    echoServPort = atoi(argv[2]);   /* Second arg: server Port */
    echoString = argv[3];         /* Third arg: string to echo */
    
    struct addrinfo* addr;
    
    printf("Par:%s \n",hostName ) ;
        int result = getaddrinfo(hostName, NULL, NULL, &addr); /* String to sockaddr_in */
 	 if (result != 0) {
    		printf("Error from getaddrinfo: %s\n", gai_strerror(result));
  	}
 	 else {

	      echoServAddr  = (struct sockaddr_in*) addr->ai_addr; /* Pointer to sockaddr_in Local address IP */
 	      echoServAddr->sin_family = AF_INET;
 	      echoServAddr->sin_port        = htons(echoServPort); /* Server port */
	      servIP = inet_ntoa(echoServAddr->sin_addr) ;
  	      printf("getaddrinfo: %s\n", servIP);
      }

    /* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) echoServAddr, sizeof(struct sockaddr_in )) < 0)
        DieWithError("connect() failed");

    echoStringLen = strlen(echoString);          /* Determine input length */

    /* Send the string to the server */
    if (send(sock, echoString, echoStringLen, 0) != echoStringLen)
        DieWithError("send() sent a different number of bytes than expected");

    /* Receive the same string back from the server */
    totalBytesRcvd = 0;
    printf("Received: ");                /* Setup to print the echoed string */
    while (totalBytesRcvd < echoStringLen)
    {
        /* Receive up to the buffer size (minus 1 to leave space for
           a null terminator) bytes from the sender */
        if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0)
            DieWithError("recv() failed or connection closed prematurely");
        totalBytesRcvd += bytesRcvd;   /* Keep tally of total bytes */
        echoBuffer[bytesRcvd] = '\0';  /* Terminate the string! */
        printf("%s", echoBuffer);      /* Print the echo buffer */
    }

    printf("\n");    /* Print a final linefeed */

    close(sock);
    exit(0);
}
