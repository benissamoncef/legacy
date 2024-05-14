/*E*/
/*  Fichier : $Id: xzae144sp.prc,v 1.3 1999/02/26 11:24:30 gaborit Exp $      Release : $Revision: 1.3 $        Date : $Date: 1999/02/26 11:24:30 $
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
* Niepceron	24 Nov 1998	  : Modif pour intergestionnaire dem/1724 v1.3
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP XZAE144_Synthese_Situation_Strada
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

if exists (select * from sysobjects where name = 'XZAE144' and type = 'P')
	drop procedure XZAE144
go

create procedure XZAE144
	@va_alerte_in        int
as
	if @va_alerte_in = null
		return XDC_ARG_INV

	select emetteur,reference,type,horodate from EXP..ADA_SIT where alerte=@va_alerte_in

	select texte from EXP..ADA_SIT SIT , EXP..ADA_TXT_SYN TXT 
	where alerte=@va_alerte_in and
		SIT.emetteur=TXT.emetteur and
		SIT.reference=TXT.reference
	order by morceau
	
	return XDC_OK
go

