/*E*/
/*  Fichier : $Id: xzaf09sp.prc,v 1.2 1995/11/02 11:52:35 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/11/02 11:52:35 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzaf09sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm de la formation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.  11/01/95        : Cr≈ation
* B.G.	24/10/95	: gestion du sens inverse A57 (1.2)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare l'animation IHM (voies RADT invers≈es)
* 
* Sequence d'appel
* SP	XZAF09_Animer_Voies_RADT_Inversees
* 
* Arguments en entree
* XDY_Evt		va_NumEvt_in
* XDY_Autoroute		va_Autoroute_in
* XDY_PR		va_PR_deb_in
* XDY_PR		va_PR_fin_in
* XDY_Sens		va_Sens_in
* XDY_Octet		va_VR_in
* XDY_Octet		va_VM2_in
* XDY_Octet		va_VM1_in
* XDY_Octet		va_VL_in
* XDY_Octet		va_BAU_in
* XDY_Octet		va_VR_I_in
* XDY_Octet		va_VM2_I_in
* XDY_Octet		va_VM1_I_in
* XDY_Octet		va_VL_I_in
* XDY_Octet		va_BAU_I_in
* 
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: arguments invalides
* 
* Conditions d'utilisation
* appel≈e par XZAF;08
* 
------------------------------------------------------*/

use PRC
go

create table #SYN_INV_SEG
(
	evenement	int,
	cle		tinyint,
	portion		smallint,
	PR		int,
	sens		tinyint,
	voie		tinyint,
	translation	int,
	extension	int,
	station		smallint,
	sens_station	tinyint
)
go

create procedure  XZAF;09
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null,
	@va_Autoroute_in	T_AUTOROUTE	= null,
	@va_PRdeb_in		T_PR		= null,
	@va_PRfin_in		T_PR		= null,
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

/*A declaration des variables */

	declare @vl_voie	tinyint,
		@vl_PR		int,
		@vl_portion	smallint,
		@vl_PRdebP	int, -- d≈but portion
		@vl_PRfinP	int, -- fin portion
		@vl_PRdebI	int, -- d≈but zone influence
		@vl_PRfinI	int, -- fin zone influence
		@vl_PRdebS	int, -- d≈but segment
		@vl_PRfinS	int, -- fin segment
		@vl_sens	tinyint, -- sens segment
		@vl_extension	int,
		@vl_translation int,
		@vl_station	smallint


/*A Test des param…tres d'appel */

/*B	Test des param…tres nuls */

	if @va_NumEvt_in=null or
	   @va_CleEvt_in=null or
	   @va_Autoroute_in=null or
	   @va_PRdeb_in=null or
	   @va_PRfin_in=null or
	   @va_Sens_in=null or
	   @va_VR_in=null or 
	   @va_VM2_in=null or
	   @va_VM1_in=null or
	   @va_VL_in=null or
	   @va_BAU_in=null or
	   @va_VR_I_in=null or
	   @va_VM2_I_in=null or
	   @va_VM1_I_in=null or
	   @va_VL_I_in=null or
	   @va_BAU_I_in=null
		return XDC_ARG_INV

/*A Si le sens est nord : echange des variables voies et des PR */

	if @va_Sens_in=XDC_SENS_NORD
	begin
		select @vl_voie=@va_VR_in
		select @va_VR_in=@va_VR_I_in
		select @va_VR_I_in=@vl_voie

		select @vl_voie=@va_VM2_in
		select @va_VM2_in=@va_VM2_I_in
		select @va_VM2_I_in=@vl_voie

		select @vl_voie=@va_VM1_in
		select @va_VM1_in=@va_VM1_I_in
		select @va_VM1_I_in=@vl_voie

		select @vl_voie=@va_VL_in
		select @va_VL_in=@va_VL_I_in
		select @va_VL_I_in=@vl_voie

		select @vl_voie=@va_BAU_in
		select @va_BAU_in=@va_BAU_I_in
		select @va_BAU_I_in=@vl_voie
	end
	
/*A Si le sens est nord ou sur A57 */

	if	@va_PRdeb_in>@va_PRfin_in
	begin
		select @vl_PR=@va_PRdeb_in
		select @va_PRdeb_in=@va_PRfin_in
		select @va_PRfin_in=@vl_PR
	end

/*A Recherche des segments concern≈s */

	declare Segment cursor for
		select	CFG..RES_POR.numero,
			CFG..RES_INF.PR_debut,
			CFG..RES_INF.PR_fin,
			CFG..RES_POR.PR_debut,
			CFG..RES_POR.PR_fin,
			CFG..RES_SEG.PR_debut,
			CFG..RES_SEG.sens,
			CFG..RES_SEG.nombre_de_voies,
			CFG..RES_INF.station

		from	CFG..RES_POR, CFG..RES_SEG, CFG..RES_INF

		where	CFG..RES_POR.numero=CFG..RES_INF.portion and 
			CFG..RES_INF.portion=CFG..RES_SEG.portion and -- meme portion
			CFG..RES_POR.autoroute=@va_Autoroute_in and -- autoroute
			CFG..RES_INF.sens<>CFG..RES_SEG.sens and -- sens oppos≈
			CFG..RES_INF.PR_fin>=@va_PRdeb_in and
			CFG..RES_INF.PR_debut<@va_PRfin_in and
			CFG..RES_SEG.PR_debut<@va_PRfin_in

	open Segment
	
	fetch Segment into	@vl_portion,
				@vl_PRdebI,
				@vl_PRfinI,
				@vl_PRdebP,
				@vl_PRfinP,
				@vl_PRdebS,
				@vl_sens,
				@vl_voie,
				@vl_station

/*A Pour chaque segment trouv≈ : */

	while (@@sqlstatus = 0)
	begin

/*A	recherche le PR de fin : debut du suivant ou fin de portion */

		select @vl_PRfinS=min(PR_debut)
		from CFG..RES_SEG
		where	portion=@vl_portion and
			sens=@vl_sens and
			PR_debut>@vl_PRdebS

		select @vl_PRfinS=isnull(@vl_PRfinS,@vl_PRfinP)

/*A	raccourcit le segment » la zone d'influence */

		if (@vl_PRdebS<@vl_PRdebI) select @vl_PRdebS=@vl_PRdebI

		if (@vl_PRfinS>@vl_PRfinI) select @vl_PRfinS=@vl_PRfinI

/*A	v≈rifie que le segment est bien dans la zone d'influence et de basculement */

		if	(@vl_PRdebS<@vl_PRfinI) and
			(@vl_PRfinS>@vl_PRdebI) and
			(@vl_PRdebS<@va_PRfin_in) and
			(@vl_PRfinS>@va_PRdeb_in)
		begin

/*A		calcule les coefficients de translation et extension */

/*B			si le d≈but est apr…s le d≈but du segment : on le prend */

			if @vl_PRdebS<@va_PRdeb_in
			select @vl_PR=@va_PRdeb_in
			else
			select @vl_PR=@vl_PRdebS

/*B			si la fin est avant la fin du segment : on la prend */

			if @vl_PRfinS>@va_PRfin_in
			select @vl_PRfinS=@va_PRfin_in

/*B			Translation */

			select @vl_translation=
				10000
				*((@vl_PR+@vl_PRfinS)/2-@vl_PRdebP)
				/(@vl_PRfinP-@vl_PRdebP)

/*B			Extension */

			select @vl_extension=
				10000
				*(@vl_PRfinS-@vl_PR)
				/(@vl_PRfinP-@vl_PRdebP)

/*A		ins…re les voies invers≈es en fonction du sens et du nombre de voies */

			if @vl_sens=XDC_SENS_SUD

/*B			cas segment sud */

			begin
			
/*B				on traite en premier la BAU */

				if	@va_BAU_in=XDC_VOIE_SENS_INVERSE
				insert	#SYN_INV_SEG
					(
					evenement,
					cle,
					portion,
					PR,
					sens,
					voie,
					translation,
					extension,
					station,
					sens_station
					)
					values
					(
					@va_NumEvt_in,
					@va_CleEvt_in,
					@vl_portion,
					@vl_PRdebS,
					@vl_sens,
					6-@vl_voie,
					@vl_translation,
					@vl_extension,
					@vl_station,
					XDC_SENS_NORD
					)

/*B				cas 1 voie */

				if @vl_voie=1
				begin
					if	@va_VL_in=XDC_VOIE_SENS_INVERSE
					insert	#SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						translation,
						extension,
						station,
						sens_station
						)
						values
						(
						@va_NumEvt_in,
						@va_CleEvt_in,
						@vl_portion,
						@vl_PRdebS,
						@vl_sens,
						6,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_NORD
						)
				end
				else

/*B				cas 2 voie */

				if @vl_voie=2
				begin
					if	@va_VR_in=XDC_VOIE_SENS_INVERSE
					insert	#SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						translation,
						extension,
						station,
						sens_station
						)
						values
						(
						@va_NumEvt_in,
						@va_CleEvt_in,
						@vl_portion,
						@vl_PRdebS,
						@vl_sens,
						6,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_NORD
						)
					if	@va_VL_in=XDC_VOIE_SENS_INVERSE
					insert	#SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						translation,
						extension,
						station,
						sens_station
						)
						values
						(
						@va_NumEvt_in,
						@va_CleEvt_in,
						@vl_portion,
						@vl_PRdebS,
						@vl_sens,
						5,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_NORD
						)
				end
				else

/*B				cas 3 voie */

				if @vl_voie=3
				begin
					if	@va_VR_in=XDC_VOIE_SENS_INVERSE
					insert	#SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						translation,
						extension,
						station,
						sens_station
						)
						values
						(
						@va_NumEvt_in,
						@va_CleEvt_in,
						@vl_portion,
						@vl_PRdebS,
						@vl_sens,
						6,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_NORD
						)
					if	@va_VM1_in=XDC_VOIE_SENS_INVERSE
					insert	#SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						translation,
						extension,
						station,
						sens_station
						)
						values
						(
						@va_NumEvt_in,
						@va_CleEvt_in,
						@vl_portion,
						@vl_PRdebS,
						@vl_sens,
						5,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_NORD
						)
					if	@va_VL_in=XDC_VOIE_SENS_INVERSE
					insert	#SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						translation,
						extension,
						station,
						sens_station
						)
						values
						(
						@va_NumEvt_in,
						@va_CleEvt_in,
						@vl_portion,
						@vl_PRdebS,
						@vl_sens,
						4,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_NORD
						)
				end
				else

/*B				cas 4 voie */

				if @vl_voie=4
				begin
					if	@va_VR_in=XDC_VOIE_SENS_INVERSE
					insert	#SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						translation,
						extension,
						station,
						sens_station
						)
						values
						(
						@va_NumEvt_in,
						@va_CleEvt_in,
						@vl_portion,
						@vl_PRdebS,
						@vl_sens,
						6,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_NORD
						)
					if	@va_VM2_in=XDC_VOIE_SENS_INVERSE
					insert	#SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						translation,
						extension,
						station,
						sens_station
						)
						values
						(
						@va_NumEvt_in,
						@va_CleEvt_in,
						@vl_portion,
						@vl_PRdebS,
						@vl_sens,
						5,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_NORD
						)
					if	@va_VM1_in=XDC_VOIE_SENS_INVERSE
					insert	#SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						translation,
						extension,
						station,
						sens_station
						)
						values
						(
						@va_NumEvt_in,
						@va_CleEvt_in,
						@vl_portion,
						@vl_PRdebS,
						@vl_sens,
						4,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_NORD
						)
					if	@va_VL_in=XDC_VOIE_SENS_INVERSE
					insert	#SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						translation,
						extension,
						station,
						sens_station
						)
						values
						(
						@va_NumEvt_in,
						@va_CleEvt_in,
						@vl_portion,
						@vl_PRdebS,
						@vl_sens,
						3,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_NORD
						)
				end
			end
			else

/*B			cas segment nord */

			begin
			
/*B				on traite en premier la BAU */

				if	@va_BAU_I_in=XDC_VOIE_SENS_INVERSE
				insert	#SYN_INV_SEG
					(
					evenement,
					cle,
					portion,
					PR,
					sens,
					voie,
					translation,
					extension,
					station,
					sens_station
					)
					values
					(
					@va_NumEvt_in,
					@va_CleEvt_in,
					@vl_portion,
					@vl_PRdebS,
					@vl_sens,
					6-@vl_voie,
					@vl_translation,
					@vl_extension,
					@vl_station,
					XDC_SENS_SUD
					)

/*B				cas 1 voie */

				if @vl_voie=1
				begin
					if	@va_VL_I_in=XDC_VOIE_SENS_INVERSE
					insert	#SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						translation,
						extension,
						station,
						sens_station
						)
						values
						(
						@va_NumEvt_in,
						@va_CleEvt_in,
						@vl_portion,
						@vl_PRdebS,
						@vl_sens,
						6,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_SUD
						)
				end
				else

/*B				cas 2 voie */

				if @vl_voie=2
				begin
					if	@va_VR_I_in=XDC_VOIE_SENS_INVERSE
					insert	#SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						translation,
						extension,
						station,
						sens_station
						)
						values
						(
						@va_NumEvt_in,
						@va_CleEvt_in,
						@vl_portion,
						@vl_PRdebS,
						@vl_sens,
						6,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_SUD
						)
					if	@va_VL_I_in=XDC_VOIE_SENS_INVERSE
					insert	#SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						translation,
						extension,
						station,
						sens_station
						)
						values
						(
						@va_NumEvt_in,
						@va_CleEvt_in,
						@vl_portion,
						@vl_PRdebS,
						@vl_sens,
						5,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_SUD
						)
				end
				else

/*B				cas 3 voie */

				if @vl_voie=3
				begin
					if	@va_VR_I_in=XDC_VOIE_SENS_INVERSE
					insert	#SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						translation,
						extension,
						station,
						sens_station
						)
						values
						(
						@va_NumEvt_in,
						@va_CleEvt_in,
						@vl_portion,
						@vl_PRdebS,
						@vl_sens,
						6,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_SUD
						)
					if	@va_VM1_I_in=XDC_VOIE_SENS_INVERSE
					insert	#SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						translation,
						extension,
						station,
						sens_station
						)
						values
						(
						@va_NumEvt_in,
						@va_CleEvt_in,
						@vl_portion,
						@vl_PRdebS,
						@vl_sens,
						5,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_SUD
						)
					if	@va_VL_I_in=XDC_VOIE_SENS_INVERSE
					insert	#SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						translation,
						extension,
						station,
						sens_station
						)
						values
						(
						@va_NumEvt_in,
						@va_CleEvt_in,
						@vl_portion,
						@vl_PRdebS,
						@vl_sens,
						4,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_SUD
						)
				end
				else

/*B				cas 4 voie */

				if @vl_voie=4
				begin
					if	@va_VR_I_in=XDC_VOIE_SENS_INVERSE
					insert	#SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						translation,
						extension,
						station,
						sens_station
						)
						values
						(
						@va_NumEvt_in,
						@va_CleEvt_in,
						@vl_portion,
						@vl_PRdebS,
						@vl_sens,
						6,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_SUD
						)
					if	@va_VM2_I_in=XDC_VOIE_SENS_INVERSE
					insert	#SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						translation,
						extension,
						station,
						sens_station
						)
						values
						(
						@va_NumEvt_in,
						@va_CleEvt_in,
						@vl_portion,
						@vl_PRdebS,
						@vl_sens,
						5,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_SUD
						)
					if	@va_VM1_I_in=XDC_VOIE_SENS_INVERSE
					insert	#SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						translation,
						extension,
						station,
						sens_station
						)
						values
						(
						@va_NumEvt_in,
						@va_CleEvt_in,
						@vl_portion,
						@vl_PRdebS,
						@vl_sens,
						4,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_SUD
						)
					if	@va_VL_I_in=XDC_VOIE_SENS_INVERSE
					insert	#SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						translation,
						extension,
						station,
						sens_station
						)
						values
						(
						@va_NumEvt_in,
						@va_CleEvt_in,
						@vl_portion,
						@vl_PRdebS,
						@vl_sens,
						3,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_SUD
						)
				end
			end
		end

/*A	passe au segment suivant */

		fetch Segment into	@vl_portion,
					@vl_PRdebI,
					@vl_PRfinI,
					@vl_PRdebP,
					@vl_PRfinP,
					@vl_PRdebS,
					@vl_sens,
					@vl_voie,
					@vl_station
		
	end
	return XDC_OK
go

drop table #SYN_INV_SEG
go
