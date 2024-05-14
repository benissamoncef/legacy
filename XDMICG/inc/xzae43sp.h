/*E*/
/*X  Fichier : $Id: xzae43sp.h,v 1.1 1994/11/03 14:53:17 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:53:17 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae43sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	30/08/94	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs d'une fiche main courante poste operateur
* 
* Sequence d'appel
* SP	XZAE43_Lire_Fiche_Poste_Operateur
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* XDY_Horodate		va_Horodate_in : par defaut heure courante(non utilisee)
* 
* Arguments en sortie
* XDY_Nom		va_Operateur_out
* 
* Code retour
* XDC_OK
* XDC_NOK     : pas de complement trouve
* XDC_ARG_INV : arguments en entree obligatoires non values
* 
* Conditions d'utilisation
* En formation, preciser l'horodate
* 
* Fonction
* Select dans la table POSTE_OPERATEUR
* where numero evenement and horodate
------------------------------------------------------*/

 
#define XZAE43_Lire_Fiche_Poste_Operateur	"XZAE;43" 
