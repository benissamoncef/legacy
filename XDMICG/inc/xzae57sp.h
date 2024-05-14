/*E*/
/*  Fichier : $Id: xzae57sp.h,v 1.1 1994/11/03 14:53:42 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:53:42 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae57sp.prc
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
* Modifier une fiche de fermeture echangeur relative a un evenement existant
* 
* Sequence d'appel
* SP	XZAE57_Ecrire_Fiche_Fermeture_Echangeur
* 
* Arguments en entree
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* XDY_Booleen		va_Entree1_in
* XDY_Booleen		va_Entree2_in
* XDY_Booleen		va_Sortie1_in
* XDY_Booleen		va_Sortie2_in
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
* 
* Fonction
* Modifier dans la table FERMETURE_ECHANGEUR
-------------------------------------------------------*/

 
#define XZAE57_Ecrire_Fiche_Fermeture_Echangeur	"XZAE;57" 
