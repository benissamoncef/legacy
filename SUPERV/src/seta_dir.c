/*E*/
/* Fichier : $Id: seta_dir.c,v 1.25 2020/11/03 17:53:43 pc2dpdy Exp $	Release : $Revision: 1.25 $        Date : $Date: 2020/11/03 17:53:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_DIR * FICHIER seta_dir.c
******************************************************
* DESCRIPTION DU MODULE :
*
* Module pribncipale de la tache SETAT.
*	Permet de se connecter au RTServer et d'initialiser
*	les divers mecanismes RTWorks
*
******************************************************
* HISTORIQUE :
*
* T.Milleville  14 Oct 1994	: Creation
* T.Milleville  7 Fev 1995	: Modification Test abonnement 
*					Datagroups V 1.5
* T.Milleville  28 Juin 1995	: Modification Declenchement du Msg vie
*			V1.8
* T.Milleville  25 Jul  1995	: Ajout de commentaires V1.9
* T.Milleville  6 Aou  1995	: Modification code seta_sortie. V1.10
* T.Milleville  8 Aou  1995	: Ajout traces  V1.11
* T.Milleville  25 Aou  1995	: Ajout priorite pour les synchro
*	de taches V1.12
* T.Milleville  21 Sep 1995	: Ajout destruction connexion avec  le RTserveur
*	dans la focntion de sortie V1.13
* T.Milleville  27 Sep 1995	: Ajout abonnement a NSUPETAT_DN et NSUPETAT_DM 
*	si SD. Ajout d'un champ dans le message XDM_TSP_MIGRAZUR pour renseigner 
*	le site qui emet ce message V1.14
* T.Milleville  13 Oct 1995	: Gestion des serveurs Passifs V1.15
* P.Niepceron   23 Jan 1996	: Demande seta_init_etat a l'init pour recuperer l'etat 
				de la machine apres redemar. de tsetat sans demarr de tsaplg V1.16
* P.Niepceron   30 Jan 		: Correction de l'envoie de l'etat de la tache v1.16
* P.Niepceron   05 Fev 1996	: Mise a lour de l'etat de la machine meme si ce n'est pas un serveur v1.17
***
* P.Niepceron   22 Aou 1996	: Ajout du type de machine PC simplifie v1.18
* P.Niepceron   16 Dec 1997	: Ajout du type de machine PC 2 +STA  v1.19
* P.Niepceron   01 Jul 1998     : Ajout du type de machine Historique (dem/1697) v1.20
* JMG           29/09/09       : SECTO DEM 887
* JMG		26/06/10	 : linux DEM/934
* JPL		11/01/11 : Migration architecture HP ia64 (DEM 971) : suppression warnings mineurs  1.23
* JPL		08/02/18 : Migration Linux 64 bits (DEM 1274) : definition du type et des valeurs de retour des fonctions  1.24
* JMG					07/03/20 : EMS 1.25
* LCL		25/09/2023	: Adaptation code pour rabbitmq DEM-483
* LCL 27/09/23 : Adaptations CNA DEM-483
*******************************************************/

#define DEBUG 

/* fichiers inclus */

#include <xzsem.h>
#include "xzems.h"
#include "seta_don.h"
#include "seta_dir.h"
#include "seta_sur.h"
#include "seta_bas.h"
#include "seta_arc.h"
#include "seta_tra.h"
#include "seta_act.h"
#include "seta_con.h"
#include "seta_ini.h"
#include "seta_rep.h"

#include "xdc.h"
#include "xdg.h"
#include "xdm.h"

#include "xzsa.h"
#include "xzsc.h"
#include "xzst.h"
#include "xzss.h"
#include "xzsm.h"
#include "xzsg.h"

/* definitions de constantes */
#define SEC_TEMPO_MACHINE 10
#define SEC_TEMPO_ACTIF   10
#define SEC_TEMPO_PASSIF  SEC_TEMPO_ACTIF + 20

/* definitions de types locaux */

typedef struct {
		  int NumMsg;      /* Numero message RTW */
		  void (*FoncCB)(); /* Fonction callback */
               } tm_callback;

/* definition de macro locales */

/* declaration de variables locales */
static char *version = "$Id: seta_dir.c,v 1.25 2020/11/03 17:53:43 pc2dpdy Exp $ : seta_dir" ;

tm_callback pm_TabCB[] = {
                	     { XDM_TSE_QUES, (void (*)())seta_synchromachine_cb },
			     { XDM_TSE_MODI, (void (*)())seta_stmodei_cb },
			     { XDM_TSE_MODR, (void (*)())seta_stmoder_cb },
			     { XDM_TSE_MACH, (void (*)())seta_etamach_cb },
			     { XDM_TSE_SSYS, (void (*)())seta_etassys_cb },
			     { XDM_TSE_TACH, (void (*)())seta_etatach_cb },
			     { XDM_TSE_PERI, (void (*)())seta_etaperi_cb },
			     { XDM_TSE_INTE, (void (*)())seta_etainte_cb },
			     { XDM_TSE_TTMP, (void (*)())seta_etattmp_cb },
			     { XDM_TSE_MODE, (void (*)())seta_etamode_cb },
			     { XDM_TST_TRAC, (void (*)())seta_etatrac_cb },
			     { XDM_TSE_ISAG, (void (*)())seta_synopag_cb },
			     { XDM_TSE_ISSS, (void (*)())seta_synopss_cb },
			     { XDM_TSE_ISTC, (void (*)())seta_synoptc_cb },
			     { XDM_TST_INIT, (void (*)())seta_synopts_cb },
			     { XDM_TSE_INIT, (void (*)())seta_init_cb },
			     { XDM_TSE_MESG, (void (*)())seta_mesg_cb },
			     { XDM_TSE_DIFE, (void (*)())seta_diffetat_cb },
			     { XDM_TSE_QACT, (void (*)())seta_synchroactif_cb },
			     { T_MT_INFO,    (void (*)())seta_archiver_cb },
			     { XDM_TSE_SURV, (void (*)())seta_survie_cb },
			     { XDM_TSE_VIE, (void (*)())seta_msgvie_cb },
			     { XDM_TSE_RES, (void (*)())seta_etares_cb },
			     { XDM_TSE_APPLI, (void (*)())seta_etappli_cb },
			     { XDM_TSE_BASC, (void (*)())seta_bascule_cb },
			     { XDM_TSE_STARTTMP, (void (*)())seta_init_starttmp_cb },
                          };



/* declaration de fonctions internes */
int seta_servactif();
int seta_servpassif();
int seta_synchroactif();
int seta_synchromachine();
void seta_arrettache(int);
void seta_sortir();
int seta_abonnedg();

int seta_questrepons(T_IPC_MSG va_MsgQuestion,
	               XZSCT_Datagroup pa_NomDatagroup,
		       T_IPC_MSG *pa_MsgReponse,
	               T_IPC_MT va_TypeMsgReponse,
	               int va_DureeAttente);
int seta_init_etat(
	XZSCT_NomMachine        va_NomMachine,
	int                     va_NumOrdre,
	void                    *pa_Args);
			
int seta_connexion(
	XZSCT_NomMachine 	pa_NomMachine,
     	int 			va_NumOrdre,
	void                    *pa_Args);



/* definition de fonctions externes */



/*X*/
/* Fichier : $Id: seta_dir.c,v 1.25 2020/11/03 17:53:43 pc2dpdy Exp $	Release : $Revision: 1.25 $        Date : $Date: 2020/11/03 17:53:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SEDIRIGER
******************************************************
* DESCRIPTION DE LA FONCTION :
*	Fonction principale de la tache tsetat.x
*	Connexion au Rtserver, armement des callbacks et abonnement
*	au datagroupes.
*	Determination du calculateur actif.
*	Synchronisation avec tsaplg.x, puis declanchement des timer 
*	d'envoie de msg de vie et de surveillance des machines
*
******************************************************
* SEQUENCE D'APPEL :
*/

int main(int argc, char *argv[])

/*
* PARAMETRES EN ENTREE :
*	argc	: Nombre d'arguments
*       argv	: Valeurs des arguments
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : 
*	Aucun
*
* CONDITION D'ERREUR :
*
* 	Aucune
*
* CONDITIONS LIMITES :
*
* 	Aucune
*
******************************************************/
{
   static char *version = "$Id: seta_dir.c,v 1.25 2020/11/03 17:53:43 pc2dpdy Exp $ : main" ;
   int 			vl_nCB		= 0;
   int 			vl_EtatMachine	= 0;
   XZSCT_Datagroup 	pl_NomDatagroup	= "";
   T_IPC_MSG 		pl_Msg;
   T_IPC_MT  		vl_mt		= 0;
   XZSCT_NomMachine	vl_NomSDActif	= "";
   int 			vl_PeriodeVie	= 0;
   int			vl_NbElts	= 0;
   int			vl_IndPre	= 0;
   int			vl_NumOrdre	= -1;
   int			vl_Attente	= 0;
   T_INT4		vl_SockRT	= 0;


	XZST_03EcritureTrace(XZSTC_FONCTION, 
		"Main  : IN  \n");

	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

   /* Initialisation de la tache et connexion au RTServer */
   if( XZSC_01InitCnxSup(argv[1],
	      argv[0],
              XZSCC_INTERDIRE,
	      XZSCC_AUTORISER,
	      XZSCC_INTERDIRE,
	      seta_sortir,
	      "/dev/null") != XDC_NOK)
   {


//  XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE,XZSTC_NECRAN);

      XZST_03EcritureTrace(XZSTC_INFO, "Connexion reussie INFO");

      /* Initialisation des variables de configuration */
      XZSC_04NomSite(SEP_NOM_SITE);
      XZSC_07NomMachine(SEP_NOM_MACHINE);
      XZSC_05TypeMachine(&SEV_TYPE_MACHINE);
      XZSC_06NomTache(SEP_NOM_TACHE);

      /*A Initialisation des callbacks de la tache */
      for (vl_nCB = 0; 
		vl_nCB < (sizeof(pm_TabCB)/sizeof(pm_TabCB[0])); vl_nCB++)
      {
	 vl_mt = XDM_IdentMsg(pm_TabCB[vl_nCB].NumMsg);
	 if(!TipcSrvProcessCbCreate(vl_mt,
				pm_TabCB[vl_nCB].FoncCB,
				(T_CB_ARG)NULL))
         {
	    XZST_03EcritureTrace(XZSTC_WARNING,
			"main : Erreur armement du callback %d",vl_nCB);
         }
      }
    
#ifndef _TIBCO_EMS
      /* verification de la presence d'un SD ou d'un SC */
      if ((SEV_TYPE_MACHINE == XZSCC_TYPEM_ST) 
		|| (SEV_TYPE_MACHINE == XZSCC_TYPEM_STA)
		|| (SEV_TYPE_MACHINE == XZSCC_TYPEM_HIS) /* ajout PC2 modif 1.20 */
		|| (SEV_TYPE_MACHINE == XZSCC_TYPEM_SC)
		|| (SEV_TYPE_MACHINE == XZSCC_TYPEM_PC2)
		|| (SEV_TYPE_MACHINE == XZSCC_TYPEM_PC2LT)	/* ajout PC2 modif 1.19*/
		|| (SEV_TYPE_MACHINE == XZSCC_TYPEM_PCS)	/* ajout PCS modif 1.18 */
		|| (SEV_TYPE_MACHINE == XZSCC_TYPEM_SP)
		|| (SEV_TYPE_MACHINE == XZSCC_TYPEM_LT) )
      {
		/* verification de la presence d'un SD ou d'un SC */
         	seta_synchromachine();
      }
#endif

#if 0
      /* traitements particuliers a a un serveur 
      *				de communication ou de donnees */
      if ((SEV_TYPE_MACHINE == XZSCC_TYPEM_SC) ||
	  (SEV_TYPE_MACHINE == XZSCC_TYPEM_SD))
      {
	 /* definition de l'etat de la machine et de la machine de secours  */
	 seta_synchroactif();
      }
      else
      {
		/*A On force l'etat de la machine a ACTIF */
	        XZSE_25ActualiserEtatMachine(XZSEC_ETAT_NACTIF);
      }
#endif

#if 1
      	/* traitements particuliers a a un serveur
	*                         de communication ou de donnees */
	/*  Si SC et serveur passif alors memorisation de l'etat a PASSIF
	*  Si SD et serveur Passif alors memorisation de l'etat a PASSIF */
	if ( (SEV_TYPE_MACHINE == XZSCC_TYPEM_SD) ||
			(SEV_TYPE_MACHINE == XZSCC_TYPEM_SC) ||
			(SEV_TYPE_MACHINE == XZSCC_TYPEM_PC2) ||
			(SEV_TYPE_MACHINE == XZSCC_TYPEM_PC2LT) || /* ajout PC2 modif 1.19 */
			(SEV_TYPE_MACHINE == XZSCC_TYPEM_PCS) )  /* ajout PCS modif 1.18 */
	{
		/*  Si SC et serveur passif alors memorisation de l'etat a PASSIF
		*        Si SD et serveur Passif alors memorisation de l'etat a PASSIF */
		if ( (!strcmp(SEP_NOM_MACHINE,XDC_PASSIF_DN))
				|| (!strcmp(SEP_NOM_MACHINE,XDC_PASSIF_DM))
				|| (!strcmp(SEP_NOM_MACHINE,XDC_PASSIF_CI)))
		{
			XZST_03EcritureTrace(XZSTC_WARNING," Serveur Passif %s \n",
							SEP_NOM_MACHINE);
			/*A On force l'etat de la machine a PASSIF */
			XZSE_25ActualiserEtatMachine(XZSEC_ETAT_NPASSIF);
		}
		else
		{
			/*A On force l'etat de la machine a ACTIF */
			XZSE_27ActualiserNomMachineActive(SEP_NOM_MACHINE);
			XZSE_25ActualiserEtatMachine(XZSEC_ETAT_NACTIF);
		}
	}
	else
	{  /* modif v1.17 */
	XZSE_25ActualiserEtatMachine(XZSEC_ETAT_NACTIF);
	}
#endif

      /*A Reservation des ressources d'administration */
      XZSA_20InitAdmin();

      /*A initialisation des donnees de la tache */ 
      seta_init();

      	/*A Abonnement aux DataGroups specifiques aux
	*	serveurs ACTIFS   */
      	seta_abonnedg();

#if 0
	/*A Initialisation des etats des SSystemes, des taches et des machines
	*	suivant sur quel calculatuer on se trouve */
	if ( SEV_TYPE_MACHINE == XZSCC_TYPEM_SD)
	{
		/*B Envoie du message XDM_TSP_ETAT a toute les 
		*	machines */
		XZSA_01ListeMachine(&seta_init_etat, &vl_NbElts, NULL);
	}
	else if (( SEV_TYPE_MACHINE == XZSCC_TYPEM_SC) || 
		 ( SEV_TYPE_MACHINE == XZSCC_TYPEM_PCS) || 
		 ( SEV_TYPE_MACHINE == XZSCC_TYPEM_PC2) || /* ajout PC2 modif 1.19 */
		  ( SEV_TYPE_MACHINE == XZSCC_TYPEM_PC2LT) )  /* ajout PCS modif 1.18 */
	{
		/*B  Envoie  du msg XDM_TSP_ETAT a toute les machines
		*		du site **/
		XZSA_19ListeMachineSite(SEP_NOM_SITE,
				&seta_init_etat, &vl_NbElts,NULL);
	}

#endif

      /* recuperation de l'etat de la machine locale */
      XZSE_09EtatMachine(&vl_EtatMachine);

      /* realisation des traitements relatifs 
      *		a la synchromachine selon le type de  la machine */
      if ((SEV_TYPE_MACHINE == XZSCC_TYPEM_SC) ||
	  (SEV_TYPE_MACHINE == XZSCC_TYPEM_PCS)||
	  (SEV_TYPE_MACHINE == XZSCC_TYPEM_PC2)||
	  (SEV_TYPE_MACHINE == XZSCC_TYPEM_PC2LT)|| /* ajout PC2 modif 1.19 */
	  (SEV_TYPE_MACHINE == XZSCC_TYPEM_SD))	/* ajout PCS modif 1.18 */
      {
	 if (vl_EtatMachine == XZSEC_ETAT_NACTIF)
	 {
		XZST_03EcritureTrace(XZSTC_INFO," Serveur actif %d \n",
							SEV_TYPE_MACHINE);
		/*B Serveur Actif */
	    	seta_servactif();
         }
	 else
	 { 
		XZST_03EcritureTrace(XZSTC_INFO," Serveur passif %d \n",
							SEV_TYPE_MACHINE);
		/*B Serveur Passif */
	    	seta_servpassif();
         }
      }
      

      /* Recuperation du nom du SD actif */
      XZSE_19NomSDActif(vl_NomSDActif);

      /*A synchronisation avec TSAPLG */

	/* Envoie Msg XDM_TSE_MAJD */
      XDG_EncodeDG2(pl_NomDatagroup, XDG_NSUPAPLG,SEP_NOM_MACHINE);

      XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoie du Msg XDM_TSE_MAJD \
		-- SDActif %s \n",vl_NomSDActif);

      TipcSrvMsgWrite(pl_NomDatagroup
		     ,XDM_IdentMsg(XDM_TSE_MAJD)
		     ,XDM_FLG_SRVMSGWRITE
		     ,T_IPC_FT_STR
		     ,vl_NomSDActif
		     ,NULL);

	/* Envoie Msg XDM_TSE_MAJE */
      XDG_EncodeDG2(pl_NomDatagroup, XDG_NSUPAPLG,SEP_NOM_MACHINE);

      XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoie du Msg XDM_TSE_MAJE \
		-- EtatMachine  %d \n",vl_EtatMachine);

      TipcSrvMsgWrite(pl_NomDatagroup
		     ,XDM_IdentMsg(XDM_TSE_MAJE)
		     ,XDM_FLG_SRVMSGWRITE
		     ,T_IPC_FT_INT4
		     ,vl_EtatMachine
		     ,NULL);

      /* pause pour attente lancement des autres 
      *			taches (spng et stim entre autre */

      if (( SEV_TYPE_MACHINE == XZSAC_TYPEM_ST ) || ( SEV_TYPE_MACHINE == XZSAC_TYPEM_STA ) || ( SEV_TYPE_MACHINE == XZSAC_TYPEM_HIS) )
      sleep(30);
      else
      sleep(20); 

      /*A Initialisation services ping */
      XZSG_00InitServices();

      /*A Initialisation services timer */
      XZSM_00InitServices (); 

      /*A Demande d'envoi du message de surveillance des machines au timer */
      XZSC_03PeriodeTestReseau(&vl_PeriodeVie);
      XDG_EncodeDG2(pl_NomDatagroup, SEP_NOM_MACHINE, SEP_NOM_TACHE);
      if (XZSM_01DemanderProgrammation(0, vl_PeriodeVie, 
		pl_NomDatagroup, XDM_TSE_SURV,"XDM_TSE_SURV" , 1) != XDC_OK)
      { 
	XZST_03EcritureTrace(XZSTC_WARNING ,
		"main : Erreur demande programmation surveillance Machine \n");
      }

      /* Armement de l'envoi du message de vie */
      /* envoi du message via timer */
      XDG_EncodeDG2(pl_NomDatagroup, XDG_NSUPVIE, SEP_NOM_MACHINE);
      if (XZSM_01DemanderProgrammation(0, vl_PeriodeVie, pl_NomDatagroup, 
		XDM_TSE_VIE, SEP_NOM_MACHINE, 10) != XDC_OK)
      {
	XZST_03EcritureTrace(XZSTC_DEBUG1,
		"main : Erreur demande programmation surveillance reseau \n");
      } 

	/*A Envoie de l'etat de la  Tache */
	XZST_03EcritureTrace(XZSTC_INFO ,"Envoie etat tache %s -- %d \
		dans seta_dir.c  \n",SEP_NOM_TACHE,
		vl_EtatMachine);
        XZST_03EcritureTrace(XZSTC_WARNING ,"Envoie etat tache %s -- %d \
			dans seta_dir.c  \n",SEP_NOM_TACHE,
					vl_EtatMachine);



	/* Correction v1.16 
	XZSE_12EnvoieEtatTache(SEP_NOM_MACHINE,
				SEP_NOM_TACHE, vl_EtatMachine, 10);*/

	XZSE_12EnvoieEtatTache(SEP_NOM_MACHINE,
			SEP_NOM_TACHE, XZSEC_ETAT_OK , 10); 

	/*A Envoie des msg de connexion aux diverses machines
	*	suivant le type du serveur */
	if ( (SEV_TYPE_MACHINE == XZSCC_TYPEM_SD) &&
		 (vl_EtatMachine != XZSEC_ETAT_NPASSIF) )

	{
		/*B Envoie du message XDM_TSP_MIGRAZUR a toute les 
		*	machines */
		XZSA_01ListeMachine(&seta_connexion, &vl_NbElts, NULL);
	}
	else if ( ((SEV_TYPE_MACHINE == XZSCC_TYPEM_SC) || 
		   (SEV_TYPE_MACHINE == XZSCC_TYPEM_PC2) ||
		   (SEV_TYPE_MACHINE == XZSCC_TYPEM_PC2LT) ||	 /* ajout PC2 modif 1.19 */
		   (SEV_TYPE_MACHINE == XZSCC_TYPEM_PCS)) &&
		(vl_EtatMachine != XZSEC_ETAT_NPASSIF) )		/* ajout PCS modif 1.18 */

	{
		/*B  Envoie  du msg XDM_TSP_MIGRAZUR a toute les machines
		*		du site **/
		XZSA_19ListeMachineSite(SEP_NOM_SITE,
				&seta_connexion, &vl_NbElts,NULL);
	}
	else
	{
	XZST_03EcritureTrace(XZSTC_WARNING ,"Appel seta_connexion \
		mach %s \n",SEP_NOM_MACHINE);


		/*B  Envoie  du msg XDM_TSP_MIGRAZUR a la machine */ 
		seta_connexion(SEP_NOM_MACHINE,vl_NumOrdre,NULL);
	}

#if 1
	/*A Initialisation des etats des SSystemes, des taches et des machines
	*      suivant sur quel calculatuer on se trouve */
	if ( (SEV_TYPE_MACHINE == XZSCC_TYPEM_SD) &&
				(vl_EtatMachine != XZSEC_ETAT_NPASSIF) )
	{
		/*B Envoie du message XDM_TSP_ETAT a toute les  machines */
		XZSA_01ListeMachine(&seta_init_etat, &vl_NbElts, NULL);
	}
	else if ( ((SEV_TYPE_MACHINE == XZSCC_TYPEM_SC) || (SEV_TYPE_MACHINE == XZSCC_TYPEM_PCS) || 
		   (SEV_TYPE_MACHINE == XZSCC_TYPEM_PC2) || (SEV_TYPE_MACHINE == XZSCC_TYPEM_PC2LT)) &&
			(vl_EtatMachine != XZSEC_ETAT_NPASSIF) )  /* ajout PCS modif 1.18 */ /* ajout PC2 modif 1.19 */
	{
		/*B  Envoie  du msg XDM_TSP_ETAT a toute les machines
		*               du site **/
		XZSA_19ListeMachineSite(SEP_NOM_SITE,
					&seta_init_etat, &vl_NbElts,NULL);
	}
	else 
	{ /* Demande l'etat de sa machine  Ajout v1.16*/ 
		seta_init_etat( SEP_NOM_MACHINE , 0 , 0 );	
	}
#endif


      /* Attente bloquante des messages du RTServer */
      while(1)
      {

		/* Recuperation du msg RTworks suivant */
		pl_Msg = TipcSrvMsgNext(T_TIMEOUT_FOREVER);

        if(isMessageNotNULL(pl_Msg))
	 	{
			/*TipcMsgPrint(pl_Msg, TutOut);*/
			TipcSrvMsgProcess(pl_Msg);
			TipcMsgDestroy(pl_Msg);

			/* Memorisation Test Presence RTserver */
			vl_IndPre = 0;
         }
#ifndef _TIBCO_EMS
	 /* erreur RTWORKS */
	 else 
         {
		/* Test de Presence RTserver */
		if (vl_IndPre == 0) 
		{
			if (errno)
				XZST_03EcritureTrace (XZSTC_WARNING, "Erreur RTWorks num: %d -- errno %d \n",
						TutErrNumGet(), errno);
		}
		else if (vl_IndPre >= XDC_EXIT_RTCLIENT)
		{
			XZST_03EcritureTrace (XZSTC_WARNING, "Bouclage et arret tache tsetat %d \n",vl_IndPre);
			/*A Arret de la tache */
			seta_sortir();
		}

		/*B INdication Non Presence RTserver */
			if (errno)
			 vl_IndPre++;

		/*B Attente aleatoire pour permettre une reconnexion */
		vl_Attente = rand()%15;
	}
#endif
      }
   }
   else
   {
      /* erreur de connexion de la tache */
      XZST_03EcritureTrace(XZSTC_WARNING,
			"Erreur de connexion de la tache\n");
   }

   return (0);
}	/* Fin du Main  */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet d'envoyer le message XDM_TSP_ETAT sur le
*	DatagRoup XDG_NSUPAPLG_NomMachine 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int seta_init_etat(
	XZSCT_NomMachine	va_NomMachine,
	int			va_NumOrdre,
	void 			*pa_Args)

/*
* ARGUMENTS EN ENTREE :
*    	va_NomMachine : Nom de la machine
*	va_NumOrdre : Numero d'ordre de la machine dans la liste 
*	pa_Args :	 Arguments 
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   XDC_OK	: ok
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Envoie le message XDM_TSP_ETAT sur le datagroup XDG_NSUPAPLG_NomMachine
*
------------------------------------------------------*/
{
	static char *version="$Id: seta_dir.c,v 1.25 2020/11/03 17:53:43 pc2dpdy Exp $ : seta_init_etat " ;
	int		vl_ValRet	= XDC_OK;
	XZSCT_Datagroup vl_NomDatagroup = "";



      	XZST_03EcritureTrace(XZSTC_FONCTION ,
		" seta_init_etat : IN -- Nom Mach %s \
		-- Num Ordre %d \n",va_NomMachine,va_NumOrdre);

	/*A Envoie du Msg XDM_TSP_ETAT sur le datagroup 
	*				XDG_NSUPAPLG_NomMAchine */

	/*B Construction du Datagroup XDG_NSUPAPLG_NomMAchine */
	XDG_EncodeDG2(vl_NomDatagroup,XDG_NSUPAPLG,va_NomMachine);

	/*B Envoie du Msg XDM_TSP_ETAT */
	if (!TipcSrvMsgWrite(vl_NomDatagroup,
			XDM_IdentMsg(XDM_TSP_ETAT),
			XDM_FLG_SRVMSGWRITE,
			NULL) )
	{
		/* erreur creation message de demande */
		XZST_03EcritureTrace(XZSTC_WARNING,
	    		"seta_init_etat : erreur envoie msg XDM_TSP_ETAT \n");

		vl_ValRet = XDC_NOK;
       	}
	else
	{
		XZST_03EcritureTrace(XZSTC_MESSAGE ,
			"Envoie du Msg XDM_TSP_ETAT a la machine %s \n",
			va_NomMachine);
	}

      	XZST_03EcritureTrace(XZSTC_FONCTION ,
		" seta_init_etat : OUT  -- Code Retour %d \n",vl_ValRet);

	return (vl_ValRet);

}	/* Fin seta_init_etat */



/*X*/
/* Fichier : $Id: seta_dir.c,v 1.25 2020/11/03 17:53:43 pc2dpdy Exp $	Release : $Revision: 1.25 $        Date : $Date: 2020/11/03 17:53:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SEDIRIGER
******************************************************
* DESCRIPTION DE LA FONCTION :
* Abonne la tache au Datagroups suivants selon le type
* de la machine:
*   - XDG_NSUPHIST (SD)
*   - XDG_NSUPHIST_NomSite (SD et SC)
*   - XDG_NSUPETAT_NomSite (SD et SC)
*   - XDG_NSUPETAT (SD)
*   - XDG_NSUPETAT_NomMachine (ST, SC, SD)
*
******************************************************
* SEQUENCE D'APPEL :
*/

int seta_abonnedg()

/*
* PARAMETRES EN ENTREE :
* 	Aucun
*
* PARAMETRES EN SORTIE :
*	Aucun
*
* VALEUR(S) DE RETOUR : 
* XDC_OK	: ok
* XDC_NOK	: nok
*
* CONDITION D'ERREUR :
* XDC_NOK: erreur RTWorks 
*
* CONDITIONS LIMITES :
* - 
*
*
******************************************************/
{
   static char *version="$Id: seta_dir.c,v 1.25 2020/11/03 17:53:43 pc2dpdy Exp $ : seta_abonnedg " ;
   int 			vl_ValRet 	= XDC_OK;
   XZSCT_Datagroup 	pl_NomDatagroup	= "";
   XZSCT_NomMachine	vl_NomSDActif	= "";
   int			vl_EtatMachine	= 0;

      	XZST_03EcritureTrace(XZSTC_FONCTION ,
		" seta_abonnedg : IN \n");


	/* recuperation de l'etat de la machine locale */
      	XZSE_09EtatMachine(&vl_EtatMachine);

   if ( ((SEV_TYPE_MACHINE == XZSCC_TYPEM_SC) ||
       (SEV_TYPE_MACHINE == XZSCC_TYPEM_PCS) ||
       (SEV_TYPE_MACHINE == XZSCC_TYPEM_PC2) || /* ajout PC2 modif 1.19 */
       (SEV_TYPE_MACHINE == XZSCC_TYPEM_PC2LT) ||
       (SEV_TYPE_MACHINE == XZSCC_TYPEM_SD))
       && (vl_EtatMachine != XZSEC_ETAT_NPASSIF) )		/* ajout PCS modif 1.18 */
   {
      /* Abonnement au DataGroup XDG_NSUPHIST_NomSite */
      XDG_EncodeDG2(pl_NomDatagroup, XDG_NSUPHIST, SEP_NOM_SITE);
      if (!XDG_JoindreDG(pl_NomDatagroup)) 
      {
         /* erreur abonnement */
	 XZST_03EcritureTrace(XZSTC_WARNING,
		      "WARNING : erreur abonnement au DG %s", pl_NomDatagroup);
	 vl_ValRet = XDC_NOK;
      }

      /* Abonnement au DataGroup XDG_NSUPETAT_NomSite */
      XDG_EncodeDG2(pl_NomDatagroup, XDG_NSUPETAT, SEP_NOM_SITE);
      if (!XDG_JoindreDG(pl_NomDatagroup)) 
      {
         /* erreur abonnement */
	 XZST_03EcritureTrace(XZSTC_WARNING,
		      "WARNING : erreur abonnement au DG %s", pl_NomDatagroup);
	 vl_ValRet = XDC_NOK;
      }

#if 0

      /*A Si Machine est un SC ou PCS non connectee a un SD */
      /*B Recuperation du nom du SD actif */
      XZSE_19NomSDActif(vl_NomSDActif);
      if ( ((SEV_TYPE_MACHINE ==  XZSCC_TYPEM_PC2) || (SEV_TYPE_MACHINE ==  XZSCC_TYPEM_PC2LT) || 
	    (SEV_TYPE_MACHINE ==  XZSCC_TYPEM_PCS) || (SEV_TYPE_MACHINE ==  XZSCC_TYPEM_SC)) &&
		(strlen(vl_NomSDActif) == 0) )	/* ajout PCS modif 1.18 */ /* ajout PC2 modif 1.19 */
	{
		/*A Abonnement a NSUPETAT */
		if (!XDG_JoindreDG(XDG_NSUPETAT))
		{
			/* erreur abonnement */
			XZST_03EcritureTrace(XZSTC_WARNING,
	   "WARNING : erreur abonnement au DG %s", XDG_NSUPETAT);
			vl_ValRet = XDC_NOK;
		 }
	}
#endif

      if (SEV_TYPE_MACHINE ==  XZSCC_TYPEM_SD)
      {
	/*B Teste si Serveur Non Passif */
	if (vl_EtatMachine != XZSEC_ETAT_NPASSIF)
	{
         /* Abonnement au DataGroups XDG_NSUPHIST */
         if (!XDG_JoindreDG(XDG_NSUPHIST))
         {
	    /* erreur abonnement */
	    XZST_03EcritureTrace(XZSTC_WARNING,
	         "WARNING : erreur abonnement au DG %s", XDG_NSUPHIST);
	    vl_ValRet = XDC_NOK;
         }

         /* Abonnement au DataGroups XDG_NSUPETAT */
         if (!XDG_JoindreDG(XDG_NSUPETAT))
         {
	    /* erreur abonnement */
	    XZST_03EcritureTrace(XZSTC_WARNING,
	         "WARNING : erreur abonnement au DG %s", XDG_NSUPETAT);
	    vl_ValRet = XDC_NOK;
         }

	}

      }
   }

   /* abonnement au datagroup XDG_NSUPETAT_NomMachine */
   XDG_EncodeDG2(pl_NomDatagroup, XDG_NSUPETAT, SEP_NOM_MACHINE);
   if(!XDG_JoindreDG(pl_NomDatagroup))
   {
      /* erreur abonnement */
      XZST_03EcritureTrace(XZSTC_WARNING,
	   "WARNING : erreur abonnement au DG %s", pl_NomDatagroup);
      vl_ValRet = XDC_NOK;
   
   }

      	XZST_03EcritureTrace(XZSTC_FONCTION ,
		" seta_abonnedg : OUT  -- Code Retour %d \n",vl_ValRet);

   return(vl_ValRet);

}	/* Fin seta_abonnedg */



/*X*/
/* Fichier : $Id: seta_dir.c,v 1.25 2020/11/03 17:53:43 pc2dpdy Exp $	Release : $Revision: 1.25 $        Date : $Date: 2020/11/03 17:53:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SEDIRIGER
******************************************************
* DESCRIPTION DE LA FONCTION :
*	Permet de se synchroniser avec la tache tsetat.x
*	du serveur de son site. Encoie me msg XDM_TSE_QUES et attends
*	le msg XDM_TSE_MAJD XDM_TSE_MAJC  permettant de recuperer 
*	le nom du SD actif et du SC actif
*
******************************************************
* SEQUENCE D'APPEL :
*/

int seta_synchromachine()

/*
* PARAMETRES EN ENTREE :
*	Aucun
*
* PARAMETRES EN SORTIE :
*	Aucun
*
* VALEUR(S) DE RETOUR : 
*	Aucune
*
* CONDITION D'ERREUR :
*
* 	Aucune
*
* CONDITIONS LIMITES :
*
* 	Aucune
*
******************************************************/
{
   static char *version="$Id: seta_dir.c,v 1.25 2020/11/03 17:53:43 pc2dpdy Exp $ : seta_synchromachine " ;
   int 			vl_ValRet 	= XDC_OK;
   XZSCT_Datagroup 	pl_NomDatagroup	= "";
   T_IPC_MSG 		pl_MsgQuestion;
   T_IPC_MSG 		pl_MsgReponse;


      	XZST_03EcritureTrace(XZSTC_FONCTION ,
		" seta_synchromachine  : IN \n");

#ifndef _TIBCO_EMS

   /* construction du nom du datagroup d'envoi du message de demande
   selon le type de la machine locale */
   if (    (SEV_TYPE_MACHINE  == XZSCC_TYPEM_ST)
	|| (SEV_TYPE_MACHINE  == XZSCC_TYPEM_STA)
	|| (SEV_TYPE_MACHINE  == XZSCC_TYPEM_HIS)
	|| (SEV_TYPE_MACHINE  == XZSCC_TYPEM_LT)
	|| (SEV_TYPE_MACHINE  == XZSCC_TYPEM_SP) )
   {
	/*B Construction du Nom du Datagroup XDG_NSUPHIST_SEP_NOM_SITE */
      	XDG_EncodeDG2(pl_NomDatagroup, XDG_NSUPHIST, SEP_NOM_SITE);
   }
   else
   {
	/*B Construction du Nom du Datagroup XDG_NSUPHIST */
      	strcpy(pl_NomDatagroup, XDG_NSUPHIST);
   }
      
   /*A construction du message de demande XDM_TSE_QUES 
   *	avec SEP_NOM_MACHINE et SEP_NOM_TACHE */
   pl_MsgQuestion = TipcMsgCreate(XDM_IdentMsg(XDM_TSE_QUES));

/*	Correction bug 
   if (!TipcSrvMsgWrite(pl_NomDatagroup,
		     XDM_IdentMsg(XDM_TSE_QUES),
                     T_IPC_FT_STR, SEP_NOM_MACHINE,
		     T_IPC_FT_STR, SEP_NOM_TACHE,
		     NULL))
*/
   if (!TipcMsgWrite(pl_MsgQuestion,
		           T_IPC_FT_STR, SEP_NOM_MACHINE,
		           T_IPC_FT_STR, SEP_NOM_TACHE,
		           NULL))
   {
       /* erreur creation message de demande */
       XZST_03EcritureTrace(XZSTC_WARNING,
       	    "seta_synchromachine : erreur creation message de demande ");
   }
       
       /*A Envoi de la question et attente de la reponse 
       *	du type XDM_TSE_MAJD */
   vl_ValRet = seta_questrepons(pl_MsgQuestion,
 			      pl_NomDatagroup,
			      &pl_MsgReponse,
			      XDM_IdentMsg(XDM_TSE_MAJD),
		              SEC_TEMPO_MACHINE);

   /* arret de la tache si reponse non recue */
   if ((vl_ValRet == XDC_OK))
   {
      if (pl_MsgReponse == NULL)
      {
          /* arret de la tache */
          seta_arrettache(SEV_TYPE_MACHINE);
      }
      else
      {
          /* exploitation du message */
	  TipcSrvMsgProcess(pl_MsgReponse);
	  TipcMsgDestroy(pl_MsgReponse);
      } 
   }

       /*A Envoi de la question et attente de la reponse 
       *	du type XDM_TSE_MAJC */
   vl_ValRet = seta_questrepons(pl_MsgQuestion,
 			      pl_NomDatagroup,
			      &pl_MsgReponse,
			      XDM_IdentMsg(XDM_TSE_MAJC),
		              SEC_TEMPO_MACHINE);

   /* arret de la tache si reponse non recue */
   if ((vl_ValRet == XDC_OK))
   {
      if (pl_MsgReponse == NULL)
      {
          /* arret de la tache */
          seta_arrettache(SEV_TYPE_MACHINE);
      }
      else
      {
          /* exploitation du message */
	  TipcSrvMsgProcess(pl_MsgReponse);
	  TipcMsgDestroy(pl_MsgReponse);
      } 
   }

      	XZST_03EcritureTrace(XZSTC_FONCTION ,
		" seta_synchromachine : OUT  -- Code Retour %d \n",vl_ValRet);
#endif
   return(vl_ValRet);

}	/* Fin seta_synchromachine */




/*X*/
/* Fichier : $Id: seta_dir.c,v 1.25 2020/11/03 17:53:43 pc2dpdy Exp $	Release : $Revision: 1.25 $        Date : $Date: 2020/11/03 17:53:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE MODULE
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction stoppe la tache. 
* Elle est appele de facon asynchrone a l'aide du
* signal SIGALRM lorsque le nom du serveur de donnees
* actif n'a pas ete recu dans le temps imparti.
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_arrettache(	int	va_typeMachineLocale)

/*
* PARAMETRES EN ENTREE : type de la machine sur laquelle
*			tourne cette tache
*
*
* PARAMETRES EN SORTIE :
* 	Aucune
*
* VALEUR(S) DE RETOUR : 
* 	Aucune
*
* CONDITION D'ERREUR :
* 	Aucune-
*
* CONDITIONS LIMITES :
*
* 	Aucune
******************************************************/
{
   static char *version="$Id: seta_dir.c,v 1.25 2020/11/03 17:53:43 pc2dpdy Exp $ : seta_arrettache " ;
   char	vl_messageErr[XZSSC_ERRBUFSIZE]	= "";


      	XZST_03EcritureTrace(XZSTC_FONCTION ,
		" seta_arrettache : IN   -- Type Mach locale %d \n",
		va_typeMachineLocale);

   /* Determination du message d'erreur */
   if ( (va_typeMachineLocale == XZSCC_TYPEM_ST)
    	|| (va_typeMachineLocale == XZSCC_TYPEM_STA)
	|| (va_typeMachineLocale == XZSCC_TYPEM_HIS)
    	|| (va_typeMachineLocale == XZSCC_TYPEM_SP)
    	|| (va_typeMachineLocale == XZSCC_TYPEM_LT) )
   {
	strcpy(vl_messageErr,"le serveur de communications");
   }
   else
   {
	strcpy(vl_messageErr,"le serveur de donnees");
   }

   /* Trace de l'erreur de lancement de la tache */
   XZST_03EcritureTrace(	XZSTC_WARNING, 
			"Synchro machine non realisee avec %s",
				vl_messageErr);

   /* Fermeture du fichier de trace */
   /*XZST_02FermetureTrace();*/

   /* Affichage du message d'erreur sur la sortie standard */
   printf("Application MIGRAZUR: Synchro machine non realisee avec %s\n",vl_messageErr);

      	XZST_03EcritureTrace(XZSTC_FONCTION ,
		" seta_arrettache : OUT \n");

}	/* Fin seta_arrettache */



/*X*/
/* Fichier : $Id: seta_dir.c,v 1.25 2020/11/03 17:53:43 pc2dpdy Exp $	Release : $Revision: 1.25 $        Date : $Date: 2020/11/03 17:53:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SEDIRIGER
******************************************************
* DESCRIPTION DE LA FONCTION :
*	Permet de determiner le serveur actif
*	Envoie du msg XDM_TSE_QACT et attente de la reponse
*	par le msg XDM_TSE_RACT
*
******************************************************
* SEQUENCE D'APPEL :
*/

int seta_synchroactif()

/*
* PARAMETRES EN ENTREE :
* 	Aucun
*
* PARAMETRES EN SORTIE :
* 	Aucun
*
* VALEUR(S) DE RETOUR : 
*	Aucune
*
* CONDITION D'ERREUR :
*
* 	Aucune
* CONDITIONS LIMITES :
*
* 	Aucune
*
******************************************************/
{
   static char *version="$Id: seta_dir.c,v 1.25 2020/11/03 17:53:43 pc2dpdy Exp $ : seta_synchroactif " ;
   int 			vl_ValRet		= XDC_OK;
   XZSCT_Datagroup 	pl_NomDatagroup		= "";
   T_IPC_MSG 		pl_MsgQuestion;
   T_IPC_MSG 		pl_MsgReponse;
   T_STR 		pl_NomMachineSec	= NULL;
   int 			vl_EtatMachineSec	= 0;
   int 			vl_Priorite		= 0;
   int 			vl_TempoSynchro		= 0;


      	XZST_03EcritureTrace(XZSTC_FONCTION ,
		" seta_synchroactif  : IN \n");

#ifndef _TIBCO_EMS
	/*A Abonnement au datagroup de synchronisation  
	*			XDG_NSUPSYNCHRO_NomSite */
	XDG_EncodeDG2(pl_NomDatagroup, XDG_NSUPSYNCHRO, SEP_NOM_SITE);
	if (!XDG_JoindreDG(pl_NomDatagroup))
	{
	       /* erreur abonnement */
		XZST_03EcritureTrace(XZSTC_WARNING,
	      		"WARNING :erreur abonnement au DG %s", pl_NomDatagroup);
	     vl_ValRet = XDC_NOK;
         }
      
   /*A construction du message de demande XDM_TSE_QACT */
   pl_MsgQuestion = TipcMsgCreate(XDM_IdentMsg(XDM_TSE_QACT));

   if (!TipcMsgWrite(pl_MsgQuestion,
		           T_IPC_FT_STR, SEP_NOM_MACHINE,
		           T_IPC_FT_STR, SEP_NOM_TACHE,
		           NULL))
   {
      /* erreur creation message de demande */
      XZST_03EcritureTrace(XZSTC_WARNING,
	  "seta_synchroactif : erreur creation message de demande ");
   }
        

	/*A Envoi de la question et attente de reponse */
    	vl_ValRet = seta_questrepons(pl_MsgQuestion,
				    pl_NomDatagroup,
				    &pl_MsgReponse,
				    XDM_IdentMsg(XDM_TSE_RACT),
				    SEC_TEMPO_ACTIF);

   if (vl_ValRet == XDC_OK)
   {
      if (pl_MsgReponse != NULL)
      {

         /*A recuperation du nom et de l'etat de la machine de secours dans 
   	    le message de reponse */
         TipcMsgRead(pl_MsgReponse,
                     T_IPC_FT_STR, &pl_NomMachineSec,
		     T_IPC_FT_INT4, &vl_EtatMachineSec,
		     NULL);

      XZST_03EcritureTrace(XZSTC_INFO,
	  "Reception Msg RACT -- Nom Mach Sec %s __ Etat Mach Sec %d \n",
	  pl_NomMachineSec,vl_EtatMachineSec);

         strcpy(SEP_MACHINE_SECOURS, pl_NomMachineSec);

         switch(vl_EtatMachineSec)
         {
            case XZSEC_ETAT_NACTIF:
	       /* la machine de secours est la machine active */
	       XZSE_27ActualiserNomMachineActive(pl_NomMachineSec);
	       XZSE_25ActualiserEtatMachine(XZSEC_ETAT_NPASSIF);
	       break;


            case XZSEC_SANS_ETAT:

		/*B Recuperation de la priorite de la �achine */
		XZSC_02Priorite(&vl_Priorite);
	       /* l'etat de la machine est defini selon sa priorite */
	       if (vl_Priorite != 0)
	       {
	          /* la machine locale est la machine active */
	          XZSE_27ActualiserNomMachineActive(SEP_NOM_MACHINE);
	          XZSE_25ActualiserEtatMachine(XZSEC_ETAT_NACTIF);
               }
	       else
	       {
	          /* la machine de secours est la machine active */
	          XZSE_27ActualiserNomMachineActive(pl_NomMachineSec);
	          XZSE_25ActualiserEtatMachine(XZSEC_ETAT_NPASSIF);
               }

	       	/*B Envoie du Msg XDM_TSE_RACT **/
		seta_envoie_ract( SEP_MACHINE_SECOURS, SEP_NOM_TACHE);

	       break;


	    /* cet etat ne doit normalement pas se produire */
            case XZSEC_ETAT_NPASSIF: 
	       /* la machine locale est la machine active */
	       XZSE_27ActualiserNomMachineActive(SEP_NOM_MACHINE);
	       XZSE_25ActualiserEtatMachine(XZSEC_ETAT_NACTIF);
	       break;
         }

      }
      /* si message non recu */
      else
      {
	 /* la machine locale est consideree comme serveur actif par defaut */
	 XZSE_27ActualiserNomMachineActive(SEP_NOM_MACHINE);
	 XZSE_25ActualiserEtatMachine(XZSEC_ETAT_NACTIF);
      }
   } 

      	XZST_03EcritureTrace(XZSTC_FONCTION ,
		" seta_synchroactif  : OUT   -- Code Retour %d\n", vl_ValRet);

#endif
   return(vl_ValRet);
   
}	/* Fin seta_synchroactif */





/*X*/
/* Fichier : $Id: seta_dir.c,v 1.25 2020/11/03 17:53:43 pc2dpdy Exp $	Release : $Revision: 1.25 $        Date : $Date: 2020/11/03 17:53:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SEDIRIGER
******************************************************
* DESCRIPTION DE LA FONCTION :
*	Envoie de l'etat de la machine en tant que 
*	serveur actif
*
******************************************************
* SEQUENCE D'APPEL :
*/

int seta_servactif()

/*
* PARAMETRES EN ENTREE :
*	Aucun
*
* PARAMETRES EN SORTIE :
*	Aucun
B
*
* VALEUR(S) DE RETOUR : 
*	cwXDC_OK	: ok
*	XDC_NOK	: 	Envoi de l'etat non effectuee
*
* CONDITION D'ERREUR :
*
* 	Aucune
*
* CONDITIONS LIMITES :
*
* 	Aucune
*
******************************************************/
{
   static char *version="$Id: seta_dir.c,v 1.25 2020/11/03 17:53:43 pc2dpdy Exp $ : seta_servactif " ;
   int vl_ValRet = XDC_NOK; /* valeur de retour de la fonction */

      	XZST_03EcritureTrace(XZSTC_FONCTION ,
		"seta_servactif   : IN \n");


   /* Envoi de l'etat de la machine */
   if (XZSE_13EnvoieEtatMachine(SEP_NOM_MACHINE, XZSEC_ETAT_NACTIF) != XDC_OK)
   {
      /* erreur envoi etat machine */
      vl_ValRet = XDC_NOK;
   }

      	XZST_03EcritureTrace(XZSTC_FONCTION ,
		"seta_servactif   : OUT  -- Code Retour %d \n", vl_ValRet);

	return (vl_ValRet);

}	/* Fin seta_servactif */


/*X*/
/* Fichier : $Id: seta_dir.c,v 1.25 2020/11/03 17:53:43 pc2dpdy Exp $	Release : $Revision: 1.25 $        Date : $Date: 2020/11/03 17:53:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SEDIRIGER
******************************************************
* DESCRIPTION DE LA FONCTION :
*	Peremet d'envoyer l'etat de la machine en tant que
*	serveur passif
*
******************************************************
* SEQUENCE D'APPEL :
*/

int seta_servpassif()

/*
* PARAMETRES EN ENTREE :
*	Aucun
*
* PARAMETRES EN SORTIE :
*	Aucun
*
* VALEUR(S) DE RETOUR : 
*	Aucune
*
* CONDITION D'ERREUR :
*
* 	Aucune
*
* CONDITIONS LIMITES :
*
* 	Aucune
*
******************************************************/
{
   static char *version="$Id: seta_dir.c,v 1.25 2020/11/03 17:53:43 pc2dpdy Exp $ : seta_servpassif " ;
   int vl_ValRet = XDC_NOK; /* valeur de retour de la fonction */
   XZSCT_Datagroup pl_NomDatagroup;
   XZSCT_Datagroup pl_NomDatagroupLocal;

      	XZST_03EcritureTrace(XZSTC_FONCTION ,
		"seta_servpassif   : IN \n");

   /* initialisation de la surveillance des datagroups de basculement */
   /*seta_surv_basc();*/

   /* Envoi de l'etat de la machine */
   if (XZSE_13EnvoieEtatMachine(SEP_NOM_MACHINE, XZSEC_ETAT_NPASSIF) != XDC_OK)
   {
      /* erreur envoi etat machine */
      vl_ValRet = XDC_NOK;
   }
#if 0
   /* envoi du message de diffusion des etats */
   if (vl_ValRet == XDC_OK)
   {
      /* Construction du nom du datagroup */
      XDG_EncodeDG2(pl_NomDatagroup, SEP_MACHINE_SECOURS, SEP_NOM_TACHE);
      
      /* construction de la partie utile du message */
      XDG_EncodeDG2(pl_NomDatagroupLocal, SEP_NOM_MACHINE, SEP_NOM_TACHE);

      /* Envoi du message de demande */
      TipcSrvMsgWrite(pl_NomDatagroup,
                      XDM_IdentMsg(XDM_TSE_DIFE),
                      XDM_FLG_SRVMSGWRITE,
                      T_IPC_FT_STR, pl_NomDatagroupLocal,
                      NULL);

      if (TipcSrvFlush() != TRUE)
      {
         vl_ValRet = XDC_NOK;
      }
         
   }
#endif
      	XZST_03EcritureTrace(XZSTC_FONCTION ,
		"seta_servpassif   : OUT  -- Code Retour %d \n", vl_ValRet);

	return(vl_ValRet);

}	/* Fin seta_servpassif */



/*X*/
/* Fichier : $Id: seta_dir.c,v 1.25 2020/11/03 17:53:43 pc2dpdy Exp $	Release : $Revision: 1.25 $        Date : $Date: 2020/11/03 17:53:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SEDIRIGER
******************************************************
* DESCRIPTION DE LA FONCTION :
*	Permet de terminer proprement la tache
*	Annulation timer, fermeture trace	
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_sortir()

/*
* PARAMETRES EN ENTREE :
*	Aucun
*
* PARAMETRES EN SORTIE :
*	Aucun
*
* VALEUR(S) DE RETOUR : 
*	Aucune
*
* CONDITION D'ERREUR :
*
* 	Aucune
*
* CONDITIONS LIMITES :
*
* 	Aucune
*
******************************************************/
{
   static char *version="$Id: seta_dir.c,v 1.25 2020/11/03 17:53:43 pc2dpdy Exp $ : seta_sortir " ;
   XZSCT_Datagroup      pl_NomDatagroup	= "";
   XZSCT_NomMachine	vl_NomSDActif	= "";

      	XZST_03EcritureTrace(XZSTC_FONCTION ,
		"seta_sortir   : IN \n");

   /* arret de la surveillance des taches */
   seta_arret_surv(); 

   /* desarmemement des tests reseau */
   
   if (XZSM_02AnnulerProgrammation( SEP_NOM_MACHINE,
			NULL, XDM_TSE_VIE  ) != XDC_OK)
   {
	XZST_03EcritureTrace(XZSTC_WARNING ,
			"Erreur annulation Envoi msg de vie \n");
   }

   if (XZSM_02AnnulerProgrammation( "XDM_TSE_SURV",
			NULL, XDM_TSE_SURV  ) != XDC_OK)
   {
	XZST_03EcritureTrace(XZSTC_WARNING ,
			"Erreur annulation tests reseau \n");
   }
  

   /* liberation des ressources d'administration */
   if (XZSA_21LibereRessourcesAdmin() != XDC_OK)
   {
      /* erreur liberation ressources d'administration */

   }

   /* liberation de la memoire allouee */
   if (SEP_ETAT_MACH != NULL)
   {
   	free(SEP_ETAT_MACH);
   }
   if (SEP_ETAT_MACH != NULL)
   {
   	free(SEP_ETAT_SSYS);
   }
   if (SEP_ETAT_MACH != NULL)
   {
   	free(SEP_ETAT_TACH);
   }
   if (SEP_ETAT_MACH != NULL)
   {
   	free(SEP_ETAT_ENTI);
   }

	/* Envoi de l'etat de tsetat */
	if (( SEV_TYPE_MACHINE == XZSCC_TYPEM_SC) || ( SEV_TYPE_MACHINE == XZSCC_TYPEM_PCS) ||
	    ( SEV_TYPE_MACHINE == XZSCC_TYPEM_PC2) || ( SEV_TYPE_MACHINE == XZSCC_TYPEM_PC2LT)) /* Ajout dy type PCS 1.18 */ /* Ajout dy type PC2 1.19*/
	{
		/*B Recuperation Nom SD actif */
		XZSE_19NomSDActif(vl_NomSDActif);

		/*B Datagroup NSUPETAT_SDActif */
		XDG_EncodeDG2(pl_NomDatagroup,XDG_NSUPETAT, vl_NomSDActif);

		/*B Envoie etat de la tache NOK au SD */
		TipcSrvMsgWrite(pl_NomDatagroup,
				XDM_IdentMsg(XDM_TSE_TACH),
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_STR, SEP_NOM_MACHINE,
				T_IPC_FT_STR, SEP_NOM_TACHE,
				T_IPC_FT_INT4, XZSEC_ETAT_NOK,
				NULL);
	}

   /* retour au processus appelant */
   XZST_03EcritureTrace(XZSTC_INFO,"seta_sortir : Sortie demandee");

	/* On detruit la connexion completement */
	if (!TipcSrvDestroy(T_IPC_SRV_CONN_NONE))
	{
		XZST_03EcritureTrace(   XZSTC_WARNING,
			"Erreur deconnexion serveur \n");
	}

	
   XZST_03EcritureTrace(XZSTC_WARNING,"seta_sortir : Sortie demandee");

   /* fermeture du fichier de trace */
   XZST_02FermetureTrace();

   exit(0);

}	/* Fin seta_sortir */


#ifndef _TIBCO_EMS


/*X*/
/* Fichier : $Id: seta_dir.c,v 1.25 2020/11/03 17:53:43 pc2dpdy Exp $	Release : $Revision: 1.25 $        Date : $Date: 2020/11/03 17:53:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SEDIRIGER
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction gere un echange du type question
* reponse entre la tache locale et une autre tache.
* La question et la reponse sont materialisees par des
* messages RTWorks.
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

int seta_questrepons(T_IPC_MSG pa_MsgQuestion,
	               XZSCT_Datagroup pa_NomDatagroup,
		       T_IPC_MSG *pa_MsgReponse,
	               T_IPC_MT pa_TypeMsgReponse,
	               int va_DureeAttente)

/*
* PARAMETRES EN ENTREE :
* va_MsgQuestion    : message de question
* pa_NomDatagroup   : datagroup destinataire de la question
* va_TypeMsgReponse : type du message de reponse
* va_DureeAttente   : duree d'attente de la reponse
*
*
* PARAMETRES EN SORTIE :
* pa_MsgReponse : adresse du message de reponse, NULL
*                 si message non  recu
*
*
* VALEUR(S) DE RETOUR : code d'erreur 
*                       
*
* CONDITION D'ERREUR :
* XDC_OK  : execution OK (meme si reponse non recue)
* XDC_NOK : erreur RTWorks
*
*
* CONDITIONS LIMITES :
*
* 	Aucune
*
******************************************************/
{
   	static char *version="$Id: seta_dir.c,v 1.25 2020/11/03 17:53:43 pc2dpdy Exp $ : seta_questrepons " ;
   	int 		vl_ValRet 	= XDC_OK;
   	T_IPC_MSG 	vl_MsgReponse	= NULL;
	int		vl_Priorite	= 0;

	
      	XZST_03EcritureTrace(XZSTC_FONCTION ,
		"seta_questrepons   : IN  \
		-- Datagroup %s --  Type reponse %d \n", 
		pa_NomDatagroup, pa_TypeMsgReponse);

	/*A Verification  qu'un Msg du type XDM_TSE_QACT a ete recu, si l'on
	*	attendait un Msg du type RACT et que l'on ne l'a pas recu */
	if  (pa_TypeMsgReponse == XDM_IdentMsg(XDM_TSE_RACT)) 
	{

		XZST_03EcritureTrace(XZSTC_DEBUG1,"Verification qu'un \
			Msg du type QACT a ete recu \n");

			
		/*B Verification qu'un message XDM_TSE_QACT a ete recu */
		vl_MsgReponse = TipcSrvMsgSearchType
				(va_DureeAttente,XDM_IdentMsg(XDM_TSE_QACT));
		if  (vl_MsgReponse !=  NULL)
		{
			XZST_03EcritureTrace(XZSTC_DEBUG1,"Reception d'un Msg\
			XDM_TSE_QACT avnt l'envoi de a propre demande \n");

			/*B On declenche le traitement associe a la reception
			*	de ce message */
			TipcSrvMsgProcess(vl_MsgReponse);
			TipcMsgDestroy(vl_MsgReponse);
		}
	}

   /* affectation du destinataire de la question */
   TipcMsgSetDest(pa_MsgQuestion, pa_NomDatagroup);

   /* Modification de la priorite du message */
   TipcMsgSetPriority (pa_MsgQuestion, (T_INT2)vl_Priorite);


   /* envoi de la question */
   TipcSrvMsgSend(pa_MsgQuestion, XDM_FLG_SRVMSGWRITE);

   if (!TipcSrvFlush())
   {
      vl_ValRet = XDC_NOK;
      /* erreur d'envoi du message */
      XZST_03EcritureTrace(XZSTC_WARNING,
 		           "seta_questrepons: erreur envoi question  ");
   }

		XZST_03EcritureTrace(XZSTC_DEBUG1,"Attente du Msg RACT \n");

   /* Attente de la reponse */
   if (vl_ValRet == XDC_OK) 
   {
       /* Attente bloquante d'un message */
       *pa_MsgReponse= TipcSrvMsgSearchType(va_DureeAttente,pa_TypeMsgReponse);
   }

      	XZST_03EcritureTrace(XZSTC_FONCTION ,
		"seta_questrepons  : OUT -- Code retour %d \n",vl_ValRet);

   return(vl_ValRet);

}	/* Fin seta_questrepons */

#endif




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet d'envoyer le msg XDM_TSP_MIGRAZUR indiquant 
*	que la tache tsetat.x est lancee 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int seta_connexion(
	XZSCT_NomMachine 	pa_NomMachine,
     	int 			va_NumOrdre,
	void                    *pa_Args)

/*
* ARGUMENTS EN ENTREE :
*   pa_NomTache : Nom de la Tache
*   va_NumOrdre : Numero d'ordre
*   pa_NomMachine : Nom de la Machine
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
*   Envoie le msg XDM_TSP_MIGRAZUR sur le datagroup
*	Nom de la Machine
*
------------------------------------------------------*/
{
	static char *version="$Id: seta_dir.c,v 1.25 2020/11/03 17:53:43 pc2dpdy Exp $ : seta_connexion " ;
	XZSCT_Datagroup	pl_NomDatagroup	= "";
	T_IPC_MSG 	vl_Msg;
	int		vl_Priorite	= 100;
	int		vl_cr	= XDC_OK;


	XZST_03EcritureTrace(XZSTC_FONCTION,"seta_connexion : IN \
		-- Nom Machine %s \n",pa_NomMachine);

	/*A Envoie du msg XDM_TSP_MIGRAZUR sur le datagroup NomMachine */
	strcpy(pl_NomDatagroup,pa_NomMachine);

	XZST_03EcritureTrace(XZSTC_MESSAGE,"Envoie Msg XDM_TSP_MIGRAZUR \
			-- datagp %s  -- nom mach %s \n",pl_NomDatagroup,
			pa_NomMachine);
	/* creation et envoi du message de vie de la machine locale */
      if (!TipcSrvMsgWrite (pl_NomDatagroup,
                            XDM_IdentMsg (XDM_TSP_MIGRAZUR),
                            XDM_FLG_SRVMSGWRITE,
                            T_IPC_FT_STR, SEP_NOM_SITE,
                            NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "Pb creation msg %d \n",XDM_TSP_MIGRAZUR);
		vl_cr = XDC_NOK;
	}


	XZST_03EcritureTrace(XZSTC_FONCTION,"seta_connexion : OUT \n");

	return (vl_cr);
}	/* Fin seta_connexion */
