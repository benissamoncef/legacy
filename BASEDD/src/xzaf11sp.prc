/*E*/
/*  Fichier : $Id: xzaf11sp.prc,v 1.4 1995/11/02 11:53:44 gaborit Exp $      Release : $Revision: 1.4 $        Date : $Date: 1995/11/02 11:53:44 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzaf11sp.prc
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
* C.T.	02/02/95	: Modif select des evenements (V 1.2)
* C.T.	07/02/95	: Modif select du PR de localisation (V 1.3)
* B.G.	24/10/95	: gestion du sens inverse A57 (1.4)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAF11_Animer_Bouchons_Inverses
* 
* Arguments en entree
* 
* Arguments en sortie
* 
* Liste retournee :
* XDY_Entier	evenement_basc
* XDY_Octet 	cle_basc
* XDY_Entier	evenement_bou
* XDY_Octet 	cle_bou
* XDY_Mot	portion
* XDY_Entier	PR
* XDY_Octet	sens
* XDY_Octet	voie
* XDY_Entier	translation
* XDY_Entier	extension
* XDY_Octet	niveau
*
* Service rendu
* Prepare l'animation IHM pour la formation
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

create procedure  XZAF;11
as
/*A D≈claration des variables locales */
declare @vl_NumEvt		int,
	@vl_CleEvt		tinyint,
	@vl_NumTete		int,
	@vl_BascBou		bit,
	@vl_CleTete		tinyint,
	@vl_Type		smallint,
	@vl_Autoroute		T_AUTOROUTE,
	@vl_PRdeb		T_PR,
	@vl_PRfin		T_PR,
	@vl_PRInter		T_PR,
	@vl_Sens		T_SENS,
	@vl_Niveau		tinyint,
	@vl_Horodate		datetime,
	@vl_HoroDebut		datetime,
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
		
	create table #SYN_BAS_SEG
	(
		evenement_basc	int,
		cle_basc	tinyint,
		evenement_bou	int,
		cle_bou		tinyint,
		portion		smallint,
		PR		int,
		sens		tinyint,
		voie		tinyint,
		translation	int,
		extension	int,
		niveau		tinyint
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
	where	spid = @@spid and 
		(type = XZAEC_FMC_Basculement or
		type = XZAEC_FMC_QueueBouchon) and
		(sens = XDC_SENS_NORD or sens = XDC_SENS_SUD)

	open Pointeur_evt
	fetch Pointeur_evt into @vl_NumEvt, @vl_CleEvt, @vl_Type,
				@vl_Autoroute, @vl_PRdeb, @vl_Sens,
				@vl_VR, @vl_VM2, @vl_VM1, @vl_VL,
				@vl_BAU, @vl_VR_I, @vl_VM2_I, @vl_VM1_I, 
				@vl_VL_I, @vl_BAU_I
	while @@sqlstatus = 0
	begin
		/* recherche le PR de fin */
		if @vl_Type = XZAEC_FMC_Basculement
		begin
       			select @vl_PRfin=localisation_PR_fin
			from HIS..FMC_BAS
			where numero=@vl_NumEvt and cle=@vl_CleEvt

			select @vl_BascBou = XZAHC_Basculement
			select  @vl_Niveau=null
		end
		else
		begin
			/* recherche le PR de fin */
			select  @vl_NumTete=cause,
				@vl_CleTete=cle_cause
			from HIS..FMC_GEN
			where numero=@vl_NumEvt and cle=@vl_CleEvt

			select  
				@vl_PRfin = PR,  
				@vl_Horodate = horodate_validation
			from EVT_FRM
			where	numero=@vl_NumTete and
				cle=@vl_CleTete and
				spid = @@spid

			if @@rowcount<>1 return XDC_NOK

			/* recherche le niveau de trafic */
			select  @vl_Niveau=isnull(type,1)
			from HIS..FMC_BOU
			where   numero=@vl_NumEvt and cle=@vl_CleEvt and
				horodate = @vl_Horodate

			if @@rowcount<>1 return XDC_NOK

			select @vl_BascBou = XZAHC_Bouchon

			/*B si le sens est nord ou sur A57, inverse les PR de d≈but et */
			/*B de fin pour avoir debut<fin */
			if @vl_PRdeb>@vl_PRfin
			begin
				select @vl_PRInter = @vl_PRdeb
				select @vl_PRdeb = @vl_PRfin
				select @vl_PRfin = @vl_PRInter
			end
		end

		/* calcul des voies radt inversees */
		exec @vl_Status = XZAF;12 @vl_NumEvt, @vl_CleEvt, @vl_Type, @vl_Niveau,
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
				@vl_VR, @vl_VM2, @vl_VM1, @vl_VL,
				@vl_BAU, @vl_VR_I, @vl_VM2_I, @vl_VM1_I, 
				@vl_VL_I, @vl_BAU_I
	end
	close Pointeur_evt

	/* retourner la liste des voies neutalisees */
	select 
		evenement_basc,
		cle_basc,
		evenement_bou,
		cle_bou,
		portion,
		PR,
		sens,
		voie,
		translation,
		extension,
		niveau
	from #SYN_BAS_SEG

	return XDC_OK
go
