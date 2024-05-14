/*E*/
/*  Fichier : $Id: seta_sur.c,v 1.38 2020/11/03 17:54:03 pc2dpdy Exp $        Release : $Revision: 1.38 $        Date : $Date: 2020/11/03 17:54:03 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_SUR * FICHIER seta_sur.c
******************************************************
* DESCRIPTION DU MODULE :
*
* Module qui permet de specifier les etats de surveillance des machines
*	de traiter celles-ci suivant le type de machine et de diffuser 
*	les etats aux machines concernees.
*
* seta_msgvie_cb	: Callback qui un message de vie emis par une
* 	machine surveillee par la machine locale.
* seta_survie_cb	: Callback qui  surveille l'emission des messages de vie
* 	des machines surveillees par la machine locale.
* seta_survie		:  Cette fonction surveille la reception du message de vie
* 	emis par une machine. 
* seta_synchromachine_cb	: Callback qui  repond a la demande de presence de
* 	serveur actif.
* seta_arret_surv		: Cette fonction arrete la surveillance des machines
* 	par la machine locale .
* seta_dgstatus_cb		: Cette fonction traite les comptes-rendus de surveillance.
* seta_dgstatus_st		: Cette fonction traite un compte-rendu de surveillance
* 	dans le cas ou la machine locale est une station.
* seta_dgstatus_sc		: Cette fonction traite un compte-rendu de surveillance
* 	dans le cas ou la machine locale est un serveur de
* 	communication.
* seta_dgstatus_sd		:  Cette fonction traite un debut ou une fin d'absence
* 	d'une machine surveillee par la machine locale dans
* 	le cas ou cette derniere est un serveur de donnees
* seta_arret_testres		: Cette fonction stoppe le test reseau d'une machine.
* seta_arme_testres		:  Cette fonction arme le test reseau d'une machine.
* seta_arme_surv		: Cette fonction arme la surveillance d'une machine
* seta_desarme_surv		: Cette fonction desarme la surveillance d'une machine
* seta_trait_mach_superv	: Cette fonction realise un traitement sur toutes les
* 	machines surveillees par la machine locale.
* seta_trait_mach_surv		: Cette fonction realise un traitement sur toutes les
* 	machines surveillees par la machine locale.
* seta_trie_mach		:  Permet , suivant le type de la machine de memoriser le type
*       de surveillance associee
* seta_alarm_serv_peri		:  Generation des alarmes systemes signalant les pertes
* 	et reprises de communication avec les serveurs de
* 	peripheriques
* seta_init_site		:  Reinitialise a SANS_ETAT l'etat de toutes les machines
*       d'un site
* seta_diff_init		: Diffuse l'etat de la machine au synoptique
*	d'administration
*
******************************************************
* HISTORIQUE :
*
* T.Milleville 24 Oct 1994 	: Creation
* T.Milleville 7 Fev 1995 	: Modification Test pour 
*				abonnement a NSUPETAT V1.3
* T.Milleville 08 MAr 1995 	: Modification XZGA_xx en XZSI_xx V1.5
*
* T.Milleville 08 MAr 1995 	: Modification de la fonction XZSI_06 dans
*		laquelle on specifie le destinataire V1.6
* T.Milleville 31 MAr 1995 	: Modification du test reseau pour 
*	un SC ou SD. On effectue le test tant que la connexion 
*	n'est pas revenu V1.7
* T.Milleville 25 Jul 1995 	: Ajout de commentaires V1.9
* T.Milleville 26 Jul 1995 	: Mise a jour entete V1.10
* T.Milleville 6 Aou 1995 	: Ajout envoie du msg de reinitialisation dezs
*	synoptiques si reconnexiuon entre 2 sites  V1.11
* T.Milleville 8 Aou 1995 	: Ajout de traces V1.12
* T.Milleville 9 Oct 1995 	: Ajout de traces V1.15
* T.Milleville 24 Oct 1995 	: Ajout de traces V1.15
* T.Milleville 26 Oct 1995 	: Remodification pour prendre en compte
*	les deconnexions entre RTserves et donc abonnement a NSUPETAT V1.16
* T.Milleville 30 Oct 1995 	: Modification seta_trie_mach pour tenir compte
*	du serveur PASSIF V1.16
* T.Milleville 06 Nov 1995 	: Modification lors de l'initialisation pour connaitre
*	les machines surveilles. Un SC peut surveiller un autre SC V1.16
* T.Milleville 09  Nov 1995 	: Ajout a la modif 1.16. Si le SC n'esp pas sur le 
*	 meme site, alors il est surveille mais pas supervise V1.17
* T.Milleville 16  Nov 1995 	: Dans la fonction dg_status_sc, ajout d'un test
*	pour declencher le test reseau si un SD V1.17
* T.Milleville 12 Dec 1995 	: Ajout test pour surveillance inter-SC V1.18
  P.NIEPCERON  19 Dec 1995	: Correct du dg de demande de reinit des eqt sur syn V1.18
* P.NIEPCERON  15 Jan 1995	: Ajout de la demande des etat d'une machine au retour � la vie de cette machine v1.19
*	"			: rien v1.20
* P.NIEPCERON  31 Jan 1995	: Restriction a la modif v1.19 pour les machine active et si la machine demandeuse n'est pas SDCI1 v1.21
* P.NIEPCERON  16 Fev 1996	: Memorisation des nom des serveurs pour le SD Passif v1.23
* P.NIEPCERON  16 Fev 1996      : Modif pour que la remontee de SDCI2 ne soit pas comprise comme une reprise de comm 1.24
* P.NIEPCERON  20 Fev 1996      : Demande etat d'une machine en veille ou sans etat a la reception du msg de vie v1.25
****
* P NIEPCERON  22 Aout1996	: Ajout du type de machine PC simplifie	v1.26
*
* P NIEPCERON  17 Sep 1996	: Ajout de reinit etat SD apres reprise de comm a partir d'une machine PC simplifie	v1.27
* P.NIEPCERON  12 Nov 1996	: Non prise en compte du serveur esclave lors de la perte du CI v1.28
* P.NIEPCERON  29 Nov 1996	: Ajout de la surveillance active pour les PCS (dem/1295) v1.29
* P.NIEPCERON   4 dev 1997	: Ajout du raf des syn sup + bt etat site v1.30
* P.NIEPCERON  16 Dec 1997	: Ajout PC2 +STA v1.31
* P.NIEPCERON   6 Jan 1998	:  simplification des raf  des syn sup + bt etat site (SC,PC2*,PCS) 1.32
* P.Niepceron   01 Jul 1998     : Ajout du type de machine Historique (dem/1697)+ raf supplementaire du bt etat site v1.33
* P.Niepceron   12 Fev 1999	: Suppression de la surveillance active entre PCS et SC ou PLC2 v1.34
* GUILHOU			: modif tolerance on met 3 au lieu de 1
* JPL		11/01/11	: Migration architecture HP ia64 (DEM 971) : declaration fonctions et types, retour CR.  1.36
* JPL		08/02/18	: Migration Linux 64 bits (DEM 1274) : parenthesage des expressions  1.37
*******************************************************/

/* fichiers inclus */

#include <rtworks/ipc.h>

#include "seta_sur.h"
#include "seta_arc.h"
#include "seta_don.h"
#include "seta_tra.h"

#include "xdc.h"
#include "xdm.h"
#include "xdg.h"

#include "xzsi.h"
#include "xzsa.h"
#include "xzsc.h"
#include "xzse.h"
#include "xzst.h"
#include "xzsg.h"

#include "xdl.h"

/* definitions de constantes */

#define TOLERANCE 3 /* tolerance pour reception des messages de vie */

/* definitions de types locaux */

/* declaration de variables locales */

/* numero des serveurs (SD ou SC) de la machine locale */ 
int vm_NumServeur1 = -1;
int vm_NumServeur2 = -1;

/* definition de macro locales */
#define MachineSupervisee(NumMachine) (NumMachine != -1)

/*  Modif V1.18   Prise en compte Surveillance inter_SC */
#define MachineSurveillee(NumMachine) ((NumMachine != -1) && (SEP_ETAT_MACH[NumMachine].TypeSurveillance != SEC_SURV_PASSIVE))

#define MachinePresente(NumMachine) (SEP_ETAT_MACH[NumMachine].Presence == SET_PRESENCE)
#define NbServeursPresents ((MachinePresente(vm_NumServeur1)?1:0) + (MachinePresente(vm_NumServeur2)?1:0))

static char *version = "$Id: seta_sur.c,v 1.38 2020/11/03 17:54:03 pc2dpdy Exp $ : seta_sur" ;

/* declaration de fonctions internes */
void seta_dgstatus_st(XZSCT_NomMachine pa_NomMachine, int va_EtatAbsence);
void seta_dgstatus_sc(XZSCT_NomMachine pa_NomMachine, int va_EtatAbsence);
void seta_dgstatus_sd(XZSCT_NomMachine pa_NomMachine, int va_EtatAbsence);

int seta_arme_testres(XZSCT_NomMachine pa_NomMachine);
int seta_arret_testres(XZSCT_NomMachine pa_NomMachine);
int seta_trie_mach (
	XZSCT_NomMachine pa_NomMachine, int va_NumOrdre, void *pa_Args);
void seta_survie(XZSCT_NomMachine pa_NomMachine, int va_NumMachine);
void seta_alarm_serv_peri(XZSCT_NomMachine pa_NomMachine, int va_Absence);
void seta_init_site(
	XZSCT_NomSite pa_NomSite,
	int va_NumOrdre,
	void *pa_Args);
int seta_diff_init(
      XZSCT_NomMachine pa_NomMachine,
      int              va_NumOrdre,
      int              va_EtatMachine);


/* definition de fonctions externes */



/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_SUR
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction traite un message de vie emis par une
* machine surveillee par la machine locale.
* Call-back RTWORKS associe au message XDM_TSE_VIE   
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_msgvie_cb (
	T_IPC_CONN va_Conn,
	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	T_CB_ARG pa_Arg)

/*
* PARAMETRES EN ENTREE : arguments specifiques a un call-back
*                        RT-Works
*
*
* PARAMETRES EN SORTIE : -     
*	Aucun
*
* VALEUR(S) DE RETOUR : -    
*	Aucun
* CONDITION D'ERREUR : -
*	Aucune
*
* CONDITIONS LIMITES : - 
*	Acune
*
******************************************************/
{
   static char *version = "$Id: seta_sur.c,v 1.38 2020/11/03 17:54:03 pc2dpdy Exp $ : seta_msgvie_cb" ;
#ifndef _TIBCO_EMS
   T_STR vl_NomMachine	= ""; /* nom de la machine */ 
#else
   XDY_NomMachine vl_NomMachine	= ""; /* nom de la machine */ 
#endif
   int vl_NumMachine	= 0; /* numero de la machine parmi les machines surveillees */
   int                  vl_EtatMachine  = 0;
   XZSCT_Datagroup vl_NomDatagroup, vl_NomDatagroupSurv	= "";


	XZST_03EcritureTrace(XZSTC_FONCTION,
		"seta_msgvie_cb : IN \n");

   /* Recuperation du nom de la machine */
   TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_STR, &vl_NomMachine,
	       NULL); 

	XZST_03EcritureTrace(XZSTC_MESSAGE,
			"Reception msg vie machine %s \n",vl_NomMachine);


   /* recherche de la machine parmi les machine surveillees */
   seta_rechmach(SEP_ETAT_MACH, SEV_NB_MACH, vl_NomMachine, &vl_NumMachine);

   /* test si machine non surveillee */
   if (!MachineSurveillee (vl_NumMachine))
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "seta_msgvie => \
	RECEPTION MESSAGE DE VIE D'UNE MACHINE NON SURVEILLEE = Machine: %s \n",
                           vl_NomMachine);
   }
   else
   {
      /* si machine en defaut */
      if (SEP_ETAT_MACH[vl_NumMachine].HeureVie == -1)
      {
	 /* on memorise l'heure de reception du message de vie */
	 SEP_ETAT_MACH[vl_NumMachine].HeureVie = time(NULL);

	 /* remise a zero du nombre d'absences du message de vie */
         SEP_ETAT_MACH[vl_NumMachine].NbAbsenceVie = 0;
	 SEP_ETAT_MACH[vl_NumMachine].EtatAbsence = SET_PRESENCE;

         XZST_03EcritureTrace(XZSTC_INFO, "seta_msgvie => \
		PRESENCE MACHINE = Machine: %s", vl_NomMachine);

         XZST_03EcritureTrace(XZSTC_DEBUG3, "seta_msgvie => \
		PRESENCE MACHINE = Machine: %s", vl_NomMachine);

	 /* traitement de la presence machine */
         seta_dgstatus_cb(SEP_ETAT_MACH[vl_NumMachine].NomMachine, 
		SEP_ETAT_MACH[vl_NumMachine].EtatAbsence);
		
		


	/* Modif 1.22: Si on est un SD ou un SC ou un PCS (Modif 1.26) ou un PC2 (Modif 1.31) et machine revue n'est pas un SC ou PCS et ... */
	/* Modif 1.21: Si ce n'est pas le CI (SDCI1) que l'on  revoit et qu'on n'est actif alors on demande les etats de la machine revue*/
	XZSE_09EtatMachine(&vl_EtatMachine);
	
	if (( SEP_ETAT_MACH[vl_NumMachine].TypeMachine != XZSCC_TYPEM_SD) &&
		( vl_EtatMachine == XZSEC_ETAT_NACTIF) &&
		( ( SEV_TYPE_MACHINE == XZSCC_TYPEM_SD ) || 
		  ( ( SEV_TYPE_MACHINE == XZSCC_TYPEM_SC ) && ( SEP_ETAT_MACH[vl_NumMachine].TypeMachine != XZSCC_TYPEM_SC) && ( SEP_ETAT_MACH[vl_NumMachine].TypeMachine != XZSCC_TYPEM_PCS)  && 
								( SEP_ETAT_MACH[vl_NumMachine].TypeMachine != XZSCC_TYPEM_PC2)   && ( SEP_ETAT_MACH[vl_NumMachine].TypeMachine != XZSCC_TYPEM_PC2LT)   )  || 
                  ( ( SEV_TYPE_MACHINE == XZSCC_TYPEM_PC2 ) && ( SEP_ETAT_MACH[vl_NumMachine].TypeMachine != XZSCC_TYPEM_SC) &&  
								( SEP_ETAT_MACH[vl_NumMachine].TypeMachine != XZSCC_TYPEM_PC2)   && ( SEP_ETAT_MACH[vl_NumMachine].TypeMachine != XZSCC_TYPEM_PC2LT)   )  || 
		  ( ( SEV_TYPE_MACHINE == XZSCC_TYPEM_PC2LT ) && ( SEP_ETAT_MACH[vl_NumMachine].TypeMachine != XZSCC_TYPEM_SC)  && 
								( SEP_ETAT_MACH[vl_NumMachine].TypeMachine != XZSCC_TYPEM_PC2)   && ( SEP_ETAT_MACH[vl_NumMachine].TypeMachine != XZSCC_TYPEM_PC2LT)   )  )  
		)
         /* Demande l'etat de la machine qui est de retour! modif v1.19*/
         XZSE_30DemanderEtatMachineAppli(SEP_ETAT_MACH[vl_NumMachine].NomMachine);


#if 0
	 /* signalement eventuel retour de communication 
	 *			avec les serveurs d'impression */
	 seta_alarm_serv_peri(SEP_ETAT_MACH[vl_NumMachine].NomMachine, 
				SEP_ETAT_MACH[vl_NumMachine].EtatAbsence);

#endif
      }
      else
      {
	XZST_03EcritureTrace(XZSTC_DEBUG1,
			"seta_msgvie_cb : Memorisation Heure de Vie -- \
			Nom mach %s \n",vl_NomMachine);
	 SEP_ETAT_MACH[vl_NumMachine].HeureVie = time(NULL);
	 SEP_ETAT_MACH[vl_NumMachine].EtatAbsence = SET_PRESENCE;

	 if ( ( SEP_ETAT_MACH[vl_NumMachine].Etat == XZSEC_ETAT_VEILLE ) || 
	    ( SEP_ETAT_MACH[vl_NumMachine].Etat == XZSEC_SANS_ETAT ) )
	   {
	      XZSE_09EtatMachine(&vl_EtatMachine);

	      if ( ( SEP_ETAT_MACH[vl_NumMachine].TypeMachine != XZSCC_TYPEM_SD) &&
	           ( vl_EtatMachine == XZSEC_ETAT_NACTIF) &&
	           ( ( SEV_TYPE_MACHINE == XZSCC_TYPEM_SD ) || 
	           ( ( SEV_TYPE_MACHINE == XZSCC_TYPEM_SC ) && ( SEP_ETAT_MACH[vl_NumMachine].TypeMachine != XZSCC_TYPEM_SC) && 
								 ( SEP_ETAT_MACH[vl_NumMachine].TypeMachine != XZSCC_TYPEM_PC2)   && ( SEP_ETAT_MACH[vl_NumMachine].TypeMachine != XZSCC_TYPEM_PC2LT)) ||
                  ( ( SEV_TYPE_MACHINE == XZSCC_TYPEM_PC2 ) && ( SEP_ETAT_MACH[vl_NumMachine].TypeMachine != XZSCC_TYPEM_SC)   && 
								( SEP_ETAT_MACH[vl_NumMachine].TypeMachine != XZSCC_TYPEM_PC2)   && ( SEP_ETAT_MACH[vl_NumMachine].TypeMachine != XZSCC_TYPEM_PC2LT)   )  ||
		  ( ( SEV_TYPE_MACHINE == XZSCC_TYPEM_PC2LT ) && ( SEP_ETAT_MACH[vl_NumMachine].TypeMachine != XZSCC_TYPEM_SC)   && 
								( SEP_ETAT_MACH[vl_NumMachine].TypeMachine != XZSCC_TYPEM_PC2)   && ( SEP_ETAT_MACH[vl_NumMachine].TypeMachine != XZSCC_TYPEM_PC2LT)   )  )  
	           )
         	/*  Si la machine est sur le reseau mais en veille on luis demande son etat :modif v1.25*/
         	/*  ajout test PCS pour modif 1.26 */  /*  ajout test PC2 pour modif 1.31 */
	  	XZSE_30DemanderEtatMachineAppli(SEP_ETAT_MACH[vl_NumMachine].NomMachine);
	   }



      }
   }

	XZST_03EcritureTrace(XZSTC_FONCTION,
		"seta_msgvie_cb : OUT  \n");

}	/* Fin seta_msgvie_cb */


/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_SUR
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction surveille l'emission des messages de vie
* des machines surveillees par la machine locale.         
* Call-back RTWORKS associe au message XDM_TSE_SURV
* (message envoye par le timer).
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_survie_cb(
	T_IPC_CONN va_Conn,
	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	T_CB_ARG pa_Arg)

/*
* PARAMETRES EN ENTREE : arguments specifiques a un call-back
*                        RT-Works
*
*
* PARAMETRES EN SORTIE : 
*	Aucun
*
* VALEUR(S) DE RETOUR :
*	Aucun
*
* CONDITION D'ERREUR :
*	Aucune
*
* CONDITIONS LIMITES :
*	Aucune
*
******************************************************/
{
   static char *version = "$Id: seta_sur.c,v 1.38 2020/11/03 17:54:03 pc2dpdy Exp $ : seta_survie_cb" ;
   T_IPC_MSG vl_MsgVie;
   XZSCT_Datagroup pl_NomDatagroup	= "";

	XZST_03EcritureTrace(XZSTC_FONCTION,
		"seta_survie_cb : IN   \n");

   /* surveillance du message de vie pour chaque machine */
   seta_trait_mach_surv(seta_survie);

	XZST_03EcritureTrace(XZSTC_FONCTION,
		"seta_survie_cb : OUT \n");

}	/* Fin seta_survie_cb */


/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_SUR
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction surveille la reception du message de vie
* emis par une machine. Elle detecte ainsi les debuts
* d'absence courte et d'absence longue et les fins
* d'absence.
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_survie(
	XZSCT_NomMachine pa_NomMachine,
	int va_NumMachine)

/*
* PARAMETRES EN ENTREE :
* pa_NomMachine: nom de la machine 
* va_NumMachine: numero de la machine
*
*
* PARAMETRES EN SORTIE : 
*	Aucun
*
* VALEUR(S) DE RETOUR :
*	Aucune
*
* CONDITION D'ERREUR :
*	Aucune
*
* CONDITIONS LIMITES : 
*	Aucune
*
******************************************************/
{
   static char *version = "$Id: seta_sur.c,v 1.38 2020/11/03 17:54:03 pc2dpdy Exp $ : seta_survie" ;
   T_STR 	vl_NomMachine		= ""; /* nom de la machine */ 
   long 	vl_HeureCourante	= 0;
   XZSCT_Datagroup vl_NomDatagroup, vl_NomDatagroupSurv;
	/* periode de surveillance des messages de vie */
   int 	vl_PeriodeSurv		= 0; 
   int 	vl_DureeAbsenceCourte	= 0;
   int	vl_TypeMachine		= 0;

   
	XZST_03EcritureTrace(XZSTC_FONCTION,
		"seta_survie : IN  -- Nom Mach %s -- Num mach %d -- etat presence %d  \n",
		pa_NomMachine,va_NumMachine,SEP_ETAT_MACH[va_NumMachine].EtatAbsence);
	XZST_03EcritureTrace(XZSTC_WARNING,
		"seta_survie : IN  -- Nom Mach %s -- Num mach %d -- etat presence %d  \n",
		pa_NomMachine,va_NumMachine,SEP_ETAT_MACH[va_NumMachine].EtatAbsence);

   /* la periode de surveillance des messages de vie 
   *			est egale a la periode des tests reseaux */
   XZSC_03PeriodeTestReseau(&vl_PeriodeSurv);

   /* memorisation heure actuelle */
   vl_HeureCourante = time(NULL);

   /* test si machine non en defaut au niveau du message de vie */
   if (SEP_ETAT_MACH[va_NumMachine].EtatAbsence == SET_PRESENCE)
   {
      /* si temps imparti depasse */
      if (vl_HeureCourante > 
     (SEP_ETAT_MACH[va_NumMachine].HeureVie + ((1+TOLERANCE) * (long) vl_PeriodeSurv)))
      {
         SEP_ETAT_MACH[va_NumMachine].NbAbsenceVie++;
         SEP_ETAT_MACH[va_NumMachine].HeureVie = -1;

         XZST_03EcritureTrace(XZSTC_WARNING, 
		"seta_msgvie => ABSENCE COURTE MACHINE = Machine: %s -- Abscence Vie %d", 
			pa_NomMachine,SEP_ETAT_MACH[va_NumMachine].NbAbsenceVie);

	/*B Teste sur 2 periodes de Surveillance (Ceci pour le
	*		decalage Horaire ) */
	if (SEP_ETAT_MACH[va_NumMachine].NbAbsenceVie > 1)
	{
	 SEP_ETAT_MACH[va_NumMachine].EtatAbsence = SET_ABSENCE_COURTE;

         XZST_03EcritureTrace(XZSTC_WARNING, 
		"seta_msgvie => ABSENCE COURTE MACHINE = Machine: %s", 
			pa_NomMachine);

	 /* traitement de l'absence de la machine */
         seta_dgstatus_cb(SEP_ETAT_MACH[va_NumMachine].NomMachine, 
				SEP_ETAT_MACH[va_NumMachine].EtatAbsence);

	 /* signalement eventuel retour de communication 
	 *			avec les serveurs d'impression */
	 seta_alarm_serv_peri(SEP_ETAT_MACH[va_NumMachine].NomMachine, 
		SEP_ETAT_MACH[va_NumMachine].EtatAbsence);
	}
      }
   }
   else 
   {
      	/*B recuperation de la duree absence courte */
      	XZSC_17DureeAbsenceCourte(&vl_DureeAbsenceCourte);
	/*B Teste si Abscence Longue */
      	if (SEP_ETAT_MACH[va_NumMachine].NbAbsenceVie >= vl_DureeAbsenceCourte)
      	{
		/*B Memorisation Abscence Longue */
	 	SEP_ETAT_MACH[va_NumMachine].EtatAbsence = SET_ABSENCE_LONGUE;

		/*B Initialisation Nb d'abscences */
		SEP_ETAT_MACH[va_NumMachine].NbAbsenceVie = 0;
      	}
	else
	{
      		/*B incrementation du nombre d'absences */
      		SEP_ETAT_MACH[va_NumMachine].NbAbsenceVie++;
	}

	/*B Teste si la machine esu un SD ou SC et la machine est en 
	*	abscence longue .Si oui alors on continue
	*	les tests de reseau */
	 /* recuperation du type de la machine */
	 /*XZSA_17TypeMachine(pa_NomMachine, &vl_TypeMachine);
	if( ((SEP_ETAT_MACH[va_NumMachine].TypeMachine == XZSCC_TYPEM_SD)
	    || (SEP_ETAT_MACH[va_NumMachine].TypeMachine == XZSCC_TYPEM_SC)
	    || (SEP_ETAT_MACH[va_NumMachine].TypeMachine == XZSCC_TYPEM_PC2)
	    || (SEP_ETAT_MACH[va_NumMachine].TypeMachine == XZSCC_TYPEM_PC2LT)
	    || (SEP_ETAT_MACH[va_NumMachine].TypeMachine == XZSCC_TYPEM_PCS))

	    {
         	seta_dgstatus_cb(SEP_ETAT_MACH[va_NumMachine].NomMachine, 
				SEP_ETAT_MACH[va_NumMachine].EtatAbsence);
	    }*/
   }

	XZST_03EcritureTrace(XZSTC_FONCTION,
		"seta_survie : OUT \n");

}	/* Fin seta_survie */




/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_SUR
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction repond a la demande de presence de
* serveur actif.
* Call-back RTWORKS associe au message XDM_TSE_QUES          
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_synchromachine_cb(
	T_IPC_CONN va_Conn,
	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	T_CB_ARG pa_Arg)

/*
* PARAMETRES EN ENTREE :
*	Argument d'une callback IPC
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
   static char *version = "$Id: seta_sur.c,v 1.38 2020/11/03 17:54:03 pc2dpdy Exp $ : seta_synchromachine_cb" ;
   int vl_ValRet 	= XDC_OK;
#ifndef _TIBCO_EMS
   T_STR pl_NomMachine	= "";            /* nom de la mach de la tache demandeuse */ 
   T_STR pl_NomTache	= "";              /* nom de la tache demandeuse */
#else
	XDY_NomMachine  pl_NomMachine  = ""; 
	XDY_NomTache  pl_NomTache    = "";    
#endif
   int vl_EtatMachine	= 0;             /* etat de la machine locale */
   XZSCT_NomMachine pl_NomSDActif	= "";    /* nom du serveur de donnees actif */
   XZSCT_NomMachine pl_NomSCActif	= "";    /*nom du serveur de Communication actif */
   XZSCT_Datagroup pl_NomDatagroup	= "";

   XZST_03EcritureTrace(XZSTC_FONCTION,  
			"seta_synchromachine_cb : IN \n");
		  
   /* Recuperation du nom de la machine et du nom de la tache
      demandeuse */
   TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_STR, &pl_NomMachine,
               T_IPC_FT_STR, &pl_NomTache,
	       NULL); 

   XZST_03EcritureTrace(XZSTC_MESSAGE, "Reception msg Synchro machine \
	-- Nom Mach  %s -- Nom Tache %s \n",pl_NomMachine, pl_NomTache);


   /* Recuperation de l'etat de la machine locale */
   XZSE_09EtatMachine(&vl_EtatMachine);
 
   /* Recuperation du nom du serveur de donnees actif */
   XZSE_19NomSDActif(pl_NomSDActif);

   /* Recuperation du nom du serveur de communication  actif */
   XZSE_18NomSCActif(pl_NomSCActif);

   if ( (vl_EtatMachine == XZSEC_ETAT_NACTIF) ||
	(vl_EtatMachine == XZSEC_ETAT_DEGRADE) )
   {
      /* Envoi du message XDM_TSE_MAJD au datagroup XDG_NomMachine_NomTache */
      XDG_EncodeDG2(pl_NomDatagroup, pl_NomMachine, pl_NomTache);

      if(!TipcSrvMsgWrite(pl_NomDatagroup,
                     XDM_IdentMsg(XDM_TSE_MAJD),
		     XDM_FLG_SRVMSGWRITE,
                     T_IPC_FT_STR, pl_NomSDActif,
                     NULL))
      {
	XZST_03EcritureTrace(XZSTC_WARNING,
			"Pb envoie msg XDM_TSE_MAJD \n");
         vl_ValRet = XDC_NOK;
      }

      /* Envoi du message XDM_TSE_MAJC au datagroup XDG_NomMachine_NomTache */
      if(!TipcSrvMsgWrite(pl_NomDatagroup,
                     XDM_IdentMsg(XDM_TSE_MAJC),
		     XDM_FLG_SRVMSGWRITE,
                     T_IPC_FT_STR, pl_NomSCActif,
                     NULL))
      {
	XZST_03EcritureTrace(XZSTC_WARNING,
			"Pb envoie msg XDM_TSE_MAJC \n");
         vl_ValRet = XDC_NOK;
      }
   }


   XZST_03EcritureTrace(XZSTC_FONCTION,  
			"seta_synchromachine_cb : OUT -- Code Retour %d\n",
			vl_ValRet);

}	/* Fin seta_synchromachine_cb */





/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE MODULE
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction arrete la surveillance des machines
* par la machine locale .
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_arret_surv()

/*
* PARAMETRES EN ENTREE :
* 	Aucun
*
* PARAMETRES EN SORTIE :
* 	Aucun
*
* VALEUR(S) DE RETOUR :
* 	Aucune
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
   static char *version = "$Id: seta_sur.c,v 1.38 2020/11/03 17:54:03 pc2dpdy Exp $ : seta_arret_surv" ;
   XZST_03EcritureTrace(XZSTC_FONCTION,  
			"seta_arret_surv : IN  \n");

   XZST_03EcritureTrace(XZSTC_WARNING ,  
			" Pb Pas de Code dans cette fonction \n");


   XZST_03EcritureTrace(XZSTC_FONCTION,  
			"seta_arret_surv : OUT \n");

}	/* Fin seta_arret_surv */


/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_SUR
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction traite les comptes-rendus de surveillance.
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_dgstatus_cb(
	XZSCT_NomMachine pa_NomMachine, 
	int va_EtatAbsence)

/*
* PARAMETRES EN ENTREE :
*	pa_NomMachine	: Nom de la machine
*	va_EtatAbsence	: Etat de la machine
*
*
* PARAMETRES EN SORTIE :
* 	Aucun
*
* VALEUR(S) DE RETOUR :
* 	Aucune
*
* CONDITION D'ERREUR :
* 	Aucune
*
* CONDITIONS LIMITES :
* 	Aucune
*
******************************************************/
{
   static char *version = "$Id: seta_sur.c,v 1.38 2020/11/03 17:54:03 pc2dpdy Exp $ : seta_dgstatus_cb" ;
   int vl_EtatMachine	= 0;

   XZST_03EcritureTrace(XZSTC_FONCTION,  
			"seta_dgstatus_cb : IN  \n");

   XZST_03EcritureTrace(XZSTC_DEBUG3,  
	"seta_dgstatus_cb : IN  -- Nom Machien %s -- Abscence %d  \n",
		pa_NomMachine,va_EtatAbsence);


   /* traitement du compte-rendu selon le type de la
      machine locale */
   switch(SEV_TYPE_MACHINE)
   {
      case XZSCC_TYPEM_ST:
      case XZSCC_TYPEM_STA:
      case XZSCC_TYPEM_HIS:
      case XZSCC_TYPEM_LT:
      case XZSCC_TYPEM_SP:
	 
	 /* analyse de la surveillance par une station de 
	    travail , serveurs de protocols ou Local Technique */
	 seta_dgstatus_st(pa_NomMachine, va_EtatAbsence);
	 break;


      case XZSCC_TYPEM_PCS: /* ajout PCS modif 1.26 */
      case XZSCC_TYPEM_PC2: /* ajout PC2 modif 1.31 */
      case XZSCC_TYPEM_PC2LT:
      case XZSCC_TYPEM_SC:

	 /* recuperation de l'etat de la machine locale */
	 XZSE_09EtatMachine(&vl_EtatMachine);

	/*B Traitement si Serveur Actif */
	if (vl_EtatMachine != XZSEC_ETAT_NPASSIF)
	{
   		XZST_03EcritureTrace(XZSTC_WARNING,  
			"appel de seta_dgstatus_sc mach %s -- abscence %d \n",
			pa_NomMachine, va_EtatAbsence);

	 	seta_dgstatus_sc(pa_NomMachine, va_EtatAbsence);
	}


	 break;


      case XZSCC_TYPEM_SD:

	 /* recuperation de l'etat de la machine locale */
	 XZSE_09EtatMachine(&vl_EtatMachine);

	/*B Traitement si Serveur Actif */
	if (vl_EtatMachine != XZSEC_ETAT_NPASSIF)
	{
	 	seta_dgstatus_sd(pa_NomMachine, va_EtatAbsence);
	}

	 break;
     }

   XZST_03EcritureTrace(XZSTC_FONCTION,  
			"seta_dgstatus_cb : OUT   \n");

}	/* seta_dgstatus_cb */



/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_SUR
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction traite un compte-rendu de surveillance
* dans le cas ou la machine locale est une station.
* Rappel: une ST surveille le SC ou le SD de son site
* ainsi que la machine de secours associee.
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_dgstatus_st(
	XZSCT_NomMachine pa_NomMachine,
	int va_EtatAbsence)

/*
* PARAMETRES EN ENTREE :
* pa_NomDGSurv : nom du datagroup de surveillance
* va_EtatAbsence   : SET_ABSENCE_COURTE ou  SET_PRESENCE 
*
* PARAMETRES EN SORTIE :
* 	Aucun
*
* VALEUR(S) DE RETOUR :
* 	Aucun
* CONDITION D'ERREUR :
* 	Aucne
*
* CONDITIONS LIMITES :
*
* 	Aucune
******************************************************/
{
   static char *version = "$Id: seta_sur.c,v 1.38 2020/11/03 17:54:03 pc2dpdy Exp $ : seta_dgstatus_st" ;
   int vl_TypeMachine, vl_NumMessage	= 0;
   XZSCT_Datagroup pl_NomDatagroup	= "";
   XZIAT_destinataire	vl_Destinataire;


   XZST_03EcritureTrace(XZSTC_FONCTION,  
			"seta_dgstatus_st : IN    \n");

	/*A Affectation du Destinataire */
	strcpy(vl_Destinataire.site,SEP_NOM_SITE);

   /* si coupure des deux serveurs */
   if  ( (SEP_ETAT_MACH[vm_NumServeur1].EtatAbsence != SET_PRESENCE) &&
        (SEP_ETAT_MACH[vm_NumServeur2].EtatAbsence != SET_PRESENCE) ) 
   {
      /*A envoi du message systeme supervision du site local hors service */
      XZST_03EcritureTrace(XZSTC_INFO,"Envoi Msg System dans fenetre Admini \
	-- Destinataire %s -- Num Texte %d -- Nom Machine %s -- Site %s \n",
	vl_Destinataire,XDL_SUP30_SUPERHS,SEP_NOM_MACHINE,SEP_NOM_SITE);
      XZSI_08AffMsgAdm(vl_Destinataire,XDL_SUP30_SUPERHS,
				SEP_NOM_MACHINE,SEP_NOM_SITE,NULL);
   }
   else
   {
      /* si remontee de l'un des serveurs apres chute des deux serveurs */
      if ( ( (SEP_ETAT_MACH[vm_NumServeur1].EtatAbsence != SET_PRESENCE) &&
             (SEP_ETAT_MACH[vm_NumServeur2].EtatAbsence == SET_PRESENCE) ) || 
           ( (SEP_ETAT_MACH[vm_NumServeur2].EtatAbsence != SET_PRESENCE) &&
             (SEP_ETAT_MACH[vm_NumServeur1].EtatAbsence == SET_PRESENCE) ) )
      {
	 if (va_EtatAbsence == SET_PRESENCE)
	 {
	 	/* recuperation du type de la machine */
	 	XZSA_17TypeMachine(pa_NomMachine, &vl_TypeMachine);

         	/* envoi du message systeme de reprise 
			de communication avec le serveur local */ 
		if ( (vl_TypeMachine == XZSCC_TYPEM_SC) || 
		     (vl_TypeMachine == XZSCC_TYPEM_PCS) || 
		     (vl_TypeMachine == XZSCC_TYPEM_PC2) || 
		     (vl_TypeMachine == XZSCC_TYPEM_PC2LT) )  /* Ajout type PCS modif 1.26 */ /* Ajout type PC2 modif 1.31*/
		{
			vl_NumMessage =  XDL_SUP50_COMSCOK ;
		}
		else
		{
			vl_NumMessage =  XDL_SUP51_COMSDOK;
		}
      XZST_03EcritureTrace(XZSTC_INFO,"Envoi Msg System dans fenetre Admini \
	-- Destinataire %s -- Num Texte %d -- Nom Machine %s -- Mach %s \n",
	vl_Destinataire,vl_NumMessage,SEP_NOM_MACHINE,pa_NomMachine);
         	XZSI_08AffMsgAdm(vl_Destinataire,vl_NumMessage, 
					SEP_NOM_MACHINE,pa_NomMachine,NULL);

	 }
      }
   }

   XZST_03EcritureTrace(XZSTC_FONCTION,  
			"seta_dgstatus_st : OUT     \n");

}	/* Fin seta_dgstatus_st */




/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_SUR
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction traite un compte-rendu de surveillance
* dans le cas ou la machine locale est un serveur de
* communication.
* Rappel: un SC surveille les stations  de son site
* ainsi que son serveur de donnees et la machine de
* secours associee.
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_dgstatus_sc(
	XZSCT_NomMachine pa_NomMachine, 
	int va_EtatAbsence)

/*
* PARAMETRES EN ENTREE :
* pa_NomDGSurv : nom du datagroup de surveillance
* va_EtatAbsence   : SET_ABSENCE_COURTE ou  SET_PRESENCE
*
* PARAMETRES EN SORTIE :
* 	Aucun
*
* VALEUR(S) DE RETOUR :
* 	Aucune
*
* CONDITION D'ERREUR :
* 	Aucune
*
* CONDITIONS LIMITES :
* 	Aucune
*
******************************************************/
{
   static char *version = "$Id: seta_sur.c,v 1.38 2020/11/03 17:54:03 pc2dpdy Exp $ : seta_dgstatus_sc" ;
   int vl_EtatMachine	= 0;             /* etat de la machine locale */
   int vl_NumMachine	= 0;
   int vl_NbElt		= 0;                   /* pour appel XZSA */
   XZSCT_Datagroup pl_NomDatagroup, pl_NomDatagroupRec;
   int vl_NumTache	= 0;
   int vl_TypeMachine	= 0;
   XZSCT_NomSite	vl_NomSite	= "";
   char 		vl_Texte[255]="";
    XDLT_SystemeAlarme  vl_MessageCaract;
    XZIAT_destinataire vl_Destinataire;

   XZST_03EcritureTrace(XZSTC_FONCTION,  
		"seta_dgstatus_sc : IN  -- nom mach %s -- Etat Abscence %d \n",
		pa_NomMachine,va_EtatAbsence);

   /*A recuperation du type et du site de la machine */
   XZSA_23SiteMachine(pa_NomMachine, vl_NomSite);
   XZSA_17TypeMachine(pa_NomMachine, &vl_TypeMachine);

   /*A recherche de la machine parmi les machines 
			supervisees par la machine locale */
   seta_rechmach(SEP_ETAT_MACH, SEV_NB_MACH, pa_NomMachine, &vl_NumMachine);
XZST_03EcritureTrace(XZSTC_WARNING,"SEP_ETAT_MACH[vl_NumMachine].Etat=%d",SEP_ETAT_MACH[vl_NumMachine].Etat);
   /* si la machine est un serveur de donnees surveille par la machine locale */
   if ( (SEP_ETAT_MACH[vl_NumMachine].TypeMachine==XZSCC_TYPEM_SD)
	&& (  strcmp( pa_NomMachine , XDC_PASSIF_CI ) ) /* Modif v1.24 : pour que la remontee de SDCI2 ne soit pas compris comme une reprise de comm */
	&& ( (SEP_ETAT_MACH[vl_NumMachine].Etat==XZSEC_ETAT_NACTIF)
    || (SEP_ETAT_MACH[vl_NumMachine].Etat==XZSEC_ETAT_DEGRADE) 
    || (SEP_ETAT_MACH[vl_NumMachine].Etat==XZSEC_ETAT_VEILLE) 
    || (SEP_ETAT_MACH[vl_NumMachine].Etat==XZSEC_SANS_ETAT)) )
   {

	 XZST_03EcritureTrace(XZSTC_WARNING," Machine %s  du type SD=%d \
		Etat Abscence Serveur 1 %d -- Serveur 2 %d \
		argument abscence %d \n",pa_NomMachine,SEP_ETAT_MACH[vl_NumMachine].TypeMachine,
		SEP_ETAT_MACH[vm_NumServeur1].EtatAbsence,SEP_ETAT_MACH[vm_NumServeur2].EtatAbsence,
		va_EtatAbsence);

      /* si coupure des deux serveurs */
      if  ((SEP_ETAT_MACH[vm_NumServeur1].EtatAbsence != SET_PRESENCE) &&
	   (va_EtatAbsence != SET_PRESENCE)) 
      {
            /* recuperation de l'etat de la machine locale */
            XZSE_09EtatMachine(&vl_EtatMachine);

	    if ( (vl_EtatMachine == XZSEC_ETAT_NACTIF)  ||
		(vl_EtatMachine == XZSEC_ETAT_DEGRADE ) ) 
	    {
/*  Modif V1.16   */
#if 1 
	    	/* abonnement au datagroup XDG_NSUPETAT */
	    	if (XDG_JoindreDG(XDG_NSUPETAT) != TRUE)
	    	{
	       		XZST_03EcritureTrace(XZSTC_WARNING,
                     "seta_dgstatus_sc: erreur abonnement DG NSUPETAT");
            	}
#endif

	       /* diffusion a toutes les machines du site du message
	       d'alarme de perte de communication avec le serveur de 
	       donnees */
	    	XZST_10ArchiverMsgSyst(XDL_SUP32_COMSIHS,SEP_NOM_MACHINE,NULL);

   		XZST_03EcritureTrace(XZSTC_DEBUG3,  
			"Abonnement au Datagroup XDG_NSUPETAT \n");


	    	/*A Rafraichissement des Synoptiques avec les etats des 
	    	*	machines des autres SITES a SANS_ETAT */
	    	XZSA_18ListeSite(seta_init_site,&vl_NbElt,NULL);


#if 1	/* Modif  V1.17  */
		/* lancement du test reseau */
         	seta_arme_testres(pa_NomMachine);
#endif

            }
      }
      else
      {
   		XZST_03EcritureTrace(XZSTC_WARNING,  
			"remontee d'un Serveur !! -- \
			Server 1 %d -- Server 2 %d -- Etat Pres %d \n",
			SEP_ETAT_MACH[vm_NumServeur1].EtatAbsence,
			SEP_ETAT_MACH[vm_NumServeur2].EtatAbsence,
			va_EtatAbsence);

         /* si remontee de l'un des serveurs apres chute 
	 *				des deux serveurs */
	 if (va_EtatAbsence == SET_PRESENCE)
         {
	       /* recuperation de l'etat de la machine locale */
	       XZSE_09EtatMachine(&vl_EtatMachine);

	       if ( (vl_EtatMachine == XZSEC_ETAT_NACTIF) ||
			(vl_EtatMachine == XZSEC_ETAT_DEGRADE) ) 
	       {
	          /* diffusion a toutes les machines du site du message
	          d'alarme de retour de la communication avec le serveur de 
	          donnees */
/*	    		XZST_10ArchiverMsgSyst(XDL_SUP47_COMBEOK,
							SEP_NOM_MACHINE,NULL);
*/  	       	
			
			sprintf( vl_Texte, vl_MessageCaract.FormatFranc, SEP_NOM_MACHINE );
			strcpy(vl_Destinataire.site,SEP_NOM_SITE);
	  		XZSI_06AffMsgSyst(vl_Destinataire, XDL_SUP47_COMBEOK, SEP_NOM_MACHINE,NULL);
			
   			XZST_03EcritureTrace(XZSTC_DEBUG3,  
				"Desabonnement au Datagroup XDG_NSUPETAT \n");
			/* desabonnement au datagroup XDG_NSUPETAT */
	       		if (XDG_QuitterDG(XDG_NSUPETAT) != TRUE)
	       		{
	          		XZST_03EcritureTrace(XZSTC_WARNING,
		    	"seta_dgstatus_sc: erreur desabonnement DG NSUPETAT");
	       		}
	       	
			 XZSI_03DiffEtatServeur("SDCI1",XZSEC_ETAT_NACTIF);
			 sleep(1);

                       XZSE_20InitSynoptiqueAG();
                     
#if 1	/* Modif  V1.17  */
         	/* desarmement de l'eventuel test reseau en cours */
         	seta_arret_testres(pa_NomMachine);
#endif
		}
         }
      }
   }   
   else
   /* si la machine n'est pas un serveur de donnees surveilles par
      la machine locale mais une station du site local 
      *	(ou sa machine de secours ?) */
   {
      /* enclenchement d'un test reseau si le nombre de client est passe a 0 */

      if  (va_EtatAbsence == SET_ABSENCE_COURTE) 
      {
	 /* lancement du test reseau */
         seta_arme_testres(pa_NomMachine);

	 /* initialisation des taches et des sous-systemes */
	 seta_etat_init(pa_NomMachine,0,XZSEC_SANS_ETAT);
                                          
      }
      else if (va_EtatAbsence == SET_PRESENCE)
      {
         /* desarmement de l'eventuel test reseau en cours */
         seta_arret_testres(pa_NomMachine);

	if (SEP_ETAT_MACH[vl_NumMachine].TypeMachine==XZSCC_TYPEM_SD)
	   {
	   XZST_03EcritureTrace(XZSTC_DEBUG3,
		  "Desabonnement au Datagroup XDG_NSUPETAT \n");
	  /* desabonnement au datagroup XDG_NSUPETAT */
	  if (XDG_QuitterDG(XDG_NSUPETAT) != TRUE)
	  	{
	  	XZST_03EcritureTrace(XZSTC_WARNING,
	  		"seta_dgstatus_sc: erreur desabonnement DG NSUPETAT");
	  	}
																											  }
	 /* demande de l'etat de la station 
	 *		(utile dans le cas d'une coupure reseau) */
         XZSE_30DemanderEtatMachineAppli(pa_NomMachine);
                       
	 /* recuperation de l'etat de la machine locale */
	 XZSE_09EtatMachine(&vl_EtatMachine);

	 /* Envoi du nom du serveur de communication a toutes les taches 
	    de la station afin de prendre en compte un eventuel basculement
	    du serveur de communication durant la coupure de la station */
         if ( (vl_EtatMachine == XZSEC_ETAT_NACTIF) ||
		(vl_EtatMachine == XZSEC_ETAT_DEGRADE) ) 
         {
	    for (vl_NumTache = 0; vl_NumTache < SEV_NB_TACH; vl_NumTache++)
	    {
               if (!strcmp(SEP_ETAT_TACH[vl_NumTache].NomMachine,pa_NomMachine))
	       {
		  /* construction du nom du datagroup XDG_NomMachine_NomTache */
		  XDG_EncodeDG2(pl_NomDatagroup,
			      pa_NomMachine,
			      SEP_ETAT_TACH[vl_NumTache].NomTache);

                  /* envoi du message XDM_TSE_MAJC a la tache */
		  TipcSrvMsgWrite(pl_NomDatagroup,
				  XDM_IdentMsg(XDM_TSE_MAJC),
				  XDM_FLG_SRVMSGWRITE,
				  T_IPC_FT_STR, SEP_NOM_MACHINE,
				  NULL);
               }

	    }
         } 

          
      }
   }
   XZST_03EcritureTrace(XZSTC_FONCTION,  
			"seta_dgstatus_sc : OUT \n");

}	/* Fin seta_dgstatus_sc */




/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_SUR
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction traite un debut ou une fin d'absence  
* d'une machine surveillee par la machine locale dans
* le cas ou cette derniere est un serveur de donnees.     
* Rappel: un SD surveille:
*    - sa machine de secours
*    - les ST de son site
*    - les SC de tous les sites    
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_dgstatus_sd(
	XZSCT_NomMachine pa_NomMachine, 
	int va_EtatAbsence)

/*
* PARAMETRES EN ENTREE :
* pa_NomDGSurv : nom du datagroup de surveillance
* va_EtatAbsence   : SET_ABSENCE_COURTE ou  SET_PRESENCE
*
* PARAMETRES EN SORTIE :
* 	Aucun
*
* VALEUR(S) DE RETOUR :
* 	Aucune
* CONDITION D'ERREUR :
* 	Aucune
*
* CONDITIONS LIMITES :
* 	Aucune
*
******************************************************/
{
   static char *version = "$Id: seta_sur.c,v 1.38 2020/11/03 17:54:03 pc2dpdy Exp $ : seta_dgstatus_sd" ;
	/* nom du site de la machine surveillee */
   XZSCT_NomSite    	pl_NomSite		= "";    
   int 			vl_EtatMachine		= 0;
   int 			vl_NumTache		= 0;
   int 			vl_TypeMachine		= 0;
   int 			vl_NbElt, vl_NumMachine	= 0;
   XZSCT_Datagroup 	pl_NomDatagroup		= "";
   XZSCT_Datagroup	pl_NomDatagroupRec	= "";


   XZST_03EcritureTrace(XZSTC_FONCTION,  
		"seta_dgstatus_sd : IN -- Nom Mach %s -- Etat Abscence %d \n",
		pa_NomMachine,va_EtatAbsence);

   /* recuperation du type et du site de la machine */
   XZSA_23SiteMachine(pa_NomMachine, pl_NomSite);
   XZSA_17TypeMachine(pa_NomMachine, &vl_TypeMachine);

   /* enclenchement d'un test reseau si le nombre de client est passe a 0 */
   if  (va_EtatAbsence == SET_ABSENCE_COURTE) 
   {
      /* lancement du test reseau */
      seta_arme_testres(pa_NomMachine);
                                         
      /*A Teste si SC et ACTIF ou PCS */	/* ajout PCS modif 1.26  et 1.31 */
      if ( (vl_TypeMachine == XZSCC_TYPEM_SC) || 
	   (vl_TypeMachine == XZSCC_TYPEM_PCS) ||
	   (vl_TypeMachine == XZSCC_TYPEM_PC2) ||
	   (vl_TypeMachine == XZSCC_TYPEM_PC2LT) )
      {
         /* diffusion a toutes les machine du site du message d'alarme de perte
	    de communication avec le site */
	    XZST_03EcritureTrace(XZSTC_WARNING, "Perte SC : Affichage XDL_SUP_39 %s \n",pa_NomMachine); 
	   /* XZST_10ArchiverMsgSyst(XDL_SUP39_SCHS,SEP_NOM_MACHINE,pa_NomMachine,pl_NomSite,NULL);*/

	    /*B Teste si la machine est SDDN1 ou SDDM1 */
	    if ( (!strcmp(pa_NomMachine,XDC_ACTIF_DN)) || 
	   		( !strcmp(pa_NomMachine,XDC_ACTIF_DM)) || (vl_TypeMachine == XZSCC_TYPEM_PCS) || 
			(vl_TypeMachine == XZSCC_TYPEM_PC2) || (vl_TypeMachine == XZSCC_TYPEM_PC2LT) )  
            {	
	    	XZST_03EcritureTrace(XZSTC_WARNING, "Perte SC : Affichage XDL_SUP_34 %s \n",pa_NomMachine); 

		/* Perte du Site d'exploitation */
	    	XZST_10ArchiverMsgSyst(XDL_SUP34_COMEXHS,SEP_NOM_MACHINE,pl_NomSite,NULL);
	    }

		XZST_03EcritureTrace(XZSTC_WARNING, " Perte comm avec site %s -- \
			Valeur arg abscence %d -- Nom Machine %s - Type mach %d  \n",
			pl_NomSite,va_EtatAbsence,pa_NomMachine,vl_TypeMachine);
      }
   }
   else if  (va_EtatAbsence == SET_ABSENCE_LONGUE) 
   {
	/*B Abscence Longue */
	XZST_03EcritureTrace(XZSTC_WARNING," Abscence Longue avec site %s -- \
		Valeur arg abscence %d -- Nom Machine %s - Type mach %d  \n",
			pl_NomSite,va_EtatAbsence,pa_NomMachine,vl_TypeMachine);
   }
   else
   {
      /* desarmement de l'eventuel test reseau en cours */
      seta_arret_testres(pa_NomMachine);


sleep(5.0);

     /* demande de l'etat de la station 
     *			(utile dans le cas d'une coupure reseau) */
     XZSE_30DemanderEtatMachineAppli(pa_NomMachine);
     
      /*B recuperation de l'etat de la machine locale */
      XZSE_09EtatMachine(&vl_EtatMachine);


	/* Si machine est un SC Actif */
      if  ( ( (vl_TypeMachine == XZSCC_TYPEM_SC)
		&& ( (!strcmp(pa_NomMachine,XDC_ACTIF_DN)) || (!strcmp(pa_NomMachine,XDC_ACTIF_DM)) )) ||
	    (vl_TypeMachine == XZSCC_TYPEM_PCS) ||
	    (vl_TypeMachine == XZSCC_TYPEM_PC2) ||
	    (vl_TypeMachine == XZSCC_TYPEM_PC2LT) )
      {
         /* diffusion a toutes les machine du site du message 
	 *	d'alarme de reprise de communication avec le site */
	/* REprise communication  du Site d'exploitation */
	XZST_10ArchiverMsgSyst(XDL_SUP48_COMEXOK,SEP_NOM_MACHINE,pl_NomSite,NULL);

	/*B Diffusion du message de reinitialisation des synoptiques lorsque
	*	les sites se reconnectent. Envoi sur les 2 sites qui dialoguent
	*	entre eux	*/

		/* Site Locale Serveur de donnees */
	/* construction du nom du datagroup XDG_IREI_EQT_NomSite correction V1.18*/
	XDG_EncodeDG2(pl_NomDatagroup, XDG_IREI_EQT,SEP_NOM_SITE);

        /* envoi du message XDM_IREI_EQT a la tache */
	TipcSrvMsgWrite(pl_NomDatagroup,
			XDM_IdentMsg(XDM_IREI_EQT),
			XDM_FLG_SRVMSGWRITE, NULL);

		/* Site Locale Serveur de communications */
	/* construction du nom du datagroup XDG_IREI_EQT_NomSite */
	XDG_EncodeDG2(pl_NomDatagroup, XDG_IREI_EQT,pl_NomSite);

        /* envoi du message XDM_IREI_EQT a la tache */
	TipcSrvMsgWrite(pl_NomDatagroup,
			XDM_IdentMsg(XDM_IREI_EQT),
			XDM_FLG_SRVMSGWRITE, NULL);
      }
                       

      /* Envoi du nom du serveur de donnees a toutes les taches 
         de la machine afin de prendre en compte un eventuel basculement
         du serveur de donnees durant la coupure de la station */
      if ( (vl_EtatMachine == XZSEC_ETAT_NACTIF) ||
		(vl_EtatMachine == XZSEC_ETAT_DEGRADE) ) 
      {
         for (vl_NumTache = 0; vl_NumTache < SEV_NB_TACH; vl_NumTache++)
	 {
               if (!strcmp(SEP_ETAT_TACH[vl_NumTache].NomMachine,pa_NomMachine))
	       {
		  /* construction du nom du datagroup XDG_NomMachine_NomTache */
		  XDG_EncodeDG2(pl_NomDatagroup,
			      pa_NomMachine,
			      SEP_ETAT_TACH[vl_NumTache].NomTache);

                  /* envoi du message XDM_TSE_MAJD a la tache */
		  TipcSrvMsgWrite(pl_NomDatagroup,
				  XDM_IdentMsg(XDM_TSE_MAJD),
				  XDM_FLG_SRVMSGWRITE,
				  T_IPC_FT_STR, SEP_NOM_MACHINE,
				  NULL);

                  /* recuperation du site de la machine */
                  XZSA_23SiteMachine(pa_NomMachine, pl_NomSite);

                  /* Pour une station sur le meme site que le serveur 
		  *	de donnees, on envoie le nom du serveur 
		  *	de donnees actif en guise de nom de
                  *   serveur de communication actif */
                  if (!strcmp(pl_NomSite, SEP_NOM_SITE))
                  {
                     /* envoi du message XDM_TSE_MAJC a la tache */
		     TipcSrvMsgWrite(pl_NomDatagroup,
		   		     XDM_IdentMsg(XDM_TSE_MAJC),
				     XDM_FLG_SRVMSGWRITE,
				     T_IPC_FT_STR, SEP_NOM_MACHINE,
				     NULL);
                  }
               }

	 }
      } 
          
   }

   XZST_03EcritureTrace(XZSTC_FONCTION,  
		"seta_dgstatus_sd : OUT \n");

}	/* Fin seta_dgstatus_sd */



/* definition des fonctions internes */


/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE MODULE
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction stoppe le test reseau d'une machine.
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

int seta_arret_testres(
	XZSCT_NomMachine pa_NomMachine)

/*
* PARAMETRES EN ENTREE :
* pa_NomMachine : nom de la machine concernee
*
*
* PARAMETRES EN SORTIE :
* 	Aucun
*
* VALEUR(S) DE RETOUR :
* 	Aucune
* CONDITION D'ERREUR :
* 	Aucune
*
* CONDITIONS LIMITES :
* 	Aucune
*
******************************************************/
{
   static char *version = "$Id: seta_sur.c,v 1.38 2020/11/03 17:54:03 pc2dpdy Exp $ : seta_arret_testres" ;
   XZSCT_Datagroup pl_NomDatagroup	= "";
   int vl_ValRet 	= XDC_OK;

   XZST_03EcritureTrace(XZSTC_FONCTION,  
		"seta_arret_testres : IN -- Nom mach %s \n",pa_NomMachine);


   /* arret du test reseau */
   if (XZSG_02ArretSurveiller(pa_NomMachine) != XDC_OK)
   {
      XZST_03EcritureTrace(XZSTC_WARNING,
                     "seta_arret_testres : echec de l'annulation du timer");
   }

   XZST_03EcritureTrace(XZSTC_FONCTION,  
		"seta_arret_testres : OUT \n");

   return(vl_ValRet);
}	/* Fin seta_arret_testres */



/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE MODULE
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction arme le test reseau d'une machine.
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

int seta_arme_testres(
	XZSCT_NomMachine pa_NomMachine)

/*
* PARAMETRES EN ENTREE :
* pa_NomMachine : nom de la machine concernee
*
*
* PARAMETRES EN SORTIE :
* 	Aucun
*
* VALEUR(S) DE RETOUR :
* 	Aucune
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
   static char *version = "$Id: seta_sur.c,v 1.38 2020/11/03 17:54:03 pc2dpdy Exp $ : seta_arme_testres" ;
   int vl_ValRet 	= XDC_OK;

   XZST_03EcritureTrace(XZSTC_FONCTION,  
		"seta_arme_testres : IN -- Nom mach %s \n",pa_NomMachine);

   /* Armement du test reseau */
   if (XZSG_01SurveillerReseau(pa_NomMachine) != XDC_OK)
   {
      XZST_03EcritureTrace(XZSTC_WARNING,
              "seta_arme_testres : echec de la programmation du test reseau");
   };

   XZST_03EcritureTrace(XZSTC_FONCTION,  
		"seta_arme_testres : OUT \n");

   return(vl_ValRet);
}	/* Fin seta_arme_testres */




/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_SUR
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction arme la surveillance d'une machine
* en abonnant la machine locale au datagroup  
* XDG_NSUPVIE_NomMachine. 
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

int seta_arme_surv(
	XZSCT_NomMachine pa_NomMachine)

/*
* PARAMETRES EN ENTREE :
* pa_NomMachine : nom de la machine a surveiller
*
* PARAMETRES EN SORTIE :
*	Aucun
*
* VALEUR(S) DE RETOUR :
* XDC_OK ou XDC_NOK (echec abonnement)
*
* CONDITION D'ERREUR :
* 	Aucune
* CONDITIONS LIMITES :
* 	Aucune
*
******************************************************/
{
   static char *version = "$Id: seta_sur.c,v 1.38 2020/11/03 17:54:03 pc2dpdy Exp $ : seta_arme_surv" ;
   XZSCT_Datagroup pl_NomDatagroup	= "";
   int vl_ValRet 			= XDC_OK;

   XZST_03EcritureTrace(XZSTC_FONCTION,  
		"seta_arme_surv : IN -- Nom mach %s \n",pa_NomMachine);

   /* creation du nom du datagroup XDG_NSUPVIE_NomMachine */ 
   XDG_EncodeDG2(pl_NomDatagroup, XDG_NSUPVIE, pa_NomMachine);
XZST_03EcritureTrace(XZSTC_DEBUG1,"pl_NomDatagroup=%s",pl_NomDatagroup);
   /* abonnement au  datagroup XDG_NSUPVIE_NomMachine */
   if(!XDG_JoindreDG(pl_NomDatagroup))
   {
      /* erreur abonnement datagroup */
      XZST_03EcritureTrace(XZSTC_WARNING,
			   "seta_arme_surv: erreur abonnement datagroup XDG_NSUPVIE_%s",
			   pa_NomMachine);
      vl_ValRet = XDC_NOK;
   }

   XZST_03EcritureTrace(XZSTC_DEBUG1,  
		"Abonnement Datagroup %s \n",pl_NomDatagroup);

   XZST_03EcritureTrace(XZSTC_FONCTION,  
		"seta_arme_surv : OUT  -- Code Retour %d \n",vl_ValRet);

   return(vl_ValRet);

}	/* Fin seta_arme_surv */



/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_SUR
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction desarme la surveillance d'une machine
* en desabonnant la machine locale du datagroup
* XDG_NSUPVIE_NomMachine. 
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

int seta_desarme_surv(
	XZSCT_NomMachine pa_NomMachine)

/*
* PARAMETRES EN ENTREE :
* pa_NomMachine : nom de la machine a surveiller
*
*
* PARAMETRES EN SORTIE :
*	Aucun
*
* VALEUR(S) DE RETOUR :
* XDC_OK ou XDC_NOK
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
   static char *version = "$Id: seta_sur.c,v 1.38 2020/11/03 17:54:03 pc2dpdy Exp $ : seta_desarme_surv" ;
   XZSCT_Datagroup pl_NomDatagroup	= "";
   int vl_ValRet 			= XDC_OK;

   XZST_03EcritureTrace(XZSTC_FONCTION,  
		" seta_desarme_surv : IN -- Nom mach %s \n",pa_NomMachine);

   /* creation du nom du datagroup XDG_NSUPVIE_NomMachine */ 
   XDG_EncodeDG2(pl_NomDatagroup, XDG_NSUPVIE, pa_NomMachine);

   /* desabonnement du datagroup XDG_NSUPVIE_NomMachine */
   if(!XDG_QuitterDG(pl_NomDatagroup))
   {
      /* erreur desabonnement datagroup */
      XZST_03EcritureTrace(XZSTC_WARNING,
			   "seta_desarmesurv: erreur desabonnement datagroup XDG_NSUPVIE_%s",
			   pa_NomMachine);
      vl_ValRet = XDC_NOK;
   }

   XZST_03EcritureTrace(XZSTC_FONCTION,  
		" seta_desarme_surv : OUT -- Code Retour %d \n",vl_ValRet);

   return(vl_ValRet);

}	/* Fin seta_desarme_surv */



/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_SUR
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction realise un traitement sur toutes les
* machines surveillees par la machine locale.
* Elle fonctionne de la meme maniere qu'une fonction
* du type XZSA.
*
******************************************************
* SEQUENCE D'APPEL :
*/

int seta_trait_mach_superv(void (*pa_Traitement)())

/*
* PARAMETRES EN ENTREE :
* pa_NomMachine : nom du serveur
* va_NbClient  : nombre de clients
*
*
* PARAMETRES EN SORTIE :
* -
*
* VALEUR(S) DE RETOUR : code d'erreur
*
* CONDITION D'ERREUR :
*
* XDC_OK  : memorisation effectuee
* XDC_NOK : machine non trouvee dans pm_ServSurv 
*
* CONDITIONS LIMITES :
*
* 	Aucune
*
******************************************************/
{
   static char *version = "$Id: seta_sur.c,v 1.38 2020/11/03 17:54:03 pc2dpdy Exp $ : seta_trait_mach_superv" ;
   int vl_NumMachine, vl_NbMachine	= 0;
   int vl_ValRet 	= XDC_OK;

   XZST_03EcritureTrace(XZSTC_FONCTION,  
		" seta_trait_mach_superv : IN \n");

   /* test si le tableau SEP_ETAT_MACH est deja renseigne */
   if (SEP_ETAT_MACH == NULL)
   {
      /* il faut le creer */
      /* dans ce cas il faut discerner les machines 
	*	a surveiller parmi toutes les  machines */
      strcpy(SEP_NOM_SERVEUR1, "");
      strcpy(SEP_NOM_SERVEUR2, "");

   XZST_03EcritureTrace(XZSTC_WARNING,  
	"seta_trait_mach_superv :   trie des machines \n");

      XZSA_01ListeMachine(seta_trie_mach, &vl_NbMachine, NULL);

   }

   /* on utilise le tableau */
   if (pa_Traitement != NULL)
   {
      for (vl_NumMachine = 0; vl_NumMachine < SEV_NB_MACH; vl_NumMachine++) 
      {
         /* on applique la fonction a la machine */
         (* pa_Traitement)(SEP_ETAT_MACH[vl_NumMachine].NomMachine, 
							vl_NumMachine);
      }
   }

   XZST_03EcritureTrace(XZSTC_FONCTION,  
		" seta_trait_mach_superv : OUT  \n");

   return(vl_ValRet);
}	/* Fin seta_trait_mach_superv */


/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_SUR
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction realise un traitement sur toutes les
* machines surveillees par la machine locale.
* Elle fonctionne de la meme maniere qu'une fonction
* du type XZSA.
*
******************************************************
* SEQUENCE D'APPEL :
*/

int seta_trait_mach_surv(void (*pa_Traitement)())

/*
* PARAMETRES EN ENTREE :
* pa_NomMachine : nom du serveur
* va_NbClient  : nombre de clients
*
*
* PARAMETRES EN SORTIE :
* 	Aucun-
*
* VALEUR(S) DE RETOUR : code d'erreur
*
* CONDITION D'ERREUR :
*
* XDC_OK  : memorisation effectuee
* XDC_NOK : machine non trouvee dans pm_ServSurv 
*
* CONDITIONS LIMITES :
*
* 	Aucune
*
******************************************************/
{
   static char *version = "$Id: seta_sur.c,v 1.38 2020/11/03 17:54:03 pc2dpdy Exp $ : seta_trait_mach_surv" ;
   int vl_NumMachine, vl_NbMachine	= 0;
   int vl_ValRet 	= XDC_OK;

   XZST_03EcritureTrace(XZSTC_FONCTION,  
		" seta_trait_mach_surv : IN   \n");

   /* test si le tableau SEP_ETAT_MACH est deja renseigne */
   if (SEP_ETAT_MACH == NULL)
   {
      /* il faut le creer */
      /* dans ce cas il faut discerner les machines a surveiller 
						parmi toutes les  machines */
      strcpy(SEP_NOM_SERVEUR1, "");
      strcpy(SEP_NOM_SERVEUR2, "");
      XZSA_01ListeMachine(seta_trie_mach, &vl_NbMachine, NULL);

   }

   /* on utilise le tableau */
   if (pa_Traitement != NULL)
   {

   XZST_03EcritureTrace(XZSTC_DEBUG1," Traitement non null \
	-- NB machi Super %d \n",SEV_NB_MACH);


      for (vl_NumMachine = 0; vl_NumMachine < SEV_NB_MACH; vl_NumMachine++) 
      {
	XZST_03EcritureTrace(XZSTC_DEBUG1,"Surveiilance machine %s \
		-- Type surveillane %d \n",
		SEP_ETAT_MACH[vl_NumMachine].NomMachine,
		SEP_ETAT_MACH[vl_NumMachine].TypeSurveillance);

	 /* test si machine surveillee (et non pas supervisee) */
	 if (SEP_ETAT_MACH[vl_NumMachine].TypeSurveillance != SEC_SURV_PASSIVE)
	 {
	    /* on applique la fonction a la machine */
            (* pa_Traitement)(SEP_ETAT_MACH[vl_NumMachine].NomMachine, 
							vl_NumMachine);
         }
      }
   }

   XZST_03EcritureTrace(XZSTC_FONCTION,  
		" seta_trait_mach_surv : OUT    \n");

   return(vl_ValRet);
}	/* Fin seta_trait_mach_surv  */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet , suivant le type de la machine de memoriser le type 
*	de surveillance associee
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int seta_trie_mach (
	XZSCT_NomMachine pa_NomMachine, 
	int va_NumOrdre, 
	void *pa_Args)

/*
* ARGUMENTS EN ENTREE :
*   pa_NomMachine	: Nom de la machine
*   va_NumOrdre	:	Numer d'ordre
*   	pa_Args		: valeur de l'argument
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Fonction declenche par une fonction d'interface XZSA
*
* FONCTION 
*   Recupere le type de la machine, et selon memorise le fait 
*	que la machine est surveillee ou supervisee
*
------------------------------------------------------*/
{
   static char *version = "$Id: seta_sur.c,v 1.38 2020/11/03 17:54:03 pc2dpdy Exp $ : seta_trie_mach" ;
   int vl_MachineSupervisee = XDC_NOK;
   int vl_MachineSurveillee = XDC_NOK; 
   XZSCT_NomSite vl_NomSite	= "";
   int vl_TypeMachine	= 0;
   int vl_DureeAbsenceCourte	= 0;
   int	vl_EtatMachine		= 0;
   int vl_ValRet 		= XDC_OK;


   XZST_03EcritureTrace(XZSTC_FONCTION,  
		" seta_trie_mach : IN  mach %s \n", pa_NomMachine);

   /* recuperation du site de la machine */
   XZSA_23SiteMachine(pa_NomMachine, vl_NomSite);

   /* recuperation du type de la machine */
   XZSA_17TypeMachine(pa_NomMachine, &vl_TypeMachine);

/* Modif V1.16  Recuperation de l'etat de la machine */
	XZSE_09EtatMachine(&vl_EtatMachine);

   switch(SEV_TYPE_MACHINE)
   {
      
      case XZSCC_TYPEM_SD:

	/* Suivant si le SD est PASSIF ou pas */
	if (vl_EtatMachine != XZSEC_ETAT_NPASSIF)
	{
	 	/* la machine fait partie des machines supervisees */
	 	vl_MachineSupervisee = XDC_OK;

	 	/* si la machine est une machine du site autre 
	 	*	que la machine locale ou s'il s'agit d'un SC ou PCS ou PC2 */   /* ajout PCS modif 1.26  et 1.31 */
         	if ((!strcmp(vl_NomSite, SEP_NOM_SITE) && strcmp(pa_NomMachine, SEP_NOM_MACHINE)) 
			|| (vl_TypeMachine == XZSCC_TYPEM_SC) || (vl_TypeMachine == XZSCC_TYPEM_PCS) || (vl_TypeMachine == XZSCC_TYPEM_PC2) || (vl_TypeMachine == XZSCC_TYPEM_PC2LT) )
         	{
	    		/* c'est une machine surveillee */
	    		vl_MachineSurveillee = XDC_OK;
         	}
	 }
	 else	/* Serveur PASSIF se comporte come une station */
	 {
	 	/* si la machine est un serveur du site ou 
			la machine locale, elle est supervisee */
	 	if (!strcmp(vl_NomSite, SEP_NOM_SITE))
	 	{
	    		if(( (vl_TypeMachine != XZSCC_TYPEM_ST) 
				&&(vl_TypeMachine != XZSCC_TYPEM_STA) 
				&&(vl_TypeMachine != XZSCC_TYPEM_HIS)
				&&(vl_TypeMachine != XZSCC_TYPEM_LT )
				&&(vl_TypeMachine != XZSCC_TYPEM_SP) )
				|| !strcmp(pa_NomMachine, SEP_NOM_MACHINE))
            		{
	       			vl_MachineSupervisee = XDC_OK;

               			/* s'il s'agit d'une machine autre que la machine locale, 
	       			*			elle est surveillee */
	       			if (strcmp(pa_NomMachine, SEP_NOM_MACHINE))
	       			{
	          			vl_MachineSurveillee = XDC_OK;
/*#if 0*/ /* Modif v1.23 */
	          			/* comme il s'agit d'un serveur du site, 
		  			*			on memorise son numero */  
                  			vm_NumServeur1 = 
			(vm_NumServeur1 == -1 ? SEV_NB_MACH : vm_NumServeur1);
		  			if (vm_NumServeur1 == SEV_NB_MACH)
		  			{
		     				strcpy(SEP_NOM_SERVEUR1, pa_NomMachine);
                  			}

	          			vm_NumServeur2 = 
			(vm_NumServeur1 == -1 ? vm_NumServeur2 : SEV_NB_MACH);
		  			if (vm_NumServeur2 == SEV_NB_MACH)
		  			{
		     				strcpy(SEP_NOM_SERVEUR2, pa_NomMachine);
                  			}
/*#endif*/
               			}
            		}
         	}
	 }

	 break;

      case XZSCC_TYPEM_SC:
	 /* si la machine est une machine du site 
		ou bien un SD, elle est supervisee */
   XZST_03EcritureTrace(XZSTC_WARNING,  
		" seta_trie_mach : Type SC site %s -- vl_TypeMachine %d \n",
		vl_NomSite,vl_TypeMachine);

	/* Suivant si le SD est PASSIF ou pas */
	if (vl_EtatMachine != XZSEC_ETAT_NPASSIF)
	{

#if 1 	/* Modif V1.16.  Un SC peut surveiller un autre SC. Ceci pour permettre de 
*	declencher un test reseau entre SC  et ainsi permettre de se reconnecter entre RTserveur */
         	if (!strcmp(vl_NomSite, SEP_NOM_SITE) 
			|| (vl_TypeMachine == XZSCC_TYPEM_SD)
			|| (vl_TypeMachine == XZSCC_TYPEM_SC)
			|| (vl_TypeMachine == XZSCC_TYPEM_PC2)
			|| (vl_TypeMachine == XZSCC_TYPEM_PC2LT))
#endif
         	{
#if 0
#if 1 	/* Modif 1.17   Si SC pas du meme site, alors surveille mais pas supervise */
			if ( !strcmp(vl_NomSite, SEP_NOM_SITE) || 
				(vl_TypeMachine != XZSCC_TYPEM_SC) )
			{		
	    			vl_MachineSupervisee = XDC_OK;
/* 	Modif V1.18  */
			/*	vl_MachineSurveillee = SEC_SURV_ACTIVE_SC;*/
			}
			else
			{
	    			vl_MachineSupervisee = XDC_OK;
			}
#endif
#endif
	    		vl_MachineSupervisee = XDC_OK;

            		/* s'il s'agit d'une machine autre que la machine locale, 
	    		*			elle est surveillee */
	    		if (strcmp(pa_NomMachine, SEP_NOM_MACHINE))
	    		{
				if ( strcmp(vl_NomSite, SEP_NOM_SITE) && 
					((vl_TypeMachine == XZSCC_TYPEM_SC)
					 || (vl_TypeMachine == XZSCC_TYPEM_PC2)
					 || (vl_TypeMachine == XZSCC_TYPEM_PC2LT)) )
				{
					vl_MachineSurveillee = SEC_SURV_ACTIVE_SC;
				}
				else
				{
	       				vl_MachineSurveillee = XDC_OK;
				}


   				XZST_03EcritureTrace(XZSTC_WARNING,  
					" seta_trie_mach : machine surveillee   mach %s \n", pa_NomMachine);

	       			/* s'il s'agit d'un serveur de donnees, 
	       			*on memorise son numero et son nom */  
	       			if(vl_TypeMachine == XZSCC_TYPEM_SD)
	       			{
		  			vm_NumServeur1 = 
			(vm_NumServeur1 == -1 ? SEV_NB_MACH : vm_NumServeur1);
		  			if (vm_NumServeur1 == SEV_NB_MACH)
		  			{
		     				strcpy(SEP_NOM_SERVEUR1, pa_NomMachine);
                  			}

		  		vm_NumServeur2 = 
			(vm_NumServeur1 == -1 ? vm_NumServeur2 : SEV_NB_MACH);
		  			if (vm_NumServeur2 == SEV_NB_MACH)
		  			{
		     				strcpy(SEP_NOM_SERVEUR2, pa_NomMachine);
                  			}
	       			}
   XZST_03EcritureTrace(XZSTC_WARNING,  
	"TrieMach num Serveur 1 %d  -- Num serveur 2 %d -- Nom mach 2 %s \n",
	vm_NumServeur1,vm_NumServeur2,SEP_NOM_SERVEUR2);
            		}
         	}
	}
	 else	/* Serveur PASSIF se comporte come une station */
	 {
	 	/* si la machine est un serveur du site ou 
			la machine locale, elle est supervisee */
	 	if (!strcmp(vl_NomSite, SEP_NOM_SITE))
	 	{
	    		if(( (vl_TypeMachine != XZSCC_TYPEM_ST) 
				&&(vl_TypeMachine != XZSCC_TYPEM_STA) 
				&&(vl_TypeMachine != XZSCC_TYPEM_HIS)
				&&(vl_TypeMachine != XZSCC_TYPEM_LT )
				&&(vl_TypeMachine != XZSCC_TYPEM_SP) )
				|| !strcmp(pa_NomMachine, SEP_NOM_MACHINE))
            		{
	       			vl_MachineSupervisee = XDC_OK;

               			/* s'il s'agit d'une machine autre que la machine locale, 
	       			*			elle est surveillee */
	       			if (strcmp(pa_NomMachine, SEP_NOM_MACHINE))
	       			{
	          			vl_MachineSurveillee = XDC_OK;

	          			/* comme il s'agit d'un serveur du site, 
		  			*			on memorise son numero */  
                  			vm_NumServeur1 = 
			(vm_NumServeur1 == -1 ? SEV_NB_MACH : vm_NumServeur1);
		  			if (vm_NumServeur1 == SEV_NB_MACH)
		  			{
		     				strcpy(SEP_NOM_SERVEUR1, pa_NomMachine);
                  			}

	          			vm_NumServeur2 = 
			(vm_NumServeur1 == -1 ? vm_NumServeur2 : SEV_NB_MACH);
		  			if (vm_NumServeur2 == SEV_NB_MACH)
		  			{
		     				strcpy(SEP_NOM_SERVEUR2, pa_NomMachine);
                  			}
               			}
            		}
         	}
	 }

	 break;

      case XZSCC_TYPEM_PCS:  /* ajout PCS modif 1.26 */
	 /* si la machine est une machine du site 
		ou bien un SD, elle est supervisee */
   XZST_03EcritureTrace(XZSTC_WARNING,  " seta_trie_mach : Type PCS site %s -- vl_TypeMachine %d \n",
		vl_NomSite,vl_TypeMachine);

	/* Suivant si le SD est PASSIF ou pas */
	if (vl_EtatMachine != XZSEC_ETAT_NPASSIF)
	{

 	/* Modif V1.16.  Un SC peut surveiller un autre SC. Ceci pour permettre de 
*	declencher un test reseau entre SC  et ainsi permettre de se reconnecter entre RTserveur */ /* MAIS un PCS ne surveille pas un SC car les RTserveur ne sont pas connectes */
         	if ((!strcmp(vl_NomSite, SEP_NOM_SITE)) 
			|| (vl_TypeMachine == XZSCC_TYPEM_SD))

         	{
	    		vl_MachineSupervisee = XDC_OK;

            		/* s'il s'agit d'une machine autre que la machine locale, 
	    		*			elle est surveillee */
	    		if (strcmp(pa_NomMachine, SEP_NOM_MACHINE))
	    		{
	       				vl_MachineSurveillee = XDC_OK;


   				XZST_03EcritureTrace(XZSTC_WARNING,  
					" seta_trie_mach : machine surveillee   mach %s \n", pa_NomMachine);

	       			/* s'il s'agit d'un serveur de donnees, 
	       			*on memorise son numero et son nom */  
	       			if(vl_TypeMachine == XZSCC_TYPEM_SD)
	       			{
		  			vm_NumServeur1 = 
			(vm_NumServeur1 == -1 ? SEV_NB_MACH : vm_NumServeur1);
		  			if (vm_NumServeur1 == SEV_NB_MACH)
		  			{
		     				strcpy(SEP_NOM_SERVEUR1, pa_NomMachine);
                  			}

		  		vm_NumServeur2 = 
			(vm_NumServeur1 == -1 ? vm_NumServeur2 : SEV_NB_MACH);
		  			if (vm_NumServeur2 == SEV_NB_MACH)
		  			{
		     				strcpy(SEP_NOM_SERVEUR2, pa_NomMachine);
                  			}
	       			}
   XZST_03EcritureTrace(XZSTC_WARNING,  
	"TrieMach num Serveur 1 %d  -- Num serveur 2 %d -- Nom mach 2 %s \n",
	vm_NumServeur1,vm_NumServeur2,SEP_NOM_SERVEUR2);
            		}
         	}
	}
	 else	/* Serveur PASSIF se comporte come une station */
	 {
	 	/* si la machine est un serveur du site ou 
			la machine locale, elle est supervisee */
	 	if (!strcmp(vl_NomSite, SEP_NOM_SITE))
	 	{
	    		if(( (vl_TypeMachine != XZSCC_TYPEM_ST) 
				&&(vl_TypeMachine != XZSCC_TYPEM_STA) 
				&&(vl_TypeMachine != XZSCC_TYPEM_HIS)
				&&(vl_TypeMachine != XZSCC_TYPEM_LT )
				&&(vl_TypeMachine != XZSCC_TYPEM_SP) )
				|| !strcmp(pa_NomMachine, SEP_NOM_MACHINE))
            		{
	       			vl_MachineSupervisee = XDC_OK;

               			/* s'il s'agit d'une machine autre que la machine locale, 
	       			*			elle est surveillee */
	       			if (strcmp(pa_NomMachine, SEP_NOM_MACHINE))
	       			{
	          			vl_MachineSurveillee = XDC_OK;

	          			/* comme il s'agit d'un serveur du site, 
		  			*			on memorise son numero */  
                  			vm_NumServeur1 = 
			(vm_NumServeur1 == -1 ? SEV_NB_MACH : vm_NumServeur1);
		  			if (vm_NumServeur1 == SEV_NB_MACH)
		  			{
		     				strcpy(SEP_NOM_SERVEUR1, pa_NomMachine);
                  			}

	          			vm_NumServeur2 = 
			(vm_NumServeur1 == -1 ? vm_NumServeur2 : SEV_NB_MACH);
		  			if (vm_NumServeur2 == SEV_NB_MACH)
		  			{
		     				strcpy(SEP_NOM_SERVEUR2, pa_NomMachine);
                  			}
               			}
            		}
         	}
	 }

	 break;
      case XZSCC_TYPEM_PC2LT:  /* ajout PC2 modif 1.31 */
      case XZSCC_TYPEM_PC2:  /* ajout PC2 modif 1.31 */
	 /* si la machine est une machine du site 
		ou bien un SD, elle est supervisee */
   XZST_03EcritureTrace(XZSTC_WARNING,  " seta_trie_mach : Type PC2 site %s -- vl_TypeMachine %d \n",
		vl_NomSite,vl_TypeMachine);

	/* Suivant si le SD est PASSIF ou pas */
	if (vl_EtatMachine != XZSEC_ETAT_NPASSIF)
	{

 	/* Modif V1.16.  Un SC peut surveiller un autre SC. Ceci pour permettre de 
*	declencher un test reseau entre SC  et ainsi permettre de se reconnecter entre RTserveur */ /* MAIS un PCS ne surveille pas un SC car les RTserveur ne sont pas connectes */
         	if ((!strcmp(vl_NomSite, SEP_NOM_SITE)) 
			|| (vl_TypeMachine == XZSCC_TYPEM_SD)
			|| (vl_TypeMachine == XZSCC_TYPEM_SC)
			|| (vl_TypeMachine == XZSCC_TYPEM_PC2)
			|| (vl_TypeMachine == XZSCC_TYPEM_PC2LT))

         	{
	    		vl_MachineSupervisee = XDC_OK;

            		/* s'il s'agit d'une machine autre que la machine locale, 
	    		*			elle est surveillee */
	    		if (strcmp(pa_NomMachine, SEP_NOM_MACHINE))
	    		{
				if ( strcmp(vl_NomSite, SEP_NOM_SITE) && 
					((vl_TypeMachine == XZSCC_TYPEM_SC)
					 || (vl_TypeMachine == XZSCC_TYPEM_PC2)
					 || (vl_TypeMachine == XZSCC_TYPEM_PC2LT))) 
				{
					vl_MachineSurveillee = SEC_SURV_ACTIVE_SC;
				}
				else
				{
	       				vl_MachineSurveillee = XDC_OK;
				}


   				XZST_03EcritureTrace(XZSTC_WARNING,  
					" seta_trie_mach : machine surveillee   mach %s \n", pa_NomMachine);

	       			/* s'il s'agit d'un serveur de donnees, 
	       			*on memorise son numero et son nom */  
	       			if(vl_TypeMachine == XZSCC_TYPEM_SD)
	       			{
		  			vm_NumServeur1 = 
			(vm_NumServeur1 == -1 ? SEV_NB_MACH : vm_NumServeur1);
		  			if (vm_NumServeur1 == SEV_NB_MACH)
		  			{
		     				strcpy(SEP_NOM_SERVEUR1, pa_NomMachine);
                  			}

		  		vm_NumServeur2 = 
			(vm_NumServeur1 == -1 ? vm_NumServeur2 : SEV_NB_MACH);
		  			if (vm_NumServeur2 == SEV_NB_MACH)
		  			{
		     				strcpy(SEP_NOM_SERVEUR2, pa_NomMachine);
                  			}
	       			}
   XZST_03EcritureTrace(XZSTC_WARNING,  
	"TrieMach num Serveur 1 %d  -- Num serveur 2 %d -- Nom mach 2 %s \n",
	vm_NumServeur1,vm_NumServeur2,SEP_NOM_SERVEUR2);
            		}
         	}
	}
	 else	/* Serveur PASSIF se comporte come une station */
	 {
	 	/* si la machine est un serveur du site ou 
			la machine locale, elle est supervisee */
	 	if (!strcmp(vl_NomSite, SEP_NOM_SITE))
	 	{
	    		if(( (vl_TypeMachine != XZSCC_TYPEM_ST) 
				&&(vl_TypeMachine != XZSCC_TYPEM_STA) 
				&&(vl_TypeMachine != XZSCC_TYPEM_HIS)
				&&(vl_TypeMachine != XZSCC_TYPEM_LT )
				&&(vl_TypeMachine != XZSCC_TYPEM_SP) )
				|| !strcmp(pa_NomMachine, SEP_NOM_MACHINE))
            		{
	       			vl_MachineSupervisee = XDC_OK;

               			/* s'il s'agit d'une machine autre que la machine locale, 
	       			*			elle est surveillee */
	       			if (strcmp(pa_NomMachine, SEP_NOM_MACHINE))
	       			{
	          			vl_MachineSurveillee = XDC_OK;

	          			/* comme il s'agit d'un serveur du site, 
		  			*			on memorise son numero */  
                  			vm_NumServeur1 = 
			(vm_NumServeur1 == -1 ? SEV_NB_MACH : vm_NumServeur1);
		  			if (vm_NumServeur1 == SEV_NB_MACH)
		  			{
		     				strcpy(SEP_NOM_SERVEUR1, pa_NomMachine);
                  			}

	          			vm_NumServeur2 = 
			(vm_NumServeur1 == -1 ? vm_NumServeur2 : SEV_NB_MACH);
		  			if (vm_NumServeur2 == SEV_NB_MACH)
		  			{
		     				strcpy(SEP_NOM_SERVEUR2, pa_NomMachine);
                  			}
               			}
            		}
         	}
	 }

	 break;

      case XZSCC_TYPEM_STA:
      case XZSCC_TYPEM_HIS:
      case XZSCC_TYPEM_ST:
      case XZSCC_TYPEM_LT:
      case XZSCC_TYPEM_SP:
	 /* si la machine est un serveur du site ou 
			la machine locale, elle est supervisee */
	 if (!strcmp(vl_NomSite, SEP_NOM_SITE))
	 {
	    if(( (vl_TypeMachine != XZSCC_TYPEM_ST) 
			&&(vl_TypeMachine != XZSCC_TYPEM_STA) 
			&&(vl_TypeMachine != XZSCC_TYPEM_LT )
			&&(vl_TypeMachine != XZSCC_TYPEM_HIS)
			&&(vl_TypeMachine != XZSCC_TYPEM_SP) )
		|| !strcmp(pa_NomMachine, SEP_NOM_MACHINE))
            {
	       vl_MachineSupervisee = XDC_OK;

               /* s'il s'agit d'une machine autre que la machine locale, 
	       *			elle est surveillee */
	       if (strcmp(pa_NomMachine, SEP_NOM_MACHINE))
	       {
	          vl_MachineSurveillee = XDC_OK;

	          /* comme il s'agit d'un serveur du site, 
		  *			on memorise son numero */  
                  vm_NumServeur1 = 
			(vm_NumServeur1 == -1 ? SEV_NB_MACH : vm_NumServeur1);
		  if (vm_NumServeur1 == SEV_NB_MACH)
		  {
		     strcpy(SEP_NOM_SERVEUR1, pa_NomMachine);
                  }

	          vm_NumServeur2 = 
			(vm_NumServeur1 == -1 ? vm_NumServeur2 : SEV_NB_MACH);
		  if (vm_NumServeur2 == SEV_NB_MACH)
		  {
		     strcpy(SEP_NOM_SERVEUR2, pa_NomMachine);
                  }
               }
            }
         }
	 break;

   }

   if (vl_MachineSupervisee == XDC_OK)
   {
      SEV_NB_MACH++;
      SEP_ETAT_MACH = realloc(SEP_ETAT_MACH,SEV_NB_MACH *sizeof(SET_ETAT_MACH));
      strcpy(SEP_ETAT_MACH[SEV_NB_MACH - 1 ].NomMachine, pa_NomMachine);
      strcpy(SEP_ETAT_MACH[SEV_NB_MACH - 1 ].NomSite, vl_NomSite);
      SEP_ETAT_MACH[SEV_NB_MACH - 1 ].TypeMachine = vl_TypeMachine;
      if (vl_MachineSurveillee == XDC_OK)
      {
         SEP_ETAT_MACH[SEV_NB_MACH - 1 ].TypeSurveillance = SEC_SURV_ACTIVE;
         /* on met l'heure de reception du dernier message de vie pour 
	    realiser automatiquement un test reseau sur la machine */
         SEP_ETAT_MACH[SEV_NB_MACH - 1 ].HeureVie = 0;
         SEP_ETAT_MACH[SEV_NB_MACH - 1 ].NbAbsenceVie = 0;
         SEP_ETAT_MACH[SEV_NB_MACH - 1 ].EtatAbsence = SET_PRESENCE;
      }
      else if (vl_MachineSurveillee == SEC_SURV_ACTIVE_SC)
      {
         SEP_ETAT_MACH[SEV_NB_MACH - 1 ].TypeSurveillance = SEC_SURV_ACTIVE_SC;
         /* on met l'heure de reception du dernier message de vie pour 
	    realiser automatiquement un test reseau sur la machine */
         SEP_ETAT_MACH[SEV_NB_MACH - 1 ].HeureVie = 0;
         SEP_ETAT_MACH[SEV_NB_MACH - 1 ].NbAbsenceVie = 0;
         SEP_ETAT_MACH[SEV_NB_MACH - 1 ].EtatAbsence = SET_PRESENCE;
      }
      else
      {
         SEP_ETAT_MACH[SEV_NB_MACH - 1 ].TypeSurveillance = SEC_SURV_PASSIVE;
         SEP_ETAT_MACH[SEV_NB_MACH - 1 ].HeureVie = -1;
	 XZSC_17DureeAbsenceCourte(&vl_DureeAbsenceCourte);
         SEP_ETAT_MACH[SEV_NB_MACH - 1 ].NbAbsenceVie = vl_DureeAbsenceCourte;
         SEP_ETAT_MACH[SEV_NB_MACH - 1 ].EtatAbsence = SET_ABSENCE_LONGUE;
      }
   }

   XZST_03EcritureTrace(XZSTC_FONCTION,  
		" seta_trie_mach : OUT      \n");

   return(vl_ValRet);
}	/* Fin seta_trie_mach */



/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETRAITER
******************************************************
* DESCRIPTION DE LA FONCTION :
* Generation des alarmes systemes signalant les pertes
* et reprises de communication avec les serveurs de
* peripheriques
******************************************************
* SEQUENCE D'APPEL :
*/
void seta_alarm_serv_peri(
	XZSCT_NomMachine pa_NomMachine,
	int              va_Absence)

/*
* PARAMETRES EN ENTREE :
*	pa_NomMachine	: Nom de la machine
*	va_Absence	: etat de la machine
*
* PARAMETRES EN SORTIE :
* 	Aucun
*
* VALEUR(S) DE RETOUR :
* 	Aucune
*
* CONDITION D'ERREUR :
* 	Aucune
* CONDITIONS LIMITES :
* 	Aucune
*
******************************************************/
{
   static char *version = "$Id: seta_sur.c,v 1.38 2020/11/03 17:54:03 pc2dpdy Exp $ : seta_alarm_serv_peri" ;
   int vl_NumMachine	= 0;
   XZSCT_NomSite vl_NomSite	= "";
   XZSCT_NomMachine pl_NomMachineImp, pl_NomMachineTrac	= "";
   int vl_NumMsgSyst	= 0;

   XZST_03EcritureTrace(XZSTC_FONCTION,  
		" seta_alarm_serv_peri : IN   -- Nom mach %s -- Abscence %d \n",
		pa_NomMachine,va_Absence);

   XZST_03EcritureTrace(XZSTC_WARNING ,  
		" Pas de code dans cette fonction \n");


   XZST_03EcritureTrace(XZSTC_FONCTION,  
		" seta_alarm_serv_peri : OUT \n");

}	/* Fin seta_alarm_serv_peri */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Reinitialise a SANS_ETAT l'etat de toutes les machines
*	d'un site
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void seta_init_site(
	XZSCT_NomSite pa_NomSite,
	int va_NumOrdre,
	void *pa_Args)

/*
* ARGUMENTS EN ENTREE :
*   	pa_NomSite	: Nom du Site
*	va_NumOrdre	: numero d'ordre
*	pa_Args		: valeur de l'argument
*
*
* ARGUMENTS EN SORTIE :
*   	Aucun
*
* CODE RETOUR : 
*   	Aucun
*
* CONDITION D'UTILISATION
*   	Appele par une fonction d'interface XZSA
*
* FONCTION 
*   	Appel de la fonction XZSA19
*
------------------------------------------------------*/
{
   static char *version = "$Id: seta_sur.c,v 1.38 2020/11/03 17:54:03 pc2dpdy Exp $ : seta_init_site" ;
	int	vl_NbElt	= 0;


	if ( strcmp(pa_NomSite , SEP_NOM_SITE))
	{
		/* Diffusion a SANS_ETAT des etats machines */
		XZSA_19ListeMachineSite(pa_NomSite,
			&seta_diff_init,&vl_NbElt,XZSEC_SANS_ETAT);
	}

}	/* Fin seta_init_site */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Diifuse l'etat de la machine
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int seta_diff_init(
      XZSCT_NomMachine pa_NomMachine,
      int              va_NumOrdre,
      int              va_EtatMachine)

      

/*
* ARGUMENTS EN ENTREE :
*   	pa_NomMachine	: Nom de la machine
*	va_NumOrdre	: Numero d'ordre
*	va_EtatMachine	: Etat de la machine
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Appele par une fonction d'interface XZSA
*     de la fonction 
*
* FONCTION 
*   Suivant le type de la machine , appel la fonctin d'interface
*	permettant de diffuser l'etat de la machine
*
------------------------------------------------------*/
{
	static char *version = "$Id: seta_sur.c,v 1.38 2020/11/03 17:54:03 pc2dpdy Exp $ : seta_diff_init" ;
	int     vl_TypeMachine  = 0;
	int vl_ValRet 	= XDC_OK;


	XZST_03EcritureTrace(XZSTC_FONCTION ,
	"seta_mach_init : IN  -- Nom Mach %s -- Eta mach %d  \n",
	pa_NomMachine,va_EtatMachine);

	/*A Recuperation du Type de la Machine */
	XZSA_17TypeMachine(pa_NomMachine,&vl_TypeMachine);

	if ( (vl_TypeMachine != XZSCC_TYPEM_SD)
	&&  (vl_TypeMachine != XZSCC_TYPEM_PC2)
	&&  (vl_TypeMachine != XZSCC_TYPEM_PC2LT)
	&&  (vl_TypeMachine != XZSCC_TYPEM_PCS)
	&&  (vl_TypeMachine != XZSCC_TYPEM_SC) )
	{
		/*A Diffusion de l'etat des machines a l'IHM */
		XZSI_11DiffEtatStation(pa_NomMachine, va_EtatMachine);
	}
	else
	{
		/*A Diffusion de l'etat des machines a l'IHM */
		XZSI_03DiffEtatServeur(pa_NomMachine, va_EtatMachine);
	}

	 XZST_03EcritureTrace(XZSTC_FONCTION , "seta_mach_init : OUT \n");

   return(vl_ValRet);
}	/* Fin seta_diff_init */

