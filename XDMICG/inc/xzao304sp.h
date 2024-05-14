/*E*/
/*  Fichier : $Id: xzao304sp.h,v 1.1 1994/11/17 19:35:07 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/17 19:35:07 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao304sp.prc
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
* retourne la liste des lits d'arret
* 
* Sequence d'appel
* SP	XZAO304_Liste_Lits_Arret_Ax
* 
* Arguments en entree
* 
* Arguments en sortie
* 
* XDY_Octet		va_Numero_out		-- arguments
* XDY_Nom		va_Nom_out		-- retournes
* XDY_Octet		va_Autoroute_out	-- dans une liste
* XDY_PR		va_PR_out
* XDY_Sens		va_Sens_out
* XDY_District		va_District_out
* XDY_Booleen		va_Supprime_out
* 
* Code retour
* XDC_OK
* <0 erreur sybase 
* 
* Conditions d'utilisation
* Toutes les informations sont retournees dans une liste
* L'identicateur de suppression est toujours positionne a faux
* 
* Fonction
* Select from EQT_GEN
---------------------------------------------------*/

 
#define XZAO304_Liste_Lits_Arret_Ax	"XZAO;304" 
