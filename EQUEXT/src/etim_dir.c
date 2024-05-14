/*Fichier : $Id: etim_dir.c,v 1.15 2021/05/31 13:19:00 devgfi Exp $      Release : $Revision: 1.15 $        Date : $Date: 2021/05/31 13:19:00 $
------------------------------------------------------
 * GTIE *  PROJET MIGRAZUR
------------------------------------------------------
 * SOUS-SYSTEME : EQUEXT
------------------------------------------------------
 * MODULE TETIM * FICHIER etim_dir.c
------------------------------------------------------
 * DESCRIPTION DU MODULE :
 *
 * Module directeur de la tache TETIM
 * Gestion des demandes de reveil emises par les taches de traitement des equipements.
 * La tache TETIM est lancee par TESRV et communique avec les taches
 * TEzzz par socket (Mode serveur).
 *
------------------------------------------------------
 * HISTORIQUE :
 *
 * Mismer.D	version 1.1	31 Aug 1994	: Creation
 * Mismer.D	version 1.2	28 Oct 1994	:
 * Mismer.D	version 1.3	09 Dec 1994	:
 * Fontaine.C	version 1.4	23 Mai 1995	: Reprise entete, historique
 * Mismer.D	version 1.5	22 Jun 1995	: Modif nom liste timers
 * Mismer.D	version 1.6	29 Aou 1995	: Modif connexion RTserver
 * Mismer.D	version 1.6	29 Aou 1995	: Correction modif connexion RTserver
 * Mismer.D	version 1.7	31 Aou 1995	: Correction argument 1 de XZSC_01InitCnxSup
 * Mismer.D	version 1.8	08 Sep 1995	: Correction gestion connexion RTserver
 *
 * JPL	25/02/11 : Migration HP ia64 (DEM 975) : interface SS68 : type socket 'vg_SockRT'  1.11
 * RGR 17/05/19 : DEM1331 - Ajout Mecanisme KEEPALIVE 1.12
 * JPL	10/05/21 : Modification traces, suppression traces de debug  1.14
 * JPL	20/05/21 : Controles d'echeance et reveil des taches clientes hors interruption (SAE-272)  1.15
------------------------------------------------------*/

/* fichiers inclus */

#include <xzsem.h>
#include "ex_mrtf.h"

#include "etim_dir.h"


/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */


double				vg_Instant_T;
ETIM_ENRG_TIMER *		pg_ListeTimer 	= NULL;
XZEXT_MASK *			vg_SockMask   	= NULL;
T_SOCK				vg_SockRT	= 0;
int				vg_SockEct	= 0;

/* Indicateur de reception de signal ALARM */
XDY_Booleen			vg_Temporisation_Ecoulee = XDC_FAUX;

sigset_t			vm_maskAlarm;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*------------------------------------------------------
 * SERVICE RENDU :
 *  Fonction marquant la reception d'un signal de temporisation
 *
 *  Executee sur declenchement du signal ALRM.
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void ed_Notifier_Alarme_Tempo ( int va_signal )
{
  vg_Temporisation_Ecoulee = XDC_VRAI;
}



/*------------------------------------------------------
 * SERVICE RENDU :
 *  Fonction verifiant si une notification d'echeance
 *  de temporisation a ete recue
 *
 *  Si c'est le cas, reveille les taches dont l'heure programmee est echue
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void ed_controler_Temporisation ( XDY_Booleen va_Forcer_Verif_Reveils )

/*
 * ARGUMENTS EN ENTREE :
 *   aucun
 *
 *
 * ARGUMENTS EN SORTIE :
 *   aucun
 *
 *
 * CODE RETOUR :
 *   aucun
 *
 * CONDITION D'UTILISATION
 *
 * FONCTION
 *   Inhibition du signal d'alarme
 *
------------------------------------------------------*/
{
  XDY_Booleen	vl_Tempo_Ecoulee;

  /* Consulter l'indicateur de notification de temporisation ecoulee */
  sigprocmask (SIG_BLOCK, &vm_maskAlarm, (sigset_t *) NULL);
  vl_Tempo_Ecoulee = vg_Temporisation_Ecoulee;
  vg_Temporisation_Ecoulee = XDC_FAUX;
  sigprocmask (SIG_UNBLOCK, &vm_maskAlarm, (sigset_t *) NULL);

  /* Verifier l'echeance des reveils si demande ou si la temporisation est ecoulee */
  if ( ( vl_Tempo_Ecoulee == XDC_VRAI )  ||  ( va_Forcer_Verif_Reveils == XDC_VRAI ) )
  {
    et_reveiller_signal (SIGALRM);
  }
}



/*X*/
/*----------------------------------------------------
 * SERVICE RENDU :
 *
 * Programme principal
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
 *   Redirection du signal d'alarme UNIX vers la fonction et_reveiller_signal
 *   Connexion au RTServer.
 *   Connexion socket mode client avec TESRV et mode serveur avec les taches traitements
 *
------------------------------------------------------*/

int	argc;
char	**argv;
{

  XDY_NomSite		pl_NomSite = "";
  int			vl_SockDon, vl_SockRet;
  int			vl_ValRet = XDC_OK;
  XDY_Booleen		vl_demandes_reveil_modifiees = XDC_FAUX;
  struct sigaction	vl_sig;

  /* Materiel de blocage / deblocage du signal d'ALARMe */
  sigemptyset( &vm_maskAlarm );
  sigaddset( &vm_maskAlarm, SIGALRM );

extern void et_dump(int);
vl_sig.sa_handler = et_dump;
vl_sig.sa_flags = 0;
vl_sig.sa_mask = vm_maskAlarm;
sigaction( SIGUSR1, &vl_sig, NULL );

  /*
   * Mettre en place les traitements du signal d'alarme pour les Timers
   */
  vl_sig.sa_handler = ed_Notifier_Alarme_Tempo;
  vl_sig.sa_flags = 0;
  vl_sig.sa_mask = vm_maskAlarm;
  sigaction( SIGALRM, &vl_sig, NULL );


	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

  /*
   * Connexion au RTserver en fournissant la fonction ed_arreter
   */

  if ( XZSC_01InitCnxSup( NULL,
      argv[ 0 ],
      XZSCC_INTERDIRE,
      XZSCC_AUTORISER,
      XZSCC_INTERDIRE,
      ed_arreter,
      "/dev/null" ) != XDC_OK )
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "Ne peut ouvrir la connexion avec le RTserver.\n" );
    exit( XDC_NOK );
  }
  else
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "Ouverture de la connexion avec le RTserver.\n" );
  }

  /* Recuperation du site de la machine */
  if (XZSC_04NomSite(pl_NomSite) != XDC_OK)
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible." );
  }


  /* Creation du repertoire pour les sockets */
  ex_rep_socket ( argv[1] );

  /* Connexion socket en mode serveur pour communiquer avec les taches traitements */
  if ( ex_cnx_serveur ( 	XZEXC_ADR_SOCK_TETIM_SRV,
      &vg_SockMask,
      &vg_SockEct ) != XDC_OK )
  {
    /*B Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_FATAL, "Impossible de creer la connexion serveur \n" );
  }

  /* Connexion socket en mode client pour communiquer avec la tache TESRV */
  if ( ex_cnx_client ( 	XZEXC_ADR_SOCK_TETIM,
      XZEXC_ADR_SOCK_TESRV,
      &vg_SockMask,
      &vl_SockDon ) != XDC_OK )
  {
    XZST_03EcritureTrace( XZSTC_FATAL, "Impossible de creer la connexion client avec TESRV \n" );
  }


  /* Abonnement au DataGroup KEEPALIVE */
  XZSC_31AbtKeepAlive(pl_NomSite);

  XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE,XZSTC_NECRAN);


  /* Attente des messages sur les sockets (RTserver, d'ecoute et de donnees */
  while (1)
  {
    XZST_03EcritureTrace(XZSTC_DEBUG3, "attente message");
    vl_ValRet = ex_attendre_msg ( vg_SockEct, &vg_SockMask, &vl_SockRet );

    /* Verifier l'echeance d'une temporisation et traiter les demandes de reveil echues */
    ed_controler_Temporisation ( (XDY_Booleen) XDC_FAUX );
    vl_demandes_reveil_modifiees = XDC_FAUX;

    if ( vl_ValRet != XDC_OK )
    {
      if (vl_SockRet != XZEXC_NO_EVT )
      {
        XZST_03EcritureTrace(XZSTC_WARNING, "Probleme attente msg, socket %d (ect:%d don:%d rt:%d)", vl_SockRet,vg_SockEct, vl_SockDon,vg_SockRT);
        ex_ConnexionSocketRTserver ( &vg_SockRT, vl_SockRet, &vg_SockMask );
      }
      else XZST_03EcritureTrace(XZSTC_DEBUG3, "attente msg a echoue");
    }
    else
    {
      XZST_03EcritureTrace(XZSTC_DEBUG3, "verif. connexion RTserver");
      ex_ConnexionSocketRTserver ( &vg_SockRT, 0, &vg_SockMask );

      /* Si une socket a ete activee */
      if ( vl_SockRet == XZEXC_NO_EVT )
      {
        XZST_03EcritureTrace(XZSTC_DEBUG3, "pas d'evenement");
      }
      else
      {
        /* Si il s'agit de la socket RTserver ou de la socket d'ecoute*/
        if ( ( vl_SockRet == vg_SockRT ) || ( vl_SockRet == vg_SockEct ) )
        {
          /* Le message est lu par les primitive standard de RTWorks ou il s'agit
           * d'une connexion d'un client (gerer par la fonction ex_attendre_msg) */
          XZST_03EcritureTrace(XZSTC_DEBUG3, "Evenement sur socket %s",
                                                (vl_SockRet == vg_SockRT) ? "RTserver" : "d'ecoute");
        }
        else
        {
          /* Si il s'agit de la socket avec la tache TESRV */
          if ( vl_SockRet == vl_SockDon )
          {
            /* Appel de la fonction ed_cmd pour traiter la demande d'arret */
            ed_cmd ( vl_SockRet );
          }
          else
          {
            /* Il s'agit d'une socket de donnee avec une tache traitement : analyser la demande */
            et_analyse_dem ( vl_SockRet );
            vl_demandes_reveil_modifiees = XDC_VRAI;
          }
        }   /* fin else socket RTserver */
      }   /* fin si socket active */
    }

    /* Verifier l'echeance d'une temporisation et traiter les demandes de reveil echues */
    ed_controler_Temporisation ( vl_demandes_reveil_modifiees );
  }   /* fin while */

}  /* Fin main */


/*------------------------------------------------------
 * SERVICE RENDU :
 *  Fonction effectuant les operation de terminaison de la tache TETIM
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void ed_arreter ( void )

/*
 * ARGUMENTS EN ENTREE :
 *   aucun
 *
 *
 * ARGUMENTS EN SORTIE :
 *   aucun
 *
 *
 * CODE RETOUR :
 *   aucun
 *
 * CONDITION D'UTILISATION
 *   Fonction declenchee par la supervision lors de l'arret de la tache ou apres
 *   reception du message d'arret de la part de TESRV
 *
 * FONCTION
 *   Inhibition du signal d'alarme
 *   Annulation de l'alarme en cours
 *   Liberation des ressources memoires
 *   Fermeture de toutes les sockets
 *
------------------------------------------------------*/
{

  ETIM_ENRG_TIMER *	pl_Tempo, *pl_Tempo_A_Liberer;
  struct itimerval	vl_Prog;
  XZEXT_MASK * 	pl_masque;
  struct sigaction	vl_sig;

  /* Inhibition du signal d'alarme */
  vl_sig.sa_handler = SIG_IGN;
  vl_sig.sa_flags = 0;
  sigemptyset( &vl_sig.sa_mask );
  sigaction ( SIGALRM, &vl_sig, NULL );

  /* Ecriture trace sortie de la tache */
  XZST_03EcritureTrace(XZSTC_INFO,"ed_arreter : Sortie demandee");

  /* Annulation de l'alarme en cours */
  vl_Prog.it_interval.tv_sec  	= 0;
  vl_Prog.it_interval.tv_usec 	= 0;
  vl_Prog.it_value.tv_sec 	= 0;
  vl_Prog.it_value.tv_usec 	= 0;
  setitimer ( ITIMER_REAL, &vl_Prog, (struct itimerval *)NULL );

  /*
   * Fermeture de toutes les sockets
   */

  /* Pour toute les sockets du masque */
  for ( pl_masque = vg_SockMask; pl_masque != NULL; pl_masque = pl_masque->Suivant )
  {
    /* Si socket d'ecoute et si differente de la socket RTserver */
    if ( ( pl_masque->desc_sock != vg_SockEct ) && ( pl_masque->desc_sock != vg_SockRT ) )
    {
      /* Fermeture de la socket courante */
      ex_fin_cnx ( pl_masque->desc_sock, &vg_SockMask );
    }
  }

  /* Liberation des temporisations  et des ressources memoires */
  for ( pl_Tempo = pg_ListeTimer; pl_Tempo != NULL; free(pl_Tempo_A_Liberer) )
  {
    pl_Tempo_A_Liberer = pl_Tempo;
    pl_Tempo           = pl_Tempo->ptrEnrSuiv;
  }

  /* Fin de la tache TETIM */
  exit(0);

}


/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *  Cette fonction traite les message recu de TESRV
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

int ed_cmd ( int va_NumSock )

/*
 * ARGUMENTS EN ENTREE :
 *   va_NumSock		: Numero de la socket de TESRV
 *
 *
 * ARGUMENTS EN SORTIE :
 *   aucun
 *
 *
 * CODE RETOUR :
 *   XDC_OK		: Message de TESRV correct
 *   XDC_NOK		: Message de TESRV incorrect
 *
 * CONDITION D'UTILISATION
 *
 *
 * FONCTION
 *   Lecture du message recu dans la socket avec TESRV
 *   Examen du message
 *
------------------------------------------------------*/
{

  int			vl_IndCnx = 0,
      vl_LgMsg  = 0;
  XZEXT_MSG_SOCKET	pl_Msg;


  /*
   *  Lecture du message recu dans la socket TESRV
   */

  if ( ex_lire_msg ( va_NumSock, pl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
  {
    /* Ecriture trace Erreur lecture message socket. */
    XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket. \n" );
    ed_arreter ();
  }
  else
  {
    /* S'il s'agit d'un message de deconnexion */
    if ( (vl_IndCnx == XZEXC_DECONNEC) || (vl_LgMsg == 0) )
    {
      /* Appel de ed_arreter */
      ed_arreter ();
    }
    else
    {
      /* Si le message est une demande d'arret de TESRV ou une deconnexion de TESRV  */
      if ( ( strstr ( pl_Msg, XZEXC_DEMANDE_ARRET ) != NULL ) || ( vl_IndCnx == XZEXC_DECONNEC ) )
      {
        /* Appel de ed_arreter */
        ed_arreter ();
      }
      else
      {
        /* Ecriture trace Message incorrect en provenance de TESRV */
        XZST_03EcritureTrace( XZSTC_WARNING, " Message incorrect en provenance de TESRV. \n" );
        return ( XDC_NOK );
      }
    }
  }

  return (XDC_OK);
}
