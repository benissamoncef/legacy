/*E*/
/*  Fichier : $Id: xzae128sp.prc,v 1.2 1997/12/01 15:36:26 verdier Exp $      Release : $Revision: 1.2 $        Date : $Date: 1997/12/01 15:36:26 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae128sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
* supprime un evt strada de la liste des evts strada
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou  08/05/97        : Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE128_Efface_EVT_Strada
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

if exists (select * from sysobjects where name = 'XZAE128' and type = 'P')
	drop procedure XZAE128
go

create procedure XZAE128
	@va_reference_in	char(50) = null,
	@va_type_in		char(50) = null
as
	/*controle les parametres obligatoires*/
	if @va_reference_in=null or @va_type_in=null
	  return XDC_ARG_INV

	/*A Recherche les infos associees a un evt */
	begin tran
		delete from EXP..ADA_SYN
		where reference = @va_reference_in
		and type = @va_type_in
	commit tran

	return XDC_OK

go

