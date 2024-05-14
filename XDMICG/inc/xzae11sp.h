/*E*/
/*  Fichier : $Id: xzae11sp.h,v 1.2 2007/04/06 11:02:45 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2007/04/06 11:02:45 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae11sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* retourne la synth…se d'un evt ou operation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.     	18/10/94	: Creation 	V 1.1
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAE11_Valider_Fiche_MC
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
* 
* Arguments en sortie
* 
* Service rendu
* Met en base la fiche correspondante 
* (doit Ítre appele apres Ecrire_Fiche_xxx)
* 
* Code retour
* XDC_OK
* XDC_NOK		: alerte inconnue, cause de l'evenement inconnue, insertion 
*                         impossible, declencher d'un reveil impossible
* XDC_ARG_INV		: argument d'entree null, ou valeur incorrecte
* XDC_PRC_INC		: procedure stockee n'existe pas en base
* XZAEC_FMC_PAS_ENR	: pas le droit d'enrichir
* XZAEC_FMC_ETAT_INC	: l'evenement n'est pas dans l'etat precise en entree
* XZAEC_FMC_DEB_PRV_INC	: date de debut des travaux en prevision doit etre superieur
*                         a la date courante + 24h
* XZAEC_FMC_VOIE_INC	: la description des voies est incorrecte
* XZAEC_FMC_LOC_INC	: la localisation de l'evenement est incorrecte
* < 0          		: erreur sybase
* 
* Conditions d'utilisation
* 
* Fonction
* Controler si le poste operateur a le droit d'enrichir la fiche MC.
* Dans le cas de fiche "a traiter", tester si le poste 
* d'enrichissement est le poste operateur passe en parametre 
* dans le cas ou le poste d'enrichissement est value.
* Update la table EVENEMENT 
* Suivant le cas de fiche :
* 	"A traiter" :	Insert dans la table EVENEMENT_HISTORIQUE
* 	"Prevision" :	Update ou insert dans la table EVENEMENT_HISTORIQUE
* 	"Clos" :	Update dans la table EVENEMENT_HISTORIQUE
* Envoyer un reveil aux operateurs qui traitent l'evenement avec XZAR03
-------------------------------------------------------*/

 
#define XZAE11_Valider_Fiche_MC	"XZAE11" 
