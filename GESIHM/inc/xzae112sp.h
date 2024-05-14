/*X  Fichier : $Id: xzae112sp.h,v 1.2 2007/04/16 19:05:16 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2007/04/16 19:05:16 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae112sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	27/11/96	: Creation (1106)
* JPL	16/04/07	: Changement de nom de la procedure v1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Rechercher les liste des types de fmc triee suivant leur
* position 
*
* Sequence d'appel
* SP    XZAE112_Liste_Types_FMC_Tries
*
* Arguments en entree
*
* Arguments en sortie
* XDY_Octet	va_Type_out
* XDY_Octet	va_Classe_out
* varchar(100)	va_Nom_out
* char(10)	va_Abreviation_out
* XDY_Booleen	va_Operation_out
* XDY_Booleen	va_Supprime_out
*
* Code retour
* XDC_OK
* <0 erreur sybase
*
* Conditions d'utilisation
*
* Fonction
---------------------------------------------------*/

 
#define XZAE112_Liste_Types_FMC_Tries	"XZAE112"
