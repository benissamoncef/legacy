/*E*/
/*  Fichier : $Id: xzae134sp.prc,v 1.2 2019/04/01 17:56:13 devgfi Exp $        $Revision: 1.2 $        $Date: 2019/04/01 17:56:13 $
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
* JPL		15/03/19 : Sélection et retour des données les plus récentes (DEM 1327)  1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE134_Infos_Manif_Pour_Strada
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

if exists (select * from sysobjects where name = 'XZAE134' and type = 'P')
	drop procedure XZAE134
go

create procedure XZAE134
	@va_numero_in		int	= null,
	@va_cle_in		tinyint = null,
	@va_nombre_out		smallint output,
	@va_perception_out	bit output,
	@va_violence_out	bit output
as
	if @va_numero_in is null
		return XDC_ARG_INV
		
	if @va_cle_in is null
		return XDC_ARG_INV
	
	select 	@va_nombre_out = nombre_de_manifestants,
		@va_violence_out = violence,
		@va_perception_out = perception_peage
	from EXP..FMC_MNF
	where 
		numero = @va_numero_in and
		cle = @va_cle_in 
	group by numero, cle
	having validation = max(validation)

	if @@rowcount != 1
		return XDC_NOK

	return XDC_OK
go

