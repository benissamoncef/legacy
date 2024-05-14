/*E*/
/*  Fichier : $Id: xzah14sp.h,v 1.1 1994/11/04 10:55:06 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/04 10:55:06 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah14sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm
*
------------------------------------------------------
* HISTORIQUE :
*
* B.G.  02/11/94        : Cr≈ation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare l'animation IHM (bouchon)
* 
* Sequence d'appel
* SP	XZAH14_Animer_Bouchon
* 
* Arguments en entree
* XDY_Evt		va_NumEvt_in
* XDY_Booleen		va_TeteQueue_in
* XDY_Autoroute		va_Autoroute_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
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

 
#define XZAH14_Animer_Bouchon	"XZAH;14" 
