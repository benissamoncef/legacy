/* Fichier : erdt_dir.c
------------------------------------------------------
 * GTIE *  PROJET MIGRAZUR
------------------------------------------------------
 * SOUS-SYSTEME : EQUEXT
------------------------------------------------------
 * MODULE TERDT * FICHIER erdt_dir.c
------------------------------------------------------
 * DESCRIPTION DU MODULE :
 *
 *  < Module directeur de la tache TERDT >
 *
------------------------------------------------------
 * HISTORIQUE :
 *
 * Volcic.F	version 1.1	05 Dec 1994	: Creation
 * Volcic.F	version 1.2	28 Jan 1995	:
 * Volcic.F	version 1.3	13 Fev 1995	:
 * Volcic.F	version 1.4	20 Mar 1995	: Modif Nom Cstes TIMER
 * Volcic.F	version 1.5	20 Mar 1995	:
 * Volcic.F	version 1.6	09 Avr 1995	:
 * Volcic.F	version 1.7	21 Avr 1995	: Reprise entete, historique
 * Volcic.F	version 1.8	07 Jun 1995	: Ajout definition callback et de l'abonnement pour le mode lent
 * Volcic.F	version 1.9	03 Jui 1995	: Modif recuperation machine suppleante
 * Mismer.D	version 1.10	31 Aou 1995	: Correction pour socket RTServer
 * Volcic.F	version 1.11	03 Jui 1995	: Modif recuperation machine suppleante
 * Volcic.F	version 1.12	03 Jui 1995	: Modif recuperation machine suppleante
 * Volcic.F	version 1.13	02 Oct 1995	: Modif init horodate MB
 * Mismer.D  	version 1.14	11 Oct 1995	: Ajout appel fonction init Acknoledge alarme
 * Volcic.F	version 1.15	20 Oct 1995	: Modif init horodate extraopolation MB
 * Volcic.F	version 1.16	07 Nov 1995	: Ajout Abonnement datagroup XDG_EDEM_INDIV
 * Volcic.F	version 1.17	11 Dec 1995	: Modif vl_SockTIM par vg_SockTIM
 * Volcic.F	version 1.18	06 Sep 1996	: Modif Extension Ouest (RADT_W)
 * Mismer.D	version 1.20	16 Sep 1996	: Modif Extension Ouest Suite(RADT_W)
 * Mismer.D	version 1.21	03 Sep 1996	: Modif pour mesures pesage (DEM/1227)
 * Mismer.D	version 1.22	05 Dec 1996	: Modif pour radt virtuelle (DEM/1252)
 * Mismer.D	version 1.23	12 Dec 1996	: Calcul des niveaux de services pourstations virtuelles (DEM/1338)
 * JPL		17/03/11 : Migration architecture HP ia64 (DEM 975) : suppression warnings mineurs  1.24
 * JPL		11/01/12 : Prevision definition d'une trace precise des le demarrage (pour DEM 997)  1.25
 * JMG		04/07/12 : simulation DEM 1016
 * JMG		12/11/18 : ajout IP DEM1306 1.27
 * RGR 17/05/19 : DEM1331 - Ajout Mecanisme KEEPALIVE 1.29
 * AMI 28/01/21 : SAE_191: chantier date appel XZSM07
 * JPL	23/11/21 : Consultation echeance minuterie et traitement des alarmes en fin de boucle principale (SAE_339)  1.31
------------------------------------------------------*/

/* fichiers inclus */

#include <xzsem.h>
#include "xzsa.h"

#include "ex_mala.h"
#include "ex_mrtf.h"

#include "erdt_dir.h"
#include "erdt_tra.h"


/* definitions de constantes */
XDY_Booleen 	vg_mode_simu;

/* definitions de types locaux */

typedef struct {

  int		NumMsg;		/* Numero de message */
  XDY_Fonction	FoncCB;		/* Fonction a appeler */

} tm_callback;

/* definition de macro locales */
int vg_SockTEMIP;
/* declaration de variables locales */

static tm_callback pm_TabCB[] =
    {
        { XDM_ECMD_EQPT, 			(void (*)())er_controle_cb },
        { XDM_EMES_RADT_6mn, 		(void (*)())er_EMes_6mn_Voisin_cb },
        { XDM_EConf_Voie_RADT, 		(void (*)())er_Conf_Voie_RADT_cb },
        { XDM_EDEM_Indiv, 			(void (*)())er_Dem_Indiv_cb },
        { XDM_EInit_IHM_6gl_Voie, 		(void (*)())er_Init_IHM_6gl_Voie_cb },
        { XDM_EInit_IHM_6gl_Chaussee, 	(void (*)())er_Init_IHM_6gl_Chaussee_cb },
        { XDM_EInit_IHM_6mn_parVoie,	 	(void (*)())er_Init_IHM_6mn_parVoie_cb },
        { XDM_EInit_IHM_Liaison, 		(void (*)())ep_Init_IHM_Liaison_cb },
        { XDM_ELCR, 				(void (*)())er_CMD_LCR },
        { XDM_MODE_LENT, 			(void (*)())er_Mode_Lent_cb },
        { XDM_Acknoledge, 			(void (*)())er_Acknoledge_cb },
        { XDM_Acknoledge_Pesage, 		(void (*)())er_Acknoledge_Pesage_cb }
    };


/* declaration de fonctions internes */

/* definition de fonctions externes */
extern int er_traitement_cyclique ( int );


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
  ERDT_DONNEES_RADT 	*pl_listeRADT;
  int	 		vl_ValRet	= XDC_OK;
  int			vl_SockRet	= 0;
  int			vl_SockSRV	= 0;
  int			vl_resultat	= 0;
  XDY_Entier		vl_retcode	= 0;
  XDY_Datagroup		pl_DG;
  int			vl_indice_callback = 0;
//  time_t			vl_time	= 0;  SAE_191
 XDY_Horodate		vl_time = 0;
 int 			vl_jourSemaine = 0;


  /*A  Initialisation du contexte */
  vg_Contexte = XDC_JOUR;
  vg_SockTEMIP=-1;

  /*A  Recuperation du nom de la machine */
  strcpy ( vg_NomMachine, argv[1] );


  XZST_03EcritureTrace(XZSTC_FONCTION, "IN:main ");

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
      er_sortir,
      "/dev/null" ) != XDC_OK )
  {
    XZST_03EcritureTrace( XZSTC_FATAL, "erdt_dir : Ne peut ouvrir la connexion avec le RTserver." );
  }
  else
  {
    XZST_03EcritureTrace( XZSTC_DEBUG1, "erdt_dir : Ouverture de la connexion avec le RTserver." );
  }

  if ( !access("simuRADT", F_OK))
  {
    vg_mode_simu=TRUE;
    XZST_03EcritureTrace( XZSTC_WARNING, "mode SIMU");
  }
  else
    vg_mode_simu=FALSE;

  /*A Recuperation du site de la machine */
  if ( (vl_ValRet = XZSC_04NomSite(vg_NomSite)) != XDC_OK)
  {
    XZST_03EcritureTrace( XZSTC_FATAL, "erdt_dir : Recuperation du Nom du site impossible . " );
  }

  /*A Recuperation du type de la machine */
  if ( XZSA_17TypeMachine ( vg_NomMachine, &vg_Type_Machine ) != XDC_OK )
  {
    if ( XZSA_17TypeMachine ( vg_NomMachine, &vg_Type_Machine ) != XDC_OK)
      XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de trouve le type de la machine" );
  }

  if ( vg_Type_Machine == XZSCC_TYPEM_PCS )
    vg_Mac_Ext_Ouest = XDC_VRAI;

  /*A Si LT <> ext ouest */
  if (!vg_Mac_Ext_Ouest)
  {
    /*B  Recuperation du nom de la machine suppleante */
    if ( ex_lire_suppleant ( &vg_NomSuppleant ) != XDC_OK )
      XZST_03EcritureTrace( XZSTC_FATAL, "erdt_dir : Impossible de recuperer le nom de la machine suppleante de %s", vg_NomMachine );
    XZST_03EcritureTrace( XZSTC_INFO, "erdt_dir : Suppleant de %s = %s", vg_NomMachine, vg_NomSuppleant );
  }

  XZST_03EcritureTrace( XZSTC_INFO, "erdt_dir : Suppleant de %s = %s", vg_NomMachine, vg_NomSuppleant );

  /*A Rangement des sockets */
  ex_rep_socket ( argv[1] );

  /*A Init pour Acknoledge Alarme */
  ex_init_alarme ( argv[1], argv[0] );

  /*A Connexion socket en mode client pour communiquer avec la tache TESRV */
  if ( ex_cnx_client ( 	XZEXC_ADR_SOCK_TERDT,
      XZEXC_ADR_SOCK_TESRV,
      &vg_SockMask,
      &vl_SockSRV ) != XDC_OK )
  {
    XZST_03EcritureTrace( XZSTC_FATAL, "erdt_dir : Impossible de creer la connexion client avec TESRV  ." );
  }


  /*A Connexion socket en mode client pour communiquer avec la tache TETIM */
  if ( ex_cnx_client ( 	XZEXC_ADR_SOCK_RDTTIM,
      XZEXC_ADR_SOCK_TETIM_SRV,
      &vg_SockMask,
      &vg_SockTIM ) != XDC_OK )
  {
    XZST_03EcritureTrace( XZSTC_FATAL, "erdt_dir : Impossible de creer la connexion client avec TETIM ." );
  }

  /*A Si LT <> Simplifie => Connexion socket en mode client pour communiquer avec la tache TETDC */
  if (!vg_Mac_Ext_Ouest)
  {
    if ( ex_cnx_client ( XZEXC_ADR_SOCK_RDTTDC,
        XZEXC_ADR_SOCK_TETDC_SRV,
        &vg_SockMask,
        &vg_SockTDC ) != XDC_OK )
    {
      XZST_03EcritureTrace( XZSTC_FATAL, "erdt_dir : Impossible de creer la connexion client avec TETDC." );
    }
  }

//XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_DEBUG1|XZSTC_DEBUG2|XZSTC_DEBUG3|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE,XZSTC_NECRAN);
  /*A Appel de la fonction d'initialisation des tableaux de valeurs des mesures */
  er_init_tab_mes ( );

  /*A Appel de MCFG fonction er_init */
  if ( ( vl_retcode = er_init( vg_NomMachine ) ) != XDC_OK )
  {
    XZST_03EcritureTrace( XZSTC_FATAL, "erdt_dir : er_init retourne %d", vl_retcode );
  }

  /*A Armement des Callback sur reception des message */

  /*B Initialisation des callbacks de la tache en fonction du type de machine */
  if (!vg_Mac_Ext_Ouest)
  {
    for (vl_indice_callback = 0; vl_indice_callback < (sizeof(pm_TabCB)/sizeof(pm_TabCB[0])); vl_indice_callback++)
    {
      if(!TipcSrvProcessCbCreate(	XDM_IdentMsg(pm_TabCB[vl_indice_callback].NumMsg ) ,
          pm_TabCB[vl_indice_callback].FoncCB ,
          (T_CB_ARG)NULL) )
      {
        XZST_03EcritureTrace(XZSTC_FATAL,
            "erdt_dir : erreur init callback '%s'",
            pm_TabCB[vl_indice_callback].FoncCB);
      }
    }
  }
  else
  {
    for (vl_indice_callback = 0; vl_indice_callback < (sizeof(pm_TabCB)/sizeof(pm_TabCB[0])); vl_indice_callback++)
    {
      if ( ( pm_TabCB[vl_indice_callback].NumMsg != XDM_EInit_IHM_6gl_Voie ) &&
          ( pm_TabCB[vl_indice_callback].NumMsg != XDM_EInit_IHM_6gl_Chaussee ) &&
          ( pm_TabCB[vl_indice_callback].NumMsg != XDM_MODE_LENT ) )
      {
        if(!TipcSrvProcessCbCreate(	XDM_IdentMsg(pm_TabCB[vl_indice_callback].NumMsg ) ,
            pm_TabCB[vl_indice_callback].FoncCB ,
            (T_CB_ARG)NULL) )
        {
          XZST_03EcritureTrace(XZSTC_FATAL,
              "erdt_dir : erreur init callback '%s'",
              pm_TabCB[vl_indice_callback].FoncCB);
        }
      }
    }
  }

  /*A Abonnement sur le message XDM_ECMD_JourNuit */
  XDG_EncodeDG2 ( pl_DG, ERDTC_JOURNUIT_RADT, vg_NomMachine) ;
  if( ( vl_retcode = XZEA29_Abt_Etat_JourNuit(XZEAC_DEBUT_ABONNEMENT,pl_DG,er_journuit,vg_NomSite,&vl_resultat) ) != XDC_OK)
  {
    XZST_03EcritureTrace(XZSTC_FATAL,"TERDT : Impossible de s'abonner a JourNuit");
    vg_Contexte=XDC_JOUR;
  }

  /*A Si LT = PC simplifie */
  if (vg_Mac_Ext_Ouest)
  {
    /*B Abonnement aux etats de liaison entre les sites => appel de XZSE_Abt_Etat_Comm_Site */
    if( ( vl_retcode = XZSE_Abt_Etat_Comm_Site(XZEAC_DEBUT_ABONNEMENT,er_etat_liaison) ) != XDC_OK)
    {
      XZST_03EcritureTrace(XZSTC_FATAL,"TERDT : Impossible de s'abonner a EtatComm");
    }
  }

  /*A Abonnement aux Datagroup */

  /*B Demande des Mesures Individuelles : XDG_EDEM_INDIV_<NomMachine> */
  sprintf ( pl_DG, "%s_%s", XDG_EDEM_INDIV, vg_NomMachine) ;
  /* joint ce datagroup */
  if ( !XDG_JoindreDG( pl_DG ) )
  {
    XZST_03EcritureTrace(XZSTC_FATAL,"erdt_dir : Abonnement au Datagroup %s non effectue ", pl_DG );
  }

  if (!vg_Mac_Ext_Ouest)
  {
    /*B Mode Lent : XDG_MODE_LENT_<NomMachine> */
    sprintf ( pl_DG, "%s_%s", XDG_MODE_LENT, vg_NomMachine) ;

    /* joint ce datagroup */
    if ( !XDG_JoindreDG( pl_DG ) )
    {
      XZST_03EcritureTrace(XZSTC_FATAL,"erdt_dir : Abonnement au Datagroup %s non effectue ", pl_DG );
    }

  }

  /*A Messages de demande de mesure RADT : XDG_EInit_IHM_RADT_NomSite */
  sprintf ( pl_DG, "%s_%s", XDG_EInit_IHM_RADT, vg_NomSite) ;

  /* joint ce datagroup */
  if ( !XDG_JoindreDG( pl_DG ) )
  {
    XZST_03EcritureTrace(XZSTC_FATAL,"erdt_dir : Abonnement au Datagroup %s non effectue ", pl_DG );
  }

  /*A Messages de demande de mesure RADT : XDG_EInit_IHM_RADT */
  /* joint ce datagroup */
  if ( !XDG_JoindreDG( XDG_EInit_IHM_RADT ) )
  {
    XZST_03EcritureTrace(XZSTC_FATAL,"erdt_dir : Abonnement au Datagroup %s non effectue ", pl_DG );
  }

  /*B Messages de controle : XDG_ECMD_RADT_<NomMachine> */
  sprintf ( pl_DG, "%s_%s", XDG_ECMD_RADT, vg_NomMachine) ;

  /* joint ce datagroup */
  if ( !XDG_JoindreDG( pl_DG ) )
  {
    XZST_03EcritureTrace(XZSTC_FATAL,"erdt_dir : Abonnement au Datagroup %s non effectue ", pl_DG );
  }

  /*B Messages de commance de contraste : XDG_EInit_IHM_Liaison_<NomMachine> */
  XDG_EncodeDG2 ( pl_DG, XDG_EInit_IHM_Liaison, vg_NomMachine) ;
  /* joint ce datagroup */
  if ( !XDG_JoindreDG( pl_DG ) )
  {
    XZST_03EcritureTrace(XZSTC_FATAL,"Abonnement au Datagroup %s non effectue ", pl_DG );
  }

  /*A Messages de configuration de voie : XDG_ECONF_VOIE_RADT_<NomMachine> */
  sprintf ( pl_DG, "%s_%s", XDG_ECONF_VOIE_RADT, vg_NomMachine) ;

  /* joint ce datagroup */
  if ( !XDG_JoindreDG( pl_DG ) )
  {
    XZST_03EcritureTrace(XZSTC_FATAL,"erdt_dir : Abonnement au Datagroup %s non effectue ", pl_DG );
  }

  /* Abonnement au DataGroup KEEPALIVE */
  XZSC_31AbtKeepAlive(vg_NomSite);

  /*A Definition et initialisation des timers cycliques et Demande de l'heure courante */
  
  // time ( &vl_time );  / SAE_191 modif chantier date
  XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME, &vl_jourSemaine, &vl_time );
  vg_Date_Changement = (XDY_Horodate) vl_time;
  vg_Date_Actuelle   = (XDY_Horodate) vl_time;

  /*B Pour tous les RADT de la machine => demande ST */
  for ( pl_listeRADT = pg_debutRADT; pl_listeRADT != NULL; pl_listeRADT = pl_listeRADT->Suivant )
  {

    /*B Abonnement pour l'Acknoledge des Mesures 6 Min : XDG_ACKNOLEDGE_R<NumRADT> */
    sprintf ( pl_DG, "%s_R%d", XDG_ACKNOLEDGE, pl_listeRADT->ConfRADT.NumeroStation ) ;

    /* joint ce datagroup */
    if ( !XDG_JoindreDG( pl_DG ) )
    {
      XZST_03EcritureTrace(XZSTC_FATAL,"erdt_dir : Abonnement au Datagroup %s non effectue ", pl_DG );
    }

    if ( pl_listeRADT->ConfRADT.Liaison > 0 )
    {
      /*B Lecture de l'horodate des dernieres mesures recues dans un fichier */
      er_Lecture_derniere_heure ( pl_listeRADT );

      /*B Initialisation de pl_listeRADT->Don_HisRADT.Demande_Histo */
      pl_listeRADT->Don_HisRADT.Demande_Histo = XDC_FAUX;

      /*B demande de status complet */
      if (!strcmp(pl_listeRADT->ConfRADT.AdresseIP,XZECC_IP_VIDE)) {
        if ( ( vl_retcode = er_EnvoyerTrame("ST",
            XZEXC_TRAME_STAT_COMP,
            pl_listeRADT->ConfRADT.RGS,
            XZEXC_NOCMD,
            XZEXC_PRIORITEFAIBLE,
            pl_listeRADT->Socket ) ) != XDC_OK )
        {
          XZST_03EcritureTrace( XZSTC_WARNING,"EnvoyerTrame retourne %d", vl_retcode );
        }
      }
      else
      {
        if ( ( vl_retcode = EnvoyerTrameIP("ST",
                                           XZEXC_TRAME_STAT_COMP,
                                           pl_listeRADT->ConfRADT.AdresseIP,
                                           pl_listeRADT->ConfRADT.Port,
                                           pl_listeRADT->ConfRADT.RGS,
                                           XZEXC_NOCMD,
                                           XZEXC_PRIORITEFAIBLE,
                                           pl_listeRADT->Socket ) ) != XDC_OK )
        {
          XZST_03EcritureTrace( XZSTC_WARNING,"EnvoyerTrame retourne %d", vl_retcode );
        }
      }
    }

  } /* Fin demande ST a l'init */

  /*A Attente des messages sur les sockets (RTserver, d'ecoute et de donnees */
  while (1)
  {
    if ( ex_attendre_msg ( XZEXC_SOCKET_NULL, &vg_SockMask, &vl_SockRet ) != XDC_OK )
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "erdt_dir : Probleme retour fonction select  ." );
      ex_ConnexionSocketRTserver ( &vg_SockRT, vl_SockRet, &vg_SockMask );
    }
    else
    {
      ex_ConnexionSocketRTserver ( &vg_SockRT, 0, &vg_SockMask );

      /*B Si tous les timers n'ont pas etes enclenches => appel de la fonction er_Lancement_Timer */
      if ( vg_Timers_Enclenches == XDC_FAUX )
      {
        er_Lancement_Timer ( vg_SockTIM );
      }

      /*B Si une socket a ete activee */
      if ( (vl_SockRet != XZEXC_NO_EVT) && (vl_SockRet != vg_SockRT) )
      {
        /*B Selon le type de socket */
        /*B Si Socket TETIM (retour Timer) */
        if ( vl_SockRet == vg_SockTIM )
        {
          /*B Appel de la fonction er_traitement_cyclique de MCYC */
          er_traitement_cyclique ( vg_SockTIM );
        }
        else
        {
          /*B Socket TESRV (Init ou Arret tache) */
          if ( vl_SockRet == vl_SockSRV )
          {
            /*B Appel de la fonction er_controle de MCFG */
            er_controle ( vl_SockSRV, vg_NomMachine );
          }
          /*B Sinon */
          else
          {
            /*B Appel de la fonction er_traitement_trame */
            er_traitement_trame( vl_SockRet );
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

