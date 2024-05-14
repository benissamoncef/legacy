#ifndef EOPC_OPC_H_
#define EOPC_OPC_H_

#include <stdio.h>
#include <stdlib.h>

#include <uaclient_config.h>
#include <opcua_trace.h>
#include <opcua_string.h>
#include <opcua_datetime.h>
#include <opcua_memory.h>
#include <opcua_types.h>

#include <uaclient_module.h>
#include <uaclient_session.h>
#include <uaclient_subscription.h>
#include <uaclient_discovery.h>

#include <uabase_statuscode.h>

#include <opcua_pkifactory.h>

#define UACLIENT_APPLICATION_NAME   "UaSdkC - SAE Connector"
#define UACLIENT_APPLICATION_URI    "urn:Sae:UaSdkC:Client"
#define UACLIENT_PRODUCT_URI        "urn:Sae:UaSdkC:Client"

#define EOPC_MAX_CLIENT_SUBSCRIPTION 100
#define EOPC_MAX_NODE_ID_LENGTH 100

typedef enum _EopcState {
	/**
	 * stack opc-ua en cours d'initialisation
	 */
	State_Initialize,

	/**
	 * stack opc-ua initialisée, en attente de configuration (hostname/port du serveur)
	 */
	State_Idle,

	/**
	 * recherche des configurations supportées par le serveur
	 */
	State_Discover,

	/**
	 * la découverte est terminé, initialisation de la session avec un des endpoint
	 */
	State_Discovered,

	/**
	 * une configuration a été reçu, en cours de connexion au serveur
	 */
	State_Connect,

	/**
	 * le serveur a accepté la connexion
	 */
	State_Connected,

	/**
	 * la création d'un abonnement est en cours sur le serveur
	 */
	State_SubscriptionCreate,

	/**
	 * la création de l'abonnement est acceptée par le serveur
	 */
	State_SubscriptionCreated,

	/**
	 * enregistrement de nouveaux datapoint dans l'abonnement
	 */
	State_MonitoredItemCreate,

	/**
	 * statut nominal, en attente des notifications coté serveur opc-ua
	 */
	State_MonitoredItemCreated,

	/**
	 * demande de suppression des items enregistrés
	 */
	State_MonitoredItemDelete,

	/**
	 * les items on été supprimés de l'abonnement
	 */
	State_MonitoredItemDeleted,

	/**
	 * suppression d'abonnement
	 */
	State_SubscriptionDelete,

	/**
	 * abonnement supprimé
	 */
	State_SubscriptionDeleted,

	/**
	 * déconnexion en cours
	 */
	State_Disconnect,

	/**
	 * déconnection effectuée
	 */
	State_Disconnected,

	/**
	 * la lib est déchargée
	 */
	State_Shutdown,

	/**
	 * c'est la panique...
	 */
	State_Error,
} EopcState;

typedef struct _EopcConfig {
	char Endpoint[500];
	char Username[500];
	char Password[500];
	char ClientCertificateLocation[1024];
	char ClientPrivateKeyLocation[1024];

	void *Data;
	void (*DataChangedCallBack)(void *, char *, char *);
} EopcServerConfig;

typedef struct _EopcStackContext {
	OpcUa_Handle ProxyStubPlatformLayer;
	OpcUa_ProxyStubConfiguration ProxyStubConfiguration;
} EopcStackContext;

typedef enum _EopcMonitoredItemsState {
	MonitoredItemState_Free,
	MonitoredItemState_Pending,
	MonitoredItemState_Subscribed
} EopcMonitoredItemsState;

typedef struct _EopcMonitoredItems {
	char NodeId[EOPC_MAX_NODE_ID_LENGTH];
	OpcUa_MonitoredItemCreateRequest Request;
	int RequestIndex;
	EopcMonitoredItemsState Status;
	OpcUa_Int32 ClientHandle;
} EopcMonitoredItems;

typedef struct _EopcClientContext {
	UaClient Client;
	OpcUa_Boolean ClientInitialized;
	OpcUa_EndpointDescription *pSelectedEndpoint;
	UaClient_Configuration *pClientConfiguration;
	UaClient_Discovery *pDiscovery;
	UaClient_Session *pSession;
	UaClient_Subscription *pSubscription;

	EopcMonitoredItems MonitoredItems[EOPC_MAX_CLIENT_SUBSCRIPTION];
	unsigned int PendingMonitoredItemRequestCount;
} EopcClientContext;

typedef struct _EopcContext {
	/* état actuel de la connexion */
	EopcState State;

	/* demande d'arrêt */
	OpcUa_Boolean Stop;

	/* context de la stack OPC */
	EopcStackContext StackContext;

	/* configuration du serveur */
	EopcServerConfig ServerConfiguration;

	/* contexte du client opcua */
	EopcClientContext ClientContext;
} EopcContext;

void Eopc_InitContext(EopcContext *context);
void Eopc_ResetContext(EopcContext *context);
void Eopc_Work(EopcContext *context);
void Eopc_SetClientCertificate(EopcContext *context, char *certificate, char *key);
void Eopc_SetConfig(EopcContext *context, char *endpoint, char *username, char *password);
void Eopc_SetDataChangeCallBack(EopcContext *context, void *data, void (*callback)(void *, char *, char *));
int  Eopc_Listen(EopcContext *context, char *nodeId);
int  Eopc_Write_String(EopcContext *context, char *nodeId, char *data);
int  Eopc_Write_Int32(EopcContext *context, char *nodeId, char *data);
void Eopc_Stop(EopcContext *context);

#endif

