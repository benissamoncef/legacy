/*E*/
/*  Fichier : $Id: xzag31sp.prc,v 1.2 2002/03/07 10:10:27 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2002/03/07 10:10:27 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag31sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	10/08/95	: Creation	(V 1.1)
* P.C.	06/03/02        : Correctionn pour HIS V1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Purge des donnees de la base de l'historique datant
* d'au moins la date precisee
* 
* Sequence d'appel
* SP	XZAG31_Purge_Appels_Horaire_Base_Historique
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
* Suppression des lignes vielles de plus de 5 ans
-------------------------------------------------------*/

use PRC
go

create procedure XZAG;31
	@va_Horodate_in		datetime = null
as
#ifdef HIST
	/*A controle du parametre en entree */
	if @va_Horodate_in = null
		return XDC_ARG_INV

	/*A Effacer tous les appels par fax anterieures a la date */
	/*A donnee                                                */
	delete HIS..APL_HOR where horodate <= @va_Horodate_in
#endif
	return XDC_OK
go
