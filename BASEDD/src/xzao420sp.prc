/*E*/
/*Fichier : $Id: xzao420sp.prc,v 1.2 2002/06/03 15:58:53 gesconf Exp $     Release : $Revision: 1.2 $        Date :  $Date: 2002/06/03 15:58:53 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao420sp.prc
------------------------------------------------------
* HISTORIQUE :
*
* guilhou       20/09/00        : creation vide 1.1
* cluchague     10/12/01        : modif pour tenir compte a57 et dispo equipements 1.2
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao420sp.h" */

/* static char *version = " $Id: xzao420sp.prc,v 1.2 2002/06/03 15:58:53 gesconf Exp $: xzao420" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
* recherche des PMV amont d'un PMV (avec
* leurs zones d'influence) et qui incluent
* la bonne zone d'influence
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO420' and type = 'P')
	drop procedure XZAO420
go

create procedure XZAO420
   @va_Autoroute_PMV_in    tinyint = null,
   @va_PR_PMV_in           int = null,
   @va_Sens_PMV_in         tinyint = null,
   @va_Dmax_in		   int = null,
   @va_Autoroute_Zone_in   tinyint = null,
   @va_PR_min_in           int = null,
   @va_PR_max_in           int = null,
   @va_Sens_Zone_in        tinyint = null
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
		@vl_distance_1_pmv_amont int,
		@vl_distance_1_pmv_amont_bis int,
		@vl_distance_1_pmv_amont_ter int,
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
    distance 			int null
   )

   /*recupere les premiers pmv amont*/
   select @vl_autoroute_1_pmv_amont = autoroute_1_pmv_amont,
	  @vl_sens_1_pmv_amont = sens_1_pmv_amont,
	  @vl_PR_1_pmv_amont = PR_1_pmv_amont,
	  @vl_distance_1_pmv_amont = distance_1_pmv_amont,
	  @vl_autoroute_1_pmv_amont_bis = autoroute_1_pmv_amont_bis,
	  @vl_sens_1_pmv_amont_bis = sens_1_pmv_amont_bis,
	  @vl_PR_1_pmv_amont_bis = PR_1_pmv_amont_bis,
	  @vl_distance_1_pmv_amont_bis = distance_1_pmv_amont_bis,
	  @vl_autoroute_1_pmv_amont_ter = autoroute_1_pmv_amont_ter,
	  @vl_sens_1_pmv_amont_ter = sens_1_pmv_amont_ter,
	  @vl_PR_1_pmv_amont_ter = PR_1_pmv_amont_ter,
	  @vl_distance_1_pmv_amont_ter = distance_1_pmv_amont_ter
   from CFG..ZON_PMV
   where autoroute_pmv = @va_Autoroute_PMV_in
   and sens_pmv = @va_Sens_PMV_in
   and PR_pmv = @va_PR_PMV_in

   if @vl_autoroute_1_pmv_amont > 0  
      and @vl_distance_1_pmv_amont <= @va_Dmax_in
   begin
	/*recuperation de la zone eventuelle qui contient l'evt*/
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
			@vl_distance_1_pmv_amont
	from CFG..ZON_PMV
	where inhibition = 0
	and autoroute_pmv = @vl_autoroute_1_pmv_amont
	and sens_pmv = @vl_sens_1_pmv_amont
	and PR_pmv = @vl_PR_1_pmv_amont
	and  autoroute_zone = @va_Autoroute_Zone_in
	and  sens_zone = @va_Sens_Zone_in
	and  (
                /* la zone de parcours doit avoir un pied dans la perturbation */
	        (PR_debut_zone between @va_PR_min_in and @va_PR_max_in) or
	        (PR_fin_zone between @va_PR_min_in and @va_PR_max_in  ) or
	        (PR_debut_zone <= @va_PR_min_in and PR_fin_zone >= @va_PR_max_in) or
	        (PR_debut_zone >= @va_PR_min_in and PR_fin_zone <= @va_PR_max_in )
             )
   end

   if @vl_autoroute_1_pmv_amont_bis > 0  
      and @vl_distance_1_pmv_amont_bis <= @va_Dmax_in
   begin
	/*recuperation de la zone eventuelle qui contient l'evt*/
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
			@vl_distance_1_pmv_amont_bis
	from CFG..ZON_PMV
	where inhibition = 0
	and autoroute_pmv = @vl_autoroute_1_pmv_amont_bis
	and sens_pmv = @vl_sens_1_pmv_amont_bis
	and PR_pmv = @vl_PR_1_pmv_amont_bis
	and  autoroute_zone = @va_Autoroute_Zone_in
	and  sens_zone = @va_Sens_Zone_in
	and  (
                /* la zone de parcours doit avoir un pied dans la perturbation */
	        (PR_debut_zone between @va_PR_min_in and @va_PR_max_in) or
	        (PR_fin_zone between @va_PR_min_in and @va_PR_max_in  ) or
	        (PR_debut_zone <= @va_PR_min_in and PR_fin_zone >= @va_PR_max_in) or
	        (PR_debut_zone >= @va_PR_min_in and PR_fin_zone <= @va_PR_max_in )
             )
   end

   if @vl_autoroute_1_pmv_amont_ter > 0  
      and @vl_distance_1_pmv_amont_ter <= @va_Dmax_in
   begin
	/*recuperation de la zone eventuelle qui contient l'evt*/
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
			@vl_distance_1_pmv_amont_ter
	from CFG..ZON_PMV
	where inhibition = 0
	and autoroute_pmv = @vl_autoroute_1_pmv_amont_ter
	and sens_pmv = @vl_sens_1_pmv_amont_ter
	and PR_pmv = @vl_PR_1_pmv_amont_ter
	and  autoroute_zone = @va_Autoroute_Zone_in
	and  sens_zone = @va_Sens_Zone_in
	and  (
                /* la zone de parcours doit avoir un pied dans la perturbation */
	        (PR_debut_zone between @va_PR_min_in and @va_PR_max_in) or
	        (PR_fin_zone between @va_PR_min_in and @va_PR_max_in  ) or
	        (PR_debut_zone <= @va_PR_min_in and PR_fin_zone >= @va_PR_max_in) or
	        (PR_debut_zone >= @va_PR_min_in and PR_fin_zone <= @va_PR_max_in )
             )
   end

/* une fois les zones trouvees , on regarde si les pmv sont disponibles */
declare pmv cursor
for select numero_zone, numero_pmv from #TMP_PMV

open pmv

fetch pmv into @vl_numero_zone, @vl_numero_pmv

while (@@sqlstatus = 0)
begin
  /* supprimer les pmv non operationels */
  select @vl_HS=HS,@vl_inhibe=inhibe,@vl_desactive=desactive,@vl_majeure=majeure,@vl_critique=critique
  from EXP..EQT_DSP
  where type=XDC_EQT_PMV and dernier=1 and equipement=@vl_numero_pmv

  if ( @vl_HS = 1 or @vl_inhibe=1 or @vl_desactive=1 or @vl_majeure=1 or @vl_critique=1 )
    delete #TMP_PMV where numero_zone = @vl_numero_zone and numero_pmv = @vl_numero_pmv

  fetch pmv into @vl_numero_zone, @vl_numero_pmv

end

close pmv

   select * from #TMP_PMV

   return XDC_OK
go 
	 
