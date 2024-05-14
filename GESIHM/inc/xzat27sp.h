/*E*/
/*  Fichier : $Id: xzat27sp.h,v 1.1 1998/01/20 14:27:23 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1998/01/20 14:27:23 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzat27sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	16/01/98	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Met le compte rendu d'une action sur PAL en base
* 
* Sequence d'appel
* SP	XZAT27_CompteRendu_PAL
* 
* Arguments en entree
* XDY_Mot	va_NumAction_in
* XDY_Horodate	va_Horodate_in
* XDY_Booleen	va_ResultatAction_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: action non trouvee ou 
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* code d'erreur de XZAR03 si pb a son appel
* 
* Conditions d'utilisation
* 
* Fonction
* mise È jour de ACTION et appeler XZAR03 si l'action a ÅchouÅe
-----------------------------------------------------------------------*/

 
#define XZAT27_CompteRendu_PAL	"XZAT;27" 
