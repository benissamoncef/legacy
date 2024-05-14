/*E*/
/*  Fichier : $Id: xzaz01sp.h,v 1.1 1995/01/18 11:55:59 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/18 11:55:59 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzaz01sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture pour satir
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	17/01/95	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Creer une fiche travaux et la valider
* -----------------------------------------------------
* Sequence d'appel
* SP	XZAZ01_Ecrire_Travaux_Satir
* 
* Arguments en entree
* XDY_Horodate	va_DebutPrevu_in 
* XDY_Horodate	va_Debut_in 
* XDY_Horodate	va_FinPrevue_in 
* XDY_Autoroute	va_Autoroute_out
* XDY_PR	va_PR_in
* XDY_Sens	va_Sens_in
* XDY_Voie	va_VR_in
* XDY_Voie	va_VM2_in
* XDY_Voie	va_VM1_in
* XDY_Voie	va_VL_in
* XDY_Voie	va_BAU_in
* XDY_Voie	va_VR_I_in
* XDY_Voie	va_VM2_I_in
* XDY_Voie	va_VM1_I_in
* XDY_Voie	va_VL_I_in
* XDY_Voie	va_BAU_I_in
* XDY_Commentaire	va_Texte_in
* XDY_Booleen   va_Nature_in
* XDY_Octet	va_Type_in
* XDY_Booleen   va_Continu_in
* XDY_Horodate	va_HoroDebC_in
* XDY_Horodate	va_HoroFinC_in
* XDY_Booleen   va_Mobile_in
* XDY_PR	va_PRqueueM_in
* XDY_PR	va_PRteteM_in
* XDY_Sens	va_SensM_in
* XDY_PR	va_PRtete_in
* XDY_Booleen   va_VoiesRet_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK		: fiche non trouvee
* XZAZC_DEBUT_INC	: debut prevue est inferieure a la date
*                         actuelle + 24h.
* XZAZC_LOC_INC		: localisation incorrecte
* XDC_ARG_INV		: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* La date de debut prevue doit etre superieure a la date
* actuelle + 24h. 
* La date de debut doit etre nulle (ou valoir 1970 pour les 
* clients en C)
* 
* Fonction
* Creation d'une fiche travaux, insertion du complement
* de la fiche et validation de la fiche
----------------------------------------------------*/

 
#define XZAZ01_Ecrire_Travaux_Satir	"XZAZ;01" 
