/* Fichier : ebad_dir.c
------------------------------------------------------
 * STERIA *  PROJET MIGRAZUR
------------------------------------------------------
 * SOUS-SYSTEME : EQUEXT
------------------------------------------------------
 * MODULE TEBAD * FICHIER ebad_dir.c
------------------------------------------------------
 * DESCRIPTION DU MODULE :
 *
 * Module d'initialisation des structures en memoires
 *
------------------------------------------------------
 * HISTORIQUE :
 *
 * JMG	15/09/05 : creation
 * RGR 17/05/19 : DEM1331 - Ajout Mecanisme KEEPALIVE 1.2
 *
 * JPL	23/11/21 : Consultation echeance minuterie et traitement des alarmes en fin de boucle principale (SAE_339)  1.3
------------------------------------------------------ */

/* fichiers inclus */

#include <xzsem.h>
#include "ebad_dir.h"

/* definitions de constantes */

/* definitions de types locaux */

typedef struct {
  int		NumMsg;		/* Numero de message */
  XDY_Fonction	FoncCB;		/* Fonction a appeler */
}tm_callback;

/* definition de macro locales */

/* declaration de variables locales */


static tm_callback pm_TabCB[] = 
    {
        { XDM_ECMD_EQPT, 		(void (*)())en_controle_cb },
        { XDM_ECMD_BAD, 		(void (*)())en_cmd_BAD_cb },
        { XDM_ECMD_SCEN, 		(void (*)())en_cmd_SCEN_cb },
        { XDM_ECMD_ARRET_SCEN,	(void (*)())en_cmd_Arret_SCEN_cb },
        { XDM_EInit_IHM_BAD, 	(void (*)())en_Init_IHM_BAD_cb },
        { XDM_ECMD_Contraste, 	(void (*)())en_contraste_cb },
        { XDM_EInit_IHM_Contraste, 	(void (*)())en_Init_IHM_contraste_cb },
        { XDM_EInit_IHM_Liaison, 	(void (*)())ep_Init_IHM_Liaison_cb },
        { XDM_ELCR, 			(void (*)())en_CMD_LCR }
    };

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/* Fichier : @(#)ebad_dir.c	1.15      Release : 1.15        Date : 09/30/96
------------------------------------------------------
 * SERVICE RENDU :
 * Programme principal
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

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
 * Initialisations :connexion rtserver, connection avec les taches TESRV
 * TETIM, init des structures en memoire, gestion des callbacks
 * init des timer cycliques
 * Boucle sur les sockets et gestion des messages recus
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
  XDY_Mot			vl_MaxEqtLigne = 0;
  XDY_Mot			vl_CoefTimer = 1;

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
    XZST_03EcritureTrace( XZSTC_DEBUG1, "Ouverture de la connexion avec le RTserver." );
  }

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

// XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_DEBUG1|XZSTC_DEBUG2|XZSTC_DEBUG3|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE,XZSTC_NECRAN);
 
   /*A
   * Connexion socket en mode client pour communiquer avec la tache TESRV
   */
  if ( ex_cnx_client ( 	XZEXC_ADR_SOCK_TEBAD,
      XZEXC_ADR_SOCK_TESRV,
      &vg_SockMask,
      &vl_SockSRV ) != XDC_OK )
  {
    /*B Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_FATAL, "Impossible de creer la connexion client  ." );
  }


  /*A
   * Connexion socket en mode client pour communiquer avec la tache TETIM
   */
  if ( ex_cnx_client ( 	XZEXC_ADR_SOCK_BADTIM,
      XZEXC_ADR_SOCK_TETIM_SRV,
      &vg_SockMask,
      &vg_SockTIM ) != XDC_OK )
  {
    /*B Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_FATAL, "Impossible de creer la connexion client  ." );
  }


  /*A
   * Appel de MCFG fonction ec_init
   */
  if ( ec_init(vg_NomMachine, &vl_MaxEqtLigne ) != XDC_OK)
  {
    /*B Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_FATAL, "Lecture fichier config BAD impossible" );
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

  /*A Abonnement sur le message XDM_ECMD_JourNuit */

  XDG_EncodeDG2 ( pl_NomDG, "Einit_JourNuit_BAD", vg_NomMachine) ;
  if(XZEA29_Abt_Etat_JourNuit(XZEAC_DEBUT_ABONNEMENT,pl_NomDG,en_journuit,vg_NomSite,&vl_resultat) != XDC_OK)
    XZST_03EcritureTrace(XZSTC_WARNING,"TEBAD : Impossible de s'abonner a JourNuit");

  /*A
   * Abonnement aux Datagroup
   */
  /* Messages de commande BAD : XDG_ECMD_BAD_<NomMachine> */
  XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_BAD, vg_NomMachine) ;
  /* joint ce datagroup */
  if ( !XDG_JoindreDG( pl_NomDG ) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_FATAL, "Abonnement au Datagroup %s non effectue ", pl_NomDG );
  }

  /* Messages de commande BAD : XDG_ECMD_SCEN_<NomMachine> */
  XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_SCEN_BAD, vg_NomMachine) ;
  /* joint ce datagroup */
  if ( !XDG_JoindreDG( pl_NomDG ) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_FATAL, "Abonnement au Datagroup %s non effectue ", pl_NomDG );
  }

  /* Messages de commande BAD : XDG_ECMD_ARRET_SCEN_<NomMachine> */
  XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_ARRET_SCEN, vg_NomMachine) ;
  /* joint ce datagroup */
  if ( !XDG_JoindreDG( pl_NomDG ) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_FATAL, "Abonnement au Datagroup %s non effectue ", pl_NomDG );
  }

  /* Messages de commande BAD : XDG_EInit_IHM_BAD_<NomSite> */
  XDG_EncodeDG2 ( pl_NomDG, XDG_EInit_IHM_BAD, vg_NomSite) ;
  /* joint ce datagroup */
  if ( !XDG_JoindreDG( pl_NomDG ) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_FATAL, "Abonnement au Datagroup %s non effectue ", pl_NomDG );
  }

  /* Messages de commande BAD : XDG_EInit_IHM_BAD_<NomSite> */
  strcpy ( pl_NomDG, XDG_EInit_IHM_BAD) ;
  /* joint ce datagroup */
  if ( !XDG_JoindreDG( pl_NomDG ) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_FATAL, "Abonnement au Datagroup %s non effectue ", pl_NomDG );
  }

  /* Messages de commande PICTO : XDG_ECMD_CONTRASTE_<NomSite> */
  XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_CONTRASTE, vg_NomSite) ;
  /* joint ce datagroup */
  if ( !XDG_JoindreDG( pl_NomDG ) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_FATAL, "Abonnement au Datagroup %s non effectue ", pl_NomDG );
  }

  /* Messages de commance de contraste : XDG_EInit_IHM_CONTRASTE_<NomSite> */
  XDG_EncodeDG2 ( pl_NomDG, XDG_EInit_IHM_CONTRASTE, vg_NomSite) ;
  /* joint ce datagroup */
  if ( !XDG_JoindreDG( pl_NomDG ) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_FATAL, "Abonnement au Datagroup %s non effectue ", pl_NomDG );
  }

  /* Messages de commance de contraste : XDG_EInit_IHM_Liaison_<NomMachine> */
  XDG_EncodeDG2 ( pl_NomDG, XDG_EInit_IHM_Liaison, vg_NomMachine) ;

  /* joint ce datagroup */
  if ( !XDG_JoindreDG( pl_NomDG ) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_FATAL, "Abonnement au Datagroup %s non effectue ", pl_NomDG );
  }

  /* Abonnement au DataGroup KEEPALIVE */
  XZSC_31AbtKeepAlive(vg_NomSite);

  /*A
   * Demande des timers cycliques
   */
  /* Calcul du coef des timer en fonction du max d'equipement par ligne */
  if ( vl_MaxEqtLigne > 5 )  vl_CoefTimer = 2;
  if ( vl_MaxEqtLigne > 11 ) vl_CoefTimer = 3;
  XZST_03EcritureTrace(XZSTC_WARNING, "Nb eqt max = %d  Coef = %d ",  vl_MaxEqtLigne, vl_CoefTimer );

  /* Programation timer 5mn */
  ETIM_REVEIL_CYCLIQUE ( EBADC_REVEIL_5MN, EBADC_TIMER_5MN, vg_SockTIM, pl_MsgTimer )

  /* Programation timer 10s */
  ETIM_REVEIL_CYCLIQUE ( EBADC_REVEIL_10S * vl_CoefTimer, EBADC_TIMER_10S, vg_SockTIM, pl_MsgTimer )

  sleep (6);

  /* Programation timer 1MN */
  ETIM_REVEIL_CYCLIQUE ( EBADC_REVEIL_1MN * vl_CoefTimer, EBADC_TIMER_1MN, vg_SockTIM, pl_MsgTimer )

  /* Programation de l'heure de la synchro date */
  ETIM_SYNCHRO_DATE ( XZEXC_HEURE_SYNCHRO, "BAD_Timer_Synchro", vg_SockTIM, pl_MsgTimer )

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


