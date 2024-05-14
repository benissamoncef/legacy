/*E*/
/*  Fichier : $Id: xzae52sp.h,v 1.1 1994/11/03 14:53:31 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:53:31 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae52.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	31/08/94	: Creation
* C.T.	12/09/94	: Modif position param entree
*                         test ne fonctionne plus
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs d'une fiche main courante fermeture echangeur
* 
* Sequence d'appel
* SP	XZAE52_Lire_Fiche_Fermeture_Echangeur
* 
* Arguments en entree
* XDY_Horodate		va_Horodate_in : par defaut heure courante(non utilisee)
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* 
* Arguments en sortie
* XDY_Booleen	va_Entree1_out
* XDY_Booleen	va_Entree2_out
* XDY_Booleen	va_Sortie1_out
* XDY_Booleen	va_Sortie2_out
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de complement de fiche main courante
* XDC_ARG_INV
* < 0 erreur sybase
* 
* Conditions d'utilisation
* En formation, preciser l'horodate
* 
* Fonction
* Select dans la table FERMETURE_ECHANGEUR
* where numero evenement and horodate
----------------------------------------------------*/

 
#define XZAE52_Lire_Fiche_Fermeture_Echangeur	"XZAE;52" 
