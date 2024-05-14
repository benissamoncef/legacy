/*E*/
/*  Fichier : $Id: xzae133sp.prc,v 1.1 1997/12/01 15:31:40 verdier Exp $      Release : $Revision: 1.1 $        Date : $Date: 1997/12/01 15:31:40 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae127sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
* strada : infos de la fmc ss concessionnaire
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
* SP   XZAE133_Infos_SousCnc_Pour_Strada
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

if exists (select * from sysobjects where name = 'XZAE133' and type = 'P')
	drop procedure XZAE133
go

create procedure XZAE133
	@va_numero_in		int	= null,
	@va_cle_in		tinyint = null,
	@va_nom_out		char(25) output,
	@va_ferme_out		bit output,
	@va_penurie_out		bit output
as
	if @va_numero_in is null
		return XDC_ARG_INV
		
	if @va_cle_in is null
		return XDC_ARG_INV
	
	select 	@va_nom_out = rtrim(nom_ss_concessionnaire),
		@va_ferme_out = ferme,
		@va_penurie_out = penurie
	from EXP..FMC_CNC
	where 
		numero = @va_numero_in and
		cle = @va_cle_in 
	
	return XDC_OK
go

