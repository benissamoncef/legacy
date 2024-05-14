/*E*/
/*  Fichier : @(#)erau_don.c	1.3      Release : 1.3        Date : 05/22/95
------------------------------------------------------
* GRANIOU *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERAU * FICHIER erau_don.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le module de definition de donnees du systeme rau
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	30 Nov 1994	: Creation
* Nagiel.E	version 1.2	24 Jan 1995	:
* Fontaine.C	version 1.3	22 Mai 1995	: Reprise entete, historique
* Diaz.L 	version 1.4 	09 Oct 1998     : Modif du au PC Niv.2
*
------------------------------------------------------*/

/* fichiers inclus */

#include "erau_don.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)erau_don.c	1.3 05/22/95 : erau_don.c" ;

/* declaration de fonctions internes */

EX_FONCT_INSERER   ( ERAU_DONNEES_PAU, ed_ins_liste_PAU, pg_debutPAU, Config.Numero, Suivant, Config )
EX_FONCT_SUPPRIMER ( ERAU_DONNEES_PAU, ed_sup_liste_PAU, pg_debutPAU, Config.Numero, Suivant)	

/* definition de fonctions externes */



