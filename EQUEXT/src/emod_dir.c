/* Fichier : $Id: emod_dir.c,v 1.15 2019/06/04 13:41:11 devgtie Exp $        Release : $Revision: 1.15 $        Date : $Date: 2019/06/04 13:41:11 $
------------------------------------------------------
 * GTIE *  PROJET MIGRAZUR
------------------------------------------------------
 * SOUS-SYSTEME : EQUEXT
------------------------------------------------------
 * MODULE TEMOD * FICHIER emod_dir.c
------------------------------------------------------
 * DESCRIPTION DU MODULE :
 *
 *   Module directeur de la tache TEMOD.
 *
------------------------------------------------------
 * HISTORIQUE :
 *
 * Mismer.D	version 1.1	17 Oct 1994	: Creation
 * Mismer.D	version 1.2	25 Jan 1995	:
 * Mismer.D	version 1.3	09 Fev 1995	:
 * Fontaine.C	version 1.4	23 Mai 1995	: Reprise entete, historique
 * Mismer.D	version 1.5	06 Jun 1995	: Modif reconnexion avec TESRV
 * Mismer.D	version 1.6	21 Jun 1995	: Modif detection mode isole
 * Mismer.D	version 1.7	28 Jun 1995	: Modif time_out reprise
 * Mismer.D	version 1.8	03 Sep 1995	: Modfif commande arret RTserver
 * Mismer.D	version 1.9	17 Oct 1995	: Modfif test presence serveur
 * Mismer.D	version 1.10	22 Nov 1995	: Modif condition arret RTserver local
 * Mismer.D	version 1.11	08 Fev 1996	: Modif trace et appel ex_presence_machine nvelle version
 * PNI		v1.13		03 Avril 2009	: Ajout envoi du mode quand presence du serveur et retour a la normale DEM877
 * JPL		25/02/11 : Migration HP ia64 (DEM 975) : interface SS68 : type socket 'vg_SockRT'; retour C.R. fonctions 1.14
 * RGR 17/05/19 : DEM1331 - Ajout Mecanisme KEEPALIVE 1.15
 * LCL		25/09/2023	: Adaptation code pour rabbitmq DEM-483
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>
#include "xzems.h"
#include "ex_mrtf.h"

#include "emod_dir.h"


/* definitions de constantes */
/*#define CM_EMOD_ARRET_RTSERVER	"/produits/rtworks30/bin/hp700_hpux_9_0/rtserver.x"*/
#define CM_EMOD_ARRET_RTSERVER	"/produits/tibco/ss66/bin/hp700_hpux/rtserver.x" /*TIBCO SS66*/
#define CM_EMOD_LANCE_RTSERVER	"rtserver -node %s -server_names %s"

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

int	vg_SockLT  = 0;
T_SOCK	vg_SockRT  = 0;
int	vg_SockTIM = 0;
int	vg_SockSRV = 0;


/* declaration de fonctions internes */
void	em_envoi_mode ( char * );
void	em_arreter ( void );
void	em_close_socket ( int );
int	em_lecture_msg_file ( void );
int	em_lance_RTserver ( XDY_NomMachine );
int	em_arret_RTserver ( void );

/* definition de fonctions externes */

/*X*/
/* Fichier : @(#)emod_dir.c	1.11      Release : 1.11        Date : 03/19/96
------------------------------------------------------
 * SERVICE RENDU :
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
 *
 *
 *
------------------------------------------------------*/

int	argc;
char	**argv;
{
  XDY_NomMachine pl_NomMachine = "";
  XDY_NomMachine pl_NomServeur = "";
  XDY_NomSite    pl_NomSite = "";
  XDY_Datagroup		pl_NomDG		= "";
  int			vl_RunRTserverLocal 	= XDC_FAUX;
  int			vl_SockRet     		= 0;
  int			vl_LgMsgSock  		= 0;
  int			vl_IndCnx     		= 0;
  int			vl_EtatRTServer 	= XDC_OK;
  int			vl_Emission     	= 1;
  int			vl_Reception     	= XDC_VRAI;
  int			vl_PingServeur		= 0;
  T_IPC_SRV_CONN_STATUS   vl_Status;
  XZEXT_MSG_SOCKET        pl_MsgTimer     	= "",
      pl_MsgSock      	= "";
  int                     vl_Fd;
  char                    pl_PathEtFic[XDC_PATH_ABS_SIZE];
  XDY_Commentaire         pl_LigneFich;

	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

  /*
   * Connexion au RTserver et initialisation des variables
   */
  if ( XZSC_01InitCnxSup( NULL,
      argv[ 0 ],
      XZSCC_INTERDIRE,
      XZSCC_AUTORISER,
      XZSCC_INTERDIRE,
      em_arreter,
      "/dev/null" ) != XDC_OK )
  {
    /*B Affichage Trace */
    XZST_03EcritureTrace( XZSTC_WARNING, "Ne peut ouvrir la connexion avec le RTserver.\n" );
  }
  else
  {
    /*B Affichage Trace */
    XZST_03EcritureTrace( XZSTC_DEBUG1, "Ouverture de la connexion avec le RTserver." );
  }

  /*
   *  Recuperation du nom de la machine
   */
  if ( gethostname(pl_NomMachine, sizeof(pl_NomMachine) ) == -1)
  {
    /* Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom de la machine impossible." );
  }

  /* Recuperation du site de la machine */
  if (XZSC_04NomSite(pl_NomSite) != XDC_OK)
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible." );
  }

  /* Creation du repertoire pour les sockets */
  ex_rep_socket ( pl_NomMachine );

  /* Construction du nom du fichier XDF_Nom_Serveur  */
  sprintf ( pl_PathEtFic, "%s/%s", XZEXC_PATH_CONFIG, XDF_Nom_Serveur );

  /* Ouverture du fichier XDF_Nom_Serveur  */
  if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
  {
    /* Ecriture trace absence fichier */
    XZST_03EcritureTrace( XZSTC_FATAL, " Manque fichier %s. ", pl_PathEtFic );
  }
  else
  {

    /* Lecture de chaque ligne du fichier */
    while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF )
    {
      /* Lire ligne suivante si c'est une ligne de commentaire */
      if ( *pl_LigneFich != XDF_COMMENTAIRE )
      {
        /* Extraire le nom de l'equipement et l'option presence */
        if ( sscanf ( pl_LigneFich, "%s", pl_NomServeur ) != 1 )
        {
          XZST_03EcritureTrace( XZSTC_FATAL, "Impossible de lire le nom du serveur " );
        }
        else
        {
          XZST_03EcritureTrace( XZSTC_WARNING, "Nom du serveur :%s ", pl_NomServeur );
        }
      }
    }
    /* Fermeture du fichier XDF_Nom_Serveur  */
    XZSS_13FermerFichier( vl_Fd );
  }

  /* Abonnement au DataGroup KEEPALIVE */
  XZSC_31AbtKeepAlive(pl_NomSite);

  while (1)
  {
    if ( vg_SockTIM == 0 )
    {
      /*
       * Connexion socket en mode client pour communiquer avec la tache TETIM
       */
      if ( ex_cnx_client ( XZEXC_ADR_SOCK_MODTIM,
          XZEXC_ADR_SOCK_TETIM_SRV,
          &vg_SockMask,
          &vg_SockTIM ) == XDC_OK )
      {
        /* Programation timer 30s */
        ETIM_REVEIL_CYCLIQUE ( 30, "MOD_Timer_30s", vg_SockTIM, pl_MsgTimer )
                       XZST_03EcritureTrace( XZSTC_WARNING, "Reconnexion avec TETIM");
      }
      else
        sleep (10);
    }

    if ( vg_SockSRV == 0 )
    {
      /*
       * Connexion socket en mode client pour communiquer avec la tache TESRV
       */
      if ( ex_cnx_client ( XZEXC_ADR_SOCK_TEMOD,
          XZEXC_ADR_SOCK_TESRV,
          &vg_SockMask,
          &vg_SockSRV ) == XDC_OK )
      {
        XZST_03EcritureTrace( XZSTC_WARNING, "Reconnexion avec TESRV");
      }
      else
        sleep (10);
    }

    if( ex_attendre_msg ( XZEXC_SOCKET_NULL, &vg_SockMask, &vl_SockRet ) != XDC_OK )
    {
      em_close_socket ( vl_SockRet );
      ex_ConnexionSocketRTserver ( &vg_SockRT, vl_SockRet, &vg_SockMask );
    }
    else
    {
      /* Si une socket a ete activee */
      vl_EtatRTServer = ex_ConnexionSocketRTserver ( &vg_SockRT, 0, &vg_SockMask );
      if ( vl_SockRet != XZEXC_NO_EVT )
      {
        if ( vl_SockRet !=  vg_SockRT )
        {
          /* Lire le message de la socket de donnees */
          if ( ex_lire_msg ( vl_SockRet, pl_MsgSock, &vl_LgMsgSock, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
          {
            /* Fermeture de la socket en erreur */
            em_close_socket ( vl_SockRet );
          }
          else
          {
            /* S'il ne s'agit pas d'un message de deconnexion */
            if ( (vl_IndCnx == XZEXC_DECONNEC) || (vl_LgMsgSock == 0) )
            {
              /* Fermeture de la socket en erreur */
              em_close_socket ( vl_SockRet );
            }
            else
            {
              if ( vl_SockRet == vg_SockLT )
              {
                XZST_03EcritureTrace(XZSTC_INFO, " Reception message du serveur :<%s>",pl_MsgSock );
                vl_Reception = XDC_VRAI;
                em_envoi_mode ( pl_MsgSock );
              }

              if ( vl_SockRet == vg_SockTIM )
              {
                if ( vg_SockLT == 0 )
                  if ( ex_cnx_client_ST( pl_NomServeur, XZEXC_PORT_SERVEUR_SD, &vg_SockMask, &vg_SockLT ) == XDC_OK )
                    vl_Reception = XDC_VRAI;

                if ( vl_Emission && ((vl_PingServeur = ex_presence_machine(pl_NomServeur)) == XDC_OK) )
                {
                  em_envoi_mode (XZEXC_MODE_FONCT_NORMAL);
                  em_arret_RTserver ();
                }

                if ( ((vg_SockLT == 0) || (vl_Reception == XDC_FAUX)) && (vl_PingServeur == XDC_NOK) )
                {
                  if ( vl_RunRTserverLocal == XDC_FAUX )
                  {
                    XZST_03EcritureTrace(XZSTC_WARNING, " LT ISOLE ");
                    em_lance_RTserver (pl_NomMachine);
                    vl_RunRTserverLocal = XDC_VRAI;
                    /* Envoyer a TESRV le changement de mode */
                    em_envoi_mode ( XZEXC_MODE_FONCT_ISOLE );
                    em_close_socket ( vg_SockLT );
                  }
                }
                else
                {
                  if ( vl_RunRTserverLocal == XDC_VRAI )
                  {
                    XZST_03EcritureTrace(XZSTC_WARNING, " LT NORMAL ");/* Envoyer a TESRV le changement de mode */
                    em_envoi_mode (XZEXC_MODE_FONCT_NORMAL);
                    em_arret_RTserver ();
                    vl_RunRTserverLocal = XDC_FAUX;
                  }
                }

                if ( vl_Emission  )
                {
                  if (vg_SockLT)
                  {
                    sprintf ( pl_MsgSock, "%s %s", pl_NomMachine, XZEXC_FIN_MSG_SOCK );
                    sem_wait(&vg_semaphore); 
                    if ( write ( vg_SockLT, pl_MsgSock, strlen(pl_MsgSock) ) != strlen(pl_MsgSock) )
                    {
                      sem_post(&vg_semaphore); 
                      em_close_socket ( vg_SockLT );
                    }
                    else
                    {
                      sem_post(&vg_semaphore); 
                      vl_Reception = XDC_FAUX;
                    }
                  }

                  if ( vl_RunRTserverLocal == XDC_VRAI )
                    /* Envoyer a TESRV le changement de mode */
                    em_envoi_mode ( XZEXC_MODE_FONCT_ISOLE );
                  else
                    em_lecture_msg_file ();
                }
                vl_Emission = !vl_Emission;
              }
            }
          }
        }
      }

    }
  }


}  /* Fin main */



/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *  Fonction effectuant l'envoi du changement de mode a TESRV.
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void	em_envoi_mode ( char * pa_ModeActuel )

/*
 * ARGUMENTS EN ENTREE :
 *
 *  char * pa_ModeActuel : mode a diffuser ( isole ou normal )
 *
 * ARGUMENTS EN SORTIE :
 *  aucun
 *
 * CODE RETOUR :
 *  aucun
 *
 * CONDITION D'UTILISATION
 *  Fonction declenchee par un changement d'etat de la connexion avec le serveur de donnees du site
 *
 * FONCTION
 *  Construction du message
 *  Ecriture du message dans la socket client avec TESRV
 *
------------------------------------------------------*/
{
  XZEXT_MSG_SOCKET	pl_MsgSock  = "";
  XDY_NomTache		vl_NomTache = "";

  if ( vg_SockSRV != 0 )
  {
    if ( strstr( pa_ModeActuel, XZEXC_DEMANDE_ARRET ) != NULL )
    {
      strcpy ( vl_NomTache, strtok (pa_ModeActuel+strlen(XZEXC_DEMANDE_ARRET), " ") );
      if ( XZSS_20PidTache ( vl_NomTache ) == 0 )
        return;
      else
        sprintf ( pl_MsgSock, "%s %s %s\0", XZEXC_DEMANDE_ARRET, vl_NomTache, XZEXC_FIN_MSG_SOCK );
    }
    else
    {
      if ( !strcmp(pa_ModeActuel, XZEXC_MODE_FONCT_NORMAL) ||
          !strcmp(pa_ModeActuel, XZEXC_MODE_FONCT_ISOLE) )
        sprintf ( pl_MsgSock, "%s %s\0", pa_ModeActuel, XZEXC_FIN_MSG_SOCK );
      else
        return;
    }
    if ( send ( vg_SockSRV, pl_MsgSock, (int) strlen (pl_MsgSock), 0 ) <= 0 )
      XZST_03EcritureTrace(XZSTC_WARNING, " Impossible d'emettre un changement de mode sur TESRV." );
    else
      XZST_03EcritureTrace(XZSTC_INFO, " Emission de <%s> sur TESRV.", pl_MsgSock );
  }
  else
    XZST_03EcritureTrace(XZSTC_WARNING, "Pas de connexion avec TESRV." );

}



/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *  Fonction effectuant les operations de terminaison de la tache.
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void	em_arreter ( void )

/*
 * ARGUMENTS EN ENTREE :
 *
 *  aucun
 *
 * ARGUMENTS EN SORTIE :
 *  aucun
 *
 * CODE RETOUR :
 *  aucun
 *
 * CONDITION D'UTILISATION
 *  Fonction declenchee par SUPERV lors de l'arret de la tache
 *  TEMOD
 *
 * FONCTION
 *  Fermeture de toutes les sockets et exit
 *
------------------------------------------------------*/
{
  XZEXT_MASK *                 pl_Socket;

  /* Pour toute les sockets du masque */
  for ( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
    /* Fermeture de la socket courante */
    ex_fin_cnx ( pl_Socket->desc_sock, &vg_SockMask );

  XZST_03EcritureTrace( XZSTC_WARNING, "Sortie de TEMOD. " );

  /* Fin de la tache */
  exit (0);


}




/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *  Fonction effectuant fermeture d'une socket en defaut.
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void	em_close_socket ( int va_Socket )

/*
 * ARGUMENTS EN ENTREE :
 *
 *  int va_Socket	: descripteur de la socket en defaut
 *
 * ARGUMENTS EN SORTIE :
 *  aucun
 *
 * CODE RETOUR :
 *  aucun
 *
 * CONDITION D'UTILISATION
 *
 *  Erreur system sur fonction socket (lecture ou select)
 *
 * FONCTION
 *  Appel de ex_fin_cnx du module ex_msok
 *  Raz variable globale memorisant le descripteur concerne
 *
------------------------------------------------------*/
{

  if ( (va_Socket == vg_SockLT) && (vg_SockLT != 0) )
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "Probleme sur socket TEINI. '%d'", va_Socket );
    ex_fin_cnx ( va_Socket, &vg_SockMask );
    vg_SockLT = 0;
  }
  if ( va_Socket == vg_SockSRV )
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "Probleme sur socket TESRV. '%d'", va_Socket );
    ex_fin_cnx ( va_Socket, &vg_SockMask );
    vg_SockSRV = 0;
  }
  if ( va_Socket == vg_SockTIM )
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "Probleme sur socket TETIM. '%d'", va_Socket );
    ex_fin_cnx ( va_Socket, &vg_SockMask );
    vg_SockTIM = 0;
  }

}


/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *  Emission des messages RTworks memorises en fichier
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

int	em_lecture_msg_file ( void )

/*
 * ARGUMENTS EN ENTREE :
 *
 *  aucun
 *
 * ARGUMENTS EN SORTIE :
 *  aucun
 *
 * CODE RETOUR :
 *  aucun
 *
 * CONDITION D'UTILISATION
 *
 * Toutes les 30 secondes en mode nominal
 *
 * FONCTION
 *  Lecture du repertoire des fichiers d'enregistrements des messages RTworks
 *  Pour tou les fichiers du repertoire
 *    Lecture et emission des messages memorises
 *
------------------------------------------------------*/
{
  char 		pl_NomFichier[XDC_PATH_ABS_SIZE];
  T_IPC_MSG_FILE	vl_MsgFile;
  T_IPC_MSG	vl_Msg;
  DIR		*pl_repert;
  struct dirent	*pl_unFic;
  int		vl_ind,vl_nbFic;


#ifdef _TIBCO_EMS
    XZST_03EcritureTrace( XZSTC_WARNING, "Fonction em_lecture_msg_file deprecated sur bus ems");
#else
  /* Le repertoire est-il accessible */
  if (access(XZEXC_PATH_READ_FILE_MSG, F_OK|R_OK))
  {
    XZST_03EcritureTrace ( XZSTC_WARNING, "Acces repertoire '%s' impossible",
        XZEXC_PATH_READ_FILE_MSG);
    return(XDC_NOK);
  }

  /* Ouverture du repertoire */
  if ( (pl_repert = opendir(XZEXC_PATH_READ_FILE_MSG) ) == NULL)
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "Ouverture repertoire '%s' impossible",
        XZEXC_PATH_READ_FILE_MSG);
    return(XDC_NOK);
  }

  /* Lecture du repertoire */
  while ( ( pl_unFic = readdir(pl_repert) ) != NULL )
  {
    if ( *pl_unFic->d_name != '.' )
    {
      /* Recopie du nom de fichier lu dans le repertoire */
      sprintf ( pl_NomFichier, "%s/%s", XZEXC_PATH_READ_FILE_MSG, pl_unFic->d_name );
      /* Ouverture du fichier de message */
      XZST_03EcritureTrace( XZSTC_DEBUG1, " Fichier: %s", pl_NomFichier );
      if ( (vl_MsgFile = TipcMsgFileCreate ( pl_NomFichier, T_IPC_MSG_FILE_CREATE_READ )) == NULL)
      {
        XZST_03EcritureTrace(XZSTC_WARNING, "Echec ouverture fichier %s.", pl_NomFichier );
      }
      else
      {
        /* Lecture des message dans le fichier */
        while ( TipcMsgFileRead ( vl_MsgFile, &vl_Msg ) )
        {
          /* Envoi des message sur RTserver */
          if ( !TipcSrvMsgSend ( vl_Msg, TRUE ) )
            XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message sur le serveur " );
          else
          {
            XZST_03EcritureTrace(XZSTC_WARNING, "Envoi <%s> ", vl_Msg );
            if ( !TipcMsgDestroy (vl_Msg) )
              XZST_03EcritureTrace(XZSTC_WARNING, "Echec destroy message envoye au serveur " );
          }
        }
        /* Destruction du fichier de message */
        if ( !TipcMsgFileDestroy ( vl_MsgFile ) )
        {
          XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de detruire le fichier %s", pl_NomFichier );
        }
        else
        {
          /* Suppression de l'ancien fichier de message */
          XZSS_06DetruireFichier ( pl_NomFichier );
        }
      }
    }
  }
  /* Fermeture du repertoire */
  closedir(pl_repert);
#endif
  return (XDC_OK);
} 



/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *  Lancement RTserver Local
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

int	em_lance_RTserver ( XDY_NomMachine       	pa_NomMachine )

/*
 * ARGUMENTS EN ENTREE :
 *
 * XDY_NomMachine       	pa_NomMachine : Nom de la machine (node pour le RTserver local)
 *
 * ARGUMENTS EN SORTIE :
 *  aucun
 *
 * CODE RETOUR :
 *  aucun
 *
 * CONDITION D'UTILISATION
 *
 *  Sur detection du mode isole
 *
 * FONCTION
 *
 *  Construction de la commande
 *  Shell de la commande
 *
------------------------------------------------------*/
{ 
  char                  vl_Commande [XZSSC_LINE_CMD_SIZE] = "";
  FILE*                 vl_Cmd          		  = NULL;
  int                   vl_cr = 0;

  /* Preparation de la commande rtserver */
  sprintf (vl_Commande, CM_EMOD_LANCE_RTSERVER, pa_NomMachine, pa_NomMachine );
  /*  Execution 	*/
  if ( XZSS_20PidTache(CM_EMOD_ARRET_RTSERVER) == 0 )
  {
    if ( (vl_Cmd = popen (vl_Commande, "r"))!= NULL ) {
      vl_cr = pclose ( vl_Cmd );
    }
    XZST_03EcritureTrace( XZSTC_WARNING, "Lancement du Rtserver Local" );
  }

  if ( (vl_cr == -1) || (vl_cr == 127) )
    return (XDC_NOK);
  else
    return (XDC_OK);
}



/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *  Arret RTserver Local
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

int	em_arret_RTserver ( void )

/*
 * ARGUMENTS EN ENTREE :
 *
 *  aucun
 *
 * ARGUMENTS EN SORTIE :
 *  aucun
 *
 * CODE RETOUR :
 *  aucun
 *
 * CONDITION D'UTILISATION
 *
 *  Toutes les minutes en mode nominal
 *
 * FONCTION
 *
 *  Recherche si un RTserver local est present dans la liste des
 *  process de la machine. Si oui kill du process RTserver
 *
 *
------------------------------------------------------*/
{ 
  char                  vl_Commande [XZSSC_LINE_CMD_SIZE] = "";
  FILE*                 vl_Cmd          		  = NULL;
  int                   vl_Pid         			  = 0;

  /* Preparation de la commande rtserver */
  while ( (vl_Pid = XZSS_20PidTache(CM_EMOD_ARRET_RTSERVER)) != 0 )
  {
    /* Arret de la tache  */
    XZST_03EcritureTrace( XZSTC_WARNING, "Arret du Rtserver Local" );
    sprintf ( vl_Commande, "kill -9 %d", vl_Pid );
    if ( (vl_Cmd = popen (vl_Commande, "r")) != NULL )
    {
      pclose ( vl_Cmd );
    }
    sleep (1);
  }

  return (XDC_OK);
}
