/*E*/
/* Fichier : $Id: xzae65sp.h,v 1.2 2019/04/01 07:56:16 devgfi Exp $        $Revision: 1.2 $        $Date: 2019/04/01 07:56:16 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae65sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	03/10/94	: Creation	(V 1.1)
* JPL	19/03/19 : Procédure dégroupée (pour DEM 1327)  1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Modifier une fiche de manifestation relative a un evenement existant
* 
* Sequence d'appel
* SP	XZAE65_Ecrire_Fiche_Manif
* 
* Arguments en entree
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* XDY_Horodate		va_Horodate_in
* XDY_Mot		va_Nombre_in
* XDY_Booleen		va_Perception_in
* XDY_Booleen		va_Violence_in
* XDY_Eqt		va_PosteOperateur_in
* XDY_Octet		va_Prevision_Traiter_Clos_in
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
* Modifier dans la table MANIFESTATION
-------------------------------------------------------*/

 
#define XZAE65_Ecrire_Fiche_Manif	"XZAE65" 
