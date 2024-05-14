/*E*/
/*  Fichier : $Id: xzag10sp.h,v 1.1 1994/11/03 11:03:55 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 11:03:55 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag06sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	27/10/94	: Creation	(V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Purge les donnees situees sur le site local relatives 
* a un evenement
* 
* Sequence d'appel
* SP	XZAG10_Purge_Evenement
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in	: numero d'evenement
* XDY_Octet		va_CleEvt_in	: cle d'evenement
* XDY_Site		va_SiteEvt_in	: site de l'evenement
* XDY_Octet		va_ClasseEvt_in	: classe de l'evenement
* 
* Arguments en sortie
* XDY_Booleen		va_Resultat_out : XDC_OUI si evenement supprime
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametre d'entree null
* 
* Conditions d'utilisation
* 
* Fonction
* Suppression les actions, les SAD, l'alerte situes sur le site 
* local relatif a l'evenement. 
* Suppression de l'evenement si aucun lien persiste avec 
* l'evenement et s'il situe sur le site local
-------------------------------------------------------*/

 
#define XZAG10_Purge_Evenement	"XZAG;10" 
