/*E*/
/*  Fichier : $Id: xzat05sp.h,v 1.1 1994/12/06 09:07:00 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/12/06 09:07:00 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER asrv_t05sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des etats d'un equipement 
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	26/09/94	: Creation
* C.T.	30/09/94	: Modif interface fonction
*                         ajout de NumTypePicto et TypePMV
* C.T.	10/11/94	: Modif bugs applix V 1.3
* C.T.	15/11/94	: Modif recherche des infos au CI (V 1.4)
*                         et ajout en sortie du numero d'action
*                         Changement du nom du fichier (V 1.1)
* C.T.	24/11/94	: Ajout code de dispo PMV supprime (V 1.3)
* C.T.	29/11/94	: Modif interface fonction ajout site action (V 1.4)
*                         non teste
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne l'utilisation reelle d'un PMV a une horodate donnee.
* 
* Sequence d'appel
* CL	XZAT05_Utilisation_PMV
* 
* Arguments en entree
* XDY_Horodate	va_Horodate_in
* XDY_Mot	va_NumEqt_in
* 
* Arguments en sortie
* XDY_Octet  	va_DispoPMV_out
* XDY_Octet  	va_DispoPicto_out
* XDY_Octet  	va_TypePMV_out
* XDY_LignePMV	va_Ligne1_out
* XDY_LignePMV	va_Ligne2_out
* XDY_LignePMV	va_Ligne3_out
* XDY_LignePMV	va_Alternat1_out
* XDY_LignePMV	va_Alternat2_out
* XDY_LignePMV	va_Alternat3_out
* XDY_Booleen	va_Clignotement1_out
* XDY_Booleen	va_Clignotement2_out
* XDY_Booleen	va_Clignotement3_out
* XDY_Booleen	va_Flash_out
* char[4]	va_TypePicto_out
* char[6]	va_TextePicto_out
* XDY_Octet	va_NumTypePicto_out
* XDY_Booleen	va_ClignotementPicto_out
* XDY_Booleen	va_ClignotementPicto_out
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
* select dans les tables EQUIPEMENT, E_PMV, ACTION et 
* INDISPONIBILITE
------------------------------------------------*/

 
#define XZAT05_Utilisation_PMV	"XZAT;05" 
