#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include "serveur_client.c"

   
void end_child(int s) {
    wait(NULL);
}



int main(int argc, char *argv[]) {

    // creation du socket qui ecoute en continue 

    struct sigaction ac;
    ac.sa_handler=end_child;
    ac.sa_flags=SA_RESTART;


    int res_sig=sigaction(SIGCHLD,&ac, NULL);

    if (res_sig==-1) {
        perror("sigaction :");
    }
    int socket_ecoute = socket(AF_INET,SOCK_STREAM,0);
    if (socket_ecoute==-1) {
        perror("socket_ecoute:");
    }
    else {
        struct sockaddr_in socket_addr;
        socket_addr.sin_family = AF_INET;
        socket_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        socket_addr.sin_port = htons(4999);
        int serveur_bind = bind(socket_ecoute,(struct sockaddr *)&socket_addr,sizeof(socket_addr));
        if (serveur_bind==-1) {
            perror("socket_bind:");
        }
        else {
            int serveur_listen = listen(socket_ecoute,10);
            if (serveur_listen==-1) {
                perror("serveur_listen:");
            }
            else {
                while(1) {                    
                    struct sockaddr_in socket_addr_client;
                    unsigned int taille_struct = sizeof(socket_addr_client);
                    int serveur_accepte = accept(socket_ecoute,(struct sockaddr *)&socket_addr_client,&taille_struct);
                    switch (fork())
                    {
                    case -1 :
                        perror("fork :");
                    case 0:
                        close(socket_ecoute);
                        serveur_client(serveur_accepte,socket_addr_client);   
                    default:
                        close(serveur_accepte);
                    }


                }
            }
        }
    }

}
