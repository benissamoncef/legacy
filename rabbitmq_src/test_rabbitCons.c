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
 * Connexion à RabbitMQ.
 * Déclaration de l'échange.
 * Abonnement au topic.
 * Lecture messages.
 * Fermeture de la connexion RabbitMQ;
 * Libération des ressources de la bibliothèque RabbitMQ.
*/
void handleXDM_TST_CMDT(amqp_envelope_t envelope, char * va_argument);
void handleXDM_TST_INIT(amqp_envelope_t envelope,  char * va_argument);

void handleXDM_TST_CMDT(amqp_envelope_t envelope, char * va_argument) {
  
    printf("Traitement du message XDM_TST_CMDT(1002) [%s] \n",va_argument);
   
}

void handleXDM_TST_INIT(amqp_envelope_t envelope,  char * va_argument) {
    amqp_basic_properties_t properties = envelope.message.properties;

    printf("Traitement du message XDM_TST_INIT(1004) [%s] \n",va_argument);
 

}

int main(int argc, char *argv[]) {
    int vl_return=0;

    // Vérification des arguments
    if (argc < 2) {
        fprintf(stderr, "Usage: consumer exchange ");
        return 1;
    }
   
    char* vl_exchangeName = argv[1];
    vg_exchangeName = vl_exchangeName;

    char vl_hostname[200];
    
    if (checkGetHostName(vl_hostname, sizeof(vl_hostname)) == 0) {     // Récupération du nom d'hôte associé à l'adresse IP fournie en argument
        printf("Nom de l'hôte : %s\n", vl_hostname);
    } 


    char vl_queueName[200];
    getQueueName(vl_queueName, vl_exchangeName);//getQueueName(hostname, queueName); // génération du nom de la queue
    
    // Lecture du message à RabbitMQ
    
    connectionToRabbitMQ();

    printf("Tentative d'abonnement au topic...\n");

    if(abonnementTopic(vl_exchangeName))
    {
        vl_return=1;
//        return 1;
    }
    else
    {
        registerCallbackForMessageType(XDM_TST_CMDT, handleXDM_TST_CMDT,"argument 1");
        registerCallbackForMessageType(XDM_TST_INIT, handleXDM_TST_INIT,"argument 2");

        printf("Consommateur prêt à recevoir des messages.\n");

        while (1) {

            consumeMessages();
        }
        printf("Arrêt du consommateur.\n");
    }   
    // Fermeture de la connexion
    deconnectionRabbitmq();
    
    return vl_return;
}