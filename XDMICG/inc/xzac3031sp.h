/*  Fichier : @(#)xzac3031sp.h
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac3031sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* GGY	13/11/23	: Creation (DEM483)
-----------------------------------------------------*/

/*-----------------------------------------------------
* Service rendu
* Retour a l'etat normal de la PIC par scenario ou par 
* commande individuelle
* 
* Sequence d'appel
* SP	XZAC3031_Commande_Fin_PIC
* 
* Arguments en entree
* XDY_Horodate		va_HorodateFin_in
* XDY_Entier		va_NumEvt_in
* XDY_Octet			va_CleEvt_in
* XDY_Operateur		va_Operateur_in
* XDY_Eqt			va_NumEqt_in
* XDY_Site			va_Site_in
* XDY_Bandeau_PIC	va_Bandeau_in
* XDY_Booleen		va_Flash_in
* XDY_Picto			va_Picto_in
* XDY_Octet			va_Priorite_in
* XDY_Site			va_site_origine_in
* 
* Arguments en sortie
* XDY_Entier		NumeroAction
*
* Code retour
* XDC_OK
* XDC_NOK		: site incorrect
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
* 	ACRP16 :	Appeler XZEN03 pour commander l'equipement
--------------------------------------------------------*/
#define XZAC3031_Commande_Fin_PIC	"XZAC3031" 
