/*E*/
/*  Fichier : $Id: xzac54sp.h,v 1.1 1994/11/03 14:53:07 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:53:07 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac54sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	06/09/94	: Creation
* C.T.	13/09/94	: Rajout test sur le site
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Inserer  un morceau de texte de fax a une actio deja existante
* 
* Sequence d'appel
* SP	XZAC54_Ajouter_Texte_Fax
* 
* Arguments en entree
* XDY_Entier		va_NumAction_in
* XDY_Octet		va_Site_in
* XDY_Texte		va_Texte_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee ou site incorrect
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Il faut que l'actio existe deja : utiliser avant XZAC09
* 
* Fonction
* Inserer dans la table TEXTE_FAX
-------------------------------------------------------*/

 
#define XZAC54_Ajouter_Texte_Fax	"XZAC;54" 
