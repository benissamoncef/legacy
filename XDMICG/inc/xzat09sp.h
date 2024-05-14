/*E*/
/*  Fichier : $Id: xzat09sp.h,v 1.1 1994/11/03 14:54:03 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:54:03 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzat09sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des etats d'un equipement 
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	26/09/94	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne la file d'attente d'un ≈quipement
* 
* S≈quence d'appel
* SP	XZAT09_File_Attente
* 
* Arguments en entr≈e
* XDY_Eqt	va_NumEqt_in
* XDY_Octet	va_TypeEqt_in
* 
* Arguments en sortie (Liste)
* XDY_Octet	va_Priorite_out
* XDY_Entier	va_NumEvt_out
* XDY_Octet	va_CleEvt_out
* 
* Code retour
* XDC_OK
* XDC_NOK
* 
* Conditions d'utilisation
* 
* Fonction
* select dans la table FILE_D_ATTENTE
-------------------------------------------------------*/

 
#define XZAT09_File_Attente	"XZAT;09" 
