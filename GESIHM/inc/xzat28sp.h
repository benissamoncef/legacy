/*E*/
/*  Fichier : $Id: xzat28sp.h,v 1.2 2004/05/06 14:03:00 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2004/05/06 14:03:00 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzat28sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des etats d'un equipement 
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	19/01/98	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne l'utilisation reelle d'un PAL a une horodate donnee.
* 
* Sequence d'appel
* CL	XZAT28_Utilisation_PAL
* 
* Arguments en entree
* XDY_Horodate	va_Horodate_in
* XDY_Mot	va_NumEqt_in
* 
* Arguments en sortie
* XDY_Octet  	va_DispoPAL_out
* XDY_LignePAL	va_LibelleMod1_out
* XDY_LignePAL	va_LibelleMod2_out
* XDY_LignePAL	va_LibelleMod3_out
* XDY_LignePAL	va_LibelleMod4_out
* XDY_LignePAL	va_LibelleMod5_out
* XDY_Entier	va_NumEvt_out
* XDY_Octet	va_CleEvt_out
* XDY_Octet	va_Priorite_out
* XDY_Entier	va_DistanceEvt_out
* XDY_Entier	va_Action_out
* XDY_Site	va_SiteAction_out
* 
* Code retour
* XDC_OK
* XDC_NOK
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* La procedure stockee au CI recherche les informations 
* dans la base HIS si la date donnee est anterieure de plus d'un 
* mois tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Le code de recherche des infos de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* et du test de la date en entree (< 1 mois)
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
* 
* Fonction
* select dans les tables EQUIPEMENT, E_PAL, ACTION et 
* INDISPONIBILITE
------------------------------------------------*/

 
#define XZAT28_Utilisation_PAL	"XZAT280" 
