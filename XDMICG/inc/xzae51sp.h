/*E*/
/*  Fichier : $Id: xzae51sp.h,v 1.1 1994/11/03 14:53:29 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:53:29 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae51.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	31/08/94	: Creation
* C.T.	12/08/94	: Modif position param entree et
*                         des test sur constantes
* C.T.	13/10/94	: Correction erreur de codage	V 1.4
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs d'une fiche main courante bouchon a l'heure precisee
* 
* Sequence d'appel
* SP	XZAE51_Lire_Fiche_Bouchon
* 
* Arguments en entree
* XDY_Horodate		va_Horodate_in : par defaut heure courante(non utilisee)
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* 
* Arguments en sortie
* XDY_PR		va_PRQueue_out
* XDY_Octet		va_Type_out
* XDY_Entier		va_Longueur_out
* XDY_Entier		va_NumEvt_out
* XDY_Octet		va_CleEvt_out
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de complement de fiche main courante
* XDC_ARG_INV
* < 0 erreur sybase
* 
* Conditions d'utilisation
* En formation, preciser l'horodate
* Penser a lire les 2 fiches generiques tete et queue 
* de bouchon avant de lire le complement.
* Si l'evenement en entree correspond a une tete, le numero 
* d'evemenet en sortie est ceui de la queue et inversement
* 
* Fonction
* Select dans la table QUEUE_DE_BOUCHON
* where numero and horodate
* Calcul de la distance entre PR tete et PR queue.
-----------------------------------------------------*/

 
#define XZAE51_Lire_Fiche_Bouchon	"XZAE;51" 
