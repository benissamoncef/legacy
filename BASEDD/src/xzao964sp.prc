/*
-------------------------------------------------------------------------------
* ESCOTA * PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* FICHIER xzao964sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Gestion des graphes de Temps de parcours, utilisant les TDP FCD.
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	14/03/24 : Creation  1.1
-----------------------------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------------------------------
* Service rendu
*
* Retourne la liste des Temps de parcours calcules
* entre deux points quelconques du reseau dans une periode donnee
* avec leur horodate et leur validite.
*
* -----------------------------------------------------------------------------
* Sequence d'appel
* SP	XZAO964_Liste_TDP_Dans_Periode
*
* Arguments en entree
*
* XDY_Autoroute		@va_autoroute_in_o	Numero d'autoroute d'origine
* XDY_PR		@va_PR_in_o		PR d'origine
* XDY_Sens		@va_sens_in_o		Sens d'origine
* XDY_Autoroute		@va_autoroute_in_d	Numero d'autoroute de destination
* XDY_PR		@va_PR_in_d		PR de destination
* XDY_Sens		@va_sens_in_d		Sens de destination
* XDY_Horodate		@va_HorodateDebut_in	Horodate de debut de periode
* XDY_Horodate		@va_HorodateFin_in	Horodate de fin de periode
* 
* Arguments en sortie
*  Aucun
* 
* Code retour								
* XDC_OK
* XDC_NOK
* XDC_ARG_INV		parametre invalide
* <0 erreur sybase 
* 
* Conditions d'utilisation
*
* L'etendue de la periode est limitee (a 24 heures).
*
* Fonction
*
* Calcule et retourne le Temps de parcours et sa validite sur le trajet specifie
* pour chaque tranche horaire de 6 minutes dans la periode.
*
-----------------------------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO964' and type = 'P')
	drop procedure XZAO964
go


#ifdef HIST
	#define BDD_XPL HIS
#else
	#define BDD_XPL EXP
#endif


create procedure XZAO964
	@va_autoroute_in_o	tinyint,
	@va_PR_in_o		int,
	@va_sens_in_o		tinyint, 
	@va_autoroute_in_d	tinyint,
	@va_PR_in_d		int, 
	@va_sens_in_d		tinyint,
	@va_HorodateDebut_in	datetime	= null,
	@va_HorodateFin_in	datetime	= null
as
	declare	@vl_Debut		datetime,
		@vl_Fin			datetime,
		@vl_Debut_Min		datetime,
		@vl_Maintenant		datetime,
		@vl_tdp			int,
		@vl_validite		tinyint,
		@vl_validite_faible	bit,
		@vl_Status		tinyint

	/*A Transformation des parametres d'entree nulls */
	if @va_HorodateDebut_in = XDC_DATE_NULLE	select @va_HorodateDebut_in = null
	if @va_HorodateFin_in = XDC_DATE_NULLE		select @va_HorodateFin_in = null


	/*A Controle des parametres d'entree obligatoire */
	if not exists (select 1 from CFG..RES_AUT where numero = @va_autoroute_in_o)  or
	   not exists (select 1 from CFG..RES_AUT where numero = @va_autoroute_in_d)
		return XDC_ARG_INV

	if (@va_PR_in_o < 0)  or  (@va_PR_in_d < 0)
		return XDC_ARG_INV

	if @va_sens_in_o not in (XDC_SENS_NORD, XDC_SENS_SUD)  or
	   @va_sens_in_d not in (XDC_SENS_NORD, XDC_SENS_SUD)
		return XDC_ARG_INV


	select @vl_Maintenant = getdate()
	select @vl_Debut_Min = dateadd (month, -12, @vl_Maintenant)

	if @va_HorodateDebut_in is null
		return XDC_ARG_INV
	if @va_HorodateDebut_in > @vl_Maintenant
		return XDC_OK
	else if @va_HorodateDebut_in < @vl_Debut_Min
		select @vl_Debut = @vl_Debut_Min
	else select @vl_Debut = @va_HorodateDebut_in

	if @va_HorodateFin_in is null
		select @vl_Fin = @vl_Maintenant
	else if @va_HorodateFin_in > @vl_Maintenant
		select @vl_Fin = @vl_Maintenant
	else select @vl_Fin = @va_HorodateFin_in

	if @vl_Debut > @vl_Fin
		return XDC_ARG_INV

	/* Limiter la quantite de donnees extraites a 24 heures */
	if datediff (hour, @vl_Debut, @vl_Fin) > 24
		select @vl_Fin = dateadd (hour, 24, @vl_Debut)


	/* Caler l'horodate de debut a celle de la derniere mesure */
	begin
		declare	@vl_date_debut_txt	char(30),
			@vl_minute_mesure	int

		select	@vl_minute_mesure = datepart (hour, @vl_Debut) * 60  +  ((datepart (minute, @vl_Debut) / 6) * 6)
		select	@vl_date_debut_txt = convert (char, @vl_Debut, 103)
		select	@vl_Debut = dateadd (minute, @vl_minute_mesure, convert (datetime,  @vl_date_debut_txt))

#if defined (DEV)
		select @vl_date_debut_txt = convert (char, @vl_Debut, 109)
		print "XZAO964 Horodate debut : %1!", @vl_date_debut_txt
#endif
	end


	create table #TDP_VAL_PERIODE
	(
		horodate		datetime	not null,
		tdp			int		not null,
		validite		tinyint		not null,
		validite_faible		tinyint		not null
	)


	/* Calculer le Temps de parcours sur le trajet pour chaque tranche de 6 minutes */
	while @vl_Debut < @vl_Fin
	begin

		exec @vl_Status = PRC..XZAO963	@va_autoroute_in_o, @va_PR_in_o, @va_sens_in_o,
						@va_autoroute_in_d, @va_PR_in_d, @va_sens_in_d,
						0, 0,
						@vl_tdp output, @vl_validite output, @vl_validite_faible output,
						@vl_Debut

		insert	#TDP_VAL_PERIODE (horodate, tdp, validite, validite_faible)
		values	(@vl_Debut, @vl_tdp, @vl_validite, @vl_validite_faible)

		select	@vl_Debut = dateadd (minute, 6, @vl_Debut)
	end


	/* Retourner les resultats */
	select	horodate,
		tdp,
		validite,
		validite_faible
	from	#TDP_VAL_PERIODE
	order by horodate

	return XDC_OK
go
