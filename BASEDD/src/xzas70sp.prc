/*E*/
/*  Fichier : $Id: xzas70sp.prc,v 1.2 2001/10/30 18:32:14 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2001/10/30 18:32:14 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas70sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* lecture de la localisation des stations RADT a exporter vers Strada
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	05/01/1999	: Creation
* Niepceron	09/08/1999	: Ajout version RDS dem/1767 v1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAS70_Lect_Loc_RADT
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

if exists (select * from sysobjects where name = 'XZAS70' and type = 'P')

    drop procedure XZAS70

go

create procedure XZAS70

as
declare 
	@vl_numero	smallint,
	@vl_sens	tinyint,
	@vl_nom		char(15),
	@vl_table	char(4),
	@vl_PR		int,
	@vl_autoroute	tinyint,
	@vl_loc_rds	int,
	@vl_distance	int,
	@vl_sens_rds	char(1),
	@vl_lib_sens	char(35),
	@vl_version	char(35)

	
	create table #STA ( numero smallint,nom char(15), table_rds char(4), version_rds char(35), locrds int, distance int, sens char(1),lib_sens char(35))
	
	select @vl_table=numero, @vl_version=version from CFG..NUM_RDS
	
	declare Pointeur cursor
	for select numero, sens , nom
	from CFG..ADA_STA

	open Pointeur
	fetch Pointeur into @vl_numero,@vl_sens,@vl_nom

	while (@@sqlstatus = 0)
	begin
		select @vl_PR=PR,@vl_autoroute=autoroute 
		from CFG..EQT_GEN
		where numero=@vl_numero and type=XDC_EQT_RAD

		if ( @vl_sens=XDC_SENS_NORD) or 
		   ( @vl_sens=XDC_SENS_SORTIE_NORD) or
		   ( @vl_sens=XDC_SENS_ENTREE_NORD)
		 begin
		 	select @vl_sens_rds="N"
		 	if @vl_autoroute=1
		 		select @vl_lib_sens=XZASC_LIB_N_1
		 	else
		 	if @vl_autoroute=2
		 		select @vl_lib_sens=XZASC_LIB_N_2
		 	else
		 	if @vl_autoroute=3
		 		select @vl_lib_sens=XZASC_LIB_N_3
		 	else
		 	if @vl_autoroute=4
		 		select @vl_lib_sens=XZASC_LIB_N_4
		 	else
		 	if @vl_autoroute=5
		 		select @vl_lib_sens=XZASC_LIB_N_5
		 	else
		 	if @vl_autoroute=6
		 		select @vl_lib_sens=XZASC_LIB_N_6
		 	else
		 	if @vl_autoroute=7
		 		select @vl_lib_sens=XZASC_LIB_N_7
		 	else
		 		select @vl_lib_sens=XZASC_LIB_N_8
		 end
		 else
		 begin
		 	select @vl_sens_rds="P"
		 	if @vl_autoroute=1
		 		select @vl_lib_sens=XZASC_LIB_P_1
		 	else
		 	if @vl_autoroute=2
		 		select @vl_lib_sens=XZASC_LIB_P_2
		 	else
		 	if @vl_autoroute=3
		 		select @vl_lib_sens=XZASC_LIB_P_3
		 	else
		 	if @vl_autoroute=4
		 		select @vl_lib_sens=XZASC_LIB_P_4
		 	else
		 	if @vl_autoroute=5
		 		select @vl_lib_sens=XZASC_LIB_P_5
		 	else
		 	if @vl_autoroute=6
		 		select @vl_lib_sens=XZASC_LIB_P_6
		 	else
		 	if @vl_autoroute=7
		 		select @vl_lib_sens=XZASC_LIB_P_7
		 	else
		 		select @vl_lib_sens=XZASC_LIB_P_8
		 end
		
		
		select @vl_distance=pr-@vl_PR,@vl_loc_rds=numero from CFG..RDS 
		where pr >= @vl_PR and autoroute = @vl_autoroute having pr = min(pr)
		 
		 insert #STA
		 (numero ,nom , table_rds , version_rds, locrds, distance , sens ,lib_sens)
		 values (@vl_numero,rtrim(@vl_nom),@vl_table,@vl_version, @vl_loc_rds,@vl_distance,@vl_sens_rds,@vl_lib_sens)

		fetch Pointeur into @vl_numero,@vl_sens,@vl_nom
	
	end
	close Pointeur
	
	select * from #STA
	
	return XDC_OK

go

