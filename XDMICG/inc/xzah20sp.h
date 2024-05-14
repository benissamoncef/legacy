/*E*/
/*  Fichier : $Id: xzah20sp.h,v 1.1 1994/12/05 17:56:41 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/12/05 17:56:41 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah20sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* initialisation et mise a jour des voies pour equext
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.  30/11/94        : Creation      (V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Procedure mettant a jour l'etat des voies a chaque
* validation d'un evenement local a un district ou CI
*
* Sequence d'appel
* SP    XZAH20_Voies_Fmc
*
* Arguments en entree
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* XDY_Horodate	va_Horodate_in
* XDY_Autoroute	va_Autoroute_in
* XDY_PR	va_PR_in
* XDY_Sens	va_Sens_in
* XDY_Voie	va_VL_in
* XDY_Voie	va_VM1_in
* XDY_Voie	va_VM2_in
* XDY_Voie	va_VR_in
* XDY_Voie	va_BAU_in
* XDY_Voie	va_VL_I_in
* XDY_Voie	va_VM1_I_in
* XDY_Voie	va_VM2_I_in
* XDY_Voie	va_VR_I_in
* XDY_Voie	va_BAU_I_in
* XDY_Site	va_Site_in
*
* Arguments en sortie
*
* Code retour
* XDC_OK        : pas de probleme
* XDC_NOK       : probleme a l'insertion dans la base de sauvegarde
*                 ou donnee non trouvee
* XDC_ARG_INV   : parametre d'entree null ou date demandee trop proche
*
* Conditions d'utilisation
* La procedure est appelee par le trigger de la table FMC_HIS
* Un evenement doit rester sur la meme chaussee (conserver le sens)
*
* Fonction
* insert ou update de VOI_DAI et VOI_RAD
-------------------------------------------------------*/

 
#define XZAH20_Voies_Fmc	"XZAH;20" 
