/*E*/
/*Fichier : Fichier : $Id: xzao841sp.prc,v 1.1 2017/03/31 17:47:05 pc2dpdy Exp $     Release : $Revision: 1.1 $        Date : $Date: 2017/03/31 17:47:05 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao841sp.prc
------------------------------------------------------
* HISTORIQUE :
*
* guilhou       21/06/05      : creation  1.1
* JMG	26/11/16	filtre peage amont lot 23 1.2
* PNI 	11/09/23	: Prise en compte du flag RES_ECH.supprime=XDC_FAUX SAE-517
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao841sp.h" */

/* static char *version = " $Id: xzao841sp.prc,v 1.1 2017/03/31 17:47:05 pc2dpdy Exp $ $Revision: 1.1 $ $Date: 2017/03/31 17:47:05 $: xzao841" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  recherche les BAF en amont a un evenement
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

drop procedure XZAO841
go

create procedure XZAO841
   @va_Autoroute_in        tinyint = null,
   @va_Sens_in             tinyint = null,
   @va_PR_evt_in	   int = null,
   @va_PR_tete_evt_in	   int = null,
   @va_Filtre_peage_in	   int = null
   as 

   declare @vl_distance int,@vl_distance2 int,
	   @vl_numero_zone smallint,
		@vl_numero_baf smallint,
		@vl_autoroute_baf tinyint,
		@vl_autoroute_zone tinyint,
		@vl_sens_baf tinyint,
		@vl_PR_baf int,
		@vl_cr int, @vl_site_baf tinyint,
		@vl_doublon tinyint,
		@vl_autoroute_1_baf_amont tinyint,
		@vl_sens_1_baf_amont tinyint,
		@vl_PR_1_baf_amont int,
		@vl_autoroute_1_baf_amont_bis tinyint,
		@vl_sens_1_baf_amont_bis tinyint,
		@vl_PR_1_baf_amont_bis int,
		@vl_autoroute_1_baf_amont_ter tinyint,
		@vl_sens_1_baf_amont_ter tinyint,
		@vl_PR_1_baf_amont_ter int,
		@vl_presence_bifurc int,
		@vl_PR_min int, @vl_PR_max int,
                @vl_HS bit , @vl_inhibe bit , @vl_desactive bit , @vl_majeure bit , @vl_critique bit  ,
		@vl_PR_debut_evt int , @vl_PR_fin_evt int, @vl_destination char(15)

   create table #TMP_PEA
   (
   numero	smallint null,
   nom		char(50) null,
   autoroute	tinyint null,
   PR		int null,
   sens		tinyint null,
   distance 	int null,
   doublon	tinyint null)

   create unique index pea_index on #TMP_PEA (numero, sens)

   create table #TMP_PEA2
   (
   numero	smallint null,
   nom		char(50) null,
   autoroute	tinyint null,
   PR		int null,
   sens		tinyint null,
   distance 	int null,
   doublon	tinyint null)

   create unique index pea2_index on #TMP_PEA2 (numero, sens)

   create table #TMP_ECH
   (
   numero 	smallint null,
   nom		char(50) null,
   autoroute	tinyint null,
   PR		int null,
   sens		tinyint null,
   distance	int null,
   doublon	tinyint null)


   create unique index ech_index on #TMP_ECH (numero, sens)


   	insert #TMP_PEA
   	select pea.numero,
		pea.nom,
		por.autoroute,
		pea.PRsens1,
		XDC_SENS_SUD,
		0,
		0
   	from CFG..RES_PEA pea, CFG..RES_POR por
	where pea.portion = por.numero
	and pea.PRsens1 !=0
	order by pea.numero

   	insert #TMP_PEA
	select pea.numero,
		pea.nom,
		por.autoroute,
		pea.PRsens2,
		XDC_SENS_NORD,
		0,
		0
   	from CFG..RES_PEA pea, CFG..RES_POR por
	where pea.portion = por.numero
	and pea.PRsens2 !=0
	order by pea.numero

	insert #TMP_ECH
	select ech.numero,
		ech.nom,
		por.autoroute,
		ech.PR_entree_sens1,
		XDC_SENS_SUD,
		0,
		0
	from CFG..RES_ECH ech, CFG..RES_POR por
	where ech.portion = por.numero
	and ech.peage_entree_sens1 = 1
	and ech.supprime = XDC_FAUX

	insert #TMP_ECH
	select ech.numero,
		ech.nom,
		por.autoroute,
		ech.PR_entree_sens2,
		XDC_SENS_NORD,
		0,
		0
	from CFG..RES_ECH ech, CFG..RES_POR por
	where ech.portion = por.numero
	and ech.peage_entree_sens2 = 1
	and ech.supprime = XDC_FAUX

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

   declare pea cursor for
  	select numero, autoroute, PR, sens , distance, doublon
	from #TMP_PEA
	for update of distance, doublon
   open pea
   fetch pea into @vl_numero_zone, @vl_autoroute_zone, @vl_PR_baf, @vl_sens_baf, @vl_distance, @vl_doublon
   while (@@sqlstatus = 0)
   begin
	/*calcule distance entre peage et evt*/
	exec @vl_cr = XZAO423 @vl_autoroute_zone, @vl_PR_baf, @vl_sens_baf, 
			@va_Autoroute_in, @vl_PR_min, @va_Sens_in, @vl_PR_max ,
							@vl_distance2 output, @vl_presence_bifurc output,
							@vl_destination output
	/*select @vl_autoroute_zone, @vl_PR_baf, @vl_sens_baf,
				@va_Autoroute_in, @vl_PR_min, @va_Sens_in, @vl_PR_max ,
				@vl_distance2*/
	if @vl_distance2 = 99999 or @vl_distance2 < 0
		or @vl_distance2 > 30000
	begin
		update #TMP_PEA set doublon = 1 where numero = @vl_numero_zone and sens =  @vl_sens_baf
	end
	else
	begin
		update #TMP_PEA set distance = @vl_distance2 where numero = @vl_numero_zone and sens =  @vl_sens_baf

	end
   	fetch pea into @vl_numero_zone, @vl_autoroute_zone, @vl_PR_baf, @vl_sens_baf, @vl_distance, @vl_doublon
   end
   close pea

   insert #TMP_PEA2
   select * from #TMP_PEA where doublon = 0
   group by autoroute, sens
   having distance = min (distance)

  update #TMP_PEA set doublon = 1
  declare pea2 cursor for select numero, autoroute, PR, sens , distance
  from #TMP_PEA2
  open pea2
  fetch pea2 into @vl_numero_zone, @vl_autoroute_zone, @vl_PR_baf, @vl_sens_baf, @vl_distance
  while (@@sqlstatus = 0)
  begin
    update #TMP_PEA set doublon = 0 where numero =  @vl_numero_zone
	and sens = @vl_sens_baf
    fetch pea2 into @vl_numero_zone, @vl_autoroute_zone, @vl_PR_baf, @vl_sens_baf, @vl_distance
  end
  close pea2

	declare ech cursor for
		select numero, autoroute, PR, sens , distance, doublon
		from #TMP_ECH
		for update of distance, doublon
	open ech
	fetch ech into @vl_numero_zone, @vl_autoroute_zone, @vl_PR_baf, @vl_sens_baf, @vl_distance, @vl_doublon

   while (@@sqlstatus = 0)
   begin
	/*calcule distance entre peage et evt*/
	exec @vl_cr = XZAO423 @vl_autoroute_zone, @vl_PR_baf, @vl_sens_baf, 
			@va_Autoroute_in, @vl_PR_min, @va_Sens_in, @vl_PR_max ,
							@vl_distance2 output, @vl_presence_bifurc output,
							@vl_destination output
	/*select @vl_autoroute_zone, @vl_PR_baf, @vl_sens_baf,
				@va_Autoroute_in, @vl_PR_min, @va_Sens_in, @vl_PR_max ,
				@vl_distance2*/
	if @vl_distance2 = 99999 or @vl_distance2 < 0
		or @vl_distance2 > 30000
	begin
		update #TMP_ECH set doublon = 1 where numero = @vl_numero_zone and sens =  @vl_sens_baf
	end
	else
	begin
		update #TMP_ECH set distance = @vl_distance2 where numero = @vl_numero_zone and sens =  @vl_sens_baf

	end

	/*ne garder que les entrees en aval du peage amont*/
	
	if @vl_sens_baf = XDC_SENS_NORD
	begin
	if exists (select * from #TMP_PEA
	where autoroute = @vl_autoroute_zone
	and doublon = 0
	and sens = @vl_sens_baf
	and PR < @vl_PR_baf)
		update #TMP_ECH set doublon = 1 where numero = @vl_numero_zone and sens =  @vl_sens_baf
	end
	else
	begin
	if exists (select * from #TMP_PEA
	where autoroute = @vl_autoroute_zone
	and doublon = 0
	and sens = @vl_sens_baf
	and PR > @vl_PR_baf)
		update #TMP_ECH set doublon = 1 where numero = @vl_numero_zone and sens =  @vl_sens_baf
	end
   	fetch ech into @vl_numero_zone, @vl_autoroute_zone, @vl_PR_baf, @vl_sens_baf, @vl_distance, @vl_doublon
   end
   close ech

   /*select * from #TMP_PEA where doublon = 0*/
   /* select * from #TMP_ECH where doublon = 0*/
   create table #TMP_BAF
   (   
    numero_zone                 smallint null,
    numero_baf                  smallint null,
    autoroute_baf               tinyint null,
    sens_baf                    tinyint null,
    PR_baf                      int    null,
    site_baf                    tinyint  null,
    autoroute_zone              tinyint null,
    sens_zone                   tinyint null,
    PR_debut_zone               int    null,
    PR_fin_zone                 int   null,
    doublon 			tinyint null,
    distance 			int null
   )
   create unique index baf_index on #TMP_BAF (numero_zone)

   declare baf cursor
   for select 
		numero_zone,
		autoroute_zone,
                numero_baf,
                autoroute_baf,
                sens_baf,
                PR_baf,
		site_baf,
		doublon,
		distance
	from #TMP_BAF where doublon=0
   for update of doublon, distance

	/*recuperation de toutes les zones */
	insert #TMP_BAF
        select  numero_zone,
                        numero_baf,
                        autoroute_baf,
                        sens_baf,
                        PR_baf,
                        site_baf,
                        autoroute_zone,
                        sens_zone,
                        PR_debut_zone,
                        PR_fin_zone,
			0,
			0
	from CFG..ZON_BAF_SAGA
	where inhibition = 0
	and autoroute_zone = @va_Autoroute_in
	and sens_zone = @va_Sens_in
	and PR_debut_zone <= @va_PR_evt_in
	and PR_fin_zone >=  @va_PR_evt_in
	order by numero_baf

/**********************premiere passe : on vire les baf dans la perturbation ou non operationels ***************************/

	open baf

	fetch baf into @vl_numero_zone,
		@vl_autoroute_zone,
		@vl_numero_baf,
		@vl_autoroute_baf,
		@vl_sens_baf,
		@vl_PR_baf,
		@vl_site_baf,
		@vl_doublon,
		@vl_distance

	while (@@sqlstatus = 0)
        begin
	  /* si doublon != 0 on ne traite pas */
	  if @vl_doublon = 0
	  begin
	    /*supprimer les baf dans la perturbation */
	   /* if ( @vl_autoroute_baf=@va_Autoroute_in and @vl_sens_baf=@va_Sens_in and @vl_PR_baf between @vl_PR_min and @vl_PR_max)
	    begin
              update #TMP_BAF set doublon=3 where numero_baf = @vl_numero_baf  
            end
            else*/
	    begin
	      /* calcul de la distance entre le baf et l'evt. Si cette distance != 99999 alors le baf est en amont */
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
              exec @vl_cr = XZAO423 @vl_autoroute_baf   ,
		   		@vl_PR_baf          ,
				@vl_sens_baf,
		      		@va_Autoroute_in   ,
			    	@vl_PR_debut_evt ,
			 	@va_Sens_in    ,
			    	@vl_PR_fin_evt ,
			        @vl_distance output,
				@vl_presence_bifurc output,
				@vl_destination output

              if @vl_distance = 99999 or @vl_distance < 0
	      begin
                update #TMP_BAF set doublon=4 where numero_baf = @vl_numero_baf  
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
	        where type=XDC_EQT_SAGA and dernier=1 and equipement=@vl_numero_baf
		and equipement in (select numero from CFG..EQT_SAGA
		where type_saga = XDC_SAGA_BAF)

                if ( @vl_HS = 1 or @vl_inhibe=1 or @vl_desactive=1 or @vl_majeure=1 or @vl_critique=1 )
		begin
	          update #TMP_BAF set doublon = 2  where numero_baf = @vl_numero_baf
		end
		else
		begin
		  /* mise a jour des donnees */
	          update #TMP_BAF
	          set distance = @vl_distance
	          where numero_baf = @vl_numero_baf
                end
              end
            end

	    /*supprimer les BAF en amont du peage*/
	    if (@va_Filtre_peage_in=1)
	    begin
	    /*ne pas prendre les BAF sur section courante*/
	    if not exists (select *
	    from CFG..EQT_GEN
	    where type = XDC_EQT_SAGA and 
	    numero = @vl_numero_baf
	    and nom like 'BA$Date: 2017/03/31 17:47:05 $S%')
	    begin
		update #TMP_BAF set doublon = 2  where numero_baf = @vl_numero_baf
	    end
	    if (@va_Sens_in = XDC_SENS_NORD)
	    begin
	    	if exists (select * from #TMP_PEA
			where  autoroute =  @vl_autoroute_baf
			and sens = @vl_sens_baf
			and PR < @vl_PR_baf and doublon = 0)
		  update #TMP_BAF set doublon = 2  where numero_baf = @vl_numero_baf
	    end
	    else
	    begin
		 if exists (select * from #TMP_PEA
			where  autoroute =  @vl_autoroute_baf
			and sens = @vl_sens_baf
			and PR > @vl_PR_baf and doublon = 0)
		 begin
			update #TMP_BAF set doublon = 2  where numero_baf = @vl_numero_baf
		end
	    end
	    end
          end
	  fetch baf into @vl_numero_zone,
		@vl_autoroute_zone,
		@vl_numero_baf,
		@vl_autoroute_baf,
		@vl_sens_baf,
		@vl_PR_baf,
		@vl_site_baf,
		@vl_doublon,
		@vl_distance
        end

	close baf

/*********************on retourne les resultats*****************************************************************************/

   select distinct 
	  numero_baf,
	  site_baf,
	  autoroute_baf,
	  PR_baf,
	  distance,
	  sens_baf
  from #TMP_BAF
  where  doublon = 0 
      order by autoroute_baf,sens_baf,PR_baf

   drop table #TMP_BAF

 drop table #TMP_PEA
 drop table #TMP_ECH
   return XDC_OK
go 
	 
