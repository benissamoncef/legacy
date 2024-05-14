/*E*/
/*  Fichier : $Id: xzat10sp.h,v 1.1 1994/11/03 14:54:04 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:54:04 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzat10sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des etats d'un equipement 
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	22/09/94	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* S≈quence d'appel
* SP	XZAT10_Eqt_A_Desactiver
* 
* Arguments en entr≈e
* XDY_Eqt	va_NumEqt_in
* XDY_Octet	va_TypeEqt_in
* 
* Arguments en sortie
* 
* Service rendu
* Met l'≈quipement » d≈sactiver
* 
* Code retour
* XDC_OK
* XDC_NOK	: equipement non trouve
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
* Update dans la table ACTIVATION
--------------------------------------------------------*/

 
#define XZAT10_Eqt_A_Desactiver	"XZAT;10" 
