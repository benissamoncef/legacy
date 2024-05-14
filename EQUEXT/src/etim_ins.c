/*E*/
/*Fichier : $Id: etim_ins.c,v 1.4 1995/06/22 16:25:14 volcic Exp $      Release : $Revision: 1.4 $        Date : $Date: 1995/06/22 16:25:14 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TETIM * FICHIER etim_ins.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Ce module insere une demande de reveil dans la liste
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	01 Sep 1994	: Creation
* Mismer.D	version 1.2	28 Oct 1994	: 
* Fontaine.C	version 1.3	23 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.5	22 Jun 1995	: Modif nom liste timers
*
------------------------------------------------------*/

/* fichiers inclus */

#include "etim_ins.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: etim_ins.c,v 1.4 1995/06/22 16:25:14 volcic Exp $ : etim_ins";

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Insere dans une liste de demande de reveil, une de-
* mande de reveil. les demandes sont classees dans leurs
* ordre d'echeance. Le timer peut etre eventuellement 
* reprogrammee
*------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ei_ins_liste ( ETIM_ENRG_TIMER *	pa_Enr )

/*
* ARGUMENTS EN ENTREE :
*   pa_Enr 		: Pointeur sur enregistrement a inserer
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*
******************************************************/
{
	static char *version = "$Id: etim_ins.c,v 1.4 1995/06/22 16:25:14 volcic Exp $ : ei_ins_liste ";
	ETIM_ENRG_TIMER *	pl_Timer;
	ETIM_ENRG_TIMER *	pl_Enr;
	ETIM_ENRG_TIMER *	pl_ExEnr;
	struct timeval		vl_Prog;
	

   /*A si l'alocaltion memoire est ok */
   if ( ( pl_Timer = (ETIM_ENRG_TIMER *)malloc ( sizeof(ETIM_ENRG_TIMER) ) ) != NULL)
   {
      /*A copie enegistrement */
      memcpy ( pl_Timer, pa_Enr, sizeof(*pl_Timer) );
    
      /*A pour les enregistrements deja existant */
      for ( pl_Enr = pl_ExEnr = pg_ListeTimer; 
            pl_Enr != NULL; 
            pl_ExEnr = pl_Enr, pl_Enr = pl_Enr->ptrEnrSuiv )
      {
          /*A si l'horodate de declenchement est inferieure */
          if ( pl_Timer->HorodateSec < pl_Enr->HorodateSec )
   
             /*A la place d'insertion est localisee : on stoppe la recherche */
              break;
      }    

      /*A si la place d'insertion est la tete de liste */
      if ( pl_Enr == pg_ListeTimer )
      {
         /*A cas particulier de la tete de liste */
         pl_Timer->ptrEnrSuiv = pg_ListeTimer;
         pg_ListeTimer	      = pl_Timer;
      }
      /*A sinon insertion nominal */
      else
      {
         pl_Timer->ptrEnrSuiv = pl_ExEnr->ptrEnrSuiv;
         pl_ExEnr->ptrEnrSuiv = pl_Timer;
      }
   }

}



