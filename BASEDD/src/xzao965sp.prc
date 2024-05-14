/*
-------------------------------------------------------------------------------
* ESCOTA * PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* FICHIER xzao965sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Gestion des graphes de Temps de parcours, utilisant les TDP FCD.
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	18/03/24 : Creation pour graphes TDP, similaire a XZAO964 pour une ZDP
*
-----------------------------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------------------------------
* Service rendu
*
* Retourne la liste des Temps de parcours calcules
* dans une zone de temps de parcours dans une periode donnee
* avec leur horodate et leur validite.
*
* -----------------------------------------------------------------------------
* Sequence d'appel
*
* SP	XZAO965_Liste_TDP_Sur_ZDP
*
* Arguments en entree
*
* XDY_Mot		@va_Numero_Zdp_in	Numero de la zone TDP
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
* Calcule et retourne le Temps de parcours et sa validite sur la zone specifiee
* pour chaque tranche horaire de 6 minutes dans la periode.
*
-----------------------------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO965' and type = 'P')
	drop procedure XZAO965
go


#ifdef HIST
	#define BDD_XPL HIS
#else
	#define BDD_XPL EXP
#endif


create procedure XZAO965
	@va_Numero_Zdp_in	smallint,
	@va_HorodateDebut_in	datetime	= null,
	@va_HorodateFin_in	datetime	= null
as
	declare	@vl_Debut		datetime,
		@vl_Fin			datetime,
		@vl_Debut_Min		datetime,
		@vl_Maintenant		datetime,
		@vl_Autoroute		T_AUTOROUTE,
		@vl_Sens		T_SENS,
		@vl_PR_Debut		T_PR,
		@vl_PR_Fin		T_PR,
		@vl_tdp			int,
		@vl_validite		tinyint,
		@vl_validite_faible	bit,
		@vl_Status		tinyint

	/*A Transformation des parametres d'entree nulls */
	if @va_HorodateDebut_in = XDC_DATE_NULLE	select @va_HorodateDebut_in = null
	if @va_HorodateFin_in = XDC_DATE_NULLE		select @va_HorodateFin_in = null


	/* Verifier l'existence de la zone TDP */
	select	@vl_Autoroute = G.autoroute,
		@vl_Sens = G.sens,
		@vl_PR_Debut = G.PR,
		@vl_PR_Fin = Z.PR_fin
	   from	CFG..EQT_GEN G, CFG..EQT_ZDP Z
	  where	G.type = XDC_EQT_ZDP
	    and	G.numero = @va_Numero_Zdp_in
	    and	G.numero = Z.numero
	if @@rowcount = 0
		return XDC_ARG_INV


	/* Verifier la validite des dates */
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
		print "XZAO965 Horodate debut : %1!", @vl_date_debut_txt
#endif
	end


	create table #TDP_VAL_ZDP
	(
		horodate		datetime	not null,
		tdp			int		not null,
		validite		tinyint		not null,
		validite_faible		tinyint		not null
	)


	/* Calculer le Temps de parcours sur la zone pour chaque tranche de 6 minutes */
	while @vl_Debut < @vl_Fin
	begin

		exec @vl_Status = PRC..XZAO963	@vl_Autoroute, @vl_PR_Debut, @vl_Sens,
						@vl_Autoroute, @vl_PR_Fin, @vl_Sens,
						0, 0,
						@vl_tdp output, @vl_validite output, @vl_validite_faible output,
						@vl_Debut

		insert	#TDP_VAL_ZDP (horodate, tdp, validite, validite_faible)
		values	(@vl_Debut, @vl_tdp, @vl_validite, @vl_validite_faible)

		select	@vl_Debut = dateadd (minute, 6, @vl_Debut)
	end


	/* Retourner les resultats */
	select	horodate,
		tdp,
		validite,
		validite_faible
	from	#TDP_VAL_ZDP
	order by horodate

	return XDC_OK
go
