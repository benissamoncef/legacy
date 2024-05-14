/* Fichier : erau_dir.c
------------------------------------------------------
 * GTIE *  PROJET MIGRAZUR
------------------------------------------------------
 * SOUS-SYSTEME : EQUEXT
------------------------------------------------------
 * MODULE TERAU * FICHIER erau_dir.c
------------------------------------------------------
 * DESCRIPTION DU MODULE :
 *
 * C'est le module directeur de la tache TERAU
 *
------------------------------------------------------
 * HISTORIQUE :
 *
 * Nagiel.E	version 1.1	30 Nov 1994	: Creation
 * Nagiel.E	version 1.2	24 Jan 1995	:
 * Fontaine.C	version 1.3	22 Mai 1995	: Reprise entete, historique
 * Mismer.D  	version 1.4	07 Jun 1995	: Modif connexion socket RTserver
 * Mismer.D  	version 1.5	31 Aou 1995	: Modif connexion socket RTserver suite
 * Mismer.D  	version 1.6	11 Oct 1995	: Ajout appel fonct								on init Acknoledge alarme
 * Mismer.D	version 1.7	28 Oct 1996	: Modif pour Alerte PAU HS (DEM/1308)
 * Mismer.D	version 1.8	05 Dec 1996	: Suite modif pour Alerte PAU HS (DEM/1308)
 * Mismer.D	version 1.9	21 Jan 1998	: Modif pour basculement RAU PC2 (DEM/1542)
 * Diaz.L	version 1.10    09 Oct 1998     : Modif du au PC Niv.2
 * PNI		v1.15		07/07/2008	: ajout de SPDA pour l'appel a XZAE103 DEM790
 * PNI		23/06/09 v1.13	: renommage SPDA en SPPR DEM887
 * JMG		02/10/09 v1.14 : secto DEM887
 * JMG		18/8/16	1.14 : ajout basculement troncon
 * RGR 17/05/19 : DEM1331 - Ajout Mecanisme KEEPALIVE 1.15
 * ABE		07/04/21	: Ajout PAU DEM-SAE244 1.16
 * ABE		08/06/21	: Fix PAU DEM-SAE244 1.17
 * PNI		16/06/21 	: diminution du niveau de traces  1.18
 * JPL	23/11/21 : Consultation echeance minuterie et traitement des alarmes en fin de boucle principale (SAE_339)  1.19
------------------------------------------------------*/

/* fichiers inclus */

#include <xzsem.h>
#include "erau_dir.h"

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
        { XDM_ECMD_EQPT, 		(void (*)())er_controle_cb },
        { XDM_EINIT_ABT, 		(void (*)())er_Init_IHM_Mes_RAU_cb },
        { XDM_ECMD_RAU, 	        (void (*)())er_Cmd_RAU_cb },
        { XDM_IETAT_DISPO_EQT, 	(void (*)())er_Etat_PAU_cb },
        { XDM_ELCR, 			(void (*)())er_CMD_LCR },
        { XDM_ECMD_BASC_RAU, 	(void (*)())erau_basc_rau_cb },
	{ XDM_ECMD_SONO,	(void (*)())er_Cmd_SONO_cb }
    };

int			vg_SockTIM=0;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/* 
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
  int			vl_indice_callback, vl_Type = 0;


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
    exit( XDC_NOK );
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

  /*A
   * Connexion socket en mode client pour communiquer avec la tache TESRV
   */
  if ( ex_cnx_client ( 	XZEXC_ADR_SOCK_TERAU,
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
  if ( ex_cnx_client ( 	XZEXC_ADR_SOCK_RAUTIM,
      XZEXC_ADR_SOCK_TETIM_SRV,
      &vg_SockMask,
      &vg_SockTIM ) != XDC_OK )
  {
    /*B Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_FATAL, "Impossible de creer la connexion client  ." );
  }

  XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_MESSAGE,XZSTC_NECRAN);

  /*A
   * Appel de MCFG fonction ec_init
   */
  XZSA_17TypeMachine ( vg_NomMachine, &vl_Type );
  if (strcmp(vg_NomSite,XDC_NOM_SITE_CI))
  {
    /* appel a XZAE_103Abt_Presence_Op_PCsimpl*/
    if (XZAE103_Abt_Presence_Op_PCsimpl(XDC_DEBUT_ABONNEMENT_EQUEXT,er_Presence_PCS) != XDC_OK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING," pb dans l'appel a XZAE103_Abt_Presence_Op_PCsimpl");
    }
    vg_donsysrau.PC2 = XDC_VRAI;
  }
  else
  {
    vg_donsysrau.PC2 = XDC_FAUX;
  }
  if ( ec_init(vg_NomMachine) != XDC_OK )
  {
    /*B Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_FATAL, "Init configuration impossible" );
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
   * Abonnement aux Datagroup RAU
   */
  /* Messages de controle : XDG_ECMD_RAU_<NomSite> */
  XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_RAU, vg_NomSite) ;
  /* joint ce datagroup */
  if ( !XDG_JoindreDG( pl_NomDG ) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_FATAL,
        "Abonnement au Datagroup %s non effectue ", pl_NomDG );
  }

  /* Messages des alarmes PAU : XDG_ECMD_RAU */
  /* joint ce datagroup */
  if ( !XDG_JoindreDG( XDG_ECMD_RAU ) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_FATAL,
        "Abonnement au Datagroup %s non effectue ", XDG_ECMD_RAU );
  }

  /* Messages de controle : XDG_ECMD_RAU_<NomMachine> */
  XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_RAU, vg_NomMachine) ;
  /* joint ce datagroup */
  if ( !XDG_JoindreDG( pl_NomDG ) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_FATAL,
        "Abonnement au Datagroup %s non effectue ", pl_NomDG );
  }

  /* Messages de demande d'etat PAU : XDG_EINIT_ABT_RAU_<NomSite> */
  XDG_EncodeDG2 ( pl_NomDG, XDG_EINIT_ABT_RAU, vg_NomSite ) ;
  /* joint ce datagroup */
  if ( !XDG_JoindreDG( pl_NomDG ) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_FATAL,
        "Abonnement au Datagroup %s non effectue ", pl_NomDG );
  }

  /*A 
  *Abonnement aux Datagroup RAU SONO
  */ 
  /* Messages de controle : XDG_ECMD_SONO_<NomSite> */    
  XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_SONO, vg_NomSite) ;  
  /* joint ce datagroup */ 
  if ( !XDG_JoindreDG( pl_NomDG ) ) 
  { 
    /* Affichage Trace */ 
    XZST_03EcritureTrace(XZSTC_FATAL, 
        "Abonnement au Datagroup %s non effectue ", pl_NomDG ); 
  } 
 
  /* Messages de controle : XDG_ECMD_SONO_<NomMachine> */ 
  XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_SONO, vg_NomMachine) ; 
  /* joint ce datagroup */ 
  if ( !XDG_JoindreDG( pl_NomDG ) ) 
  { 
    /* Affichage Trace */ 
    XZST_03EcritureTrace(XZSTC_FATAL, 
        "Abonnement au Datagroup %s non effectue ", pl_NomDG ); 
  } 								
	



  /* Abonnement au DataGroup KEEPALIVE */
  XZSC_31AbtKeepAlive(vg_NomSite);

  /*A
   * Demande des timers cycliques
   */

  /* Programation timer 1mn */
  ETIM_REVEIL_CYCLIQUE ( ERAUC_REVEIL_1MN, "RAU_Timer_1mn", vg_SockTIM, pl_MsgTimer )

  /* Programation de l'heure de la synchro date */
  ETIM_SYNCHRO_DATE ( XZEXC_HEURE_SYNCHRO, "RAU_Timer_Synchro", vg_SockTIM, pl_MsgTimer )

  ETIM_REVEIL_CYCLIQUE(vg_donsysrau.Config.TimeOutRetour,"ERAU_BASCUL_GN",vg_SockTIM,pl_MsgTimer);


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

