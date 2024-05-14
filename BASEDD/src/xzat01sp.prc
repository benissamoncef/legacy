/*E*/
/*  Fichier : $Id: xzat01sp.prc,v 1.20 2013/02/26 09:28:44 devgfi Exp $      Release : $Revision: 1.20 $       Date : $Date: 2013/02/26 09:28:44 $
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
* C.T.	23/09/94	: Creation
* C.T.	29/09/94	: Modif interface de fonction
* C.T.	10/11/94	: Modif bugs applix (V 1.3)
* B.G.	06/01/95	: ajout order by (V 1.4)
* B.G.	06/01/95	: ajout horodate (V 1.5)
* B.G.	06/01/95	: caractere bizarre vuepad (V 1.6)
* C.T.	10/01/95	: Ajout d'une date dans l'interface
*                         pour la formation (V 1.7)
* B.G.  28/01/95        : modif pb sccs (1.8)
* F.V.  02/06/95        : modif tri retour select (1.9)
* F.V.  02/06/95        : modif tri retour select (1.10)
* B.G.  14/09/95	: optimisation de la requete avec table tempo (1.11)
* C.T.  19/02/96	: modif recherche de la disponibilite :
*                         recherche dans EQT_ACV si critere positionne (1.12)
* C.T.  27/02/96	: correction recherche de la disponibilite (1.13)
* C.P.	23/06/98	: Modif separation EXP et HIS (V 1.16)
* P.N	11/06/99	: Correction vl_Horodate -> @vl_Horodate pour HIS (v1.117)
* JMG	15/05/00	: optimisation mode formation
* JPL	25/02/13	: Support et conversion date NULLE en entree  (pour DEM 1061)  1.20
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

create procedure XZAT;01
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
		@vl_SiteGestion tinyint

	/*! transfo en param…tres nulls bug applix */
	if @va_District_in = 0 select @va_District_in = null
	if @va_Type_in = 0 select @va_Type_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

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
		select 	@va_Type_in, #DISPO.equipement,
			CFG..EQT_GEN.nom,
			CFG..EQT_GEN.autoroute,
			CFG..EQT_GEN.PR,
			CFG..EQT_GEN.sens,
			XDC_EQT_MINEUR * mineure + XDC_EQT_MAJEUR * majeure +
				XDC_EQT_CRITIQUE * critique +
				XDC_EQT_HS * HS + XDC_EQT_INHIBE * inhibe +
				XDC_EQT_DESACTIVE * desactive +
				XDC_EQT_A_DESACTIVER * a_desactiver +
				XDC_EQT_A_REACTIVER * activable, #DISPO.horodate,
			CFG..EQT_GEN.site_gestion
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
		select 	@va_Type_in, #DISPO.equipement,
			CFG..EQT_GEN.nom,
			CFG..EQT_GEN.autoroute,
			CFG..EQT_GEN.PR,
			CFG..EQT_GEN.sens,
			XDC_EQT_MINEUR * mineure + XDC_EQT_MAJEUR * majeure +
				XDC_EQT_CRITIQUE * critique +
				XDC_EQT_HS * HS + XDC_EQT_INHIBE * inhibe +
				XDC_EQT_DESACTIVE * desactive, #DISPO.horodate,
			CFG..EQT_GEN.site_gestion
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
			EXP..EQT_DSP.horodate,
			CFG..EQT_GEN.site_gestion
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
			EXP..EQT_DSP.horodate,
			CFG..EQT_GEN.site_gestion
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
				autoroute tinyint, PR int, sens tinyint, etat int, horodate datetime,
				site_gestion tinyint)

	create table #EQT_DSP2(type tinyint, equipement smallint,horodate datetime,
				mineure bit, majeure bit, critique bit, HS bit,
				inhibe bit, desactive bit, dernier bit, sit tinyint)

	/* Rechercher les equipements */
	declare Pointeur_eqt cursor
	for select distinct type, numero, nom, autoroute, PR, sens, site_gestion
	from CFG..EQT_GEN
	where (@va_Type_in is null or CFG..EQT_GEN.type = @va_Type_in )

	open Pointeur_eqt
	/* rechercher les etats des equipments concernes */
	fetch Pointeur_eqt into @vl_Type, @vl_Eqt, @vl_nom,
				@vl_autoroute, @vl_PR, @vl_sens,
				@vl_SiteGestion
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
			horodate,
			@vl_SiteGestion
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
				@vl_autoroute, @vl_PR, @vl_sens, @vl_SiteGestion
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

