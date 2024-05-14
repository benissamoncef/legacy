/*E*/
/*  Fichier : $Id: xzaf03sp.h,v 1.1 1995/01/18 09:41:45 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/18 09:41:45 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzaf03sp.prc
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
* SP	XZAF03_Animer_Neut_Voies
* 
* Arguments en entree
* 
* Arguments en sortie
* 
* Liste retournee :
* XDY_Entier	evenement,
* XDY_Octet 	cle,
* XDY_Octet 	debut_fin,
* XDY_Mot	portion,
* XDY_Entier	translation
* XDY_Voies	nvoie1,
* XDY_Voies	vvoie1,
* XDY_Voies	nvoie2,
* XDY_Voies	vvoie2,
* XDY_Voies	nvoie3,
* XDY_Voies	vvoie3,
* XDY_Voies	nvoie4,
* XDY_Voies	vvoie4,
* XDY_Voies	nvoie5,
* XDY_Voies	vvoie5,
* XDY_Voies	nvoie6,
* XDY_Voies	vvoie6,
* XDY_Voies	nvoie7,
* XDY_Voies	vvoie7,
* XDY_Voies	nvoie8,
* XDY_Voies	vvoie8,
* XDY_Voies	nvoie9,
* XDY_Voies	vvoie9,
* XDY_Voies	nvoie10,
* XDY_Voies	vvoie10
*
* Service rendu
* Retourne la liste des voies neutralisees pour
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

 
#define XZAF03_Animer_Neut_Voies	"XZAF;03" 
