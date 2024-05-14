/*E*/  
/*Fichier : $Id: tsinq_dir.c,v 1.12 2011/01/13 09:49:12 gesconf Exp $	      Release : $Revision: 1.12 $        Date : $Date: 2011/01/13 09:49:12 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  SUPERV
------------------------------------------------------
* MODULE TSINQ * FICHIER tsinq_dir.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Permet de transmettre les donnees relatives aux chnagements
*	d'etats des taches a la base de Donnee afin de calculer
*	l'indice qualite 
*
------------------------------------------------------
* HISTORIQUE :
*
* T.Milleville	12 Jan 1995	: Creation
* T.Milleville	24 Fev 1995	: Modification Abonnemen Datagroup NSUPINDQUA
*					V  
* T.Milleville	10 Mar 1995	: Modification pour SCCS V1.5
* T.Milleville	4 Mai  1995	: Recuperation du Site pour 
*	creer le Datagroup NSUPINDQUA_NomSite V1.6
* T.Milleville	30 Juin  1995	: Modification de la boucle de traitement RTWorks V1.7
* T.Milleville	25 Jul  1995	: Ajout de commentaires et de la constante XDC_EXIT_RTCLIENT V1.8
* T.Milleville	27 Sep  1995	: Suppression Attente lors d'une erreur RTWorks  V1.9
* Niepceron	02 Sep 1998	: connexion a SQL_HIS (dem/1697) 1.10
* JPL		13/01/11	: Migration architecture HP ia64 (DEM 971) : exit sur echec de l'initialisation  1.12
------------------------------------------------------*/

/* fichiers inclus */

#include "tsinq_dir.h"

#include <xzsem.h>
#include "xdc.h"
#include "xdm.h"
#include "xdg.h"
#include "xzag.h"
#include "xzsc.h"
#include "xzst.h"
#include "xzcg.h"
#include "xzaq.h"
#include "xzsa.h"


/* definitions de constantes */

/* definitions de types locaux */
typedef struct {
	int NumMsg;      /* Numero message RTW */
	void (*FoncCB)(); /* Fonction callback */
				} tm_callback;


/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: tsinq_dir.c,v 1.12 2011/01/13 09:49:12 gesconf Exp $ : tsinq_dir" ;

tm_callback pm_TabCB[] = {
	{ XDM_TSE_INDQUA, (void (*)())tsinq_indice_cb } };


/* declaration de fonctions internes */
int tsinq_init();


/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction principale de la tache tsinq.x
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
*	Abonnement au datagroup, armement des callbacks et attente infinie
*	des msg RTworks
*
------------------------------------------------------*/
{
	static char *version="$Id: tsinq_dir.c,v 1.12 2011/01/13 09:49:12 gesconf Exp $ : main " ;
	int 		vl_ValRet	= XDC_OK;
	int             vl_nCB          = 0;
	T_IPC_MT        vl_mt           = 0;
	XZSCT_Datagroup	pl_NomDatagroup	= "";
	XZSCT_NomSite	vl_NomSite	= "";
	T_IPC_MSG               pl_Msg;
	int                     vl_IndPre       = 0;
	int                     vl_Attente       = 0;



	XZST_03EcritureTrace(XZSTC_FONCTION," Main tsinq_dir : IN \n");
	
	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

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
	  	exit(1);
	}


	/*A Initialisation des donnees */
	XZSA_20InitAdmin();

	/*A Recuperation du Nom du Site */
	XZSC_04NomSite(vl_NomSite);

	/*A Abonnement au DataGroup XDG_NSUPINDQUA_NomSite */
      	XDG_EncodeDG2(pl_NomDatagroup, XDG_NSUPINDQUA,vl_NomSite);
      	if (!XDG_JoindreDG(pl_NomDatagroup))
      	{
         	/*A erreur abonnement */
         	XZST_03EcritureTrace(XZSTC_WARNING,
                      "WARNING : erreur abonnement au DG %s", pl_NomDatagroup);
         	vl_ValRet = XDC_NOK;
      	}


	/*A Initialisation des callbacks de la tache */
	for (vl_nCB = 0;
		vl_nCB < (sizeof(pm_TabCB)/sizeof(pm_TabCB[0])); vl_nCB++)
	{
		vl_mt = XDM_IdentMsg(pm_TabCB[vl_nCB].NumMsg);
		if(!TipcSrvProcessCbCreate(vl_mt,
			pm_TabCB[vl_nCB].FoncCB, (T_CB_ARG)NULL))
		{
			XZST_03EcritureTrace(XZSTC_WARNING,
		   "main : Erreur armement du callback %d",vl_nCB);
		}
	}

	/*A Initialisation du Processus */
	tsinq_init();


	/*A Attente bloquante des messages du RTServer */
	for (;;)
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
#if 0
			sleep(vl_Attente);
#endif
		}
	}

}	/* Fin main */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet d'initialiser le process
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int tsinq_init()

/*
* ARGUMENTS EN ENTREE :
*   Aucin
*
*
* ARGUMENTS EN SORTIE :
*   Aucune
*
*
* CODE RETOUR : 
* 	XDC_OK :: ok
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Connexion a la base de donnees
*
------------------------------------------------------*/
{
	static char *version="$Id: tsinq_dir.c,v 1.12 2011/01/13 09:49:12 gesconf Exp $ : tsinq_init " ;
	XZSCT_NomSite	pl_NomSite	= "";
	int	vl_ValRet	= XDC_OK;

	XZST_03EcritureTrace(XZSTC_FONCTION," tsinq_init : IN \n");

	/*A Recuperation du Nom du Site */
	if ( (vl_ValRet = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		/*B ** Ecriture Trace */
		XZST_03EcritureTrace( XZSTC_FATAL, "TSINQ : Recuperation du Nom du site impossible .");

		exit(1);
	}


	/*A Connexion a la Base de Donnees */
	if ( (vl_ValRet = XZAG02_Connexion_Base("HIS")) != XDC_OK)
	{
		/*B ** Ecriture Trace */
		XZST_03EcritureTrace( XZSTC_FATAL, "TSINQ : Connexion au serveur SQL de %s impossible .",pl_NomSite);
		exit(1);
	}
	else
	{
	 	XZST_03EcritureTrace( XZSTC_DEBUG1, "TSINQ : Connexion au serveur SQL de %s reussie.",pl_NomSite);
	}

	XZST_03EcritureTrace(XZSTC_FONCTION," tsinq_init : OUT \n");

	return (vl_ValRet);
}	/* Fin tsinq_init */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de transmettre le(s) etat(s) a la base de donnees
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void tsinq_indice_cb(
	T_IPC_CONN va_Conn,
	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	T_CB_ARG pa_Arg)

/*
* ARGUMENTS EN ENTREE :
*   Arguments d'une callback
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   <Aucun
*
* CONDITION D'UTILISATION
*  Declenchee sur reception du msg XDM_TSE_INDQUA 
*
* FONCTION 
*   Recupere les informations (Nom_Site,Nom_Machine, Nom_Tache,
*	Etat_Tache) et les transmet a la base de donnees
*
------------------------------------------------------*/
{
	static char *version="$Id: tsinq_dir.c,v 1.12 2011/01/13 09:49:12 gesconf Exp $ : tsinq_indice_cb " ;
	XDY_Texte		vl_NomSite	= "";
	XDY_Texte		vl_NomTache	= "";
	XDY_Texte		vl_NomMachine	= "";
	XDY_Booleen		vl_EtatTache	= 0;
#ifndef _TIBCO_EMS
	T_STR			vl_NomSiteAux	= "";
	T_STR			vl_NomMachineAux	= "";
	T_STR			vl_NomTacheAux	= "";
	T_INT4			vl_EtatTacheAux	= 0;
#else
	XDY_Texte		vl_NomSiteAux	= "";
	XDY_Texte		vl_NomMachineAux	= "";
	XDY_Texte		vl_NomTacheAux	= "";
	XDY_Entier		vl_EtatTacheAux	= 0;
#endif
	int		vl_ValRet 	= XDC_OK;



	XZST_03EcritureTrace(XZSTC_FONCTION," tsinq_indice_cb: IN \n");

	/*A Recuperation des donnnees du Mesg */
	if (!TipcMsgRead(pa_Data -> msg,
		T_IPC_FT_STR, &vl_NomSiteAux,
		T_IPC_FT_STR, &vl_NomMachineAux,
		T_IPC_FT_STR, &vl_NomTacheAux,
	    T_IPC_FT_INT4, &vl_EtatTacheAux,
	     	NULL) )
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
			"tsinq_indice_cb - pb lecture message");
		 return;
	}

	XZST_03EcritureTrace(XZSTC_DEBUG1," TRansmission des donnees \
		pour l'indice qualite \n");

	/*A Memorisation des donnees recues */
	strcpy(vl_NomSite, vl_NomSiteAux);
	strcpy(vl_NomMachine,vl_NomMachineAux);
	strcpy(vl_NomTache,vl_NomTacheAux);
	vl_EtatTache = (XDY_Booleen)vl_EtatTacheAux;

	XZST_03EcritureTrace(XZSTC_DEBUG3,"Transmission des donnees \
		-- Nom Site %s -- Nom Mach %s \
		-- Nom tache %s -- Etat tache %d \n",vl_NomSiteAux,
		vl_NomMachineAux,vl_NomTacheAux,vl_EtatTacheAux);

	XZST_03EcritureTrace(XZSTC_DEBUG3,"Transmission des donnees \
		-- Nom Site %s -- Nom Mach %s \
		-- Nom tache %s -- Etat tache %d \n",vl_NomSite,
		vl_NomMachine,vl_NomTache,vl_EtatTache);


	/*A Verification Tache Permanente */
	vl_ValRet = XZSA_15TacheTempMachine(vl_NomMachineAux,vl_NomTacheAux);
	if (vl_ValRet == XZSAC_TYPE_TPER)
	{
		/*A Transmission des donnees a la Base */
		vl_ValRet = XZAQ01_Mise_A_Jour_Etat(
					vl_NomSite, vl_NomMachine,
					vl_NomTache,vl_EtatTache);
		if (vl_ValRet != XDC_OK)
		{
			/*A Trace Erreur */
			XZST_03EcritureTrace(XZSTC_WARNING,"Pb Majj de l'indice \
			qualite -- Site %s -- Machine %s - Tache %s \
			Etat Tache %d \n",vl_NomSite,vl_NomMachine,
			vl_NomTache,vl_EtatTache);
		}

	}

	XZST_03EcritureTrace(XZSTC_FONCTION," tsinq_indice_cb: OUT \n");


}	/* Fin tsinq_indice_cb */


