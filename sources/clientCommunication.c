
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "../headers/clientCommunication.h"



/*--------------------------------------------------------------------Debut fonction send_request-------------------------------------------------------------------------------------*/
int send_request(int socket_client,int request_number) {
    //variables permettant de stocker le type de requete et les information qui y sont associer
    int informations_size;
    int informations3_size;
    int request1_informations;
    char *request_informations = (char*)malloc(26);
    char *request3_informations = (char*)malloc(26);
    for(int i=0;i<sizeof(request3_informations);i++){
        request3_informations[i]='\0';
    }
    char *nom = (char*)malloc(26);
    char *genre = (char*)malloc(26);
    write(socket_client,&request_number,sizeof(int));
    //on lui demande ensuite d'entré les inforations de la requete
    if (request_number == 1) {
        printf("Entrez une reference obtenir l'affichage des informations "
            "associées au livre concerné \n");
        scanf("%d", &request1_informations);
        printf("La requete envoye(c'est une reference) : %d\n",request1_informations);
        //une fois un numero de reference entré on l'envoi au serveur a travers la socket;
        write(socket_client,&request1_informations,sizeof(int));
    }else if(request_number==3){
             printf("Entrez le nom de l'auteur ainsi que le genre litteraire pour obtenir les livres correspondant \n");
             printf("Nom de l'auteur (il peut s'gair du nom et prenom ou juste l'un des deux et bien evidement espacé):");
             //saisie du nom
             scanf(" %[^\n]", nom);
             printf("Genre :");
             //saisie du genre
             scanf(" %[^\n]", genre);//
             //on concatene toutes ces informations dans une seule chaine de caratère en prenant soin de glisser '#' entre le nom et le genre 
             //etant donné que le serveur recoit les informaions formaté ainsi
             strcat(request3_informations,nom);
             strcat(request3_informations,"#");
             strcat(request3_informations,genre);
             strcat(request3_informations,"#");
             strcat(request3_informations,"\n");
             //on recupere la taille des informations de la requete 
             //et on l'envoi au serveur a travers la socket;
             informations3_size=strlen(request3_informations);
             write(socket_client,&informations3_size,sizeof(int));
             printf("La requete envoye : %s\n",request3_informations);
             write(socket_client, request3_informations, informations3_size);
    }else{
             if(request_number == 2) {
               printf("Entrez des mots clés pour obtenir des informations associées aux livres dont le titre contient ce ou ces informations\n");
               printf("Mot-clés(pensez a les espacé si il y en a plus de 1) :\n");
               scanf(" %[^\n]", request_informations);
               for(int i=0;i<strlen(request_informations);i++){
                 if(isspace(request_informations[i])){
                  request_informations[i]='#';
                 }
               }
               strcat(request_informations,"#");
              strcat(request_informations,"\n");
              informations_size=strlen(request_informations);
              write(socket_client,&informations_size,sizeof(int));
              printf("La requete envoye : %s\n",request_informations);
              write(socket_client, request_informations, informations_size);
             }else if(request_number == 4) {
               printf("Entrez une auteur pour obtenir un de ses livres \n");
               printf("Nom de l'auteur (il peut s'gair du nom et prenom ou juste l'un des deux et bien evidement espacé):");
               scanf(" %[^\n]", request_informations);
               /*for(int i=0;i<strlen(request_informations);i++){
                 if(isspace(request_informations[i])){
                  request_informations[i]='#';
                 }
               }*/
              strcat(request_informations,"#");
              strcat(request_informations,"\n");
              informations_size=strlen(request_informations);
              write(socket_client,&informations_size,sizeof(int));
              printf("La requete envoye : %s\n",request_informations);
              write(socket_client, request_informations, informations_size);
             }
             //pour les requete 2 et 4 pour des soucis de formatage on recupere l'entrée de l'utilisateur 
             //et on remplace tous les espacement par des # avant de les envoyer au serveur
             
              //cette partie est specifique pour les requete 2 et 4 vu que son formatage 
              //avant envoie est different de celui des requetes 1 et 3
              //on met un '#' et un '\n' a la fin de la chaine d'informations de la requete qu'on a preformaté(deja avec la boucle for un peu plus haut)
              // tout cela dans le souci d'envoyer l'inforation sous le bon format lisible par le serveur  
              
              //on recupere la taille des informations de la requete 
              //et on l'envoi au serveur a travers la socket;
              //informations_size=strlen(request_informations);
              //write(socket_client,&informations_size,sizeof(int));
              //on envoie maintenant l'informations de la requete au serveur a travers la socket;
              //printf("La requete envoye : %s\n ",request_informations);
              //write(socket_client, request_informations, informations_size);
    }
    //ici on renvoie le  numero de la requete qui sera prise en parametre par le la fonction de reception de la reponse 
    //pour qu'elle puisse faire le traitement adequat
    return request_number;
}
/*--------------------------------------------------------------------Fin fonction send_request-------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------Debut fonction receive_response-------------------------------------------------------------------------------*/
int receive_response(int socket_client, int type_req) {
    //variables permettant de stocker 
    //pour stocker le nombre de reponse afin de pouvoir dimensionner dynamique la liste de reponse 
    //et si ce numero est egale a 0 alors c'est que le serveur n'a pas trouve de reponse a  notre requete
    int number_of_response;
    // cette variable nous permet de stocker la taille de la reponse courante afin de dimensionner 
    //dynamiquement la taille de la chaine de caractère qui devra recevoir la reponse
    int response_length;
    // cette variable nous permet de stocker la reponse courante recevoir la reponse
    char *response_informations;
    //cette variable nous permet de stocker une ou plusieurs reponse avant d'effectuer le formatage pour une presentation a 
    //l'auteur elle sera dimension dynamiquement en fonction du nombre de reponse et de la taille de chaque reponse
    char **list_of_responses;
    //on fait une lecture pour recuperer le nombre de reponse et on verfie si cette lecture s'est bien effectué
    if (read(socket_client, &number_of_response, sizeof(int)) != -1) {
        //on verifie que le nombre de reponse n'est pas egale a 0 car si c'est le cas 
        //c'est que le serveur n'a pas trouvé de reponse au a la requete
        if(number_of_response==0){
            printf("Aucune reponse n'a éte trouvé a votre requete\n");
            return 0;
        }else{
            int i = 0;
            //maintenant qu'on connait le nombre de reponse on peut dimensionner la list de reponse
            list_of_responses=(char**)malloc(number_of_response*sizeof(char*));
            //tant que le nombre de reponse restant n'est pas egale a 0 on :
            printf("nb reponse %i \n",number_of_response);
            while(i < number_of_response){
                //lit la taille de la reponse courante si cette lecture se passe bien,
                if(read(socket_client, &response_length, sizeof(int)) != -1){ 
                    //maintenant qu'on connait la taille de la reponse courante 
                    //on peut dimensionner dynamiquement l'espace qui devra accueillir 
                    //cette reponse seule et l'espace qui devra accueillir cette reponse dans la liste de reponse
                    response_informations=(char*)malloc(response_length*sizeof(char));
                    for(int i=0;i<response_length;i++){
                        response_informations[i]='\0';
                    }
                    //on lit la reponse elle meme et si cette lecture se deroule bien,
                    if(read(socket_client, response_informations, response_length) != -1){
                        //printf("La reponse brut recupéré %s\n",response_informations);
                        list_of_responses[i]=(char*)malloc(response_length*sizeof(char));
                        /*for(int j=0;j<response_length;i++){
                            list_of_responses[i][j]='\0';
                        }*/
                        //maintenant que la lecture de la reponse s'est effectué sans erreur 
                        //on affecte la reponse dans la liste de reponse
                        list_of_responses[i]=response_informations;

                    }else{
                        printf("read incorrect sur une reponse\n");
                        return -1;
                    }
                }
                i++;
            }
        }
        //maintenant qu'on a recuperer toute les informations dans la liste de reponse, on la passe a la fonction alphabetic_sort 
        //qui se charge de trié la liste dans un nom alphaabetique croissant sur le nom et prenom
        if(type_req==2){
            aphabetic_sort(list_of_responses, number_of_response);
        }
        //apres avoir recuperer toutes les reponses on les parcours et on effectue le traitement
        int i = 0;
        //une fois qu'on a obtenu une liste de reponse trié on la parcours et pour chaque reponse de cette liste 
        //on effectue le formatage pour l'affichage a l'utilisateur
        if(number_of_response>1){
            printf("Le serveur a trouvé '%d' reponses a votre requete \n\n\n",number_of_response);
        }else{
            printf("Le serveur a trouvé '%d' reponse a votre requete \n\n\n",number_of_response);
        }
        printf("--------------------------------Debut Affichage des reponse --------------------------------\n");
        while(i < number_of_response){
            printf("\n---------------------Debut Reponse %d---------------------\n",i+1);
            format_result(list_of_responses[i],type_req);
            printf("---------------------Fin Reponse %d ----------------------\n",i+1);
            i++;
        }
        printf("\n--------------------------------Fin Affichage des reponse ---------------------------------\n");
    } else {
        //si on a echoue a lire le nombre de reponse alos on renvoie un message d'erreur
        perror("read");
        return -1;
    }
    //si tout se deroule bien pour la recuperation et le formatage de la reponse on renvoie 0
    return 0;
}
/*--------------------------------------------------------------------Fin fonction receive_response---------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------Debut fonction format_result---------------------------------------------------------------------------------*/
void format_result(char *resultat, int type_req) {
    char delim[]="#";
    char *ptr = strtok(resultat, delim);
    switch(type_req){
        //dans le cas 1 on affiche pour un livre pour lequel on a fourni une reference specifique
        //Nom de l'auteur
        //Titre du livre
        //Genre du livre
        //et plus de 300 pages si le champs dedié au nombre de page dans la reponse est f
        //sinon moins de 300 pages
        case 1:
            ptr = strtok(NULL, delim);
            if(ptr!= NULL){
            printf("Nom de l'auteur : %s\n",ptr);
            }else{
            printf("Nom de l'auteur renvoyé par le serveur est null: \n");
            }
            ptr = strtok(NULL, delim);
            if(ptr!= NULL){
            printf("Titre du livre : %s\n", ptr);
            }else{
            printf("Titre du livre renvoyé par le serveur est null: \n");
            }
            ptr = strtok(NULL, delim);
            if(ptr!= NULL){
            printf("Genre : %s\n", ptr);
            }else{
            printf("Le genre renvoyé par le serveur est null: \n");
            }
            ptr = strtok(NULL, delim);
            if(strcmp(ptr,"f")){
                printf("Plus de 300 pages\n");
            }else{
                printf("Moins de 300 pages\n");
            }
            break;
        //dans le cas 2 on affiche pour tous les livres dont le titre contient un ou plusieurs des mot clé 
        //qu'on a fourni au serveur et tout cela trié par ordre alphabetique ascendant(l'ordre est appliqué sur le nom puis sur les prenoms
        //Reference
        //Nom de l'auteur
        //Titre du livre
        //Genre
        case 2:
            if(ptr!= NULL){
            printf("Reference : %s\n",ptr);
            }else{
            printf("La reference renvoyé par le serveur est null: \n");
            }
            ptr = strtok(NULL, delim);
            if(ptr!= NULL){
            printf("Nom de l'auteur : %s\n",ptr);
            }else{
            printf("Nom de l'auteur renvoyé par le serveur est null: \n");
            }
            ptr = strtok(NULL, delim);
            if(ptr!= NULL){
            printf("Titre du livre : %s\n", ptr);
            }else{
            printf("Titre du livre renvoyé par le serveur est null: \n");
            }
            ptr = strtok(NULL, delim);
            if(ptr!= NULL){
            printf("Genre : %s\n", ptr);
            }else{
            printf("Le genre renvoyé par le serveur est null: \n");
            }
            break;
        //dans le cas 3 on affiche pour tous les livres qui correspondent au nom d'auteur(nom et ou prenom) et genre qu'on a fourni au serveur 
        //Reference
        //Titre du livre
        case 3:
            if(ptr!= NULL){
            printf("Reference : %s\n",ptr);
            }else{
                printf("La reference renvoyé par le serveur est null: \n");
            }
            ptr = strtok(NULL, delim);
            ptr = strtok(NULL, delim);
            if(ptr!= NULL){
            printf("Titre du livre : %s\n", ptr);
            }else{
            printf("Titre du livre renvoyé par le serveur est null: \n");
            }
            break;
        //dans le cas 4 la reponse est un livre choisi(sur base d'un critère determiné par le serveur) 
        //Reference
        //Nom de l'auteur
        //Titre du livre
        //Genre
        case 4:
            ptr = strtok(NULL, delim);
            if(ptr!= NULL){
            printf("Nom de l'auteur : %s\n",ptr);
            }else{
            printf("Nom de l'auteur renvoyé par le serveur est null: \n");
            }
            ptr = strtok(NULL, delim);
            if(ptr!= NULL){
            printf("Titre du livre : %s\n", ptr);
            }else{
            printf("Titre du livre renvoyé par le serveur est null: \n");
            }
            ptr = strtok(NULL, delim);
            if(ptr!= NULL){
            printf("Genre : %s\n", ptr);
            }else{
            printf("Le genre renvoyé par le serveur est null: \n");
            }
            ptr = strtok(NULL, delim);
            if(ptr[0]=='f'){
                printf("Plus de 300 pages\n");
            }else{
                printf("Moins de 300 pages\n");
            }
            break;
        //on n'est pas censé arrivé dans ce cas car si on choisi un type de requete different 1,2,3,4 alors 
        //le progamme nous redemande immediatement de resasir une nouvelle requete ce qui fait 
        //qu'on ne fera pas appel aux fonction send_request puis receive_response puis format_result 
        //et si on saisi 0 on sort du programme et de cette facon aussion ne fera pas appel aux fonction send_request puis receive_response puis format_result
        default:
            printf("Action non prise en charge");
            break;
    }
}
/*--------------------------------------------------------------------Fin fonction format_result-----------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------Debut fonction aphabetic_sort------------------------------------------------------------------------------- */
void aphabetic_sort(char **list_of_responses, int number_of_response){
    //char **list_of_responses_sorted=(char **) malloc(number_of_response*sizeof(char*));
    char tok1[100];
    char tok2[100];
    char name1[100];
    char name2[100];
    char lower[100];
    strcpy(lower,list_of_responses[0]);
    for(int i=0;i<number_of_response-1;i++){
        //recuperation du nom de l'auteur i
        strcpy(tok1,list_of_responses[i]);
        strtok(tok1,"#");
        strcpy(name1,strtok(NULL,"#"));
        for(int j=i+1;j<number_of_response;j++){
            //recuperation du nom de l'auteur suivant
            strcpy(tok2,list_of_responses[j]);
            strtok(tok2,"#");
            strcpy(name2,strtok(NULL,"#"));
            if(strcmp(name1,name2)>0){
                strcpy(lower,list_of_responses[j]);
                strcpy(list_of_responses[j],list_of_responses[i]);
                strcpy(list_of_responses[i],lower);
            }else{
                //strcpy(lower,list_of_responses[i]);
            }
            
        }
        
    }
    
}
 
/*--------------------------------------------------------------------Fin fonction aphabetic_sort-----------------------------------------------------------------------------------*/