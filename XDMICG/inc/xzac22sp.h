/*E*/
/*  Fichier : $Id: xzac22sp.h,v 1.1 1994/12/06 09:07:39 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/12/06 09:07:39 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac22sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	10/11/94	: Creation a partir de asrv_c22sp.prc
*                         ( V 1.1 )
* C.T.	14/11/94	: Modif recherche infos pour CI (V 1.2)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Recherche les intervenants presents sur les lieux d'un evenement 
* (secours, SIS, depanneur)
* 
* Sequence d'appel
* CL	XZAC22_Rech_Intervenants_Sur_Place
* 
* Arguments en entree
* XDY_Horodate	va_Horodate_in
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* 
* Arguments en sortie
* XDY_Booleen	va_SIS_out
* XDY_Booleen	va_Secours_out
* XDY_Booleen	va_Depanneur_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* La procedure stockee au CI recherche les informations 
* dans la base HIS si la date donnee est anterieure de plus d'un 
* mois tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Le code de creation de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* et du test de la date en entree (< 1 mois)
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
* 
* 
* Fonction
* select dans ACTION where type=XDC_ACT_PAT_SIS,
* XDC_ACT_INTERVENTION ou XDC_ACT_DEPANNEUR et verifiant
* heure_succes < Horodate < heure_fin
---------------------------------------------------------*/

 
#define XZAC22_Rech_Intervenants_Sur_Place	"XZAC;22" 
