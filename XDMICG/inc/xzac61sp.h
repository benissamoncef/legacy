/*E*/
/*  Fichier : $Id: xzac61sp.h,v 1.1 1995/04/25 11:44:45 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/04/25 11:44:45 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac61sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	24/04/95	: Creation a partir de xzac50
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Terminer l'action d'affichage PMV+picto.
* 
* Sequence d'appel
* SP	XZAC61_Commande_Fin_PMV
* 
* Arguments en entree
* XDY_Eqt	va_NumeroPMV_in
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
* Inserer dans la table ACTION la date de fin lance de l'actio.
* ACRP61 :	Appeler XZEP01 pour arreter l'affichage sur l'equipement PMV
* 		Appeler XZEP02 pour arreter l'affichage du picto
-----------------------------------------------------*/

 
#define XZAC61_Commande_Fin_PMV	"XZAC;61" 
