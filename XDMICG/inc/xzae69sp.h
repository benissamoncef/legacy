/*E*/
/*  Fichier : $Id: xzae69sp.h,v 1.1 1994/12/06 09:08:13 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/12/06 09:08:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae69sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	10/11/94	: Creation a partir de asrv_e69.prc
*                         (V 1.1)
* C.T.	14/11/94	: Modif recherche des infos au CI (V 1.2)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* retourne la liste des causes d'un evenement
* 
* Sequence d'appel
* SP	XZAE69_Liste_Causes
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* XDY_Horodate		va_Horodate_in : par defaut heure courante
* 
* Arguments en sortie
* XDY_Entier		va_NbCauses_out
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de complement de fiche main courante
* XDC_ARG_INV
* <0 erreur sybase 
* 
* Conditions d'utilisation
* En formation, preciser l'horodate
* 
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
* parcoure la table EVENEMENT avec le lien cause jusqu'au pere final
-------------------------------------------------*/

 
#define XZAE69_Liste_Causes	"XZAE;69" 
