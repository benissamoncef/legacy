/*E*/
/*  Fichier : $Id: vuem05sp.prc,v 1.9 2010/09/29 17:35:10 gesconf Exp $        Release : $Revision: 1.9 $        Date : $Date: 2010/09/29 17:35:10 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER vuem05sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Analyse 
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.  21/07/95        : Creation      (V 1.1)
* C.T.  24/08/95        : Modif ordre de retour des donnees     (V 1.2)
* C.T.  15/07/96        : Modif interfaces et calcul de volumes (V 1.3)
* CT+JMG 12/10/97	: traiter le cas limite de concession 1.4
* C.T.	24/02/98	: Modif pour pb default suite a passage en SQL 11.0.3.2 (V1.5)
* P.N.	05/02/99	: Modif interg. dem/1724 v1.7
* P.N.  19/04/99	: Correction dem/1759 v1.8
* JPL	29/09/10	: Adaptation compilateur C (# en debut de ligne) 1.9
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Calcul le pourcentage de voies coupees par district
*
* Sequence d'appel
* SP   VUEM05_Voies_Coupees_Par_District
*
* Arguments en entree :
* XDY_Entier    @va_NumDistrict_in	: Numero de district
* XDY_Date	@va_Date_in		: Date calcul
*
* Arguments en sortie :
* XDY_Entier	@va_Pourcentage_out	: Pourcentage de voies coupees
*
* Liste retournee :
*
* Code retour
* XDC_OK
* XDC_NOK               : probleme d'acces aux donnees 
* XDC_ARG_INV           : parametres d'entree null, ou valeur non autorisee ou
*                         echangeur inexistante
* XDC_PAS_SYN           : pas de resultat. 
*
* Conditions d'utilisation
*
* Fonction
*
-------------------------------------------------------*/

use PRC
go

create procedure VUEM05
	@va_Debut_in	datetime	= null,
	@va_Fin_in	datetime	= null
as
	declare 
		@vl_Mois tinyint, @vl_MoisCourant tinyint, @vl_MoisSuivant tinyint, 
		@vl_MoisDebut tinyint, @vl_MoisFin tinyint, @vl_Duree int,
		@vl_Site T_SITE, @vl_SiteQueue T_SITE, @vl_SiteTete T_SITE, @vl_Inter int,
		@vl_Debut datetime, @vl_Fin datetime, @vl_DateSuivante datetime,
		@vl_DateCourante datetime, @vl_MoisButee tinyint, @vl_DateButee datetime, @vl_Nombre int, 
		@vl_SurfaceTete int, @vl_SurfaceQueue int, @vl_ChaineAnnee char(5),
		@vl_Evt int, @vl_Cle tinyint, @vl_Horodate datetime, @vl_PRTete T_PR,
		@vl_Autoroute T_AUTOROUTE, @vl_PRDebut T_PR, @vl_PRFin T_PR

	/*A controle des parametres d'entree */
	if @va_Debut_in is null or @va_Fin_in is null 
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


	/* creation d'une table de stockage pour le comptage des chantiers */
	create table #LISTE_TRV (sit tinyint,
				nb_voies int default 0,
				nb_bascul int default 0, 
				mois tinyint)

	select @vl_Mois = datepart(month, @va_Debut_in),
		@vl_MoisFin = datepart(month, @va_Fin_in),
		@vl_DateCourante = getdate()

	if @vl_DateCourante >=  @va_Fin_in
	begin
		select @vl_DateButee = dateadd(second,-1,@va_Fin_in) 
		select @vl_MoisButee = datepart(month, @vl_DateButee)
	end
	else	select  @vl_MoisButee = datepart(month, @vl_DateCourante), 
			@vl_DateButee = @vl_DateCourante

	/*B liste des basculements sur la periode */
	select
		HIS..FMC_GEN.numero,
		HIS..FMC_GEN.cle,
		cause,
		cle_cause,
		debut,
		fin = isnull(fin, @vl_DateButee),
		mois_debut = datepart(month,debut),
		mois_fin = isnull(datepart(month,fin), @vl_MoisButee),
		HIS..FMC_HIS.autoroute,
		HIS..FMC_HIS.sens
	into #LISTE_BASCUL
	from HIS..FMC_GEN, HIS..FMC_HIS
	where type = XZAEC_FMC_Basculement and
		cause is not null and
		datex<>XDC_VRAI and
		debut < @va_Fin_in and (fin is null or fin > @va_Debut_in) and
		HIS..FMC_HIS.numero = HIS..FMC_GEN.numero and
		HIS..FMC_HIS.cle = HIS..FMC_GEN.cle
	group by HIS..FMC_GEN.numero, HIS..FMC_GEN.cle,
		HIS..FMC_HIS.numero, HIS..FMC_HIS.cle
	having type = XZAEC_FMC_Basculement and
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
	update #LISTE_BASCUL set fin = @vl_DateButee, mois_fin = @vl_MoisButee
	where fin >= @va_Fin_in 

	/* rechercher les PR de tete et queue pour chaque basculement en sens nord */
	create table #BASCUL(numero int, cle tinyint, fin datetime null,
			mois_debut tinyint, mois_fin tinyint, 
			autoroute tinyint, PR_tete int , PR int, 
			sit_queue tinyint null, sit_tete tinyint null)
	insert #BASCUL (numero, cle, fin, mois_debut, mois_fin, autoroute, PR_tete, PR)
	select distinct
		LB.numero,
		LB.cle,
		fin,
		mois_debut,
		mois_fin,
		HIS..FMC_HIS.autoroute,
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
	insert #BASCUL (numero, cle, fin, mois_debut, mois_fin, autoroute, PR_tete, PR)
	select distinct
		LB.numero,
		LB.cle,
		fin,
		mois_debut,
		mois_fin,
		HIS..FMC_HIS.autoroute,
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
	update #BASCUL set sit_queue = sit
	from #LIMITE_DIS LD, #BASCUL
	where	LD.autoroute = #BASCUL.autoroute and
		LD.PR_debut <= #BASCUL.PR and
		LD.PR_fin > #BASCUL.PR
	update #BASCUL set sit_tete = sit
	from #LIMITE_DIS LD, #BASCUL
	where	LD.autoroute = #BASCUL.autoroute and
		LD.PR_debut <= #BASCUL.PR and
		LD.PR_fin > #BASCUL.PR

	/*A compter un basculement par site concerne */
	create table #COMPTE_BASCUL( mois_debut tinyint, mois_fin tinyint, 
					autoroute tinyint, sit tinyint)
	insert #COMPTE_BASCUL
	select 
		mois_debut,
		mois_fin,
		autoroute,
		sit = sit_queue
	from #BASCUL
	where sit_tete = sit_queue
	insert #COMPTE_BASCUL
	select 
		mois_debut,
		mois_fin,
		autoroute,
		sit = sit_queue
	from #BASCUL
	where sit_tete != sit_queue
	insert #COMPTE_BASCUL
	select 
		mois_debut,
		mois_fin,
		autoroute,
		sit = sit_tete
	from #BASCUL
	where sit_tete != sit_queue

	/* calcul le nombre de basculement durant le premier mois des basculements */
	insert #LISTE_TRV (mois, sit, nb_bascul, nb_voies)
	select distinct
		mois_debut, 
		sit,
		count(*),
		0
	from #COMPTE_BASCUL
	group by sit, mois_debut

	/* calcul le nombre de basculement durant plus d'un mois sur le mois de fin des basculments */
	insert #LISTE_TRV (mois, sit, nb_bascul, nb_voies)
	select distinct
		mois_fin, 
		sit,
		count(*),
		0
	from #COMPTE_BASCUL
	where	mois_fin != mois_debut
	group by sit, mois_fin

	/* calcul le nombre de basculement pour les mois entre mois de debut et de fin */
	select @vl_Inter = @vl_Mois
	while @vl_Inter < @vl_MoisFin
	begin
		insert #LISTE_TRV (mois, sit, nb_bascul, nb_voies)
		select 
			@vl_Inter, 
			sit,
			count(*),
			0
		from #COMPTE_BASCUL
		where 	mois_debut < @vl_Inter and
			mois_fin > @vl_Inter
		group by sit

		select @vl_Inter = @vl_Inter + 1
	end

	/*B liste des travaux sur la periode demandee */
	select
		HIS..FMC_GEN.numero,
		HIS..FMC_GEN.cle,
		debut,
		fin = isnull(fin, @vl_DateButee),
		mois_debut = datepart(month,debut),
		mois_fin = isnull(datepart(month,fin), @vl_MoisButee),
		HIS..FMC_HIS.autoroute,
		HIS..FMC_HIS.sens
	into #LISTE_VOIES
	from HIS..FMC_GEN, HIS..FMC_HIS
	where type = XZAEC_FMC_Travaux and
		datex<>XDC_VRAI and
		debut < @va_Fin_in and ( fin is null or fin > @va_Debut_in) and
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
		debut < @va_Fin_in and ( fin is null or fin > @va_Debut_in) and
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
	update #LISTE_VOIES set fin = @vl_DateButee, mois_fin = @vl_MoisButee
	where fin >= @va_Fin_in

	/* rechercher les PR de tete et queue pour chaque travaux en sens nord */
	create table #TRV(numero int, cle int, fin datetime null, mois_debut tinyint, mois_fin tinyint, 
			autoroute tinyint, PR_tete int , PR int, sit_queue tinyint null, sit_tete tinyint null)
	insert #TRV (numero, cle, fin, mois_debut, mois_fin, autoroute, PR_tete, PR)
	select distinct
		LV.numero,
		LV.cle,
		LV.fin,
		mois_debut,
		mois_fin,
		LV.autoroute,
		PR_tete = isnull(min(localisation_tete),max(PR)),
		max(PR)
	from #LISTE_VOIES LV, HIS..FMC_HIS, HIS..FMC_TRH
	where LV.sens = XDC_SENS_NORD and HIS..FMC_HIS.numero = LV.numero and
		HIS..FMC_HIS.cle = LV.cle and
		HIS..FMC_TRH.numero = LV.numero and
		HIS..FMC_TRH.cle = LV.cle
	group by LV.numero, LV.cle, HIS..FMC_HIS.numero, HIS..FMC_HIS.cle,
		LV.numero, LV.cle

	/* rechercher les PR de tete et queue pour chaque travaux en sens sud */
	insert #TRV (numero, cle, fin, mois_debut, mois_fin, autoroute, PR_tete, PR)
	select distinct
		LV.numero,
		LV.cle,
		LV.fin,
		mois_debut,
		mois_fin,
		LV.autoroute,
		PR_tete = isnull(max(localisation_tete),min(PR)),
		min(PR)
	from #LISTE_VOIES LV, HIS..FMC_HIS, HIS..FMC_TRH
	where LV.sens = XDC_SENS_SUD and HIS..FMC_HIS.numero = LV.numero and
		HIS..FMC_HIS.cle = LV.cle and
		HIS..FMC_TRH.numero = LV.numero and
		HIS..FMC_TRH.cle = LV.cle
	group by LV.numero, LV.cle, HIS..FMC_HIS.numero, HIS..FMC_HIS.cle,
		LV.numero, LV.cle

	/*A calcul les districts concernes par chaque basculement */
	update #TRV set sit_queue = sit
	from #LIMITE_DIS LD, #TRV
	where	LD.autoroute = #TRV.autoroute and
		LD.PR_debut <= #TRV.PR and
		LD.PR_fin > #TRV.PR
	update #TRV set sit_tete = sit
	from #LIMITE_DIS LD, #TRV
	where	LD.autoroute = #TRV.autoroute and
		LD.PR_debut <= #TRV.PR_tete and
		LD.PR_fin > #TRV.PR_tete

	/*A compter des travaux par site concerne */
	create table #COMPTE_VOIES(mois_debut tinyint, mois_fin tinyint, sit tinyint)
	insert #COMPTE_VOIES
	select 
		mois_debut,
		mois_fin,
		sit = sit_queue
	from #TRV
	where sit_tete = sit_queue
	insert #COMPTE_VOIES
	select 
		mois_debut,
		mois_fin,
		sit = sit_queue
	from #TRV
	where sit_tete != sit_queue
	insert #COMPTE_VOIES
	select 
		mois_debut,
		mois_fin,
		sit = sit_tete
	from #TRV
	where sit_tete != sit_queue

	/* calcul le nombre de voies uniques durant le premier mois des voies uniques */
	insert #LISTE_TRV (mois, sit, nb_voies, nb_bascul)
	select 
		mois_debut, 
		sit,
		count(*),
		0
	from #COMPTE_VOIES
	group by sit, mois_debut

	/* calcul le nombre de voies uniques durant plus d'un mois sur le mois de fin des voies uniques */
	insert #LISTE_TRV (mois, sit, nb_voies, nb_bascul)
	select 
		mois_fin, 
		sit,
		count(*),
		0
	from #COMPTE_VOIES
	where mois_fin != mois_debut
	group by sit, mois_fin

	/* calcul le nombre de voie unique pour les mois entre mois de debut et de fin */
	select @vl_Inter = @vl_Mois
	while @vl_Inter < @vl_MoisFin
	begin
		insert #LISTE_TRV (mois, sit, nb_voies, nb_bascul)
		select 
			@vl_Inter, 
			sit,
			count(*),
			0
		from #COMPTE_VOIES
		where 	mois_debut < @vl_Inter and
			mois_fin > @vl_Inter 
		group by sit

		select @vl_Inter = @vl_Inter + 1
	end

	/*A ********* calcul des volumes de gene pour chaque basculement en fonction de l'evolution ********* */
	create table #TRV_PR (	numero int, cle tinyint,
				horodate datetime null, autoroute tinyint null, PR int null, PR_tete int null,
				VR tinyint, VM1 tinyint, VM2 tinyint, VL tinyint,
				nb_voies_occultees tinyint default 0)

	/*A lecture des PR des basculements */
	insert #TRV_PR (numero, cle, horodate, autoroute, PR, PR_tete, VL,VM1,VM2,VR,nb_voies_occultees)
	select
		HIS..FMC_HIS.numero,
		HIS..FMC_HIS.cle,
		HIS..FMC_HIS.horodate_validation,
		HIS..FMC_HIS.autoroute,
		HIS..FMC_BAS.localisation_PR_fin,
		HIS..FMC_HIS.PR,
		HIS..FMC_HIS.VL,
		HIS..FMC_HIS.VM1,
		HIS..FMC_HIS.VM2,
		HIS..FMC_HIS.VR,
		0
	from #BASCUL, HIS..FMC_HIS, HIS..FMC_BAS
	where	HIS..FMC_HIS.numero = #BASCUL.numero and
		HIS..FMC_HIS.cle = #BASCUL.cle and
		HIS..FMC_BAS.numero = HIS..FMC_HIS.numero and
		HIS..FMC_BAS.cle = HIS..FMC_HIS.cle
	
	/*A lecture des PR des travaux */
	insert #TRV_PR (numero, cle, horodate, autoroute, PR, PR_tete, VL,VM1,VM2,VR,nb_voies_occultees)
	select
		HIS..FMC_HIS.numero,
		HIS..FMC_HIS.cle,
		HIS..FMC_HIS.horodate_validation,
		HIS..FMC_HIS.autoroute,
		HIS..FMC_HIS.PR,
		isnull(HIS..FMC_TRH.localisation_tete,HIS..FMC_HIS.PR),
		VL,
		VM1,
		VM2,
		VR,
		0
	from #TRV, HIS..FMC_HIS, HIS..FMC_TRH
	where	HIS..FMC_HIS.numero = #TRV.numero and
		HIS..FMC_HIS.cle = #TRV.cle and
		HIS..FMC_TRH.numero = HIS..FMC_HIS.numero and
		HIS..FMC_TRH.cle = HIS..FMC_HIS.cle and
		HIS..FMC_TRH.horodate = HIS..FMC_HIS.horodate_validation
	
	/*A calcul du nombre de voies occultees */
	update #TRV_PR set nb_voies_occultees = 1
	where VL = XDC_VOIE_NEUTRALISEE or VL = XDC_VOIE_BLOQUEE_NEUTRALISEE
	update #TRV_PR set nb_voies_occultees = nb_voies_occultees + 1
	where VM1 = XDC_VOIE_NEUTRALISEE or VM1 = XDC_VOIE_BLOQUEE_NEUTRALISEE
	update #TRV_PR set nb_voies_occultees = nb_voies_occultees + 1
	where VM2 = XDC_VOIE_NEUTRALISEE or VM2 = XDC_VOIE_BLOQUEE_NEUTRALISEE
	update #TRV_PR set nb_voies_occultees = nb_voies_occultees + 1
	where VR = XDC_VOIE_NEUTRALISEE or VR = XDC_VOIE_BLOQUEE_NEUTRALISEE

	/*A ajout du dernier etat des chantiers */
	create table #DERNIER_ETAT (	numero int, cle tinyint,
				horodate datetime null, autoroute tinyint null, PR int null, PR_tete int null,
				VR tinyint, VM1 tinyint, VM2 tinyint, VL tinyint,
				nb_voies_occultees tinyint default 0)
	insert #DERNIER_ETAT(numero, cle, horodate, autoroute, PR, PR_tete, VL,VM1,VM2,VR, nb_voies_occultees)
	select
		TP.numero,
		TP.cle,
		fin,
		TP.autoroute,
		TP.PR,
		TP.PR_tete,
		TP.VL,
		TP.VM1,
		TP.VM2,
		TP.VR,
		nb_voies_occultees
	from #TRV_PR TP, #LISTE_VOIES
	where #LISTE_VOIES.numero = TP.numero and #LISTE_VOIES.cle = TP.cle and
		horodate <= fin	
	group by TP.numero, TP.cle, #LISTE_VOIES.numero, #LISTE_VOIES.cle
	having #LISTE_VOIES.numero = TP.numero and #LISTE_VOIES.cle = TP.cle and
		horodate <= fin and horodate = max(horodate)

	insert #DERNIER_ETAT(numero, cle, horodate, autoroute, PR, PR_tete, VL,VM1,VM2,VR, nb_voies_occultees)
	select
		TP.numero,
		TP.cle,
		fin,
		TP.autoroute,
		TP.PR,
		TP.PR_tete,
		TP.VL,
		TP.VM1,
		TP.VM2,
		TP.VR,
		nb_voies_occultees
	from #TRV_PR TP, #LISTE_BASCUL
	where #LISTE_BASCUL.numero = TP.numero and #LISTE_BASCUL.cle = TP.cle and
		horodate <= fin	
	group by TP.numero, TP.cle, #LISTE_BASCUL.numero, #LISTE_BASCUL.cle
	having #LISTE_BASCUL.numero = TP.numero and #LISTE_BASCUL.cle = TP.cle and
		horodate <= fin and horodate = max(horodate)

	insert #TRV_PR(numero, cle, horodate, autoroute, PR, PR_tete, VL,VM1,VM2,VR, nb_voies_occultees)
	select numero, cle, horodate, autoroute, PR, PR_tete, VL,VM1,VM2,VR, nb_voies_occultees 
	from #DERNIER_ETAT

	drop table #DERNIER_ETAT

	/*! creation d'une table de travail de stockage du graphe de travaux */
	create table #GRA_TRV ( numero int, cle tinyint, horodate datetime , horodate_fin datetime null, 
				autoroute tinyint, PR int , PR_tete int,  sit_queue tinyint null, sit_tete tinyint null,
				PR_debut_sit_queue int null, PR_fin_sit_queue int null, 
				PR_debut_sit_tete int null, PR_fin_sit_tete int null, 
				longueur_site_tete int null, longueur_site_queue int null, 
				nb_voies_occultees tinyint)

	insert #GRA_TRV (numero, cle, horodate, horodate_fin, autoroute,PR, PR_tete, nb_voies_occultees)
	select distinct
		T1.numero,
		T1.cle,
		T1.horodate,
		T2.horodate,
		T1.autoroute,
		T1.PR,
		T1.PR_tete,
		T1.nb_voies_occultees
	from #TRV_PR T1, #TRV_PR T2
	where	T2.numero = T1.numero and
		T2.cle = T1.cle and
		T2.horodate > T1.horodate
	group by T2.numero, T2.cle, T1.numero, T1.cle, T1.horodate
	having	T2.numero = T1.numero and
		T2.cle = T1.cle and
		T2.horodate > T1.horodate and
		T2.horodate = min(T2.horodate)

	/* supprimer toutes les validations ayant aucune voie occultee */
	delete #GRA_TRV
	where nb_voies_occultees = 0
	delete #GRA_TRV
	where PR = PR_tete

	/*A resteindre les validations à la periode demandee */
	update #GRA_TRV set horodate_fin = @va_Fin_in, horodate = @va_Debut_in
	where horodate < @va_Debut_in and horodate_fin >= @va_Fin_in
	delete #GRA_TRV
	where horodate >= @va_Fin_in
	delete #GRA_TRV
	where horodate_fin <= @va_Debut_in

	update #GRA_TRV set horodate_fin = @va_Fin_in
	where horodate_fin > @va_Fin_in
	update #GRA_TRV set horodate = @va_Debut_in
	where horodate < @va_Debut_in

	/*A calcul des longueurs par district */
	update #GRA_TRV set sit_queue = sit, PR_debut_sit_queue = PR_debut, PR_fin_sit_queue = PR_fin
	from #LIMITE_DIS LD, #GRA_TRV
	where LD.autoroute = #GRA_TRV.autoroute and PR_debut <= PR and PR_fin > PR

	/*limite de concession*/
	update #GRA_TRV set sit_queue = sit, PR_debut_sit_queue = PR_debut, PR_fin_sit_queue = PR_fin
	from #LIMITE_DIS LD, #GRA_TRV
	where sit_queue = null and
		LD.autoroute = #GRA_TRV.autoroute and
		PR_fin = PR and
		PR_debut <= PR

	update #GRA_TRV set sit_tete = sit, PR_debut_sit_tete = PR_debut, PR_fin_sit_tete = PR_fin
	from #LIMITE_DIS LD, #GRA_TRV
	where LD.autoroute = #GRA_TRV.autoroute and PR_debut <= PR_tete and PR_fin > PR_tete

	/*limite de concession*/
	update #GRA_TRV set sit_tete = sit, PR_debut_sit_tete = PR_debut, PR_fin_sit_tete = PR_fin
	from #LIMITE_DIS LD, #GRA_TRV
	where sit_tete= null and
		LD.autoroute = #GRA_TRV.autoroute and
		PR_fin = PR_tete and
		PR_fin = PR_tete

	/*! PR inexistant prendre le PR le plus proche */
	select * into #SIT_TETE_NULL
	from #GRA_TRV 
	where sit_tete is null

	select @vl_Nombre = count(*) from #SIT_TETE_NULL

	while @vl_Nombre > 0
	begin
		set rowcount 1
		select 
			@vl_Evt = numero,
			@vl_Cle = cle,
			@vl_Horodate = horodate,
			@vl_Autoroute = autoroute,
			@vl_PRTete = PR_tete
		from #SIT_TETE_NULL
		delete #SIT_TETE_NULL
		set rowcount 0

		/* PR inferieur a la limite inferieure du district */
		if not exists ( select * from #LIMITE_DIS where PR_debut < @vl_PRTete )
			select
				@vl_Site = sit,
				@vl_PRDebut = PR_debut,
				@vl_PRFin = PR_fin
			from #LIMITE_DIS
			where	autoroute = @vl_Autoroute
			group by autoroute
			having autoroute = @vl_Autoroute and
				PR_debut = max(PR_debut)

		/* PR en limite de concession */
		else if exists ( select * from #LIMITE_DIS where autoroute = @vl_Autoroute and PR_fin = @vl_PRTete )
			select
				@vl_Site = sit,
				@vl_PRDebut = PR_debut,
				@vl_PRFin = PR_fin
			from #LIMITE_DIS
			where	autoroute = @vl_Autoroute and PR_fin = @vl_PRTete
			
		/* PR superieure a la limite inferieure du district */
		else
			select
				@vl_Site = sit,
				@vl_PRDebut = PR_debut,
				@vl_PRFin = PR_fin
			from #LIMITE_DIS
			where autoroute = @vl_Autoroute
			group by autoroute
			having autoroute = @vl_Autoroute and
				PR_fin = min(PR_fin)

		update #GRA_TRV set 
			sit_tete = @vl_Site, PR_debut_sit_tete = @vl_PRDebut , PR_fin_sit_tete =  @vl_PRFin
		where sit_tete is null and numero = @vl_Evt and cle = @vl_Cle and horodate = @vl_Horodate

		select @vl_Nombre = @vl_Nombre - 1
	end

	/*A calcul des longueurs par district */
	update #GRA_TRV set 
		longueur_site_queue = abs(PR - PR_tete), 
		longueur_site_tete = 0
	where sit_queue=sit_tete

	update #GRA_TRV set 
		longueur_site_queue = PR - PR_debut_sit_queue,
		longueur_site_tete = PR_fin_sit_tete - PR_tete
	where sit_queue!=sit_tete and PR > PR_tete

	update #GRA_TRV set 
		longueur_site_queue = PR_fin_sit_queue - PR,
		longueur_site_tete = PR_tete - PR_debut_sit_tete
	where sit_queue!=sit_tete and PR < PR_tete

	/*A calcul des volumes */
	/*! calcul des volumes des chantiers localises sur un seul mois */
	create table #VOLUME (sit tinyint, mois tinyint, volume int, duree int)
	insert #VOLUME (sit, mois, volume, duree)
	select distinct
		sit = sit_queue,
		mois = datepart(month,horodate),
		volume = sum(longueur_site_queue * datediff(minute, horodate, horodate_fin) * nb_voies_occultees),
		duree = sum(datediff(minute, horodate, horodate_fin))
	from #GRA_TRV
	where datepart(month,horodate) = datepart(month,horodate_fin)
	group by sit_queue, datepart(month,horodate)

	insert #VOLUME (sit, mois, volume, duree)
	select distinct
		sit = sit_tete,
		mois = datepart(month,horodate),
		volume = sum(longueur_site_tete * datediff(minute, horodate, horodate_fin) * nb_voies_occultees),
		duree = sum(datediff(minute, horodate, horodate_fin))
	from #GRA_TRV
	where longueur_site_tete != 0 and datepart(month,horodate) = datepart(month,horodate_fin)
	group by sit_tete, datepart(month,horodate)

	/*! calcul des volumes des chantiers durant plusieurs mois */
	select @vl_ChaineAnnee = "/" + convert(char(4),datepart(year,@va_Debut_in))
	select 
		horodate,
		horodate_fin,
		surface_tete = longueur_site_tete * nb_voies_occultees,
		surface_queue = longueur_site_queue * nb_voies_occultees,
		sit_queue,
		sit_tete
	into #TRV_SUR_MOIS
	from #GRA_TRV
	where datepart(month,horodate) < datepart(month,horodate_fin)

	select @vl_Nombre = count(*) from #TRV_SUR_MOIS

	while @vl_Nombre > 0
	begin
		set rowcount 1
		select
			@vl_Debut = horodate,
			@vl_Fin = horodate_fin,
			@vl_MoisDebut = datepart(month,horodate),
			@vl_MoisFin = datepart(month,horodate_fin),
			@vl_SurfaceTete = surface_tete,
			@vl_SurfaceQueue = surface_queue,
			@vl_SiteQueue = sit_queue,
			@vl_SiteTete = sit_tete
		from #TRV_SUR_MOIS
		delete #TRV_SUR_MOIS
		set rowcount 0

		select @vl_DateSuivante = "01/"+convert(varchar(2),(@vl_MoisDebut + 1)) + @vl_ChaineAnnee
		select @vl_Duree = datediff(minute, @vl_Debut, @vl_DateSuivante)

		/* premier mois */
		insert #VOLUME (sit, volume, duree, mois)
		values( @vl_SiteTete, @vl_Duree * @vl_SurfaceTete, @vl_Duree, @vl_MoisDebut)
		insert #VOLUME (sit, volume, duree, mois)
		values( @vl_SiteQueue, @vl_Duree * @vl_SurfaceQueue, @vl_Duree, @vl_MoisDebut)

		if @vl_MoisDebut + 1 < @vl_MoisFin
		begin
			/* chantier sur plus de 2 mois */
			select @vl_DateCourante = @vl_DateSuivante, @vl_MoisCourant = @vl_MoisDebut + 1
			select @vl_DateSuivante = dateadd(month, 1,@vl_DateCourante), @vl_MoisSuivant = @vl_MoisDebut + 2

			while @vl_MoisSuivant < @vl_MoisFin
			begin
				select @vl_Duree = datediff(minute, @vl_DateCourante, @vl_DateSuivante)

				if @vl_SurfaceTete != 0
					insert #VOLUME (sit, volume, duree, mois)
					values( @vl_SiteTete, @vl_Duree * @vl_SurfaceTete, @vl_Duree, @vl_MoisCourant)

				insert #VOLUME (sit, volume, duree, mois)
				values( @vl_SiteQueue, @vl_Duree * @vl_SurfaceQueue, @vl_Duree, @vl_MoisCourant)

				select @vl_DateCourante = @vl_DateSuivante, 
					@vl_MoisCourant = @vl_MoisSuivant
				select @vl_DateSuivante = dateadd(month, 1,@vl_DateCourante), 
					@vl_MoisSuivant = @vl_MoisCourant + 1
			end
		end
		
		/* dernier mois */
		select @vl_Duree = datediff(minute, @vl_DateSuivante, @vl_Fin)
		insert #VOLUME (sit, volume, duree, mois)
		values( @vl_SiteTete, @vl_Duree * @vl_SurfaceTete, @vl_Duree, @vl_MoisDebut)
		insert #VOLUME (sit, volume, duree, mois)
		values( @vl_SiteQueue, @vl_Duree * @vl_SurfaceQueue, @vl_Duree, @vl_MoisDebut)

		select @vl_Nombre = @vl_Nombre - 1
	end


	/*A ******* fin calcul des volumes ************ */

	/*A calcul par mois, par district */
	create table #TABLEAU (sit tinyint, mois tinyint, duree int default 0, volume int default 0, nombre int default 0)
	insert #TABLEAU (mois,sit,nombre,duree,volume)
	select distinct
		mois,
		sit,
		sum(nb_voies)+sum(nb_bascul),
		0,
		0
	from #LISTE_TRV
	group by mois, sit

	insert #TABLEAU (mois,sit,duree,volume,nombre)
	select distinct
		mois,
		sit,
		sum(duree),
		sum(volume),
		0
	from #VOLUME
	group by mois, sit

	/*A ajout des compteurs a zero pour les districts n'ayant pas de chantier sur un mois */
	/*! par defaut 1 pour l'autoroute qui est utile uniquement pour le district du cannet */
	select @vl_Inter = datepart(month, @va_Debut_in),
		@vl_MoisFin = datepart(month, @va_Fin_in)
        if @vl_MoisFin = 1
		select @vl_MoisFin = 12
	else select @vl_MoisFin = @vl_MoisFin - 1

	while @vl_Inter <= @vl_MoisFin
	begin
		insert #TABLEAU (mois,sit,duree,volume,nombre)
		select
			@vl_Inter,
			numero,
			0,
			0,
			0
		from CFG..RES_DIS
		where numero != 43 and type != 0

		select @vl_Inter = @vl_Inter + 1
	end

	/*A retourner la liste des infos */
	select distinct
		mois,
		CFG..RES_DIS.nom,
		sum(duree),
		sum(volume),
		sum(nombre)
	from #TABLEAU, CFG..RES_DIS
	where CFG..RES_DIS.numero = sit
	group by mois, sit, CFG..RES_DIS.numero
	order by mois, sit

	return XDC_OK
go
