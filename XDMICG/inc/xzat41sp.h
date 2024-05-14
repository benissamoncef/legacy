/*E*/
/*Fichier :  $Id: xzat41sp.h,v 1.1 2008/02/13 13:24:40 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:40 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzat41sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ 09/07/2007	: Creation DEM 662
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Met le compte rendu d'une action en base
* 
* Sequence d'appel
* SP	XZAT41_CompteRendu_EHT
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
* mise a jour de ACTION et appeler XZAR03 si l'action a echouee
-----------------------------------------------------*/

 
#define XZAT41_CompteRendu_LTN	"XZAT;41" 
