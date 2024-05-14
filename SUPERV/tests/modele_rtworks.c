/*
*
*	Modele de fichier permettant de se connecter 
*	au RTSERVER par l'intermediaire des modules d'interface
*	XZSC et plus particulierement XZSC_01....
*	Ainsi que les interfaces d'ecriture de traces
*
************************************************************/ 

#include <stdio.h>

#include "xdc.h"
#include "xdg.h"
#include "xdm.h"
#include "xzsm.h"
#include "xzsc.h"
#include "xzst.h"

#include        <rtworks/common.h>
#include        <rtworks/ipc_proto.h>
#include        <rtworks/ipc.h>


void Modele_Callback(
	T_IPC_CONN                      va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
	T_CB_ARG                        pa_Arg)
{

	int 	vl_ModeEmis;
	XZSCT_Datagroup      	vl_DataGroup  = "\0";
   	int                  	vl_ValRet       = XDC_OK;
	XZSCT_NomMachine	vl_NomMachine = "\0";



	XZST_03EcritureTrace(XZSTC_FONCTION, " Callback Modele IN ");




	/** Recuperation du Nom de la Machine sur laquelle 
	*	execute la Tache  
	********/
	vl_ValRet = XZSC_07NomMachine(vl_NomMachine);
	if (vl_ValRet != XDC_OK)
	{
		/** Ecriture d'une Trcae 
		* Attention le premier parametre correspond
		*	a un niveau de traces. Pour plus de details
		* 	a ce sujet, contacter la personne adequate
		******/
		XZST_03EcritureTrace(XZSTC_WARNING,
			"Recuperation NomMachine a echouee %d \n",
				vl_ValRet);
	}

	/**** Declaration d'un DataGroup et on se joint a celui-ci  
	*	Ici on construit le Datagroup NSUPTIME_NomMachine  **/
	XDG_EncodeDG2 (vl_DataGroup, XDG_NSUPTIME, vl_NomMachine);


	/*** Modele d'envoi de Message 
	* envoie du message XDM_TSH_MODE */
	if (!TipcSrvMsgWrite(vl_DataGroup ,XDM_IdentMsg(XDM_TSH_MODE)
			       ,XDM_FLG_SRVMSGWRITE
			       ,T_IPC_FT_INT4,vl_ModeEmis,
			       NULL) )
       {
	       /* Affichage Trace */
       		XZST_03EcritureTrace(XZSTC_WARNING
	,"Modele_Callback  Envoi Message (%s,%u,XDM_FLG_SRVMSGWRIT E,,%u)"
		,vl_DataGroup ,XDM_IdentMsg(XDM_TSH_MODE) ,XZSMC_SYSTE) ;

	}

}	/* Fin Modele Callback  **/





/**********
*
*	Programme principal pour se connecter au RTSERVER 
*
***********/

main (int argc,char * argv[])

{
	XZSCT_Datagroup      	vl_DataGroup  = "\0";
   	int                  	vl_ValRet       = XDC_OK;
	XZSCT_NomMachine	vl_NomMachine = "\0";



	/****  Connexion au RTServer ***/
	/*   Les arguments correspondent en bref 
	*	argv[1]  -> Nom de l'appli (Par defaut migrazur )
	*	argv[0] ->  Nom de la tache 
	* 	XZSCC_INTERDIRE -> Permet de mettre a jour la date 
	*	XZSCC_AUTORISER -> Permet d'utilier les traces
	*	XZSCC_INTERDIRE -> Permet de se synchroniser a SUPERV
	*	NULL 	-> Fonction de sortie de la tache (XDM_TSP_EXIT) 
	*	"/dev/null" -> Fichier de redirection de la sortie standart
	*
	******/
	XZSC_01InitCnxSup (argv[1],argv[0],
			XZSCC_INTERDIRE,
			XZSCC_INTERDIRE,
			XZSCC_INTERDIRE,
			NULL,"/dev/null");


	/** Recuperation du Nom de la Machine sur laquelle 
	*	execute la Tache  
	********/
	vl_ValRet = XZSC_07NomMachine(vl_NomMachine);
	if (vl_ValRet != XDC_OK)
	{
		/** Ecriture d'une Trcae 
		* Attention le premier parametre correspond
		*	a un niveau de traces. Pour plus de details
		* 	a ce sujet, contacter la personne adequate
		******/
		XZST_03EcritureTrace(XZSTC_WARNING,
			"Recuperation NomMachine a echouee %d \n",
				vl_ValRet);
	}

	/**** Declaration d'un DataGroup et on se joint a celui-ci  
	*	Ici on construit le Datagroup NSUPTIME_NomMachine  **/
	XDG_EncodeDG2 (vl_DataGroup, XDG_NSUPTIME, vl_NomMachine);

	if ( XDG_JoindreDG(vl_DataGroup) != T_TRUE)
	{
		vl_ValRet == XZSCC_RTW_INV;
	}


	/** Declaration d'une Callback 
	*	Sur reception du Message XDM_TSA_ADMI   ***/
	if (TipcSrvProcessCbCreate(TipcMtLookupByNum(XDM_TSA_ADMI),
			 (T_IPC_CONN_PROCESS_CB_FUNC) Modele_Callback,
				       (T_CB_ARG) NULL) == T_FALSE)
	{
		/* erreur RTWorks */
		vl_ValRet = XZSCC_RTW_INV;
	}


	/***** Boucle de Reception et de traitement des Messages 
	*	RTWORKS 	***/
	for (;;)
	{
		if (!TipcSrvMainLoop(T_TIMEOUT_FOREVER)
					 != T_ERR_CONN_EOF)
		 {
		 	/* Affichage trace */
			XZST_03EcritureTrace(XZSTC_FATAL
					 ,"TipcSrvMainLoop a echoue  ");

			 break;    /* for */
		 }
	 }


}	/* fin main */
