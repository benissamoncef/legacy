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
* Copie dans une table des données 	
* 			si horodate < horodate_debut
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

create procedure XZAG014
	@va_Horodate_in	datetime = null
as
	declare @vl_Site T_SITE, @vl_Station smallint,
		@vl_Sens T_SENS, @vl_Voie T_VOIE, 
		@vl_Chaussee T_VOIE

	/*A controle du parametre en entree */
	if @va_Horodate_in = null
		return XDC_ARG_INV


	/*A Recherche du numero du site local */
	select @vl_Site = numero from CFG..RES_DIS
	      where serveur = @@servername

	/*A Effacer toutes les mesures de niveau anterieures a la date */
		/*A donnee sur le site local                                   */
		/*! (utiliser l'index de la table des mesures) */
		declare Pointeur_station cursor
		for select distinct
				station,
				sens
		from CFG..RES_CHA

	open Pointeur_station
	fetch Pointeur_station into @vl_Station, @vl_Sens
	while @@sqlstatus = 0
	begin
		delete EXP..MES_NIV 
		where station = @vl_Station and
		sens = @vl_Sens and 
		horodate < @va_Horodate_in and sit = 21

		fetch Pointeur_station into @vl_Station, @vl_Sens
	end
	close Pointeur_station

	return XDC_OK
go
