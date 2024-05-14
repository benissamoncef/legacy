/*E*/
/*  Fichier : $Id: xzaf08sp.h,v 1.1 1995/01/18 09:42:17 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/18 09:42:17 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzaf08sp.prc
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
* SP	XZAF08_Animer_Voies_RADT_Inversees
* 
* Arguments en entree
* 
* Arguments en sortie
* 
* Liste retournee :
* XDY_Entier	evenement,
* XDY_Octet 	cle,
* XDY_Mot	portion,
* XDY_Entier	PR,
* XDY_Octet	sens,
* XDY_Octet	voie,
* XDY_Entier	translation
* XDY_Entier	extenxion
* XDY_Mot	station
* XDY_Sens	sens_station
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

 
#define XZAF08_Animer_Voies_RADT_Inversees	"XZAF;08" 
