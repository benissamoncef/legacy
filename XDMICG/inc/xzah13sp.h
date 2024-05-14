/*E*/
/*  Fichier : $Id: xzah13sp.h,v 1.1 1994/11/04 10:54:58 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/04 10:54:58 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah13sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	07/09/94	: Creation bouchon
* B.G.  18/10/94	: Cr≈ation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare l'animation IHM (symbole de rabattement)
* 
* Sequence d'appel
* SP	XZAH13_Animer_Neut_Voies
* 
* Arguments en entree
* XDY_Evt		va_NumEvt_in
* XDY_Autoroute		va_Autoroute_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_Octet		va_DebutFin_in
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
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: argument invalide
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* appel≈ par fmc_his_iu (fmc_his.trg)
* 
------------------------------------------------------*/

 
#define XZAH13_Animer_Neut_Voies	"XZAH;13" 
