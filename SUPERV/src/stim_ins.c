/*E*/
/* Fichier : $Id: stim_ins.c,v 1.4 1995/07/25 11:10:30 milleville Exp $	Release : $Revision: 1.4 $        Date : $Date: 1995/07/25 11:10:30 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE STIM_INS * FICHIER stim_ins.c
******************************************************
* DESCRIPTION DU MODULE :
*
* Ce module permet d'inserer une demande de reveil dans
* la liste de demande des reveils.
*
******************************************************
* HISTORIQUE :
*
* T.Milleville  27 Sep 1994	: Creation
* T.Milleville  25 Jul 1995	: Ajout coomentaires V1.4
*
*******************************************************/


/* fichiers inclus */
#include "stim_ser.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
static char *version = "$Id: stim_ins.c,v 1.4 1995/07/25 11:10:30 milleville Exp $ : STIM_INS" ;



/* declaration de fonctions internes */

/* definition de fonctions externes */




/*X*/
/* Fichier : $Id: stim_ins.c,v 1.4 1995/07/25 11:10:30 milleville Exp $	Release : $Revision: 1.4 $        Date : $Date: 1995/07/25 11:10:30 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE STIM_INS
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Insere dans une liste de demande de reveil, une de-
* mande de reveil. Les demandes sont classees dans leurs
* ordre d'echeance. Le timer peut etre eventuellement 
* reprogrammee
*
******************************************************
* SEQUENCE D'APPEL :
*/

void	sm_Inserer_Liste(SMT_ENRG_TIMER	*pa_Enr)

/*
* PARAMETRES EN ENTREE :
*
*	pa_Enr : Pointeur sur enregistrement a inserer
*
******************************************************/
{
	static char *version = "$Id: stim_ins.c,v 1.4 1995/07/25 11:10:30 milleville Exp $ : sm_Inserer_Liste" ;
	SMT_ENRG_TIMER *	pl_Timer 	= NULL;
	SMT_ENRG_TIMER *	pl_Enr 		= NULL;
	SMT_ENRG_TIMER *	pl_ExEnr 	= NULL;
	struct timeval		vl_Prog ;


XZST_03EcritureTrace(XZSTC_FONCTION,"\nsm_Inserer_Liste IN : Demande d'insertion pour :\n");
XZST_03EcritureTrace(XZSTC_FONCTION,"\t\tHorodate=%f\n",pa_Enr->Horodate);
XZST_03EcritureTrace(XZSTC_FONCTION,"\t\tDelai=%d\n",pa_Enr->Delai);
XZST_03EcritureTrace(XZSTC_FONCTION,"\t\tIdMsg=%d\n",pa_Enr->IdMsg);
XZST_03EcritureTrace(XZSTC_FONCTION,"\t\tDg=%s\n",pa_Enr->Dg);
XZST_03EcritureTrace(XZSTC_FONCTION,"\t\tCle=%s\n",pa_Enr->Cle);

/* si l'allocation memoire est ok */
if ((pl_Timer = (SMT_ENRG_TIMER *)malloc(sizeof(SMT_ENRG_TIMER))) != NULL)
{
   /* copie enegistrement */
   memcpy(pl_Timer,pa_Enr,sizeof(*pl_Timer)) ;
    
   /* pour les enregistrements deja existant */
   for (pl_Enr = pl_ExEnr = pg_ListeTempo ; pl_Enr != NULL ; pl_ExEnr = pl_Enr,pl_Enr = pl_Enr->Suivant)
   {
       /* si l'horodate de declenchement est inferieure */
       if (pl_Timer->Horodate < pl_Enr->Horodate)
   
          /* la place d'insertion est localisee : on stoppe la recherche */
          break ;
   }

   /* si la place d'insertion est la tete de liste */
   if (pl_Enr == pg_ListeTempo)
   {
      /* cas particulier de la tete de liste */
      pl_Timer->Suivant = pg_ListeTempo ;
      pg_ListeTempo     = pl_Timer ;
   }
   /* sinon insertion nominal */
   else
   {
      pl_Timer->Suivant = pl_ExEnr->Suivant ;
      pl_ExEnr->Suivant = pl_Timer ;
   }

   XZST_03EcritureTrace(XZSTC_FONCTION,"sm_Inserer_Liste OUT : Insertion reussie\n");
}
else
{
   XZST_03EcritureTrace(XZSTC_WARNING,"sm_Inserer_Liste OUT : Echec malloc, insertion non realisee");
}


} /* fin fonction inserer */
