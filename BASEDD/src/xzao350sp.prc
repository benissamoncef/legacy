/*E*/
/*Fichier : $Id: xzao350sp.prc,v 1.2 2009/10/22 15:51:03 pc2dpdy Exp $     Release : $Revision: 1.2 $        Date :  $Date: 2009/10/22 15:51:03 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao350sp.prc
------------------------------------------------------
* HISTORIQUE :
*
* guilhou       01/10/08	: creation DEM895
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao350sp.h" */

/* static char *version = " $Id: xzao350sp.prc,v 1.2 2009/10/22 15:51:03 pc2dpdy Exp $: xzao350" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  retourne dans quelle zone est situe un evt dans un tunnel
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

drop proc XZAO350
go

create procedure XZAO350

   @va_autoroute_in	tinyint,
   @va_pr_tete_in	int,
   @va_pr_queue_in	int,
   @va_sens_in		tinyint,
   @va_tunnel_in	smallint,
   @va_zone_out		tinyint output,
   @va_type_out		tinyint output,
   @va_groupe_out	char(3) output,
   @va_PRPSTT_out	int output
   as 
	declare @vl_min int, @vl_max int
	if @va_pr_tete_in <= @va_pr_queue_in
	begin
		select  @vl_min = @va_pr_tete_in
		select @vl_max =  @va_pr_queue_in
	end
	else
	begin
		select @vl_min  =  @va_pr_queue_in
		select @vl_max = @va_pr_tete_in
	end

	select @va_zone_out=0
	select @va_type_out=0

	set rowcount 1


	select 	@va_zone_out=numero_zone,
		@va_type_out=type_zone,
		@va_groupe_out=groupe,
		@va_PRPSTT_out = PR_PSTT
	from CFG..ZON_TUN
	where inhibition = 0
	and autoroute=@va_autoroute_in 
	and sens =@va_sens_in
	and numero_tube = @va_tunnel_in
	and not (PR_fin < @vl_min or PR_debut > @vl_max)
	order by type_zone

   return XDC_OK
go 
	 
