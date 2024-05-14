/*E*/
/*Fichier : $Id: epmv_don.c,v 1.2 1995/05/18 14:48:21 cornu Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/18 14:48:21 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER epmv_don.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module de gestion des donnees de la tache TEPMV
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	06 Sep 1994	: Creation
* Fontaine.C	version 1.2	17 Mai 1995	: Reprise entete
*
------------------------------------------------------*/

/* fichiers inclus */

#include "epmv_don.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: epmv_don.c,v 1.2 1995/05/18 14:48:21 cornu Exp $ : epmv_don" ;

/* declaration de fonctions internes */

EX_FONCT_INSERER   ( EPMV_DONNEES_PMV, ed_ins_liste_PMV, pg_debutPMV, Config.Numero.NoEqt, Suivant, Config )
EX_FONCT_SUPPRIMER ( EPMV_DONNEES_PMV, ed_sup_liste_PMV, pg_debutPMV, Config.Numero.NoEqt, Suivant)	
EX_FONCT_INSERER   ( EPMV_DONNEES_Picto, ed_ins_liste_Picto, pg_debutPicto, Config.Numero.NoEqt, Suivant, Config)
EX_FONCT_SUPPRIMER ( EPMV_DONNEES_Picto, ed_sup_liste_Picto, pg_debutPicto, Config.Numero.NoEqt, Suivant)	
EX_FONCT_INSERER   ( EPMV_DONNEES_TYPE_PMV, ed_ins_liste_TypePMV, pg_debutTypePMV, TypePMV, Suivant, Suivant)
EX_FONCT_SUPPRIMER ( EPMV_DONNEES_TYPE_PMV, ed_sup_liste_TypePMV, pg_debutTypePMV, TypePMV, Suivant)	
EX_FONCT_INSERER   ( EPMV_DONNEES_TYPE_Picto, ed_ins_liste_TypePicto, pg_debutTypePicto, TypePicto, Suivant, Suivant)
EX_FONCT_SUPPRIMER ( EPMV_DONNEES_TYPE_Picto, ed_sup_liste_TypePicto, pg_debutTypePicto, TypePicto, Suivant)	

/* definition de fonctions externes */



