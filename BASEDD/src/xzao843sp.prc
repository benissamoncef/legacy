/*E*/
/* Fichier : $Id: xzao843sp.prc,v 1.1 2017/03/31 17:46:37 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2017/03/31 17:46:37 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao843sp.prc
------------------------------------------------------
* HISTORIQUE :
*
* JMG   20/03/17        creation DEM1191
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao843sp.h" */

/* static char *version = " @(#)xzao843sp.prc	1.2 06/03/02: xzao843" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  recherche tous les PMVA dans la perturbation
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO843' and type = 'P')
	drop procedure XZAO843
go


create procedure XZAO843
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

   declare @vl_distance_evt int,
	   @vl_distance_equip int,
	   @vl_numero_zone smallint,
		@vl_numero_pmv smallint,
		@vl_autoroute_pmv tinyint,
		@vl_ordre_pr smallint,
		@vl_sens_pmv tinyint,
		@vl_PR_pmv int,
		@vl_direction_zone char(15),
		@vl_site_pmv tinyint,
		@vl_doublon tinyint,
		@vl_presence_bifurc int,
		@vl_PR_min int, @vl_PR_max int,
		@vl_HS bit , @vl_inhibe bit , @vl_desactive bit , @vl_majeure bit , @vl_critique bit

	/*
	** Verifier la validite des parametres d'entree
	*/

	select @vl_ordre_pr = inversee
	from CFG..RES_AUT_INV
	where numero = @va_Autoroute_in

	if @@rowcount = 0
		return XDC_NOK


   create table #TMP_PMA
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
   create unique index pmv_index on #TMP_PMA(numero_zone)

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
		doublon,
		distance
	from #TMP_PMA
	for update of distance

	/*recuperation des zones qui contiennent l'evt*/
	insert #TMP_PMA
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
	and  autoroute_pmva = @va_Autoroute_in
	/*and  sens_pmva = @va_Sens_in*/
	and type_zone = 1
	and  autoroute_zone = @va_Autoroute_in
	and sens_zone = @va_Sens_in
        and  (
               /* la zone de parcours doit avoir un pied dans la perturbation */
	       (PR_debut_zone between @vl_PR_min and @vl_PR_max) or
	       (PR_fin_zone between @vl_PR_min and @vl_PR_max  ) or
	       (PR_debut_zone <= @vl_PR_min and PR_fin_zone >= @vl_PR_max) or
	       (PR_debut_zone >= @vl_PR_min and PR_fin_zone <= @vl_PR_max )

	       /* le pmv doit etre dans la perturbation */
	       and (PR_pmva between @vl_PR_min and @vl_PR_max) 
	     )

	open pmv

	fetch pmv into @vl_numero_zone,
		@vl_numero_pmv,
		@vl_autoroute_pmv,
		@vl_sens_pmv,
		@vl_PR_pmv,
		@vl_site_pmv,
		@vl_direction_zone,
		@vl_doublon,
		@vl_distance_equip

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
			  where type_saga = XDC_SAGA_PMVA)


          if ( @vl_HS = XDC_VRAI or @vl_majeure=XDC_VRAI or @vl_critique=XDC_VRAI or
               @vl_inhibe=XDC_VRAI or @vl_desactive=XDC_VRAI )
            update #TMP_PMA set doublon = 2  where numero_pmv = @vl_numero_pmv
          else
          begin

            /*calcul de la distance entre le PMV et l'evt*/
	    /* le pmv etant forcemment sur la meme autoroute, le calcul est simple */
	    select @vl_presence_bifurc = 0, @vl_distance_evt=0
	    if ( @va_Sens_in = XDC_SENS_SUD and @vl_ordre_pr = XDC_PR_NOR  )
	       select @vl_distance_evt =  @vl_PR_max - @vl_PR_pmv
            else if ( @va_Sens_in =  XDC_SENS_NORD and @vl_ordre_pr = XDC_PR_NOR )
	       select @vl_distance_evt =  @vl_PR_pmv - @vl_PR_min
            else if (  @va_Sens_in = XDC_SENS_SUD and @vl_ordre_pr = XDC_PR_INV )  
	       select @vl_distance_evt = @vl_PR_pmv - @vl_PR_min 
            else if (  @va_Sens_in =  XDC_SENS_NORD and @vl_ordre_pr = XDC_PR_INV )
	       select @vl_distance_evt = @vl_PR_max - @vl_PR_pmv 
	     /*met a jour la liste des PMV a retourner*/
	     update #TMP_PMA
	       set distance = @vl_distance_evt + @vl_distance_equip,
		    presence_bifurc = @vl_presence_bifurc
	     where numero_pmv = @vl_numero_pmv and numero_zone=@vl_numero_zone
          end
	  select @vl_distance_equip=0
          fetch pmv into @vl_numero_zone,
			@vl_numero_pmv,
			@vl_autoroute_pmv,
			@vl_sens_pmv,
			@vl_PR_pmv,
			@vl_site_pmv,
			@vl_direction_zone,
			@vl_doublon,
			@vl_distance_equip
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
  from #TMP_PMA
  where doublon = 0
  and PR_pmv >= @vl_PR_min
  and PR_pmv <= @vl_PR_max
  and ( (distance <= @va_Dmax_in and presence_bifurc = 0)
      or
        (distance <= @va_Dmax_bifurc_in and presence_bifurc = 1)
      )
   return XDC_OK
go 
	 
