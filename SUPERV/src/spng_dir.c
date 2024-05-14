/* Fichier : $Id: spng_dir.c,v 1.22 2020/11/03 17:54:15 pc2dpdy Exp $	Release : $Revision: 1.22 $        Date : $Date: 2020/11/03 17:54:15 $
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE SPNG_DIR * FICHIER spng_dir.c
 ******************************************************
 * DESCRIPTION DU MODULE :
 *
 * Ce module contient le main de la tache tspng.x et la
 * fonction qui permet de tester la presence d'une
 * machine sur le reseau.
 *
 ******************************************************
 * HISTORIQUE :
 *
 * T.Milleville 30 Sep 1994	: Creation
 * T.Milleville 27 Fev 1995	: Modification Boucle RtWorks V1.4
 * T.Milleville 11 Mai 1995	: Ajout de traces V1.6
 * T.Milleville 29 Juin 1995	: Modification boucle de traitements
 *	des msg RTworks V1.7
 * T.Milleville 29 Juin 1995	: Declenchement des tests par le timer sur
 *	reception du msg XDM_TSG_VERIF V1.8
 * T.Milleville 25 Jul 1995	: Ajout constante XDC_EXIT_RTCLIENT et ajout
 *	de commentaires V1.10
 * T.Milleville 8 Aou 1995	: Ajout test erreur dans la boucle infinie.
 *	Si detection du tiemout atteint, alors ce n'est pas une erreur
 *	RTWorks.  V1.11
 * T.Milleville 11 Sep 1995	: Ajout reinitialisation de vl_IndPre
 *	dans la boucle infinie  V1.13
 * T.Milleville 15 Sep 1995	: Redirectio stdout et sdterr dans
 *	dev/null  V1.14
 * T.Milleville 21 Sep 1995	: Ajout destruction connexion avec RTserver
 *	dans la fonction de sortie V1.15
 * T.Milleville 27 Sep 1995	: Suppression de l'attente lors d'une erreur
 *		RTworks V1.16
 * T.Milleville 02 Nov 1995	: Plus de demandes cycliques pour les tests reseaux
 *	mais apres chaque traitement V1.17
 * JMG	26/06/10 : portage linux DEM/934 1.18
 * JPL	12/01/11 : Migration architecture HP ia64 (DEM 971) : exit sur echec connexion au noyau supervision  1.19
 * PNI	10/06/11 : Ajout de traces pour mise en evidence manque reveil tstim 1.20
 * RGR 27/03/19 : Mise en place mecanisme Keep Alive
 * KOU   25/09/2023	: Adaptation code pour rabbitmq DEM-483
 *******************************************************/


/* fichiers inclus */

#include <xzsem.h>
#include "xdc.h"
#include "xdm.h"
#include "xzsa.h"
#include "xzsc.h"
#include "xzse.h"
#include "xzss.h"
#include "xzsg.h"
#include <xzems.h>

#include "spng_dir.h"
#include "spng_don.h"
#include "spng_tra.h"




/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */
void spng_sortir();


/* definition de fonctions externes */



/*
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE SPNG_DIR
 ******************************************************
 * DESCRIPTION DE LA FONCTION :
 *
 * Ceci est la fonction main de la tache spng.x, elle
 * effectue les actions suivantes:
 * - Connexion au noyau de supervision
 * - Recuperation du nom de la machine locale
 * - Recuperation du site sur lequel s'execute spng.x
 * - Recuperation de la periode test reseau
 * - Armement des callbacks
 * - Abonnement au datagroupe de reception des messages
 * - Envoie du message de demande des tests reseau en cour
 * - Declenchement Timer pour test cyclique reseau
 * - Boucle principale infinie
 *
 ******************************************************
 * SEQUENCE D'APPEL :
 */

main(int argc, char *argv[])
{

  XZSCT_NomMachine pl_NomMachine		= "\0";
  int 		vl_cr			= XDC_OK;
  int 		vl_PeriodeTest		= 0;
  int                  vl_IndPre       = 0;
  int                  vl_Attente       = 0;
  XZSCT_Datagroup pl_NomDatagroup         = "\0";
  T_IPC_MSG       pl_Msg                ;


	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

  /* connexion au noyau de supervision */
  vl_cr = XZSC_01InitCnxSup(argv[1],
                            argv[0],
                            XZSCC_INTERDIRE,  /* mise a jour horodate*/
                            XZSCC_AUTORISER,  /* systeme trace */
                            XZSCC_INTERDIRE,  /* synchro tsetat */
                            spng_sortir,      /* fontion de sortie */
                            "/dev/null"       /* redirection sortie standard */
                           ) ;

  if (vl_cr != XDC_OK)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "erreur XZSC_01InitCnxSup cr = %d", vl_cr);
    exit(1);
  }

  /* On redirige le Ctrl^C sur la fonction de sortie */
  signal(SIGINT,spng_sortir);

  /* Initialistion services TIMER */
  XZSM_00InitServices();

  /* Recuperation du nom de la machine locale */
  XZSC_07NomMachine(pl_NomMachine) ;

  /* Recuperation du nom du site local */
  XZSC_04NomSite(vg_NomSite);

  /* Recuperation de la periode de test reseau */
  XZSC_03PeriodeTestReseau(&vl_PeriodeTest);

  /* initialisation administration */
  XZSA_20InitAdmin();

  /* mise en place des callbacks  */
  TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSG_PING),
      (T_IPC_CONN_PROCESS_CB_FUNC)sg_arme_test_cb,
      (T_CB_ARG)NULL) ;

  TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSG_STOP_PING),
      (T_IPC_CONN_PROCESS_CB_FUNC)sg_arret_test_cb,
      (T_CB_ARG)NULL) ;

  TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSG_VERIF),
      (T_IPC_CONN_PROCESS_CB_FUNC)sg_verif_test_cb,
      (T_CB_ARG)NULL) ;

  /* abonnement au Datagroup XDG_NSUPPING_NomMachine */
  sprintf(pl_NomDatagroup,"%s_%s",XDG_NSUPPING,pl_NomMachine) ;
  XDG_JoindreDG(pl_NomDatagroup);

  /* Abonnement au DataGroup KEEPALIVE */
  XZSC_31AbtKeepAlive(vg_NomSite);

#if 1	/* Modif V1.17  Demande non cyclique */
  /*A Demande cyclique de verification des tests reseaux */
#ifdef _HPUX_SOURCE
  if (XZSM_01DemanderProgrammation(vl_PeriodeTest, 0,
      pl_NomDatagroup, XDM_TSG_VERIF,"XDM_TSG_VERIF" , 0) != XDC_OK)
  {
    XZST_03EcritureTrace(XZSTC_WARNING ,
        "main : Erreur demande programmation test reseau Machine \n");
  }
#else
  double vl_Horodate;
  int vl_Jour_De_La_Semaine;
  XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_Jour_De_La_Semaine,&vl_Horodate) ;

  if (XZSM_01DemanderProgrammation(vl_Horodate+vl_PeriodeTest, vl_PeriodeTest,
      pl_NomDatagroup, XDM_TSG_VERIF,"XDM_TSG_VERIF" , 0) != XDC_OK)
  {
    XZST_03EcritureTrace(XZSTC_WARNING ,
        "main : Erreur demande programmation test reseau Machine \n");
  }

#endif
#endif



  /* demande des test reseaux en cours a toutes les taches abonnees au service */
  XDG_EncodeDG2(pl_NomDatagroup, XDG_NPINGINIT, pl_NomMachine);
  if (!TipcSrvMsgWrite(pl_NomDatagroup,
      XDM_IdentMsg(XDM_TSG_INIT),
      XDM_FLG_SRVMSGWRITE,
      NULL))
  {
    XZST_03EcritureTrace(XZSTC_WARNING,
        "erreur Envoi Msg XDM_TSG_INIT \n");
  }

   XZST_08ForceNivVisuTraceLocal(XZSTC_INFO|XZSTC_MESSAGE|XZSTC_INTERFACE|XZSTC_FONCTION|XZSTC_DEBUG1,XZSTC_NECRAN);

  /* boucle principale RTWORKS */
  while(1)
  {
#ifndef _TIBCO_EMS 
    XZST_03EcritureTrace (XZSTC_WARNING, "Attente pdt %d  d'un message %d",vl_PeriodeTest,vl_IndPre);

    pl_Msg = TipcSrvMsgNext(vl_PeriodeTest);
#else
     pl_Msg = TipcSrvMsgNext(T_TIMEOUT_FOREVER);
#endif
    // if (pl_Msg != NULL)
    if(isMessageNotNULL(pl_Msg))
    {
      TipcSrvMsgProcess(pl_Msg);
      TipcMsgDestroy(pl_Msg);

      /* Reinitialisation boucle de comptage */
      vl_IndPre = 0;
    }
    else
    {
#ifndef _TIBCO_EMS 
    /* Test de Presence RTserver */
    if ( (vl_IndPre >= 0) && ( (TutErrNumGet() != T_ERR_TIMEOUT_REACHED)) )
    {
			if (errno)
        XZST_03EcritureTrace (XZSTC_WARNING, "Erreur RTWorks num: %d -- errno %d \n",
          TutErrNumGet(), errno);
    }
    else if (vl_IndPre >= XDC_EXIT_RTCLIENT)
    {
      XZST_03EcritureTrace (XZSTC_FATAL, "Bouclage et arret tache tspng %d \n",vl_IndPre);
      /*A Arret de la tache */
      /*exit(0);*/
    }

    /*B INdication Non Presence RTserver */
			if (errno)
        vl_IndPre++;

    /*B Attente aleatoire pour permettre une reconnexion */
    vl_Attente = rand()%15;
#endif
#if 0
    sleep(vl_Attente);
#endif
    }

  }

}	/* Fin main */




/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *  Permet de terminier proprement le process tspng.x
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void spng_sortir()

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
 *   Aucun
 *
 * CONDITION D'UTILISATION
 *   Aucune
 *
 * FONCTION
 *   Annule la programmation du timer, ferme lefichier de traces et
 *	sort du process
 *
------------------------------------------------------*/
{

  XZSCT_Datagroup pl_NomDatagroup         = "\0";
  int	vl_ValRet	= XDC_OK;
  XZSCT_NomMachine pl_NomMachine		= "\0";

  /* Recuperation du nom de la machine locale */
  XZSC_07NomMachine(pl_NomMachine) ;

  /* abonnement au Datagroup XDG_NSUPPING_NomMachine */
  sprintf(pl_NomDatagroup,"%s_%s",XDG_NSUPPING,pl_NomMachine) ;
  XDG_JoindreDG(pl_NomDatagroup);

  /*A Annulation du Timer */
  if ( (vl_ValRet = XZSM_02AnnulerProgrammation("XDM_TSG_VERIF",pl_NomDatagroup,
      XDM_TSG_VERIF) ) != XDC_OK )
  {
    /* Affichage trace */
    XZST_03EcritureTrace(XZSTC_WARNING,"XZSM_02AnnulerProgrammation ");
  }

  /*A Fermeture du fichier trace */
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

  /*A Sortie de la tache */
  exit(0);

}	/* Fin spng_sortir */
