#include "eopc_lib.h"

void Eopc_StackInitialize(EopcContext *context) {
	OpcUa_ProxyStubConfiguration *pProxyStubConfiguration;
	OpcUa_Handle *pProxyStubPlatformLayer;

	pProxyStubConfiguration = &(context->StackContext.ProxyStubConfiguration);
	pProxyStubPlatformLayer = &(context->StackContext.ProxyStubPlatformLayer);

	OpcUa_InitializeStatus(OpcUa_Module_Client, "InitializeOpcUaStack");

	OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "Initializing Stack...\n");
	OpcUa_ProxyStubConfiguration_InitializeDefault(pProxyStubConfiguration);

	pProxyStubConfiguration->bProxyStub_Trace_Enabled = OpcUa_True;
	pProxyStubConfiguration->uProxyStub_Trace_Level = OPCUA_TRACE_OUTPUT_LEVEL_WARNING;

	/* initialisation de la stack */
	uStatus = UaBase_Module_InitializeUaStack(pProxyStubPlatformLayer, pProxyStubConfiguration);
	OpcUa_GotoErrorIfBad(uStatus);

	/* initialisation ok */
	context->State = State_Idle;

	return;

	OpcUa_BeginErrorHandling;
	Eopc_StackCleanup(context);
}

void Eopc_StackCleanup(EopcContext *context) {
	OpcUa_Handle *pProxyStubPlatformLayer;

	pProxyStubPlatformLayer = &(context->StackContext.ProxyStubPlatformLayer);

	OpcUa_InitializeStatus(OpcUa_Module_Client, "CleanupOpcUaStack");

	OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "Cleanup Stack...\n");
	uStatus = UaBase_Module_ClearUaStack(pProxyStubPlatformLayer);

	/* fin du cleanup */
	context->State = State_Shutdown;

	return;

	OpcUa_BeginErrorHandling;
	/* impossible de clean up */
	exit(-1);
}

static void Eopc_ConnectionStatusChanged_CB(UaClient_Session *a_pSession, UaClient_ConnectionStatus a_status) {
	EopcContext *pClientContext = a_pSession->pUserData;
	const char *pStatus = "INVALID";
	switch (a_status) {
	case UaClient_ConnectionStatus_Disconnected:
		pStatus = "Disconnected";
		if (pClientContext->State == State_Connect) {
			OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "Failed to connect to server!\n");
		}
		if (pClientContext->State == State_Disconnect) {
			pClientContext->State = State_Disconnected;
		} else {
			pClientContext->State = State_Error; /* TODO gestion des erreurs */
		}
		break;
	case UaClient_ConnectionStatus_Connected:
		pStatus = "Connected";
		if (pClientContext->State == State_Connect) {
			pClientContext->State = State_Connected;
		} else {
			pClientContext->State = State_Error; /* TODO gestion des erreurs */
		}
		break;
	case UaClient_ConnectionStatus_Connecting:
		pStatus = "Connecting";
		break;
	case UaClient_ConnectionStatus_ConnectionWarningWatchdogTimeout:
		pStatus = "ConnectionWarningWatchdogTimeout";
		break;
	case UaClient_ConnectionStatus_ConnectionErrorClientReconnect:
		pStatus = "ConnectionErrorClientReconnect";
		break;
	case UaClient_ConnectionStatus_SessionAutomaticallyRecreated:
		pStatus = "SessionAutomaticallyRecreated";
		break;
	default:
		break;
	}

	OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "Eopc_ConnectionStatusChanged_CB: %s\n", pStatus);
}

static OpcUa_Boolean Eopc_ConnectError_CB(UaClient_Session *a_pSession, UaClient_ConnectServiceType a_serviceType, OpcUa_StatusCode a_error, OpcUa_Boolean a_overridable) {
	EopcContext *pClientContext = a_pSession->pUserData;
	const char *pServiceType = "INVALID";
	switch (a_serviceType) {
	case UaClient_ConnectServiceType_CertificateValidation:
		pServiceType = "CertificateValidation";

		/* bypass la vérification du certificat serveur */
		if (a_overridable == OpcUa_True) {
			return OpcUa_True;
		}
		break;
	case UaClient_ConnectServiceType_OpenSecureChannel:
		pServiceType = "OpenSecureChannel";
		break;
	case UaClient_ConnectServiceType_CreateSession:
		pServiceType = "CreateSession";
		break;
	case UaClient_ConnectServiceType_UserIdentityToken:
		pServiceType = "UserIdentityToken";
		break;
	case UaClient_ConnectServiceType_ActivateSession:
		pServiceType = "ActivateSession";
		break;
	default:
		break;
	}

	OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "Eopc_ConnectError_CB:\n");
	OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "    ServiceType: %s\n", pServiceType);
	OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "    Error: %s (0x%08x)\n", UaBase_StatusCode_ToString(a_error), a_error);
	OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "    Overridable: %s\n", a_overridable == OpcUa_False ? "false" : "true");
	pClientContext->State = State_Error; /* TODO gestion des erreurs */
	return OpcUa_False;
}

void Eopc_Discovery_CB(const UaClient_Discovery *a_pDiscovery,
                                OpcUa_ResponseHeader *a_pResponseHeader,
                                OpcUa_Int32 a_NoOfEndpoints,
                                OpcUa_EndpointDescription *a_pEndpoints,
                                OpcUa_Void *a_pUserData)
{
	EopcContext *context = (EopcContext*)a_pUserData;
    OpcUa_ReferenceParameter(a_pDiscovery);

    if (OpcUa_IsGood(a_pResponseHeader->ServiceResult)) {
    	OpcUa_Int32 i;
    	for (i = 0; i < a_NoOfEndpoints; i++) {
    		if (a_pEndpoints[i].SecurityMode == OPCUA_SECURITY_MODE) {
    			OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "Eopc_Discovery_CB: Sélection de l'endpoint[%d] : %s\n", i, OpcUa_String_GetRawString(&a_pEndpoints[i].EndpointUrl));
    			OpcUa_EndpointDescription_Copy(&a_pEndpoints[i], &context->ClientContext.pSelectedEndpoint);
    			context->State = State_Discovered;
    			return;
    		}
    	}

    	OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "Eopc_Discovery_CB: Pas d'endpoint valide découvert\n");
    	context->State = State_Error; /* TODO gestion des erreurs */
    } else {
    	OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "Eopc_Discovery_CB:\n");
    	OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "    Error: %s (0x%08x)\n", UaBase_StatusCode_ToString(a_pResponseHeader->ServiceResult), a_pResponseHeader->ServiceResult);
    	context->State = State_Error; /* TODO gestion des erreurs */
    }
}

void Eopc_ClientInitialize(EopcContext *context) {
	OpcUa_Trace_ChangeTraceLevel(OPCUA_TRACE_OUTPUT_LEVEL_DEBUG); 

	OpcUa_InitializeStatus(OpcUa_Module_Client, "ClientMain"); OpcUa_DeclareErrorTraceModule(OpcUa_Module_Client);
	/* initialise le client */
	uStatus = UaClient_Initialize(&(context->ClientContext.Client));
	OpcUa_GotoErrorIfBad(uStatus);
	context->ClientContext.ClientInitialized = OpcUa_True;

	/* initialise une configuration */
	context->ClientContext.pClientConfiguration = UaClient_GetConfiguration();

	/* configure ApplicationDescription */
	OpcUa_String_AttachReadOnly(&context->ClientContext.pClientConfiguration->ApplicationDescription.ApplicationUri, UACLIENT_APPLICATION_URI);
	OpcUa_String_AttachReadOnly(&context->ClientContext.pClientConfiguration->ApplicationDescription.ProductUri, UACLIENT_PRODUCT_URI);
	OpcUa_String_AttachReadOnly(&context->ClientContext.pClientConfiguration->ApplicationDescription.ApplicationName.Text, UACLIENT_APPLICATION_NAME);
	context->ClientContext.pClientConfiguration->ApplicationDescription.ApplicationType = OpcUa_ApplicationType_Client;

	/* désactivation de la PKI puisqu'on ne recevra pas le certificat de teimu */
	context->ClientContext.pClientConfiguration->PkiConfig.strPkiType = (char*)OPCUA_P_PKI_TYPE_OPENSSL;
	context->ClientContext.pClientConfiguration->PkiConfig.strTrustedCertificateListLocation = ".";

	UaClient_Discovery *pDiscovery;
	UaClient_Discovery_Create(&pDiscovery);
	OpcUa_String *url = OpcUa_String_FromCString(context->ServerConfiguration.Endpoint);
	UaClient_Discovery_BeginGetEndpoints(pDiscovery,
			OpcUa_Null,
			url,
			0,
			OpcUa_Null,
			0,
			OpcUa_Null,
			Eopc_Discovery_CB,
			context);
	context->ClientContext.pDiscovery = pDiscovery;
	context->State = State_Discover;
	return;

	OpcUa_BeginErrorHandling;
	Eopc_ClientCleanup(context);
}

void Eopc_ClientCleanup(EopcContext *context) {
	context->ClientContext.pSession->pUserData = OpcUa_Null;
	if (context->ClientContext.pSession != OpcUa_Null) {
		UaClient_Session_Delete(&(context->ClientContext.pSession));
	}

	if (context->ClientContext.pDiscovery != OpcUa_Null) {
		UaClient_Discovery_Delete(&(context->ClientContext.pDiscovery));
	}

	if (context->ClientContext.pSelectedEndpoint != OpcUa_Null) {
		OpcUa_Free(context->ClientContext.pSelectedEndpoint);
		context->ClientContext.pSelectedEndpoint = OpcUa_Null;
	}

	/* UaClient_Clear clears the PkiConfig of the client and attempts to free the strings
	 set there. As we have set literal string constants, we don't want those to be freed, so
	 we set them to NULL by initializing the structure. */
	OpcUa_CertificateStoreConfiguration_Initialize(&context->ClientContext.pClientConfiguration->PkiConfig);

	/* Clean Up Client */
	if (context->ClientContext.ClientInitialized != OpcUa_False) {
		UaClient_Clear();
	}
}

void Eopc_SessionCreate(EopcContext *context) {
	UaClient_Session_Callback sessionCallback;
	UaClient_Session *pSession;
	OpcUa_StatusCode uStatus;

   	/* attache les callbacks de gestion de la connexion */
	OpcUa_MemSet(&sessionCallback, 0, sizeof(sessionCallback));
	sessionCallback.pfConnectionStatusChanged_CB = Eopc_ConnectionStatusChanged_CB;
	sessionCallback.pfConnectError_CB = Eopc_ConnectError_CB;
	uStatus = UaClient_Session_Create(&sessionCallback, &pSession);
	context->ClientContext.pSession = pSession;
	OpcUa_GotoErrorIfBad(uStatus);

	/* défini l'endpoint du serveur */
	OpcUa_EndpointDescription_Clear(&pSession->EndpointDescription);
	pSession->EndpointDescription = *context->ClientContext.pSelectedEndpoint;

	/* désactive la reconnexion automatique */
	context->ClientContext.pSession->AutomaticReconnect = OpcUa_False;

	/* configure le chiffrement */
	context->ClientContext.pSession->EndpointDescription.SecurityMode = OpcUa_MessageSecurityMode_SignAndEncrypt;
	OpcUa_String_AttachReadOnly(&context->ClientContext.pSession->ClientCertificateLocation, context->ServerConfiguration.ClientCertificateLocation);
	OpcUa_String_AttachReadOnly(&context->ClientContext.pSession->ClientPrivateKeyLocation, context->ServerConfiguration.ClientPrivateKeyLocation);
	context->ClientContext.pSession->DisableDomainCheck = OpcUa_True;
	context->ClientContext.pSession->DisableCertificateUsageCheck = OpcUa_True;
	context->ClientContext.pSession->DisableApplicationUriCheck = OpcUa_True;

	/* configure le login / password */
	context->ClientContext.pSession->UserToken.TokenType = OpcUa_UserTokenType_UserName;
	OpcUa_String_AttachReadOnly(&context->ClientContext.pSession->UserToken.Token.UserName.User, context->ServerConfiguration.Username);
	OpcUa_String_AttachReadOnly(&context->ClientContext.pSession->UserToken.Token.UserName.Password, context->ServerConfiguration.Password);

	/* attache notre context pour tous les callbacks */
	context->ClientContext.pSession->pUserData = context;

	/* démarre le client */
	uStatus = UaClient_StartUp(&(context->ClientContext.Client));
	OpcUa_GotoErrorIfBad(uStatus);

	uStatus = UaClient_Session_BeginConnect(context->ClientContext.pSession);
	OpcUa_GotoErrorIfBad(uStatus);

	/* en cours de connexion */
	context->State = State_Connect;

	return;

	OpcUa_BeginErrorHandling;
	context->State = State_Error; /* TODO gestion des erreurs */
}

static void Eopc_Subscription_Created_CB(UaClient_Subscription *a_pUaSubscription, OpcUa_StatusCode a_status, OpcUa_ResponseHeader *a_pResponseHeader,
		OpcUa_Void *a_pUserData) {
	EopcContext *context = a_pUaSubscription->pUserData;

	if (OpcUa_IsGood(a_status)) {
		if (context->State == State_SubscriptionCreate) {
			context->State = State_SubscriptionCreated;

			/* log les infos */
			OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "Eopc_Subscription_Created_CB succeeded (0x%08x)\n", a_status);
			OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "    Revised PublishingInterval: %f\n", a_pUaSubscription->RevisedPublishingInterval);
			OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "    Revised LifetimeCount: %u\n", a_pUaSubscription->RevisedLifetimeCount);
			OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "    Revised MaxKeepAliveCount: %u\n", a_pUaSubscription->RevisedMaxKeepAliveCount);
		} else {
			context->State = State_Error; /* TODO gestion des erreurs */
		}
	} else {
		OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "Eopc_Subscription_Created_CB failed (0x%08x)\n", a_status);
		context->State = State_Error; /* TODO gestion des erreurs */
	}
}

static void Eopc_Subscription_DataChange_CB(UaClient_Subscription *a_pSubscription, OpcUa_Int32 a_noOfMonitoredItems, OpcUa_MonitoredItemNotification *a_pMonitoredItems,
		OpcUa_Int32 a_noOfDiagnosticInfos, OpcUa_DiagnosticInfo *a_pDiagnosticInfos) {
	EopcContext *context = a_pSubscription->pUserData;
	char value[200];
	int i = 0;
	int j = 0;

	for (i = 0; i < a_noOfMonitoredItems; i++) {
		/* recherche le node id */
		char *nodeId = NULL;
		for (j = 0; j < EOPC_MAX_CLIENT_SUBSCRIPTION; j++) {
			if (context->ClientContext.MonitoredItems[j].ClientHandle == a_pMonitoredItems[i].ClientHandle) {
				nodeId = context->ClientContext.MonitoredItems[j].NodeId;
				break;
			}
		}

		/* TODO gérer l'erreur si on ne trouve pas la handle */


		/* converti la lecture en string */
		if (a_pMonitoredItems[i].Value.Value.ArrayType == OpcUa_VariantArrayType_Scalar) {
			memset(value, 200, sizeof(char));

			if (a_pMonitoredItems[i].Value.Value.Datatype == OpcUaType_Int32) {
				sprintf(value, "%d", a_pMonitoredItems[i].Value.Value.Value.Int32);
			} else if (a_pMonitoredItems[i].Value.Value.Datatype == OpcUaType_Int16) {
				sprintf(value, "%d", a_pMonitoredItems[i].Value.Value.Value.Int16);
			} else if (a_pMonitoredItems[i].Value.Value.Datatype == OpcUaType_SByte) {
				sprintf(value, "%d", a_pMonitoredItems[i].Value.Value.Value.SByte);
			} else if (a_pMonitoredItems[i].Value.Value.Datatype == OpcUaType_String) {
				sprintf(value, "%s", OpcUa_String_GetRawString(&a_pMonitoredItems[i].Value.Value.Value.String));
			}

			OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "Eopc_Subscription_DataChange_CB [%s] = %s\n", nodeId, value);

			if (context->ServerConfiguration.DataChangedCallBack != NULL) {
				context->ServerConfiguration.DataChangedCallBack(context->ServerConfiguration.Data, nodeId, value);
			}
		} else {
			OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "Eopc_Subscription_DataChange_CB [%s] : Unsupported ArrayType\n");
		}
	}
}

void Eopc_SubscriptionCreate(EopcContext *context) {
	UaClient_Subscription_Callback subscriptionCallback;
	UaClient_Subscription *pSubscription = OpcUa_Null;
	OpcUa_StatusCode uStatus;

	/* mise à jou de l'état */
	context->State = State_SubscriptionCreate;

	/* affectation des callbacks */
	OpcUa_MemSet(&subscriptionCallback, 0, sizeof(subscriptionCallback));
	subscriptionCallback.pfDataChange_CB = Eopc_Subscription_DataChange_CB;

	uStatus = UaClient_Subscription_Create(context->ClientContext.pSession, &subscriptionCallback, &pSubscription);
	if (OpcUa_IsBad(uStatus)) {
		context->State = State_Error; /* TODO gestion des erreurs */
		return;
	}

	pSubscription->pUserData = context;
	pSubscription->PublishingInterval = 1000.0;

	uStatus = UaClient_Subscription_BeginCreateSubscription(pSubscription, OpcUa_Null, Eopc_Subscription_Created_CB, OpcUa_Null);
	if (OpcUa_IsBad(uStatus)) {
		UaClient_Subscription_Delete(&pSubscription);

		context->State = State_Error; /* TODO gestion des erreurs */
		return;
	}

	context->ClientContext.pSubscription = pSubscription;
}

void Eopc_SubscriptionDelete(EopcContext *context) {
	UaClient_Subscription_Delete(&context->ClientContext.pSubscription);
	context->State = State_SubscriptionDelete;
}

static void Eopc_MonitoredItems_Created_CB(const UaClient_Subscription *a_pUaSubscription, OpcUa_ResponseHeader *a_pResponseHeader, OpcUa_Int32 a_NoOfResults, OpcUa_MonitoredItemCreateResult *a_pResults, OpcUa_Int32 a_NoOfDiagnosticInfos, OpcUa_DiagnosticInfo *a_pDiagnosticInfos, OpcUa_Void *a_pUserData)
{
	EopcContext *context = a_pUaSubscription->pUserData;
	int i = 0;
	int j = 0;

	if (OpcUa_IsGood(a_pResponseHeader->ServiceResult)) {
		OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "Eopc_MonitoredItems_Created_CB succeeded, %d new subscription(s)\n", a_NoOfResults);
		context->State = State_MonitoredItemCreated;

		for (i = 0; i < a_NoOfResults; i++) {
			/* decrémente le nombre de pending requests */
			context->ClientContext.PendingMonitoredItemRequestCount--;

			/* recherche la demande de souscription d'origine */
			EopcMonitoredItems *item = NULL;
			for (j = 0; j < EOPC_MAX_CLIENT_SUBSCRIPTION; j++) {
				if (context->ClientContext.MonitoredItems[j].Status == MonitoredItemState_Pending && context->ClientContext.MonitoredItems[j].RequestIndex == i) {
					item = &context->ClientContext.MonitoredItems[j];
				}
			}

			if (item == NULL) {
				/* ne devrait jamais arriver, les ids devraient être alignés avec la request */
				OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "Eopc_MonitoredItems_Created_CB cannot find original request index (%d)\n", i);
				continue;
			}

			/* supprime l'id de la souscription pour ne jamais être reselectionné */
			item->RequestIndex = -1;

			OpcUa_Int32 clientHandle = item->ClientHandle;
			char * nodeId = item->NodeId;

			OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "    [%d]: ClientHandle:            %d\n", i, clientHandle);
			OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "    [%d]: MonitoredItemId:         %u\n", i, a_pResults[i].MonitoredItemId);
			OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "    [%d]: RevisedSamplingInterval: %f\n", i, a_pResults[i].RevisedSamplingInterval);
			OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "    [%d]: RevisedQueueSize:        %d\n", i, a_pResults[i].RevisedQueueSize);
			OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "    [%d]: Status:                  0x%08x\n", i, a_pResults[i].StatusCode);

			if (OpcUa_IsGood(a_pResults[i].StatusCode)) {
				item->Status = MonitoredItemState_Subscribed;
			} else {
				OpcUa_Trace(OPCUA_TRACE_LEVEL_ERROR, "Eopc_MonitoredItems_Created_CB Subscription to [%s] failed (0x%08x)\n", nodeId, a_pResults[i].StatusCode);
				item->Status = MonitoredItemState_Free;

				/* libère l'allocation mémoire de l'item */
				Eopc_MonitoredItemFreeContext(item);
			}
		}
	} else {
		OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "Eopc_MonitoredItems_Created_CB failed (0x%08x)\n", a_pResponseHeader->ServiceResult);
		context->State = State_Error; /* TODO gestion des erreurs */
	}
}

void Eopc_MonitoredItemsCreate(EopcContext *context) {
	if (context->ClientContext.PendingMonitoredItemRequestCount <= 0) {
		return;
	}

	OpcUa_MonitoredItemCreateRequest requests[EOPC_MAX_CLIENT_SUBSCRIPTION];
	int requestIndex = 0;
	int i = 0;
	for (i = 0; i < EOPC_MAX_CLIENT_SUBSCRIPTION; i++) {
		if (context->ClientContext.MonitoredItems[i].Status == MonitoredItemState_Pending) {
			context->ClientContext.MonitoredItems[i].RequestIndex = requestIndex;
			requests[requestIndex++] = context->ClientContext.MonitoredItems[i].Request;
		}
	}

	/* lance la creation des abonnements */
	OpcUa_StatusCode uStatus = UaClient_Subscription_BeginCreateMonitoredItems(
			context->ClientContext.pSubscription,
			OpcUa_Null,
			OpcUa_TimestampsToReturn_Both,
			context->ClientContext.PendingMonitoredItemRequestCount,
			requests,
			Eopc_MonitoredItems_Created_CB,
			OpcUa_Null);

	if (OpcUa_IsBad(uStatus)) {
		OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "Eopc_MonitoredItemsCreate failed (0x%08x)\n", uStatus);
		context->State = State_Error; /* TODO gestion des erreurs */
		return;
	}

	context->State = State_MonitoredItemCreate;
}


void Eopc_MonitoredItemFreeContext(EopcMonitoredItems *item) {
	if (item->Status != MonitoredItemState_Free) {
		OpcUa_String_Clear(&item->Request.ItemToMonitor.NodeId.Identifier.String);
	}

	item->Status = MonitoredItemState_Free;
}

void Eopc_MonitoredItemsFreeContext(EopcContext *context) {
	int i = 0;
	for (i = 0; i < EOPC_MAX_CLIENT_SUBSCRIPTION; i++) {
		Eopc_MonitoredItemFreeContext(&context->ClientContext.MonitoredItems[i]);
	}
}

void Eopc_WriteNodeValue_Send(EopcContext *context) {
	OpcUa_StatusCode uStatus = UaBase_TimedDoCom(200);
	if (OpcUa_IsBad(uStatus)) {
		OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "UaBase_DoCom failed (0x%08x)\n", uStatus);
		context->State = State_Error; /* TODO Remplacer par quelque chose de propre */
	}
}

static void Eopc_NodeWrite_CB(const UaClient_Session *a_pSession, OpcUa_ResponseHeader *a_pResponseHeader, OpcUa_Int32 a_NoOfResults, OpcUa_StatusCode *a_pResults, OpcUa_Int32 a_NoOfDiagnosticInfos, OpcUa_DiagnosticInfo *a_pDiagnosticInfos, OpcUa_Void *a_pUserData) {
	EopcContext *pClientContext = a_pSession->pUserData;

	if (OpcUa_IsGood(a_pResponseHeader->ServiceResult)) {
		OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "Eopc_NodeWrite_CB succeeded, %d node(s) written\n", a_NoOfResults);
	} else {
		OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "Eopc_NodeWrite_CB failed (0x%08x)\n", a_pResponseHeader->ServiceResult);
		pClientContext->State = State_Error; /* TODO gestion des erreurs */
	}
}


int  Eopc_WriteNodeValue_String(EopcContext *context, char *nodeId, char *data) {
 	UaClient_ServiceSettings settings;
 	UaClient_ServiceSettings_Initialize(&settings);

 	OpcUa_WriteValue write;
 	OpcUa_WriteValue_Initialize(&write);

 	/* construction du noeud */
 	write.AttributeId = OpcUa_Attributes_Value;
 	write.NodeId.IdentifierType = OpcUa_IdentifierType_String;
 	write.NodeId.NamespaceIndex = OPCUA_NS;
 	OpcUa_String_AttachReadOnly(&write.NodeId.Identifier.String, nodeId);

 	/* affectation de la valeur à attribuer */
	OpcUa_Variant_Initialize(&write.Value.Value);
	write.Value.SourceTimestamp = OpcUa_DateTime_UtcNow();
	write.Value.StatusCode = 0;
	write.Value.Value.ArrayType = OpcUa_VariantArrayType_Scalar;
	write.Value.Value.Datatype = OpcUaType_String;
	OpcUa_String_AttachReadOnly(&write.Value.Value.Value.String, data);

	/* déclanche l'écriture (asynchrone) */
	UaClient_Session_BeginWrite(
			context->ClientContext.pSession,
			&settings,
			1,
			&write,
			Eopc_NodeWrite_CB,
			NULL);

	Eopc_WriteNodeValue_Send(context);

	return 1;
}

int  Eopc_WriteNodeValue_Int32(EopcContext *context, char *nodeId, int data) {
 	UaClient_ServiceSettings settings;
 	UaClient_ServiceSettings_Initialize(&settings);

 	OpcUa_WriteValue write;
 	OpcUa_WriteValue_Initialize(&write);

 	/* construction du noeud */
 	write.AttributeId = OpcUa_Attributes_Value;
 	write.NodeId.IdentifierType = OpcUa_IdentifierType_String;
 	write.NodeId.NamespaceIndex = OPCUA_NS;
 	OpcUa_String_AttachReadOnly(&write.NodeId.Identifier.String, nodeId);

 	/* affectation de la valeur à attribuer */
	OpcUa_Variant_Initialize(&write.Value.Value);
	write.Value.SourceTimestamp = OpcUa_DateTime_UtcNow();
	write.Value.StatusCode = 0;
	write.Value.Value.ArrayType = OpcUa_VariantArrayType_Scalar;
	write.Value.Value.Datatype = OpcUaType_Int32;
	write.Value.Value.Value.Int32 = data;

	/* déclanche l'écriture (asynchrone) */
	UaClient_Session_BeginWrite(
			context->ClientContext.pSession,
			&settings,
			1,
			&write,
			Eopc_NodeWrite_CB,
			NULL);

	Eopc_WriteNodeValue_Send(context);

	return 1;
}

	
