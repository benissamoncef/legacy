/*E*/
/* Fichier : $Id: ebad_don.c,v 1.1 2007/03/26 12:00:39 gesconf Exp $     Release : $Revision: 1.1 $       Date : $Date: 2007/03/26 12:00:39 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEBAD * FICHIER ebad_don.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module d'initialisation des structures en memoires
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	15/09/05	creation
*
------------------------------------------------------ */

/* fichiers inclus */

#include "ebad_don.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)ebad_don.c	1.2 05/18/95 : ebad_don" ;

/* declaration de fonctions internes */

EX_FONCT_INSERER   ( EBAD_DONNEES_BAD, ed_ins_liste_BAD, pg_debutBAD, Config.Numero, Suivant, Config )
EX_FONCT_SUPPRIMER ( EBAD_DONNEES_BAD, ed_sup_liste_BAD, pg_debutBAD, Config.Numero, Suivant)	

/* definition de fonctions externes */



