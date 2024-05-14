/*E*/
/*  Fichier : $Id: xzae85sp.h,v 1.1 1995/04/06 16:58:47 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/04/06 16:58:47 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae85sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courantes
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	06/04/95	: Creation	(V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Controler l'existance du PR sur une autoroute
* 
* Sequence d'appel
* SP	XZAE85_Controler_Localisation
* 
* Arguments en entree
* XDY_Autoroute	va_Autoroute_in		: numero d'autoroute
* XDY_PR	va_PR_in		: pr 
* 
* Arguments en sortie
* XDY_Site	va_Site_in		: Site local
* 
* Code retour
* XDC_OK
* XDC_NOK		: probleme d'acces aux mesures
* XDC_LOC_INC		: localisation incorrecte
* XDC_ARG_INV		: parametres d'entree null
* 
* Conditions d'utilisation
*
* Fonction
-------------------------------------------------------*/

 
#define XZAE85_Controler_Localisation	"XZAE;85" 
