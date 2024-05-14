/*Fichier : $Id: etdc_dir.c,v 1.11 2020/11/03 18:12:34 pc2dpdy Exp $      Release : $Revision: 1.11 $        Date : $Date: 2020/11/03 18:12:34 $
------------------------------------------------------
 * GTIE *  PROJET MIGRAZUR
------------------------------------------------------
 * SOUS-SYSTEME : EQUEXT
------------------------------------------------------
 * MODULE TETDC * FICHIER etdc_dir.c
------------------------------------------------------
 * DESCRIPTION DU MODULE :
 *
 *  < Module directeur de la tache TETDC >
 *
------------------------------------------------------
 * HISTORIQUE :
 *
 * Volcic.F	version 1.1	05 Dec 1994	: Creation
 * Fontaine.C	version 1.2	23 Mai 1995	: RAS
 * Mismer.D  	version 1.3	07 Jun 1995	: Modif connexion socket RTserver
 * Volcic.F	version 1.4	27 Jun 1995	: Ajout Varaiables des equations d'alerte
 * Volcic.F	version 1.6	23 Oct 1995	: Suppr. Mode Isole
 * Volcic.F	version 1.7	15 Nov 1995	: Ajout test socket RTserver
 * JPL		version 1.8	30 Sep 2009	: Traces d'arret indiquant la cause (pour DEM 909)
 * JPL		25/02/11 : Migration architecture HP ia64 (DEM 975) : suppression warnings mineurs  1.9
 * RGR 17/05/19 : DEM1331 - Ajout Mecanisme KEEPALIVE 1.10
 * JMG	15/05/20 : EMS 1.11
------------------------------------------------------*/

/* fichiers inclus */

#include <xzsem.h>
#include "ex_mrtf.h"

#include "etdc_dir.h"


/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */

void	et_controle	( int, XDY_NomMachine );

void    et_sortir ( );

/* definition de fonctions externes */


/*X------------------------------------------------------
 * SERVICE RENDU :
 *
 *  Programme Principal
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

int main( argc, argv )

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
 * Initialisations :connexion rtserver, connection avec les taches TESRV
 * TETIM, init des structures en memoire, gestion des callbacks
 * init des timer cycliques
 * Boucle sur les sockets et gestion des messages recus
 *
------------------------------------------------------*/

int	argc;
char	**argv;
{
  int	 		vl_ValRet	= XDC_OK;
  int			vl_SockRet	= 0;
  int			vl_SockSRV	= 0;
  int			vl_SockTIM	= 0;
  int			vl_SockTDC	= 0;
  XDY_Entier		vl_retcode	= 0;
  XZEXT_MSG_SOCKET	pl_MsgTimer;
  int			vl_indice_callback = 0;


  /*A Initialisation du contexte */
  vg_Contexte = XDC_JOUR;

  /*A Recuperation du nom de la machine */
  strcpy(vg_NomMachine,argv[1]);

	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

  /*A Connexion au RTserver et initialisation des variables */
  if( XZSC_01InitCnxSup(  NULL,
      argv[ 0 ],
      XZSCC_INTERDIRE,
      XZSCC_AUTORISER,
      XZSCC_INTERDIRE,
      et_sortir,
      "/dev/null" ) != XDC_OK )
  {
    XZST_03EcritureTrace( XZSTC_FATAL, "etdc_dir : Ne peut ouvrir la connexion avec le RTserver." );
  }
  else
  {
    XZST_03EcritureTrace( XZSTC_DEBUG1, "etdc_dir : Ouverture de la connexion avec le RTserver." );
  }

  /*A Recuperation du site de la machine */
  if ( (vl_ValRet = XZSC_04NomSite(vg_NomSite)) != XDC_OK)
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "etdc_dir : Recuperation du Nom du site impossible . " );
  }

  /*A Rangement des sockets */
  ex_rep_socket ( argv[1] );

  /*A Connexion socket en mode client pour communiquer avec la tache TESRV */
  if ( ex_cnx_client ( 	XZEXC_ADR_SOCK_TETDC,
      XZEXC_ADR_SOCK_TESRV,
      &vg_SockMask,
      &vl_SockSRV ) != XDC_OK )
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "etdc_dir : Impossible de creer la connexion client avec TESRV  ." );
  }


  /*A Connexion socket en mode serveur pour communiquer avec la tache TERDT */
  if ( ex_cnx_serveur ( 	XZEXC_ADR_SOCK_TETDC_SRV,
      &vg_SockMask,
      &vl_SockTDC ) != XDC_OK )
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "etdc_dir : Impossible de creer la connexion serveur pour TERDT." );
  }

  /* Abonnement au DataGroup KEEPALIVE */
  XZSC_31AbtKeepAlive(vg_NomSite);

  /*A Attente des messages sur les sockets (RTserver, d'ecoute et de donnees */
  while (1)
  {

    if ( ex_attendre_msg ( vl_SockTDC, &vg_SockMask, &vl_SockRet ) != XDC_OK )
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "etdc_dir : Probleme retour fonction select  ." );
      ex_ConnexionSocketRTserver ( &vg_SockRT, vl_SockRet, &vg_SockMask );
    }
    else
    {
      ex_ConnexionSocketRTserver ( &vg_SockRT, 0, &vg_SockMask );

      /*B Si une socket a ete activee */
      if ( vl_SockRet != XZEXC_NO_EVT )
      {
        /*B Selon le type de socket */
        /*B Socket TESRV (Init ou Arret tache) */
        if ( vl_SockRet == vl_SockSRV )
        {
          /*B Appel de la fonction et_controle */
          et_controle ( vl_SockSRV, vg_NomMachine );
        }
        else
        {
          /*B Appel traitement de la trame */
          if ( ( vl_SockRet != vl_SockTDC ) && ( vl_SockRet != vg_SockRT ) )
          {
            et_traitement_trame( vl_SockRet );
          }
        }
      }   /* fin si socket active */
    }
  }   /* fin while */

}  /* Fin main */



/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *  Permet selon message socket recu de tesrv
 *    - soit de terminer la tache
 *    - soit de configurer la tache
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void	et_controle	( int 	va_NumSock, XDY_NomMachine pa_NomMachine )

/*
 * ARGUMENTS EN ENTREE :
 *   va_NumSock		: Numero de la socket de TESRV
 *   pa_NomMachine	: Nom de la machine
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
 *   Lecture du message socket
 *   Si message d'arret alors lancer la fonction et_sortir
 *
------------------------------------------------------*/
{
  static char *version = "$Id: etdc_dir.c,v 1.11 2020/11/03 18:12:34 pc2dpdy Exp $ : et_controle" ;

  int			vl_IndCnx = 0;
  int			vl_LgMsg  = 0;
  int			vl_retcode = 0;
  XZEXT_MSG_SOCKET	pl_Msg;

  /*A  Lecture du message recu dans la socket appelante */
  if ( ( vl_retcode = ex_lire_msg ( va_NumSock, pl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) ) != XDC_OK )
  {
    XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket. " );
  }
  else
  {
    /*A S'il s'agit d'une deconnexion */
    if ( vl_IndCnx == XZEXC_DECONNEC )
    {
      /*B Arreter la tache par appel de et_sortir */
      XZST_03EcritureTrace ( XZSTC_WARNING, "Arret sur perte de connexion avec TESRV. " );
      et_sortir ();
    }

    /*A Si le message est correct */
    if ( vl_LgMsg != 0 )
    {
      /*A S'il s'agit d'une demande d'arret */
      if ( strstr ( pl_Msg, XZEXC_DEMANDE_ARRET ) != NULL )
      {
        /*A Arreter la tache par appel de et_sortir  */
        XZST_03EcritureTrace ( XZSTC_WARNING, "Arret sur message d'arret recu de TESRV. " );
        et_sortir ();
      }

      /*A Maj de la variable en question */
      vg_Mode_Fonct = XZEXC_MODE_NORMAL;
    }
  }
}



/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *
 *  Effectue les operations de sortie de la tache
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void et_sortir ( )

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
  static char *version = "$Id: etdc_dir.c,v 1.11 2020/11/03 18:12:34 pc2dpdy Exp $ : et_sortir" ;

  XZEXT_MASK *			pl_Socket;
  XZEXT_ADR_SOCK		pl_TacheProt;
  XZEXT_ADR_SOCK		pl_TacheTrt;
  XZEXT_MSG_SOCKET 		pl_message;
  XDY_Datagroup		pl_NomDG;
  int				vl_resultat = 0;
  int				vl_retcode = 0;

  XZST_03EcritureTrace ( XZSTC_FONCTION, "IN : %s", version);

  /*A Attente de 3 secondes pour l'arret de la tache */
  sleep ( XZEXC_ATTENTE_ARRET_TACHE );

  /*A Fermeture de toutes les sockets */

  /*B Pour toutes les sockets du masque */
  for ( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
  {
    /*B Fermeture de la socket courante */
    ex_fin_cnx ( pl_Socket->desc_sock, &vg_SockMask );
  }

#ifndef _TIBCO_EMS
  /*B Fermeture du socket RTServer */
  if ( !TipcSrvGetSocket ( &vg_SockRT ) )
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "Echec recuperation socket RTserver." );
  }
  else
  {
    shutdown ( (int) vg_SockRT, 2 );
  }
#endif

  /*A  Fin de la tache : exit (XDC_OK) */
  exit (XDC_OK);
}
