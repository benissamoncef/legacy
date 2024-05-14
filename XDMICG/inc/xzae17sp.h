/*E*/
/*  Fichier : $Id: xzae17sp.h,v 1.2 2008/12/02 14:39:36 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2008/12/02 14:39:36 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae17sp.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* B.G.	14/10/94	: Creation
* JPL	02/12/08	: Degroupage de la procedure
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne la liste des fiches main 
* courante » traiter » l'heure precis≈e
-------------------------------------------------------
* Sequence d'appel
* SP	XZAE17_Liste_Evt_Traiter
* 
* Arguments en entree
* XDY_Machine	va_PosteOperateur_in
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
* 
* Fonction
*
----------------------------------------------------*/

 
#define XZAE17_Liste_Evt_Traiter	"XZAE17"
