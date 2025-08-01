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
int ReservationHV(char* NomFichier, int Reference , int Quantite, struct VehiculeHV *UnRecord)
{
    FILE* fp = NULL;
    int i = 0;

    fp = fopen(NomFichier, "rb+");

    if(!fp)
    {
        fp = fopen(NomFichier, "wb");
        return 0;
    }

    while(fread(UnRecord, sizeof(struct VehiculeHV), 1, fp) == 1)
    {
        if(UnRecord->Reference == Reference) 
        {
            // Si la quantité est suffisante dans le fichier
            if(UnRecord->Quantite >= Quantite)
            {
                UnRecord->Quantite -= Quantite;

                // Revenir à la position du début de l'enregistrement
                fseek(fp, -sizeof(struct VehiculeHV), SEEK_CUR);

                // Écrire la mise à jour dans le fichier
                fwrite(UnRecord, sizeof(struct VehiculeHV), 1, fp);

                fclose(fp);
                return 1;
            }
            
        }
    }
    fclose(fp);
    return 0;
}

int FacturationHV(char NomFichier[80], char NomClient[60], time_t Date, int Quantite, int Reference) 
{
    FILE* fichier = fopen(NomFichier, "ab");

    if (!fichier) 
    {
        perror("Erreur lors de l'ouverture du fichier de factures");
        return -1;
    }

    struct FactureHV nouvelleFacture;

    fseek(fichier, 0, SEEK_END);
    nouvelleFacture.NumeroFacturation = ftell(fichier) / sizeof(struct FactureHV) + 1;
    strcpy(nouvelleFacture.Acheteur, NomClient);
    nouvelleFacture.DateFacturation = Date;
    nouvelleFacture.Quantite = Quantite;
    nouvelleFacture.Reference = Reference;
    time(&nouvelleFacture.DateFacturation);

    // Écrire la nouvelle facture dans le fichier
    fwrite(&nouvelleFacture, sizeof(struct FactureHV), 1, fichier);

    fclose(fichier);

    return 0;
}
