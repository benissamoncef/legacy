/*E*/
/*  Fichier : @(#)xzae93sp.prc  1.1      Release : 1.1        Date : 20/06/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae93sp.prc
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
* SP   XZAE93_Creation_Lib_Pre
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

create procedure XZAE;93
	@va_Notyp_in		smallint	= null,
	@va_Code_in		int		= null,
	@va_Position_in		smallint	= null,
	@va_Libelle_in		char(100)	= null,
	@va_Explication_in	char(100)	= null,
	@va_Valide_in		bit		= 0
as
	/*! transfo en parametres nulls bug applix */
	if @va_Explication_in = XDC_CHAINE_VIDE select @va_Explication_in = null

	/* controle les parametres obligatoires */
	if @va_Notyp_in = null or @va_Code_in = null or @va_Position_in = null 
			or @va_Libelle_in = null 
		return XDC_ARG_INV

	/*A indiquer la fin de la commande */
	begin tran
		/*B Insertion d'un libelle predefini */
		insert CFG..LIB_PRE (notyp, code, position, libelle, explication, valide)
		values (@va_Notyp_in, @va_Code_in, @va_Position_in, @va_Libelle_in, 
			@va_Explication_in, @va_Valide_in)

	commit tran

	return XDC_OK

go

