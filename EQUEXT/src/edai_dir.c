/*Fichier : edai_dir.c
------------------------------------------------------
 * GTIE *  PROJET MIGRAZUR
------------------------------------------------------
 * SOUS-SYSTEME : EQUEXT
------------------------------------------------------
 * MODULE TEDAI * FICHIER edai_dir.c
------------------------------------------------------
 * DESCRIPTION DU MODULE :
 *
 *  Module directeur de la tache DAI
 *
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	17 Oct 1994	: Creation
* Mismer.D	version 1.2	01 Dec 1994	:
* Mismer.D	version 1.3	14 Dec 1994	:
* Nagiel.E	version 1.4	22 Dec 1994	:
* Nagiel.E	version 1.5	05 Jan 1995	:
* Nagiel.E	version 1.6	16 Jan 1995	:
* Nagiel.E	version 1.7	16 Jan 1995	:
* Volcic.F	version 1.8	04 Avr 1995	:
* Mismer.D	version 1.9	03 Mai 1995	: Ajout Abt etat liaison
* Fontaine.C	version 1.10	23 Mai 1995	: Reprise entete, historique
* Mismer.D  	version 1.11	07 Jun 1995	: Modif connexion socket RTserver
* Mismer.D  	version 1.12	31 Aou 1995	: Modif connexion socket RTserver suite
* Mismer.D  	version 1.13	11 Oct 1995	: Ajout appel fonction init Acknoledge alarme
* JPL		30/11/11 : Migration architecture HP ia64 (DEM 975) : suppression warnings mineurs  1.15
* RGR 17/05/19 : DEM1331 - Ajout Mecanisme KEEPALIVE 1.16
* JMG		05/06/19 : ajout IP DEM1333 1.17
* JPL		23/11/21 : Consultation echeance minuterie et traitement des alarmes en fin de boucle principale (SAE_339)  1.19
* GGY		20/11/23 : Ajout ed_Enregistrement_DAI_cb (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

#include <xzsem.h>
#include "ex_mrtf.h"

#include "edai_dir.h"
#include "edai_tra.h"

/* definitions de constantes */

/* definitions de types locaux */

typedef struct {
  int		NumMsg;		/* Numero de message */
  XDY_Fonction	FoncCB;		/* Fonction a appeler */
}tm_callback;

/* definition de macro locales */

/* declaration de variables locales */
int vg_SockTEMIP;
extern XDY_Horodate vg_last_switch;

static char *version = "$Id: edai_dir.c,v 1.18 2021/05/04 13:25:02 pc2dpdy Exp $ : edai_dir" ;

static tm_callback pm_TabCB[] =
    {
        { XDM_ECMD_EQPT, 			(void (*)())ed_controle_cb },
        { XDM_EInit_IHM_DAI, 		(void (*)())ed_Init_IHM_Mes_DAI_cb },
        { XDM_EConf_Voie_DAI, 		(void (*)())ed_Conf_Voie_DAI_cb },
        { XDM_EInit_IHM_Liaison, 	(void (*)())ep_Init_IHM_Liaison_cb },
        { XDM_ELCR, 				(void (*)())ed_CMD_LCR },
		{ XDM_RECORD_DAI,			(void (*)())ed_Enregistrement_DAI_cb }
    };

/* declaration de fonctions internes */

/* definition de fonctions externes */
extern int ey_traitement_cyclique ( int );

void edai_KEEPALIVE_cb(T_IPC_CONN                 va_cnx,
                 T_IPC_CONN_PROCESS_CB_DATA pa_data,
	         T_CB_ARG                   pa_arg)
{
 EDAI_DONNEES_DAI             *pl_listeDAI;

pl_listeDAI = pg_debutDAI;
                       EnvoyerTrameIP2(" \0",
                                       XZEXC_TRAME_TELEI_ACK,
                                       pl_listeDAI->Config.AdresseIP,
                                       pl_listeDAI->Config.Port,
                                       pl_listeDAI->Config.Adresse2IP,
                                       pl_listeDAI->Config.Port2,
                                       pl_listeDAI->Config.DAIBis,
                                       pl_listeDAI->Config.AdresseRGS,
                                       XZEXC_NOCMD,
                                       XZEXC_PRIORITEFAIBLE,
                                       pl_listeDAI->SocketEsclave);


}

/*------------------------------------------------------
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
 * Initialisations :connexion rtserver, connection avec les taches TESRV
 * TETIM, init des structures en memoire, gestion des callbacks
 * init des timer cycliques
 * Boucle sur les sockets et gestion des messages recus
 *
------------------------------------------------------*/

int	argc;
char	**argv;
{
  int 			vl_ValRet	=XDC_OK,
      vl_SockRet	= 0,
      vl_SockSRV	= 0,
      vl_resultat;
  XZEXT_MSG_SOCKET	pl_MsgTimer;
  XDY_Datagroup		pl_NomDG;
  int			vl_indice_callback;


  /*A
   *  Recuperation du nom de la machine
   */
  strcpy(vg_NomMachine,argv[1]);

	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

  /*A
   * Connexion au RTserver et initialisation des variables
   */
  if( XZSC_01InitCnxSup(  NULL,
      argv[ 0 ],
      XZSCC_INTERDIRE,
      XZSCC_AUTORISER,
      XZSCC_INTERDIRE,
      ec_sortir,
      "/dev/null" ) != XDC_OK )
  {
    /*B Affichage Trace */
    XZST_03EcritureTrace( XZSTC_FATAL, "Ne peut ouvrir la connexion avec le RTserver." );
  }
  else
  {
    /*B Affichage Trace */
    XZST_03EcritureTrace( XZSTC_WARNING, "Ouverture de la connexion avec le RTserver." );
  }

  /*XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_DEBUG1|XZSTC_DEBUG2|XZSTC_DEBUG3|XZSTC_INTERFACE,XZSTC_NECRAN);*/

  /*A
   * Recuperation du site de la machine
   */
  if ( (vl_ValRet = XZSC_04NomSite(vg_NomSite)) != XDC_OK)
  {
    /* Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_FATAL, "Recuperation du Nom du site impossible . " );
  }

  /*A Creation du repertoire pour les sockets */
  ex_rep_socket ( argv[1] );

  /*A Init pour Acknoledge Alarme */
  ex_init_alarme ( argv[1], argv[0] );
  vg_SockTEMIP = -1;
  /*A
   * Connexion socket en mode client pour communiquer avec la tache TESRV
   */
  if ( ex_cnx_client ( 	XZEXC_ADR_SOCK_TEDAI,
      XZEXC_ADR_SOCK_TESRV,
      &vg_SockMask,
      &vl_SockSRV ) != XDC_OK )
  {
    /*B Ecriture Trace */
    /*! XZST_03EcritureTrace( XZSTC_FATAL, "Impossible de creer la connexion client  ." ); */
    XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer la connexion client  ." );
  }


  /*A
   * Connexion socket en mode client pour communiquer avec la tache TETIM
   */
  if ( ex_cnx_client ( 	XZEXC_ADR_SOCK_DAITIM,
      XZEXC_ADR_SOCK_TETIM_SRV,
      &vg_SockMask,
      &vg_SockTIM ) != XDC_OK )
  {
    /*B Ecriture Trace */
    /*! XZST_03EcritureTrace( XZSTC_FATAL, "Impossible de creer la connexion client  ." ); */
    XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer la connexion client  ." );
  }


  XZST_03EcritureTrace( XZSTC_WARNING, "avant ec_init");
  vg_last_switch=0;

  /*A
   * Appel de MCFG fonction ec_init
   */
  if ( ec_init(vg_NomMachine) != XDC_OK )
  {
    /*B Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_FATAL, "Initialisation Config impossible" );
  }

  /*A
   * Armement des Callback sur reception des message
   */

  /* Initialisation des callbacks de la tache */

  for (vl_indice_callback = 0; vl_indice_callback < (sizeof(pm_TabCB)/sizeof(pm_TabCB[0])); vl_indice_callback++)
  {
    if(!TipcSrvProcessCbCreate(	XDM_IdentMsg(pm_TabCB[vl_indice_callback].NumMsg ) ,
        pm_TabCB[vl_indice_callback].FoncCB ,
        (T_CB_ARG)NULL) )
    {
      XZST_03EcritureTrace(XZSTC_FATAL,
          "erreur init callback '%s'",
          pm_TabCB[vl_indice_callback].FoncCB);
    }
  }

  /*A
   * Abonnement aux Datagroup
   */
  /* Messages de controle : XDG_ECMD_DAI_<NomMachine> */
  XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_DAI, vg_NomMachine) ;
  /* joint ce datagroup */
  if ( !XDG_JoindreDG( pl_NomDG ) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_FATAL,
        "Abonnement au Datagroup %s non effectue ", pl_NomDG );
  }

  /* Messages de demande de mesure DAI : XDG_EInit_IHM_DAI_N0 */
  XDG_EncodeDG2 ( pl_NomDG, XDG_EInit_IHM_DAI, "N0") ;
  /* joint ce datagroup */
  if ( !XDG_JoindreDG( pl_NomDG ) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_FATAL,
        "Abonnement au Datagroup %s non effectue ", pl_NomDG );
  }

  /* Messages de commance de contraste : XDG_EInit_IHM_Liaison_<NomMachine> */
  XDG_EncodeDG2 ( pl_NomDG, XDG_EInit_IHM_Liaison, vg_NomMachine) ;
  /* joint ce datagroup */
  if ( !XDG_JoindreDG( pl_NomDG ) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_FATAL,
        "Abonnement au Datagroup %s non effectue ", pl_NomDG );
  }


  /* Messages de configuration de voie : XDG_ECONF_VOIE_DAI_<NomMachine> */
  XDG_EncodeDG2 ( pl_NomDG, XDG_ECONF_VOIE_DAI, vg_NomMachine) ;
  /* joint ce datagroup */
  if ( !XDG_JoindreDG( pl_NomDG ) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_FATAL,
        "Abonnement au Datagroup %s non effectue ", pl_NomDG );
  }

  /* Abonnement au DataGroup KEEPALIVE */
  XDG_EncodeDG2(pl_NomDG, XDG_KEEPALIVE, vg_NomSite);
  XDG_JoindreDG( pl_NomDG );
  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSE_KEEPALIVE),edai_KEEPALIVE_cb,(T_CB_ARG) NULL) == NULL)
      {
          XZST_03EcritureTrace(XZSTC_FATAL, "Rattachement a la callback edai_process_KEEPALIVE_cb non effectue");
	        }

  /*A
   * Demande des timers cycliques
   */

  /* Programation timer 1mn */
  ETIM_REVEIL_CYCLIQUE ( EDAIC_REVEIL_1MN, "DAI_Timer_1mn", vg_SockTIM, pl_MsgTimer )

  /* Programation de l'heure de la synchro date */
  ETIM_SYNCHRO_DATE ( XZEXC_HEURE_SYNCHRO, "DAI_Timer_Synchro", vg_SockTIM, pl_MsgTimer )

  /*A Attente des messages sur les sockets (RTserver, d'ecoute et de donnees */

  while (1)
  {

    if ( ex_attendre_msg ( XZEXC_SOCKET_NULL, &vg_SockMask, &vl_SockRet ) != XDC_OK )
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "Probleme retour fonction select  ." );
      ex_ConnexionSocketRTserver ( &vg_SockRT, vl_SockRet, &vg_SockMask );
    }
    else
    {
      ex_ConnexionSocketRTserver ( &vg_SockRT, 0, &vg_SockMask );

      /* Si une socket a ete activee */
      if ( vl_SockRet != XZEXC_NO_EVT )
      {
        /* Selon le type de socket */
        /* Socket RT server */
        if ( vl_SockRet == vg_SockRT )
        {
          /* Le message est lu par les primitive standard de RTWorks */
        }
        else
        {
          /* Socket TETIM (retour Timer) */
          if ( vl_SockRet == vg_SockTIM )
          {
            /* Appel de la fonction ec_traitement_cyclique de MCYC */
            ey_traitement_cyclique ( vg_SockTIM );
          }
          else
          {
            /* Socket TESRV (Init ou Arret tache) */
            if ( vl_SockRet == vl_SockSRV )
            {
              /* Appel de la fonction ec_configuration de MCFG */
              ec_controle ( vl_SockSRV, vg_NomMachine );
            }
            else
            {
              /* Appel traitement de la trame */
              em_traitement_trame( vl_SockRet );
            }
          }
        }
      }   /* fin si socket active */
    }

    if (XZSS_15ConsulterTimeout () == XDC_OUI)
    {
      XZSS_18AnnulerTimeout ();
      ex_traiter_alarmes ();
    }
  }   /* fin while */


}  /* Fin main */

