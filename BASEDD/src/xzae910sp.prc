/*E*/
/*  Fichier : @(#)xzae910sp.prc  1.1      Release : 1.1        Date : 20/06/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae910sp.prc
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
* SP   XZAE910_Lect_Liste_CFE
*
* Arguments en entree
*
* Arguments en sortie
* smallint 	va_Numero_out
* char(25)	va_Nom_out
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Lecture de tous les elements de la table EQT_GEN (base CFG)
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE910' and type = 'P')
	drop procedure XZAE910
go

create procedure XZAE910
	as
	
	/*A Recherche des CFE */
	select numero, nom
	from CFG..EQT_GEN
	where type=66
	order by nom

	return XDC_OK

go

