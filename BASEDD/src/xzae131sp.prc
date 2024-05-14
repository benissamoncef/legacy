/*E*/
/*  Fichier : $Id: xzae131sp.prc,v 1.1 1997/12/01 15:31:29 verdier Exp $      Release : $Revision: 1.1 $        Date : $Date: 1997/12/01 15:31:29 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae127sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
* strada : infos de la fmc accident
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou	07 oct 1997 : creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE131_Infos_Delestage_Pour_Strada
*
* Arguments en entree
* - numero de la FMC
* - cle de la FMC
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
* Lecture des des infos obligatoires pour generer
* un fichier STRADA
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE131' and type = 'P')
	drop procedure XZAE131
go

create procedure XZAE131
	@va_numero_in		int	= null,
	@va_cle_in		tinyint = null,
	@va_plan_out		smallint output,
	@va_responsable		char(25) output
as
	if @va_numero_in is null
		return XDC_ARG_INV
		
	if @va_cle_in is null
		return XDC_ARG_INV
	
	select 	@va_plan_out = plan_associe,
		@va_responsable = responsable
	from EXP..FMC_DEL
	where 
		numero = @va_numero_in and
		cle = @va_cle_in 
	
	return XDC_OK
go

