/*E*/
/*  Fichier : $Id: xzaf13sp.h,v 1.1 1995/01/18 09:42:45 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/18 09:42:45 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah24sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	12/01/95	: Creation 
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare l'animation IHM (voies invers�es pour un basculement 
* avec un bouchon)
*
* Sequence d'appel
* SP    XZAH24_Animer_Bouchon_Inverse
*
* Arguments en entree
* XDY_Entier            va_NumBouchon_in
* XDY_Octet             va_CleBouchon_in
* XDY_PR                va_PRBouchonMin_in
* XDY_PR                va_PRBouchonMax_in
* XDY_Entier            va_NumBascul_in
* XDY_Octet             va_CleBascul_in
* XDY_PR                va_PRBasculMin_in
* XDY_PR                va_PRBasculMax_in
* XDY_Voie              va_VR_in
* XDY_Voie              va_VM2_in
* XDY_Voie              va_VM1_in
* XDY_Voie              va_VL_in
* XDY_Voie              va_BAU_in
* XDY_Autoroute         va_Autoroute_in
* XDY_Sens              va_SensBascul_in
*
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* XDC_ARG_INV	: argument null
*
* Conditions d'utilisation
* Les etats des voies passes en argument sont ceux du sens inverse
* c-a-d va_SensBascul_in
* Les PR sont donnes dans le sens croissant
* La procedure est appel�e par XZAF;12
*
------------------------------------------------------*/

 
#define 	"XZAF;13" 
