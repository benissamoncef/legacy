/*X  Fichier : $Id: xzao307sp.h,v 1.1 1995/03/22 14:52:16 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/03/22 14:52:16 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao307sp.prc
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
* inserer un canal pour un local video existant
*
* Sequence d'appel
* SP    XZAO307_Ecrire_CanalVideo
*
* Arguments en entree
* XDY_Eqt	va_NumLTV_in
* XDY_Nom	va_NomCanal_in
* char(3)	va_CodeCanal_in
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* XDC_NOK	: local video inexistant ou insertion impossible
* XDC_ARG_INV	: arguments invalides
* <0 erreur sybase
*
* Conditions d'utilisation
* Le nom du canal et son code doivent etre values et differents de " "
* La description donnee ne doit pas correspondre a un canal existant
* pour un meme local video
*
* Fonction
---------------------------------------------------*/

 
#define XZAO307_Ecrire_CanalVideo	"XZAO;307" 
