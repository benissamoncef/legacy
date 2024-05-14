/*E*/
/*  Fichier : $Id: xzae198sp.prc,v 1.1 2020/07/09 08:37:37 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2020/07/09 08:37:37 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae198sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Lit les coordonnées relative à une autoroute et un PR
*
---------------------------------------------------------------
* HISTORIQUE :
* ABE	23/06/2020	: Creation DEM 130 V1.0
--------------------------------------------------------------*/

/*X*/
/*-------------------------------------------------------
* Sequence d'appel
* SP	xzae198
* 
* Arguments en entree
* XDY_Octet	 va_autoroute_in
* XDY_PR	 va_pr_in
*
* Arguments en sortie
* T_STR		 va_lat_out
* T_STR		 va_lon_out
*
* Code retour
* XDC_OK
* XDC_NOK		: fiche non trouvee
* XDC_ARG_INV		: argument d'entree  null
* XZAEC_FMC_PAS_RETYPAGE : retypage non autorise (16)
* < 0			: erreur sybase 
* 
* Conditions d'utilisation
* ne fonctionne pas sur pc simplifie 
* 
* Fonction
*
----------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE198' and type = 'P')
	drop procedure XZAE198
go

create procedure XZAE198		
	@va_autoroute_in      tinyint = null,
	@va_pr_in       	T_PR = null,
	@va_lat_out        char(15) = null output,
	@va_lon_out        char(15) = null output
as

  /*A
  ** Test des parametres d'entree :
  **   Si un des parametres d'entree est egal a null alors XDC_ARG_INV
  */
  if @va_autoroute_in = 0 select @va_autoroute_in = null
  if @va_pr_in = 0 select @va_pr_in = null
	
  if @va_autoroute_in = null
  or @va_pr_in = null
	return ( XDC_ARG_INV )

	select 
		/* lecture des informations de LOC_GPS */
		--@va_lat_out	= lat,
		--@va_lon_out	= lon
		lat,
		lon
	from CFG..LOC_GPS
	where	CFG..LOC_GPS.autoroute=@va_autoroute_in and
		CFG..LOC_GPS.PR=@va_pr_in
		
		if @@rowcount = 0
		begin			
			return XDC_NOK
		end		
			return XDC_OK
go

