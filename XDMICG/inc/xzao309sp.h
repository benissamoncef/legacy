/*X  Fichier : $Id: xzao309sp.h,v 1.1 1995/03/22 14:52:35 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/03/22 14:52:35 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao309sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	22/03/95	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* retourne la liste des listes des canaux pour un local video
*
* Sequence d'appel
* SP    XZAO309_Liste_Canaux_LTV
*
* Arguments en entree
* XDY_Eqt	va_NumLTV_in
*
* Arguments en sortie
*
* Liste retournee :
* XDY_Nom		nomEqt
* char(3)		code
*
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametre d'entree non value
* <0 erreur sybase
*
* Conditions d'utilisation
*
* Fonction
---------------------------------------------------*/

 
#define XZAO309_Liste_Canaux_LTV	"XZAO;309" 
