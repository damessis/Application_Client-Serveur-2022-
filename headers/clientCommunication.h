#ifndef CLIENT_COMMUNICATION
#define CLIENT_COMMUNICATION



/*------------------------------------Debut specification fonction receive_response0(int socket_client,char type_req)---------------------------------------------*/
//cette fonction prend la socket de service du client et le type de requete(pour effectuer le formatage associer) en parametre 
//et renvoie un entier 0 si la lecture et le formatage de la reponse est correctement effectué
//elle effectue une lecture dans la socket client range dans un tableau bidimensionnelle  de char ce qu'elle lit
//la premiere dimension contient le nombre de reponse associé a la requete
//(pour pouvoir traiter les requete qui recoivent plusieurs ligne de reponse) ou un code d'erreur que le serveur
//pourra envoyer si il n'est pas en mesure de satisfaire la requete
//apres la lecture elle effectue le formatage en faisant appel a la fonction format_result(char *resultat, char type_req) 
//et si au cours de cette lecture initiale on se rend compte que le nombre de reponse est plus grand que 1 
//on repete ce nombre - 1 fois le processus de lecture puis de formatage
int receive_response(int socket_client,int type_req);
/*--------------------------------------------------------------------Fin-----------------------------------------------------------------------------------------*/



/*---------------------------------------------------Debut specification fonction send_request(int socket_client)-------------------------------------------------*/
//cette fonction prend la socket de service du client en parametre et renvoie le type de requete qui est envoyé au serveur
//Elle demande par entrée clavier a l'utilisation d'entré le type de requete:
//1. Recherche par reference
//2. Requerche par mots cles
//3. Recherche par nom d'auteur et genre litteraire
//4. Recherche par nom d'auteur
// et 0. Couper la connection
//une fois dans chacune des type de requete, l'utilisateur a encore le choix de couper la connection(avec 0) ou de revenir au menu precedent(avec -1)
//il lui demande de saisir les differentes informations requise pour la recherche
//une fois ces deux champs obtenu elle le stocke dans un tableau de char qui sera passé au serveur a travers une ecriture dans la socket
int send_request(int socket_client,int type_req) ;
/*--------------------------------------------------------------------Fin------------------------------------------------------------------------------------------*/



/*--------------------Debut specification fonction format_result(char *resultat,char type_req)-----------------------------------------------------*/
//cette fonction prend un pointeur vers la sequence de char contenant la reponse et le type de requete(pour effectuer le formatage associer) en parametre 
//elemine les element indesirable dans la reponse tel que # puis effectue un switch sur le type de requete:
//le cas 1 correspond a une recherche par reference 
//le cas 2 correspond a une recherche par mot clé 
//le cas 3 correspond a une recherche par nom d'auteur et genre literraire
void format_result(char *resultat, int type_req) ;
/*--------------------------------------------------------------------Fin------------------------------------------------------------------------------------------*/

/*------------------------------------Debut specification fonction aphabetic_sort(char **list_of_responses)-----------------------------------------------------*/
 //cette fonction prend une liste de reponse en parametre et la tri par ordre alphabetique ascendant 
 // pour chaque reponse dans la liste, on les compare a tous les autre pour determiner l'ordre alphabetique
 //dans les details cette fonction recupere le nom et prenom d'une reponse numero i dans la liste puis ensuite fait 
 //de meme avec la reponse j=i+1 et alors grace a la fonction strcmp elle compare les deux si le mom et prenom dans  la reponse numero i est inferieur(alphabetiquement parlant) a la reponse numero j alors on ne bouge rien dans la liste 
 //et on compare la reponse numero i ensuite a la reponse numero j+1 puis la reponse numero j+2 jusqu'a la fin sinon si le nom et prenom de la reponse numero i est inferieur(alphabetiquement parlant) a la reponse numero j alors on met la reponse j dans une chaine temporaire puis on deplace la reponse numero i a la place de la reponse j et une fois cela on remet la reponse numero j qu'on avait proteger dans la chaine temporaire a l'anciennne place de la reponse i
 void aphabetic_sort(char **list_of_responses,int number_of_response);
/*--------------------------------------------------------------------Fin------------------------------------------------------------------------------------------*/
#endif
