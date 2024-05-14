/*E*/
/* Fichier : $Id: xzao4350sp.prc,v 1.3 2017/11/15 12:52:12 devgfi Exp $        Release : $Revision: 1.3 $        Date : $Date: 2017/11/15 12:52:12 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao4350sp.prc
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
* SP XZAO4350_Calcul_TDP_RTFM
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
* LCL	25/01/2012: Ajout Temps de parcours etendu (DEM/1014)
* PNI	20/04/12 : Optimisation
* PNI	26/06/12 : Prise en compte du sens FI 1277
* PNI	13/12/12 : On ne prend en cpte que la zone origine ou la zone destination si ceux sont les mêmes v1.5
* PNI		26/09/16	: ajot inhibition=0 1.2 dem1187
* JPL	10/10/17 : Remplacement du champ 'code' par 'code_zone' (pour DEM 1236)  1.3
------------------------------------------------------*/
    
use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO4350' and type = 'P')
	drop procedure XZAO4350
go


create procedure XZAO4350
	
	
	@va_sens int,	
	@va_zone_origine char(30),
	@va_zone_destination char(30),
	@va_autoroute smallint,
	@va_horodate datetime,
	@va_horo_out datetime output,
	@va_temps_out  varchar(25) output,
	@va_temps_libre_out varchar(25) output
   	
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
	@vl_temps_partiel_libre int,
	@vl_temps_zone_origine int,
	@vl_temps_zone_origine_libre int,
	@vl_temps_zone_destination int,
	@vl_tps_zone_dest_libre int,
	@vl_temps_total int,
	@vl_temps_total_libre int,
	@vl_temps_total_mn int,
	@vl_temps_total_mn_libre int,
	@vl_temps_total_s int,
	@vl_temps_total_s_libre int,
	@vl_tdp1_1 varchar(25),
	@vl_tdp1_1_libre varchar(25),
	@vl_1 float,
	@vl_2 float,
	@vl_3 float,
	@vl_4 float,
	@vl_numero_zone int,
	@vl_code int,
	@vl_temps int,
	@vl_temps_libre int,
	@vl_code_origine int,
	@vl_code_destination int,
	@vl_zone_HS int,
	@vl_horo datetime,
	@vl_type bit, @vl_tdp smallint,
	@vl_cpt int
   
if   @va_zone_origine =null or @va_zone_destination = null or @va_sens = null
   	return XDC_ARG_INV


create table #TMP_TDP ( sens int,
                        origine char(30),
                        destination char(30),
                        tdp char(25),
			vl_horo datetime,
			autoroute smallint,
			tdp_libre char(25))

create table #TMP_VAL ( temps int null, temps_libre int null)

begin
/*select @vl_horodate_debut = dateadd( minute,-6,getdate())
select @vl_horodate_fin = getdate()*/

select @vl_horo = @va_horodate

select @vl_PR_debut_zone = D.PR
from	CFG..RES_DTP D
where	D.libelle_TFM = @va_zone_origine
and	D.sens = @va_sens

select @vl_PR_fin_zone = D.PR
from    CFG..RES_DTP D
where   D.libelle_TFM = @va_zone_destination
and     D.sens = @va_sens

select @vl_num_zone_origine = G.numero,@vl_code_origine = Z.code_zone
from	CFG..EQT_GEN G, CFG..EQT_ZDP Z
where 	G.numero = Z.numero
and	G.PR <= @vl_PR_debut_zone
and 	Z.PR_fin >= @vl_PR_debut_zone
and 	G.sens = @va_sens
and	G.type = 39
and	G.autoroute = @va_autoroute
and	Z.inhibition=0

select @vl_code_origine = Z.code_zone
from 	CFG..EQT_ZDP Z
where 	Z.numero = @vl_num_zone_origine

select @vl_num_zone_destination = min(G.numero)
from    CFG..EQT_GEN G, CFG..EQT_ZDP Z
where   G.numero = Z.numero
and     G.PR <= @vl_PR_fin_zone
and     Z.PR_fin >= @vl_PR_fin_zone
and     G.sens = @va_sens
and     G.type = 39
and	G.autoroute = @va_autoroute
and	Z.inhibition=0

select @vl_code_destination = Z.code_zone
from    CFG..EQT_ZDP Z
where   Z.numero = @vl_num_zone_destination and Z.type_zone =1

if @@rowcount = 0
select @vl_code_destination = 0

/*zone origine*/
select @vl_PR_1_1 = Z.PR_fin
from 	CFG..EQT_ZDP Z
where 	Z.numero = @vl_num_zone_origine

select @vl_PR_1_0 = G.PR
from 	CFG..EQT_GEN G
where 	G.numero = @vl_num_zone_origine
and	G.type = 39

/*zone destination */
select @vl_PR_2_1 = Z.PR_fin
from    CFG..EQT_ZDP Z
where   Z.numero = @vl_num_zone_destination

select @vl_PR_2_0 = G.PR
from    CFG..EQT_GEN G
where   G.numero = @vl_num_zone_destination
and     G.type = 39

if (@va_sens = 1)
begin
	select @vl_1 = (@vl_PR_1_1 - @vl_PR_debut_zone)
	select @vl_2 = (@vl_PR_1_1 - @vl_PR_1_0)
	if (@vl_1 = 0)
		select @vl_delta_origine = 1
	else
		select @vl_delta_origine = @vl_1 / @vl_2

	select @vl_3 = (@vl_PR_fin_zone - @vl_PR_2_0)
	select @vl_4 = (@vl_PR_2_1 - @vl_PR_2_0)
	select @vl_delta_destination = (@vl_3 / @vl_4) 
end
if (@va_sens = 2 )
begin
	select @vl_1 = (@vl_PR_debut_zone - @vl_PR_1_0)
	select @vl_2 = (@vl_PR_1_1 - @vl_PR_1_0)
	select @vl_delta_origine = (@vl_1 / @vl_2)

	select @vl_3 = (@vl_PR_2_1 - @vl_PR_fin_zone)
	select @vl_4 = (@vl_PR_2_1 - @vl_PR_2_0)
	if (@vl_3 = 0)
		select @vl_delta_destination = 1
	else
		select @vl_delta_destination = (@vl_3 / @vl_4)
end
	select  @vl_zone_HS = 0

if (@va_sens = 1)
begin

	insert into #TMP_VAL select  T.temps, Z.TDP_habituel
	from 	EXP..ZDP_6MN T, 
		CFG..EQT_ZDP Z,
		CFG..EQT_GEN G
	where	G.type = 39
	and	G.numero = Z.numero 
	and	T.numero = G.numero
	and 	(Z.code_zone!=0 and Z.type_zone!=0)
	and 	G.PR > @vl_PR_debut_zone
	and 	Z.PR_fin < @vl_PR_fin_zone
	and	G.sens = @va_sens and G.autoroute=@va_autoroute
	and	T.horodate <= @vl_horo
	and	T.horodate > dateadd(minute,-6, @vl_horo)

	insert into #TMP_VAL select Z.TDP_habituel, Z.TDP_habituel
	from 	EXP..ZDP_6MN T, 
		CFG..EQT_ZDP Z,
		CFG..EQT_GEN G
	where	G.type = 39
	and	G.numero = Z.numero 
	and	T.numero = G.numero
	and 	(Z.code_zone=0 or Z.type_zone=0)
	and 	G.PR > @vl_PR_debut_zone
	and 	Z.PR_fin < @vl_PR_fin_zone
	and	G.sens = @va_sens and G.autoroute=@va_autoroute
	and	T.horodate <= @vl_horo
	and	T.horodate > dateadd(minute,-6, @vl_horo)


	select @vl_cpt=count(*) from  #TMP_VAL where (temps =0) or (temps =null)
	if @vl_cpt > 0
		select @vl_zone_HS = 1

	select @vl_temps_partiel = sum(temps), 
		@vl_temps_partiel_libre = sum(temps_libre)
	from #TMP_VAL



	if @vl_temps_partiel = null
		select @vl_temps_partiel = 0
	if @vl_temps_partiel_libre= null
		select @vl_temps_partiel_libre = 0
end
else 
begin

	insert into #TMP_VAL select  T.temps, Z.TDP_habituel
	from 	EXP..ZDP_6MN T, 
		CFG..EQT_ZDP Z,
		CFG..EQT_GEN G
	where	G.type = 39
	and	G.numero = Z.numero 
	and	T.numero = G.numero
	and 	(Z.code_zone!=0 and Z.type_zone!=0)
	and 	G.PR > @vl_PR_fin_zone
	and 	Z.PR_fin < @vl_PR_debut_zone
	and	G.sens = @va_sens and G.autoroute=@va_autoroute
	and	T.horodate <= @vl_horo
	and	T.horodate > dateadd(minute,-6, @vl_horo)

	insert into #TMP_VAL select Z.TDP_habituel, Z.TDP_habituel
	from 	EXP..ZDP_6MN T, 
		CFG..EQT_ZDP Z,
		CFG..EQT_GEN G
	where	G.type = 39
	and	G.numero = Z.numero 
	and	T.numero = G.numero
	and 	(Z.code_zone=0 or Z.type_zone=0)
	and 	G.PR > @vl_PR_fin_zone
	and 	Z.PR_fin < @vl_PR_debut_zone
	and	G.sens = @va_sens and G.autoroute=@va_autoroute
	and	T.horodate <= @vl_horo
	and	T.horodate > dateadd(minute,-6, @vl_horo)



	select @vl_cpt=count(*) from  #TMP_VAL where (temps =0) or (temps =null)
	if @vl_cpt > 0
		select @vl_zone_HS = 1

	select @vl_temps_partiel = sum(temps), @vl_temps_partiel_libre = sum(temps_libre)
	from #TMP_VAL



	if @vl_temps_partiel = null
		select @vl_temps_partiel = 0
	if @vl_temps_partiel_libre= null
		select @vl_temps_partiel_libre= 0
end


if (@vl_code_origine = 0)
begin
	select @vl_temps_zone_origine = Z.TDP_habituel
	from   CFG..EQT_ZDP Z
	where  Z.numero = @vl_num_zone_origine
	select @vl_temps_zone_origine_libre = @vl_temps_zone_origine
end
else
begin
	select @vl_temps_zone_origine_libre = Z.TDP_habituel
	from   CFG..EQT_ZDP Z
	where  Z.numero = @vl_num_zone_origine

        if (@vl_num_zone_destination<>@vl_num_zone_origine ) or ( @va_sens=XDC_SENS_SUD)
        begin
        	select @vl_temps_zone_origine = T.temps
        	from    EXP..ZDP_6MN T
        	where   T.numero = @vl_num_zone_origine
		and	T.horodate <= @vl_horo
		and	T.horodate > dateadd(minute,-6, @vl_horo)
        end
        else
        	select @vl_temps_zone_origine = 1
end

if (@vl_code_destination = 0)
begin
	select @vl_temps_zone_destination = Z.TDP_habituel
	from   CFG..EQT_ZDP Z
	where  Z.numero = @vl_num_zone_destination
	select @vl_tps_zone_dest_libre = @vl_temps_zone_destination
end
else
begin
	select @vl_tps_zone_dest_libre = Z.TDP_habituel
	from   CFG..EQT_ZDP Z
	where  Z.numero = @vl_num_zone_destination

        if (@vl_num_zone_destination<>@vl_num_zone_origine ) or ( @va_sens=XDC_SENS_NORD)
        begin
        select @vl_temps_zone_destination = T.temps
        from    EXP..ZDP_6MN T
        where   T.numero = @vl_num_zone_destination
	and	T.horodate <= @vl_horo
	and	T.horodate > dateadd(minute,-6, @vl_horo)
        end
        else
        select @vl_temps_zone_destination=1
end

if ((@vl_temps_zone_origine = null) or 
	(@vl_temps_zone_origine = 0) or
	(@vl_temps_zone_destination = null) or 
	(@vl_temps_zone_destination = 0) or
	(@vl_zone_HS = 1))
	select @vl_tdp1_1 = "Tdp invalide"
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
	select @vl_tdp1_1 = convert(varchar, @vl_temps_total_mn) + "mn" +
			convert(varchar, @vl_temps_total_s) + "s"
end

select @vl_temps_total_libre = convert(int, @vl_temps_zone_origine_libre * @vl_delta_origine) +
			@vl_temps_partiel_libre +
			convert(int, @vl_tps_zone_dest_libre * @vl_delta_destination)

select @vl_temps_total_mn_libre = @vl_temps_total_libre / 60

select @vl_temps_total_s_libre = @vl_temps_total_libre - (@vl_temps_total_mn_libre * 60)

select @vl_tdp1_1_libre = convert(varchar, @vl_temps_total_mn_libre) + "mn" +
			convert(varchar, @vl_temps_total_s_libre) + "s"

insert into #TMP_TDP values(@va_sens, rtrim(@va_zone_origine), rtrim(@va_zone_destination), @vl_tdp1_1, @vl_horo,
			@va_autoroute, @vl_tdp1_1_libre)
/*select * from #TMP_TDP*/
delete from #TMP_VAL

select @va_horo_out =  @vl_horo,
	@va_temps_out = @vl_tdp1_1,
	@va_temps_libre_out = @vl_tdp1_1_libre
end


return (XDC_OK)
go
