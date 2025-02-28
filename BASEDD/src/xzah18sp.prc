/*E*/
/*  Fichier : $Id: xzah18sp.prc,v 1.4 1997/05/21 11:10:20 gaborit Exp $      Release : $Revision: 1.4 $        Date : $Date: 1997/05/21 11:10:20 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah18sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm
*
------------------------------------------------------
* HISTORIQUE :
*
* B.G.  02/11/94        : Cr�ation
* B.G.  22/12/94        : ajout else (1.2)
* B.G.	24/10/95	: gestion du sens inverse A57 (1.3)
* P.N.	10/01/97	: correction gestion du sens inverse A57 dem/1372 (1.4)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare l'animation IHM (voies RADT invers�es)
* 
* Sequence d'appel
* SP	XZAH18_Animer_Voies_RADT_Inversees
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
* XDY_Octet		va_Suppr_in
* 
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: arguments invalides
* 
* Conditions d'utilisation
* appel�e par XZAH;16
* 
------------------------------------------------------*/

use PRC
go

create procedure  XZAH;18
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
	@va_BAU_I_in            tinyint		= null,
	@va_Suppr_in		bit
as

/*A declaration des variables */

	declare @vl_voie	tinyint,
		@vl_PR		int,
		@vl_portion	smallint,
		@vl_PRdebP	int, -- d�but portion
		@vl_PRfinP	int, -- fin portion
		@vl_PRdebI	int, -- d�but zone influence
		@vl_PRfinI	int, -- fin zone influence
		@vl_PRdebS	int, -- d�but segment
		@vl_PRfinS	int, -- fin segment
		@vl_sens	tinyint, -- sens segment
		@vl_extension	int,
		@vl_translation int,
		@vl_SensAuto	smallint,
		@vl_station	smallint


/*A Test des param�tres d'appel */

/*B	Test des param�tres nuls */

	if	@va_NumEvt_in=null or
		@va_CleEvt_in=null
	return XDC_NOK

/*A Marque les lignes de SYN_INV_SEG en XDC_SUPPR */

	update EXP..SYN_INV_SEG
	set ordre=XDC_SUPPR
	where	evenement=@va_NumEvt_in and
		cle=@va_CleEvt_in

/*A S'il faut supprimer les voies -> c'est fini */

	if @va_Suppr_in=XDC_VRAI
	return XDC_OK

/*A Test des param�tres d'appel (suite)*/

/*B	Test des param�tres nuls */

	if	@va_Autoroute_in=null or
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
	return XDC_NOK

/*A Si les voies ne sont pas sur section courante : c'est fini */

	if @va_Sens_in<>XDC_SENS_NORD and @va_Sens_in<>XDC_SENS_SUD
	return XDC_OK

/*A Si le sens est nord : echange des variables voies et des PR */

	select @vl_SensAuto=inversee from CFG..RES_AUT_INV where numero=@va_Autoroute_in
	
	if (@va_Sens_in=XDC_SENS_NORD and @vl_SensAuto=1) or 
	   ( @va_Sens_in=XDC_SENS_SUD and @vl_SensAuto=-1)
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

/*A Recherche des segments concern�s */

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
			CFG..RES_INF.sens<>CFG..RES_SEG.sens and -- sens oppos�
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

/*A Pour chaque segment trouv� : */

	while (@@sqlstatus = 0)
	begin

/*A	recherche le PR de fin : debut du suivant ou fin de portion */

		select @vl_PRfinS=min(PR_debut)
		from CFG..RES_SEG
		where	portion=@vl_portion and
			sens=@vl_sens and
			PR_debut>@vl_PRdebS

		select @vl_PRfinS=isnull(@vl_PRfinS,@vl_PRfinP)

/*A	raccourcit le segment � la zone d'influence */

		if (@vl_PRdebS<@vl_PRdebI) select @vl_PRdebS=@vl_PRdebI

		if (@vl_PRfinS>@vl_PRfinI) select @vl_PRfinS=@vl_PRfinI

/*A	v�rifie que le segment est bien dans la zone d'influence et de basculement */

		if	(@vl_PRdebS<@vl_PRfinI) and
			(@vl_PRfinS>@vl_PRdebI) and
			(@vl_PRdebS<@va_PRfin_in) and
			(@vl_PRfinS>@va_PRdeb_in)
		begin

/*A		calcule les coefficients de translation et extension */

/*B			si le d�but est apr�s le d�but du segment : on le prend */

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

/*A		ins�re les voies invers�es en fonction du sens et du nombre de voies */

			if @vl_sens=XDC_SENS_SUD

/*B			cas segment sud */

			begin
			
/*B				on traite en premier la BAU */

				if	@va_BAU_in=XDC_VOIE_SENS_INVERSE
				insert	EXP..SYN_INV_SEG
					(
					evenement,
					cle,
					portion,
					PR,
					sens,
					voie,
					ordre,
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
					XDC_AJOUT,
					@vl_translation,
					@vl_extension,
					@vl_station,
					XDC_SENS_NORD
					)

/*B				cas 1 voie */

				if @vl_voie=1
				begin
					if	@va_VL_in=XDC_VOIE_SENS_INVERSE
					insert	EXP..SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						ordre,
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
						XDC_AJOUT,
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
					insert	EXP..SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						ordre,
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
						XDC_AJOUT,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_NORD
						)
					if	@va_VL_in=XDC_VOIE_SENS_INVERSE
					insert	EXP..SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						ordre,
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
						XDC_AJOUT,
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
					insert	EXP..SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						ordre,
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
						XDC_AJOUT,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_NORD
						)
					if	@va_VM1_in=XDC_VOIE_SENS_INVERSE
					insert	EXP..SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						ordre,
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
						XDC_AJOUT,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_NORD
						)
					if	@va_VL_in=XDC_VOIE_SENS_INVERSE
					insert	EXP..SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						ordre,
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
						XDC_AJOUT,
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
					insert	EXP..SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						ordre,
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
						XDC_AJOUT,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_NORD
						)
					if	@va_VM2_in=XDC_VOIE_SENS_INVERSE
					insert	EXP..SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						ordre,
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
						XDC_AJOUT,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_NORD
						)
					if	@va_VM1_in=XDC_VOIE_SENS_INVERSE
					insert	EXP..SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						ordre,
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
						XDC_AJOUT,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_NORD
						)
					if	@va_VL_in=XDC_VOIE_SENS_INVERSE
					insert	EXP..SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						ordre,
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
						XDC_AJOUT,
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
				insert	EXP..SYN_INV_SEG
					(
					evenement,
					cle,
					portion,
					PR,
					sens,
					voie,
					ordre,
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
					XDC_AJOUT,
					@vl_translation,
					@vl_extension,
					@vl_station,
					XDC_SENS_SUD
					)

/*B				cas 1 voie */

				if @vl_voie=1
				begin
					if	@va_VL_I_in=XDC_VOIE_SENS_INVERSE
					insert	EXP..SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						ordre,
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
						XDC_AJOUT,
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
					insert	EXP..SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						ordre,
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
						XDC_AJOUT,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_SUD
						)
					if	@va_VL_I_in=XDC_VOIE_SENS_INVERSE
					insert	EXP..SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						ordre,
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
						XDC_AJOUT,
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
					insert	EXP..SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						ordre,
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
						XDC_AJOUT,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_SUD
						)
					if	@va_VM1_I_in=XDC_VOIE_SENS_INVERSE
					insert	EXP..SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						ordre,
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
						XDC_AJOUT,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_SUD
						)
					if	@va_VL_I_in=XDC_VOIE_SENS_INVERSE
					insert	EXP..SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						ordre,
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
						XDC_AJOUT,
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
					insert	EXP..SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						ordre,
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
						XDC_AJOUT,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_SUD
						)
					if	@va_VM2_I_in=XDC_VOIE_SENS_INVERSE
					insert	EXP..SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						ordre,
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
						XDC_AJOUT,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_SUD
						)
					if	@va_VM1_I_in=XDC_VOIE_SENS_INVERSE
					insert	EXP..SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						ordre,
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
						XDC_AJOUT,
						@vl_translation,
						@vl_extension,
						@vl_station,
						XDC_SENS_SUD
						)
					if	@va_VL_I_in=XDC_VOIE_SENS_INVERSE
					insert	EXP..SYN_INV_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						ordre,
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
						XDC_AJOUT,
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
