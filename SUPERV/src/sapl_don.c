/*E*/
/* Fichier : $Id: sapl_don.c,v 1.5 2020/11/03 17:53:00 pc2dpdy Exp $	Release : $Revision: 1.5 $        Date : %G
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_DON * FICHIER sapl_don.c
******************************************************
* DESCRIPTION DU MODULE :
*
* Ce module appartient a la tache tsaplg
*
******************************************************
* HISTORIQUE :
*
* T.Milleville   10 Oct 1994 	: Creation
* T.Milleville   2 Mai 1995 	: Ajout de traces V1.3
* T.Milleville   25 Jul 1995 	: Ajout de commentaires V1.4
*
*******************************************************/
/******************************************************
* TESTS UNITAIRES  : RELECTURE CROISEE
*
*******************************************************/


/* fichiers inclus */
#include "xdc.h"
#include "xdg.h"
#include "xdm.h"

#include "xzsa.h"
#include "xzsc.h"
#include "xzst.h"

#define SAPL_DON_C
#include "sapl_don.h"


/* declaration de variables locales */
static char *version = "$Id: sapl_don.c,v 1.5 2020/11/03 17:53:00 pc2dpdy Exp $ : SAPL_DON" ;

/* routines externes */



/*X*/
/* Fichier : $Id: sapl_don.c,v 1.5 2020/11/03 17:53:00 pc2dpdy Exp $	Release : $Revision: 1.5 $        Date : $Date: 2020/11/03 17:53:00 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_DON
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*  Recherche d'informations dans le tableau de surveillance des taches
*
******************************************************
* SEQUENCE D'APPEL :
*/

SPT_SurvTach *sp_rechercherTache( XZSCT_NomTache	va_NomTache )

/*
* PARAMETRES EN ENTREE : nom de la tache
*
* PARAMETRES EN SORTIE : aucun
*
* VALEUR(S) DE RETOUR :   pointeur sur la structure d'informations
*			ou
*			  NULL
*
* CONDITION D'ERREUR : neant
*
* CONDITIONS LIMITES :
*
* La tache passee en parametre n'existe pas dans le tableau de
* surveillance des taches
*
******************************************************/
{
  static char *version = "$Id: sapl_don.c,v 1.5 2020/11/03 17:53:00 pc2dpdy Exp $ : sp_rechercherTache" ;
  int	i 	= 0;
  int	vl_trouve = XDC_FAUX;


  	XZST_03EcritureTrace(	XZSTC_DEBUG1,
			"IN : sp_rechercherTache : va_NomTache='%s'",
			va_NomTache);

  	while ( (i < vg_sp_nb_surv_tach) && (vl_trouve == XDC_FAUX) )
	{
  	XZST_03EcritureTrace(XZSTC_DEBUG2,"Tache surveillee [%s]",
			pg_sp_surv_tach[i].NomTache);

		if ( strcmp (va_NomTache , pg_sp_surv_tach[i].NomTache ) == 0 )
		{
			vl_trouve = XDC_VRAI;
		}
		else
		{
			i++ ; /* suivant */
		}
	}

  	if (vl_trouve == XDC_VRAI)
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,
				"OUT : sp_rechercherTache : tache trouvee");
		return(&(pg_sp_surv_tach[i])) ;  /* Ok, trouve */
	}
  	else
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,
			"OUT : sp_rechercherTache : tache non trouvee");
		return(NULL) ;
	}

}	/* Fin sp_RechercherTache */



/*X*/
/* Fichier : $Id: sapl_don.c,v 1.5 2020/11/03 17:53:00 pc2dpdy Exp $	Release : $Revision: 1.5 $        Date : $Date: 2020/11/03 17:53:00 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_DON
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*  Recherche d'informations dans le tableau de
*  surveillance des sous-systemes
*
******************************************************
* SEQUENCE D'APPEL :
*/

SPT_SurvSsys *sp_rechercherSSysteme( XZSCT_NomSSysteme	va_NomSSysteme )

/*
* PARAMETRES EN ENTREE : nom du sous-systeme
*
* PARAMETRES EN SORTIE : aucun
*
* VALEUR(S) DE RETOUR :   pointeur sur la structure d'informations
*			ou
*			  NULL
*
* CONDITION D'ERREUR : neant
*
* CONDITIONS LIMITES :
*
* Le sous-systeme passe en parametre n'existe pas dans le 
* tableau de surveillance des sous systemes
*
******************************************************/
{
  static char *version = "$Id: sapl_don.c,v 1.5 2020/11/03 17:53:00 pc2dpdy Exp $ : sp_rechercherSSysteme" ;
  int	i 		= 0;
  int	vl_trouve 	= XDC_FAUX;


  XZST_03EcritureTrace(	XZSTC_FONCTION,
			"IN : sp_rechercherSSysteme : va_NomSSysteme='%s'",
			va_NomSSysteme);

  	while ( (i < vg_sp_nb_surv_ssys) && (vl_trouve == XDC_FAUX) )
	{
		if ( strcmp (va_NomSSysteme , 
				pg_sp_surv_ssys[i].NomSSysteme ) == 0 )
		{
			vl_trouve = XDC_VRAI;
		}
		else
		{
			i++ ; /* suivant */
		}
	}

  	if (vl_trouve == XDC_VRAI)
	{
  		XZST_03EcritureTrace(XZSTC_FONCTION,
			"OUT : sp_rechercherSSysteme : sous systeme trouve");
		return(&(pg_sp_surv_ssys[i])) ;  /* Ok, trouve */
	}
  	else
	{
		XZST_03EcritureTrace(XZSTC_FONCTION,
		     "OUT : sp_rechercherSSysteme : sous systeme non trouve");
		return(NULL) ;
	}

}	/* Fin sp_RechercherSysteme */


