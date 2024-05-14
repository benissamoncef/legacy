/*E*/
/*  Fichier : $Id: xzah12sp.h,v 1.1 1994/11/04 10:54:45 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/04 10:54:45 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah12sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	06/09/94	: Creation bouchon
* B.G.  16/10/94        : Creation (v1.2)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAH12_Animer_Objet_Mobile
* 
* Arguments en entree
* XDY_Evt		va_NumEvt_in
* XDY_Entier		va_NumAction_in
* XDY_Octet		va_Site_in
* XDY_Octet		va_TypeObjet_in
* XDY_Autoroute		va_Autoroute_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_Voie		va_Voie_in
* XDY_Octet		va_TypPtCar_in
* XDY_Octet		va_PtCar_in
* XDY_Octet		va_PosPtCar_in
* XDY_Booleen		va_Suppr_in
* 
* Arguments en sortie
* 
* Service rendu
* Prepare l'animation IHM
*
* Code retour
* XDC_OK
* XDC_NOK	
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
*
* appele sur trigger (fmc_his et act_gen)
---------------------------------------------------*/

 
#define XZAH12_Animer_Objet_Mobile	"XZAH;12" 
