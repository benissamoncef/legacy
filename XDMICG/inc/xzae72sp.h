/*E*/
/*  Fichier : $Id: xzae72sp.h,v 1.2 2011/10/10 15:20:50 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2011/10/10 15:20:50 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae72sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.     	20/10/94 : Creation	V 1.1 
* JPL		10/10/11 : Degroupee (pour DEM 995)  1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne l'intitule d'une fiche main courante 
* 
* Sequence d'appel
* SP	XZAE72_Lire_Intitule_FMC
* 
* Arguments en entree
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* 
* Arguments en sortie
* XDY_Mot	va_Type_out
* XDY_Horodate	va_HoroDebut_out
* XDY_Autoroute	va_NumAutoroute_out
* XDY_PR	va_PR_out
* XDY_Sens	va_Sens_out
* XDY_Octet	va_PtCaracteristique_out
* XDY_Octet	va_TypePtCaracteristique_out
* XDY_Octet	va_Position_out
* XDY_Nom	va_Origine_out
* 
* Code retour
* XDC_OK	
* XDC_NOK	evenement pas trouve
* 
* Conditions d'utilisation
* 
* Fonction
* retourne les informations generales de la FMC
* 
-------------------------------------------------------*/

 
#define	XZAE72_Lire_Intitule_FMC	"XZAE72"
