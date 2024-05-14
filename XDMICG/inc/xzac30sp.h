/*E*/
/*  Fichier : $Id: xzac30sp.h,v 1.1 1994/11/03 14:52:49 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:52:49 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac30sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	08/09/94	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Lecture d'une actio d'envoi d'un fax
* 
* Sequence d'appel
* SP	XZAC30_Lire_Fax
* 
* Arguments en entree
* XDY_Entier		va_NumAction_in
* XDY_Octet		va_Site_in
* 
* Arguments en sortie
* XDY_Entier		va_Interlocuteur_out
* XDY_Tel		va_NumeroTalFax_out
* XDY_Horodate		va_HoroLancement_out
* XDY_Operateur		va_Operateur_out
* XDY_Horodate		va_HeureSucces_out
* XDY_Horodate		va_HeureEchec_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Le texte du fax est retourne sous forme de liste de morceaux de texte ordonnes
* 
* Fonction
* Recuperer de la table TEXTE_FAX le texte du fax par morceau et le reconstituer
* Appeler XZAC31
----------------------------------------------------*/

 
#define XZAC30_Lire_Fax	"XZAC;30" 
