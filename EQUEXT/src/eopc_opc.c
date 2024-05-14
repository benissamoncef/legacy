#include "eopc_opc.h"
#include "eopc_lib.h"

/**
 * Point d'entré princpal, permet de géréer la communication OPC-UA
 */
void Eopc_Work(EopcContext *context) {
	OpcUa_StatusCode uStatus;

	switch (context->State) {
	case State_Initialize:
		Eopc_StackInitialize(context);
		break;

	case State_Idle:
		if (strlen(context->ServerConfiguration.Endpoint) > 0) {
			Eopc_ClientInitialize(context);
		}

		break;

	case State_Discover:
		/* en cours de découverte des services, rien à faire */
		break;

	case State_Discovered:
		Eopc_SessionCreate(context);
		break;

	case State_Connect:
		/* en cours de connexion, rien à faire */
		break;

	case State_Connected:
		/* le client est connecté, demande la création d'une souscription */
		Eopc_SubscriptionCreate(context);
		break;

	case State_SubscriptionCreate:
		/* souscription en cours de création, rien à faire */
		break;

	case State_SubscriptionCreated:
		/* lance la création des monitored items */
		Eopc_MonitoredItemsCreate(context);
		break;

	case State_MonitoredItemCreate:
		/* création des monitored items en cours, rien à faire */
		break;

	case State_MonitoredItemCreated:
		/* client complètement opérationnel, en attente d'évenements */
		if (context->Stop == OpcUa_True) {
			/* si une demande de d'arret survient, lance le nettoyage */
			/* TODO replacer par la version finale */
			Eopc_SubscriptionDelete(context);
		} else if (context->ClientContext.PendingMonitoredItemRequestCount > 0) {
			/* s'il y a des abonnements en attente, les traites en retournant à l'étape State_SubscriptionCreated */
			context->State = State_SubscriptionCreated;
		}

		break;

	case State_MonitoredItemDelete:
		break;

	case State_MonitoredItemDeleted:
		break;

	case State_SubscriptionDelete:
		/* TODO à coder */
		context->State = State_Disconnect;
		UaClient_Session_BeginDisconnect(context->ClientContext.pSession, OpcUa_True);

		break;

	case State_SubscriptionDeleted:
		break;

	case State_Disconnect:
		/* en cours de déconnexion, rien à faire */
		break;

	case State_Disconnected:
		/* on est désormais déconnecté, on nettoie le client */
		Eopc_ClientCleanup(context);
		if (context->Stop == OpcUa_True) {
			Eopc_StackCleanup(context);
		}
		break;

	case State_Shutdown:
		/* le client est à l'arrêt, rien à faire */
		break;

	case State_Error:
		printf("Panic !\n");
		exit(-1);
		break;

	}

	if (context->State > State_Idle && context->State < State_Disconnected) {
		uStatus = UaBase_TimedDoCom(200);
		if (OpcUa_IsBad(uStatus)) {
			OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "UaBase_DoCom failed (0x%08x)\n", uStatus);
			context->State = State_Error; /* TODO Remplacer par quelque chose de propre */
		}
	}
}

void Eopc_InitContext(EopcContext *context) {
	context->State = State_Initialize;
	context->Stop = OpcUa_False;

	context->StackContext.ProxyStubPlatformLayer = OpcUa_Null;

	memset(context->ServerConfiguration.Endpoint, 0, 500 * sizeof(char));
	memset(context->ServerConfiguration.Username, 0, 500 * sizeof(char));
	memset(context->ServerConfiguration.Password, 0, 500 * sizeof(char));
	memset(context->ServerConfiguration.ClientCertificateLocation, 0, 1024 * sizeof(char));
	memset(context->ServerConfiguration.ClientPrivateKeyLocation, 0, 1024 * sizeof(char));
	context->ServerConfiguration.Data = NULL;
	context->ServerConfiguration.DataChangedCallBack = NULL;

	context->ClientContext.ClientInitialized = OpcUa_False;
	context->ClientContext.pClientConfiguration = OpcUa_Null;
	context->ClientContext.pSession = OpcUa_Null;
	context->ClientContext.pSubscription = OpcUa_Null;
	context->ClientContext.pSelectedEndpoint = OpcUa_Null;

	int i = 0;
	for (i = 0; i < EOPC_MAX_CLIENT_SUBSCRIPTION; i++) {
		context->ClientContext.MonitoredItems[i].Status = MonitoredItemState_Free;
	}

	context->ClientContext.PendingMonitoredItemRequestCount = 0;
}

void Eopc_ResetContext(EopcContext *context) {
	Eopc_MonitoredItemsFreeContext(context);
	Eopc_InitContext(context);
}

void Eopc_SetClientCertificate(EopcContext *context, char *certificate, char *key) {
	strcpy(context->ServerConfiguration.ClientCertificateLocation, certificate);
	strcpy(context->ServerConfiguration.ClientPrivateKeyLocation, key);
}

void Eopc_SetConfig(EopcContext *context, char *endpoint, char *username, char *password) {
	strcpy(context->ServerConfiguration.Endpoint, endpoint);
	strcpy(context->ServerConfiguration.Username, username);
	strcpy(context->ServerConfiguration.Password, password);
}

void Eopc_SetDataChangeCallBack(EopcContext *context, void *data, void (*callback)(void *, char *, char *)) {
	context->ServerConfiguration.Data = data;
	context->ServerConfiguration.DataChangedCallBack = callback;
}

int Eopc_Listen(EopcContext *context, char *nodeId) {
	static OpcUa_Int32 clientHandle = 0;

	/* Initialisation d'une requête de souscription */
	OpcUa_MonitoredItemCreateRequest request;
	OpcUa_MonitoredItemCreateRequest_Initialize(&request);

	request.MonitoringMode = OpcUa_MonitoringMode_Reporting;
	request.RequestedParameters.SamplingInterval = 1000.0;
	request.RequestedParameters.QueueSize = 1;
	request.RequestedParameters.DiscardOldest = OpcUa_True;
	request.RequestedParameters.ClientHandle = clientHandle;

	request.ItemToMonitor.AttributeId = OpcUa_Attributes_Value;
	request.ItemToMonitor.NodeId.IdentifierType = OpcUa_IdentifierType_String;
	request.ItemToMonitor.NodeId.NamespaceIndex = 2;
	OpcUa_String_AttachCopy(&request.ItemToMonitor.NodeId.Identifier.String, nodeId);

	/* Recherche une place libre dans la liste des souscriptions */
	int found = 0;
	unsigned int cpt = 0;
	for (cpt = 0; cpt < EOPC_MAX_CLIENT_SUBSCRIPTION; ++cpt) {
		if (context->ClientContext.MonitoredItems[cpt].Status == MonitoredItemState_Free) {
			context->ClientContext.MonitoredItems[cpt].Status = MonitoredItemState_Pending;
			context->ClientContext.MonitoredItems[cpt].ClientHandle = clientHandle;
			context->ClientContext.MonitoredItems[cpt].Request = request;
			strcpy(context->ClientContext.MonitoredItems[cpt].NodeId, nodeId);

			context->ClientContext.PendingMonitoredItemRequestCount++;
			clientHandle++;

			break;
		}
	}

	return found;
}

int Eopc_Write_String(EopcContext *context, char *nodeId, char *data) {
	return Eopc_WriteNodeValue_String(context, nodeId, data);
}

int Eopc_Write_Int32(EopcContext *context, char *nodeId, char *data) {
	/* conversion du type */
	int value = 0;
	sscanf(data, "%d", &value);

	return Eopc_WriteNodeValue_Int32(context, nodeId, value);
}

void Eopc_Stop(EopcContext *context) {
	context->Stop = OpcUa_True;
}

