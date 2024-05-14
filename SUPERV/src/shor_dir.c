/* Fichier : $Id: shor_dir.c,v 1.38 2021/07/19 16:14:41 pc2dpdy Exp $	Release : $Revision: 1.38 $	      Release : $Revision: 1.38 $        Date : $Date: 2021/07/19 16:14:41 $
------------------------------------------------------
 * STERIA *  PROJET MIGRAZUR
------------------------------------------------------
 * SOUS-SYSTEME  SUPERV
------------------------------------------------------
 * MODULE SHOR_DIR * FICHIER shor_dir.c
------------------------------------------------------
 * DESCRIPTION DU MODULE :
 *
 * Module directeur de la tache TSHORL.
 * Tache principale declarant les callbacks, et traitant les msg
 *	RTworks.
 * Tache de sortie declencher pour un arret de tache.
 *
------------------------------------------------------
 * HISTORIQUE :
 *
 * Milleville.T	09 Aug 1994	: Creation
 * Milleville.T	27 Fev 1995	: Modification boucle RTworks V1.6
 * Milleville.T	03 Mar 1995	: Modification  du type de datagroup
 *		pour envoyer l'heure  V1.8
 * Milleville.T	08 Mar 1995	: Modification  des valeurs des arguments
 *	pour l'appel de la fonction XZEH_01 V1.9
 * Milleville.T	09 Mar 1995	: Modification de la periode de lecture
 *		de l'horloge. Positionnement toutes les heures V1.10
 * Milleville.T	03 Avr 1995	: Modification pour tenir compte de
 *	l'extraction automatique des comentaires V1.11
 * Milleville.T	21 Avr 1995	: Modification de la boucle infinie
 *	de RTworks V1.12
 * Milleville.T	30 Juin  1995	: Modification de la boucle infinie
 *		V1.14
 * Milleville.T	25 Jul  1995	: Ajout constante XDC_EXIT_RTCLIENT dans
 *	la boucle infinie V1.16
 * Milleville.T	6 Aou  1995	: Ajout test etat degrade V1.17
 * Milleville.T	21 Sep  1995	: Ajout destruction deconnexion avec RTserver dans
 *	la fonction de sortie V1.19
 * Milleville.T	27 Sep  1995	: Suppression de l'attente lors d'erreur
 *	RTWorks V1.20
 * Milleville.T	10 Oct  1995	: Ajout test pour connaitre l'etat du
 *	serveur et ne declenche que si il n'est pas Passif V1.21
 * Milleville.T	17 Oct  1995	: Ajout de traces V1.23
 * Niepceron	14 Jan 1998	: Ajout des nom RS232 suivant les type de machines HP v1.24
 * Niepceron     15 Mar 1999	: DA et DS ont une horloge v1.25
 * CLUCHAGUE     04 Sep 2002	: Le L1000 de SDCI1 recoit sur ttyd0p1 v1.26 (REPERE1)
 * JMG		10 oct 2002     : le CI synchronise TOUT PC2 et PC3 v1.26
 * Niepceron	30 Mars 2009	: ajout tty pour RS232 ia64 DEM875 v1.27
 * PNI		23 oct 2009	: prise en compte du cas specifique SDCA1 v1.29
 * JMG    10/02/10        : portage Linux DEM/934
 * JPL		12/01/11 : Migration architecture HP ia64 (DEM 971) : suppression message traite et non connexion  1.31
 * PNI		02/12/2014	: Abt a XDM_TSE_KEEPALIVE pour activer socket de tsaplg DEM1116 1.32
 * JPL		31/05/16 : Lecture du peripherique serie de datation en configuration (DEM 1167)  1.33
 * RGR    27/03/19 : DEM1331 - Mise en place du Keep alive  1.36
 * PNI    10/05/19 : Correction Bug datation                1.37
 * LCL	  20/04/21 : Correction EMS errno		    1.38
 * LCL		25/09/2023	: Adaptation code pour rabbitmq DEM-483
------------------------------------------------------*/

/* fichiers inclus */

#include	<stdio.h>

#include <xzsem.h>
#include	<xdc.h>
#include	<xzsa.h>
#include	<xzsc.h>
#include	<xzsm.h>
#include	"xzeh.h"
#include "xzems.h"

#include	<rtworks/common.h>

#include	<rtworks/ipc.h>
#ifdef _HPUX_SOURCE
#include        <rtworks/ipc_proto.h> /*LINUX*/
#endif

#include 	"shor_tra.h"


/* definitions de constantes */

#define C_PERIODE_LEC (60)*(5)		/* Periode de lesture de l'heure  
/*#define C_PERIODE_LEC (60)*(60)*/		/* Periode de lesture de l'heure  
 *    system en secondes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* Datagroup pour la lecture horloge */
XZSCT_Datagroup         vg_LecTime	= "";	
/* Cle programmation Timer */
XZSMT_CleProg           vg_CleProg	= "";	


static	XDY_Nom		SHORV_PERIPH_DATE = XZEHC_NOM_RS232_DEFAUT;


/* declaration de fonctions internes */

static void sh_sortir();


/* definition de fonctions externes */



/* ---------------------------------------------------
 * SERVICE RENDU :
 * Programme principal
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

int main(int argc, char *argv[] )

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
 *    Supervision, recuperation du site de la machine
 *    ainsi que le type de celle-ci.
 *	Si la machine est SC ou SD alors armements de
 * 	callbacks pour mise a jour de l'horodate.Abonnement au
 *       Datagroup XDG_NSUPHORL_NomSite.Suivant le type
 *	de calculateur envoie du message XDM_TSH_MODE.
 *
 *     Attente des messages RTserver
 *
 *
------------------------------------------------------*/

{

  time_t	 		vl_Date;
  int			vl_EtatMachine 	= XZSEC_ETAT_NACTIF;
  struct tm		*tl_Tm		= NULL;
  int            	      	vl_ValRet 	= XDC_OK;
  XZSCT_NomSite        	pl_NomSite	= "";
  XZSCT_NomMachine       	pl_NomMachine	= "PodFA2";
  int                  	pl_TypeMachine	= 0;
  XZSCT_Datagroup         vl_NomTacheHorl	= "";
  XZSCT_Datagroup         vl_Datagroup	= "";
  T_IPC_MSG              	pl_Msg;
  int 			vl_ModeEmis	= 0;
  int 			vl_Inter	= 0;
  int                     vl_IndPre       = 0;
  int                     vl_Attente       = 0;


	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

  /*
   * Connexion au RTserver et initialisation des variables administration
   */
  if( XZSC_01InitCnxSup(argv[ 1 ],
      argv[ 0 ],
      XZSCC_AUTORISER,
      XZSCC_AUTORISER,
      XZSCC_INTERDIRE /*XZSCC_AUTORISER*/, /*linux*/
      sh_sortir,
      "/dev/null" ) != XDC_OK )
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "Ne peut ouvrir la connexion avec le RTserver.\n" );
    exit( XDC_NOK );
  }
  else
  {
    XZST_03EcritureTrace( XZSTC_DEBUG1, "Ouverture de la connexion avec le RTserver.\n" );
  }

  /* Reservation des ressources d'administration */
  XZSA_20InitAdmin();

  /* Initialisation des services Timers */
  XZSM_00InitServices();
  /*
        XZST_08ForceNivVisuTraceLocal(XZSTC_INFO|XZSTC_MESSAGE|XZSTC_INTERFACE|XZSTC_FONCTION,XZSTC_NECRAN);
   */
  XZST_08ForceNivVisuTraceLocal(XZSTC_INFO|XZSTC_MESSAGE|XZSTC_INTERFACE,XZSTC_NECRAN);

  /* Recuperation du site de la machine */
  if ((vl_ValRet = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible.\n" );
  }

  /* Recuperation du type de la machine */
  if ( (vl_ValRet = XZSC_05TypeMachine(&pl_TypeMachine)) != XDC_OK)
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Type de la machine impossible.\n ");
  }
  else
  {

    /* Nom de Machine */
    XZSC_07NomMachine(pl_NomMachine);

    /* Abonnement au Keep Alive */
    XZSC_31AbtKeepAlive(pl_NomSite);


    /* Recherche de l'etat de la machine */
    XZSE_09EtatMachine(&vl_EtatMachine) ;

    if (vl_EtatMachine != XZSEC_ETAT_NPASSIF)
    {
      XZST_03EcritureTrace( XZSTC_WARNING, " Machine Active %d de type %d \n", vl_EtatMachine, pl_TypeMachine);

      /*  Armement Callback sur Reception XDM_TSH_HORO */
      if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSH_HORO),
          sh_actualiserhoro_cb,
          (T_CB_ARG)NULL) == NULL)
      {
        XZST_03EcritureTrace( XZSTC_WARNING, "Callback sur XDM_TSH_HORO non declenchee.\n");
      }

      /*  Armement Callback sur Reception XDM_TSH_SYNC */
      if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSH_SYNC),
          sh_synchroniser_cb,
          (T_CB_ARG)NULL) == NULL)
      {
        XZST_03EcritureTrace( XZSTC_WARNING,"Callback sur XDM_TSH_SYNC non declenchee.\n ");
      }


      /*  Armement Callback sur Reception XDM_TSH_SYST */
      if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSH_SYST),
          sh_actualisersyste_cb,
          (T_CB_ARG)NULL) == NULL)
      {
        XZST_03EcritureTrace( XZSTC_WARNING, "Callback sur XDM_TSH_SYST non declenchee. \n ");
      }


      /* Abonnement au Datagroup XDG_NSUPHORL_NomSite */
      XDG_EncodeDG2 (vl_NomTacheHorl,  XDG_NSUPHORL, pl_NomSite);
      if (!XDG_JoindreDG(vl_NomTacheHorl))
      {
        XZST_03EcritureTrace(XZSTC_WARNING, "Abonnement au Datagroup XDG_NSUPHORL_%s non effectue \n", pl_NomSite);
      }

      /* Mise a jour du mode d'emission avec XZSMC_ECRITURE */
      vl_Inter = XZSMC_SYSTE;
      if ( (vl_ValRet = XZSM_09ModeHorodate(XZSMC_ECRITURE,&vl_Inter)) != XDC_OK)
      {
        XZST_03EcritureTrace(XZSTC_WARNING, "Maj Emission Impossible \n");
      }
      else
      {

        /* si le calculateur est actif ou degrade  */
        if ( (vl_EtatMachine == XZSEC_ETAT_NACTIF) || (vl_EtatMachine == XZSEC_ETAT_DEGRADE) )
        {
          /* Recuperation du mode d'emission */
          XZSM_09ModeHorodate(XZSMC_LECTURE,&vl_ModeEmis);

          /* Envoie sur le Datagroup XDG_TIME_NomSite */
          XDG_EncodeDG2 (vl_Datagroup, XDG_TIME, pl_NomSite);

          /* envoie du message XDM_TSH_MODE */
          if (!TipcSrvMsgWrite(vl_Datagroup,
              XDM_IdentMsg(XDM_TSH_MODE),
              XDM_FLG_SRVMSGWRITE,
              T_IPC_FT_INT4,
              vl_ModeEmis,
              NULL))
          {
            XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_TSH_MODE non effectue ");
          }

        }
        else
        {
          XZST_03EcritureTrace(XZSTC_INFO, "XZSM_ActualiserHorodate:TipcSrvMsgWrite(vl_Datagroup, XDM_FLG_SRVMSGWRITE,,%f) = OK");
        }

      }


      /*  Recuperation du mode d'emission avec XZSMC_LECTURE  */
      XZSM_09ModeHorodate(XZSMC_LECTURE, &vl_ModeEmis);

      /*  Recuperation heure systeme si XZSMC_SYSTEM */
      if (vl_ModeEmis == XZSMC_SYSTE)
      {

        /*  Abonnement au Datagroup XDG_NSUPHORL_NomSite */
        XDG_EncodeDG2 (vg_LecTime,  XDG_NSUPHORL, pl_NomSite);

        /*  Construction de la cle Timer */
        sprintf(vg_CleProg, "%s_%s", "LEC_SYST", pl_NomSite);

        /*  Programmation timer */
        vl_ValRet = XZSM_01DemanderProgrammation(XZSMC_HORODATE_NUL,
            C_PERIODE_LEC,
            vg_LecTime,
            XDM_TSH_SYST,
            vg_CleProg,
            0 );

        if (vl_ValRet != XDC_OK)
        {
          XZST_03EcritureTrace(XZSTC_WARNING, "XZSM_01DemanderProgrammation ");
        }

      }
    }
    else
    {
      XZST_03EcritureTrace( XZSTC_WARNING, " Machine Passif %d \n", vl_EtatMachine);
    }

  }


  /* Boucle infinie pour le traitement des messages RTServer */
  while(1)
  {

    pl_Msg = TipcSrvMsgNext(T_TIMEOUT_FOREVER);

    /*  Si msg recu */
    if (isMessageNotNULL(pl_Msg))
    {
      /* Execution callback associe au msg et destruction du msg */
      TipcSrvMsgProcess(pl_Msg);
      TipcMsgDestroy(pl_Msg);
      /* Memorisation Test Presence RTserver */
      vl_IndPre = 0;
    }
    else
    {
      /* Test de Presence RTserver */
      if (vl_IndPre == 0)
      {
        /* erreur RTWorks */
        XZST_03EcritureTrace (XZSTC_WARNING, "Erreur RTWorks num: %d -- errno %d \n", TutErrNumGet(), errno);
      }
      else if (vl_IndPre >= XDC_EXIT_RTCLIENT)
      {
        XZST_03EcritureTrace (XZSTC_WARNING, "Bouclage et arret tache tshor  %d \n", vl_IndPre);
        /* Arret de la tache */
        exit(0);
      }

      /* INdication Non Presence RTserver */
      if (errno)
	      vl_IndPre++;

      /* Attente aleatoire pour permettre une reconnexion */
      vl_Attente = rand()%15;
#if O 
      sleep(vl_Attente);
#endif
    }
  }

}  /* Fin main */

/*
------------------------------------------------------
 * SERVICE RENDU :
 *  Fonction permettant de sortir de la tache TSHORL
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

static void sh_sortir()

/*
 * ARGUMENTS EN ENTREE :
 *   Aucun
 *
 *
 * ARGUMENTS EN SORTIE :
 *   Aucun
 *
 *
 * CODE RETOUR :
 *  Aucun
 *
 * CONDITION D'UTILISATION
 *   Aucune
 *
 * FONCTION
 *   Fermeture de la liaison RS232(XZEH_03)
 *   puis annulation de la programmation timer(XZSM_02)
 *   puis fermeture du fichier de trace (XZST_02)
 *
------------------------------------------------------*/
{

  int            	      	vl_ValRet = XDC_OK;


  /*  Fermeture de la liaison serie RS232 */
  if ( (vl_ValRet = XZEH03_Fermeture_Horloge()) != XDC_OK)
  {
    /* Affichage trace */
    XZST_03EcritureTrace(XZSTC_WARNING,"XZEH01_Ouverture_Horloge " );
  }

  /* Annulation de la programmation Timer */
  if ( (vl_ValRet = XZSM_02AnnulerProgrammation(vg_CleProg,vg_LecTime,
      XDM_TSH_SYST) ) != XDC_OK )
  {
    /* Affichage trace */
    XZST_03EcritureTrace(XZSTC_WARNING,"XZSM_02AnnulerProgrammation ");
  }

  /* Fermeture du fichier trace */
  if ( (vl_ValRet = XZST_02FermetureTrace() ) != XDC_OK )
  {

    /* Affichage trace */
    XZST_03EcritureTrace(XZSTC_WARNING,"XZST_02FermetureTrace ");
  }

  /* On detruit la connexion completement */
  if (!TipcSrvDestroy(T_IPC_SRV_CONN_NONE))
  {
    XZST_03EcritureTrace(   XZSTC_WARNING,
        "Erreur deconnexion serveur \n");
  }

  /* Sortie de la tache */
  exit(0);


}  /* Fin sh_sortir */
