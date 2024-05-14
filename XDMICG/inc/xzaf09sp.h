/*E*/
/*  Fichier : $Id: xzaf09sp.h,v 1.1 1995/01/18 09:42:23 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/18 09:42:23 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzaf09sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm de la formation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.  11/01/95        : Cr≈ation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare l'animation IHM (voies RADT invers≈es)
* 
* Sequence d'appel
* SP	XZAF09_Animer_Voies_RADT_Inversees
* 
* Arguments en entree
* XDY_Evt		va_NumEvt_in
* XDY_Autoroute		va_Autoroute_in
* XDY_PR		va_PR_deb_in
* XDY_PR		va_PR_fin_in
* XDY_Sens		va_Sens_in
* XDY_Octet		va_VR_in
* XDY_Octet		va_VM2_in
* XDY_Octet		va_VM1_in
* XDY_Octet		va_VL_in
* XDY_Octet		va_BAU_in
* XDY_Octet		va_VR_I_in
* XDY_Octet		va_VM2_I_in
* XDY_Octet		va_VM1_I_in
* XDY_Octet		va_VL_I_in
* XDY_Octet		va_BAU_I_in
* 
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: arguments invalides
* 
* Conditions d'utilisation
* appel≈e par XZAF;08
* 
------------------------------------------------------*/

 
#define XZAF09_Animer_Voies_RADT_Inversees	"XZAF;09" 
