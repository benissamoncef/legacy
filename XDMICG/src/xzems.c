/* Define a positionner en fonction de la cible voulue */
/* #define _RABBITMQ : compilation pour Rabbitmq */
/* pas de #define _RABBITMQ : compilation EMS */
// #define _RABBITMQ

//#ifdef _TIBCO_EMS

#ifndef _RABBITMQ
#include <xzsem.h>

#include "time.h"
#include <sys/time.h>
#include "xzems.h"
#include "xzsm.h"

/*variables statiques du module*/
char*                           serverUrl    = NULL;
char*                           userName     = "MIGRAZUR";
char*                           password     = "MIGRAZUR";
tibems_bool                     useTopic     = TIBEMS_TRUE;
tibemsAcknowledgeMode           ackMode      = TIBEMS_AUTO_ACKNOWLEDGE;

/*-----------------------------------------------------------------------
 * Variables
 *----------------------------------------------------------------------*/
tibemsConnectionFactory         factory      = NULL;
tibemsConnection                connection   = NULL;
tibemsSession                   session      = NULL;
tibemsMsgConsumer               msgConsumer  = NULL;
tibemsAdmin						admin 	     = (tibemsAdmin)NULL;
tibemsMsgProducer               msgProducer  = NULL;

T_Dest               *tg_destination=NULL;
T_CBEMS		*tg_cb = NULL;
int			vm_nb_cb=0;
int			vm_nb_dg_surv = 0;
XDY_Datagroup		*tg_dg_surv=NULL;
int					*tg_dg_surv_presence=NULL;
int			vm_nb_ConnectionErrors=0;

tibemsSSLParams                 sslParams    = NULL;
tibems_int                      receive      = 1;

tibemsErrorContext              errorContext = NULL;

/* 
*  FONCTION : IsMessageNotNULL(T_IPC_MSG va_msg)
*  Etat     : 
*/
/* 
*  FONCTION 	: isMessageNotNULL(T_IPC_MSG va_msg)
*  OBJECTIF     : Retourne un booleen pour savoir si un message est non NULL
*/
int isMessageNotNULL(T_IPC_MSG va_msg)
{
	return (va_msg != NULL);
}


void onMessage(
    tibemsMsgConsumer           consumer,
    tibemsMsg                   msg,
    void*                       closure);
const char *tibems_errorString(tibemsErrorContext          errContext);

void TipcSrvSubjectGetSubscribeDestination(char *va_topic, T_Dest *va_dest)
{
int status;
int vl_indice;
char name[255];
T_Dest *vl_dest=tg_destination;
tibems_int vl_size;

	va_dest=NULL;
	while (vl_dest!=NULL)
	{
		vl_size=sizeof(name);
		status = tibemsDestination_GetName(vl_dest->dest,name,vl_size);
	 	if (status != TIBEMS_OK)
    		{
 			XZST_03EcritureTrace(XZSTC_WARNING,"erreur destination name  %s(%s)",tibems_errorString (errorContext), name);
			va_dest=NULL;
			return ;
  		}
XZST_03EcritureTrace(XZSTC_INFO, "TipcSrvSubjectGetSubscribeDestination va_topic [%s] name [%s]",va_topic, name);
		if (!strcmp(va_topic,name)) {
			XZST_03EcritureTrace(XZSTC_INFO, "TipcSrvSubjectGetSubscribeDestination trouve '%s'", name);
			va_dest=vl_dest;
			return;
		}
		vl_dest=vl_dest->suivant;
	}
}

void TipcSrvSubjectGetSubscribe(char *va_topic, int *va_etat)
{
int status;
int vl_indice;
char name[255];
T_Dest *vl_dest=tg_destination;
tibems_int vl_size;

	/* cas des desabonnements avec topic=NULL */
	if ( va_topic==NULL )
		return;

	*va_etat=FALSE;
	while (vl_dest!=NULL)
	{
		vl_size=sizeof(name);
		status = tibemsDestination_GetName(vl_dest->dest,name,vl_size);
	 	if (status != TIBEMS_OK)
    		{
 			XZST_03EcritureTrace(XZSTC_WARNING,"erreur destination name  %s(%s)",tibems_errorString (errorContext), name);
			*va_etat=FALSE;
			return ;
  		}
		if (!strcmp(va_topic,name)) {
			XZST_03EcritureTrace(XZSTC_INFO, "TipcSrvSubjectGetSubscribe trouve '%s'", name);
			*va_etat=TRUE;
			return;
		}
		vl_dest=vl_dest->suivant;
	}
}


int XDG_QuitterDG(char *topic)
{
    int status;
	int vl_indice;
	char name[100];
	T_Dest *vl_dest=tg_destination;
	T_Dest *vl_dest_clean=tg_destination;
	tibemsDestination *vl_current_destination;
	tibemsMsgConsumer *vl_current_consumer;

	while (vl_dest!=NULL)
	{
		status = tibemsDestination_GetName(tg_destination->dest, name,100);
  		if (status != TIBEMS_OK)
    		{
 			XZST_03EcritureTrace(XZSTC_WARNING,"erreur destintion name  %s", 
					tibems_errorString (errorContext));
			return (TRUE) ;
  		 }
		if (!strcmp(topic,name)) {
			XZST_03EcritureTrace(XZSTC_WARNING, "XDG_QuitterDG trouve %s - fermeture consumer et destination associes",name);
			vl_current_destination=vl_dest->dest;
			vl_current_consumer=vl_dest->consumer;
			tibemsMsgConsumer_Close(vl_current_consumer);
			free(vl_current_consumer);
			tibemsDestination_Destroy(vl_current_destination);
			/* re-parcours de la liste chainee pour supprimer l'element */
			if (vl_dest==tg_destination)
			{
				/* On supprime le premier element de la liste */
				tg_destination = vl_dest->suivant;
				free(vl_dest);
			}
			else
			{
				/* recherche du predecesseur de la destination a supprimer */
				vl_dest_clean=tg_destination;
				while (vl_dest_clean->suivant!=vl_dest && vl_dest_clean->suivant!=NULL)
					vl_dest_clean=vl_dest_clean->suivant;
				/* on a trouve le predecesseur de la daestination a supprimer */
				if (vl_dest_clean->suivant!=NULL && vl_dest_clean->suivant==vl_dest)
				{
					/* on met à jour son successeur */
					vl_dest_clean->suivant=vl_dest->suivant;
					free(vl_dest);
				}
			}
			return (FALSE);
		}
		vl_dest=vl_dest->suivant;
	}
	return (TRUE);
}



int XDG_JoindreDG(char * va_topic) 
{
int status;
T_Dest *vl_dest;
tibemsDestination destination;
tibemsMsgConsumer  *myConsumer;
int vl_indice;
int etat;
char name[50];

	XZST_03EcritureTrace(XZSTC_WARNING,"XDG_JoindreDG %s",va_topic);
printf("XDG_JoindreDG IN [%s]\n"),va_topic;
	TipcSrvSubjectGetSubscribe(va_topic,&etat);
	if (etat)
		return (TRUE);

	vl_dest=(T_Dest *)malloc(sizeof(struct S_Dest));

	if (vl_dest==NULL) 
	{
 		XZST_03EcritureTrace(XZSTC_WARNING,"erreur malloc tg_destination");
 		//printf("erreur malloc tg_destination\n");
		return (FALSE);
	}

	status = tibemsTopic_Create(&destination,va_topic);
	if (status != TIBEMS_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"erreur create destination [%s] %s",va_topic, tibems_errorString (errorContext));
		//printf("erreur create destination %s\n",tibems_errorString (errorContext));
		free(vl_dest);
		return (FALSE);
	}	
	//printf("tibemsTopic_Create successful destination\n ");	
	vl_dest->dest=destination;
	vl_dest->suivant = tg_destination;
	tg_destination = vl_dest;
	myConsumer = malloc(sizeof(tibemsMsgConsumer));
	vl_dest->consumer=myConsumer;

	status = tibemsSession_CreateConsumer(session,myConsumer,destination,NULL,TIBEMS_FALSE);
	if (status != TIBEMS_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"erreur create consumer  %s", tibems_errorString (errorContext));
		//printf("�erreur create consumer  %s\n", tibems_errorString (errorContext));
		return (FALSE);
	}

	/* set the message listener */
	status = tibemsMsgConsumer_SetMsgListener(*myConsumer, onMessage, NULL);
	if (status != TIBEMS_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"erreur set listener %s",tibems_errorString (errorContext));
	 	//printf("erreur set listener %s\n",tibems_errorString (errorContext));
		
		return (FALSE);
	}
	return (TRUE);
}

T_BOOL  TipcMonSubjectSubscribeGetWatch(char *name, T_BOOL *etat)
{
int vl_indice;
	/*recherche si dg deja surveille*/
	for (vl_indice=0; vl_indice<vm_nb_dg_surv;vl_indice++) {
		if (!strcmp(tg_dg_surv[vl_indice], name)) {
			XZST_03EcritureTrace(XZSTC_INFO,"deja surveille %s %s",name,tg_dg_surv[vl_indice]);
			*etat=TRUE;
			return (TRUE);
		}
	}
	*etat = FALSE;
	return (TRUE);
}

void XDG_tibemsCollection_Destroy(tibemsCollection va_collection)
{
	tibemsCollection_Destroy(va_collection);
}

tibemsCollection XZEMS_03IsTopic(char *name)
{
tibemsCollection 	topicInfoCollection=NULL;
tibems_status   	status;
tibemsTopicInfo 	topicInfo;
char 			topic[100];
tibems_int		count;
tibems_int		countDurable;

	XZST_03EcritureTrace(XZSTC_FONCTION,"XZEMS_03IsTopic IN : [%s]",name);

	status = tibemsAdmin_GetTopics(admin, &topicInfoCollection, name, TIBEMS_DEST_GET_ALL);
	if (status == TIBEMS_NOT_FOUND) 
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZEMS_03IsTopic : Methode tibemsAdmin_GetTopics : Topic inexistant");
		XDG_tibemsCollection_Destroy(topicInfoCollection);
		return ((tibemsCollection)NULL);
	}

	if (status != TIBEMS_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"erreur grave tibemsAdmin_GetTopics %d %s",
			status,
			tibems_errorString (errorContext));
		XDG_tibemsCollection_Destroy(topicInfoCollection);
		return ((tibemsCollection)NULL);
	}
	
	if (topicInfoCollection==NULL)
	{												
		XZST_03EcritureTrace(XZSTC_WARNING,"XZEMS_03IsTopic : Methode tibemsAdmin_GetTopics : Topic inexistant");
		XDG_tibemsCollection_Destroy(topicInfoCollection);
		return ((tibemsCollection)NULL);
	}

	status = tibemsCollection_GetCount(topicInfoCollection, &count);
	if (status != TIBEMS_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"erreur grave tibemsCollection_GetCount %d %s",
			status,
			tibems_errorString (errorContext));
		XDG_tibemsCollection_Destroy(topicInfoCollection);
		return ((tibemsCollection)NULL);
	}

	if (count == 0)
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"XZEMS_03IsTopic : Methode tibemsCollection_GetCount sur topicInfoCollection : Topic inexistant");
		XDG_tibemsCollection_Destroy(topicInfoCollection);
		return ((tibemsCollection)NULL);
	}
																										

	status = tibemsCollection_GetFirst(topicInfoCollection, &topicInfo);
	if (status != TIBEMS_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"erreur grave tibemsCollection_GetFirst %d %s",
			status,
			tibems_errorString (errorContext));
		XDG_tibemsCollection_Destroy(topicInfoCollection);
		return ((tibemsCollection)NULL);
	}

	status = tibemsTopicInfo_GetSubscriptionCount( topicInfo, &count);
	if (status != TIBEMS_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"erreur grave tibemsTopicInfo_GetSubscriptionCount %d %s",
			status,
			tibems_errorString (errorContext));
		XDG_tibemsCollection_Destroy(topicInfoCollection);
		return ((tibemsCollection)NULL);
	}

	status = tibemsTopicInfo_GetDurableSubscriptionCount( topicInfo, &countDurable);
	if (status != TIBEMS_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"erreur grave tibemsTopicInfo_GetDurableSubscriptionCount %d %s",
			status,
			tibems_errorString (errorContext));
		XDG_tibemsCollection_Destroy(topicInfoCollection);
		return ((tibemsCollection)NULL);
	}

	if ( count-countDurable > 0 )
	{
		XZST_03EcritureTrace(XZSTC_FONCTION,"XZEMS_03IsTopic - OUT : retourne %d abonne(s)", count-countDurable );
		return (topicInfoCollection);
	}
	else
	{												
		XZST_03EcritureTrace(XZSTC_FONCTION,"XZEMS_03IsTopic - OUT : retourne %d abonne donc pas d'abonne", count-countDurable );
		XDG_tibemsCollection_Destroy(topicInfoCollection);
		return ((tibemsCollection)NULL);
	}

}

void TutCbDestroy(void (*CB)())
{
	int vl_indice;

//XZST_03EcritureTrace(XZSTC_INFO,"TutCbDestroy : IN");
	for (vl_indice=0; vl_indice<vm_nb_cb;vl_indice++) {
		if (tg_cb[vl_indice].CB == CB) {
			XZST_03EcritureTrace(XZSTC_INFO,"cb trouve");
			free(tg_cb[vl_indice].CB);
			tg_cb[vl_indice].CB=NULL;
//XZST_03EcritureTrace(XZSTC_INFO,"TutCbDestroy : OUT");
			return ;
		}
/*		else
XZST_03EcritureTrace(XZSTC_INFO,"TutCbDestroy : vl_indice=%d vm_nb_cb=%d",vl_indice, vm_nb_cb);
*/
//XZST_03EcritureTrace(XZSTC_INFO,"TutCbDestroy : OUT");

	}
}

// int TipcSrvProcessCbCreate(int va_msg, void (*CB)(T_IPC_CONN c,T_IPC_CONN_PROCESS_CB_DATA d ,T_CB_ARG a), void *CB_ARG)
int TipcSrvProcessCbCreate(int va_msg, void (*CB)(T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG ), void *CB_ARG)
{
	int vl_indice;
	int vl_index;

	XZST_03EcritureTrace(XZSTC_INTERFACE,"TipcSrvProcessCbCreate : %d",va_msg);
	/*recherche si cb deja cree*/
	for (vl_indice=0; vl_indice<vm_nb_cb;vl_indice++) 
	{
		if (tg_cb[vl_indice].Numero == va_msg) 
		{
			XZST_03EcritureTrace(XZSTC_INFO,"cb deja existant sur %d",va_msg);
			return (TRUE);
		}
	}

	/*ajout du CB dans le tableau*/
	if (vm_nb_cb==0) 
	{
		tg_cb = (T_CBEMS *)malloc(sizeof(T_CBEMS));
		if (tg_cb==NULL) 
		{
			//printf("erreur malloc tg_CB");
 			XZST_03EcritureTrace(XZSTC_WARNING,"erreur malloc tg_CB");
			return (FALSE);
		}		
		vm_nb_cb ++;
	}
	else 
	{
		tg_cb = (T_CBEMS *)realloc(tg_cb,((size_t) vm_nb_cb +1)*sizeof(T_CBEMS));
		if (tg_cb==NULL) {
 			XZST_03EcritureTrace(XZSTC_WARNING,"erreur realloc tg_CB");
			return (FALSE);
		}
		vm_nb_cb ++;
	}	
	tg_cb[vm_nb_cb-1].Numero = va_msg;
	tg_cb[vm_nb_cb-1].CB = CB;
	tg_cb[vm_nb_cb-1].CB_ARG = CB_ARG;

	return (T_TRUE);
}

int TipcMsgNextInt4(tibemsMsg va_msg,T_INT4 *pd)
{ 
tibems_int tmp;
	tibemsStreamMsg_ReadInt(va_msg, &tmp);
	*pd = tmp;
	return (T_TRUE);
}
int TipcMsgNextStr(tibemsMsg va_msg,T_STR *str)
{
	tibemsStreamMsg_ReadString(va_msg, str);
	return (T_TRUE);
}

int XZEMS_03DestroyMsg(tibemsMsg va_msg)
{
	if (va_msg!=NULL)
{

		TipcMsgDestroy(va_msg); 
}
	
	va_msg=NULL;
	return (T_TRUE);
}

int TipcMsgRead(tibemsMsg va_msg, ...)
{
va_list vl_listeArgument;
int i=0;
int indice;
int vl_type;
short *tab;
int *tab4;
double *tabf;
tibems_int nb;
tibems_int taille;
tibems_int *nbparam;
char *vl_str=NULL;
char *tabstr;
char *str;
int vl_field_size;
short vl_field_type;
int vl_field_count;
tibemsData vl_field_data;
int vl_type_origine;
tibems_bool vl_TIBCO_IMPORTED = TIBEMS_FALSE;
tibems_bool vl_property_exists = TIBEMS_FALSE;
tibems_status               status  = TIBEMS_OK;
tibemsMsgField vl_field;

	//XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgRead - IN");

	status = tibemsMsg_PropertyExists(va_msg,"JMS_TIBCO_IMPORTED", &vl_property_exists);
  	if (status != TIBEMS_OK)
	{
 		XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgRead : property JMS_TIBCO_IMPORTED non trouvee - message ems");
		vl_property_exists = TIBEMS_FALSE;
 	}
	else
	{
//		if (vl_property_exists==TIBEMS_TRUE)
//			XZST_03EcritureTrace(XZSTC_INFO,"TipcMsgRead : Message importe par la gateway depuis SS");
//		else
//			XZST_03EcritureTrace(XZSTC_INFO,"TipcMsgRead : Message jms ems");
	}

	if (vl_property_exists==TIBEMS_TRUE)
		status = tibemsMsg_GetBooleanProperty(va_msg, "JMS_TIBCO_IMPORTED", &vl_TIBCO_IMPORTED);
	else
		vl_TIBCO_IMPORTED=TIBEMS_FALSE;
	
	va_start(vl_listeArgument, va_msg);

	while (TRUE)
	{
		vl_type = va_arg(vl_listeArgument,int);
//XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgRead Type %d",vl_type);
		if (vl_type==0)
			break;

		if (vl_type==T_IPC_FT_INT4 ) 
		{
//XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgRead INT4");
			tibems_int *pd = va_arg(vl_listeArgument, int *);
			if (pd==NULL) 
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"erreur recuperation argument INT4");
				return (FALSE);
			}
			tibemsStreamMsg_ReadInt(va_msg, pd);
XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgRead ReadInt %d",*pd);
		}
		else if (vl_type==T_IPC_FT_INT2) 
		{
//XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgRead INT2");
			tibems_short *psi= va_arg(vl_listeArgument, short *);
			if (psi==NULL)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"erreur recuperation argument INT2");
				return (FALSE);
			}			
			tibemsStreamMsg_ReadShort(va_msg, psi);
XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgRead ReadShort %d",*psi);
		}		
		else if (vl_type==T_IPC_FT_INT2_ARRAY) 
		{

			if (vl_TIBCO_IMPORTED==TIBEMS_TRUE)
			{
				tibemsStreamMsg_ReadField(va_msg, &vl_field);
				vl_field_size=vl_field.size;
				vl_field_type=vl_field.type;
				vl_field_count=vl_field.count;
				vl_field_data=vl_field.data;
//XZST_03EcritureTrace(XZSTC_WARNING,"INT2_ARRAY : size %d ",vl_field_size);
//XZST_03EcritureTrace(XZSTC_WARNING,"INT2_ARRAY : count %d",vl_field_count);

				short *ps2;
				ps2=va_arg(vl_listeArgument, short *);
				memcpy(ps2,vl_field_data.arrayValue,vl_field_count*vl_field_size);
				nbparam= va_arg(vl_listeArgument, int *);
				*nbparam=vl_field_count;
								
				tibemsStreamMsg_FreeField( &vl_field );
			}
			else
			{
//XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgRead ReadShortArray ems ");
				tibems_short *ps2= va_arg(vl_listeArgument, short *);
				tibemsStreamMsg_ReadInt(va_msg,&nb);
//				tab= (short *)malloc((sizeof(short)*nb));
//				*ps2=tab;
				for (indice=0;indice<nb;indice++) 
				{
//					tibemsStreamMsg_ReadShort(va_msg,&tab[indice]);
//					*(ps2+indice)=tab[indice];
					tibemsStreamMsg_ReadShort(va_msg,ps2+indice);
				}		
				nbparam= va_arg(vl_listeArgument, int *);
				*nbparam=nb;
			}
		}		
		else if (vl_type==T_IPC_FT_STR_ARRAY) 
		{
		
//XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgRead ReadStrArray ems ");
				char **S;
				S = va_arg(vl_listeArgument, char **);
				tibemsStreamMsg_ReadInt(va_msg,&nb);
				tibemsStreamMsg_ReadInt(va_msg,&taille);
//XZST_03EcritureTrace(XZSTC_WARNING,"STR_ARRAY : nb %d ",nb);
//XZST_03EcritureTrace(XZSTC_WARNING,"STR_ARRAY : taille %d ",taille);
				nbparam= va_arg(vl_listeArgument, int *);
				*nbparam=nb;
				/* Initialisation pointeur sur adresse variable passee en parametre */
				char *ptr;
				ptr=&(S[0]);
				for (indice=0;indice<nb;indice++) 
				{
					tibemsStreamMsg_ReadString(va_msg, &str);
//XZST_03EcritureTrace(XZSTC_WARNING,"STR_ARRAY :ptr=(%x)",ptr);
					/* On met 1 octet de plus pour copier le \0 */
					memcpy(ptr,str,strlen(str)+1);
//XZST_03EcritureTrace(XZSTC_WARNING,"STR_ARRAY : str lue  [%s] ",(char*)ptr);
					ptr+=taille;
				}
			
		}		
		else if (vl_type==T_IPC_FT_INT4_ARRAY) 
		{
//XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgRead ReadIntArray ems ");
				tibems_int *ps4= va_arg(vl_listeArgument, int *);
				tibemsStreamMsg_ReadInt(va_msg,&nb);
//				tab4= (int *)malloc((sizeof(int)*nb));
//				*ps4=tab4;
				for (indice=0;indice<nb;indice++) 
				{
//					tibemsStreamMsg_ReadInt(va_msg,&tab4[indice]);
//					*(ps4+indice)=tab4[indice];
					tibemsStreamMsg_ReadInt(va_msg,ps4+indice);
				}		
				nbparam= va_arg(vl_listeArgument, int *);
				*nbparam=nb;
			
		}		
		else if (vl_type==T_IPC_FT_REAL8_ARRAY) 
		{
			if (vl_TIBCO_IMPORTED==TIBEMS_TRUE)
			{
//XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgRead ReadDoubleArray ss ");
				tibemsStreamMsg_ReadField(va_msg, &vl_field);
				vl_field_size=vl_field.size;
				vl_field_type=vl_field.type;
				vl_field_count=vl_field.count;
				vl_field_data=vl_field.data;
//XZST_03EcritureTrace(XZSTC_WARNING,"REAL8_ARRAY : type %d ",vl_field_type);
//XZST_03EcritureTrace(XZSTC_WARNING,"REAL8_ARRAY : size %d ",vl_field_size);
//XZST_03EcritureTrace(XZSTC_WARNING,"REAL8_ARRAY : count %d",vl_field_count);

				double *pf4;
				pf4=va_arg(vl_listeArgument, double *);
				tabf= (double *)malloc((sizeof(tibems_double)*vl_field_count));
//XZST_03EcritureTrace(XZSTC_WARNING,"REAL8_ARRAY : tabf %x",tabf);
//XZST_03EcritureTrace(XZSTC_WARNING,"REAL8_ARRAY : *pf4 %x",*pf4);
				*pf4=(int)tabf;
//XZST_03EcritureTrace(XZSTC_WARNING,"REAL8_ARRAY : *pf4 %x",*pf4);

				memcpy(tabf,vl_field_data.arrayValue,vl_field_count*vl_field_size);
				nbparam= va_arg(vl_listeArgument, int *);
				*nbparam=vl_field_count;

				tibemsStreamMsg_FreeField( &vl_field );
			}
			else
			{
//XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgRead ReadDoublerray ems ");
				tibems_double *pf4= va_arg(vl_listeArgument, double  *);
				tibemsStreamMsg_ReadInt(va_msg,&nb);
//				tabf= (double *)malloc((sizeof(double)*nb));
//				*pf4=(int)tabf;
				for (indice=0;indice<nb;indice++) 
				{
//					tibemsStreamMsg_ReadDouble(va_msg,&tabf[indice]);
//					*(pf4+indice)=(float)tabf[indice];
					tibemsStreamMsg_ReadDouble(va_msg,pf4+indice);
				}		
				nbparam= va_arg(vl_listeArgument, int *);
				*nbparam=nb;
			}
		}
		else if (vl_type ==T_IPC_FT_STR) 
		{
			vl_str = va_arg(vl_listeArgument, char *);
			if (vl_str==NULL)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"erreur recuperation argument STR");
				return (FALSE);
			}			
			tibemsStreamMsg_ReadString(va_msg, &str);
			memcpy(vl_str,str,strlen(str)+1);
XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgRead ReadStr [%s] ",vl_str);
		}
		else if (vl_type==T_IPC_FT_REAL8) 
		{
			tibems_double *pf = va_arg(vl_listeArgument, double *);
			if (pf==NULL)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "erreur recuperation argument REAL8");
				return (FALSE);
			}			
			tibemsStreamMsg_ReadDouble(va_msg, pf);
XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgRead ReadIDouble %f ",*pf);
		}
		else if (vl_type==T_IPC_FT_CHAR) 
		{
			unsigned char *pc = va_arg(vl_listeArgument, unsigned char *);
			tibems_wchar mychar;
			if (pc==NULL)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"erreur recuperation argument CHAR");
				return (FALSE);
			}			
			tibemsStreamMsg_ReadChar(va_msg, &mychar);
			*pc=mychar;
XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgRead ReadChar Asc(%d) ",*pc);
		}	
	}
	va_end(vl_listeArgument);
//	XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgRead - OUT");
//	XZEMS_03DestroyMsg(va_msg);
	return (TRUE);
}

int TipcMsgGetType(tibemsMsg msg, int *type) 
{
tibems_status               status  = TIBEMS_OK;
const char *vl_type;

	status = tibemsMsg_GetType(msg, &vl_type);
	if (status != TIBEMS_OK)
	{
 		XZST_03EcritureTrace(XZSTC_WARNING,"pb recuperation type message %s",tibems_errorString (errorContext));
		return (FALSE);
 	}
	if (sscanf(vl_type,"%d",type) !=1) {
 		XZST_03EcritureTrace(XZSTC_WARNING,"pb recuperation type message depuis %s",vl_type);
		return (FALSE);
	}
	return (TRUE);
}

void onMessage(
    tibemsMsgConsumer           consumer,
    tibemsMsg                   msg,
    void*                       closure)
{
tibems_status               status  = TIBEMS_OK;
const char *vl_type;
int vl_numero;
int vl_index;
int vl_trouve;
T_IPC_CONN_PROCESS_CB_DATA data=NULL;
tibemsDestination vl_destination;
char vl_name[250];
tibems_bool vl_property_exists = TIBEMS_FALSE;
tibems_bool vl_TIBCO_IMPORTED = TIBEMS_FALSE;
char vl_str[10];
tibems_int vl_size;

	//XZST_03EcritureTrace(XZSTC_INFO"IN : onMessage");
	//sem_wait(&vg_semaphore); 
	/* En mode DEBUG. Permet d'afficher le message brut
	tibemsMsg_Print( msg );
	*/

	status = tibemsMsg_PropertyExists(msg,"JMS_TIBCO_IMPORTED", &vl_property_exists);
  	if (status != TIBEMS_OK)
	{
 		XZST_03EcritureTrace(XZSTC_WARNING,"property JMS_TIBCO_IMPORTED non trouvee - message ems");
		vl_property_exists = TIBEMS_FALSE;
 	}
	else
	{
//		if (vl_property_exists==TIBEMS_TRUE)
//			XZST_03EcritureTrace(XZSTC_INFO,"onMessage : Message importe par la gateway depuis SS");
//		else
//			XZST_03EcritureTrace(XZSTC_INFO,"onMessage : Message jms ems");
	}

	if (vl_property_exists==TIBEMS_TRUE)
		status = tibemsMsg_GetBooleanProperty(msg, "JMS_TIBCO_IMPORTED", &vl_TIBCO_IMPORTED);
	else
		vl_TIBCO_IMPORTED=TIBEMS_FALSE;

	if ( vl_TIBCO_IMPORTED == TIBEMS_TRUE )
	{
		status = tibemsMsg_PropertyExists(msg,"JMS_TIBCO_SS_TYPE_NUM", &vl_property_exists);
	  	if (status != TIBEMS_OK)
		{
	 		XZST_03EcritureTrace(XZSTC_WARNING,"pb recuperation existence numero SS message %s",tibems_errorString (errorContext));
			XZEMS_03DestroyMsg(msg);
//			sem_post(&vg_semaphore); 
			return ;
	 	}

		vl_numero=-1;

		if ( vl_property_exists == TIBEMS_TRUE )
		{
			status = tibemsMsg_GetIntProperty(msg, "JMS_TIBCO_SS_TYPE_NUM", &vl_numero);
		  	if (status != TIBEMS_OK)
			{
		 		XZST_03EcritureTrace(XZSTC_WARNING,"pb recuperation numero SS message %s",tibems_errorString (errorContext));
				XZEMS_03DestroyMsg(msg);
//				sem_post(&vg_semaphore); 
				return ;
		 	}
			else
			{
//				XZST_03EcritureTrace(XZSTC_WARNING,"onMessage : numero SS recupere : %d",vl_numero);
			}
		}

		if ( vl_numero>0 )
		{
			/* retypage du message */
			sprintf( vl_str, "%d", vl_numero );
			status = tibemsMsg_SetType(msg,(const char *)vl_str);
		  	if (status != TIBEMS_OK)
			{
		 		XZST_03EcritureTrace(XZSTC_WARNING,"onMessage OUT : pb retypage  message %s",tibems_errorString (errorContext));
				XZEMS_03DestroyMsg(msg);
//				sem_post(&vg_semaphore); 
				return ;
		 	}
			else
			{
//				XZST_03EcritureTrace(XZSTC_WARNING,"onMessage : message retype en %d",vl_numero);
			}
		}
	}

	status = tibemsMsg_GetDestination(msg, &vl_destination);
  	if (status != TIBEMS_OK)
	{
 		XZST_03EcritureTrace(XZSTC_WARNING,"onMessage OUT : pb recuperation destination message %s",tibems_errorString (errorContext));
		XZEMS_03DestroyMsg(msg);
//		sem_post(&vg_semaphore); 
		return ;
 	}
	
	
	vl_size=sizeof(vl_name);
	status = tibemsDestination_GetName(vl_destination, vl_name, vl_size);
  	if (status != TIBEMS_OK)
	{
 		XZST_03EcritureTrace(XZSTC_WARNING,"onMessage OUT : pb recuperation nom destination message %s",tibems_errorString (errorContext));
		XZEMS_03DestroyMsg(msg);
//		sem_post(&vg_semaphore); 
		return ;
 	}
	else
	{
//		XZST_03EcritureTrace(XZSTC_WARNING,"onMessage : nom destination recupere : %s",vl_name);
	}

	status = tibemsMsg_GetType(msg, &vl_type);
  	if (status != TIBEMS_OK)
	{
 		XZST_03EcritureTrace(XZSTC_WARNING,"onMessage OUT : pb recuperation type message %s",tibems_errorString (errorContext));
		XZEMS_03DestroyMsg(msg);
//		sem_post(&vg_semaphore); 
		return ;
 	}
	else
	{
//		XZST_03EcritureTrace(XZSTC_WARNING,"onMessage : type recupere : %s",vl_type);
	}
	
	if (vl_type!=NULL)
	{
		if (sscanf(vl_type,"%d",&vl_numero) !=1) {
 			XZST_03EcritureTrace(XZSTC_WARNING,"onMessage OUT : pb recuperation type message depuis %s",vl_type);
			XZEMS_03DestroyMsg(msg);
//			sem_post(&vg_semaphore); 
			return ;
		}
	}
	else
		vl_numero=0;

	/*recherche du CB*/
	vl_trouve=0;
	for (vl_index=0; vl_index < vm_nb_cb ; vl_index ++) 
	{
		if (tg_cb[vl_index].Numero == vl_numero) 
		{
			vl_trouve=1;
			break;
		}
	}
	if (!vl_trouve) 
	{
//	 		XZST_03EcritureTrace(XZSTC_WARNING,"onMessage OUT : pas de CB pour message %d",vl_numero);
			/* destroy the message */
			XZEMS_03DestroyMsg(msg);
//			sem_post(&vg_semaphore); 
			return ;	
	}
	
//	XZST_03EcritureTrace(XZSTC_WARNING,"appel du CB tg_cb[%d].Numero [%d]", vl_index, tg_cb[vl_index].Numero);
	/*appel du CB*/
	data=(T_IPC_CONN_PROCESS_CB_DATA) malloc(sizeof(T_IPC_CONN_PROCESS_CB_DATA_STRUCT));
	
	if (data==NULL) 
	{
 		XZST_03EcritureTrace(XZSTC_WARNING,"onMessage OUT : pb malloc data");
		XZEMS_03DestroyMsg(msg);
//		sem_post(&vg_semaphore); 
		return ;
	}
//	sem_post(&vg_semaphore); 
	data->msg = msg;
//	(*(tg_cb[vl_index].CB))((T_IPC_CONN)NULL, data, (T_CB_ARG)NULL);
	(*(tg_cb[vl_index].CB))((T_IPC_CONN)NULL, data, (T_CB_ARG)tg_cb[vl_index].CB_ARG);

	/* destroy the message */
	XZEMS_03DestroyMsg(msg);
	free(data);

    // reinitialise ne nombre d'erreurs de connexion a 0
    vm_nb_ConnectionErrors = 0;
//	XZST_03EcritureTrace(XZSTC_WARNING,"onMessage OUT");
}


const char *tibems_errorString(tibemsErrorContext          errContext)
{
tibems_status               status = TIBEMS_OK;
char                 str[255];

	status = tibemsErrorContext_GetLastErrorString(errContext, (const char **)&str);
	return (str);
}

int TipcSrvFlush() 
{
	return (TRUE);
}

int TutErrNumGet()
{
	return (0);
}

double TutGetWallTime()
{
	time_t montime;
	double h;
	struct tm *t;
	
	time(&montime);
	t=localtime(&montime);
	 const char * strDate = asctime( localtime( & montime ) );
	h = (double)montime;
        return (h);
}

double TutGetCurrentTime()
{
        return (TutGetWallTime());
}

int TipcMtLookupByNum(int num)
{
        return (num);
}

T_IPC_MSG TipcSrvMsgNext(T_REAL8 timeout) 
{
	tibemsMsg msg=NULL;
	T_IPC_MSG ipc;

	if (timeout == T_TIMEOUT_FOREVER) 
//		tibems_Sleep(1000);
		tibems_Sleep(TIMEOUT_MAIN_LOOP_MS);
	else
	{
		tibems_Sleep(timeout);
//		tibemsMsg_Create(&msg);
	}
	return (msg);

	
}

void TipcSrvMsgProcess(T_IPC_MSG msg)
{
        return ;
}

void TipcMsgDestroy(T_IPC_MSG msg)
{
	if (msg!=NULL)
	{
		if (tibemsMsg_Destroy(msg) != TIBEMS_OK)
			XZST_03EcritureTrace(XZSTC_WARNING,"erreur destroy message %s",tibems_errorString (errorContext));
//		else
//		  	 XZST_03EcritureTrace(XZSTC_FONCTION,"TipcMsgDestroy : message destroyed");
	}
	msg=NULL;
        return ;
}

void TutCommandParseFile(char *file)
{
        return ;
}

int TipcSrvDestroy(int param)
{
        return (T_TRUE);
}

int TutCommandParseStr(char *va_str)
{
	return (XDC_OK);
}

void TipcMonSubjectsWatch(XDY_FonctionInt va_FonctionUtilisateur_in)
{
	int 				vl_indice;
	tibemsCollection	topicInfoCollection=NULL;

   	/*A
        **  test les parametres d'entree
        */
	XZST_03EcritureTrace(XZSTC_FONCTION,"TipcMonSubjectsWatch : IN");

	if ( va_FonctionUtilisateur_in == NULL)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"TipcMonSubjectsWatch : va_FonctionUtilisateur_in est nulle");
		return (XDC_NOK);
	}

	/*A on parcourt le tableau des topics a monitorer */
	for (vl_indice=0;vl_indice<vm_nb_dg_surv;vl_indice++)
	{
		topicInfoCollection=XZEMS_03IsTopic(tg_dg_surv[vl_indice]);
		if (topicInfoCollection==NULL)
		{
			XZST_03EcritureTrace(XZSTC_DEBUG1,"TipcMonSubjectsWatch : Watch [%s] - Pas d'abonne",tg_dg_surv[vl_indice]);
			if (tg_dg_surv_presence[vl_indice]==1)
					/* Appel de sp_status(vl_Dg,0) */
					(*va_FonctionUtilisateur_in)(tg_dg_surv[vl_indice],0);
			tg_dg_surv_presence[vl_indice]=0;
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_DEBUG1,"TipcMonSubjectsWatch : Watch [%s] - abonne(s) present(s)",tg_dg_surv[vl_indice]);
			if (tg_dg_surv_presence[vl_indice]==0)
				/* Appel de sp_status(vl_Dg,1) */
				(*va_FonctionUtilisateur_in)(tg_dg_surv[vl_indice],1);
			tg_dg_surv_presence[vl_indice]=1;
			XDG_tibemsCollection_Destroy(topicInfoCollection);
		}
	}
	XZST_03EcritureTrace(XZSTC_FONCTION,"TipcMonSubjectsWatch : OUT");
}

int TipcMonSubjectSubscribeSetWatch(char *name, T_BOOL va_param)
{
int vl_indice;
	XZST_03EcritureTrace(XZSTC_INFO,"TipcMonSubjectSubscribeSetWatch name %s",name);
	/*recherche si cb deja cree*/
	for (vl_indice=0; vl_indice<vm_nb_dg_surv;vl_indice++) {
		if (!strcmp(tg_dg_surv[vl_indice], name)) {
			XZST_03EcritureTrace(XZSTC_INFO,"TipcMonSubjectSubscribeSetWatch deja surveille %s",name);
			return (TRUE);
		}
	}

	/*ajout du CB dans le tableau*/
	if (vm_nb_dg_surv==0) {
		tg_dg_surv = (XDY_Datagroup *)malloc(sizeof(XDY_Datagroup));
		if (tg_dg_surv==NULL) 
		{
 			XZST_03EcritureTrace(XZSTC_WARNING,"erreur malloc tg_CB");
			return (FALSE);
		}	
		tg_dg_surv_presence = (int *)malloc(sizeof(int));
		if (tg_dg_surv_presence==NULL) 
		{
 			XZST_03EcritureTrace(XZSTC_WARNING,"erreur malloc tg_CB presence");
			return (FALSE);
		}
		/* Initialisation tableau presence du topic a absent */
		tg_dg_surv_presence[vm_nb_dg_surv] = 0;
		vm_nb_dg_surv ++;
	}
	else 
	{
		tg_dg_surv = (XDY_Datagroup *)realloc(tg_dg_surv,((size_t) vm_nb_dg_surv +1)*sizeof(XDY_Datagroup));
		if (tg_dg_surv==NULL) 
		{
 			XZST_03EcritureTrace(XZSTC_WARNING,"erreur realloc tg_CB");
			return (FALSE);
		}
		tg_dg_surv_presence = (int *)realloc(tg_dg_surv_presence,((size_t) vm_nb_dg_surv +1)*sizeof(int));
		if (tg_dg_surv_presence==NULL) 
		{
 			XZST_03EcritureTrace(XZSTC_WARNING,"erreur realloc tg_CB presence");
			return (FALSE);
		}
		/* Initialisation tableau presence du topic a absent */
		tg_dg_surv_presence[vm_nb_dg_surv] = 0;
		vm_nb_dg_surv ++;
	}	
	strcpy(tg_dg_surv[vm_nb_dg_surv-1],name);
XZST_03EcritureTrace(XZSTC_WARNING,"Ajout dans tg_dg_surv[%d] de [%s]",vm_nb_dg_surv-1,name);
    return (T_TRUE);
}

int TipcSrvSetAutoFlushSize(int size)
{
	return (XDC_OK);
}

int TipcSrvDefaultCbCreate(T_IPC_CONN_PROCESS_CB_FUNC func, T_CB_ARG arg)
{
	return (XDC_OK);
}

int TipcMtCreate(char *va_nom, int va_numero, char*va_grammaire)
{
	return (XDC_OK);
}

int TipcSrvMsgWrite(char *va_topic, int va_num_msg, int flag, ...)
{
tibems_status		status			= TIBEMS_OK;
tibemsDestination	destination		= NULL;
T_Dest 				*vl_dest		= tg_destination;
tibemsStreamMsg		msg				= NULL;
va_list				vl_listeArgument;
int					i				= 0;
int					vl_type;
int					indice;
char				vl_str[11000];
tibems_int			taille;
char				vl_destination_name[255];
tibems_int          vl_size;
int					vl_destination_trouvee = 0;
int					vl_destination_cree = 0;
char				vl_trace_message[15000];

	if (!strcmp(va_topic,""))
		return(XDC_OK);

//	XZST_03EcritureTrace(XZSTC_WARNING,"TipcSrvMsgWrite IN : numero message %d sur topic [%s]",va_num_msg, va_topic);

	vl_size=sizeof(vl_destination_name);
	strcpy(vl_destination_name,"");
	strcpy(vl_trace_message,"");
	while ( (vl_dest!=NULL) && (!vl_destination_trouvee))
	{
		status = tibemsDestination_GetName( vl_dest->dest, vl_destination_name, vl_size);
		if (status != TIBEMS_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"TipcSrvMsgWrite : erreur recherche destination phase 1 %s", tibems_errorString (errorContext));
			return (XDC_NOK);
		}
//		XZST_03EcritureTrace(XZSTC_DEBUG1,"TipcSrvMsgWrite : BOUCLE : topic cherche : [%s] - destination trouvee : [%s]",va_topic,vl_destination_name);
		if (!strcmp(va_topic,vl_destination_name))
		{
			/* La destination est trouvee */
			vl_destination_trouvee = 1;
			destination = vl_dest->dest;
		}
		else
			vl_dest=vl_dest->suivant;
	}
	if (!vl_destination_trouvee)
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"TipcSrvMsgWrite : Creation destination temporaire [%s]", va_topic);
		status = tibemsTopic_Create(&destination,va_topic);
		if (status != TIBEMS_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"erreur creation topic %s", tibems_errorString (errorContext));
		    return (XDC_NOK);
		}
		vl_destination_cree = 1;
	}
	XZST_03EcritureTrace(XZSTC_DEBUG1,"TipcSrvMsgWrite : topic cherche : [%s] - destination trouvee : [%s]",va_topic,vl_destination_name);

	if (msgProducer==NULL)
	{
		status = tibemsSession_CreateProducer(session,&msgProducer,destination);
		if (status != TIBEMS_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "TipcSrvMsgWrite : erreur creation producer %d", tibems_errorString (errorContext));
			if (vl_destination_cree)
			{
				/* detruire la destination cree */
				tibemsDestination_Destroy( destination);
			}
	    	return (XDC_NOK);
		}
		XZST_03EcritureTrace(XZSTC_INFO,"TipcSrvMsgWrite : tibemsSession_CreateProducer succes");
	}

	status = tibemsStreamMsg_Create(&msg);
	if (status != TIBEMS_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"TipcSrvMsgWrite : erreur creation message %s", tibems_errorString (errorContext));
		if (vl_destination_cree)
		{
			/* detruire la destination cree */
			tibemsDestination_Destroy( destination);
		}
	    return (XDC_NOK);
	}
	sprintf(vl_str,"%d",va_num_msg);
	XZST_03EcritureTrace(XZSTC_DEBUG1, "TipcSrvMsgWrite : Num message %d",va_num_msg);
	status = tibemsMsg_SetType(msg,(const char *)vl_str);
    if (status != TIBEMS_OK)
    {
  		XZST_03EcritureTrace(XZSTC_WARNING,"TipcSrvMsgWrite : Erreur set type message %s", tibems_errorString (errorContext));
		/* destroy the message */
		XZEMS_03DestroyMsg(msg);
		if (vl_destination_cree)
		{
			/* detruire la destination cree */
			tibemsDestination_Destroy( destination);
		}
	    return (XDC_NOK);
	}
	
	/*ajout des champs*/
	va_start(vl_listeArgument, flag);
//XZST_03EcritureTrace(XZSTC_WARNING,"Avant generation message");

	while (TRUE)
	{
		vl_type = va_arg(vl_listeArgument,int);
		if (vl_type==0)
			break;
//			XZST_03EcritureTrace(XZSTC_DEBUG1,"TipcSrvMsgWrite : Type courant : %d",vl_type);

		if (vl_type==T_IPC_FT_INT4 ) 
		{
			tibems_int pd = va_arg(vl_listeArgument, int );
			tibemsStreamMsg_WriteInt(msg, pd);
//			XZST_03EcritureTrace(XZSTC_DEBUG1, "int %d",pd);
			sprintf(vl_trace_message,"%s {%d}", vl_trace_message, pd);
		}
		else if (vl_type==T_IPC_FT_INT2) 
		{
			tibems_short psi= va_arg(vl_listeArgument, int );
			tibemsStreamMsg_WriteShort(msg, psi);
//			XZST_03EcritureTrace(XZSTC_DEBUG1, "int2 %d",psi);
			sprintf(vl_trace_message,"%s {%d}", vl_trace_message, psi);
		}		
		else if (vl_type==T_IPC_FT_INT2_ARRAY) 
		{
//			XZST_03EcritureTrace(XZSTC_DEBUG1, "array int2");
			tibems_short *pst= va_arg(vl_listeArgument, short *);
			tibems_int nb = va_arg(vl_listeArgument, int );
//			XZST_03EcritureTrace(XZSTC_WARNING, "array nb %d // elt 0 %d %d",nb,pst[0], pst[1]);
			tibemsStreamMsg_WriteInt(msg,nb);
			sprintf(vl_trace_message,"%s [%d", vl_trace_message, nb);
			for (indice=0;indice<nb;indice++)
			{
				tibemsStreamMsg_WriteShort(msg,pst[indice]);
				sprintf(vl_trace_message,"%s {%d}", vl_trace_message, pst[indice]);
			}
			sprintf(vl_trace_message,"%s]", vl_trace_message);
		}	
		else if (vl_type==T_IPC_FT_INT4_ARRAY) 
		{
//			XZST_03EcritureTrace(XZSTC_DEBUG1, "array int4");
			tibems_int *psti= va_arg(vl_listeArgument, int *);
			tibems_int nbi = va_arg(vl_listeArgument, int );
//			XZST_03EcritureTrace(XZSTC_WARNING, "array nb %d // elt 0 %d %d",nbi,psti[0], psti[1]);
			tibemsStreamMsg_WriteInt(msg,nbi);
			sprintf(vl_trace_message,"%s [%d", vl_trace_message, nbi);
			for (indice=0;indice<nbi;indice++)
			{
				tibemsStreamMsg_WriteInt(msg,psti[indice]);
				sprintf(vl_trace_message,"%s {%d}", vl_trace_message, psti[indice]);
			}
			sprintf(vl_trace_message,"%s]", vl_trace_message);
		}		
		else if (vl_type==T_IPC_FT_REAL8) 
		{
			tibems_double pf = va_arg(vl_listeArgument, double );
			tibemsStreamMsg_WriteDouble(msg, pf);
//			XZST_03EcritureTrace(XZSTC_DEBUG1, "d %f",pf);
			sprintf(vl_trace_message,"%s {%f}", vl_trace_message, pf);
		}
		else if (vl_type==T_IPC_FT_REAL8_ARRAY) 
		{
			XZST_03EcritureTrace(XZSTC_DEBUG1, "array real8");
			tibems_double *pstf= va_arg(vl_listeArgument, double *);
			tibems_int nbf = va_arg(vl_listeArgument, int );
//			XZST_03EcritureTrace(XZSTC_WARNING, "array nb %d // elt 0f%f %f",nbf,pstf[0], pstf[1]);
			tibemsStreamMsg_WriteInt(msg,nbf);
			sprintf(vl_trace_message,"%s [%d", vl_trace_message, nbf);
			for (indice=0;indice<nbf;indice++)
			{
				tibemsStreamMsg_WriteDouble(msg,pstf[indice]);
				sprintf(vl_trace_message,"%s {%f}", vl_trace_message, pstf[indice]);
			}
			sprintf(vl_trace_message,"%s]", vl_trace_message);
		}		
		else if (vl_type ==T_IPC_FT_STR) 
		{
			char *vl_str = va_arg(vl_listeArgument, char *);
//			XZST_03EcritureTrace(XZSTC_WARNING, "vl_str = %x",vl_str);
			if (vl_str!=NULL)
			{
				tibemsStreamMsg_WriteString(msg, vl_str);
			}
			else
			{
//				XZST_03EcritureTrace(XZSTC_WARNING, "tibemsStreamMsg_WriteString(msg, '''');");
				tibemsStreamMsg_WriteString(msg, "");
			}
//			XZST_03EcritureTrace(XZSTC_DEBUG1, "str [%s]",vl_str);
			sprintf(vl_trace_message,"%s {\"%s\"}", vl_trace_message, vl_str);
		}
		else if (vl_type==T_IPC_FT_STR_ARRAY) 
		{
//			XZST_03EcritureTrace(XZSTC_DEBUG1, "array str");
			char *psts= va_arg(vl_listeArgument, char *);
			tibems_int nbs = va_arg(vl_listeArgument, int );
			taille = va_arg(vl_listeArgument, int );
//			XZST_03EcritureTrace(XZSTC_WARNING, "array nb %d ",nbs);
//			XZST_03EcritureTrace(XZSTC_WARNING, "array size %d ",taille);
			tibemsStreamMsg_WriteInt(msg,nbs);
			tibemsStreamMsg_WriteInt(msg,taille);
			sprintf(vl_trace_message,"%s [%d %d", vl_trace_message, nbs, taille);
			for (indice=0;indice<nbs;indice++) 
			{
//XZST_03EcritureTrace(XZSTC_WARNING, "ptr=%d elt [%s]",taille*indice,psts+taille*indice);
				tibemsStreamMsg_WriteString(msg,psts+taille*indice);
				sprintf(vl_trace_message,"%s {\"%s\"}", vl_trace_message, psts+taille*indice);
			}		
			sprintf(vl_trace_message,"%s]", vl_trace_message);
//XZST_03EcritureTrace(XZSTC_WARNING, "Write [%d] : {%s }", va_num_msg, vl_trace_message);
		}		
		else if (vl_type==T_IPC_FT_CHAR) 
		{
			tibems_wchar pc = (tibems_wchar) va_arg(vl_listeArgument, int );
			tibemsStreamMsg_WriteChar(msg, pc);
//			XZST_03EcritureTrace(XZSTC_DEBUG1, "char (ascii) [%d]",pc);
			sprintf(vl_trace_message,"%s {'%d'}", vl_trace_message, pc);
		}	
	}
	va_end(vl_listeArgument);

       /* publish the message */
/*        status = tibemsMsgProducer_SendToDestination (producer,destination,msg);
*/
	status = tibemsMsgProducer_SendToDestinationEx (msgProducer,destination,msg,TIBEMS_NON_PERSISTENT,5,10000L);

	if (status != TIBEMS_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"TipcSrvMsgWrite : erreur publish message %s", tibems_errorString (errorContext));
	}
	else
		// reinitialise ne nombre d'erreurs de connexion a 0
		vm_nb_ConnectionErrors = 0;

	XZST_03EcritureTrace(XZSTC_WARNING, "Write [%d] : {%s }", va_num_msg, vl_trace_message);

     /* destroy the message */
	XZEMS_03DestroyMsg(msg);
	if (vl_destination_cree)
	{
		/* detruire la destination cree */
		tibemsDestination_Destroy( destination);
	}

//	XZST_03EcritureTrace(XZSTC_WARNING,"TipcSrvMsgWrite OUT");

	return (TRUE);
}

void onException(
tibemsConnection            conn,
tibems_status               reason,
void*                       closure)
{
	/* print the connection exception status */

	if (reason == TIBEMS_SERVER_NOT_CONNECTED)
	{
//		vm_nb_ConnectionErrors++;
//		if (vm_nb_ConnectionErrors>(TIMEOUT_LOSS_CONNECTION_TERM_MS/TIMEOUT_MAIN_LOOP_MS))
			XZST_03EcritureTrace(XZSTC_FATAL," Server ems Disconnected");
//		else
//		   	XZST_03EcritureTrace(XZSTC_WARNING," Server Disconnected - retry %d/%d",vm_nb_ConnectionErrors,TIMEOUT_LOSS_CONNECTION_TERM_MS/TIMEOUT_MAIN_LOOP_MS);
	}
	else
   		XZST_03EcritureTrace(XZSTC_WARNING," Catch Exception reason=%d", reason);
	
	if (msgProducer!=NULL)
		tibemsMsgProducer_Close(msgProducer);
}

int XZEMS_02Connexion(char *va_URL)
{
tibems_status               status       = TIBEMS_OK;
tibemsTextMsg               msg          = NULL;
tibems_int                  i;
tibemsSSLParams 	SSL=NULL;
char			vl_URL[100];

	status = tibemsErrorContext_Create(&errorContext);

	if (status != TIBEMS_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"erreur create context %s", tibems_errorString (errorContext));
	  	//printf("erreur create context %s", tibems_errorString (errorContext));
	        return (FALSE);    
	}

	factory = tibemsConnectionFactory_Create();
	if (!factory)
	{
  		XZST_03EcritureTrace(XZSTC_WARNING,"erreur create factory %s", tibems_errorString (errorContext));
	  	//printf("erreur create factory %s", tibems_errorString (errorContext));
	        return (FALSE);    
	}

	//printf("URL %s\n",va_URL);
	sprintf(vl_URL,"tcp://%s:7222",va_URL);
	XZST_03EcritureTrace(XZSTC_WARNING,"Url du serveur ems : %s",vl_URL);
	status = tibemsConnectionFactory_SetServerURL(factory,vl_URL);
	if (status != TIBEMS_OK) 
	{
  		XZST_03EcritureTrace(XZSTC_WARNING,"erreur set URL %s", tibems_errorString (errorContext));
	        return (XDC_NOK);    
	}

	//printf("avant connection\n");
	status = tibemsConnectionFactory_CreateConnection(factory,&connection,
                                                      userName,password);
	if (status != TIBEMS_OK)
	{
 		XZST_03EcritureTrace(XZSTC_WARNING," erreur create connection %s", tibems_errorString (errorContext));
	        return (FALSE);    
	}

	/* set the exception listener */
	status = tibemsConnection_SetExceptionListener(connection,
            onException, NULL);
	if (status != TIBEMS_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"erreur exception listener %s", tibems_errorString (errorContext));
	        return (FALSE);    
	}

	/* create the session */
	status = tibemsConnection_CreateSession(connection,
            &session,TIBEMS_FALSE,TIBEMS_AUTO_ACKNOWLEDGE);
	if (status != TIBEMS_OK)
	{
   		XZST_03EcritureTrace(XZSTC_WARNING,"erreur create session %s", tibems_errorString (errorContext));
	        return (FALSE);    
	}

	status = tibemsSession_CreateProducer(session,
            &msgProducer,NULL);
	if (status != TIBEMS_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"TipcSrvMsgWrite3: erreur creation producer %s", tibems_errorString (errorContext));
	        return (XDC_NOK);
	}

	//printf("avant start\n");
	/* start the connection */
	status = tibemsConnection_Start(connection);
	if (status != TIBEMS_OK)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"erreur start %s", tibems_errorString (errorContext));
	        return (FALSE);    
	}
	SSL= tibemsSSLParams_Create();
	tibemsSSLParams_SetVerifyHost(SSL , TIBEMS_FALSE);
	//sprintf(vl_URL,"%s:7222",vl_URL);
	//printf("URL admin : %s\n",vl_URL);
	status = tibemsAdmin_Create(&admin, vl_URL,"admin",NULL,SSL);
	if (status != TIBEMS_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"erreur grave tibemsAdmin_Create %s",tibems_errorString (errorContext));

	}

	// initialise ne nombre d'erreurs de connexion a 0
	vm_nb_ConnectionErrors = 0;
	//printf("sortie\n");
	return (XDC_OK);
}

void TipcMsgGetSender(tibemsMsg msg,T_STR *str)
{
tibemsDestination       vl_destinataire = NULL;
char                    vl_sender[255];
tibems_int              vl_size;
tibems_status           status       = TIBEMS_OK;

        status = tibemsMsg_GetReplyTo( msg, &vl_destinataire );
        if (status != TIBEMS_OK)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"erreur get sender %s", tibems_errorString (errorContext));
        }
//else
//XZST_03EcritureTrace(XZSTC_WARNING,"get reply to %s", vl_destinataire );

		if (vl_destinataire!=NULL)
		{
        	vl_size=sizeof(vl_sender);
        	status = tibemsDestination_GetName( vl_destinataire, vl_sender, vl_size );
        	if (status != TIBEMS_OK)
        	{
            	XZST_03EcritureTrace(XZSTC_WARNING,"erreur get sender name %s", tibems_errorString (errorContext));
				strcpy(vl_sender,"");
        	}
		}
		else
			strcpy(vl_sender,"");

        XZST_03EcritureTrace(XZSTC_WARNING,"tibemssender retourne [%s]",vl_sender);

        strcpy( str, vl_sender );
}

void TipcMsgGetDest(tibemsMsg msg,T_STR *str)
{
tibemsDestination 	vl_destinataire = NULL;
char 			vl_dest[255];
tibems_int		vl_size;
tibems_status		status       = TIBEMS_OK;

	status = tibemsMsg_GetDestination( msg, &vl_destinataire );
	if (status != TIBEMS_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"erreur get destinataire %s", tibems_errorString (errorContext));
	}
	vl_size=sizeof(vl_dest);
	status = tibemsDestination_GetName( vl_destinataire, vl_dest, vl_size );
	if (status != TIBEMS_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"erreur get destinataire name %s", tibems_errorString (errorContext));
	}
	XZST_03EcritureTrace(XZSTC_WARNING,"tibemsDestination_GetName retourne [%s]",vl_dest);

	strcpy( str, vl_dest );
}
#endif

/*
* Librairie bus de message.
* Objectif : Migration bus de message ems -> rabbitmq
*
* EXPLICATION : Le modèle AMQP 0-9-1 : Les messages sont publies dans des echanges
									   Les echanges distribuent ensuite des copies des messages aux files d'attente à l'aide de regles appelees "bindings".
									   Ensuite, le courtier livre les messages aux consommateurs abonnes aux files d'attente, ou les consommateurs recuperent les messages des files d'attente à la demande.

* HISTRORIQUE :
* 
* Adaptations   : KAMMAH Oumaima
* Verification  : CLAUDEL Laurent 
*
*/
#ifdef _RABBITMQ
#include <xzsem.h>
#include "time.h"
#include <sys/time.h>
#include "xzems.h"
#include "xzsm.h"


/*-----------------------------------------------------------------------
 * Variables
 *----------------------------------------------------------------------*/

int							   vm_nb_cb=0;
int							   vm_nb_dg_surv = 0;
XDY_Datagroup				   *tg_dg_surv=NULL;
int							   *tg_dg_surv_presence=NULL;

amqp_basic_consume_ok_t* 		vg_consume_ok;
amqp_channel_t 					vg_amqp_channel = 1;                         // Numero du canal AMQP utilise pour la communication.
amqp_rpc_reply_t 				vg_rpc_reply;                                // Variable globale qui stocke la reponse procedure a distance RPC (Remote Procedure Call) AMQP.
T_CALLBACKS_MSG					*tg_cb= NULL;
char 							vg_executablePath[256];                      // Tableau de caracteres pour stocker le chemin absolu de l'executable en cours. 
char 							vg_executableName[256];                      // Tableau de caracteres qui stocke le nom de la tâche (nom de l'executable) avec une taille maximale definie par MAX_EXECUTABLE_NAME_LENGTH.
char							vg_hostname[256];
char							vg_appId[512];
amqp_connection_state_t         vg_conn= NULL;                         		 // Variable globale qui représente l'etat de la connexion AMQP (Advanced Message Queuing Protocol).
amqp_channel_close_ok_t 		vg_close_ok;

const char *tibems_errorString(amqp_rpc_reply_t va_reply);

/* 
*  FONCTION : isMessageNotNULL(T_IPC_MSG va_msg)
*  Etat     : 
*/
/* 
*  FONCTION 	: isMessageNotNULL(T_IPC_MSG va_msg)
*  OBJECTIF     : Retourne un booleen pour savoir si un message est non NULL
*/
int isMessageNotNULL(T_IPC_MSG va_msg)
{
	return (va_msg.exchange.len>0);
}

/* 
*  FONCTION : TipcSrvSubjectGetSubscribeDestination(char *va_topic, T_Dest *va_dest)
*  Etat     : Pas utulise
*/
/* 
*  FONCTION 	: TipcSrvSubjectGetSubscribe(char *va_topic, int *va_etat)
*  OBJECTIF     : Savoir si on est abonne a un topic.
*/
void TipcSrvSubjectGetSubscribe(char *va_topic, int *va_etat)
{
	*va_etat = XDG_JoindreDG(va_topic);
}

int declareExchange(char *va_topic)
{
    amqp_exchange_declare_ok_t *vl_exchange_info;
	vl_exchange_info = amqp_exchange_declare(
												vg_conn,                      // Etat connexion 
												vg_amqp_channel,			  // Canal
												amqp_cstring_bytes(va_topic), // Nom echange
												amqp_cstring_bytes("fanout"), // Type echange
												0,						  	// Passive
												0,						  	// Durable
												1,   					  	// Auto-delete
												0,						  	// Internal
												amqp_empty_table             // Arguments
											);

	vg_rpc_reply = amqp_get_rpc_reply(vg_conn);
	check_reply(vg_rpc_reply, "declareExchange : Echec amqp_exchange_declare\n");

	return(XDC_OK);
}
/*
*  FONCTION 	: XDG_QuitterDG(char *va_topic)
*  OBJECTIF     : Desabonnement du data Group
*/
int XDG_QuitterDG(char *va_topic)
{
    // Convertir le consumer_tag en chaîne de caractères
//    char vl_consumer_tag_str[vg_consume_ok->consumer_tag.len + 1];
    char vl_consumer_tag_str[1000];
printf("********************************  A RECODER\n");
return XDC_OK;

    memcpy(vl_consumer_tag_str, vg_consume_ok->consumer_tag.bytes, vg_consume_ok->consumer_tag.len);
    vl_consumer_tag_str[vg_consume_ok->consumer_tag.len] = '\0';

    // Annuler l'abonnement du consommateur à la file d'attente
    amqp_basic_cancel(vg_conn, vg_amqp_channel, amqp_cstring_bytes(vl_consumer_tag_str));

    // Vérifier si la demande d'annulation a reussi
    vg_rpc_reply = amqp_get_rpc_reply(vg_conn);
    if (vg_rpc_reply.reply_type == AMQP_RESPONSE_NORMAL) {
        XZST_03EcritureTrace(XZSTC_INFO,"Le consommateur avec le consumer_tag [%s] s est desabonne du sujet [%s].\n",vl_consumer_tag_str, va_topic);
        return XDC_OK; // Succes
    } else {
        XZST_03EcritureTrace(XZSTC_WARNING,"La desinscription du consommateur avec le consumer_tag [%s] du sujet [%s] a echoue.\n",vl_consumer_tag_str, va_topic);
        return XDC_NOK; // Echec
    } 
}
/*
 * FONCTION		:setExecutableName()
 * OBJECTIF		: Récupérer et stocker le nom de l'exécutable en cours d'exécution
 */

int setExecutableName() 
{
    ssize_t vl_len = readlink("/proc/self/exe", vg_executablePath, sizeof(vg_executablePath) - 1);      // Effectue la lecture du chemin absolu de l'exécutable en cours et stocke sa longueur dans la variable vl_len.
    if (vl_len != -1) {
        vg_executablePath[vl_len] = '\0';                                                               // Assure que la chaîne de caractères est correctement terminée
        char* vl_executableName = basename(vg_executablePath);                                          // Obtene le nom de fichier de l'executable en utilisant la fonction basename()
        strcpy(vg_executableName, vl_executableName);                                                   // Copie le nom de fichier de l'executable dans la variable globale vg_executableName

    } else {
        fprintf(stderr, "setExecutableName : Erreur lors de la récupération du nom de l'exécutable\n");
        return XDC_NOK;                                                                                      
    }
    return XDC_OK;                                                                                          
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

int checkGetHostName(char *va_hostname, size_t size) 
{
    char vl_hostname[256];                                                                                                 
    // Récupération du nom de la machine hôte
    if (gethostname(vl_hostname, sizeof(vl_hostname)) == -1) {
        /* Ecriture Trace */
        fprintf(stderr, "checkGetHostName : Récupération du nom de la machine impossible.\n");
        return XDC_NOK;
    } else {
        // Vérifier si la taille du nom d'hôte est inférieure à la taille de la chaîne de destination
        if (strlen(vl_hostname) < size) {
            strncpy(va_hostname, vl_hostname, size);                                                       // Copie le nom d'hote dans la chaine de destination avec une taille maximale de "size"
            va_hostname[size - 1] = '\0';                                                                                  
        } else {
            fprintf(stderr, "checkGetHostName : Taille de la chaîne de caractères de destination insuffisante.\n");
            return XDC_NOK;
        }
        return XDC_OK;                                                                                                           
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
int getQueueName(char* va_queueName, char* va_exchangeName) 
{
    // Construction du nom de la file d'attente:

    setExecutableName();
    if(vg_executableName == NULL){
		XZST_03EcritureTrace(XZSTC_WARNING,"getQueueName : Nom de la tache non initialisee au demarrage.");
	    return XDC_NOK;
    }
    else
    {
    	sprintf(va_queueName, "%s.%s.%s", vg_hostname, vg_executableName, va_exchangeName);
        return XDC_OK;
    }
}

int declareQueue(char *va_topic)
{
   
    amqp_queue_declare_ok_t* vl_queue_declare_ok;      //stocke infos de retour après déclaration queue

    char vl_queueName[200];
	if(getQueueName(vl_queueName, va_topic))
    {
		XZST_03EcritureTrace(XZSTC_WARNING,"declareQueue : Echec generation du nom de la queue");
		return XDC_NOK;
	}
    else
    {
        // Déclaration de la politique d'expiration des messages
        amqp_table_entry_t entries[1];
        entries[0].key = amqp_cstring_bytes("x-message-ttl");
        entries[0].value.kind = AMQP_FIELD_KIND_I32;
        entries[0].value.value.i32 = TTL_MS_MESSAGE_IN_QUEUE;

        amqp_table_t arguments;
        arguments.num_entries = 1;
        arguments.entries = entries;

		amqp_queue_declare_t queue_declare = {
            		.ticket = false,
            		.queue = amqp_cstring_bytes(vl_queueName),
            		.passive = false,
            		.durable = true,
            		.exclusive = false,
            		.auto_delete = true,
             		.arguments = amqp_empty_table
        };

        //Déclaration de la queue
        vl_queue_declare_ok = amqp_queue_declare(
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
		if (vg_rpc_reply.reply_type != AMQP_RESPONSE_NORMAL) 
		{
		 	XZST_03EcritureTrace(XZSTC_WARNING,"declareQueue : amqp_queue_declare failed [%s] - %s", vl_queueName, tibems_errorString(vg_rpc_reply));
            return XDC_NOK;			
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_INFO,"declareQueue : amqp_queue_declare successful [%s]",vl_queueName);
        }
      
	}
    return XDC_OK;
}


int bindExchangeToQueue(char *va_topic, char* va_queueName) 
{
        amqp_queue_bind_ok_t * vl_queue_bind_ok;
		// Lier la file d'attente à l'échange
        vl_queue_bind_ok = amqp_queue_bind(
                                    vg_conn,
                                    vg_amqp_channel,
                                    amqp_cstring_bytes(va_queueName),
                                    amqp_cstring_bytes(va_topic),
                                    amqp_cstring_bytes(""),
                                    amqp_empty_table);
    	
		vg_rpc_reply = amqp_get_rpc_reply(vg_conn);
		if (vg_rpc_reply.reply_type != AMQP_RESPONSE_NORMAL) 
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"bindExchangeToQueue : amqp_queue_bind failed [%s] - %s", va_queueName, tibems_errorString(vg_rpc_reply));
		    return XDC_NOK;
	    } 
        else
        {
            XZST_03EcritureTrace(XZSTC_INFO,"bindExchangeToQueue : amqp_queue_bind successful [%s]", va_queueName);
        }
        return XDC_OK;
}

void handle_channel_close(amqp_channel_t va_amqp_channel) 
{
    // Envoyer la réponse AMQP_CHANNEL_CLOSE_OK_METHOD
    amqp_channel_close_ok_t close_ok;
    amqp_frame_t frame;
    frame.payload.method.id = AMQP_CHANNEL_CLOSE_OK_METHOD;
    frame.payload.method.decoded = &close_ok;
    frame.channel = va_amqp_channel;
    frame.frame_type = AMQP_FRAME_METHOD;
    amqp_send_frame(vg_conn, &frame);
    amqp_rpc_reply_t rpc_reply = amqp_get_rpc_reply(vg_conn);
	// L'appel à amqp_send_frame ne nécessite pas d'appel ultérieur à amqp_get_rpc_reply.
	// amqp_send_frame est utilisé pour envoyer un frame directement à RabbitMQ sans attente de réponse RPC. 
	// Il envoie simplement un cadre sur la connexion vers RabbitMQ et retourne le statut de l'opération d'envoi. 
	// Cette fonction est principalement utilisée pour l'envoi de cadres spécifiques du protocole AMQP.
}

int XDG_JoindreDG(char *va_topic) 
{
	int vl_etat;
    char vl_queueName[1000];
	amqp_bytes_t  vl_consumer_tag;

	XZST_03EcritureTrace(XZSTC_WARNING,"XDG_JoindreDG [%s]",va_topic);

	if (declareExchange(va_topic)==XDC_NOK)
    {    
		XZST_03EcritureTrace(XZSTC_WARNING,"XDG_JoindreDG : Echec de la declaration de l'exchange [%s]",va_topic);
        return FALSE;
    }

	if(declareQueue(va_topic))
    {    
		XZST_03EcritureTrace(XZSTC_WARNING,"XDG_JoindreDG : Echec de la declaration de la queue [%s]",vl_queueName);
        return FALSE;
    }

	if(getQueueName(vl_queueName, va_topic))
    {
		XZST_03EcritureTrace(XZSTC_WARNING,"XDG_JoindreDG : Echec de la generation du nom de la queue [%s]!\n",vl_queueName);
		return FALSE;
	}
    else 
    {

        if(bindExchangeToQueue(va_topic, vl_queueName))
        {
            return FALSE;
        }
        else
        {

            XZST_03EcritureTrace(XZSTC_INFO,"XDG_JoindreDG : Abonnement au topic  [%s] avec succes!\n",va_topic);
            // Configuration de la file d'attente en tant que consommateur
            vg_consume_ok = amqp_basic_consume(
												vg_conn,
												vg_amqp_channel,
												amqp_cstring_bytes(vl_queueName),
												amqp_empty_bytes,
												0,					// no_local
												1,					// no_ack
												0,					// exclusive
												amqp_empty_table);
          
			vg_rpc_reply = amqp_get_rpc_reply(vg_conn);
    		if (check_reply(vg_rpc_reply, "XDG_JoindreDG : Echec de amqp_basic_consume")) 
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"XDG_JoindreDG : amqp_basic_consume failed [%s] - %s", vl_queueName, tibems_errorString(vg_rpc_reply));
                deconnectionRabbitmq();
                return FALSE;
            }
            else
            {
                XZST_03EcritureTrace(XZSTC_INFO,"XDG_JoindreDG : amqp_basic_consume successful [%s]",vl_queueName);  
                // Afficher les informations sur les consommateurs actifs
                vl_consumer_tag = vg_consume_ok->consumer_tag;
            }   
        } 
    }  
    return TRUE;
}

T_BOOL  TipcMonSubjectSubscribeGetWatch(char *name, T_BOOL *etat)
{
int vl_indice;
	/*recherche si dg deja surveille*/
	for (vl_indice=0; vl_indice<vm_nb_dg_surv;vl_indice++) {
		if (!strcmp(tg_dg_surv[vl_indice], name)) {
			XZST_03EcritureTrace(XZSTC_INFO,"deja surveille %s %s",name,tg_dg_surv[vl_indice]);
			*etat=TRUE;
			return (TRUE);
		}
	}
	*etat = FALSE;
	return (TRUE);
}

void XDG_tibemsCollection_Destroy(int va_collection)
{
	return;
}

int XZEMS_03IsTopic(char *va_name)
{
 	// Vérifier si l'échange est toujours lié à des files d'attente
 	XZST_03EcritureTrace(XZSTC_FONCTION,"XZEMS_03IsTopic IN : [%s]",va_name);

	// On utilise un canal specifique pour tester la presence d'un exchange
//	int vl_amqp_check_channel = AMQP_DEFAULT_MAX_CHANNELS - vg_amqp_channel;
	int vl_amqp_check_channel = AMQP_DEFAULT_MAX_CHANNELS;

	amqp_channel_open_ok_t* vl_channel_open_ok;
    vl_channel_open_ok = amqp_channel_open(vg_conn, vl_amqp_check_channel);
    vg_rpc_reply = amqp_get_rpc_reply(vg_conn);
    if (check_reply(vg_rpc_reply, "XZEMS_03IsTopic : Echec de l'ouverture du check channel\n")) 
	{
        return(NULL);
    }

    amqp_exchange_declare_ok_t *vl_exchange_info = amqp_exchange_declare(
																			vg_conn, 
																			vl_amqp_check_channel, 
																			amqp_cstring_bytes(va_name),
                                                                     		amqp_cstring_bytes("fanout"),
                                                                     		1, 		// Passive
																			0, 		// Durable
																			1,		// Auto Delete
																			0, 		// Internal
																			amqp_empty_table);

	vg_rpc_reply = amqp_get_rpc_reply(vg_conn);
	if (vg_rpc_reply.reply_type == AMQP_RESPONSE_SERVER_EXCEPTION && vg_rpc_reply.reply.id == AMQP_CHANNEL_CLOSE_METHOD)
	{
		// Acquittement de la demande de fermeture
		// sinon le channel n'accepte pas la reouverture la fois suivante
		handle_channel_close(vl_amqp_check_channel);
		
		amqp_channel_close_t *vl_chan_close;
		vl_chan_close = (amqp_channel_close_t *) vg_rpc_reply.reply.decoded;
		XZST_03EcritureTrace(XZSTC_WARNING, "%.*s",(int)vl_chan_close->reply_text.len, (char*)vl_chan_close->reply_text.bytes);
		if (vl_chan_close->reply_code==AMQP_NOT_FOUND)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"XZEMS_03IsTopic : Topic inexistant [%s]", va_name);
			return NULL;
		}
		return NULL;
	}
    amqp_channel_close(vg_conn, vl_amqp_check_channel, AMQP_REPLY_SUCCESS);
    amqp_rpc_reply_t close_reply = amqp_get_rpc_reply(vg_conn);
    if (close_reply.reply_type != AMQP_RESPONSE_NORMAL) 
	{
        XZST_03EcritureTrace(XZSTC_WARNING,"%s Erreur lors de la fermeture du canal %d", vg_appId,vl_amqp_check_channel);
    }

	XZST_03EcritureTrace(XZSTC_WARNING,"XZEMS_03IsTopic : Topic existant [%s]", va_name);
	return TRUE;
}

void TutCbDestroy(void (*CB)())
{
	int vl_indice;

//XZST_03EcritureTrace(XZSTC_INFO,"TutCbDestroy : IN");
	for (vl_indice=0; vl_indice<vm_nb_cb;vl_indice++) {
		if (tg_cb[vl_indice].CB == CB) {
			XZST_03EcritureTrace(XZSTC_INFO,"cb trouve");
			free(tg_cb[vl_indice].CB);
			tg_cb[vl_indice].CB=NULL;
//XZST_03EcritureTrace(XZSTC_INFO,"TutCbDestroy : OUT");
			return ;
		}
/*		else
XZST_03EcritureTrace(XZSTC_INFO,"TutCbDestroy : vl_indice=%d vm_nb_cb=%d",vl_indice, vm_nb_cb);
*/
//XZST_03EcritureTrace(XZSTC_INFO,"TutCbDestroy : OUT");

	}
}

/*
* DESCRIPTION FONCTION: TipcSrvProcessCbCreate(int va_msg, void (*CB)(T_IPC_CONN c,T_IPC_CONN_PROCESS_CB_DATA d ,T_CB_ARG a), void *CB_ARG)
*
* But				  :	Créer et enregistrer un rappel (callback) de traitement pour un message donné. 
* Entrés			  :	Numéro du message va_msg, un pointeur vers la fonction de rappel CB, et un pointeur générique CB_ARG qui peut être utilisé pour transmettre des données supplémentaires à la fonction de rappel.	
* Logique             : Recherche d'abord si un rappel pour le message spécifié (va_msg) existe déjà dans le tableau tg_cb. Si oui, cela signifie que le rappel a déjà été créé pour ce message, et la fonction retourne TRUE pour indiquer que le rappel est déjà existant.
						Sinon elle procède à son ajout dans le tableau tg_cb. Si le tableau est vide (c'est-à-dire, aucun rappel n'a été créé auparavant), elle alloue dynamiquement de l'espace pour le premier élément du tableau et l'initialise. Sinon, elle utilise la fonction realloc pour agrandir le tableau existant d'un élément et y ajouter le nouveau rappel.
* Sortie			  : Une fois le rappel ajouté avec succès, la fonction retourne TRUE.	
* P
*/

int TipcSrvProcessCbCreate(int va_msg, void (*CB)(T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG ), void *CB_ARG)
{
	int vl_indice;
	int vl_index;

	XZST_03EcritureTrace(XZSTC_INTERFACE,"TipcSrvProcessCbCreate : %d",va_msg);

	/*recherche si cb deja cree*/
	for (vl_indice=0; vl_indice<vm_nb_cb;vl_indice++) 
	{
		if (tg_cb[vl_indice].Numero == va_msg) 
		{
			XZST_03EcritureTrace(XZSTC_INFO,"cb deja existant sur %d",va_msg);
			return TRUE;
		}
	}

	/*ajout du CB dans le tableau*/
	if (vm_nb_cb==0) 
	{
		tg_cb = (T_CALLBACKS_MSG *)malloc(sizeof(T_CALLBACKS_MSG));
		if (tg_cb==NULL) 
		{
			//printf("erreur malloc tg_CB");
 			XZST_03EcritureTrace(XZSTC_WARNING,"erreur malloc tg_CB");
			return FALSE;
		}		
		vm_nb_cb ++;
	}
	else 
	{
		tg_cb = (T_CALLBACKS_MSG *)realloc(tg_cb,((size_t) vm_nb_cb +1)*sizeof(T_CALLBACKS_MSG));
		if (tg_cb==NULL) {
 			XZST_03EcritureTrace(XZSTC_WARNING,"erreur realloc tg_CB");
			return FALSE;
		}
		vm_nb_cb ++;
	}	
	tg_cb[vm_nb_cb-1].Numero = va_msg;
	tg_cb[vm_nb_cb-1].CB = CB;
	tg_cb[vm_nb_cb-1].CB_ARG = CB_ARG;

	return T_TRUE;
}

/*
* TipcMsgNextShort : On utilise %hd dans sscanf pour indiquer que nous lisons un (short) qui est dans le champs. La fonction renverra 1 si la conversion est réussie, sinon elle renverra 0.
* TipcMsgNextChar  : Extraire et stocker un seule caractere du message
* TipcMsgNextReal  : Extraire et stocker un double
*/

int TipcMsgNextInt2(amqp_envelope_t va_msg, short *va_value) {
    char *vl_payload = (char *)va_msg.message.body.bytes;
    size_t vl_payload_len = va_msg.message.body.len;
	amqp_basic_properties_t vl_properties = va_msg.message.properties;
	long vl_offset = 0L;
	amqp_basic_properties_t *vl_ptr_properties = &vl_properties;
	amqp_table_t vl_headers = vl_ptr_properties->headers;

	// On verifie si le message est en cours de décodage
	if (vl_ptr_properties->_flags & AMQP_BASIC_HEADERS_FLAG)
	{
		vl_offset=vl_headers.entries[0].value.value.i64;
	}

//	XZST_03EcritureTrace(XZSTC_DEBUG1, "TipcMsgNextInt2 : Offset lu : [%ld]",vl_offset);
	// on recale le début du payload
	vl_payload += vl_offset;
//	XZST_03EcritureTrace(XZSTC_WARNING, "TipcMsgNextInt2 : Payload lu : [%s]",vl_payload);

    // Recherche de la première accolade ouvrante '{'
    char *vl_start_brace = strchr(vl_payload, '{');
    if (vl_start_brace == NULL) {
		XZST_03EcritureTrace(XZSTC_WARNING, "Accolade ouvrante manquante dans la chaîne du message RabbitMQ [%s]", vl_payload);
        return XDC_NOK;  // Échec de la lecture
    }

    // Recherche de la première accolade fermante '}' à partir de l'accolade ouvrante trouvée
    char *vl_end_brace = strchr(vl_start_brace, '}');
    if (vl_end_brace == NULL) 
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "Accolade fermante manquante dans la chaîne du message RabbitMQ [%s]", vl_payload);
        return XDC_NOK;  // Échec de la lecture
    }

    // Utilisation de sscanf pour extraire l'entier court (short) de la chaîne réelle
    if (sscanf(vl_start_brace + 1, "%hd", va_value) != 1) 
	{  
       XZST_03EcritureTrace(XZSTC_WARNING, "Echec de la conversion de la chaine en entier court (Int2).");
        return XDC_NOK;  // Échec de la conversion
    }
//	XZST_03EcritureTrace(XZSTC_DEBUG1, "TipcMsgNextInt2 : Int2 en String lu : [%s] de longueur %d",vl_start_brace + 1,vl_end_brace - vl_start_brace - 1);
//	XZST_03EcritureTrace(XZSTC_WARNING, "TipcMsgNextInt2 : Int2 lu : [%d]",*va_value);

	if (vl_ptr_properties->_flags & AMQP_BASIC_HEADERS_FLAG)
	{
//		XZST_03EcritureTrace(XZSTC_DEBUG1, "TipcMsgNextInt2 : Offset calculé : [%d]",(int)vl_end_brace - (int)vl_start_brace + 1);
		vl_headers.entries[0].value.value.i64 += (int)vl_end_brace - (int)vl_start_brace + 1;
//		XZST_03EcritureTrace(XZSTC_DEBUG1, "TipcMsgNextInt2 : Offset stocké : [%ld]",vl_headers.entries[0].value.value.i64);
	}
    return XDC_OK;  // Succès de la lecture
}

int TipcMsgNextChar(amqp_envelope_t va_msg, char *va_value) {
    char *vl_payload = (char *)va_msg.message.body.bytes;
    size_t vl_payload_len = va_msg.message.body.len;
	amqp_basic_properties_t vl_properties = va_msg.message.properties;
	long vl_offset = 0L;
	amqp_basic_properties_t *vl_ptr_properties = &vl_properties;
	amqp_table_t vl_headers = vl_ptr_properties->headers;

	// On verifie si le message est en cours de décodage
	if (vl_ptr_properties->_flags & AMQP_BASIC_HEADERS_FLAG)
	{
		vl_offset=vl_headers.entries[0].value.value.i64;
	}

//	XZST_03EcritureTrace(XZSTC_DEBUG1, "TipcMsgNextChar : Offset lu : [%ld]",vl_offset);
	// on recale le début du payload
	vl_payload += vl_offset;
//	XZST_03EcritureTrace(XZSTC_WARNING, "TipcMsgNextChar : Payload lu : [%s]",vl_payload);

    // Recherche de la première accolade ouvrante '{'
    char *vl_start_brace = strchr(vl_payload, '{');
    if (vl_start_brace == NULL) {
		
        XZST_03EcritureTrace(XZSTC_WARNING, "Accolade ouvrante manquante dans la chaîne du message RabbitMQ [%s]", vl_payload);
        return XDC_NOK;  // Échec de la lecture
    }

    // Recherche de la première accolade fermante '}' à partir de l'accolade ouvrante trouvée
    char *vl_end_brace = strchr(vl_start_brace, '}');
    if (vl_end_brace == NULL) 
	{
        XZST_03EcritureTrace(XZSTC_WARNING, "Accolade fermante manquante dans la chaine du message RabbitMQ [%s]", vl_payload);
        return XDC_NOK;  // Échec de la lecture
    }

    // Vérification que la longueur de la chaîne entre les accolades est de 2 caractères (y compris '{}')
    if (vl_end_brace - vl_start_brace != 1) 
	{
        XZST_03EcritureTrace(XZSTC_WARNING, "La chaine entre les accolades ne contient pas un seul caractère [%s]", vl_payload);
        return XDC_NOK;  // Échec de la lecture
    }
    // Copie du caractere situe entre les accolades dans la variable va_value
    *va_value = vl_start_brace[1];  // Le caractère après '{'
//	XZST_03EcritureTrace(XZSTC_WARNING, "TipcMsgNextChar : Char lu : [%c]",*va_value);

	if (vl_ptr_properties->_flags & AMQP_BASIC_HEADERS_FLAG)
	{
//		XZST_03EcritureTrace(XZSTC_DEBUG1, "TipcMsgNextChar : Offset calculé : [%d]",vl_end_brace - vl_start_brace + 1);
		vl_headers.entries[0].value.value.i64 += vl_end_brace - vl_start_brace + 1;
//		XZST_03EcritureTrace(XZSTC_DEBUG1, "TipcMsgNextChar : Offset stocké : [%ld]",vl_headers.entries[0].value.value.i64);
	}

    return XDC_OK;  // Succès de la lecture
}


int TipcMsgNextReal(amqp_envelope_t va_msg, double *va_value) {
    char *vl_payload = (char *)va_msg.message.body.bytes;
    size_t vl_payload_len = va_msg.message.body.len;
	amqp_basic_properties_t vl_properties = va_msg.message.properties;
	long vl_offset = 0L;
	amqp_basic_properties_t *vl_ptr_properties = &vl_properties;
	amqp_table_t vl_headers = vl_ptr_properties->headers;

	// On verifie si le message est en cours de décodage
	if (vl_ptr_properties->_flags & AMQP_BASIC_HEADERS_FLAG)
	{
		vl_offset=vl_headers.entries[0].value.value.i64;
	}

//	XZST_03EcritureTrace(XZSTC_DEBUG1, "TipcMsgNextReal : Offset lu : [%ld]",vl_offset);
	// on recale le début du payload
	vl_payload += vl_offset;
//	XZST_03EcritureTrace(XZSTC_WARNING, "TipcMsgNextReal : Payload lu : [%s]",vl_payload);

    // Recherche de la première accolade ouvrante '{'
    char *vl_start_brace = strchr(vl_payload, '{');
    if (vl_start_brace == NULL) 
	{
        XZST_03EcritureTrace(XZSTC_WARNING, "Accolade ouvrante manquante dans la chaîne du message RabbitMQ [%s]", vl_payload);
        return XDC_NOK;  // Échec de la lecture
    }

    // Recherche de la première accolade fermante '}' à partir de l'accolade ouvrante trouvée
    char *vl_end_brace = strchr(vl_start_brace, '}');
    if (vl_end_brace == NULL) 
	{
        XZST_03EcritureTrace(XZSTC_WARNING, "Accolade fermante manquante dans la chaîne du message RabbitMQ [%s]", vl_payload);
        return XDC_NOK;  // Échec de la lecture
    }

    // Utilisation de sscanf pour extraire l'entier court (short) de la chaîne réelle
    if (sscanf(vl_start_brace + 1, "%lf", va_value) != 1)
	{
        XZST_03EcritureTrace(XZSTC_WARNING, "Echec de la conversion de la chaine du message RabbitMQ [%s] en double", vl_payload);
        return XDC_NOK;  // Échec de la conversion
    }

//	XZST_03EcritureTrace(XZSTC_DEBUG1, "TipcMsgNextReal : Real en String lu : [%s] de longueur %d",vl_start_brace + 1,vl_end_brace - vl_start_brace - 1);
//	XZST_03EcritureTrace(XZSTC_WARNING, "TipcMsgNextReal : Real lu : [%lf]",*va_value);

	if (vl_ptr_properties->_flags & AMQP_BASIC_HEADERS_FLAG)
	{
//		XZST_03EcritureTrace(XZSTC_DEBUG1, "TipcMsgNextReal : Offset calculé : [%d]",vl_end_brace - vl_start_brace + 1);
		vl_headers.entries[0].value.value.i64 += vl_end_brace - vl_start_brace + 1;
//		XZST_03EcritureTrace(XZSTC_DEBUG1, "TipcMsgNextReal : Offset stocké : [%ld]",vl_headers.entries[0].value.value.i64);
	}

    return(XDC_OK);  // Succès de la lecture
}

int TipcMsgNextInt4(amqp_envelope_t va_msg, int *va_value) {
    char *vl_payload = (char *)va_msg.message.body.bytes;
    size_t vl_payload_len = va_msg.message.body.len;
	amqp_basic_properties_t vl_properties = va_msg.message.properties;
	long vl_offset = 0L;
	amqp_basic_properties_t *vl_ptr_properties = &vl_properties;
	amqp_table_t vl_headers = vl_ptr_properties->headers;

	// On verifie si le message est en cours de décodage
	if (vl_ptr_properties->_flags & AMQP_BASIC_HEADERS_FLAG)
	{
		vl_offset=vl_headers.entries[0].value.value.i64;
	}

//	XZST_03EcritureTrace(XZSTC_DEBUG1, "TipcMsgNextInt4 : Offset lu : [%ld]",vl_offset);
	// on recale le début du payload
	vl_payload += vl_offset;
//	XZST_03EcritureTrace(XZSTC_WARNING, "TipcMsgNextInt4 : Payload lu : [%s]",vl_payload);

    // Recherche de la première accolade ouvrante '{'
    char *vl_start_brace = strchr(vl_payload, '{');
    if (vl_start_brace == NULL) 
	{
        XZST_03EcritureTrace(XZSTC_WARNING, "Accolade ouvrante manquante dans la chaine du message RabbitMQ [%s]", vl_payload);
        return XDC_NOK;  // Échec de la lecture
    }

    // Recherche de la première accolade fermante '}' à partir de l'accolade ouvrante trouvée
    char *vl_end_brace = strchr(vl_start_brace, '}');
    if (vl_end_brace == NULL) 
	{
        XZST_03EcritureTrace(XZSTC_WARNING, "Accolade fermante manquante dans la chaine du message RabbitMQ [%s]", vl_payload);
        return XDC_NOK;  // Échec de la lecture
    }

    // Utilisation de sscanf pour extraire l'entier court (short) de la chaîne réelle
    if (sscanf(vl_start_brace + 1, "%d", va_value) != 1)
	{ 
        XZST_03EcritureTrace(XZSTC_WARNING, "Echec de la conversion de la chaine message RabbitMQ  [%s] en entier", vl_payload);
        return XDC_NOK;  // Échec de la conversion
    }

//	XZST_03EcritureTrace(XZSTC_DEBUG1, "TipcMsgNextInt4 : Int4 en String lu : [%s] de longueur %d",vl_start_brace + 1,vl_end_brace - vl_start_brace - 1);
//	XZST_03EcritureTrace(XZSTC_WARNING, "TipcMsgNextInt4 : Int4 lu : [%d]",*va_value);

	if (vl_ptr_properties->_flags & AMQP_BASIC_HEADERS_FLAG)
	{
//		XZST_03EcritureTrace(XZSTC_DEBUG1, "TipcMsgNextInt4 : Offset calculé : [%d]",vl_end_brace - vl_start_brace + 1);
		vl_headers.entries[0].value.value.i64 += vl_end_brace - vl_start_brace + 1;
//		XZST_03EcritureTrace(XZSTC_DEBUG1, "TipcMsgNextInt4 : Offset stocké : [%ld]",vl_headers.entries[0].value.value.i64);
	}

    return XDC_OK;  // Succès de la lecture
}

int TipcMsgNextStr(amqp_envelope_t va_msg, char **va_value) 
{
	char *vl_payload = (char *)va_msg.message.body.bytes;
	amqp_basic_properties_t vl_properties = va_msg.message.properties;
	long vl_offset = 0L;
	amqp_basic_properties_t *vl_ptr_properties = &vl_properties;
	amqp_table_t vl_headers = vl_ptr_properties->headers;

	// On verifie si le message est en cours de décodage
	if (vl_ptr_properties->_flags & AMQP_BASIC_HEADERS_FLAG)
	{
		vl_offset=vl_headers.entries[0].value.value.i64;
	}

//	XZST_03EcritureTrace(XZSTC_DEBUG1, "TipcMsgNextStr : Offset lu : [%ld]",vl_offset);
	// on recale le début du payload
	vl_payload += vl_offset;
//	XZST_03EcritureTrace(XZSTC_WARNING, "TipcMsgNextStr : Payload lu : [%s]",vl_payload);

    // Recherche de la première accolade ouvrante '{' avec strchr
    char *vl_start_brace = strchr(vl_payload, '{'); 
    if (vl_start_brace == NULL) {
        XZST_03EcritureTrace(XZSTC_WARNING, "Accolade ouvrante manquante dans la chaine du message RabbitMQ [%s]", vl_payload);
        return XDC_NOK;  // Échec de la lecture
    }
    // Recherche de la première accolade fermante '}' à partir de l'accolade ouvrante trouvée
    char *vl_end_brace = strchr(vl_start_brace, '}');
    if (vl_end_brace == NULL) {
        XZST_03EcritureTrace(XZSTC_WARNING, "Accolade fermante manquante dans la chaine du message RabbitMQ [%s]", vl_payload);
        return XDC_NOK;  // Échec de la lecture
    }

    // Allocation d'une mémoire tampon pour stocker la chaîne du int
    char vl_str_str[(int)(vl_end_brace - vl_start_brace)];
    size_t vl_str_length = (int)vl_end_brace - (int)vl_start_brace - 1;
	strncpy(vl_str_str, (int)vl_start_brace + 1, vl_str_length);
	vl_str_str[vl_str_length]='\0';
	memcpy(va_value,vl_str_str,sizeof(vl_str_str));
//	XZST_03EcritureTrace(XZSTC_WARNING, "TipcMsgNextStr : String lue : [%s]",va_value);

	if (vl_ptr_properties->_flags & AMQP_BASIC_HEADERS_FLAG)
	{
//		XZST_03EcritureTrace(XZSTC_DEBUG1, "TipcMsgNextStr : Offset calculé : [%d]",vl_str_length+2);
		vl_headers.entries[0].value.value.i64 += vl_str_length+2;
//		XZST_03EcritureTrace(XZSTC_DEBUG1, "TipcMsgNextStr : Offset stocké : [%ld]",vl_headers.entries[0].value.value.i64);
	}

    return XDC_OK;  // Succès de la lecture
}

int TipcMsgRead(amqp_envelope_t va_msg, ...)
{
va_list vl_listeArgument;
int i=0;
int indice;
int vl_type;
int *vl_int4;
short *tab;
int *tab4;
double *tabf;
int nb;          // Lire le nombre d'éléments dans le tableau depuis le corps du message
int taille;
int *nbparam;
char *vl_str=NULL;
char *tabstr;
char *str;
int vl_field_size;
short vl_field_type;
int vl_field_count;
int vl_type_origine;
int str_length;

	va_start(vl_listeArgument, va_msg);

	while (TRUE)
	{
		vl_type = va_arg(vl_listeArgument, int);
        if (vl_type == 0)
            break;

        if (vl_type == T_IPC_FT_INT4)
        {
            int *vl_pd = va_arg(vl_listeArgument, int *);
            if (vl_pd == NULL)
            {
                XZST_03EcritureTrace(XZSTC_WARNING, "erreur recuperation argument INT4");
                return (FALSE);
            }
            if (TipcMsgNextInt4(va_msg, vl_pd)==XDC_NOK)
            {
                XZST_03EcritureTrace(XZSTC_WARNING, "Erreur lors de la lecture de T_IPC_FT_INT4");
                return (FALSE);
            }
            XZST_03EcritureTrace(XZSTC_WARNING, "TipcMsgRead ReadInt4 [%d]", *vl_pd);
		}
		else if (vl_type == T_IPC_FT_INT4_ARRAY)
		{
			int *ps4= va_arg(vl_listeArgument, int *);
			TipcMsgNextInt4(va_msg,&nb);
			XZST_03EcritureTrace(XZSTC_WARNING, "TipcMsgRead Array ReadInt4 taille [%d]", nb);
			for (indice=0;indice<nb;indice++) 
			{
				TipcMsgNextInt4(va_msg,ps4+indice);
				XZST_03EcritureTrace(XZSTC_WARNING, "TipcMsgRead Array ReadInt4[%d]=[%d]", indice, *(ps4+indice));
			}		
			nbparam= va_arg(vl_listeArgument, int *);
			*nbparam=nb;
		}
		else if (vl_type == T_IPC_FT_INT2)
		{
			short *vl_ps2 = va_arg(vl_listeArgument, short *);
			if (vl_ps2 == NULL)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "erreur recuperation argument INT2");
				return (FALSE);
			}
			if (TipcMsgNextInt2(va_msg, vl_ps2)==XDC_NOK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "Erreur lors de la lecture de T_IPC_FT_INT2");
				return (FALSE);
			}
			XZST_03EcritureTrace(XZSTC_WARNING, "TipcMsgRead ReadShort [%d]", (int)*vl_ps2);
		}
		else if (vl_type == T_IPC_FT_INT2_ARRAY)
		{
			short *ps2= va_arg(vl_listeArgument, short *);
			TipcMsgNextInt4(va_msg,&nb);
			XZST_03EcritureTrace(XZSTC_WARNING, "TipcMsgRead Array ReadShort taille [%d]", nb);
			for (indice=0;indice<nb;indice++) 
			{
				TipcMsgNextInt2(va_msg,ps2+indice);
				XZST_03EcritureTrace(XZSTC_WARNING, "TipcMsgRead Array ReadShort[%d]=[%d]", indice, *(ps2+indice));
			}		
			nbparam= va_arg(vl_listeArgument, int *);
			*nbparam=nb;
		}
		else if (vl_type == T_IPC_FT_STR)
        {
            char *vl_str = va_arg(vl_listeArgument, char *);
            if (vl_str == NULL)
            {
                XZST_03EcritureTrace(XZSTC_WARNING, "Erreur recuperation argument STR");
                return (FALSE);
            }
            if (TipcMsgNextStr(va_msg, vl_str)==XDC_NOK)
            {
                XZST_03EcritureTrace(XZSTC_WARNING, "Erreur lors de la lecture du T_IPC_FT_STR");
                return (FALSE);
            }
            XZST_03EcritureTrace(XZSTC_WARNING, "TipcMsgRead ReadStr [%s]", vl_str);
        }
		else if (vl_type == T_IPC_FT_STR_ARRAY)
        {
			char **S;
			S = va_arg(vl_listeArgument, char **);
			TipcMsgNextInt4(va_msg,&nb);
			TipcMsgNextInt4(va_msg,&taille);
			nbparam= va_arg(vl_listeArgument, int *);
			*nbparam=nb;
			XZST_03EcritureTrace(XZSTC_WARNING, "TipcMsgRead Array ReadStr taille [%d]", nb);
			/* Initialisation pointeur sur adresse variable passee en parametre */
			char *ptr;
			ptr=&(S[0]);
			for (indice=0;indice<nb;indice++) 
			{
				TipcMsgNextStr(va_msg, &str);
				/* On met 1 octet de plus pour copier le \0 */
				memcpy(ptr,str,strlen(str)+1);
				ptr+=taille;
				XZST_03EcritureTrace(XZSTC_WARNING, "TipcMsgRead Array ReadStr[%d]=[%s]", indice, str);
			}
		}
		else if (vl_type == T_IPC_FT_REAL8)
        {
            double *vl_pf = va_arg(vl_listeArgument, double *);
            if (vl_pf == NULL)
            {
                XZST_03EcritureTrace(XZSTC_WARNING, "Erreur recuperation argument REAL8");
                return (FALSE);
            }
            if (TipcMsgNextReal(va_msg, vl_pf)==XDC_NOK)
            {
                XZST_03EcritureTrace(XZSTC_WARNING, "Erreur lors de la lecture du T_IPC_FT_REAL8");
                return (FALSE);
            }
            XZST_03EcritureTrace(XZSTC_WARNING, "TipcMsgRead ReadDouble [%f]", *vl_pf);
        }
		else if (vl_type == T_IPC_FT_REAL8_ARRAY)
        {
			double *pf4= va_arg(vl_listeArgument, double  *);
			TipcMsgNextInt4(va_msg,&nb);
			XZST_03EcritureTrace(XZSTC_WARNING, "TipcMsgRead Array ReadDouble taille [%d]", nb);
			for (indice=0;indice<nb;indice++) 
			{
				TipcMsgNextReal(va_msg,pf4+indice);
				XZST_03EcritureTrace(XZSTC_WARNING, "TipcMsgRead Array ReadDouble[%d]=[%s]", indice, *(pf4+indice));
			}		
			nbparam= va_arg(vl_listeArgument, int *);
			*nbparam=nb;
		}
		else if (vl_type == T_IPC_FT_CHAR)
        {
            char *vl_pc = va_arg(vl_listeArgument, char *);
            if (vl_pc == NULL)
            {
                XZST_03EcritureTrace(XZSTC_WARNING, "Erreur recuperation argument CHAR");
                return (FALSE);
            }
            if (TipcMsgNextChar(va_msg, vl_pc))
            {
                XZST_03EcritureTrace(XZSTC_WARNING, "Erreur lors de la lecture du T_IPC_FT_CHAR");
                return (FALSE);
            }
            XZST_03EcritureTrace(XZSTC_WARNING, "TipcMsgRead ReadChar [%c]", *vl_pc);
        }
		else
// A FINIR DE CODER
XZST_03EcritureTrace(XZSTC_WARNING, "FINIR DE CODER LES ARRAYS DE INT2/INT4/REAL8/STR vl_type non traite : %d", vl_type);		
	}
	va_end(vl_listeArgument);

	return (TRUE);
}

int TipcMsgGetType(amqp_envelope_t envelope, int *va_type) {
	amqp_basic_properties_t vl_properties = envelope.message.properties;
    char *vl_type = NULL;
	char vl_str_message_id[10]; 

	// Extraire message_id depuis l enveloppe
	if (vl_properties._flags & AMQP_BASIC_MESSAGE_ID_FLAG) 
	{
		strcpy(vl_str_message_id, (char*)(vl_properties.message_id.bytes));
	} else 
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "TipcMsgGetType : Type non defini");
		return XDC_NOK; 
	}

	// Conversion integer
	*va_type=atoi(vl_str_message_id);
	XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgGetType : Message Id int %d", *va_type);

    return XDC_OK;
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
int onMessage(amqp_envelope_t envelope) {
    amqp_basic_properties_t vl_properties = envelope.message.properties;
	amqp_basic_properties_t *vl_ptr_properties = &vl_properties;
	int vl_numero;
	int vl_index;
	int vl_type_message=0;
    char vl_str_message_id[10];
    int vl_cb_trouve = 0;
    char* vl_appId = NULL;
    char vl_sender[1024];
	char vl_buffer[1024];
    
	T_IPC_CONN_PROCESS_CB_DATA data=NULL;

	sprintf(vl_buffer,"%.*s", (int)envelope.exchange.len, (char*)envelope.exchange.bytes);
	if (!strcmp(vl_buffer,""))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"onMessage : Message recu depuis un topic NULL");
		return(XDC_OK);
	}

    XZST_03EcritureTrace(XZSTC_WARNING,"onMessage : topic = [%s]\n", vl_buffer);

	// Récupérer l'expéditeur
    TipcMsgGetSender(envelope, (T_STR*) vl_sender);
//    XZST_03EcritureTrace(XZSTC_WARNING,"Expediteur du message : [%s] Receptionnaire du message : [%s]", vl_sender, vg_appId);

	// Verifier si je suis l'expediteur
	if (!strcmp(vg_appId,vl_sender))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"onMessage : Je suis l'emetteur, je ne traite pas le message");
		return(XDC_OK);
	}

    // Vérification si le content type est bien défini
/*    if (vl_properties._flags & AMQP_BASIC_CONTENT_TYPE_FLAG) 
	{
        XZST_03EcritureTrace(XZSTC_WARNING,"onMessage : Content Type: %.*s", (int)vl_properties.content_type.len, (char*)vl_properties.content_type.bytes);
    } else 
	{
        XZST_03EcritureTrace(XZSTC_WARNING,"onMessage : Content Type non défini");
    }
*/    
	if (TipcMsgGetType(envelope, &vl_type_message)==XDC_OK)
	{
        XZST_03EcritureTrace(XZSTC_WARNING,"onMessage : Message Id : %d", vl_type_message);
        // Trouver la fonction de rappel basée sur vl_type_message
       	for (vl_index=0; vl_index < vm_nb_cb ; vl_index ++)
		{
            if (tg_cb[vl_index].Numero == vl_type_message) 
			{
                vl_cb_trouve = 1;
                break;
            }
        }
        if (!vl_cb_trouve) 
		{
            XZST_03EcritureTrace(XZSTC_WARNING,"onMessage : Aucune fonction de callback trouvee pour le type de message");
			return XDC_NOK;
        }

		// Si pas de propriete header a la reception du message on l'ajoute pour gerer le pointeur de lecture des champs
		if (!(vl_ptr_properties->_flags & AMQP_BASIC_HEADERS_FLAG))
		{
			vl_ptr_properties->_flags |= AMQP_BASIC_HEADERS_FLAG;
			amqp_table_t vl_header;
			amqp_table_entry_t vl_header_entries[1];

			vl_header_entries[0].key = amqp_cstring_bytes(PTR_CURRENT_FIELD);
			vl_header_entries[0].value.kind = AMQP_FIELD_KIND_I64;
			vl_header_entries[0].value.value.i64 = 0L;
			vl_header.num_entries=1;
			vl_header.entries = vl_header_entries;
			vl_ptr_properties->headers = vl_header;
		}

		/*appel du CB*/
		data=(T_IPC_CONN_PROCESS_CB_DATA) malloc(sizeof(T_IPC_CONN_PROCESS_CB_DATA_STRUCT));

		if (data==NULL) 
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"onMessage OUT : pb malloc data");
			return XDC_NOK;
		}
		data->msg = envelope;

		(*(tg_cb[vl_index].CB))((T_IPC_CONN)NULL, data, (T_CB_ARG)tg_cb[vl_index].CB_ARG);
		free(data);

    } 
	else 
	{
        XZST_03EcritureTrace(XZSTC_WARNING,"onMessage : Type de message non défini!\n");
    }

	return(XDC_OK);
}


const char *tibems_errorString(amqp_rpc_reply_t va_reply)
{
	static char vl_error_str[255];
    amqp_connection_close_t * vl_conn_close;
    amqp_channel_close_t *vl_chan_close;

    switch (va_reply.reply_type) 
	{
        case AMQP_RESPONSE_NORMAL:
			strcpy(vl_error_str,"");
            break;
        case AMQP_RESPONSE_NONE:
			strcpy(vl_error_str,"AMQP_RESPONSE_NONE");
			break;
        case AMQP_RESPONSE_LIBRARY_EXCEPTION:
			sprintf(vl_error_str,"%s %s","AMQP_RESPONSE_LIBRARY_EXCEPTION", amqp_error_string2(va_reply.library_error));
			break;
        case AMQP_RESPONSE_SERVER_EXCEPTION:
            switch (va_reply.reply.id) 
			{
                case AMQP_CONNECTION_CLOSE_METHOD: 
                    vl_conn_close = (amqp_connection_close_t *) va_reply.reply.decoded;
					sprintf(vl_error_str,
							"%s %d %.*s", 
							"AMQP_RESPONSE_SERVER_EXCEPTION / AMQP_CONNECTION_CLOSE_METHOD : ", 
							vl_conn_close->reply_code, 
							(int)vl_conn_close->reply_text.len, 
							(char*)vl_conn_close->reply_text.bytes
							);
                    break;

                case AMQP_CHANNEL_CLOSE_METHOD: // Cas canal ferme par le serveur.
                    vl_chan_close = (amqp_channel_close_t *) va_reply.reply.decoded;
					sprintf(vl_error_str,
							"%s %d %.*s", 
							"AMQP_RESPONSE_SERVER_EXCEPTION / AMQP_CHANNEL_CLOSE_METHOD : ", 
							vl_chan_close->reply_code, 
							(int)vl_chan_close->reply_text.len, 
							(char*)vl_chan_close->reply_text.bytes
							);
                    break;
                default:
					strcpy(vl_error_str,"EXCEPTION AMQP inattendue");
					break;
            }
        default:
            strcpy(vl_error_str,"EXCEPTION AMQP Serveur inattendue");
			break;
   }
	return(vl_error_str);
}


int TipcSrvFlush() 
{
	return (TRUE);
}

int TutErrNumGet()
{
	return (0);
}

double TutGetWallTime()
{
	time_t montime;
	double h;
	struct tm *t;
	
	time(&montime);
	t=localtime(&montime);
	 const char * strDate = asctime( localtime( & montime ) );
	h = (double)montime;
        return (h);
}

double TutGetCurrentTime()
{
        return (TutGetWallTime());
}

int TipcMtLookupByNum(int num)
{
        return (num);
}

T_IPC_MSG TipcSrvMsgNext(T_REAL8 timeout) 
{
	T_IPC_MSG vl_envelope;
	amqp_rpc_reply_t vl_result;
    struct timeval vl_timeout = {0, 500000}; // Délai d'attente de 500 millisecondes

    vl_result = amqp_consume_message(vg_conn, &vl_envelope, &vl_timeout, 0);
    if (vl_result.reply_type == AMQP_RESPONSE_NORMAL) {
        onMessage(vl_envelope);
    } else if (vl_result.reply_type == AMQP_RESPONSE_LIBRARY_EXCEPTION && vl_result.library_error == AMQP_STATUS_TIMEOUT) {
        // Aucun message disponible, attendre un certain temps spécifié par timeout
		XZST_03EcritureTrace(XZSTC_INFO,".");
        if (timeout != T_TIMEOUT_FOREVER) {
            // Convertir le temps en microsecondes
            T_REAL8 timeout_microseconds = timeout * 1000000;
            // Utiliser usleep pour attendre le temps spécifié
            usleep((useconds_t)timeout_microseconds);
        }
    } 
	else 
	{
        XZST_03EcritureTrace(XZSTC_WARNING,"TipcSrvMsgNext : Erreur lors de la recuperation du message. vl_result.reply_type = %d vl_result.library_error = %d",vl_result.reply_type,vl_result.library_error);
        XZST_03EcritureTrace(XZSTC_FATAL,"TipcSrvMsgNext : Perte de connexion avec le serveur rabbit");
    }

 	return vl_envelope;
}

void TipcSrvMsgProcess(T_IPC_MSG msg)
{
        return ;
}

int TipcMsgDestroy(T_IPC_MSG msg)   // msg c est enveloppe
{
    if (vg_conn == NULL)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"Erreur : Connexion invalide");
        return XDC_NOK;
    }

    // Détruire le message uniquement s'il est valide (non NULL)
    if (msg.message.body.bytes != NULL)
    {
        // Libérer la mémoire du contenu du message
        free(msg.message.body.bytes);

        // Marquer le message comme NULL
        msg.message.body.bytes = NULL;
        msg.message.body.len = 0;

        XZST_03EcritureTrace(XZSTC_WARNING,"Message detruit avec succes");
    }

	// Libération de la mémoire des en-têtes du message 
/*
    amqp_basic_properties_t vl_properties = msg.message.properties;
	amqp_basic_properties_t *vl_ptr_properties = &vl_properties;

	if (vl_ptr_properties != NULL) 
	{
		if (vl_ptr_properties->headers.num_entries > 0 && vl_ptr_properties->headers.entries != NULL) 
		{
			amqp_table_entry_t *vl_ptr_entries = &(vl_ptr_properties->headers.entries);
			for (size_t i = 0; i < vl_ptr_properties->headers.num_entries; i++) 
			{
				amqp_bytes_t *vl_ptr_key = &(vl_ptr_entries[i]->key);
XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgDestroy header.key=[%s]",vl_ptr_key);
				if (vl_ptr_key!=NULL)
					free(vl_ptr_key);
				if (vl_ptr_entries[i]->value!=NULL)
					free(vl_ptr_entries[i]->value);
			}
			if (vl_ptr_entries!=NULL)
				free(vl_ptr_entries);
		}
		if (vl_ptr_properties!=NULL)
			free(vl_ptr_properties);
	}
*/
    // Libérer la mémoire associée à l'enveloppe du message
    amqp_destroy_envelope(&msg);
	return XDC_OK;
}

void TutCommandParseFile(char *file)
{
        return ;
}

int TipcSrvDestroy(int param)
{
        return (T_TRUE);
}

int TutCommandParseStr(char *va_str)
{
	return (XDC_OK);
}

void TipcMonSubjectsWatch(XDY_FonctionInt va_FonctionUtilisateur_in)
{
	int 				vl_indice;
	int 				vl_topicExistSubscriber;

   	/*A
        **  test les parametres d'entree
        */
	XZST_03EcritureTrace(XZSTC_FONCTION,"TipcMonSubjectsWatch : IN");

	if ( va_FonctionUtilisateur_in == NULL)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"TipcMonSubjectsWatch : va_FonctionUtilisateur_in est nulle");
		return;
	}

	/*A on parcourt le tableau des topics a monitorer */
	for (vl_indice=0;vl_indice<vm_nb_dg_surv;vl_indice++)
	{
		vl_topicExistSubscriber=XZEMS_03IsTopic(tg_dg_surv[vl_indice]);
		if (vl_topicExistSubscriber == NULL)
		{
			XZST_03EcritureTrace(XZSTC_DEBUG1,"TipcMonSubjectsWatch : Watch [%s] - Pas d'abonne",tg_dg_surv[vl_indice]);
			if (tg_dg_surv_presence[vl_indice]==1)
					/* Appel de sp_status(vl_Dg,0) */
					(*va_FonctionUtilisateur_in)(tg_dg_surv[vl_indice],0);
			tg_dg_surv_presence[vl_indice]=0;
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_DEBUG1,"TipcMonSubjectsWatch : Watch [%s] - abonne(s) present(s)",tg_dg_surv[vl_indice]);
			if (tg_dg_surv_presence[vl_indice]==0)
				/* Appel de sp_status(vl_Dg,1) */
				(*va_FonctionUtilisateur_in)(tg_dg_surv[vl_indice],1);
			tg_dg_surv_presence[vl_indice]=1;
		}
	}
	XZST_03EcritureTrace(XZSTC_FONCTION,"TipcMonSubjectsWatch : OUT");
}

int TipcMonSubjectSubscribeSetWatch(char *name, T_BOOL va_param)
{
	int vl_indice;

	XZST_03EcritureTrace(XZSTC_INFO,"TipcMonSubjectSubscribeSetWatch name %s",name);
	/*recherche si cb deja cree*/
	for (vl_indice=0; vl_indice<vm_nb_dg_surv;vl_indice++) {
		if (!strcmp(tg_dg_surv[vl_indice], name)) {
			XZST_03EcritureTrace(XZSTC_INFO,"TipcMonSubjectSubscribeSetWatch deja surveille %s",name);
			return XDC_OK;
		}
	}

	/*ajout du CB dans le tableau*/
	if (vm_nb_dg_surv==0) {
		tg_dg_surv = (XDY_Datagroup *)malloc(sizeof(XDY_Datagroup));
		if (tg_dg_surv==NULL) 
		{
 			XZST_03EcritureTrace(XZSTC_WARNING,"erreur malloc tg_CB");
			return XDC_NOK;
		}	
		tg_dg_surv_presence = (int *)malloc(sizeof(int));
		if (tg_dg_surv_presence==NULL) 
		{
 			XZST_03EcritureTrace(XZSTC_WARNING,"erreur malloc tg_CB presence");
			return XDC_NOK;
		}
		/* Initialisation tableau presence du topic a absent */
		tg_dg_surv_presence[vm_nb_dg_surv] = 0;
		vm_nb_dg_surv ++;
	}
	else 
	{
		tg_dg_surv = (XDY_Datagroup *)realloc(tg_dg_surv,((size_t) vm_nb_dg_surv +1)*sizeof(XDY_Datagroup));
		if (tg_dg_surv==NULL) 
		{
 			XZST_03EcritureTrace(XZSTC_WARNING,"erreur realloc tg_CB");
			return XDC_NOK;
		}
		tg_dg_surv_presence = (int *)realloc(tg_dg_surv_presence,((size_t) vm_nb_dg_surv +1)*sizeof(int));
		if (tg_dg_surv_presence==NULL) 
		{
 			XZST_03EcritureTrace(XZSTC_WARNING,"erreur realloc tg_CB presence");
			return XDC_NOK;
		}
		/* Initialisation tableau presence du topic a absent */
		tg_dg_surv_presence[vm_nb_dg_surv] = 0;
		vm_nb_dg_surv ++;
	}	
	strcpy(tg_dg_surv[vm_nb_dg_surv-1],name);

    return XDC_OK;
}

int TipcSrvSetAutoFlushSize(int size)
{
	return (XDC_OK);
}

int TipcSrvDefaultCbCreate(T_IPC_CONN_PROCESS_CB_FUNC func, T_CB_ARG arg)
{
	return (XDC_OK);
}

int TipcMtCreate(char *va_nom, int va_numero, char*va_grammaire)
{
	return (XDC_OK);
}

int TipcSrvMsgWrite(char *va_topic, int va_num_msg, int flag, ...)
{

va_list					vl_listeArgument;
int						i				= 0;
int						vl_type;
int						vl_indice;
char					vl_str[11000];
int						taille;
char					vl_destination_name[255];
int         			vl_size;
amqp_basic_properties_t vl_props;
amqp_basic_properties_t *vl_ptr_props=&vl_props;
amqp_table_t 			vl_header;
amqp_table_entry_t 		vl_header_entries[1];
char					vl_message_id[512];
char					vl_correlation_id[512];
int 					vl_randomValue = rand();
struct 					timeval vl_tv;
time_t 					vl_currentTime;
char					vl_trace_message[15000];
int						vl_etat;
int						vl_retour_publish=0;
char					vl_expiration[15];

	// Déclaration des propriétés du message
	XZST_03EcritureTrace(XZSTC_WARNING,"TipcSrvMsgWrite IN : ExchangeName : %s", va_topic);

	memset(vl_trace_message,'\0',15000);
	
	if (!strcmp(va_topic,""))
		return(TRUE);

    if(XZEMS_03IsTopic(va_topic))
	// Si l'exchange existe, c'est qu'il y a au moins un consommateur
	{
		// Init du header des flags
		vl_ptr_props->_flags = AMQP_BASIC_HEADERS_FLAG;
		vl_header_entries[0].key = amqp_cstring_bytes(PTR_CURRENT_FIELD);
		vl_header_entries[0].value.kind = AMQP_FIELD_KIND_I64;
		vl_header_entries[0].value.value.i64 = 0L;
		vl_header.num_entries=1;
		vl_header.entries = vl_header_entries;
		vl_ptr_props->headers = vl_header;

        // Definition du mode de livraison du message
        vl_ptr_props->_flags |= AMQP_BASIC_DELIVERY_MODE_FLAG;
        vl_ptr_props->delivery_mode = AMQP_DELIVERY_NONPERSISTENT;

        // Definition du content_type du message
		vl_ptr_props->_flags |= AMQP_BASIC_CONTENT_TYPE_FLAG;
		vl_ptr_props->content_type = amqp_cstring_bytes(CONTENT_TYPE_TEXT_PLAIN);

		sprintf(vl_message_id,"%d",va_num_msg);
		XZST_03EcritureTrace(XZSTC_WARNING, "TipcSrvMsgWrite : Num message [%s]",vl_message_id);

        // Stockage message_id
        vl_ptr_props->_flags |= AMQP_BASIC_MESSAGE_ID_FLAG;
		vl_ptr_props->message_id = amqp_cstring_bytes(vl_message_id);

        // Définition de la durée d'expiration du message
		sprintf(vl_expiration,"%d",TTL_MS_MESSAGE_IN_QUEUE);
        vl_ptr_props->_flags |= AMQP_BASIC_EXPIRATION_FLAG;
        vl_ptr_props->expiration = amqp_cstring_bytes(vl_expiration); 

		// Definition de la propriete app_id
		vl_ptr_props->_flags |= AMQP_BASIC_APP_ID_FLAG; 
		vl_ptr_props->app_id = amqp_cstring_bytes(vg_appId);
		
        // Definition correlation Id
		gettimeofday(&vl_tv, NULL);
		vl_currentTime = (time_t)vl_tv.tv_sec;
	    snprintf(vl_correlation_id, sizeof(vl_correlation_id), "%s.%ld.%u", vg_appId, vl_currentTime, vl_randomValue);
	    vl_ptr_props->_flags |= AMQP_BASIC_CORRELATION_ID_FLAG;;
		vl_ptr_props->correlation_id = amqp_cstring_bytes(vl_correlation_id); 

		/*ajout des champs*/
		va_start(vl_listeArgument, flag);

		while (TRUE)
		{
			vl_type = va_arg(vl_listeArgument, int);
			if (vl_type == 0)
				break;
			if (vl_type == T_IPC_FT_INT4 ) 
			{
				int vl_pd = va_arg(vl_listeArgument, int);
				sprintf(vl_trace_message, "%s{%d}", vl_trace_message, vl_pd);
			}
			else if (vl_type == T_IPC_FT_INT2) 
			{
				int vl_psi = va_arg(vl_listeArgument, int);
				sprintf(vl_trace_message, "%s{%d}", vl_trace_message, vl_psi);
			}	
			else if (vl_type == T_IPC_FT_INT2_ARRAY)
			{
				short *vl_pst = va_arg(vl_listeArgument, short *);
                int vl_nb = va_arg(vl_listeArgument, int);
				// Ajout taille avant le tableau
				sprintf(vl_trace_message, "%s{%d}",vl_trace_message, vl_nb);
				for (vl_indice = 0; vl_indice < vl_nb; vl_indice++) {
					sprintf(vl_trace_message, "%s{%d}", vl_trace_message, vl_pst[vl_indice]);
				}
			}	
			else if (vl_type == T_IPC_FT_INT4_ARRAY) 
			{
				int *vl_psti = va_arg(vl_listeArgument, int *);
                int vl_nbi = va_arg(vl_listeArgument, int);

				// Ajout taille avant le tableau
				sprintf(vl_trace_message,"%s{%d}",vl_trace_message, vl_nbi);

                for (vl_indice = 0; vl_indice < vl_nbi; vl_indice++) {
                    sprintf(vl_trace_message, "%s{%d}",vl_trace_message, vl_psti[vl_indice]);
				}
			}
			else if (vl_type == T_IPC_FT_REAL8) 
			{
				double vl_pf = va_arg(vl_listeArgument, double);
				sprintf(vl_trace_message, "%s{%lf}",vl_trace_message, vl_pf);
			}
			else if (vl_type == T_IPC_FT_REAL8_ARRAY) 
			{
				double *vl_pstf = va_arg(vl_listeArgument, double *);
                int vl_nbf = va_arg(vl_listeArgument, int);     // Taille tableau
				// Ajout taille avant le tableau
				sprintf(vl_trace_message,"%s{%d}",vl_trace_message, vl_nbf);
                
				for (vl_indice = 0; vl_indice < vl_nbf; vl_indice++) {
					sprintf(vl_trace_message, "%s{%f}",vl_trace_message, vl_pstf[vl_indice]);
				}
			}
			else if (vl_type == T_IPC_FT_STR) 
			{
				char *vl_str = va_arg(vl_listeArgument, char *);
				if (vl_str != NULL)
				{
					sprintf(vl_trace_message, "%s{%s}", vl_trace_message, vl_str);
				}
			}
			else if (vl_type == T_IPC_FT_STR_ARRAY) 
			{
				char **vl_psts = va_arg(vl_listeArgument, char **); //  On récupère un tableau de pointeurs de chaînes de caractères (char **) à partir de la liste d'arguments variables
                int vl_nbs = va_arg(vl_listeArgument, int);       // Extrait nombre d'elements dans le tableau
                
				// Ajout taille avant le tableau
				sprintf(vl_trace_message, "%s{%d}", vl_trace_message, vl_nbs);

				for (vl_indice = 0; vl_indice < vl_nbs; vl_indice++) {
					sprintf(vl_trace_message, "%s{%s}", vl_trace_message, vl_psts[vl_indice]);
				}
			}
			else if (vl_type == T_IPC_FT_CHAR) 
			{
				char vl_pc = va_arg(vl_listeArgument, int);
				sprintf(vl_trace_message, "%s{%c}", vl_trace_message, vl_pc);// Concaténer la chaîne formatée.
			}
		}
		va_end(vl_listeArgument);

		// Publication du message
		vl_retour_publish = amqp_basic_publish(
												vg_conn,
												vg_amqp_channel,
												amqp_cstring_bytes(va_topic),
												amqp_cstring_bytes(""),
												0,
												0,
												vl_ptr_props,
												amqp_cstring_bytes(vl_trace_message));

		if (vl_retour_publish!=0)
			XZST_03EcritureTrace(XZSTC_WARNING,"TipcSrvMsgWrite : amqp_basic_publish failed. return %d", vl_retour_publish);

		vg_rpc_reply = amqp_get_rpc_reply(vg_conn);
		if (check_reply(vg_rpc_reply,"TipcSrvMsgWrite - amqp_basic_publish"))
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"TipcSrvMsgWrite : amqp_basic_publish failed - %s", tibems_errorString(vg_rpc_reply));
			return(FALSE);
		} 

		XZST_03EcritureTrace(XZSTC_WARNING, "Write [%d] : {%s}", va_num_msg, vl_trace_message);
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"TipcSrvMsgWrite : Exchange [%s] inexistant : pas de consommateur. Message non poste",va_topic);
		// ca n'est pas une erreur
        return (TRUE);
	}
	return(TRUE);
}

amqp_channel_t compute_channel_id(char *va_hostname,char *va_executableName)
{
	amqp_channel_t vl_amqp_channel;
//	int vl_offset_sous_systeme=0;
//	int vl_poids_

	// Determination du channel en fonction du thread
	vl_amqp_channel = 1;
	if (!strcmp(va_executableName,"tsaplg.x"))
		vl_amqp_channel = 2;
	if (!strcmp(va_executableName,"tsetat.x"))
		vl_amqp_channel = 3;
	if (!strcmp(va_executableName,"tstim.x"))
		vl_amqp_channel = 4;
	if (!strcmp(va_executableName,"tspng.x"))
		vl_amqp_channel = 5;
	if (!strcmp(va_executableName,"tasrv.x"))
		vl_amqp_channel = 6;
	if (!strcmp(va_executableName,"tdrol.x"))
		vl_amqp_channel = 7;
	if (!strcmp(va_executableName,"tacli.x"))
		vl_amqp_channel = 8;
	if (!strcmp(va_executableName,"tapur.x"))
		vl_amqp_channel = 9;
	if (!strcmp(va_executableName,"tesrv.x"))
		vl_amqp_channel = 10;
	if (!strcmp(va_executableName,"tesur.x"))
		vl_amqp_channel = 11;
	if (!strcmp(va_executableName,"titre.x"))
		vl_amqp_channel = 12;
	if (!strcmp(va_executableName,"tisse.x"))
		vl_amqp_channel = 13;
	if (!strcmp(va_executableName,"tetim.x"))
		vl_amqp_channel = 14;
	if (!strcmp(va_executableName,"teim2.x"))
		vl_amqp_channel = 15;
	if (!strcmp(va_executableName,"teim4.x"))
		vl_amqp_channel = 16;
	if (!strcmp(va_executableName,"teim12.x"))
		vl_amqp_channel = 17;
	if (!strcmp(va_executableName,"teim47.x"))
		vl_amqp_channel = 18;
	if (!strcmp(va_executableName,"teim69.x"))
		vl_amqp_channel = 19;
	if (!strcmp(va_executableName,"teini.x"))
		vl_amqp_channel = 20;
	if (!strcmp(va_executableName,"tepmv.x"))
		vl_amqp_channel = 21;
	if (!strcmp(va_executableName,"tedai.x"))
		vl_amqp_channel = 22;
	if (!strcmp(va_executableName,"terau.x"))
		vl_amqp_channel = 23;
	if (!strcmp(va_executableName,"tebaf.x"))
		vl_amqp_channel = 24;
	if (!strcmp(va_executableName,"tepic.x"))
		vl_amqp_channel = 25;
	if (!strcmp(va_executableName,"tcgcd.x"))
		vl_amqp_channel = 26;

	if (!strcmp(va_hostname,"SDHCA2D"))
		vl_amqp_channel+=100;

	return(vl_amqp_channel);
}

int XZEMS_02Connexion(char *va_URL)
{
   // Déclaration variables
    char			vl_hostname[100];
    amqp_socket_t* 	vl_socket= NULL;
	int 			vl_status;
	int				vl_indice;

	XZST_03EcritureTrace(XZSTC_INFO,"XZEMS_02Connexion Bus rabbitmq: IN");
	printf("XZEMS_02Connexion IN Rabbit\n");

	// Definition d'ID d'application
	setExecutableName();
	// Récupération du nom de la machine hôte
	if (checkGetHostName(vg_hostname, sizeof(vg_hostname))) 
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "XZEMS_02Connexion : Recuperation nom de la machine impossible.\n");
		strcpy(vl_hostname,"GHOST_MACHINE");
	}

	// Récupération du nom de l executable
	if(vg_executableName == NULL)                                  
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "XZEMS_02Connexion : Recuperation nom de la tache impossible.\n");
		strcpy(vg_executableName,"GHOST_TASK");
	}

	//Concatenation nom machine.nom executable       
	sprintf(vg_appId, "%s.%s", vg_hostname, vg_executableName);     

	vg_amqp_channel = compute_channel_id(vg_hostname,vg_executableName);
	printf("%s channel calcule pour %s : %d\n",vg_appId,vg_appId,vg_amqp_channel);
    // Création de la connexion RabbitMQ
    vg_conn = amqp_new_connection ();
    
    if (!vg_conn) {
		XZST_03EcritureTrace(XZSTC_FATAL,"XZEMS_02Connexion : amqp_new_connection failed");
		printf("XZEMS_02Connexion : amqp_new_connection failed\n");
        return XDC_NOK;
    }
	XZST_03EcritureTrace(XZSTC_INFO,"XZEMS_02Connexion : amqp_new_connection successful");
    
    // Création du socket TCP
    vl_socket = amqp_tcp_socket_new(vg_conn);
    if (!vl_socket) {
        //fprintf("Erreur lors de la création du socket TCP : %s\n", amqp_error_string2(vg_conn->err));
		XZST_03EcritureTrace(XZSTC_FATAL, "XZEMS_02Connexion : amqp_tcp_socket_new failed");
		printf("%s XZEMS_02Connexion : amqp_tcp_socket_new failed\n", vg_appId);
        return XDC_NOK;
    }

    // Configuration de l'URL complet
    sprintf(vl_hostname, "%s", va_URL);
	XZST_03EcritureTrace(XZSTC_DEBUG1, "XZEMS_02Connexion : Hostname du serveur rabbit : %s", vl_hostname);
 	printf("%s Hostname du serveur rabbit : %s\n", vg_appId, vl_hostname);

    // Ouverture du socket TCP
    vl_status = amqp_socket_open(vl_socket, vl_hostname, PORT);
	if (vl_status)
	{
		vg_rpc_reply = amqp_get_rpc_reply(vg_conn);
		if (check_reply(vg_rpc_reply, "XZEMS_02Connexion : amqp_socket_open failed")) 
		{
			printf("%s XZEMS_02Connexion : status KO %d avec hostname [%s]\n", vg_appId, vl_status, vl_hostname);

			// test localhost
			sprintf(vl_hostname, "%s", "localhost");
			printf("%s XZEMS_02Connexion : Hostname du serveur rabbit : %s\n", vg_appId, vl_hostname);

			// Ouverture du socket TCP
			vl_status = amqp_socket_open(vl_socket, vl_hostname, PORT);
			if (vl_status)
			{
				if (check_reply(vg_rpc_reply, "XZEMS_02Connexion : amqp_socket_open failed")) 
				{
					vg_rpc_reply = amqp_get_rpc_reply(vg_conn);
					printf("%s XZEMS_02Connexion : status KO %d avec hostname [%s]\n", vg_appId, vl_status, vl_hostname);
					return(XDC_NOK);
				}
			}
		}
    }

   	XZST_03EcritureTrace(XZSTC_INFO, "XZEMS_02Connexion : amqp_socket_open successful");

    // Configuration de la connexion RabbitMQ avec le socket
    // Connexion à RabbitMQ
    vg_rpc_reply = amqp_login(vg_conn, "/", 0, AMQP_DEFAULT_FRAME_SIZE, 0, AMQP_SASL_METHOD_PLAIN, USERNAME, PASSWORD);
	if (vg_rpc_reply.reply_type != AMQP_RESPONSE_NORMAL)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"XZEMS_02Connexion : amqp_login failed - %s", tibems_errorString(vg_rpc_reply));
		printf("%s amqp_login failed\n", vg_appId);
        return XDC_NOK;
    }
	amqp_channel_open_ok_t* vl_channel_open_ok;
    vl_channel_open_ok = amqp_channel_open(vg_conn, vg_amqp_channel);
    vg_rpc_reply = amqp_get_rpc_reply(vg_conn);
    if (check_reply(vg_rpc_reply, "XZEMS_02Connexion : amqp_channel_open failed")) 
	{
		printf("%s XZEMS_02Connexion : amqp_channel_open failed\n", vg_appId);
        return XDC_NOK;
    }


    XZST_03EcritureTrace(XZSTC_DEBUG1, "XZEMS_02Connexion : Connexion à RabbitMQ etablie - AppId = [%s]",vg_appId);
	printf("%s XZEMS_02Connexion : Connexion à RabbitMQ etablie - AppId = [%s]\n", vg_appId,vg_appId);
    return XDC_OK;
}

		
void TipcMsgGetSender(amqp_envelope_t msg,T_STR *vl_appId)
{
	char vl_buffer[1024];
	amqp_basic_properties_t *vl_props = &(msg.message.properties);
	const char *vl_bufptr = (const char *)vl_props->app_id.bytes;

    // Vérifier si la propriété app_id est définie
	strcpy(vl_appId,vl_bufptr);

	XZST_03EcritureTrace(XZSTC_DEBUG1,"TipcMsgGetSender retourne [%s]",vl_appId);
}

void TipcMsgGetDest(amqp_envelope_t envelope, T_STR *str)
{
    // Extraire nom d'echange
    char vl_dest[255];
    snprintf(vl_dest, sizeof(vl_dest), "%.*s", (int)envelope.exchange.len, (char *)envelope.exchange.bytes);

    XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgGetDest retourne [%s]",vl_dest);
    // copier le dest
    strcpy(*str, vl_dest);
}


// Fonction pour verifier la reponse de la procedure a distance
int check_reply(amqp_rpc_reply_t va_reply, char* va_error_msg) {
    amqp_connection_close_t * vl_conn_close;
    amqp_channel_close_t *vl_chan_close;

    switch (va_reply.reply_type) {
        case AMQP_RESPONSE_NORMAL:
            // Cas traitement normale, aucune action.
            break;
        case AMQP_RESPONSE_NONE:
			// On peut pas tracer en FATAL, car cela necessite le bus de message pour envoyer un message
            //XZST_03EcritureTrace(XZSTC_FATAL, "XZEMS_02Connexion : %s\n", va_error_msg);
            XZST_03EcritureTrace(XZSTC_WARNING, "check_reply : %s", va_error_msg);
			// on force le suicide de la tache
			exit(1);
            return (XDC_NOK);

        case AMQP_RESPONSE_LIBRARY_EXCEPTION:
//            XZST_03EcritureTrace(XZSTC_FATAL, "Erreur de la bibliothèque RabbitMQ-C : %s\n", amqp_error_string2(va_reply.library_error));
            XZST_03EcritureTrace(XZSTC_WARNING, "Erreur de la bibliothèque RabbitMQ-C : %s\n", amqp_error_string2(va_reply.library_error));
            return (XDC_NOK);

         case AMQP_RESPONSE_SERVER_EXCEPTION:
            switch (va_reply.reply.id) {
                case AMQP_CONNECTION_CLOSE_METHOD: // Cas connexion fermee par le serveur.
                    vl_conn_close = (amqp_connection_close_t *) va_reply.reply.decoded;
                    printf ("%s - %s : Connexion fermée par le serveur : code %d, message : %.*s\n",
                            vg_appId, va_error_msg, vl_conn_close->reply_code, (int)vl_conn_close->reply_text.len, (char*)vl_conn_close->reply_text.bytes);
//                    XZST_03EcritureTrace(XZSTC_FATAL, "%s : Connexion fermée par le serveur : code %d, message : %.*s\n",
//                            va_error_msg, vl_conn_close->reply_code, (int)vl_conn_close->reply_text.len, (char*)vl_conn_close->reply_text.bytes);
                    XZST_03EcritureTrace(XZSTC_WARNING, "%s : Connexion fermée par le serveur : [%d] [%.*s]\n",
                            va_error_msg, vl_conn_close->reply_code, (int)vl_conn_close->reply_text.len, (char*)vl_conn_close->reply_text.bytes);
                    break;

                case AMQP_CHANNEL_CLOSE_METHOD: // Cas canal ferme par le serveur.
                    vl_chan_close = (amqp_channel_close_t *) va_reply.reply.decoded;
//                    XZST_03EcritureTrace(XZSTC_FATAL, "%s : Canal ferme par le serveur : code %d, message : %.*s\n",
//                            va_error_msg, vl_chan_close->reply_code, (int)vl_chan_close->reply_text.len, (char*)vl_chan_close->reply_text.bytes);
                    XZST_03EcritureTrace(XZSTC_WARNING, "%s : Canal ferme par le serveur : [%d] [%.*s]\n",
                            va_error_msg, vl_chan_close->reply_code, (int)vl_chan_close->reply_text.len, (char*)vl_chan_close->reply_text.bytes);
                    printf ("%s - %s : Canal ferme par le serveur : code %d, message : %.*s\n",
                            vg_appId, va_error_msg, vl_chan_close->reply_code, (int)vl_chan_close->reply_text.len, (char*)vl_chan_close->reply_text.bytes);
                    break;

                default:
//                    XZST_03EcritureTrace(XZSTC_FATAL, "%s : Reponse d'exception inattendue du serveur\n", va_error_msg);
                    XZST_03EcritureTrace(XZSTC_WARNING, "%s : Reponse d'exception inattendue du serveur\n", va_error_msg);
                    printf ("%s - %s : Reponse d'exception inattendue du serveur\n", vg_appId, va_error_msg);
					break;
            }
            return (XDC_NOK);
        default:
//            XZST_03EcritureTrace(XZSTC_FATAL, "%s : Reponse inattendue du serveur\n", va_error_msg);
            printf ("%s - %s : Reponse inattendue du serveur\n", vg_appId, va_error_msg);
				return (XDC_NOK);
    }
    return (XDC_OK);
}

int deconnectionRabbitmq(){
  // Fermeture de la connexion
    amqp_channel_close(vg_conn, vg_amqp_channel, AMQP_REPLY_SUCCESS);
	vg_rpc_reply = amqp_get_rpc_reply(vg_conn);
	if (vg_rpc_reply.reply_type != AMQP_RESPONSE_NORMAL) 
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"deconnectionRabbitmq : Fermeture canal a echoue! - %s", tibems_errorString(vg_rpc_reply));
        return (XDC_NOK);
    }

	handle_channel_close(vg_amqp_channel);

    amqp_connection_close(vg_conn, AMQP_REPLY_SUCCESS);
	vg_rpc_reply = amqp_get_rpc_reply(vg_conn);
	if (vg_rpc_reply.reply_type != AMQP_RESPONSE_NORMAL) 
	{
  		XZST_03EcritureTrace(XZSTC_FATAL,"deconnectionRabbitmq : Fermeture connexion a echoue! - %s", tibems_errorString(vg_rpc_reply));
        return (XDC_NOK);
    }

    amqp_destroy_connection(vg_conn);
	vg_rpc_reply = amqp_get_rpc_reply(vg_conn);
	if (vg_rpc_reply.reply_type != AMQP_RESPONSE_NORMAL) 
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"deconnectionRabbitmq : Destruction connexion a echoue! - %s", tibems_errorString(vg_rpc_reply));
        return (XDC_NOK);
    }
    XZST_03EcritureTrace(XZSTC_FATAL,"deconnectionRabbitmq : Vous êtes deconnecté de Rabbitmq!\n");
    return (XDC_OK);
}

#endif

//#endif
