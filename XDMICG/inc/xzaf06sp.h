/*E*/
/*  Fichier : $Id: xzaf06sp.h,v 1.2 1995/06/08 17:17:13 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/06/08 17:17:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzaf06sp.prc
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
* SP	XZAF06_Animer_Voies_Neutralisees
* 
* Arguments en entree
* 
* Arguments en sortie
* 
* Liste retournee :
* XDY_Entier	evenement,
* XDY_Octet 	cle,
* XDY_Mot	portion,
* XDY_PR	PR,
* XDY_Octet	sens,
* XDY_Octet	voie,
* XDY_Entier	translation
* XDY_Entier	extension
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

 
#define XZAF06_Animer_Voies_Neutralisees	"XZAF;06" 
