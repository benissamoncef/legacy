/*E*/
/*  Fichier : $Id: xzah19sp.prc,v 1.4 1996/12/23 14:55:58 gaborit Exp $      Release : $Revision: 1.4 $        Date : $Date: 1996/12/23 14:55:58 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah19sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.  06/12/94        : Cr≈ation
* B.G.  22/12/94        : Modif suite a tests (1.2)
* B.G.	24/10/95	: gestion du sens inverse A57 (1.4)
* C.T.  11/12/94        : ajout animation escargot (1.5)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare l'animation IHM (voies invers≈es par basculement avec bouchon)
*
* Sequence d'appel
* SP    XZAH19_Animer_Bouchons_Inverses
*
* Arguments en entree
* XDY_Entier            va_NumEvt_in	: numero d'un basculement ou d'une queue de bouchon
* XDY_Octet             va_CleEvt_in
* XDC_Booleen		va_TypeFmc_in : indicateur de type d'evenement
* XDY_Octet		va_Type_in	: type de bouchon
* XDY_Autoroute         va_Autoroute_in
* XDY_PR                va_PRDeb_in
* XDY_PR                va_PRFin_in
* XDY_Sens              va_Sens_in
* XDY_Octet             va_VR_in
* XDY_Octet             va_VM2_in
* XDY_Octet             va_VM1_in
* XDY_Octet             va_VL_in
* XDY_Octet             va_BAU_in
* XDY_Octet             va_VR_I_in
* XDY_Octet             va_VM2_I_in
* XDY_Octet             va_VM1_I_in
* XDY_Octet             va_VL_I_in
* XDY_Octet             va_BAU_I_in
* XDY_Octet             va_Suppr_in
*
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* XDC_ARG_INV       : arguments invalides
*
* Conditions d'utilisation
* appel≈e par XZAH;14 ou par XZAH;16
*
------------------------------------------------------*/

use PRC
go

create procedure  XZAH;19
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null,
	@va_TypeFmc_in		smallint	= null,
	@va_Type_in		tinyint		= null,
	@va_Autoroute_in	T_AUTOROUTE	= null,
	@va_PRDeb_in		T_PR		= null,
	@va_PRFin_in		T_PR		= null,
	@va_Sens_in		T_SENS		= null,
	@va_VR_in               tinyint		= null,
	@va_VM2_in              tinyint		= null,
	@va_VM1_in              tinyint		= null,
	@va_VL_in               tinyint		= null,
	@va_BAU_in              tinyint		= null,
	@va_VR_I_in             tinyint		= null,
	@va_VM2_I_in            tinyint		= null,
	@va_VM1_I_in            tinyint		= null,
	@va_VL_I_in             tinyint		= null,
	@va_BAU_I_in            tinyint		= null,
	@va_Suppr_in		bit
as
	declare @vl_Status int, @vl_NumEvt int, @vl_CleEvt tinyint,
		@vl_PRDebut T_PR, @vl_PRFin T_PR, @vl_PRD T_PR, @vl_PRF T_PR, 
		@vl_PR T_PR, @vl_Type tinyint, @vl_Horodate datetime,
		@vl_NumTete int, @vl_CleTete tinyint, @vl_Sens tinyint,
		@vl_TypeFMC smallint, @vl_Longueur int

	/*A Controle des parametres en entree */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV

	/*A Marque les lignes de SYN_BAS_SEG en XDC_SUPPR */
	update EXP..SYN_BAS_SEG
	set ordre=XDC_SUPPR
	where	((@va_TypeFmc_in = XZAEC_FMC_QueueBouchon or
		  @va_TypeFmc_in = XZAEC_FMC_TeteBouchon or
		  @va_TypeFmc_in = XZAEC_FMC_Escargot) and
		 evenement_bou = @va_NumEvt_in and
		 cle_bou = @va_CleEvt_in) or
		(@va_TypeFmc_in = XZAEC_FMC_Basculement and
		 evenement_basc = @va_NumEvt_in and
		 cle_basc = @va_CleEvt_in)

	/*A S'il faut supprimer les voies -> c'est fini */
	if @va_Suppr_in=XDC_VRAI
		return XDC_OK

	/*A Controle des parametres en entree (suite) */
	if @va_Autoroute_in=null or
	   @va_PRDeb_in=null or
	   @va_PRFin_in=null or
	   @va_Sens_in=null or
	   (@va_TypeFmc_in = XZAEC_FMC_Basculement and
	    (@va_VR_in=null or
	    @va_VM2_in=null or
	    @va_VM1_in=null or
	    @va_VL_in=null or
	    @va_BAU_in=null or
	    @va_VR_I_in=null or
	    @va_VM2_I_in=null or
	    @va_VM1_I_in=null or
	    @va_VL_I_in=null or
	    @va_BAU_I_in=null)) or
	   ((@va_TypeFmc_in = XZAEC_FMC_QueueBouchon or
	     @va_TypeFmc_in = XZAEC_FMC_TeteBouchon or
	     @va_TypeFmc_in = XZAEC_FMC_Escargot) and
	    @va_Type_in = null)
		return XDC_ARG_INV

	/*A Si les voies ne sont pas sur section courante : c'est fini */
	if @va_Sens_in<>XDC_SENS_NORD and @va_Sens_in<>XDC_SENS_SUD
		return XDC_OK

	/*A Rechercher les bouchons si l'evenement en entree est un basculement     */
	/*A ou rechercher les basculements si l'evenement en entree est un          */
	/*A bouchon qui ont une intersection de zone (entre bouchon et basculement) */
	if @va_TypeFmc_in = XZAEC_FMC_QueueBouchon or @va_TypeFmc_in = XZAEC_FMC_TeteBouchon or
		@va_TypeFmc_in = XZAEC_FMC_Escargot
	begin

		declare Pointeur_basculement cursor
		for	select  distinct
				EXP..FMC_HIS.numero,
				EXP..FMC_HIS.cle,
				max(EXP..FMC_HIS.horodate_validation)
			from	EXP..FMC_GEN,
				EXP..FMC_HIS
			where	EXP..FMC_GEN.type = XZAEC_FMC_Basculement and
				EXP..FMC_GEN.numero=EXP..FMC_HIS.numero and
				EXP..FMC_GEN.cle=EXP..FMC_HIS.cle and
				EXP..FMC_GEN.debut <> null and
				EXP..FMC_GEN.fin = null
			group by EXP..FMC_HIS.numero,EXP..FMC_HIS.cle
				
		/*B Rechercher s'il y a des basculements dans la zone de ce bouchon */


		open Pointeur_basculement

		fetch Pointeur_basculement into @vl_NumEvt, @vl_CleEvt, @vl_Horodate

		/*B Pour chaque basculement */
		while @@sqlstatus = 0
		begin
			select  @vl_PRFin=localisation_PR_fin
			from	EXP..FMC_BAS
			where	numero=@vl_NumEvt and
				cle=@vl_CleEvt
			
			select	@vl_PRDebut=PR,
				@va_VR_in=VR,
				@va_VM2_in=VM2,
				@va_VM1_in=VM1,
				@va_VL_in=VL,
				@va_BAU_in=BAU,
				@va_VR_I_in=VR_I,
				@va_VM2_I_in=VM2_I,
				@va_VM1_I_in=VM1_I,
				@va_VL_I_in=VL_I,
				@va_BAU_I_in=BAU_I
			from 	EXP..FMC_HIS
			where	numero=@vl_NumEvt and
				cle=@vl_CleEvt and
				horodate_validation=@vl_Horodate and
				autoroute = @va_Autoroute_in and
				sens = @va_Sens_in  and
				((EXP..FMC_HIS.PR < @va_PRFin_in and
				@vl_PRFin > @va_PRDeb_in) or
				(EXP..FMC_HIS.PR > @va_PRDeb_in and
				@vl_PRFin < @va_PRFin_in))
				
			if @@rowcount<>0
			begin

				/*A Remise dans l'ordre PR debut< PR fin */
				if @vl_PRDebut>@vl_PRFin
				begin
					select @vl_PR=@vl_PRFin
					select @vl_PRFin=@vl_PRDebut
					select @vl_PRDebut=@vl_PR
				end

				/*A Inserer les voies inversees pour ce basculement */
				if @va_Sens_in = XDC_SENS_SUD
					exec @vl_Status = XZAH;24 @va_NumEvt_in, @va_CleEvt_in, 
							@va_PRDeb_in, @va_PRFin_in, @va_Type_in,
							@vl_NumEvt, @vl_CleEvt, @vl_PRDebut, @vl_PRFin,
							@va_VR_I_in, @va_VM2_I_in, @va_VM1_I_in, 
							@va_VL_I_in, @va_BAU_I_in, @va_Autoroute_in, 
							XDC_SENS_NORD
				else	exec @vl_Status = XZAH;24 @va_NumEvt_in, @va_CleEvt_in, 
							@va_PRDeb_in, @va_PRFin_in, @va_Type_in,
							@vl_NumEvt, @vl_CleEvt, @vl_PRDebut, @vl_PRFin,
							@va_VR_I_in, @va_VM2_I_in, @va_VM1_I_in, 
							@va_VL_I_in, @va_BAU_I_in, @va_Autoroute_in, 
							XDC_SENS_SUD

				if @vl_Status is null
					return XDC_PRC_INC
			end

			/*! passage au basculement suivant */
			fetch Pointeur_basculement into @vl_NumEvt, @vl_CleEvt, @vl_Horodate
		end
		close Pointeur_basculement
	end
	else
	begin
		/*B Rechercher les bouchons ou escargot qui sont sur la zone de basculement */
		declare Pointeur_bouchon cursor
		for select distinct
			EXP..FMC_HIS.numero,
			EXP..FMC_HIS.cle,
			max(EXP..FMC_HIS.horodate_validation),
			type
		from EXP..FMC_HIS, EXP..FMC_GEN
		where   (EXP..FMC_GEN.type = XZAEC_FMC_QueueBouchon or EXP..FMC_GEN.type = XZAEC_FMC_Escargot) and
			EXP..FMC_GEN.debut <> null and
			EXP..FMC_GEN.fin = null and
			EXP..FMC_HIS.numero = EXP..FMC_GEN.numero and 
			EXP..FMC_HIS.cle = EXP..FMC_GEN.cle and
			autoroute=@va_Autoroute_in
		group by EXP..FMC_HIS.numero, EXP..FMC_HIS.cle

		open Pointeur_bouchon
		fetch Pointeur_bouchon into @vl_NumEvt, @vl_CleEvt, @vl_Horodate, @vl_TypeFMC

		/*B Pour chaque bouchon situe sur la zone de basculement */
		while @@sqlstatus = 0
		begin
			select @vl_PRDebut=null, @vl_PRFin=null
			
			/*A Rechercher le PR queue et le niveau */
			
			select @vl_PRDebut=PR, @vl_Sens=sens, @vl_Longueur = longueur
			from EXP..FMC_HIS
			where	numero=@vl_NumEvt and
				cle=@vl_CleEvt and
				horodate_validation=@vl_Horodate
			
			if (@vl_TypeFMC = XZAEC_FMC_Escargot)
				select @vl_Type=XZAEC_BOU_Escargot
			else
			begin
				select @vl_Type=type
				from EXP..FMC_BOU
				where numero=@vl_NumEvt and
				      cle=@vl_CleEvt and
				      horodate=@vl_Horodate
				      
				select @vl_Type=isnull(@vl_Type,1)
			end
			
			/*A ram…ne le bouchon sur section courante */

			if	@vl_Sens=XDC_SENS_SORTIE_SUD or
				@vl_Sens=XDC_SENS_ENTREE_SUD
			select	@vl_Sens=XDC_SENS_SUD

			if	@vl_Sens=XDC_SENS_SORTIE_NORD or
				@vl_Sens=XDC_SENS_ENTREE_NORD
			select	@vl_Sens=XDC_SENS_NORD
			
			if (@vl_TypeFMC = XZAEC_FMC_Escargot)
			begin
				if (@vl_Sens = XDC_SENS_NORD) or 
					(@vl_Sens=XDC_SENS_SUD and exists ( select * from CFG..RES_AUT_INV where numero = @va_Autoroute_in))
					select @vl_PRFin= @vl_PRDebut - @vl_Longueur
				else	select @vl_PRFin= @vl_PRDebut + @vl_Longueur
			end
			else
			begin
				/*A Rechercher le PR tete */
				
				select  @vl_NumTete=cause,
					@vl_CleTete=cle_cause
				from	EXP..FMC_GEN
				where	numero=@vl_NumEvt and
					cle=@vl_CleEvt
				
				select	@vl_PRFin=PR
				from	EXP..FMC_HIS
				where	numero=@vl_NumTete and
					cle=@vl_CleTete
				having	horodate_validation=max(horodate_validation) and
					numero=@vl_NumTete and
					cle=@vl_CleTete
			end

			/*A Tester si le bouchon correspond */
			
			if	@vl_PRDebut<>null and
				@vl_PRFin<>null and
				@vl_Sens=@va_Sens_in and
				@vl_PRDebut<>@vl_PRFin
			begin
			

				/*A Remise dans l'ordre PR debut< PR fin */
				if @vl_PRDebut>@vl_PRFin
				begin
					select @vl_PR=@vl_PRFin
					select @vl_PRFin=@vl_PRDebut
					select @vl_PRDebut=@vl_PR
				end
				
				if @va_PRFin_in>@va_PRDeb_in
				select @vl_PRD=@va_PRDeb_in,@vl_PRF=@va_PRFin_in
				else
				select @vl_PRD=@va_PRFin_in,@vl_PRF=@va_PRDeb_in
				
				/*A Inserer les voies inversees pour ce bouchon */
				if @va_Sens_in = XDC_SENS_SUD
					exec @vl_Status = XZAH;24 @vl_NumEvt, @vl_CleEvt, 
						@vl_PRDebut, @vl_PRFin, @vl_Type,
						@va_NumEvt_in, @va_CleEvt_in, 
						@vl_PRD, @vl_PRF,
						@va_VR_I_in, @va_VM2_I_in, @va_VM1_I_in, 
						@va_VL_I_in, @va_BAU_I_in, @va_Autoroute_in, 
						XDC_SENS_NORD
				else	exec @vl_Status = XZAH;24 @vl_NumEvt, @vl_CleEvt,
						@vl_PRDebut, @vl_PRFin, @vl_Type,
						@va_NumEvt_in, @va_CleEvt_in, 
						@vl_PRD, @vl_PRF,
						@va_VR_I_in, @va_VM2_I_in, @va_VM1_I_in, 
						@va_VL_I_in, @va_BAU_I_in, @va_Autoroute_in, 
						XDC_SENS_SUD

				if @vl_Status is null
					return XDC_PRC_INC
			end

			/*! passage au bouchon suivant */
			fetch Pointeur_bouchon into @vl_NumEvt, @vl_CleEvt, @vl_Horodate, @vl_TypeFMC
		end
		close Pointeur_bouchon
	end

	return XDC_OK
go
