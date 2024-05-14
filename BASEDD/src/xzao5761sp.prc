/* Fichier : $Id: xzao5761sp.prc
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao5761sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* GGY	08/12/23 : Creation pour PIC (DEM-483)
------------------------------------------------------*/

/* fichiers inclus */
/* declaration de fonctions internes */
/* definition de fonctions externes */

/*------------------------------------------------------
* SERVICE RENDU : 
*
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO5761' and type = 'P')
	drop procedure XZAO5761
go

create procedure XZAO5761
	@va_Autoroute_in tinyint,
	@va_Sens_in      tinyint,
	@va_Debut_in     int,
	@va_Fin_in       int
	as 
/* 
*
* SP    XZAO576SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd  va_Basedd_in
*             va_Autoroute_in
*             va_Sens_in
*             va_Debut_in
*             va_Fin_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf va_Resultat_out    

* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
*
* FONCTION :
*
* Recherche les PIC dans la zone de chantier et le premier PIC en amont du chantier.
*
* MECANISMES :
*
------------------------------------------------------*/
	declare @vl_pr_min int, 
	        @vl_pr_max int

	create table #TMP_PIC
	(
		numero    smallint null,
		district  tinyint  null,
		autoroute tinyint  null,
		PR        int      null,
		sens      tinyint  null,
		amont     int      null
	)


	if  @va_Autoroute_in = null or @va_Debut_in = null or @va_Fin_in = null or @va_Sens_in = null 
		return XDC_ARG_INV

	if @va_Debut_in <= @va_Fin_in
	begin
		select @vl_pr_min = @va_Debut_in
		select @vl_pr_max = @va_Fin_in
	end
	else
	begin
		select @vl_pr_max = @va_Debut_in
		select @vl_pr_min = @va_Fin_in	
	end

	/* Ramene le sens sur section courante s'il s'agit d'une sortie ou d'une entree */
	if  @va_Sens_in = XDC_SENS_SORTIE_NORD or @va_Sens_in = XDC_SENS_ENTREE_NORD or @va_Sens_in = XDC_SENS_NORD
		select  @va_Sens_in = XDC_SENS_NORD

	if  @va_Sens_in = XDC_SENS_SORTIE_SUD or @va_Sens_in = XDC_SENS_ENTREE_SUD or @va_Sens_in = XDC_SENS_SUD
		select  @va_Sens_in = XDC_SENS_SUD
		
	if  @va_Sens_in <> XDC_SENS_NORD and @va_Sens_in <> XDC_SENS_SUD
		return XDC_OK
		
	insert into #TMP_PIC
		select distinct 
			EQT.numero,
			DSP.sit,
			@va_Autoroute_in,
			EQT.PR,
			@va_Sens_in,0
				from CFG..EQT_GEN EQT,
					CFG..EQT_PIC PIC,
					EXP..EQT_DSP DSP
						where	EQT.type = XDC_EQT_PIC           	and
								EQT.autoroute = @va_Autoroute_in 	and
								EQT.sens = @va_Sens_in           	and
								EQT.PR >= @vl_pr_min             	and
								EQT.PR <= @vl_pr_max             	and
								PIC.numero  = EQT.numero      		and
								DSP.equipement = EQT.numero      	and
								DSP.type = XDC_EQT_PIC           	and
								DSP.dernier = 1                  	and     -- derniere dispo connue
								DSP.inhibe = 0                   	and     -- non inhibe
								DSP.desactive = 0                	and     -- non desactive
								DSP.majeure = 0                  	and     -- en panne
								DSP.HS = 0                       	        -- non HS
		
	set rowcount 0
	select * from #TMP_PIC
	return XDC_OK

go  
