/*E*/
/*  Fichier : $Id: xzah25sp.h,v 1.1 1994/12/08 18:48:12 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/12/08 18:48:12 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah25sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* initialisation et mise a jour des voies pour equext
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.  08/12/94        : Creation      (V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Mise a jour l'etat des voies pour un evenement
*
* Sequence d'appel
* SP    XZAH25_Ecrire_Voies_Fmc
*
* Arguments en entree
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* XDY_Autoroute	va_Autoroute_in
* XDY_PR	va_PRDebut_in
* XDY_PR	va_PRFin_in
* XDY_Sens	va_Sens_in
* XDY_Voie	va_VL_in
* XDY_Voie	va_VM1_in
* XDY_Voie	va_VM2_in
* XDY_Voie	va_VR_in
* XDY_Voie	va_BAU_in
*
* Arguments en sortie
*
* Code retour
* XDC_OK        : pas de probleme
* XDC_NOK       : probleme a l'insertion dans la base de sauvegarde
*                 ou donnee non trouvee
* XDC_ARG_INV   : parametre d'entree null
*
* Conditions d'utilisation
* La procedure est appelee par XZAH;20
* Un evenement doit rester sur la meme chaussee (conserver le sens)
*
* Fonction
* insert ou update de VOI_DAI et VOI_RAD
-------------------------------------------------------*/

 
#define XZAH25_Ecrire_Voies_Fmc	"XZAH;25" 
