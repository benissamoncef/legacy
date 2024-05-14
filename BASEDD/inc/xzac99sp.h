/*E*/
/*  Fichier : $Id: xzac99sp.h,v 1.1 2005/09/14 17:22:50 gesconf Exp $     Release : $Revision: 1.1 $       Date : $Date: 2005/09/14 17:22:50 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac99sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
*  JMG	09/06/05	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retour a l'etat normal de la BAD par scenario ou par 
* commande individuelle
* 
* Sequence d'appel
* SP	XZAC99_Commande_Fin_BAD
* 
* Arguments en entree
* XDY_Eqt	va_NumeroNAV_in
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
* L'execution de la commande de retour a l'etat normal 
* est executee si Manuelle est positionne a XDC_Faux
* 
* Fonction
* Inserer dans la table ACTION la date de fin de l'action.
* 
* Dans le cas de commande automatique par scenario :
* 	ACRP03 :	Appeler XZEN03 pour commander l'equipement
--------------------------------------------------------*/

 
#define XZAC99_Commande_Fin_BAD	"XZAC99" 
