/*E*/
/*Fichier : $Id: xzao825sp.prc,v 1.1 2017/03/31 17:48:14 pc2dpdy Exp $     Release : $Revision: 1.1 $        Date :  $Date: 2017/03/31 17:48:14 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao825sp.prc
------------------------------------------------------
* HISTORIQUE :
*
* JMG   20/03/17        creation DEM1191
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao825sp.h" */

/* static char *version = " $Id: xzao825sp.prc,v 1.1 2017/03/31 17:48:14 pc2dpdy Exp $: xzao825" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  recherche tous les PMV dans la perturbation
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO825' and type = 'P')
	drop procedure XZAO825
go

create procedure XZAO825
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
		@vl_HS bit , @vl_inhibe bit , @vl_desactive bit , @vl_majeure bit , @vl_critique bit


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
   create unique index pmv_index on #TMP_PMV(numero_zone)

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
	from #TMP_PMV
	for update of distance

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
	and  autoroute_pmv = @va_Autoroute_in
	and  sens_pmv = @va_Sens_in
	and  autoroute_zone = @va_Autoroute_in
	and sens_zone = @va_Sens_in
        and  (
               /* la zone de parcours doit avoir un pied dans la perturbation */
	       (PR_debut_zone between @vl_PR_min and @vl_PR_max) or
	       (PR_fin_zone between @vl_PR_min and @vl_PR_max  ) or
	       (PR_debut_zone <= @vl_PR_min and PR_fin_zone >= @vl_PR_max) or
	       (PR_debut_zone >= @vl_PR_min and PR_fin_zone <= @vl_PR_max )

	       /* le pmv doit etre dans la perturbation */
	       and (PR_pmv between @vl_PR_min and @vl_PR_max) 
	     )

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
            update #TMP_PMV set doublon = 2  where numero_pmv = @vl_numero_pmv
          else
          begin

            /*calcul de la distance entre le PMV et l'evt*/
	    /* le pmv etant forcemment sur la meme autoroute, le calcul est simple */
	    /* sauf sur A50 a cause du trou des PR */
	    select @vl_presence_bifurc = 0
	    if (@va_Autoroute_in = 8)
	    begin
              if (@va_Sens_in =  1) select @vl_distance = @vl_PR_pmv - @vl_PR_min
	      else if (@va_Sens_in =  2) select @vl_distance = @vl_PR_max - @vl_PR_pmv
	    end
	    /* verrue pour A50 */
	    else if (@va_Autoroute_in = 3)
	    begin
              if (@va_Sens_in =  2 and @vl_PR_pmv > 25847 and  @vl_PR_min < 25847 ) select  @vl_distance =  @vl_PR_pmv - @vl_PR_min - (25847 - 16698)
	      else if (@va_Sens_in =  1) select  @vl_distance =  @vl_PR_max - @vl_PR_pmv
	      else select  @vl_distance =  @vl_PR_pmv - @vl_PR_min
	    end
	    /* fin de verrue pour A50*/
	    else
	    begin
	      if (@va_Sens_in =  1) select @vl_distance =  @vl_PR_max - @vl_PR_pmv
	      else if (@va_Sens_in =  2) select @vl_distance =  @vl_PR_pmv - @vl_PR_min
	    end

	     /*met a jour la liste des PMV a retourner*/
	     update #TMP_PMV
	       set distance = @vl_distance,
		    presence_bifurc = @vl_presence_bifurc
	     where numero_pmv = @vl_numero_pmv
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

   select numero_pmv,
	  site_pmv,
	  autoroute_pmv,
	  PR_pmv,
	  distance,
	  sens_pmv,
	  direction_zone,
	  presence_bifurc
  from #TMP_PMV
  where doublon = 0
  and PR_pmv >= @vl_PR_min
  and PR_pmv <= @vl_PR_max
  and ( (distance <= @va_Dmax_in and presence_bifurc = 0)
      or
        (distance <= @va_Dmax_bifurc_in and presence_bifurc = 1)
      )
   return XDC_OK
go 
	 
