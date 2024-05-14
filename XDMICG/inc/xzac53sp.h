/*E*/
/*  Fichier : $Id: xzac53sp.h,v 1.1 1994/11/03 14:53:05 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:53:05 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac53sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	21/09/94	: Creation
* C.T.	22/09/94	: Modif numero de commande 
*                         pour ACRP08 est negatif (V 1.3)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Remet l'echangeur a l'etat normal
* 
* Sequence d'appel
* SP	XZAC53_Commande_Fin_Echangeur
* 
* Arguments en entree
* XDY_Entier	va_NumeroAction_in
* XDY_Entier	va_Site_in
* XDY_Horodate	va_HeureLancementFin_in
* XDY_Booleen	va_Manuelle_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: action non trouvee ou site incorrect
*                 update impossible
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* La commande est executee si Manuelle est a XDC_Faux
* 
* Fonction
* Verifier la non indisponibilite de l'equipement.
* Inserer dans la table ACTION la date de fin de l'action.
* Dans le cas de commande automatique :
* 	ACRP53 :	Appeler XZET04 pour com
------------------------------------------------------*/

 
#define XZAC53_Commande_Fin_Echangeur	"XZAC;53" 
