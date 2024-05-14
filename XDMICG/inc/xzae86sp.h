/*E*/
/*  Fichier : $Id: xzae86sp.h,v 1.2 2010/08/26 12:23:12 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2010/08/26 12:23:12 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae86sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.     	10/04/95	: Creation
* C.T.     	29/05/95	: Ajout du cas XDC_VOIE_SENS_NORMALE pour la BAU V 1.1
* C.T.		26/08/10	: Degroupage (renommee sans ';') 1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourner la configuration des voies pour une localisation
* donn≈e en prenant en compte les modifications dues aux evenements 
* de type travaux ou basculement en cours
* 
* Sequence d'appel
* SP	XZAE86_Configuration_Voies
* 
* Arguments en entree
* XDY_Autoroute	va_NumAutoroute_in
* XDC_PR	va_PR_in
* XDC_Sens	va_Sens_in
* 
* Arguments en sortie
* XDY_Voie	va_VR_out
* XDY_Voie	va_VM2_out
* XDY_Voie	va_VM1_out
* XDY_Voie	va_VL_out
* XDY_Voie	va_BAU_out
* XDY_Voie	va_VR_I_out
* XDY_Voie	va_VM2_I_out
* XDY_Voie	va_VM1_I_out
* XDY_Voie	va_VL_I_out
* XDY_Voie	va_BAU_I_out
* 
* Code retour
* XDC_OK
* XDC_NOK
* XDC_ARG_INV		: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
----------------------------------------------------*/

 
#define XZAE86_Configuration_Voies	"XZAE86" 
