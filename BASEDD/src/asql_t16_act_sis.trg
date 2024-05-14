/*E*/
/*  Fichier : $Id: asql_t16_act_sis.trg,v 1.3 1996/12/06 16:47:40 verdier Exp $      Release : $Revision: 1.3 $        Date : $Date: 1996/12/06 16:47:40 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER asql_t16_act_sis.trg
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 
* trigger sur la table ACT_SIS
*
------------------------------------------------------
* HISTORIQUE :
*
* B.G.  25/01/95        : Creation
* C.T.  28/03/95        : Ajout de la zone de balisage (V 1.2)
* JMG	22/11/96	: ajout annulation_intervention 1.3
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* 
* anime l'ihm
*
* Code retour
* 
* n≈ant
*
* Conditions d'utilisation
* base EXP 
---------------------------------------------------*/

use EXP
go

drop trigger act_sis_iu_trg
go

create trigger act_sis_iu_trg
on ACT_SIS
for insert,update
as

/*A D≈claration des variables locales */

	declare @vl_action int,
		@vl_cleact tinyint,
		@vl_typesyn tinyint,
		@vl_evt int,
		@vl_cleevt tinyint,
		@vl_horodeb datetime,
		@vl_horofin datetime,
		@vl_HorodateDebutBal datetime,
		@vl_HorodateFinBal datetime,
		@vl_HoroFinEvt datetime,
		@vl_autoroute tinyint,
		@vl_PR int,
		@vl_PRDebut int,
		@vl_PRFin int,
		@vl_Sens tinyint,
		@vl_TypPtCar tinyint,
		@vl_PtCar tinyint,
		@vl_PosPtCar tinyint,
		@vl_PoseBalisage bit,
		@vl_VR tinyint,
		@vl_VM2 tinyint,
		@vl_VM1 tinyint,
		@vl_VL tinyint,
		@vl_BAU tinyint,
		@vl_Suppr bit,
		@vl_Annulation tinyint

/*A Recuperation des valeurs */

	select 
		@vl_action = actio,
		@vl_cleact = sit,
		@vl_PoseBalisage = pose_balisage,
		@vl_HorodateDebutBal = debut_balisage,
		@vl_HorodateFinBal = fin_balisage,
		@vl_PRDebut = PR_debut,
		@vl_PRFin = PR_fin,
		@vl_Annulation = annulation_intervention
	from inserted
	
	select  @vl_typesyn=XZAHC_Patrouille
	
	
	select  @vl_evt=evenement,
		@vl_cleevt=cle,
		@vl_horodeb=heure_succes,
		@vl_horofin=heure_fin
	from	EXP..ACT_GEN
	where	numero=@vl_action and
		sit=@vl_cleact
	
	select	@vl_autoroute=autoroute,
		@vl_PR=PR,
		@vl_Sens=sens,
		@vl_TypPtCar=point_caracteristique,
		@vl_PtCar=numero_point_caracteristique,
		@vl_PosPtCar=position_sur_aire_ou_echangeur,
		@vl_VR = VR,
		@vl_VM2 = VM2,
		@vl_VM1 = VM1,
		@vl_VL = VL,
		@vl_BAU = BAU
	from	EXP..FMC_HIS
	where	numero=@vl_evt and
		cle=@vl_cleevt
	having	numero=@vl_evt and
		cle=@vl_cleevt and
		horodate_validation=max(horodate_validation)
		
	select  @vl_Suppr=XDC_VRAI
	
	if @vl_horodeb<>null and @vl_horofin=null
	select @vl_Suppr=XDC_FAUX
	
	exec PRC..XZAH;12	@vl_evt,
				@vl_cleevt,
				@vl_action,
				@vl_cleact,
				@vl_typesyn,
				@vl_autoroute,
				@vl_PR,
				@vl_Sens,
				XZAHC_VoieInconnue,
				@vl_TypPtCar,
				@vl_PtCar,
				@vl_PosPtCar,
				@vl_Suppr

	/*A ajout de la zone de balisage */
	if @vl_PoseBalisage = XDC_VRAI
	begin
		/* recherche date de fin de l'evenement */
		select @vl_HoroFinEvt = fin
		from EXP..FMC_GEN
		where numero = @vl_evt and cle = @vl_cleevt

		if @vl_HorodateFinBal is not null or
		   @vl_horofin is not null or
		   @vl_HoroFinEvt is not null
			select @vl_Suppr = XDC_VRAI
		else	select @vl_Suppr = XDC_FAUX

		/* ajout ou suppression de voies neutralisees uniquement */
		/* sur le sens de l'evenement                            */
		exec PRC..XZAH;17	@vl_evt,
				@vl_cleevt,
				@vl_autoroute,
				@vl_PRDebut,
				@vl_PRFin,
				@vl_Sens,
				@vl_VR,
				@vl_VM2,
				@vl_VM1,
				@vl_VL,
				@vl_BAU,
				XDC_VOIE_SENS_NORMAL,	-- pas d'animation sur voies inverses
				XDC_VOIE_SENS_NORMAL,
				XDC_VOIE_SENS_NORMAL,
				XDC_VOIE_SENS_NORMAL,
				XDC_VOIE_BAU_NORMALE,
				@vl_Suppr
	end
go
