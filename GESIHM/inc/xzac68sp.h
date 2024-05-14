/*E*/
/*  Fichier : $Id: xzac68sp.h,v 1.1 1998/01/20 14:27:27 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1998/01/20 14:27:27 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac68sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	19/01/98	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Lecture de l'action d'affichage PAL
* 
* Sequence d'appel
* SP	XZAC68_Lire_Commande_PAL
* 
* Arguments en entree
* XDY_Entier	va_NumAction_in
* XDY_Octet	va_Site_in
* 
* Arguments en sortie
* XDY_Eqt	va_NumEqt_out
* XDY_LignePAL	va_LibelleMod1_out
* XDY_LignePAL	va_LibelleMod2_out
* XDY_LignePAL	va_LibelleMod3_out
* XDY_LignePAL	va_LibelleMod4_out
* XDY_LignePAL	va_LibelleMod5_out
* XDY_Horodate	va_HeureLancement_out
* XDY_Horodate	va_HeureLancementFin_out
* XDY_Horodate	va_HeureSucces_out
* XDY_Horodate	va_HeureEchec_out
* XDY_Horodate	va_HeureFin_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* La procedure stockee au CI recherche les informations 
* dans la base HIS si les informations n'existent pas dans
* EXP tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Le code de recherche d'infos de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
* 
* 
* Fonction
* Select dans les tables ACTION et E_PAL
-----------------------------------------------------------------------*/

 
#define XZAC68_Lire_Commande_PAL	"XZAC;68" 
