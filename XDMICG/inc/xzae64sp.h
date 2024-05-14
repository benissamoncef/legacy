/*E*/
/*  Fichier : $Id: xzae64sp.h,v 1.2 2009/03/03 11:09:43 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2009/03/03 11:09:43 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae64sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	03/10/94	: Creation
* C.T.	24/10/94	: Modif	de l'appel a XZAE;18 et test avec XZAE;18 reelle
*                         modif de la construction de la liste des evts a traiter 1.1
* JPL	14/01/2009	: Degroupage de la procedure 1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Modifier une fiche de poste operateur relative a un evenement existant
* 
* Sequence d'appel
* SP	XZAE64_Ecrire_Fiche_Poste_Operateur
* 
* Arguments en entree
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* XDY_Nom		va_Operateur_in
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
* Modifier dans la table POSTE_OPERATEUR
* S'il s'agit de la creation de la FMC, reconstruire la liste 
* des evenements a traiter pour les 24 prochaines heures (XZAE18)
-------------------------------------------------------*/

 
#define XZAE64_Ecrire_Fiche_Poste_Operateur	"XZAE64" 
