/*E*/
/*  Fichier : $Id: xzaf12sp.prc,v 1.4 1995/11/02 11:54:51 gaborit Exp $      Release : $Revision: 1.4 $        Date : $Date: 1995/11/02 11:54:51 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzaf12sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm de formation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	12/01/95	: Creation 
* C.T.	07/02/95	: Ajout dans la clause le spid (V 1.3)
* B.G.	24/10/95	: gestion du sens inverse A57 (1.4)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare l'animation IHM (voies invers≈es par basculement avec bouchon)
*
* Sequence d'appel
* SP    XZAF12_Animer_Bouchon_Inverse_Evt
*
* Arguments en entree
* XDY_Entier            va_NumEvt_in	: numero d'un basculement ou d'une queue de bouchon
* XDY_Octet             va_CleEvt_in
* XDC_Booleen		va_BouchonBascul_in : indicateur de type d'evenement
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
*
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* XDC_ARG_INV       : arguments invalides
*
* Conditions d'utilisation
* appel≈e par XZAF;11
*
------------------------------------------------------*/

use PRC
go

create procedure  XZAF;12
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null,
	@va_BouchonBascul_in	tinyint		= null,
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
	@va_BAU_I_in            tinyint		= null
as
	declare @vl_Status int, @vl_NumEvt int, @vl_CleEvt tinyint,
		@vl_PRDebut T_PR, @vl_PRFin T_PR, @vl_PRD T_PR, @vl_PRF T_PR, @vl_PR T_PR, @vl_Type tinyint, @vl_Horodate datetime,
		@vl_NumTete int, @vl_CleTete tinyint, @vl_Sens tinyint

	/*A Controle des parametres en entree */
	if @va_NumEvt_in = null or @va_CleEvt_in = null or
	   @va_Autoroute_in=null or
	   @va_PRDeb_in=null or
	   @va_PRFin_in=null or
	   @va_Sens_in=null or
	   (@va_BouchonBascul_in = XZAEC_FMC_Basculement and
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
	   ((@va_BouchonBascul_in = XZAEC_FMC_TeteBouchon or
	     @va_BouchonBascul_in = XZAEC_FMC_QueueBouchon) and
	    @va_Type_in = null)
		return XDC_ARG_INV

	/*A Rechercher les bouchons si l'evenement en entree est un basculement     */
	/*A ou rechercher les basculements si l'evenement en entree est un          */
	/*A bouchon qui ont une intersection de zone (entre bouchon et basculement) */
	if @va_BouchonBascul_in = XZAEC_FMC_TeteBouchon or
	   @va_BouchonBascul_in = XZAEC_FMC_QueueBouchon
	begin

		declare Pointeur_basculement cursor
		for	select  distinct
				EVT_FRM.numero,
				EVT_FRM.cle,
				EVT_FRM.horodate_validation
			from	HIS..FMC_GEN,
				EVT_FRM
			where	HIS..FMC_GEN.type = XZAEC_FMC_Basculement and
				EVT_FRM.spid=@@spid and
				HIS..FMC_GEN.numero=EVT_FRM.numero and
				HIS..FMC_GEN.cle=EVT_FRM.cle and
				HIS..FMC_GEN.debut <> null and
				HIS..FMC_GEN.fin = null
				
		/*B Rechercher s'il y a des basculements dans la zone de ce bouchon */


		open Pointeur_basculement

		fetch Pointeur_basculement into @vl_NumEvt, @vl_CleEvt, @vl_Horodate

		/*B Pour chaque basculement */
		while @@sqlstatus = 0
		begin
			select  @vl_PRFin=localisation_PR_fin
			from	HIS..FMC_BAS
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
			from 	EVT_FRM
			where	numero=@vl_NumEvt and
				cle=@vl_CleEvt and
				spid=@@spid and
				horodate_validation=@vl_Horodate and
				autoroute = @va_Autoroute_in and
				sens = @va_Sens_in  and
				((PR < @va_PRFin_in and
				@vl_PRFin > @va_PRDeb_in) or
				(PR > @va_PRDeb_in and
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
					exec @vl_Status = XZAF;13 @va_NumEvt_in, @va_CleEvt_in, 
							@va_PRDeb_in, @va_PRFin_in, @va_Type_in,
							@vl_NumEvt, @vl_CleEvt, @vl_PRDebut, @vl_PRFin,
							@va_VR_I_in, @va_VM2_I_in, @va_VM1_I_in, 
							@va_VL_I_in, @va_BAU_I_in, @va_Autoroute_in, 
							XDC_SENS_NORD
				else	exec @vl_Status = XZAF;13 @va_NumEvt_in, @va_CleEvt_in, 
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
		/*B Rechercher les bouchons qui sont sur la zone de basculement */
		declare Pointeur_bouchon cursor
		for select distinct
			EVT_FRM.numero,
			EVT_FRM.cle,
			EVT_FRM.horodate_validation
		from EVT_FRM, HIS..FMC_GEN
		where   HIS..FMC_GEN.type = XZAEC_FMC_QueueBouchon and
			HIS..FMC_GEN.debut <> null and
			HIS..FMC_GEN.fin = null and
			EVT_FRM.spid=@@spid and
			EVT_FRM.numero = HIS..FMC_GEN.numero and 
			EVT_FRM.cle = HIS..FMC_GEN.cle

		open Pointeur_bouchon
		fetch Pointeur_bouchon into @vl_NumEvt, @vl_CleEvt, @vl_Horodate

		/*B Pour chaque bouchon situe sur la zone de basculement */
		while @@sqlstatus = 0
		begin
			select @vl_PRDebut=null, @vl_PRFin=null
			
			/*A Rechercher le PR queue et le niveau */
			
			select @vl_PRDebut=PR, @vl_Sens=sens
			from EVT_FRM
			where	horodate_validation=@vl_Horodate and
				spid=@@spid and
				autoroute=@va_Autoroute_in
			
			select @vl_Type=type
			from HIS..FMC_BOU
			where numero=@vl_NumEvt and
			      cle=@vl_CleEvt and
			      horodate=@vl_Horodate
			      
			select @vl_Type=isnull(@vl_Type,1)
			
			/*A ram…ne le bouchon sur section courante */

			if	@vl_Sens=XDC_SENS_SORTIE_SUD or
				@vl_Sens=XDC_SENS_ENTREE_SUD
			select	@vl_Sens=XDC_SENS_SUD

			if	@vl_Sens=XDC_SENS_SORTIE_NORD or
				@vl_Sens=XDC_SENS_ENTREE_NORD
			select	@vl_Sens=XDC_SENS_NORD
			
			/*A Rechercher le PR tete */
			
			select  @vl_NumTete=cause,
				@vl_CleTete=cle_cause
			from	HIS..FMC_GEN
			where	numero=@vl_NumEvt and
				cle=@vl_CleEvt
			
			select	@vl_PRFin=PR
			from	EVT_FRM
			where	numero=@vl_NumTete and
				cle=@vl_CleTete and
				spid=@@spid

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
					exec @vl_Status = XZAF;13 @vl_NumEvt, @vl_CleEvt, 
						@vl_PRDebut, @vl_PRFin, @vl_Type,
						@va_NumEvt_in, @va_CleEvt_in, 
						@vl_PRD, @vl_PRF,
						@va_VR_I_in, @va_VM2_I_in, @va_VM1_I_in, 
						@va_VL_I_in, @va_BAU_I_in, @va_Autoroute_in, 
						XDC_SENS_NORD
				else	exec @vl_Status = XZAF;13 @vl_NumEvt, @vl_CleEvt,
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
			fetch Pointeur_bouchon into @vl_NumEvt, @vl_CleEvt, @vl_Horodate
		end
		close Pointeur_bouchon
	end

	return XDC_OK
go
