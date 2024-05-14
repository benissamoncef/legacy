/*X  Fichier :  $Id: xzae10sp.h,v 1.3 2008/02/13 13:24:24 devgtie Exp $      Release : $Revision: 1.3 $        Date : $Date: 2008/02/13 13:24:24 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae10.sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* retourne la synthÉse d'un evt ou operation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	07/07/95 : Creation
* JPL	30/05/07 : Renommage conforme a xzae10 (travaux -> FMC)
-----------------------------------------------------*/


/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP    XZAE10_Annuler_FMC_En_Prevision
*
* Arguments en entree
* XDY_Entier	va_NumEvenement_in
* XDY_Octet	va_CleEvenement_in
* XDY_Machine	va_PosteOperateur_in		: numero de la machine de l'operateur
* XDY_Octet	va_Prevision_Traiter_Clos_in	: etat de l'evenement
* XDY_Mot	va_Type_in
* XDY_Horodate	va_DebutPrevu_in
* XDY_Horodate	va_Debut_in
* XDY_Horodate	va_FinPrevu_in
* XDY_Horodate	va_Fin_in
* XDY_Horodate	va_Validation_in
* XDY_Entier	va_NumCause_in
* XDY_Octet	va_CleCause_in
* XDY_Alerte	va_NumAlerte_in
* XDY_Nom	va_Origine_in
* XDY_Eqt	va_PosteEnrich_in		: numero de la machine de l'enrichisseur
* XDY_Booleen	va_ConfirmeSignale_in
* XDY_Autoroute	va_Autoroute_in
* XDY_PR	va_PR_in
* XDY_Sens	va_Sens_in
* XDY_Octet	va_PtCaracteristique_in		: numero du point caracteristique
* XDY_Octet	va_TypePtCaracteristique_in	: type du point caracteristique
* XDY_Octet	va_Position_in			: position sur aire ou echangeur ou lit
* XDY_Voie	va_VR_in
* XDY_Voie	va_VM2_in
* XDY_Voie	va_VM1_in
* XDY_Voie	va_VL_in
* XDY_Voie	va_BAU_in
* XDY_Voie	va_VR_I_in
* XDY_Voie	va_VM2_I_in
* XDY_Voie	va_VM1_I_in
* XDY_Voie	va_VL_I_in
* XDY_Voie	va_BAU_I_in
* Arguments en sortie
*
* Service rendu
* Met en base la fiche correspondante
*
* Code retour
* XDC_OK
* XDC_NOK               : alerte inconnue, cause de l'evenement inconnue, insertion
*                         impossible, declencher d'un reveil impossible
* XDC_ARG_INV           : argument d'entree null, ou valeur incorrecte
* XDC_PRC_INC           : procedure stockee n'existe pas en base
* code d'erreur de XZAE11 ET XZA14
* < 0                   : erreur sybase
*
* Conditions d'utilisation
*
* Fonction
-------------------------------------------------------*/

 
#define XZAE10_Annuler_FMC_En_Prevision	"XZAE;10"
