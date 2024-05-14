/*E*/
/*  Fichier : $Id: xzaf04sp.h,v 1.1 1995/01/18 09:41:51 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/18 09:41:51 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzaf04sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm pour la formation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	11/01/95	: Creation 
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAF04_Neut_Voies_Evt
* 
* Arguments en entree
* XDY_Evt		va_NumEvt_in
* XDY_Autoroute		va_Autoroute_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_Octet		va_DebutFin_in
* XDY_Octet		va_VR_in
* XDY_Octet		va_VM2_in_in
* XDY_Octet		va_VM1_in_in
* XDY_Octet		va_VL_in
* XDY_Octet		va_BAU_in
* XDY_Octet		va_VR_I_in
* XDY_Octet		va_VM2_I_in_in
* XDY_Octet		va_VM1_I_in_in
* XDY_Octet		va_VL_I_in
* XDY_Octet		va_BAU_I_in
* 
* Arguments en sortie
* 
* Service rendu
* Retourne la liste des voies neutralisees pour
* l'animation IHM pour un evenement
*
* Code retour
* XDC_OK
* XDC_NOK	
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
*
---------------------------------------------------*/

 
#define XZAF04_Neut_Voies_Evt	"XZAF;04" 
