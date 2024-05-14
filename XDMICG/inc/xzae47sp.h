/*E*/
/*  Fichier : $Id: xzae47sp.h,v 1.1 1994/11/03 14:53:23 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:53:23 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae47sp.prc
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
* Retourne les champs d'une fiche main courante manifestation 
* 
* Sequence d'appel
* SP	XZAE47_Lire_Fiche_Manifestation
* 
* Arguments en entree
* XDY_Horodate		va_Horodate_in : par defaut heure courante (non utilisee)
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* 
* Arguments en sortie
* XZAET_Manifestation	Description
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de complement de fiche main courante
* XDC_ARG_INV
* < 0 erreur sybase
* 
* Conditions d'utilisation
* 
* Fonction
* Select dans la table MANIFESTATION
* where numero evenement and horodate
------------------------------------------------------*/

 
#define XZAE47_Lire_Fiche_Manifestation	"XZAE;47" 
