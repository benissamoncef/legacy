/*E*/
/*  Fichier : @(#)xzae94sp.prc  1.1      Release : 1.1        Date : 20/06/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae94sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des types de libelles et des libelles predefinis
*
------------------------------------------------------
* HISTORIQUE :
*
* P.V.  20/06/96        : Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE94_MAJ_LIB
*
* Arguments en entree
* smallint   	va_Notyp_in
* int 		va_Code_in
* smallint 	va_Position_in
* char(100)	va_Libelle_in
* char(100)	va_Explication_in
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
* Mise a jour d'un element de la table LIB_PRE (base CFG)
-------------------------------------------------------*/

 
#define XZAE94_MAJ_LIB	"XZAE;94" 
