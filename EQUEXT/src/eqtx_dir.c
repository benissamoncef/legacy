/* Fichier : eqtx_dir.c
-------------------------------------------------------------------------------------------
 * GTIE / GRANIOU *  PROJET MIGRAZUR
-------------------------------------------------------------------------------------------
 * SOUS-SYSTEME : EQUEXT
-------------------------------------------------------------------------------------------
 * MODULE TEQTX * FICHIER eqtx_dir.c
-------------------------------------------------------------------------------------------
 * DESCRIPTION DU MODULE :
 *
 * C'est le module directeur de la tache TEQTX
 *
-------------------------------------------------------------------------------------------
 * HISTORIQUE :
 *
 * Orengo.A	version 1.1	29 Oct 1996	: Creation
 * Orengo.A	version 1.2	07 Nov 1996	: Modification du nom des equipements
 *
 * JPL		25/02/11 : Migration architecture HP ia64 (DEM 975) : suppression warnings mineurs  1.4
 * RGR 17/05/19 : DEM1331 - Ajout Mecanisme KEEPALIVE 1.5
 * JPL	23/11/21 : Consultation echeance minuterie et traitement des alarmes en fin de boucle principale (SAE_339)  1.6
-----------------------------------------------------------------------------------------*/

/* fichiers inclus */
/* --------------- */

#include <xzsem.h>
#include	<xdy.h>
#include	<xdc.h>
#include	<xzst.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include 	"ex_msok.h"
#include	"ex_mala.h"
#include 	"ex_mlcr.h"
#include	"ex_mrtf.h"
#include 	"etim_don.h"

#include	"eqtx_don.h"
#include	"eqtx_cfg.h"
#include	"eqtx_cmd.h"
#include	"eqtx_cyc.h"
#include	"eqtx_tra.h"

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
            (void (*)())er_controle_cb
        },
        {
            /* Declaration du CallBack de la demande Fenetre LCR */

            XDM_ELCR,
            (void (*)())er_CMD_LCR
        },
        {
            /* Declaration du CallBack acquitement de l'envoi des donnees meteo */

            XDM_ACQ_DONNEES_METEO_QTX,
            (void (*)())er_ACQ_donnees_meteo_QTX_cb
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

      strcpy( vg_donnees_systeme_qtx.NomMachine, "POLTM11");
      break;

    case 2:
    case 3:
      /* Fonctionnement mormal de la tache */

      strcpy( vg_donnees_systeme_qtx.NomMachine, argv[1] );
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
    /*B Affichage Trace */
    XZST_03EcritureTrace( XZSTC_DEBUG1,
        "Ouverture de la connexion avec le RTserver." );
  }

  /*A
   * Recuperation du site de la machine
   */
  if ( (vl_ValRet = XZSC_04NomSite(vg_donnees_systeme_qtx.NomSite)) != XDC_OK)
  {
    /* Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_FATAL,"Recuperation du Nom du site impossible . " );
  }

  /*A Creation du repertoire pour les sockets */
  ex_rep_socket( vg_donnees_systeme_qtx.NomMachine );

  /*A Init pour Acknoledge Alarme */
  ex_init_alarme( vg_donnees_systeme_qtx.NomMachine, argv[0] );

  /*A
   * Connexion socket en mode client pour communiquer avec la tache TESRV
   */
  if( ex_cnx_client( XZEXC_ADR_SOCK_TEQTX,XZEXC_ADR_SOCK_TESRV,
      &vg_SockMask,&vl_SockSRV ) != XDC_OK )
  {
    /*B Ecriture Trace */
    XZST_03EcritureTrace(XZSTC_FATAL,"Impossible de creer la connexion client  ." );
  }

  /*A
   * Connexion socket en mode client pour communiquer avec la tache TETIM
   */
  if( ex_cnx_client ( XZEXC_ADR_SOCK_QTXTIM,XZEXC_ADR_SOCK_TETIM_SRV,
      &vg_SockMask,&vg_SockTIM ) != XDC_OK )
  {
    /*B Ecriture Trace */
    XZST_03EcritureTrace(XZSTC_FATAL,"Impossible de creer la connexion client  ." );
  }

  /*A
   * Appel de MCFG fonction ec_init
   */
  if ( ec_init(vg_donnees_systeme_qtx.NomMachine) != XDC_OK )
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
  /* Messages de controle : XDG_ECMD_QTX_<NomMachine> */

  XDG_EncodeDG2( pl_NomDG, XDG_ECMD_QTX, vg_donnees_systeme_qtx.NomMachine);

  /* joint ce datagroup */
  if( !XDG_JoindreDG( pl_NomDG ) )
  {
    XZST_03EcritureTrace(XZSTC_FATAL, "Abonnement au Datagroup %s non effectue ", pl_NomDG );
  }

  /* Abonnement au DataGroup KEEPALIVE */
  XZSC_31AbtKeepAlive(vg_donnees_systeme_qtx.NomSite);

  /*A
   * Demande des timers cycliques
   */

  /* Programation timer 1mn */
  ETIM_REVEIL_CYCLIQUE( EQTXC_REVEIL_1H, CLE_CYCLE_TIMER_QTX_1H,
      vg_SockTIM, pl_MsgTimer )

  lancement_timer_eqtx( EQTXC_REVEIL_1MN, CLE_CYCLE_TIMER_QTX_1H );


  /* Programation de l'heure de la synchro date */
  ETIM_SYNCHRO_DATE ( XZEXC_HEURE_SYNCHRO, CLE_SYNCHRO_HEURE_QTX,
      vg_SockTIM, pl_MsgTimer )

  /* Programation des heures de la demande donnees */

  for(i=0; i!=24; i++)
  {
    sprintf( vl_chaine_travail, "%02d%s", i, EQTXC_DEMANDE_HEURE_X_DONNEES_QTX);
    sprintf( vl_chaine_travail_cle, "%s%02d", CLE_DEMANDE_HEURE_X_DONNEES_QTX, i);
    ETIM_SYNCHRO_DATE (  vl_chaine_travail, vl_chaine_travail_cle,
        vg_SockTIM, pl_MsgTimer )
  }
  /* Lancement du timer pour une demande immediate des donnees */

  i=99;
  sprintf( vl_chaine_travail_cle, "%s%02d", CLE_DEMANDE_HEURE_X_DONNEES_QTX, i);
  lancement_timer_eqtx( EQTXC_REVEIL_1MN, vl_chaine_travail_cle);

  /* Timer pour le test de la ligne avec le protocole Esclave */

  for(i=0; i!=NB_EQT_MAX_TEQTX; i++)
  {
    if( vg_donnees_systeme_qtx.DonneesEqt[i].ChampValide == Valide)
    {
      sprintf( vl_chaine_travail_cle, "%s%02d",
          CLE_CYCLE_TIMER_QTX_LIGNE_ESCLAVE, i);
      lancement_timer_eqtx( EQTXC_REVEIL_LIGNE_ESCLAVE, vl_chaine_travail_cle);
    }
  }

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

              ec_controle( vl_SockSRV, vg_donnees_systeme_qtx.NomMachine );
            }
            else
            {
              /* Boucle sur l'ensemble des Sockets Maitre et Esclave */

              for( i=0; i!=NB_EQT_MAX_TEQTX; i++)
              {
                if( vl_SockRet ==
                    vg_donnees_systeme_qtx.DonneesEqt[i].SocketMaitre
                    && vg_donnees_systeme_qtx.DonneesEqt[i].ChampValide==Valide
                    && vg_donnees_systeme_qtx.DonneesEqt[i].LSMaitreValide
                    ==Valide)
                {
                  /* Appel traitement de la trame protocole Maitre */

                  em_traitement_trame_maitre( vl_SockRet, i);
                  break;
                }
                else
                {
                  if( vl_SockRet ==
                      vg_donnees_systeme_qtx.DonneesEqt[i].SocketEsclave
                      && vg_donnees_systeme_qtx.DonneesEqt[i].ChampValide
                      ==Valide)
                  {
                    /* Appel traitement de la trame protocole Esclave */

                    em_traitement_trame_esclave( vl_SockRet, i);
                    break;
                  }
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

