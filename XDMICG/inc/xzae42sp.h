/*E*/
/*  Fichier : $Id: xzae42sp.h,v 1.1 1994/11/03 14:53:16 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:53:16 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae42sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture de fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	30/08/94	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs d'une fiche main courante delestage 
* 
* Sequence d'appel
* SP	XZAE42_Lire_Fiche_Delestage
* 
* Arguments en entree
* XDY_Entier		NumEvt
* XDY_Octet		CleEvt
* XDY_Horodate		Horodate : par defaut heure courante (non utilisee)
* 
* Arguments en sortie
* XDY_Nom		va_NomPlan_out
* XDY_Mot		va_NumPlan_out
* XDY_Nom		va_Hierarchie_out
* 
* Code retour
* XDC_OK
* XDC_NOK     : pas trouve de complement de fiche 
* XDC_ARG_INV : argument en entree null
* 
* Conditions d'utilisation
* En formation, preciser l'horodate
* 
* Fonction
* Select dans la table DELESTAGE
* where numero evenement and horodate
------------------------------------------------------*/

 
#define XZAE42_Lire_Fiche_Delestage	"XZAE;42" 
