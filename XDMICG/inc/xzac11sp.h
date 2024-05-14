/*E*/
/*  Fichier : $Id: xzac11sp.h,v 1.1 1994/11/03 14:52:30 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:52:30 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac11sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	05/09/94	: Creation
* C.T.	13/09/94	: Modif. ajout type astreinte en entree
*                         et l'insertion de l'enregistement
* C.T.	02/11/94	: Modif. les valeurs possibles de TypeAppel
*                         et modif de la valeur inseree pour
*                         le type d'action dans ACT_GEN
*			  (pas reteste) V 1.3
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Inserer une action d'appel telephonique ou bip.
* 
* Sequence d'appel
* SP	XZAC11_Ajouter_Appel_Tel_Bip
* 
* Arguments en entree
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* XDY_Mot	va_Interlocuteur_in
* XDY_Octet	va_TypeAstreinte_in
* XDY_Tel	va_NumeroTelFax_in
* XDY_Octet	va_TypeAppel_in		: tel ou bip ou fax
* XDY_Horodate	va_HoroLancement_in
* XDY_Operateur	va_Operateur_in
* XDY_Horodate	va_HoroEchec_in
* XDY_Horodate	va_HoroSucces_in
* 
* Arguments en sortie
* XDY_Entier		NumeroAction
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Appele une seule fois une fois que l'operateur a declare le succes ou l'echec 
* de l'appel sauf pour un fax
* 
* Fonction
* Verifier l'existence de l'evenement
* Inserer dans les tables ACTION et APPEL_TELEPHONE_BIP_OU_FAX avec le 
* site defini en interne
-----------------------------------------------------------------------*/

 
#define XZAC11_Ajouter_Appel_Tel_Bip	"XZAC;11" 
