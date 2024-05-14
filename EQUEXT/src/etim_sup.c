/*E*/
/*Fichier : $Id: etim_sup.c,v 1.5 2010/12/07 12:13:49 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/12/07 12:13:49 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TETIM * FICHIER etim_sup.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Ce module supprime une demande de reveil de la liste
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	31 Aug 1994	: Creation
* Mismer.D	version 1.2	28 Oct 1994	:
* Fontaine.C	version 1.3	23 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.5	22 Jun 1995	: Modif nom liste timers
* JMG		version 1.6	12/12/10	: portage linux
------------------------------------------------------*/

/* fichiers inclus */

#include "etim_sup.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: etim_sup.c,v 1.5 2010/12/07 12:13:49 pc2dpdy Exp $ : etim_sup";

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Dans une liste de demande de reveil, detruit un reveil
* donne selon la cle et le numero de la socket de donnee.
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/


void		es_sup_liste ( 	char 	*pa_CleProg,
				int      va_NumSock )

/*
* ARGUMENTS EN ENTREE :
*   pa_CleProg 		: Cle du reveil ( Si NULL supprimer toute les programation d'un client )
*   va_NumSock		: Numero de la socket de la tache traitement
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
*   Recherche dans la liste chainee de l'enregistrement timer a supprimer
*   Suppression de l'enregistrement
*
------------------------------------------------------*/
{
	static char *version = "$Id: etim_sup.c,v 1.5 2010/12/07 12:13:49 pc2dpdy Exp $ : es_sup_liste ";
	ETIM_ENRG_TIMER *	pl_Tempo;
	ETIM_ENRG_TIMER *	pl_ExTempo;
 
 
   /*A pour chaque enregistrement de la liste */
   for ( pl_ExTempo = pl_Tempo = pg_ListeTimer;
         pl_Tempo != NULL; 
         pl_Tempo = pl_Tempo->ptrEnrSuiv)
   {
       /*A si la tempo est localise */
	if ( ( ( pa_CleProg == NULL ) ||( !strcmp (pl_Tempo->CleProg, pa_CleProg)  ) )
         && ( pl_Tempo->NumSocket == va_NumSock ) )
       {     
          /*A supprime cette enregistrement */
          if ( (pl_ExTempo == pg_ListeTimer) && (pl_ExTempo == pl_Tempo) )
          {
	     pg_ListeTimer = pl_Tempo->ptrEnrSuiv;
	     pl_ExTempo    = pl_Tempo->ptrEnrSuiv;
          }
          else
          {
             pl_ExTempo->ptrEnrSuiv = pl_Tempo->ptrEnrSuiv;
          }
          /*A Liberation de la memoire */
          free(pl_Tempo);
          /*A Sortie de la boucle si ce n'est pas une suppression d'un client */
          if ( pa_CleProg != NULL )
          {
             break;
          }
       }
       else
       {
          pl_ExTempo = pl_Tempo;
       }
   }
   
}


