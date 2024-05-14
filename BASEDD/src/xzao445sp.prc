/*E*/
/*Fichier : Fichier : %W%     Release : %I%        Date : %G%
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao445sp.prc
------------------------------------------------------
* HISTORIQUE :
*
* guilhou       21/06/05      : creation  1.1
* JMG	26/11/16	filtre peage amont lot 23 1.1
* PNI   24/08/20        sae-161	having doublon = 0 1.4
* PNI 	11/09/23	: Prise en compte du flag RES_ECH.supprime=XDC_FAUX SAE-517
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao441sp.h" */

/* static char *version = " %W% %I% %G%: xzao441" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  recherche les peages amont
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

drop procedure XZAO445
go

create procedure XZAO445
   @va_Autoroute_in        tinyint = null,
   @va_Sens_in             tinyint = null,
   @va_PR_evt_in	   int = null,
   @va_PR_tete_evt_in	   int = null
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
		rtrim(aut.nom) + '/' + ech.sortie,
		por.autoroute,
		ech.PR_entree_sens1,
		XDC_SENS_SUD,
		0,
		0
	from CFG..RES_ECH ech, CFG..RES_POR por, CFG..RES_AUT aut
	where ech.portion = por.numero
	and ech.peage_entree_sens1 = 1
	and por.autoroute = aut.numero
	and ech.supprime = XDC_FAUX

	insert #TMP_ECH
	select ech.numero,
		rtrim(aut.nom) + '/' + ech.sortie,
		por.autoroute,
		ech.PR_entree_sens2,
		XDC_SENS_NORD,
		0,
		0
	from CFG..RES_ECH ech, CFG..RES_POR por, CFG..RES_AUT aut
	where ech.portion = por.numero
	and ech.peage_entree_sens2 = 1
	and por.autoroute = aut.numero
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
   having doublon = 0 and distance = min (distance)

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

   select numero, nom, autoroute, PR, sens 
   from #TMP_PEA where doublon = 0
   union
   select numero, nom, autoroute, PR, sens 
   from #TMP_ECH where doublon = 0

 drop table #TMP_PEA
 drop table #TMP_ECH
   return XDC_OK
go 
	 
