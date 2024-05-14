/*E*/
/*  Fichier : @(#)xzae231sp.prc  1.1      Release : 1.1        Date : 20/06/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae231sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture desbelles de plans de feux 
*
------------------------------------------------------
* HISTORIQUE :
*
* JMg	01/012/18	creation AVA 1.1
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE231_Creation_Lib_Plan_De_Feux
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

drop procedure XZAE231
go

create procedure XZAE231
	@va_identifiant_in	char(50)	,
	@va_numero_in		int		,
	@va_cle_in		tinyint
as

	/* controle les parametres obligatoires */
	if @va_identifiant_in = null or @va_numero_in = null or @va_cle_in = null 
		return XDC_ARG_INV

	/*A indiquer la fin de la commande */
	if exists (select * from EXP..FMC_AVA
	where identifiant  = @va_identifiant_in
	and numero = @va_numero_in
	and cle = @va_cle_in)
		return XDC_NOK

	begin tran
		insert EXP..FMC_AVA (identifiant, numero, cle)
		values (@va_identifiant_in, @va_numero_in, @va_cle_in)

	commit tran

	return XDC_OK

go

