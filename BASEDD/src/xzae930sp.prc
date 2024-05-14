/*E*/
/*  Fichier : @(#)xzae930sp.prc  1.1      Release : 1.1        Date : 20/06/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae930sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture desbelles de plans de feux 
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL  16/04/18        : Creation CTRL_FEUX 1.1 DEM1284
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE930_Creation_Lib_Plan_De_Feux
*
* Arguments en entree
* smallint   	va_NumeroCFE_in
* int 		va_Code_in
* char(150)	va_Libelle_in
* bit 		va_Valide_in
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* XDC_ARG_INV   : parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Ajout d'un enregistrement dans la table LIB_PRE (base CFG)
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE930' and type = 'P')
	drop procedure XZAE930
go

create procedure XZAE930
	@va_NumeroCFE_in	smallint	= null,
	@va_Code_in		int		= null,
	@va_Libelle_in		char(150)	= null,
	@va_Valide_in		bit		= 0
as

	/* controle les parametres obligatoires */
	if @va_NumeroCFE_in = null or @va_Code_in = null or @va_Libelle_in = null 
		return XDC_ARG_INV

	/*A indiquer la fin de la commande */
	begin tran
		/*B Insertion d'un libelle predefini */
		insert CFG..DEF_PDF (numero, plan_de_feux, libelle, valide)
		values (@va_NumeroCFE_in, @va_Code_in, @va_Libelle_in, @va_Valide_in)

	commit tran

	return XDC_OK

go

