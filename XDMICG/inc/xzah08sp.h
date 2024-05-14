/*E*/
/*  Fichier : $Id: xzah08sp.h,v 1.2 1996/09/06 11:23:07 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1996/09/06 11:23:07 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah08sp.prc
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
* SP XZAH08_Envoyer_Objet_Mobile_Aff_Voies
* 
* Arguments en entree
* XDY_Booleen	va_Init_in
* 
* Arguments en sortie (tous dans un select)
* XDY_Entier    va_NumFmc_out
* XDY_Octet     va_CleFmc_out
* XDY_Booleen   va_DebFin
* XDY_Mot       va_IdPortion_out
* XDY_octet     va_TypeRaf_out XDC_AJOUT,MODIF,SUPPR
* XDY_Entier    va_CoefTranslation_out
* XDY_Octet     va_NVoie1_out
* XDY_Octet     va_AVoie1_out
* XDY_Octet     va_NVoie2_out
* XDY_Octet     va_AVoie2_out
* XDY_Octet     va_NVoie3_out
* XDY_Octet     va_AVoie3_out
* XDY_Octet     va_NVoie4_out
* XDY_Octet     va_AVoie4_out
* XDY_Octet     va_NVoie5_out
* XDY_Octet     va_AVoie5_out
* XDY_Octet     va_NVoie6_out
* XDY_Octet     va_AVoie6_out
* XDY_Octet     va_NVoie7_out
* XDY_Octet     va_AVoie7_out
* XDY_Octet     va_NVoie8_out
* XDY_Octet     va_AVoie8_out
* XDY_Octet     va_NVoie9_out
* XDY_Octet     va_AVoie9_out
* XDY_Octet     va_NVoie10_out
* XDY_Octet     va_AVoie10_out
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

 
#define XZAH08_Envoyer_Objet_Mobile_Aff_Voies	"XZAH;08" 
