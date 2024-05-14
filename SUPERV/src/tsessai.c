/*E*/  
/*Fichier : @(#)tsdis_dir.c	1.11	      Release : 1.11        Date : 07/25/95
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  SUPERV
------------------------------------------------------
* MODULE TSDIS * FICHIER tsdis_dir.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Permet de tester cycliquement si un disque Miroir 
*	est arrete 
*
------------------------------------------------------
* HISTORIQUE :
*
* T.Milleville	12 Jan 1995	: Creation
* T.Milleville	06 Mar 1995	: Verification des pertes memoires V1.5
* T.Milleville	10 Mar 1995	: Modification pour SCCS V1.6
* T.Milleville	12 Mai 1995	: Ajout initialisation des Timers V1.7
* T.Milleville	12 Juin 1995	: Modification de la fermeture du fixhier
*	lors de la commande de verification dataserver V1.8
* T.Milleville	30 Juin 1995	: Modification de la boucle de traitement
*	des msg RTworks V1.9
* T.Milleville	20 Jul 1995	: Ajout commentaires V1.10
* T.Milleville	25 Jul 1995	: Ajout commentaires et de la constante
*	XDC_EXIT_RTCLIENT V1.11
*
------------------------------------------------------*/

/* fichiers inclus */


#include "xdc.h"
#include "xdl.h"
#include "xdm.h"
#include "xdg.h"
#include "xzsc.h"
#include "xzst.h"
#include "xzsm.h"
#include "xzss.h"
#include "xzse.h"


/* definitions de constantes */




/* definition de macro locales */

/* declaration de variables locales */


/*tm_callback pm_TabCB[] = { };*/


	/* Pointeur de memorisation des Volumes Groupes */

	/* Nombre de Volumes groupes */

	/* Pointeur de memorisation des Disques Physiques */

	/* Nombre de disques physiques */


	/* Nom du Site Local */



/* declaration de fonctions internes */



/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction principale de la tache tsdis.x 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

main(int argc, char *argv[])

/*
* ARGUMENTS EN ENTREE :
*   argc	: Nb arguments
*   argv        : Valeurs des arguments
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Connexion au Noyau de Supervision, puis a la base de donnees
*	Abonnement au datagroup, armement des callbacks 
*	Decelenchement des Timers pour tester les disques miroirs
*	et la presence du Dataserver
*
------------------------------------------------------*/
{
	static char *version="@(#)tsdis_dir.c	1.11 07/25/95 : main " ;
	int 		vl_ValRet	= XDC_OK;
	int             vl_nCB          = 0;
	T_IPC_MT       	vl_mt           = 0;
	XZSCT_Datagroup	pl_DataGroup	= "";
	XZSCT_NomMachine	vl_NomMachine	= "";
	int                     vl_IndPre       = 0;
	T_IPC_MSG               pl_Msg;
	int			vl_Attente	= 0;



	XZST_03EcritureTrace(XZSTC_FONCTION," Main tsdis_dir : IN \n");
	
	/*A connexion au noyau de supervision */
	vl_ValRet = XZSC_01InitCnxSup(argv[1]
	 	,argv[0]
		,XZSCC_INTERDIRE /* mise a jour horodate*/
		,XZSCC_AUTORISER /* systeme trace */
		,XZSCC_INTERDIRE /* synchro tsetat */
		,(void (*)())NULL /* fontion de sortie */
		,(char *)NULL     /* redirection sortie standard */
		);

	if (vl_ValRet != XDC_OK)
	{
	  	XZST_03EcritureTrace(XZSTC_FATAL,
	  		"erreur XZSC_01InitCnxSup cr = %d",vl_ValRet);
	  	exit;
	}


	/*A Attente bloquante des messages du RTServer */
	while(1)
	{
		 XZST_03EcritureTrace (XZSTC_WARNING, "Attente infinie d'un message \n");

		/* attente infinie d'un message */
		pl_Msg = TipcSrvMsgNext (T_TIMEOUT_FOREVER);

		/* si message recu */
		if(pl_Msg != NULL)
		{
			/* execution du call-back associe au message et destruction du message */
			TipcSrvMsgProcess(pl_Msg);
			TipcMsgDestroy(pl_Msg);

			/* Memorisation Test Presence RTserver */
			vl_IndPre = 0;
		}
		else
		/* erreur RTWORKS */
		{
			/* Test de Presence RTserver */
			if (vl_IndPre == 0)
			{
				if (errno)
					XZST_03EcritureTrace (XZSTC_WARNING, "Erreur RTWorks num: %d -- errno %d \n",
							TutErrNumGet(), errno);

			}
			else if (vl_IndPre >= XDC_EXIT_RTCLIENT)
			{
				XZST_03EcritureTrace (XZSTC_WARNING, "Bouclage et arret tache tsdis %d \n",vl_IndPre);
				/*A Arret de la tache */
				exit(0);
			}

			/*B INdication Non Presence RTserver */
			if (errno)
				vl_IndPre++;

			/*B Attente aleatoire pour permettre une reconnexion */
			vl_Attente = rand()%15;
			sleep(vl_Attente);
		}
	}


	XZST_03EcritureTrace(XZSTC_FONCTION," Main tsdis_dir : IN \n");


}	/* Fin main */

