/*E*/
/*  Fichier : $Id: xzac64sp.h,v 1.1 1995/04/25 11:45:24 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/04/25 11:45:24 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac64sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	24/04/95	: Creation a partir de xzac53
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Remet l'echangeur a l'etat normal
* 
* Sequence d'appel
* SP	XZAC64_Commande_Fin_Echangeur
* 
* Arguments en entree
* XDY_Eqt	va_NumeroEch_in
* XDY_Entier	va_Site_in
* XDY_Horodate	va_HeureLancementFin_in
* XDY_Booleen	va_Manuelle_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: site incorrect
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* La commande est executee si Manuelle est a XDC_Faux
* 
* Fonction
* Inserer dans la table ACTION la date de fin de l'action.
* Dans le cas de commande automatique :
* 	ACRP64 :	Appeler XZET04 pour com
------------------------------------------------------*/

 
#define XZAC64_Commande_Fin_Echangeur	"XZAC;64" 
