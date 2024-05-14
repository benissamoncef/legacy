/*X  Fichier : @(#)xzao311sp.h	1.1      Release : 1.1        Date : 07/05/95
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao311sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	07/06/95	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Ecrire le numero de sortie echangeur sur reseau Escota
*
* Sequence d'appel
* SP    XZAO311_Ecrire_Sortie_Ech
*
* Arguments en entree
* XDY_Octet	va_NumEch_in
* XDY_Octet	va_Sortie_in
*
* Arguments en sortie
*
* Liste retournee :
*
* Code retour
* XDC_OK
* XDC_NOK	: insert ou update impossible
* <0 erreur sybase
*
* Conditions d'utilisation
*
* Fonction
---------------------------------------------------*/

 
#define XZAO311_Ecrire_Sortie_Ech	"XZAO;311" 
