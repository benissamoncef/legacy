/*E*/
/*Fichier : $Id: eprv_don.c,v 1.1 2012/07/04 14:20:58 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 14:20:58 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER eprv_don.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module de gestion des donnees de la tache TEPRV
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	19/12/11		: Creation (DEM/1016)
*
------------------------------------------------------*/

/* fichiers inclus */

#include "eprv_don.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)eprv_don.c	1.1 1.1 09/14/05 : eprv_don" ;

/* declaration de fonctions internes */

EX_FONCT_INSERER   ( EPRV_DONNEES_PRV, ed_ins_liste_PRV, pg_debutPRV, Config.Numero.NoEqt, Suivant, Config )
EX_FONCT_SUPPRIMER ( EPRV_DONNEES_PRV, ed_sup_liste_PRV, pg_debutPRV, Config.Numero.NoEqt, Suivant)		

/* definition de fonctions externes */



