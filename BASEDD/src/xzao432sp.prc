/*E*/
/* Fichier : $Id: xzao432sp.prc,v 1.6 2017/11/15 12:52:23 devgfi Exp $        Release : $Revision: 1.6 $        Date : $Date: 2017/11/15 12:52:23 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao432sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/
/* fichiers inclus */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des zones Temps De Parcours allant de la zone origine vers la zone destination
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
* SP	XZAO432SP
*
* PARAMETRES EN ENTREE :
*
* @va_zone_origine_in          char(50)
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Retourne la liste des Zone De Parcours destination possibles  a partir de la zone origine
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* Retourne les  libelles des zones destinations tdp possibles
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* CHI 	17/05/2005: Creation DEM468 v1.1- v1.2
* CHI	24/06/2005: Modif pour filtre des zones qui se chevauchent (champ type_zone) v1.3
* LCL   18/01/2011: Modification pour TdP etendus : tous autoroutes toutes zones (DEM/1014)
* JPL	10/10/17 : Remplacement du champ 'code' par 'code_zone' (pour DEM 1236)  1.6
------------------------------------------------------*/
    
use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO432' and type = 'P')
	drop procedure XZAO432
go


create procedure XZAO432
   @va_autoroute_in		integer,
   @va_zone_origine_in_1       	integer,
   @va_zone_origine_in_2	integer
   as 
   declare
		@vl_sens		int,
		@vl_PR1                 int,
		@vl_PR2			int

   if @va_autoroute_in = 0 
   begin
	select distinct numero, nom
	from CFG..RES_AUT
	order by numero
   end
   else
   if @va_zone_origine_in_1 = 0
   begin
	select distinct ZDP.numero,ZDP.libelle 
	from CFG..EQT_ZDP ZDP, CFG..EQT_GEN GEN
	where ZDP.code_zone != 0
	and   GEN.type         = XDC_EQT_ZDP
	and   GEN.autoroute    = @va_autoroute_in
	and   GEN.numero       = ZDP.numero
	and   ZDP.type_zone   != 0
	and   ZDP.inhibition   = 0
	order by GEN.sens, GEN.autoroute, ZDP.PR_fin, ZDP.numero
   end
   else   
   if @va_zone_origine_in_2 = 0
   begin
   
          select @vl_sens = GEN.sens, @vl_PR1 = ZDP.PR_fin
	  from  CFG..EQT_GEN GEN,
		CFG..EQT_ZDP ZDP
	  where	GEN.type         = XDC_EQT_ZDP
	  and   GEN.numero       = @va_zone_origine_in_1
	  and   GEN.autoroute    = @va_autoroute_in
	  and	GEN.numero	 = ZDP.numero


          if @vl_sens = 1
	  begin
	  	select distinct ZDP.numero, ZDP.libelle
              	from    CFG..EQT_GEN GEN, 
              		CFG..RES_AUT AUT,
              		CFG..EQT_ZDP ZDP
               	where GEN.type         = XDC_EQT_ZDP
               	and GEN.numero       = ZDP.numero
               	and AUT.numero       = GEN.autoroute
		and GEN.autoroute    = @va_autoroute_in
               	and ZDP.inhibition   = 0
		and GEN.sens 	     = @vl_sens
		and ZDP.PR_fin       > @vl_PR1
		and ZDP.code_zone	     != 0
               	order by GEN.sens, GEN.autoroute, ZDP.PR_fin, ZDP.numero
    	  end

          if @vl_sens = 2
	  begin
	  	select distinct ZDP.numero, ZDP.libelle
              	from    CFG..EQT_GEN GEN, 
              		CFG..RES_AUT AUT,
              		CFG..EQT_ZDP ZDP
               	where GEN.type         = XDC_EQT_ZDP
               	and GEN.numero       = ZDP.numero
               	and AUT.numero       = GEN.autoroute
		and GEN.autoroute    = @va_autoroute_in
               	and ZDP.inhibition   = 0
		and GEN.sens 	     = @vl_sens
		and ZDP.PR_fin       < @vl_PR1 
		and ZDP.code_zone         != 0
                order by GEN.sens, GEN.autoroute, ZDP.PR_fin desc, ZDP.numero
    	 end

    end
    else
    if @va_zone_origine_in_2 <> 0
    begin
		
		select @vl_sens = GEN.sens
		from    CFG..EQT_GEN GEN
		where   GEN.type         = XDC_EQT_ZDP
		  and   GEN.numero       = @va_zone_origine_in_1
		
		if @vl_sens = 1
		begin	
			select @vl_PR1 = ZDP.PR_fin
			from	CFG..EQT_GEN GEN,
				CFG..EQT_ZDP ZDP
			where	GEN.type	= XDC_EQT_ZDP
		  	and  	GEN.numero	= @va_zone_origine_in_1
			and 	GEN.numero	= ZDP.numero

			select @vl_PR2 = ZDP.PR_fin
			from    CFG..EQT_GEN GEN,
				CFG..EQT_ZDP ZDP
			where   GEN.type        = XDC_EQT_ZDP
			and     GEN.numero      = @va_zone_origine_in_2
			and	GEN.numero	= ZDP.numero
		end

		if @vl_sens = 2
		begin	
			select @vl_PR2 = ZDP.PR_fin
			from    CFG..EQT_GEN GEN,
				CFG..EQT_ZDP ZDP
			where   GEN.type        = XDC_EQT_ZDP
			and     GEN.numero      = @va_zone_origine_in_1
			and	GEN.numero	= ZDP.numero

			select @vl_PR1 = ZDP.PR_fin
			from    CFG..EQT_GEN GEN,
				CFG..EQT_ZDP ZDP
			where   GEN.type        = XDC_EQT_ZDP
			and     GEN.numero      = @va_zone_origine_in_2
			and	GEN.numero	= ZDP.numero
		end

		select distinct
			ZDP.numero, ZDP.libelle
		from    CFG..EQT_GEN GEN,
			CFG..EQT_ZDP ZDP
		where GEN.type         = XDC_EQT_ZDP
		and GEN.numero       = ZDP.numero
		and ZDP.inhibition   = 0
		and GEN.sens         = @vl_sens
		and ZDP.PR_fin       >= @vl_PR1
		and ZDP.PR_fin       <= @vl_PR2
		and ZDP.code_zone         != 0
		and GEN.autoroute    = @va_autoroute_in
		and ZDP.inhibition   = 0
		order by GEN.sens, GEN.autoroute, ZDP.PR_fin asc, ZDP.numero

    end
   return XDC_OK
         
go  
