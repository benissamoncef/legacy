/*E*/
/*Fichier : @(#)etel_don.c	1.2      Release : 1.2        Date : 05/04/95
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  EQUEXT
------------------------------------------------------
* MODULE TETEL * FICHIER etel_don.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Declaration des fonctions utilisees dans le module TETEL
*
*
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille.M	Version 1.1	30 Nov 1994	: Creation
* Fontaine.C    Version 1.2     03 Mai 1995     : Reprise entete
* Diaz.L 	version 1.3     09 Oct 1998     : Suppresion de traces du PABX
*
------------------------------------------------------*/

/* fichiers inclus */

#include "etel_don.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)etel_don.c	1.2 05/04/95 : etel_don" ;

/* declaration de fonctions internes */

EX_FONCT_INSERER   ( ETEL_DONNEES_MAA, ed_ins_liste_MAA, pg_debutMAA, Config.Numero, Suivant, Config )
EX_FONCT_SUPPRIMER ( ETEL_DONNEES_MAA, ed_sup_liste_MAA, pg_debutMAA, Config.Numero, Suivant)	
EX_FONCT_INSERER   ( ETEL_DONNEES_AUTOTEL, ed_ins_liste_AUTOTEL, pg_debutAUTOTEL, Config.Numero, Suivant, Config)
EX_FONCT_SUPPRIMER ( ETEL_DONNEES_AUTOTEL, ed_sup_liste_AUTOTEL, pg_debutAUTOTEL, Config.Numero, Suivant)	

/* definition de fonctions externes */



