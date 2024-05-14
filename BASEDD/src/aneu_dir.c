/*E*/
/*  Fichier : $Id: aneu_dir.c,v 1.4 2019/01/23 12:22:47 devgfi Exp $      Release : $Revision: 1.4 $        Date : $Date: 2019/01/23 12:22:47 $
-------------------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR / PASTRE
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* MODULE TANEU * FICHIER aneu_dir.c
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  C'est le module directeur de la tache TANEU
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	21/01/13 : Creation d'apres TETDP (DEM 1061)  1.1
* JPL	20/03/13 : Trace forcee au depart  1.2
* JPL	09/10/17 : Ajout demarrage des mesures a l'initialisation  1.3
* JPL	02/01/19 : Support d'un 2eme argument indiquant l'�quipement � g�rer ou � ignorer  1.4
* LCL	24/01/22 : test sur errno dans boucle principale pour ems SAE-312
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
#include	<xzad.h>

#include	"aneu_cfg.h"
#include	"aneu_cyc.h"


/* definitions de constantes */
/* ------------------------- */



/* definitions de types locaux */
/* --------------------------- */



/* declaration de variables */
/* ------------------------ */

	XDY_Eqt	vg_pmv_gere = 0;
	XDY_Eqt	vg_pmv_ignore = 0;

	XDY_Eqt	vg_pmva_gere = 0;
	XDY_Eqt	vg_pmva_ignore = 0;



/* declaration de variables locales */
/* -------------------------------- */

static char *version = "$Id: aneu_dir.c,v 1.4 2019/01/23 12:22:47 devgfi Exp $ : aneu_dir.c" ;



/* Declaration de fonctions internes */
/* --------------------------------- */

void aneu_sortir (void);




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
	char	*pl_car;
	int	vl_car;
	int	vl_i_cb;
	int	vl_IndPre = 0;
	int	vl_Attente = 0;
	int	i;

	pl_car = strrchr (argv[0], '/');
	if (pl_car == NULL) {
		strcpy ( vg_ConfigNeutr.NomTache, argv[0] );
	} else {
		strcpy ( vg_ConfigNeutr.NomTache, pl_car + 1 );
	}


	/*A
	** Memoriser l'�quipement � g�rer ou � ignorer s'il est indiqu�
	** en deuxieme argument sous la forme "{+-}pmv[a]NN"
	*/

	if (argc > 2) {
		vl_car = *argv[2];
		if ( (vl_car == '+')  ||  (vl_car == '-') ) {
			pl_car = argv[2] + 1;
			if (strncasecmp (pl_car, "pmva", strlen ("pmva")) == 0) {
				if (vl_car == '-') {
					vg_pmva_ignore = (XDY_Eqt) atoi (pl_car + strlen ("pmva"));
				} else {
					vg_pmva_gere = (XDY_Eqt) atoi (pl_car + strlen ("pmva"));
				}
			} else if (strncasecmp (pl_car, "pmv", strlen ("pmv")) == 0) {
				if (vl_car == '-') {
					vg_pmv_ignore = (XDY_Eqt) atoi (pl_car + strlen ("pmv"));
				} else {
					vg_pmv_gere = (XDY_Eqt) atoi (pl_car + strlen ("pmv"));
				}
			}
		}
	}

	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);


	/*A
	** Connexion au RTserver et initialisation des variables
	*/

	if (XZSC_01InitCnxSup( argv[1], vg_ConfigNeutr.NomTache, XZSCC_INTERDIRE, XZSCC_AUTORISER,
				XZSCC_INTERDIRE, aneu_sortir, "/dev/null" ) != XDC_OK )
	{
		XZST_03EcritureTrace( XZSTC_FATAL, "Connexion au RTserver impossible." );
		exit ( XDC_NOK );
	}
	else
	{
		XZST_03EcritureTrace ( XZSTC_DEBUG1, "Ouverture de la connexion avec le RTserver." );
	}


	/*XZST_08ForceNivVisuTraceLocal(XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE, XZSTC_NECRAN);*/
	XZST_08ForceNivVisuTraceLocal(XZSTC_WARNING|XZSTC_INFO|XZSTC_INTERFACE|XZSTC_MESSAGE, XZSTC_NECRAN);


	/* Recuperation du nom de la machine */
	XZSC_07NomMachine ( vg_ConfigNeutr.NomMachine );
	XZST_03EcritureTrace ( XZSTC_DEBUG1, "Recuperation du Nom machine %s.", vg_ConfigNeutr.NomMachine );


	/*A
	** Recuperation du site de la machine
	*/

	if ( (vl_ValRet = XZSC_04NomSite (vg_ConfigNeutr.NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace ( XZSTC_FATAL, "Recuperation du Nom du site impossible." );
		exit ( XDC_NOK );
	}
	vg_ConfigNeutr.NumeroSite = XZSC_22NumeroSiteLocal ();


	/*A
	** Connexion a la base de donnees
	*/

	if ( ( vl_ValRet = XZAG02_Connexion_Base (vg_ConfigNeutr.NomSite) ) != XDC_OK )
	{
		XZST_03EcritureTrace ( XZSTC_WARNING, "Impossible de se connecter a la base %s (err=%d).",
		                                       vg_ConfigNeutr.NomSite, vl_ValRet );
		aneu_sortir ();
	}


	/*A
	** Initialisation de MCFG
	*/

        if ( aneu_cfg_init () != XDC_OK )
	{
		XZST_03EcritureTrace ( XZSTC_WARNING, "Init configuration impossible" );
		aneu_sortir ();
	}


	/*A
	** Initialisation du module de calcul des Temps de Parcours
	*/

        if ( XZAD101_Initialiser_TDP (vg_ConfigNeutr.NumeroSite) != XDC_OK )
	{
		XZST_03EcritureTrace ( XZSTC_WARNING, "Init du module Temps de Parcours impossible" );
		aneu_sortir ();
	}


	/*A
	** Demarrage des mesures de Temps de Parcours
	*/

        if ( XZAD110_Demarrer_TDP () != XDC_OK )
	{
		XZST_03EcritureTrace ( XZSTC_WARNING, "Erreur au demarrage des mesures de Temps de Parcours" );
		//aneu_sortir ();
	}


	/*A
	** Recuperation du point de connexion RTworks
	*/

	/*TipcSrvGetSocket (&vg_SockRT);*/


	/*A
	** Programmation des timers cycliques
	*/

	if (aneu_cyc_lancement_timer () != XDC_OK)
	{
		XZST_03EcritureTrace ( XZSTC_WARNING, "Erreur au lancement du timer de mise au neutre" );
		aneu_sortir ();
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
				aneu_sortir ();
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
*  Effectue les operation de terminaison de la tache
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void aneu_sortir (void)

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
	static char *version = "$Id: aneu_dir.c,v 1.4 2019/01/23 12:22:47 devgfi Exp $ : aneu_sortir" ;

	int		vl_ValRet = XDC_OK;
	XDY_Datagroup	vl_datagroup;

	XZST_03EcritureTrace (XZSTC_FONCTION, "IN %s", version);

	XZST_03EcritureTrace (XZSTC_WARNING, "Arret  *********************");


	/* Arret des timers de la tache */
	aneu_cyc_arret_timer ();


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
