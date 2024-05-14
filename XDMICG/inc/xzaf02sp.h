/*E*/
/*  Fichier : $Id: xzaf02sp.h,v 1.1 1995/01/18 09:41:38 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/18 09:41:38 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzaf02sp.prc
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
* SP	XZAF02_Animer_Objet_Mobile
* 
* Arguments en entree
* XDY_Horodate  va_Horodate_in
* 
* Arguments en sortie
* 
* Liste retournee :
* XDY_Entier	evenement,
* XDY_Octet 	cle,
* XDY_Mot	portion,
* XDY_Entier	actio,
* XDY_Entier 	cle_action,
* XDY_Octet	type_objet,
* XDY_Octet	sens,
* XDY_Octet	voie,
* XDY_Entier	translation
*
* Service rendu
* Prepare l'animation IHM
*
* Code retour
* XDC_OK
* XDC_NOK	
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
*
---------------------------------------------------*/

 
#define XZAF02_Animer_Objet_Mobile	"XZAF;02" 
