/*E*/
/*  Fichier : $Id: xzat25sp.h,v 1.1 1994/11/03 14:54:14 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:54:14 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzat25sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	22/09/94	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Met le compte rendu d'une action sur PMV en base
*
* Sequence d'appel
* SP	XZAT25_CompteRendu_PMV
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
------------------------------------------------------*/

 
#define XZAT25_CompteRendu_PMV	"XZAT;25" 
