/*E*/
/*  Fichier : $Id: xzag54sp.prc,v 1.7 2020/10/07 08:43:26 gesconf Exp $      Release : $Revision: 1.7 $        Date : $Date: 2020/10/07 08:43:26 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag54sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.	27/02/97	: Creation	(dem/1396) (V 1.1)
* C.P.	07/01/98	: Correction gestion multi-sites dans 
*			  les procs (V 1.2) (dem / 1536)
* JMG	20/08/13	: ajout REG_TRA 1.3
* PNI	09/09/20	: ajout table GAB_TRA 1.4
* LPE	24/07/20	: ajout TDP_FCD 1.4 DEM-SAE152
* PNI	02/10/20	: purge de TDP_FCD à 2 semaines v1.5
* PNI	07/10/20	: purge de TDP_FCD à 1 j v1.6
* LCL	02/10/23	: purge PIT_LOG et REF_PIT_MESSAGE DEM-527
* LCL	11/10/23	: Correction purges Cockpit DEM-527
* PNI	14/11/23	: purger TDP_FCD uniquement sur CI SAE-548
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Purge des tdp 6mn de la base d'exploitation datant
* d'au moins la date precisee
* 
* Sequence d'appel
* SP	XZAG20_Purge_TDP_6mn_Base_Exploitation
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

create procedure XZAG;54
	@va_Horodate_in		datetime = null
as
	declare @vl_Site T_SITE, @vl_Numero smallint,
		@vl_Horodate datetime, @vl_horodateFCD datetime, @vl_reference varchar(50)
		
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
		delete EXP..ZDP_6MN 
		where numero=@vl_Numero and
			horodate <= @va_Horodate_in and sit = @vl_Site

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
		delete EXP..REG_TRA
		where horodate <= @va_Horodate_in
		and zone = @vl_Numero

		fetch Pointeur_zone into @vl_Numero
	end
	close Pointeur_zone

	declare Pointeur_gab cursor
	for select
		distinct echangeur
	from EXP..GAB_TRA
	where echangeur<255

	open Pointeur_gab
	fetch Pointeur_gab into @vl_Numero
	while @@sqlstatus = 0
	begin
		delete EXP..GAB_TRA
		where horodate <= @va_Horodate_in
		and echangeur = @vl_Numero

		fetch Pointeur_gab into @vl_Numero
	end
	close Pointeur_gab

	delete EXP..FRN_TDP
	where fin <= @va_Horodate_in and
	      fin <> null and site_zdp = @vl_Site
	

	/* SAE-548 : purge TDP_FCD uniquement au CI */
#else

	/*A rechercher les autoroutes */
	select
		numero
	into #TDP
	from CFG..RES_AUT

	/* Purge de sdonnees FCD à 2 semaines au lieu d'1 mois !!!!!!!!*/
	select @vl_horodateFCD = dateadd(day,-1,getdate())

	/*A Effacer toutes les tdp anterieures a la date pour TDP_FCD*/
	declare Pointeur_tdp cursor
	for select
		numero
	from #TDP

	open Pointeur_tdp
	fetch Pointeur_tdp into @vl_Numero
	while @@sqlstatus = 0
	begin	
                delete EXP..TDP_FCD 
		where autoroute=@vl_Numero and
			horodate <= @vl_horodateFCD

		fetch Pointeur_tdp into @vl_Numero
	end
	close Pointeur_tdp

	/* purge des donnees relatives a Cockpit : PIT_LOG et REF_PIT_MESSAGE */
	declare Pointeur_cockpit cursor
	for select reference
		from EXP..PIT_LOG
		where EXP..PIT_LOG.horodate <= @va_Horodate_in

	open Pointeur_cockpit
	fetch Pointeur_cockpit into @vl_reference
	while @@sqlstatus = 0
	begin	
        delete EXP..REF_PIT_MESSAGE 
		where EXP..REF_PIT_MESSAGE.reference=@vl_reference
        delete EXP..FMC_REF_EXT 
		where EXP..FMC_REF_EXT.reference=@vl_reference

		fetch Pointeur_cockpit into @vl_reference
	end
	close Pointeur_cockpit
	delete EXP..PIT_LOG where EXP..PIT_LOG.horodate <= @va_Horodate_in

#endif

	return XDC_OK
go
