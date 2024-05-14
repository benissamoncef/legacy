/*E*/
/* Fichier : $Id: xzao574sp.prc,v 1.2 2018/11/19 15:42:41 devgfi Exp $      Release : $Revision: 1.2 $        Date : $Date: 2018/11/19 15:42:41 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao574sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		13/02/12 : Creation (DEM/1016)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao574sp.h" */

/* static char *version = "@(#)xzao574.c	1.1 23/18/94 : xzao574" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO574' and type = 'P')
	drop procedure XZAO574
go

	create procedure XZAO574
	   @va_Zone_in			smallint,
	   @va_Autoroute_in		tinyint,
	   @va_PR_PMV_in		int,
	   @va_Sens_in			tinyint,
	   @va_amont_in			int
	   as 

/* 
*
* SP    XZAO553SP
*
* PARAMETRES EN ENTREE :
*
*					va_PR_PMV_in
*  					va_Autoroute_in
* 					va_Sens_in
*
* PARAMETRES EN SORTIE :
*

* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
*
* FONCTION :
*
  Recherche le PRV immediatement en amont du PMV/PMVA et dans la zone de regulation
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    13 Fev 2012    : Creation (DEM/1016)
------------------------------------------------------*/
	declare @vl_vitesse smallint,
		@vl_txt varchar(6)

	if @va_amont_in=1
	begin
		/*rechercher vitesse nominale de la zone*/
		select @vl_vitesse = vitesse_amont - 20
		from CFG..ZON_REG
		where numero = @va_Zone_in

		if (@vl_vitesse < 100)
			select 'L'+convert(char(2),@vl_vitesse)
		else
			select 'L'+convert(char(3),@vl_vitesse)
		return XDC_OK
	end

	if @va_Sens_in = XDC_SENS_NORD
	begin
		select PRV.vitesse
		from   EXP..ACT_GEN ACT,
		  	CFG..EQT_GEN GEN ,
		  	EXP..ACT_PRV PRV
		where  ACT.type = XDC_ACT_PRV
		and	ACT.heure_lancement <= getdate()
		and	ACT.heure_fin is null
		and 	ACT.numero  = PRV.actio
		and 	ACT.sit = PRV.sit
		and	GEN.type = XDC_EQT_PRV
		and	GEN.numero  = ACT.equipement
		and    GEN.PR 	>= @va_PR_PMV_in		
		and   GEN.autoroute = @va_Autoroute_in
		and	GEN.sens = @va_Sens_in
		and GEN.PR in (select min(PR) from CFG..EQT_GEN
				where type = XDC_EQT_PRV
				and autoroute = @va_Autoroute_in
				and sens = @va_Sens_in
				and PR >= @va_PR_PMV_in)
		order by ACT.heure_lancement desc

		if @@rowcount= 0
		begin
			select @vl_vitesse = vitesse_nominale
			from CFG..EQT_PRV PRV, CFG..EQT_GEN GEN
			where GEN.type = XDC_EQT_PRV
			and	GEN.numero = PRV.numero
			and    GEN.PR   >= @va_PR_PMV_in
			and   GEN.autoroute = @va_Autoroute_in
			and     GEN.sens = @va_Sens_in
			and GEN.PR in (select min(PR) from CFG..EQT_GEN
					where type = XDC_EQT_PRV
					and autoroute = @va_Autoroute_in
					and sens = @va_Sens_in
					and PR >= @va_PR_PMV_in)
			if (@vl_vitesse < 100)
				select 'L'+convert(char(2),@vl_vitesse)
			else
				select 'L'+convert(char(3),@vl_vitesse)
			return XDC_OK
		end
	end
	else
	begin
		select PRV.vitesse
		from   EXP..ACT_GEN ACT,
		  	CFG..EQT_GEN GEN ,
		  	EXP..ACT_PRV PRV
		where  ACT.type = XDC_ACT_PRV
		and	ACT.heure_lancement <= getdate()
		and	ACT.heure_fin is null
		and 	ACT.numero  = PRV.actio
		and 	ACT.sit = PRV.sit
		and	GEN.type = XDC_EQT_PRV
		and	GEN.numero  = ACT.equipement
		and    GEN.PR 	<= @va_PR_PMV_in		
		and   GEN.autoroute = @va_Autoroute_in
		and	GEN.sens = @va_Sens_in
		and GEN.PR in (select max(PR) from CFG..EQT_GEN
				where type = XDC_EQT_PRV
				and autoroute = @va_Autoroute_in
				and sens = @va_Sens_in
				and PR <= @va_PR_PMV_in)
		order by ACT.heure_lancement desc

		if @@rowcount= 0
		begin
			select @vl_vitesse = vitesse_nominale
			from CFG..EQT_PRV PRV, CFG..EQT_GEN GEN
			where GEN.type = XDC_EQT_PRV
			and	GEN.numero = PRV.numero
			and    GEN.PR   <= @va_PR_PMV_in
			and   GEN.autoroute = @va_Autoroute_in
			and     GEN.sens = @va_Sens_in
			and GEN.PR in (select min(PR) from CFG..EQT_GEN
					where type = XDC_EQT_PRV
					and autoroute = @va_Autoroute_in
					and sens = @va_Sens_in
					and PR <= @va_PR_PMV_in)
			if (@vl_vitesse < 100)
				select 'L'+convert(char(2),@vl_vitesse)
			else
				select 'L'+convert(char(3),@vl_vitesse)
			return XDC_OK
		end
	end
    return XDC_OK
         
go  
