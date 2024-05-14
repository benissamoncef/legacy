/*E*/
/*  Fichier : $Id: xzaf16sp.h,v 1.1 1995/01/23 17:49:02 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/23 17:49:02 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzaf16sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm pour la formation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	16/01/95	: Creation 
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAF16_Lire_Niveaux_Station
* 
* Arguments en entree
* XDC_Mot	va_Station_in	
* XDC_Horodate	va_Horodate_in	
* 
* Arguments en sortie
* XDC_Sens	va_Sens1_in	
* XDC_Octet	va_Niveau1_in	
* XDC_Sens	va_Sens2_in	
* XDC_Octet	va_Niveau2_in	
* 
* Service rendu
* Retourner les niveaux d'une station de comptage a
* la date precisee 
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Utilisable au CI uniquement
*
* Fonction
* select from MES_NIV
*
---------------------------------------------------*/

 
#define XZAF16_Lire_Niveaux_Station	"XZAF;16" 
