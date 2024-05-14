/*E*/
/*  Fichier : $Id: anet_dir.c,v 1.3 2017/05/17 09:30:26 devgfi Exp $      Release : $Revision: 1.3 $        Date : $Date: 2017/05/17 09:30:26 $
-------------------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* MODULE ANET * FICHIER anet_dir.c
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  C'est le module directeur de la tache TANET
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	03/04/17 : Creation (DEM 1221)  1.1
* JPL	26/04/17 : Ajout d'une deuxieme phase d'initialisation de configuration  1.2
* JPL	16/05/17 : Ajout d'une action de demarrage des mesures de Temps de parcours  1.3
-----------------------------------------------------------------------------------------*/

/* fichiers inclus */
/* --------------- */

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include <xzsem.h>
#include	<xdc.h>
#include	<xdy.h>
#include	<xzst.h>

#include	<xzao.h>

#include	"anet_cfg.h"
#include	"anet_tra.h"
#include	"anet_cyc.h"


/* definitions de constantes */
/* ------------------------- */



/* definitions de types locaux */
/* --------------------------- */



/* declaration de variables */
/* ------------------------ */



/* declaration de variables locales */
/* -------------------------------- */

static char *version = "$Id: anet_dir.c,v 1.3 2017/05/17 09:30:26 devgfi Exp $ : anet_dir.c" ;



/* Declaration de fonctions internes */
/* --------------------------------- */

void anet_sortir (void);




/*X*/
/*
-------------------------------------------------------------------------------------------
*
* SERVICE RENDU :
* Programme principal
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int main ( argc, argv )

/*
* ARGUMENTS EN ENTREE :
*
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
* Initialisations :
*   - connexion au RTServer,
*   - init des structures en memoire, gestion des callbacks
*   - abonnement aux datagroups
*   - init des timers cycliques
*   - boucle sur les sockets et gestion des messages recus.
*
-----------------------------------------------------------------------------------------*/

int	argc;
char	**argv;
{
	XDY_Datagroup	pl_NomDG;
	T_IPC_MSG	pl_Msg;

	int	vl_ValRet = XDC_OK;
	char	*pl_carSepar;
	int	vl_i_cb;
	int	vl_IndPre = 0;
	int	vl_Attente = 0;
	int	i;

	pl_carSepar = strrchr (argv[0], '/');
	if (pl_carSepar == NULL) {
		strcpy ( vg_ConfigNeutreSaga.NomTache, argv[0] );
	} else {
		strcpy ( vg_ConfigNeutreSaga.NomTache, pl_carSepar + 1 );
	}

	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);


	/*A
	** Connexion au RTserver et initialisation des variables
	*/

	if (XZSC_01InitCnxSup( argv[1], vg_ConfigNeutreSaga.NomTache, XZSCC_INTERDIRE, XZSCC_AUTORISER,
				XZSCC_INTERDIRE, anet_sortir, "/dev/null" ) != XDC_OK )
	{
		XZST_03EcritureTrace( XZSTC_FATAL, "Connexion au RTserver impossible." );
		exit ( XDC_NOK );
	}
	else
	{
		XZST_03EcritureTrace ( XZSTC_DEBUG1, "Ouverture de la connexion avec le RTserver." );
	}


//	XZST_08ForceNivVisuTraceLocal (XZSTC_INFO | XZSTC_FONCTION | XZSTC_INTERFACE | XZSTC_MESSAGE, XZSTC_NECRAN);
	XZST_08ForceNivVisuTraceLocal (XZSTC_INFO | XZSTC_INTERFACE | XZSTC_MESSAGE, XZSTC_NECRAN);


	/* Recuperation du nom de la machine */
	XZSC_07NomMachine ( vg_ConfigNeutreSaga.NomMachine );
	XZST_03EcritureTrace ( XZSTC_DEBUG1, "Recuperation du Nom machine %s.", vg_ConfigNeutreSaga.NomMachine );


	/*A
	** Recuperation du site de la machine
	*/

	if ( (vl_ValRet = XZSC_04NomSite (vg_ConfigNeutreSaga.NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace ( XZSTC_FATAL, "Recuperation du Nom du site impossible." );
		exit ( XDC_NOK );
	}
	vg_ConfigNeutreSaga.NumeroSite = XZSC_22NumeroSiteLocal ();


	/*A
	** Connexion a la base de donnees
	*/

	if ( ( vl_ValRet = XZAG02_Connexion_Base (vg_ConfigNeutreSaga.NomSite) ) != XDC_OK )
	{
		XZST_03EcritureTrace ( XZSTC_WARNING, "Impossible de se connecter a la base %s (err=%d).",
		                                       vg_ConfigNeutreSaga.NomSite, vl_ValRet );
		anet_sortir ();
	}


	/*A
	** Initialisation de MCFG
	*/

        if ( anet_cfg_init () != XDC_OK )
	{
		XZST_03EcritureTrace ( XZSTC_WARNING, "Init configuration impossible" );
		anet_sortir ();
	}


	/*A
	** Initialisation du module de calcul des Temps de Parcours
	*/

        if ( XZAD101_Initialiser_TDP (vg_ConfigNeutreSaga.NumeroSite) != XDC_OK )
	{
		XZST_03EcritureTrace ( XZSTC_WARNING, "Init du module Temps de Parcours impossible" );
		anet_sortir ();
	}


	/*A
	** Complement d'initialisation de la configuration
	*/

        if ( anet_cfg_init_2 () != XDC_OK )
	{
		XZST_03EcritureTrace ( XZSTC_WARNING, "Init configuration etape 2 impossible" );
		anet_sortir ();
	}


	/*A
	** Demarrage des mesures de Temps de parcours
	*/

        if ( XZAD110_Demarrer_TDP () != XDC_OK )
	{
		XZST_03EcritureTrace ( XZSTC_WARNING, "Erreur au demarrage des mesures de Temps de parcours" );
		//anet_sortir ();
	}


	/*A
	** Initialisation des echanges externes
	** (reception des etats equipements)
	*/

        if ( anet_tra_init () != XDC_OK )
	{
		XZST_03EcritureTrace ( XZSTC_WARNING, "Init echanges externes impossible" );
		anet_sortir ();
	}



	/*A
	** Recuperation du point de connexion RTworks
	*/

	/*TipcSrvGetSocket (&vg_SockRT);*/


	/*A
	** Programmation des timers cycliques
	*/

	if (anet_cyc_lancement_timer () != XDC_OK)
	{
		XZST_03EcritureTrace ( XZSTC_WARNING, "Erreur au lancement du timer de mise au neutre" );
		anet_sortir ();
	}


	/*A
	** Attente bloquante sur les messages du RTserver.
	*/

	while (TRUE)
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "Attente infinie d'un message \n");

		pl_Msg = TipcSrvMsgNext (T_TIMEOUT_FOREVER);

		/* si message recu */
		if (pl_Msg != NULL)
		{
			/* execution du call-back associe au message et destruction du message */
			TipcSrvMsgProcess (pl_Msg);
			TipcMsgDestroy (pl_Msg);

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
				XZST_03EcritureTrace (XZSTC_WARNING, "Bouclage et arret tache %d \n", vl_IndPre);
				/*A Arret de la tache */
				anet_sortir ();
			}

			/*B INdication Non Presence RTserver */
			if (errno)
				vl_IndPre++;

			/*B Attente aleatoire pour permettre une reconnexion */
			vl_Attente = rand() % 15;
			sleep (vl_Attente);
		}
	}   /* fin while */

}  /* Fin main */




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*  Effectue les operations de terminaison de la tache
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void anet_sortir (void)

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
*   Annulation de la programmation des timers ;
*   Liberation des ressources memoires ;
*   Fermeture du fichier de trace.
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: anet_dir.c,v 1.3 2017/05/17 09:30:26 devgfi Exp $ : anet_sortir" ;

	int		vl_ValRet = XDC_OK;
	XDY_Datagroup	vl_datagroup;

	XZST_03EcritureTrace (XZSTC_FONCTION, "IN %s", version);

	XZST_03EcritureTrace (XZSTC_WARNING, "Arret  *********************");


	/* Arret des timers de la tache */
	anet_cyc_arret_timer ();


	/* Arret des echanges externes */
	anet_tra_fin ();


	/*A Terminaison du module de calcul des Temps de Parcours */
	XZAD102_Terminer_TDP ();


	/*A Deconnexion de la base */
	if (XZAG03_Deconnexion_Base () != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "Deconnexion base non effectuee");
	}


	/*A Fermeture du fichier de trace */
	XZST_02FermetureTrace ();

	/* Fin de la tache */
	exit (0);
}
