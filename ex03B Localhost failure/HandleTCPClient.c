#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include "LibHV.h"

#define RCVBUFSIZE 32   /* Size of receive buffer */


void HandleTCPClient(int clntSocket)
{
    char echoBuffer[RCVBUFSIZE];        /* Buffer for echo string */
    int recvMsgSize;                    /* Size of received message */
    int sendMsgSize ; 			  /* Size of send message  */

    /* Receive message from client */
    if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
        DieWithError("recv() failed");
    else
       printf("Bytes received:%d\n", recvMsgSize) ;

    /* Send received string and receive again until end of transmission */
    while (recvMsgSize > 0)      /* zero indicates end of transmission */
    {
        /* Echo message back to client */
        
        sendMsgSize = send(clntSocket, echoBuffer, recvMsgSize, 0) ;
        if ( sendMsgSize != recvMsgSize)
            DieWithError("send() failed");
        else
           printf("Bytes send:%d\n", sendMsgSize) ;

        /* See if there is more data to receive */
        if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
            DieWithError("recv() failed");
        else
       	printf("Bytes received:%d\n", recvMsgSize) ;
    }

    close(clntSocket);    /* Close client socket */
}
