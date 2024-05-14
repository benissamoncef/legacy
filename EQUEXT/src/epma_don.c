/*E*/
/*Fichier : $Id: epma_don.c,v 1.1 2005/09/14 15:11:13 gesconf Exp $  Release : $Revision: 1.1 $      Date : $Date: 2005/09/14 15:11:13 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER epma_don.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module de gestion des donnees de la tache TEPMVA
*
------------------------------------------------------
* HISTORIQUE :
*
* jmg	10/06/05	creation
*
------------------------------------------------------*/

/* fichiers inclus */

#include "epma_don.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: epma_don.c,v 1.1 2005/09/14 15:11:13 gesconf Exp $ $Revision: 1.1 $ $Date: 2005/09/14 15:11:13 $ : epma_don" ;

/* declaration de fonctions internes */

EX_FONCT_INSERER   ( EPMVA_DONNEES_PMVA, ed_ins_liste_PMVA, pg_debutPMVA, Config.Numero.NoEqt, Suivant, Config )
EX_FONCT_SUPPRIMER ( EPMVA_DONNEES_PMVA, ed_sup_liste_PMVA, pg_debutPMVA, Config.Numero.NoEqt, Suivant)		

/* definition de fonctions externes */



