/*E*/
/*  Fichier : @(#)xzae91sp.prc  1.1      Release : 1.1        Date : 20/06/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae91sp.prc
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
* SP   XZAE91_Lect_Type_Libelles
*
* Arguments en entree
*
* Arguments en sortie
* smallint 	va_Notyp_out
* char(25)	va_Nom_out
* smallint 	va_Lgmax_out
* char(150)	va_Explication_out
* bit		va_Ajposs_out
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Lecture de tous les elements de la table LIB_TYP (base CFG)
-------------------------------------------------------*/

 
#define XZAE91_Lect_Type_Libelles	"XZAE;91" 
