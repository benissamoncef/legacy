/*X  Fichier : @(#)xzao312sp.h	1.1      Release : 1.1        Date : 07/05/95
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao312sp.prc
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
* Lecture de numero de sortie d'un echangeur sur reseau Escota
*
* Sequence d'appel
* SP    XZAO312_Lire_Sortie_Ech
*
* Arguments en entree
* XDY_Octet	va_NumEch_in
*
* Arguments en sortie
* XDY_Octet	va_Sortie_out
*
* Liste retournee :
*
* Code retour
* XDC_OK
* <0 erreur sybase
*
* Conditions d'utilisation
* si l'echangeur n'est pas trouve alors son numero est 
* retourne a la place du numero de sortie
*
* Fonction
---------------------------------------------------*/

 
#define XZAO312_Lire_Sortie_Ech	"XZAO;312" 
