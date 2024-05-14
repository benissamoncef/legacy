/*E*/
/*  Fichier : $Id: xzat04sp.h,v 1.1 1994/11/03 14:54:02 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:54:02 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzat04sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des etats d'un equipement 
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	23/09/94	: Creation
* C.T.	29/09/94	: Enlever la mise en commentaires
*                         de l'appel a la RP 
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAT04_Lever_Inhibition
* 
* Arguments en entree
* XDY_TypeEqt	va_Type_in
* XDY_Eqt		va_NumEqt_in
* 
* Arguments en sortie
* 
* Service rendu
* Supprime l'indicateur d'inhibition.
* 
* Code retour
* XDC_OK
* XDC_NOK	: equipement non trouve  ou
*                 action interdite sur ce site
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* code de ATRP18 si pb a son appel
* 
* Conditions d'utilisation
* Operateur district
* 
* Fonction
* insert INDISPONIBILITE
-------------------------------------------------------*/

 
#define XZAT04_Lever_Inhibition	"XZAT;04" 
