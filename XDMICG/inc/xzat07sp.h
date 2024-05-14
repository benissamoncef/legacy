/*E*/
/*  Fichier : $Id: xzat07sp.h,v 1.1 1994/12/06 09:07:13 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/12/06 09:07:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER asrv_t07sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des etats d'un equipement 
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	26/09/94	: Creation
* C.T.	10/11/94	: Modif bugs applix V 1.2
* C.T.  14/11/94          changement du nom de fichier V 1.1
* C.T.	14/11/94	: Modif recherche des infos au CI (V 1.2)
*                         et ajout en sortie du numero d'action
* C.T.	29/11/94	: Modif interface fonction ajout site action (V 1.3)
*                         non teste
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne l'utilisation r≈elle d'un tunnel » une horodate donn≈e.
* 
* S≈quence d'appel
* CL	XZAT07_Utilisation_Tunnel
* 
* Arguments en entr≈e
* XDY_Horodate	va_Horodate_in
* XDY_Eqt	va_NumEqt_in
* 
* Arguments en sortie
* XDY_Octet	va_DispoTube_out
* XDY_Octet	va_Sequence_out
* XDY_Evt	va_NumEvt_out
* XDY_Evt	va_CleEvt_out
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
* 
* Fonction
* select dans les tables EQUIPEMENT, AFFICHAGE_TUNNEL, 
* ACTION et INDISPONIBILITE
-------------------------------------------------------------*/

 
#define XZAT07_Utilisation_Tunnel	"XZAT;07" 
