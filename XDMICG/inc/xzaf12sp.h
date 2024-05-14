/*E*/
/*  Fichier : $Id: xzaf12sp.h,v 1.1 1995/01/18 09:42:39 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/18 09:42:39 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzaf12sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm de formation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	12/01/95	: Creation 
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare l'animation IHM (voies invers≈es par basculement avec bouchon)
*
* Sequence d'appel
* SP    XZAF12_Animer_Bouchon_Inverse_Evt
*
* Arguments en entree
* XDY_Entier            va_NumEvt_in	: numero d'un basculement ou d'une queue de bouchon
* XDY_Octet             va_CleEvt_in
* XDC_Booleen		va_BouchonBascul_in : indicateur de type d'evenement
* XDY_Octet		va_Type_in	: type de bouchon
* XDY_Autoroute         va_Autoroute_in
* XDY_PR                va_PRDeb_in
* XDY_PR                va_PRFin_in
* XDY_Sens              va_Sens_in
* XDY_Octet             va_VR_in
* XDY_Octet             va_VM2_in
* XDY_Octet             va_VM1_in
* XDY_Octet             va_VL_in
* XDY_Octet             va_BAU_in
* XDY_Octet             va_VR_I_in
* XDY_Octet             va_VM2_I_in
* XDY_Octet             va_VM1_I_in
* XDY_Octet             va_VL_I_in
* XDY_Octet             va_BAU_I_in
*
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* XDC_ARG_INV       : arguments invalides
*
* Conditions d'utilisation
* appel≈e par XZAF;11
*
------------------------------------------------------*/

 
#define XZAF12_Animer_Bouchon_Inverse_Evt	"XZAF;12" 
