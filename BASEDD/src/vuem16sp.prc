/*E*/
/*  Fichier : $Id: vuem16sp.prc,v 1.5 2016/09/13 17:30:29 devgfi Exp $        Release : $Revision: 1.5 $        Date : $Date: 2016/09/13 17:30:29 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER vuem16_Recap_Actions_BRAsp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* fiche metier dont l'objet est de connaitre la liste
* des actions BRA ainsi que leurs consequences sur
* le debit du trafic
*
------------------------------------------------------
* HISTORIQUE :
*
* L.V.	21/02/1997	: Creation V1.0
* P.N.	05/02/99	: Modif interg. dem/1724 v1.3
* JPL	29/09/10	: Adaptation compilateur C (# en debut de ligne) 1.4
* JPL	13/09/16	: Selection des actions par le site de gestion equipement (pour DEM 1171) 1.5
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	VUEM16_Recap_Actions_BRA
* 
* Arguments en entree
* XDY_Horodate	va_Debut_in
* XDY_Horodate	va_Fin_in
* XDY_Site	va_District_in	va_District_in=0 si tous les districts
* 
* Arguments en sortie
* 
* Service rendu
* Liste les actions BRA sur un ou tous les districts
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
-----------------------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'VUEM16' and type = 'P')
	drop procedure VUEM16
go


create procedure VUEM16
	@va_Debut_in	datetime	= null,
	@va_Fin_in	datetime	= null,
	@va_District_in	int		= null
as
	declare
		@vl_Station smallint,
		@vl_Sens tinyint,
		@vl_Numero int,
		@vl_Sit int,
		@vl_Horo_Ouverture datetime,
		@vl_Horo_Fermeture datetime,
		@vl_Horo_Ouverture_Pred datetime,
		@vl_Horo_Fermeture_Prev datetime,
		@vl_Q_Ouverture int,
		@vl_Horo_Q_Max datetime,
		@vl_Q_Max int,
		@vl_Q_Fermeture int,
		@vl_Cause char(25),
		@vl_Scenario char(25),
		@vl_BRA int,
		@vl_Implantation char(25),
		@vl_Autoroute char(5),
		@vl_PR int



	/*A controle des parametres d'entree */
	if @va_Debut_in is null
		return XDC_ARG_INV
		
	if @va_Fin_in is null
		return XDC_ARG_INV
		
	if @va_District_in is null
		return XDC_ARG_INV

	/*A Est-ce une requete sur tous les districts? */
	if @va_District_in = 0 select @va_District_in = null

	/*A Creation de la table resultat */	
	create table #LISTE_ACTION_BRA
				(numero int not null,
				sit int not null,
				cause char(25),
				scenario char(25) null,
				BRA int not null,
				implantation char(25) null,
				autoroute char(5) null,
				PR int null,
				sens tinyint null,
				station smallint null,
				horo_ouverture datetime null,
				Q_ouverture int null,
				horo_Q_max datetime null,
				Q_max int null, 
				horo_fermeture datetime null,
				Q_fermeture int null)
				

	/*A Creation de la table contenant la liste de tous les BRA */
	/*  et de leur station correspondante.		*/
	create table #LISTE_BRA
		(numero smallint not null,
		type_BRA char(25) null,
		autoroute char(5) null,
		PR int null,
		sens tinyint null,
		site_gestion tinyint null,
		portion int null,
		station smallint null)
		
	/*A Creation de la table contenant les debits associes a */
	/*  une action BRA 	*/
	create table #MES_Q
		(station smallint,
		sens tinyint null,
		voie tinyint null,
		horodate datetime null,
		Q smallint null)
	
		
					 
	/*A Creation de la table temporaire contenant la liste de tous les */
	/*  BRA et de leur station correspondante.	*/
	
	insert #LISTE_BRA (numero, type_BRA, autoroute, PR, sens, site_gestion, portion, station)
	select
		CFG..EQT_GEN.numero,
		CFG..TYP_NAV.nom,
		CFG..RES_AUT.nom,
		CFG..EQT_GEN.PR,
		CFG..EQT_GEN.sens,
		CFG..EQT_GEN.site_gestion,
		portion = CFG..RES_POR.numero,
		CFG..RES_INF.station
	from
		CFG..EQT_GEN,
		CFG..EQT_NAV,
		CFG..RES_AUT,
		CFG..RES_POR,
		CFG..TYP_NAV,
		CFG..RES_INF
	where
		CFG..EQT_GEN.type = XDC_EQT_NAV and
		CFG..EQT_GEN.numero = CFG..EQT_NAV.numero and
		CFG..EQT_NAV.type = CFG..TYP_NAV.numero and
		CFG..EQT_GEN.autoroute = CFG..RES_AUT.numero and
		CFG..RES_POR.autoroute = CFG..EQT_GEN.autoroute and
		CFG..EQT_GEN.sens = CFG..RES_INF.sens and
		CFG..RES_INF.PR_debut <= CFG..EQT_GEN.PR and
		CFG..RES_INF.PR_fin > CFG..EQT_GEN.PR and
		CFG..RES_POR.numero = CFG..RES_INF.portion and
		CFG..EQT_NAV.type != 3



	/*A Ajout des BRA de contole d'acces (CFG..EQT_NAV.type=3) a la liste des BRA */
	/*  car les BRA de controle d'acces n'ont pas de station de comptage */
	
/*
	insert #LISTE_BRA (numero, type_BRA, autoroute, PR, sens, site_gestion, portion, station)
	select
		CFG..EQT_GEN.numero,
		CFG..TYP_NAV.nom,
		CFG..RES_AUT.nom,
		CFG..EQT_GEN.PR,
		CFG..EQT_GEN.sens,
		CFG..EQT_GEN.site_gestion,
		null,
		null
	from
		CFG..EQT_GEN,
		CFG..TYP_NAV,
		CFG..RES_AUT,
		CFG..EQT_NAV
	where
		CFG..EQT_NAV.type = 3 and
		CFG..EQT_GEN.type = XDC_EQT_NAV and
		CFG..EQT_GEN.autoroute = CFG..RES_AUT.numero and
		CFG..EQT_GEN.numero = CFG..EQT_NAV.numero and
		CFG..EQT_NAV.type = CFG..TYP_NAV.numero
*/


	/*A Recherche de la liste des actions BRA avec un curseur */
	
	declare pointeur_action_BRA cursor
	for select
		HIS..ACT_GEN.numero,
		HIS..ACT_GEN.sit,
		CFG..TYP_FMC.nom,
		HIS..ACT_NAV.scenario,
		LBRA.numero,
		LBRA.type_BRA,
		LBRA.autoroute,
		LBRA.PR,
		LBRA.sens,
		LBRA.station,
		HIS..ACT_GEN.heure_succes,
		HIS..ACT_GEN.heure_fin
	from 
		HIS..ACT_GEN, #LISTE_BRA LBRA,
		HIS..FMC_GEN,
		HIS..ACT_NAV,
		CFG..TYP_FMC
	where	
		/* filtrage sur horodate et district des actions BRA */
		HIS..ACT_GEN.type = XDC_ACT_NAV and
		HIS..ACT_GEN.heure_succes >= @va_Debut_in and
		HIS..ACT_GEN.heure_succes <= @va_Fin_in and

		/* jointure avec LBRA et FMC_GEN */
		HIS..ACT_GEN.equipement = LBRA.numero	and
		HIS..ACT_GEN.evenement = HIS..FMC_GEN.numero and
		HIS..ACT_GEN.cle = HIS..FMC_GEN.cle and 

		/* filtrage du site de recherche */
		( (@va_District_in = 99 and HIS..FMC_GEN.datex = XDC_VRAI) or
		  (@va_District_in is null) or
		  (@va_District_in = LBRA.site_gestion and HIS..FMC_GEN.datex <> XDC_VRAI) )  and

		/* recherche du scenario par jointure avec ACT_NAV */
		HIS..ACT_GEN.numero = HIS..ACT_NAV.actio and
		HIS..ACT_GEN.sit = HIS..ACT_NAV.sit and

		/* recherche du nom du type FMC */
		CFG..TYP_FMC.numero = HIS..FMC_GEN.type
	open pointeur_action_BRA

	fetch pointeur_action_BRA into	@vl_Numero,
					@vl_Sit,
					@vl_Cause,
					@vl_Scenario,
					@vl_BRA,
					@vl_Implantation,
					@vl_Autoroute,
					@vl_PR,
					@vl_Sens, 	
					@vl_Station,
					@vl_Horo_Ouverture,
					@vl_Horo_Fermeture
	
	
	/*A On parcourt les actions BRA une a une avec le pointeur */
	/*  pour y ajouter les debits associes dans la table #LISTE_ACTION_BRA */
	
	while (@@sqlstatus = 0)
	begin
		/*A On insere chaque action dans la table #LISTE_ACTION_BRA */

		insert #LISTE_ACTION_BRA 
		(numero,
		sit,
		cause,
		scenario, 
		BRA, 
		implantation, 
		autoroute, 
		PR, 
		sens,
		station, 
		horo_ouverture, 
		horo_fermeture)
						
		values (@vl_Numero,
			@vl_Sit,
			@vl_Cause,
			@vl_Scenario,
			@vl_BRA,
			@vl_Implantation,
			@vl_Autoroute,
			@vl_PR,
			@vl_Sens, 	
			@vl_Station,
			@vl_Horo_Ouverture,
			@vl_Horo_Fermeture)		

		/*A On ne recherche pas les debits liees aux BRA qui n'ont pas de station */
		if @vl_Station is not null
		
		begin
	
			/* On s'interresse a la mesure precedent l'ouverture du BRA */ 
			select @vl_Horo_Ouverture_Pred = dateadd(minute,-6,@vl_Horo_Ouverture)
			/* Et a la mesure suivant la fermeture du BRA */
			select @vl_Horo_Fermeture_Prev = dateadd(minute,+6,@vl_Horo_Fermeture)
		
			/*A Extraction dans une table temporaire des debits de la
			station qui est associee a l'action BRA */
		
			insert #MES_Q (station, sens, voie, horodate, Q)
			select
				HIS..MES_TRA.station,
				HIS..MES_TRA.sens,
				HIS..MES_TRA.voie,
				HIS..MES_TRA.horodate,
				HIS..MES_TRA.Q
			from
				HIS..MES_TRA
			where
				HIS..MES_TRA.station = @vl_Station and
				HIS..MES_TRA.sens = @vl_Sens and
				HIS..MES_TRA.voie = XDC_RADT_CHAUSSEE and
				HIS..MES_TRA.horodate > @vl_Horo_Ouverture_Pred and
				HIS..MES_TRA.horodate < @vl_Horo_Fermeture_Prev
			
			
			/* On calcule le debit max lie a l'action BRA */
			/* !!!!!!!!!!! attention il faut 1 seul debit max !!!!!!!!!! */
			select @vl_Q_Max = null
			select @vl_Horo_Q_Max = null
			select
				@vl_Q_Max = max(#MES_Q.Q),
				@vl_Horo_Q_Max = #MES_Q.horodate
			from #MES_Q


			/* On calcule le debit d'ouverture */
			select @vl_Q_Ouverture = null
			select
				@vl_Q_Ouverture = Q
			from #MES_Q
			where
				horodate <= @vl_Horo_Ouverture and
				horodate > @vl_Horo_Ouverture_Pred
			
			/* On calcule le debit de fermeture */
			select @vl_Q_Fermeture = null
			select
				@vl_Q_Fermeture = Q
			from #MES_Q
			where
				horodate >= @vl_Horo_Fermeture and
				horodate < @vl_Horo_Fermeture_Prev
			
			
			/* On update la table #LISTE_ACTION_BRA avec les debits trouves */
			update #LISTE_ACTION_BRA
			set
				Q_ouverture = @vl_Q_Ouverture,
				horo_Q_max = @vl_Horo_Q_Max,
				Q_max = @vl_Q_Max,
				Q_fermeture = @vl_Q_Fermeture
			where
				numero = @vl_Numero and
				sit = @vl_Sit
				
		
			/* On purge la table temporaire des debits pour l'action BRA suivante */
			delete #MES_Q
	
		end
		
		fetch pointeur_action_BRA into	
					@vl_Numero,
					@vl_Sit,
					@vl_Cause,
					@vl_Scenario,
					@vl_BRA,
					@vl_Implantation,
					@vl_Autoroute,
					@vl_PR,
					@vl_Sens, 	
					@vl_Station,
					@vl_Horo_Ouverture,
					@vl_Horo_Fermeture
					
	end


	close pointeur_action_BRA


	/*A retourner la liste */
	
	select
		PR,
		autoroute,
		sens,
		implantation,
		horo_ouverture=convert(char(10),horo_ouverture,103)+' '+convert(char(8),horo_ouverture,8),
		Q_ouverture,
		horo_Q_max=convert(char(10),horo_Q_max,103)+' '+convert(char(8),horo_Q_max,8),
		Q_max,
		horo_fermeture=convert(char(10),horo_fermeture,103)+' '+convert(char(8),horo_fermeture,8),
		Q_fermeture,
		scenario,
		cause 
	from #LISTE_ACTION_BRA


	return XDC_OK
go
