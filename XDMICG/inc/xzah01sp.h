/*E*/
/*  Fichier : $Id: xzah01sp.h,v 1.2 1996/09/06 11:22:53 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1996/09/06 11:22:53 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah01sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm
*
------------------------------------------------------
* HISTORIQUE :
*
* B.G.	13/10/94	: Creation
* niepceron	27/08/94	: Ajout du traitement par BLOC et arg d'entree devient TINYINT (1.2)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAH01_Envoyer_Bouchon_Chaussee_Portion
* 
* Arguments en entree
* XDY_Booleen	va_Init_in
* 
* Arguments en sortie (tous dans un select)
* XDY_Entier    va_NumFmc_out
* XDY_Octet     va_CleFmc_out
* XDY_Mot       va_IdPortion_out
* XDY_octet     va_TypeRaf_out XDC_AJOUT,MODIF,SUPPR
* XDY_Sens      va_Sens_out
* XDY_Entier    va_CoefTranslation_out
* XDY_Entier    va_CoefExtension_out
* XDY_Octet     va_niveau_out
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

 
#define XZAH01_Envoyer_Bouchon_Chaussee_Portion	"XZAH;01" 
