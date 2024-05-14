/*E*/
/*  Fichier : @(#)vuem12sp.prc	1.2      Release : 1.2        Date : 05/23/97
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER vuem12sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* vues metiers
*
------------------------------------------------------
* HISTORIQUE :
*
* L.V.	09/04/97	: Creation a partir de vuem14sp.prc
* L.V.	23/05/97	: Prise en compte des fausses alertes nulles (v1.2)
* P.N.  09/0798		: suppressin des recherche dans EXP (dem/1696) 1.4
* P.N.	05/02/99	: Modif interg. dem/1724 v1.18
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	VUEM12_VolumeSection
* 
* Arguments en entree
* XDY_Horodate	va_Debut_in
* XDY_Horodate	va_Fin_in
* 
* Arguments en sortie
* 
* Service rendu
* Recherche les infos de volume, de duree sur les bouchons
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametres d'entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* en entree, c'est une queue de bouchon
* 
* Fonction
-----------------------------------------------------------------------*/

use PRC
go

create procedure VUEM12
	@va_Debut_in		datetime	= null,
	@va_Fin_in		datetime	= null
as
	declare @vl_EvtTete int, @vl_CleTete tinyint, @vl_Cause int,
		@vl_CleCause tinyint, @vl_Sens T_SENS, @vl_NumAutoroute int,
		@vl_DebutDensite datetime, @vl_FinDensite datetime,
		@vl_Autoroute char(4), @vl_PR T_PR, 
		@vl_Debut char(19), @vl_Fin char(19), 
		@vl_Longueur int,
		@vl_TypeCause char(25), @vl_Suppression bit, 
		@vl_Volume int, @vl_NumEvt int, @vl_CleEvt tinyint, 
		@vl_PRFin T_PR, @vl_PRDebut T_PR, @vl_DateCourante datetime,@vl_Nbre  int,
		@vl_NbreVolumes int, @vl_HoroDeb datetime, @vl_HoroFin datetime, @vl_Surface int,
		@vl_NbVoies tinyint, @vl_VR tinyint, @vl_VM1 tinyint, @vl_VM2 tinyint, @vl_VL tinyint,
		@vl_NumTypeCause tinyint, @vl_Inter int

	/*A controle des parametres d'entree */
	if @va_Debut_in is null or @va_Fin_in is null
		return XDC_ARG_INV

	create table #LISTE_BOU (autoroute int, PR int, debut char(19) , fin char(19) null, mois int null, num_type_cause tinyint null,
				 type_cause char(25), volume int null)

	create table #BOU_PR (numero int, cle tinyint, horodate datetime null, PR int null, cause int null, 
			cle_cause tinyint null, VR tinyint null, VM2 tinyint null, VM1 tinyint null, VL tinyint null)

	/*! creation d'une table de travail de stockage du graphe de bouchon */
	create table #GRA_BOU ( horodate datetime , horodate_fin datetime null, PR_debut int , PR_fin int, VR tinyint, VM2 tinyint, VM1 tinyint, VL tinyint)
	create table #GRAPHE (horodate datetime null, horodate_fin datetime null, PR_debut int , PR_fin int, VR tinyint, VM2 tinyint, VM1 tinyint, VL tinyint)
	create table #VOLUME (horodate datetime null, horodate_fin datetime null, PR_debut int , PR_fin int, volume int null, longueur int null,
				VR tinyint, VM2 tinyint, VM1 tinyint, VL tinyint)
	create table #SEGMENTS ( PR_fin_por int, PR_debut_seg int, nombre_de_voies tinyint)
	create table #SEGMENTS_LONG ( PR_fin_por int, PR_debut_seg int, PR_fin_seg int, nombre_de_voies tinyint)

	select @vl_DateCourante = getdate()

	/*A recherche des bouchons ayant commence dans la periode choisie */
	select
		numero,
		cle,
		cause,
		cle_cause,
		debut = convert(char(10),debut,103)+' '+convert(char(8),debut,8),
		fin = convert(char(10),fin,103)+' '+convert(char(8),fin,8)
	into #LISTE
	from HIS..FMC_GEN
	where type = XZAEC_FMC_QueueBouchon and
		datex<>XDC_VRAI and
		(fausse_alerte != XDC_VRAI or fausse_alerte is null) and
		debut >= @va_Debut_in and debut < @va_Fin_in

	/*A recherche pour chaque bouchon des informations */
	select @vl_Nbre = count(*) from #LISTE
	while @vl_Nbre > 0
	begin

	   set rowcount 1
	   select 
		@vl_NumEvt = numero , @vl_CleEvt = cle , @vl_EvtTete = cause, 
		@vl_CleTete = cle_cause,  @vl_Debut =debut, @vl_Fin = fin
	   from #LISTE
	   delete #LISTE
	   set rowcount 0

	   /*B rechercher la premiere localisation de la tete */
	   select
			@vl_NumAutoroute = autoroute,
			@vl_Autoroute = nom,
			@vl_PR = HIS..FMC_HIS.PR,
			@vl_Sens = HIS..FMC_HIS.sens,
			@vl_VR = HIS..FMC_HIS.VR,
			@vl_VM1 = HIS..FMC_HIS.VM1,
			@vl_VM2 = HIS..FMC_HIS.VM2,
			@vl_VR = HIS..FMC_HIS.VR
	   from HIS..FMC_HIS, CFG..RES_AUT
	   where HIS..FMC_HIS.numero = @vl_EvtTete and HIS..FMC_HIS.cle = @vl_CleTete and
			CFG..RES_AUT.numero = HIS..FMC_HIS.autoroute and
			horodate_validation in ( select min(horodate_validation)
					from HIS..FMC_HIS
					where numero = @vl_EvtTete and cle = @vl_CleTete)

	   if @@rowcount > 0
	   begin
		/*B rechercher les validations du bouchons */
		select @vl_Volume= 0, @vl_Longueur = 0, @vl_Suppression = XDC_FAUX
			/*B lecture des PR de debut de bouchon */
			insert #BOU_PR (numero, cle, horodate, PR)
			select 
				HIS..FMC_HIS.numero,
				HIS..FMC_HIS.cle,
				HIS..FMC_HIS.horodate_validation,
				HIS..FMC_HIS.PR
			from HIS..FMC_GEN, HIS..FMC_HIS
			where	HIS..FMC_GEN.numero = @vl_EvtTete and
				HIS..FMC_GEN.cle = @vl_CleTete and
				HIS..FMC_GEN.datex<>XDC_VRAI and
				HIS..FMC_HIS.numero = HIS..FMC_GEN.numero and
				HIS..FMC_HIS.cle = HIS..FMC_GEN.cle 
			
			if @@rowcount = 0
				select @vl_Suppression = XDC_VRAI
	
			/*B lecture des PR de fin de bouchon */
			insert #BOU_PR (numero, cle, horodate, PR, cause, cle_cause, VR, VM2, VM1, VL)
			select 
				HIS..FMC_HIS.numero,
				HIS..FMC_HIS.cle,
				HIS..FMC_HIS.horodate_validation,
				HIS..FMC_HIS.PR,
				HIS..FMC_GEN.cause,
				HIS..FMC_GEN.cle_cause,
				HIS..FMC_HIS.VR,
				HIS..FMC_HIS.VM2,
				HIS..FMC_HIS.VM1,
				HIS..FMC_HIS.VL
			from HIS..FMC_GEN, HIS..FMC_HIS
			where	HIS..FMC_GEN.numero = @vl_NumEvt and
				HIS..FMC_GEN.cle = @vl_CleEvt and
				HIS..FMC_GEN.datex<>XDC_VRAI and
				HIS..FMC_HIS.numero = HIS..FMC_GEN.numero and
				HIS..FMC_HIS.cle = HIS..FMC_GEN.cle
	
			if @@rowcount = 0
				select @vl_Suppression = XDC_VRAI
	
		if @vl_Sens = XDC_SENS_INCONNU
			select @vl_Sens = XDC_SENS_INCONNU
		else if @vl_Sens % 2 = XDC_SENS_SUD
			select @vl_Sens = XDC_SENS_SUD
		else
			select @vl_Sens = XDC_SENS_NORD

		/*B calculer le volume du bouchon */
		if @vl_Suppression = XDC_FAUX
		begin
			insert #GRA_BOU (horodate, horodate_fin, PR_debut, PR_fin, VR, VM2, VM1, VL)
			select distinct
				Q.horodate,
				null,
				T.PR,
				Q.PR,
				Q.VR,
				Q.VM2,
				Q.VM1,
				Q.VL
			from #BOU_PR T, #BOU_PR Q
			where	T.numero = Q.cause and
				T.cle = Q.cle_cause and
				T.horodate <= Q.horodate
			group by T.numero,T.cle, Q.numero,Q.cle,Q.horodate
			having	T.numero = Q.cause and
				T.cle = Q.cle_cause and
				T.horodate <= Q.horodate and
				T.horodate = max(T.horodate)
		
			insert #GRA_BOU (horodate, horodate_fin, PR_debut, PR_fin, VR, VM2, VM1, VL)
			select distinct
				T.horodate,
				null,
				T.PR,
				Q.PR,
				Q.VR,
				Q.VM2,
				Q.VM1,
				Q.VL
			from #BOU_PR T, #BOU_PR Q
			where	Q.cause = T.numero and
				Q.cle_cause = T.cle and
				Q.horodate <= T.horodate
			group by 
				Q.numero,Q.cle,
				T.numero,T.cle,T.horodate
			having	Q.cause = T.numero and
				Q.cle_cause = T.cle and
				Q.horodate <= T.horodate and
				Q.horodate = max(Q.horodate)
		
			insert #GRAPHE (horodate, horodate_fin, PR_debut, PR_fin,VR, VM2, VM1, VL)
			select
				horodate = g2.horodate,
				horodate_fin = g1.horodate,
				g2.PR_debut,
				g2.PR_fin,
				g2.VR,
				g2.VM2,
				g2.VM1,
				g2.VL
			from #GRA_BOU g1, #GRA_BOU g2
			where g1.horodate > g2.horodate
			group by g2.horodate
			having g1.horodate > g2.horodate and
				g1.horodate = min(g1.horodate)
		
			/*A Calcul du volume d'encombrement */
			insert #VOLUME (horodate, horodate_fin, PR_debut, PR_fin, longueur,VR, VM2, VM1, VL)
			select distinct 
				horodate,
				horodate_fin,
				PR_debut,
				PR_fin,
				longueur = abs(PR_debut - PR_fin),
				VR,
				VM2,
				VM1,
				VL
			from #GRAPHE
			
			/* On remplace la plus petite horodate de debut par l'horo de FMC_GEN */
			update #VOLUME
			set horodate = convert(datetime,@vl_Debut)
			where horodate in
				(select min(horodate) from #VOLUME)
			
		
			
			/* calcul du volume du bouchon en fonction du nombre de voies */
			select @vl_NbreVolumes = count(*) from #VOLUME
			while @vl_NbreVolumes > 0
			begin
		
				set rowcount 1
				select
					@vl_HoroDeb = horodate,
					@vl_HoroFin = horodate_fin,
					@vl_PRDebut = PR_debut,
					@vl_PRFin = PR_fin,
					@vl_VR = VR,
					@vl_VM2 = VM2,
					@vl_VM1 = VM1,
					@vl_VL = VL
				from #VOLUME
				delete #VOLUME
				set rowcount 0
		
				if @vl_PRDebut > @vl_PRFin
				begin
					select @vl_Inter = @vl_PRFin
					select @vl_PRFin = @vl_PRDebut
					select @vl_PRDebut = @vl_Inter
				end
		
				/*calcul du nombre de voies bloquees par le bouchon*/
				select @vl_NbVoies = 0
				if @vl_VR = XDC_VOIE_BLOQUEE
					select @vl_NbVoies = @vl_NbVoies + 1
				if @vl_VM2 = XDC_VOIE_BLOQUEE
					select @vl_NbVoies = @vl_NbVoies + 1
				if @vl_VM1 = XDC_VOIE_BLOQUEE
					select @vl_NbVoies = @vl_NbVoies + 1
				if @vl_VL = XDC_VOIE_BLOQUEE
					select @vl_NbVoies = @vl_NbVoies + 1

				select @vl_Surface = (@vl_PRFin - @vl_PRDebut) * @vl_NbVoies
		
				/* calcul du volume total du bouchon */
				select @vl_Volume = @vl_Volume + datediff(minute, @vl_HoroDeb, @vl_HoroFin) * @vl_Surface
				select @vl_NbreVolumes = @vl_NbreVolumes - 1
			end

			delete #SEGMENTS_LONG 
			delete #SEGMENTS 
			delete #GRA_BOU 
			delete #BOU_PR 
			delete #VOLUME 
			delete #GRAPHE 
		end

		/*B recherche le type de la cause */
		select
			@vl_Cause = cause,
			@vl_CleCause = cle_cause
		from HIS..FMC_GEN
		where HIS..FMC_GEN.numero = @vl_EvtTete and cle = @vl_CleTete and
			datex<>XDC_VRAI

		/*B recherche des info sur la cause du bouchon */
		select @vl_NumTypeCause = null
		
		if @vl_Cause is not null
		begin
			select
				@vl_Debut = convert(char(10),debut,103)+' '+convert(char(8),debut,8),
				@vl_Fin = convert(char(10),fin,103)+' '+convert(char(8),fin,8),
				@vl_NumTypeCause = type,
				@vl_TypeCause = nom
			from HIS..FMC_GEN, CFG..TYP_FMC
			where HIS..FMC_GEN.numero = @vl_Cause and cle = @vl_CleCause and
				CFG..TYP_FMC.numero = type and datex<>XDC_VRAI

			if @@rowcount = 0
			begin
				select @vl_TypeCause = XDC_CHAINE_VIDE
			end
			else if @vl_NumTypeCause = XZAEC_FMC_TeteBouchon
				select @vl_TypeCause = "BOUCHON"
		end
		else	
		begin
			select @vl_TypeCause = XDC_CHAINE_VIDE
		end

		insert #LISTE_BOU (autoroute, PR, debut, fin, mois, num_type_cause, type_cause, volume)
		values(@vl_NumAutoroute, @vl_PR, @vl_Debut, @vl_Fin, datepart(month,@vl_Debut), @vl_NumTypeCause, @vl_TypeCause, @vl_Volume)
	   end

	   select @vl_Nbre = @vl_Nbre - 1
	end

	select 
		autoroute,
		PR,
		mois,
		num_type_cause,
		volume
	from #LISTE_BOU
	order by autoroute, PR, debut

	return XDC_OK
go
