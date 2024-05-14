/*E*/
/*  Fichier : @(#)xzae940sp.prc  1.1      Release : 1.1        Date : 20/06/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae940sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des types de libelles et des libelles predefinis
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL	13/04/18        : Creation CTRL_FEUX 1.1 DEM1284
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE940_MAJ_LIB_PLAN_DE_FEUX
*
* Arguments en entree
* smallint   	va_NumeroCFE_in
* int 		va_Code_in
* char(150)	va_Libelle_in
* bit 		va_Valide_in
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* XDC_NOK
* XDC_ARG_INV   : parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Mise a jour d'un element de la table DEF_PDF (base CFG)
-------------------------------------------------------*/

 
#define XZAE940_MAJ_LIB_PLAN_DE_FEUX	"XZAE940" 
