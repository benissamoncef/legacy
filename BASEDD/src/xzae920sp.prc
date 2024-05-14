/*E*/
/*  Fichier : @(#)xzae920sp.prc  1.1      Release : 1.1        Date : 20/06/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae920sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des types de libelles et des libelles predefinis
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
* SP   XZAE920_Lect_Lib_CFE
*
* Arguments en entree
* smallint   va_Numero_in
*
* Arguments en sortie
* smallint 	va_Numero_out
* int		va_Code_out
* char(150)	va_Libelle_out
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

drop procedure XZAE920
go

create procedure XZAE920
	@va_Numero_in	smallint
as
	/*A Recherche les libelles associes a un type*/
	select *
	from CFG..DEF_PDF
	where numero = @va_Numero_in
	order by plan_de_feux

	return XDC_OK

go

