/*E*/
/*  Fichier : @(#)xzae94sp.prc  1.1      Release : 1.1        Date : 20/06/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae94sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des types de libelles et des libelles predefinis
*
------------------------------------------------------
* HISTORIQUE :
*
* P.V.  20/06/96        : Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE94_MAJ_LIB
*
* Arguments en entree
* smallint   	va_Notyp_in
* int 		va_Code_in
* smallint 	va_Position_in
* char(100)	va_Libelle_in
* char(100)	va_Explication_in
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
* Mise a jour d'un element de la table LIB_PRE (base CFG)
-------------------------------------------------------*/

use PRC
go

create procedure XZAE;94
	@va_Notyp_in		smallint	= null,
	@va_Code_in		int		= null,
	@va_Position_in		smallint	= null,
	@va_Libelle_in		char(100)	= null,
	@va_Explication_in	char(100)	= null,
	@va_Valide_in		bit		= 0
as

	/*! transfo en parametres nulls bug applix */
	if @va_Notyp_in = 0 select @va_Notyp_in = null
	if @va_Explication_in = XDC_CHAINE_VIDE select @va_Explication_in = null

	/* controle les parametres obligatoires */
	if @va_Notyp_in = null or @va_Code_in = null or @va_Position_in = null 
			or @va_Libelle_in = null 
		return XDC_ARG_INV
	
	/*A controle existence du libelle */
	if not exists (select * 
			from CFG..LIB_PRE
			where notyp = @va_Notyp_in and code = @va_Code_in)
		return XDC_NOK
/*	if @@rowcount != 1 return XDC_NOK
*/
	/*A indiquer la fin de la commande */
	begin tran
		/*B Mise a jour d'un libelle predefini */
		update CFG..LIB_PRE
			set position 		= @va_Position_in,
			    libelle		= @va_Libelle_in,
			    explication		= @va_Explication_in,
			    valide		= @va_Valide_in
			where notyp = @va_Notyp_in and code = @va_Code_in

	commit tran

	return XDC_OK

go

