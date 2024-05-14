/*E*/
/*  Fichier : $Id: xzaf11sp.h,v 1.1 1995/01/18 09:42:34 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/18 09:42:34 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzaf11sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm pour la formation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	10/01/95	: Creation 
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAF11_Animer_Bouchons_Inverses
* 
* Arguments en entree
* 
* Arguments en sortie
* 
* Liste retournee :
* XDY_Entier	evenement_basc
* XDY_Octet 	cle_basc
* XDY_Entier	evenement_bou
* XDY_Octet 	cle_bou
* XDY_Mot	portion
* XDY_Entier	PR
* XDY_Octet	sens
* XDY_Octet	voie
* XDY_Entier	translation
* XDY_Entier	extension
* XDY_Octet	niveau
*
* Service rendu
* Prepare l'animation IHM pour la formation
*
* Code retour
* XDC_OK
* XDC_NOK	
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
*
---------------------------------------------------*/

 
#define XZAF11_Animer_Bouchons_Inverses	"XZAF;11" 
