/*E*/
/*  Fichier : $Id: xzac63sp.h,v 1.1 1995/04/25 11:45:14 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/04/25 11:45:14 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac63sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	24/04/95	: Creation a partir xzac52
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Remet le tunnel a l'etat normal
*
* Sequence d'appel
* SP	XZAC63_Commande_Fin_Tunnel
*
* Arguments en entree
* XDY_Eqt	va_NumeroTunnel_in
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
* Inserer dans la table ACTION la date de fin de l'actio.
* Dans le cas de commande automatique :
* 	ACRP63 :	Appeler XZET03 pour commander l'equipement
-----------------------------------------------------------------------*/

 
#define XZAC63_Commande_Fin_Tunnel	"XZAC;63" 
