/*E*/
/*  Fichier : $Id: xzae46sp.h,v 1.1 1994/11/03 14:53:22 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:53:22 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae46sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	31/08/94	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs d'une fiche main courante vehicule lent a l'heure precisee
* 
* Sequence d'appel
* SP	XZAE46_Lire_Fiche_VehLent
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* XDY_Horodate		va_Horodate_in : par defaut heure courante
* 
* Arguments en sortie
* XDY_Mot	va_Longueur_out
* XDY_Octet	va_Vitesse_out
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de complement de fiche main courante
* XDC_ARG_INV
* 
* Conditions d'utilisation
* En formation, preciser l'horodate
* 
* Fonction
* Select dans la table VEHICULE_LENT
* where numero evenement and horodate
-----------------------------------------------------------------------*/

 
#define XZAE46_Lire_Fiche_VehLent	"XZAE;46" 
