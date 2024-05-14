/*E*/
/*  Fichier : $Id: xzae87sp.h,v 1.1 1995/08/04 18:06:41 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/08/04 18:06:41 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae87sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* lecture et ecriture des fmc
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	03/0r87/95	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAE87_SyntheseTravaux
* 
* Arguments en entree
* XDY_Horodate	va_HoroDeb_in
* XDY_Horodate	va_HoroFin_in
* 
* Arguments en sortie
* 
* liste retournee :
* XDY_Horodate	va_HoroFin_in

* Service rendu
* Synthese des travaux en cours ou en prevision a partir de la
* date actuelle
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametres d'entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Si la date de fin n'est pas valuée alors la synthese est sur 7 jours
* 
* Fonction
-----------------------------------------------------------------------*/

 
#define XZAE87_SyntheseTravaux	"XZAE;87" 
