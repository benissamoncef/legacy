/*E*/
/*Fichier : Fichier : $Id: xzao840sp.prc,v 1.1 2017/03/31 17:45:28 pc2dpdy Exp $     Release : $Revision: 1.1 $        Date : $Date: 2017/03/31 17:45:28 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao840sp.prc
------------------------------------------------------
* HISTORIQUE :
*
* JMG   20/03/17        creation DEM1191
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao840sp.h" */

/* static char *version = " $Id: xzao840sp.prc,v 1.1 2017/03/31 17:45:28 pc2dpdy Exp $ $Revision: 1.1 $ $Date: 2017/03/31 17:45:28 $: xzao840" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  recherche les PMVA en amont a un evenement
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO840' and type = 'P')
	drop procedure XZAO840
go


create procedure XZAO840
   @va_Autoroute_in        tinyint = null,
   @va_Sens_in             tinyint = null,
   @va_PR_evt_in	   int = null,
   @va_PR_tete_evt_in	   int = null,
   @va_type_zone_in	   tinyint = null
   as 

   declare @vl_distance_evt int,
	   @vl_distance_equip int,
	   @vl_numero_zone smallint,
		@vl_numero_pmv smallint,
		@vl_autoroute_pmv tinyint,
		@vl_autoroute_zone tinyint,
		@vl_sens_pmv tinyint,
		@vl_sens_pmv_revu tinyint,
		@vl_sens_evt_revu tinyint,
		@vl_PR_pmv int,
		@vl_direction_zone char(15),
		@vl_cr int, @vl_site_pmv tinyint,
		@vl_doublon tinyint,
		@vl_sens_tmp tinyint,
		@vl_ordre_pr smallint,
		@vl_presence_bifurc int,
		@vl_PR_min int, @vl_PR_max int,
                @vl_HS bit , @vl_inhibe bit , @vl_desactive bit , @vl_majeure bit , @vl_critique bit  ,
		@vl_PR_debut_evt int , @vl_PR_fin_evt int, @vl_destination char(15)

	/*
	** Verifier la validite des parametres d'entree
	*/

	select @vl_ordre_pr = inversee
	from CFG..RES_AUT_INV
	where numero = @va_Autoroute_in

	if @@rowcount = 0
		return XDC_NOK


   create table #TMP_PMVA
   (   
    numero_zone                 smallint null,
    numero_pmv                  smallint null,
    autoroute_pmv               tinyint null,
    sens_pmv                    tinyint null,
    PR_pmv                      int    null,
    site_pmv                    tinyint  null,
    autoroute_zone              tinyint null,
    sens_zone                   tinyint null,
    PR_debut_zone               int    null,
    PR_fin_zone                 int   null,
    direction_zone              char(15) null,
    doublon 			tinyint null,
    distance 			int null,
    presence_bifurc		int null
   )
   create unique index pmva_index on #TMP_PMVA (numero_zone)

   /* on ordonne les PR de debut et de fin d'evt en sens croissant */ 
   if @va_PR_evt_in < @va_PR_tete_evt_in
   begin
	select @vl_PR_min = @va_PR_evt_in
	select @vl_PR_max = @va_PR_tete_evt_in
   end
   else
   begin
	select @vl_PR_min = @va_PR_tete_evt_in
 	select @vl_PR_max = @va_PR_evt_in	
   end

	      /* ramener le sens de l'evenement aux cas de base Nord / Sud */
	      if @va_Sens_in=XDC_SENS_SORTIE_SUD or @va_Sens_in=XDC_SENS_ENTREE_SUD
	      begin
		select @vl_sens_evt_revu = XDC_SENS_SUD
	      end
	      else if @va_Sens_in=XDC_SENS_SORTIE_NORD or @va_Sens_in=XDC_SENS_ENTREE_NORD
	      begin
		select @vl_sens_evt_revu = XDC_SENS_NORD
	      end
	      else
	      begin
		select @vl_sens_evt_revu = @va_Sens_in
              end

	      /* ordonner les PR de debut et fin de l'evenement selon ce sens */
	      if ( (@vl_sens_evt_revu = XDC_SENS_SUD and @vl_ordre_pr = XDC_PR_NOR) or
	           (@vl_sens_evt_revu = XDC_SENS_NORD and @vl_ordre_pr = XDC_PR_INV) )
	      begin
                if @va_PR_evt_in < @va_PR_tete_evt_in 
	        begin 
	          select @vl_PR_debut_evt = @va_PR_evt_in,
		         @vl_PR_fin_evt   = @va_PR_tete_evt_in
                end
                else 
        	begin
    	          select @vl_PR_debut_evt = @va_PR_tete_evt_in,
		         @vl_PR_fin_evt   = @va_PR_evt_in
                end
	      end
              else if ( (@vl_sens_evt_revu = XDC_SENS_NORD and @vl_ordre_pr = XDC_PR_NOR) or
	                (@vl_sens_evt_revu = XDC_SENS_SUD and @vl_ordre_pr = XDC_PR_INV) )
	      begin
                if @va_PR_evt_in > @va_PR_tete_evt_in 
	        begin 
	          select @vl_PR_debut_evt = @va_PR_evt_in,
		         @vl_PR_fin_evt   = @va_PR_tete_evt_in
                end
                else 
	        begin
	          select @vl_PR_debut_evt = @va_PR_tete_evt_in,
		         @vl_PR_fin_evt   = @va_PR_evt_in
                end
	      end

   declare pmva cursor
   for select 
		numero_zone,
		autoroute_zone,
                numero_pmv,
                autoroute_pmv,
                sens_pmv,
                PR_pmv,
		site_pmv,
		direction_zone,
		doublon,
		distance
	from #TMP_PMVA where doublon=0
   for update of doublon, distance

	/*recuperation de toutes les zones */
	insert #TMP_PMVA
        select  numero_zone,
                        numero_pmva,
                        autoroute_pmva,
                        sens_zone,
                        PR_pmva,
                        site_pmva,
                        autoroute_zone,
                        sens_zone,
                        PR_debut_zone,
                        PR_fin_zone,
                        upper(direction_zone),
			0,
			distance,
			0
	from CFG..ZON_PMA_SAGA
	where inhibition = 0
	and type_zone = @va_type_zone_in
	and autoroute_zone =  @va_Autoroute_in
	and sens_zone = @va_Sens_in 
	and not (PR_fin_zone < @vl_PR_min or PR_debut_zone > @vl_PR_max)
	order by numero_pmva

/**********************premiere passe : on vire les pmva dans la perturbation ou non operationels ***************************/

	open pmva

	fetch pmva into @vl_numero_zone,
		@vl_autoroute_zone,
		@vl_numero_pmv,
		@vl_autoroute_pmv,
		@vl_sens_pmv,
		@vl_PR_pmv,
		@vl_site_pmv,
		@vl_direction_zone,
		@vl_doublon,
		@vl_distance_equip

	      if @vl_sens_pmv=XDC_SENS_SORTIE_SUD or @vl_sens_pmv=XDC_SENS_ENTREE_SUD
	      begin
		select @vl_sens_pmv_revu = XDC_SENS_SUD
	      end
	      else if @vl_sens_pmv=XDC_SENS_SORTIE_NORD or @vl_sens_pmv=XDC_SENS_ENTREE_NORD
	      begin
		select @vl_sens_pmv_revu = XDC_SENS_NORD
	      end
	      else
	      begin
		select @vl_sens_pmv_revu = @vl_sens_pmv
              end

	while (@@sqlstatus = 0)
        begin
	  /* si doublon != 0 on ne traite pas */
	  if @vl_doublon = 0
	  begin
	    if ( @vl_autoroute_pmv=@va_Autoroute_in and @vl_sens_pmv_revu=@va_Sens_in and @vl_PR_pmv between @vl_PR_min and @vl_PR_max)
	    begin
              update #TMP_PMVA set doublon=3 where numero_pmv = @vl_numero_pmv  
            end
            else
	    begin
	      /* calcul de la distance entre le pmv et l'evt. Si cette distance != 99999 alors le pmv est en amont */
              exec @vl_cr = XZAO423 @vl_autoroute_pmv   ,
		   		@vl_PR_pmv          ,
				@vl_sens_pmv_revu,
		      		@va_Autoroute_in   ,
			    	@vl_PR_debut_evt ,
			 	@vl_sens_evt_revu, /*@va_Sens_in    ,*/
			    	@vl_PR_fin_evt ,
			        @vl_distance_evt output,
				@vl_presence_bifurc output,
				@vl_destination output

	      update #TMP_PMVA
		set presence_bifurc = @vl_presence_bifurc
		where numero_pmv = @vl_numero_pmv

              if @vl_distance_evt = 99999 or @vl_distance_evt = 99998 /*or @vl_distance_evt <= 0*/
	      begin
		/*essayer la recherche dans l autre sens*/
		if @vl_sens_pmv_revu=XDC_SENS_SUD
		begin
			select @vl_sens_tmp=XDC_SENS_NORD
		end
		else
		begin	
			select @vl_sens_tmp=XDC_SENS_SUD
		end
		exec @vl_cr = XZAO423 @vl_autoroute_pmv   ,
				@vl_PR_pmv          ,
				@vl_sens_tmp,
				@va_Autoroute_in   ,
				@vl_PR_debut_evt ,
			 	@vl_sens_evt_revu, /*@va_Sens_in    ,*/
				@vl_PR_fin_evt ,
				@vl_distance_evt output,
				@vl_presence_bifurc output,
				@vl_destination output
		update #TMP_PMVA
		set presence_bifurc = @vl_presence_bifurc
		where numero_pmv = @vl_numero_pmv

		if @vl_distance_evt = 99999 /*or @vl_distance_evt <= 0*/
		begin
                	update #TMP_PMVA set doublon=4 where numero_pmv = @vl_numero_pmv  
		end
	      end
	      /*else*/

	      if @vl_distance_evt < 0
	      begin
		select @vl_distance_evt=0
	      end
	      if @vl_distance_evt != 99999 and @vl_distance_evt >= 0
	      begin
                /* supprimer les PMV non operationels */
	        select @vl_HS=HS,@vl_inhibe=inhibe,@vl_desactive=desactive,@vl_majeure=majeure,@vl_critique=critique
#ifdef HIST
                from HIS..EQT_DSP
#else
	        from EXP..EQT_DSP
#endif
	        where type=XDC_EQT_SAGA and dernier=1 and equipement=@vl_numero_pmv
		and equipement in (select numero from CFG..EQT_SAGA
		where type_saga = XDC_SAGA_PMVA)

                if ( @vl_HS = XDC_VRAI or @vl_majeure=XDC_VRAI or @vl_critique=XDC_VRAI or
                     @vl_inhibe=XDC_VRAI or @vl_desactive=XDC_VRAI )
		begin
	          update #TMP_PMVA set doublon = 2  where numero_pmv = @vl_numero_pmv
		end
		else
		begin
		  /* mise a jour des donnees */
	          update #TMP_PMVA
	          set distance = @vl_distance_evt + @vl_distance_equip
	          where numero_pmv = @vl_numero_pmv
                end
              end
            end
          end
	  fetch pmva into @vl_numero_zone,
		@vl_autoroute_zone,
		@vl_numero_pmv,
		@vl_autoroute_pmv,
		@vl_sens_pmv,
		@vl_PR_pmv,
		@vl_site_pmv,
		@vl_direction_zone,
		@vl_doublon,
		@vl_distance_equip
        end

	close pmva

/*********************on retourne les resultats*****************************************************************************/

   select distinct 
	  numero_pmv,
	  site_pmv,
	  autoroute_pmv,
	  PR_pmv,
	  distance,
	  sens_pmv,
	  direction_zone,
	  presence_bifurc
  from #TMP_PMVA
  where  doublon = 0 
      order by autoroute_pmv,sens_pmv,PR_pmv

   drop table #TMP_PMVA

   return XDC_OK
go 
	 
