/*E*/
/*  Fichier : $Id: xzah15sp.h,v 1.1 1994/11/04 10:55:14 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/04 10:55:14 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah15sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm
*
------------------------------------------------------
* HISTORIQUE :
*
* B.G.  18/10/94        : Cr≈ation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare l'animation IHM (travaux)
* 
* Sequence d'appel
* SP	XZAH15_Animer_Travaux
* 
* Arguments en entree
* XDY_Evt		va_NumEvt_in
* XDY_Autoroute		va_Autoroute_in
* XDY_PR		va_PR_deb_in
* XDY_PR		va_PR_fin_in
* XDY_Sens		va_Sens_in
* XDY_Octet		va_TypPtCar_in
* XDY_Octet		va_PtCar_in
* XDY_Octet		va_PosPtCar_in
* XDY_Horodate		va_Horoval_in
* XDY_Horodate		va_Horodeb_in
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
* XDY_Octet		va_Suppr_in
* 
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: argument invalide
* 
* Conditions d'utilisation
* 
* appel≈ sur trigger par fmc_his_iu (fmc_his.trg)
------------------------------------------------------*/

 
#define XZAH15_Animer_Travaux	"XZAH;15" 
