/*E*/
/*  Fichier : $Id: xzae130sp.prc,v 1.2 2008/12/10 13:19:26 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2008/12/10 13:19:26 $
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
* JMG	03/12/08 : ajout validation dans FMC_ACC DEM/852
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE130_Infos_Accident_Pour_Strada
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

if exists (select * from sysobjects where name = 'XZAE130' and type = 'P')
	drop procedure XZAE130
go

create procedure XZAE130
	@va_numero_in		int	= null,
	@va_cle_in		tinyint = null,
	@va_vehicules_out	tinyint output,
	@va_cars 		tinyint output,
	@va_blesses		tinyint output,
	@va_morts		tinyint output

as
	if @va_numero_in is null
		return XDC_ARG_INV
		
	if @va_cle_in is null
		return XDC_ARG_INV
	
	select 	@va_vehicules_out = nombre_de_vl + nombre_de_pl + nombre_de_remorques + nombre_de_car + nombre_de_moto ,
		@va_cars = nombre_de_car,
		@va_blesses = blesses_legers + blesses_graves,
		@va_morts = morts
	from EXP..FMC_ACC
	where 
		numero = @va_numero_in and
		cle = @va_cle_in 
	group by numero, cle
	having validation=max(validation)

	if @@rowcount != 1
		return XDC_NOK

	return XDC_OK
go

