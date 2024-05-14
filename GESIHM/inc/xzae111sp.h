/*E*/
/*  Fichier : $Id: xzae111sp.h,v 1.2 2009/11/05 10:34:05 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2009/11/05 10:34:05 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae111sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.	29/10/96	: Creation
* JPL	04/11/09	: Procedure degroupee (renommee sans ";") 1.2
---------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne la localisation d'une fiche main 
* courante a l'heure precisee
* -----------------------------------------------------
* Sequence d'appel
* SP	XZAE111_Lire_Localisation_FMC
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

 
#define XZAE111_Lire_Localisation_FMC	"XZAE111" 
