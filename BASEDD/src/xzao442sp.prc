/*E*/
/*Fichier : Fichier : $Id: xzao442sp.prc,v 1.1 2007/03/26 15:48:51 gesconf Exp $     Release : $Revision: 1.1 $        Date : $Date: 2007/03/26 15:48:51 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao442sp.prc
------------------------------------------------------
* HISTORIQUE :
*
* guilhou       21/06/05      : creation  1.1
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao442sp.h" */

/* static char *version = " $Id: xzao442sp.prc,v 1.1 2007/03/26 15:48:51 gesconf Exp $ $Revision: 1.1 $ $Date: 2007/03/26 15:48:51 $: xzao442" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  recherche les BAD en amont a un evenement
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO442' and type = 'P')
	drop procedure XZAO442
go

create procedure XZAO442
   @va_Autoroute_in        tinyint = null,
   @va_Sens_in             tinyint = null,
   @va_PR_evt_in	   int = null,
   @va_PR_tete_evt_in	   int = null
   as 

   declare @vl_distance int,
	   @vl_numero_zone smallint,
		@vl_numero_bad smallint,
		@vl_autoroute_bad tinyint,
		@vl_autoroute_zone tinyint,
		@vl_sens_bad tinyint,
		@vl_PR_bad int,
		@vl_cr int, @vl_site_bad tinyint,
		@vl_doublon tinyint,
		@vl_autoroute_1_bad_amont tinyint,
		@vl_sens_1_bad_amont tinyint,
		@vl_PR_1_bad_amont int,
		@vl_autoroute_1_bad_amont_bis tinyint,
		@vl_sens_1_bad_amont_bis tinyint,
		@vl_PR_1_bad_amont_bis int,
		@vl_autoroute_1_bad_amont_ter tinyint,
		@vl_sens_1_bad_amont_ter tinyint,
		@vl_PR_1_bad_amont_ter int,
		@vl_presence_bifurc int,
		@vl_PR_min int, @vl_PR_max int,
                @vl_HS bit , @vl_inhibe bit , @vl_desactive bit , @vl_majeure bit , @vl_critique bit  ,
		@vl_PR_debut_evt int , @vl_PR_fin_evt int, @vl_destination char(15)

   create table #TMP_BAD
   (   
    numero_zone                 smallint null,
    numero_bad                  smallint null,
    autoroute_bad               tinyint null,
    sens_bad                    tinyint null,
    PR_bad                      int    null,
    site_bad                    tinyint  null,
    autoroute_zone              tinyint null,
    sens_zone                   tinyint null,
    PR_debut_zone               int    null,
    PR_fin_zone                 int   null,
    doublon 			tinyint null,
    distance 			int null
   )
   create unique index bad_index on #TMP_BAD (numero_zone)

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


   declare bad cursor
   for select 
		numero_zone,
		autoroute_zone,
                numero_bad,
                autoroute_bad,
                sens_bad,
                PR_bad,
		site_bad,
		doublon,
		distance
	from #TMP_BAD where doublon=0
   for update of doublon, distance

	/*recuperation de toutes les zones */
	insert #TMP_BAD
        select  numero_zone,
                        numero_bad,
                        autoroute_bad,
                        sens_bad,
                        PR_bad,
                        site_bad,
                        autoroute_zone,
                        sens_zone,
                        PR_debut_zone,
                        PR_fin_zone,
			0,
			0
	from CFG..ZON_BAD
	where inhibition = 0
	and autoroute_zone = @va_Autoroute_in
	and sens_zone = @va_Sens_in
	and PR_debut_zone <= @va_PR_evt_in
	and PR_fin_zone >=  @va_PR_evt_in
	order by numero_bad

/**********************premiere passe : on vire les bad dans la perturbation ou non operationels ***************************/

	open bad

	fetch bad into @vl_numero_zone,
		@vl_autoroute_zone,
		@vl_numero_bad,
		@vl_autoroute_bad,
		@vl_sens_bad,
		@vl_PR_bad,
		@vl_site_bad,
		@vl_doublon,
		@vl_distance

	while (@@sqlstatus = 0)
        begin
	  /* si doublon != 0 on ne traite pas */
	  if @vl_doublon = 0
	  begin
	    /*supprimer les bad dans la perturbation */
	    if ( @vl_autoroute_bad=@va_Autoroute_in and @vl_sens_bad=@va_Sens_in and @vl_PR_bad between @vl_PR_min and @vl_PR_max)
	    begin
              update #TMP_BAD set doublon=3 where numero_bad = @vl_numero_bad  
            end
            else
	    begin
	      /* calcul de la distance entre le bad et l'evt. Si cette distance != 99999 alors le bad est en amont */
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
              exec @vl_cr = XZAO423 @vl_autoroute_bad   ,
		   		@vl_PR_bad          ,
				@vl_sens_bad,
		      		@va_Autoroute_in   ,
			    	@vl_PR_debut_evt ,
			 	@va_Sens_in    ,
			    	@vl_PR_fin_evt ,
			        @vl_distance output,
				@vl_presence_bifurc output,
				@vl_destination output

              if @vl_distance = 99999 or @vl_distance <= 0
	      begin
                update #TMP_BAD set doublon=0 where numero_bad = @vl_numero_bad  
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
	        where type=XDC_EQT_BAD and dernier=1 and equipement=@vl_numero_bad
                if ( @vl_HS = 1 or @vl_inhibe=1 or @vl_desactive=1 or @vl_majeure=1 or @vl_critique=1 )
		begin
	          update #TMP_BAD set doublon = 2  where numero_bad = @vl_numero_bad
		end
		else
		begin
		  /* mise a jour des donnees */
	          update #TMP_BAD
	          set distance = @vl_distance
	          where numero_bad = @vl_numero_bad
                end
              end
            end
          end
	  fetch bad into @vl_numero_zone,
		@vl_autoroute_zone,
		@vl_numero_bad,
		@vl_autoroute_bad,
		@vl_sens_bad,
		@vl_PR_bad,
		@vl_site_bad,
		@vl_doublon,
		@vl_distance
        end

	close bad

/*********************on retourne les resultats*****************************************************************************/

   select distinct 
	  numero_bad,
	  site_bad,
	  autoroute_bad,
	  PR_bad,
	  distance,
	  sens_bad
  from #TMP_BAD
  where  doublon = 0 
      order by autoroute_bad,sens_bad,PR_bad

   drop table #TMP_BAD

   return XDC_OK
go 
	 
