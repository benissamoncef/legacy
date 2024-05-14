/*E*/
/*  Fichier : $Id: xzag50sp.prc,v 1.3 2002/03/07 10:11:56 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 2002/03/07 10:11:56 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag50sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.	09/10/96	: Creation	(dem/1227) (V 1.1)
* P.N.	10/10/96	: Ajout des actions sur MES_MET	(dem/1232) (V 1.2)
* P.C.	06/03/02        : Correction pour HIS V1.3
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Transfere les donnees Pesage du mois dans la base de sauvegarde
* /restauration
* 
* Sequence d'appel
* SP	XZAG04_Transfert_Mesures_Pesage
* 
* Arguments en entree
* XDY_Mot	va_Mois_in
* XDY_Mot	va_Annee_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK	: 
* XDC_ARG_INV	: parametre d'entree null ou date demandee trop proche
* 
* Conditions d'utilisation
* - Il faut que la date courante soit au moins superieure a
* au mois specifie plus 7 jours sinon erreur de parametre
* - Le mois vaut de 1 a 12
* - L'annee vaut au moins 1900
* - La periode de sauvegarde est du debut de mois - 7 jours a la
* la fin du mois + 7 jours
* 
* Fonction
* Copier de la base historique dans la base sauvegarde/restauration
-------------------------------------------------------*/

use PRC
go

create procedure XZAG;50
	@va_Mois_in	smallint = null,
	@va_Annee_in	smallint = null
as
#ifdef HIST
	declare @vl_Status int,  @vl_Station smallint, @vl_Sens T_SENS, @vl_Voie T_VOIE,
		@vl_Horodate1900 datetime, @vl_DateCourante datetime,
		@vl_HorodateDebut datetime, @vl_HorodateFin datetime, 
		@vl_DeltaJours int, @vl_Chaussee T_VOIE

	/*A controle du parametre en entree */
	if @va_Mois_in = null or @va_Annee_in = null or 
	   @va_Mois_in > 12 or @va_Mois_in < 1 or @va_Annee_in < 1900
		return XDC_ARG_INV

	/* date de debut et de fin de sauvegarde */
	select	@vl_DateCourante = getdate(),
		@vl_Horodate1900 = dateadd(year, @va_Annee_in - 1900, "01/01/1900 00:00:00")
	select	@vl_HorodateDebut = dateadd(month, @va_Mois_in - 1, @vl_Horodate1900),
		@vl_HorodateFin = dateadd(month, @va_Mois_in, @vl_Horodate1900)
	select	@vl_DeltaJours = datediff(day, @vl_HorodateFin, @vl_DateCourante)
	select	@vl_HorodateDebut = dateadd(day, -7, @vl_HorodateDebut),
		@vl_HorodateFin = dateadd(day, 7, @vl_HorodateFin)

	/*A Controle que la date specifiee est anterieure a la date courante */
	/*A moins 7 jours                                                    */
	if @vl_DeltaJours < 7
		return XDC_ARG_INV

	/*A rechercher les points de mesures */
	select
		station,
		sens,
		voie
	into #STATIONS
	from CFG..EQT_PNT_MES

	/*A ajout de la chaussee dans les points de mesures */
	select 	@vl_Chaussee = 0
	insert #STATIONS
	select distinct
		station,
		sens,
		@vl_Chaussee
	from #STATIONS

	/*A Sauvegarder toutes les mesures de trafic du mois specifie */
	/*! (utiliser l'index de la table des mesures) */
	declare Pointeur_station cursor
	for select
		station,
		sens,
		voie
	from #STATIONS

	open Pointeur_station
	fetch Pointeur_station into @vl_Station, @vl_Sens, @vl_Voie
	while @@sqlstatus = 0
	begin
		insert SVG..MES_KCS
		select * from HIS..MES_KCS
		where station = @vl_Station and
			sens = @vl_Sens and voie = @vl_Voie and
			horodate < @vl_HorodateFin and horodate >= @vl_HorodateDebut
		insert SVG..MES_QPC
		select * from HIS..MES_QPC
		where station = @vl_Station and
			sens = @vl_Sens and voie = @vl_Voie and
			horodate < @vl_HorodateFin and horodate >= @vl_HorodateDebut
		insert SVG..MES_NEC
		select * from HIS..MES_NEC
		where station = @vl_Station and
			sens = @vl_Sens and voie = @vl_Voie and
			horodate < @vl_HorodateFin and horodate >= @vl_HorodateDebut

		fetch Pointeur_station into @vl_Station, @vl_Sens, @vl_Voie
	end
	close Pointeur_station

	insert SVG..MES_MET
	select * from HIS..MES_MET
	where jour < @vl_HorodateFin and jour >= @vl_HorodateDebut

	return XDC_OK
#else
	return XDC_NOK
#endif
go
