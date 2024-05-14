/*E*/
/*  Fichier : $Id: xzas42sp.prc,v 1.2 1998/09/22 13:32:06 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1998/09/22 13:32:06 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas42sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des mesures de trafic
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	28/07/95	: Creation	(V 1.1)
* P.N.	29/06/98	: separation EXP/HIS (1.2)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare un graphe bouchon
* 
* Sequence d'appel
* SP	XZAS42_Volume_Bouchon
* 
* Arguments en entree
* XDY_Autoroute	va_Autoroute_in		: numero d'autoroute
* XDY_PR	va_PRDebut_in		
* XDY_PR	va_PRFin_in		
* XDY_Datetime	va_HorodateDebut_in	: date de debut des mesures multiple de 6mn
* 
* Arguments en sortie
* XDY_Entier	va_Volume_out		
* XDY_Entier	va_Longueur_out		
*
* Code retour
* XDC_OK
* XDC_ARG_INV		: parametres d'entree null, ou valeur non autorisee 
* 
* Conditions d'utilisation
* Attention aucun controle n'est effectue pour verifier que le PR
* de debut et de fin de bouchon correspondent au sens de l'autoroute
* au niveau des fiches evenements : pour le graphe c'est identique
*
* La procedure stockee au CI recherche les informations 
* dans la base HIS si la date donnee est anterieure de plus d'un 
* mois tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Pas executable aux districts
* 
*
* Fonction
* selectionner dans la table FMC_GEN, FMC_HIS, FMC_BOU
-------------------------------------------------------*/

use PRC
go

create procedure XZAS42
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null,
	@va_Volume_out		int		= null	output,
	@va_Longueur_out	int		= null	output
as
	declare @vl_Type tinyint, 
		@vl_Cause int, @vl_CleCause tinyint,
		@vl_Numero int, @vl_Cle tinyint,
		@vl_PRDebut T_PR, @vl_PRFin T_PR, @vl_HorodateDebut datetime,
		@vl_Status int,
		@vl_HorodateFin datetime

	/*A controle parametres en entree */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV

	/* Recherche la tete de bouchon */
	select
		@vl_Cause = cause,
		@vl_CleCause = cle_cause
	from HIS..FMC_GEN
	where numero = @va_NumEvt_in and cle = @va_CleEvt_in

	/*! creation d'une table de stockage des PR de tete et */
	/*! queue de bouchon */
	create table #BOU_PR (numero int, cle tinyint, 
	horodate_debut datetime null, horodate_fin datetime null, 
	PR_debut int null, PR_fin int null, cause int null, 
	cle_cause tinyint null, type tinyint null)

#ifdef CI
	/*! cas ou la procedure est creee au CI */
	if not exists (select * from EXP..FMC_GEN
			where numero = @va_NumEvt_in and cle = @va_CleEvt_in)
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAS42
			@va_NumEvt_in		,
			@va_CleEvt_in		,
			@va_Volume_out		output,
			@va_Longueur_out	output

		if @vl_Status = null
                       	return XDC_PRC_INC
                else return @vl_Status
	end
	else
	begin
		/*B lecture des PR de debut de bouchon */
		insert #BOU_PR (numero, cle, horodate_debut, PR_debut)
		select 
			EXP..FMC_HIS.numero,
			EXP..FMC_HIS.cle,
			EXP..FMC_HIS.horodate_validation,
			EXP..FMC_HIS.PR
		from EXP..FMC_GEN, EXP..FMC_HIS
		where	EXP..FMC_GEN.numero = @vl_Cause and
			EXP..FMC_GEN.cle = @vl_CleCause and
			EXP..FMC_HIS.numero = EXP..FMC_GEN.numero and
			EXP..FMC_HIS.cle = EXP..FMC_GEN.cle 

		/*B lecture des PR de fin de bouchon */
		/*B pour ce morceau d'autoroute      */
		/*! en prenant en plus les derniers PR */
		/*! inferieurs a la date de debut et   */
		/*! et les premiers superieurs a la date */
		/*! de fin pour chaque bouchon         */
		insert #BOU_PR (numero, cle, horodate_fin, 
		PR_fin, cause, cle_cause, type)
		select 
			EXP..FMC_HIS.numero,
			EXP..FMC_HIS.cle,
			EXP..FMC_HIS.horodate_validation,
			EXP..FMC_HIS.PR,
			EXP..FMC_GEN.cause,
			EXP..FMC_GEN.cle_cause,
			EXP..FMC_BOU.type
		from EXP..FMC_GEN, EXP..FMC_HIS, EXP..FMC_BOU
		where	EXP..FMC_GEN.numero = @va_NumEvt_in and
			EXP..FMC_GEN.cle = @va_CleEvt_in and
			EXP..FMC_HIS.numero = EXP..FMC_GEN.numero and
			EXP..FMC_HIS.cle = EXP..FMC_GEN.cle and
			EXP..FMC_BOU.numero = EXP..FMC_GEN.numero and
			EXP..FMC_BOU.cle = EXP..FMC_GEN.cle and
			EXP..FMC_BOU.horodate = EXP..FMC_HIS.horodate_validation
	end
#else
#ifdef HIST
		/*B lecture des PR de debut de bouchon */
		insert #BOU_PR (numero, cle, horodate_debut, PR_debut)
		select 
			HIS..FMC_HIS.numero,
			HIS..FMC_HIS.cle,
			HIS..FMC_HIS.horodate_validation,
			HIS..FMC_HIS.PR
		from HIS..FMC_GEN, HIS..FMC_HIS
		where	HIS..FMC_GEN.numero = @vl_Cause and
			HIS..FMC_GEN.cle = @vl_CleCause and
			HIS..FMC_HIS.numero = HIS..FMC_GEN.numero and
			HIS..FMC_HIS.cle = HIS..FMC_GEN.cle 
		
		/*B lecture des PR de fin de bouchon */
		insert #BOU_PR (numero, cle, horodate_fin, 
		PR_fin, cause, cle_cause, type)
		select 
			HIS..FMC_HIS.numero,
			HIS..FMC_HIS.cle,
			HIS..FMC_HIS.horodate_validation,
			HIS..FMC_HIS.PR,
			HIS..FMC_GEN.cause,
			HIS..FMC_GEN.cle_cause,
			HIS..FMC_BOU.type
		from HIS..FMC_GEN, HIS..FMC_HIS, HIS..FMC_BOU
		where	HIS..FMC_GEN.numero = @va_NumEvt_in and
			HIS..FMC_GEN.cle = @va_CleEvt_in and
			HIS..FMC_HIS.numero = HIS..FMC_GEN.numero and
			HIS..FMC_HIS.cle = HIS..FMC_GEN.cle and
			HIS..FMC_BOU.numero = HIS..FMC_GEN.numero and
			HIS..FMC_BOU.cle = HIS..FMC_GEN.cle and
			HIS..FMC_BOU.horodate = HIS..FMC_HIS.horodate_validation

#else
		/*B lecture des PR de debut de bouchon */
		insert #BOU_PR (numero, cle, horodate_debut, PR_debut)
		select 
			EXP..FMC_HIS.numero,
			EXP..FMC_HIS.cle,
			EXP..FMC_HIS.horodate_validation,
			EXP..FMC_HIS.PR
		from EXP..FMC_GEN, EXP..FMC_HIS
		where	EXP..FMC_GEN.numero = @vl_Cause and
			EXP..FMC_GEN.cle = @vl_CleCause and
			EXP..FMC_HIS.numero = EXP..FMC_GEN.numero and
			EXP..FMC_HIS.cle = EXP..FMC_GEN.cle 

		/*B lecture des PR de fin de bouchon */
		/*B pour ce morceau d'autoroute      */
		/*! en prenant en plus les derniers PR */
		/*! inferieurs a la date de debut et   */
		/*! et les premiers superieurs a la date */
		/*! de fin pour chaque bouchon         */
		insert #BOU_PR (numero, cle, horodate_fin, 
		PR_fin, cause, cle_cause, type)
		select 
			EXP..FMC_HIS.numero,
			EXP..FMC_HIS.cle,
			EXP..FMC_HIS.horodate_validation,
			EXP..FMC_HIS.PR,
			EXP..FMC_GEN.cause,
			EXP..FMC_GEN.cle_cause,
			EXP..FMC_BOU.type
		from EXP..FMC_GEN, EXP..FMC_HIS, EXP..FMC_BOU
		where	EXP..FMC_GEN.numero = @va_NumEvt_in and
			EXP..FMC_GEN.cle = @va_CleEvt_in and
			EXP..FMC_HIS.numero = EXP..FMC_GEN.numero and
			EXP..FMC_HIS.cle = EXP..FMC_GEN.cle and
			EXP..FMC_BOU.numero = EXP..FMC_GEN.numero and
			EXP..FMC_BOU.cle = EXP..FMC_GEN.cle and
			EXP..FMC_BOU.horodate = EXP..FMC_HIS.horodate_validation
#endif
#endif

	/*! creation d'une table de travail de stockage du graphe de bouchon */
	create table #GRA_BOU (numero int, cle tinyint, 
	horodate datetime , horodate_fin datetime null, PR_debut int , 
	PR_fin int, type tinyint)

	/*! creation d'une table des bouchon en sens inconnu */
	/*! a virer                                          */
	create table #BOU_INC (numero int, cle tinyint)

	/*! creation d'un curseur pour parcourir la table des */
	/*! PR de queue de bouchon */
	declare Pointeur_Pr_Queue cursor
	for select numero, cle, horodate_fin, PR_fin, cause, cle_cause, type
	from #BOU_PR
	where PR_debut is null and PR_fin is not null
	order by horodate_fin

	open Pointeur_Pr_Queue

	/*A construction du graphe en recherchant toutes  */
	/*A les queues de bouchon dont la date de         */
	/*A validation est comprise dans l'intervalle     */
	/*A de temps (date donnee , date donnee + 6h )    */
	fetch Pointeur_Pr_Queue into @vl_Numero, @vl_Cle, @vl_HorodateFin, 
	@vl_PRFin, @vl_Cause, @vl_CleCause, @vl_Type
	while @@sqlstatus = 0
	begin
		/*B rechercher le PR de debut de bouchon */
		/*B correspondant a la queue */
		select 
			@vl_PRDebut = PR_debut,
			@vl_HorodateDebut = horodate_debut
		from #BOU_PR
		where numero = @vl_Cause and cle = @vl_CleCause and
			horodate_debut in (select max(horodate_debut) from #BOU_PR
					where numero = @vl_Cause and cle = @vl_CleCause and
						horodate_debut <= @vl_HorodateFin)
		if @@rowcount = 0
		begin
			insert #BOU_INC (numero, cle)
			values(@vl_Numero, @vl_Cle)
			insert #BOU_INC (numero, cle)
			values(@vl_Cause, @vl_CleCause)
		end
		else
		begin

			insert #GRA_BOU
			values( @vl_Cause, @vl_CleCause, 
			@vl_HorodateFin, null, @vl_PRDebut, @vl_PRFin, @vl_Type)
		end

		/* lecture de la queue de bouchon suivante */
		fetch Pointeur_Pr_Queue into @vl_Numero, @vl_Cle, @vl_HorodateFin, 
		@vl_PRFin, @vl_Cause, @vl_CleCause, @vl_Type
	end
	close Pointeur_Pr_Queue

	delete #GRA_BOU
	from #GRA_BOU, #BOU_INC
	where #GRA_BOU.numero = #BOU_INC.numero and
		#GRA_BOU.cle = #BOU_INC.cle

	/* creation d'un curseur pour parcourir la table des */
	/* PR de tete de bouchon */
	declare Pointeur_Pr_Tete cursor
	for select numero, cle, horodate_debut, PR_debut
	from #BOU_PR
	where PR_fin is null and PR_debut is not null
	order by horodate_debut desc
	open Pointeur_Pr_Tete

	/*A construction du graphe en recherchant toutes    */
	/*A les tetes de bouchon dont le PR et la date      */
	/*A de validation sont compris dans les intervalles */
	/*A specifies en entree                             */
	fetch Pointeur_Pr_Tete into @vl_Numero, @vl_Cle, 
	@vl_HorodateDebut, @vl_PRDebut
	while @@sqlstatus = 0
	begin
		/*B rechercher le PR de fin de bouchon */
		/*B correspondant a la tete */
		select 
			@vl_Type = type,
			@vl_PRFin = PR_fin,
			@vl_HorodateFin = horodate_fin
		from #BOU_PR
		where cause = @vl_Numero and cle_cause = @vl_Cle and
			horodate_fin in (select max(horodate_fin)from #BOU_PR
					where cause = @vl_Numero and cle_cause = @vl_Cle and
					horodate_fin <= @vl_HorodateDebut)

		/*! si une queue est trouvee alors on insere la mesure */
		/*! correspondant a la validation de la tete sinon     */
		/*! pas d'insertion car le bouchon n'existe pas encore */
		if @@rowcount != 0
		begin
			insert #GRA_BOU
			values( @vl_Numero, @vl_Cle, 
			@vl_HorodateDebut, null, @vl_PRDebut, 
			@vl_PRFin, @vl_Type)
		end

		/* lecture de la tete de bouchon suivante */
		fetch Pointeur_Pr_Tete into @vl_Numero, @vl_Cle, 
		@vl_HorodateDebut, @vl_PRDebut
	end
	close Pointeur_Pr_Tete


	/* creation d'une table de stockage du graphe de bouchon */
	create table #GRAPHE ( horodate datetime, horodate_fin datetime, 
	PR_debut int, PR_fin int, type tinyint, volume int null)

	/*! positionner la date de fin de chaque mesure */
	insert #GRAPHE (horodate, horodate_fin, PR_debut, PR_fin, type)
	select
		g2.horodate,
		g1.horodate,
		g2.PR_debut,
		g2.PR_fin,
		g2.type
	from #GRA_BOU g1, #GRA_BOU g2
	where g2.numero = g1.numero and g2.cle = g1.cle and
		g1.horodate > g2.horodate
	group by g1.numero, g1.cle, g2.numero, g2.cle, g2.horodate
	having g1.horodate = min(g1.horodate)

	/*A Calcul du volume d'encombrement */
	select distinct *
	into #VOLUME
	from #GRAPHE

	update #VOLUME set volume = datediff(minute, horodate, horodate_fin) * abs(PR_debut - PR_fin)

	select 
		@va_Volume_out = sum(volume), 
		@va_Longueur_out = abs(max(PR_debut - PR_fin))
	from #VOLUME

	return XDC_OK
go
