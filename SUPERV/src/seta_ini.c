/*E*/
/*
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_INI * FICHIER seta_ini.c
******************************************************
* DESCRIPTION DU MODULE :
*
* Module d'initialisation de la tache TSETAT.
*
******************************************************
* HISTORIQUE :
*
* T.Milleville   21 Oct 1994 	: Creation
* T.Milleville   09 Avr  1995  	: Modification de l'init des 
*	taches et creation de la fonction seta_init_starttmp_cb V1.5
* T.Milleville   25 Jul  1995  	: Ajout de commentaires V1.4
* T.Milleville   6 Aou   1995  	: Modification des valeurs par defaut
*	d'une initialisation machine. Presence par defaut et en 
*	abscence COURTE V1.5
* T.Milleville   8 Aou   1995  	: Remodification des valeurs par defaut
*	d'une initialisation machine. Presence par defaut. V1.6 
* T.Milleville   9 Oct  1995  	: Ajout du test SD == NACTIf pour
*	initialiser les machines a surveiller V1.8
* T.Milleville   24 Oct  1995  	: Initialisation etat tache a NOK V1.9
* P.Niepceron    24 Jan 1996	: Test taille nom tache ds seta_init_starttmp v1.10
* P.Niepceron    05 Fev 1996	: On ne supervise pas les taches d'une machine en surv inter SC v1.11
* P.Niepceron    12 Fev 1995	: Lors d'une demande de reinit on arret la tache pour qu'elle se relance : seta_init_cb v1.12
* JPL		11/01/11 : Migration architecture HP ia64 (DEM 971) : suppression warnings mineurs  1.14
*******************************************************/

/* fichiers inclus */

#include "xzsx.h"
#include "xzsc.h"
#include "xzsa.h"

#include "seta_ini.h"
#include "seta_don.h"
#include "seta_sur.h"
#include "seta_don.h"
#include "seta_bas.h"


/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
SET_ETAT_MACH *pm_EtatMachine;
SET_ETAT_SSYS *pm_EtatSSysteme;
SET_ETAT_TACH *pm_EtatTache;
SET_ETAT_ENTI *pm_EtatEntite;

int vm_NbMachine;
int vm_NbSSysteme;
int vm_NbTache;
int vm_NbEntite;

int vm_NumOrdreSSysteme;
int vm_NumOrdreTache;


static char *version = "$Id: seta_ini.c,v 1.15 2020/11/03 17:53:51 pc2dpdy Exp $ : seta_inir" ;

/* declaration de fonctions internes */
int seta_init_nombre(
	XZSCT_NomMachine NomMachine, int NumOrdre, void *pa_Args);
void seta_init_mach(
	XZSCT_NomMachine pa_NomMachine, int va_NumOrdre, void *pa_Args);
void seta_init_ssys(
	XZSCT_NomSSysteme pa_NomSSysteme, int va_NumOrdre, 
	XZSCT_NomMachine pa_NomMachine);
void seta_init_tach(
	XZSCT_NomTache pa_NomTache, int va_NumOrdre, 
	XZSCT_NomMachine pa_nomMachine);
void seta_init_enti(
	XZSCT_NomEntite pa_NomEntite, int va_TypeEntite, 
	int va_NumOrdre, void *pa_Args);

/* definition de fonctions externes */
extern void seta_sortir();


/*X*/
/* Fichier : $Id: seta_ini.c,v 1.15 2020/11/03 17:53:51 pc2dpdy Exp $	Release : $Revision: 1.15 $        Date : $Date: 2020/11/03 17:53:51 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE seta_init
******************************************************
* DESCRIPTION DE LA FONCTION :
*	Permert d'initialiser toutes les variables a la memorisation
*	des etats de machines, sous-systemes et taches.
*

* SEQUENCE D'APPEL :
*/

int seta_init()

/*
* PARAMETRES EN ENTREE :
*	Aucun
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
*
* CONDITIONS LIMITES :
*
* 	Aucune
*
******************************************************/
{
	static char *version = "$Id: seta_ini.c,v 1.15 2020/11/03 17:53:51 pc2dpdy Exp $ : seta_init" ;
   	int vl_ValRet		= XDC_OK; /* valeur de retour de la fonction */
   	int vl_NbElem		= 0; /* necessaire pour ligne 221 XZSA_01 */
	int	vl_EtatMachine 	= 0;


	XZST_03EcritureTrace(XZSTC_FONCTION, 
		"seta_init : IN \n");

   /* arret eventuel de la surveillance des machines */
   seta_arret_surv();

   /* initialisation des variables utilises par le module */
   pm_EtatMachine = SEP_ETAT_MACH;
   vm_NbMachine = SEV_NB_MACH;
   SEV_NB_MACH = 0;

   pm_EtatSSysteme = SEP_ETAT_SSYS;
   vm_NbSSysteme = SEV_NB_SSYS;
   vm_NumOrdreSSysteme = 0;
   SEV_NB_SSYS = 0;

   pm_EtatTache = SEP_ETAT_TACH;
   vm_NbTache = SEV_NB_TACH;
   vm_NumOrdreTache = 0;
   SEV_NB_TACH = 0;

   pm_EtatEntite = SEP_ETAT_ENTI;
   vm_NbEntite = SEV_NB_ENTI;
   SEV_NB_ENTI = 0;
	
	/*A On reinitialse les pointeurs des etsts 
	*	machines, sous-systemes, taches et entites */
   	SEP_ETAT_MACH = NULL;
   	SEP_ETAT_SSYS = NULL;
   	SEP_ETAT_TACH = NULL;
   	SEP_ETAT_ENTI = NULL;


   /* initialisation du tableau des machines a superviser */
   seta_trait_mach_superv(NULL);

   /*A intialisation des NOMBRES de taches, sous-systemes 
   *		et entites a superviser selon le type de machine */
   seta_trait_mach_superv((void (*)()) seta_init_nombre); 

   /*A allocation memoire des tableaux */
   SEP_ETAT_SSYS = malloc (SEV_NB_SSYS * sizeof(SET_ETAT_SSYS));

   if ( (SEV_TYPE_MACHINE == XZSCC_TYPEM_SD)
	&& (vl_EtatMachine != XZSEC_ETAT_NPASSIF) )
   {
	/*B Initialisation du Nb des Entites */
      XZSA_24ListeEntite(NULL, &SEV_NB_ENTI, NULL);

	/*B Allocation memoire pour memoriser l'etat es Entites */
      SEP_ETAT_ENTI = malloc (SEV_NB_ENTI * sizeof(SET_ETAT_ENTI));
   }


	XZST_03EcritureTrace(XZSTC_WARNING, 
		" Initialisation des machines a supervisees  \n");

   /*A intialisation des ETATS  de taches, sous-systemes 
   *			et entites a superviser selon le type de machine */
   SEP_ETAT_TACH = malloc (((size_t) SEV_NB_TACH +1) * sizeof(SET_ETAT_TACH));
   seta_trait_mach_superv(seta_init_mach);

   if ( (SEV_TYPE_MACHINE == XZSCC_TYPEM_SD)
	&& (vl_EtatMachine != XZSEC_ETAT_NPASSIF) )
   {
      XZSA_24ListeEntite(seta_init_enti, &vl_NbElem, NULL);
   }

   /* Liberation memoire de l'ancien ensemble de tableaux */
   if (pm_EtatMachine != NULL)
   {
      free(pm_EtatMachine);
   }

   if (pm_EtatTache != NULL)
   {
      free(pm_EtatTache);
   }

   if (pm_EtatSSysteme != NULL)
   {
      free(pm_EtatSSysteme);
   }

   if ( (SEV_TYPE_MACHINE == XZSCC_TYPEM_SD)
	&& (vl_EtatMachine != XZSEC_ETAT_NPASSIF) )
   {
      if (pm_EtatEntite != NULL)
      {
         free(pm_EtatEntite);
      }
   }

	XZST_03EcritureTrace(XZSTC_WARNING, 
		" Initialisation des machines a SURVEILLEES  \n");
   /* armement de la surveillance des machines */
   seta_trait_mach_surv((void (*)()) seta_arme_surv); 

	XZST_03EcritureTrace(XZSTC_FONCTION, 
		"seta_init : OUT \n");

   return(vl_ValRet);

}	/* Fin seta_init */


/*X*/
/* Fichier : $Id: seta_ini.c,v 1.15 2020/11/03 17:53:51 pc2dpdy Exp $	Release : $Revision: 1.15 $        Date : $Date: 2020/11/03 17:53:51 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE seta_init_cb
******************************************************
* DESCRIPTION DE LA FONCTION :
*	Declenchee sur reception du msg XDM_TSE_INIT
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_init_cb( 	
	T_IPC_CONN	va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	T_CB_ARG       pa_Arg)

/*
* PARAMETRES EN ENTREE :
* arguments specifiques a un callback.
*
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
*
* CONDITIONS LIMITES :
*
* 	Aucune
******************************************************/
{
	static char *version = "$Id: seta_ini.c,v 1.15 2020/11/03 17:53:51 pc2dpdy Exp $ : seta_init_cb" ;
	XZST_03EcritureTrace(XZSTC_FONCTION, 
		"seta_initcb  : IN \n");

/*modif v1.12   seta_init(); */

	seta_sortir();	

	XZST_03EcritureTrace(XZSTC_FONCTION, 
		"seta_initcb  : OUT \n");

}	/* Fin seta_init_cb */



/*X*/
/* Fichier : $Id: seta_ini.c,v 1.15 2020/11/03 17:53:51 pc2dpdy Exp $	Release : $Revision: 1.15 $        Date : $Date: 2020/11/03 17:53:51 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_INI
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction met a jour SEV_NB_SSYS et
* SEV_NB_TACH en leur additionnant respectivement
* le nombre de sous-systeme et le nombre de taches
* de la machine specifiee en argument.
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

int seta_init_nombre(
	XZSCT_NomMachine pa_NomMachine,
        int va_NumOrdre,
	void *pa_Args)

/*
* PARAMETRES EN ENTREE :
* NomMachine: nom de la machine
* NumOrdre  : numero d'ordre de la machine fourni par
*             XZSA.
*
*
* PARAMETRES EN SORTIE :
*	Aucun
*
* VALEUR(S) DE RETOUR : 
*	Aucune
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
	static char *version = "$Id: seta_ini.c,v 1.15 2020/11/03 17:53:51 pc2dpdy Exp $ : seta_init_nombre" ;
   	int vl_ValRet = XDC_OK; /* valeur de retour de la fonction */ 
   	int vl_NbSSystemes;     /* nombre de sous-systemes de la machine */
   	int vl_NbTaches;        /* nombre de taches de la machine */


	XZST_03EcritureTrace(XZSTC_FONCTION, 
		"seta_init_nombre  : IN  --  Nom Machine %s -- Num Ordre %d \n",
		pa_NomMachine, va_NumOrdre);

   /* mise a jour du nombre de sous-systemes */
   if (XZSA_02ListeSSysteme(pa_NomMachine, NULL, 
			&vl_NbSSystemes, NULL) == XDC_OK)
   {
      SEV_NB_SSYS += vl_NbSSystemes;
   }
   else
   {
      vl_ValRet = XDC_NOK;
   }

   /* mise a jour du nombre de taches */
 /*  if (vl_ValRet == XDC_OK)
   {
      if (XZSA_03ListeTacheMachine(pa_NomMachine, NULL, 
					&vl_NbTaches, NULL) == XDC_OK)
      {
         SEV_NB_TACH += vl_NbTaches;
      }
      else
      {
         vl_ValRet = XDC_NOK;
      }
   }*/
 
	XZST_03EcritureTrace(XZSTC_FONCTION, 
		"seta_init_nombre  : OUT :   Code Retour %d \n",vl_ValRet);

	return (vl_ValRet);


}	/* Fin seta_init_nombre */



/* definition des fonctions internes */


/*X*/
/* Fichier : $Id: seta_ini.c,v 1.15 2020/11/03 17:53:51 pc2dpdy Exp $	Release : $Revision: 1.15 $        Date : $Date: 2020/11/03 17:53:51 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_INI
******************************************************
* DESCRIPTION DE LA FONCTION :
*	Initialise l'etat de la  machine, de ses  sous-systemes
*	et de ses  taches.
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_init_mach(XZSCT_NomMachine pa_NomMachine,
                  int va_NumOrdre,
		  void *pa_Args)

/*
* PARAMETRES EN ENTREE :
*	pa_NomMachine : Nom e la machine locale
*	va_NumOrdre   : Numero d'ordre
*	pa_Args	: Valeur des arguments
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
	static char *version = "$Id: seta_ini.c,v 1.15 2020/11/03 17:53:51 pc2dpdy Exp $ : seta_init_nombre" ;
   	int vl_ValRet = XDC_OK;
   	int vl_Indice = -1;
   	int vl_NbSSysteme, vl_NbTache	= 0;
   	XZSCT_NomMachine pl_NomMachineSec	= "";
   	int 	i =0;

	XZST_03EcritureTrace(XZSTC_FONCTION, 
		"seta_init_mach  : IN : Nom Mach %s -- Num Ordre %d\n",
		pa_NomMachine,va_NumOrdre);

	XZST_03EcritureTrace(XZSTC_DEBUG2, 
		"seta_init_mach  : IN : Nom Mach %s -- Num Ordre %d\n",
		pa_NomMachine,va_NumOrdre);


   /* recherche si l'etat de la machine est deja defini */
   if (pm_EtatMachine != NULL)
   {
      seta_rechmach(pm_EtatMachine, vm_NbMachine, pa_NomMachine, &vl_Indice);
   }


   /* si l'etat de la machine a deja ete defini */
   if (vl_Indice != -1)
   {
       /* Recuperation de cet etat */
       strcpy(SEP_ETAT_MACH[va_NumOrdre].NomMachine, pa_NomMachine); 
       SEP_ETAT_MACH[va_NumOrdre].Etat = pm_EtatMachine[vl_Indice].Etat;
       SEP_ETAT_MACH[va_NumOrdre].EtatModeIns = 
				pm_EtatMachine[vl_Indice].EtatModeIns;
       SEP_ETAT_MACH[va_NumOrdre].EtatModeRec = 
				pm_EtatMachine[vl_Indice].EtatModeRec;
       SEP_ETAT_MACH[va_NumOrdre].HeureVie = pm_EtatMachine[vl_Indice].HeureVie;
   }
   else
   {
       /* Initialisation de l'etat de la machine */
       strcpy(SEP_ETAT_MACH[va_NumOrdre].NomMachine, pa_NomMachine); 
       SEP_ETAT_MACH[va_NumOrdre].Etat = XZSEC_SANS_ETAT;
       SEP_ETAT_MACH[va_NumOrdre].EtatModeIns = XZSEC_MODE_NOR;
       SEP_ETAT_MACH[va_NumOrdre].EtatModeRec = XZSEC_MODE_NOR;

       /* MODIFICATION */
       SEP_ETAT_MACH[va_NumOrdre].EtatAbsence = SET_PRESENCE;
   }

   /* recherche de la machine redondante */
   if (!strcmp(SEP_MACHINE_SECOURS, ""))
   {
      seta_definir_secours(pa_NomMachine);
   }


   /* Si ce n'est pas une surv inter SC alors on memorise les taches et ssys  v1.11 */
   if ( SEP_ETAT_MACH[va_NumOrdre].TypeSurveillance != SEC_SURV_ACTIVE_SC )
     {
     /* initialisation de l'etat des sous-systeme de la machine */

     XZSA_02ListeSSysteme(pa_NomMachine, seta_init_ssys,
				   &vl_NbSSysteme, pa_NomMachine);

   /* initialisation de l'etat des taches de la machine */

     XZSA_03ListeTacheMachine(pa_NomMachine, seta_init_tach, 
					&vl_NbTache, pa_NomMachine);
     }


	XZST_03EcritureTrace(XZSTC_DEBUG2,"Fin initialisation Tache \
		-- Nb taches %d \n",SEV_NB_TACH);


	XZST_03EcritureTrace(XZSTC_FONCTION, 
		"seta_init_mach  : OUT -- Cod Retour %d\n",vl_ValRet);

}	/* Fin seta_init_mach */




/*X*/
/* Fichier : $Id: seta_ini.c,v 1.15 2020/11/03 17:53:51 pc2dpdy Exp $	Release : $Revision: 1.15 $        Date : $Date: 2020/11/03 17:53:51 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_INI
******************************************************
* DESCRIPTION DE LA FONCTION :
*	Permet d'initialiser le sous-systeme
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_init_ssys(XZSCT_NomSSysteme pa_NomSSysteme,
	          int va_NumOrdre,
		  XZSCT_NomMachine pa_NomMachine)

/*
* PARAMETRES EN ENTREE :
*	pa_NomSSysteme	: Nom du sous-systeme
*	va_NumOrdre	: Numero d'ordre
*	pa_NomMachine	: Nom de la machine
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
	static char *version = "$Id: seta_ini.c,v 1.15 2020/11/03 17:53:51 pc2dpdy Exp $ : seta_init_ssys" ;
   	int vl_ValRet = XDC_OK;
   	int vl_Indice = -1;

	XZST_03EcritureTrace(XZSTC_FONCTION, 
		"seta_init_ssys  : IN -- Nom SSyst %s -- Num Ordre %d \n",
		pa_NomSSysteme,va_NumOrdre);

   /* recherche si l'etat du sous-systeme est deja defini */
   if (pm_EtatSSysteme != NULL)
   {
      seta_rechssys(pm_EtatSSysteme, vm_NbSSysteme, pa_NomMachine, pa_NomSSysteme, &vl_Indice);
   }


   /* Affectation du nom de la machine et du nom du sous-systeme */
   strcpy(SEP_ETAT_SSYS[vm_NumOrdreSSysteme].NomMachine, pa_NomMachine); 
   strcpy(SEP_ETAT_SSYS[vm_NumOrdreSSysteme].NomSSysteme, pa_NomSSysteme); 

   /* affectation de l'etat du sous systeme */       
   if (vl_Indice != -1)
   {
       /* Recuperation de l'etat existant */
       SEP_ETAT_SSYS[vm_NumOrdreSSysteme].Etat = pm_EtatSSysteme[vl_Indice].Etat;

   }
   else
   {
       /* Affectation de l'etat par defaut */
       SEP_ETAT_SSYS[vm_NumOrdreSSysteme].Etat = XZSEC_SANS_ETAT;
   }

   vm_NumOrdreSSysteme++; 

	XZST_03EcritureTrace(XZSTC_FONCTION, 
		"seta_init_ssys  : OUT -- Code Retour %d \n",vl_ValRet);

}	/* Fin seta_init_ssys */


/*X*/
/* Fichier : $Id: seta_ini.c,v 1.15 2020/11/03 17:53:51 pc2dpdy Exp $	Release : $Revision: 1.15 $        Date : $Date: 2020/11/03 17:53:51 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_INI
******************************************************
* DESCRIPTION DE LA FONCTION :
*	Initialisation des parametres de la tache
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_init_tach(XZSCT_NomTache pa_NomTache,
	     int va_NumOrdre,
	     XZSCT_NomMachine pa_NomMachine)

/*
* PARAMETRES EN ENTREE :
*	pa_NomTache	: Nom de la tache
*	va_NumOrdre	: Numero d'ordre
*	pa_NomMachine	: Nom de la machine
*
* PARAMETRES EN SORTIE :
*	Aucun
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
	static char *version = "$Id: seta_ini.c,v 1.15 2020/11/03 17:53:51 pc2dpdy Exp $ : seta_init_tach" ;
   	int vl_ValRet = XDC_OK;
   	int vl_ValRet_Rech = XDC_NOK;
   	int vl_Indice = -1;


	XZST_03EcritureTrace(XZSTC_FONCTION, 
		"seta_init_tache   : IN -- Nom Tache %s -- Num Ordre %d\n",
		pa_NomTache,va_NumOrdre);

  /*A Teste si Tache Permanente ou tache Temporaire explicitement 
  *	demandee a etre lance */
  vl_ValRet = XZSA_15TacheTempMachine(pa_NomMachine,pa_NomTache);

	XZST_03EcritureTrace(XZSTC_DEBUG2, 
		"seta_init_tache   : IN -- Nom Tache %s -- Nom Mach %s \
		-- Num Ordre %d -- retour Temporaire %d -- Nb taches %d \n",
		pa_NomTache,pa_NomMachine,va_NumOrdre, vl_ValRet,
		SEV_NB_TACH);


  if( (vl_ValRet ==  XZSAC_TYPE_TPER) 
		|| ( (vl_ValRet == XDC_OK) && (va_NumOrdre == -1)) ) 
  {
	XZST_03EcritureTrace(XZSTC_DEBUG2, "Incrementation du Nombre de taches \
		pointeur = %d \n",pm_EtatTache);

   	/*A recherche si l'etat des taches est define et on reinitialise */
   	if (pm_EtatTache != NULL) 
   	{
      		vl_ValRet_Rech = seta_rechtach(pm_EtatTache, vm_NbTache, 
				pa_NomMachine, pa_NomTache, &vl_Indice);
		XZST_03EcritureTrace(XZSTC_DEBUG2, "Retour recherche tache \
			- Code Retour %d -- Indice %d \n",vl_ValRet_Rech,vl_Indice);

		/*B Affectation de l'indice de Taleau */
		vm_NumOrdreTache = vl_Indice;
   	}
	else if (SEV_NB_TACH != 0)
	{
		/*A Ajout de Tache , ou tache temporaire relance */
		vl_ValRet_Rech = seta_rechtach(SEP_ETAT_TACH,SEV_NB_TACH,
			 	pa_NomMachine, pa_NomTache, &vl_Indice);

	XZST_03EcritureTrace(XZSTC_DEBUG2, "Retour recherche tache \
		- Code Retour %d -- Indice %d \n",vl_ValRet_Rech,vl_Indice);

		/*B Affectation de l'indice de Taleau */
		vm_NumOrdreTache = vl_Indice;
	}

   	/*A Affectation du nom dela machine et du nom de la tache */
   	strcpy(SEP_ETAT_TACH[vm_NumOrdreTache].NomMachine, pa_NomMachine); 
   	strcpy(SEP_ETAT_TACH[vm_NumOrdreTache].NomTache, pa_NomTache); 

   	/* affectation de l'etat de la tache */       
   	if (vl_ValRet_Rech == XDC_OK )
   	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"Affectation tache NON default \
			num ordre %d -- Nom mach %s -- Nom tac %s \
			-- Etat %d -- NivTrace %d -- VisuTrace %d \n",
			vm_NumOrdreTache, SEP_ETAT_TACH[vm_NumOrdreTache].NomMachine,
			SEP_ETAT_TACH[vm_NumOrdreTache].NomTache,
			SEP_ETAT_TACH[vm_NumOrdreTache].Etat,
			SEP_ETAT_TACH[vm_NumOrdreTache].NivTrace,
			SEP_ETAT_TACH[vm_NumOrdreTache].VisuTrace);


       		/* Recuperation de l'etat existant */
       		if (pm_EtatTache != NULL)
       		{
       			SEP_ETAT_TACH[vm_NumOrdreTache].Etat = 
				pm_EtatTache[vm_NumOrdreTache].Etat;
       			SEP_ETAT_TACH[vm_NumOrdreTache].NivTrace = 
				pm_EtatTache[vm_NumOrdreTache].NivTrace;
       			SEP_ETAT_TACH[vm_NumOrdreTache].VisuTrace = 
       				pm_EtatTache[vm_NumOrdreTache].VisuTrace;
       		}
   	}
   	else
   	{
		XZST_03EcritureTrace(XZSTC_DEBUG2,"Affectation tache default \
			num ordre %d -- Nom mach %s -- Nom tac %s \n",
			vm_NumOrdreTache, SEP_ETAT_TACH[vm_NumOrdreTache].NomMachine,
			SEP_ETAT_TACH[vm_NumOrdreTache].NomTache);

		/*B INcrementtion du Nb de taches */
		SEV_NB_TACH++;

       		/* Affectation de l'etat par defaut */
       		SEP_ETAT_TACH[vm_NumOrdreTache].Etat = XZSEC_ETAT_NOK;

#if 0
       		SEP_ETAT_TACH[vm_NumOrdreTache].Etat = XZSEC_SANS_ETAT;
#endif
       		SEP_ETAT_TACH[vm_NumOrdreTache].NivTrace = 
			XZSTC_FATAL | XZSTC_WARNING| XZSTV_NiveauTrace;
       		SEP_ETAT_TACH[vm_NumOrdreTache].VisuTrace = XZSTV_VisuTrace;
#if 1
		/* Incrementation Nombre de Taches */
   		vm_NumOrdreTache++; 

		/*B reallocation du tacbleau pour memoriser
		*	les etats des taches */
		SEP_ETAT_TACH = realloc(SEP_ETAT_TACH, ((size_t) SEV_NB_TACH +1) * sizeof(SET_ETAT_TACH));
#endif
   	}

#if 0
	/* Incrementation Nombre de Taches */
   	vm_NumOrdreTache++; 

	/*B reallocation du tacbleau pour memoriser
	*	les etats des taches */
	SEP_ETAT_TACH = realloc(SEP_ETAT_TACH, (SEV_NB_TACH+1) * sizeof(SET_ETAT_TACH));
#endif

  }

	XZST_03EcritureTrace(XZSTC_FONCTION, 
		"seta_init_tache   : OUT  -- Code Retour %d \n",vl_ValRet);

}	/* Fin seta_init_tach */



/*X*/
/* Fichier : $Id: seta_ini.c,v 1.15 2020/11/03 17:53:51 pc2dpdy Exp $	Release : $Revision: 1.15 $        Date : $Date: 2020/11/03 17:53:51 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_INI
******************************************************
* DESCRIPTION DE LA FONCTION :
*	Initialisation des valeurs de l'entite
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_init_enti(
	XZSCT_NomEntite pa_NomEntite,
	int va_TypeEntite,
	int va_NumOrdre,
	void *pa_Args)

/*
* PARAMETRES EN ENTREE :
*	pa_NomEntite	: Nom de l'entite
*	va_NumOrdre	: Numero d'ordre
*	pa_Args		: Valeur des arguments
*
* PARAMETRES EN SORTIE :
*	Aucun
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
	static char *version = "$Id: seta_ini.c,v 1.15 2020/11/03 17:53:51 pc2dpdy Exp $ : seta_init_enti" ;
   	int vl_ValRet 	= XDC_OK;
   	int vl_Indice 	= -1;
   	int vl_NumEntite	= 0;

	XZST_03EcritureTrace(XZSTC_FONCTION, "seta_init_enti   : IN \
		-- Nom Entite %s -- Type entite %d --\
		Num Ordre %d \n",pa_NomEntite, va_TypeEntite,va_NumOrdre);

   /* recherche si l'etat de la machine est deja defini */
   if (pm_EtatEntite != NULL)
   {
      seta_rechenti(pm_EtatEntite, vm_NbEntite, 
			pa_NomEntite, va_TypeEntite, &vl_Indice);
   }


   /* affectation du nom et du type de l'entite */
   strcpy(SEP_ETAT_ENTI[va_NumOrdre].NomEntite, pa_NomEntite); 
   SEP_ETAT_ENTI[va_NumOrdre].TypeEntite = va_TypeEntite;

   /* Affectation de l'etat */
   if (vl_Indice != -1)
   {
       /* Recuperation de l'etat anterieur */
       SEP_ETAT_ENTI[va_NumOrdre].Etat = pm_EtatEntite[vl_Indice].Etat;
   }
   else
   {
       /* Affectation avec la valeur par defaut */
       SEP_ETAT_ENTI[va_NumOrdre].Etat = XZSEC_SANS_ETAT;
   }

	XZST_03EcritureTrace(XZSTC_FONCTION, 
		"seta_init_enti   : OUT  -- Code Retour %d \n",vl_ValRet);

}	/* Fin seta_init_enti */


/*X*/
/* Fichier : $Id: seta_ini.c,v 1.15 2020/11/03 17:53:51 pc2dpdy Exp $	Release : $Revision: 1.15 $        Date : $Date: 2020/11/03 17:53:51 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE seta_init_cb
******************************************************
* DESCRIPTION DE LA FONCTION :
*	Declenche sur reception du msg XDM_TSE_STARTTMP
*	Permet de mettre a jour les variables concernant la tache
*	sur la machine recuperes en argument
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_init_starttmp_cb( 	
	T_IPC_CONN	va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	T_CB_ARG       pa_Arg)

/*
* PARAMETRES EN ENTREE :
* arguments specifiques a un callback.
*
*
* PARAMETRES EN SORTIE :
* 	Aucun-
*
*
* VALEUR(S) DE RETOUR : i
*	Aucun
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
	static char *version = "$Id: seta_ini.c,v 1.15 2020/11/03 17:53:51 pc2dpdy Exp $ : seta_init_starttmp_cb" ;
#ifndef _TIBCO_EMS
	T_STR  		vl_NomTache     = "";
	T_STR  		vl_NomMachine     = "";
#else
	/*A vl_NomTache declare en XDY_Texte car reception de noms de taches telles que [/produits/migrazur/appliSD/exec/teopc.x] */
	XDY_Texte vl_NomTache     = "";
	XDY_NomMachine vl_NomMachine     = "";
#endif
	int             vl_NumOrdre     = -1;


	XZST_03EcritureTrace(XZSTC_FONCTION, 
			"seta_init_starttmp  : IN \n");

	/*A Recuperation du Nom dse la Tache */
	TipcMsgRead(pa_Data -> msg,
		T_IPC_FT_STR, &vl_NomTache,
		T_IPC_FT_STR, &vl_NomMachine,
		NULL);

	XZST_03EcritureTrace(XZSTC_MESSAGE, 
		" Reception TSE_STARTTMP tache %s -- Ordre %d -- Machine %s \n",
		vl_NomTache,vl_NumOrdre,vl_NomMachine);


	/*A Initialisation des etats de la Taches */
	if ( strlen( vl_NomTache ) < sizeof( XZSCT_NomTache ) )  /* Ajout test V1.10 */
	   seta_init_tach(vl_NomTache,vl_NumOrdre,vl_NomMachine);
	else
	   XZST_03EcritureTrace(XZSTC_WARNING, "seta_init_starttmp  :Nom de tache trop long: etat non memorise:%s", vl_NomTache);


	XZST_03EcritureTrace(XZSTC_FONCTION, 
			"seta_init_starttmp  : OUT  \n");

}	/* Fin seta_init_starttmp_cb */
