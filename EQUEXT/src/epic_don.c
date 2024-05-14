/*E*/
/*Fichier : $Id: epic_don.c,v 1.1 2023/08/17 14:20:58 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2023/08/17 14:20:58 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER epic_don.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module de gestion des donnees de la tache TEPICTO
*
------------------------------------------------------
* HISTORIQUE :
*
* ABK   	19/12/11		: Creation (DEM/483)
*
------------------------------------------------------*/

/* fichiers inclus */

#include "epic_don.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)epic_don.c	1.1 1.1 09/14/05 : epic_don" ;

/* declaration de fonctions internes */

EX_FONCT_INSERER   ( EPIC_DONNEES_PIC, ed_ins_liste_PIC, pg_debutPIC, Config.Numero.NoEqt, Suivant, Config )
EX_FONCT_SUPPRIMER ( EPIC_DONNEES_PIC, ed_sup_liste_PIC, pg_debutPIC, Config.Numero.NoEqt, Suivant)		

/* definition de fonctions externes */



