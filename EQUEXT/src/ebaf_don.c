/*E*/
/* Fichier : $Id: ebaf_don.c,v 1.1 2007/03/26 12:00:41 gesconf Exp $     Release : $Revision: 1.1 $       Date : $Date: 2007/03/26 12:00:41 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEBAF * FICHIER ebaf_don.c
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

#include "ebaf_don.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)ebaf_don.c	1.2 05/18/95 : ebaf_don" ;

/* declaration de fonctions internes */

EX_FONCT_INSERER   ( EBAF_DONNEES_BAF, ed_ins_liste_BAF, pg_debutBAF, Config.Numero, Suivant, Config )
EX_FONCT_SUPPRIMER ( EBAF_DONNEES_BAF, ed_sup_liste_BAF, pg_debutBAF, Config.Numero, Suivant)	

/* definition de fonctions externes */



