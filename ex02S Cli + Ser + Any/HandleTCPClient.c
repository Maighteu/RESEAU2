#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <string.h>
#include "LibSer.h"
#include "Requete.h"

#define RCVBUFSIZE 32   /* Size of receive buffer */


void HandleTCPClient(int clntSocket)
{
    int recvMsgSize;                    /* Size of received message */
    struct Requete UneRequete ;

    /* Receive message from client */
    if ((recvMsgSize = recv(clntSocket, &UneRequete, sizeof (struct Requete), 0)) < 0)
        DieWithError("recv() failed");
    while(recvMsgSize>0) 
    {   
      printf("#%s\n",UneRequete.Chaine) ;       
    /* Echo message back to client */
    if (write(clntSocket, &UneRequete, recvMsgSize) != recvMsgSize)
            DieWithError("send() failed");
    
    /* See if there is more data to receive */
    if ((recvMsgSize = read(clntSocket, &UneRequete, sizeof (struct Requete))) < 0)
            DieWithError("recv() failed");
    }
    printf("Connexion Closed\n")  ;
    close(clntSocket);    /* Close client socket */
}
