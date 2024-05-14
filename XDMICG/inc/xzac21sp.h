/*E*/
/*  Fichier : $Id: xzac21sp.h,v 1.2 2008/12/09 14:45:40 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2008/12/09 14:45:40 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac21sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	12/09/94	: Creation
* JPL	09/12/08	: Degroupage de la procedure
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne la liste des actions realisees pour un evenement (numero+libelle)
* 
* Sequence d'appel
* SP	XZAC21_Actions_Evt
* 
* Arguments en entree
* XDY_Horodate	va_Horodate_in
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* 
* Arguments en sortie
* XDY_Entier	va_Numero_out		- Informations retournees dans une liste
* XDY_Octet	va_Site_out		-
* XDY_Octet	va_Type_out		-
* XDY_Horodate	va_Heure_out		-
* XDY_Octet	va_Etat_out		-
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* les informations sur chaque actio sont retournees dans une liste
* 
* Fonction
* Select dans la table ACTION where evenement, cle = NumEvt
-----------------------------------------------------------------------*/

 
#define XZAC21_Actions_Evt	"XZAC21"
