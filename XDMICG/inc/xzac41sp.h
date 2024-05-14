/*E*/
/*  Fichier : $Id: xzac41sp.h,v 1.1 1994/11/03 14:53:00 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:53:00 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac41sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	09/09/94	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Lecture de l'actio de deployement de NAV par scenario
* 
* Sequence d'appel
* SP	XZAC41_Lire_Commande_NAV
* 
* Arguments en entree
* XDY_Entier		va_NumeroAction_in
* XDY_Octet		va_Site_in
* 
* Arguments en sortie
* XDY_Eqt		va_NumEqt_in
* XDY_Horodate		va_HeureLancement_in
* XDY_Horodate		va_HeureLancementFin_in
* XDY_Horodate		va_HeureSucces_in
* XDY_Horodate		va_HeureEchec_in
* XDY_Horodate		va_HeureFin_in
* XDY_Booleen		va_Manuelle_in
* char(4)		va_Scenario_in
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
* Select des tables ACTION et E_NAV
-----------------------------------------------------------------------*/

 
#define XZAC41_Lire_Commande_NAV	"XZAC;41" 
