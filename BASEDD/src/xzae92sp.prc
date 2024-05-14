/*E*/
/*  Fichier : @(#)xzae92sp.prc  1.1      Release : 1.1        Date : 20/06/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae92sp.prc
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
* SP   XZAE92_Lect_Lib_Pre
*
* Arguments en entree
* smallint   va_Notyp_in
*
* Arguments en sortie
* smallint 	va_Notyp_out
* int		va_Code_out
* smallint 	va_Position_out
* char(100)	va_Libelle_out
* char(100)	va_Explication_out
* bit		va_Valide_out
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Lecture des libelles associes au type de libelles en entree (base CFG)
-------------------------------------------------------*/

use PRC
go

create procedure XZAE;92
	@va_Notyp_in	smallint
as
	/*A Recherche les libelles associes a un type*/
	select *
	from CFG..LIB_PRE
	where notyp = @va_Notyp_in
	order by position, libelle

	return XDC_OK

go

