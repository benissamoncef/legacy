/*E*/
/* Fichier : $Id: seta_don.c,v 1.6 2011/01/11 14:50:11 gesconf Exp $	Release : $Revision: 1.6 $        Date : $Date: 2011/01/11 14:50:11 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE  SETA_DON * FICHIER seta_don.c
******************************************************
* DESCRIPTION DU MODULE :
*
* Module qui permet de recuperer les positions des machines, sous-systemes
*	taches et entites dans les tableaux de variables
*
* seta_rechmach		: Cette fonction retourne l'indice de l'etat d'une machine
* dans un tableau d'etat machine.
* seta_rechssys		:  Cette fonction retourne l'indice de l'etat d'un sous-systeme
* dans un tableau d'etat sous-systeme.
* seta_rechtach		:  Cette fonction retourne l'indice de l'etat d'une tache
* dans un tableau d'etat tache.
* seta_rechenti		: Cette fonction retourne l'indice de l'etat d'une entite
* dans un tableau d'etat entite.
*
******************************************************
* HISTORIQUE :
*
* T. Milleville  221 Oct 1994	: Creation
* T. Mllleville  09 Avr 1995	: Modification de la rcecherche
*	si une tache est deja definie 
* T. Mllleville  25 Jul 1995	: Ajout de commentaires V1.3
* T. Mllleville  23 Aug 1995	: Modifi traces V1.5
* JPL		11/01/11 : Migration architecture HP ia64 (DEM 971) : retour C.R. OK ou NOK pour plusieurs fonctions 1.6
*******************************************************/

/* fichiers inclus */

#include "xzsc.h"

#include "seta_don.h"

/* definitions de constantes */

/* definitions des types globaux */

/* definitions des variables globales */


XZSCT_NomSite    SEP_NOM_SITE;        /* nom du site de la machine locale */
XZSCT_NomMachine SEP_NOM_MACHINE;     /* nom de la machine locale */
int              SEV_TYPE_MACHINE;    /* type de la machine locale */
XZSCT_NomMachine SEP_MACHINE_SECOURS; /* nom de la machine de secours */
XZSCT_NomTache   SEP_NOM_TACHE;       /* nom de la tache */
XZSCT_NomMachine SEP_NOM_SERVEUR1, SEP_NOM_SERVEUR2;
 
    /* pointeurs sur les tableaux d'etat des elements a surveiller */
SET_ETAT_SSYS *SEP_ETAT_SSYS = NULL; /* etat sous systeme */
SET_ETAT_TACH *SEP_ETAT_TACH = NULL; /* etat tache */
SET_ETAT_MACH *SEP_ETAT_MACH = NULL; /* etat machine */
SET_ETAT_ENTI *SEP_ETAT_ENTI = NULL; /* etat entite */

   /* nombre d'elements a surveiller par la tache */
int SEV_NB_MACH = 0; /* nombre de machines */
int SEV_NB_SSYS = 0; /* nombre de sous-systemes */
int SEV_NB_TACH = 0; /* nombre de taches */
int SEV_NB_ENTI = 0; /* nombre d'entites */


static char *version = "$Id: seta_don.c,v 1.6 2011/01/11 14:50:11 gesconf Exp $ : seta_don" ;

int REDONDANCE_INTERNE	= XDC_FAUX;

/* definition des fonctions externes */



/*X*/
/* Fichier : $Id: seta_don.c,v 1.6 2011/01/11 14:50:11 gesconf Exp $	Release : $Revision: 1.6 $        Date : $Date: 2011/01/11 14:50:11 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SEDONNEES
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction retourne l'indice de l'etat d'une machine
* dans un tableau d'etat machine.
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

int seta_rechmach(
	SET_ETAT_MACH *pa_EtatMachine,
        int va_TailleTab,
         XZSCT_NomMachine pa_NomMachine,
         int *pa_Indice)



/*
* PARAMETRES EN ENTREE :
* pa_EtatMachine : tableau ou s'effectue la recherche
* va_TailleTab   : taille du tableau
* pa_NomMachine  : nom de la machine
*
*
* PARAMETRES EN SORTIE :
* pa_Indice : indice dans le tableau ou -1 si machine
*             non trouvee
*
*
* VALEUR(S) DE RETOUR : 
* XDC_OK	: Tache Trouvee 
* XDC_NOK	: Tache Non  Trouvee 
*
*
* CONDITION D'ERREUR :
* -
*
*
* CONDITIONS LIMITES :
* -
*
*
******************************************************/
{
	static char *version = "$Id: seta_don.c,v 1.6 2011/01/11 14:50:11 gesconf Exp $ : seta_rechmach" ;
    	int vl_NumMachine	= 0;
	int vl_ValRet	= XDC_NOK;

	XZST_03EcritureTrace(XZSTC_FONCTION,"seta_rechmach  \
	-- taille tab %d -- nom mach %s -- indice %d \n",
	va_TailleTab,pa_NomMachine, pa_Indice);

    for (vl_NumMachine = 0, *pa_Indice = -1;
	(vl_NumMachine < va_TailleTab);
	vl_NumMachine ++)
    {

       if (!strcmp(pa_EtatMachine[vl_NumMachine].NomMachine, pa_NomMachine))
       {
	  *pa_Indice = vl_NumMachine;
	  vl_ValRet = XDC_OK;
	XZST_03EcritureTrace(XZSTC_DEBUG1,"seta_rechmach -- %s \
		-- %s -- %d -- %d \n",pa_EtatMachine[vl_NumMachine].NomMachine,
		pa_NomMachine, va_TailleTab,*pa_Indice);
	  break;
       }
    }

    return (vl_ValRet);
}	/* Fin seta_rechmach */
	 


/*X*/
/* Fichier : $Id: seta_don.c,v 1.6 2011/01/11 14:50:11 gesconf Exp $	Release : $Revision: 1.6 $        Date : $Date: 2011/01/11 14:50:11 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SEDONNEES
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction retourne l'indice de l'etat d'un sous-systeme
* dans un tableau d'etat sous-systeme.
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

int seta_rechssys(SET_ETAT_SSYS *pa_EtatSSysteme,
	    int va_TailleTab,
	    XZSCT_NomMachine pa_NomMachine,
	    XZSCT_NomSSysteme pa_NomSSysteme,
	    int *pa_Indice)

/*
* PARAMETRES EN ENTREE :
* pa_EtatMachine : tableau ou s'effectue la recherche
* va_TailleTab   : taille du tableau
* pa_NomMachine  : nom de la machine du sous-systeme
* pa_NomSSysteme : nom du sous-systeme
*
*
* PARAMETRES EN SORTIE :
* pa_Indice : indice dans le tableau ou -1 si sous-systeme 
*             non trouvee
*
*
* VALEUR(S) DE RETOUR : 
* -
*
*
* CONDITION D'ERREUR :
* -
*
*
* CONDITIONS LIMITES :
* -
*
*
******************************************************/
{
	static char *version = "$Id: seta_don.c,v 1.6 2011/01/11 14:50:11 gesconf Exp $ : seta_rechssys" ;
    	int vl_NumSSysteme 	= 0;


    for (vl_NumSSysteme = 0, *pa_Indice =-1;
	 (vl_NumSSysteme < va_TailleTab) ;
	 vl_NumSSysteme ++)
    {
       if (!strcmp(pa_EtatSSysteme[vl_NumSSysteme].NomMachine, pa_NomMachine) &&
           !strcmp(pa_EtatSSysteme[vl_NumSSysteme].NomSSysteme, pa_NomSSysteme))
       {
	  *pa_Indice = vl_NumSSysteme;
	  break;
       }
    }

    return (XDC_OK);
}
	 


/*X*/
/* Fichier : $Id: seta_don.c,v 1.6 2011/01/11 14:50:11 gesconf Exp $	Release : $Revision: 1.6 $        Date : $Date: 2011/01/11 14:50:11 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SEDONNEES
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction retourne l'indice de l'etat d'une tache
* dans un tableau d'etat tache.
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

int seta_rechtach(SET_ETAT_TACH *pa_EtatTache,
	    int va_TailleTab,
	    XZSCT_NomMachine pa_NomMachine,
	    XZSCT_NomTache pa_NomTache,
	    int *pa_Indice)

/*
* PARAMETRES EN ENTREE :
* pa_EtatMachine : tableau ou s'effectue la recherche
* va_TailleTab   : taille du tableau
* pa_NomMachine  : nom de la machine du sous-systeme
* pa_NomSSysteme : nom du sous-systeme
*
*
* PARAMETRES EN SORTIE :
* pa_Indice : indice dans le tableau ou -1 si sous-systeme 
*             non trouvee
*
*
* VALEUR(S) DE RETOUR : 
* XDC_OK	: Tache Trouvee 
* XDC_NOK	: Tache Non  Trouvee 
*
*
* CONDITION D'ERREUR :
*	Aucune
*
* CONDITIONS LIMITES :
*	Aucune
*
******************************************************/
{
	static char *version = "$Id: seta_don.c,v 1.6 2011/01/11 14:50:11 gesconf Exp $ : seta_rechtach" ;
    	int vl_NumTache = 	0;
	int vl_ValRet	= 	XDC_NOK;

	XZST_03EcritureTrace(XZSTC_FONCTION,"seta_rechtach : IN \
		-- Taille tab  %d \n",va_TailleTab);

    /*A Recherche de la tache dans celles existantes */
    for (vl_NumTache = 0, *pa_Indice = -1;
	 (vl_NumTache < va_TailleTab) ;
	 vl_NumTache ++)
    {
	XZST_03EcritureTrace(XZSTC_DEBUG2,"seta_rechtach : Numero %d -- \
		Nom Machine %s -- \
		Tache %s \n",vl_NumTache,
		pa_EtatTache[vl_NumTache].NomMachine,
		pa_EtatTache[vl_NumTache].NomTache);

       if (!strcmp(pa_EtatTache[vl_NumTache].NomMachine, pa_NomMachine) &&
           !strcmp(pa_EtatTache[vl_NumTache].NomTache, pa_NomTache))
       {
	  *pa_Indice = vl_NumTache;
	  vl_ValRet = XDC_OK;

	  break;
       }
    }

	/*A Memorisation de l'indice */
    	*pa_Indice = vl_NumTache;

	XZST_03EcritureTrace(XZSTC_FONCTION,"seta_rechtach : OUT \
		- Code retour %d \n",vl_ValRet);

	return (vl_ValRet);


}	/* Fin seta_rechtach */



/*X*/
/* Fichier : $Id: seta_don.c,v 1.6 2011/01/11 14:50:11 gesconf Exp $	Release : $Revision: 1.6 $        Date : $Date: 2011/01/11 14:50:11 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SEDONNEES
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction retourne l'indice de l'etat d'une entite 
* dans un tableau d'etat entite.
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

int seta_rechenti(SET_ETAT_ENTI *pa_EtatEntite,
	    int va_TailleTab,
	    XZSCT_NomEntite pa_NomEntite,
	    int va_TypeEntite,
	    int *pa_Indice)

/*
* PARAMETRES EN ENTREE :
* pa_EtatEntite  : tableau ou s'effectue la recherche
* va_TailleTab   : taille du tableau
* pa_NomEntite   : nom de l'entite
* va_TypeEntite  : type de l'entite 
*
*
* PARAMETRES EN SORTIE :
* pa_Indice : indice dans le tableau ou -1 si sous-systeme 
*             non trouvee
*
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
	static char *version = "$Id: seta_don.c,v 1.6 2011/01/11 14:50:11 gesconf Exp $ : seta_rechenti" ;
    	int vl_NumEntite 	= 0;

    for (vl_NumEntite = 0, *pa_Indice = -1;
	 (vl_NumEntite < va_TailleTab) ;
	 vl_NumEntite ++)
    {
       if (!strcmp(pa_EtatEntite[vl_NumEntite].NomEntite, pa_NomEntite) &&
           (pa_EtatEntite[vl_NumEntite].TypeEntite !=  va_TypeEntite))
       {
	  *pa_Indice = vl_NumEntite;
	  break;
       }
    }

    return (XDC_OK);
}	/* Fin seta_rechenti */

