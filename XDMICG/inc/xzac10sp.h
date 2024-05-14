/*E*/
/*  Fichier : $Id: xzac10sp.h,v 1.1 1994/11/03 14:52:29 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:52:29 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac10sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	05/09/94	: Creation
* C.T.	13/09/94	: Rajout du test sur le site
*                         et du param d'entree TypeAstreinte
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAC10_Ajouter_Dest_Fax
* 
* Arguments en entree
* XDY_Entier	va_NumAction_in
* XDY_Octet	va_Site_in
* XDY_Entier	va_Interlocuteur_in
* XDY_Octet	va_TypeAstreinte_in
* XDY_Tel	va_NumeroTalFax_in
* XDY_Horodate	va_HoroLancement_in
* XDY_Operateur	va_Operateur_in
* 
* Arguments en sortie
* XDY_Entier	va_NumAction_out
* 
* Service rendu
* Inserer une actio d'envoi d'un fax deja envoye a un autre destinataire avce XZAC09.
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
* Verifier l'existence de l'actio et du fax lie a l'actio passee en entree
* Appeler XZAC11
-----------------------------------------------------------------------*/

 
#define XZAC10_Ajouter_Dest_Fax	"XZAC;10" 
