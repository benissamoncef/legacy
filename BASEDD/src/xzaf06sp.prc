/*E*/
/*  Fichier : $Id: xzaf06sp.prc,v 1.2 1995/06/01 18:52:42 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/06/01 18:52:42 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzaf06sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm pour la formation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	10/01/95	: Creation 
* C.T.	29/05/95	: Ajout des zones de balisage des 
*                         actions SIS (V 1.2)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAF06_Animer_Voies_Neutralisees
* 
* Arguments en entree
* XDY_Horodate  va_Horodate_in
* 
* Arguments en sortie
* 
* Liste retournee :
* XDY_Entier	evenement,
* XDY_Octet 	cle,
* XDY_Mot	portion,
* XDY_PR	PR,
* XDY_Octet	sens,
* XDY_Octet	voie,
* XDY_Entier	translation
* XDY_Entier	extension
*
* Service rendu
* Prepare l'animation IHM
*
* Code retour
* XDC_OK
* XDC_NOK	
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
*
---------------------------------------------------*/

use PRC
go

create procedure  XZAF;06
	@va_Horodate_in datetime = null
as

/*A DÅclaration des variables locales */
declare @vl_NumEvt		int,
	@vl_CleEvt		tinyint,
	@vl_Site		tinyint,
	@vl_NumAction		int,
	@vl_Type		smallint,
	@vl_Autoroute		T_AUTOROUTE,
	@vl_PRdeb		T_PR,
	@vl_PRfin		T_PR,
	@vl_Sens		T_SENS,
	@vl_Horodate		datetime,
	@vl_HoroDebut		datetime,
	@vl_HorodateDebutBal	datetime,
	@vl_HorodateFinBal	datetime,
	@vl_PoseBalisage	bit,
	@vl_VR			T_VOIE,
	@vl_VM2			T_VOIE,
	@vl_VM1			T_VOIE,
	@vl_VL			T_VOIE,
	@vl_BAU			T_VOIE,
	@vl_VR_I		T_VOIE,
	@vl_VM2_I		T_VOIE,
	@vl_VM1_I		T_VOIE,
	@vl_VL_I		T_VOIE,
	@vl_BAU_I		T_VOIE,
	@vl_Status		int
		

	/* table de stockage des voies neutralisees */
	create table #SYN_NEU_SEG
	(
		evenement	int,
		cle		tinyint,
		portion		smallint,
		PR		int,
		sens		tinyint,
		voie		tinyint,
		translation	int,
		extension	int
	)

	/*A Recherche des evenements en cours a la date precisee */
	declare Pointeur_evt cursor
	for select 
		numero,
		cle,
		type,
		autoroute,
		PR,
		sens,
		horodate_validation,
		VR,
		VM2,
		VM1,
		VL,
		BAU,
		VR_I,
		VM2_I,
		VM1_I,
		VL_I,
		BAU_I
	from EVT_FRM
	where	spid = @@spid and (type = XZAEC_FMC_Travaux or
		type = XZAEC_FMC_Basculement) and
		(sens = XDC_SENS_NORD or sens = XDC_SENS_SUD)

	open Pointeur_evt
	fetch Pointeur_evt into @vl_NumEvt, @vl_CleEvt, @vl_Type,
				@vl_Autoroute, @vl_PRdeb, @vl_Sens,
				@vl_Horodate, @vl_VR, @vl_VM2, @vl_VM1, @vl_VL,
				@vl_BAU, @vl_VR_I, @vl_VM2_I, @vl_VM1_I, 
				@vl_VL_I, @vl_BAU_I
	while @@sqlstatus = 0
	begin
		/* recherche le PR de fin */
		if @vl_Type = XZAEC_FMC_Travaux
        		select @vl_PRfin=localisation_tete
			from HIS..FMC_TRH
			where numero=@vl_NumEvt and cle=@vl_CleEvt and
				horodate = @vl_Horodate

		else if @vl_Type = XZAEC_FMC_Basculement
        		select @vl_PRfin=localisation_PR_fin
			from HIS..FMC_BAS
			where numero=@vl_NumEvt and cle=@vl_CleEvt

		if @@rowcount<>1 return XDC_NOK

		/*B calcul des voies neutalisees */
		exec @vl_Status = XZAF;07 @vl_NumEvt, @vl_CleEvt, 
				@vl_Autoroute, @vl_PRdeb, @vl_PRfin, @vl_Sens,
				@vl_VR, @vl_VM2, @vl_VM1, @vl_VL,
				@vl_BAU, @vl_VR_I, @vl_VM2_I, @vl_VM1_I, 
				@vl_VL_I, @vl_BAU_I

		if @vl_Status = null
			return XDC_PRC_INC
		else if @vl_Status != XDC_OK
			return XDC_NOK

		/* passage a l'evenement suivant */
		fetch Pointeur_evt into @vl_NumEvt, @vl_CleEvt, @vl_Type,
				@vl_Autoroute, @vl_PRdeb, @vl_Sens,
				@vl_Horodate, @vl_VR, @vl_VM2, @vl_VM1, @vl_VL,
				@vl_BAU, @vl_VR_I, @vl_VM2_I, @vl_VM1_I, 
				@vl_VL_I, @vl_BAU_I
	end
	close Pointeur_evt

	/*A Recherche des actions de balisage SIS en cours a la date precisee */
	declare Pointeur_act cursor
	for select
		numero,
		sit,
		evenement,
		cle
	from ACT_FRM
	where spid = @@spid and type = XDC_ACT_PAT_SIS

	open Pointeur_act
	fetch Pointeur_act into @vl_NumAction, @vl_Site,
				@vl_NumEvt, @vl_CleEvt
	while @@sqlstatus = 0
	begin
		select
			@vl_PoseBalisage = pose_balisage,
			@vl_HorodateDebutBal = debut_balisage,
			@vl_HorodateFinBal = fin_balisage,
			@vl_PRdeb = PR_debut,
			@vl_PRfin = PR_fin
		from HIS..ACT_SIS
		where actio = @vl_NumAction and sit = @vl_Site

		/*B ajout de la zone de balisage si elle existe */
		if @vl_PoseBalisage = XDC_VRAI and
		   @vl_HorodateFinBal > @va_Horodate_in and
		   @vl_HorodateDebutBal <= @va_Horodate_in
		begin
			/* recherche les informations sur l'evenement associe */
			select
				@vl_Autoroute = autoroute,
				@vl_Sens = sens,
				@vl_VR = VR,
				@vl_VM2 = VM2,
				@vl_VM1 = VM1,
				@vl_VL = VL,
				@vl_BAU = BAU
			from EVT_FRM
			where numero = @vl_NumEvt and cle = @vl_CleEvt and
				spid = @@spid
	
			/* ajout de la zone de balisage */
			exec PRC..XZAF;07       @vl_NumEvt,
						@vl_CleEvt,
						@vl_Autoroute,
						@vl_PRdeb,
						@vl_PRfin,
						@vl_Sens,
						@vl_VR,
						@vl_VM2,
						@vl_VM1,
						@vl_VL,
						@vl_BAU,
						XDC_VOIE_SENS_NORMAL,
						XDC_VOIE_SENS_NORMAL,
						XDC_VOIE_SENS_NORMAL,
						XDC_VOIE_SENS_NORMAL,
						XDC_VOIE_BAU_NORMALE
		end

		/* passage a l'action SIS suivante */
		fetch Pointeur_act into @vl_NumAction, @vl_Site,
					@vl_NumEvt, @vl_CleEvt
	end


	/* retourner la liste des voies neutalisees */
	select 
		evenement,
		cle,
		portion,
		PR,
		sens,
		voie,
		translation,
		extension
	from #SYN_NEU_SEG

	return XDC_OK
go
