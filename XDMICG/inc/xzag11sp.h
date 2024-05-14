/*E*/
/*  Fichier : $Id: xzag11sp.h,v 1.1 1994/11/03 11:04:00 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 11:04:00 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag11sp.prc
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
* Purge les donnees situees la base historique locales relatives 
* a un evenement
* 
* Sequence d'appel
* SP	XZAG11_Purge_Evenement_historise
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in	: numero d'evenement
* XDY_Octet		va_CleEvt_in	: cle d'evenement
* XDY_Octet		va_ClasseEvt_in	: classe de l'evenement
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametre d'entree null
* 
* Conditions d'utilisation
* Prodecure appelee en interne uniquement et n'existant qu'au CI
* 
* Fonction
* Suppression les actions, l'alerte relatif a l'evenement. 
* et l'evenement 
-------------------------------------------------------*/

 
#define XZAG11_Purge_Evenement_historise	"XZAG;11" 
