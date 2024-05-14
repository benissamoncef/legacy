/*Fichier : $Id: xzat5201sp.h
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzat5201sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
*  GGY	13/11/23	: Creation (DEM483)
-----------------------------------------------------*/

/*-----------------------------------------------------
* Service rendu
* Met le compte rendu d'une action sur PIC en base
*
* Sequence d'appel
* SP	XZAT5201_CompteRendu_PIC
* 
* Arguments en entree
* XDY_Mot		va_NumAction_in
* XDY_Horodate	va_Horodate_in
* XDY_Booleen	va_ResultatAction_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK		: action non trouvee ou 
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* code d'erreur de XZAR03 si pb a son appel
* 
* Conditions d'utilisation
* 
* Fonction
* mise a jour de ACTION et appeler XZAR03 si l'action a echouee
------------------------------------------------------*/

 
#define XZAT5201_CompteRendu_PIC	"XZAT5201" 
