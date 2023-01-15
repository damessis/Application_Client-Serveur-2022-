
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h> /* See NOTES */
#include <unistd.h>

#include "clientCommunication.c"

int main(int argc, char *argv[]) {
  //variable contenant le nom de l'hote(machine serveur)pour y acceder
  char hostname[30];
  if(argc<3){
    perror("Format incorret\nLe format correct est ./client <nom de l'hote><port> \nLe nom de l'hote est sous la forme : <nom batiment> <numero salle> <-> <numero de poste> \n");
    exit(-1);
  }
  strcpy(hostname,argv[1]);
  if(strlen(argv[1])==7 && argv[1][0]=='f'){
    strcat(hostname,".e-im2ag.ujf-grenoble.fr");
  }
  //varaible de navigation
  char request_number;
  //variable pour savoir si on est arrivé a la fin de la communication ou non
  bool fin = false;
  if (fin) {
    printf("un warning de moins");
  }
  //variable nous permettant de garder le numero de la requete pour pouvoir effectuer le formatage associé
  int type_req=0;
  // déclaration et création d'une socket p, de type SOCK_STREAM, pour ledomaine
  // AF_INET
  int socket_client = socket(AF_INET, SOCK_STREAM, 0);
  // déclaration d'une variable host de type struct hostent *
  struct hostent *host;
  // déclaration d'une variable s de type struct sockaddr_in
  struct sockaddr_in socket_server;
  // declaration du nom du serveur
  // char *name_server;
  // récupération dans h des informations concernant le serveur (utili-sation de
  // gethostbyname)

  host = gethostbyname(hostname);

  // préparation des champs sin_family, sin_port et sin_addr.s_addr de
  // lavariable s le champ sin_family reçoit ici la valeur AF_INET
  socket_server.sin_family = AF_INET;
  // le champ sin_addr.s_addr recevra une valeur déduite du champh_addr de la
  // structure contenant les informations sur le serveur h
  memcpy(&socket_server.sin_addr.s_addr, host->h_addr_list[0],8);
  // le champ sin_port recevra la valeur htons(noport) où noport sera le numéro
  // de port choisi (par le serveur)
  //socket_server.sin_port = htons(4999);
  //on choisi le port en en ligne de commande
  socket_server.sin_port = htons(atoi(argv[2]));
  // demande de connexion en utilisant la socket p et la structure définie dans
  // s (utilisation de connect)
  int client_connect = connect(socket_client, (struct sockaddr *)&socket_server,
                               sizeof(socket_server));
  if (client_connect == -1) {
    perror("connect");
    exit(0);
  } else {
      //menu
        printf("\nBonjour et bienvenue\n");
        printf("0. Couper la connection\n");
        printf("1. Recherche par reference\n");
        printf("2. Requerche par mots cles\n");
        printf("3. Recherche par nom d'auteur et genre litteraire\n");
        printf("4. Recherche par nom d'auteur\n");
        printf("Quelle est votre requette : ");
        scanf("%s", &request_number);
        while(request_number!='0'){
         if(request_number=='1' || request_number=='2' || request_number=='3' || request_number=='4' ){
            printf("\n-------------------------------------------------------Début de requete---------------------------------------------------------\n");
            //on envoie la requete au client
            send_request(socket_client,atoi(&request_number));
            //on recoit la reponse du client puis on formate le resultat
            type_req = receive_response(socket_client,atoi(&request_number));
            if(type_req==-1) {
              printf("Erreur vous êtes maintenant deconnecter \n");
              request_number='0';
              break;
            }
            printf("\n-------------------------------------------------------Fin de requete-----------------------------------------------------------\n\n\n\n");
            printf("0. Couper la connection\n");
            printf("1. Recherche par reference\n");
            printf("2. Requerche par mots cles\n");
            printf("3. Recherche par nom d'auteur et genre litteraire\n");
            printf("4. Recherche par nom d'auteur\n");
            printf("Quelle est votre requette : ");
            scanf("%s", &request_number);
         }else{
          printf("Valeur de requete non prise en charge;veuillez entrez un entier compris entre 0 et 4\n");
          printf("0. Couper la connection\n");
          printf("1. Recherche par reference\n");
          printf("2. Requerche par mots cles\n");
          printf("3. Recherche par nom d'auteur et genre litteraire\n");
          printf("4. Recherche par nom d'auteur\n");
          printf("Quelle est votre requette : ");
          scanf("%s", &request_number);
         }
          
        }
        if(request_number=='0'){
          printf("Au revoir \n");
          exit(0);

        }
  }
  return 0;
}