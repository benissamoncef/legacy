/*E*/
/*  Fichier : $Id: xzag52sp.prc,v 1.3 2002/03/07 10:12:40 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 2002/03/07 10:12:40 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag52sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.	09/10/95	: Creation	(V 1.1)
* P.N.	10/10/95	: Ajout des actions sur MES_MET	(V 1.2)
* P.C.	06/03/02        : Correction pour HIS V1.3   
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Purge des mesures peasage de la base de l'historique datant
* d'au moins la date precisee
* 
* Sequence d'appel
* SP	XZAG28_Purge_Mesures_Pesage_Base_Historique
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
* Suppression des lignes vielles de plus de 5 ans
-------------------------------------------------------*/

use PRC
go

create procedure XZAG;52
	@va_Horodate_in		datetime = null
as
	declare @vl_Site T_SITE, @vl_Station smallint,
		@vl_Sens T_SENS, @vl_Voie T_VOIE,
		@vl_Chaussee T_VOIE

#ifdef HIST
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
		delete HIS..MES_QPC 
		where station = @vl_Station and
			sens = @vl_Sens and voie = @vl_Voie and
			horodate <= @va_Horodate_in
		delete HIS..MES_NEC 
		where station = @vl_Station and
			sens = @vl_Sens and voie = @vl_Voie and
			horodate <= @va_Horodate_in
		delete HIS..MES_KCS 
		where station = @vl_Station and
			sens = @vl_Sens and voie = @vl_Voie and
			horodate <= @va_Horodate_in

		fetch Pointeur_station into @vl_Station, @vl_Sens, @vl_Voie
	end
	close Pointeur_station
	
	
	delete HIS..MES_MET 
	where jour <= @va_Horodate_in
#endif

	return XDC_OK
go
