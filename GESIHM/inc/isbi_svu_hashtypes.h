/*E Fichier : $Id: isbi_svu_hashtypes.h,v 1.2 2012/05/29 10:08:13 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2012/05/29 10:08:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GESIHM
------------------------------------------------------
* MODULE MSVU * FICHIER isyn_svu_hashtypes.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 
*   Module d'affichage des Vues Synoptiques : 
*     Definitions des types pour la table de Hash de 
*     Variable d'Animation Graphique.
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	15 Sep 1994	: Creation
------------------------------------------------------ */

#ifndef MSVU_hashtypes
#define MSVU_hashtypes

/* fichiers inclus */



/* definitions de constantes exportees */
#define CM_LONG_NOMVUE	80
#define CM_LONG_NOMDS	50
#define CM_LONG_NOMVAG	80

typedef void* HASHTABLE;
typedef void* HASHNODE;
typedef void* KEY;
typedef void* VALUE;

#endif
