/*E*/
/*  Fichier : $Id: xzag28sp.prc,v 1.6 2013/03/29 10:59:50 gesconf Exp $      Release : $Revision: 1.6 $        Date : $Date: 2013/03/29 10:59:50 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag28sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	10/08/95	: Creation	(V 1.1)
* C.T.	06/12/95	: Ajout purge des voies 0 (V 1.2)
* C.T.	09/01/96	: Supprimer le site dans la clause de suppression (V 1.3)
* P.C.	06/03/02        : correction pour HIS V1.4          
* JPL.	12/06/08	: Purge mesures de la veille si aucune de 7h a 22h (DEM 680) V1.5
* PNI	17/10/11	: exclure 7h30 dans la plage de vérification de données inexistantes DEM 1004 v1.6
* JPC	24/11/23	: Forcer le format de date en JJ/MM/AAAA (SAE-529)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* . Purge des mesures trafic de la base historique datant
*   d'au moins la date precisee
* . Purge des mesures trafic de la veille de la base historique
*   pour toute station de sens 1 ou 2 sans mesure de 07:00 a 22:00.
* 
* Sequence d'appel
* SP	XZAG28_Purge_Mesures_Trafic_Base_Historique
* 
* Arguments en entree
* XDY_Datetime	va_Horodate_in	: date de fin de purge
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametre d'entree null
* 
* Conditions d'utilisation
* 
* Fonctions
*  . Suppression des lignes anterieures a la date indiquee (5 ans)
*  . Suppression des lignes de la veille si aucune mesure de 7h a 22h
-------------------------------------------------------*/

use PRC
go

create procedure XZAG;28
	@va_Horodate_in		datetime = null
as
	/* Format de date en Jour/Mois/Annee */
	set dateformat 'dmy'
#ifdef HIST
	declare @vl_Site T_SITE, @vl_Station smallint,
		@vl_Sens T_SENS, @vl_Voie T_VOIE,
		@vl_Chaussee T_VOIE

	/*A controle du parametre en entree */
	if @va_Horodate_in = null
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

	/*A Effacer toutes les mesures de trafic */
	/*A anterieures a la date donnee         */
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
		delete HIS..MES_TRA 
		where station = @vl_Station and
			sens = @vl_Sens and voie = @vl_Voie and
			horodate <= @va_Horodate_in

		fetch Pointeur_station into @vl_Station, @vl_Sens, @vl_Voie
	end
	close Pointeur_station


	/*A Pour chaque station en section courante (sens 1 ou 2), */
	/*A effacer toutes les mesures de trafic de la veille      */
	/*A s'il n'y a pas de mesure de 7h a 22h                   */
	declare @vl_maintenant datetime, @vl_aujourd_hui datetime,
		@vl_hier datetime, @vl_hier_a_07h30 datetime, @vl_hier_a_22h00 datetime

	select @vl_maintenant = getdate()
	-- obtenir la date a 00:00 par conversion de la partie 'date' sans la partie 'heure'
	select @vl_aujourd_hui = convert(datetime, convert(char, @vl_maintenant, 103))
	select @vl_hier = dateadd(day, -1, @vl_aujourd_hui)
	select @vl_hier_a_07h30 = dateadd(minute, 7 * 60 + 30, @vl_hier)
	select @vl_hier_a_22h00 = dateadd(hour, 22, @vl_hier)

	-- Parcourir a nouveau la liste des points de mesure
	open Pointeur_station
	fetch Pointeur_station into @vl_Station, @vl_Sens, @vl_Voie
	while @@sqlstatus = 0
	begin
		-- et pour les stations en section courante
		if @vl_Sens = 1  or  @vl_Sens = 2

			if not exists (select * from HIS..MES_TRA
					where station = @vl_Station and
						sens = @vl_Sens and
						voie = @vl_Voie and
						horodate > @vl_hier_a_07h30 and horodate < @vl_hier_a_22h00)
			begin
				delete HIS..MES_TRA 
				where station = @vl_Station and
					sens = @vl_Sens and
					voie = @vl_Voie and
					horodate >= @vl_hier and horodate < @vl_aujourd_hui
			end

		fetch Pointeur_station into @vl_Station, @vl_Sens, @vl_Voie
	end
	close Pointeur_station

#endif
	return XDC_OK
go
