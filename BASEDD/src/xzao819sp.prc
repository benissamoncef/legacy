/*E*/
/*Fichier : @(#)xzao819sp.prc	1.2     Release : 1.2        Date :  06/03/02
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao819sp.prc
------------------------------------------------------
* HISTORIQUE :
*
* JMG   20/03/17        creation DEM1191
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao819sp.h" */

/* static char *version = " @(#)xzao819sp.prc	1.2 06/03/02: xzao819" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  recherche les premiers PMV en amont a un evenement
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

drop procedure XZAO819
go

create procedure XZAO819
   @va_Autoroute_in        tinyint = null,
   @va_Sens_in             tinyint = null,
   @va_PR_evt_in	   int = null,
   @va_PR_tete_evt_in	   int = null,
   @va_Dmax_in		   int = null,
   @va_Dmax_bifurc_in	   int = null,
   @va_pr_bifamont_in	   int = null,
   @va_pr_bifamont_tete_in    int = null,
   @va_aut_bifamont_in        tinyint = null,
   @va_aut_bifamont_tete_in   tinyint = null,
   @va_pr_debut_bifamont_in   int = null,
   @va_pr_debut_bifamont_tete_in int = null
   as 

   declare @vl_distance int,
	   @vl_numero_zone smallint,
		@vl_numero_pmv smallint,
		@vl_autoroute_pmv tinyint,
		@vl_sens_pmv tinyint,
		@vl_PR_pmv int,
		@vl_direction_zone char(15),
		@vl_cr int, @vl_site_pmv tinyint,
		@vl_doublon tinyint,
		@vl_autoroute_1_pmv_amont tinyint,
		@vl_sens_1_pmv_amont tinyint,
		@vl_PR_1_pmv_amont int,
		@vl_autoroute_1_pmv_amont_bis tinyint,
		@vl_sens_1_pmv_amont_bis tinyint,
		@vl_PR_1_pmv_amont_bis int,
		@vl_autoroute_1_pmv_amont_ter tinyint,
		@vl_sens_1_pmv_amont_ter tinyint,
		@vl_PR_1_pmv_amont_ter int,
		@vl_presence_bifurc int,
		@vl_PR_min int, @vl_PR_max int,
                @vl_HS bit , @vl_inhibe bit , @vl_desactive bit , @vl_majeure bit , @vl_critique bit  ,
		@vl_PR_debut_evt int , @vl_PR_fin_evt int, @vl_destination char(15)

   create table #TMP_PMV2
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
    PR_1_pmv_amont              int  null,
    sens_1_pmv_amont            tinyint    null ,
    autoroute_1_pmv_amont       tinyint      null,
    distance_1_pmv_amont        int null,
    PR_1_pmv_amont_bis          int    null,
    sens_1_pmv_amont_bis        tinyint  null,
    autoroute_1_pmv_amont_bis   tinyint   null,
    distance_1_pmv_amont_bis    int  null,
    PR_1_pmv_amont_ter          int      null,
    sens_1_pmv_amont_ter        tinyint   null,
    autoroute_1_pmv_amont_ter   tinyint  null,
    distance_1_pmv_amont_ter    int null,
    doublon 			tinyint null,
    distance 			int null,
    presence_bifurc		int null
   )
   create unique index pmv_index on #TMP_PMV2 (numero_zone)

   create table #TMP_PMV
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
    PR_1_pmv_amont              int  null,
    sens_1_pmv_amont            tinyint    null ,
    autoroute_1_pmv_amont       tinyint      null,
    distance_1_pmv_amont        int null,
    PR_1_pmv_amont_bis          int    null,
    sens_1_pmv_amont_bis        tinyint  null,
    autoroute_1_pmv_amont_bis   tinyint   null,
    distance_1_pmv_amont_bis    int  null,
    PR_1_pmv_amont_ter          int      null,
    sens_1_pmv_amont_ter        tinyint   null,
    autoroute_1_pmv_amont_ter   tinyint  null,
    distance_1_pmv_amont_ter    int null,
    doublon 			tinyint null,
    distance 			int null,
    presence_bifurc		int null
   )
   create unique index pmv_index on #TMP_PMV (numero_zone)

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


   declare pmv cursor
   for select 
		numero_zone,
                numero_pmv,
                autoroute_pmv,
                sens_pmv,
                PR_pmv,
		site_pmv,
		direction_zone,
		autoroute_1_pmv_amont,
		PR_1_pmv_amont,
		sens_1_pmv_amont,
		autoroute_1_pmv_amont_bis,
		PR_1_pmv_amont_bis,
		sens_1_pmv_amont_bis,
		autoroute_1_pmv_amont_ter,
		PR_1_pmv_amont_ter,
		sens_1_pmv_amont_ter,
		doublon,
		distance
	from #TMP_PMV where doublon=0
   for update of doublon, distance

	/*recuperation des zones qui contiennent l'evt*/
	insert #TMP_PMV
        select  numero_zone,
                        numero_pmv,
                        autoroute_pmv,
                        sens_pmv,
                        PR_pmv,
                        site_pmv,
                        autoroute_zone,
                        sens_zone,
                        PR_debut_zone,
                        PR_fin_zone,
                        upper(direction_zone),
			PR_1_pmv_amont,
    			sens_1_pmv_amont,
    			autoroute_1_pmv_amont ,
    			distance_1_pmv_amont,
    			PR_1_pmv_amont_bis ,
    			sens_1_pmv_amont_bis,
    			autoroute_1_pmv_amont_bis ,
    			distance_1_pmv_amont_bis ,
    			PR_1_pmv_amont_ter,
    			sens_1_pmv_amont_ter,
    			autoroute_1_pmv_amont_ter,
    			distance_1_pmv_amont_ter,
			0,
			0,
			0
	from CFG..ZON_PMV_SAGA
	where inhibition = 0
	and  autoroute_zone = @va_Autoroute_in
	and  sens_zone = @va_Sens_in
	and  (
	       (PR_debut_zone between @vl_PR_min and @vl_PR_max) or
	       (PR_fin_zone between @vl_PR_min and @vl_PR_max  ) or 
	       (PR_debut_zone <= @vl_PR_min and PR_fin_zone >= @vl_PR_max) or
               (PR_debut_zone >= @vl_PR_min and PR_fin_zone <= @vl_PR_max )
	     )
/**********************premiere passe : on vire les pmv dans la perturbation ou non operationels ***************************/

	open pmv

	fetch pmv into @vl_numero_zone,
		@vl_numero_pmv,
		@vl_autoroute_pmv,
		@vl_sens_pmv,
		@vl_PR_pmv,
		@vl_site_pmv,
		@vl_direction_zone,
		@vl_autoroute_1_pmv_amont,
		@vl_PR_1_pmv_amont,
		@vl_sens_1_pmv_amont,
		@vl_autoroute_1_pmv_amont_bis,
		@vl_PR_1_pmv_amont_bis,
		@vl_sens_1_pmv_amont_bis,
		@vl_autoroute_1_pmv_amont_ter,
		@vl_PR_1_pmv_amont_ter,
		@vl_sens_1_pmv_amont_ter,
		@vl_doublon,
		@vl_distance

	while (@@sqlstatus = 0)
        begin
	  /*supprimer les pmv dans la perturbation */
	  if @vl_autoroute_pmv=@va_Autoroute_in and @vl_sens_pmv=@va_Sens_in and @vl_PR_pmv between @vl_PR_min and @vl_PR_max
	  begin
            update #TMP_PMV set doublon=3 where numero_pmv = @vl_numero_pmv  
          end
          else
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
	    where type_saga = XDC_SAGA_PMV_1_18 or
	    type_saga= XDC_SAGA_PMV_2_18 or
	    type_saga = XDC_SAGA_PMV_2_15 or
	    type_saga = XDC_SAGA_PMV_3_15 or
	    type_saga = XDC_SAGA_PMV_2_18_P or
	    type_saga = XDC_SAGA_PMV_3_15_P)
   	    
            if ( @vl_HS = 1 or @vl_inhibe=1 or @vl_desactive=1 or @vl_majeure=1 or @vl_critique=1 )
	    begin
	      update #TMP_PMV set doublon = 2  where numero_pmv = @vl_numero_pmv
            end
          end
	  fetch pmv into @vl_numero_zone,
		@vl_numero_pmv,
		@vl_autoroute_pmv,
		@vl_sens_pmv,
		@vl_PR_pmv,
		@vl_site_pmv,
		@vl_direction_zone,
		@vl_autoroute_1_pmv_amont,
		@vl_PR_1_pmv_amont,
		@vl_sens_1_pmv_amont,
		@vl_autoroute_1_pmv_amont_bis,
		@vl_PR_1_pmv_amont_bis,
		@vl_sens_1_pmv_amont_bis,
		@vl_autoroute_1_pmv_amont_ter,
		@vl_PR_1_pmv_amont_ter,
		@vl_sens_1_pmv_amont_ter,
		@vl_doublon,
		@vl_distance
        end

	close pmv

/********************* deuxieme passe : on calcule les distances pour les zones restantes        ***************************/

	open pmv

	fetch pmv into @vl_numero_zone,
		@vl_numero_pmv,
		@vl_autoroute_pmv,
		@vl_sens_pmv,
		@vl_PR_pmv,
		@vl_site_pmv,
		@vl_direction_zone,
		@vl_autoroute_1_pmv_amont,
		@vl_PR_1_pmv_amont,
		@vl_sens_1_pmv_amont,
		@vl_autoroute_1_pmv_amont_bis,
		@vl_PR_1_pmv_amont_bis,
		@vl_sens_1_pmv_amont_bis,
		@vl_autoroute_1_pmv_amont_ter,
		@vl_PR_1_pmv_amont_ter,
		@vl_sens_1_pmv_amont_ter,
		@vl_doublon,
		@vl_distance
	
	while (@@sqlstatus = 0)
	begin

	  /*calcul de la distance entre le PMV et l'evt*/
	  if (  @va_Sens_in = 1 and @va_Autoroute_in != 8 or @va_Sens_in = 2 and @va_Autoroute_in = 8 )
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
          else if ( @va_Sens_in = 2 and @va_Autoroute_in != 8  or  @va_Sens_in = 1 and @va_Autoroute_in = 8 )
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
          exec @vl_cr = XZAO423 @vl_autoroute_pmv   ,
		   		@vl_PR_pmv          ,
				@vl_sens_pmv,
		      		@va_Autoroute_in   ,
			    	@vl_PR_debut_evt ,
			 	@va_Sens_in    ,
			    	@vl_PR_fin_evt ,
			        @vl_distance output,
			        @vl_presence_bifurc output,
				@vl_destination output
/*            select @vl_distance,@vl_presence_bifurc,@vl_destination*/

	    /*met a jour la liste des PMV a retourner*/
	    update #TMP_PMV
	    set distance = @vl_distance,
    	    presence_bifurc = @vl_presence_bifurc
	    where numero_pmv = @vl_numero_pmv

          fetch pmv into @vl_numero_zone,
	  @vl_numero_pmv,
	  @vl_autoroute_pmv,
	  @vl_sens_pmv,
	  @vl_PR_pmv,
	  @vl_site_pmv,
	  @vl_direction_zone,
	  @vl_autoroute_1_pmv_amont,
	  @vl_PR_1_pmv_amont,
	  @vl_sens_1_pmv_amont,
	  @vl_autoroute_1_pmv_amont_bis,
	  @vl_PR_1_pmv_amont_bis,
	  @vl_sens_1_pmv_amont_bis,
	  @vl_autoroute_1_pmv_amont_ter,
	  @vl_PR_1_pmv_amont_ter,
	  @vl_sens_1_pmv_amont_ter,
	  @vl_doublon,
	  @vl_distance
        end

	close pmv
/**********************Troisieme passe : on vire les pmv "chainer"  ***************************/
	insert #TMP_PMV2
        select   * from #TMP_PMV where doublon=0
	order by  distance desc

   declare pmv2 cursor
   for select 
		numero_zone,
                numero_pmv,
                autoroute_pmv,
                sens_pmv,
                PR_pmv,
		site_pmv,
		direction_zone,
		autoroute_1_pmv_amont,
		PR_1_pmv_amont,
		sens_1_pmv_amont,
		autoroute_1_pmv_amont_bis,
		PR_1_pmv_amont_bis,
		sens_1_pmv_amont_bis,
		autoroute_1_pmv_amont_ter,
		PR_1_pmv_amont_ter,
		sens_1_pmv_amont_ter,
		doublon,
		distance
	from #TMP_PMV2 where doublon=0

   for update of doublon, distance

	open pmv2

	fetch pmv2 into @vl_numero_zone,
		@vl_numero_pmv,
		@vl_autoroute_pmv,
		@vl_sens_pmv,
		@vl_PR_pmv,
		@vl_site_pmv,
		@vl_direction_zone,
		@vl_autoroute_1_pmv_amont,
		@vl_PR_1_pmv_amont,
		@vl_sens_1_pmv_amont,
		@vl_autoroute_1_pmv_amont_bis,
		@vl_PR_1_pmv_amont_bis,
		@vl_sens_1_pmv_amont_bis,
		@vl_autoroute_1_pmv_amont_ter,
		@vl_PR_1_pmv_amont_ter,
		@vl_sens_1_pmv_amont_ter,
		@vl_doublon,
		@vl_distance

	while (@@sqlstatus = 0)
        begin
	      /*les pmv dont le pr est mentionne dans les pmv amont au pmv traites par le curseur sont ecartes*/
	      /*on ecarte egalement les zones du meme pmv pour d'autres destinations (exemple pmv25600 pour evt*/
	      /*  sur A52 du 19000 au 21000 sens 1                                                             */
	      update #TMP_PMV set doublon=1 
	      where ( PR_pmv in (@vl_PR_1_pmv_amont,@vl_PR_1_pmv_amont_bis,@vl_autoroute_1_pmv_amont_ter) or
		      PR_pmv = @vl_PR_pmv and  direction_zone != @vl_direction_zone)
                        and doublon=0

	  fetch pmv2 into @vl_numero_zone,
		@vl_numero_pmv,
		@vl_autoroute_pmv,
		@vl_sens_pmv,
		@vl_PR_pmv,
		@vl_site_pmv,
		@vl_direction_zone,
		@vl_autoroute_1_pmv_amont,
		@vl_PR_1_pmv_amont,
		@vl_sens_1_pmv_amont,
		@vl_autoroute_1_pmv_amont_bis,
		@vl_PR_1_pmv_amont_bis,
		@vl_sens_1_pmv_amont_bis,
		@vl_autoroute_1_pmv_amont_ter,
		@vl_PR_1_pmv_amont_ter,
		@vl_sens_1_pmv_amont_ter,
		@vl_doublon,
		@vl_distance
        end

	close pmv2

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
  from #TMP_PMV
 where  doublon = 0
  and ( (distance <= @va_Dmax_in and presence_bifurc = 0)
      or
        (distance <= @va_Dmax_bifurc_in and presence_bifurc = 1)
      )
   and distance >=0
  group by autoroute_pmv,sens_pmv
  having 
    doublon = 0
    and ( (distance <= @va_Dmax_in and presence_bifurc = 0)
	  or
          (distance <= @va_Dmax_bifurc_in and presence_bifurc = 1)
        )
   and distance >= 0
     and distance = min(distance)

/* test */
/* select distinct
	  numero_pmv,
	  site_pmv,
	  autoroute_pmv,
	  PR_pmv,
	  distance,
	  sens_pmv,
	  direction_zone,
	  presence_bifurc,
	  doublon
  from #TMP_PMV
*/
   drop table #TMP_PMV

   return XDC_OK
go 

	 
