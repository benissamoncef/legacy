/*E*/
/*  Fichier : $Id: xzaf01sp.h,v 1.1 1995/01/17 11:12:29 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/17 11:12:29 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzaf01sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Formation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	10/01/95	: Creation (V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne la liste des fiches main 
* courante » traiter » l'heure precis≈e
-------------------------------------------------------
* Sequence d'appel
* SP	XZAF01_Liste_Evt_Traiter
* 
* Arguments en entree
* XDY_Machine	va_PosteOperateur_in
* XDY_Horodate	va_Horodate_in
* 
* Arguments en sortie
* XDY_Octet	va_CleEvt_out
* XDY_Entier	va_NumEvt_out
* XDY_Octet	va_Type_out
* XDY_Autoroute	va_Autoroute_out
* XDY_PR	va_PR_out
* XDY_Sens	va_Sens_out
* XDY_Horodate	va_Horodate_out
* XDY_Booleen	va_Enrichir_out
* XDY_Booleen	va_Reveil_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: fiche non trouvee
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* va_PosteOperateur_in n'est pas utilise
* 
* Fonction
*
----------------------------------------------------*/

 
#define XZAF01_Liste_Evt_Traiter	"XZAF;01" 
