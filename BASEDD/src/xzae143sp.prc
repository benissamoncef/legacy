/*E*/
/*  Fichier : @(#)xzae143sp.prc	1.1      Release : 1.1        Date : 05/13/97
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae143sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* lecture des evenemets strada en base
------------------------------------------------------
* HISTORIQUE :
*
*  Penot	05 Nov 1997       : Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE143_Purger_Evenement
*
* Arguments en entree
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Lecture des des infos associees a un evt
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE143' and type = 'P')
	drop procedure XZAE143
go

create procedure XZAE143
	@va_reference_in        char(50)
as
	if @va_reference_in = null
		return XDC_ARG_INV

	delete from EXP..ADA_SYN
	where reference = @va_reference_in
	return XDC_OK
go

