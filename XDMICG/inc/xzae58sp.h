/*E*/
/*  Fichier : $Id: xzae58sp.h,v 1.1 1994/11/03 14:53:44 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:53:44 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae58sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	03/10/94	: Creation	(V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Modifier une fiche sous concessionnaire relative a un evenement existant
* 
* Sequence d'appel
* SP	XZAE58_Ecrire_Fiche_Sous_Concess
* 
* Arguments en entree
* XZAET_SousConcess	Description
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* XDY_Nom		va_Nom_in
* XDY_Booleen		va_Ferme_in
* XDY_Booleen		va_Penurie_in
* XDY_Octet		va_Carburant_in
* XDY_Eqt		va_PosteOperateur_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve, update impossible,
*                 poste enrichisseur different
* XDC_ARG_INV	: parametres d'entree null
* 
* Conditions d'utilisation
* Preciser le carburant uniquement si penurie
* 
* Fonction
* Modifier dans la table SS_CONCESSIONNAIRE
------------------------------------------------------*/

 
#define XZAE58_Ecrire_Fiche_Sous_Concess	"XZAE;58" 
