/*E*/
/*Fichier : $Id: enav_don.c,v 1.2 1995/05/18 18:16:18 cornu Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/18 18:16:18 $
------------------------------------------------------
* GRANIOU *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENAV * FICHIER enav_don.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module decrivant les structures de donn≈es internes
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	18 Nov 1994	: Creation
* Fontaine.C	version 1.2	18 Mai 1995	: Reprise entete 
*
------------------------------------------------------*/

/* fichiers inclus */

#include "enav_don.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: enav_don.c,v 1.2 1995/05/18 18:16:18 cornu Exp $ : enav_don" ;

/* declaration de fonctions internes */

EX_FONCT_INSERER   ( ENAV_DONNEES_NAV, ed_ins_liste_NAV, pg_debutNAV, Config.Numero, Suivant, Config )
EX_FONCT_SUPPRIMER ( ENAV_DONNEES_NAV, ed_sup_liste_NAV, pg_debutNAV, Config.Numero, Suivant)	
EX_FONCT_INSERER   ( ENAV_DONNEES_TYPE_NAV, ed_ins_liste_type_NAV, pg_debutTypeNAV, Numero, Suivant, Suivant )
EX_FONCT_SUPPRIMER ( ENAV_DONNEES_TYPE_NAV, ed_sup_liste_type_NAV, pg_debutTypeNAV, Numero, Suivant)	

/* definition de fonctions externes */



