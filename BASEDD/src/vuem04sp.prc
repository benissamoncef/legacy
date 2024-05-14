/*E*/
/*  Fichier : $Id: vuem04sp.prc,v 1.13 2019/12/05 17:43:09 gesconf Exp $        Release : $Revision: 1.13 $        Date : $Date: 2019/12/05 17:43:09 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER vuem04sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* vues metiers
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	19/07/95	: Creation
* C.T.	24/08/95	: Modif ordre des districts (V 1.2)
* C.T.	26/09/95	: correction des recherches en cas de travaux en cours  (V 1.3)
* C.T.	08/07/96	: optimisation et modif comptage des voies uniques (V 1.4)
* C.T.	24/02/98	: Modif pour pb default suite a passage en SQL 11.0.3.2 (V1.5)
* C.T.	08/04/98	: correction erreur d'insert dans LISTE_TRV (1.6)
* P.N.	05/02/99	: Modif interg. dem/1724 v1.8
* P.N.  19/04/99	: Correction dem/1759 v1.9
* P.N	22/04/99	: Correction dem/1761 v1.10
* P.N	28/05/99	: Mise en coherence des demandes annee et mois v1.11
* JPL	29/09/10	: Adaptation compilateur C (# en debut de ligne) 1.12
* P.N.	05/12/19	: Ajout des basculement pour travaux 1.13
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	VUEM04_Signalisation_Travaux
* 
* Arguments en entree
* XDY_Horodate	va_Debut_in
* XDY_Horodate	va_Fin_in
* XDY_Entier	va_DebutJour_in
* XDY_Entier	va_FinJour_in
* XDY_Entier	va_DebutNuit_in
* XDY_Entier	va_FinNuit_in
* 
* Arguments en sortie
*
* liste retournee
* XDC_Octet	mois
* XDC_NOM	district
* XDC_Entier	nb_voies_jour
* XDC_Entier	nb_voies_nuit
* XDC_Entier	nb_bascul_jour
* XDC_Entier	nb_bascul_nuit
* char(4)	autoroute
* 
* Service rendu
* Pose des signalisations travaux par district/par mois/ par
* jour-nuit sur un an
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametres d'entree null ou sit incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* recherche sur une annee max
* la date de fin est excluse
* la date de fin doit etre le premier jour à la premiere seconde d'un mois :ex : "01/02/1996"
* 
* Fonction
-----------------------------------------------------------------------*/

use PRC
go
drop procedure VUEM04
go

create procedure VUEM04
	@va_Debut_in	datetime	= null,
	@va_Fin_in	datetime	= null,
	@va_DebutJour_in	int	= null,
	@va_FinJour_in		int	= null,
	@va_DebutNuit_in	int	= null,
	@va_FinNuit_in		int	= null
as
	declare 
		@vl_Mois tinyint, @vl_MoisFin tinyint,
		@vl_Site T_SITE, @vl_Inter int,
		@vl_DateCourante datetime, @vl_DateButee datetime,
		@vl_MoisBute tinyint, @vl_HeureButee tinyint

	/*A controle des parametres d'entree */
	if @va_Debut_in is null or @va_Fin_in is null or @va_DebutJour_in is null or
	   @va_FinJour_in is null or @va_DebutNuit_in is null or
	   @va_FinNuit_in is null
		return XDC_ARG_INV

	/*A rechercher les limites de chaque district sur chaque autoroute */
	select
		sit = CFG..RES_DIS.numero,
		autoroute,
		PR_debut = min(PR_debut),
		PR_fin = max(PR_fin)
	into #LIMITE_DIS
	from CFG..RES_DIS,CFG..RES_POR
	where CFG..RES_DIS.numero != 43 and type != 0 and
		district = CFG..RES_DIS.numero
	group by CFG..RES_DIS.numero, district,autoroute

	create table #LISTE_TRV (sit tinyint,
				nb_voies_jour int default 0,
				nb_voies_nuit int default 0, 
				nb_bascul_jour int default 0, 
				nb_bascul_nuit int default 0, 
				mois tinyint,
				autoroute tinyint default 1)

	select @vl_Mois = datepart(month, @va_Debut_in),
		@vl_MoisFin = datepart(month, @va_Fin_in),
		@vl_DateCourante = getdate()

	if @vl_DateCourante >  @va_Fin_in
	begin
		select  @vl_DateButee = dateadd(second,-1,@va_Fin_in)
		select  @vl_MoisBute = datepart(month, @vl_DateButee),
			@vl_HeureButee = datepart(hour, @vl_DateButee)
	end
	else
	begin
		select  @vl_DateButee = @vl_DateCourante,
			@vl_MoisBute = datepart(month, @vl_DateCourante),
			@vl_HeureButee = datepart(hour, @vl_DateCourante)
	end

	/*B liste des basculements sur un mois */
	select
		HIS..FMC_GEN.numero,
		HIS..FMC_GEN.cle,
		cause,
		cle_cause,
		debut,
		fin = isnull(fin, @vl_DateButee),
		mois_debut = datepart(month,debut),
		mois_fin = isnull(datepart(month,fin), @vl_MoisBute),
		heure_debut = datepart(hour,debut),
		heure_fin = isnull(datepart(hour,fin), @vl_HeureButee),
		HIS..FMC_HIS.autoroute,
		HIS..FMC_HIS.sens
	into #LISTE_BASCUL
	from HIS..FMC_GEN, HIS..FMC_HIS
	where type in(XZAEC_FMC_Basculement,XZAEC_FMC_BasculTravaux) and
		datex<>XDC_VRAI and
		cause is not null and
		debut < @va_Fin_in and (fin is null or fin > @va_Debut_in) and
		HIS..FMC_HIS.numero = HIS..FMC_GEN.numero and
		HIS..FMC_HIS.cle = HIS..FMC_GEN.cle
	group by HIS..FMC_GEN.numero, HIS..FMC_GEN.cle,
		HIS..FMC_HIS.numero, HIS..FMC_HIS.cle
	having type in(XZAEC_FMC_Basculement,XZAEC_FMC_BasculTravaux) and
		datex<>XDC_VRAI and
		cause is not null and
		debut < @va_Fin_in and (fin is null or fin > @va_Debut_in) and
		HIS..FMC_HIS.numero = HIS..FMC_GEN.numero and
		HIS..FMC_HIS.cle = HIS..FMC_GEN.cle and
		horodate_validation = max(horodate_validation)

	/*B liste des basculements sur un mois dus aux travaux */
	delete #LISTE_BASCUL
	from #LISTE_BASCUL, HIS..FMC_GEN
	where HIS..FMC_GEN.numero = #LISTE_BASCUL.cause and
		HIS..FMC_GEN.cle = #LISTE_BASCUL.cle_cause and
		HIS..FMC_GEN.type != XZAEC_FMC_Travaux and
		HIS..FMC_GEN.datex<>XDC_VRAI

	/*B restreindre sur la periode demandee */
	update #LISTE_BASCUL set debut = @va_Debut_in, mois_debut = @vl_Mois
	where debut < @va_Debut_in
	update #LISTE_BASCUL set fin = @vl_DateButee, mois_fin = @vl_MoisBute
	where fin >= @va_Fin_in

	/* rechercher les PR de tete et queue pour chaque basculement en sens nord */
	create table #BASCUL(mois_debut tinyint, mois_fin tinyint, heure_debut tinyint, heure_fin tinyint, 
			autoroute tinyint, PR_tete int , PR int, 
			sit_queue tinyint null, sit_tete tinyint null)
	insert #BASCUL (mois_debut, mois_fin, heure_debut, heure_fin, autoroute, PR_tete, PR)
	select
		mois_debut,
		mois_fin,
		heure_debut,
		heure_fin,
		LB.autoroute,
		min(PR),
		max(localisation_PR_fin)
	from #LISTE_BASCUL LB, HIS..FMC_HIS, HIS..FMC_BAS
	where LB.sens = XDC_SENS_NORD and HIS..FMC_HIS.numero = LB.numero and
		HIS..FMC_HIS.cle = LB.cle and
		HIS..FMC_BAS.numero = LB.numero and
		HIS..FMC_BAS.cle = LB.cle
	group by LB.numero, LB.cle, 
		HIS..FMC_HIS.numero, HIS..FMC_HIS.cle, HIS..FMC_BAS.numero, HIS..FMC_BAS.cle

	/* rechercher les PR de tete et queue pour chaque basculement en sens sud */
	insert #BASCUL (mois_debut, mois_fin, heure_debut, heure_fin, autoroute, PR_tete, PR)
	select
		mois_debut,
		mois_fin,
		heure_debut,
		heure_fin,
		LB.autoroute,
		max(PR),
		min(localisation_PR_fin)
	from #LISTE_BASCUL LB, HIS..FMC_HIS, HIS..FMC_BAS
	where LB.sens = XDC_SENS_SUD and HIS..FMC_HIS.numero = LB.numero and
		HIS..FMC_HIS.cle = LB.cle and
		HIS..FMC_BAS.numero = LB.numero and
		HIS..FMC_BAS.cle = LB.cle
	group by LB.numero, LB.cle,
		HIS..FMC_HIS.numero, HIS..FMC_HIS.cle, HIS..FMC_BAS.numero, HIS..FMC_BAS.cle

	/*A calcul les districts concernes par chaque basculement */
	/*update #BASCUL set sit_queue = district
	from CFG..RES_POR,#BASCUL
	where	CFG..RES_POR.autoroute = #BASCUL.autoroute and
		CFG..RES_POR.PR_debut <= #BASCUL.PR and
		CFG..RES_POR.PR_fin > #BASCUL.PR
	update #BASCUL set sit_tete = district
	from CFG..RES_POR,#BASCUL
	where	CFG..RES_POR.autoroute = #BASCUL.autoroute and
		CFG..RES_POR.PR_debut <= #BASCUL.PR and
		CFG..RES_POR.PR_fin > #BASCUL.PR*/

	update #BASCUL set sit_queue = sit
	from #LIMITE_DIS LD, #BASCUL B
	where   LD.autoroute = B.autoroute and
		LD.PR_debut <= B.PR and
		LD.PR_fin > B.PR
	update #BASCUL set sit_tete = sit
	from #LIMITE_DIS LD, #BASCUL B
	where   LD.autoroute = B.autoroute and
		LD.PR_debut <= B.PR and
		LD.PR_fin > B.PR

	/*A compter un basculement par site concerne */
	create table #COMPTE_BASCUL(mois_debut tinyint, mois_fin tinyint, heure_debut tinyint, heure_fin tinyint, 
					autoroute tinyint, sit tinyint)
	insert #COMPTE_BASCUL
	select 
		mois_debut,
		mois_fin,
		heure_debut,
		heure_fin,
		autoroute,
		sit = sit_queue
	from #BASCUL
	where sit_tete = sit_queue
	insert #COMPTE_BASCUL
	select 
		mois_debut,
		mois_fin,
		heure_debut,
		heure_fin,
		autoroute,
		sit = sit_queue
	from #BASCUL
	where sit_tete != sit_queue
	insert #COMPTE_BASCUL
	select 
		mois_debut,
		mois_fin,
		heure_debut,
		heure_fin,
		autoroute,
		sit = sit_tete
	from #BASCUL
	where sit_tete != sit_queue

	/* calcul le nombre de basculement de jour durant le premier mois des basculements */
	insert #LISTE_TRV (mois, sit, nb_bascul_jour, autoroute, nb_voies_jour, nb_voies_nuit, nb_bascul_nuit)
	select distinct
		mois_debut, 
		sit,
		count(*),
		autoroute,
		0,
		0,
		0
	from #COMPTE_BASCUL
	where 	heure_debut >= @va_DebutJour_in and 
		heure_debut < @va_FinJour_in and
		heure_fin >= @va_DebutJour_in and
		heure_fin < @va_FinJour_in
	group by sit, mois_debut, autoroute

	/* calcul le nombre de basculement de jour durant plus d'un mois sur le mois de fin des basculments */
	insert #LISTE_TRV (mois, sit, nb_bascul_jour, autoroute, nb_voies_jour, nb_voies_nuit, nb_bascul_nuit)
	select distinct
		mois_fin, 
		sit,
		count(*),
		autoroute,
		0,
		0,
		0
	from #COMPTE_BASCUL
	where	mois_fin != mois_debut and
		heure_debut >= @va_DebutJour_in and 
		heure_debut < @va_FinJour_in and
		heure_fin >= @va_DebutJour_in and
		heure_fin < @va_FinJour_in
	group by sit, mois_fin, autoroute

	/* calcul le nombre de basculement de nuit durant le premier mois des basculements */
	insert #LISTE_TRV (mois, sit, nb_bascul_nuit, autoroute, nb_voies_jour, nb_voies_nuit, nb_bascul_jour)
	select 
		mois_debut, 
		sit,
		count(*),
		autoroute,
		0,
		0,
		0
	from #COMPTE_BASCUL
	where 	(heure_debut >= @va_DebutNuit_in or 
		heure_debut < @va_FinNuit_in) or
		(heure_fin >= @va_DebutNuit_in or
		heure_fin < @va_FinNuit_in)
	group by sit, mois_debut, autoroute

	/* calcul le nombre de basculement de nuit durant plus d'un mois sur le mois de fin des basculments */
	insert #LISTE_TRV (mois, sit, nb_bascul_nuit, autoroute, nb_voies_jour, nb_voies_nuit, nb_bascul_jour)
	select 
		mois_fin, 
		sit,
		count(*),
		autoroute,
		0,
		0,
		0
	from #COMPTE_BASCUL
	where ((heure_debut >= @va_DebutNuit_in or 
		heure_debut < @va_FinNuit_in) or
		(heure_fin >= @va_DebutNuit_in or
		heure_fin < @va_FinNuit_in)) and
		mois_fin != mois_debut
	group by sit, mois_fin, autoroute

	/* calcul le nombre de basculement de jour pour les mois entre mois de debut et de fin */
        select @vl_Inter = @vl_Mois
        while @vl_Inter < @vl_MoisFin
        begin
                insert #LISTE_TRV (mois, sit, nb_bascul_jour, autoroute, nb_voies_jour, nb_voies_nuit, nb_bascul_nuit)
                select
                        @vl_Inter,
                        sit,
                        count(*),
                        autoroute,
                        0,
                        0,
                        0
                from #COMPTE_BASCUL
                where   mois_debut < @vl_Inter and
                        mois_fin > @vl_Inter and
                        heure_debut >= @va_DebutJour_in and
                        heure_debut < @va_FinJour_in and
                        heure_fin >= @va_DebutJour_in and
                        heure_fin < @va_FinJour_in
                group by sit, autoroute

                insert #LISTE_TRV (mois, sit, nb_bascul_nuit, autoroute, nb_voies_jour, nb_voies_nuit, nb_bascul_jour)
                select
                        @vl_Inter,
                        sit,
                        count(*),
                        autoroute,
                        0,
                        0,
                        0
                from #COMPTE_BASCUL
                where   mois_debut < @vl_Inter and
                        mois_fin > @vl_Inter and
                        ((heure_debut >= @va_DebutNuit_in or
                        heure_debut < @va_FinNuit_in) or
                        (heure_fin >= @va_DebutNuit_in or
                        heure_fin < @va_FinNuit_in))
                group by sit, autoroute

                select @vl_Inter = @vl_Inter + 1
        end

	/*B liste des travaux sur la periode demandee */
	select
		HIS..FMC_GEN.numero,
		HIS..FMC_GEN.cle,
		debut,
		fin = isnull(fin, @vl_DateButee),
		mois_debut = datepart(month,debut),
		mois_fin = isnull(datepart(month,fin), @vl_MoisBute),
		heure_debut = datepart(hour,debut),
		heure_fin = isnull(datepart(hour,fin), @vl_HeureButee),
		HIS..FMC_HIS.autoroute,
		HIS..FMC_HIS.sens
	into #LISTE_VOIES
	from HIS..FMC_GEN, HIS..FMC_HIS
	where type = XZAEC_FMC_Travaux and
		datex<>XDC_VRAI and
		debut < @va_Fin_in and (fin is null or fin > @va_Debut_in) and
		HIS..FMC_HIS.numero = HIS..FMC_GEN.numero and
		HIS..FMC_HIS.cle = HIS..FMC_GEN.cle and
		((VL = XDC_VOIE_NEUTRALISEE or VL = XDC_VOIE_BLOQUEE_NEUTRALISEE) or
		(VR = XDC_VOIE_NEUTRALISEE or VR = XDC_VOIE_BLOQUEE_NEUTRALISEE) or
		(VM1 = XDC_VOIE_NEUTRALISEE or VM1 = XDC_VOIE_BLOQUEE_NEUTRALISEE) or
		(VM2 = XDC_VOIE_NEUTRALISEE or VM2 = XDC_VOIE_BLOQUEE_NEUTRALISEE))
	group by HIS..FMC_GEN.numero, HIS..FMC_GEN.cle,
		HIS..FMC_HIS.numero, HIS..FMC_HIS.cle
	having 	type = XZAEC_FMC_Travaux and
		datex<>XDC_VRAI and
		debut < @va_Fin_in and (fin is null or fin > @va_Debut_in) and
		HIS..FMC_HIS.numero = HIS..FMC_GEN.numero and
		HIS..FMC_HIS.cle = HIS..FMC_GEN.cle and
		((VL = XDC_VOIE_NEUTRALISEE or VL = XDC_VOIE_BLOQUEE_NEUTRALISEE) or
		(VR = XDC_VOIE_NEUTRALISEE or VR = XDC_VOIE_BLOQUEE_NEUTRALISEE) or
		(VM1 = XDC_VOIE_NEUTRALISEE or VM1 = XDC_VOIE_BLOQUEE_NEUTRALISEE) or
		(VM2 = XDC_VOIE_NEUTRALISEE or VM2 = XDC_VOIE_BLOQUEE_NEUTRALISEE)) and
		horodate_validation = max(horodate_validation)

	/*B restreindre sur la periode demandee */
	update #LISTE_VOIES set debut = @va_Debut_in, mois_debut = @vl_Mois
	where debut < @va_Debut_in
	update #LISTE_VOIES set fin = @vl_DateButee, mois_fin = @vl_MoisBute
	where fin >= @va_Fin_in

	/* rechercher les PR de tete et queue pour chaque basculement en sens nord */
	create table #TRV(mois_debut tinyint, mois_fin tinyint, heure_debut tinyint, heure_fin tinyint, 
			autoroute tinyint, PR_tete int , PR int, sit_queue tinyint null, sit_tete tinyint null)
	insert #TRV (mois_debut, mois_fin, heure_debut, heure_fin, autoroute, PR_tete, PR)
	select
		mois_debut,
		mois_fin,
		heure_debut,
		heure_fin,
		LV.autoroute,
		PR_tete = isnull(min(localisation_tete),max(PR)),
		max(PR)
	from #LISTE_VOIES LV, HIS..FMC_HIS, HIS..FMC_TRH
	where LV.sens = XDC_SENS_NORD and HIS..FMC_HIS.numero = LV.numero and
		HIS..FMC_HIS.cle = LV.cle and
		HIS..FMC_TRH.numero = LV.numero and
		HIS..FMC_TRH.cle = LV.cle
	group by LV.numero, LV.cle,
		HIS..FMC_HIS.numero, HIS..FMC_HIS.cle,
		LV.numero, LV.cle

	/* rechercher les PR de tete et queue pour chaque basculement en sens sud */
	insert #TRV (mois_debut, mois_fin, heure_debut, heure_fin, autoroute, PR_tete, PR)
	select
		mois_debut,
		mois_fin,
		heure_debut,
		heure_fin,
		LV.autoroute,
		PR_tete = isnull(max(localisation_tete),min(PR)),
		min(PR)
	from #LISTE_VOIES LV, HIS..FMC_HIS, HIS..FMC_TRH
	where LV.sens = XDC_SENS_SUD and HIS..FMC_HIS.numero = LV.numero and
		HIS..FMC_HIS.cle = LV.cle and
		HIS..FMC_TRH.numero = LV.numero and
		HIS..FMC_TRH.cle = LV.cle
	group by LV.numero, LV.cle,
		HIS..FMC_HIS.numero, HIS..FMC_HIS.cle,
		LV.numero, LV.cle

	/*A calcul les districts concernes par chaque basculement */
	/*update #TRV set sit_queue = district
	from CFG..RES_POR,#TRV
	where	CFG..RES_POR.autoroute = #TRV.autoroute and
		CFG..RES_POR.PR_debut <= #TRV.PR and
		CFG..RES_POR.PR_fin > #TRV.PR
	update #TRV set sit_tete = district
	from CFG..RES_POR,#TRV
	where	CFG..RES_POR.autoroute = #TRV.autoroute and
		CFG..RES_POR.PR_debut <= #TRV.PR_tete and
		CFG..RES_POR.PR_fin > #TRV.PR_tete*/

	update #TRV set sit_queue = sit
	from #LIMITE_DIS LD, #TRV
	where   LD.autoroute = #TRV.autoroute and
		LD.PR_debut <= #TRV.PR and
		LD.PR_fin > #TRV.PR
	update #TRV set sit_tete = sit
	from #LIMITE_DIS LD, #TRV
	where   LD.autoroute = #TRV.autoroute and
		LD.PR_debut <= #TRV.PR_tete and
		LD.PR_fin > #TRV.PR_tete

	/*A compter un basculement par site concerne */
	create table #COMPTE_VOIES(mois_debut tinyint, mois_fin tinyint, 
				heure_debut tinyint, heure_fin tinyint, 
				autoroute tinyint, sit tinyint)
	insert #COMPTE_VOIES
	select 
		mois_debut,
		mois_fin,
		heure_debut,
		heure_fin,
		autoroute,
		sit = sit_queue
	from #TRV
	where sit_tete = sit_queue
	insert #COMPTE_VOIES
	select 
		mois_debut,
		mois_fin,
		heure_debut,
		heure_fin,
		autoroute,
		sit = sit_queue
	from #TRV
	where sit_tete != sit_queue
	insert #COMPTE_VOIES
	select 
		mois_debut,
		mois_fin,
		heure_debut,
		heure_fin,
		autoroute,
		sit = sit_tete
	from #TRV
	where sit_tete != sit_queue

	/* calcul le nombre de voies uniques de jour durant le premier mois des voies uniques */
	insert #LISTE_TRV (mois, sit, nb_voies_jour, autoroute, nb_voies_nuit, nb_bascul_jour, nb_bascul_nuit)
	select 
		mois_debut, 
		sit,
		count(*),
		autoroute,
		0,
		0,
		0
	from #COMPTE_VOIES
	where 	heure_debut >= @va_DebutJour_in and 
		heure_debut < @va_FinJour_in and
		heure_fin >= @va_DebutJour_in and
		heure_fin < @va_FinJour_in 
	group by sit, mois_debut, autoroute

	/* calcul le nombre de voies uniques de jour durant plus d'un mois sur le mois de fin des voies uniques */
	insert #LISTE_TRV (mois, sit, nb_voies_jour, autoroute, nb_voies_nuit, nb_bascul_jour, nb_bascul_nuit)
	select 
		mois_fin, 
		sit,
		count(*),
		autoroute,
		0,
		0,
		0
	from #COMPTE_VOIES
	where heure_debut >= @va_DebutJour_in and 
		heure_debut < @va_FinJour_in and
		heure_fin >= @va_DebutJour_in and
		heure_fin < @va_FinJour_in and
		mois_fin != mois_debut
	group by sit, mois_fin, autoroute

	/* calcul le nombre de voies uniques de nuit durant le premier mois des voies uniques */
	insert #LISTE_TRV (mois, sit, nb_voies_nuit, autoroute, nb_voies_jour, nb_bascul_jour, nb_bascul_nuit)
	select 
		mois_debut, 
		sit,
		count(*),
		autoroute,
		0,
		0,
		0
	from #COMPTE_VOIES
	where 	(heure_debut >= @va_DebutNuit_in or 
		heure_debut < @va_FinNuit_in) or
		(heure_fin >= @va_DebutNuit_in or
		heure_fin < @va_FinNuit_in)
	group by sit, mois_debut, autoroute

	/* calcul le nombre de voies uniques de nuit durant plus d'un mois sur le mois de fin des voies uniques */
	insert #LISTE_TRV (mois, sit, nb_voies_nuit, autoroute,  nb_voies_jour, nb_bascul_jour, nb_bascul_nuit)
	select 
		mois_fin, 
		sit,
		count(*),
		autoroute,
		0,
		0,
		0
	from #COMPTE_VOIES
	where ((heure_debut >= @va_DebutNuit_in or 
		heure_debut < @va_FinNuit_in) or
		(heure_fin >= @va_DebutNuit_in or
		heure_fin < @va_FinNuit_in)) and
		mois_fin != mois_debut
	group by sit, mois_fin, autoroute

	/* calcul le nombre de basculement de jour pour les mois entre mois de debut et de fin */
        select @vl_Inter = @vl_Mois
        while @vl_Inter < @vl_MoisFin
        begin

                insert #LISTE_TRV (mois, sit, nb_voies_jour, autoroute, nb_voies_nuit, nb_bascul_jour, nb_bascul_nuit)
                select
                        @vl_Inter,
                        sit,
                        count(*),
                        autoroute,
                        0,
                        0,
                        0
                from #COMPTE_VOIES
                where   mois_debut < @vl_Inter and
                        mois_fin > @vl_Inter and
                        heure_debut >= @va_DebutJour_in and
                        heure_debut < @va_FinJour_in and
                        heure_fin >= @va_DebutJour_in and
                        heure_fin < @va_FinJour_in
                group by sit, autoroute

                insert #LISTE_TRV (mois, sit, nb_voies_nuit, autoroute, nb_voies_jour, nb_bascul_jour, nb_bascul_nuit)
                select
                        @vl_Inter,
                        sit,
                        count(*),
                        autoroute,
                        0,
                        0,
                        0
                from #COMPTE_VOIES
                where   mois_debut < @vl_Inter and
                        mois_fin > @vl_Inter and
                        ((heure_debut >= @va_DebutNuit_in or
                        heure_debut < @va_FinNuit_in) or
                        (heure_fin >= @va_DebutNuit_in or
                        heure_fin < @va_FinNuit_in))
                group by sit, autoroute

                select @vl_Inter = @vl_Inter + 1
        end
       /* end*/


	/*A calcul par mois, par district */
	create table #TABLEAU (sit tinyint, mois tinyint, nb_voies_jour int null, nb_voies_nuit int null, 
			nb_bascul_jour int null, nb_bascul_nuit int null, autoroute tinyint null)
	insert #TABLEAU (mois,sit,nb_voies_jour, nb_voies_nuit, nb_bascul_jour, nb_bascul_nuit, autoroute)
	select distinct
		mois,
		sit,
		sum(nb_voies_jour),
		sum(nb_voies_nuit),
		sum(nb_bascul_jour),
		sum(nb_bascul_nuit),
		1
	from #LISTE_TRV
	where sit != 13
	group by mois, sit

	insert #TABLEAU (mois,sit,nb_voies_jour, nb_voies_nuit, nb_bascul_jour, nb_bascul_nuit, autoroute)
	select distinct
		mois,
		sit,
		sum(nb_voies_jour),
		sum(nb_voies_nuit),
		sum(nb_bascul_jour),
		sum(nb_bascul_nuit),
		autoroute
	from #LISTE_TRV
	where sit = 13
	group by mois, sit, autoroute

	/*A ajout des compteurs a zero pour les districts n'ayant pas de chantier sur un mois */
	/*! par defaut 1 pour l'autoroute qui est utile uniquement pour le district du cannet */
	select @vl_Inter = datepart(month, @va_Debut_in),
			@vl_MoisFin = datepart(month, @va_Fin_in)
	/*select @vl_Inter = @vl_Mois*/
        if @vl_MoisFin = 1
		select @vl_MoisFin = 12
	else select @vl_MoisFin = @vl_MoisFin - 1

	while @vl_Inter <= @vl_MoisFin
	begin
		insert #TABLEAU (mois,sit,nb_voies_jour, nb_voies_nuit, nb_bascul_jour, nb_bascul_nuit, autoroute)
		select
			@vl_Inter,
			numero,
			0,
			0,
			0,
			0,
			1
		from CFG..RES_DIS
		where numero != 13 and numero != 43 and type != 0

		insert #TABLEAU (mois,sit,nb_voies_jour, nb_voies_nuit, nb_bascul_jour, nb_bascul_nuit, autoroute)
		select distinct
			@vl_Inter,
			district,
			0,
			0,
			0,
			0,
			autoroute
		from CFG..RES_POR
		where district = 13 and autoroute not in (select autoroute from #TABLEAU where mois = @vl_Inter and sit = 13)

		select @vl_Inter = @vl_Inter + 1
	end

	/*A retourner la liste des infos */
        select distinct
		mois,
		CFG..RES_DIS.nom,
		sum(nb_voies_jour),
		sum(nb_voies_nuit),
		sum(nb_bascul_jour),
		sum(nb_bascul_nuit),
		CFG..RES_AUT.nom
	from #TABLEAU, CFG..RES_DIS,  CFG..RES_AUT
	where CFG..RES_DIS.numero = sit and CFG..RES_AUT.numero = autoroute
	group by mois, sit, autoroute, CFG..RES_DIS.numero, CFG..RES_AUT.numero
	order by mois, sit, autoroute
	return XDC_OK
go
