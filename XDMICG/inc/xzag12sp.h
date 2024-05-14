/*E*/
/*  Fichier : $Id: xzag12sp.h,v 1.1 1994/11/03 11:04:04 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 11:04:04 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag12sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	02/11/94	: Creation	(V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Purge les donnees situees la base historique locales relatives 
* a un evenement
* 
* Sequence d'appel
* SP	XZAG12_Sauvegarde_Evenement_historise
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in	: numero d'evenement
* XDY_Octet		va_CleEvt_in	: cle d'evenement
* XDY_Octet		va_ClasseEvt_in	: classe de l'evenement
* XDY_Entier		va_NumCause_in	: numero de la cause d'evenement
* XDY_Octet		va_CleCause_in	: cle de la cause d'evenement
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK	: probleme a l'insertion dans la base de 
*                 sauvegarde/restauration
* XDC_ARG_INV	: parametre d'entree null
* 
* Conditions d'utilisation
* procedure appelee en interne (par XZAG;01)
* Cette procedure n'existe qu'au CI
* 
* Fonction
* Sauvegarder les actions, l'alerte relatif a l'evenement,
* l'evenement et sa cause
-------------------------------------------------------*/

 
#define XZAG12_Sauvegarde_Evenement_historise	"XZAG;12" 
