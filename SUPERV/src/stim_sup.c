/*E*/
/* Fichier : stim_sup.c 
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE STIM_SUP * FICHIER stim_sup.c
******************************************************
* DESCRIPTION DU MODULE :
*
* Ce module permet de supprimer une demande de reveil dans
* la liste de demande des reveils.
******************************************************
* HISTORIQUE :
*
* T.Milleville  27 Sep 1994	: Creation
* BG		23 Mai 1995	: Modif free dans sm_Supprimer_Liste (1.2)
* T.Milleville  25 Jul 1995	: Ajout commentaires V1.4
 * JMG		6/06/10		: linux dem/934 1.5
* JPL		05/10/22	: Gestion liberation memoire; suppression de plusieurs elements de cle nulle  1.6
* JPL		06/10/22	: Simplification de la fonction  1.7
*******************************************************/



/* fichiers inclus */
#include "stim_ser.h"


/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */


/* declaration de variables locales */
static char *version = "stim_sup.c 1.7" ;



/* declaration de fonctions internes */

/* definition de fonctions externes */



/*X*/
/*
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE STIM_SUP
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Dans une liste de demande de reveil, detruit un reveil
* donne selon la cle le datagroup de diffusion et le numero
* de message. La cle peut etre nulle
*
******************************************************
* SEQUENCE D'APPEL :
*/

void		sm_Supprimer_Liste(
	char * pa_Cle,
	char * pa_Dg,
	int    va_IdMsg)

/*
* PARAMETRES EN ENTREE :
*
* pa_Cle	: Cle (NULL si pas de cle)
* pa_Dg		: Canal de diffusion
* va_IdMsg	: Numero de message 
*
******************************************************/
{
	SMT_ENRG_TIMER *	pl_Tempo 	= NULL;
	SMT_ENRG_TIMER *	pl_ExTempo 	= NULL;
	XZSMT_CleProg           C_CLE_NULLE 	= "\0";
	XDY_Booleen		vl_cleEstNulle	= XDC_VRAI;


	XZST_03EcritureTrace(XZSTC_FONCTION,
		"sm_Supprimer_Liste IN : Demande de suppression pour : \n \
		IdMsg=%d \tDg=%s \tCle=%s \n",va_IdMsg, pa_Dg , pa_Cle);

	if ( strcmp(pa_Cle, C_CLE_NULLE ) )
	{
		vl_cleEstNulle = XDC_FAUX;
	}

	/*A pour chaque enregistrement de la liste */
	for ( pl_ExTempo = pl_Tempo = pg_ListeTempo ; pl_Tempo != NULL ; )
	{
		/* si la Cle est precisee et egale celui de l'element courant ou */
		/* la Cle est nulle et le Datagroup ou le Message egale celui de l'element courant */
		if ( ( (vl_cleEstNulle == XDC_FAUX)  &&  (!strcmp(pl_Tempo->Cle,pa_Cle)) )  ||
		     ( (vl_cleEstNulle == XDC_VRAI)  &&  (!strcmp(pl_Tempo->Dg,pa_Dg)) && (pl_Tempo->IdMsg == va_IdMsg) ) )
		{
			/* Suppression de l'enregistrement */
			XZST_03EcritureTrace(XZSTC_INFO,
				"sm_Supprimer_Liste : suppression de : Horodate=%f \tDelai=%d \tIdMsg=%d \tDg=%s \tCle=%s \n ",
				pl_Tempo->Horodate, pl_Tempo->Delai, pl_Tempo->IdMsg, pl_Tempo->Dg, pl_Tempo->Cle);

			if (pl_Tempo == pg_ListeTempo)
			{
				pg_ListeTempo = pl_Tempo->Suivant ;
				free (pl_Tempo) ;
				pl_ExTempo = pl_Tempo = pg_ListeTempo ;
			}
			else
			{
				pl_ExTempo->Suivant = pl_Tempo->Suivant ;
				free (pl_Tempo) ;
				pl_Tempo = pl_ExTempo->Suivant ;
			}

			/* si la cle est precisee et trouvee, on ne cherche pas d'autres elements a supprimer */
			if ( vl_cleEstNulle == XDC_FAUX )
			{
				break;
			}
		}
		else
		{
			pl_ExTempo = pl_Tempo ;
			pl_Tempo = pl_Tempo->Suivant ;
		}
	}

	XZST_03EcritureTrace(XZSTC_FONCTION,"sm_Supprimer_Liste OUT");

} /* fin fonction suppression */
