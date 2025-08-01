#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "data.h"

void AProposServeurHV(char *Version,char *Nom1,char* Nom2) ;

int RechercheMV(char* NomFichier,int Reference ,struct VehiculeHV *UnRecord);
void DieWithError(char *errorMessage) ;
