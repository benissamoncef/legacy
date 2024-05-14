/*E*/
/*  Fichier : @(#)xzat001sp.prc	1.17      Release : 1.17        Date : 05/15/00
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzat01sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des etats d'un equipement 
*
------------------------------------------------------
* HISTORIQUE :
*

* P.N	28/11/05	: Creation
* LCL   09/07/07        : Suppression DY 
* JMG   02/10/09        : secto DEM887
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.4
* ABE	06/01/21	: MOVIS ajout PRA DEM-SAE93 1.5
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAT01_Liste_Eqt_Dispo
* 
* Arguments en entr≈e
* XDY_TypeEqt	va_Type_in :		par d≈faut tous les types
* XDY_District	va_District_in :	par d≈faut tous les districts
* 
* XZATT_Dispo	va_AjoutDispo_in :	ou entre les valeurs :
* 				XDC_EQT_MINEUR
* 				XDC_EQT_MAJEUR
* 				XDC_EQT_CRITIQUE
* 				XDC_EQT_HS
* 				XDC_EQT_INHIBE
* 				XDC_EQT_DESACTIVE
* 				XDC_EQT_AREACTIVER
* 				XDC_EQT_ADESACTIVER
* 
* XZATT_Dispo	va_AjoutNonDispo_in :	ou entre les valeurs :
* 				XDC_EQT_NON_MINEUR
* 				XDC_EQT_NON_MAJEUR
* 				XDC_EQT_NON_CRITIQUE
* 				XDC_EQT_NON_HS
* 				XDC_EQT_NON_INHIBE
* 				XDC_EQT_NON_DESACTIVE
* 				XDC_EQT_NON_AREACTIVER
* 				XDC_EQT_NON_ADESACTIVER
* XZATT_Dispo	va_RetraitDispo_in :	ou entre les valeurs 
* XZATT_Dispo	va_RetraitNonDispo_in :	ou entre les valeurs 
* XDY_Horodate	va_Horodate_in :	date 
* 
* Arguments en sortie
* XZATT_Liste		ListeEqts
* 
* Service rendu
* Retourne la liste des ≈quipements du district et du type pr≈cis≈, 
* ayant la disponibilit≈ pr≈cis≈e (plusieurs valeurs possibles).
* 
* Code retour
* XDC_OK
* XDC_NOK
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* code de ATRP18 si pb a son appel
* 
* Conditions d'utilisation
* Pr≈ciser au moins un crit…re Type ou Dispo pour ne pas avoir une 
* taille trop importante de requete.
* AjoutDispo et AjoutNonDispo permettent de selectionner les
* equipements ayant la disponibilite AjoutDispo ou AjoutNonDispo
* et RetraitDispo et RetraitNonDispo enlevent de la liste formee
* les equipements ayant la disponibilite RetraitDispo ou RetraitNonDispo
* 
* Fonction
* select from EQUIPEMENT where crit…res
------------------------------------------------------------*/

use PRC
go
drop proc XZAT02
go

create procedure XZAT02
	@va_Type_in		tinyint = null,
	@va_District_in		T_DISTRICT = null,
	@va_AjoutDispo_in	int = null,
	@va_AjoutNonDispo_in	int = null,
	@va_RetraitDispo_in	int = null,
	@va_RetraitNonDispo_in	int = null,
	@va_Horodate_in		datetime = null
as
	declare @vl_Eqt T_EQUIPEMENT, @vl_Type tinyint, 
		@vl_Horodate datetime,
		@vl_AjoutCritique bit, @vl_AjoutHS bit, 
		@vl_AjoutMajeure bit, @vl_AjoutMineure bit, 
		@vl_AjoutInhibe bit, @vl_AjoutDesactive bit, 
		@vl_AjoutADesactiver bit, @vl_AjoutAReactiver bit,
		@vl_AjoutNonMineure bit, @vl_AjoutNonCritique bit, 
		@vl_AjoutNonHS bit, @vl_AjoutNonMajeure bit,
		@vl_AjoutNonInhibe bit, @vl_AjoutNonDesactive bit, 
		@vl_AjoutNonADesactiver bit, @vl_AjoutNonAReactiver bit,
		@vl_RetraitCritique bit, @vl_RetraitHS bit, @vl_RetraitMajeure bit, 
		@vl_RetraitMineure bit, @vl_RetraitInhibe bit, @vl_RetraitDesactive bit, 
		@vl_RetraitADesactiver bit, @vl_RetraitAReactiver bit,
		@vl_RetraitNonMineure bit, @vl_RetraitNonCritique bit, 
		@vl_RetraitNonHS bit, @vl_RetraitNonMajeure bit,
		@vl_RetraitNonInhibe bit, @vl_RetraitNonDesactive bit, 
		@vl_RetraitNonADesactiver bit, @vl_RetraitNonAReactiver bit,
		@vl_autoroute tinyint,
		@vl_PR int,
		@vl_sens tinyint,
		@vl_nom char(10),
		@vl_Status int,
		@vl_serveur_pra char(6)

	/*! transfo en param…tres nulls bug applix */
	if @va_District_in = 0 select @va_District_in = null
	if @va_Type_in = 0 select @va_Type_in = null

	/*A controle le parametre obligatoire */
	if @va_AjoutDispo_in = null or @va_AjoutNonDispo_in = null or
	   @va_RetraitDispo_in = null or @va_RetraitNonDispo_in = null
		return XDC_ARG_INV

	/*A positionnement des etats a tester */
	select	@vl_AjoutMineure	= (@va_AjoutDispo_in /XDC_EQT_MINEUR) % 2,
		@vl_AjoutMajeure	= (@va_AjoutDispo_in /XDC_EQT_MAJEUR) % 2,
		@vl_AjoutCritique	= (@va_AjoutDispo_in /XDC_EQT_CRITIQUE) % 2,
		@vl_AjoutHS		= (@va_AjoutDispo_in /XDC_EQT_HS) % 2,
		@vl_AjoutInhibe		= (@va_AjoutDispo_in /XDC_EQT_INHIBE) % 2,
		@vl_AjoutDesactive	= (@va_AjoutDispo_in /XDC_EQT_DESACTIVE) % 2,
		@vl_AjoutADesactiver	= (@va_AjoutDispo_in /XDC_EQT_A_DESACTIVER) % 2,
		@vl_AjoutAReactiver	= (@va_AjoutDispo_in /XDC_EQT_A_REACTIVER) % 2,
		@vl_AjoutNonMajeure	= (@va_AjoutNonDispo_in /XDC_EQT_MAJEUR) % 2,
		@vl_AjoutNonMineure	= (@va_AjoutNonDispo_in /XDC_EQT_MINEUR) % 2,
		@vl_AjoutNonCritique	= (@va_AjoutNonDispo_in /XDC_EQT_CRITIQUE) % 2,
		@vl_AjoutNonHS		= (@va_AjoutNonDispo_in /XDC_EQT_HS) % 2,
		@vl_AjoutNonInhibe	= (@va_AjoutNonDispo_in /XDC_EQT_INHIBE) % 2,
		@vl_AjoutNonDesactive	= (@va_AjoutNonDispo_in /XDC_EQT_DESACTIVE) % 2,
		@vl_AjoutNonADesactiver	= (@va_AjoutNonDispo_in /XDC_EQT_A_DESACTIVER) % 2,
		@vl_AjoutNonAReactiver	= (@va_AjoutNonDispo_in /XDC_EQT_A_REACTIVER) % 2,
		@vl_RetraitMineure	= (@va_RetraitDispo_in /XDC_EQT_MINEUR) % 2,
		@vl_RetraitMajeure	= (@va_RetraitDispo_in /XDC_EQT_MAJEUR) % 2,
		@vl_RetraitCritique	= (@va_RetraitDispo_in /XDC_EQT_CRITIQUE) % 2,
		@vl_RetraitHS		= (@va_RetraitDispo_in /XDC_EQT_HS) % 2,
		@vl_RetraitInhibe	= (@va_RetraitDispo_in /XDC_EQT_INHIBE) % 2,
		@vl_RetraitDesactive	= (@va_RetraitDispo_in /XDC_EQT_DESACTIVE) % 2,
		@vl_RetraitADesactiver	= (@va_RetraitDispo_in /XDC_EQT_A_DESACTIVER) % 2,
		@vl_RetraitAReactiver	= (@va_RetraitDispo_in /XDC_EQT_A_REACTIVER) % 2,
		@vl_RetraitNonMajeure	= (@va_RetraitNonDispo_in /XDC_EQT_MAJEUR) % 2,
		@vl_RetraitNonMineure	= (@va_RetraitNonDispo_in /XDC_EQT_MINEUR) % 2,
		@vl_RetraitNonCritique	= (@va_RetraitNonDispo_in /XDC_EQT_CRITIQUE) % 2,
		@vl_RetraitNonHS	= (@va_RetraitNonDispo_in /XDC_EQT_HS) % 2,
		@vl_RetraitNonInhibe	= (@va_RetraitNonDispo_in /XDC_EQT_INHIBE) % 2,
		@vl_RetraitNonDesactive	= (@va_RetraitNonDispo_in /XDC_EQT_DESACTIVE) % 2,
		@vl_RetraitNonADesactiver	= (@va_RetraitNonDispo_in /XDC_EQT_A_DESACTIVER) % 2,
		@vl_RetraitNonAReactiver	= (@va_RetraitNonDispo_in /XDC_EQT_A_REACTIVER) % 2

   /*A liste des equipements de tous les districts */
#ifndef HIST

   if @va_Horodate_in is null
   begin
	if (@va_District_in != XDC_CI and @@servername="SQL_CI")
	begin
		/* Init serveur PRA */
		select @vl_serveur_pra=serveur_pra from CFG..RES_PRA where numero=@va_Site

		/* if @vl_Site = XDC_DP*/
		if @vl_serveur_pra = XDC_SQLDP
			exec @vl_Status = SQL_DS.PRC..XZAT02 
				@va_Type_in,@va_District_in,@va_AjoutDispo_in,@va_AjoutNonDispo_in,
				@va_RetraitDispo_in,@va_RetraitNonDispo_in,@va_Horodate_in
		/*else if @vl_Site = XDC_VC*/
		else if @vl_serveur_pra = XDC_SQLVC
			exec @vl_Status = SQL_DM.PRC..XZAT02 
				@va_Type_in,@va_District_in,@va_AjoutDispo_in,@va_AjoutNonDispo_in,
				@va_RetraitDispo_in,@va_RetraitNonDispo_in,@va_Horodate_in
		else if @vl_serveur_pra = XDC_SQLCI
			exec @vl_Status = SQL_CI.PRC..XZAT02
				@va_Type_in,@va_District_in,@va_AjoutDispo_in,@va_AjoutNonDispo_in,
				@va_RetraitDispo_in,@va_RetraitNonDispo_in,@va_Horodate_in

		else return XDC_NOK

		if @vl_Status = null
			return XDC_PRC_INC
		else return @vl_Status
	end

      if @va_Type_in is not null
      begin
	create table #DISPO (
				equipement	smallint,
				mineure		bit,
				majeure		bit,
				critique	bit,
				HS		bit,
				inhibe		bit,
				desactive	bit,
				horodate	datetime)
	
	insert #DISPO 
		select	equipement,
			mineure,
			majeure,
			critique,
			HS,
			inhibe,
			desactive,
			horodate
		from	EXP..EQT_DSP
		where	type=@va_Type_in
		  and	dernier=XDC_OUI
		  and	(@va_District_in is null or sit = @va_District_in)
			

	delete  #DISPO
	where	 (@vl_RetraitMineure = XDC_OUI and mineure = XDC_OUI) or
		 (@vl_RetraitMajeure = XDC_OUI and majeure = XDC_OUI) or
		 (@vl_RetraitCritique = XDC_OUI and critique = XDC_OUI) or
		 (@vl_RetraitInhibe = XDC_OUI and inhibe = XDC_OUI) or
		 (@vl_RetraitHS = XDC_OUI and HS = XDC_OUI) or
		 (@vl_RetraitDesactive = XDC_OUI and desactive = XDC_OUI) or
		 (@vl_RetraitNonMineure = XDC_OUI and mineure = XDC_NON) or
		 (@vl_RetraitNonMajeure = XDC_OUI and majeure = XDC_NON) or
		 (@vl_RetraitNonCritique = XDC_OUI and critique = XDC_NON) or
		 (@vl_RetraitNonInhibe = XDC_OUI and inhibe = XDC_NON) or
		 (@vl_RetraitNonHS = XDC_OUI and HS = XDC_NON) or
		 (@vl_RetraitNonDesactive = XDC_OUI and desactive = XDC_NON)

	if @vl_RetraitNonAReactiver = XDC_OUI or @vl_RetraitADesactiver = XDC_OUI or
	   @vl_RetraitAReactiver = XDC_OUI or @vl_RetraitNonADesactiver = XDC_OUI or
	   @vl_AjoutNonAReactiver = XDC_OUI or @vl_AjoutADesactiver = XDC_OUI or
	   @vl_AjoutAReactiver = XDC_OUI or @vl_AjoutNonADesactiver = XDC_OUI 
		select 	@va_Type_in,
			#DISPO.equipement,
			CFG..EQT_GEN.nom,
			CFG..EQT_GEN.autoroute,
			CFG..EQT_GEN.PR,
			CFG..EQT_GEN.sens,
			XDC_EQT_MINEUR * mineure + XDC_EQT_MAJEUR * majeure +
				XDC_EQT_CRITIQUE * critique +
				XDC_EQT_HS * HS + XDC_EQT_INHIBE * inhibe +
				XDC_EQT_DESACTIVE * desactive +
				XDC_EQT_A_DESACTIVER * a_desactiver +
				XDC_EQT_A_REACTIVER * activable,
			#DISPO.horodate
		from	#DISPO, CFG..EQT_GEN, EXP..EQT_ACV
		where	CFG..EQT_GEN.numero = #DISPO.equipement and
			EXP..EQT_ACV.equipement = #DISPO.equipement and
			EXP..EQT_ACV.type = CFG..EQT_GEN.type and
			CFG..EQT_GEN.type = @va_Type_in and
			((@vl_RetraitAReactiver = XDC_NON or activable = XDC_NON) and
			 (@vl_RetraitADesactiver = XDC_NON or a_desactiver = XDC_NON) and
			 (@vl_RetraitNonAReactiver = XDC_NON or activable = XDC_OUI) and
			 (@vl_RetraitNonADesactiver = XDC_NON or a_desactiver = XDC_OUI)) and
			((@vl_AjoutMineure = XDC_OUI and mineure = XDC_OUI) or
			 (@vl_AjoutMajeure = XDC_OUI and majeure = XDC_OUI) or
			 (@vl_AjoutCritique = XDC_OUI and critique = XDC_OUI) or
			 (@vl_AjoutInhibe = XDC_OUI and inhibe = XDC_OUI) or
			 (@vl_AjoutHS = XDC_OUI and HS = XDC_OUI) or
			 (@vl_AjoutDesactive = XDC_OUI and desactive = XDC_OUI) or
			 (@vl_AjoutAReactiver = XDC_OUI and activable = XDC_OUI) or
			 (@vl_AjoutADesactiver = XDC_OUI and a_desactiver = XDC_OUI) or
			 (@vl_AjoutNonMineure = XDC_OUI and mineure = XDC_NON) or
			 (@vl_AjoutNonMajeure = XDC_OUI and majeure = XDC_NON) or
			 (@vl_AjoutNonCritique = XDC_OUI and critique = XDC_NON) or
			 (@vl_AjoutNonInhibe = XDC_OUI and inhibe = XDC_NON) or
			 (@vl_AjoutNonHS = XDC_OUI and HS = XDC_NON) or
			 (@vl_AjoutNonDesactive = XDC_OUI and desactive = XDC_NON) or
			 (@vl_AjoutNonAReactiver = XDC_OUI and activable = XDC_NON) or
			 (@vl_AjoutNonADesactiver = XDC_OUI and a_desactiver = XDC_NON))
		order by CFG..EQT_GEN.autoroute,CFG..EQT_GEN.PR desc,#DISPO.equipement
	else
		select 	@va_Type_in,
			#DISPO.equipement,
			CFG..EQT_GEN.nom,
			CFG..EQT_GEN.autoroute,
			CFG..EQT_GEN.PR,
			CFG..EQT_GEN.sens,
			XDC_EQT_MINEUR * mineure + XDC_EQT_MAJEUR * majeure +
				XDC_EQT_CRITIQUE * critique +
				XDC_EQT_HS * HS + XDC_EQT_INHIBE * inhibe +
				XDC_EQT_DESACTIVE * desactive,
			#DISPO.horodate
		from	#DISPO, CFG..EQT_GEN
		where	CFG..EQT_GEN.numero = #DISPO.equipement and
			CFG..EQT_GEN.type = @va_Type_in and
			((@vl_AjoutMineure = XDC_OUI and mineure = XDC_OUI) or
			 (@vl_AjoutMajeure = XDC_OUI and majeure = XDC_OUI) or
			 (@vl_AjoutCritique = XDC_OUI and critique = XDC_OUI) or
			 (@vl_AjoutInhibe = XDC_OUI and inhibe = XDC_OUI) or
			 (@vl_AjoutHS = XDC_OUI and HS = XDC_OUI) or
			 (@vl_AjoutDesactive = XDC_OUI and desactive = XDC_OUI) or
			 (@vl_AjoutNonMineure = XDC_OUI and mineure = XDC_NON) or
			 (@vl_AjoutNonMajeure = XDC_OUI and majeure = XDC_NON) or
			 (@vl_AjoutNonCritique = XDC_OUI and critique = XDC_NON) or
			 (@vl_AjoutNonInhibe = XDC_OUI and inhibe = XDC_NON) or
			 (@vl_AjoutNonHS = XDC_OUI and HS = XDC_NON) or
			 (@vl_AjoutNonDesactive = XDC_OUI and desactive = XDC_NON))
		order by CFG..EQT_GEN.autoroute,CFG..EQT_GEN.PR desc,#DISPO.equipement

      end
      else
      begin
	if @vl_RetraitNonAReactiver = XDC_OUI or @vl_RetraitADesactiver = XDC_OUI or
	   @vl_RetraitAReactiver = XDC_OUI or @vl_RetraitNonADesactiver = XDC_OUI or
	   @vl_AjoutNonAReactiver = XDC_OUI or @vl_AjoutADesactiver = XDC_OUI or
	   @vl_AjoutAReactiver = XDC_OUI or @vl_AjoutNonADesactiver = XDC_OUI 
		select  
			EXP..EQT_DSP.type,
			EXP..EQT_DSP.equipement,
			CFG..EQT_GEN.nom,
			CFG..EQT_GEN.autoroute,
			CFG..EQT_GEN.PR,
			CFG..EQT_GEN.sens,
			XDC_EQT_MINEUR * EXP..EQT_DSP.mineure + XDC_EQT_MAJEUR * EXP..EQT_DSP.majeure + 
				XDC_EQT_CRITIQUE * EXP..EQT_DSP.critique +
				XDC_EQT_HS * EXP..EQT_DSP.HS + XDC_EQT_INHIBE * EXP..EQT_DSP.inhibe + 
				XDC_EQT_DESACTIVE * EXP..EQT_DSP.desactive +
				XDC_EQT_A_DESACTIVER * EXP..EQT_ACV.a_desactiver + 
				XDC_EQT_A_REACTIVER * EXP..EQT_ACV.activable,
			EXP..EQT_DSP.horodate
		from EXP..EQT_DSP, CFG..EQT_GEN, EXP..EQT_ACV
		where	EXP..EQT_DSP.dernier = XDC_OUI and
			(@va_District_in is null or EXP..EQT_DSP.sit = @va_District_in) and
			((@vl_RetraitMineure = XDC_NON or EXP..EQT_DSP.mineure = XDC_NON) and
			 (@vl_RetraitMajeure = XDC_NON or EXP..EQT_DSP.majeure = XDC_NON) and
			 (@vl_RetraitCritique = XDC_NON or EXP..EQT_DSP.critique = XDC_NON) and
			 (@vl_RetraitInhibe = XDC_NON or EXP..EQT_DSP.inhibe = XDC_NON) and
			 (@vl_RetraitHS = XDC_NON or EXP..EQT_DSP.HS = XDC_NON) and
			 (@vl_RetraitDesactive = XDC_NON or EXP..EQT_DSP.desactive = XDC_NON) and
			 (@vl_RetraitAReactiver = XDC_NON or EXP..EQT_ACV.activable = XDC_NON) and
			 (@vl_RetraitADesactiver = XDC_NON or EXP..EQT_ACV.a_desactiver = XDC_NON) and
			 (@vl_RetraitNonMineure = XDC_NON or EXP..EQT_DSP.mineure = XDC_OUI) and
			 (@vl_RetraitNonMajeure = XDC_NON or EXP..EQT_DSP.majeure = XDC_OUI) and
			 (@vl_RetraitNonCritique = XDC_NON or EXP..EQT_DSP.critique = XDC_OUI) and
			 (@vl_RetraitNonInhibe = XDC_NON or EXP..EQT_DSP.inhibe = XDC_OUI) and
			 (@vl_RetraitNonHS = XDC_NON or EXP..EQT_DSP.HS = XDC_OUI) and
			 (@vl_RetraitNonDesactive = XDC_NON or EXP..EQT_DSP.desactive = XDC_OUI) and
			 (@vl_RetraitNonAReactiver = XDC_NON or EXP..EQT_ACV.activable = XDC_OUI) and
			 (@vl_RetraitNonADesactiver = XDC_NON or EXP..EQT_ACV.a_desactiver = XDC_OUI)) and
			((@vl_AjoutMineure = XDC_OUI and EXP..EQT_DSP.mineure = XDC_OUI) or
			 (@vl_AjoutMajeure = XDC_OUI and EXP..EQT_DSP.majeure = XDC_OUI) or
			 (@vl_AjoutCritique = XDC_OUI and EXP..EQT_DSP.critique = XDC_OUI) or
			 (@vl_AjoutInhibe = XDC_OUI and EXP..EQT_DSP.inhibe = XDC_OUI) or
			 (@vl_AjoutHS = XDC_OUI and EXP..EQT_DSP.HS = XDC_OUI) or
			 (@vl_AjoutDesactive = XDC_OUI and EXP..EQT_DSP.desactive = XDC_OUI) or
			 (@vl_AjoutAReactiver = XDC_OUI and EXP..EQT_ACV.activable = XDC_OUI) or
			 (@vl_AjoutADesactiver = XDC_OUI and EXP..EQT_ACV.a_desactiver = XDC_OUI) or
			 (@vl_AjoutNonMineure = XDC_OUI and EXP..EQT_DSP.mineure = XDC_NON) or
			 (@vl_AjoutNonMajeure = XDC_OUI and EXP..EQT_DSP.majeure = XDC_NON) or
			 (@vl_AjoutNonCritique = XDC_OUI and EXP..EQT_DSP.critique = XDC_NON) or
			 (@vl_AjoutNonInhibe = XDC_OUI and EXP..EQT_DSP.inhibe = XDC_NON) or
			 (@vl_AjoutNonHS = XDC_OUI and EXP..EQT_DSP.HS = XDC_NON) or
			 (@vl_AjoutNonDesactive = XDC_OUI and EXP..EQT_DSP.desactive = XDC_NON) or
			 (@vl_AjoutNonAReactiver = XDC_OUI and EXP..EQT_ACV.activable = XDC_NON) or
			 (@vl_AjoutNonADesactiver = XDC_OUI and EXP..EQT_ACV.a_desactiver = XDC_NON)) and
			EXP..EQT_ACV.sit = EXP..EQT_DSP.sit and
			CFG..EQT_GEN.numero = EXP..EQT_DSP.equipement and
			EXP..EQT_ACV.equipement = EXP..EQT_DSP.equipement and
			EXP..EQT_ACV.type = EXP..EQT_DSP.type and
			CFG..EQT_GEN.type = EXP..EQT_DSP.type
		order by EXP..EQT_DSP.type, CFG..EQT_GEN.autoroute, CFG..EQT_GEN.PR desc,EXP..EQT_DSP.equipement
	else
		select  
			EXP..EQT_DSP.type,
			EXP..EQT_DSP.equipement,
			CFG..EQT_GEN.nom,
			CFG..EQT_GEN.autoroute,
			CFG..EQT_GEN.PR,
			CFG..EQT_GEN.sens,
			XDC_EQT_MINEUR * EXP..EQT_DSP.mineure + XDC_EQT_MAJEUR * EXP..EQT_DSP.majeure + 
				XDC_EQT_CRITIQUE * EXP..EQT_DSP.critique +
				XDC_EQT_HS * EXP..EQT_DSP.HS + XDC_EQT_INHIBE * EXP..EQT_DSP.inhibe + 
				XDC_EQT_DESACTIVE * EXP..EQT_DSP.desactive,
			EXP..EQT_DSP.horodate
		from EXP..EQT_DSP, CFG..EQT_GEN, EXP..EQT_ACV
		where	EXP..EQT_DSP.dernier = XDC_OUI and
			(@va_District_in is null or EXP..EQT_DSP.sit = @va_District_in) and
			((@vl_RetraitMineure = XDC_NON or EXP..EQT_DSP.mineure = XDC_NON) and
			 (@vl_RetraitMajeure = XDC_NON or EXP..EQT_DSP.majeure = XDC_NON) and
			 (@vl_RetraitCritique = XDC_NON or EXP..EQT_DSP.critique = XDC_NON) and
			 (@vl_RetraitInhibe = XDC_NON or EXP..EQT_DSP.inhibe = XDC_NON) and
			 (@vl_RetraitHS = XDC_NON or EXP..EQT_DSP.HS = XDC_NON) and
			 (@vl_RetraitDesactive = XDC_NON or EXP..EQT_DSP.desactive = XDC_NON) and
			 (@vl_RetraitNonMineure = XDC_NON or EXP..EQT_DSP.mineure = XDC_OUI) and
			 (@vl_RetraitNonMajeure = XDC_NON or EXP..EQT_DSP.majeure = XDC_OUI) and
			 (@vl_RetraitNonCritique = XDC_NON or EXP..EQT_DSP.critique = XDC_OUI) and
			 (@vl_RetraitNonInhibe = XDC_NON or EXP..EQT_DSP.inhibe = XDC_OUI) and
			 (@vl_RetraitNonHS = XDC_NON or EXP..EQT_DSP.HS = XDC_OUI) and
			 (@vl_RetraitNonDesactive = XDC_NON or EXP..EQT_DSP.desactive = XDC_OUI)) and
			((@vl_AjoutMineure = XDC_OUI and EXP..EQT_DSP.mineure = XDC_OUI) or
			 (@vl_AjoutMajeure = XDC_OUI and EXP..EQT_DSP.majeure = XDC_OUI) or
			 (@vl_AjoutCritique = XDC_OUI and EXP..EQT_DSP.critique = XDC_OUI) or
			 (@vl_AjoutInhibe = XDC_OUI and EXP..EQT_DSP.inhibe = XDC_OUI) or
			 (@vl_AjoutHS = XDC_OUI and EXP..EQT_DSP.HS = XDC_OUI) or
			 (@vl_AjoutDesactive = XDC_OUI and EXP..EQT_DSP.desactive = XDC_OUI) or
			 (@vl_AjoutNonMineure = XDC_OUI and EXP..EQT_DSP.mineure = XDC_NON) or
			 (@vl_AjoutNonMajeure = XDC_OUI and EXP..EQT_DSP.majeure = XDC_NON) or
			 (@vl_AjoutNonCritique = XDC_OUI and EXP..EQT_DSP.critique = XDC_NON) or
			 (@vl_AjoutNonInhibe = XDC_OUI and EXP..EQT_DSP.inhibe = XDC_NON) or
			 (@vl_AjoutNonHS = XDC_OUI and EXP..EQT_DSP.HS = XDC_NON) or
			 (@vl_AjoutNonDesactive = XDC_OUI and EXP..EQT_DSP.desactive = XDC_NON)) and
			EXP..EQT_ACV.sit = EXP..EQT_DSP.sit and
			CFG..EQT_GEN.numero = EXP..EQT_DSP.equipement and
			EXP..EQT_ACV.equipement = EXP..EQT_DSP.equipement and
			EXP..EQT_ACV.type = EXP..EQT_DSP.type and
			CFG..EQT_GEN.type = EXP..EQT_DSP.type
		order by EXP..EQT_DSP.type,CFG..EQT_GEN.autoroute,CFG..EQT_GEN.PR desc, EXP..EQT_DSP.equipement
      end
   end
#else
   begin
	/* cas de la formation */

	create table #EQT_DSP1 (type tinyint, equipement smallint, nom char(10),
				autoroute tinyint, PR int, sens tinyint, etat int, horodate datetime)

	create table #EQT_DSP2(type tinyint, equipement smallint,horodate datetime,
				mineure bit, majeure bit, critique bit, HS bit,
				inhibe bit, desactive bit, dernier bit, sit tinyint)

	/* Rechercher les equipements */
	declare Pointeur_eqt cursor
	for select distinct type, numero, nom, autoroute, PR, sens
	from CFG..EQT_GEN
	where (@va_Type_in is null or CFG..EQT_GEN.type = @va_Type_in )

	open Pointeur_eqt
	/* rechercher les etats des equipments concernes */
	fetch Pointeur_eqt into @vl_Type, @vl_Eqt, @vl_nom,
				@vl_autoroute, @vl_PR, @vl_sens
	while (@@sqlstatus = 0)
	begin
		/* derniere date de disponibilite avant la date actuelle */
/*		select @vl_Horodate = max(horodate) from HIS..EQT_DSP
		where type = @vl_Type and equipement = @vl_Eqt and 
			horodate <= getdate()*/

		insert #EQT_DSP2
		select *
		from HIS..EQT_DSP
		where   HIS..EQT_DSP.equipement = @vl_Eqt and
			HIS..EQT_DSP.type = @vl_Type and
			HIS..EQT_DSP.horodate = (select max(horodate) from HIS..EQT_DSP
				where type = @vl_Type and equipement = @vl_Eqt and
				horodate <= getdate())

		/* informations sur un equipement a la derniere date */
		insert #EQT_DSP1
		select
			@vl_Type,
			@vl_Eqt,
			@vl_nom,
			@vl_autoroute,
			@vl_PR,
			@vl_sens,
			XDC_EQT_MINEUR * mineure + 
				XDC_EQT_MAJEUR * majeure + 
				XDC_EQT_CRITIQUE * critique +
				XDC_EQT_HS * HS + 
				XDC_EQT_INHIBE * inhibe + 
				XDC_EQT_DESACTIVE * desactive,
			horodate
		from #EQT_DSP2
		where ((@vl_RetraitMineure = XDC_NON or #EQT_DSP2.mineure = XDC_NON) and
			 (@vl_RetraitMajeure = XDC_NON or #EQT_DSP2.majeure = XDC_NON) and
			 (@vl_RetraitCritique = XDC_NON or #EQT_DSP2.critique = XDC_NON) and
			 (@vl_RetraitInhibe = XDC_NON or #EQT_DSP2.inhibe = XDC_NON) and
			 (@vl_RetraitHS = XDC_NON or #EQT_DSP2.HS = XDC_NON) and
			 (@vl_RetraitDesactive = XDC_NON or #EQT_DSP2.desactive = XDC_NON) and
			 (@vl_RetraitNonMineure = XDC_NON or #EQT_DSP2.mineure = XDC_OUI) and
			 (@vl_RetraitNonMajeure = XDC_NON or #EQT_DSP2.majeure = XDC_OUI) and
			 (@vl_RetraitNonCritique = XDC_NON or #EQT_DSP2.critique = XDC_OUI) and
			 (@vl_RetraitNonInhibe = XDC_NON or #EQT_DSP2.inhibe = XDC_OUI) and
			 (@vl_RetraitNonHS = XDC_NON or #EQT_DSP2.HS = XDC_OUI) and
			 (@vl_RetraitNonDesactive = XDC_NON or #EQT_DSP2.desactive = XDC_OUI)) and
			((@vl_AjoutMineure = XDC_OUI and #EQT_DSP2.mineure = XDC_OUI) or
			 (@vl_AjoutMajeure = XDC_OUI and #EQT_DSP2.majeure = XDC_OUI) or
			 (@vl_AjoutCritique = XDC_OUI and #EQT_DSP2.critique = XDC_OUI) or
			 (@vl_AjoutInhibe = XDC_OUI and #EQT_DSP2.inhibe = XDC_OUI) or
			 (@vl_AjoutHS = XDC_OUI and #EQT_DSP2.HS = XDC_OUI) or
			 (@vl_AjoutDesactive = XDC_OUI and #EQT_DSP2.desactive = XDC_OUI) or
			 (@vl_AjoutNonMineure = XDC_OUI and #EQT_DSP2.mineure = XDC_NON) or
			 (@vl_AjoutNonMajeure = XDC_OUI and #EQT_DSP2.majeure = XDC_NON) or
			 (@vl_AjoutNonCritique = XDC_OUI and #EQT_DSP2.critique = XDC_NON) or
			 (@vl_AjoutNonInhibe = XDC_OUI and #EQT_DSP2.inhibe = XDC_NON) or
			 (@vl_AjoutNonHS = XDC_OUI and #EQT_DSP2.HS = XDC_NON) or
			 (@vl_AjoutNonDesactive = XDC_OUI and #EQT_DSP2.desactive = XDC_NON)) and
			(@va_District_in is null or #EQT_DSP2.sit = @va_District_in)	

		/* passage a l'equiepement suivant */
		fetch Pointeur_eqt into @vl_Type, @vl_Eqt, @vl_nom,
				@vl_autoroute, @vl_PR, @vl_sens
	end
	close Pointeur_eqt

	/* envoi des informations */
	select * from #EQT_DSP1 order by type, autoroute, PR desc, equipement

	drop table #EQT_DSP1
	drop table #EQT_DSP2
   end
#endif

   return XDC_OK

go

