/*E*/
/*  Fichier : $Id: xzao591sp.prc,v 1.2 2018/01/09 12:03:13 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2018/01/09 12:03:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao591sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* efface la config des fmc  regulation
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou  	08/05/12 : Creation (DEM/1016)
* LCL		05/07/17 : modif PRA
* JPC		23/03/23 : Correction virgule avant "from"
* GGY		26/09/23 : Ajout district CA (DEM483)
* GGY		17/10/23 : Correction si CFG vide (DEM483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAO591_Efface_Config_FMC_Regulation
*
* Arguments en entree
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Lecture des des infos associees a un evt
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO591' and type = 'P')
	drop procedure XZAO591
go

create procedure XZAO591
	@va_Basedd_in                     char(3)
as
declare 	@vl_Site	char(10)

		/* Verification que le site est bien celui du CI ou CA */
		select  @vl_Site = @@servername
	   
		/* Si le site est CI ou CA */
		if @vl_Site=XDC_SQLCI or @vl_Site=XDC_SQLCA
		begin
  
		/*A Recherche les infos associees a un evt */
		if @va_Basedd_in = XDC_BASE_CFG
			delete from CFG..REG_TYP
		else
			delete from CFT..REG_TYP
 	end	
  /*A
  ** Si autre serveur
  */
else
	begin
		/*A Recherche les infos associees a un evt */
		if @va_Basedd_in = XDC_BASE_CFG
			delete from CFG..REG_TYP
	end
	return XDC_OK

go

