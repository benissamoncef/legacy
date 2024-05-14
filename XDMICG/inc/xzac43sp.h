/*E*/
/*  Fichier : $Id: xzac43sp.h,v 1.1 1994/11/03 14:53:01 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:53:01 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac43sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	09/09/94	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Lecture de l'actio d'affichage echangeur
* 
* Sequence d'appel
* SP	XZAC43_Lire_Commande_Echangeur
* 
* Arguments en entree
* XDY_Entier	va_NumeroAction_in
* XDY_Octet	va_Site_in
* 
* Arguments en sortie
* XDY_Eqt	va_NumEqt_out
* XDY_Horodate	va_HeureLancement_out
* XDY_Horodate	va_HeureLancementFin_out
* XDY_Horodate	va_HeureSucces_out
* XDY_Horodate	va_HeureEchec_out
* XDY_Horodate	va_HeureFin_out
* XDY_Booleen	va_Manuelle_out
* XDY_Octet	va_Sequence_out
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
* Select des tables ACTION et AFFICHAGE_ECHANGEUR
-----------------------------------------------------*/

 
#define XZAC43_Lire_Commande_Echangeur	"XZAC;43" 
