#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <string.h>

#include "commonRabbitmqNew.h"
/**
 * Création du socket TCP.
 * Ouverture du socket TCP.
 * Création de la connexion RabbitMQ.
 * Configuration de la connexion RabbitMQ avec le socket.
 * Abonnement au topic.
 * Ecrirure messages.
 * Fermeture de la connexion RabbitMQ & Libération des ressources de la bibliothèque RabbitMQ.
*/
int main(int argc, char *argv[]) {

    // Vérification des arguments
    if (argc < 4) {

        fprintf(stderr, "Usage: Producer exchange typeMsg payloadMsg\n"); // fprintf(stderr, "Usage: consumer host port queueName exchange bindingkey username password");
        return 1;
    }
     
    // vg_executablePath = argv[0];
    // vg_executableName = basename(vg_executablePath);
   
    char* vl_exchangeName = argv[1];
    vg_exchangeName = vl_exchangeName;
    printf("vg_exchange= %s: vl_exchange = %s\n",vg_exchangeName,vl_exchangeName);
   
    char vl_message[255] = "";                     // Récuperation message
    for(int i = 3 ; i< argc; i++){
        strcat(vl_message,argv[i]);
        strcat(vl_message," ");
    }
    int vl_IdentTypeMsg = atoi(argv[2]);
   
   
    char vl_hostname[200];
    
    if (checkGetHostName(vl_hostname, sizeof(vl_hostname)) == 0) {     // Récupération du nom d'hôte associé à l'adresse IP fournie en argument
        printf("Nom de l'hôte : %s\n", vl_hostname);
    } 

   
    while (1)
    {
        connectionToRabbitMQ();                         // Connexion à Rabbitmq
        publishMessage(vl_message,vl_exchangeName,vl_IdentTypeMsg); // Ecriture du message sur le topic
        sleep(5);
    }
    

    deconnectionRabbitmq();                      // Fermeture de la connexion
    return 0;
}