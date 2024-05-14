/*X  Fichier : $Id: xzao306sp.h,v 1.2 2016/06/22 17:58:25 devgfi Exp $      Release : $Revision: 1.2 $        Date : $Date: 2016/06/22 17:58:25 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao306sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	09/03/95	: Creation
* JPL	22/06/16	: Renommage suite a degroupage  1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* retourne la liste des RADT sur un troncon ou une portion
*
* Sequence d'appel
* SP    XZAO306_Liste_RADT
*
* Arguments en entree
* char(3)	va_Basedd_in
* XDY_Nom	va_NomTroncon_in
* XDY_Mot	va_NumPortion_in
*
* Arguments en sortie
*
* Liste retournee :
* XDY_Eqt		numeroEqt
* XDY_Autoroute		autoroute
* XDY_Sens		sens
* XDY_PR		PR
*
* Code retour
* XDC_OK
* <0 erreur sybase
*
* Conditions d'utilisation
* Si NomTroncon et NumPortion sont non values alors la liste des
* RADT de l'ensemble des autoroutes est retournee
* Si NomTroncon est value alors la liste des RADT du troncon est 
* retournee
* Si NumPortion est value alors la liste des RADT de la portion est 
* retournee
*
* Fonction
---------------------------------------------------*/

 
#define XZAO306_Liste_RADT	"XZAO306" 
