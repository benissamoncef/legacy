/*E*/
/*  Fichier : $Id: xzah19sp.h,v 1.1 1994/12/08 18:47:52 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/12/08 18:47:52 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah19sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.  06/12/94        : Cr≈ation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare l'animation IHM (voies invers≈es par basculement avec bouchon)
*
* Sequence d'appel
* SP    XZAH9_Animer_Bouchons_Inverses
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
* XDY_Octet             va_Suppr_in
*
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* XDC_ARG_INV       : arguments invalides
*
* Conditions d'utilisation
* appel≈e par XZAH;14 ou par XZAH;16
*
------------------------------------------------------*/

 
#define XZAH9_Animer_Bouchons_Inverses	"XZAH;19" 
