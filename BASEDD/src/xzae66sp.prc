/*E*/
/*  Fichier : $Id: xzae66sp.prc,v 1.18 2010/08/31 12:06:49 gesconf Exp $        Release : $Revision: 1.18 $        Date : $Date: 2010/08/31 12:06:49 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae66sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.     	20/10/94	: Creation	V 1.1 
* B.G.          09/11/94        : modif null pour applix (1.2)
* C.T.          30/11/94        : ajout initialisation voies en sortie (1.4)
* B.G.          03/01/95        : modif voies retournees (largeur BAU) (1.6)
* B.G.          07/04/95        : suppression having (1.7)
* B.G.          01/06/95        : modif la clause de recherche ds les portions
*                                 (stricte en PR debut) (1.8)
* C.T.          15/02/96        : correction calcul PR sur peage (1.9)
* C.T.          13/11/96        : prise en compte de la bau etroite (1.10-1.11)
* C.T.          11/12/96        : suppression des traces (1.13)
* C.T.          05/03/98        : bau etroite sur echangeur (1.14) (dem 1325)
* P.NI          15/03/98        : Appel de xzao pour verif distric de recouv v1.15 dem439
* P.NI          26/04/98        : Correction pour evt sur bretelle v1.16 correction  dem439
* JPL		16/04/09        : Degroupage; test des Pr aux bornes de portions selon table portions (DEM 878) 1.17
* PNI		25/06/09        : District de recouvr: Ne pas prendre en compte le sens pour les FMC basculement et basculement pour travaux  (DEM 937) 1.18
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Indiquer si la localisation est valide, et la complete si 
* necessaire. Retourner la description des voies pour cette localisation
* 
* Sequence d'appel
* SP	XZAE66_Configuration_Voies
* 
* Arguments en entree
* XDY_Autoroute	va_NumAutoroute_in
* XDC_PR	va_PR_in
* XDC_Sens	va_Sens_in
* XDC_Octet	va_PtCaracteristique_in
* XDC_Octet	va_TypePtCaracteristique_in
* XDC_Octet	va_Position_in
* XDC_Octet	va_Type_in
* 
* Arguments en sortie
* XDY_Autoroute	va_NumAutoroute_out
* XDC_PR	va_PR_out
* XDC_Sens	va_Sens_out
* XDC_Octet	va_PtCaracteristique_out
* XDC_Octet	va_TypePtCaracteristique_out
* XDC_Octet	va_Position_out
* XDY_Voie	va_VR_out
* XDY_Voie	va_VM2_out
* XDY_Voie	va_VM1_out
* XDY_Voie	va_VL_out
* XDY_Voie	va_BAU_out
* XDY_Voie	va_VR_I_out
* XDY_Voie	va_VM2_I_out
* XDY_Voie	va_VM1_I_out
* XDY_Voie	va_VL_I_out
* XDY_Voie	va_BAU_I_out
* XDC_Octet	va_District_out
* XDC_Octet	va_DistrictRecouvr_out
* XDY_Entier	va_Resultat_out
* 
* Code retour
* XDC_OK
* XDC_NOK
* XZAEC_LOC_PT_CAR_INC	: point caracteristique inexistant
* XDC_ARG_INV		: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* - Tester la localisation et rechercher la localisation precise a partir :
* 	d'un echangeur
* 	d'un peage
* 	d'une aire
* 	d'une autoroute, PR, sens
* 	d'un lit d'arret
* - L'argument de sortie va_Resultat_out indique :
*	- bit 1 : le sens incorrect (XDC_LOC_SENS_MOD)
*	- bit 2 : le PR incorrect (XDC_LOC_PR_MOD)
*	- bit 3 : l'autoroute incorrect (XDC_LOC_AUT_MOD)
*	- bit 4 : la localisation incorrecte (XDC_LOC_INC)
* dans le cas de point caracteristique ou localisation incorrect la
* configuration des voies n'est pas donnee
* - Pour une aire ou un echangeur, la configuration des voies 
* inverses est valuee XDC_VOIE_INEXISTANTE (car n'existe pas)
* - Si le sens est incorrect ou non precise en entree et
* impossible a definir alors XDC_SENS_INCONNU est retourne pour le sens
* et aucune configuration de voie n'est renseignee 
* (arguments de sortie des voies a XDC_VOIE_INEXISTANTE)
* 
* Fonction
* Tester si la localisation est valide
* Si la localisation est valide alors chercher la description 
* des voies dans la table SEGMENT_HOMOGENE
----------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE66' and type = 'P')
	drop procedure XZAE66
go


create procedure XZAE66
	@va_NumAutoroute_in	T_AUTOROUTE 	= null,
	@va_PR_in		T_PR	 	= null,
	@va_Sens_in		T_SENS	 	= null,
	@va_PtCaracteristique_in	tinyint	= null,
	@va_TypePtCaracteristique_in	tinyint	= null,
	@va_Position_in		tinyint	 	= null,
	@va_Type_in		smallint	 = null,
	@va_NumAutoroute_out	T_AUTOROUTE 	= null	output,
	@va_PR_out		T_PR	 	= null	output,
	@va_Sens_out		T_SENS	 	= null	output,
	@va_PtCaracteristique_out	tinyint	= null	output,
	@va_TypePtCaracteristique_out	tinyint	= null	output,
	@va_Position_out	tinyint	 	= null	output,
	@va_VR_out		T_VOIE	 	= null	output,
	@va_VM2_out		T_VOIE	 	= null	output,
	@va_VM1_out		T_VOIE	 	= null	output,
	@va_VL_out		T_VOIE	 	= null	output,
	@va_BAU_out		T_VOIE	 	= null	output,
	@va_VR_I_out		T_VOIE	 	= null	output,
	@va_VM2_I_out		T_VOIE	 	= null	output,
	@va_VM1_I_out		T_VOIE	 	= null	output,
	@va_VL_I_out		T_VOIE	 	= null	output,
	@va_BAU_I_out		T_VOIE	 	= null	output,
	@va_BAU_Etroite_out	tinyint	 	= null	output,
	@va_BAU_I_Etroite_out	tinyint	 	= null	output,
	@va_District_out	tinyint	 	= null	output,
	@va_DistrictRecouvr_out	tinyint	 	= null	output,
	@va_Resultat_out	int	 	= null	output
as
	declare @vl_NbVoiesInverses tinyint, @vl_NbVoies tinyint,
		@vl_LargeurBAU tinyint, @vl_LargeurBAUInverse tinyint,
		@vl_Portion smallint, @vl_PR T_PR, @vl_PRN T_PR, @vl_PRS T_PR, 
		@vl_PREntree T_PR,
		@vl_PRSortie T_PR, @vl_PREntreeN T_PR, @vl_PRSortieN T_PR,
		@vl_PREntreeS T_PR, @vl_PRSortieS T_PR, @vl_NbVoiesEntN tinyint,
		@vl_NbVoiesEntS tinyint, @vl_NbVoiesSortN tinyint,
		@vl_NbVoiesSortS tinyint, @vl_PrDebut T_PR,
		@vl_Sens T_SENS,
		@vl_District tinyint,
		@vl_Districti_verifie tinyint

	/*! transfo en paramÉtres nulls bug applix */
	
	if @va_TypePtCaracteristique_in = 0 select @va_TypePtCaracteristique_in = null
	if @va_PtCaracteristique_in = 0 select @va_PtCaracteristique_in = null

	/* Controle des parametres d'entree */
	if @va_NumAutoroute_in = null or @va_PR_in = null or @va_Sens_in = null or
	   ((@va_NumAutoroute_in = XDC_AUT_INCONNUE or @va_PR_in = XDC_PR_INCONNU) and
	    (@va_TypePtCaracteristique_in = null or @va_PtCaracteristique_in = null))
	   	return XDC_ARG_INV

	/* initialisation */
	select	@va_Resultat_out = 0, 
		@va_VL_out = XDC_VOIE_INEXISTANTE,
		@va_VM2_out = XDC_VOIE_INEXISTANTE,
		@va_VM1_out = XDC_VOIE_INEXISTANTE,
		@va_VR_out = XDC_VOIE_INEXISTANTE,
		@va_BAU_out = XDC_VOIE_INEXISTANTE,
		@va_VL_I_out = XDC_VOIE_INEXISTANTE,
		@va_VM2_I_out = XDC_VOIE_INEXISTANTE,
		@va_VM1_I_out = XDC_VOIE_INEXISTANTE,
		@va_VR_I_out = XDC_VOIE_INEXISTANTE,
		@va_BAU_I_out = XDC_VOIE_INEXISTANTE,
		@va_PtCaracteristique_out = @va_PtCaracteristique_in,
		@va_TypePtCaracteristique_out = @va_TypePtCaracteristique_in,
		@va_Position_out = @va_Position_in

	/*A calcul de la localisation par rapport au point caracteristique */
	if @va_PtCaracteristique_in != null
	begin
		if @va_TypePtCaracteristique_in = XDC_POINT_CARACT_AIRE
		begin
			/* recherche la position de l'aire */
			select 
				@va_NumAutoroute_out	= CFG..RES_POR.autoroute,
				@vl_PREntree 		= CFG..RES_AIR.PRentree,
				@vl_PRSortie 		= CFG..RES_AIR.PRsortie,
				@va_Sens_out 		= CFG..RES_AIR.sens,
				@vl_Portion		= CFG..RES_POR.numero,
				@va_District_out	= CFG..RES_POR.district,
				@va_DistrictRecouvr_out = CFG..RES_POR.district_recouvrement
			from CFG..RES_AIR, CFG..RES_POR
			where	CFG..RES_AIR.numero = @va_PtCaracteristique_in and
				CFG..RES_POR.numero = CFG..RES_AIR.portion

			if @@rowcount != 1
				return XZAEC_LOC_PT_CAR_INC

			/* determiner le PR suivant le sens donne en entree */
			else if @va_Sens_in = XDC_SENS_INCONNU
			begin
				select	@va_PR_out = @vl_PREntree
				if @va_Sens_out = XDC_SENS_NORD
					select @va_Sens_out = XDC_SENS_ENTREE_NORD
				else	select @va_Sens_out = XDC_SENS_ENTREE_SUD
			end
			else if @va_Sens_in = XDC_SENS_ENTREE_NORD and 
				@va_Sens_out = XDC_SENS_NORD
			begin
				select @va_Sens_out = XDC_SENS_ENTREE_NORD,
					@va_PR_out = @vl_PREntree
			end
			else if @va_Sens_in = XDC_SENS_ENTREE_SUD and 
				@va_Sens_out = XDC_SENS_SUD
			begin
				select @va_Sens_out = XDC_SENS_ENTREE_SUD,
					@va_PR_out = @vl_PREntree
			end
			else if @va_Sens_in = XDC_SENS_SORTIE_NORD and 
				@va_Sens_out = XDC_SENS_NORD
			begin
				select @va_Sens_out = XDC_SENS_SORTIE_NORD,
					@va_PR_out = @vl_PRSortie
			end
			else if @va_Sens_in = XDC_SENS_SORTIE_SUD and 
				@va_Sens_out = XDC_SENS_SUD
			begin
				select @va_Sens_out = XDC_SENS_SORTIE_SUD,
					@va_PR_out = @vl_PRSortie
			end
			else if @va_Sens_in = XDC_SENS_SUD or @va_Sens_in = XDC_SENS_NORD
				select	@va_Sens_out =  @va_Sens_in,
					@va_PR_out = (	isnull(@vl_PREntree, @vl_PRSortie) + 
							isnull(@vl_PRSortie, @vl_PREntree)) /2
			else return XDC_ARG_INV

		end
		else if @va_TypePtCaracteristique_in = XDC_POINT_CARACT_ECHANGEUR
		begin
			/* position de l'echangeur */
			select 
				@va_NumAutoroute_out	= CFG..RES_POR.autoroute,
				@vl_PREntreeN 		= CFG..RES_ECH.PR_entree_sens2,
				@vl_PRSortieN 		= CFG..RES_ECH.PR_sortie_sens2,
				@vl_PREntreeS 		= CFG..RES_ECH.PR_entree_sens1,
				@vl_PRSortieS 		= CFG..RES_ECH.PR_sortie_sens1,
				@vl_PR	 		= CFG..RES_ECH.PR,
				@vl_NbVoiesEntN		= CFG..RES_ECH.voies_entree_sens2,
				@vl_NbVoiesEntS		= CFG..RES_ECH.voies_entree_sens1,
				@vl_NbVoiesSortN	= CFG..RES_ECH.voies_sortie_sens2,
				@vl_NbVoiesSortS	= CFG..RES_ECH.voies_sortie_sens1,
				@vl_Portion		= CFG..RES_POR.numero,
				@va_District_out	= CFG..RES_POR.district,
				@va_DistrictRecouvr_out = CFG..RES_POR.district_recouvrement
			from CFG..RES_ECH, CFG..RES_POR
			where	CFG..RES_ECH.numero = @va_PtCaracteristique_in and
				CFG..RES_POR.numero = CFG..RES_ECH.portion

			if @@rowcount != 1
				return XZAEC_LOC_PT_CAR_INC

			/* determiner le PR suivant le sens donne en entree */
			else if @va_Sens_in = XDC_SENS_INCONNU or 
				@va_Sens_in = XDC_SENS_SUD or
				@va_Sens_in = XDC_SENS_NORD
			begin
				select	@va_PR_out = @vl_PR,
					@va_Sens_out = @va_Sens_in
			end
			else if @va_Sens_in = XDC_SENS_ENTREE_NORD 
			begin
				if @vl_NbVoiesEntN = 0
				begin
					if @vl_NbVoiesSortN != 0
						select  @va_PR_out = @vl_PRSortieN,
							@va_Sens_out = XDC_SENS_SORTIE_NORD
					else if @vl_NbVoiesEntS !=0
						select  @va_PR_out = @vl_PREntreeS,
							@va_Sens_out = XDC_SENS_ENTREE_SUD
					else if @vl_NbVoiesSortS !=0
						select  @va_PR_out = @vl_PRSortieS,
							@va_Sens_out = XDC_SENS_SORTIE_SUD
					else select @va_Resultat_out = XDC_LOC_INC

				end
				else	select @va_Sens_out = XDC_SENS_ENTREE_NORD,
						@va_PR_out = @vl_PREntreeN

			end
			else if @va_Sens_in = XDC_SENS_ENTREE_SUD 
			begin
				if @vl_NbVoiesEntS = 0
				begin
					if @vl_NbVoiesSortS != 0
						select  @va_PR_out = @vl_PRSortieS,
							@va_Sens_out = XDC_SENS_SORTIE_SUD
					else if @vl_NbVoiesEntN !=0
						select  @va_PR_out = @vl_PREntreeN,
							@va_Sens_out = XDC_SENS_ENTREE_NORD
					else if @vl_NbVoiesSortN !=0
						select  @va_PR_out = @vl_PRSortieN,
							@va_Sens_out = XDC_SENS_SORTIE_NORD
					else select @va_Resultat_out = XDC_LOC_INC
				end
				else	select	@va_Sens_out = XDC_SENS_ENTREE_SUD,
						@va_PR_out = @vl_PREntreeS
			end
			else if @va_Sens_in = XDC_SENS_SORTIE_NORD 
			begin
				if @vl_NbVoiesSortN = 0
				begin
					if @vl_NbVoiesEntN != 0
						select  @va_PR_out = @vl_PREntreeN,
							@va_Sens_out = XDC_SENS_ENTREE_NORD
					else if @vl_NbVoiesSortS !=0
						select  @va_PR_out = @vl_PRSortieS,
							@va_Sens_out = XDC_SENS_SORTIE_SUD
					else if @vl_NbVoiesEntS !=0
						select  @va_PR_out = @vl_PREntreeS,
							@va_Sens_out = XDC_SENS_ENTREE_SUD
					else select @va_Resultat_out = XDC_LOC_INC
				end
				else	select @va_Sens_out = XDC_SENS_SORTIE_NORD,
						@va_PR_out = @vl_PRSortieN
			end
			else if @va_Sens_in = XDC_SENS_SORTIE_SUD
			begin
				if @vl_NbVoiesSortS = 0
				begin
					if @vl_NbVoiesEntS != 0
						select  @va_PR_out = @vl_PREntreeS,
							@va_Sens_out = XDC_SENS_ENTREE_SUD
					else if @vl_NbVoiesSortN !=0
						select  @va_PR_out = @vl_PRSortieN,
							@va_Sens_out = XDC_SENS_SORTIE_NORD
					else if @vl_NbVoiesEntN !=0
						select  @va_PR_out = @vl_PREntreeN,
							@va_Sens_out = XDC_SENS_ENTREE_NORD
					else select @va_Resultat_out = XDC_LOC_INC
				end
				else	select	@va_Sens_out = XDC_SENS_SORTIE_SUD,
						@va_PR_out = @vl_PRSortieS
			end
			else return XDC_ARG_INV
		end
		else if @va_TypePtCaracteristique_in = XDC_POINT_CARACT_PEAGE
		begin
			/* position du peage */
			select 
				@va_NumAutoroute_out	= CFG..RES_POR.autoroute,
				@vl_PRN 		= CFG..RES_PEA.PRsens2,
				@vl_PRS 		= CFG..RES_PEA.PRsens1,
				@vl_Portion		= CFG..RES_POR.numero,
				@va_District_out	= CFG..RES_POR.district,
				@va_DistrictRecouvr_out = CFG..RES_POR.district_recouvrement
			from CFG..RES_PEA, CFG..RES_POR
			where	CFG..RES_PEA.numero = @va_PtCaracteristique_in and
				CFG..RES_POR.numero = CFG..RES_PEA.portion

			if @@rowcount != 1
				return XZAEC_LOC_PT_CAR_INC

			/* determiner le PR suivant le sens donne en entree */
			else if @va_Sens_in = XDC_SENS_INCONNU or
			        @va_Sens_in = XDC_SENS_SORTIE_NORD or 
				@va_Sens_in = XDC_SENS_ENTREE_NORD or
			   	@va_Sens_in = XDC_SENS_ENTREE_SUD or 
				@va_Sens_in = XDC_SENS_SORTIE_SUD
			begin
				/* tester si les PR sont values */
				if @vl_PRS = 0
					select	@vl_PRS = null
				if @vl_PRN = 0
					select	@vl_PRN = null

				select	@va_PR_out = (	isnull(@vl_PRN, @vl_PRS) + 
							isnull(@vl_PRS, @vl_PRN)) / 2,
					@va_Sens_out = XDC_SENS_INCONNU
			end
			else if @va_Sens_in = XDC_SENS_NORD 
				select	@va_Sens_out = @va_Sens_in,
					@va_PR_out = @vl_PRN
			else if @va_Sens_in = XDC_SENS_SUD 
				select	@va_Sens_out = @va_Sens_in,
					@va_PR_out = @vl_PRS
			else return XDC_ARG_INV

		end
		else if @va_TypePtCaracteristique_in = XDC_POINT_CARACT_LIT_ARRET
		begin
			/* localisation du lit d'arret */
			select
				@va_NumAutoroute_out	= CFG..EQT_GEN.autoroute,
				@va_PR_out 		= CFG..EQT_GEN.PR,
				@va_Sens_out		= CFG..EQT_GEN.sens,
				@vl_Portion		= CFG..RES_POR.numero,
				@va_District_out	= CFG..RES_POR.district,
				@va_DistrictRecouvr_out = CFG..RES_POR.district_recouvrement
			from CFG..EQT_GEN, CFG..RES_POR
			where	CFG..EQT_GEN.numero = @va_PtCaracteristique_in and
				CFG..EQT_GEN.type = XDC_CAPT_LIT_ARRET and
				CFG..RES_POR.autoroute = CFG..EQT_GEN.autoroute and
				CFG..EQT_GEN.PR > CFG..RES_POR.PR_debut and
				CFG..EQT_GEN.PR <= CFG..RES_POR.PR_fin

			if @@rowcount != 1
				return XZAEC_LOC_PT_CAR_INC
		end
		else return XDC_ARG_INV
	end

	/*A recherche de la portion sur laquelle se trouve la localisation */
	if @va_NumAutoroute_in != XDC_AUT_INCONNUE and @va_PR_in != XDC_PR_INCONNU and
	   @va_PtCaracteristique_in = null 
	begin
		/* verifier la localisation (autoroute, PR, sens) */
		/* pas de point caracteristique en entree */

		/* recherche sur quelle portion se trouve la localisation */
		select	@vl_Portion = numero
		from CFG..RES_POR
		where	autoroute = @va_NumAutoroute_in 

		if @@rowcount = 0
			return XDC_NOK

		/* tester si le PR existe sur cette portion */
		select	@vl_PrDebut = PR_debut,
			@vl_Portion = numero,
			@va_District_out = district,
			@va_DistrictRecouvr_out = district_recouvrement
		from CFG..RES_POR
		where	autoroute = @va_NumAutoroute_in and
			PR_debut <= @va_PR_in and @va_PR_in < PR_fin 

		if @@rowcount != 1
			select @va_Resultat_out = XDC_LOC_INC

		/* conservation des valeurs d'entree pour la sortie */
		select	@va_Sens_out = @va_Sens_in,
			@va_NumAutoroute_out = @va_NumAutoroute_in,
			@va_PR_out = @va_PR_in
	end
	
	/* la localisation donnee en entree est incorrecte */
	if @va_Resultat_out = XDC_LOC_INC 
	begin
		/* pas de configuration des voies */
		return XDC_OK
	end

	/* DEM439 : Appel de XZAO428 pour verifier le district de recouvrement */
	/* vis a vis du sens (sauf pour fmc contresens et basculements) */
	if @va_Type_in != XZAEC_FMC_Contresens and
		@va_Type_in != XZAEC_FMC_Basculement and 
		@va_Type_in != XZAEC_FMC_BasculTravaux and 
		@va_Type_in != 0 and 
		@va_Sens_out != XDC_SENS_INCONNU and  
		@va_Sens_out != XDC_SENS_ENTREE_NORD and  
		@va_Sens_out != XDC_SENS_ENTREE_SUD
	begin
		select  @vl_Sens=@va_Sens_out 
		if @vl_Sens = XDC_SENS_SORTIE_NORD
			select @vl_Sens=XDC_SENS_NORD

		if @vl_Sens = XDC_SENS_SORTIE_SUD
			select @vl_Sens=XDC_SENS_SUD
			
		exec XZAO428 @va_District_out,@va_NumAutoroute_out,@vl_Portion,@vl_Sens,@va_DistrictRecouvr_out,@vl_District output
		
		if @vl_District=null 
			select @va_DistrictRecouvr_out=@va_District_out
			
	end



	/*A description de la configuration des voies pour la localisation */
	if @va_Sens_out = XDC_SENS_ENTREE_SUD or @va_Sens_out = XDC_SENS_SUD or
	   @va_Sens_out = XDC_SENS_SORTIE_SUD
	begin
		set rowcount 1
		select
			@vl_NbVoies = nombre_de_voies,
			@vl_LargeurBAU = largeur_BAU
		from CFG..RES_SEG
		where	portion = @vl_Portion and PR_debut <= @va_PR_out and
			sens = XDC_SENS_SUD
		order by PR_debut desc

		if @@rowcount != 1
			select	@va_Sens_out = XDC_SENS_INCONNU

		set rowcount 0

		/*B si pas sur section courante alors 1 seule voie */
		
		if @va_Sens_out <> XDC_SENS_SUD and @va_TypePtCaracteristique_in = XDC_POINT_CARACT_ECHANGEUR
			select @vl_NbVoies = 1
		else if @va_Sens_out <> XDC_SENS_SUD select @vl_NbVoies = 1,@vl_LargeurBAU = 0
	end
	if @va_Sens_out = XDC_SENS_ENTREE_NORD or @va_Sens_out = XDC_SENS_NORD or
	   @va_Sens_out = XDC_SENS_SORTIE_NORD
	begin
		set rowcount 1
		select
			@vl_NbVoies = nombre_de_voies,
			@vl_LargeurBAU = largeur_BAU
		from CFG..RES_SEG
		where	portion = @vl_Portion and PR_debut <= @va_PR_out and
			sens = XDC_SENS_NORD
                order by PR_debut desc

		if @@rowcount != 1
			select	@va_Sens_out = XDC_SENS_INCONNU

		set rowcount 0

		/*B si pas sur section courante alors 1 seule voie */
		
		if @va_Sens_out <> XDC_SENS_NORD and @va_TypePtCaracteristique_in = XDC_POINT_CARACT_ECHANGEUR
			select @vl_NbVoies = 1
		else if @va_Sens_out <> XDC_SENS_NORD select @vl_NbVoies = 1,@vl_LargeurBAU = 0
	end

	/* indiquer dans le resultat  de la procedure les donnees incorrectes */
	if @va_NumAutoroute_in != @va_NumAutoroute_out and 
	   @va_NumAutoroute_in != XDC_AUT_INCONNUE
		select @va_Resultat_out = @va_Resultat_out + XDC_LOC_AUT_MOD
	if @va_PR_in != @va_PR_out and @va_PR_in != XDC_PR_INCONNU
		select @va_Resultat_out = @va_Resultat_out + XDC_LOC_PR_MOD
	if @va_Sens_in != @va_Sens_out and @va_Sens_in != XDC_SENS_INCONNU
		select @va_Resultat_out = @va_Resultat_out + XDC_LOC_SENS_MOD

	/* sens non definissable a partir de la localisation : pas de config de voie */
	if @va_Sens_out = XDC_SENS_INCONNU
	begin

		return XDC_OK
	end

	/*A description de la configuration des voies inverses pour la localisation */
	select @vl_NbVoiesInverses = null, @vl_LargeurBAUInverse = null
	if @va_Sens_out = XDC_SENS_NORD
	begin
		set rowcount 1
		select
			@vl_NbVoiesInverses = nombre_de_voies,
			@vl_LargeurBAUInverse = largeur_BAU
		from CFG..RES_SEG
		where	portion = @vl_Portion and PR_debut <= @va_PR_out and
			sens = XDC_SENS_SUD
                order by PR_debut desc
		set rowcount 0
	end
	if @va_Sens_out = XDC_SENS_SUD
	begin
		set rowcount 1
		select
			@vl_NbVoiesInverses = nombre_de_voies,
			@vl_LargeurBAUInverse = largeur_BAU
		from CFG..RES_SEG
		where	portion = @vl_Portion and PR_debut <= @va_PR_out and
			sens = XDC_SENS_NORD
                order by PR_debut desc
		set rowcount 0
	end

	/* initialisation des arguments de sortie pour description de la */
	/* configuration des voies inverses */
	if (@va_Sens_out = XDC_SENS_NORD or @va_Sens_out = XDC_SENS_SUD)
	begin /*! si bau < XZAEC_SEUIL_BAU_ETROITE metre alors bau etroite */
		if @vl_LargeurBAUInverse < XZAEC_SEUIL_BAU_ETROITE 
			select @va_BAU_I_Etroite_out = XDC_VRAI
		else	select @va_BAU_I_Etroite_out = XDC_FAUX

		if @vl_LargeurBAUInverse = 0 or @vl_LargeurBAUInverse = null
			select @va_BAU_I_out = XDC_VOIE_INEXISTANTE
		else	select @va_BAU_I_out = XDC_VOIE_BAU_NORMALE
	
		if @vl_NbVoiesInverses = 1
			select @va_VL_I_out = XDC_VOIE_SENS_NORMAL,
				@va_VM2_I_out = XDC_VOIE_INEXISTANTE,
				@va_VM1_I_out = XDC_VOIE_INEXISTANTE,
				@va_VR_I_out = XDC_VOIE_INEXISTANTE
		else if @vl_NbVoiesInverses = 2
			select	@va_VL_I_out = XDC_VOIE_SENS_NORMAL,
				@va_VM2_I_out = XDC_VOIE_INEXISTANTE,
				@va_VM1_I_out = XDC_VOIE_INEXISTANTE,
				@va_VR_I_out = XDC_VOIE_SENS_NORMAL
		else if @vl_NbVoiesInverses = 3
			select	@va_VL_I_out = XDC_VOIE_SENS_NORMAL,
				@va_VM2_I_out = XDC_VOIE_INEXISTANTE,
				@va_VM1_I_out = XDC_VOIE_SENS_NORMAL,
				@va_VR_I_out = XDC_VOIE_SENS_NORMAL
		else if @vl_NbVoiesInverses = 4
			select	@va_VL_I_out = XDC_VOIE_SENS_NORMAL,
				@va_VM2_I_out = XDC_VOIE_SENS_NORMAL,
				@va_VM1_I_out = XDC_VOIE_SENS_NORMAL,
				@va_VR_I_out = XDC_VOIE_SENS_NORMAL
		else return XDC_NOK
	end

	/* initialisation des arguments de sortie pour description de */
	/* la configuration des voies                                 */
	begin /*! si bau < XZAEC_SEUIL_BAU_ETROITE mettre alors bau etroite */
		if @vl_LargeurBAU < XZAEC_SEUIL_BAU_ETROITE or 
		   @va_TypePtCaracteristique_in = XDC_POINT_CARACT_ECHANGEUR
			select @va_BAU_Etroite_out = XDC_VRAI
		else	select @va_BAU_Etroite_out = XDC_FAUX

		if @vl_LargeurBAU = 0 or @vl_LargeurBAU = null
			select @va_BAU_out = XDC_VOIE_INEXISTANTE
		else	select @va_BAU_out = XDC_VOIE_BAU_NORMALE
	
		if @vl_NbVoies = 1
			select @va_VL_out = XDC_VOIE_SENS_NORMAL,
				@va_VM2_out = XDC_VOIE_INEXISTANTE,
				@va_VM1_out = XDC_VOIE_INEXISTANTE,
				@va_VR_out = XDC_VOIE_INEXISTANTE
		else if @vl_NbVoies = 2
			select	@va_VL_out = XDC_VOIE_SENS_NORMAL,
				@va_VM2_out = XDC_VOIE_INEXISTANTE,
				@va_VM1_out = XDC_VOIE_INEXISTANTE,
				@va_VR_out = XDC_VOIE_SENS_NORMAL
		else if @vl_NbVoies = 3
			select	@va_VL_out = XDC_VOIE_SENS_NORMAL,
				@va_VM2_out = XDC_VOIE_INEXISTANTE,
				@va_VM1_out = XDC_VOIE_SENS_NORMAL,
				@va_VR_out = XDC_VOIE_SENS_NORMAL
		else if @vl_NbVoies = 4
			select	@va_VL_out = XDC_VOIE_SENS_NORMAL,
				@va_VM2_out = XDC_VOIE_SENS_NORMAL,
				@va_VM1_out = XDC_VOIE_SENS_NORMAL,
				@va_VR_out = XDC_VOIE_SENS_NORMAL
		else return XDC_NOK
	end


	return XDC_OK
go
