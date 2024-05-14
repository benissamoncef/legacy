/*E*/
/*  Fichier : @(#)xzae158sp.prc	1.6      Release : 1.6        Date : 03/19/99
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae158sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Recherche des infos appartenant au bloc obligatoire
* d'un fichier STRADA.
------------------------------------------------------
* HISTORIQUE :
*
* ABE	15/04/21	: Creation DEM-SAE130 1.1
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE158_Date_prevue
*
* Arguments en entree
* - numero de la FMC
* - cle de la FMC
*
* Arguments en sortie
* XDY_Horodate		va_debut_prevu_out
* XDY_Horodate		va_fin_prevu_out

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

if exists (select * from sysobjects where name = 'XZAE158' and type = 'P')
	drop procedure XZAE158
go

create procedure XZAE158
	@va_numero_in		int	= null,
	@va_cle_in		tinyint = null,
	@va_debut_prevu_out	datetime output,
	@va_fin_prevu_out	datetime output
	
as
	
	if @va_numero_in is null
		return XDC_ARG_INV
		
	if @va_cle_in is null
		return XDC_ARG_INV
	
	/*A Recherche les infos associees a un evt ds FMC_GEN */
	select 	@va_fin_prevu_out = fin_prevue,
		@va_debut_prevu_out = debut_prevu
	from EXP..FMC_GEN 
	where 
		numero = @va_numero_in and
		cle = @va_cle_in 

	return XDC_OK

go

