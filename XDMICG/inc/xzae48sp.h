/*E*/
/*  Fichier : $Id: xzae48sp.h,v 1.1 1994/11/03 14:53:24 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:53:24 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae48sp.prc
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
* Retourne les champs d'une fiche main courante basculement a l'heure precisee
* 
* Sequence d'appel
* SP	XZAE48_Lire_Fiche_Basculement
* 
* Arguments en entree
* XDY_Horodate		va_Horodate_in : par defaut heure courante 
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* 
* Arguments en sortie
* XDY_PR		va_PR_out
* XDY_Entier		va_Longueur_out
* XDY_Octet		va_VR_out
* XDY_Octet		va_VM2_out
* XDY_Octet		va_VM1_out
* XDY_Octet		va_VL_out
* XDY_Octet		va_BAU_out
* XDY_Octet		va_VR_I_out
* XDY_Octet		va_VM2_I_out
* XDY_Octet		va_VM1_I_out
* XDY_Octet		va_VL_I_out
* XDY_Octet		va_BAU_I_out
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
* Select dans la table BASCULEMENT
* where numero evenement and horodate
-------------------------------------------------*/

 
#define XZAE48_Lire_Fiche_Basculement	"XZAE;48" 
