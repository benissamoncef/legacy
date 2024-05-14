/*E*/
/*  Fichier : $Id: xzag19sp.prc,v 1.2 1995/09/15 09:53:54 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/09/15 09:53:54 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag19sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	20/06/95	: Creation	(V 1.1)
* C.T.	15/09/95	: Ajout delete de FMC_ACT	(V 1.2)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Purge des donnees de la base d'exploitation datant
* d'au moins la date precisee
* 
* Sequence d'appel
* SP	XZAG19_Purge_Appels_Fax_Base_Exploitation
* 
* Arguments en entree
* XDY_Datetime	va_Horodate_in	: date de fin de purge
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametre d'entree null
* 
* Conditions d'utilisation
* 
* Fonction
* Suppression des lignes vielles de plus d'un mois
-------------------------------------------------------*/

use PRC
go

create procedure XZAG;19
	@va_Horodate_in		datetime = null
as
	/*A controle du parametre en entree */
	if @va_Horodate_in = null
		return XDC_ARG_INV

	/*A Effacer tous les appels par fax anterieures a la date */
	/*A donnee                                                */
	delete EXP..APL_FAX where horodate <= @va_Horodate_in

	/*A effacer tous les compteurs d'actions, evenements */
	delete EXP..FMC_ACT

	return XDC_OK
go
