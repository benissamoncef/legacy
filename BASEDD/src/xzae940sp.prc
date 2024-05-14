/*E*/
/*  Fichier : @(#)xzae940sp.prc  1.1      Release : 1.1        Date : 20/06/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae940sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des types de libelles et des libelles predefinis
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL  13/04/18        : Creation CTRL_FEUX 1.1 DEM1284
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE940_MAJ_LIB
*
* Arguments en entree
* smallint   	va_NumeroCFE_in,
* int 		va_Code_in
* char(150)	va_Libelle_in
* bit 		va_Valide_in
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* XDC_NOK
* XDC_ARG_INV   : parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Mise a jour d'un element de la table DEF_PDF (base CFG)
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE940' and type = 'P')
	drop procedure XZAE940
go

create procedure XZAE940
	@va_NumeroCFE_in		smallint	= null,
	@va_Code_in		int		= null,
	@va_Libelle_in		char(150)	= null,
	@va_Valide_in		bit		= 0
as

	/*! transfo en parametres nulls bug applix */
	if @va_NumeroCFE_in = 0 select @va_NumeroCFE_in = null

	/* controle les parametres obligatoires */
	if @va_NumeroCFE_in = null or @va_Code_in = null 
			or @va_Libelle_in = null 
		return XDC_ARG_INV
	
	/*A controle existence du libelle */
	if not exists (select * 
			from CFG..DEF_PDF
			where numero = @va_NumeroCFE_in and plan_de_feux = @va_Code_in)
		return XDC_NOK
	/*A indiquer la fin de la commande */
	begin tran
		/*B Mise a jour d'un libelle plan de feux */
		update CFG..DEF_PDF
			set 
			    libelle		= @va_Libelle_in,
			    valide		= @va_Valide_in
			where numero = @va_NumeroCFE_in and plan_de_feux = @va_Code_in

	commit tran

	return XDC_OK

go

