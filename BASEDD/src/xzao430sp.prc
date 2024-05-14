/*E*/
/*  Fichier : @(#)xzao430sp.prc	1.1        Release : 1.1        Date : 03/15/05
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao430sp.prc
------------------------------------------------------
*
*/
/*------------------------------------------------------
* HISTORIQUE :
*
* CHI	04/04/05: Creation
* ---------------------------------------------------*/

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste les libelles des bouchons pre-definis
*
------------------------------------------------------
* SEQUENCE D'APPEL :

* SP	XZA0430_Select_DEF_BOU
* 
* Arguments en entree
* 
* Arguments en sortie
*
* Service rendu
*
* Liste les libelles des bouchons pre-definis pour config table DEF_BOU
*
* Code retour
* XDC_OK
* XDC_NOK       :
* XDC_ARG_INV   : parametres d'entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
-------------------------------------------------------*/
use PRC
go

if exists (select * from sysobjects where name = 'XZAO430' and type = 'P')
	drop procedure XZAO430
go

create procedure XZAO430

as
	declare @vl_Site T_SITE

	begin
  		select  
			libelle,
			confirme_ou_signale,
			autoroute,
			PR,
			sens,
			point_caracteristique,
			numero_point_caracteristique,
			VR,
			VM2,
			VM1,
			VL,
			BAU,
			type,
			longueur,
			sit

		from CFG..DEF_BOU 
	end
	return XDC_OK

go
