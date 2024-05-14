/*E*/
/*  Fichier : $Id: xzaf05sp.h,v 1.1 1995/01/18 09:41:57 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/18 09:41:57 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzaf05sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm pour la formation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	11/01/95	: Creation 
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAF05_Animer_Bouchon
* 
* Arguments en entree
* 
* Arguments en sortie
* 
* Liste retournee :
* XDY_Entier	evenement
* XDY_Octet 	cle
* XDY_Mot	portion
* XDY_Octet 	sens
* XDY_Entier	translation
* XDY_Entier	extension
* XDY_Octet	niveau
*
* Service rendu
* Retourne la liste des bouchons pour
* l'animation IHM pour la formation
*
* Code retour
* XDC_OK
* XDC_NOK	
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
*
---------------------------------------------------*/

 
#define XZAF05_Animer_Bouchon	"XZAF;05" 
