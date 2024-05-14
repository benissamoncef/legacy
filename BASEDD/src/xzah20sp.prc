/*E*/
/*  Fichier : $Id: xzah20sp.prc,v 1.11 2007/09/19 17:48:36 gesconf Exp $        Release : $Revision: 1.11 $        Date : $Date: 2007/09/19 17:48:36 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah20sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* initialisation et mise a jour des voies pour equext
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.  30/11/94        : Creation      (V 1.1)
* C.T.  08/12/94        : Modif ajout des etats des voies dans le sens 
*                         du roulement pour le basculement (V 1.2)
* C.T.  06/01/95        : Suppression du test sur le site 
*                         au niveau de l'evenement mais
*                         rajout au niveau des dai et radt
*                         ( V 1.3)
* P.N.  29/01/99        : Inhibition des DAI ds Bouchon EVOL18 dem/1731 v1.4 
* P.N.  10/09/99	: Correction DAI EVOL18 
* P.N.  29/11/99	: Correction DAI EVOL18 (transf. BLOQUEE-> NEUTRALISEE v1.5
* P.C   02/12/1999      : Correction du bug pour cas travaux (double select en commentaire) 1.6
* P.N   24/06/05        : Ajout d'un arg a xzah;25 pour ne pas supprimer les config de voie dans le second appel pour les basculement v1.10 DEM480
* JPL	11/06/07	: Prise en compte des Fmc Basculement pour Travaux (DEM 647) 1.8
* P.N.	29/08/07	: Appel de xzah25 pour basculement et basculement travaux 
			  Calcul des PR extremes du basculement pour trvx DEM695 1.9
* PNI	19/09/07	: Consolidation : neutralisation de toutes les voies de la chaussée basculées  DEM705 1.10
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Procedure mettant a jour l'etat des voies a chaque
* validation d'un evenement local a un district ou CI
*
* Sequence d'appel
* SP    XZAH20_Voies_Fmc
*
* Arguments en entree
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* XDY_Horodate	va_Horodate_in
* XDY_Autoroute	va_Autoroute_in
* XDY_PR	va_PR_in
* XDY_Sens	va_Sens_in
* XDY_Voie	va_VL_in
* XDY_Voie	va_VM1_in
* XDY_Voie	va_VM2_in
* XDY_Voie	va_VR_in
* XDY_Voie	va_BAU_in
* XDY_Voie	va_VL_I_in
* XDY_Voie	va_VM1_I_in
* XDY_Voie	va_VM2_I_in
* XDY_Voie	va_VR_I_in
* XDY_Voie	va_BAU_I_in
* XDY_Site	va_Site_in
*
* Arguments en sortie
*
* Code retour
* XDC_OK        : pas de probleme
* XDC_NOK       : probleme a l'insertion dans la base de sauvegarde
*                 ou donnee non trouvee
* XDC_ARG_INV   : parametre d'entree null
* XDC_PRC_INC   : procedure XZAH;25 inexistante
*
* Conditions d'utilisation
* La procedure est appelee par le trigger de la table FMC_HIS
* Un evenement doit rester sur la meme chaussee (conserver le sens)
*
* Fonction
* insert ou update de VOI_DAI et VOI_RAD
-------------------------------------------------------*/

use PRC
go

create procedure XZAH;20
	@va_NumEvt_in	int = null,
	@va_CleEvt_in	tinyint = null,
	@va_Horodate_in	datetime = null,
	@va_Autoroute_in	T_AUTOROUTE = null,
	@va_PR_in	T_PR = null,
	@va_Sens_in	T_SENS = null,
	@va_VL_in	T_VOIE = null,
	@va_VM1_in	T_VOIE = null,
	@va_VM2_in	T_VOIE = null,
	@va_VR_in	T_VOIE = null,
	@va_BAU_in	T_VOIE = null,
	@va_VL_I_in	T_VOIE = null,
	@va_VM1_I_in	T_VOIE = null,
	@va_VM2_I_in	T_VOIE = null,
	@va_VR_I_in	T_VOIE = null,
	@va_BAU_I_in	T_VOIE = null,
	@va_Site_in	T_SITE = null
as
	declare @vl_Classe tinyint, 
                @vl_PR T_PR,
		@vl_PRDebut_I T_PR,
		@vl_PRFin_I T_PR, 
                @vl_PRDebut T_PR,
		@vl_PRFin T_PR, 
                @vl_PRDebut_BAU T_PR,
		@vl_PRFin_BAU T_PR, 
                @vl_PRDebut_VL T_PR,
		@vl_PRFin_VL T_PR, 
                @vl_PRDebut_VM1 T_PR,
		@vl_PRFin_VM1 T_PR, 
                @vl_PRDebut_VM2 T_PR,
		@vl_PRFin_VM2 T_PR, 
                @vl_PRDebut_VR T_PR,
		@vl_PRFin_VR T_PR, 
                @vl_PRDebut_BAU_I T_PR,
		@vl_PRFin_BAU_I T_PR, 
                @vl_PRDebut_VL_I T_PR,
		@vl_PRFin_VL_I T_PR, 
                @vl_PRDebut_VM1_I T_PR,
		@vl_PRFin_VM1_I T_PR, 
                @vl_PRDebut_VM2_I T_PR,
		@vl_PRFin_VM2_I T_PR, 
                @vl_PRDebut_VR_I T_PR,
		@vl_PRFin_VR_I T_PR, 
		@vl_DebutEvt datetime,
		@vl_FinEvt datetime, @vl_Status int, @vl_cause int,
		@vl_type tinyint

	/*A controler les arguments en entree */
	if @va_NumEvt_in = null or @va_CleEvt_in = null or
	   @va_Horodate_in = null or @va_Autoroute_in = null or
	   @va_PR_in = null or @va_Sens_in = null or
	   @va_VL_in = null or @va_VM1_in = null or
	   @va_VM2_in = null or @va_VR_in = null or
	   @va_BAU_in = null or @va_VL_I_in = null or
	   @va_VM1_I_in = null or @va_VM2_I_in = null or
	   @va_VR_I_in = null or @va_BAU_I_in = null or
	   @va_Site_in = null
		return XDC_ARG_INV


	/*A recherche de la classe d'evenement */
	select	@vl_Classe = CFG..TYP_FMC.classe,
		@vl_DebutEvt = EXP..FMC_GEN.debut,
		@vl_FinEvt = EXP..FMC_GEN.fin,
		@vl_type=EXP..FMC_GEN.type
	from CFG..TYP_FMC, EXP..FMC_GEN
	where	EXP..FMC_GEN.numero = @va_NumEvt_in and
		EXP..FMC_GEN.cle = @va_CleEvt_in and
		CFG..TYP_FMC.numero = EXP..FMC_GEN.type

	/*A Si l'evenement n'a pas commence alors */
	/*A aucun traitement a realise : sortir   */
	if @vl_DebutEvt is null
		return XDC_OK

	/*! positionner tous les etats des dai  et des radt du a cet evenement */
	/*! lorsqu'il est termine ou n'est plus sur la meme chaussee a         */
	/*! "supprimer etat du a evenement"  et sortir                         */
	/*! si c'est un basculement ou des travaux                             */
	if (@vl_FinEvt is not null or 
	    @va_Sens_in = XDC_SENS_ENTREE_SUD or
	    @va_Sens_in = XDC_SENS_SORTIE_SUD or 
	    @va_Sens_in = XDC_SENS_ENTREE_NORD or
	    @va_Sens_in = XDC_SENS_SORTIE_NORD) and
	    @vl_Classe in (XZAEC_ClasseBasculement,
	                   XZAEC_ClasseTravaux,
	                   XZAEC_ClasseBasculTrav,
	                   XZAEC_ClasseBouchon)
	begin
		update EXP..VOI_DAI set mode = XZAHC_VOIES_SUPP
		where evenement = @va_NumEvt_in and cle = @va_CleEvt_in

		update EXP..VOI_RAD set mode = XZAHC_VOIES_SUPP
		where evenement = @va_NumEvt_in and cle = @va_CleEvt_in

		return XDC_OK
	end

	/*A si l'evenement n'est pas de classe basculement ou  */
	/*A travaux alors aucun traitement a realiser : sortir */
	/*A sinon rechercher sur la zone en PR de l'evenement  */
	/*A puis inserer les etats des voies                   */ 
	if @vl_Classe = XZAEC_ClasseBasculement
	begin
		select 
			@vl_PRDebut = @va_PR_in,
			@vl_PRFin = localisation_PR_fin
		from EXP..FMC_BAS
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in

		if @@rowcount != 1
			return XDC_NOK

		/*B memoriser les etats des voies pour les dai et les radt */
		/*B dans le sens de l'evenement                            */
		exec @vl_Status = XZAH;25  @va_NumEvt_in, @va_CleEvt_in,
					@va_Autoroute_in, @vl_PRDebut, @vl_PRFin, 
					@va_Sens_in, @va_VL_in, @va_VM1_in, 
					@va_VM2_in, @va_VR_in, @va_BAU_in, XDC_FAUX

		if @vl_Status = null
			return XDC_PRC_INC
		else if @vl_Status != XDC_OK
			return @vl_Status

		/*B memoriser les etats des voies pour les dai et les radt */
		/*B dans le sens inverse                                   */
		if @va_Sens_in = XDC_SENS_SUD 
			exec @vl_Status = XZAH;25  @va_NumEvt_in, @va_CleEvt_in,
					@va_Autoroute_in, @vl_PRFin, @vl_PRDebut, 
					XDC_SENS_NORD, @va_VL_I_in, @va_VM1_I_in, 
					@va_VM2_I_in, @va_VR_I_in, @va_BAU_I_in, XDC_VRAI
		else if @va_Sens_in = XDC_SENS_NORD
			exec @vl_Status = XZAH;25  @va_NumEvt_in, @va_CleEvt_in,
					@va_Autoroute_in, @vl_PRFin, @vl_PRDebut,
					XDC_SENS_SUD, @va_VL_I_in, @va_VM1_I_in, 
					@va_VM2_I_in, @va_VR_I_in, @va_BAU_I_in, XDC_VRAI

		if @vl_Status = null
			return XDC_PRC_INC
		else if @vl_Status != XDC_OK
			return @vl_Status
	end
	else if @vl_Classe in (XZAEC_ClasseTravaux, XZAEC_ClasseBasculTrav)
	begin
		select 
			@vl_PRDebut = localisation_tete,
			@vl_PRFin = @va_PR_in
		from EXP..FMC_TRH
		where	numero = @va_NumEvt_in and 
			cle = @va_CleEvt_in and
			horodate = @va_Horodate_in

		if @@rowcount != 1
			return XDC_NOK

		if @vl_Classe=XZAEC_ClasseBasculTrav
		begin
			select 
				@vl_PRFin = localisation_tete,
				@vl_PRDebut = @va_PR_in
			from EXP..FMC_TRH
			where	numero = @va_NumEvt_in and 
				cle = @va_CleEvt_in and
				horodate = @va_Horodate_in

			if (@vl_PRDebut>@vl_PRFin)
			begin
				select @vl_PR=@vl_PRDebut
				select @vl_PRDebut=@vl_PRFin
				select @vl_PRFin=@vl_PR
			end
			select @vl_PRDebut_I=@vl_PRFin,@vl_PRFin_I=@vl_PRDebut


			select 	@vl_PRDebut_BAU = isnull(PR_debut_BAU,-1),
				@vl_PRFin_BAU = isnull(PR_fin_BAU, -1),
                		@vl_PRDebut_VL = isnull(PR_debut_VL,-1),
				@vl_PRFin_VL = isnull(PR_fin_VL, -1),
		                @vl_PRDebut_VM1 = isnull(PR_debut_VM1,-1),
		                @vl_PRFin_VM1 = isnull(PR_fin_VM1,-1),
		                @vl_PRDebut_VM2 = isnull(PR_debut_VM2,-1),
				@vl_PRFin_VM2 = isnull(PR_fin_VM2, -1),
		                @vl_PRDebut_VR = isnull(PR_debut_VR,-1),
				@vl_PRFin_VR = isnull(PR_fin_VR, -1),
				@vl_PRDebut_BAU_I = isnull(PR_debut_BAU_I,-1),
				@vl_PRFin_BAU_I = isnull(PR_fin_BAU_I, -1),
                		@vl_PRDebut_VL_I = isnull(PR_debut_VL_I,-1),
				@vl_PRFin_VL_I = isnull(PR_fin_VL_I, -1),
		                @vl_PRDebut_VM1_I = isnull(PR_debut_VM1_I,-1),
		                @vl_PRFin_VM1_I = isnull(PR_fin_VM1_I,-1),
		                @vl_PRDebut_VM2_I = isnull(PR_debut_VM2_I,-1),
				@vl_PRFin_VM2_I = isnull(PR_fin_VM2_I, -1),
		                @vl_PRDebut_VR_I = isnull(PR_debut_VR_I,-1),
				@vl_PRFin_VR_I = isnull(PR_fin_VR_I,-1)
			from EXP..FMC_TRF
			where	numero = @va_NumEvt_in and 
				cle = @va_CleEvt_in

			if @@rowcount != 1
			return XDC_NOK
			
			/* On considere que toute la chaussee du sens du basculement est neutralisee */
				select @va_BAU_in=XDC_VOIE_NEUTRALISEE
				select @va_VL_in=XDC_VOIE_NEUTRALISEE
				select @va_VM1_in=XDC_VOIE_NEUTRALISEE
				select @va_VM2_in=XDC_VOIE_NEUTRALISEE
				select @va_VR_in=XDC_VOIE_NEUTRALISEE

/*			if (@vl_PRDebut_BAU_I!=-1)
				select @va_BAU_I_in=XDC_VOIE_NEUTRALISEE
			if (@vl_PRDebut_VL_I!=-1)
				select @va_VL_I_in=XDC_VOIE_NEUTRALISEE
			if (@vl_PRDebut_VM1_I!=-1)
				select @va_VM1_I_in=XDC_VOIE_NEUTRALISEE
			if (@vl_PRDebut_VM_I2!=-1)
				select @va_VM2_I_in=XDC_VOIE_NEUTRALISEE
			if (@vl_PRDebut_VR_I!=-1)
				select @va_VR_I_in=XDC_VOIE_NEUTRALISEE*/


			/* Recherche des PR debut et fin extremes des trvx */
/* recherche min pour PRdebut */
				if (@vl_PRDebut_BAU<@vl_PRDebut) and (@vl_PRDebut_BAU!=-1)
					select @vl_PRDebut=@vl_PRDebut_BAU
				if (@vl_PRDebut_VL<@vl_PRDebut) and (@vl_PRDebut_VL!=-1)
					select @vl_PRDebut=@vl_PRDebut_VL
				if (@vl_PRDebut_VM1<@vl_PRDebut) and (@vl_PRDebut_VM1!=-1)
					select @vl_PRDebut=@vl_PRDebut_VM1
				if (@vl_PRDebut_VM2<@vl_PRDebut) and (@vl_PRDebut_VM2!=-1)
					select @vl_PRDebut=@vl_PRDebut_VM2
				if (@vl_PRDebut_VR<@vl_PRDebut) and (@vl_PRDebut_VR!=-1)
					select @vl_PRDebut=@vl_PRDebut_VR

				if (@vl_PRFin_BAU<@vl_PRDebut) and (@vl_PRFin_BAU!=-1)
					select @vl_PRDebut=@vl_PRFin_BAU
				if (@vl_PRFin_VL<@vl_PRDebut) and (@vl_PRFin_VL!=-1)
					select @vl_PRDebut=@vl_PRFin_VL
				if (@vl_PRFin_VM1<@vl_PRDebut) and (@vl_PRFin_VM1!=-1)
					select @vl_PRDebut=@vl_PRFin_VM1
				if (@vl_PRFin_VM2<@vl_PRDebut) and (@vl_PRFin_VM2!=-1)
					select @vl_PRDebut=@vl_PRFin_VM2
				if (@vl_PRFin_VR<@vl_PRDebut) and (@vl_PRFin_VR!=-1)
					select @vl_PRDebut=@vl_PRFin_VR


/* recherche max pour PRFin */
				if (@vl_PRFin_BAU>@vl_PRFin) and (@vl_PRFin_BAU!=-1)
					select @vl_PRFin=@vl_PRFin_BAU
				if (@vl_PRFin_VL>@vl_PRFin) and (@vl_PRFin_VL!=-1)
					select @vl_PRFin=@vl_PRFin_VL
				if (@vl_PRFin_VM1>@vl_PRFin) and (@vl_PRFin_VM1!=-1)
					select @vl_PRFin=@vl_PRFin_VM1
				if (@vl_PRFin_VM2>@vl_PRFin) and (@vl_PRFin_VM2!=-1)
					select @vl_PRFin=@vl_PRFin_VM2
				if (@vl_PRFin_VR>@vl_PRFin) and (@vl_PRFin_VR!=-1)
					select @vl_PRFin=@vl_PRFin_VR

				if (@vl_PRDebut_BAU>@vl_PRFin) and (@vl_PRDebut_BAU!=-1)
					select @vl_PRFin=@vl_PRDebut_BAU
				if (@vl_PRDebut_VL>@vl_PRFin) and (@vl_PRDebut_VL!=-1)
					select @vl_PRFin=@vl_PRDebut_VL
				if (@vl_PRDebut_VM1>@vl_PRFin) and (@vl_PRDebut_VM1!=-1)
					select @vl_PRFin=@vl_PRDebut_VM1
				if (@vl_PRDebut_VM2>@vl_PRFin) and (@vl_PRDebut_VM2!=-1)
					select @vl_PRFin=@vl_PRDebut_VM2
				if (@vl_PRDebut_VR>@vl_PRFin) and (@vl_PRDebut_VR!=-1)
					select @vl_PRFin=@vl_PRDebut_VR

/* PR inverse */
/* recherche max pour PRdebut */

				if (@vl_PRDebut_BAU_I>@vl_PRDebut_I) and (@vl_PRDebut_BAU_I!=-1)
					select @vl_PRDebut_I=@vl_PRDebut_BAU_I
				if (@vl_PRDebut_VL_I>@vl_PRDebut_I) and (@vl_PRDebut_VL_I!=-1)
					select @vl_PRDebut_I=@vl_PRDebut_VL_I
				if (@vl_PRDebut_VM1_I>@vl_PRDebut_I) and (@vl_PRDebut_VM1_I!=-1)
					select @vl_PRDebut_I=@vl_PRDebut_VM1_I
				if (@vl_PRDebut_VM2_I>@vl_PRDebut_I) and (@vl_PRDebut_VM2_I!=-1)
					select @vl_PRDebut_I=@vl_PRDebut_VM2_I
				if (@vl_PRDebut_VR_I>@vl_PRDebut_I) and (@vl_PRDebut_VR_I!=-1)
					select @vl_PRDebut_I=@vl_PRDebut_VR_I

				if (@vl_PRFin_BAU_I>@vl_PRDebut_I) and (@vl_PRFin_BAU_I!=-1)
					select @vl_PRDebut_I=@vl_PRFin_BAU_I
				if (@vl_PRFin_VL_I>@vl_PRDebut_I) and (@vl_PRFin_VL_I!=-1)
					select @vl_PRDebut_I=@vl_PRFin_VL_I
				if (@vl_PRFin_VM1_I>@vl_PRDebut_I) and (@vl_PRFin_VM1_I!=-1)
					select @vl_PRDebut_I=@vl_PRFin_VM1_I
				if (@vl_PRFin_VM2_I>@vl_PRDebut_I) and (@vl_PRFin_VM2_I!=-1)
					select @vl_PRDebut_I=@vl_PRFin_VM2_I
				if (@vl_PRFin_VR_I>@vl_PRDebut_I) and (@vl_PRFin_VR_I!=-1)
					select @vl_PRDebut_I=@vl_PRFin_VR_I



/* recherche min pour PRFin */
				if (@vl_PRFin_BAU_I<@vl_PRFin_I) and (@vl_PRFin_BAU_I!=-1)
					select @vl_PRFin_I=@vl_PRFin_BAU_I
				if (@vl_PRFin_VL_I<@vl_PRFin_I) and (@vl_PRFin_VL_I!=-1)
					select @vl_PRFin_I=@vl_PRFin_VL_I
				if (@vl_PRFin_VM1_I<@vl_PRFin_I) and (@vl_PRFin_VM1_I!=-1)
					select @vl_PRFin_I=@vl_PRFin_VM1_I
				if (@vl_PRFin_VM2_I<@vl_PRFin_I) and (@vl_PRFin_VM2_I!=-1)
					select @vl_PRFin_I=@vl_PRFin_VM2_I
				if (@vl_PRFin_VR_I<@vl_PRFin_I) and (@vl_PRFin_VR_I!=-1)
					select @vl_PRFin_I=@vl_PRFin_VR_I

				if (@vl_PRDebut_BAU_I<@vl_PRFin_I) and (@vl_PRDebut_BAU_I!=-1)
					select @vl_PRFin_I=@vl_PRDebut_BAU_I
				if (@vl_PRDebut_VL_I<@vl_PRFin_I) and (@vl_PRDebut_VL_I!=-1)
					select @vl_PRFin_I=@vl_PRDebut_VL_I
				if (@vl_PRDebut_VM1_I<@vl_PRFin_I) and (@vl_PRDebut_VM1_I!=-1)
					select @vl_PRFin_I=@vl_PRDebut_VM1_I
				if (@vl_PRDebut_VM2_I<@vl_PRFin_I) and (@vl_PRDebut_VM2_I!=-1)
					select @vl_PRFin_I=@vl_PRDebut_VM2_I
				if (@vl_PRDebut_VR_I<@vl_PRFin_I) and (@vl_PRDebut_VR_I!=-1)
					select @vl_PRFin_I=@vl_PRDebut_VR_I




			/* Inhibe les DAI dans le sens des trvx */
			/*B memoriser les etats des voies pour les dai et les radt */
			exec @vl_Status = XZAH;25  @va_NumEvt_in, @va_CleEvt_in,
					@va_Autoroute_in, @vl_PRFin, @vl_PRDebut, 
					@va_Sens_in, @va_VL_in, @va_VM1_in, 
					@va_VM2_in, @va_VR_in, @va_BAU_in, XDC_FAUX

			if @vl_Status = null
				return XDC_PRC_INC
			else if @vl_Status != XDC_OK
				return @vl_Status

			/*B memoriser les etats des voies pour les dai et les radt */
			/*B dans le sens inverse                                   */
			/* DEM695 */
			if @va_Sens_in = XDC_SENS_SUD 
				exec @vl_Status = XZAH;25  @va_NumEvt_in, @va_CleEvt_in,
					@va_Autoroute_in, @vl_PRFin_I, @vl_PRDebut_I, 
					XDC_SENS_NORD, @va_VL_I_in, @va_VM1_I_in, 
					@va_VM2_I_in, @va_VR_I_in, @va_BAU_I_in, XDC_VRAI
			else if @va_Sens_in = XDC_SENS_NORD
				exec @vl_Status = XZAH;25  @va_NumEvt_in, @va_CleEvt_in,
					@va_Autoroute_in, @vl_PRFin_I, @vl_PRDebut_I,
					XDC_SENS_SUD, @va_VL_I_in, @va_VM1_I_in, 
					@va_VM2_I_in, @va_VR_I_in, @va_BAU_I_in, XDC_VRAI

			if @vl_Status = null
				return XDC_PRC_INC
			else if @vl_Status != XDC_OK
				return @vl_Status


		end
		else
		begin

			/*B memoriser les etats des voies pour les dai et les radt */
			exec @vl_Status = XZAH;25  @va_NumEvt_in, @va_CleEvt_in,
					@va_Autoroute_in, @vl_PRFin, @vl_PRDebut, 
					@va_Sens_in, @va_VL_in, @va_VM1_in, 
					@va_VM2_in, @va_VR_in, @va_BAU_in, XDC_FAUX
			if @vl_Status = null
				return XDC_PRC_INC
			else if @vl_Status != XDC_OK
				return @vl_Status
		end

	end
	else if @vl_Classe = XZAEC_ClasseBouchon
	begin
		if @vl_type=XZAEC_FMC_TeteBouchon
		begin
			select @vl_cause=numero
			from EXP..FMC_GEN
			where	cause = @va_NumEvt_in and 
				cle_cause = @va_CleEvt_in 
		
			select 
				@vl_PRDebut = @va_PR_in,
				@vl_PRFin = PR
			from EXP..FMC_HIS
			where	numero = @vl_cause and 
				cle = @va_CleEvt_in and
				horodate_validation <= @va_Horodate_in
                        having horodate_validation=max(horodate_validation) and
				numero = @vl_cause and
				cle = @va_CleEvt_in and
			 	horodate_validation <= @va_Horodate_in
		end
		else
		begin
			select @vl_cause=cause
			from EXP..FMC_GEN
			where	numero = @va_NumEvt_in and 
				cle = @va_CleEvt_in 
		
			select 
				@vl_PRDebut = PR,
				@vl_PRFin = @va_PR_in
			from EXP..FMC_HIS
			where	numero = @vl_cause and 
				cle = @va_CleEvt_in and
				horodate_validation <= @va_Horodate_in
			having horodate_validation=max(horodate_validation) and
				numero = @vl_cause and
				cle = @va_CleEvt_in and
				 horodate_validation <= @va_Horodate_in

		end
		
		if @@rowcount != 1
			return XDC_NOK

		/*B memoriser les etats des voies pour les dai et les radt */

		/* Transformation de BLOQUEE --> NEUTRALISEE pour le traitement de l'inhibition
			des DAI par tedai.x */

		if  @va_VL_in = XDC_VOIE_BLOQUEE
			select @va_VL_in = XDC_VOIE_NEUTRALISEE
                if  @va_VR_in = XDC_VOIE_BLOQUEE
			select @va_VR_in = XDC_VOIE_NEUTRALISEE
                if  @va_VM1_in = XDC_VOIE_BLOQUEE
			select @va_VM1_in = XDC_VOIE_NEUTRALISEE
		if  @va_VM2_in = XDC_VOIE_BLOQUEE
			select @va_VM2_in = XDC_VOIE_NEUTRALISEE
		if  @va_BAU_in = XDC_VOIE_BLOQUEE
			select @va_BAU_in = XDC_VOIE_NEUTRALISEE

		exec @vl_Status = XZAH;25  @va_NumEvt_in, @va_CleEvt_in,
					@va_Autoroute_in, @vl_PRFin, @vl_PRDebut, 
					@va_Sens_in, @va_VL_in, @va_VM1_in, 
					@va_VM2_in, @va_VR_in, @va_BAU_in, XDC_FAUX
		if @vl_Status = null
			return XDC_PRC_INC
		else if @vl_Status != XDC_OK
			return @vl_Status
	end

	return XDC_OK
go
