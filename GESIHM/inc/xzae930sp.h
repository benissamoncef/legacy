/*E*/
/*  Fichier : @(#)xzae930sp.prc  1.1      Release : 1.1        Date : 20/06/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae930sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des types de libelles et des libelles de plans de feux
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL  16/04/18        : Creation CTRL_FEUX 1.1 DEM1284
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE930_Creation_Lib_Plan_De_Feux
*
* Arguments en entree
* smallint   	va_NumeroPlanFeux_in
* int 		va_Code_in
* char(100)	va_Libelle_in
* bit 		va_Valide_in
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* XDC_ARG_INV   : parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Ajout d'un enregistrement dans la table LIB_PRE (base CFG)
-------------------------------------------------------*/

 
#define XZAE930_CREATION_LIB_PLAN_DE_FEUX	"XZAE930" 
