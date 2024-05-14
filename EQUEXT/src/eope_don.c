/*E*/
/*Fichier : $Id: eope_don.c,v 1.2 1995/05/04 20:12:25 cornu Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/04 20:12:25 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  EQUEXT
------------------------------------------------------
* MODULE eope_don * FICHIER eope_don.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Declaration des fonctions utilisees par le module TEOPE
*
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille.M	Version 1.1	02 Dec 1994	: Creation
* Fontaine.C    Version 1.2     04 Mai 1995     : Reprise entete
------------------------------------------------------*/

/* fichiers inclus */

#include "eope_don.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eope_don.c,v 1.2 1995/05/04 20:12:25 cornu Exp $ : eope_don" ;

/* declaration de fonctions internes */

EX_FONCT_INSERER   ( EOPE_DONNEES_OPER, ed_ins_liste_OPER, pg_debutOPER, Config.Numero, Suivant, Config )
EX_FONCT_SUPPRIMER ( EOPE_DONNEES_OPER, ed_sup_liste_OPER, pg_debutOPER, Config.Numero, Suivant)	

/* definition de fonctions externes */



