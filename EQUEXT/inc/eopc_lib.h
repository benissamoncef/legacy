#ifndef EOPC_LIB_H_
#define EOPC_LIB_H_

#include "eopc_opc.h"

#define OPCUA_NS 2
#define OPCUA_SECURITY_MODE OpcUa_MessageSecurityMode_SignAndEncrypt

void Eopc_StackInitialize(EopcContext *context);
void Eopc_StackCleanup(EopcContext *context);

void Eopc_ClientInitialize(EopcContext *context);
void Eopc_ClientCleanup(EopcContext *context);

void Eopc_SessionCreate(EopcContext *context);

void Eopc_SubscriptionCreate(EopcContext *context);
void Eopc_SubscriptionDelete(EopcContext *context);

void Eopc_MonitoredItemsCreate(EopcContext *context);
void Eopc_MonitoredItemFreeContext(EopcMonitoredItems *context);
void Eopc_MonitoredItemsFreeContext(EopcContext *context);

int  Eopc_WriteNodeValue_String(EopcContext *context, char *nodeId, char *data);
int  Eopc_WriteNodeValue_Int32(EopcContext *context, char *nodeId, int data);

#endif

