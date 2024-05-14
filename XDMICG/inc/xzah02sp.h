/*E*/
/*  Fichier : @(#)xzah02sp.h	1.2      Release : 1.2        Date : 09/06/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah02sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm
*
------------------------------------------------------
* HISTORIQUE :
*
* CHI	17/03/05:	Creation
*
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP XZAH02_Bouchon_Chaussee_Segment
* 
* Arguments en entree
* XDY_Booleen	va_Init_in
* 
* Arguments en sortie (tous dans un select)
* XDY_Entier    va_NumFmc_out
* XDY_Octet     va_CleFmc_out
* XDY_Mot       va_IdPortion_out
* XDY_Entier    va_PR_out
* XDY_Sens      va_Sens_out
* XDY_Voie      va_Voie_out
* XDY_octet     va_TypeRaf_out XDC_AJOUT,MODIF,SUPPR
* XDY_Entier    va_CoefTranslation_out
* XDY_Entier    va_CoefExtension_out
*
* Service rendu
* Envoie È l'IHM les modifications des variables d'animation synoptique
*
* Code retour
* XDC_OK
* XDC_NOK	
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
---------------------------------------------------*/

 
#define XZAH02_Bouchon_Chaussee_Segment	"XZAH02" 
