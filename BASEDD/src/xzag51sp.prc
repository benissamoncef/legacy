/*E*/
/*  Fichier : $Id: xzag51sp.prc,v 1.2 1998/01/08 16:18:37 penot Exp $      Release : $Revision: 1.2 $        Date : $Date: 1998/01/08 16:18:37 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag51sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.	09/10/96	: Creation	(dem/1227) (V 1.1)
* C.P.	07/01/98	: Correction gestion multi-sites dans 
*			  les procs (V 1.2) (dem / 1536)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Purge des mesures pesage de la base d'exploitation datant
* d'au moins la date precisee
* 
* Sequence d'appel
* SP	XZAG20_Purge_Mesures_Pesage_Base_Exploitation
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
* Fonction
* Suppression des lignes vielles de plus d'un mois
-------------------------------------------------------*/

use PRC
go

create procedure XZAG;51
	@va_Horodate_in		datetime = null
as
	declare @vl_Site T_SITE, @vl_Station smallint,
		@vl_Sens T_SENS, @vl_Voie T_VOIE, 
		@vl_Chaussee T_VOIE
		
#ifndef CI
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

	/*A ajout de la chaussee dans les points de mesures */
	select 	@vl_Chaussee = 0
	insert #STATIONS
	select distinct
		station,
		sens,
		@vl_Chaussee
	from #STATIONS

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
		delete EXP..MES_KCS 
		where station = @vl_Station and
			sens = @vl_Sens and voie = @vl_Voie and
			horodate <= @va_Horodate_in and sit = @vl_Site
		delete EXP..MES_NEC 
		where station = @vl_Station and
			sens = @vl_Sens and voie = @vl_Voie and
			horodate <= @va_Horodate_in and sit = @vl_Site
		delete EXP..MES_QPC 
		where station = @vl_Station and
			sens = @vl_Sens and voie = @vl_Voie and
			horodate <= @va_Horodate_in and sit = @vl_Site

		fetch Pointeur_station into @vl_Station, @vl_Sens, @vl_Voie
	end
	close Pointeur_station
#endif
	return XDC_OK
go
