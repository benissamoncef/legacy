/*E*/
/* Fichier : $Id: epal_don.c,v 1.1 1998/01/30 15:18:58 mismer Exp $     Release : $Revision: 1.1 $      Date :  $Date: 1998/01/30 15:18:58 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPAL * FICHIER epal_don.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module decrivant les structures de donn≈es internes
*
------------------------------------------------------
* HISTORIQUE :
*
* D.Mismer	version 1.1	14 Jan 1998	: Creation
*
------------------------------------------------------*/

/* fichiers inclus */

#include "epal_don.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: epal_don.c,v 1.1 1998/01/30 15:18:58 mismer Exp $ : epal_don" ;

/* declaration de fonctions internes */

EX_FONCT_INSERER   ( EPAL_DONNEES_PAL, ed_ins_liste_PAL, pg_debutPAL, Config.Numero, Suivant, Config )
EX_FONCT_SUPPRIMER ( EPAL_DONNEES_PAL, ed_sup_liste_PAL, pg_debutPAL, Config.Numero, Suivant)	

/* definition de fonctions externes */



