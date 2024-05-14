/*E*/
/*  Fichier : $Id: xzao303sp.h,v 1.1 1994/11/17 19:35:00 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/17 19:35:00 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao303sp.prc
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
* retourne la liste des aires
* 
* Sequence d'appel
* SP	XZAO303_Liste_Aires_Ax
* 
* Arguments en entree
* 
* Arguments en sortie
* 
* XDY_Octet		va_Numero_out		-- arguments
* XDY_Nom		va_Nom_out		-- retournes
* char(15)		va_Abrev_out		-- dans une liste
* XDY_Octet		va_Autoroute_out
* XDY_PR		va_PREntree_out
* XDY_PR		va_PRSortie_out
* XDY_Sens		va_Sens_out
* XDY_Mot		va_CapaciteVL_out
* XDY_Mot		va_CapacitePL_out
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
* Select from RES_AIR
---------------------------------------------------*/

 
#define XZAO303_Liste_Aires_Ax	"XZAO;303" 
