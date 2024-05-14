#ifndef COMMONRABBITMQNEW_H
#define COMMONRABBITMQNEW_H

#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>


#include <stdbool.h>

#define USERNAME "admin"
#define PASSWORD "magistra"
#define HOSTIP "10.6.73.118"
#define PORT "5672"
#define MAX_CORRELATION_ID_LENGTH 200
#define MAX_APP_ID_LENGTH 512
#define MAX_EXECUTABLE_NAME_LENGTH 256
#define MAX_MESSAGE_TYPES 10
#define MAX_NUM_CONTENT_TYPES 5



/* definition des constantes internes */

#define OFFSET_SUP 1000
#define OFFMSG_TST (OFFSET_SUP + 0 )
#define OFFMSG_TSH (OFFSET_SUP + 100 )
#define OFFMSG_TSG (OFFSET_SUP + 200 )
#define OFFMSG_TSP (OFFSET_SUP + 300 )
#define OFFMSG_TSA (OFFSET_SUP + 400 )
#define OFFMSG_TSE (OFFSET_SUP + 500 )
#define OFFMSG_SYN (OFFSET_SUP + 600 )

#define OFFSET_EXT 2000

#define OFFSET_GES 3000

#define OFFSET_IHM 4000

#define OFFSET_SAI 5000

#define OFFSET_BAS 6000

#define OFFSET_ARC 7000		/* Utilise dans XDL */

#define OFFSET_ALG 8000		/* Utilise dans XDL */

#define OFFSET_MAI 9000		/* sous système MAINTE */

#define OFFSET_MAG 10000        /* Offset message Magistra */

#define OFFSET_GTW 11000        /* Offset message gateways externes*/.


/* Numeros des messages du noyau de supervision */
// #define XDM_TST_PROG    (OFFMSG_TST + 0)
#define XDM_TST_PROG    1000
#define XDM_TST_ANNU    (OFFMSG_TST + 1)
#define XDM_TST_CMDT    1002
#define XDM_TST_TRAC    (OFFMSG_TST + 3)
#define XDM_TST_INIT    1004


extern amqp_rpc_reply_t rpc_reply;

extern amqp_envelope_t envelope;

// extern char vg_executableName;
// extern char* vg_executablePath;
extern char* vg_exchangeName;
extern char vg_hostname[200];



typedef struct {
    const char *name;
    const char *value;
} T_CONTENTTYPEMSG;

typedef struct
{
    int    IdMsg ;
    void   (*Callback)();
    void*  cb_arguments;                                                      //void* permet de passer des données supplémentaires au callback sans connaître le type de ces données à l'avance. 
} T_CALLBACKS_MSG;



int check_reply(amqp_rpc_reply_t va_reply, char* va_error_msg);

int bindExchangeToQueue(char* va_exchangeName, char* vl_queueName);
int declareExchange(char* va_exchangeName);
int declareQueue(char* va_exchangeName);

int setAppID(amqp_basic_properties_t *va_props);
int setContentType(amqp_basic_properties_t *va_props, const char *va_contentType);
int setCorrelationId(amqp_basic_properties_t *va_props, unsigned int va_counterMsgPublish );

void updateCounterValue(unsigned int value);
unsigned int retrieveLastCounterValue();
int deconnectionRabbitmq();
int getAppId(amqp_basic_properties_t va_props, char* va_appId);

int consumeMessages() ;
void onMessage(amqp_envelope_t envelope);

int registerCallbackForMessageType(int va_IdentMsg, void (*va_Callback)(), void *va_cb_arguments);
int getQueueName(char* va_queueName, char* va_exchangeName);
int checkGetHostName(char *va_hostname, size_t size);
int createExchangeAndQueue(char* va_exchangeName);
int publishMessage(char* va_message, char* va_exchangeName, int va_IdentifMsgv);
int connectionToRabbitMQ();

int exchangeExists(amqp_connection_state_t conn);
int queueExists(amqp_connection_state_t conn);
int abonnementTopic(char* va_exchangeName);


void (*findCallbackForMessageType(int va_IdentMsg))(amqp_envelope_t envelope);

int getCorrelationId(amqp_basic_properties_t va_props, char** va_correlationId);
long long getCurrentTimestamp();
void setMessageExpiration(amqp_basic_properties_t* va_props, int va_expirationSec);

#endif //COMMONRABBITMQNEW_H


