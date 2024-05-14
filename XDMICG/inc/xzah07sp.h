/*E*/
/*  Fichier : $Id: xzah07sp.h,v 1.2 1996/09/06 11:23:04 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1996/09/06 11:23:04 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah07sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm
*
------------------------------------------------------
* HISTORIQUE :
*
* B.G.	14/10/94	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP XZAH07_Envoyer_Objet_Mobile_Portion
* 
* Arguments en entree
* XDY_Booleen	va_Init_in
* 
* Arguments en sortie (tous dans un select)
* XDY_Entier    va_NumFmc_out
* XDY_Octet     va_CleFmc_out
* XDY_Mot       va_IdPortion_out
* XDY_octet     va_TypeRaf_out XDC_AJOUT,MODIF,SUPPR
* XDY_Entier    va_NumAction_out
* XDY_Octet     va_CleAction_out
* XDY_Octet     va_TypeObjet_out
* XDY_Sens      va_Sens_out
* XDY_Voie      va_Voie_out
* XDY_Entier    va_CoefTranslation_out
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

 
#define XZAH07_Envoyer_Objet_Mobile_Portion	"XZAH;07" 
