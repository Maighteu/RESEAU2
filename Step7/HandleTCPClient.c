#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <string.h>
#include "LibSerHV.h"
#include "Requete.h"
#include "HandleTCPClient.h"

#define RCVBUFSIZE 32   /* Size of receive buffer */


void HandleTCPClient(int clntSocket)
{
    int recvMsgSize;                    /* Size of received message */
    struct Requete UneRequete ;
    struct VehiculeHV UnVehicule;
    int res,ach,fct;

    if ((recvMsgSize = recv(clntSocket, &UneRequete, sizeof (struct Requete), 0)) < 0)
        DieWithError("recv() failed");

    while(recvMsgSize>0) 
    {   
        switch(UneRequete.Type ) 
        { 
        case 1:  
            res = RechercheMV("VehiculesHV", UneRequete.Reference, &UnVehicule);
            printf("Reference reçue du client : %d\n", UneRequete.Reference);
            if (res == 1) {
                UneRequete.Reference = UnVehicule.Reference;
                strcpy(UneRequete.Constructeur, UnVehicule.Constructeur);
                strcpy(UneRequete.Modele, UnVehicule.Modele);
                strcpy(UneRequete.Transmission, UnVehicule.Transmission);
                UneRequete.Quantite = UnVehicule.Quantite;
                UneRequete.Type = OK;

            } 
            else 
            {
                UneRequete.Type = Fail;
                strcpy(UneRequete.Constructeur, "Non trouvé");
                strcpy(UneRequete.Modele, "Non trouvé");
                strcpy(UneRequete.Transmission, "Non trouvé");
                UneRequete.Quantite = 0;
            }
            break;
        case 2:
               res = RechercheMV("VehiculesHV",UneRequete.Reference ,&UnVehicule) ;
               AfficheRequete(stderr, UneRequete);
               /* reponse avec psor qui contient toujours l'adresse du dernier client */
 
               memset(&UneRequete,0, sizeof(struct Requete));
               if(res == 1) 
               {
                   // Reservation de l'achat
                   ach = ReservationHV("VehiculesHV", UneRequete.Reference , UneRequete.Quantite, &UnVehicule);

                   if(ach == 1)
                   {
                     printf("Mise à jour du fichier faite !\n");
                     time_t currentTime;
                     fct = FacturationHV("FactureHV", UneRequete.NomClient, currentTime, UneRequete.Quantite, UneRequete.Reference);
                   }
                   else
                     printf("Echec de la mise à jour dans le fichier...\n");
               }

               break;

        default : 
            fprintf(stderr,"Code incorrect %d\n",UneRequete.Type ) ;
        } 

    if (write(clntSocket, &UneRequete, recvMsgSize) != recvMsgSize)
            DieWithError("send() failed");
    /* See if there is more data to receive */
    if ((recvMsgSize = read(clntSocket, &UneRequete, sizeof (struct Requete))) < 0)
            DieWithError("recv() failed");
    }

    printf("Connexion Closed\n")  ;
    close(clntSocket);    /* Close client socket */
}
