/*E*/
/*  Fichier : $Id: xzac67sp.h,v 1.1 1998/01/20 14:27:25 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1998/01/20 14:27:25 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac67sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	19/01/98	: Creation 
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Terminer l'action d'affichage PAL
* 
* Sequence d'appel
* SP	XZAC67_Commande_Fin_PAL
* 
* Arguments en entree
* XDY_Eqt	va_NumeroPAL_in
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
* Pour revenir l'affichage par defaut sur PAL
* 
* Fonction
* Inserer dans la table ACTION la date de fin lance de l'actio.
* ACRP61 :	Appeler XZEP01 pour arreter l'affichage sur l'equipement PAL
* 		Appeler XZEP02 pour arreter l'affichage du picto
-----------------------------------------------------*/

 
#define XZAC67_Commande_Fin_PAL	"XZAC;67" 
