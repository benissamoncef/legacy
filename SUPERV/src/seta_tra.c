/*E*/
/* Fichier : $Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $ Release : $Revision: 1.33 $        Date : $Date: 2020/11/03 17:54:09 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_TRA * FICHIER seta_tra.c
******************************************************
* DESCRIPTION DU MODULE :
*
*	Module qui permet l'etat de la machine, de ses sous-systemes
*	et de ses taches. Egalement pour les machines surveillees l'etat 
*	reseau de celles-ci.
*
* seta_etappli_cb	: Cette fonction modifie l'etat de la machine locale
* 	en fonction de l'etat de l'applicatif fourni par SAPL.
* seta_etares_cb	: Cette fonction modifie l'etat d'une machine surveillee
* 	par la machine locale en fonction d'un etat reseau.
* seta_etamach_cb	: Cette fonction traite l'etat d'une machine qu'elle
*	surveille (historisation, affichage synoptique, ...)
* seta_etassys_cb	: Cette fonction demande l'historisation de l'etat d'un
* 	sous-systeme.
* seta_etatach_cb	:  Cette fonction demande l'historisation de l'etat d'une
* 	tache.
* seta_etamode_cb	: Cette fonction demande l historisation du mode
* 	d une machine
* seta_etatrac_cb	:  Cette fonction demande l historisation de l etat
* 	d une tache temporaire
* seta_etaperi_cb	: Cette fonction demande l historisation de l etat
* 	d un peripherique.
* seta_etainte_cb	: Cette fonction demande l historisation de l etat
* 	d une interface
* seta_etattmp_cb	:  Cette fonction demande l historisation de l etat
* 	d une tache temporaire
* seta_etamach		: Cette fonction demande l'historisation de l'etat d'une
* 	machine.
* seta_etassys		:  Cette fonction demande l'historisation de l'etat d'un
* 	sous-systeme.
* seta_etatach		: Cette fonction traite l'etat d'une tache.
* seta_etamode		:  Cette fonction demande l historisation du mode
* 	d une machine
* seta_etatrac		:  Memorisation de l'etat des traces d'une tache
* seta_etat_init	:  Cette fonction reinitialise les entites logicielles
* 	d une machine
* seta_etat_prec	: Cette fonction recherche l etat precedent d une
* 	machine
* seta_etat_prec_enti	:  Cette fonction recherche l etat precedent d une
* 	entite annexe
* seta_informer		: Cette fonction diffuse les informations a une tache
* 	les informations necessaires a son lancement.
* seta_distr_info	: Permet d'envoyer le message sur le Datagroup avec les
*       arguments passes en parametres
* seta_mach_init	:  Permet d'initialiser les differentes etats des machines
* 
******************************************************
* HISTORIQUE :
*
* T.Milleville  24 Oct 1994 	: Creation
*
* T.Milleville  03 Jan 1995 	: Modification de la fonction
*					seta_etat_init.Ajout d'un
*					2 eme parametre pour permettre
*					l'utilisation avec les fonctions XZSA
* T.Milleville  23 Fev 1995 	: Modification du Datagroup pour envoyer 
*				l'indice qualite V1.4
* T.Milleville  08 Mar 1995 	: Modification de XZGA_xx en XZSI_xx V1.5
* T.Milleville  09 Mar 1995 	: Modification de la fonction seta_etatra afin de 
*		specifier si on diffuse le msg Systeme V1.6 
* T.Milleville  29 Mar 1995 	: Modification de XZGA_xx en XZSI_xx V1.5
* T.Milleville  29 Mar 1995 	: Recuperation du code retour de la 
*	fonction seta_actualiser_etattach et suivant celui-ci maj
*	des etats IHM  V1.7
* T.Milleville  27Juin 1995 	: Ajout de traces V1.8
* T.Milleville  25 Jul 1995 	: Ajout de commentaires V1.9
* T.Milleville  26 Jul 1995 	: Mise a jour entete V1.10
* T.Milleville  18 Aou 1995 	: Envoie du msg _disconnect avant _connect
*		Recommandation UNITECHNIC.    V1.11
* T.Milleville  25 Aou 1995 	: Envoie de _disconnect et _connect 
*	seulement si SD ou SC V1.13
* T.Milleville  22 Sep 1995 	: Envoie de l'indice qualite que sur 
*	un SD et si ma MAJ de l'etat a etet effectuee V1.15
* T.Milleville  16 Oct 1995 	: Ajout dans la fonction seta_etaperi_cb de la recuperation
* du type du peripherique V1.16
* T.Milleville  30  Oct 1995 	: Si perte de comm entre un SD Actif et un SC Actif alors 
*	initialisation a SANS ETAT des machines du Site concernee V1.17
* TM et PN      18 Dec 1995	: Ajout du nume de machine ds setat_etamach pour tester le type de surveillance (permet surv inter sc sans supervision) v1.18
* P.NIEPCERON   16 Jan 1996	: Modif dans seta_distr_info v1.19
****
* P.NIEPCERON	22 aou 1996	: Ajout du type de machine PC simplie v1.21
* P.NIEPCERON  12 Nov 1996	: Ajout de la reconnexion RTserver au PC simplifie v1.23
* P.NIEPCERON	22 Dec 1997	: Ajout du type de machine PC 2 et STA v1.24 1.25
* P.NIEPCERON   23 Dec 1997     : _disconnect devient disconnect avec RTW3.5 v1.25 (idem _connect)
* P.NIEPCERON   30 Avr 1998	: tracage du disconnect connect v1.26
* P.Niepceron   01 Jul 1998     : Ajout du type de machine Historique (dem/1697) v1.27
				: modif rtserver v1.28
* P.Niepceron   20 Aou 2009	: Suppression de l'�criture des �tats des p�riph�riques dans le journal. Remplacer les no d'�tat machine & tache par des libell�s DEM900 v1.29
* JPL		16/12/10 : Migration architecture HP ia64 (DEM 961) : suppression erreurs mineures  1.30
* JPL		11/01/11 : Migration architecture HP ia64 (DEM 971) : retaur. instruction en commentaire par erreur  1.31
* LCL		20/12/21	: MOVIS P2 ems SAE-312
* LCL		25/09/2023	: Adaptation code pour rabbitmq DEM-483
*******************************************************/

/******************************************************
* TESTS UNITAIRES  : RELECTURE CROISEE
*
*******************************************************/

/* fichiers inclus */

#include <rtworks/ipc.h>

#include "xdc.h"
#include "xdm.h"
#include "xdg.h"
#include "xdl.h"    /* definition des numeros de libelle de msg */

#include "xzsc.h"
#include "xzsa.h"
#include "xzsg.h"
#include "xzst.h"
#include "xzse.h"
#include "xzsi.h"

#include "seta_don.h"
#include "seta_tra.h"
#include "seta_arc.h"
#include "seta_sur.h"
#include "seta_act.h"


/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

int gl_NbElem;        /* Nombre d element retourne par XZSA_19 */


/* declaration des fonctions internes */

void seta_informer(
	XZSCT_NomMachine pa_NomMachine,
        XZSCT_NomTache pa_NomTache);

void seta_etat_prec_enti(
	XZSCT_NomEntite pa_NomEntite
	,int             va_TypeEntite
	,int            *pa_EtatEntite);

void seta_etat_prec(
	XZSCT_NomMachine pa_NomMachine
	,int             *pa_EtatMachine);

int seta_etat_init(
	XZSCT_NomMachine pa_NomMachine,
	int		va_NumOrdre,
	int		va_Etat);

void seta_etatrac(
	XZSCT_NomMachine pa_NomMachine,
	XZSCT_NomTache pa_NomTache,
	int va_NivTrace,
	int va_VisuTrace,
	int va_Distribution,
	int va_FlagMsgSyst);

void seta_etamode(
	XZSCT_NomMachine pa_NomMachine,
	int              va_ModeIns,
	int              va_ModeRec,
	int 		    va_Distribution);


/* 	Modif V1.18  Ajout va_NumMachine*/
void seta_etamach(
	XZSCT_NomMachine pa_NomMachine,
	int              va_EtatMachine,
	int 		 va_Distribution,
	int 		 va_NumMachine);

void seta_etatach(	
	XZSCT_NomMachine pa_NomMachine,
	XZSCT_NomTache   pa_NomTache,
	int              va_EtatTache,
	int              va_Distribution,
	int 		va_FlagMsgSyst);

void seta_etassys(
	XZSCT_NomMachine  pa_NomMachine,
	XZSCT_NomSSysteme pa_NomSSysteme,
	int               va_EtatSSysteme,
	int		     va_Distribution);

void seta_distr_info_dg(XZSCT_Datagroup va_Datagroup, int va_TypeMessage, ...);
void seta_distr_info(int va_TypeMessage, ...);



static char *version = "$Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $ : SETA_TRA" ;



/*****************************************************/
/*           definition de fonctions externes        */
/*****************************************************/


/*X*/
/* Fichier : $Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $	$Revision: 1.33 $        Release : $Revision: 1.33 $        Date : $Date: 2020/11/03 17:54:09 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_TRA
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction modifie l'etat de la machine locale 
* en fonction de l'etat de l'applicatif fourni par SAPL.
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_etappli_cb(
	T_IPC_CONN va_Conn,
        T_IPC_CONN_PROCESS_CB_DATA pa_Data,
        T_CB_ARG pa_Arg)

/*
* PARAMETRES EN ENTREE :
* Arguments specifiques a un callback IPC.
*
*
* PARAMETRES EN SORTIE :
* 	Aucun
*
* VALEUR(S) DE RETOUR :
* 	Aucun
*
* CONDITION D'ERREUR :
* 	Aucune
*
* CONDITIONS LIMITES :
* 	Aucune
*
******************************************************/
{
   static char *version = "$Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $ : seta_etappli_cb" ;
#ifndef _TIBCO_EMS
   T_STR	    vl_NomMachine	= "";
   T_INT4           vl_EtatApplication	= 0;
#else
   XDY_NomMachine	    vl_NomMachine	= "";
   XDY_Entier           vl_EtatApplication	= 0;
#endif
   int              vl_EtatMachine	= 0;
   int              vl_EtatMachineLoc	= 0;
   char vl_ChaineEtat[3]		= "";
   int		    vl_NumMachine	= -1;


	 XZST_03EcritureTrace(XZSTC_FONCTION ,
		       "seta_etappli_cb: IN \n");


   /* recuperation du nom de la machine et de l'etat Application */ 
   TipcMsgRead(pa_Data -> msg,
	       T_IPC_FT_STR, &vl_NomMachine,
	       T_IPC_FT_INT4, &vl_EtatApplication,
	       NULL);

    sprintf(vl_ChaineEtat, "%d", vl_EtatApplication);
    XZST_03EcritureTrace(XZSTC_INFO, 
	"seta_etappli_cb => NOUVEL ETAT APPLICATION = Etat: %s",
				     vl_ChaineEtat);


   switch(vl_EtatApplication)
   {
      case XZSEC_SANS_ETAT:
      case XZSEC_ETAT_NACTIF:
      case XZSEC_ETAT_NPASSIF:
	 vl_EtatMachine = vl_EtatApplication;
	 break;

      case XZSEC_ETAT_DEGRADE:
	 if (!strcmp(vl_NomMachine,SEP_MACHINE_SECOURS))
 	 {
         	vl_EtatMachine = XZSEC_ETAT_NPASSIF;
	 }
	 else
	 {
    		XZSE_09EtatMachine(&vl_EtatMachineLoc);
    		if (vl_EtatMachineLoc  == XZSEC_ETAT_NPASSIF)
    		{
			/*B Alors Etat Machine est NPASSIF */
			vl_EtatMachine = XZSEC_ETAT_NPASSIF;
    		}
		else
		{
	 		vl_EtatMachine = vl_EtatApplication;
		}
	 }

	 /*B Memorisation etat de la Machine */
	 /*vl_EtatMachine = vl_EtatApplication;*/
	 break;

      default: 
         XZST_03EcritureTrace(XZSTC_WARNING,
             	"seta_etappli_cb: etat application inconnu; etat %d" ,
		vl_EtatMachine);  
   }


/* Modif V1.18  Recherche numero de machine */
   seta_rechmach(SEP_ETAT_MACH, SEV_NB_MACH, vl_NomMachine, &vl_NumMachine);
   /* traitement de l'etat machine avec distribtion aux machines concernees */
   seta_etamach(SEP_NOM_MACHINE, vl_EtatMachine, XDC_OK, vl_NumMachine);

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
		       "seta_etappli_cb: OUT  \n");

}	/* Fin seta_etappli_cb */




/*X*/
/* Fichier : $Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $	$Revision: 1.33 $        Release : $Revision: 1.33 $        Date : $Date: 2020/11/03 17:54:09 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_TRA
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction modifie l'etat d'une machine surveillee
* par la machine locale en fonction d'un etat reseau.
* Important: cette fonction n'est normalement executee
* que par un serveur (pas de test reseau effectue par
* les stations)
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_etares_cb(	
	T_IPC_CONN va_Conn,
        T_IPC_CONN_PROCESS_CB_DATA pa_Data,
        T_CB_ARG pa_Arg)

/*
* PARAMETRES EN ENTREE :
* Arguments specifiques a un callback IPC.
*
*
* PARAMETRES EN SORTIE :
*	Aucun
*
*
* VALEUR(S) DE RETOUR :
* 	Aucune
*
* CONDITION D'ERREUR :
* 	Aucune-
*
* CONDITIONS LIMITES :
* 	Aucune
*
*
******************************************************/
{
   static char *version = "$Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $ : seta_etares_cb" ;
#ifndef _TIBCO_EMS
   T_STR            pl_NomMachine	= "";
   T_INT4           vl_EtatReseau	= 0;
#else
   XDY_NomMachine	    pl_NomMachine	= "";
   XDY_Entier           vl_EtatReseau	= 0;
#endif
   int              vl_EtatMachine	= 0;
   int 		    vl_NumMachine	= 0;
   int              vl_TypeMachine	= 0;
   char             CommandeRTW[100]        = "\0";
   int			vl_TypeMach_Recu	= 0;

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
		       "seta_etares_cb: IN  \n");

   /* recuperation du nom de la machine et de l etat reseau */ 
   TipcMsgRead(pa_Data -> msg,
	       T_IPC_FT_STR,  &pl_NomMachine,
	       T_IPC_FT_INT4, &vl_EtatReseau,
	       NULL);

    XZST_03EcritureTrace(XZSTC_INFO, 
	"seta_etares_cb => NOUVEL ETAT RESEAU = Machine: %s  Etat: %d",
				     pl_NomMachine, vl_EtatReseau);

   /* recherche du numero de la machine */
   seta_rechmach(SEP_ETAT_MACH, SEV_NB_MACH, pl_NomMachine, &vl_NumMachine);

   if ((vl_NumMachine == -1) 
	|| (SEP_ETAT_MACH[vl_NumMachine].TypeSurveillance == SEC_SURV_PASSIVE))
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "seta_etares_cb => \
	RECEPTION ETAT RESEAU MACHINE %s NON SUPERVISEE OU NON SURVEILLEE\n",
	 pl_NomMachine);
   }
   else
   {
#if 0
      /* test si machine toujours en surveillance reseau */
      if (SEP_ETAT_MACH[vl_NumMachine].HeureVie == -1)
      {
#endif
         switch(vl_EtatReseau)
	 {
            case XZSGC_RES_OK: 
               vl_EtatMachine = XZSEC_ETAT_VEILLE;

		/* Recuperation du type de la machine recue */
		if ( XZSA_17TypeMachine (pl_NomMachine, &vl_TypeMach_Recu) != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,
				" Erreur recuperation type d la machine \n");
		}	
/*TIBCO SS66*/
#if 0
		/* Test d'envoi du msg de reconnexion si 
		*	la machine locale est un SD et la mahine recu est un SC ou PCS
		*	ou la machine locale est un SC et la mahine recu est un SD ou SC*/  /* ajout PCS modif 1.21 !! pas de reconnexion entre SC et PCS car pas de RTserveur connecte */
		if ( ((SEV_TYPE_MACHINE == XZSCC_TYPEM_SD) && ((vl_TypeMach_Recu == XZSCC_TYPEM_PCS)||(vl_TypeMach_Recu == XZSCC_TYPEM_PC2)||(vl_TypeMach_Recu == XZSCC_TYPEM_PC2LT)||(vl_TypeMach_Recu == XZSCC_TYPEM_SC)))
			|| ((SEV_TYPE_MACHINE == XZSCC_TYPEM_SC) && 
				((vl_TypeMach_Recu == XZSCC_TYPEM_SD)||(vl_TypeMach_Recu == XZSCC_TYPEM_SC)||(vl_TypeMach_Recu == XZSCC_TYPEM_PC2)||(vl_TypeMach_Recu == XZSCC_TYPEM_PC2LT)))
			|| ((SEV_TYPE_MACHINE == XZSCC_TYPEM_PC2) && 
				((vl_TypeMach_Recu == XZSCC_TYPEM_SD)||(vl_TypeMach_Recu == XZSCC_TYPEM_SC)||(vl_TypeMach_Recu == XZSCC_TYPEM_PC2)||(vl_TypeMach_Recu == XZSCC_TYPEM_PC2LT)))
			|| ((SEV_TYPE_MACHINE == XZSCC_TYPEM_PC2LT) && 
				((vl_TypeMach_Recu == XZSCC_TYPEM_SD)||(vl_TypeMach_Recu == XZSCC_TYPEM_SC)||(vl_TypeMach_Recu == XZSCC_TYPEM_PC2)||(vl_TypeMach_Recu == XZSCC_TYPEM_PC2LT)))
			|| ((SEV_TYPE_MACHINE == XZSCC_TYPEM_PCS) &&
				((vl_TypeMach_Recu == XZSCC_TYPEM_SD))) )
		{
		/*A Reconnexion des RTservers */
		XZST_03EcritureTrace(XZSTC_WARNING,
				"Envoi message de reconnexion RTServer pour %s",pl_NomMachine);

		/*B Envoie du message de Controle _disconnect
		* aux differents RTServers */
		if (!TipcSrvMsgWrite(T_IPC_SUBJECT_SERVER,
				XDM_IdentMsg(T_MT_CONTROL),
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_STR,"disconnect",
				NULL))
		{
			XZST_03EcritureTrace(XZSTC_WARNING,
				"erreur TipcSrvMsgWrite T_MT_CONTROL _disconnect \n");
		}

		/*B Envoie du message de Controle _connect
		* aux differents RTServers */
		if (!TipcSrvMsgWrite(T_IPC_SUBJECT_SERVER,
				XDM_IdentMsg(T_MT_CONTROL),
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_STR,"connect",
				NULL))
		{
			XZST_03EcritureTrace(XZSTC_WARNING,
				"erreur TipcSrvMsgWrite T_MT_CONTROL _connect\n");
		}

		}

	       break;
#endif
	    case XZSGC_RES_HS:

		/*B Affectation a SANS_ETAT */				
	       	vl_EtatMachine = XZSEC_SANS_ETAT;

	       	break;

	    default: 
               XZST_03EcritureTrace(XZSTC_WARNING,
                           "seta_etares_cb => ETAT RESEAU INCONNU");  

	 }

/* 	Modif V1.18     Passage numero de machine */
         /* traitement de l'etat de la machine avec distribution 
	 *				aux machines concernees */ 
	 seta_etamach(pl_NomMachine, vl_EtatMachine, XDC_OK, vl_NumMachine);

#if 0
      }
      else
      {
	 /* on ne fait rien, il s'agit d'un message reseau residuel
	    ou d'un test reseau mal desarme */
         XZST_03EcritureTrace(XZSTC_WARNING,
                           "seta_etares_cb => MESSAGE RESEAU RESIDUEL");  
      }
#endif
   }

    XZST_03EcritureTrace(XZSTC_INFO, 
	"seta_etares_cb => OUT \n");

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
		       "seta_etaes_cb: OUT  \n");

}	/* Fin seta_etares_cb */



/*X*/
/* Fichier : $Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $	$Revision: 1.33 $        Release : $Revision: 1.33 $        Date : $Date: 2020/11/03 17:54:09 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_TRA
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction traite l'etat d'une machine qu'elle
* surveille (historisation, affichage synoptique, ...)
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_etamach_cb(
	T_IPC_CONN va_Conn,
        T_IPC_CONN_PROCESS_CB_DATA pa_Data,
        T_CB_ARG pa_Arg)

/*
* PARAMETRES EN ENTREE :
* Arguments specifiques a un callback IPC.
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
   static char *version = "$Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $ : seta_etamach_cb" ;
#ifndef _TIBCO_EMS
   T_STR	    pl_NomMachine	= "";
   T_INT4 vl_EtatMachine	= 0;
#else
   XDY_NomMachine	    pl_NomMachine	= "";
   XDY_Entier vl_EtatMachine	= 0;
#endif
   int vl_NumMachine		= -1;
   char vl_ChaineEtat[3]	= "";
   
	 XZST_03EcritureTrace(XZSTC_FONCTION ,
		       "seta_etamach_cb: IN  \n");

   /* recuperation du nom de la machine et de l'etat de la machine */ 
   TipcMsgRead(pa_Data -> msg,
	       T_IPC_FT_STR,  &pl_NomMachine,
	       T_IPC_FT_INT4, &vl_EtatMachine,
	       NULL);

    sprintf(vl_ChaineEtat, "%d", vl_EtatMachine);
    XZST_03EcritureTrace(XZSTC_MESSAGE, 
	"seta_etamach_cb => NOUVEL ETAT MACHINE = Machine: %s  Etat: %s",
				     pl_NomMachine, vl_ChaineEtat);

    /* recherche de la machine parmi les machines supervisees */
    seta_rechmach(SEP_ETAT_MACH, SEV_NB_MACH, pl_NomMachine, &vl_NumMachine);

    /* test si machine supervisee */
    if (vl_NumMachine != -1)
    {
       if(strcmp(pl_NomMachine, SEP_NOM_MACHINE))
       {
          /* on traite l'etat machine que si celle 
	  *				si n'est pas en test reseau */ 
          if ((SEP_ETAT_MACH[vl_NumMachine].TypeSurveillance==SEC_SURV_PASSIVE) 
		|| (SEP_ETAT_MACH[vl_NumMachine].EtatAbsence == SET_PRESENCE))
	  {
/* 	Modif V1.18     Passage numero de machine */
	     /* traitement de l'etat machine */
             seta_etamach(pl_NomMachine, vl_EtatMachine, XDC_OK, vl_NumMachine);
          }
	  else
	  {
	     XZST_03EcritureTrace(XZSTC_WARNING, 
		"seta_etamach_cb => NOUVEL ETAT MACHINE ABSENTE NON TRAITE %s \n",pl_NomMachine);

/* 	Modif V1.18     Passage numero de machine */
	     /* traitement de l'etat machine */
	     if ((SEP_ETAT_MACH[vl_NumMachine].TypeSurveillance == SEC_SURV_ACTIVE) ||
		(SEP_ETAT_MACH[vl_NumMachine].TypeSurveillance == SEC_SURV_PASSIVE) ) /* Ajout test sur passive v*/
	     {
             	seta_etamach(pl_NomMachine, vl_EtatMachine, XDC_OK, vl_NumMachine);
	     }
          }
       }
       else
       {
          /* s'il s'agit de la machine locale, le message est
	     un message residuel envoye par XZSE13_EnvoiEtatMachine
	     pour avertir la machine redondante */
           XZST_03EcritureTrace(XZSTC_WARNING, 
		"seta_etamach_cb => NOUVEL ETAT MACHINE LOCALE NON TRAITE");
       }
   }
   else
   {
      XZST_03EcritureTrace(XZSTC_WARNING, 
	"seta_etamach_cb => MACHINE NON SUPERVIEE PAR LA MACHINE LOCALE");
   }

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
		       "seta_etamach_cb: OUT  \n");

}	/* Fin seta_etamach_cb */









/*X*/
/* Fichier : $Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $	$Revision: 1.33 $        Release : $Revision: 1.33 $        Date : $Date: 2020/11/03 17:54:09 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_TRA
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction demande l'historisation de l'etat d'un
* sous-systeme.                                
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_etassys_cb(
	T_IPC_CONN va_Conn,
        T_IPC_CONN_PROCESS_CB_DATA pa_Data,
        T_CB_ARG pa_Arg)

/*
* PARAMETRES EN ENTREE :
* Arguments specifiques a un callback IPC.
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
   static char *version = "$Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $ : seta_etassys_cb" ;
#ifndef _TIBCO_EMS
   T_STR	    pl_NomMachine	= "";
   T_STR  pl_NomSSysteme	= "";
   T_INT4 vl_EtatSSysteme	= 0;
#else
   XDY_NomMachine	    pl_NomMachine	= "";
  XDY_NomSysteme pl_NomSSysteme        = "";
   XDY_Entier vl_EtatSSysteme	= 0;
#endif
   int vl_NumMachine		= 0;
   char vl_ChaineEtat[3]	= "";

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
		       "seta_etassys_cb: IN  \n");

   /* recuperation du nom de la machine, du nom de la
      tache et de l'etat de la tache dans le message */
   TipcMsgRead(pa_Data -> msg,
	       T_IPC_FT_STR,&pl_NomMachine,
	       T_IPC_FT_STR, &pl_NomSSysteme,
	       T_IPC_FT_INT4, &vl_EtatSSysteme,
	       NULL);
  
    sprintf(vl_ChaineEtat, "%d", vl_EtatSSysteme);
    XZST_03EcritureTrace(XZSTC_INFO, "seta_etassys_cb => \
	NOUVEL ETAT TACHE = Machine: %s  SSysteme: %s   Etat: %s",
			     pl_NomMachine, pl_NomSSysteme, vl_ChaineEtat);

    /* recherche de la machine parmi les machines supervisees */
    seta_rechmach(SEP_ETAT_MACH, SEV_NB_MACH, pl_NomMachine, &vl_NumMachine);

    /* test si machine supervisee */
    if (vl_NumMachine != -1)
    {
       /* on traite l'etat machine que si celle si n'est pas en test reseau */ 
       if ((SEP_ETAT_MACH[vl_NumMachine].TypeSurveillance == SEC_SURV_PASSIVE) 
       || (SEP_ETAT_MACH[vl_NumMachine].EtatAbsence == SET_PRESENCE))
       {
          /* traitement de l'etat du sous-systeme */
	  seta_etassys(pl_NomMachine,pl_NomSSysteme,vl_EtatSSysteme, XDC_OK);
       }
       else
       {
          XZST_03EcritureTrace(XZSTC_WARNING, "seta_etassys_cb => NOUVEL ETAT SOUS-SYSTEME MACHINE ABSENTE NON TRAITE");
	  if (SEP_ETAT_MACH[vl_NumMachine].TypeSurveillance == SEC_SURV_ACTIVE)
	  {
          	/* traitement de l'etat du sous-systeme */
	  	seta_etassys(pl_NomMachine,pl_NomSSysteme,vl_EtatSSysteme, XDC_OK);
	  }
       }
    }
    else
    {
       XZST_03EcritureTrace(XZSTC_WARNING, "seta_etassys_cb => MACHINE NON SUPERVISEE PAR LA MACHINE LOCALE");
    }

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
		       "seta_etassys_cb: OUT  \n");

}	/* Fin seta_etassys_cb */


/*X*/
/* Fichier : $Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $	$Revision: 1.33 $        Release : $Revision: 1.33 $        Date : $Date: 2020/11/03 17:54:09 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_TRA
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction demande l'historisation de l'etat d'une
* tache.                                
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_etatach_cb(
	T_IPC_CONN va_Conn,
        T_IPC_CONN_PROCESS_CB_DATA pa_Data,
        T_CB_ARG pa_Arg)

/*
* PARAMETRES EN ENTREE :
* Arguments specifiques a un callback IPC.
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
   static char *version = "$Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $ : seta_etatach_cb" ;
#ifndef _TIBCO_EMS
   T_STR  pl_NomMachine = "";
   T_STR  pl_NomTache	= "";
   T_INT4 vl_EtatTache	= 0;
#else
   XDY_NomMachine	    pl_NomMachine	= "";
   XDY_NomTache	    pl_NomTache	= "";
   XDY_Entier vl_EtatTache	= 0;
#endif
   int vl_NumMachine	= -1;
   char vl_ChaineEtat[3]	= "";

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
		       "seta_etatach_cb : IN  \n");

   /* recuperation du nom de la machine, du nom de la
      tache et de l'etat de la tache dans le message */
   TipcMsgRead(pa_Data -> msg,
	       T_IPC_FT_STR,&pl_NomMachine,
	       T_IPC_FT_STR, &pl_NomTache,
	       T_IPC_FT_INT4, &vl_EtatTache,
	       NULL);
  
    XZST_03EcritureTrace(XZSTC_MESSAGE, "Reception Msg XDM_TSE_TACH \
	-- Nom Machine %s -- Nom Tache %s -- Etat Tache %d \n",
	pl_NomMachine ,  pl_NomTache , vl_EtatTache);


    /*A recherche de la machine parmi les machines supervisees */
    seta_rechmach(SEP_ETAT_MACH, SEV_NB_MACH, pl_NomMachine, &vl_NumMachine);

    /* test si machine supervisee */
    if (vl_NumMachine != -1)
    {
	XZST_03EcritureTrace(XZSTC_DEBUG1,"Teste type de surveillance \
		Type Survellance %d -- Etat Abscence %d \n",
		SEP_ETAT_MACH[vl_NumMachine].TypeSurveillance,
		SEP_ETAT_MACH[vl_NumMachine].EtatAbsence);

       /* on traite l'etat machine que si celle si n'est pas en test reseau */ 
       if ((SEP_ETAT_MACH[vl_NumMachine].TypeSurveillance == SEC_SURV_PASSIVE) 
		|| (SEP_ETAT_MACH[vl_NumMachine].EtatAbsence == SET_PRESENCE))
       {
	  XZST_03EcritureTrace(XZSTC_DEBUG1, "seta_etatach_cb => \
				NOUVEL ETAT TACHE MACHINE PRESENTE mach %s - tache %s -- etat tach %d -- Num Machine %d \n "
				, pl_NomMachine, pl_NomTache, vl_EtatTache, vl_NumMachine);
          /* traitement de l'etat de la tache */
          seta_etatach(pl_NomMachine,pl_NomTache,vl_EtatTache, XDC_OK, XDC_VRAI);
       }
       else
       {
	  XZST_03EcritureTrace(XZSTC_WARNING, "seta_etatach_cb => \
				NOUVEL ETAT TACHE MACHINE ABSENTE NON TRAITE mach %s - tache %s -- etat tach %d \n "
				, pl_NomMachine, pl_NomTache, vl_EtatTache);
          /* traitement de l'etat de la tache */
       /*   seta_etatach(pl_NomMachine,pl_NomTache,vl_EtatTache, XDC_OK, XDC_VRAI);*/
       }
    }
    else
    {
       XZST_03EcritureTrace(XZSTC_WARNING, "seta_etatach_cb => \
			MACHINE NON SUPERVISEE PAR LA MACHINE LOCALE");
    }

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
		       "seta_etatach_cb : OUT   \n");

}	/* Fin seta_etatach_cb  */



/*X*/
/* Fichier : $Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $	$Revision: 1.33 $        Release : $Revision: 1.33 $        Date : $Date: 2020/11/03 17:54:09 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_TRA
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction demande l historisation du mode 
* d une machine
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_etamode_cb(
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
* CONDITIONS LIMITES :
*
* 	Aucune
*
******************************************************/
{
   static char *version = "$Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $ : seta_etatach_cb" ;
#ifndef _TIBCO_EMS
   T_STR	    pl_NomMachine	= "";
   T_INT4 vl_ModeIns ,vl_ModeRec	= 0;
#else
   XDY_NomMachine	    pl_NomMachine	= "";
   XDY_Entier vl_ModeIns ,vl_ModeRec	= 0;
#endif
   int vl_NumMachine		= 0;
   char vl_ChaineModeIns[3], vl_ChaineModeRec[3]	= "";

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
		       "seta_etamode_cb  : IN   \n");

   /* recuperation du nom de la machine, ModeIns et ModeRec
      dans le message */
   TipcMsgRead(pa_Data -> msg,
	       T_IPC_FT_STR,&pl_NomMachine,
	       T_IPC_FT_INT4, &vl_ModeIns,
	       T_IPC_FT_INT4, &vl_ModeRec,
	       NULL);

    sprintf(vl_ChaineModeIns, "%d", vl_ModeIns);
    sprintf(vl_ChaineModeRec, "%d", vl_ModeRec);
    XZST_03EcritureTrace(XZSTC_INFO, "seta_etamode_cb => \
	NOUVEAU MODE STATION = Machine: %s  Mode Ins: %s   Mode Rec: %s",
		     pl_NomMachine, vl_ChaineModeIns, vl_ChaineModeRec);

    /* recherche de la machine parmi les machines supervisees */
    seta_rechmach(SEP_ETAT_MACH, SEV_NB_MACH, pl_NomMachine, &vl_NumMachine);

    /* test si machine supervisee */
    if (vl_NumMachine != -1)
    {
       /* on traite l'etat machine que si celle si n'est pas en test reseau */ 
       if ((SEP_ETAT_MACH[vl_NumMachine].TypeSurveillance == SEC_SURV_PASSIVE) 
		|| (SEP_ETAT_MACH[vl_NumMachine].EtatAbsence == SET_PRESENCE))
       {
          /* traitement et diffusion du mode de la station */
          seta_etamode(pl_NomMachine, vl_ModeIns, vl_ModeRec, XDC_OK);
       }
       else
       {
	  XZST_03EcritureTrace(XZSTC_WARNING, "seta_etamode_cb => \
			NOUVEAU MODE STATION ABSENTE NON TRAITE");
          /* traitement et diffusion du mode de la station */
          seta_etamode(pl_NomMachine, vl_ModeIns, vl_ModeRec, XDC_OK);
       }
    }
    else
    {
       XZST_03EcritureTrace(XZSTC_WARNING, "seta_etamode_cb => \
			MACHINE NON SUPERVISEE PAR LA MACHINE LOCALE");
    }

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
		       "seta_etamode_cb  : OUT   \n");

}	/* Fin seta_etamode_cb */


/*X*/
/* Fichier : $Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $	$Revision: 1.33 $        Release : $Revision: 1.33 $        Date : $Date: 2020/11/03 17:54:09 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_TRA
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction demande l historisation de l etat
* d une tache temporaire 
******************************************************
* SEQUENCE D'APPEL :
*/

            
void seta_etatrac_cb(
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
   static char *version = "$Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $ : seta_etatrac_cb" ;
#ifndef _TIBCO_EMS
   T_STR	    pl_NomMachine	= "";
   T_STR   pl_NomTache	= "";
#else
   XDY_NomMachine	    pl_NomMachine	= "";
   XDY_NomTache   pl_NomTache	= "";
#endif
   int vl_NumMachine	= 0;
   int     vl_NivTrace ,vl_VisuTrace	= 0;
   char vl_ChaineNivTrace[3], vl_ChaineVisuTrace[3]	= "";

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
		       "seta_etatrac_cb  : IN   \n");

   /* Recuperation de Nivtrace,VisuTrace,NomMachine et NomTache dans le message */
   TipcMsgRead(pa_Data -> msg,
	       T_IPC_FT_STR, &pl_NomMachine,
	       T_IPC_FT_STR, &pl_NomTache,
	       T_IPC_FT_INT4,&vl_NivTrace,
	       T_IPC_FT_INT4,&vl_VisuTrace,
	       NULL);

    sprintf(vl_ChaineNivTrace, "%d", vl_NivTrace);
    sprintf(vl_ChaineVisuTrace, "%d", vl_VisuTrace);
    XZST_03EcritureTrace(XZSTC_INFO, "seta_etatrac_cb => NOUVEL ETAT \
	TRACE TACHE = Machine: %s  Tache: %s  Niveau: %s   Visu: %s",
	     pl_NomMachine, pl_NomTache, vl_ChaineNivTrace, vl_ChaineVisuTrace);

    /* recherche de la machine parmi les machines supervisees */
    seta_rechmach(SEP_ETAT_MACH, SEV_NB_MACH, pl_NomMachine, &vl_NumMachine);

    /* test si machine supervisee */
    if (vl_NumMachine != -1)
    {
       /* on traite l'etat machine que si celle si n'est pas en test reseau */ 
       if ( (SEP_ETAT_MACH[vl_NumMachine].TypeSurveillance == SEC_SURV_PASSIVE) ) 
	/*	|| (SEP_ETAT_MACH[vl_NumMachine].EtatAbsence == SET_PRESENCE))*/
       {
          /* traitement et diffusion du mode de la station */
          seta_etatrac(pl_NomMachine, pl_NomTache, vl_NivTrace, 
					vl_VisuTrace, XDC_OK, XDC_VRAI);
       }
       else
       {
	  XZST_03EcritureTrace(XZSTC_DEBUG1,
			"seta_etatrac_cb => NOUVEL ETAT TRACE TACHE MACHINE ABSENTE NON TRAITE %s -- tache %s type surv %d -- etat absce %d \n",
			pl_NomMachine,pl_NomTache,
			SEP_ETAT_MACH[vl_NumMachine].TypeSurveillance,
			SEP_ETAT_MACH[vl_NumMachine].EtatAbsence);
	  if (SEP_ETAT_MACH[vl_NumMachine].TypeSurveillance == SEC_SURV_ACTIVE)
	  {
          	/* traitement et diffusion du mode de la station */
          	seta_etatrac(pl_NomMachine, pl_NomTache, vl_NivTrace, 
					vl_VisuTrace, XDC_OK, XDC_VRAI);
	  }
       }
    }
    else
    {
       XZST_03EcritureTrace(XZSTC_WARNING, "seta_etatrac_cb => MACHINE %s (tache %s ) NON SUPERVISEE PAR LA MACHINE LOCALE\n",
		 pl_NomMachine, pl_NomTache);
    }

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
		       "seta_etatrac_cb  : OUT    \n");

}	/* Fin seta_etatrac_cb */


/*X*/
/* Fichier : $Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $	$Revision: 1.33 $        Release : $Revision: 1.33 $        Date : $Date: 2020/11/03 17:54:09 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_TRA
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction demande l historisation de l etat
* d un peripherique.
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_etaperi_cb(
	T_IPC_CONN va_Conn,
        T_IPC_CONN_PROCESS_CB_DATA pa_Data,
        T_CB_ARG pa_Arg)

/*
* PARAMETRES EN ENTREE :
* Arguments specifiques a un callback IPC.
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
* CONDITIONS LIMITES :
* 	Aucune
*
******************************************************/
{
   	static char *version = "$Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $ : seta_etaperi_cb" ;
#ifndef _TIBCO_EMS
   T_STR	    pl_NomPeri	= "";
#else
   XDY_NomMachine	    pl_NomPeri	= "";
#endif
   	XZSCT_NomSite  pl_NomSite	= "";

		/* etat du peripherique */
   	int	vl_EtatPeri	= XZSEC_SANS_ETAT;    
		/* etat precedent du peripherique */
	int	vl_EtatPrec	= XZSEC_SANS_ETAT;
        int	vl_EtatMachLoc  =  XZSEC_SANS_ETAT;
	int 	vl_TypeEntite	= 0;


	 XZST_03EcritureTrace(XZSTC_FONCTION ,
		       "seta_etaperi_cb   : IN    \n");

#if 1 	/* Modif V1.16 */
   /* recuperation du Nom du peripherique et de l etat du peripherique
      dans le message */
   TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_STR,&pl_NomPeri,
               T_IPC_FT_INT4, &vl_EtatPeri,
               T_IPC_FT_INT4, &vl_TypeEntite,
	       NULL);
#endif
	 XZST_03EcritureTrace(XZSTC_MESSAGE,
			 " Recuperation message entite %s -- etat entite %d-- type entite %d \n",
			 pl_NomPeri,vl_EtatPeri,vl_TypeEntite);


   /*recuperation etat de la machine locale */
   XZSE_09EtatMachine(&vl_EtatMachLoc);

   if ( (vl_EtatMachLoc == XZSEC_ETAT_NACTIF)
	|| (vl_EtatMachLoc == XZSEC_ETAT_DEGRADE) )
   {

      /* recuperation de l etat precedent de pl_NomPeri */
      seta_etat_prec_enti(pl_NomPeri,vl_TypeEntite,&vl_EtatPrec);

      if ( (vl_EtatPrec == XZSEC_ETAT_OK || vl_EtatPrec == XZSEC_SANS_ETAT)
		&& (vl_EtatPeri == XZSEC_ETAT_NOK) )
      {
	 /* Indique que le periph est OFF */
	 /*XZST_10ArchiverMsgSyst(XDL_SUP45_PERIPHS,pl_NomPeri,NULL);*/

	 /* diffusion de l eat du peripherique */
	 XZSI_04DiffEtatPeriph(pl_NomPeri, vl_EtatPeri);

         /* recuperation du site du periph */
         XZSA_28SiteEntite(pl_NomPeri, vl_TypeEntite, pl_NomSite);
      }
      else if ( (vl_EtatPrec==XZSEC_ETAT_NOK || vl_EtatPrec==XZSEC_SANS_ETAT) 
	    && (vl_EtatPeri == XZSEC_ETAT_OK) )
      {
         /* Indique que le periph est ON */
	 /*XZST_10ArchiverMsgSyst(XDL_SUP57_PERIPOK,pl_NomPeri,NULL);*/

	  /* diffusion de l eat du peripherique */
	  XZSI_04DiffEtatPeriph(pl_NomPeri, vl_EtatPeri);

	  /* recuperation du site du periph */
	  XZSA_28SiteEntite(pl_NomPeri, vl_TypeEntite, pl_NomSite);
      }
   }

   seta_actualiser_etaenti(pl_NomPeri, vl_TypeEntite, vl_EtatPeri);

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
		       "seta_etaperi_cb   : OUT    \n");

}	/* Fin seta_etaperi_cb */


/*X*/
/* Fichier : $Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $	$Revision: 1.33 $        Release : $Revision: 1.33 $        Date : $Date: 2020/11/03 17:54:09 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_TRA
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction demande l historisation de l etat 
* d une interface
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_etainte_cb(
	T_IPC_CONN va_Conn,
        T_IPC_CONN_PROCESS_CB_DATA pa_Data,
        T_CB_ARG pa_Arg)

/*
* PARAMETRES EN ENTREE :
*	Argument d'une callback IPC
*
* PARAMETRES EN SORTIE :
*	Aucune
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
   	static char *version = "$Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $ : seta_etainte_cb" ;
#ifndef _TIBCO_EMS
   T_STR	    pl_NomInte	= "";
#else
   XDY_NomMachine	    pl_NomInte	= "";
#endif

   	int vl_EtatMachineLoc	= 0; /* etat de la machine locale */
   	int vl_EtatInte		= 0;       /* etat de l interface */
   	int  	vl_Maj_EtatInt	= 0;


	 XZST_03EcritureTrace(XZSTC_FONCTION ,
		       "seta_etainte_cb   : IN    \n");

   /* recuperation du nom de l interface et l etat de l interface
      dans le message */
   TipcMsgRead(pa_Data -> msg,
	       T_IPC_FT_STR,&pl_NomInte,
	       T_IPC_FT_INT4, &vl_EtatInte,
	       NULL);
   
   vl_Maj_EtatInt = 
	seta_actualiser_etaenti(pl_NomInte, XZSAC_TYPE_INTE, vl_EtatInte);

   /* recuperation etat machine locale */
   XZSE_09EtatMachine(&vl_EtatMachineLoc);

   if ( (vl_EtatMachineLoc == XZSEC_ETAT_NACTIF)
   	|| (vl_EtatMachineLoc == XZSEC_ETAT_DEGRADE) )
   {

      /* Diffusion de l etat de l interface */
      if (vl_Maj_EtatInt == XDC_VRAI)
      {
      	XZSI_05DiffEtatInterface(pl_NomInte, vl_EtatInte);
      }
   }
   else
   {
      XZST_03EcritureTrace(XZSTC_INFO,
	   "seta_etainte_cb : machine locale non active :pas de diffusion");
   }

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
		       "seta_etainte_cb    : OUT    \n");

}	/* Fin seta_etainte_cb  */




/*X*/
/* Fichier : $Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $	$Revision: 1.33 $        Release : $Revision: 1.33 $        Date : $Date: 2020/11/03 17:54:09 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_TRA
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction demande l historisation de l etat
* d une tache temporaire 
******************************************************
* SEQUENCE D'APPEL :
*/

            
void seta_etattmp_cb(
	T_IPC_CONN va_Conn,
        T_IPC_CONN_PROCESS_CB_DATA pa_Data,
        T_CB_ARG pa_Arg)
/*
* PARAMETRES EN ENTREE :
*	Arggument d'une callback IPC
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
   	static char *version = "$Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $ : seta_etattmp_cb" ;
#ifndef _TIBCO_EMS
   T_STR	    pl_NomEntite	= "";
#else
   XDY_NomMachine	    pl_NomEntite	= "";
#endif
   XZSCT_NomMachine 	pl_NomMachine	= "";
   XZSCT_NomTache   	pl_NomTache	= "";
   int   		vl_EtatTache	= 0; 
   int 			vl_EtatMachineLoc = 0;  /* Etat de la machine locale */
   int			vl_Maj_EtatEnt	= 0;


	 XZST_03EcritureTrace(XZSTC_FONCTION ,
		       "seta_etatmp_cb    : IN    \n");

   /* Recuperation du nom de l'entite dans le message */
   TipcMsgRead(pa_Data -> msg,
	       T_IPC_FT_STR,  &pl_NomEntite,
	       T_IPC_FT_INT4, &vl_EtatTache,
	       NULL);

   /* extraction du nom de la machine et de la tache de l'entite */
   sscanf(pl_NomEntite,"%s %s",pl_NomMachine,pl_NomTache);
   vl_Maj_EtatEnt = seta_actualiser_etaenti(pl_NomEntite,XZSAC_TYPE_TTMP,vl_EtatTache);
   
   /* Recuperation etat machine locale */
   XZSE_09EtatMachine(&vl_EtatMachineLoc);

   if ( ((vl_EtatMachineLoc == XZSEC_ETAT_NACTIF)
   	|| (vl_EtatMachineLoc == XZSEC_ETAT_DEGRADE))
	&& (vl_Maj_EtatEnt == XDC_VRAI) )
   {
      /* Diffusion de l etat de la tache */
      XZSI_02DiffEtatTache(pl_NomMachine,pl_NomTache,vl_EtatTache);
   }
   else
   {
      XZST_03EcritureTrace(XZSTC_INFO,
  	   "seta_etattmp_cb:machine locale non active: pas de diffusion");
   }

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
		       "seta_etatmp_cb     : OUT    \n");

}	/* Fin seta_etatmp_cb */


/*************************************/
/* declaration de fonctions internes */
/*************************************/

/*X*/
/* Fichier : $Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $	$Revision: 1.33 $        Release : $Revision: 1.33 $        Date : $Date: 2020/11/03 17:54:09 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_TRA
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction demande l'historisation de l'etat d'une
* machine.                                
* 
******************************************************
* SEQUENCE D'APPEL :
*/


	/* Modif V1.18     Ajout du numero de mqchine en Argument */
void seta_etamach(
	XZSCT_NomMachine pa_NomMachine,
	int va_EtatMachine,
	int va_Distribution,
	int va_NumMachine)

/*
* PARAMETRES EN ENTREE :
* pa_NomMachine:  nom de la machine        
* va_EtatMachine: etat de la machine
* va_Distribution	: Flag de distribution
* va_NumMachine  :  Numero de machine dans le tableau des etats
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
   	static char *version = "$Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $ : seta_etamach" ;
   int 			vl_EtatMachineLoc	= 0;
   int	  		vl_TypeMachine		= 0;
	/*etat machine sous forme de chaine */
   char   		vl_ChaineEtat[3]	= ""; 
   XZSCT_Datagroup 	pl_NomDatagroup		= "";
   XZSCT_NomMachine 	vl_NomMachineLocale	= "";
   XZSCT_NomSite	vl_NomSiteLoc		= "";
   int 			vl_EtatMachine		= va_EtatMachine;
   int			vl_NumMachine		= 0;
   int			vl_SC_ACTIF		= XDC_FAUX;
   int			vl_Maj_EtatMach		= 0;

	XZSCT_Datagroup vl_Datagroup;

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
		"seta_etamach     : IN  -- Nom Mach %s -- Etat Mach %d \
		 Distrivbution %d -- numero de machine %d \n",pa_NomMachine,va_EtatMachine,
		 va_Distribution, va_NumMachine);

/*   Modif V1.18 	On teste le type de surveillance pour prise en compte de la
*	surveillance inter-SC */

/*A   Si surveillance inter-SC , alors pas d'envoi d'etats de machine */
if ( (va_NumMachine != -1) && (SEP_ETAT_MACH[va_NumMachine].TypeSurveillance != SEC_SURV_ACTIVE_SC) ) 
{


	/*A Recuperation du Site de la Machine Locale */
	XZSA_23SiteMachine(pa_NomMachine,vl_NomSiteLoc);

	/*A recuperation du type de la machine */
	XZSA_17TypeMachine(pa_NomMachine, &vl_TypeMachine);

   	/*A distribution eventuelle du message */
   	if (va_Distribution == XDC_OK)
   	{
#ifndef _TIBCO_EMS
       		/* distribution de l'info aux machines concernees */
       		seta_distr_info(XDM_TSE_MACH,
		     T_IPC_FT_STR, pa_NomMachine,
		     T_IPC_FT_INT4, va_EtatMachine,
		     NULL);
#else
       		/* distribution de l'info aux machines concernees */
       		seta_distr_info_dg(vl_Datagroup,
		     XDM_TSE_MACH,
		     T_IPC_FT_STR, pa_NomMachine,
		     T_IPC_FT_INT4, va_EtatMachine,
		     NULL);
/*
        XDG_EncodeDG2(vl_Datagroup, XDG_NSUPHIST, SEP_NOM_SITE);
        TipcSrvMsgWrite(vl_Datagroup,
                        XDM_IdentMsg(XDM_TSE_MACH),
                        XDM_FLG_SRVMSGWRITE,
                        T_IPC_FT_STR, pa_NomMachine,
                     T_IPC_FT_INT4, va_EtatMachine,
                         NULL);
        strcpy(vl_Datagroup, XDG_NSUPHIST);
*/
        TipcSrvMsgWrite(vl_Datagroup,
                        XDM_IdentMsg(XDM_TSE_MACH),
                        XDM_FLG_SRVMSGWRITE,
                        T_IPC_FT_STR, pa_NomMachine,
                     T_IPC_FT_INT4, va_EtatMachine,
                         NULL);

#endif
   	}

      	/*A Recuperation de l etat de la machine locale */
      	XZSE_09EtatMachine(&vl_EtatMachineLoc);

      	/*A Recuperation du nom de la machine locale */
      	XZSC_07NomMachine(vl_NomMachineLocale);


/* Modif experimentale **/
#if 1
      	if ( (vl_EtatMachineLoc == XZSEC_ETAT_NACTIF)
      		|| (vl_EtatMachineLoc == XZSEC_ETAT_DEGRADE) ) 
	/*	|| (vl_EtatMachineLoc == XZSEC_ETAT_NPASSIF) ) 	*/
      	{
         if (SEV_TYPE_MACHINE == XZSCC_TYPEM_SD)
         {
	    if (!strcmp(pa_NomMachine,SEP_MACHINE_SECOURS) 
				&& va_EtatMachine == XZSEC_ETAT_NPASSIF)
	    {
	 XZST_03EcritureTrace(XZSTC_WARNING ,
		"seta_etamach     : REDONDANCE des serveurs de Donnees \n");

	       /*B indique la redondance des serveurs de donnees */
	       XZST_10ArchiverMsgSyst(XDL_SUP38_SVDONRD,NULL);
	    }
         }
         else if (SEV_TYPE_MACHINE == XZSCC_TYPEM_SC)
         {
            if (!strcmp(pa_NomMachine,SEP_MACHINE_SECOURS)
                	&& va_EtatMachine == XZSEC_ETAT_NPASSIF)
            {
	       /*B indique la redondance des serveurs de communication */
	       XZST_10ArchiverMsgSyst(XDL_SUP36_SVCOMRD,vl_NomSiteLoc,NULL);
            }
         }


      	/*A Diffusion du nouvel etat de la machine si le site est identique
       	* ou si il s agit d un SD surveillant un SC
       	*/
#if 0
      	if ( (!strcmp(vl_NomSiteLoc , SEP_NOM_SITE) && (SEV_TYPE_MACHINE == XZSAC_TYPEM_SD))
		|| (SEV_TYPE_MACHINE == XZSAC_TYPEM_SC) 
		|| ((vl_TypeMachine == XZSAC_TYPEM_SC) && (SEV_TYPE_MACHINE == XZSAC_TYPEM_SD)) )
#endif
	 XZST_03EcritureTrace(XZSTC_INFO ,
		" Diffusion etat machine Site %s -- site local %s -- type mach %d -- type mach locale %d \n", 
		SEP_NOM_SITE, vl_NomSiteLoc, vl_TypeMachine, SEV_TYPE_MACHINE);

      	if ( (!strcmp(vl_NomSiteLoc , SEP_NOM_SITE) && 
		((SEV_TYPE_MACHINE == XZSAC_TYPEM_SD) || (SEV_TYPE_MACHINE == XZSAC_TYPEM_SC) || (SEV_TYPE_MACHINE == XZSAC_TYPEM_PCS)|| 
		(SEV_TYPE_MACHINE == XZSAC_TYPEM_PC2) || (SEV_TYPE_MACHINE == XZSAC_TYPEM_PC2LT)) )
		|| ((vl_TypeMachine == XZSAC_TYPEM_SC) && (SEV_TYPE_MACHINE == XZSAC_TYPEM_SD)) || 
		   ((vl_TypeMachine == XZSAC_TYPEM_PCS) && (SEV_TYPE_MACHINE == XZSAC_TYPEM_SD))|| 
		   ((vl_TypeMachine == XZSAC_TYPEM_PC2LT) && (SEV_TYPE_MACHINE == XZSAC_TYPEM_SD))|| 
		   ((vl_TypeMachine == XZSAC_TYPEM_PC2) && (SEV_TYPE_MACHINE == XZSAC_TYPEM_SD)) ) 
		/* ajout PCS pour modif 1.21 */
      	{
            	/*B indique l etat de la machine si etat a change  */
            	if ((SEV_TYPE_MACHINE != XZSAC_TYPEM_SD) || (vl_TypeMachine != XZSAC_TYPEM_PCS))
            	   {
            	   /*sprintf(vl_ChaineEtat,"%d",vl_EtatMachine);*/
		   switch (va_EtatMachine)
		   {
			case XZSIC_ETAT_NACTIF:
				sprintf(vl_ChaineEtat,"ACTIF");
				break;
			case XZSIC_ETAT_NPASSIF:
				sprintf(vl_ChaineEtat,"PASSIF");
				break;
			case XZSIC_ETAT_VEILLE:
				sprintf(vl_ChaineEtat,"VEILLE");
				break;
			case XZSIC_ETAT_DEGRADE:
				sprintf(vl_ChaineEtat,"DEGRADE");
				break;
			case XZSEC_SANS_ETAT:
				sprintf(vl_ChaineEtat,"SANS ETAT");
				break;
			default:
				sprintf(vl_ChaineEtat,"%d",vl_EtatMachine);
				break;

		   }
            	   XZST_10ArchiverMsgSyst(XDL_SUP20_MACETAT,
				pa_NomMachine,vl_ChaineEtat,NULL);
		   }

           	if ( (vl_TypeMachine == XZSAC_TYPEM_SD)
			|| (vl_TypeMachine == XZSAC_TYPEM_SC)|| (vl_TypeMachine == XZSAC_TYPEM_PCS)
			|| (vl_TypeMachine == XZSAC_TYPEM_PC2)|| (vl_TypeMachine == XZSAC_TYPEM_PC2LT) ) /* ajout PCS pour modif 1.21 */
            	{
   	       		/*B Diffusion de l etat du serveur de donnees */
	       		XZSI_03DiffEtatServeur(pa_NomMachine, vl_EtatMachine);
            	}
            	else
            	{
	       		/*B Diffusion etat de la station 
			*  ou du serveur de protocole ou du local technique */
	       		XZSI_11DiffEtatStation(pa_NomMachine, vl_EtatMachine);
            	}
         }
     }

#endif

   
	/*A actualisation de l'etat machine en interne (tableau SEP_ETAT_MACH) */
   	vl_Maj_EtatMach = 
		seta_actualiser_etamach(pa_NomMachine,vl_EtatMachine);



	/* RAZ des taches et sous systemes (si le site est identique
    	*      ou si il s agit d un SD surveillant un SC ou PCS)
    	*           (et machine en veille ou sans etat) */ /* ajout PCS pour modif 1.21 */
   	if ( ((!strcmp(vl_NomSiteLoc , SEP_NOM_SITE)) 
				|| (vl_TypeMachine == XZSAC_TYPEM_SC) || (vl_TypeMachine == XZSAC_TYPEM_PCS)
				|| (vl_TypeMachine == XZSAC_TYPEM_PC2) || (vl_TypeMachine == XZSAC_TYPEM_PC2LT)) 
     		&&(va_EtatMachine == XZSEC_SANS_ETAT 
				|| va_EtatMachine == XZSEC_ETAT_VEILLE) )

	{

/* Modif V1.17  Si SC Actif et la machine Locale est un SD   */
		/*B Maj des Etats des Machines a SANS ETAT si c'est un SD 
		*	surveillant un SC Actif */
	 	if ( (((vl_TypeMachine == XZSAC_TYPEM_SC) && (SEV_TYPE_MACHINE == XZSAC_TYPEM_SD))
			&& ( (!strcmp(pa_NomMachine,XDC_ACTIF_DN)) || (!strcmp(pa_NomMachine,XDC_ACTIF_DM)) )) ||
			((vl_TypeMachine == XZSAC_TYPEM_PCS) && (SEV_TYPE_MACHINE == XZSAC_TYPEM_SD))||
			((vl_TypeMachine == XZSAC_TYPEM_PC2) && (SEV_TYPE_MACHINE == XZSAC_TYPEM_SD))||
			((vl_TypeMachine == XZSAC_TYPEM_PC2LT) && (SEV_TYPE_MACHINE == XZSAC_TYPEM_SD)) )		/* ajout PCS pour modif 1.21 */
		{

			
			/* Initialisation des etats des machines du Site */ 
			XZST_03EcritureTrace(XZSTC_INFO,
				"Initialisation des etats du Site %s  car SD surveille SC %s \n", vl_NomSiteLoc, pa_NomMachine);
			XZSA_19ListeMachineSite(vl_NomSiteLoc, &seta_etat_init,&gl_NbElem,XZSEC_SANS_ETAT);

			XZSA_19ListeMachineSite(vl_NomSiteLoc, &seta_mach_init,&gl_NbElem,XZSEC_SANS_ETAT);


#if 0
			vl_SC_ACTIF = XDC_FAUX;
			for (vl_NumMachine = 0; vl_NumMachine < SEV_NB_MACH; vl_NumMachine++)
			 {
				/*A Verifie si au moins un SC est ACTIF */
				if ( (!strcmp(SEP_ETAT_MACH[vl_NumMachine].NomSite, vl_NomSiteLoc)) && ((SEP_ETAT_MACH[vl_NumMachine].TypeMachine == XZSAC_TYPEM_SC)||
				                                                                        (SEP_ETAT_MACH[vl_NumMachine].TypeMachine == XZSAC_TYPEM_PC2)||
				                                                                        (SEP_ETAT_MACH[vl_NumMachine].TypeMachine == XZSAC_TYPEM_PC2LT)) && (strcmp(SEP_ETAT_MACH[vl_NumMachine].NomMachine,pa_NomMachine)) )
				{
					/*B Teste si Actif */
					if ( (SEP_ETAT_MACH[vl_NumMachine].Etat == XZSEC_ETAT_NACTIF) || (SEP_ETAT_MACH[vl_NumMachine].Etat == XZSEC_ETAT_DEGRADE) )
					{
						/*B On arrete le Test , donc sortie Boucle */
			XZST_03EcritureTrace(XZSTC_DEBUG3,
			" Au moins un SC Actif !!!\n");

						vl_SC_ACTIF = XDC_VRAI;
						break;
					}
				}

				/* Initialisation des etats des machines du Site */ 
				XZST_03EcritureTrace(XZSTC_INFO,
					"Initialisation des etats du Site \
					car SD surveille SC \n");
/*				XZSA_19ListeMachineSite(vl_NomSiteLoc,
					&seta_etat_init,&gl_NbElem,XZSEC_SANS_ETAT);*/
			 }

			/*B Recherche des differentes machines 
			*	associes au Site du SC si les 2 SC sont HS*/
			if (vl_SC_ACTIF == XDC_FAUX)
			{
				XZST_03EcritureTrace(XZSTC_DEBUG3,
					"RAZ des machines du Site !! \n");

				/*B Initialisation des etats des machines du Site */ 
				XZST_03EcritureTrace(XZSTC_INFO,
					"Initialisation des etats du Site \
					car les 2 SC HS \n");
				XZSA_19ListeMachineSite(vl_NomSiteLoc,
					&seta_etat_init,&gl_NbElem,XZSEC_SANS_ETAT);

				/*B Initialisation des Etats Machines */
				XZST_03EcritureTrace(XZSTC_INFO,
					"Initialisation des etats des machines\
					du site car les 2 SC HS \n");
				XZSA_19ListeMachineSite(vl_NomSiteLoc,
					&seta_mach_init,&gl_NbElem,XZSEC_SANS_ETAT);

				/*B Signale la perte des 2 serveurs de Com */
				/*XZST_10ArchiverMsgSyst(XDL_SUP34_COMEXHS,SEP_NOM_MACHINE,vl_NomSiteLoc,NULL);*/
			}
#endif 

		}
		/*B Machine du meme SITE */
		else
		{
			/*B Maj des etats SSYS et taches a SANS_ETAT */
			seta_etat_init(pa_NomMachine,0,XZSEC_SANS_ETAT);
		}
         }

}

	 XZST_03EcritureTrace(XZSTC_FONCTION , "seta_etamach     : OUT \n");

}	/* Fin seta_etamach */


/*X*/
/* Fichier : $Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $	$Revision: 1.33 $        Release : $Revision: 1.33 $        Date : $Date: 2020/11/03 17:54:09 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_TRA
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction demande l'historisation de l'etat d'un
* sous-systeme.                                
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_etassys(	
	XZSCT_NomMachine  pa_NomMachine,
        XZSCT_NomSSysteme pa_NomSSysteme,
	int               va_EtatSSysteme,
	int               va_Distribution)

/*
* PARAMETRES EN ENTREE :
* 	pa_NomMachine	: Nom de la machine
*	pa_NomSSysteme	: Nom du Sous-systeme
*	va_EtatSSysteme	: Etat du sous-systeme
*	va_Distribution	: Filag de distribution
*
* PARAMETRES EN SORTIE :
* 	Aucun
*
* VALEUR(S) DE RETOUR :
* 	Aucun
*
* CONDITION D'ERREUR :
* 	Aucune
*
* CONDITIONS LIMITES :
* 	Aucune
*
******************************************************/

{
   	static char *version = "$Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $ : seta_etassys" ;
   	int	vl_EtatMachLoc	= 0;
		/* recupere sous chaine 1 etat numerique */
   	char    vl_ChaineEtat[3]	= "";  
   	XZSCT_NomSite pl_NomSite	= "";
	int 	vl_TypeMachine	= 0;
	int	vl_Maj_EtatSSys	= 0;	

XZSCT_Datagroup vl_Datagroup;


	 XZST_03EcritureTrace(XZSTC_FONCTION ,
	       "seta_etassys : IN  -- Nom Mach %s -- Nom SSYSt %s -- \
	       eta Syst %d -- Distribuion %d \n",pa_NomMachine,pa_NomSSysteme,
	       va_EtatSSysteme,va_Distribution);

   /* actualisation de l'etat du sous-systeme */
   vl_Maj_EtatSSys = 
	seta_actualiser_etassys(pa_NomMachine,pa_NomSSysteme,va_EtatSSysteme);

   /* distribution eventuelle du message */
   if (va_Distribution == XDC_OK)
   {
       /* distribution de l'info aux machines concernees */
#ifndef _TIBCO_EMS
       seta_distr_info(XDM_TSE_SSYS,
		     T_IPC_FT_STR, pa_NomMachine,
		     T_IPC_FT_STR, pa_NomSSysteme,
		     T_IPC_FT_INT4, va_EtatSSysteme,
		     NULL);
#else
       /* distribution de l'info aux machines concernees */
       seta_distr_info_dg(vl_Datagroup,
       		     XDM_TSE_SSYS,
		     T_IPC_FT_STR, pa_NomMachine,
		     T_IPC_FT_STR, pa_NomSSysteme,
		     T_IPC_FT_INT4, va_EtatSSysteme,
		     NULL);
/*        XDG_EncodeDG2(vl_Datagroup, XDG_NSUPHIST, SEP_NOM_SITE);
        TipcSrvMsgWrite(vl_Datagroup,
                        XDM_IdentMsg(XDM_TSE_SSYS),
                        XDM_FLG_SRVMSGWRITE,
		     T_IPC_FT_STR, pa_NomMachine,
		     T_IPC_FT_STR, pa_NomSSysteme,
		     T_IPC_FT_INT4, va_EtatSSysteme,
                         NULL);
        strcpy(vl_Datagroup, XDG_NSUPHIST);
*/
        TipcSrvMsgWrite(vl_Datagroup,
                        XDM_IdentMsg(XDM_TSE_SSYS),
                        XDM_FLG_SRVMSGWRITE,
		     T_IPC_FT_STR, pa_NomMachine,
		     T_IPC_FT_STR, pa_NomSSysteme,
		     T_IPC_FT_INT4, va_EtatSSysteme,
                         NULL);

#endif
   }
		     
   /* recuperation de l'etat de la machine locale */
   XZSE_09EtatMachine(&vl_EtatMachLoc);

#if 1 	/* Modif V1.16 */
   if ( (vl_EtatMachLoc == XZSEC_ETAT_NACTIF)
   	|| (vl_EtatMachLoc == XZSEC_ETAT_DEGRADE) 
   	|| (vl_EtatMachLoc == XZSEC_ETAT_NPASSIF) )
   {
      	/* recuperation du site de la machine */
      	XZSA_23SiteMachine(pa_NomMachine, pl_NomSite);

	/*A recuperation du type de la machine */
	XZSA_17TypeMachine(pa_NomMachine, &vl_TypeMachine);

      /*A si la machine de la tache se trouve sur le meme site 
	*	que la machine locale ou SD surveille un SC ou PCS ou PC2  */		/* ajout PCS modif 1.21 */
	if ( (!strcmp(pl_NomSite , SEP_NOM_SITE) &&
		((SEV_TYPE_MACHINE == XZSAC_TYPEM_SD) || (SEV_TYPE_MACHINE == XZSAC_TYPEM_SC) || (SEV_TYPE_MACHINE == XZSAC_TYPEM_PCS) || (SEV_TYPE_MACHINE == XZSAC_TYPEM_PC2) || (SEV_TYPE_MACHINE == XZSAC_TYPEM_PC2LT)) )
	  || ((vl_TypeMachine == XZSAC_TYPEM_SC) && (SEV_TYPE_MACHINE == XZSAC_TYPEM_SD)) || ((vl_TypeMachine == XZSAC_TYPEM_PCS) && (SEV_TYPE_MACHINE == XZSAC_TYPEM_SD)) || 
	     ((vl_TypeMachine == XZSAC_TYPEM_PC2) && (SEV_TYPE_MACHINE == XZSAC_TYPEM_SD)) || ((vl_TypeMachine == XZSAC_TYPEM_PC2LT) && (SEV_TYPE_MACHINE == XZSAC_TYPEM_SD)) )
#if 0
      if  ((!strcmp(pl_NomSite, SEP_NOM_SITE) && (SEV_TYPE_MACHINE == XZSAC_TYPEM_SD))
		|| (SEV_TYPE_MACHINE == XZSAC_TYPEM_SC))
#endif
      {
	 /*B diffusion de l'etat du sous-systeme a l'IHM */
	 if (vl_Maj_EtatSSys == XDC_VRAI)
	 {
	 	/*B Indique l'etat du sous-systeme de la machine  */
            	if ((SEV_TYPE_MACHINE != XZSAC_TYPEM_SD) || (vl_TypeMachine != XZSAC_TYPEM_PCS) || 
            	    (SEV_TYPE_MACHINE != XZSAC_TYPEM_PC2) || (vl_TypeMachine != XZSAC_TYPEM_PC2LT))
            	   {
         	   /*sprintf(vl_ChaineEtat,"%d",va_EtatSSysteme);*/
		   switch (va_EtatSSysteme)
		   {
			case XZSEC_ETAT_OK:
				sprintf(vl_ChaineEtat,"OK");
				break;
			case XZSEC_ETAT_NOK:
				sprintf(vl_ChaineEtat,"NOK");
				break;
			case XZSEC_SANS_ETAT:
				sprintf(vl_ChaineEtat,"SANS ETAT");
				break;
      			default: 
				sprintf(vl_ChaineEtat,"%d",va_EtatSSysteme);
		   }

         	   XZST_10ArchiverMsgSyst(XDL_SUP21_MACSSYS, 
			pa_NomMachine,pa_NomSSysteme,vl_ChaineEtat,NULL);
		   }

		/*B Diffusion etat du SSystem */
	 	XZSI_01DiffEtatSSysteme(pa_NomMachine,pa_NomSSysteme, va_EtatSSysteme);
	}
      }
   }
#endif

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
	       "seta_etassys : OUT \n");

}	/* Fin seta_etassys */


/*X*/
/* Fichier : $Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $	$Revision: 1.33 $        Release : $Revision: 1.33 $        Date : $Date: 2020/11/03 17:54:09 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_TRA
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction traite l'etat d'une tache.                                
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_etatach(
	XZSCT_NomMachine pa_NomMachine,
	XZSCT_NomTache   pa_NomTache,
	int              va_EtatTache,
	int              va_Distribution,
	int              va_FlagMsgSyst)

/*
* PARAMETRES EN ENTREE :
*	pa_NomMachine	: Nom de la machine
*	pa_NomTache	: Nom de la tache
*	va_EtatTache	: etat de la tache
*	va_Distribution	: Flag de distribution
*	va_FlagMsgSyst	: Flag si creation msg systeme
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
   	static char *version = "$Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $ : seta_etatach" ;
   	XZSCT_NomSite 	pl_NomSite	= "";
		/* recupere sous chaine 1 etat numerique */
   	char            vl_ChaineEtat[3]	= "";  
		/* etat de la machine locale */
   	int 		vl_EtatMachLoc	= 0; 
   	int 		vl_TypeMachine	= 0;
   	XZSCT_NomSite 	vl_NomSite	= "";
	XZSCT_Datagroup pl_NomDatagroup = "";
	int 		vl_Maj_EtatTach	= 0; 
XZSCT_Datagroup vl_Datagroup;




	 XZST_03EcritureTrace(XZSTC_FONCTION ,
	       "seta_etatach : IN -- Nom Mach %s -- Nom tache %s \
	       -- Etat Tache %d -- Distribution %d \n",pa_NomMachine,
	       pa_NomTache, va_EtatTache, va_Distribution);

	 XZST_03EcritureTrace(XZSTC_INFO ,
	       "seta_etatach : IN -- Nom Mach %s -- Nom tache %s \
	       -- Etat Tache %d -- Distribution %d \n",pa_NomMachine,
	       pa_NomTache, va_EtatTache, va_Distribution);


   /* actualisation de l'etat de la tache */
   vl_Maj_EtatTach = seta_actualiser_etatach(pa_NomMachine,pa_NomTache,va_EtatTache);

   /* distribution eventuelle du message */
   if (va_Distribution == XDC_OK)
   {
       /* distribution de l'info aux machines concernees */
#ifndef _TIBCO_EMS
       seta_distr_info(XDM_TSE_TACH,
		     T_IPC_FT_STR, pa_NomMachine,
		     T_IPC_FT_STR, pa_NomTache,
		     T_IPC_FT_INT4, va_EtatTache,
		     NULL);
#else
       seta_distr_info_dg(vl_Datagroup,
       		     XDM_TSE_TACH,
		     T_IPC_FT_STR, pa_NomMachine,
		     T_IPC_FT_STR, pa_NomTache,
		     T_IPC_FT_INT4, va_EtatTache,
		     NULL);
/*
        XDG_EncodeDG2(vl_Datagroup, XDG_NSUPHIST, SEP_NOM_SITE);
        TipcSrvMsgWrite(vl_Datagroup,
                        XDM_IdentMsg(XDM_TSE_TACH),
                        XDM_FLG_SRVMSGWRITE,
		     T_IPC_FT_STR, pa_NomMachine,
		     T_IPC_FT_STR, pa_NomTache,
		     T_IPC_FT_INT4, va_EtatTache,
                         NULL);
        strcpy(vl_Datagroup, XDG_NSUPHIST);
*/
        TipcSrvMsgWrite(vl_Datagroup,
                        XDM_IdentMsg(XDM_TSE_TACH),
                        XDM_FLG_SRVMSGWRITE,
		     T_IPC_FT_STR, pa_NomMachine,
		     T_IPC_FT_STR, pa_NomTache,
		     T_IPC_FT_INT4, va_EtatTache,
                         NULL);

#endif
   }
		     
   /* diffusion des informations de synchronisation 
				a la tache si celle-ci est Ok */
   if (va_EtatTache == XZSEC_ETAT_OK)
   {
      /* envoi des informations de synchronisation */
      seta_informer(pa_NomMachine,pa_NomTache);
   }
   else
   {
      /* mise a jour des informations de trace */
      seta_etatrac(pa_NomMachine, pa_NomTache, 
		XZSTC_FATAL | XZSTC_WARNING| XZSTV_NiveauTrace, 
		XZSTV_VisuTrace	, XDC_OK, XDC_VRAI);
   }

   /* recuperation de l'etat de la machine locale */
   XZSE_09EtatMachine(&vl_EtatMachLoc);

#if 1	/* Modif V1.16*/
   if ( (vl_EtatMachLoc == XZSEC_ETAT_NACTIF)
   	|| (vl_EtatMachLoc == XZSEC_ETAT_DEGRADE) 
   	|| (vl_EtatMachLoc == XZSEC_ETAT_NPASSIF) )
   {
      /* recuperation du site de la machine */
      XZSA_23SiteMachine(pa_NomMachine, pl_NomSite);

	/* recuperation du type de la machine */
	XZSA_17TypeMachine(pa_NomMachine, &vl_TypeMachine);

      /*A si la machine de la tache se trouve sur le meme site qu'un SD 
      *	ou si la machine locale est un SC et dans les deux cas que la 
      *	tache a un etat different que le precedent */
      if ( ((!strcmp(pl_NomSite, SEP_NOM_SITE) && (SEV_TYPE_MACHINE == XZSAC_TYPEM_SD))
		|| (SEV_TYPE_MACHINE == XZSAC_TYPEM_SC) || (SEV_TYPE_MACHINE == XZSAC_TYPEM_PCS)
		|| (SEV_TYPE_MACHINE == XZSAC_TYPEM_PC2) || (SEV_TYPE_MACHINE == XZSAC_TYPEM_PC2LT)) 
				&& vl_Maj_EtatTach == XDC_VRAI)	/* ajout PCS modif 1.21 */
      {
	 /* Indique l etat de la tache de la machine  */
         /*sprintf(vl_ChaineEtat,"%d",va_EtatTache);*/
	switch (va_EtatTache)
	{
	case XZSEC_ETAT_OK:
		sprintf(vl_ChaineEtat,"OK");
		break;
	case XZSEC_ETAT_NOK:
		sprintf(vl_ChaineEtat,"NOK");
		break;
	case XZSEC_SANS_ETAT:
		sprintf(vl_ChaineEtat,"SANS ETAT");
		break;
      	default: 
		sprintf(vl_ChaineEtat,"%d",va_EtatTache);
	}
 
	 /*B diffusion de l'etat de la tache a l'IHM */
	 XZSI_02DiffEtatTache(pa_NomMachine, pa_NomTache, va_EtatTache);

	 /*B Teste si diffusion du Msg Systeme */
	 if (va_FlagMsgSyst == XDC_VRAI)
	 {
        	 XZST_10ArchiverMsgSyst(XDL_SUP22_MACTACE, 
			pa_NomMachine,pa_NomTache,vl_ChaineEtat,NULL);
	}
      }
   }
#endif

	/*A Envoie de l'etat de la Tache pour memorisation
	*	dans la omBD afin d'effectuer les calculs d'indice Qualite 
	*	si SD si la mise a jour a etet effectuee  */
	if (vl_Maj_EtatTach == XDC_VRAI)
	{

	if ( (SEV_TYPE_MACHINE == XZSAC_TYPEM_SD) &&
		(vl_EtatMachLoc != XZSEC_ETAT_NPASSIF) )
	{	

	XZST_03EcritureTrace(XZSTC_MESSAGE,"Envoie XDM_TSE_INDQUA - \
		Nom Site %s -- Nom Machine %s -- Nom Tache %s \
		etat tache %d \n",pl_NomSite, pa_NomMachine,
		pa_NomTache,va_EtatTache);


		/*A envoi du message de synchro XDM_TSE_INDQUA */
   		/*B creation du nom du datagroup XDG_NSUPINDQUA_NomSite */
		XZSC_04NomSite(vl_NomSite);
   		XDG_EncodeDG2(pl_NomDatagroup, XDG_NSUPINDQUA, vl_NomSite);
		if (!TipcSrvMsgWrite(pl_NomDatagroup,
	  			XDM_IdentMsg(XDM_TSE_INDQUA),
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_STR, pl_NomSite,
				T_IPC_FT_STR, pa_NomMachine,
				T_IPC_FT_STR, pa_NomTache,
	       			T_IPC_FT_INT4, va_EtatTache, NULL))
		{
	      		XZST_03EcritureTrace(XZSTC_WARNING, 
				"Erreur envoi   XDM_TSE_INDQUA \
				Nom Site %s -- Nom Mach %s -- Nom Tache %s \
				Etat Tache %d \n",
				pl_NomSite, pa_NomMachine, 
				pa_NomTache, va_EtatTache);
		}
		TipcSrvFlush();
	  }
	}	

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
	       "seta_etatach : OUT \n");

}	/* Fin seta_etatach */


/*X*/
/* Fichier : $Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $	$Revision: 1.33 $        Release : $Revision: 1.33 $        Date : $Date: 2020/11/03 17:54:09 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_TRA
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction demande l historisation du mode 
* d une machine
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_etamode(
	XZSCT_NomMachine pa_NomMachine,
	int              va_ModeIns,
	int              va_ModeRec,
	int 		 va_Distribution)

/*
* PARAMETRES EN ENTREE :
*	pa_NomMachine	: Nom de la machine
*	va_ModeIns	: Etat du mode instruction
*	va_ModeRec	: Etat du mode normal
*	va_Distribution	: Flag de distribution
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
   	static char *version = "$Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $ : seta_etamode" ;
   int    vl_EtatMachLoc	= 0;
   /* recupere sous chaine 1 modeIns numerique */
   char             vl_ChaineIns[3]	= "";  
   /* recupere sous chaine 1 ModeRec numerique */
   char             vl_ChaineRec[3]	= "";  
   XZSCT_NomSite 	pl_NomSite	= "";
   int 			vl_Maj_EtatMode	= 0;
XZSCT_Datagroup vl_Datagroup;


	 XZST_03EcritureTrace(XZSTC_FONCTION ,
	       "seta_etamode : IN -- Nom mach %s -- Mode Ins %d --\
	       -- Mode Reconst %d -- Distribution %d \n",
	       pa_NomMachine,va_ModeIns,va_ModeRec,va_Distribution);

   /*A Actualisation du mode */
   vl_Maj_EtatMode = seta_actualiser_etamode(pa_NomMachine,va_ModeIns,va_ModeRec);

   /* distribution eventuelle du message */
   if (va_Distribution == XDC_OK)
   {
#ifndef _TIBCO_EMS
       /* distribution de l'info aux machines concernees */
       seta_distr_info(XDM_TSE_MODE,
		     T_IPC_FT_STR, pa_NomMachine,
		     T_IPC_FT_INT4, va_ModeIns,
		     T_IPC_FT_INT4, va_ModeRec,
		     NULL);
#else
       seta_distr_info_dg(vl_Datagroup,
       		     XDM_TSE_MODE,
		     T_IPC_FT_STR, pa_NomMachine,
		     T_IPC_FT_INT4, va_ModeIns,
		     T_IPC_FT_INT4, va_ModeRec,
		     NULL);
/*        XDG_EncodeDG2(vl_Datagroup, XDG_NSUPHIST, SEP_NOM_SITE);
        TipcSrvMsgWrite(vl_Datagroup,
                        XDM_IdentMsg(XDM_TSE_MODE),
                        XDM_FLG_SRVMSGWRITE,
		     T_IPC_FT_STR, pa_NomMachine,
		     T_IPC_FT_INT4, va_ModeIns,
		     T_IPC_FT_INT4, va_ModeRec,
                         NULL);
        strcpy(vl_Datagroup, XDG_NSUPHIST);
*/
        TipcSrvMsgWrite(vl_Datagroup,
                        XDM_IdentMsg(XDM_TSE_MODE),
                        XDM_FLG_SRVMSGWRITE,
		     T_IPC_FT_STR, pa_NomMachine,
		     T_IPC_FT_INT4, va_ModeIns,
		     T_IPC_FT_INT4, va_ModeRec,
                         NULL);

#endif 
   }
		     
   /* recuperation de l'etat de la machine locale */
   XZSE_09EtatMachine(&vl_EtatMachLoc);

   if ( (vl_EtatMachLoc == XZSEC_ETAT_NACTIF)
   	|| (vl_EtatMachLoc == XZSEC_ETAT_DEGRADE) )
   {
      /* recuperation du site de la machine */
      XZSA_23SiteMachine(pa_NomMachine, pl_NomSite);

      /* si la machine de la tache se trouve sur le 
      *			meme site que la machine locale */
      if ( (!strcmp(pl_NomSite, SEP_NOM_SITE) && (SEV_TYPE_MACHINE == XZSAC_TYPEM_SD))
		|| (SEV_TYPE_MACHINE == XZSAC_TYPEM_SC) || (SEV_TYPE_MACHINE == XZSAC_TYPEM_PCS ) 
		|| (SEV_TYPE_MACHINE == XZSAC_TYPEM_PC2) || (SEV_TYPE_MACHINE == XZSAC_TYPEM_PC2LT) ) /*  ajout PCS modif 1.21 */
      {
	 /*B Diffusion du mode de la station */
	 if (vl_Maj_EtatMode == XDC_VRAI)
	 {
	 	/*B Indique le mode de la station  */
	 	sprintf(vl_ChaineIns,"%d",va_ModeIns);
         	sprintf(vl_ChaineRec,"%d",va_ModeIns);
         	XZST_10ArchiverMsgSyst(XDL_SUP26_MACMODE, pa_NomMachine,
					vl_ChaineIns, vl_ChaineRec,NULL);

		/*B Diifusion au Synoptique */
	 	XZSI_10DiffModeStation(pa_NomMachine,va_ModeIns);
	 }
      }
   }

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
	       "seta_etamode : OUT \n");

}	/* Fin seta_etamode */


/*X*/
/* Fichier : $Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $	$Revision: 1.33 $        Release : $Revision: 1.33 $        Date : $Date: 2020/11/03 17:54:09 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_TRA
******************************************************
* DESCRIPTION DE LA FONCTION :
*	Memorisation de l'etat des traces d'une tache
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_etatrac(	
	XZSCT_NomMachine pa_NomMachine,
	XZSCT_NomTache   pa_NomTache,
	int              va_NivTrace,
	int              va_VisuTrace,
	int		 va_Distribution,
	int              va_FlagMsgSyst)

/*
* PARAMETRES EN ENTREE :
*	pa_NomMachine	: Nom de la machine
*	pa_NomTache	: Nom de la tache
*	va_NivTrace	: Niveau de traces
*	va_VisuTrace	: Visualisation des traces sur ECRAN ou pas
*	va_Distribution : Flag de distribution
*	va_FlagMsgSyst	: Flag si affichage msg systeme
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
   	static char *version = "$Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $ : seta_etatrac" ;

   	/* Etat de la machine locale */
   int           vl_EtatMachLoc		= 0;    
	/* recupere sous chaine 1 etat numerique */
   char             vl_ChaineNiv[255]	= "(Wa - Fa "; 
   char             vl_ChaineAffich[255]	= "( Wa - Fa "; 
   	/* recupere sous chaine 1 etat numerique */
   char             vl_ChaineVisu[10]	= "";  
   XZSCT_NomSite pl_NomSite		= "";
	char vl_Libelle_Fonction[3] 	= "Fo";
	char vl_Libelle_Interface[3] 	= "In";
	char vl_Libelle_Regle[3] 	= "Re";
	char vl_Libelle_Message[3] 	= "Me";
	char vl_Libelle_Info[3] 	= "In";
	char vl_Libelle_Debug1[3] 	= "D1";
	char vl_Libelle_Debug2[3] 	= "D2";
	char vl_Libelle_Debug3[3] 	= "D3";
	char vl_Libelle_Sgbd[3] 	= "Ba";
	char vl_ChaineFin[2] 	= ")";
	int	vl_Maj_EtatTra	= 0;
XZSCT_Datagroup vl_Datagroup;


	 XZST_03EcritureTrace(XZSTC_FONCTION ,
	       "seta_etatrace : IN  -- Nom Mach %s -- Nom tache %s -- \
	       Niv trace %x -- Visu trace %d -- Distribu %d \n",
	       pa_NomMachine, pa_NomTache,va_NivTrace,
	       va_VisuTrace,va_Distribution);

	/*A Actualiser l'etat des traces */
   	vl_Maj_EtatTra = seta_actualiser_etatrac(pa_NomMachine,pa_NomTache,
					va_NivTrace,va_VisuTrace);

   /* distribution eventuelle du message */
   if (va_Distribution == XDC_OK)
   {
       /* distribution de l'info aux machines concernees */
#ifndef _TIBCO_EMS
       seta_distr_info(XDM_TST_TRAC,
		     T_IPC_FT_STR, pa_NomMachine,
		     T_IPC_FT_STR, pa_NomTache,
		     T_IPC_FT_INT4,va_NivTrace,
		     T_IPC_FT_INT4,va_VisuTrace,
		     NULL);
#else
		XDG_EncodeDG2(vl_Datagroup, XDG_NSUPHIST, SEP_NOM_SITE);
       seta_distr_info_dg(vl_Datagroup,
       		     XDM_TST_TRAC,
		     T_IPC_FT_STR, pa_NomMachine,
		     T_IPC_FT_STR, pa_NomTache,
		     T_IPC_FT_INT4,va_NivTrace,
		     T_IPC_FT_INT4,va_VisuTrace,
		     NULL);
/*        XDG_EncodeDG2(vl_Datagroup, XDG_NSUPHIST, SEP_NOM_SITE);
        TipcSrvMsgWrite(vl_Datagroup,
                        XDM_IdentMsg(XDM_TST_TRAC),
                        XDM_FLG_SRVMSGWRITE,
		     T_IPC_FT_STR, pa_NomMachine,
		     T_IPC_FT_STR, pa_NomTache,
		     T_IPC_FT_INT4,va_NivTrace,
		     T_IPC_FT_INT4,va_VisuTrace,
                         NULL);
        strcpy(vl_Datagroup, XDG_NSUPHIST);
*/
        TipcSrvMsgWrite(vl_Datagroup,
                        XDM_IdentMsg(XDM_TST_TRAC),
                        XDM_FLG_SRVMSGWRITE,
		     T_IPC_FT_STR, pa_NomMachine,
		     T_IPC_FT_STR, pa_NomTache,
		     T_IPC_FT_INT4,va_NivTrace,
		     T_IPC_FT_INT4,va_VisuTrace,
                         NULL);
#endif
}
		     
   /* recuperation de l'etat de la machine locale */
   XZSE_09EtatMachine(&vl_EtatMachLoc);

   if ( (vl_EtatMachLoc == XZSEC_ETAT_NACTIF)
   	|| (vl_EtatMachLoc == XZSEC_ETAT_DEGRADE) 
   	|| (vl_EtatMachLoc == XZSEC_ETAT_NPASSIF) )
   {
      /* recuperation du site de la machine */
      XZSA_23SiteMachine(pa_NomMachine, pl_NomSite);

      /* si la machine de la tache se trouve sur le meme site que la machine locale */
      if ( (!strcmp(pl_NomSite, SEP_NOM_SITE) && (SEV_TYPE_MACHINE == XZSAC_TYPEM_SD))
		|| (SEV_TYPE_MACHINE == XZSAC_TYPEM_SC) || (SEV_TYPE_MACHINE == XZSAC_TYPEM_PCS)
		|| (SEV_TYPE_MACHINE == XZSAC_TYPEM_PC2) || (SEV_TYPE_MACHINE == XZSAC_TYPEM_PC2LT) )	/* ajout PCS modif 1.21 */
      {
	/*A Recherche du Niveau de Trace*/
	if (va_VisuTrace == XZSTC_ECRAN)
	{
		/*B Affichage Ecran */
		strncpy(vl_ChaineVisu,"ECRAN )",strlen("ECRAN )"));
	}
	else
	{
		/*B Pas Affichage Ecran */
		strncpy(vl_ChaineVisu,"NECRAN )",strlen("NECRAN )"));
	}

	/*A Recherche des Niveaux de Traces */
	if ( (va_NivTrace & XZSTC_FONCTION) >0 )
	{
		strcat (vl_ChaineNiv," - ");
		strcat( vl_ChaineNiv, vl_Libelle_Fonction);
	}
	if ( (va_NivTrace & XZSTC_MESSAGE) >0 )
	{
		strcat (vl_ChaineNiv," - ");
		strcat( vl_ChaineNiv, vl_Libelle_Message);
	}
	if ( (va_NivTrace & XZSTC_INFO) >0)
	{
		strcat (vl_ChaineNiv," - ");
		strcat( vl_ChaineNiv, vl_Libelle_Info);
	}
	if ( (va_NivTrace & XZSTC_SGBD) >0)
	{
		strcat (vl_ChaineNiv," - ");
		strcat( vl_ChaineNiv, vl_Libelle_Sgbd);
	}
	if ( (va_NivTrace & XZSTC_DEBUG1) >0)
	{
		strcat (vl_ChaineNiv," - ");
		strcat( vl_ChaineNiv, vl_Libelle_Debug1);
	}
	if ( (va_NivTrace & XZSTC_DEBUG2) >0)
	{
		strcat (vl_ChaineNiv," - ");
		strcat( vl_ChaineNiv, vl_Libelle_Debug2);
	}
	if (( va_NivTrace & XZSTC_DEBUG3) >0)
	{
		strcat (vl_ChaineNiv," - ");
		strcat( vl_ChaineNiv, vl_Libelle_Debug3);
	}
	if ( (va_NivTrace & XZSTC_INTERFACE) >0 )
	{
		strcat (vl_ChaineNiv," - ");
		strcat( vl_ChaineNiv, vl_Libelle_Interface);
	}
	if ( (va_NivTrace & XZSTC_REGLE)> 0)
	{
		strcat (vl_ChaineNiv," - ");
		strcat( vl_ChaineNiv, vl_Libelle_Regle);
	}

	strcat (vl_ChaineNiv,vl_ChaineFin);

        
	 /*A Diffusion niveau de trace de la tache si 
	 *	 la Maj a etet effectuee */
	 if (vl_Maj_EtatTra == XDC_VRAI)
	 {
		/*B Envoi de maj des traces aux synoptiues */
	 	XZSI_07DiffNivVisuTrace(va_NivTrace,va_VisuTrace,
					pa_NomTache,pa_NomMachine);

		/*B Teste si diffusion du Msg Systeme */
		if (va_FlagMsgSyst == XDC_VRAI)
		{
			XZST_10ArchiverMsgSyst(XDL_SUP27_MACVISU, pa_NomMachine,
			pa_NomTache,vl_ChaineNiv,vl_ChaineVisu,NULL);
		}
	 }
      }
   }

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
	       "seta_etatrace : OUT \n");

}	/* Fin  seta_etatrac */


/*X*/
/* Fichier : $Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $	$Revision: 1.33 $        Release : $Revision: 1.33 $        Date : $Date: 2020/11/03 17:54:09 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_TRA
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction reinitialise les entites logicielles
* d une machine
*
******************************************************
* SEQUENCE D'APPEL :
*/

int seta_etat_init(
	XZSCT_NomMachine pa_NomMachine,
	int              va_NumOrdre,
	int		va_Etat)
            
/*
* PARAMETRES EN ENTREE :
* pa_NomMachine  : nom de la machine
* va_NumOrdre    : Numero d'ordre
* va_Etat	: Etat de la machine	
*
*
* PARAMETRES EN SORTIE :
*	Aucun
*
* VALEUR(S) DE RETOUR : 
*   Aucune
*
* CONDITIONS LIMITES :
*
* Appelee par une fonction d'interface XZSA
*
******************************************************/
{
   	static char *version = "$Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $ : seta_etat_init" ;
   	int vl_IndiceSSys	= 0;
   	int vl_IndiceTach	= 0;
   	int vl_ValRet		= XDC_OK;

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
	       "seta_etat_init : IN \n");

   /* RAZ des modes de la machine */
   seta_etamode(pa_NomMachine,XZSEC_MODE_NOR, XZSEC_MODE_NOR, XDC_OK);

   /* RAZ de l'etat des sous-systemes de la machine */
   for (vl_IndiceSSys = 0 ; vl_IndiceSSys < SEV_NB_SSYS ; vl_IndiceSSys ++)
   {
      if (!strcmp(SEP_ETAT_SSYS[vl_IndiceSSys].NomMachine, pa_NomMachine))
      {
	 seta_etassys(pa_NomMachine , SEP_ETAT_SSYS[vl_IndiceSSys].NomSSysteme 
						,XZSEC_SANS_ETAT, XDC_OK);
      }
   }

   /* RAZ de l'etat des taches et de leur niveau de trace de la machine */
   for (vl_IndiceTach = 0 ; vl_IndiceTach < SEV_NB_TACH ; vl_IndiceTach ++)
   {
      if (!strcmp(SEP_ETAT_TACH[vl_IndiceTach].NomMachine, pa_NomMachine))
      {
	 seta_etatach(pa_NomMachine ,SEP_ETAT_TACH[vl_IndiceTach].NomTache ,
					XZSEC_SANS_ETAT,XDC_OK, XDC_FAUX);
         seta_etatrac(pa_NomMachine, SEP_ETAT_TACH[vl_IndiceTach].NomTache, 
		XZSTC_FATAL | XZSTC_WARNING| XZSTV_NiveauTrace, 
		XZSTV_VisuTrace	, XDC_OK, XDC_FAUX);
         vl_ValRet = XDC_NOK;
      }
   }

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
	       "seta_etat_init : OUT  \n");

   return (vl_ValRet);
}	/* Fin seta_etat_init */



/*X*/
/* Fichier : $Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $	$Revision: 1.33 $        Release : $Revision: 1.33 $        Date : $Date: 2020/11/03 17:54:09 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_TRA
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction recherche l etat precedent d une 
* machine
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_etat_prec(
	XZSCT_NomMachine pa_NomMachine,
	int             *pa_EtatMachine)
            
/*
* PARAMETRES EN ENTREE :
* pa_NomMachine  : nom de la machine 
*
*
* PARAMETRES EN SORTIE :
* pa_EtatMachine : Etat de la machine
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
   	static char *version = "$Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $ : seta_etat_prec" ;
   	int vl_IndiceMachine	= 0;  /* Indice de la machine ds le tab SEP_ETAT_MACH */

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
	       "seta_etat_prec : IN  -- Nom Mach %s \n", pa_NomMachine);

   seta_rechmach(SEP_ETAT_MACH,SEV_NB_MACH,pa_NomMachine,&vl_IndiceMachine);

   if (vl_IndiceMachine != -1)
   {
      *pa_EtatMachine = SEP_ETAT_MACH[vl_IndiceMachine].Etat;
   }
   else
   {
      XZST_03EcritureTrace(XZSTC_WARNING,
			  "seta_etat_prec : erreur machine %s introuvable",
			   pa_NomMachine);  
   }

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
	       "seta_etat_prec : OUT   -- Etat Mach %d \n",*pa_EtatMachine);

}	/* Fin seta_etat_prec */


/*X*/
/* Fichier : $Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $	$Revision: 1.33 $        Release : $Revision: 1.33 $        Date : $Date: 2020/11/03 17:54:09 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_TRA
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction recherche l etat precedent d une 
* entite annexe
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_etat_prec_enti(
	XZSCT_NomEntite pa_NomEntite,
	int             va_TypeEntite,
	int            *pa_EtatEntite)
            
/*
* PARAMETRES EN ENTREE :
* pa_NomEntite  : nom de la Entite 
*
*
* PARAMETRES EN SORTIE :
* pa_EtatEntite : Etat de la Entite
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
   	static char *version = "$Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $ : seta_etat_prec_enti" ;
   	int vl_IndiceEntite	= 0;  /* Indice de la Entite ds le tab SEP_ETAT_MACH */

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
	       "seta_etat_prec_enti : IN    -- Nom entite %s \
	       Type entite %d \n",pa_NomEntite,va_TypeEntite);

	/*A Rechereche de l'entite */
   	seta_rechenti(SEP_ETAT_ENTI,SEV_NB_ENTI,pa_NomEntite,
				va_TypeEntite,&vl_IndiceEntite);

   if (vl_IndiceEntite != -1)
   {
      *pa_EtatEntite = SEP_ETAT_MACH[vl_IndiceEntite].Etat;
   }
   else
   {
	/*A Affectation de l'entite a SANS_ETAT */
      	*pa_EtatEntite = XZSEC_SANS_ETAT;

      	XZST_03EcritureTrace(XZSTC_WARNING,
			 "seta_etat_prec_enti : erreur Entite %s introuvable",
			   pa_NomEntite);  
   }

	 XZST_03EcritureTrace(XZSTC_FONCTION , "seta_etat_prec_enti : OUT \
			-- etat entite %d\n",*pa_EtatEntite);

}	/* Fin seta_etat_prec_enti */



/*X*/
/* Fichier : $Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $	$Revision: 1.33 $        Release : $Revision: 1.33 $        Date : $Date: 2020/11/03 17:54:09 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_TRA
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction diffuse les informations a une tache
* les informations necessaires a son lancement.
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_informer(
	XZSCT_NomMachine pa_NomMachine,
        XZSCT_NomTache   pa_NomTache)

/*
* PARAMETRES EN ENTREE :
* pa_NomMachine : nom de la machine de la tache
* pa_NomTache   : nom de la tache
*
*
* PARAMETRES EN SORTIE :
*	Aucun
*
* VALEUR(S) DE RETOUR : 
	Aucune
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
   	static char *version = "$Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $ : seta_informer" ;
   	XZSCT_NomSite pl_NomSite	= "";
   	int vl_TypeMachine	= 0;
   	int vl_EtatMachineLoc	= 0;    /* Etat machine locale */

   XZSCT_NomMachine pl_NomMachine
		   ,pl_NomSCActif   /* Serveur Comm actif */
		   ,pl_NomSDActif;  /* Serveur Data actif */

   XZSCT_NomTache pl_NomTache;
   XZSCT_Datagroup pl_NomDatagroup;
   int vl_IndiceMachine;

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
       		"seta_informer : IN  -- Nom machi %s -- Nom tache %s\n",
		pa_NomMachine,pa_NomTache);

   /* recuperation des infos relatives a la tache */
   XZSA_23SiteMachine(pa_NomMachine, pl_NomSite);
   XZSA_17TypeMachine(pa_NomMachine, &vl_TypeMachine);

   /* recuperation de l'etat de la machine locale */
   XZSE_09EtatMachine(&vl_EtatMachineLoc);

   /* creation du nom du datagroup XDG_NomMachine_NomTache */
   XDG_EncodeDG2(pl_NomDatagroup, pa_NomMachine, pa_NomTache);

   /* si le site de la tache est identique au site de la machine
      locale */
   if (!strcmp(SEP_NOM_SITE, pl_NomSite))
   {
        /* recherche des modes de la machine */
        seta_rechmach(SEP_ETAT_MACH, SEV_NB_MACH, 
				pa_NomMachine, &vl_IndiceMachine);

        if (vl_IndiceMachine != -1)
        {
		XZST_03EcritureTrace(XZSTC_MESSAGE, 
			"Envoie du Msg XDM_TSE_MAJM \
			-- Mode Ins %d -- Mode Rec %d  \n",
			SEP_ETAT_MACH[vl_IndiceMachine].EtatModeIns,
			SEP_ETAT_MACH[vl_IndiceMachine].EtatModeRec);

	   /* envoi du message de synchro XDM_TSE_MAJM */
	   if (!TipcSrvMsgWrite(pl_NomDatagroup,
	  	XDM_IdentMsg(XDM_TSE_MAJM),
		XDM_FLG_SRVMSGWRITE,
		T_IPC_FT_INT4, SEP_ETAT_MACH[vl_IndiceMachine].EtatModeIns,
	       	T_IPC_FT_INT4, SEP_ETAT_MACH[vl_IndiceMachine].EtatModeRec,
	       	NULL))
	   {
	      XZST_03EcritureTrace(XZSTC_WARNING,
				"seta_informer: erreur TipcSrvMsgWrite XDM_TSE_MAJM EtatModeIns %d, EtatModeRec %d" ,
				SEP_ETAT_MACH[vl_IndiceMachine].EtatModeIns
				,SEP_ETAT_MACH[vl_IndiceMachine].EtatModeRec);
	   }
	   TipcSrvFlush();
        }
        else
        {
	   XZST_03EcritureTrace(XZSTC_WARNING,
		       "seta_informer: erreur machine a informer inexistante");
        }

        /* recuperation du nom du serveur de communication actif */
        XZSE_18NomSCActif(pl_NomSCActif);

		XZST_03EcritureTrace(XZSTC_MESSAGE, 
			"Envoie du Msg XDM_TSE_MAJC \
			-- SCActif   %s \n",pl_NomSCActif);

        /* envoi du message de synchro XDM_TSE_MAJC */
        if (!TipcSrvMsgWrite(pl_NomDatagroup,
        	            XDM_IdentMsg(XDM_TSE_MAJC),
		            XDM_FLG_SRVMSGWRITE,
		            T_IPC_FT_STR, pl_NomSCActif,
		            NULL))
        {
           XZST_03EcritureTrace(XZSTC_WARNING, "seta_informer: \
		erreur TipcSrvMsgWrite XDM_TSE_MAJC: Serveur Comm Actif %s"
			       ,pl_NomSCActif);
        }
        TipcSrvFlush();

        if ( (vl_TypeMachine == XZSAC_TYPEM_ST)
		|| (vl_TypeMachine == XZSAC_TYPEM_STA)
		|| (vl_TypeMachine == XZSAC_TYPEM_LT)
		|| (vl_TypeMachine == XZSAC_TYPEM_HIS)
		|| (vl_TypeMachine == XZSAC_TYPEM_SP) )
        {
		XZST_03EcritureTrace(XZSTC_MESSAGE, 
			"Envoie 1 du Msg XDM_TSE_MAJE \
			-- DataGroup %s  -- EtatMachine  %d \n",
			pl_NomDatagroup,XZSEC_ETAT_NACTIF);

           /* envoi du message de synchro XDM_TSE_MAJE */
           if (!TipcSrvMsgWrite(pl_NomDatagroup,
	                   XDM_IdentMsg(XDM_TSE_MAJE),
	  	           XDM_FLG_SRVMSGWRITE,
                           T_IPC_FT_INT4, XZSEC_ETAT_NACTIF,
		           NULL))
           {
              XZST_03EcritureTrace(XZSTC_WARNING,
                      "seta_informer: erreur TipcSrvMsgWrite XDM_TSE_MAJE ");
           }
           TipcSrvFlush();

        }

        /* recuperation du serveur de donnee actif */
	
        XZSE_19NomSDActif(pl_NomSDActif);

		XZST_03EcritureTrace(XZSTC_MESSAGE, 
			"Envoie du Msg XDM_TSE_MAJD \
			-- SDActif %s \n",pl_NomSDActif);

        /* envoi du message de synchro XDM_TSE_MAJD */
        if (!TipcSrvMsgWrite(pl_NomDatagroup,
        	        XDM_IdentMsg(XDM_TSE_MAJD),
		        XDM_FLG_SRVMSGWRITE,
		        T_IPC_FT_STR, pl_NomSDActif,
		        NULL))
        {
           XZST_03EcritureTrace(XZSTC_WARNING, "seta_informer: \
		erreur TipcSrvMsgWrite XDM_TSE_MAJD Serveur Donnees Actif %s"
				,pl_NomSDActif);
        }
        TipcSrvFlush();
  }

  if (!strcmp(pa_NomMachine,SEP_NOM_MACHINE))
  {
		XZST_03EcritureTrace(XZSTC_MESSAGE, 
			"Envoie 2 du Msg XDM_TSE_MAJE \
			-- DataGroup %s  -- EtatMachine  %d \n",
			pl_NomDatagroup,vl_EtatMachineLoc);

      /* envoi du message de synchro XDM_TSE_MAJE */
      if (!TipcSrvMsgWrite(pl_NomDatagroup,
        	      XDM_IdentMsg(XDM_TSE_MAJE),
		      XDM_FLG_SRVMSGWRITE,
		      T_IPC_FT_INT4, vl_EtatMachineLoc,
		      NULL))
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "seta_informer: \
		erreur TipcSrvMsgWrite XDM_TSE_MAJE Etat machine locale %d"
			      ,vl_EtatMachineLoc);
      }
      TipcSrvFlush();

   }

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
       		"seta_informer : OUT \n");

}	/* Fin seta_informer */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de determiner le Datagroup pour l'envoi des donnees
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void seta_distr_info_dg(XZSCT_Datagroup va_Datagroup,int va_TypeMessage, ...)

/*
* ARGUMENTS EN ENTREE :
*   Aucun
*
* ARGUMENTS EN SORTIE :
*   Le datagroup en fonction de la machine
*
*
* CODE RETOUR : 
*   XDC_OK 	: ok
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Determine le datagroup en fonction de la machine�
*
------------------------------------------------------*/
{
	static char *version="$Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $ : seta_distr_info " ;
   	va_list 	pl_ListeDonnees;
   	char 		*pl_NomMachine		= NULL;
   	int 		vl_TypeMachine		= 0;
   	T_IPC_FT 	pl_TypeDonnees		= 0;
   	XZSCT_Datagroup vl_Datagroup		= "";
   	int 		vl_EtatMachineLoc	= 0;

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
       		"seta_distr_info : IN -- Type Msg %d \n",
			va_TypeMessage);

   /* recuperation des donnees variables */
   va_start(pl_ListeDonnees, va_TypeMessage);

   /* recuperation du nom de la machine a laquelle 
   *			appartient l'element en question */
   pl_TypeDonnees = va_arg(pl_ListeDonnees, T_IPC_FT);
   pl_NomMachine = va_arg(pl_ListeDonnees, char *);

   /* s'il s'agit d'un etat d'un element appartenant a la machine locale */
   if(!strcmp(pl_NomMachine, SEP_NOM_MACHINE))
   {
      if ( (SEV_TYPE_MACHINE == XZSCC_TYPEM_ST)
	 ||  (SEV_TYPE_MACHINE == XZSCC_TYPEM_STA)
	 ||  (SEV_TYPE_MACHINE == XZSCC_TYPEM_HIS)
	 || (SEV_TYPE_MACHINE == XZSCC_TYPEM_LT)
	 || (SEV_TYPE_MACHINE == XZSCC_TYPEM_SP) 
	 || (!strcmp(pl_NomMachine,XDC_PASSIF_CI))
	 )
      {
         /* dans le cas d'une station l'etat 
			est diffuse aux serveurs du site */
         XDG_EncodeDG2(vl_Datagroup, XDG_NSUPHIST, SEP_NOM_SITE);
      }
   }

   /* recuperation de l'etat de la machine locale */
   XZSE_09EtatMachine(&vl_EtatMachineLoc);

#if 1	/* Modif V1.16 */
   /* dans tous les cas si la machine locale est un SC nominal actif */
/*   if ( ((vl_EtatMachineLoc == XZSEC_ETAT_NACTIF) ||
	(vl_EtatMachineLoc == XZSEC_SANS_ETAT) || 
	(vl_EtatMachineLoc == XZSEC_ETAT_DEGRADE) )*/
	if ( (SEV_TYPE_MACHINE == XZSCC_TYPEM_SC) || (SEV_TYPE_MACHINE == XZSCC_TYPEM_PCS) || 
	     (SEV_TYPE_MACHINE == XZSCC_TYPEM_PC2) || (SEV_TYPE_MACHINE == XZSCC_TYPEM_PC2LT)) /* ajout PCS modif 1.21 */
   	{
		if (vl_EtatMachineLoc != XZSEC_ETAT_NPASSIF) 	
		{
      			/*A envoi de l'etat a la machine au serveur de donnees 
      			*	uniquement si l'etat n'est pas celui d'un des SD */
      			if (XZSA_17TypeMachine(pl_NomMachine, &vl_TypeMachine) == XDC_OK)
      			{
	 			if (vl_TypeMachine != XZSCC_TYPEM_SD)
	 			{
					/*B Construction du Datagroup */
            				strcpy(vl_Datagroup, XDG_NSUPHIST);
         			}
      			}
		}
		else 
		{
         		/* dans le cas d'une station l'etat 
				est diffuse aux serveurs du site */
         		XDG_EncodeDG2(vl_Datagroup, XDG_NSUPHIST, SEP_NOM_SITE);
		}	
   	}
#endif

   /* retourne le datagroup */
   strcpy( va_Datagroup, vl_Datagroup );

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
       		"seta_distr_info_dg : OUT \n");

}	/* Fin seta_distr_info_dg */

#ifndef _TIBCO_EMS

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet d'envoyer le message sur le Datagroup avec les
*	arguments passes en parametres 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void seta_distr_info(int va_TypeMessage, ...)

/*
* ARGUMENTS EN ENTREE :
*   Argumets variables compremant le type de message, le datagroup
*	et les divers arguments du message 
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   XDC_OK 	: ok
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Recupere les parametres, puis construit les elts du message
*
------------------------------------------------------*/
{
	static char *version="$Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $ : seta_distr_info " ;
   	va_list 	pl_ListeDonnees;
   	char 		*pl_NomMachine		= NULL;
   	int 		vl_TypeMachine		= 0;
   	T_IPC_FT 	pl_TypeDonnees		= 0;
   	XZSCT_Datagroup vl_Datagroup		= "";
   	int 		vl_EtatMachineLoc	= 0;

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
       		"seta_distr_info : IN -- Type Msg %d \n",
			va_TypeMessage);

   /* recuperation des donnees variables */
   va_start(pl_ListeDonnees, va_TypeMessage);

   /* recuperation du nom de la machine a laquelle 
   *			appartient l'element en question */
   pl_TypeDonnees = va_arg(pl_ListeDonnees, T_IPC_FT);
   pl_NomMachine = va_arg(pl_ListeDonnees, char *);

   /* s'il s'agit d'un etat d'un element appartenant a la machine locale */
   if(!strcmp(pl_NomMachine, SEP_NOM_MACHINE))
   {
      if ( (SEV_TYPE_MACHINE == XZSCC_TYPEM_ST)
	 ||  (SEV_TYPE_MACHINE == XZSCC_TYPEM_STA)
	 ||  (SEV_TYPE_MACHINE == XZSCC_TYPEM_HIS)
	 || (SEV_TYPE_MACHINE == XZSCC_TYPEM_LT)
	 || (SEV_TYPE_MACHINE == XZSCC_TYPEM_SP) 
	 || (!strcmp(pl_NomMachine,XDC_PASSIF_CI))
	 )
      {
         /* dans le cas d'une station l'etat 
			est diffuse aux serveurs du site */
         XDG_EncodeDG2(vl_Datagroup, XDG_NSUPHIST, SEP_NOM_SITE);
      }
   }

   /* recuperation de l'etat de la machine locale */
   XZSE_09EtatMachine(&vl_EtatMachineLoc);

#if 1	/* Modif V1.16 */
   /* dans tous les cas si la machine locale est un SC nominal actif */
/*   if ( ((vl_EtatMachineLoc == XZSEC_ETAT_NACTIF) ||
	(vl_EtatMachineLoc == XZSEC_SANS_ETAT) || 
	(vl_EtatMachineLoc == XZSEC_ETAT_DEGRADE) )*/
	if ( (SEV_TYPE_MACHINE == XZSCC_TYPEM_SC) || (SEV_TYPE_MACHINE == XZSCC_TYPEM_PCS) || 
	     (SEV_TYPE_MACHINE == XZSCC_TYPEM_PC2) || (SEV_TYPE_MACHINE == XZSCC_TYPEM_PC2LT)) /* ajout PCS modif 1.21 */
   	{
		if (vl_EtatMachineLoc != XZSEC_ETAT_NPASSIF) 	
		{
      			/*A envoi de l'etat a la machine au serveur de donnees 
      			*	uniquement si l'etat n'est pas celui d'un des SD */
      			if (XZSA_17TypeMachine(pl_NomMachine, &vl_TypeMachine) == XDC_OK)
      			{
	 			if (vl_TypeMachine != XZSCC_TYPEM_SD)
	 			{
					/*B Construction du Datagroup */
            				strcpy(vl_Datagroup, XDG_NSUPHIST);
         			}
      			}
		}
		else 
		{
         		/* dans le cas d'une station l'etat 
				est diffuse aux serveurs du site */
         		XDG_EncodeDG2(vl_Datagroup, XDG_NSUPHIST, SEP_NOM_SITE);
		}	
   	}
#endif


   /* envoi effectif du message */
   if (strlen(vl_Datagroup) != 0)
   {
      	/*B repositionnement sur le debut des donnees variables */
      	va_start(pl_ListeDonnees, va_TypeMessage);

	XZST_03EcritureTrace(XZSTC_MESSAGE,"Envoie du Msg %d \
		-- Datagroup %s \n",va_TypeMessage, vl_Datagroup);

      	/*B envoi du message */ 
      	if (!TipcSrvMsgWriteVa(vl_Datagroup,
			     XDM_IdentMsg(va_TypeMessage),
			     XDM_FLG_SRVMSGWRITE,
			     pl_ListeDonnees))
      	{
	 	/*B erreur envoi message */
		XZST_03EcritureTrace(XZSTC_WARNING,"Pb envoie du Msg  %d \
		-- Datagroup %s \n",va_TypeMessage, vl_Datagroup);
      	}
   }

	 XZST_03EcritureTrace(XZSTC_FONCTION ,
       		"seta_distr_info : OUT \n");

}	/* Fin seta_distr_info */

#endif


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet d'initialiser les differentes etats des machines
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int seta_mach_init(
	XZSCT_NomMachine pa_NomMachine,
	int              va_NumOrdre,
	int              va_EtatMachine)

/*
* ARGUMENTS EN ENTREE :
*   pa_NomMachine  : Nom de la Machine
*   va_NumOrdre : Numero d'ordre de la machine dans la liste
*   va_EtatMachine :        Etat de la Machine
*
* ARGUMENTS EN SORTIE :
*   Aucune
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Utiliser avec un module d'interface XZSA 
*
* FONCTION 
*   Recupere le nom de la machine, puis diffuse l'etat de celle-ci
*	a l'IHM avec l'Etat SANS_ETAT
*
------------------------------------------------------*/
{
	static char *version="$Id: seta_tra.c,v 1.33 2020/11/03 17:54:09 pc2dpdy Exp $ : seta_mach_init " ;
	int	vl_TypeMachine	= 0;
	int	vl_ValRet	= XDC_OK;


	 XZST_03EcritureTrace(XZSTC_FONCTION ,
       		"seta_mach_init : IN  -- Nom Mach %s -- Eta mach %d  \n",
		pa_NomMachine,va_EtatMachine);

	/*A Recuperation du Type de la Machine */
	XZSA_17TypeMachine(pa_NomMachine,&vl_TypeMachine);

	if ( (vl_TypeMachine != XZSCC_TYPEM_SD) 
		&&  (vl_TypeMachine != XZSCC_TYPEM_SC) &&  (vl_TypeMachine != XZSCC_TYPEM_PCS)  
		&&  (vl_TypeMachine != XZSCC_TYPEM_PC2)  &&  (vl_TypeMachine != XZSCC_TYPEM_PC2LT) ) /* ajout PCS modif 1.21 */
	{
		/* actualisation de l'etat machine en interne 
		*			(tableau SEP_ETAT_MACH) */
		seta_actualiser_etamach(pa_NomMachine,va_EtatMachine);

		/*A Diffusion de l'etat des machines a l'IHM */
  		XZSI_11DiffEtatStation(pa_NomMachine, va_EtatMachine);
	}
	
	 XZST_03EcritureTrace(XZSTC_FONCTION ,
       		"seta_mach_init : OUT \n");
	
	return (vl_ValRet);
}	/* Fin seta_mach_init */









