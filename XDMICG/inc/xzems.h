#ifndef xzems
#define xzems

#ifdef _TIBCO_EMS


#ifdef _RABBITMQ


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>
#include <unistd.h>
#include <libgen.h>
#include <rabbitmq-c/framing.h>
#include <sys/time.h>
#include "xdc.h"
#include "xdy.h"
#include "xzst.h"

#include <string.h>
#include <stdarg.h>

#include <stdbool.h>

/*variables statiques du module*/
#define USERNAME "admin"
#define PASSWORD "magistra"
#define PORT 5672
//#define AMQP_DEFAULT_FRAME_SIZE 131072   // frame_max contrôle la taille maximale des données que le client peut envoyer ou recevoir dans une seule trame. Il est exprimé en octets

#define TTL_MS_MESSAGE_IN_QUEUE                 20000
#define CONTENT_TYPE_TEXT_PLAIN                 "text/plain"
#define CONTENT_TYPE_JSON                       "application/json"
#define CONTENT_TYPE_BYTES                      "application/octet-stream"
#define CONTENT_TYPE_XML                        "application/xml"

#define PTR_CURRENT_FIELD                       "ptr_current_read_field"

#define OFFSET_CHANNEL_SUPERV                   0       // 4 tâches
#define OFFSET_CHANNEL_BASEDD                   0       // 16 tâches + 3 tâches par district du sous-concessionnaire 
#define OFFSET_CHANNEL_SAIDEC                   0       // 10 tâches
#define OFFSET_CHANNEL_GESIHM                   0       // 3 tâches
#define OFFSET_CHANNEL_GECONF                   0       // 1 tâche
#define OFFSET_CHANNEL_EQUEXT                   0       


typedef struct {
        int     Numero;
        void     (*CB)(T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA, T_CB_ARG);
        void    *CB_ARG;
} T_CALLBACKS_MSG;

const char *tibems_errorString(amqp_rpc_reply_t va_reply);

int TipcMsgNextInt4(amqp_envelope_t va_msg, int *);
int TipcMsgNextInt2(amqp_envelope_t va_msg, short *);

int TipcMsgNextReal(amqp_envelope_t va_msg, double *va_value);
int TipcMsgNextStr(amqp_envelope_t va_msg, char **va_value);
// int TipcMsgNextStr(amqp_envelope_t *envelope, T_STR *);

int deconnectionRabbitmq();

void TipcSrvSubjectGetSubscribe(char *va_topic, int *va_etat);
int XDG_QuitterDG(char *va_topic);

int check_reply(amqp_rpc_reply_t va_reply, char* va_error_msg);
int XZEMS_02Connexion(char *va_URL);
int XZEMS_03IsTopic(char *name); 								// Le but de cette fonction est uniquement de savoir si on a 0 abonné sur un topic ou x abonnés 

int checkGetHostName(char *va_hostname, size_t size);
int setExecutableName();
int getQueueName(char* va_queueName, char* va_exchangeName);

int verifyExchangeInfo(char* va_exchange);
int declareQueue(char *va_topic);
int declareExchange(char *va_topic);
int bindExchangeToQueue(char *va_topic, char* va_queueName);
int XDG_JoindreDG(char *va_topic);

int onMessage(amqp_envelope_t envelope);
int consumeMessages(amqp_envelope_t va_envelope);
int TipcSrvProcessCbCreate(int va_msg, void (*CB)(T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG ), void *);
void TipcMonSubjectsWatch(XDY_FonctionInt);

int XZEMS_03DestroyMsg(amqp_envelope_t va_msg);
int TipcMsgGetType(amqp_envelope_t envelope, int *type);

void TipcMsgGetDest(amqp_envelope_t envelope, T_STR *str);

T_IPC_MSG TipcSrvMsgNext(T_REAL8 timeout);

void TipcMsgGetSender(amqp_envelope_t,T_STR *);
int TipcMsgDestroy(T_IPC_MSG);

int isMessageNotNULL(T_IPC_MSG va_msg);

#else

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <tibems/tibems.h>
#include <tibems/emsadmin.h>
#include "xdc.h"
#include "xdy.h"
#include "xzst.h"

#define TIMEOUT_MAIN_LOOP_MS                    1000
#define TIMEOUT_LOSS_CONNECTION_TERM_MS         30000

typedef struct {
        int     Numero;
        void     (*CB)(T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA, T_CB_ARG);
        void    *CB_ARG;
} T_CBEMS;

typedef struct S_Dest {
	tibemsDestination dest;
        tibemsMsgConsumer consumer;
	struct S_Dest *suivant;
} T_Dest;
	
int XDG_JoindreDG(char * va_topic);
int XDG_QuitterDG(char * va_topic);

int TipcSrvProcessCbCreate(int va_msg, void (*CB)(T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG ), void *);

int TipcMsgRead(tibemsMsg va_msg, ...) ;

int TipcSrvMsgWrite(char *va_topic, int va_num_msg,int flag, ...);

int XZEMS_02Connexion(char *va_URL);
int XZEMS_03DestroyMsg(tibemsMsg);
int TipcMtLookupByNum(int num);

void TipcSrvSubjectGetSubscribe(char *va_topic, int *va_etat);
double TutGetWallTime();
double TutGetCurrentTime();
int TipcSrvDestroy(int);
void TutCbDestroy(void     (*CB)());
tibemsCollection XZEMS_03IsTopic(char *);
T_BOOL TipcMonSubjectSubscribeGetWatch(char *, T_BOOL *);
int TipcMonSubjectSubscribeSetWatch(char *, T_BOOL);

int TipcMsgGetType(tibemsMsg , int *);


void TipcMsgGetSender(tibemsMsg ,T_STR   *);
void TipcMsgGetDest(tibemsMsg ,T_STR *);
int TipcMsgNextInt4(tibemsMsg ,T_INT4 *);
int TipcMsgNextStr(tibemsMsg ,T_STR *);
T_IPC_MSG TipcSrvMsgNext(T_REAL8 );
void TipcMsgDestroy(T_IPC_MSG);
void TipcMonSubjectsWatch(XDY_FonctionInt);
const char *tibems_errorString(tibemsErrorContext          errContext);

int isMessageNotNULL(T_IPC_MSG va_msg);

#endif

#endif

#endif
