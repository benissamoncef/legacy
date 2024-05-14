/*E*/
/*Fichier : @(#)xzao435sp.prc	1.3      Release : 1.3        Date : 02/14/97
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao435sp.prc
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
*  Calcul les tdp pour message RTFM en fonction des zones origines et destinations recues
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
* SP XZAO435_Calcul_TDP_RTFM
*
* PARAMETRES EN ENTREE :
* sens int
* origine char(30)
* destination char(30)
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
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
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* CHI	05/07/2005: Creation v1.1
*
------------------------------------------------------*/
    
use PRC
go
drop proc XZAS103
go
create procedure XZAS103
	
	@va_Autoroute_in	tinyint		= null,
	@va_PRDebut_in int,
	@va_PRFin_in int,
	@va_Sens_in int,	
   	@va_Tdp_out int	output

	as 
   	declare
	@vl_horodate_debut datetime,
	@vl_horodate_fin datetime,
	@vl_PR_debut_zone int,
	@vl_PR_fin_zone int,
	@vl_num_zone_origine int,
	@vl_num_zone_destination int,
	@vl_PR_1_1 int,
	@vl_PR_1_0 int,
	@vl_delta_origine float,
	@vl_PR_2_1 int,
	@vl_PR_2_0 int,
	@vl_delta_destination float,
	@vl_temps_partiel int,
	@vl_temps_zone_origine int,
	@vl_temps_zone_destination int,
	@vl_temps_total int,
	@vl_temps_total_mn int,
	@vl_temps_total_s int,
	@vl_tdp1_1 char(25),
	@vl_1 float,
	@vl_2 float,
	@vl_3 float,
	@vl_4 float,
	@vl_numero_zone int,
	@vl_code int,
	@vl_temps int,
	@vl_code_origine int,
	@vl_code_destination int,
	@vl_zone_HS int
   

create table #TMP_TDP ( sens int null,
                        origine char(30),
                        destination char(30),
                        tdp char(25))

create table #TMP_VAL ( temps int null)

begin
select @vl_horodate_debut = dateadd( minute,-6,getdate())
select @vl_horodate_fin = getdate()

if (@va_Sens_in = XDC_SENS_NORD) and (@va_PRDebut_in<@va_PRFin_in)
begin
	select @vl_PR_1_1  = @va_PRFin_in
	select @va_PRFin_in = @va_PRDebut_in
	select @va_PRDebut_in =@vl_PR_1_1
end


select @vl_num_zone_origine = G.numero,@vl_code_origine = Z.code
from	CFG..EQT_GEN G, CFG..EQT_ZDP Z
where 	G.numero = Z.numero
and	G.PR <= @va_PRDebut_in
and 	Z.PR_fin >= @va_PRDebut_in
and 	G.sens = @va_Sens_in
and	G.type = 39
--select @vl_num_zone_origine

select @vl_code_origine = Z.code
from 	CFG..EQT_ZDP Z
where 	Z.numero = @vl_num_zone_origine
--select @vl_code_origine

select @vl_num_zone_destination = min(G.numero)
from    CFG..EQT_GEN G, CFG..EQT_ZDP Z
where   G.numero = Z.numero
and     G.PR <= @va_PRFin_in
and     Z.PR_fin >= @va_PRFin_in
and     G.sens = @va_Sens_in
and     G.type = 39
--select @vl_num_zone_destination

select @vl_code_destination = Z.code
from    CFG..EQT_ZDP Z
where   Z.numero = @vl_num_zone_destination

/*zone origine*/
select @vl_PR_1_1 = Z.PR_fin
from 	CFG..EQT_ZDP Z
where 	Z.numero = @vl_num_zone_origine
--select @vl_PR_1_1

select @vl_PR_1_0 = G.PR
from 	CFG..EQT_GEN G
where 	G.numero = @vl_num_zone_origine
and	G.type = 39
--select @vl_PR_1_0

/*zone destination */
select @vl_PR_2_1 = Z.PR_fin
from    CFG..EQT_ZDP Z
where   Z.numero = @vl_num_zone_destination
--select @vl_PR_2_1

select @vl_PR_2_0 = G.PR
from    CFG..EQT_GEN G
where   G.numero = @vl_num_zone_destination
and     G.type = 39
--select @vl_PR_2_0 

if (@va_Sens_in = 1)
begin
	select @vl_1 = (@vl_PR_1_1 - @va_PRDebut_in)
	select @vl_2 = (@vl_PR_1_1 - @vl_PR_1_0)
	if (@vl_1 = 0)
		select @vl_delta_origine = 1
	else
		select @vl_delta_origine = @vl_1 / @vl_2

	select @vl_3 = (@va_PRFin_in - @vl_PR_2_0)
	select @vl_4 = (@vl_PR_2_1 - @vl_PR_2_0)
	select @vl_delta_destination = (@vl_3 / @vl_4) 
end
if (@va_Sens_in = 2 )
begin
	select @vl_1 = (@va_PRDebut_in - @vl_PR_1_0)
	select @vl_2 = (@vl_PR_1_1 - @vl_PR_1_0)
	select @vl_delta_origine = (@vl_1 / @vl_2)

	select @vl_3 = (@vl_PR_2_1 - @va_PRFin_in)
	select @vl_4 = (@vl_PR_2_1 - @vl_PR_2_0)
	if (@vl_3 = 0)
		select @vl_delta_destination = 1
	else
		select @vl_delta_destination = (@vl_3 / @vl_4)
end
--select @vl_delta_origine
--select @vl_delta_destination

if (@va_Sens_in = 1)
begin
	declare recherche_virtual_zone cursor for
	select T.numero,Z.code 
	from 	EXP..ZDP_6MN T, 
		CFG..EQT_ZDP Z,
		CFG..EQT_GEN G
	where	G.type = 39
	and	G.numero = Z.numero 
	and	T.numero = G.numero
	and 	G.PR > @va_PRDebut_in
	and 	Z.PR_fin < @va_PRFin_in
	and	G.sens = @va_Sens_in
	and	T.horodate between @vl_horodate_debut and  @vl_horodate_fin
	and	Z.type_zone = 1

	open recherche_virtual_zone
	fetch recherche_virtual_zone into @vl_numero_zone, @vl_code

	while (@@sqlstatus = 0 )
	begin
	 	if (@vl_code = 0) 
		begin
			select @vl_temps = Z.TDP_habituel
			from   CFG..EQT_ZDP Z
			where  Z.numero = @vl_numero_zone
			insert into #TMP_VAL values (@vl_temps)
		end
		else 
		begin
			select @vl_temps = T.temps
			from 	EXP..ZDP_6MN T
			where 	T.numero = @vl_numero_zone
			insert into #TMP_VAL values (@vl_temps)
			
			if ((@vl_temps =0) or (@vl_temps =null))
			begin
				select @vl_zone_HS = 1
			end
		end
		fetch recherche_virtual_zone into @vl_numero_zone, @vl_code
	end
	close recherche_virtual_zone
	
	select @vl_temps_partiel = sum(temps)
	from #TMP_VAL

	if @vl_temps_partiel = null
		select @vl_temps_partiel = 0
end
else 
begin
	select  @vl_zone_HS = 0
	declare recherche_virtual_zone cursor for
	select T.numero , Z.code
	from 	EXP..ZDP_6MN T, 
		CFG..EQT_ZDP Z,
		CFG..EQT_GEN G
	where	G.type = 39
	and	G.numero = Z.numero 
	and	T.numero = G.numero
	and 	G.PR > @va_PRFin_in
	and 	Z.PR_fin < @va_PRDebut_in
	and	G.sens = @va_Sens_in
	and	T.horodate between @vl_horodate_debut and  @vl_horodate_fin
	and	Z.type_zone = 1
	
	open recherche_virtual_zone
	fetch recherche_virtual_zone into @vl_numero_zone, @vl_code
	
	while (@@sqlstatus = 0 )
	begin
		if (@vl_code = 0) 
		begin
			select @vl_temps = Z.TDP_habituel
			from   CFG..EQT_ZDP Z
			where  Z.numero = @vl_numero_zone
			insert into #TMP_VAL values (@vl_temps)
		end
		else 
		begin
			select @vl_temps = T.temps
			from 	EXP..ZDP_6MN T
			where 	T.numero = @vl_numero_zone
			insert into #TMP_VAL values (@vl_temps)

			if ((@vl_temps =0) or (@vl_temps =null))
			begin
				select @vl_zone_HS = 1
			end
		end
		fetch recherche_virtual_zone into @vl_numero_zone, @vl_code
	end
	close recherche_virtual_zone
	
	select @vl_temps_partiel = sum(temps)
	from #TMP_VAL
	
	if @vl_temps_partiel = null
		select @vl_temps_partiel = 0
end
--select @vl_temps_partiel

if (@vl_code_origine = 0)
begin
	select @vl_temps_zone_origine = Z.TDP_habituel
	from   CFG..EQT_ZDP Z
	where  Z.numero = @vl_num_zone_origine
end
else
begin
	select @vl_temps_zone_origine = T.temps
	from 	EXP..ZDP_6MN T
	where	T.numero = @vl_num_zone_origine
	and 	T.horodate between @vl_horodate_debut and  @vl_horodate_fin
end
--select @vl_temps_zone_origine

if (@vl_code_destination = 0)
begin
	select @vl_temps_zone_destination = Z.TDP_habituel
	from   CFG..EQT_ZDP Z
	where  Z.numero = @vl_num_zone_destination
end
else
begin
	select @vl_temps_zone_destination = T.temps
	from    EXP..ZDP_6MN T
	where   T.numero = @vl_num_zone_destination
	and     T.horodate between @vl_horodate_debut and  @vl_horodate_fin
end
--select @vl_temps_zone_destination

if ((@vl_temps_zone_origine = null) or 
	(@vl_temps_zone_origine = 0) or
	(@vl_temps_zone_destination = null) or 
	(@vl_temps_zone_destination = 0) or
	(@vl_zone_HS = 1))
	select @vl_tdp1_1 = "Tdp invalide: zone tdp HS"
else
begin
	select @vl_temps_total = convert(int, @vl_temps_zone_origine * @vl_delta_origine) +
			 @vl_temps_partiel +
			 convert(int, @vl_temps_zone_destination * @vl_delta_destination)

	select @vl_temps_total_mn = @vl_temps_total / 60

	select @vl_temps_total_s = @vl_temps_total - (@vl_temps_total_mn * 60)

	/*select @vl_tdp1_1 = convert(char(5), @vl_temps_total) + "(" +
		     convert(char(2), @vl_temps_total_mn) + "mn" +
		     convert(char(2), @vl_temps_total_s) + "s)" */
	select @vl_tdp1_1 = convert(char(2), @vl_temps_total_mn) + "mn" +
			convert(char(2), @vl_temps_total_s) + "s"
end
select @vl_temps_total
	select @va_Tdp_out=@vl_temps_total



delete from #TMP_VAL
end

return (XDC_OK)
go
