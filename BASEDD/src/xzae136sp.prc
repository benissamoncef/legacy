/*E*/
/*  Fichier : $Id: xzae136sp.prc,v 1.4 2008/08/25 14:49:14 gesconf Exp $      Release : $Revision: 1.4 $        Date : $Date: 2008/08/25 14:49:14 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae127sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
* strada : infos de la fmc bouchon
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou	07 oct 1997 : creation
* Niepceron	07 jan 1999 : modif datex dem/1724 1.2
* Cluchague	12 jan 2007 : modif METACOR ajour param sortie localisation DEM605 v1.3
* Niepceron	25/08/08   : correction sens A57 DEM701
* Niepcero 22/09/22 : Coorection :retour de la valeur abs de la longueur bouchon SAE-420
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE136_Infos_Bouchon_Pour_Strada
*
* Arguments en entree
* - numero de la FMC
* - cle de la FMC
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
* Lecture des des infos obligatoires pour generer
* un fichier STRADA
-------------------------------------------------------*/

use PRC
go

drop proc XZAE136
go

create procedure XZAE136
	@va_numero_in		int	= null,
	@va_cle_in		tinyint = null,
	@va_type_out		smallint output,
	@va_longueur_out	int output,
	@va_autoroute_out	tinyint output,
	@va_PR_out		T_PR output,
	@va_sens_out		tinyint output,
	@va_numero_cause_out	int     = null output,
	@va_cle_cause_out    	tinyint = null output,
	@va_type_loc_out 	tinyint output,
	@va_nom_loc_out 	char(25) output,
	@va_num_loc_out 	int output
as
declare
	@vl_evtcause int,
	@vl_clecause tinyint,
	@vl_pt_car  tinyint,
	@vl_num_pt_car tinyint,
	@vl_inv			smallint
	
	if @va_numero_in is null
		return XDC_ARG_INV
		
	if @va_cle_in is null
		return XDC_ARG_INV
	
	select 	@va_type_out = type
	from EXP..FMC_BOU
	where 
		numero = @va_numero_in and
		cle = @va_cle_in 
	
        select @va_longueur_out = abs(longueur),
		@va_autoroute_out=autoroute,
		@va_PR_out=PR,
		@va_sens_out=sens
        from EXP..FMC_HIS
        where numero = @va_numero_in and
              cle = @va_cle_in
        group by numero, cle
        having horodate_validation=max(horodate_validation)
        
        select 
		@vl_evtcause=cause,
		@vl_clecause=cle_cause
        from EXP..FMC_GEN
        where numero=@va_numero_in and
              cle = @va_cle_in
              
        select  @va_autoroute_out=autoroute,
		@va_PR_out=PR,
		@va_sens_out=sens
        from EXP..FMC_HIS
        where numero = @vl_evtcause and
              cle = @vl_clecause
        group by numero, cle
        having horodate_validation=max(horodate_validation)

	/* Traitement A57 */
	select @vl_inv=inversee
	from CFG..RES_AUT_INV
	where numero=@va_autoroute_out

	if ( @vl_inv<0)
	begin
		if (@va_sens_out=XDC_SENS_SUD) select @va_sens_out=XDC_SENS_NORD
		else
		if (@va_sens_out=XDC_SENS_SORTIE_SUD) select @va_sens_out=XDC_SENS_SORTIE_NORD
		else
		if (@va_sens_out=XDC_SENS_ENTREE_SUD) select @va_sens_out=XDC_SENS_ENTREE_NORD
		else
		if (@va_sens_out=XDC_SENS_NORD) select @va_sens_out=XDC_SENS_SUD
		else
		if (@va_sens_out=XDC_SENS_SORTIE_NORD) select @va_sens_out=XDC_SENS_SORTIE_SUD
		else if (@va_sens_out=XDC_SENS_ENTREE_NORD) select @va_sens_out=XDC_SENS_ENTREE_SUD
	end

	select
		@va_numero_cause_out=cause,
		@va_cle_cause_out=cle_cause
	from EXP..FMC_GEN
	where numero = @vl_evtcause and
	      cle = @vl_clecause

        /* construction de la localisation */
	select @vl_pt_car=point_caracteristique, @vl_num_pt_car=numero_point_caracteristique
	  from EXP..FMC_HIS_DER where numero = @vl_evtcause and cle = @vl_clecause
	
	if @vl_pt_car = XDC_POINT_CARACT_AIRE
	begin
	  select @va_type_loc_out = XDC_POINT_CARACT_AIRE
	  select @va_num_loc_out =  @vl_num_pt_car
          select @va_nom_loc_out = nom from CFG..RES_AIR where numero=@vl_num_pt_car
	end
	else if @vl_pt_car = XDC_POINT_CARACT_ECHANGEUR
	begin
	  /* les bifurcations sont typees en base comme des echangeurs reste 99 modulo 100 (99,199,299...)*/
	  if @vl_num_pt_car%100 = 99 select @va_type_loc_out =XDC_POINT_CARACT_BIFURCATION
	  else select @va_type_loc_out =XDC_POINT_CARACT_ECHANGEUR
	    select @va_nom_loc_out = nom from CFG..RES_ECH where numero=@vl_num_pt_car
	    select @va_num_loc_out =  @vl_num_pt_car
	end
	else
	begin
	  select @va_type_loc_out = 0
	  select @va_nom_loc_out = nom from CFG..RES_AUT where numero=@va_autoroute_out
	  select @va_num_loc_out =  0 
	end

	return XDC_OK
go

