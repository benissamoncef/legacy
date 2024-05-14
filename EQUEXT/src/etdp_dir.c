/* Fichier : etdp_dir.c
-------------------------------------------------------------------------------------------
 * GTIE  *  PROJET MIGRAZUR
-------------------------------------------------------------------------------------------
 * SOUS-SYSTEME : EQUEXT
-------------------------------------------------------------------------------------------
 * MODULE TETDP * FICHIER etdp_dir.c
-------------------------------------------------------------------------------------------
 * DESCRIPTION DU MODULE :
 *
 * C'est le module directeur de la tache TETDP
 *
-------------------------------------------------------------------------------------------
 * HISTORIQUE :
 *
 * Mismer.D	version 1.1	13 Fev 1997	: Creation (DEM/1396)
 * Mismer.D	version 1.2	25 Mar 1997	: Affichage TDP sur PMV (DEM/1417)
 * Mismer.D	version 1.3	23 Jui 1997	: Ajout datagroup pour fenetre LCR (DEM/1455)
 *
 * JPL		07/03/11 : Migration architecture HP ia64 (DEM 975) : suppression warnings mineurs  1.4
 * JPL		17/03/11 : Migration HP ia64 (DEM 975) : interface SS68 : type socket et args. des fonctions IPC  1.5
 * JMG		06/02/12 : tdp etendu dem/1014 1.6
 * JMG	05/06/12 : ajout callback sur Msg TDP_PMVA (DEM/1014 PMA)
 * RGR 17/05/19 : DEM1331 - Ajout Mecanisme KEEPALIVE 1.8
 * JMG 	13/11/19 : tdp surcharge DEM1365 1.9
 * JPL	23/11/21 : Consultation echeance minuterie et traitement des alarmes en fin de boucle principale (SAE_339)  1.10
-----------------------------------------------------------------------------------------*/

/* fichiers inclus */
/* --------------- */

#include <xzsem.h>
#include	<xdy.h>
#include	<xdc.h>
#include	<xzst.h>
#include	<xzao.h>
#include	<xzez.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include 	"ex_msok.h"
#include 	"ex_mlcr.h"
#include 	"ex_mala.h"
#include 	"ex_mrtf.h"
#include 	"etim_don.h"

#include	"etdp_don.h"
#include	"etdp_cfg.h"
#include	"etdp_cmd.h"
#include	"etdp_cyc.h"
#include	"etdp_tra.h"

/* definitions de constantes */
/* ------------------------- */

/* definitions de types locaux */
/* --------------------------- */

typedef struct
{
  int		NumMsg;		/* Numero de message */
  XDY_Fonction	FoncCB;		/* Fonction a appeler */
}tm_callback;

/* definition de macro locales */
/* --------------------------- */

/* declaration de variables */
/* ------------------------ */

int			vg_SockTIM=0;

XZEXT_MASK *		vg_SockMask = NULL;
T_SOCK			vg_SockRT;

/* declaration de variables locales */
/* -------------------------------- */


static tm_callback pm_TabCB[] =
    {
        {
            /* Declaration du CallBack de disponibilite de l'equipement */

            XDM_ECMD_EQPT,
            (void (*)())et_controle_cb
        },
        {
            XDM_ETDP_SURCHARGE,
            (void (*)())et_surcharge
        },
        {
            XDM_ETDP_INIT_SURCHARGE,
            (void (*)())et_init_surcharge
        },
        {
            /* Declaration du CallBack de la demande Fenetre LCR */

            XDM_ELCR,
            (void (*)())et_CMD_LCR
        },
        {
            /* Declaration du CallBack de la demande init mesure TDP */

            XDM_EINIT_ABT,
            (void (*)())et_Init_IHM_Mes_TDP_cb
        },
        {
            /* Declaration du CallBack de la commande PMV avec TDP */

            XDM_ETDP_PMV,
            (void (*)())et_cmd_PMV_cb
        },
        {
            /**************************************/
            /* Declaration du CallBack de la commande PMV avec TDP */
            XDM_ETDP_PMVA,
            (void (*)())et_cmd_PMVA_cb
            /**************************************/
        },
        {
            /* Declaration du CallBack de la demande de calcul TDP */

            XDM_ETDP_IHM,
            (void (*)())et_demande_TDP_cb
        },
        {
            /**************************************/
            /* Declaration du CallBack de la demande de calcul TDP PMVA*/
            XDM_ETDP_IHM_PMVA,
            (void (*)())et_demande_TDP_PMVA_cb
            /**************************************/
        },
        {
            /* Declaration du CallBack de la reception des mesures des zones voisines */

            XDM_EMES_TDP_6MN_PAR_ZONE,
            (void (*)())et_TDP_voisin_cb
        },
        {
            /* Declaration du CallBack de la reception des compte rendu action PMV */

            XDM_CR_TDP,
            (void (*)())et_CompteRendu_cb
        },

        {
            /**************************************/
            /* Declaration du CallBack de la reception des compte rendu action PMV */
            XDM_CR_TDP_PMVA,
            (void (*)())et_CompteRendu_PMVA_cb
            /**************************************/
        },
        {
            /* Declaration du CallBack acquitement de la mise en base des donnee TDP */

            XDM_ACQ_DONNEES_TDP,
            (void (*)())et_ACQ_donnees_TDP_cb
        }
    };

/*X*/
/* 
-------------------------------------------------------------------------------------------
 * SERVICE RENDU :
 * Programme principal
-------------------------------------------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

int main( argc, argv )

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
 * Initialisations :connexion rtserver, connection avec les taches TESRV
 * TETIM, init des structures en memoire, gestion des callbacks
 * init des timer cycliques
 * Boucle sur les sockets et gestion des messages recus
 *
-----------------------------------------------------------------------------------------*/

int	argc;
char	**argv;
{
  int vl_ValRet	= XDC_OK;
  int vl_SockRet	= 0;
  int vl_SockSRV	= 0;
  XZEXT_MSG_SOCKET pl_MsgTimer;
  XDY_Datagroup pl_NomDG;
  int vl_indice_callback;
  char vl_chaine_travail[10];
  char vl_chaine_travail_cle[100];
  int i;

  /* Recuperation du nom de la machine */

  switch(argc)
  {
    case 1:
      /* Pour la phase de debug */

      strcpy( vg_ConfigTDP.NomMachine, "POLTM11");
      break;

    case 2:
    case 3:
      /* Fonctionnement mormal de la tache */

      strcpy( vg_ConfigTDP.NomMachine, argv[1] );
      break;

    default:
      /* Fonctionnement amormal de la tache */

      exit(0);
      break;
  }

	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

  /*A
   * Connexion au RTserver et initialisation des variables
   */
  if( XZSC_01InitCnxSup( NULL,argv[ 0 ],XZSCC_INTERDIRE,XZSCC_AUTORISER,
      XZSCC_INTERDIRE,ec_sortir,"/dev/null" ) != XDC_OK )
  {
    /*B Affichage Trace */
    XZST_03EcritureTrace( XZSTC_FATAL,
        "Ne peut ouvrir la connexion avec le RTserver." );
    exit( XDC_NOK );
  }
  else
  {
    XZST_03EcritureTrace( XZSTC_DEBUG1, "Ouverture de la connexion avec le RTserver." );
  }

  /*A
   * Recuperation du site de la machine
   */
  if ( (vl_ValRet = XZSC_04NomSite(vg_ConfigTDP.NomSite)) != XDC_OK)
  {
    /* Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_FATAL,"Recuperation du Nom du site impossible . " );
  }

  /*A Creation du repertoire pour les sockets */
  ex_rep_socket( vg_ConfigTDP.NomMachine );

  /*A Init pour Acknoledge Alarme */
  ex_init_alarme( vg_ConfigTDP.NomMachine, argv[0] );

  XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE,XZSTC_NECRAN);


  /*A
   * Connexion socket en mode client pour communiquer avec la tache TESRV
   */
  if( ex_cnx_client( XZEXC_ADR_SOCK_TETDP,XZEXC_ADR_SOCK_TESRV,
      &vg_SockMask,&vl_SockSRV ) != XDC_OK )
  {
    /*B Ecriture Trace */
    XZST_03EcritureTrace(XZSTC_FATAL,"Impossible de creer la connexion client  ." );
  }

  /*A
   * Connexion socket en mode client pour communiquer avec la tache TETIM
   */
  if( ex_cnx_client ( XZEXC_ADR_SOCK_TDPTIM,XZEXC_ADR_SOCK_TETIM_SRV,
      &vg_SockMask,&vg_SockTIM ) != XDC_OK )
  {
    /*B Ecriture Trace */
    XZST_03EcritureTrace(XZSTC_FATAL,"Impossible de creer la connexion client  ." );
  }

  /*A
   * Appel de MCFG fonction ec_init
   */
  if ( ec_init(vg_ConfigTDP.NomMachine) != XDC_OK )
  {
    /*B Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_FATAL, "Init configuration impossible" );
  }


  /*A
   * Armement des Callback sur reception des messages
   */

  /* Initialisation des callbacks de la tache */

  for( vl_indice_callback = 0;
      vl_indice_callback < (sizeof(pm_TabCB)/sizeof(pm_TabCB[0])); vl_indice_callback++)
  {
    if(!TipcSrvProcessCbCreate(	XDM_IdentMsg(pm_TabCB[vl_indice_callback].NumMsg ) ,
        pm_TabCB[vl_indice_callback].FoncCB ,(T_CB_ARG)NULL) )
    {
      XZST_03EcritureTrace(XZSTC_FATAL,
          "erreur init callback '%s'",pm_TabCB[vl_indice_callback].FoncCB);
    }
  }

  /*A
   * Abonnement aux Datagroup
   */
  /* Messages de controle : XDG_ECMD_TDP_<NomMachine> */

  XDG_EncodeDG2( pl_NomDG, XDG_ECMD_TDP, vg_ConfigTDP.NomMachine);

  XDG_JoindreDG(XDG_ETDP_SURCHARGE);

  /* joint ce datagroup */
  if( !XDG_JoindreDG( pl_NomDG ) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_FATAL,
        "Abonnement au Datagroup %s non effectue ", pl_NomDG );
  }

  XDG_EncodeDG2( pl_NomDG, "ECMD_UGTP", vg_ConfigTDP.NomMachine);

  /* joint ce datagroup */
  if( !XDG_JoindreDG( pl_NomDG ) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_FATAL,
        "Abonnement au Datagroup %s non effectue ", pl_NomDG );
  }

  /* Messages d'abonnement : XDG_EINIT_ABT_TDP */
  sprintf(pl_NomDG,"%s_%d",XDG_EINIT_ABT_TDP,XZSC_22NumeroSiteLocal());
  if( !XDG_JoindreDG( pl_NomDG ) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_FATAL,
        "Abonnement au Datagroup %s non effectue ", XDG_EINIT_ABT_TDP );
  }

  /* Abonnement au DataGroup KEEPALIVE */
  XZSC_31AbtKeepAlive(vg_ConfigTDP.NomSite);

  /*A
   * Demande des timers cycliques
   */

  lancement_timer_etdp( ETDPC_REVEIL_6MN, ETDPC_TIMER_6MN );
  lancement_timer_etdp( 0, ETDPC_TIMER_1H );


  /* Programation de l'heure de la synchro date */
  ETIM_SYNCHRO_DATE ( 	XZEXC_HEURE_SYNCHRO,
      ETDPC_TIMER_SYNCHRO,
      vg_SockTIM,
      pl_MsgTimer )


  /*A Attente des messages sur les sockets (RTserver, d'ecoute et de donnees) */

  while (1)
  {
    if( ex_attendre_msg( XZEXC_SOCKET_NULL, &vg_SockMask, &vl_SockRet ) != XDC_OK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "Probleme retour fonction select  ." );
      ex_ConnexionSocketRTserver ( &vg_SockRT, vl_SockRet, &vg_SockMask );
    }
    else
    {
      ex_ConnexionSocketRTserver( &vg_SockRT, 0, &vg_SockMask );

      /* Si une socket a ete activee */

      if( vl_SockRet != XZEXC_NO_EVT )
      {
        /* Selon le type de socket */

        /* Socket RT server */

        if( vl_SockRet == vg_SockRT )
        {
          /* Le message est lu par les primitives standard de RTWorks */
        }
        else
        {
          /* Socket TETIM (retour Timer) */

          if( vl_SockRet == vg_SockTIM )
          {
            /* Appel de la fonction ec_traitement_cyclique de MCYC */

            ey_traitement_cyclique( vg_SockTIM );
          }
          else
          {
            /* Socket TESRV (Init ou Arret tache) */

            if( vl_SockRet == vl_SockSRV )
            {
              /* Appel de la fonction ec_configuration de MCFG */

              ec_controle( vl_SockSRV, vg_ConfigTDP.NomMachine );
            }
            else
            {
              /* Boucle sur l'ensemble des Sockets LS */

              for( i=0; i!=ETDPC_NB_MAX_UGTP; i++)
              {
                if ( (vl_SockRet == vg_DonneesUGTP[i].SocketLS) &&
                    (vg_DonneesUGTP[i].ChampValide == Valide) )
                {
                  /* Appel traitement de la trame protocole LCR */
                  em_traitement_trame ( vl_SockRet, i);
                  break;
                }
              }
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

