/*X  Fichier : $Id: xzao308sp.h,v 1.1 1995/03/22 14:52:22 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/03/22 14:52:22 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao308sp.prc
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
* Suprimer un canal pour un local video existant
*
* Sequence d'appel
* SP    XZAO308_Suprimer_CanalVideo
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
* XDC_NOK	: canal video inexistant
* XDC_ARG_INV	: arguments invalides
* <0 erreur sybase
*
* Conditions d'utilisation
* Tous les parametres d'entree doivent correspondre a un canal video
*
* Fonction
---------------------------------------------------*/

 
#define XZAO308_Suprimer_CanalVideo	"XZAO;308" 
