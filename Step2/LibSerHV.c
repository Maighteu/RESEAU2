#include "LibSerHV.h"

void AProposServeurHV(char *Version,char *Nom1,char* Nom2)
{
 printf("Version : %s \n",Version ) ;
 printf("Nom1 : %s \n",Nom1 ) ;
 printf("Nom2 : %s \n",Nom2 ) ;
}


int RechercheMV(char* NomFichier,int Reference ,struct VehiculeHV *UnRecord)
{
    FILE* fp = NULL;
    int i = 0;

    fp = fopen(NomFichier, "rb");

    if(!fp)
    {
        fp = fopen(NomFichier, "wb");
        return 0;
    }

    while(fread(UnRecord, sizeof(struct VehiculeHV), 1, fp) == 1)
    {
        if(UnRecord->Reference == Reference) 
        {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}
void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}
