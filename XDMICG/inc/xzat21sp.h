/*E*/
/*  Fichier : $Id: xzat21sp.h,v 1.1 1994/11/03 14:54:09 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:54:09 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzat21sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des etats d'un equipement 
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	23/09/94	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Met un equipement en service
* 
* Sequence d'appel
* SP	XZAT21_Mise_ES
* 
* Arguments en entree
* XDY_Eqt	va_NumEqt_in
* XDY_Octet	va_TypeEqt_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: equipement non trouve 
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* code de ATRP18 si pb a son appel
* 
* Conditions d'utilisation
* 
* Fonction
* mise a jour de INDISPONIBILITE
-------------------------------------------------------*/

 
#define XZAT21_Mise_ES	"XZAT;21" 
