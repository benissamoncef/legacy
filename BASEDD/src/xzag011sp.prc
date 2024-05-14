/*E*/
/*  Fichier : @(#)xzag20sp.prc	1.5      Release : 1.5        Date : 05/15/97
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag007sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* copie de tables
*
------------------------------------------------------
* HISTORIQUE :
*
* C.PENOT	22/07/1997	: Creation	(V 1.1)
* JMG		27/02/17 : regio 1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Copie dans une table des données 	si horodate < horodate_fin
* 					si horodate > horodate_debut
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
-------------------------------------------------------*/

use PRC
go

create procedure XZAG011
	@va_HorodateDebut_in	datetime	= null,
	@va_HorodateFin_in	datetime	= null,
	@va_Site		T_SITE
as
	declare @vl_Site T_SITE, @vl_Station smallint,
		@vl_Sens T_SENS, @vl_Voie T_VOIE, 
		@vl_DateDebut datetime, @vl_DateFin datetime,
		@vl_Chaussee T_VOIE


	select @vl_Site = numero from CFG..RES_DIS
	      where serveur = @@servername

	/*A rechercher les points de mesures */
	select
		station,
		sens,
		voie
	into #STATIONS
	from CFG..EQT_PNT_MES
	where voie<>0

	select  @vl_Chaussee = 0
	insert #STATIONS
	select distinct
		station,
		sens,
		@vl_Chaussee
	from CFG..RES_CHA


	/*A Copier toutes les lignes dont la date est comprise dans 	*/
	/*A l'interval donne par la procedure 				*/
	/*! (utiliser l'index de la table des mesures) 			*/
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
		select @vl_DateDebut=@va_HorodateDebut_in
			
		while @vl_DateDebut<=@va_HorodateFin_in
		begin
			insert EXP..MES_TRA 
			select * from HIS..MES_TRA2
			where station = @vl_Station and
				sens = @vl_Sens and voie = @vl_Voie 
				and sit= @va_Site and horodate=@vl_DateDebut
				
			select @vl_DateDebut  = dateadd(minute, 6, @vl_DateDebut)
		
		end

		fetch Pointeur_station into @vl_Station, @vl_Sens, @vl_Voie
	end
	close Pointeur_station

	return XDC_OK
go
