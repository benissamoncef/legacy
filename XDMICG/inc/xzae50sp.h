/*E*/
/*  Fichier : $Id: xzae50sp.h,v 1.1 1994/11/03 14:53:27 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:53:27 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae50.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	31/08/94	: Creation
* C.T.	12/08/94	: Modif position param entree
*                         (test ne fonctionne plus)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs d'une fiche main courante incident sous concessionnaire
* 
* Sequence d'appel
* SP	XZAE50_Lire_Fiche_Sous_Concess
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* XDY_Horodate		va_Horodate_in : par defaut heure courante(non utilisee)
* 
* Arguments en sortie
* XDY_Nom	va_Nom_out
* XDY_Booleen	va_Ferme_out
* XDY_Booleen	va_Penurie_out
* XDY_Octet	va_Carburant_out
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de complement de fiche main courante
* XDC_ARG_INV
* <0 erreur sybase 
* 
* Conditions d'utilisation
* En formation, preciser l'horodate
* 
* Fonction
* Select dans la table SS_CONCESSIONNAIRE
* where numero evenement and horodate
------------------------------------------------------*/

 
#define XZAE50_Lire_Fiche_Sous_Concess	"XZAE;50" 
