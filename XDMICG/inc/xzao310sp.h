/*X  Fichier : $Id: xzao310sp.h,v 1.2 2018/10/09 13:27:29 devgfi Exp $      Release : $Revision: 1.2 $        Date : $Date: 2018/10/09 13:27:29 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao310sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	07/06/95	: Creation
* JPL	08/10/18	: Degroupee  1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* retourne les informations sur une station RADT
*
* Sequence d'appel
* SP    XZAO310_Lire_PointDeMesure
*
* Arguments en entree
* XDY_Eqt	va_NumStation_in
*
* Arguments en sortie
*
* Liste retournee :
* XDY_Sens		sens
* XDY_Voie		voie
*
* Code retour
* XDC_OK
* <0 erreur sybase
*
* Conditions d'utilisation
*
* Fonction
---------------------------------------------------*/

 
#define XZAO310_Lire_PointDeMesure	"XZAO310" 
