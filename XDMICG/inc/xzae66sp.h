/*E*/
/*  Fichier : $Id: xzae66sp.h,v 1.2 2009/04/16 15:39:42 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2009/04/16 15:39:42 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae66sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.     	20/10/94	: Creation	V 1.1 
* JPL     	16/04/09	: Degroupee	1.12
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Indiquer si la localisation est valide, et la complete si 
* necessaire. Retourner la description des voies pour cette localisation
* 
* Sequence d'appel
* SP	XZAE66_Configuration_Voies
* 
* Arguments en entree
* XDY_Autoroute	va_NumAutoroute_in
* XDC_PR	va_PR_in
* XDC_Sens	va_Sens_in
* XDC_Octet	va_PtCaracteristique_in
* XDC_Octet	va_TypePtCaracteristique_in
* XDC_Octet	va_Position_in
* 
* Arguments en sortie
* XDY_Autoroute	va_NumAutoroute_out
* XDC_PR	va_PR_out
* XDC_Sens	va_Sens_out
* XDC_Octet	va_PtCaracteristique_out
* XDC_Octet	va_TypePtCaracteristique_out
* XDC_Octet	va_Position_out
* XDY_Voie	va_VR_out
* XDY_Voie	va_VM2_out
* XDY_Voie	va_VM1_out
* XDY_Voie	va_VL_out
* XDY_Voie	va_BAU_out
* XDY_Voie	va_VR_I_out
* XDY_Voie	va_VM2_I_out
* XDY_Voie	va_VM1_I_out
* XDY_Voie	va_VL_I_out
* XDY_Voie	va_BAU_I_out
* XDC_Octet	va_District_out
* XDC_Octet	va_DistrictRecouvr_out
* XDY_Entier	va_Resultat_out
* 
* Code retour
* XDC_OK
* XDC_NOK
* XZAEC_LOC_PT_CAR_INC	: point caracteristique inexistant
* XDC_ARG_INV		: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* - Tester la localisation et rechercher la localisation precise a partir :
* 	d'un echangeur
* 	d'un peage
* 	d'une aire
* 	d'une autoroute, PR, sens
* 	d'un lit d'arret
* - L'argument de sortie va_Resultat_out indique :
*	- bit 1 : le sens incorrect (XDC_LOC_SENS_MOD)
*	- bit 2 : le PR incorrect (XDC_LOC_PR_MOD)
*	- bit 3 : l'autoroute incorrect (XDC_LOC_AUT_MOD)
*	- bit 4 : la localisation incorrecte (XDC_LOC_INC)
* dans le cas de point caracteristique ou localisation incorrect la
* configuration des voies n'est pas donnee
* - Pour une aire ou un echangeur, la configuration des voies 
* inverses n'est pas valuee (car n'existe pas)
* - Si le sens est incorrect ou non precise en entree et
* impossible a definir alors XDC_SENS_INCONNU est retourne pour le sens
* et aucune configuration de voie n'est renseignee 
* (arguments de sortie des voies a null)
* 
* Fonction
* Tester si la localisation est valide
* Si la localisation est valide alors chercher la description 
* des voies dans la table SEGMENT_HOMOGENE
----------------------------------------------------*/

 
#define XZAE66_Configuration_Voies	"XZAE66" 
