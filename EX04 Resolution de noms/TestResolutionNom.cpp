#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h> // pour memset
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <netinet/in.h>
#include <arpa/inet.h>
int main()
{
// Pour la recherche
struct addrinfo hints;
struct addrinfo *results;
// Pour l'affichage des resultats
char host[NI_MAXHOST];
char port[NI_MAXSERV];
struct addrinfo* info;
// On fournit l'hote et le service
memset(&hints,0,sizeof(struct addrinfo)); // initialisation à 0
hints.ai_family = AF_INET;
hints.ai_socktype = SOCK_STREAM;
printf("Pour www.google.be avec le service http :\n");
if (getaddrinfo("www.google.be","http",&hints,&results) != 0)
printf("Erreur de getaddrinfo");
else
{
// Affichage du contendu des adresses obtenues au format numérique
for (info = results ; info != NULL ; info = info->ai_next)
{
getnameinfo(info->ai_addr,info->ai_addrlen,
host,NI_MAXHOST,
port,NI_MAXSERV,
NI_NUMERICSERV | NI_NUMERICHOST);
printf("Adresse IP: %s -- Port: %s\n",host,port);
}
freeaddrinfo(results);
}
// On fournit l'adresse IP et le port directement
memset(&hints,0,sizeof(struct addrinfo)); // initialisation à 0
hints.ai_family = AF_INET;
hints.ai_socktype = SOCK_STREAM;
hints.ai_flags = AI_NUMERICSERV | AI_NUMERICHOST ;         // On a supprimé la résolution du port
printf("Pour inet 192.168.1.56 : 1300  :\n");
if (getaddrinfo("192.168.1.56","1300",&hints,&results) != 0)
printf("Erreur de getaddrinfo");
else
{
// Affichage du contendu des adresses obtenues au format "hote" et "service"
for (info = results ; info != NULL ; info = info->ai_next)
{
getnameinfo(info->ai_addr,info->ai_addrlen,
host,NI_MAXHOST,
port,NI_MAXSERV,
0);
printf("Hote: %s -- Service: %s\n",host,port);
}
freeaddrinfo(results);
}

// le localhost
{
struct addrinfo* addr;
  int result = getaddrinfo("localhost", NULL, NULL, &addr);
  if (result != 0) {
    printf("Error from getaddrinfo: %s\n", gai_strerror(result));
  }
  else {
  struct sockaddr_in* internet_addr = (struct sockaddr_in*) addr->ai_addr;
  printf("Localhost is at: %s\n", inet_ntoa(internet_addr->sin_addr));
}
}

{
struct addrinfo* addr;
  int result = getaddrinfo("vanstap2", NULL, NULL, &addr);
  if (result != 0) {
    printf("Error from getaddrinfo: %s\n", gai_strerror(result));
  }
  else {
  struct sockaddr_in* internet_addr = (struct sockaddr_in*) addr->ai_addr;
  printf("vanstap2 is at: %s\n", inet_ntoa(internet_addr->sin_addr));
}
}


exit(0);
}
