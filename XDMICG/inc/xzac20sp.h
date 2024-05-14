/*E*/
/*  Fichier : $Id: xzac20sp.h,v 1.2 2009/11/04 09:12:04 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2009/11/04 09:12:04 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac20sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	20/09/94	: Creation
* JPL	03/11/09	: Procedure degroupee (renommee sans ";")
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Inserer  un morceau de texte de tfm a une actio deja existante
* 
* Sequence d'appel
* SP	XZAC20_Ecrire_Texte_TFM
* 
* Arguments en entree
* XDY_Entier		va_NumAction_in
* XDY_Octet		va_Site_in
* XDY_Texte		va_Texte_in
* XDY_Booleen		va_FinTexte_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee ou site incorrect ou 
*                 pb a l'appel de XZIT06
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Il faut que l'actio existe deja : utiliser avant XZAC19
* 
* Fonction
* Inserer dans la table ACT_TXT_TFM
* Appelle XZIT06 pour prevenir l'animateur radio 
* quand le texte est fini
-------------------------------------------------------*/

 
#define XZAC20_Ecrire_Texte_TFM	"XZAC20" 
