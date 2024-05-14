/* Fichier : $Id: ereg_dir.c,v 1.5 2020/11/03 18:12:26 pc2dpdy Exp $        Release : $Revision: 1.5 $        Date : $Date: 2020/11/03 18:12:26 $
------------------------------------------------------
 * GTIE *  PROJET MIGRAZUR
------------------------------------------------------
 * SOUS-SYSTEME : EQUEXT
------------------------------------------------------
 * MODULE TEINI * FICHIER ereg_dir.c
------------------------------------------------------
 * DESCRIPTION DU MODULE :
 *
 * < Module directeur de la tache TEREG >
 *
------------------------------------------------------
 * HISTORIQUE :
 *
 * SDU       13/03/2012 : Creation DEM/1016
 * JMG	15/07/13 : sortie sur fatal
 * RGR 17/05/19 : DEM1331 - Ajout Mecanisme KEEPALIVE 1.4
 * GR  09/07/2020 : EMS 1.5
 * LCL		25/09/2023	: Adaptation code pour rabbitmq DEM-483
------------------------------------------------------*/

/* fichiers inclus */

#include <xzsem.h>
#include "ereg_dir.h"
#include "ereg_cfg.h"
#include "ereg_trace.h"
#include "ereg_detection.h"
#include "ereg_application.h"
#include "ereg_valide.h"
#include "xzav03.h"
/*#include "xzag.h"*/
#include "xzst.h"
#include "xzems.h"


/* definitions de constantes */

/* definitions de types locaux */


/* definition de macro locales */

/* declaration de variables locales */

/* declaration de variables globales */
XDY_Booleen			vg_ModeSimu 		= XDC_FAUX;
XDY_Booleen			vg_SimuEnCours 		= XDC_FAUX;
XDY_NomSite     	vg_NomSite;
XDY_NomMachine  	vg_NomMachine;
XDY_TypeMachine		vg_TypeMachine		= 0;
XDY_Horodate		vg_HorodateSimu;
XDY_Horodate		vg_horodate_debut_simu;
XDY_Horodate		vg_horodate_fin_simu;
XDY_NomMachine		vg_machineSimu;

int rt_error = 0;
XDY_Booleen			vg_tebeg = XDC_FAUX;
XDY_Booleen			vg_tersi = XDC_FAUX;

extern T_Liste_ZonesRegul *pg_Zones;
extern T_Param            tg_params[EREG_NB_PARAM_MAX];

/* declaration de fonctions internes */
void dir_init(int argc, char **argv);
void dir_sortie(void);
static void ereg_start_simu_cb(T_IPC_CONN 			va_Cnx,
    T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
    T_CB_ARG                        pa_ARG);

/* declaration de fonctions externes */
extern int valide_scenario(T_ZONE_REGUL *pa_zone);
extern void det_ideal_cb();
extern void det_simu_trafic_cb();


/*
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
 *   Connexion au RTServer avec initialisation des variables de
 *   supervision, recuperation du site de la machine
 *   ainsi que le type de celle-ci.
 *   Armements de callbacks.
 *   Abonnement au Datagroup XDG_EINIT_NomSite et XDG_ECONFIG_EQPT_NomSite.
 *   Constitution des fichier de config equipt pour toutes les machines (appel ed_config)
 *
------------------------------------------------------*/

int	argc;
char	**argv;
{
  T_IPC_MSG   pl_Msg;
  int vl_IndPre = 0;

	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

  /*A
   * Initialisation du module directeur
   */
  dir_init(argc, argv);

  XZST_03EcritureTrace(XZSTC_INFO, "***** Fin de l'Init : Attente des donnees *****");

  /*A Attente des messages sur les sockets (RTserver, d'ecoute et de donnees */
  while (1)
  {
    XZST_03EcritureTrace(XZSTC_DEBUG1, "[INFO] while waiting for RT message\n");

    pl_Msg = TipcSrvMsgNext(T_TIMEOUT_FOREVER);

    XZST_03EcritureTrace(XZSTC_DEBUG1, "[RT] Msg message received");

    if (isMessageNotNULL(pl_Msg))
    {
      XZST_03EcritureTrace(XZSTC_DEBUG1, "[RT] pl_Msg message not NULL: processing");

      TipcSrvMsgProcess(pl_Msg);
      TipcMsgDestroy(pl_Msg);

      /* Memorisation Test Presence RTserver */
      vl_IndPre = 0;
    }
    else
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "[RT] pl_Msg message NULL");

      /* Test de Presence RTserver */
      if (vl_IndPre == 0)
      {
			if (errno)
        XZST_03EcritureTrace(XZSTC_WARNING, "[RT] WARNING RTWorks Error");

			if (errno)
        XZST_03EcritureTrace(XZSTC_WARNING,
            "Erreur RTWorks num: %d -- errno %d \n",
            TutErrNumGet(), errno);
      }
      else if (vl_IndPre >= XDC_EXIT_RTCLIENT)
      {
        XZST_03EcritureTrace(XZSTC_WARNING, "WARNING stopping tereg.x task");

        XZST_03EcritureTrace(XZSTC_WARNING,
            "Bouclage et arret tache tereg.x %d \n", vl_IndPre);

        /* Arret de la tache */
        vl_IndPre = 0;
        dir_sortie();
      }
      /* Indication absence RTserver */
      if (errno)
      	vl_IndPre++;
    }

  }   /* fin while */

}  /* Fin main */


/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *  initialisation du module Directeur de la tache.
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */
void dir_init(int argc, char **argv)

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
 * FONCTION
 *
------------------------------------------------------*/
{
  XDY_Datagroup		pl_NomDG;
  int            	    vl_ValRet = XDC_OK;

  T_INT4 				vl_TypeParam;
  XDY_Entier 	vl_cr;

  /*A
   * Connexion au RTserver et initialisation des variables
   */
  if ( XZSC_01InitCnxSup( argv[1],
      argv[ 0 ],
      XZSCC_INTERDIRE,
      XZSCC_AUTORISER,
      XZSCC_INTERDIRE,
      dir_sortie,
      "/dev/null" ) != XDC_OK )
  {
    /*B Affichage Trace */
    XZST_03EcritureTrace( XZSTC_WARNING, "Ne peut ouvrir la connexion de TEREG avec le RTserver." );
    exit( XDC_NOK );
  }
  else
  {
    /*B Affichage Trace */
    XZST_03EcritureTrace( XZSTC_WARNING, "Ouverture de la connexion de TEREG avec le RTserver." );
  }

  XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE,XZSTC_NECRAN);
  if (!strcmp(argv[0],"tebeg.x")) {
    vg_tebeg = XDC_VRAI;
    XZST_03EcritureTrace( XZSTC_WARNING, "vg_tebeg = XDC_VRAI");
  }

  if (!strcmp(argv[0],"tersi.x"))
  {
    vg_tersi = XDC_VRAI;
    vg_ModeSimu = XDC_VRAI;
    XZST_03EcritureTrace( XZSTC_WARNING, "vg_tersi = XDC_VRAI");
  }

  /*A Recuperation du nom de la machine */
  if ( XZSC_07NomMachine(vg_NomMachine) != XDC_OK)
  {
    /* Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_FATAL, "Recuperation du Nom de la machine impossible . " );
    exit( XDC_NOK );
  }

  /*A Recuperation du site de la machine */
  if ( (vl_ValRet = XZSC_04NomSite(vg_NomSite)) != XDC_OK)
  {
    /* Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible. Erreur : %d", vl_ValRet);
    exit( XDC_NOK );
  }

  XZSM_00InitServices();
  XZSM_02AnnulerProgrammation("cle_timer_simu", XDG_REGUL_START_SIMU, XDM_REGUL_SIMU);


  /* Initialisation du module CONFIG */
  if (cfg_init(vg_ModeSimu) == XDC_NOK)
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "erreur Initialisation module Config.");
    dir_sortie();
  }

  XZST_03EcritureTrace(XZSTC_WARNING," mode simu (%s)",
      vg_ModeSimu==XDC_VRAI ? "VRAI" : "FAUX");


  if (vg_ModeSimu)
  {
    XZST_03EcritureTrace( XZSTC_INFO, "Mode Simulation detecte.");

    /*A  Armement Callback sur Reception XDM_REGUL_START_SIMU */
    if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_REGUL_START_SIMU),
        ereg_start_simu_cb,(T_CB_ARG)NULL) == NULL)
    {
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, "Callback sur XDM_REGUL_START_SIMU non declenchee.");
    }

    /*A Abonnement au Datagroup XDG_REGUL_START_SIMU */
    /*XDG_EncodeDG2 ( pl_NomDG, XDG_REGUL_START_SIMU , vg_NomSite) ;*/
    /*A joint ce datagroup */
    if ( !XDG_JoindreDG( XDG_REGUL_START_SIMU ) )
    {
      /* Affichage Trace */
      XZST_03EcritureTrace(XZSTC_WARNING, "Abonnement au Datagroup %s non effectue ", XDG_REGUL_START_SIMU );
    }
  }
  else
  {
    XZST_03EcritureTrace( XZSTC_INFO, "Mode Simulation NON detecte.");
  }

  /* Initialisation du module TRACE */
  if (trace_init() == XDC_NOK)
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "ERREUR Initialisation module Trace.");
    dir_sortie();
  }

  /* Initialisation du module DETECTION */
  if (det_init(argv[2]) == XDC_NOK)
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "ERREUR Initialisation module Detection.");
    dir_sortie();
  }

  /* Initialisation du module APPLICATION */
  if (appli_init() == XDC_NOK)
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "ERREUR Initialisation module Application.");
    dir_sortie();
  }

  /* Initialisation du module VALIDE */
  if (valide_init() == XDC_NOK)
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "ERREUR Initialisation module Valide.");
    dir_sortie();
  }

  XZST_03EcritureTrace(XZSTC_FONCTION, "dir_init() : OUT");
}

void valider_scenarios()
{
  T_Liste_ZonesRegul *pl_zones;

  XZST_03EcritureTrace(XZSTC_FONCTION,"valider_scenarios : IN");

  pl_zones = pg_Zones;
  while (pl_zones != NULL){
    valide_scenario(&(pl_zones->zone));
    pl_zones = pl_zones->suivant;
  }

  XZST_03EcritureTrace(XZSTC_FONCTION,"valider_scenarios : OUT");
}


/*------------------------------------------------------
 * SERVICE RENDU :
 *  Fonction effectuant les operations de terminaison de la tache.
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void dir_sortie ( void )

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
 *  aucun
 *
 * FONCTION
 *  Deconnexion de la base de donnees (appel XZAG03)
 *
------------------------------------------------------*/
{
  XZST_03EcritureTrace(XZSTC_WARNING,"dir_sortie : IN");
  T_Liste_ZonesRegul *tmpZone = NULL;
  if (vg_ModeSimu == XDC_FAUX)
  {
    tmpZone = pg_Zones;

    /* Mode Temps Reel*/
    /* Parcours de toute les zones avec le scenario nominal*/
    while(tmpZone != NULL)
    {
      valide_arret(&(tmpZone->zone));
      tmpZone = tmpZone->suivant;
    }
  }

  /*supprimer le timer de lecture des donnees trafic*/
  XZSM_02AnnulerProgrammation("cle_timer_detection", XDG_EMES_RADT_6MN, XDM_EMES_6mn_Chaussee);
  /*supprimer le timer de recherche du scenario ideal*/
  XZSM_02AnnulerProgrammation("cle_timer_detection", XDG_REGUL_SIMU, XDM_REGUL_DETECTION);
  XZSM_02AnnulerProgrammation("cle_timer_simu", XDG_REGUL_START_SIMU, XDM_REGUL_SIMU);

  /*A Deconnexion de la base de donnees */
  XZAG03_Deconnexion_Base();

  XZST_03EcritureTrace(XZSTC_WARNING,"Sortie de tache TEREG .");

  XZST_03EcritureTrace(XZSTC_FONCTION,"dir_sortie : OUT");
  /*A Arret de la tache */
  exit ( 0 );
}



/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *  < Descriptif de la fonction >
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

static void	ereg_start_simu_cb (	T_IPC_CONN 			va_Cnx,
    T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
    T_CB_ARG                        pa_ARG )

/*
 * ARGUMENTS EN ENTREE :
 *  Message RTworks
 *
 * ARGUMENTS EN SORTIE :
 *   aucun
 *
 *
 * CODE RETOUR :
 *   aucun
 *
 * CONDITION D'UTILISATION
 *  Fonction appelee lors de la reception du message
 *  XDM_REGUL_START_SIMU
 *
 * FONCTION
 *
------------------------------------------------------*/
{

    #ifndef _TIBCO_EMS
    T_STR			vl_NomMachine;
  T_INT2			vl_NumMachine;
  T_REAL8	vl_horodate_debut, vl_horodate_fin;
    #else
    XDY_NomMachine		vl_NomMachine;
    XDY_Mot			vl_NumMachine;
    XDY_Horodate	vl_horodate_debut, vl_horodate_fin;
    #endif  

  XZSMT_Horodate  vl_Horodate_courante;

  XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ereg_start_simu_cb ");

  if(! TipcMsgRead(pa_Data->msg,T_IPC_FT_REAL8,&vl_horodate_debut,
      T_IPC_FT_REAL8,&vl_horodate_fin,
      T_IPC_FT_INT2,&vl_NumMachine,
      T_IPC_FT_STR, &vl_NomMachine, NULL))
  {
    XZST_03EcritureTrace(XZSTC_WARNING,
        "ereg_start_simu_cb: Impossible de recuperer le message");
    return;
  }


  vg_HorodateSimu = vl_horodate_debut- 6*60;
  vg_horodate_debut_simu = vl_horodate_debut;
  vg_horodate_fin_simu = vl_horodate_fin;

  XZST_03EcritureTrace(XZSTC_INFO,"(%d) debut simu %lf // fin simu %lf",
      vg_SimuEnCours, vl_horodate_debut,vl_horodate_fin);

  XZSM_11ConversionHorodate(vl_horodate_debut , &vl_Horodate_courante);
  XZST_03EcritureTrace(XZSTC_INFO,"debut simu %s",vl_Horodate_courante);
  XZSM_11ConversionHorodate(vl_horodate_fin , &vl_Horodate_courante);
  XZST_03EcritureTrace(XZSTC_INFO,"fin simu %s",vl_Horodate_courante);

  if (vl_horodate_debut==vl_horodate_fin)
  {
    XZIT20_Fin_Simulation(vg_machineSimu);
    vg_SimuEnCours = FALSE;
    XZSM_02AnnulerProgrammation("cle_timer_simu", XDG_REGUL_START_SIMU, XDM_REGUL_SIMU);
    dir_sortie();
    return;
  }

  if(vg_SimuEnCours)
  {
    XZST_03EcritureTrace(XZSTC_INFO,"simu deja en cours");
    if (XZIT21_Simulation_En_Cours(vl_NomMachine) != XDC_OK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "pb appel a XZIT21_Simulation_En_Cours");
      return;
    }
    XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ereg_start_simu_cb ");
    return;
  }

  reinit_contexte();
  strcpy(vg_machineSimu , vl_NomMachine);

  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_REGUL_SIMU),
      det_simu_trafic_cb,(T_CB_ARG)NULL) == NULL)
  {
    XZST_03EcritureTrace( XZSTC_DEBUG1,
        "Callback sur XDM_REGUL_SIMU non cree." );
  }

  if (XZSM_01DemanderProgrammation(       0,
      tg_params[EREG_PARAM_APP_REG_CADENCE_MODE_SIMU].Valeur, /*TODO bonne valeur*/
      XDG_REGUL_START_SIMU,
      XDM_REGUL_SIMU,
      "cle_timer_simu",
      XZSMC_SEC_VRAI) != XDC_OK)
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "Programmation du timer XDM_REGUL_SIMU non declenchee.");
    return ;
  }

  /* Abonnement au DataGroup KEEPALIVE */
  XZSC_31AbtKeepAlive(vg_NomSite);

  vg_SimuEnCours=TRUE;

  /*Historisation*/
  trace_process(0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      "purge");

  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ereg_start_simu_cb ");

}

