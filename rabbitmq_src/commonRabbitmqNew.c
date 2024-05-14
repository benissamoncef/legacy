#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "commonRabbitmqNew.h"
#include <stdbool.h>
#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>
#include <unistd.h>
#include <libgen.h>
#include <rabbitmq-c/framing.h>
#include <sys/time.h>


//Variables globaux

int vg_numCb = 0;                                                                                      // Variable globale entiere qui represente le nombre de types de messages enregistres.
amqp_connection_state_t vg_conn;                                                                       // Variable globale qui représente l'etat de la connexion AMQP (Advanced Message Queuing Protocol).
amqp_channel_t vg_amqp_channel = 1 ;                                                                   // Variable globale qui represente le numero du canal AMQP utilisé pour la communication.
amqp_rpc_reply_t vg_rpc_reply;                                                                         // Variable globale qui stocke la reponse procedure a distance RPC (Remote Procedure Call) AMQP.

char vg_executableName[MAX_EXECUTABLE_NAME_LENGTH];                                                    // Tableau de caracteres qui stocke le nom de la tâche (nom de l'executable) avec une taille maximale definie par MAX_EXECUTABLE_NAME_LENGTH.
char vg_executablePath[256];                                                                           // Tableau de caracteres pour stocker le chemin absolu de l'executable en cours. 
char *vg_exchangeName;                                                                                 // Pointeur vers une chaine de caracteres qui represente le nom de l'échange (Topic) dans le systeme de messagerie.
unsigned int vg_countMsgPublish;                                                                       // Variable entiere non signee qui represente le nombre de messages publies.


/*
* Deux tableaux pour stocker des informations liées aux callbacks et aux types de contenu.
*/
T_CALLBACKS_MSG tg_callbacks[MAX_MESSAGE_TYPES];                                                       // Tableau de structure T_CALLBACKS_MSG appelé tg_callbacks, avec une taille maximale de MAX_MESSAGE_TYPES.
T_CONTENTTYPEMSG CONTENT_TYPES[MAX_NUM_CONTENT_TYPES] = {                                              // Tableau de structures T_CONTENTTYPEMSG appelé CONTENT_TYPES, avec une taille maximale de MAX_NUM_CONTENT_TYPES. Ce tableau est initialise avec quatre elements, chacun representant un type de contenu et son identifiant. 
    {"CONTENT_TYPE_TEXT_PLAIN","text/plain"},
    {"CONTENT_TYPE_JSON","application/json"},
    {"CONTENT_TYPE_BYTES","application/octet-stream"  },
    {"CONTENT_TYPE_XML","application/xml" }
};


/*
* DESCRIPTION FONTION registerCallbackForMessageType():
* 
* Objectif: Associer un rappel à un type de message spécifique, en stockant l'identifiant du message, le rappel et les arguments éventuels
*
* Declaree avec trois paramètres : L'identifiant du message (va_IdentMsg), le pointeur vers le rappel (va_Callback) et un pointeur vers les arguments du rappel (va_cb_arguments);
* 
*
*/

int registerCallbackForMessageType(int va_IdentMsg, void (*va_Callback)(), void *va_cb_arguments) {
    if (vg_numCb < MAX_MESSAGE_TYPES) {                                                                 // Verifie si le nombre de callbacks enregistres (vg_numCb) est inférieur à la limite maximale de types de messages 
        tg_callbacks[vg_numCb].IdMsg = va_IdentMsg;                                                     // Stocke l'identifiant du message dans le tableau tg_callbacks à l'indice vg_numCb.
        tg_callbacks[vg_numCb].Callback = va_Callback;                                                  // Stocke le pointeur vers le rappel dans le tableau tg_callbacks à l'indice vg_numCb.
        tg_callbacks[vg_numCb].cb_arguments = va_cb_arguments;                                          // Stocke le pointeur vers les arguments du rappel dans le tableau tg_callbacks à l'indice vg_numCb.
        vg_numCb++;                                                                                     // Incremente le nombre de callbacks enregistres.
    } 
    else
    {
        fprintf(stderr, "Attention : Nombre maximal de types de messages atteint.\n");
        return 1;                                                                                       // Retourne la valeur 1 pour indiquer une erreur.
    }
    return 0;                                                                                           // Retourne la valeur 0 pour indiquer que l'enregistrement du rappel s'est effectue avec succes.
}

/*
 * DESCRIPTION DE LA FONCTION setExecutableName():
 * 
 * Objectif: Récupérer et stocker le nom de l'exécutable en cours d'exécution
 */

int setExecutableName() {
    ssize_t vl_len = readlink("/proc/self/exe", vg_executablePath, sizeof(vg_executablePath) - 1);      // Effectue la lecture du chemin absolu de l'exécutable en cours et stocke sa longueur dans la variable vl_len.
    if (vl_len != -1) {
        vg_executablePath[vl_len] = '\0';                                                               // Assure que la chaîne de caractères est correctement terminée
        char* vl_executableName = basename(vg_executablePath);                                          // Obtene le nom de fichier de l'executable en utilisant la fonction basename()
        strcpy(vg_executableName, vl_executableName);                                                   // Copie le nom de fichier de l'executable dans la variable globale vg_executableName

    } else {
        fprintf(stderr, "setExecutableName : Erreur lors de la récupération du nom de l'exécutable\n");
        return 1;                                                                                      
    }
    return 0;                                                                                           
}


/*
 * DESCRIPTION DE LA FONCTION checkGetHostName():
 * 
 * Objectif: Verifier et recuperer le nom de l'hote (nom de la machine) en utilisant la fonction gethostname.
 * 
 * Si la recuperation du nom de la machine echoue, un message d'erreur est affiche et la fonction retourne 1 pour indiquer une erreur.
 * Si la recuperation du nom de la machine est reussie, la fonction verifie si la taille du nom d'hote < la taille de la chaine de destination specifiee.
 * 
 *                              -> Si c'est le cas, le nom d'hote est copie dans la chaine de destination avec une taille maximale de size. 
 * 
 * La fonction s'assure egalement que la chaine de caracteres est correctement terminee en ajoutant le caractère nul à la fin.
 * 
 * Si la taille de la chaine de destination est insuffisante, un message d'erreur est affiche et la fonction retourne 1 pour indiquer une erreur de taille de la chaîne de destination.
 */

int checkGetHostName(char *va_hostname, size_t size) {
    char vl_hostname[256];                                                                                                 
    // Récupération du nom de la machine hôte
    if (gethostname(vl_hostname, sizeof(vl_hostname)) == -1) {
        /* Ecriture Trace */
        fprintf(stderr, "checkGetHostName : Récupération du nom de la machine impossible.\n");
        return 1;
    } else {
        // Vérifier si la taille du nom d'hôte est inférieure à la taille de la chaîne de destination
        if (strlen(vl_hostname) < size) {
            strncpy(va_hostname, vl_hostname, size);                                                       // Copie le nom d'hote dans la chaine de destination avec une taille maximale de "size"
            va_hostname[size - 1] = '\0';                                                                                  
        } else {
            fprintf(stderr, "checkGetHostName : Taille de la chaîne de caractères de destination insuffisante.\n");
            return 1;
        }
        return 0;                                                                                                           
    }
}

/*
* DESCRIPTION DE LA FONCTION getQueueName:
* 
* Objectif : Utilisee pour construire le nom d'une file d'attente en fonction du nom d'hote, du nom executable et du nom d'echange fournis en parametres. 
* 
* Utilisation du sprintf pour formater le nom de la file d'attente en concatenant le nom d'hote, le nom executable et le nom d'echange. Le resultat est stocke dans le tableau de caracteres va_queueName.
*
*/

int getQueueName(char* va_queueName, char* va_exchangeName) {
    // Construction du nom de la file d'attente:
    char vl_hostname[256]; 
    setExecutableName();
    if(vg_executableName == NULL){
    	printf("getQueueName : Récupération nom de la tache impossible.\n");
	    return 1;
    }
    else
    {
        if (checkGetHostName(vl_hostname, sizeof(vl_hostname))) {
        return 1;
        }
    	sprintf(va_queueName, "%s.%s.%s", vl_hostname, vg_executableName, va_exchangeName);
   	    printf("getQueueName : Nom de la file d'attente : %s\n", va_queueName);
        return 0;
    }
}

/*
* DESCRIPTION DE LA FONCTION setAppID:
* 
* Objectif : Utilisee pour definir la propriete app_id dans une structure amqp_basic_properties_t, qui est utilisee pour representer les proprietes d'un message AMQP. 
* 
* Notes:
*       va_props->_flags |= AMQP_BASIC_APP_ID_FLAG; active le drapeau AMQP_BASIC_APP_ID_FLAG dans la variable _flags de la structure amqp_basic_properties_t. Ce drapeau indique que la propriété app_id est présente dans les propriétés du message.
*
*/

int setAppID(amqp_basic_properties_t *va_props){
    char vl_appId[MAX_APP_ID_LENGTH]; 
    char vl_hostname[200];
    setExecutableName();
    // Récupération du nom de la machine hôte
    if (checkGetHostName(vl_hostname, sizeof(vl_hostname))) {
        return 1;
    }
    // Récupération du nom de l executable
    else if(vg_executableName == NULL)                                  
    {
    	printf("setAppID : Récupération nom de la tache impossible.\n");
	    return 1;
    }
    else
    {
        //Concatenation nom machine.nom executable
        
        sprintf(vl_appId, "%s.%s", vl_hostname, vg_executableName);     
        printf("%s\n",vl_appId);
        // Definition de la propriete app_id
        va_props->_flags |= AMQP_BASIC_APP_ID_FLAG;                                                 // Activation du drapeau dans la variable _flags de la structure amqp_basic_properties_t
        va_props->app_id = amqp_cstring_bytes(vl_appId);                                            // Definition de la valeur app_id dans les props du messsage en convertissant la chaine en objet amqp_bytes_t
        char vl_appID[200];
        memcpy(vl_appID, va_props->app_id.bytes, va_props->app_id.len);                             // Copie les octets  de properties.app_id dans vl_appId
        vl_appID[va_props->app_id.len] = '\0';
        printf("setAppID : %s\n", vl_appID);
    }
    return 0;
}

/*
* DESCRIPTION DE LA FONCTION setContentType:
*
* Objectif: Definir la propriete content_type dans une structure amqp_basic_properties_t en fonction du type de contenu fourni en paramètre. 
*         - La fonction utilise une boucle for pour parcourir un tableau de structures CONTENT_TYPES contenant des associations entre des noms de types de contenu et leurs valeurs correspondantes.
*         - La valeur correspondante du type de contenu trouvé (CONTENT_TYPES[i].value) est utilisée pour définir la propriété content_type dans les propriétés du message en convertissant la chaîne en un objet amqp_bytes_t à l'aide de la fonction amqp_cstring_bytes. 
*/

int setContentType(amqp_basic_properties_t *va_props, const char *va_contentType) {
    for (size_t i = 0; i < MAX_NUM_CONTENT_TYPES; ++i) {
        if (strcmp(va_contentType, CONTENT_TYPES[i].name) == 0) {
            va_props->_flags |= AMQP_BASIC_CONTENT_TYPE_FLAG;
            va_props->content_type = amqp_cstring_bytes(CONTENT_TYPES[i].value);
            return 0;
        }
    }
    return 1;
}

/*
* DESCRIPTION DE LA FONCTION getAppId:
*
* Objectif: Utilisee pour extraire la valeur de la propriete app_id d'une structure amqp_basic_properties_t et la copier dans une chaîne de caracteres fournie en paramètre (va_appId).
*            
*/
int getAppId(amqp_basic_properties_t va_props, char* va_appId) {
    if (va_props._flags & AMQP_BASIC_APP_ID_FLAG) {
        size_t vl_len = va_props.app_id.len;
        printf("getAppId : Taille de appId : [%zu]\n", vl_len);

        if (vl_len < MAX_APP_ID_LENGTH) {
            strncpy(va_appId, (char*)va_props.app_id.bytes, vl_len);
            va_appId[vl_len] = '\0';
            printf("getAppId : Après copie : AppID [%s] : Nouvelle taille : [%zu]\n", va_appId, vl_len);

            // Vérification de la taille de va_appId
            if (strlen(va_appId) != vl_len) {
                printf("getAppId : Erreur de taille. La copie de appId est incorrecte.\n");
                return 1;
            }
        } else {
            strncpy(va_appId, (char*)va_props.app_id.bytes, MAX_APP_ID_LENGTH - 1);
            va_appId[MAX_APP_ID_LENGTH - 1] = '\0';
            printf("getAppId : Copie tronquée à la taille maximale : [%d]\n", MAX_APP_ID_LENGTH - 1);

            // Vérification de la taille de va_appId
            if (strlen(va_appId) != MAX_APP_ID_LENGTH - 1) {
                printf("getAppId : Erreur de taille. La copie tronquée de appId est incorrecte.\n");
                return 1;
            }
        }
    } else {
        strcpy(va_appId, ": -1");

        // Vérification de la taille de va_appId
        if (strlen(va_appId) != 2) { // Longueur de "-1"
            printf("getAppId : Erreur de taille. La copie de AppID non défini est incorrecte.\n");
            return 1;
        }
    }

    return 0;
}


//Fonction pour generer le Id de correlation
int generateCorrelationId(char* va_correlationId, char* va_appId, time_t va_currentTime, unsigned int va_counterMsgPublish) {
     
    snprintf(va_correlationId, MAX_CORRELATION_ID_LENGTH + 100, "%s.%ld.%u", va_appId, va_currentTime, va_counterMsgPublish);
    return 0;

}

/*
* DESCRIPTION DE LA FONCTION getCorrelationId: 
*                Prend en paramètre une structure amqp_basic_properties_t appelee va_props et un pointeur vers un pointeur de caracteres va_correlationId. Elle renvoie un entier.
* Fonction pour obtenir l'ID de corrélation */

int getCorrelationId(amqp_basic_properties_t va_props, char** va_correlationId) {
     if (va_props._flags & AMQP_BASIC_CORRELATION_ID_FLAG) {                    // verifie si le drapeau _flags de la structure va_props contient le drapeau AMQP_BASIC_CORRELATION_ID_FLAG
        size_t len = va_props.correlation_id.len;                               // initialise  len avec la longueur du champ correlation_id
        *va_correlationId = (char*)malloc(len + 1);                             // Un espace mémoire est alloué à *va_correlationId pour stocker la valeur du champ correlation_id. La taille allouée est de len + 1 pour accueillir également le caractère de fin de chaîne.
        memcpy(*va_correlationId, va_props.correlation_id.bytes, len);          // Copie len octets à partir de l'adresse va_props.correlation_id.bytes vers l'adresse *va_correlationId, ce qui permet de copier la valeur du champ correlation_id.
        (*va_correlationId)[len] = '\0';
    } else {
        *va_correlationId = strdup("getCorrelationId : Correlation ID non défini"); // Initialise *va_correlationId avec une chaîne de caractères statique indiquant que le Correlation ID n'est pas défini dans la fonction getCorrelationId.
    }
    return 0;
}

//Fonction pour definir le ID de correlation
int setCorrelationId(amqp_basic_properties_t va_props, unsigned int va_counterMsgPublish ) {
    struct timeval tv;
    time_t vl_currentTime;
    
    gettimeofday(&tv, NULL);
    vl_currentTime = (time_t)tv.tv_sec;

    char va_appId[200];
    getAppId(va_props, va_appId);
    
    char vl_correlationId[MAX_CORRELATION_ID_LENGTH + 100];
    generateCorrelationId(vl_correlationId, va_appId, vl_currentTime, va_counterMsgPublish);
  
    va_props._flags |= AMQP_BASIC_CORRELATION_ID_FLAG;
    va_props.correlation_id.bytes = malloc(strlen(vl_correlationId));
   
    strcpy((char*)va_props.correlation_id.bytes, vl_correlationId);
    va_props.correlation_id.len = strlen(vl_correlationId);

    printf("setCorrelationId : %s\n", vl_correlationId);
}


//Verification reponse procedure à distance
int check_reply(amqp_rpc_reply_t va_reply, char* va_error_msg) {
    amqp_connection_close_t * vl_conn_close ;
    amqp_channel_close_t *vl_chan_close ;

    switch (va_reply.reply_type) {
        case AMQP_RESPONSE_NORMAL:
            // Cas traitement normale, aucune action.
            break;
        case AMQP_RESPONSE_NONE:

            fprintf(stderr, "check_reply : %s : Aucune réponse du serveur\n", va_error_msg);
            return 1;

        case AMQP_RESPONSE_LIBRARY_EXCEPTION:

            fprintf(stderr, "check_reply : %s : Erreur de la bibliothèque RabbitMQ-C : %s\n", va_error_msg, amqp_error_string2(va_reply.library_error));
            return 1;

        case AMQP_RESPONSE_SERVER_EXCEPTION:

            switch (va_reply.reply.id) {

                case AMQP_CONNECTION_CLOSE_METHOD: // Cas connexion fermee par le serveur.

                    vl_conn_close = (amqp_connection_close_t *) va_reply.reply.decoded;
                    fprintf(stderr, "check_reply : %s : Connexion fermée par le serveur : code %d, message : %.*s\n",
                            va_error_msg, vl_conn_close->reply_code, (int)vl_conn_close->reply_text.len, (char*)vl_conn_close->reply_text.bytes);
                    break;

                case AMQP_CHANNEL_CLOSE_METHOD: // Cas canal ferme par le serveur.

                    vl_chan_close = (amqp_channel_close_t *) va_reply.reply.decoded;
                    fprintf(stderr, "check_reply : %s : Canal fermé par le serveur : code %d, message : %.*s\n",
                            va_error_msg, vl_chan_close->reply_code, (int)vl_chan_close->reply_text.len, (char*)vl_chan_close->reply_text.bytes);
                    break;

                default:

                    fprintf(stderr, "check_reply : %s : Réponse d'exception inattendue du serveur\n", va_error_msg);
                    break;
            }
            return 1;
        default:

            fprintf(stderr, "check_reply : %s : Réponse inattendue du serveur\n", va_error_msg);
            return 1;
    }
    return 0;
}

int declareExchange(char* va_exchangeName){
   
    // Déclaration variables
    amqp_exchange_declare_ok_t* vl_exchange_declare_ok;//Stocke infos de retour après déclaration échange
    amqp_channel_open_ok_t* vl_channel_open_ok;

    vl_channel_open_ok = amqp_channel_open(vg_conn, vg_amqp_channel);
    vg_rpc_reply = amqp_get_rpc_reply(vg_conn);
    
    if(check_reply(vg_rpc_reply, "declareExchange : Echec de l'ouverture du canal\n")){
         return 1;
	}
    else
    {
        amqp_exchange_declare_t exchange_declare = {
            .ticket = false,
            .exchange = amqp_cstring_bytes(va_exchangeName),
            .type = amqp_cstring_bytes("fanout"),
            .passive = false,
            .durable = true,
            .auto_delete = false,
            .internal = false,
            .arguments = amqp_empty_table
        };
        //Declaration de l'echange
        vl_exchange_declare_ok = amqp_exchange_declare(
                                    vg_conn,
                                    vg_amqp_channel,
                                    exchange_declare.exchange,
                                    exchange_declare.type,
                                    exchange_declare.passive,
                                    exchange_declare.durable,
                                    exchange_declare.auto_delete,
                                    exchange_declare.internal,
                                    exchange_declare.arguments
        );
        vg_rpc_reply = amqp_get_rpc_reply(vg_conn);
        if(check_reply(vg_rpc_reply, "declareExchange : Echec de la declaration de l'echange!\n")){
            return 1;
        } 
        else
        {
            printf("declareExchange : Declaration echange reussite!\n");
            //Fermeture finale du canal
            amqp_channel_close(vg_conn, vg_amqp_channel, AMQP_REPLY_SUCCESS);
            vg_rpc_reply = amqp_get_rpc_reply(vg_conn);
            if(check_reply(vg_rpc_reply, "declareExchange: Fermeture canal a échoué!")){
                return 1;
            };
            return 0;
        }
    }
}

int declareQueue(char* va_exchangeName){
   
    amqp_queue_declare_ok_t* queue_declare_ok;      //stocke infos de retour après déclaration queue
    amqp_channel_open_ok_t* vl_channel_open_ok;
    
    char vl_queueName[200];
	if(getQueueName(vl_queueName, va_exchangeName))
    {
		printf("declareQueue : Echec de la generation du nom de la queue!\n");
		return 1;
	}
    else
    {
        
        vl_channel_open_ok = amqp_channel_open(vg_conn, vg_amqp_channel);
        vg_rpc_reply = amqp_get_rpc_reply(vg_conn);
        if(check_reply(vg_rpc_reply, "declareQueue : Echec de l'ouverture du canal\n")){
            return 1;
        };
		amqp_queue_declare_t queue_declare = {
            		.ticket = false,
            		.queue = amqp_cstring_bytes(vl_queueName),
            		.passive = false,
            		.durable = true,
            		.exclusive = false,
            		.auto_delete = false,
             		.arguments = amqp_empty_table
        };

        //Déclaration de la queue
        queue_declare_ok = amqp_queue_declare(
                                vg_conn,
                                vg_amqp_channel,
                                queue_declare.queue,
                                queue_declare.passive,
                                queue_declare.durable,
                                queue_declare.exclusive,
                                queue_declare.auto_delete,
                                queue_declare.arguments
        );
    
        vg_rpc_reply = amqp_get_rpc_reply(vg_conn);
        if(check_reply(vg_rpc_reply, "declareQueue : Echec de la declaration de la queue!\n")){
            return 1;			
        }
        else
        {
            printf("declareQueue : Declaration queue reussite!\n");
        }
      
	}
    return 0;
}

int bindExchangeToQueue(char* va_exchangeName, char* va_queueName) {
        amqp_queue_bind_ok_t * queue_bind_ok;
	 // Lier la file d'attente à l'échange
        queue_bind_ok = amqp_queue_bind(
                                    vg_conn,
                                    vg_amqp_channel,
                                    amqp_cstring_bytes(va_queueName),
                                    amqp_cstring_bytes(va_exchangeName),
                                    amqp_cstring_bytes(""),
                                    amqp_empty_table);
        vg_rpc_reply = amqp_get_rpc_reply(vg_conn);
        if(check_reply(vg_rpc_reply, "bindExchangeToQueue : La liaison entre la queue et l'échange a échoué!\n")){
		    return 1;
	    } 
        else
        {
            printf("bindExchangeToQueue : La liaison entre la queue et l'échange réussite!\n--------\n");
        }
        return 0;
}

 amqp_basic_properties_t vl_props;
int publishMessage(char* va_message, char* va_exchangeName, int va_IdentifMsg ) {
    
    unsigned int vl_countMsgPublish = vg_countMsgPublish;

    // Création de l'échange 
    if (declareExchange(va_exchangeName)) {
        return 1;
    } else {
        // Déclaration des propriétés du message
        printf("publishMessage : ExchangeName : %s\n", va_exchangeName);
        printf("publishMessage : message : %s\n", va_message);
        amqp_channel_open_ok_t* vl_channel_open_ok;
        vl_channel_open_ok = amqp_channel_open(vg_conn, vg_amqp_channel);
        vg_rpc_reply = amqp_get_rpc_reply(vg_conn);
        if (check_reply(vg_rpc_reply, "publishMessage : Echec de l'ouverture du canal\n")) {
            return 1;
        };

        // Definition du mode de livraison du message
        vl_props._flags = AMQP_BASIC_DELIVERY_MODE_FLAG;
        vl_props.delivery_mode = AMQP_DELIVERY_NONPERSISTENT; // Mode de livraison NonPersistant

        // Definition du content_type du message
        if (setContentType(&vl_props, "CONTENT_TYPE_TEXT_PLAIN")) {
            printf("publishMessage : Le content type n'est pas defini!\n");
            return 1;
        }

        // Stockage messag_id
        char messageIdString[20];
        sprintf(messageIdString, "%d", va_IdentifMsg);
        vl_props._flags |= AMQP_BASIC_MESSAGE_ID_FLAG;
        vl_props.message_id.bytes = (void*)messageIdString;
        vl_props.message_id.len = strlen(messageIdString);

        // Définition de la durée d'expiration du message
        // props._flags = AMQP_BASIC_EXPIRATION_FLAG;
        // props.expiration = amqp_cstring_bytes("15000"); 

        // Definition d'ID d'application
        setAppID(&vl_props);
        // Definition correlation Id
        setCorrelationId(vl_props, vl_countMsgPublish);
        vl_countMsgPublish++;
        // Publication du message
        amqp_basic_publish(
            vg_conn,
            vg_amqp_channel,
            amqp_cstring_bytes(va_exchangeName),
            amqp_cstring_bytes(""),
            false, // Mandatory : indiquer au courtier que le message DOIT être acheminé vers une file d'attente. Si le courtier ne peut pas le faire, il doit répondre avec une méthode basic.return.
            false, // Immediate
            &vl_props,
            amqp_cstring_bytes(va_message));

        vg_rpc_reply = amqp_get_rpc_reply(vg_conn);
        if (check_reply(amqp_get_rpc_reply(vg_conn), "publishMessage : Echec de la publication du message\n")) {
            return 1;
        } else {
            printf("publishMessage : Message publie.\n");
        }

        vg_countMsgPublish = vl_countMsgPublish;
        return 0;
    }
}

int connectionToRabbitMQ() {
   //Déclaration variables
    amqp_socket_t* vl_socket = NULL;

    //Création de la connexion RabbitMQ
    vg_conn = amqp_new_connection();
    if(!vg_conn){
        fprintf(stderr,"connectionToRabbitMQ : Erreur lord de la création de la connexion RabbitMQ\n");
        return 1;
    }
    //Création du socket TCP
    vl_socket = amqp_tcp_socket_new(vg_conn);
    if (!vl_socket) {
        fprintf(stderr, "connectionToRabbitMQ : Erreur lors de la création du RabbitMQ TCP socket\n");
        return 1;
    }

    //Ouverture du socket TCP
    int status = amqp_socket_open(vl_socket, HOSTIP, atoi(PORT));
    if (status) {
        fprintf(stderr, "connectionToRabbitMQ : Impossible d'ouvrir le socket TCP RabbitMQ\n");
        return 1;
    }
    printf("connectionToRabbitMQ : Ouverture socket TCP avec succès\n-------\n");

    //Configuration de la connexion RabbitMQ avec le socket
    //Connexion à RabbitMQ
    amqp_rpc_reply_t login_reply = amqp_login(vg_conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, USERNAME, PASSWORD);
    if(check_reply(login_reply, "connectionToRabbitMQ : Connexion impossible à RabbitMQ")){
    	return 1;
    };
    printf("connectionToRabbitMQ : Connexion à RabbitMQ établit!\n--------\n");
    return 0;

}
 
int abonnementTopic(char* va_exchangeName){
    if(declareExchange(va_exchangeName))
    {
        return 1;
    }
    else if(declareQueue(va_exchangeName))
    {    
        return 1;
    }
    char vl_queueName[200];
	if(getQueueName(vl_queueName, va_exchangeName))
    {
		printf("declareQueue : Echec de la generation du nom de la queue!\n");
		return 1;
	}
    else 
    {
        if(bindExchangeToQueue(va_exchangeName, vl_queueName))
        {
            return 1;
        }
        else
        {
            printf("abonnementTopic : Abonnement au topic avec succès!\n");
            // Configuration de la file d'attente en tant que consommateur
            amqp_basic_consume_ok_t* consume_ok;
            consume_ok = amqp_basic_consume(
                vg_conn,
                vg_amqp_channel,
                amqp_cstring_bytes(vl_queueName),
                amqp_empty_bytes,
                false,
                true,
                false,
                amqp_empty_table);
            vg_rpc_reply = amqp_get_rpc_reply(vg_conn);
            if(check_reply(amqp_get_rpc_reply(vg_conn), "abonnementTopic : Echec de la configuration du consommateur\n")){
                deconnectionRabbitmq();
                return 1;
            }
            else
            {
                printf("abonnementTopic : Configuration consommateur est réussite!\n");  
            }   
        } 
    }  
    return 0;
}

/******************************************************
* DESCRIPTION DE LA FONCTION onMessage(amqp_envelope_t envelope)  :
*
* Ojjectif : Traite les messages recus.
*           - Prend en paramètre l'enveloppe du message (amqp_envelope_t) 
*           - Effectue les opérations de traitement spécifiques au message y compris la recherche de la fonction de rappel appropriée basée sur l'ID du message.
*           - Appelee lorsque la consommation du message est réussie.
*  
******************************************************/
void onMessage(amqp_envelope_t envelope) {
    amqp_basic_properties_t vl_properties = envelope.message.properties;
    char vl_str_message_id[10];
    int vl_cb_trouve = 0;
    char vl_appId[200];

    printf("\n *Message reçu:\n\n");
    printf("onMessage : Topic: %.*s\n", (int)envelope.exchange.len, (char*)envelope.exchange.bytes);
    printf("onMessage : Delivery Tag: %u\n", (unsigned)envelope.delivery_tag);

    // Vérification si le content type est bien défini
    if (vl_properties._flags & AMQP_BASIC_CONTENT_TYPE_FLAG) {
        printf("onMessage : Content Type: %.*s\n***********************\n", (int)vl_properties.content_type.len, (char*)vl_properties.content_type.bytes);
    } else {
        printf("onMessage : Content Type non défini!\n");
    }
    

    if (vl_properties._flags & AMQP_BASIC_MESSAGE_ID_FLAG) {
        strcpy(vl_str_message_id, (char*)(vl_properties.message_id.bytes));
        printf("onMessage : Message Id [%s]:\n\n", vl_str_message_id);
        // Trouver la fonction de rappel basée sur vl_str_message_id
        for (int i = 0; i < vg_numCb; i++) {
            if (tg_callbacks[i].IdMsg == atoi(vl_str_message_id)) {

                (*(tg_callbacks[i].Callback))(envelope, (char*)tg_callbacks[i].cb_arguments);
                // Affichage des arguments
                vl_cb_trouve = 1;
                break;
            }
        }
        if (!vl_cb_trouve) {
            printf("onMessage : Aucune fonction de rappel trouvée pour le type de message.\n");
        }
    } else {
        printf("onMessage : Type de message non défini!\n");
    }
    // Obtenir l'ID de l'application
    if (!getAppId(vl_properties, vl_appId)) {
        printf("onMessage : Erreur lors de la récupération de l'AppID.\n");
        return;
    }

    printf("onMessage : Récupération ID application: %s\n***********************\n", vl_appId);
}

/******************************************************
* DESCRIPTION DE LA consumeMessage() :
*
*  Ojjectif: Consommation des messages à partir de la connexion RabbitMQ.
*        1 - Declaration des variables                 : 
*                                                          $ amqp_envelope_t envelope : Une structure qui sera utilisee pour stocker les informations du message consomme.
*                                                          $ struct timeval timeout   : Une structure qui represente le delai d'attente pour la consommation du message. 
*        2 - Appel de la fonction amqp_consume_message :
*                                                          $ Les arguments passes a cette fonction sont : la connexion, un pointeur vers la structure envelope où les informations du message seront stockees, le delai d'attente specifie par timeout, et un indicateur 0 qui indique que le message consomme doit etre automatiquement acquitte.
*        3 - Verification du resultat                  :
*                                                          $ Le resultat de l'appel amqp_consume_message est stocké dans la variable result, qui est de type amqp_rpc_reply_t.
*                                                          $ Si result.reply_type est égal à AMQP_RESPONSE_NORMAL, cela signifie que la consommation du message s'est déroulée normalement. 
*                                                               -> Dans ce cas, la fonction onMessage est appelée en passant la structure envelope en tant qu'argument. 
*
*                                                          $ Si result.reply_type est égal à AMQP_RESPONSE_LIBRARY_EXCEPTION, cela signifie qu'il y a eu une exception au niveau de la bibliothèque RabbitMQ.
*                                                               -> Dans ce cas, un message est affiché indiquant qu'on attend de nouveaux messages, une pause de 4 secondes est effectuée (avec sleep(4)), et la fonction retourne 1 pour indiquer qu'il faut continuer à consommer des messages.
*                                                         
*                                                          $ Si result.reply_type ne correspond à aucune des deux situations ci-dessus, cela signifie qu'il y a eu une erreur lors de la consommation du message.
*        4 - Affichage d'un message de fin et retour : 
*                                                          $ Si la fonction n'a pas retourné 1 à ce stade, cela signifie que la consommation des messages s'est terminée normalement.
*                                                          $ Un message est affiché pour indiquer l'arrêt du consommateur, et la fonction retourne 0 pour indiquer une exécution réussie.
*
*
******************************************************/
int consumeMessages() {
    amqp_envelope_t envelope;
    struct timeval timeout = {0, 500000}; // Délai d'attente de 500 millisecondes

    amqp_rpc_reply_t result = amqp_consume_message(vg_conn, &envelope, &timeout, 0);
    if (result.reply_type == AMQP_RESPONSE_NORMAL) {
        onMessage(envelope);
    } else if (result.reply_type == AMQP_RESPONSE_LIBRARY_EXCEPTION) {
        printf("----------------------------\n En attente de messages...\n----------------------------\n");
        sleep(4);
        return 1;
    } else {
        printf("consumeMessages : Erreur lors de la consommation du message.\n");
        return 1;
    }

    return 0;
}

int deconnectionRabbitmq(){
  // Fermeture de la connexion
    amqp_channel_close(vg_conn, vg_amqp_channel, AMQP_REPLY_SUCCESS);
    vg_rpc_reply = amqp_get_rpc_reply(vg_conn);
    if(check_reply(vg_rpc_reply, "deconnectionRabbitmq : Fermeture canal a échoué!")){
        return 1;
    };

    amqp_connection_close(vg_conn, AMQP_REPLY_SUCCESS);
    vg_rpc_reply = amqp_get_rpc_reply(vg_conn);
    if(check_reply(vg_rpc_reply, "deconnectionRabbitmq : Fermeture connexion a échoué!")){
        return 1;
    };

    amqp_destroy_connection(vg_conn);
    vg_rpc_reply = amqp_get_rpc_reply(vg_conn);
    if(check_reply(vg_rpc_reply, "deconnectionRabbitmq : Destruction connexion a échoué!")){
        return 1;
    };
    printf("deconnectionRabbitmq : Vous êtes déconnecté de Rabbitmq!\n");
    return 0;
}


/*
* DESCRIPTION FONCTION setMessageExpiration(amqp_basic_properties_t* props, int va_expirationMillis)
*
* Objectif: Fonction pour configurer la propriété d'expiration du message
*           Entrees :  Prend un pointeur vers une structure amqp_basic_properties_t & une durée d'expiration en millisecondes.
*           Etapes  :  - Formate la duree d'expiration en tant que chaine de caracteres.
*                      - Active le drapeau d'expiration
*                      - Affecte l'adresse et la longueur de la chaîne de caracteres a la propriete expiration de la structure amqp_basic_properties_t.  
*/

void setMessageExpiration(amqp_basic_properties_t* va_props, int va_expirationSec) {
    char vl_expirationString[20];
    snprintf(vl_expirationString, sizeof(vl_expirationString), "%d", va_expirationSec * 1000);
    
    va_props->_flags |= AMQP_BASIC_EXPIRATION_FLAG;
    va_props->expiration.bytes = (void*)vl_expirationString;                    //Pointeur generique qui pointe vers les octets representant la duree d expiration
    va_props->expiration.len = strlen(vl_expirationString);
}













