/*E*/
/*  Fichier : @(#)xzae127sp.prc	1.6      Release : 1.6        Date : 03/19/99
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae127sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Recherche des infos appartenant au bloc obligatoire
* d'un fichier STRADA.
------------------------------------------------------
* HISTORIQUE :
*
* Vathelot  12/05/97        : Creation
* Guilhou 02/12/97	: ajout zz et sens en sortie 1.4
* Guilhou	23/12/98 : Modif pour interg 1.5
* Niepceron	04/03/1999 : Recuperation trafic et gravite dans ACT_EXP_ADA v1.6
* F.Lizot       07/05/2004 : Ajout de la reference datex (SAGA) v1.7
* Niepceron	06/09/07   : Ajout PHR#LSx DEM701
* Niepceron	05/06/08   : Correction concat LNP;correction sens A57 DEM701
* Niepceron	10/07/12   : Correction LNP : C -> H v1.10
* Niepceron	26/02/13   : Une FMC en fin n'est pas en prévision v1.11
* JMG		22/04/15 : linux chaines sortie 1.12
* Niepceron	20/02/16   : forcage de la date de validation a la date courante pour eviter les latences 1.13 DEM1216
* Niepceron	28/08/19   : plafonne la duree de FMC si durée > 20 ans  1.14 DEM1358
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE127_Infos_obligatoires
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
drop proc XZAE127
go

create procedure XZAE127
	@va_numero_in		int	= null,
	@va_cle_in		tinyint = null,
	@va_debut_out 		datetime output,
	@va_fin_out		datetime output,
	@va_fausse_alerte_out	char(1)	output,
	@va_duree_out		int output,
	@va_fin_evt_out		char(1) output,
	@va_validation_out	datetime output,
	@va_numero_cause_out 	int output, 
	@va_cle_cause_out 	tinyint output, 
	@va_type_cause_out 	char(25) output,
	@va_veracite_out	tinyint	output,
	@va_prevision_out	char(1) output,
	@va_localisation_out	char(5) output,
	@va_zz_out		int output,
	@va_sens_out		tinyint output,
	@va_pr_out		int output,
	@va_autoroute_out	tinyint output,
	@va_trafic_out		tinyint output,
	@va_gravite_out		tinyint output,
	@va_nbvoiesop_out	tinyint output,
        @va_reference_out       char(35) output /* Ajout SAGA */
as
declare
	@vl_numero 		int,
	@vl_cle			tinyint,
	@vl_fausse_alerte	tinyint,
	@vl_cloture		datetime,
	@vl_debut_prevu		datetime,
	@vl_fin_prevue		datetime,
	@vl_confirme_ou_signale	bit,
	@vl_type_cause		smallint,
	@vl_BAU			tinyint,
	@vl_VL			tinyint,
	@vl_VM1			tinyint,
	@vl_VM2			tinyint,
	@vl_VR			tinyint,
	@vl_local 		char(5),
	@vl_baubloq 		char(2),
	@vl_inv			smallint
	
	if @va_numero_in is null
		return XDC_ARG_INV
		
	if @va_cle_in is null
		return XDC_ARG_INV
	
	select @va_fausse_alerte_out="",@va_fin_evt_out="",@va_type_cause_out="",@va_prevision_out="",@va_localisation_out="",@va_reference_out=""	
	/*A Recherche les infos associees a un evt ds FMC_GEN */
	select 	@vl_numero = numero, 
		@vl_cle = cle, 
		@va_debut_out = debut,
		@va_fin_out = fin,
		@vl_fin_prevue = fin_prevue,
		@vl_fausse_alerte = fausse_alerte,
		@vl_cloture = cloture,
		@va_numero_cause_out = cause, 
		@va_cle_cause_out = cle_cause, 
		@vl_debut_prevu = debut_prevu,
		@va_zz_out = zz
	from EXP..FMC_GEN 
	where 
		numero = @va_numero_in and
		cle = @va_cle_in 

	select @va_zz_out = count(*) 
	from EXP..FMC_HIS
	where numero = @va_numero_in and
		cle = @va_cle_in

	if @va_numero_cause_out != null
	begin
		select @va_type_cause_out = nom
		from CFG..TYP_FMC
		where CFG..TYP_FMC.numero = @vl_type_cause
	end

	/* On recherche les infos de FMC_HIS */
	select	@va_validation_out = getdate(),
		@vl_confirme_ou_signale = confirme_ou_signale,
		@vl_BAU = BAU,
		@vl_VL = VL,
		@vl_VM1 = VM1,
		@vl_VM2 = VM2,
		@vl_VR = VR,
		@va_sens_out = sens,
		@va_pr_out = PR,
		@va_autoroute_out = autoroute
	from EXP..FMC_HIS
	where
		numero = @va_numero_in and
		cle = @va_cle_in 
	group by
		numero, cle
	having
		horodate_validation = max(horodate_validation)
		
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
	
	/* on calcule la duree si on a une heure de fin */
	select @va_duree_out = 3600
	if (@vl_fin_prevue is not null) and (@va_debut_out is not null)
	begin
		select @va_duree_out = 3600
		if datediff (year,@va_debut_out, @vl_fin_prevue) > 20
			select @va_duree_out = 32000
		else
		begin
		  if datediff (second, @va_debut_out, @vl_fin_prevue) > 32000
			select @va_duree_out = 32000
		  else
			select @va_duree_out = datediff (second, @va_debut_out, @vl_fin_prevue)
		end
	end
	else
	begin
		select @va_duree_out = 3600
		if datediff (year,@va_debut_out,getdate() ) > 20
		        select @va_duree_out = 32000
		else
		begin
		  if datediff (second, @va_debut_out, getdate()) > 32000
			select @va_duree_out = 32000
		  else
			select @va_duree_out = datediff (second, @va_debut_out, getdate())
		end
	end
	/* on forme le champ fausse alerte */
	select @va_fausse_alerte_out = "N"
	if @vl_fausse_alerte = 1
		select @va_fausse_alerte_out = "Y"
	
	/* on forme le champ fin evt */
	select @va_fin_evt_out = "N"
	if @va_fin_out is not null 
		select @va_fin_evt_out = "Y"
		
	/* on forme le champ veracite */
	if @vl_confirme_ou_signale = 0
		select @va_veracite_out = 5
	else
		select @va_veracite_out = 1
	
	/* on forme le champ prevision */
	select @va_prevision_out = "N"
	if (@vl_debut_prevu is not null and @va_debut_out is null and @va_fin_out is null)
	begin
		select @va_prevision_out = "Y"	,
			@va_debut_out=@vl_debut_prevu
        end 	
	/*on forme le champ localisation*/
	select @vl_local="",@va_localisation_out=""

	select @vl_baubloq="*"

	if @vl_BAU = XDC_VOIE_NEUTRALISEE or
	   @vl_BAU = XDC_VOIE_BLOQUEE_NEUTRALISEE or
	   @vl_BAU = XDC_VOIE_BLOQUEE or
	   @vl_BAU = XDC_VOIE_SENS_INVERSE
        begin
		select @vl_local = rtrim(@va_localisation_out) + "H"
		select @va_localisation_out = @vl_local
		select @vl_baubloq="H*"
	end

	if @vl_VL = XDC_VOIE_NEUTRALISEE or
	   @vl_VL = XDC_VOIE_BLOQUEE_NEUTRALISEE or
	   @vl_VL = XDC_VOIE_BLOQUEE or
	   @vl_VL = XDC_VOIE_SENS_INVERSE
	begin
		select @vl_local = rtrim(@va_localisation_out) + "1"
		select @va_localisation_out = @vl_local
	end

	if @vl_VM1 = XDC_VOIE_NEUTRALISEE or
	   @vl_VM1 = XDC_VOIE_BLOQUEE_NEUTRALISEE or
	   @vl_VM1 = XDC_VOIE_BLOQUEE or
	   @vl_VM1 = XDC_VOIE_SENS_INVERSE
	begin
		select @vl_local = rtrim(@va_localisation_out) + "2"
		select @va_localisation_out = @vl_local
	end

	if @vl_VM2 = XDC_VOIE_NEUTRALISEE or
	   @vl_VM2 = XDC_VOIE_BLOQUEE_NEUTRALISEE or
	   @vl_VM2 = XDC_VOIE_BLOQUEE or
	   @vl_VM2 = XDC_VOIE_SENS_INVERSE
	begin
		select @vl_local = rtrim(@va_localisation_out) + "3"
		select @va_localisation_out = @vl_local
	end

	if @vl_VR = XDC_VOIE_NEUTRALISEE or
	   @vl_VR = XDC_VOIE_BLOQUEE_NEUTRALISEE or
	   @vl_VR = XDC_VOIE_BLOQUEE or
	   @vl_VR = XDC_VOIE_SENS_INVERSE
	begin
		if @vl_VM1 = XDC_VOIE_INEXISTANTE 
		begin
			select @vl_local = rtrim(@va_localisation_out) + "2"
			select @va_localisation_out = @vl_local
		end
		else 
		begin
			if @vl_VM2 = XDC_VOIE_INEXISTANTE
			begin
				select @vl_local = rtrim(@va_localisation_out) + "3"
				select @va_localisation_out = @vl_local
			end
			else
			begin
				select @vl_local = rtrim(@va_localisation_out) + "4"
				select @va_localisation_out = @vl_local
			end
		end
	end

	select @va_nbvoiesop_out=0
	if @vl_VR = XDC_VOIE_SENS_NORMAL
		select @va_nbvoiesop_out=@va_nbvoiesop_out+1
	if @vl_VM1 = XDC_VOIE_SENS_NORMAL
		select @va_nbvoiesop_out=@va_nbvoiesop_out+1
	if @vl_VM2 =XDC_VOIE_SENS_NORMAL
		select @va_nbvoiesop_out=@va_nbvoiesop_out+1
	if @vl_VL = XDC_VOIE_SENS_NORMAL
		select @va_nbvoiesop_out=@va_nbvoiesop_out+1

	/* si tout est bloqué alors * ou H* */
	if @va_nbvoiesop_out=0
		select @va_localisation_out =@vl_baubloq
       select
	       @va_trafic_out=trafic,
	       @va_gravite_out=gravite
       from EXP..ACT_EXP_ADA, EXP..ACT_GEN
       where evenement=@va_numero_in and
	       cle = @va_cle_in and 
	       type=XDC_ACT_EXP and
	       heure_lancement <> null and 
	       heure_fin=null and
	       actio=numero and
	       EXP..ACT_EXP_ADA.sit=EXP..ACT_GEN.sit

	/* Ajout SAGA                                              */
	/* Recherche d'une eventuelle reference DATEX dans ADA_SIT */
	select @va_reference_out=null
	select @va_reference_out=reference from EXP..ADA_SIT where evenement=@va_numero_in and cle=@va_cle_in

	select @va_fausse_alerte_out,@va_fin_evt_out,@va_type_cause_out,@va_prevision_out,@va_localisation_out,@va_reference_out
	return XDC_OK

go

