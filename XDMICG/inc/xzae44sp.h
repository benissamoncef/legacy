/*E*/
/*  Fichier : $Id: xzae44sp.h,v 1.1 1994/11/03 14:53:20 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:53:20 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae44sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	30/08/94	: Creation
* C.T.	12/08/94	: Modif position param entree
*                         (test ne fonctionne plus)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs d'une fiche main courante meteo a 
* l'heure precisee
* -------------------------------------------------------
* Sequence d'appel
* SP	XZAE44_Lire_Fiche_Meteo
* 
* Arguments en entree
* XDY_Horodate		va_Horodate_in : par defaut heure courante(non utilisee)
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* 
* Arguments en sortie
* XDY_PR	va_PRFin_out
* XDY_Octet	va_Perturbation_out
* XDY_Mot	va_Visibilite_out
* XDY_Entier	va_Longueur_out
* 
* Code retour
* XDC_OK
* XDC_NOK		: pas de complement de fiche trouve
* XDC_ARG_INV        	: parametre obligatoire manquant
* <0 erreur sybase 
* 
* Conditions d'utilisation
* En formation, preciser l'horodate
* 
* Fonction
* Select dans la table METEO_HISTORIQUE
* where numero evenement and horodate
---------------------------------------------------------*/

 
#define XZAE44_Lire_Fiche_Meteo	"XZAE;44" 
