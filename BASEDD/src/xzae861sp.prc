/* Fichier : $Id: xzae861sp.prc
------------------------------------------------------
* STERIA * PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* FICHIER xzae86sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* GGY	18/01/24 : Creation (DEM-483)
-----------------------------------------------------*/

/*-----------------------------------------------------
* Service rendu
*	Permet de calculer le predicat basculement_tunnel pour les PA.
*	Cette proc vérifie si l'ensemble des FMC declarees dans le tunnel possedent une voie inversee.
*		Si une voie est en sens inverse dans le tunnel alors le predicat est vrai.
* 
* Sequence d'appel
*	SP				XZAE861_Basculement_Tunnel
* 
* Arguments en entree
*	XDY_Autoroute	va_NumAutoroute_in
*	XDC_PR			va_PR_in
*	XDC_Sens		va_Sens_in
* 
* Arguments en sortie
*	XDY_Booleen		va_BasculementTunnel_out
* 
* Code retour
*	XDC_OK
*	XDC_NOK
*	XDC_ARG_INV	: parametres d'entree null
*	<0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
----------------------------------------------------*/
use PRC
go

if exists (select * from sysobjects where name = 'XZAE861' and type = 'P')
	drop procedure XZAE861
go

create procedure XZAE861
	@va_NumAutoroute_in			T_AUTOROUTE = null,
	@va_PRDebut_in				T_PR	 	= null,
	@va_PRFin_in				T_PR		= null,
	@va_BasculementTunnel_out	bit			= null	output

as
	declare		@vl_PREvt T_PR, @vl_PRExtremite T_PR, @vl_Sens T_SENS, @vl_VR T_VOIE, 
				@vl_VM2 T_VOIE, @vl_VM1 T_VOIE, @vl_VL T_VOIE, @vl_BAU T_VOIE,
				@vl_VR_I T_VOIE, @vl_VM2_I T_VOIE,@vl_VM1_I T_VOIE,
				@vl_VL_I T_VOIE, @vl_BAU_I T_VOIE, @vl_Type tinyint,
				@vl_HoroVal datetime, @vl_Evt int, @vl_Cle tinyint,
				@vl_debut int, @vl_fin int,
				@vl_min int,@vl_max int,
				@vl_a_garder tinyint, @vl_numero int,
				@vl_pr_max int, @vl_pr_min int,
		 		@vl_nb_zi tinyint

	/* Creation des tables temporaires */
	create table #ZI (numero int, autoroute tinyint, sens tinyint, debut int, fin int, a_garder tinyint)

	create unique index zi_index on #ZI(numero)

	create table #FMC (	numero int, 		cle tinyint,	autoroute tinyint,	PR int,			sens tinyint, 
						VR tinyint,			VM2 tinyint,	VM1 tinyint,		VL tinyint,		BAU tinyint,
						VR_I tinyint, 		VM2_I tinyint,	VM1_I tinyint,		VL_I tinyint,	BAU_I tinyint,
						a_garder tinyint,	PR_tete int)

	create unique index fmc_index on #FMC (numero, cle)

	/* Controle des parametres d'entree */
	if @va_NumAutoroute_in = null or @va_PRDebut_in = null or
	 @va_NumAutoroute_in = XDC_AUT_INCONNUE or @va_PRDebut_in = XDC_PR_INCONNU or @va_PRFin_in = null
		return XDC_ARG_INV

	/* Initialisation des parametres de sortie */
	select @va_BasculementTunnel_out = XDC_FAUX

	/* On recupere d abord toutes les zones d influence pour l autoroute */
	insert into #ZI
		select g.numero, g.autoroute, g.sens, g.PR, t.PR_Influence, 0
			from CFG..EQT_GEN g, CFG..EQT_TUB t
				where	g.type=XDC_EQT_TUB and
						g.numero = t.numero and
						g.autoroute = @va_NumAutoroute_in

	if @va_PRDebut_in < @va_PRFin_in
	begin
		select @vl_pr_min = @va_PRDebut_in
		select @vl_pr_max = @va_PRFin_in
	end
	else
	begin
		select @vl_pr_max = @va_PRDebut_in
		select @vl_pr_min = @va_PRFin_in
	end

	/* Parcourt cette liste pour ne garder que les zones qui ont une intersection */
	/* avec la FMC passee en entree pour les deux sens */
	declare Pointeur_zi cursor 
		for select debut, fin, a_garder, numero
			from #ZI
		
		for update of a_garder

		open Pointeur_zi
			fetch Pointeur_zi 
				into @vl_debut, @vl_fin, @vl_a_garder, @vl_numero

		while (@@sqlstatus = 0)
		begin
			/* On recupere PR min et max*/
			if (@vl_debut < @vl_fin)
			begin
				select @vl_min = @vl_debut
				select @vl_max = @vl_fin
			end
			else
			begin
				select @vl_min = @vl_fin
				select @vl_max = @vl_debut
			end

			if (@vl_max < @vl_pr_min)
				select @vl_a_garder=0
			else if (@vl_min > @vl_pr_max)
				select @vl_a_garder=0
			else
				select @vl_a_garder=1
			update  #ZI
			set a_garder=@vl_a_garder
			where  numero = @vl_numero

			fetch Pointeur_zi into @vl_debut, @vl_fin, @vl_a_garder, @vl_numero
		end
	close Pointeur_zi

	select * from #ZI

	/*A rechercher les evenements en cours */
	declare Pointeur_zi2 cursor 
		for select debut, fin
			from #ZI
				where a_garder = 1

	/*on cree d oabord une table temporaire avec les FMC basculements travaux en cours*/
	/*sur l autroute et dans le meme sens ou le sens inverse*/
	/*mais sur la section courante*/
	insert into #FMC
	select
		g.numero,
		g.cle,
		h.autoroute,
		h.PR,
		h.sens,
		h.VR,
		h.VM2,
		h.VM1,
		h.VL,
		h.BAU,
		h.VR_I,
		h.VM2_I,
		h.VM1_I,
		h.VL_I,
		h.BAU_I,
		0,
		localisation_PR_fin
	from EXP..FMC_HIS_DER h, EXP..FMC_GEN g, EXP..FMC_BAS b
/*	where	g.type in (XZAEC_FMC_Basculement,XZAEC_FMC_BasculTravaux) and */
	where	g.type = XZAEC_FMC_BasculTravaux and
			debut is not null and fin is null and
			h.numero = g.numero and
			h.cle = g.cle and
			h.autoroute = @va_NumAutoroute_in and
			b.numero = g.numero and
			b.cle = g.cle
	
	select * from #FMC

	declare Pointeur_evts_en_cours cursor
	for select	sens,
				VR,
				VM2,
				VM1,
				VL,
				BAU,
				VR_I,
				VM2_I,
				VM1_I,
				VL_I,
				BAU_I
					from #FMC
						where a_garder=1

	update #FMC 
	set a_garder=0

	open Pointeur_zi2

	fetch Pointeur_zi2 into @vl_debut, @vl_fin
	while (@@sqlstatus = 0)
	begin
		if @vl_debut < @vl_fin
		begin
			select @vl_pr_min = @vl_debut
			select @vl_pr_max = @vl_fin
		end
		else
		begin
			select @vl_pr_max = @vl_debut
			select @vl_pr_min =  @vl_fin
		end

		select  @vl_pr_min, @vl_pr_max
		
		/*on ne doit garder que les FMC qui ont une intersection*/
		/*avec la ZI parcourue*/
		update #FMC
		set a_garder=1 
		where not ((PR > @vl_pr_max) or (PR_tete < @vl_pr_min))
	
		fetch Pointeur_zi2 into @vl_debut, @vl_fin
	end

	close Pointeur_zi2

	select * from #FMC
		where a_garder = 1

	/*A rechercher la configuration des voies en fonction des evts en cours */
	open Pointeur_evts_en_cours
		fetch Pointeur_evts_en_cours 
			into	@vl_Sens, @vl_VR, @vl_VM2, @vl_VM1, @vl_VL, @vl_BAU,
					@vl_VR_I, @vl_VM2_I, @vl_VM1_I, @vl_VL_I, @vl_BAU_I
	
	while @@sqlstatus = 0
	begin
		/* repositionner la configuration des voies en groupant */
		/* l'etat neutralise,bloque avec l'etat neutralise */
		if @vl_VR = XDC_VOIE_BLOQUEE_NEUTRALISEE
			select @vl_VR = XDC_VOIE_NEUTRALISEE
		if @vl_VM2 = XDC_VOIE_BLOQUEE_NEUTRALISEE
			select @vl_VM2 = XDC_VOIE_NEUTRALISEE
		if @vl_VM1 = XDC_VOIE_BLOQUEE_NEUTRALISEE
			select @vl_VM1 = XDC_VOIE_NEUTRALISEE
		if @vl_VL = XDC_VOIE_BLOQUEE_NEUTRALISEE
			select @vl_VL = XDC_VOIE_NEUTRALISEE
		if @vl_BAU = XDC_VOIE_BLOQUEE_NEUTRALISEE
			select @vl_BAU = XDC_VOIE_NEUTRALISEE
		if @vl_VR_I = XDC_VOIE_BLOQUEE_NEUTRALISEE
			select @vl_VR_I = XDC_VOIE_NEUTRALISEE
		if @vl_VM2_I = XDC_VOIE_BLOQUEE_NEUTRALISEE
			select @vl_VM2_I = XDC_VOIE_NEUTRALISEE
		if @vl_VM1_I = XDC_VOIE_BLOQUEE_NEUTRALISEE
			select @vl_VM1_I = XDC_VOIE_NEUTRALISEE
		if @vl_VL_I = XDC_VOIE_BLOQUEE_NEUTRALISEE
			select @vl_VL_I = XDC_VOIE_NEUTRALISEE
		if @vl_BAU_I = XDC_VOIE_BLOQUEE_NEUTRALISEE
			select @vl_BAU_I = XDC_VOIE_NEUTRALISEE


		/* passage a l'evenement en cours suivant */
		fetch Pointeur_evts_en_cours 
			into	@vl_Sens, @vl_VR, @vl_VM2, @vl_VM1, @vl_VL, @vl_BAU,
					@vl_VR_I, @vl_VM2_I, @vl_VM1_I, @vl_VL_I, @vl_BAU_I
	
		/* Recuperation du parametre de sortie */
		if	@vl_VR = XDC_VOIE_SENS_INVERSE or @vl_VM2 = XDC_VOIE_SENS_INVERSE or
			@vl_VM1 = XDC_VOIE_SENS_INVERSE or @vl_VL = XDC_VOIE_SENS_INVERSE or
			@vl_BAU = XDC_VOIE_SENS_INVERSE or @vl_VR_I = XDC_VOIE_SENS_INVERSE or
			@vl_VM2_I = XDC_VOIE_SENS_INVERSE or @vl_VM1_I = XDC_VOIE_SENS_INVERSE or
			@vl_VL_I = XDC_VOIE_SENS_INVERSE or @vl_BAU_I = XDC_VOIE_SENS_INVERSE
				select @va_BasculementTunnel_out = XDC_VRAI
	end

	close Pointeur_evts_en_cours

	return XDC_OK

go
