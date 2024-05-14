/*E*/
/* Fichier : $Id: seta_act.c,v 1.4 1995/07/26 11:34:45 milleville Exp $	Release : $Revision: 1.4 $        Date : $Date: 1995/07/26 11:34:45 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_ACT * FICHIER seta_act.c
******************************************************
* DESCRIPTION DU MODULE :
*
* Module qui permet la mise a jour des variables d'etat des machines,
*	des sous-systemes, des taches ainsi que leurs niveaux de traces.
*
* seta_actualiser_etamach	: Cette fonction met a jour l'etat d'une machine.
* seta_actualiser_etassys	: Cette fonction met a jour l'etat d'un sous-systeme.
* seta_actualiser_etatach	: Cette fontion met a jour l'etat d'une tache.
* seta_actualiser_etaenti	: Cette fonction met a jour l'etat d'une entite.
* seta_actualiser_etamode	: Cette fonction met a jour les modes d'une machine.
* seta_actualiser_etatrac	: Cette fontion met a jour le niveau de trace d'une tache.
*
*
******************************************************
* HISTORIQUE :
*
* T.Milleville  24 Oct 1994 : Creation
* T.Milleville  29 Mar 1995 : Modification de la valeur de retour
*	de la fonction seta_actualiser_etatach,seta_actualiser_etatssys,
*	seta_actualiser_etatent  V1.2 
* T.Milleville  25 Jul 1995 : Ajout de commentaires V1.3
* T.Milleville  26 Jul 1995 : Mise a jour entete V1.4
*******************************************************/


/******************************************************
* TESTS UNITAIRES  : RELECTURE CROISEE
*
*
*******************************************************/

/* fichiers inclus */

#include "xdc.h"
#include "xzst.h"

#include "seta_don.h"
#include "seta_act.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: seta_act.c,v 1.4 1995/07/26 11:34:45 milleville Exp $ : SETA_ACT" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */





/*X*/
/* Fichier : $Id: seta_act.c,v 1.4 1995/07/26 11:34:45 milleville Exp $	Release : $Revision: 1.4 $        Date : $Date: 1995/07/26 11:34:45 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_ACT
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction met a jour l'etat d'une machine.
*
******************************************************
* SEQUENCE D'APPEL :
*/

int seta_actualiser_etamach(
	XZSCT_NomMachine pa_NomMachine,
        int va_Etat)

/*
* PARAMETRES EN ENTREE :
* pa_NomMachine: nom de la machine
* va_Etat:       nouvel etat de la machine
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : 
*	XDC_VRAI	: mise a jour effectue
*	XDC_FAUX	: mise a jour non effectuee
*
* CONDITION D'ERREUR :
*
* Aucune
*
* CONDITIONS LIMITES :
*
* Aucune
*
******************************************************/
{
   static char *version = "$Id: seta_act.c,v 1.4 1995/07/26 11:34:45 milleville Exp $ : seta_actualiser_etamach" ;
   int vl_ValRet 	= XDC_OK;
   int vl_MajEffectuee 	= XDC_FAUX;
   int vl_NumMachine	= 0;

   XZST_03EcritureTrace(XZSTC_FONCTION,
		"seta_actualiser_etamach : IN -- Nom Mach %s -- \
		Etat Mach %d \n", pa_NomMachine, va_Etat);

   /* recherche de la machine dans la liste des machines surveillees */
   for (vl_NumMachine = 0;
	(vl_NumMachine < SEV_NB_MACH) && (vl_MajEffectuee != XDC_VRAI);
	vl_NumMachine++)
   {
      /*A si machine trouvee */
      if (!strcmp(SEP_ETAT_MACH[vl_NumMachine].NomMachine , pa_NomMachine))
      {

      XZST_03EcritureTrace(XZSTC_DEBUG1,"Maj Etat mach %d -- Nom Mach %s  \n",
			va_Etat,SEP_ETAT_MACH[vl_NumMachine].NomMachine);

	/*B Teste si nouvelle etat different du precedent */
	if ( SEP_ETAT_MACH[vl_NumMachine].Etat != va_Etat)
	{
		/*B Maj effectuee */
         	vl_MajEffectuee = XDC_VRAI;   
	}

	 /*B  Enregistrement nouvel etat */
         SEP_ETAT_MACH[vl_NumMachine].Etat = va_Etat;
      }
   }

   XZST_03EcritureTrace(XZSTC_FONCTION,
			"seta_actualiser_etamach : OUT -- Maj effectuee %d \n", 
				vl_MajEffectuee);

   return(vl_MajEffectuee);

}	/* Fin seta_actualiser_etamach */




/*X*/
/* Fichier : $Id: seta_act.c,v 1.4 1995/07/26 11:34:45 milleville Exp $	Release : $Revision: 1.4 $        Date : $Date: 1995/07/26 11:34:45 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_ACT
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction met a jour l'etat d'un sous-systeme.
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

int seta_actualiser_etassys(
	XZSCT_NomMachine pa_NomMachine,
        XZSCT_NomSSysteme pa_NomSSysteme,
        int va_Etat)

/*
* PARAMETRES EN ENTREE :
* pa_NomMachine  : nom de la machine du sous-systeme 
* pa_NomSSysteme : nom du sous-systeme
* va_Etat        : nouvel etat du sous-systeme
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : 
*	XDC_VRAI	: mise a jour effectue
*	XDC_FAUX	: mise a jour non effectuee
*
* CONDITION D'ERREUR :
*
* Aucun
*
* CONDITIONS LIMITES :
*
* Aucune
*
******************************************************/
{
   static char *version = "$Id: seta_act.c,v 1.4 1995/07/26 11:34:45 milleville Exp $ : seta_actualiser_etassys" ;
   int vl_ValRet 	= XDC_OK;
   int vl_MajEffectuee 	= XDC_FAUX;
   int vl_NumSSysteme	= 0;


   XZST_03EcritureTrace(XZSTC_FONCTION,
	"seta_actualiser_etassys : IN  -- Nom mach %s -- Nom SSys %s \
	Eta SSyst %d \n", pa_NomMachine,pa_NomSSysteme,va_Etat);

   /*A recherche du sous-systeme dans la liste des sous-systemes a surveiller */
   for (vl_NumSSysteme = 0;
	(vl_NumSSysteme < SEV_NB_SSYS) && (vl_MajEffectuee != XDC_VRAI);
	vl_NumSSysteme++)
   {
      /*A si sous-systeme trouve */
      if (!strcmp(SEP_ETAT_SSYS[vl_NumSSysteme].NomMachine , pa_NomMachine) &&
          !strcmp(SEP_ETAT_SSYS[vl_NumSSysteme].NomSSysteme , pa_NomSSysteme))
      {
	 /*B Teste si nouvel etat different du precedent */
	 if (SEP_ETAT_SSYS[vl_NumSSysteme].Etat != va_Etat)
	 {
		/*B Maj effectuee */
         	vl_MajEffectuee = XDC_VRAI;  
	 }

	 /*B Enregistrement nouvel etat */
	 SEP_ETAT_SSYS[vl_NumSSysteme].Etat = va_Etat;
      }
   }

   XZST_03EcritureTrace(XZSTC_FONCTION,
	"seta_actualiser_etassys : OUT -- Maj Effectuee %d \n",vl_MajEffectuee);

   return(vl_MajEffectuee);

}	/* Fin seta_actualiser_etassys */



/*X*/
/* Fichier : $Id: seta_act.c,v 1.4 1995/07/26 11:34:45 milleville Exp $	Release : $Revision: 1.4 $        Date : $Date: 1995/07/26 11:34:45 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_ACT
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fontion met a jour l'etat d'une tache.
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

int seta_actualiser_etatach(
	XZSCT_NomMachine pa_NomMachine,
        XZSCT_NomTache pa_NomTache,
        int va_Etat)

/*
* PARAMETRES EN ENTREE :
* pa_NomMachine : nom de la machine de la tache
* pa_NomTache   : nom de la tache
* va_Etat       : nouvel etat de la tache
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : 
*	XDC_VRAI	: mise a jour effectue
*	XDC_FAUX	: mise a jour non effectuee
*
* CONDITION D'ERREUR :
*
* Aucune
*
* CONDITIONS LIMITES :
*
* Aucune
*
******************************************************/
{
   static char *version = "$Id: seta_act.c,v 1.4 1995/07/26 11:34:45 milleville Exp $ : seta_actualiser_etatach" ;
   int vl_ValRet 	= XDC_OK;
   int vl_MajEffectuee 	= XDC_FAUX;
   int vl_NumTache	= 0;

   XZST_03EcritureTrace(XZSTC_FONCTION,
	"seta_actualiser_etatach : IN  -- Nommach %s -- Nom Tach %s \
	Eta Tach %d \n", pa_NomMachine,pa_NomTache,va_Etat);

   XZST_03EcritureTrace (XZSTC_DEBUG3," Actualiser Tache -- \
	Nom Mach %s -- Nom Tache %s \n",pa_NomMachine,pa_NomTache);


   /*A recherche si l'etat de la tache est deja defini */
   for (vl_NumTache = 0;
	(vl_NumTache < SEV_NB_TACH) && (vl_MajEffectuee != XDC_VRAI);
	vl_NumTache++)
   {
   XZST_03EcritureTrace (XZSTC_DEBUG3," Actualiser Tache num %d -- \
	Nom Mach %s -- Nom Tache %s \n",
	vl_NumTache, SEP_ETAT_TACH[vl_NumTache].NomMachine,
	SEP_ETAT_TACH[vl_NumTache].NomTache);

      if (!strcmp(SEP_ETAT_TACH[vl_NumTache].NomMachine , pa_NomMachine) &&
          !strcmp(SEP_ETAT_TACH[vl_NumTache].NomTache , pa_NomTache))
      {
	 /*B Teste si Etat de la Tache est differente de la precedente */
	 if (SEP_ETAT_TACH[vl_NumTache].Etat != va_Etat)
	 {
		/*B MAJ effectuee */
         	vl_MajEffectuee = XDC_VRAI;  
	 }

	 /*B Enregistrement du nouvel etat */
	 SEP_ETAT_TACH[vl_NumTache].Etat = va_Etat;
      }
   }

   XZST_03EcritureTrace(XZSTC_FONCTION,
	"seta_actualiser_etatach : OUT -- Maj Effectuee %d \n",vl_MajEffectuee);


   return(vl_MajEffectuee);

}	/* Fin seta_actualiser_etatach */



/*X*/
/* Fichier : $Id: seta_act.c,v 1.4 1995/07/26 11:34:45 milleville Exp $	Release : $Revision: 1.4 $        Date : $Date: 1995/07/26 11:34:45 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_ACT
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction met a jour l'etat d'une entite.
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

int seta_actualiser_etaenti(
	XZSCT_NomEntite pa_NomEntite,
        int va_TypeEntite,
        int va_Etat)

/*
* PARAMETRES EN ENTREE :
* pa_NomEntite  : nom de l'entite
* va_TypeEntite : type de l'entite
* va_Etat       : etat de l'entite
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : 
*	XDC_VRAI	: mise a jour effectue
*	XDC_FAUX	: mise a jour non effectuee
*
* CONDITION D'ERREUR :
*
* Aucune
*
* CONDITIONS LIMITES :
*
* Aucune
*
******************************************************/
{
   static char *version = "$Id: seta_act.c,v 1.4 1995/07/26 11:34:45 milleville Exp $ : seta_actualiser_etaenti" ;
   int vl_ValRet 	= XDC_OK;
   int vl_MajEffectuee 	= XDC_FAUX;
   int vl_NumEntite	= 0;

   XZST_03EcritureTrace(XZSTC_FONCTION,
		"seta_actualiser_etaenti : IN  -- NomEnt  %s -- \
		Type Ent Tach %d -- Eta Tach %d \n", 
		pa_NomEntite,va_TypeEntite,va_Etat);

   /*A recherche de l'entite dans la liste des entites a surveiller */
   for (vl_NumEntite = 0;
	(vl_NumEntite < SEV_NB_ENTI) && (vl_MajEffectuee != XDC_VRAI);
	vl_NumEntite++)
   {
      if (!strcmp(SEP_ETAT_ENTI[vl_NumEntite].NomEntite, pa_NomEntite) &&
	  (SEP_ETAT_ENTI[vl_NumEntite].TypeEntite == va_TypeEntite))
      {
	 /*B Teste si nouvel etat est different du precedent */
	 if (SEP_ETAT_ENTI[vl_NumEntite].Etat != va_Etat)
	 {
		/*B Maj effectuee */
         	vl_MajEffectuee = XDC_VRAI;  
	 }

	 /*B Enregistrement nouvel etat */
	 SEP_ETAT_ENTI[vl_NumEntite].Etat = va_Etat; 
      }
   }

   XZST_03EcritureTrace(XZSTC_FONCTION,
		"seta_actualiser_etaenti : OUT -- Maj Effectuee %d \n",
		vl_MajEffectuee);

   return(vl_MajEffectuee);

}	/* Fin seta_actualiser_etaenti */



/*X*/
/* Fichier : $Id: seta_act.c,v 1.4 1995/07/26 11:34:45 milleville Exp $	Release : $Revision: 1.4 $        Date : $Date: 1995/07/26 11:34:45 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_ACT
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction met a jour les modes d'une machine.
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

int seta_actualiser_etamode(
	XZSCT_NomMachine pa_NomMachine,
        int va_ModeIns,
	int va_ModeRec)

/*
* PARAMETRES EN ENTREE :
* pa_NomMachine  : nom de la machine
* va_ModeIns     : mode d'instruction de la machine 
* va_ModeRec     : mode de reconstruction de la machine 
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : 
*	XDC_VRAI	: mise a jour effectue
*	XDC_FAUX	: mise a jour non effectuee
* CONDITION D'ERREUR :
*
* Aucune
*
* CONDITIONS LIMITES :
*
* Aucune
*
******************************************************/
{
   static char *version = "$Id: seta_act.c,v 1.4 1995/07/26 11:34:45 milleville Exp $ : seta_actualiser_etamode" ;
   int vl_ValRet 	= XDC_OK;
   int vl_MajEffectuee 	= XDC_FAUX;
   int vl_NumMachine	= 0;

   XZST_03EcritureTrace(XZSTC_FONCTION,
	"seta_actualiser_etamode : IN  -- NomMachine   %s -- Mode Insertion %d \
	-- Mode Reconstruction %d\n",pa_NomMachine,va_ModeIns,va_ModeRec);

   /* recherche de la machine dans la liste des machines a surveiller */
   for (vl_NumMachine = 0;
	(vl_NumMachine < SEV_NB_MACH) && (vl_MajEffectuee != XDC_VRAI);
	vl_NumMachine++)
   {
      /* si machine trouvee */
      if (!strcmp(SEP_ETAT_MACH[vl_NumMachine].NomMachine , pa_NomMachine))
      {

	 /*B Teste si nouvel etat different du precedent */
	 if ( (SEP_ETAT_MACH[vl_NumMachine].EtatModeIns != va_ModeIns)
		|| (SEP_ETAT_MACH[vl_NumMachine].EtatModeRec != va_ModeRec) )
	{
		/*B Maj effectuee */
         	vl_MajEffectuee = XDC_VRAI;   
	 }

	 /*B Enregistrement des nouveaux etats */
         SEP_ETAT_MACH[vl_NumMachine].EtatModeIns = va_ModeIns;
         SEP_ETAT_MACH[vl_NumMachine].EtatModeRec = va_ModeRec;

      }
   }

   XZST_03EcritureTrace(XZSTC_FONCTION,
	"seta_actualiser_etamode : OUT -- Maj Effectuee %d \n",vl_MajEffectuee);

   return(vl_MajEffectuee);
   
}	/* Fin seta_actualiser_etamode */



/*X*/
/* Fichier : $Id: seta_act.c,v 1.4 1995/07/26 11:34:45 milleville Exp $	Release : $Revision: 1.4 $        Date : $Date: 1995/07/26 11:34:45 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_ACT
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fontion met a jour le niveau de trace d'une tache.
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

int seta_actualiser_etatrac(XZSCT_NomMachine pa_NomMachine,
                           XZSCT_NomTache pa_NomTache,
			   XZSTT_NiveauTrace va_NivTrace,
	                   int va_VisuTrace)

/*
* PARAMETRES EN ENTREE :
* pa_NomMachine : nom de la machine de la tache
* pa_NomTache   : nom de la tache
* va_NivTrace   : nouveau niveau de trace de la tache
* va_VisuTrace  : indicateur de redirection des traces a l'ecran
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : 
*	XDC_VRAI	: mise a jour effectue
*	XDC_FAUX	: mise a jour non effectuee
*
* CONDITION D'ERREUR :
*
* Aucune
*
* CONDITIONS LIMITES :
*
* Aucune
*
******************************************************/
{
   static char *version = "$Id: seta_act.c,v 1.4 1995/07/26 11:34:45 milleville Exp $ : seta_actualiser_etatrac" ;
   int vl_ValRet 	= XDC_OK;
   int vl_MajEffectuee 	= XDC_FAUX;
   int vl_NumTache	= 0;



   XZST_03EcritureTrace(XZSTC_FONCTION,
	"seta_actualiser_etatrac : IN  -- NomMachine   %s -- Nom Tache %s \
	NivTrace %d -- VisuTrace %d \n",pa_NomMachine,
	pa_NomTache,va_NivTrace,va_VisuTrace);

   /*A recherche de la tache dans la liste des taches a surveiller */
   for (vl_NumTache = 0;
	(vl_NumTache < SEV_NB_TACH) && (vl_MajEffectuee != XDC_VRAI);
	vl_NumTache++)
   {


   XZST_03EcritureTrace (XZSTC_DEBUG3," Actualiser Tache num %d -- \
	Nom Mach %s -- Nom Tache %s \n",
	vl_NumTache, SEP_ETAT_TACH[vl_NumTache].NomMachine,
	SEP_ETAT_TACH[vl_NumTache].NomTache);

      if (!strcmp(SEP_ETAT_TACH[vl_NumTache].NomMachine, pa_NomMachine) &&
          !strcmp(SEP_ETAT_TACH[vl_NumTache].NomTache, pa_NomTache))
      {
	 /*B Teste si nouveaux etats different des precedents */
	 if ( (SEP_ETAT_TACH[vl_NumTache].NivTrace != va_NivTrace) 
		|| (SEP_ETAT_TACH[vl_NumTache].VisuTrace != va_VisuTrace) )
	 {
   		XZST_03EcritureTrace (XZSTC_INFO,
			" Actualiser Tache Nouvel etat Num Tache  %d -- \
			Nom tac %s -- Nom mach %s \n",
			vl_NumTache,SEP_ETAT_TACH[vl_NumTache].NomTache,
			SEP_ETAT_TACH[vl_NumTache].NomMachine);

		/*B Maj effectuee */
         	vl_MajEffectuee = XDC_VRAI;   
	 }

	 /*B Enregistrement des nouveaux etats */
	 SEP_ETAT_TACH[vl_NumTache].NivTrace = va_NivTrace;
	 SEP_ETAT_TACH[vl_NumTache].VisuTrace = va_VisuTrace;
      }
   }

   XZST_03EcritureTrace(XZSTC_FONCTION,
	"seta_actualiser_etatrac : OUT -- Maj Effectuee %d \n",vl_MajEffectuee);

   return(vl_MajEffectuee);


}	/* Fin seta_actualiser_etatrac */


