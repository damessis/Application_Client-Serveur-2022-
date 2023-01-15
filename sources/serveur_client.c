#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include "serveur_doc.c"

void serveur_client(int descripteur, struct sockaddr_in socket_addr_client)
{
    int fini = 0;
    while (fini == 0)
    {
        int num_requette=0;
        read(descripteur, &num_requette, sizeof(int));
        printf("1 read %i \n",num_requette);
        if (num_requette == 0)
        {
            fini = 1;
        }
        else if (num_requette == 1)
        {
            int ref;
            read(descripteur, &ref, sizeof(int));

            char *resultat;
            resultat = serveur_doc(1, ref,"");
            if (resultat[0] != '\n')
            {
                int nb_reponse=1;
                write(descripteur, &nb_reponse, sizeof(int));

                //compte la taille de la reponse
                int i = 0;
                while (resultat[i]!='\n') {
                    i++;
                }                
                //envoie la reponse
                write(descripteur, &i, sizeof(int));
                char reponse_1[i];
                for (int j = 0; j < i; j++)
                {
                    reponse_1[j] = resultat[j];
                }
                write(descripteur, &reponse_1, sizeof(reponse_1));

            }
            else
            {
                int nb_reponse=0;
                write(descripteur, &nb_reponse, sizeof(int));
            }            
            printf("fin de req 1 \n");
        }

        else if (num_requette == 2)
        {
            int taille_tab_client;
            read(descripteur, &taille_tab_client, sizeof(int));
            char valeur_requette[taille_tab_client];
            read(descripteur, &valeur_requette, sizeof(valeur_requette));

            printf("recoie %s \n",valeur_requette);

            char *resultat;
            resultat = serveur_doc(2,0, valeur_requette);
            // on a dans resultat la liste des references

            if (resultat[0] != '\n')//si reponse
            {
                int indice_dans_ref = 0;
                int taille_res=0;
                while(resultat[taille_res] != '\n') {
                    taille_res++;
                }
                char ref[taille_res];
                for (int j = 0; j < taille_res; j++)
                {
                    ref[j] = '\0';
                }
                int nb_res = 0;
                for (int i = 0; i < taille_res; i++)
                {
                    //compte le nombre de resultat
                    if (resultat[i] == '#')
                    {
                        nb_res++;
                    }
                }
                printf("on renvoie %i reponse depuis 2 \n",nb_res);
                write(descripteur, &nb_res, sizeof(int));

                for (int i = 0; i < taille_res; i++)
                {
                    if (resultat[i] == '#')
                    {
                        

                        char *resultat_une_ligne;
                        resultat_une_ligne = serveur_doc(1, atoi(ref),resultat_une_ligne);


                        int k = 0;
                        while (resultat_une_ligne[k]!='\n') {
                            k++;
                        }
                        write(descripteur, &k, sizeof(int));
                        char reponse_2[k];

                        for (int j = 0; j < k; j++)
                        {
                            reponse_2[j] = resultat_une_ligne[j];
                        }
                        printf("reponse = %s \n",reponse_2);

                        write(descripteur, &reponse_2, sizeof(reponse_2));

                        indice_dans_ref = 0;
                        for (int j = 0; j < 100; j++)
                        {
                            ref[j] = '\0';
                        }

                    }
                    else
                    {
                        ref[indice_dans_ref] = resultat[i];
                        indice_dans_ref++;
                    }
                }
            }

            else
            {
                int nb_reponse=0;
                write(descripteur, &nb_reponse, sizeof(int));
            }
            printf("fin de req 2 \n");
        }

        else if (num_requette == 3)
        {
            int taille_tab_client;
            read(descripteur, &taille_tab_client, sizeof(int));
            char valeur_requette[taille_tab_client];
            printf("la taille sera : %s \n",valeur_requette);
            read(descripteur, &valeur_requette, sizeof(valeur_requette));

            printf("on recoit %s \n",valeur_requette);

            char *resultat;
            resultat = serveur_doc(3,0, valeur_requette);
            // on a dans resultat la liste des references

            if (resultat[0] != '\n')
            {
                int indice_dans_ref = 0;
                int taille_res=0;
                while(resultat[taille_res] != '\n') {
                    taille_res++;
                }
                char ref[taille_res];
                for (int j = 0; j < taille_res; j++)
                {
                    ref[j] = '\0';
                }
                int nb_res = 0;
                for (int i = 0; i < taille_res; i++)
                {
                    if (resultat[i] == '#')
                    {
                        nb_res++;
                    }
                }
                write(descripteur, &nb_res, sizeof(int));

                for (int i = 0; i < taille_res; i++)
                {
                    if (resultat[i] == '#')
                    {
                        
                        ref[indice_dans_ref] = '\n'; // peut etre qu'on peut l'enlever
                        char *resultat_une_ligne;
                        resultat_une_ligne = serveur_doc(1, atoi(ref),resultat_une_ligne);

                        int k = 0;
                        while (resultat_une_ligne[k]!='\n') {
                            k++;
                        }
                        write(descripteur, &k, sizeof(int));

                        char reponse_2[k];

                        for (int j = 0; j < k; j++)
                        {
                            reponse_2[j] = resultat_une_ligne[j];
                        }
                        printf("reponse = %s \n",reponse_2);

                        write(descripteur, &reponse_2, sizeof(reponse_2));

                        indice_dans_ref = 0;
                        for (int j = 0; j < 100; j++)
                        {
                            ref[j] = '\0';
                        }

                    }
                    else
                    {
                        ref[indice_dans_ref] = resultat[i];
                        indice_dans_ref++;
                    }
                }
            }

            else
            {
                int nb_reponse=0;
                write(descripteur, &nb_reponse, sizeof(int));
            }
            printf("fin de req 3 \n");
        }

        else if (num_requette == 4)
        {

            int taille_tab_client;
            read(descripteur, &taille_tab_client, sizeof(int));
            char valeur_requette[taille_tab_client];
            read(descripteur, &valeur_requette, sizeof(valeur_requette));

            char *resultat;
            resultat = serveur_doc(4,0, valeur_requette);
            
            if (resultat[0] != '\n')
            {
                int taille_res=0;
                while(resultat[taille_res] != '\n'&&resultat[taille_res] != '#') {
                    taille_res++;
                }
                char ref[taille_res];
                for (int j = 0; j < taille_res; j++) //tant que on est pas arrivé a l'avant dernier
                {
                    ref[j] = resultat[j];
                }
                int nb_reponse=1;
                write(descripteur, &nb_reponse, sizeof(int));

                char *resultat_une_ligne;
                printf("4 ref = %i \n",atoi(ref));
                printf("4 ref = %s \n",ref);
                resultat_une_ligne = serveur_doc(1, atoi(ref),resultat_une_ligne);

                int k = 0;
                while (resultat_une_ligne[k]!='\n') {
                    k++;
                }
                write(descripteur, &k, sizeof(int));

                char reponse_4[k];

                for (int j = 0; j < k; j++)
                {
                    reponse_4[j] = resultat_une_ligne[j];
                }
                printf("reponse = %s \n",reponse_4);

                write(descripteur, &reponse_4, sizeof(reponse_4));

            }
            else {
                int nb_reponse=0;
                write(descripteur, &nb_reponse, sizeof(int));
            }
            
            printf("fin de req 4 \n");
        }

        else
        {
            printf("?????? \n");
        }
    }
    close(descripteur);
    exit(0);
}
