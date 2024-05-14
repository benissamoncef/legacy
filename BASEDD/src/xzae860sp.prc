/*E*/
/*  Fichier : $Id: xzae860sp.prc,v 1.3 2010/09/08 12:21:39 gesconf Exp $     Release : $Revision: 1.3 $        Date : $Date: 2010/09/08 12:21:39 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
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
* JMG	26/11/08	creation DEM895
* JMG	29/04/10 : exclusion d autres FMC non bloquantes sur les voies SAE174 1.2
* JPL	23/08/10 : Prise en compte des Fmc Zone de Stockage (DEM 942) 1.3
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourner la configuration des voies pour une localisation
* donn≈e en prenant en compte les modifications dues aux evenements 
* de type travaux ou basculement en cours
* 
* Sequence d'appel
* SP	XZAE860_Configuration_Voies
* 
* Arguments en entree
* XDY_Autoroute	va_NumAutoroute_in
* XDC_PR	va_PR_in
* XDC_Sens	va_Sens_in
* 
* Arguments en sortie
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
* 
* Code retour
* XDC_OK
* XDC_NOK
* XDC_ARG_INV		: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
----------------------------------------------------*/

use PRC
go

drop proc XZAE860
go

create procedure XZAE860
	@va_NumAutoroute_in	T_AUTOROUTE 	= null,
	@va_PRDebut_in		T_PR	 	= null,
	@va_Sens_in		T_SENS	 	= null,
	@va_PRFin_in		T_PR		= null   ,
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
	@va_Nombre_ZI_out	T_VOIE		= null	output 
as
	declare @vl_PREvt T_PR, @vl_PRExtremite T_PR, @vl_Sens T_SENS, @vl_VR T_VOIE, 
		@vl_VM2 T_VOIE, @vl_VM1 T_VOIE, @vl_VL T_VOIE, @vl_BAU T_VOIE,
		@vl_VR_I T_VOIE, @vl_VM2_I T_VOIE,@vl_VM1_I T_VOIE,
		@vl_VL_I T_VOIE, @vl_BAU_I T_VOIE, @vl_Type tinyint,
		@vl_HoroVal datetime, @vl_Evt int, @vl_Cle tinyint,
		@vl_debut int, @vl_fin int,
		@vl_min int,@vl_max int,
		@vl_a_garder tinyint,@vl_numero int,
		@vl_pr_max int, @vl_pr_min int ,  /*
		 @va_VR_out T_VOIE, @va_VM2_out T_VOIE, @va_VM1_out T_VOIE, 
		 @va_VL_out T_VOIE, @va_BAU_out T_VOIE, 
		 @va_VR_I_out T_VOIE, @va_VM2_I_out T_VOIE, @va_VM1_I_out T_VOIE, 
		 @va_VL_I_out T_VOIE, @va_BAU_I_out T_VOIE,@va_Nombre_ZI_out tinyint,*/
		 @vl_nb_zi tinyint

	create table #ZI (numero int, autoroute tinyint, sens tinyint, debut int, fin int, a_garder tinyint)

	create unique index zi_index on #ZI(numero)

	create table #FMC (numero int, cle tinyint, autoroute tinyint, PR int, sens tinyint, 
				VR tinyint, VM2 tinyint, VM1 tinyint, VL tinyint, BAU tinyint,
				VR_I tinyint, VM2_I tinyint, VM1_I tinyint, VL_I tinyint, BAU_I tinyint,
				a_garder tinyint, PR_tete int)
	create unique index fmc_index on #FMC (numero,cle)

	/* Controle des parametres d'entree */
	if @va_NumAutoroute_in = null or @va_PRDebut_in = null or @va_Sens_in = null or
	   @va_NumAutoroute_in = XDC_AUT_INCONNUE or @va_PRDebut_in = XDC_PR_INCONNU or @va_PRFin_in = null
	   	return XDC_ARG_INV

	select @va_Nombre_ZI_out = 0

	/*on recupere d abord toutes les zones d influence */
	/*pour l autoroute et le sens en entree*/
	insert into #ZI
	select g.numero, g.autoroute, g.sens, g.PR, t.PR_Influence, 0
	from CFG..EQT_GEN g, CFG..EQT_TUB t
	where g.type=XDC_EQT_TUB
	and g.numero = t.numero
	and g.autoroute = @va_NumAutoroute_in
	and g.sens = @va_Sens_in

	if @va_PRDebut_in < @va_PRFin_in
	begin
		select @vl_pr_min = @va_PRDebut_in
		select @vl_pr_max = @va_PRFin_in
	end
	else
	begin
		select @vl_pr_max = @va_PRDebut_in
		select @vl_pr_min =  @va_PRFin_in
	end

	/*parcourt cette liste*/
	/*pour ne garder que les zones qui ont une intersection*/
	/*avec la FMC passee en entree*/

	declare Pointeur_zi cursor 
	for select debut, fin, a_garder, numero
	from #ZI
	for update of a_garder

	open Pointeur_zi
	fetch Pointeur_zi into @vl_debut, @vl_fin, @vl_a_garder, @vl_numero

	while (@@sqlstatus = 0)
	begin
		/*on recupere PR min et max*/
		if (@vl_debut < @vl_fin)
		begin
			select @vl_min=@vl_debut
			select @vl_max=@vl_fin
		end
		else
		begin
			select @vl_min=@vl_fin
			select @vl_max=@vl_debut
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

	/* initialisation de la configuration des voies */
	select @va_VR_I_out = XDC_VOIE_INEXISTANTE,
		@va_VM2_I_out = XDC_VOIE_INEXISTANTE,
		@va_VM1_I_out = XDC_VOIE_INEXISTANTE,
		@va_VL_I_out = XDC_VOIE_INEXISTANTE,
		@va_BAU_I_out = XDC_VOIE_INEXISTANTE,
		@va_VR_out = XDC_VOIE_INEXISTANTE,
		@va_VM2_out = XDC_VOIE_INEXISTANTE,
		@va_VM1_out = XDC_VOIE_INEXISTANTE,
		@va_VL_out = XDC_VOIE_INEXISTANTE,
		@va_BAU_out = XDC_VOIE_INEXISTANTE

	/*A rechercher les evenements en cours positionn≈s a cette localisation */
	/*cad dans les Z.I qu'il faut garder*/
	declare Pointeur_zi2 cursor 
	for select debut, fin
	from #ZI
	where a_garder=1

	select @va_Nombre_ZI_out = count(*)
	from #ZI
	where a_garder=1

	/*on cree d abord une table temporaire avec les FMC en cours*/
	/*sur l autoroute et dans le meme sens ou le sens inverse*/
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
			h.PR
	from EXP..FMC_HIS_DER h, EXP..FMC_GEN g
		where   debut is not null and fin is null and
			h.numero = g.numero and
			h.cle = g.cle and
			h.autoroute = @va_NumAutoroute_in and
			h.sens = @va_Sens_in
			and g.type not in (XZAEC_FMC_Cirque, XZAEC_FMC_ConvoiExc, XZAEC_FMC_ConvoiMil, XZAEC_FMC_VehLent, XZAEC_FMC_ChausseDeter, XZAEC_FMC_FlaqueHuile, XZAEC_FMC_AnimalErrant, XZAEC_FMC_PietonChaussee, XZAEC_FMC_Salage, XZAEC_FMC_TraitChaussees, XZAEC_FMC_Meteo, XZAEC_FMC_Inondation, XZAEC_FMC_Eboulement, XZAEC_FMC_ChausseeGliss, XZAEC_FMC_Travaux, XZAEC_FMC_ZoneStockage, XZAEC_FMC_QueueBouchon)

	/*on rajoute les basculements qui sont situes dans l autre sens*/
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
	where g.type in (XZAEC_FMC_Basculement,XZAEC_FMC_BasculTravaux) and
		debut is not null and fin is null and
		h.numero = g.numero and
		h.cle = g.cle and
		h.autoroute = @va_NumAutoroute_in and
		h.sens != @va_Sens_in and
		b.numero = g.numero and
		b.cle = g.cle

	/*on rajoute les travaux */
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
			b.localisation_tete
	from EXP..FMC_HIS_DER h, EXP..FMC_GEN g, EXP..FMC_TRH b
	where g.type in (XZAEC_FMC_Travaux, XZAEC_FMC_ZoneStockage) and
		debut is not null and fin is null and
		h.numero = g.numero and
		h.cle = g.cle and
		h.autoroute = @va_NumAutoroute_in and
		h.sens = @va_Sens_in and
		b.numero = g.numero and
		b.cle = g.cle and
		b.horodate = h.horodate_validation
	
	select * from #FMC

	declare Pointeur_evts_en_cours cursor
	for select
		sens,
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
		if @va_Sens_in=XDC_SENS_SUD
		begin
			update #FMC
			set a_garder=1 
			where not ((PR > @vl_pr_max) or (PR_tete < @vl_pr_min))
		end
		else
		begin
			update #FMC
			set a_garder=1
			where not ((PR_tete > @vl_pr_max) or (PR < @vl_pr_min))
		end
		fetch Pointeur_zi2 into @vl_debut, @vl_fin
	end

	close Pointeur_zi2

	select * from #FMC
	where a_garder=1

	/*A rechercher la configuration des voies en fonction des evts en cours */
	open Pointeur_evts_en_cours
	fetch Pointeur_evts_en_cours into @vl_Sens, @vl_VR, @vl_VM2, @vl_VM1, @vl_VL, @vl_BAU,
			@vl_VR_I, @vl_VM2_I, @vl_VM1_I, @vl_VL_I, @vl_BAU_I
	while @@sqlstatus = 0
	begin
		/* repositionner la configuration des voies en groupant */
		/* l'etat neutralise,bloque avec l'etat neutralise      */
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

			if @vl_VR = XDC_VOIE_SENS_INVERSE  or
		    	   @vl_VR = XDC_VOIE_NEUTRALISEE or
			   @vl_VR =  XDC_VOIE_BLOQUEE 
			begin
				if @va_Sens_in = @vl_Sens 
					select @va_VR_out = @vl_VR
				else if @va_Sens_in != @vl_Sens 
					select @va_VR_I_out = @vl_VR
			end
			else
			begin
				if @vl_VR = XDC_VOIE_SENS_NORMAL
				begin
					if @va_VR_out = XDC_VOIE_INEXISTANTE
						select @va_VR_out = @vl_VR
				end
			end
			if @vl_VM2 = XDC_VOIE_SENS_INVERSE or
		    	   @vl_VM2 = XDC_VOIE_NEUTRALISEE  or
			   @vl_VM2 = XDC_VOIE_BLOQUEE 
			begin
				if @va_Sens_in = @vl_Sens 
					select @va_VM2_out = @vl_VM2
				else if @va_Sens_in != @vl_Sens
					select @va_VM2_I_out = @vl_VM2
			end
			else
			begin
				if @vl_VM2 = XDC_VOIE_SENS_NORMAL
				begin
					if @va_VM2_out = XDC_VOIE_INEXISTANTE
						select @va_VM2_out = @vl_VM2
				end
			end
			if @vl_VM1 = XDC_VOIE_SENS_INVERSE  or
		    	   @vl_VM1 = XDC_VOIE_NEUTRALISEE  or
			   @vl_VM1 = XDC_VOIE_BLOQUEE 
			begin
				if @va_Sens_in = @vl_Sens 
					select @va_VM1_out = @vl_VM1
				else if @va_Sens_in != @vl_Sens 
					select @va_VM1_I_out = @vl_VM1
			end
			else
			begin
				if @vl_VM1 = XDC_VOIE_SENS_NORMAL
				begin
					if @va_VM1_out = XDC_VOIE_INEXISTANTE
						select @va_VM1_out = @vl_VM1
				end
			end
			if @vl_VL = XDC_VOIE_SENS_INVERSE  or
		    	   @vl_VL = XDC_VOIE_NEUTRALISEE  or
			   @vl_VL = XDC_VOIE_BLOQUEE 
			begin
				if @va_Sens_in = @vl_Sens 
					select @va_VL_out = @vl_VL
				else if @va_Sens_in != @vl_Sens 
					select @va_VL_I_out = @vl_VL
			end
			else
			begin
				if @vl_VL = XDC_VOIE_SENS_NORMAL
				begin
					if @va_VL_out = XDC_VOIE_INEXISTANTE
						select @va_VL_out = @vl_VL
				end
			end
			if @vl_BAU = XDC_VOIE_SENS_INVERSE  or
		    	   @vl_BAU = XDC_VOIE_NEUTRALISEE or
			   @vl_BAU = XDC_VOIE_BLOQUEE
			begin
				if @va_Sens_in = @vl_Sens 
					select @va_BAU_out = @vl_BAU
				else if @va_Sens_in != @vl_Sens 
					select @va_BAU_I_out = @vl_BAU
			end
			else if @vl_BAU = XDC_VOIE_SENS_NORMAL or @vl_BAU = XDC_VOIE_BAU_NORMALE
			begin
				if @va_Sens_in = @vl_Sens and @va_BAU_out = XDC_VOIE_INEXISTANTE
					select @va_BAU_out = @vl_BAU
				else if @va_Sens_in != @vl_Sens and @va_BAU_I_out = XDC_VOIE_INEXISTANTE
					select @va_BAU_I_out = @vl_BAU
			end

			/* configuration des voies inverses de l'evenement */
			if @vl_VR_I = XDC_VOIE_SENS_INVERSE  or
		    	   @vl_VR_I = XDC_VOIE_NEUTRALISEE  or
			   @vl_VR_I = XDC_VOIE_BLOQUEE 
			begin
				if @va_Sens_in = @vl_Sens and @va_VR_I_out != XDC_VOIE_NEUTRALISEE
					select @va_VR_I_out = @vl_VR_I
				else if @va_Sens_in != @vl_Sens and @va_VR_out != XDC_VOIE_NEUTRALISEE
					select @va_VR_out = @vl_VR_I
			end
			if @vl_VM2_I = XDC_VOIE_SENS_INVERSE or
		    	   @vl_VM2_I = XDC_VOIE_NEUTRALISEE or
			   @vl_VM2_I = XDC_VOIE_BLOQUEE 
			begin
				if @va_Sens_in = @vl_Sens and @va_VM2_I_out != XDC_VOIE_NEUTRALISEE
					select @va_VM2_I_out = @vl_VM2_I
				else if @va_Sens_in != @vl_Sens and @va_VM2_out != XDC_VOIE_NEUTRALISEE
					select @va_VM2_out = @vl_VM2_I
			end
			if @vl_VM1_I = XDC_VOIE_SENS_INVERSE  or
		    	   @vl_VM1_I = XDC_VOIE_NEUTRALISEE or
			   @vl_VM1_I = XDC_VOIE_BLOQUEE
			begin
				if @va_Sens_in = @vl_Sens and @va_VM1_I_out != XDC_VOIE_NEUTRALISEE
					select @va_VM1_I_out = @vl_VM1_I
				else if @va_Sens_in != @vl_Sens and @va_VM1_out != XDC_VOIE_NEUTRALISEE
					select @va_VM1_out = @vl_VM1_I
			end
			if @vl_VL_I = XDC_VOIE_SENS_INVERSE  or
		    	   @vl_VL_I = XDC_VOIE_NEUTRALISEE or
			   @vl_VL_I = XDC_VOIE_BLOQUEE 
			begin
				if @va_Sens_in = @vl_Sens and @va_VL_I_out != XDC_VOIE_NEUTRALISEE
					select @va_VL_I_out = @vl_VL_I
				else if @va_Sens_in != @vl_Sens and @va_VL_out != XDC_VOIE_NEUTRALISEE
					select @va_VL_out = @vl_VL_I
			end
			if @vl_BAU_I = XDC_VOIE_SENS_INVERSE  or
		    	   @vl_BAU_I = XDC_VOIE_NEUTRALISEE  or
			   @vl_BAU_I = XDC_VOIE_BLOQUEE 
			begin
				if @va_Sens_in = @vl_Sens and @va_BAU_I_out != XDC_VOIE_NEUTRALISEE
					select @va_BAU_I_out = @vl_BAU_I
				else if @va_Sens_in != @vl_Sens and @va_BAU_out != XDC_VOIE_NEUTRALISEE
					select @va_BAU_out = @vl_BAU_I
			end
			else if @vl_BAU_I = XDC_VOIE_SENS_NORMAL or @vl_BAU_I = XDC_VOIE_BAU_NORMALE
			begin
				if @va_Sens_in = @vl_Sens and @va_BAU_I_out = XDC_VOIE_INEXISTANTE
					select @va_BAU_I_out = @vl_BAU_I
				else if @va_Sens_in != @vl_Sens and @va_BAU_out = XDC_VOIE_INEXISTANTE
					select @va_BAU_out = @vl_BAU_I
			end

		/* passage a l'evenement en cours suivant */
		fetch Pointeur_evts_en_cours into @vl_Sens, @vl_VR, @vl_VM2, @vl_VM1, @vl_VL, @vl_BAU,
			@vl_VR_I, @vl_VM2_I, @vl_VM1_I, @vl_VL_I, @vl_BAU_I
	end

	close Pointeur_evts_en_cours

	select @va_Nombre_ZI_out = @vl_nb_zi

	select @va_VR_I_out ,
		@va_VM2_I_out,
		@va_VM1_I_out,
		@va_VL_I_out,
		@va_BAU_I_out,
		@va_VR_out,
		@va_VM2_out,
		@va_VM1_out,
		@va_VL_out,
		@va_BAU_out,@va_Nombre_ZI_out

	return XDC_OK
go
