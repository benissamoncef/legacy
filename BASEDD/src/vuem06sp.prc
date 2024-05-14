/*E*/
/*  Fichier : $Id: vuem06sp.prc,v 1.11 2016/07/22 18:35:07 devgfi Exp $      Release : $Revision: 1.11 $        Date : $Date: 2016/07/22 18:35:07 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER vuem06sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* vues metiers
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	19/07/95	: Creation
* C.T.	24/08/95	: Modif ordre de retour des donnees (V 1.2)
* C.T.  11/12/95	: Modif recherche nb eveneement (V1.3)
* C.T.  01/07/96	: Modif interface E/S pour retourner les
*                         statistiques bouchons en volume et en
*                         nombre (V1.4) et calcul des volumes 
*                         en fonction du nombre de voies
* JMG   22/10/96        : modif calcul volume suite a FMC/13 (DEM/1249) 1.5
* C.T.  04/11/96	: Modif recherche des evts (supp fausse alerte) FMC18 1.6
* P.N.  09/0798		: suppressin des recherche dans EXP (dem/1696) 1.7
* P.N.	05/02/99	: Modif interg. dem/1724 v1.18
* JPL	22/07/16	: Syntaxe des directives pour le compilateur C  1.10
* JPL	22/07/16	: Filtrer les secteurs hors reseau et obsoletes (type RES_DIS)  1.11
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	VUEM06_Encombrement_Bouchon
* 
* Arguments en entree
* XDY_Horodate	va_Debut_in
* XDY_Horodate	va_Fin_in
* 
* Arguments en sortie
*
* liste retournee
* XDC_Octet	mois
* XDC_NOM	district
* XDC_Entier	volume
* XDC_Entier	nombre
* 
* Service rendu
* Statistiques bouchons par district/par mois decomposÅs en volume ou
* en nombre
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametres d'entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* dans le cas des volumes, les statistiques sont uniquement sur le mois
* demande
* 
* Fonction
-----------------------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'VUEM06' and type = 'P')
	drop procedure VUEM06
go


create procedure VUEM06
	@va_Debut_in		datetime	= null,
	@va_Fin_in		datetime	= null
as
	declare @vl_EvtTete int, @vl_CleTete tinyint, 
		@vl_Volume int, @vl_NumEvt int, @vl_CleEvt tinyint, 
		@vl_Sens T_SENS, @vl_NumAutoroute T_AUTOROUTE, @vl_PR T_PR, @vl_Suppression bit, 
		@vl_PRFin T_PR, @vl_PRDebut T_PR, @vl_Nbre  int,
		@vl_NbreVolumes int, @vl_HoroDeb datetime, @vl_HoroFin datetime, @vl_Surface int,
		@vl_Inter int, @vl_Site T_SITE, @vl_MoisFin tinyint, @vl_Mois tinyint, @vl_NbVoies int ,
		@vl_VR tinyint, @vl_VM2 tinyint, @vl_VM1 tinyint, @vl_VL tinyint

	/*A controle des parametres d'entree */
	if @va_Debut_in is null or @va_Fin_in is null
		return XDC_ARG_INV

	create table #LISTE_BOU (mois tinyint, sit tinyint, volume int null)

	create table #BOU_PR (numero int, cle tinyint, horodate datetime null, PR int null, cause int null, 
			cle_cause tinyint null, VR int null, VM2 int null, VM1 int null, VL int null)

	/*! creation d'une table de travail de stockage du graphe de bouchon */
	create table #GRA_BOU ( horodate datetime , horodate_fin datetime null, PR_debut int , PR_fin int, VR int, VM2 int, VM1 int, VL int)
	create table #GRAPHE (horodate datetime null, horodate_fin datetime null, PR_debut int , PR_fin int, VR int, VM2 int, VM1 int, VL int)
	create table #VOLUME (horodate datetime null, horodate_fin datetime null, PR_debut int , PR_fin int, volume int null, longueur int null, VR int, VM2 int, VM1 int, VL int)
	create table #SEGMENTS ( PR_fin_por int, PR_debut_seg int, nombre_de_voies tinyint)
	create table #SEGMENTS_LONG ( PR_fin_por int, PR_debut_seg int, PR_fin_seg int, nombre_de_voies tinyint)

	/*A recherche des bouchons ayant commence dans la periode choisie */
	select
		numero,
		cle,
		cause,
		cle_cause,
		mois = datepart(month,debut)
	into #LISTE
	from HIS..FMC_GEN
	where type = XZAEC_FMC_QueueBouchon and
		datex<>XDC_VRAI and
		fausse_alerte != XDC_VRAI and
		debut >= @va_Debut_in and debut < @va_Fin_in

	/*A recherche pour chaque bouchon des informations */
	select @vl_Nbre = count(*) from #LISTE
	while @vl_Nbre > 0
	begin

	   set rowcount 1
	   select 
		@vl_NumEvt = numero , @vl_CleEvt = cle , @vl_EvtTete = cause, 
		@vl_CleTete = cle_cause, @vl_Mois = mois
	   from #LISTE
	   delete #LISTE
	   set rowcount 0

	   /*B rechercher la premiere localisation de la tete */
	   select
			@vl_Site = district,
			@vl_NumAutoroute = HIS..FMC_HIS.autoroute,
			@vl_Sens = HIS..FMC_HIS.sens
	   from HIS..FMC_HIS, CFG..RES_POR
	   where HIS..FMC_HIS.numero = @vl_EvtTete and HIS..FMC_HIS.cle = @vl_CleTete and
		horodate_validation in ( select min(horodate_validation)
					from HIS..FMC_HIS
					where numero = @vl_EvtTete and cle = @vl_CleTete) and
		CFG..RES_POR.autoroute = HIS..FMC_HIS.autoroute and
		CFG..RES_POR.PR_debut <= HIS..FMC_HIS.PR and
		CFG..RES_POR.PR_fin > HIS..FMC_HIS.PR

	   if @@rowcount > 0
	   begin
		/*B rechercher les validations du bouchons */
		select @vl_Volume= 0, @vl_Suppression = XDC_FAUX
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
				HIS..FMC_HIS.numero = HIS..FMC_GEN.numero and
				HIS..FMC_HIS.cle = HIS..FMC_GEN.cle and
				HIS..FMC_GEN.datex<>XDC_VRAI
			
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
				HIS..FMC_HIS.numero = HIS..FMC_GEN.numero and
				HIS..FMC_HIS.cle = HIS..FMC_GEN.cle and
				HIS..FMC_GEN.datex<>XDC_VRAI
	
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

			insert #GRAPHE (horodate, horodate_fin, PR_debut, PR_fin, VR, VM2, VM1, VL)
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
			insert #VOLUME (horodate, horodate_fin, PR_debut, PR_fin, longueur, VR, VM2, VM1, VL)
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
		

			if @vl_Sens = XDC_SENS_SUD
			begin
				select 
					@vl_PRDebut = min(PR_fin),
					@vl_PRFin = max(PR_debut)
				from #VOLUME
			end
			else
				select 
					@vl_PRDebut = min(PR_debut),
					@vl_PRFin = max(PR_fin)
				from #VOLUME
#if 0
			/*A rechercher les segments concernés sur le max de longueur */
			insert #SEGMENTS ( PR_fin_por, PR_debut_seg, nombre_de_voies)
			select 
				PR_fin_por = CFG..RES_POR.PR_fin,
				PR_debut_seg = CFG..RES_SEG.PR_debut,
				nombre_de_voies
			from CFG..RES_POR, CFG..RES_SEG
			where autoroute = @vl_NumAutoroute and
				CFG..RES_POR.PR_debut < @vl_PRFin and 
				CFG..RES_POR.PR_fin >= @vl_PRDebut and
				portion = numero and
				sens = @vl_Sens and
				CFG..RES_SEG.PR_debut < @vl_PRFin
			order by CFG..RES_SEG.PR_debut desc

			/*A rechercher le PR de fin de chaque segment */
			insert #SEGMENTS_LONG ( PR_fin_por, PR_debut_seg, PR_fin_seg,nombre_de_voies)
			select
				D.PR_fin_por,
				D.PR_debut_seg,
				PR_fin_seg = F.PR_debut_seg,
				D.nombre_de_voies
			from #SEGMENTS D, #SEGMENTS F
			where F.PR_debut_seg > D.PR_debut_seg
			group by D.PR_debut_seg
			having F.PR_debut_seg > D.PR_debut_seg and
				F.PR_debut_seg = min(F.PR_debut_seg)
		
			/*A prise en compte des PR fantomes */
			update #SEGMENTS_LONG set PR_fin_seg = PR_fin_por
			where PR_fin_por < PR_fin_seg
		
			/*A supression des premiers segments non sur la zone de bouchon */
			delete #SEGMENTS_LONG
			where PR_fin_seg < @vl_PRDebut
		
			/*A ajout du dernier segment avec pour PR de fin celui du max de la longueur */
			set rowcount 1
			insert #SEGMENTS_LONG ( PR_fin_por, PR_debut_seg, PR_fin_seg,nombre_de_voies)
			select
				PR_fin_por,
				PR_debut_seg,
				@vl_PRFin,
				nombre_de_voies
			from #SEGMENTS
			set rowcount 0
#endif
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
		
				if @vl_Sens = XDC_SENS_SUD
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

				/* calcul de la surface : longueur * nombre de voies */
				/*! rechercher la surface sur les segments complets */
				/* pour les segments ou le nb de voies est < nb voies bloquees par le bouchon*/
				select @vl_Surface = (@vl_PRFin - @vl_PRDebut) * @vl_NbVoies

#if 0
				select
					@vl_Surface = isnull(sum((PR_fin_seg - PR_debut_seg) * nombre_de_voies), 0)
				from #SEGMENTS_LONG
				where PR_debut_seg < @vl_PRFin and
					PR_fin_seg >= @vl_PRDebut and
					PR_debut_seg >= @vl_PRDebut and
					PR_fin_seg <= @vl_PRFin and
					nombre_de_voies <= @vl_NbVoies

				select
					 @vl_Surface = @vl_Surface + isnull(sum((PR_fin_seg - PR_debut_seg) * @vl_NbVoies), 0)
				from #SEGMENTS_LONG
				where PR_debut_seg < @vl_PRFin and
					PR_fin_seg >= @vl_PRDebut and
					PR_debut_seg >= @vl_PRDebut and
					PR_fin_seg <= @vl_PRFin and
					nombre_de_voies > @vl_NbVoies

				/*! rechercher la surface sur le premier segment */
				select
					@vl_Surface = @vl_Surface + ((PR_fin_seg - @vl_PRDebut) * nombre_de_voies)
				from #SEGMENTS_LONG
				where PR_debut_seg < @vl_PRDebut and
					PR_fin_seg > @vl_PRDebut and
					PR_fin_seg < @vl_PRFin
		
				/*! rechercher dans le cas ou le bouchon est sur un seul segment la surface sur ce segment */
				/*! dans le debut est strictement inferieur au min */
				select
					@vl_Surface = @vl_Surface + ((@vl_PRFin - @vl_PRDebut) * nombre_de_voies)
				from #SEGMENTS_LONG
				where PR_fin_seg >= @vl_PRFin and
					PR_debut_seg < @vl_PRDebut
#endif		
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

		insert #LISTE_BOU (sit, mois, volume)
		values( @vl_Site, @vl_Mois, @vl_Volume)
	   end

	   select @vl_Nbre = @vl_Nbre - 1
	end


	
	/*A calcul par mois, par district */
	create table #TABLEAU (sit tinyint, mois tinyint, volume int null, nombre int null)
	insert #TABLEAU (mois,sit,volume,nombre)
	select distinct
		mois,
		sit,
		sum(volume),
		count(mois)
	from #LISTE_BOU
	group by mois, sit

	/*A ajout des compteurs a zero pour les districts n'ayant pas de bouchon sur un mois */
	select @vl_Mois = datepart(month,@va_Debut_in), @vl_MoisFin = datepart(month,@va_Fin_in)
        if @vl_MoisFin = 1
		select @vl_MoisFin = 12
	else select @vl_MoisFin = @vl_MoisFin - 1

	while @vl_Mois <= @vl_MoisFin
	begin
		insert #TABLEAU (mois,sit,volume,nombre)
		select
			@vl_Mois,
			numero,
			0,
			0
		from CFG..RES_DIS
		where numero != XDC_CI and type != 0 and numero not in (select sit from #TABLEAU where mois = @vl_Mois)

		select @vl_Mois = @vl_Mois + 1
	end

	/*A retourner les resultats */
	select
		mois,
		nom,
		volume,
		nombre
	from #TABLEAU, CFG..RES_DIS
	where numero = sit
	order by mois, sit

	return XDC_OK
go
