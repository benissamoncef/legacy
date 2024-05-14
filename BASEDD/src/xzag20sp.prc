/*E*/
/*  Fichier : $Id: xzag20sp.prc,v 1.9 2020/11/12 09:27:37 devgtie Exp $      Release : $Revision: 1.9 $        Date : $Date: 2020/11/12 09:27:37 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag20sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	20/06/95	: Creation	(V 1.1)
* C.T.	10/08/95	: Modif facon de supprimer (V 1.2)
* C.T.	19/09/95	: Correction pour chaussee (V 1.3)
* C.T.	06/09/96	: Ajout test pour serveur DC et DC (V 1.4)
* C.P.	07/01/98	: Correction gestion multi-sites dans les procs (V 1.6) (dem / 1536)
* JPL.	12/06/08	: Purge mesures de la veille si aucune de 07:00 a 22:00 (DEM 680) V1.7
* PNI	17/10/11	: exclure 7h30 dans la plage de vérification de données inexistantes DEM 1004 v1.8
* JPC	24/11/23	: Forcer le format de date en JJ/MM/AAAA (SAE-529)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* . Purge des mesures trafic de la base d'exploitation datant
*   d'au moins la date precisee
* . Purge des mesures trafic de la veille de la base d'exploitation
*   pour toute station de sens 1 ou 2 sans mesure de 07:00 a 22:00.
* 
* Sequence d'appel
* SP	XZAG20_Purge_Mesures_Trafic_Base_Exploitation
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
*  . Suppression des lignes anterieures a la date indiquee (1 mois)
*  . Suppression des lignes de la veille si aucune mesure de 7h a 22h
-------------------------------------------------------*/

use PRC
go

create procedure XZAG;20
	@va_Horodate_in		datetime = null
as
	/* Format de date en Jour/Mois/Annee */
	set dateformat 'dmy'

	declare @vl_Site T_SITE, @vl_Station smallint,
		@vl_Sens T_SENS, @vl_Voie T_VOIE, 
		@vl_Chaussee T_VOIE

	/*A controle du parametre en entree */
	if @va_Horodate_in = null
		return XDC_ARG_INV

	/*A Recherche du numero du site local */
	select @vl_Site = numero 
        from CFG..RES_DIS
        where serveur = @@servername

        if @vl_Site is null
           	return XDC_NOK

	/*A rechercher les points de mesures */
	select
		station,
		sens,
		voie
	into #STATIONS
	from CFG..EQT_PNT_MES
	where voie<>0

	/*A ajout de la chaussee dans les points de mesures */
	select 	@vl_Chaussee = 0
/*	insert #STATIONS
	select distinct
		station,
		sens,
		@vl_Chaussee
	from #STATIONS*/
	
	select 	@vl_Chaussee = 0
	insert #STATIONS
	select distinct
		station,
		sens,
		@vl_Chaussee
	from CFG..RES_CHA		

	/*A Effacer toutes les mesures de trafic anterieures a la date */
	/*A donnee sur le site local                                   */
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
		delete EXP..MES_TRA 
		where station = @vl_Station and
			sens = @vl_Sens and voie = @vl_Voie and
			horodate <= @va_Horodate_in and sit = @vl_Site

		fetch Pointeur_station into @vl_Station, @vl_Sens, @vl_Voie
	end
	close Pointeur_station


#ifndef CI
	/*A Pour chaque station en section courante (sens 1 ou 2), */
	/*A effacer toutes les mesures de trafic de la veille      */
	/*A sur le site local s'il n'y a pas de mesure de 7h a 22h */
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

			if not exists (select * from EXP..MES_TRA
					where station = @vl_Station and
						sens = @vl_Sens and
						voie = @vl_Voie and
						horodate > @vl_hier_a_07h30 and horodate < @vl_hier_a_22h00)
			begin
				-- alors supprimer toutes les mesures d hier pour cette voie
				-- SI LA STATION SE SITUE SUR LE SITE LOCAL
				delete EXP..MES_TRA 
				where station = @vl_Station and
					sens = @vl_Sens and
					voie = @vl_Voie and
					sit = @vl_Site and
					horodate >= @vl_hier and horodate < @vl_aujourd_hui
			end

		fetch Pointeur_station into @vl_Station, @vl_Sens, @vl_Voie
	end
	close Pointeur_station
#endif

	return XDC_OK
go
