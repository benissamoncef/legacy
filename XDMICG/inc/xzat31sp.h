/*E*/
/*  Fichier : $Id: xzat31sp.h,v 1.1 2005/09/14 14:27:17 gesconf Exp $     Release : $Revision: 1.1 $       Date : $Date: 2005/09/14 14:27:17 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzat31sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	09/06/05	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Met le compte rendu d'une action sur BAD en base
* 
* Sequence d'appel
* SP	XZAT31_CompteRendu_BAD
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

 
#define XZAT31_CompteRendu_BAD	"XZAT31" 
