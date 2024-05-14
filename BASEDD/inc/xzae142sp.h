/*E*/
/*  Fichier : @(#)xzae111sp.h	1.1      Release : 1.1        Date : 11/29/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae142sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture  des evenements strada en base
*
------------------------------------------------------
* HISTORIQUE :
*
*  C. Penot	05 nov 1997	: Creation
---------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* -----------------------------------------------------
* Sequence d'appel
* SP	XZAE142_Liste_Evenements
* 
* Arguments en entree
* XDY_Horodate	va_Horodate_in : par defaut heure courante
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* 
* Arguments en sortie
* XDY_Autoroute	va_Autoroute_out
* XDY_PR	va_PR_out
* XDY_PR	va_PRtete_out
* XDY_Sens	va_Sens_out
*
*
* 
* Code retour
* XDC_OK
* XDC_NOK	: fiche non trouvee
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* 
* Fonction
* Select dans les tables EVENEMENT, EVENEMENT_HISTORIQUE, 
* ALERTE, COMMENTAIRES where numero evenement and horodate
----------------------------------------------------*/

 
#define XZAE142_Liste_Evenements	"XZAE142" 
