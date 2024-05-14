/*E*/
/*  Fichier : $Id: xzac31sp.h,v 1.1 1994/11/03 14:52:49 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:52:49 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac31sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	08/09/94	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Lecture d'une actio d'appel telephonique ou bip.
* 
* Sequence d'appel
* SP	XZAC31_Lire_Appel_Tel_Bip
* 
* Arguments en entree
* XDY_Entier	va_NumAction_in
* XDY_Octet	va_Site_in
* 
* Arguments en sortie
* XDY_Entier	va_Interlocuteur_out
* XDY_Tel	va_NumeroTalFax_out
* XDY_Octet	va_TypeAppel_out
* XDY_Horodate	va_HoroLancement_out
* XDY_Operateur	va_Operateur_out
* XDY_Horodate	va_HoroEchec_out
* XDY_Horodate	va_HoroSucces_out
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
* Select dans les tables ACTION et APPEL_TELEPHONE_BIP_OU_FAX
------------------------------------------------------*/

 
#define XZAC31_Lire_Appel_Tel_Bip	"XZAC;31" 
