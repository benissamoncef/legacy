/*E*/
/*  Fichier : $Id: xzae62sp.h,v 1.2 2012/05/29 14:33:50 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2012/05/29 14:33:50 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae62sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	03/10/94	: Creation	(V 1.1)
* JMG	29/05/12	: suppression ';' 1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Modifier une fiche de basculement relative a un evenement existant
* 
* Sequence d'appel
* SP	XZAE62_Ecrire_Fiche_Basculement
* 
* Arguments en entree
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* XDY_PR		va_PRfin_in
* XDY_PR		va_PR_in
* XDY_Entier		va_Longueur_in
* XDY_Voie		va_VR_in
* XDY_Voie		va_VM2_in
* XDY_Voie		va_VM1_in
* XDY_Voie		va_VL_in
* XDY_Voie		va_BAU_in
* XDY_Voie		va_VR_I_in
* XDY_Voie		va_VM2_I_in
* XDY_Voie		va_VM1_I_in
* XDY_Voie		va_VL_I_in
* XDY_Voie		va_BAU_I_in
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
* Modifier dans la table BASCULEMENT
--------------------------------------------------------*/

 
#define XZAE62_Ecrire_Fiche_Basculement	"XZAE62" 
