/*E*/
/*  Fichier : $Id: xzah34sp.prc,v 1.2 2018/06/27 11:17:21 devgfi Exp $      Release : $Revision: 1.2 $        Date : $Date: 2018/06/27 11:17:21 $
------------------------------------------------------
* STERIA *  PROJET PASTRE
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah34sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm
*
------------------------------------------------------
* HISTORIQUE :
*
* Hilmarcher 16/03/05 : Creation
* JPL	26/06/18 : Ajout 'drop procedure'  1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare l'animation IHM (bouchons par segment)
* 
* Sequence d'appel
* SP	XZAH34_Animer_Voies_Bouchon
* 
* Arguments en entree
* XDY_Evt		va_NumEvt_in
* XDY_Cle		va_CleEvt_in
* XDY_Booleen           va_TeteQueue_in
* XDY_Autoroute		va_Autoroute_in
* XDY_PR		va_PR_in
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
* appel≈e par XZAH;30
* 
------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAH34' and type = 'P')
	drop procedure XZAH34
go


create procedure  XZAH34
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null,
	@va_TeteQueue_in	bit,
	@va_Autoroute_in	T_AUTOROUTE	= null,
	@va_PR_in		T_PR		= null,
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
		@vl_PRdeb	int, -- debut bouchon
		@vl_PRfin	int, -- fin bouchon
		@vl_NumTete     int,
		@vl_CleTete     int,
		@vl_PRdebP	int, -- d≈but portion
		@vl_PRfinP	int, -- fin portion
		@vl_PRdebS	int, -- d≈but segment
		@vl_PRfinS	int, -- fin segment
		@vl_sens	tinyint, -- sens segment
		@vl_extension	int,
		@vl_translation int,
		@vl_Niveau      tinyint

/*A Test des param…tres d'appel */

/*B	Test des param…tres nuls */

	if	@va_NumEvt_in=null or
		@va_CleEvt_in=null
	return XDC_NOK

/*A Marque les lignes de SYN_BOU_SEG en XDC_SUPPR */

	update EXP..SYN_BOU_SEG
	set ordre=XDC_SUPPR
	where	evenement=@va_NumEvt_in and
		cle=@va_CleEvt_in

/*A S'il faut supprimer les voies -> c'est fini */

	if @va_Suppr_in=XDC_VRAI
	return XDC_OK

/*A Test des param…tres d'appel (suite)*/

/*B	Test des param…tres nuls */

	if	@va_Autoroute_in=null or
		@va_PR_in=null or
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

/*A Si le sens est nord : echange des variables voies */

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
	
/*Hilmarcher vl_PRdeb et vl_PRfin */
/*A s'il s'agit d'une t¡te, lecture de la queue */
	if      @va_TeteQueue_in=XZAHC_Tete
	begin
		
			select  @vl_NumTete=@va_NumEvt_in,
				@vl_CleTete=@va_CleEvt_in,
				@vl_PRfin=@va_PR_in

/*B     lecture du numero de la queue */
		select  @va_NumEvt_in=numero,
			@va_CleEvt_in=cle
		from    EXP..FMC_GEN
			where   cause=@vl_NumTete and
			cle_cause=@vl_CleTete and
			type=XZAEC_FMC_QueueBouchon

		if @@rowcount<>1 return XDC_NOK -- bouchon sans queue

/*B     lecture du PR de la queue */
		select  @vl_PRdeb=PR
		from    EXP..FMC_HIS
		where   numero=@va_NumEvt_in and
			cle=@va_CleEvt_in
		having  horodate_validation=max(horodate_validation) and
			numero=@va_NumEvt_in and
			cle=@va_CleEvt_in

		if @@rowcount<>1 return XDC_NOK -- historique invalide
	end

/*A s'il s'agit d'une queue, lecture de la t¡te */
	
	else
	begin
		select @vl_PRdeb=@va_PR_in

/*B     lecture du numero de la tete */

		select  @vl_NumTete=cause,
			@vl_CleTete=cle_cause
		from    EXP..FMC_GEN
		where   numero=@va_NumEvt_in and
			cle=@va_CleEvt_in

		if @@rowcount<>1 return XDC_NOK -- bouchon sans tete

/*B     lecture du PR de la tete */
	
		select  @vl_PRfin=PR
		from    EXP..FMC_HIS
		where   numero=@vl_NumTete and
			cle=@vl_CleTete
		having  horodate_validation=max(horodate_validation) and
			numero=@vl_NumTete and
			cle=@vl_CleTete

		if @@rowcount<>1 return XDC_NOK -- historique invalide
	end
			
/*fin hilmarcher */

/*hilmarcher vl_niveau */
/*A lecture du niveau de bouchon */

	select  @vl_Niveau=isnull(type,1)
	from    EXP..FMC_BOU
	where   numero=@va_NumEvt_in and
		cle=@va_CleEvt_in
	having  horodate=max(horodate) and
		numero=@va_NumEvt_in and
		cle=@va_CleEvt_in

	if @@rowcount<>1 return XDC_NOK -- historique bouchon invalide

/*A Si le sens est nord ou sur A57 : echange des PR */
	
	if @va_PR_in>@vl_PRfin
	begin
		select @vl_PR=@vl_PRdeb
		select @vl_PRdeb=@vl_PRfin
		select @vl_PRfin=@vl_PR
	end

/*A Recherche des segments concern≈s */
	declare Segment cursor for
		select	CFG..RES_POR.numero,
			CFG..RES_POR.PR_debut,
			CFG..RES_POR.PR_fin,
			CFG..RES_SEG.PR_debut,
			CFG..RES_SEG.sens,
			CFG..RES_SEG.nombre_de_voies

		from	CFG..RES_POR, CFG..RES_SEG

		where	CFG..RES_POR.numero=CFG..RES_SEG.portion and
			CFG..RES_POR.autoroute=@va_Autoroute_in and
			CFG..RES_POR.PR_fin>=@vl_PRdeb and
			CFG..RES_POR.PR_debut<@vl_PRfin and
			CFG..RES_SEG.PR_debut<@vl_PRfin

	open Segment
	
	fetch Segment into	@vl_portion,
				@vl_PRdebP,
				@vl_PRfinP,
				@vl_PRdebS,
				@vl_sens,
				@vl_voie

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

/*A	v≈rifie que le segment est bien dans la zone */

		if (@vl_PRdebS<@vl_PRfin) and (@vl_PRfinS>@vl_PRdeb)
		begin

/*A		calcule les coefficients de translation et extension */

/*B			si le d≈but est apr…s le d≈but du segment : on le prend */

			if @vl_PRdebS<@vl_PRdeb
			select @vl_PR=@vl_PRdeb
			else
			select @vl_PR=@vl_PRdebS

/*B			si la fin est avant la fin du segment : on la prend */

			if @vl_PRfinS>@vl_PRfin
			select @vl_PRfinS=@vl_PRfin

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

/*A		ins…re les voies neutralis≈es en fonction du sens et du nombre de voies */

			if @vl_sens=XDC_SENS_SUD

/*B			cas segment sud */

			begin
			
/*B				on traite en premier la BAU */

				if	@va_BAU_in=XDC_VOIE_BLOQUEE
				insert	EXP..SYN_BOU_SEG
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
					niveau
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
					@vl_Niveau
					)

/*B				cas 1 voie */

				if @vl_voie=1
				begin
					if	@va_VL_in=XDC_VOIE_BLOQUEE 
					insert	EXP..SYN_BOU_SEG
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
						niveau
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
						@vl_Niveau
						)
				end
				else

/*B				cas 2 voie */

				if @vl_voie=2
				begin
					if	@va_VR_in=XDC_VOIE_BLOQUEE
					insert	EXP..SYN_BOU_SEG
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
						niveau
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
						@vl_Niveau
						)
					if	@va_VL_in=XDC_VOIE_BLOQUEE
					insert	EXP..SYN_BOU_SEG
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
						niveau
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
						@vl_Niveau
						)
				end
				else

/*B				cas 3 voie */

				if @vl_voie=3
				begin
					if	@va_VR_in=XDC_VOIE_BLOQUEE
					insert	EXP..SYN_BOU_SEG
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
						niveau
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
						@vl_Niveau
						)
					
					/*Hilmarcher: remplissage voies medianes */
					if	(@va_VR_in=XDC_VOIE_BLOQUEE AND  @va_VL_in=XDC_VOIE_BLOQUEE)
						select @va_VM1_in=XDC_VOIE_BLOQUEE

					if	@va_VM1_in=XDC_VOIE_BLOQUEE
					insert	EXP..SYN_BOU_SEG
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
						niveau
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
						@vl_Niveau
						)
					if	@va_VL_in=XDC_VOIE_BLOQUEE
						insert	EXP..SYN_BOU_SEG
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
						niveau
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
						@vl_Niveau
						)
				end
				else

/*B				cas 4 voie */

				if @vl_voie=4
				begin
					if	@va_VR_in=XDC_VOIE_BLOQUEE
					insert	EXP..SYN_BOU_SEG
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
						niveau
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
						@vl_Niveau
						)
	                                
				        /*Hilmarcher: remplissage voies medianes */
                                        if      (@va_VR_in=XDC_VOIE_BLOQUEE AND  @va_VL_in=XDC_VOIE_BLOQUEE)
                                        begin 
						select @va_VM1_in=XDC_VOIE_BLOQUEE
						select @va_VM2_in=XDC_VOIE_BLOQUEE
					end
					
					if	@va_VM2_in=XDC_VOIE_BLOQUEE
					insert	EXP..SYN_BOU_SEG
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
						niveau
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
						@vl_Niveau
						)
					if	@va_VM1_in=XDC_VOIE_BLOQUEE
					insert	EXP..SYN_BOU_SEG
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
						niveau
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
						@vl_Niveau
						)
					if	@va_VL_in=XDC_VOIE_BLOQUEE
					insert	EXP..SYN_BOU_SEG
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
						niveau
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
						@vl_Niveau
						)
				end
			end

/*B			cas segment nord */

			else
			begin
/*B				on traite en premier la BAU */

				if	@va_BAU_I_in=XDC_VOIE_BLOQUEE
				insert	EXP..SYN_BOU_SEG
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
					niveau
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
					@vl_Niveau
					)

/*B				cas 1 voie */

				if @vl_voie=1
				begin
					if	@va_VL_I_in=XDC_VOIE_BLOQUEE
					insert	EXP..SYN_BOU_SEG
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
						niveau
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
						@vl_Niveau
						)
				end
				else

/*B				cas 2 voie */

				if @vl_voie=2
				begin
					if	@va_VR_I_in=XDC_VOIE_BLOQUEE
					insert	EXP..SYN_BOU_SEG
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
						niveau
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
						@vl_Niveau
						)
					if	@va_VL_I_in=XDC_VOIE_BLOQUEE
					insert	EXP..SYN_BOU_SEG
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
						niveau
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
						@vl_Niveau
						)
				end
				else

/*B				cas 3 voie */

				if @vl_voie=3
				begin
					if	@va_VR_I_in=XDC_VOIE_BLOQUEE
					insert	EXP..SYN_BOU_SEG
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
						niveau
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
						@vl_Niveau
						)

	                        	/*Hilmarcher: remplissage voies medianes */
				   	if      (@va_VR_I_in=XDC_VOIE_BLOQUEE AND  @va_VL_I_in=XDC_VOIE_BLOQUEE)
				       	select @va_VM1_I_in=XDC_VOIE_BLOQUEE

					if	@va_VM1_I_in=XDC_VOIE_BLOQUEE
					insert	EXP..SYN_BOU_SEG
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
						niveau
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
						@vl_Niveau
						)
					if	@va_VL_I_in=XDC_VOIE_BLOQUEE
					insert	EXP..SYN_BOU_SEG
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
						niveau
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
						@vl_Niveau
						)
				end
				else

/*B				cas 4 voie */

				if @vl_voie=4
				begin
					if	@va_VR_I_in=XDC_VOIE_BLOQUEE
					insert	EXP..SYN_BOU_SEG
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
						niveau
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
						@vl_Niveau
						)
	                                        
					/*Hilmarcher: remplissage voies medianes */
                                        if      (@va_VR_I_in=XDC_VOIE_BLOQUEE AND  @va_VL_I_in=XDC_VOIE_BLOQUEE)
                                        begin
						select @va_VM1_I_in=XDC_VOIE_BLOQUEE
						select @va_VM2_I_in=XDC_VOIE_BLOQUEE
					end

					if	@va_VM2_I_in=XDC_VOIE_BLOQUEE
					insert	EXP..SYN_BOU_SEG
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
						niveau
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
						@vl_Niveau
						)
					if	@va_VM1_I_in=XDC_VOIE_BLOQUEE
					insert	EXP..SYN_BOU_SEG
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
						niveau
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
						@vl_Niveau
						)
					if	@va_VL_I_in=XDC_VOIE_BLOQUEE
					insert	EXP..SYN_BOU_SEG
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
						niveau
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
						@vl_Niveau
						)
				end
			end
		end

/*A	passe au segment suivant */

		fetch Segment into	@vl_portion,
					@vl_PRdebP,
					@vl_PRfinP,
					@vl_PRdebS,
					@vl_sens,
					@vl_voie
		
	end
	return XDC_OK
go
