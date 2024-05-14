/*E*/
/*  Fichier : $Id: xzae132sp.prc,v 1.1 1997/12/01 15:31:38 verdier Exp $      Release : $Revision: 1.1 $        Date : $Date: 1997/12/01 15:31:38 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae127sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
* strada : infos de la fmc vehicule lent
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
* SP   XZAE130_Infos_VehLent_Pour_Strada
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

if exists (select * from sysobjects where name = 'XZAE132' and type = 'P')
	drop procedure XZAE132
go

create procedure XZAE132
	@va_numero_in		int	= null,
	@va_cle_in		tinyint = null,
	@va_longueur_out	smallint output,
	@va_vitesse_out		tinyint output
as
	if @va_numero_in is null
		return XDC_ARG_INV
		
	if @va_cle_in is null
		return XDC_ARG_INV
	
	select 	@va_longueur_out = longueur,
		@va_vitesse_out = vitesse
	from EXP..FMC_VLN
	where 
		numero = @va_numero_in and
		cle = @va_cle_in 
        group by numero, cle
        having horodate = max(horodate)
	
	return XDC_OK
go

