/*E*/
/*  Fichier : $Id: xzae49sp.h,v 1.2 2012/05/29 14:34:08 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2012/05/29 14:34:08 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae49sp.prc
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
* JMG   29/05/12        : suppression ';' 1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs d'une fiche main courante travaux a l'heure precisee
* 
* Sequence d'appel
* SP	XZAE49_Lire_Fiche_Travaux
* 
* Arguments en entree
* XDY_Horodate		va_Horodate_in : par defaut heure courante (non utilisee)
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* 
* Arguments en sortie
* XDY_Booleen		va_Nature_out
* XDY_Octet		va_Type_out
* XDY_Booleen		va_Continu_out
* XDY_Horodate		va_HoroDebC_out
* XDY_Horodate		va_HoroFinC_out
* XDY_Booleen		va_Mobile_out
* XDY_Autoroute		va_Autoroute_out
* XDY_PR		va_PRteteM_out
* XDY_PR		va_PRqueueM_out
* XDY_Sens		va_Sens_out
* XDY_PR		va_PRtete_out
* XDY_Booleen		va_VoieRet_out
* XDY_Entier		va_Longueur_out
* XDY_Entier		va_LongueurM_out
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
* Select dans la table TRAVAUX et dans la table TRAVAUX_HISTORIQUE
* where numero evenement and horodate
-----------------------------------------------------------------------*/

 
#define XZAE49_Lire_Fiche_Travaux	"XZAE49" 
