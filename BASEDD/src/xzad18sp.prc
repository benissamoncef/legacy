/*E*/
/* Fichier : $Id: xzad18sp.prc,v 1.1 2018/04/12 11:49:44 devgfi Exp $        $Revision: 1.1 $        $Date: 2018/04/12 11:49:44 $
-------------------------------------------------------------------------------
* ESCOTA * PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* FICHIER xzad18sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Liste des mesures 6mn des Temps de parcours
*  relevees par une station donnee dans une periode donnee.
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	02/02/18 : Creation  1.1
-----------------------------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------------------------------
* Service rendu
*
* Retourne la liste des mesures de Temps de parcours
* relevees par une station donnee entre deux dates donnees
* avec leur horodate de mesure et leur validite.
*
* -----------------------------------------------------------------------------
* Sequence d'appel
* SP	XZAD18_Liste_TDP_6mn
*
* Arguments en entree
*
* XDY_Mot		va_Numero_Zdp_in
* XDY_Horodate		va_HorodateDebut_in
* XDY_Horodate		va_HorodateFin_in
*
* Arguments en sortie
*  Aucun
*
* Code retour
* XDC_OK
* XDC_NOK
* <0 erreur sybase
*
* Conditions d'utilisation
*
-----------------------------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAD18' and type = 'P')
	drop procedure XZAD18
go


create procedure XZAD18
	@va_Numero_Zdp_in	smallint	= null,
	@va_HorodateDebut_in	datetime	= null,
	@va_HorodateFin_in	datetime	= null
as
	declare	@vl_Debut	datetime,
		@vl_Fin		datetime,
		@vl_temps	smallint,
		@vl_validite	tinyint

	/*A Transformation des parametres d'entree nulls */
	if @va_Numero_Zdp_in = 0			select @va_Numero_Zdp_in = null
	if @va_HorodateDebut_in = XDC_DATE_NULLE	select @va_HorodateDebut_in = null
	if @va_HorodateFin_in = XDC_DATE_NULLE		select @va_HorodateFin_in = null


	/*A Controle des parametres d'entree obligatoire */
	if @va_HorodateDebut_in = null  or  @va_HorodateFin_in = null
		return XDC_ARG_INV

	if not exists (select 1 from CFG..EQT_GEN where type = XDC_EQT_ZDP  and  numero = @va_Numero_Zdp_in)
		return XDC_ARG_INV


	create table #TDP_ZONE
	(
		horodate	datetime	not null,
		temps		smallint	not null,
		validite	tinyint		not null
	)


	/* Ordonner les dates de début et fin */
	if @va_HorodateDebut_in <= @va_HorodateFin_in
		select	@vl_Debut = @va_HorodateDebut_in,
			@vl_Fin = @va_HorodateFin_in
	else
		select	@vl_Debut = @va_HorodateFin_in,
			@vl_Fin = @va_HorodateDebut_in


	/*A Caler l'horodate de début sur celle de la première mesure qui suit */
	begin
		declare	@vl_date_debut_txt	char(10),
			@vl_minute_mesure	int

		select	@vl_Debut = dateadd (second, -1, dateadd (minute, 6, @vl_Debut))
		select	@vl_minute_mesure = datepart (hour, @vl_Debut) * 60  +  ((datepart (minute, @vl_Debut) / 6) * 6)
		select	@vl_date_debut_txt = convert (char(10), @vl_Debut, 103)
		select	@vl_Debut = dateadd (minute, @vl_minute_mesure, convert (datetime,  @vl_date_debut_txt, 103))
	end


	/*A Extraire les mesures de la station indiquee et dans la periode */
	while @vl_Debut <= @vl_Fin
	begin
		select	@vl_temps = temps,
			@vl_validite = validite
		from	HIS..ZDP_6MN
		where	numero = @va_Numero_Zdp_in
		  and	horodate = @vl_Debut

		if @@rowcount > 0
			insert	#TDP_ZONE (horodate, temps, validite)
			values	(@vl_Debut, @vl_temps, @vl_validite)
		else
			/*B En l'absence de mesure, inserer une valeur 0, de validite inconnue */
			insert	#TDP_ZONE (horodate, temps, validite)
			values	(@vl_Debut, 0, XDC_RADT_INCONNU)

		select	@vl_Debut = dateadd (minute, 6, @vl_Debut)
	end


	/*A Retourner les resultats */
	select	convert(char(10), horodate, 103) + ' ' + convert(char(8), horodate, 8),
		temps,
		validite
	from	#TDP_ZONE
	order by horodate

	return XDC_OK
go
