/*E*/
/*  Fichier : $Id: xzac83sp.h,v 1.1 2005/09/13 18:08:49 gesconf Exp $     Release : $Revision: 1.1 $       Date : $Date: 2005/09/13 18:08:49 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac83sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	09/06/05	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Terminer l'action d'affichage PMVA
* 
* Sequence d'appel
* SP	XZAC83_Commande_Fin_PMVA
* 
* Arguments en entree
* XDY_Entier	va_NumeroAction_in
* XDY_Entier	va_Site_in
* XDY_Horodate	va_HeureLancementFin_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee ou site incorrect
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Pour revenir l'affichage par defaut sur PMV
* 
* Fonction
* Verifier la non indisponibilite de l'equipement.
* Inserer dans la table ACTION la date de fin lance de l'actio.
* ACRP83 :	Appeler XZEP01 pour arreter l'affichage sur l'equipement PMV
* 		Appeler XZEP02 pour arreter l'affichage du picto
-----------------------------------------------------*/

 
#define XZAC83_Commande_Fin_PMVA	"XZAC83" 
