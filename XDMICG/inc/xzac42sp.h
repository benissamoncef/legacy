/*E*/
/*  Fichier : $Id: xzac42sp.h,v 1.1 1994/11/03 14:53:00 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:53:00 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac42sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	09/09/94	: Creation
* C.T.	03/11/94	: Enlever les lignes en double generees par sccs
*                         (pas reteste)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Lecture de l'actio d'affichage tunnel
* 
* Sequence d'appel
* SP	XZAC42_Lire_Commande_Tunnel
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
* Select des tables ACTION et AFFICHAGE_TUNNEL
-----------------------------------------------------*/

 
#define XZAC42_Lire_Commande_Tunnel	"XZAC;42" 
