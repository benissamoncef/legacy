/*E*/
/*  Fichier : $Id: xzaf10sp.h,v 1.1 1995/01/18 09:42:29 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/18 09:42:29 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzaf10sp.prc
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
* SP	XZAF10_Rechercher_Evts_Actions
* 
* Arguments en entree
* XDY_Horodate va_Horodate_in 
* 
* Arguments en sortie
* 
* Service rendu
* Rechercher tous les actions et evenements en cours a
* la date precisee et clos actuellement
*
* Code retour
* XDC_OK
* XDC_NOK	
* XDC_ARG_INV	: argument invalide	
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
*
---------------------------------------------------*/

 
#define XZAF10_Rechercher_Evts_Actions	"XZAF;10" 
