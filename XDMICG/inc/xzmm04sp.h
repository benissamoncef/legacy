/*E*/
/*  Fichier : $Id: xzmm04sp.h,v 1.1 1995/09/05 18:44:33 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/09/05 18:44:33 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzmm04sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Maintenance
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	28/08/95	: Creation	(V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Inserer un equipement 
* 
* Sequence d'appel
* SP	XZMM04_Existance_Eqt_Easycam
* 
* Arguments en entree
* XDC_Eqt	va_NumEqt_in
* XDC_Octet	va_TypeEqt_in
* 
* Arguments en sortie
* XDC_Entier	va_NumEasyCam_in
*
* Code retour
* XDC_OK
* XDC_NOK	: equipement inexitant
* XDC_ARG_INV	: parametre d'entree incorrect
* 
* Conditions d'utilisation
* 
* Fonction
-------------------------------------------------------*/

 
#define XZMM04_Existance_Eqt_Easycam	"XZMM;04" 
