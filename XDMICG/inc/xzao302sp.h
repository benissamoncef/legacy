/*E*/
/*  Fichier : $Id: xzao302sp.h,v 1.1 1994/11/17 19:34:53 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/17 19:34:53 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao302sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	16/11/94	: Creation	(V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* retourne la liste des gares de peages
* 
* Sequence d'appel
* SP	XZAO302_Liste_Gares_Peage_Ax
* 
* Arguments en entree
* 
* Arguments en sortie
* 
* XDY_Mot		va_Numero_out		-- arguments
* XDY_Nom		va_Nom_out		-- retournes
* char(15)		va_Abrev_out		-- dans une liste
* XDY_Octet		va_Autoroute_out
* XDY_PR		va_PRSens1_out
* XDY_PR		va_PRSens2_out
* XDY_District		va_District_out
* XDY_Booleen		va_Supprime_out
* 
* Code retour
* XDC_OK
* <0 erreur sybase 
* 
* Conditions d'utilisation
* Toutes les informations sont retournees dans une liste
* 
* Fonction
* Select from RES_PEA
---------------------------------------------------*/

 
#define XZAO302_Liste_Gares_Peage_Ax	"XZAO;302" 
