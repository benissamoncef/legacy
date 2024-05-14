/*E Fichier : $Id: exa0_tra.h,v 1.2 1995/05/23 18:39:15 cornu Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/23 18:39:15 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEXA0 * FICHIER exa0_tra.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de traitement de la
*  tache TEXA0, protocole de la gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	23 Nov 1994	: Creation
* Fontaine.C	version 1.2	23 Mai 1995	: Reprise entete
*
------------------------------------------------------ */

#ifndef exa0_tra
#define exa0_tra

/* fichiers inclus */

#include "exa0_don.h"
#include "exa0_cfg.h"
#include "exa0_ctl.h"
#include "exa0_cmd.h"


/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
  
/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Traitement des info recu de AprilNet
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void ea_traitement_April ( EXA0_AST_PARAM *	pa_AstParam );

/*
* ARGUMENTS EN ENTREE :
*   
* EXA0_AST_PARAM *	pa_AstParam  : structure evenements AprilNet
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* execute sur evenements AprilNet
*
* FONCTION 
*
*  Recherche du flag evenement et appel de la fonction de lecture automate
*
------------------------------------------------------*/






#endif
