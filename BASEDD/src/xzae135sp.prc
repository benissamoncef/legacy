/*E*/
/*  Fichier : $Id: xzae135sp.prc,v 1.2 1999/02/26 14:20:25 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1999/02/26 14:20:25 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae127sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
* strada : infos de la fmc meteo
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou	07 oct 1997 : creation
* Niepceron     11 Jan 1999 : modif intergest : !!! la longueur renvoir le pr tete !!! v1.2 dem/1624
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE135_Infos_Meteo_Pour_Strada
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

if exists (select * from sysobjects where name = 'XZAE135' and type = 'P')
	drop procedure XZAE135
go

create procedure XZAE135
	@va_numero_in		int	= null,
	@va_cle_in		tinyint = null,
	@va_longueur_out	T_PR output,
	@va_visibilite_out	smallint output,
	@va_type_out		tinyint output
as
	if @va_numero_in is null
		return XDC_ARG_INV
		
	if @va_cle_in is null
		return XDC_ARG_INV
	
	select 	@va_longueur_out = MET.PR,
		@va_visibilite_out = MET.visibilite,
		@va_type_out = MET.type_de_perturbation
	from EXP..FMC_MET MET
	where 
		MET.numero = @va_numero_in and
		MET.cle = @va_cle_in 
	return XDC_OK
go

