/*E*/
/*  Fichier : $Id: xzag55sp.prc,v 1.6 2020/11/26 16:44:12 devgtie Exp $      Release : $Revision: 1.6 $        Date : $Date: 2020/11/26 16:44:12 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag55sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.	27/02/97	: Creation	(dem/1396) (V 1.1)
* PNI	27/06/16	: Ajout de ZDP_6MN v1.3
* PNI	09/09/20	: ajout table GAB_TRA 
* LPE	24/07/20	: Ajout de TDP_FCD v1.4 DEM-SAE152
* PNI	02/10/20	: purge de TDP_FCD à 2 mois v1.5
* LCL	02/10/23	: purge PIT_LOG et REF_PIT_MESSAGE DEM-527
* LCL	11/10/23	: Correction purges Cockpit DEM-527
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Purge des tdp 1h de la base d'exploitation datant
* d'au moins la date precisee
* 
* Sequence d'appel
* SP	XZAG20_Purge_TDP_1h_Base_Historique
* 
* Arguments en entree
* XDY_Datetime	va_Horodate_in	: date de fin de purge
* 
* Arguments en sortie
* 
* Code retour
 XDC_OK
* XDC_ARG_INV	: parametre d'entree null
* 
* Conditions d'utilisation
* 
* Fonction
* Suppression des lignes vielles de plus d'un mois
-------------------------------------------------------*/

use PRC
go

create procedure XZAG;55
	@va_Horodate_in		datetime = null
as
	declare @vl_Site T_SITE, @vl_Numero smallint,
		@vl_Horodate datetime, @vl_horodateFCD datetime,
		@vl_reference varchar(50), @vl_horodateCockpit datetime
		
#ifdef HIST
	/*A controle du parametre en entree */
	if @va_Horodate_in = null
		return XDC_ARG_INV


	/*A rechercher les points de mesures */
	select
		numero
	into #ZDP
	from CFG..EQT_ZDP


	/*A Effacer toutes les tdp anterieures a la date */
	/*A donnee sur le site local                                   */
	/*! (utiliser l'index de la table des mesures) */
	declare Pointeur_zdp cursor
	for select
		numero
	from #ZDP

	open Pointeur_zdp
	fetch Pointeur_zdp into @vl_Numero
	while @@sqlstatus = 0
	begin

		delete HIS..ZDP_HEU 
		where numero=@vl_Numero and
			horodate <= @va_Horodate_in 
		delete HIS..ZDP_6MN
		where numero=@vl_Numero and
			horodate <= @va_Horodate_in 

		fetch Pointeur_zdp into @vl_Numero
	end
	close Pointeur_zdp

	declare Pointeur_zone cursor
	for select
		numero
	from CFG..ZON_REG

	open Pointeur_zone
	fetch Pointeur_zone into @vl_Numero
	while @@sqlstatus = 0
	begin
		delete HIS..REG_TRA
		where horodate <= @va_Horodate_in
		and zone = @vl_Numero

		fetch Pointeur_zone into @vl_Numero
	end
	close Pointeur_zone

	declare Pointeur_gab cursor
	for select
		distinct echangeur
	from HIS..GAB_TRA
	where echangeur<255

	open Pointeur_gab
	fetch Pointeur_gab into @vl_Numero
	while @@sqlstatus = 0
	begin
		delete HIS..GAB_TRA
		where horodate <= @va_Horodate_in
		and echangeur = @vl_Numero

		fetch Pointeur_gab into @vl_Numero
	end
	close Pointeur_gab


	/*A rechercher les autoroutes */
	select
		numero
	into #TDP
	from CFG..RES_AUT
	/* Purge de sdonnees FCD à 12 mois au lieu d'10 ans !!!!!!!!*/
	select @vl_horodateFCD = dateadd(month,-12,getdate())

	/*A Effacer toutes les tdp anterieures a la date pour TDP_FCD*/
	declare Pointeur_tdp cursor
	for select
		numero
	from #TDP

	open Pointeur_tdp
	fetch Pointeur_tdp into @vl_Numero
	while @@sqlstatus = 0
	begin	
                delete HIS..TDP_FCD 
		where autoroute=@vl_Numero and
			horodate <= @vl_horodateFCD

		fetch Pointeur_tdp into @vl_Numero
	end
	close Pointeur_tdp

	/* Purge des donnees Cockpit à 1 mois au lieu de 10 ans */
	select @vl_horodateCockpit = dateadd(month,-1,getdate())

	/* purge des donnees relatives a Cockpit : PIT_LOG et REF_PIT_MESSAGE */
	declare Pointeur_cockpit cursor
	for select reference
		from HIS..PIT_LOG
		where HIS..PIT_LOG.horodate <= @vl_horodateCockpit

	open Pointeur_cockpit
	fetch Pointeur_cockpit into @vl_reference
	while @@sqlstatus = 0
	begin	
        delete HIS..REF_PIT_MESSAGE 
		where HIS..REF_PIT_MESSAGE.reference=@vl_reference
        delete HIS..FMC_REF_EXT 
		where HIS..FMC_REF_EXT.reference=@vl_reference

		fetch Pointeur_cockpit into @vl_reference
	end
	close Pointeur_cockpit
	delete HIS..PIT_LOG where HIS..PIT_LOG.horodate <= @vl_horodateCockpit

#endif
	return XDC_OK
go
