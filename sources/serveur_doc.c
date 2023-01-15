
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

char *serveur_doc(int nb_recherche, int ref, char *recherche) {

  char *resultat;
  resultat = (char *)malloc(1000 * sizeof(char));
  int taille_malloc = 1000;
  for (int i = 0; i < 100; i++) {
    resultat[i] = '\0';
  }

  FILE *file;
  file = fopen("doc/documentaire.txt", "r");

  if (file != NULL) {
    switch (nb_recherche) {
      /*p
      met dans resultat le nom si il est sur la ligne puis # puis le titre si il
      y est puis # genre # t si page <300 f sinon
      */
    case 1: {
      if (ref != 0) {
        char valeur_ref[10000];
        int i = 0;
        char caractereActuel;
        int trouve = 0;
        int fini = 0;
        /*
        int j = 0;
        while (recherche[j] != '\n')
        {
            j++;
        }*/
        // recherche si le numero demander est dans le document
        while (trouve != 1 && fini != 1) {
          i = 0;
          do {
            caractereActuel = fgetc(file); // On lit le caractère
            valeur_ref[i] = caractereActuel;
            if (caractereActuel == EOF) {
              fini = 1;
            }
            i++;
          } while (caractereActuel != '#' && caractereActuel != EOF &&
                   caractereActuel != '\n');
          char **endPtr=NULL;
          int nb_taille_tmp = 0;


          //test de si la reference de la ligne est la bonne
          while (valeur_ref[nb_taille_tmp] != '#') {
            nb_taille_tmp++;
          }
          if (nb_taille_tmp > 0) {
            char tmp[nb_taille_tmp - 1];
            for (int taille_tmp = 0; taille_tmp < nb_taille_tmp; taille_tmp++) {
              tmp[taille_tmp] = valeur_ref[taille_tmp];
            }
            long val_ref = strtol(tmp, endPtr, 10);
            if (ref == val_ref) {
              trouve = 1;
            }
          }
          /*
          traitement dans le cas ou le fichier demandé soit trouvé; on revoie le
          reste sous la forme approprié
          */
          if (trouve == 0) {
            do {

              if (caractereActuel == EOF) {
                fini = 1;
              } else {
                caractereActuel = fgetc(file);
              }

            } while (caractereActuel != '\n' && caractereActuel != EOF);
          }
        }
        if (trouve) {
          int k = 0;
          while (k < i) {
            resultat[k] = valeur_ref[k];
            k++;
          }

          caractereActuel = fgetc(file);

          while (caractereActuel != '#' && caractereActuel != EOF &&
                 caractereActuel != '\n') {
            if (i > taille_malloc) {
              taille_malloc += 1000;
              resultat = (char *)realloc(resultat, taille_malloc * sizeof(int));
            }
            resultat[i] = caractereActuel; // on ajoute l'auteur
            caractereActuel = fgetc(file); // On lit le caractère
            i++;
          }
          if (caractereActuel ==
              '#') // on verifie que on est arrivé a la fin du champ auteur
          {
            if (i > taille_malloc) {
              taille_malloc += 1000;
              resultat = (char *)realloc(resultat, taille_malloc * sizeof(int));
            }
            resultat[i] = caractereActuel; // on separe les deux partie
            i++;
            caractereActuel = fgetc(file);
            while (caractereActuel != '#' && caractereActuel != EOF &&
                   caractereActuel != '\n') {
              if (i > taille_malloc) {
                taille_malloc += 1000;
                resultat =
                    (char *)realloc(resultat, taille_malloc * sizeof(int));
              }

              resultat[i] = caractereActuel; // on ajoute le titre
              caractereActuel = fgetc(file); // On lit le caractère
              i++;
            }

            if (caractereActuel == '#') // on verifie que on est arrivé a la fin du champ titre
            {
              if (i > taille_malloc) {
                taille_malloc += 1000;
                resultat =
                    (char *)realloc(resultat, taille_malloc * sizeof(int));
              }
              resultat[i] = caractereActuel; // on separe les deux partie
              i++;
              caractereActuel = fgetc(file);
              while (caractereActuel != '#' && caractereActuel != EOF &&
                     caractereActuel != '\n') {
                if (i > taille_malloc) {
                  taille_malloc += 1000;
                  resultat =
                      (char *)realloc(resultat, taille_malloc * sizeof(int));
                }
                resultat[i] = caractereActuel; // on ajoute le genre
                caractereActuel = fgetc(file); // On lit le caractère
                i++;
              }

              if (caractereActuel ==
                  '#') // on verifie que on est arrivé a la fin du champ genre
              {
                if (i > taille_malloc) {
                  taille_malloc += 1000;
                  resultat =
                      (char *)realloc(resultat, taille_malloc * sizeof(int));
                }
                resultat[i] = caractereActuel; // on separe les deux partie
                i++;
                caractereActuel = fgetc(file);
                char test_superieur_300[3];
                int int_superieur_300 = 0;
                while (caractereActuel != '#' && caractereActuel != EOF &&
                       caractereActuel != '\n') {
                  test_superieur_300[int_superieur_300] = caractereActuel;
                  caractereActuel = fgetc(file); // On lit le caractère
                  int_superieur_300++;
                }
                if (int_superieur_300 <= 2 ||
                    (int_superieur_300 == 3 &&
                     (test_superieur_300[0] == '1' ||
                      test_superieur_300[0] == '2' ||
                      test_superieur_300[0] == '0'))) {
                  resultat[i] = 't';
                } else {
                  resultat[i] = 'f';
                }
                i++;
              }
            }
          }
          resultat[i] = '\n';
        }

        else {
          resultat[0] = '\n';
        }
        
      } else {
        resultat[0] = '\n';
      }
      break;
    }
    case 2: {

      int indice_dans_res = 0;
      int nb_mot_cles = 0;
      for (int i = 0; i < 100; i++) {
        if (recherche[i] == '#') {
          nb_mot_cles++;
        }
        if (recherche[i] == '\n') {
          break;
        }
      }
      char mot_cles[nb_mot_cles][100];

      for (int clean = 0; clean < nb_mot_cles; clean++) // on clean
      {
        for (int clean2 = 0; clean2 < 100; clean2++) {
          mot_cles[clean][clean2] = '\0';
        }
      }

      // on remplie le tableau de mot cles
      int numero_mot_cles = 0;
      int j = 0;
      for (int i = 0; i < 100; i++) {
        if (recherche[i] == '\n') {
          break;
        }
        if (recherche[i] == '#') {
          numero_mot_cles++;
          // mot_cles[numero_mot_cles][j]=recherche[i];
          j = 0;
        } else {
          mot_cles[numero_mot_cles][j] = recherche[i];
          j++;
        }
      }

      // recherche de livre corespondant
      int fini = 0;
      char caractereActuel;
      char tampon_indice[10];
      int taille_indice = 0;
      char tampon_titre[50];
      for (int clean = 0; clean < 50; clean++) // on clean tampon_titre
      {
        tampon_titre[clean] = '\0';
      }
      int i = 0; // le char courant sur la ligne
      int compte_etat = 0;
      while (fini != 1) {
        caractereActuel = fgetc(file); // On lit le caractère
        if (caractereActuel == EOF) {
          fini = 1;
        } else if (caractereActuel == '\n') {
          taille_indice = 0;
          compte_etat = 0;
          i = 0;
        } else {
          if (compte_etat == 0) {
            tampon_indice[i] = caractereActuel;
            taille_indice++;
            i++;
          }
          if (compte_etat == 2) { // si on est dans le titre
            tampon_titre[i] = caractereActuel;
            i++;
          }

          if (caractereActuel == '#') {
            compte_etat++;
            i = 0;
            if (compte_etat == 3) { // dans le cas ou on est a la fin du titre

              int en_a_trouver_un = 0;
              int les_a_tous_trouver = 1;

              for (int k = 0; k < nb_mot_cles; k++) {
                if (strstr(tampon_titre, mot_cles[k]) != NULL) {
                  en_a_trouver_un = 1;
                } else {
                  les_a_tous_trouver = 0;
                }
              }
              if (en_a_trouver_un == 1 && les_a_tous_trouver == 1) {
                for (int indice_res = 0; indice_res < taille_indice;
                     indice_res++) {
                  resultat[indice_dans_res] = tampon_indice[indice_res];
                  indice_dans_res++;
                }
              }

              for (int clean = 0; clean < 50; clean++) // on clean tampon_titre
              {
                tampon_titre[clean] = '\0';
              }
            }
          }
        }
      }
      resultat[indice_dans_res] = '\n';

      break;
    }
    case 3: {
      printf("recherche : %s\n", recherche);
      int trouve = 0;
      int indice_dans_res_1 = 0;
      int taille_reference_1 = 0;
      char tampon_reference1[100];
      char tampon_auteur[20];
      char tampon_reference[20];
      char tampon_genre[20];
      int fini = 0;
      int j = 0;
      int l = 0;
      char caractereActuel;
      int compte_etat;
      char auteur[20];
      char genre[20];
      for (int v = 0; v < 30; v++) {
        auteur[v] = '\0';
        genre[v] = '\0';
      }
      int hastag = 0;
      for (int i = 0; i < 100; i++) {

        if (recherche[i] == '\n') {
          break;
        } else if (recherche[i] == '#') {
          hastag = i;
        } else {
          if (hastag == 0) {
            auteur[i] = recherche[i];
          } else if (i > hastag) {
            genre[l] = recherche[i];
            l++;
          }
          genre[l] = '\0';
        }
      }

      while (fini != 1) {
        caractereActuel = fgetc(file); // On lit le caractère
        if (caractereActuel == EOF) {
          fini = 1;
        } else if (caractereActuel == '\n') {
          taille_reference_1 = 0;
          compte_etat = 0;
          j = 0;
        } else {
          if (compte_etat == 0) {
            tampon_reference1[j] = caractereActuel;
            taille_reference_1++;
            j++;
          }

          if (compte_etat == 1) { // si on est dans l'auteur
            tampon_auteur[j] = caractereActuel;
            j++;
          }

          if (compte_etat == 3) {
            tampon_genre[j] = caractereActuel;
            j++;
          }

          if (caractereActuel == '#') {
            compte_etat++;
            j = 0;
            if (compte_etat ==
                2) { // dans le cas ou on est a la fin de l'auteur
              if (strstr(tampon_auteur, auteur) != NULL) {
                trouve = 1;
                for (int i = 0; i < taille_reference_1; i++) {
                  tampon_reference[i] = tampon_reference1[i];
                }
              }

              for (int clean = 0; clean < 20; clean++) // on clean tampon_auteur
              {
                tampon_auteur[clean] = '\0';
              }
            }
            // printf("apres fin auteur\n");
            if (compte_etat == 4) { // dans le cas ou on est a la fin du genre
              if (strstr(tampon_genre, genre) != NULL) {
                if (trouve == 1) {
                  if (strstr(tampon_reference1, tampon_reference) != NULL) {
                    for (int indice_res_1 = 0;
                         indice_res_1 < taille_reference_1; indice_res_1++) {
                      resultat[indice_dans_res_1] =
                          tampon_reference[indice_res_1];
                      indice_dans_res_1++;
                    }
                  }
                }
              }
              for (int clean = 0; clean < 20; clean++) // on clean tampon_genre
              {
                tampon_genre[clean] = '\0';
              }
            }
          }
        }
      }
      resultat[indice_dans_res_1] = '\n';

      break;
    }
    case 4: {

      int indice_dans_res = 0;

      // on recupere le nom d'auteur
      int taille_nom = 0;
      while (recherche[taille_nom] != '\n' && recherche[taille_nom] != '#') {
        taille_nom++;
      }
      char nom_auteur[taille_nom + 4];
      for (int i = 0; i < taille_nom + 4; i++) {
        nom_auteur[i] = '\0';
      }
      for (int i = 0; i < taille_nom; i++) {
        nom_auteur[i] = recherche[i];
      }

      // recherche de livre corespondant
      int fini = 0;
      char caractereActuel;

      char tampon_indice[10];
      char tampon_nb_page[10];
      int nb_page_taille = 0;
      char tampon_note[1];

      // pour sauvegarder le maximum
      int max_trouve_prem = 0;
      char tampon_indice_max[10];
      int taille_indice_max = 0;
      int nb_page_taille_min = 0;
      char tampon_note_max[1];

      int taille_indice = 0;
      char tampon_auteur[50];
      for (int clean = 0; clean < 50; clean++) // on clean le nom d'auteur
      {
        tampon_auteur[clean] = '\0';
      }
      int i = 0; // le numero du char courant sur
      int compte_etat = 0;
      while (fini != 1) {
        caractereActuel = fgetc(file); // On lit le caractère
        if (caractereActuel == EOF) {
          fini = 1;
        } else if (caractereActuel == '\n') {
          compte_etat = 0;
          i = 0;
          // test de si on l'auteur rechercher est celui de cette ligne
          //  puis ajouts des informations dans le cas ou ce soit le meilleur
          //  document par nb de page puis note (ou le premier)
          if (strstr(tampon_auteur, nom_auteur) != NULL) {
            if (max_trouve_prem == 0) {

              for (int indice_res = 0; indice_res < taille_indice;
                   indice_res++) {
                tampon_indice_max[indice_res] = tampon_indice[indice_res];
              }
              max_trouve_prem = 1;

              // for (int indice_res=0;indice_res<nb_page_taille;indice_res++) {
              //    tampon_nb_page_max[indice_res]=tampon_indice[indice_res];
              //}
              taille_indice_max = taille_indice;
              nb_page_taille_min = atoi(tampon_nb_page);
              tampon_note_max[0] = tampon_note[0];
            } else {

              nb_page_taille = atoi(tampon_nb_page);

              if (nb_page_taille_min >
                  nb_page_taille) { // si il y a moins de page
                for (int indice_res = 0; indice_res < taille_indice;
                     indice_res++) {
                  tampon_indice_max[indice_res] = tampon_indice[indice_res];
                }
                taille_indice_max = taille_indice;
                nb_page_taille_min = nb_page_taille;
                tampon_note_max[0] = tampon_note[0];
              } else if (nb_page_taille_min == nb_page_taille) {
                if (tampon_note_max[0] - tampon_note[0] >
                    0) { // si la note du precedant max est moins bonne
                  for (int indice_res = 0; indice_res < taille_indice;
                       indice_res++) {
                    tampon_indice_max[indice_res] = tampon_indice[indice_res];
                  }
                  taille_indice_max = taille_indice;
                  nb_page_taille_min = nb_page_taille;
                  tampon_note_max[0] = tampon_note[0];
                }
              }
            }
          }
          taille_indice = 0;
          for (int clean = 0; clean < 10; clean++) // on clean tampon_titre
          {
            tampon_nb_page[clean] = '\0';
          }
          for (int clean = 0; clean < 50; clean++) // on clean tampon_titre
          {
            tampon_auteur[clean] = '\0';
          }
        } else {
          if (compte_etat == 0) {
            tampon_indice[i] = caractereActuel;
            taille_indice++;
            i++;
          }
          if (compte_etat == 1) { // si on est dans l'auteur
            tampon_auteur[i] = caractereActuel;
            i++;
          }
          if (compte_etat == 4 &&
              caractereActuel != '#') { // si on est dans le nombre de page
            tampon_nb_page[i] = caractereActuel;
            nb_page_taille++;
            i++;
          }
          if (compte_etat == 5) { // si on est dans la note
            tampon_note[i] = caractereActuel;
            i++;
          }

          if (caractereActuel == '#') {
            compte_etat++;
            i = 0;
            
          }
        }
      }

      for (int indice_res = 0; indice_res < taille_indice_max; indice_res++) {
        resultat[indice_dans_res] = tampon_indice_max[indice_res];
        indice_dans_res++;
      }
      if (taille_indice_max == 0) {
        resultat[0] = '\n';
      }

      break;
    }

    default:
    }
    fclose(file);
  }
  printf("resultat pour %i = %s ", nb_recherche, resultat);
  return resultat;
}
