/*E*/
/*  Fichier : $Id: xzae185sp.prc,v 1.7 2018/03/07 17:09:56 gesconf Exp $     Release : $Revision: 1.7 $       Date : $Date: 2018/03/07 17:09:56 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae185sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* lecture des dernieres fmc modifiees a exporter vers
* le PC strada
------------------------------------------------------
* HISTORIQUE :
*
* JMG	11/10/08	creation DEM/83
* JMG	22/09/09	plus d EXERCICE 1.2
* JMG	11/11/12 	regulation
* JMG	15/05/13 	correction sens 1
* JMG	17/08/17	correction XZAE1850->XZAE185 + filtre CFG..ADA_EVT
* PNI	07/03/18	correction liste FMC regulation : prise en cpte diffusion_datex
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE185_Recherche_Dernieres_FMC
*
* Arguments en entree
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Lecture des des infos associees a un evt
-------------------------------------------------------*/

use PRC
go
drop procedure XZAE185
go

create procedure XZAE185
as
	declare @vl_evt int, @vl_cle tinyint, @vl_diff int,
		@vl_scenario smallint,
		@vl_num int, @vl_zz int,@vl_indice int, @vl_zone smallint,
		@vl_horodate_validation datetime,
		@vl_commande smallint, @vl_autoroute smallint,
		@vl_PR int, @vl_sens tinyint,
		@vl_pr_fin int, @vl_nb int,
		@vl_debut_zone int, @vl_fin_zone int,
		@vl_PR2 int, @vl_sens_zone tinyint, @vl_commande2 smallint

create table #EXPORT_DTX (
        evenement int null,
        cle tinyint null,
        type smallint null,
        horodate_validation datetime null,
        VR tinyint null,
        VM2 tinyint null,
        VM1 tinyint null,
        VL tinyint null,
        BAU tinyint null,
        VR_I tinyint null,
        VM2_I tinyint null,
        VM1_I tinyint null,
        VL_I tinyint null,
        BAU_I tinyint null,
        confirme tinyint null,
        autoroute tinyint null,
        PR int null,
        sens tinyint null,
        point_caracteristique tinyint null,
        numero_point_caracteristique tinyint null,
        longueur int null,
	zz int,
	PR_fin int null)

create table #EXPORT_DTX_REGUL (
        evenement int null,
        cle tinyint null,
        type smallint null,
        horodate_validation datetime null,
        VR tinyint null,
        VM2 tinyint null,
        VM1 tinyint null,
        VL tinyint null,
        BAU tinyint null,
        VR_I tinyint null,
        VM2_I tinyint null,
        VM1_I tinyint null,
        VL_I tinyint null,
        BAU_I tinyint null,
        confirme tinyint null,
        autoroute tinyint null,
        PR int null,
        sens tinyint null,
        point_caracteristique tinyint null,
        numero_point_caracteristique tinyint null,
        longueur int null,
	zz int,
	PR_fin int null)

declare fmc cursor
for select evenement, cle, horodate_validation, zz
from #EXPORT_DTX_REGUL

	/*recherche FMC a exporter*/
	insert into #EXPORT_DTX
	select G.numero, G.cle, G.type,
        H.horodate_validation,
        H.VR,
        H.VM2,
        H.VM1,
        H.VL,
        H.BAU,
        H.VR_I,
        H.VM2_I,
        H.VM1_I,
        H.VL_I,
        H.BAU_I,
        H.confirme_ou_signale,
        H.autoroute,
        H.PR,
        H.sens,
        H.point_caracteristique,
        H.numero_point_caracteristique,
        0,
	G.zz,
	0
	from EXP..FMC_GEN G, EXP..FMC_HIS_DER H
	where G.numero = H.numero and G.cle = H.cle
	and (H.diffusion_datex = 0 or diffusion_datex=1)
	and G.origine_creation not like '%ESSAI%'
	and G.origine_creation not like '%EXERCICE%'
	and G.datex = XDC_FAUX
	and G.type <> XZAEC_FMC_TeteBouchon
	and G.type <> XZAEC_FMC_PosteOperateur
	and G.type <> XZAEC_FMC_Regulation
	and H.autoroute in (select distinct autoroute from CFG..ADA_EVT)

/*and (G.numero != 90708 or G.cle != 15)*/
/*and (G.numero != 88564 or G.cle != 15)*/
	/*recherche FMC a exporter*/
	insert into #EXPORT_DTX_REGUL
	select G.numero, G.cle, G.type,
        H.horodate_validation,
        H.VR,
        H.VM2,
        H.VM1,
        H.VL,
        H.BAU,
        H.VR_I,
        H.VM2_I,
        H.VM1_I,
        H.VL_I,
        H.BAU_I,
        H.confirme_ou_signale,
        H.autoroute,
        H.PR,
        H.sens,
        H.point_caracteristique,
        H.numero_point_caracteristique,
        0,
	G.zz,
	0
	from EXP..FMC_GEN G, EXP..FMC_HIS_DER H
	where G.numero = H.numero and G.cle = H.cle
	and (H.diffusion_datex = 0 or diffusion_datex=1)
	and G.origine_creation not like '%ESSAI%'
	and G.origine_creation not like '%EXERCICE%'
	and G.datex = XDC_FAUX
	and type = XZAEC_FMC_Regulation
open fmc 

create table #CMD(
	commande smallint null,
	autoroute smallint null,
	PR	int null,
	sens tinyint null)

declare cmd cursor
for select commande, autoroute, PR, sens
from #CMD


fetch fmc into @vl_num,@vl_cle, @vl_horodate_validation, @vl_zz
while @@sqlstatus = 0
begin
	/*recup des commandes de la fmc regulation*/
	select @vl_scenario = -1
	select @vl_scenario = scenario, @vl_zone = zone from EXP..FMC_REG
	where numero = @vl_num and cle = @vl_cle
	and horodate in (select max(horodate) from EXP..FMC_REG
		where   numero = @vl_num and cle = @vl_cle )

	select @vl_debut_zone=debut, @vl_fin_zone=fin, @vl_sens_zone = sens
	from CFG..ZON_REG
	where numero = @vl_zone

	if @vl_scenario <> -1
	begin
		delete #CMD

		insert into #CMD
		select commande, autoroute, PR, sens
		from CFG..REG_SCE_CMD CMD, CFG..EQT_GEN GEN
		where CMD.scenario = @vl_scenario
		and GEN.numero = CMD.prv
		and GEN.type = XDC_EQT_PRV
		and sens = @vl_sens_zone
		order by PR

		select @vl_nb = count(*) from #CMD

		select * from #CMD

		open cmd
		select @vl_indice = 0
		fetch cmd into @vl_commande,@vl_autoroute, @vl_PR,@vl_sens
		while @@sqlstatus = 0
		begin
			if @vl_sens_zone = XDC_SENS_NORD
			begin
				if @vl_indice < @vl_nb 
				begin
				insert into #EXPORT_DTX values( @vl_num, @vl_cle, XZAEC_FMC_Regulation,
					@vl_horodate_validation, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, @vl_autoroute,
					@vl_debut_zone, @vl_sens, null, null, @vl_commande, @vl_zz, @vl_PR)
				select @vl_indice = @vl_indice  + 1
				select @vl_debut_zone = @vl_PR
				end
			end
			else /*zone en sens 1*/
			begin
				if @vl_indice = 0
				begin
					/*premier PR*/
					select @vl_PR2 = @vl_PR
					select @vl_commande2 = @vl_commande
					select @vl_indice = @vl_indice  + 1
				end
				else if @vl_indice < @vl_nb
				begin
				insert into #EXPORT_DTX values( @vl_num, @vl_cle, XZAEC_FMC_Regulation,
					@vl_horodate_validation, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, @vl_autoroute,
					@vl_PR2, @vl_sens, null, null, @vl_commande2, @vl_zz, @vl_PR)
				select @vl_indice = @vl_indice  + 1
				select @vl_PR2 = @vl_PR
				select @vl_commande2 = @vl_commande
				end
				
				if @vl_indice = @vl_nb
				begin
				insert into #EXPORT_DTX values( @vl_num, @vl_cle, XZAEC_FMC_Regulation,
					@vl_horodate_validation, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, @vl_autoroute,
					 @vl_PR2, @vl_sens, null, null, @vl_commande2, @vl_zz, @vl_fin_zone)
				end
			end	
			fetch cmd into @vl_commande,@vl_autoroute, @vl_PR,@vl_sens
		end
		close cmd
	end
	fetch fmc into @vl_num,@vl_cle, @vl_horodate_validation, @vl_zz
end
close fmc

	select * from #EXPORT_DTX
	order by evenement, cle
	return XDC_OK

go

