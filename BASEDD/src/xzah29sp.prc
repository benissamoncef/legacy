/*E*/
/*  Fichier : $Id: xzah29sp.prc,v 1.3 2020/11/06 09:55:53 devgtie Exp $      Release : $Revision: 1.3 $        Date : $Date: 2020/11/06 09:55:53 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah29sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 
* animation des synoptiques
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.  02/04/95        : Creation
* C.T.  05/04/95        : Ajout du type XZAHC_Intervention (V 1.2)
* RGR   06/11/20	: Retrait d'un commentaire emettant un warning lors de la precompil
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* 
* anime les actions liees a un evenement modifie 
*
* Code retour
* XDC_OK
* XDC_NOK
* 
* n≈ant
*
* Conditions d'utilisation
---------------------------------------------------*/

use PRC
go

create procedure XZAH;29
	@va_Evt_in 		int,
	@va_Cle_in 		tinyint,
	@va_HoroFinEvt_in	datetime,
	@va_Autoroute_in 	tinyint,
	@va_PR_in 		int,
	@va_Sens_in 		tinyint,
	@va_TypPtCar_in		tinyint,
	@va_PtCar_in 		tinyint,
	@va_PosPtCar_in		tinyint,
	@va_VL_in 		tinyint,
	@va_VM1_in 		tinyint,
	@va_VM2_in 		tinyint,
	@va_VR_in 		tinyint,
	@va_BAU_in 		tinyint
as
	/*A D≈claration des variables locales */
	declare @vl_Action int,
		@vl_Site T_SITE,
		@vl_HoroSucces 	datetime,
		@vl_HoroFinAct	datetime,
		@vl_TypeSyn tinyint,
		@vl_Type tinyint,
		@vl_HorodateDebutBal datetime,
		@vl_HorodateFinBal datetime,
		@vl_PRDebut int,
		@vl_PRFin int,
		@vl_PoseBalisage bit,
		@vl_Suppr bit

	/*A rechercher des actions liees a l'evenement */
	declare Pointeur_actions cursor
	for select
		numero,
		sit,
		type,
		heure_succes,
		heure_fin
	from EXP..ACT_GEN
	where	evenement = @va_Evt_in and cle = @va_Cle_in and
		(type = XDC_ACT_INTERVENTION or type = XDC_ACT_PAT_SIS or
		 type = XDC_ACT_DEPANNEUR)

	open Pointeur_actions

	fetch Pointeur_actions into @vl_Action, @vl_Site, @vl_Type,
				@vl_HoroSucces, @vl_HoroFinAct
	while @@sqlstatus = 0
	begin
		/* recherche le type de l'objet synoptique a modifier */
		if @vl_Type = XDC_ACT_INTERVENTION
		begin
			select @vl_Type = type
			from EXP..ACT_INT
			where actio = @vl_Action and sit = @vl_Site

			if @vl_Type=XDC_ACT_INT_GND
				select  @vl_TypeSyn = XZAHC_Gendarme
			else if @vl_Type=XDC_ACT_INT_POM
				select  @vl_TypeSyn = XZAHC_Pompier 
			else	select  @vl_TypeSyn = XZAHC_Intervention -- par defaut
		end
		else if @vl_Type = XDC_ACT_PAT_SIS
		begin
			/* rechercher les infos sur le balisage s'il existe */
			select 
				@vl_PoseBalisage = pose_balisage,
				@vl_HorodateDebutBal = debut_balisage,
				@vl_HorodateFinBal = fin_balisage,
				@vl_PRDebut = PR_debut,
				@vl_PRFin = PR_fin
			from EXP..ACT_SIS
			where actio = @vl_Action and sit = @vl_Site

			/*B ajout de la zone de balisage */
			if @vl_PoseBalisage = XDC_VRAI
			begin
				if @vl_HorodateFinBal is not null or
				   @vl_HoroFinAct is not null or
				   @va_HoroFinEvt_in is not null
					select @vl_Suppr = XDC_VRAI
				else	select @vl_Suppr = XDC_FAUX
		
				/* modification ou suppression de voies neutralisees uniquement */
				/* sur le sens de l'evenement                                   */
				exec PRC..XZAH;17	@va_Evt_in,
						@va_Cle_in,
						@va_Autoroute_in,
						@vl_PRDebut,
						@vl_PRFin,
						@va_Sens_in,
						@va_VR_in,
						@va_VM2_in,
						@va_VM1_in,
						@va_VL_in,
						@va_BAU_in,
						XDC_VOIE_SENS_NORMAL,
						XDC_VOIE_SENS_NORMAL,
						XDC_VOIE_SENS_NORMAL,
						XDC_VOIE_SENS_NORMAL,
						XDC_VOIE_BAU_NORMALE,
						@vl_Suppr
			end

			/* type de l'objet */
			select  @vl_TypeSyn = XZAHC_Patrouille

		end
		else if @vl_Type = XDC_ACT_DEPANNEUR
			select  @vl_TypeSyn=XZAHC_Depanneur
	
		if @vl_HoroSucces<>null and @vl_HoroFinAct=null
			select @vl_Suppr=XDC_FAUX
		else	select  @vl_Suppr=XDC_VRAI
	
		/*B modification de l'objet mobile */
		exec PRC..XZAH;12	@va_Evt_in,
					@va_Cle_in,
					@vl_Action,
					@vl_Site,
					@vl_TypeSyn,
					@va_Autoroute_in,
					@va_PR_in,
					@va_Sens_in,
					XZAHC_VoieInconnue,
					@va_TypPtCar_in,
					@va_PtCar_in,
					@va_PosPtCar_in,
					@vl_Suppr

		/* passage a l'action suivante */
		fetch Pointeur_actions into @vl_Action, @vl_Site, @vl_Type,
					@vl_HoroSucces, @vl_HoroFinAct
	end

	close Pointeur_actions

	return XDC_OK
go
