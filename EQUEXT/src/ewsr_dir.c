/*E*/
/*Fichier :  $Id: ewsr_dir.c,v 1.6 2020/10/02 14:47:53 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2020/10/02 14:47:53 $ ------------------------------------------------------
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TESWT * FICHIER ewsr_dir.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* C'est le module directeur de la tache EWSR
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	    11/04/22    creation DEM-SAE380
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>
#include "xzsm.h"
#include "xzsc.h"
#include "xzems.h"

#include "ewsr_dir.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */
void    ec_sortir       (  );

/* declaration de variables locales */

void main( argc, argv )

/*
* ARGUMENTS EN ENTREE :
*   argc : Nombre d'arguments
*   argv : liste des arguments
*
* ARGUMENTS EN SORTIE :
*   Aucune
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*  
* Initialisations : Init du callback provenant de l'ihm
* Boucle sur les sockets et gestion des messages recus 
* 
* 
------------------------------------------------------*/

int	argc;
char	**argv;
{
	int             vl_IndPre       = 0;
	int             vl_cr   = XDC_OK;
	T_IPC_MSG       pl_Msg;

	int 			vl_ValRet	=XDC_OK;
    
	XZSCT_NomTache	vl_NomTache="";
    XZSCT_NomSite   vl_NomSite="";
	    
	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

	/*A
	 * Connexion au RTserver et initialisation des variables
	 */
	if( XZSC_01InitCnxSup(   argv[1],
				argv[ 0 ],
				XZSCC_INTERDIRE,
				XZSCC_AUTORISER,
				XZSCC_INTERDIRE,
				ec_sortir,
				"/dev/null" ) != XDC_OK )
	{
	   /*B Affichage Trace */
	   XZST_03EcritureTrace( XZSTC_FATAL, "Ne peut ouvrir la connexion avec le RTserver." );
	   exit( XDC_NOK );
	}
	else
	{
	   /*B Affichage Trace */
	   XZST_03EcritureTrace( XZSTC_DEBUG1, "Ouverture de la connexion avec le RTserver." );
	}

    /* force le niveau de la trace */
	XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE,XZSTC_NECRAN);
	
	/* Init de l'etat de l'alarme */
	vg_etat_alarme = XDC_FAUX;

	XZSC_07NomMachine(vg_NomMachine);
	XZSC_06NomTache(vl_NomTache);

	/*A
	 * Recuperation du site de la machine 
	 */
	if ( (vl_ValRet = XZSC_04NomSite(vl_NomSite)) != XDC_OK)
	{
	   /* Ecriture Trace */
           XZST_03EcritureTrace( XZSTC_FATAL, "tewsr_dir Recuperation du Nom du site impossible . " );
	} 

	/* abonnement data group */
	if (! XDG_JoindreDG(XDG_API_GW_RAD) ) //Pour le message vers la gateway
	{
		/* Ecriture Trace */
		XZST_03EcritureTrace( XZSTC_WARNING,"tewsr_dir : Abonnement sur %s non effectue. \n " ,XDG_API_GW_RAD);
		return;
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_INFO,"tewsr_dir : Abonnement sur %s effectue. \n " ,XDG_API_GW_RAD);
	}

	if (! XDG_JoindreDG(XDG_ECMD_AUTORAD) ) //Pour le message depuis l'ihm
	{
		/* Ecriture Trace */
		XZST_03EcritureTrace( XZSTC_WARNING,"tewsr_dir : Abonnement sur %s non effectue. \n " ,XDG_ECMD_AUTORAD);
		return;
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_INFO,"tewsr_dir : Abonnement sur %s effectue. \n " ,XDG_ECMD_AUTORAD);
	}

    /*
    * Armement des Callback 
    */
    if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_ECMD_AUTORAD),
								(T_IPC_CONN_PROCESS_CB_FUNC) cb_tewsr_appel_radio,
								(T_CB_ARG) NULL) == NULL)
    {
        XZST_03EcritureTrace(XZSTC_FATAL,"tewsr_dir : probleme lors de la creation du callback de reception de XDM_ECMD_AUTORAD");
        exit (1);
    }	

    if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_GTW_RAD_RETOUR_APPEL),
								(T_IPC_CONN_PROCESS_CB_FUNC) cb_tewsr_retour_appel,
								(T_CB_ARG) NULL) == NULL)
    {
        XZST_03EcritureTrace(XZSTC_FATAL,"tewsr_dir : probleme lors de la creation du callback de reception de XDM_ECMD_AUTORAD");
        exit (1);
    }

	/*A Attente des messages sur les sockets (RTserver, d'ecoute et de donnees */
	
	while (1)
	{
		/* Recuperation du msg RTworks suivant */
		pl_Msg = TipcSrvMsgNext(T_TIMEOUT_FOREVER);

		if(isMessageNotNULL(pl_Msg))
		{
			TipcSrvMsgProcess(pl_Msg);
			TipcMsgDestroy(pl_Msg);

			/* Memorisation Test Presence RTserver */
			vl_IndPre = 0;
		}
		/* erreur RTWORKS */
		else
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
				XZST_03EcritureTrace (XZSTC_WARNING, "Bouclage et arret tache tewsp %d \n",vl_IndPre);
			}

			/*B INdication Non Presence RTserver */
			if (errno)
				vl_IndPre++;

		}
	}   /* fin while */
}  /* Fin main */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Effectue les operation de terminaison de la tache
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void    ec_sortir       (  )

/*
* ARGUMENTS EN ENTREE :
*   aucun
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Liberation des ressources memoires
*   Envoyer le message aux taches protocoles
*   Attendre x secondes
*   Fermeture des sockets
*
------------------------------------------------------*/
{
	exit(0);
}
