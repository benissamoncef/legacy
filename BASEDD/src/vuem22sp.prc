/*E*/
/*  Fichier : $Id: vuem22sp.prc,v 1.6 2010/09/29 17:56:07 gesconf Exp $        Release : $Revision: 1.6 $        Date : $Date: 2010/09/29 17:56:07 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER vuem22sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* fiche metier dont l'objet est de rechercher les 
* evenements lies a des conditions accidentogenes
*
------------------------------------------------------
* HISTORIQUE :
*
* L.V.	04/03/1997	: Creation V1.0
* L.V.	26/05/1997	: Prise en compte des peages V1.1
* P.N.	05/02/99	: Modif interg. dem/1724 v1.5
* JPL	29/09/10	: Adaptation compilateur C (# en debut de ligne) 1.6
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	VUEM22_RechercheAccidentogene
* 
* Arguments en entree
* XDY_Horodate	va_Debut_in
* XDY_Horodate	va_Fin_in
* int		va_Num_evt_in 
* int		va_Type_condition_in 	type de condition accidentogene parmi les conditions
*				      	suivantes : meteo, travaux, chaussee, type d'ouvrage
*					Les types de condition accidentogene sont definis dans xdc.h
* int		va_Condition_in		condition accidentogene pour les types chaussee et type d'ouvrage
*					parmi les conditions suivantes :
*					chaussee avec BAU < 2m, chaussee avec pente ou rampe > seuil
*					chaussee avec un nbre de voies donne, presence d'un ouvrage
*					tunnel ou echangeur ou PI ou PS
*					Les conditions accidentogenes sont definies dans xdc.h
* int		va_Seuil_pente_in
* int		va_Nb_voies_in
* 
* Arguments en sortie
* char(25)	type
* datetime	horodate
* int		numero
* int		PR
* tinyint	sens
* char(4)	autoroute
* smallint	Q
* tinyint	V
* tinyint	T_O 
*	
* Service rendu
* Recherche les evenements qui se sont produits a cause d'une condition accidentogene
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

create procedure VUEM22
	@va_Debut_in		datetime	= null,
	@va_Fin_in		datetime	= null,
	@va_Num_evt_in		int		= null,
	@va_Type_condition_in	int		= null,
	@va_Condition_in	int		= null,
	@va_Seuil_pente_in	int		= null,
	@va_Nb_voies_in		tinyint		= null
as
	declare
		@vl_numero		int,
		@vl_cle			tinyint,
		@vl_horodate		datetime,
		@vl_PR			int,
		@vl_sens		tinyint,
		@vl_autoroute		tinyint,
		@vl_portion		smallint,
		@vl_station		smallint,
		@vl_Q			smallint,
		@vl_V			tinyint,
		@vl_T_O			tinyint,
		@vl_type		char(25),
		@vl_numero_cause	int,
		@vl_cle_cause		tinyint,
		@vl_type_cause		smallint,
		@vl_BAU			tinyint,
		@vl_nb_voies		tinyint,
		@vl_nom_autoroute	char(4)





/*A controle des parametres d'entree */
if @va_Debut_in is null
	return XDC_ARG_INV
		
if @va_Fin_in is null
	return XDC_ARG_INV		

if @va_Num_evt_in is null
	return XDC_ARG_INV
	
if @va_Type_condition_in is null
	return XDC_ARG_INV
	
if (@va_Type_condition_in = XDC_TYPE_CONDITION_CHAUSSEE and @va_Condition_in = XDC_CONDITION_CHAUSSEE_AVEC_NB_VOIES and @va_Nb_voies_in is null)
	return XDC_ARG_INV			

if (@va_Type_condition_in = XDC_TYPE_CONDITION_CHAUSSEE and @va_Condition_in = XDC_CONDITION_CHAUSSEE_AVEC_PENTE and @va_Seuil_pente_in is null)
	return XDC_ARG_INV

if (@va_Type_condition_in = XDC_TYPE_CONDITION_CHAUSSEE and @va_Condition_in = XDC_CONDITION_CHAUSSEE_AVEC_RAMPE and @va_Seuil_pente_in is null)
	return XDC_ARG_INV


/*A Est-ce une requete pour tous les evenements? */
if @va_Num_evt_in = 0 select @va_Num_evt_in = null

			
/*A Creation de la table des evenements bis  mais cette fois avec le PR, l'horodate, le sens et l'aut. */	
create table #LISTE_EVTS
			(numero int null,
			cle tinyint null,
			type char(25) null,
			horodate datetime null,
			PR int null,
			sens tinyint null,
			autoroute tinyint null,
			portion smallint null,
			numero_cause int null,
			cle_cause tinyint null,
			type_cause  smallint null,
			nb_voies tinyint null,
			BAU tinyint null,
			pente smallint null,
			tunnel bit,
			echangeur tinyint null,
			PI smallint null,
			PS smallint null
			)
			
			
/*A Creation de la table resultat final a renvoyer */
create 	table #RESULTAT
			(numero int null,
			cle tinyint null,
			type char(25) null,
			horodate datetime null,
			PR int null,
			sens tinyint null,
			autoroute char(4) null,
			numero_cause int null,
			cle_cause tinyint null,
			type_cause  smallint null,
			portion smallint null,
			station	smallint null,
			Q int null,
			V tinyint null,
			T_O tinyint null,
			nb_voies int null,
			BAU tinyint null,
			pente smallint null,
			tunnel bit,
			echangeur tinyint null,
			PI smallint null,
			PS smallint null
			)
			
			 

/* ======================================================================= */
/*A TRAITEMENT DANS LE CAS OU LA CONDITION ACCIDENTOGENE EST DE TYPE METEO */
/* ======================================================================= */	

if @va_Type_condition_in = XDC_TYPE_CONDITION_METEO
begin

	/*A Insertion dans la table des evenements (sauf les bouchons!!!) qui sont la cause */
	/* d'evt meteo.	 */
	
	select
		numero = GEN1.numero,
		cle = GEN1.cle,
		type = CFG..TYP_FMC.nom,
		numero_cause = GEN2.numero,
		cle_cause = GEN2.cle,
		type_cause = GEN2.type
	into #EVENEMENTS
	from
		HIS..FMC_GEN GEN1, HIS..FMC_GEN GEN2, CFG..TYP_FMC
	where
		/* condition sur l'horodate : l'evt doit commencer ds la periode demandee et etre termine */
		GEN1.debut >= @va_Debut_in and
		GEN1.debut <= @va_Fin_in and
		/* condition sur le type de l'evt */
		(GEN1.type = @va_Num_evt_in or
		@va_Num_evt_in is null) and
		GEN1.type != XZAEC_FMC_QueueBouchon and
		GEN1.type != XZAEC_FMC_TeteBouchon and
		GEN1.datex<>XDC_VRAI and
		/* condition sur l'evt cause */
		GEN2.type = XZAEC_FMC_Meteo and
		GEN2.datex<>XDC_VRAI and
		/* jointure entre GEN1 et GEN2 */
		GEN1.cause = GEN2.numero and
		GEN1.cle_cause = GEN2.cle and
		/* jointure entre GEN1 et TYP_FMC */
		GEN1.type = CFG..TYP_FMC.numero

	
	/*A On ajoute la jointure avec FMC_HIS pour les evts autres que les bouchons */
	
	if (@va_Num_evt_in != XZAEC_FMC_TeteBouchon or @va_Num_evt_in != XZAEC_FMC_QueueBouchon)
	begin
	
		insert #LISTE_EVTS (numero, cle, type , horodate, PR, sens, autoroute, numero_cause, cle_cause, type_cause,tunnel)
		select
			EVT.numero,
			EVT.cle,
			EVT.type,
			HIS..FMC_HIS.horodate_validation,
			HIS..FMC_HIS.PR,
			HIS..FMC_HIS.sens,
			HIS..FMC_HIS.autoroute,
			EVT.numero_cause,
			EVT.cle_cause,
			EVT.type_cause,
			XDC_FAUX
		from	#EVENEMENTS EVT, HIS..FMC_HIS
		where
			/* jointure entre les 2 */
			EVT.numero = HIS..FMC_HIS.numero and
			EVT.cle = HIS..FMC_HIS.cle
		group by
			HIS..FMC_HIS.numero,
			HIS..FMC_HIS.cle,
			EVT.numero,
			EVT.cle
		having
			horodate_validation = max(horodate_validation)
			
			
	
	end
	
		
		
	/*A Si on demande les evts de type bouchon on les ajoute a la table #EVENEMENTS */
			 
	if (@va_Num_evt_in is null or @va_Num_evt_in = XZAEC_FMC_TeteBouchon or @va_Num_evt_in = XZAEC_FMC_QueueBouchon)
	begin

		/*A On retire tout de #EVENEMENTS pour pouvoir y mettre les bouchons */
		delete #EVENEMENTS
		
		
		/*A On insere les evts de type tete de bouchons */
		
		insert #EVENEMENTS (numero, cle, type, numero_cause, cle_cause, type_cause)
		select
			GEN1.numero,
			GEN1.cle,
			CFG..TYP_FMC.nom,
			GEN2.numero,
			GEN2.cle,
			GEN2.type
		from
			HIS..FMC_GEN GEN1, HIS..FMC_GEN GEN2, CFG..TYP_FMC
		where
			/* condition sur l'horodate : l'evt doit commencer ds la periode demandee et etre termine */
			GEN1.debut >= @va_Debut_in and
			GEN1.debut <= @va_Fin_in and
			/* condition sur le type de l'evt */
			GEN1.type = XZAEC_FMC_TeteBouchon and
			GEN1.datex<>XDC_VRAI and
			/* condition sur l'evt cause */
			GEN2.type = XZAEC_FMC_Meteo and
			GEN2.datex<>XDC_VRAI and
			/* jointure entre GEN1 et GEN2 */
			GEN1.cause = GEN2.numero and
			GEN1.cle_cause = GEN2.cle and
			/* jointure entre GEN1 et TYP_FMC */
			GEN1.type = CFG..TYP_FMC.numero
			
			
		/*A Et on fait la jointure avec FMC_HIS ds #LISTE_EVTS mais on prend la 1ere validation */
		
		insert #LISTE_EVTS (numero, cle, type , horodate, PR, sens, autoroute, numero_cause, cle_cause, type_cause,tunnel)
		select
			EVT.numero,
			EVT.cle,
			EVT.type,
			HIS..FMC_HIS.horodate_validation,
			HIS..FMC_HIS.PR,
			HIS..FMC_HIS.sens,
			HIS..FMC_HIS.autoroute,
			EVT.numero_cause,
			EVT.cle_cause,
			EVT.type_cause,
			XDC_FAUX
		from	#EVENEMENTS EVT, HIS..FMC_HIS
		where
			/* jointure entre les 2 */
			EVT.numero = HIS..FMC_HIS.numero and
			EVT.cle = HIS..FMC_HIS.cle
		group by
			HIS..FMC_HIS.numero,
			HIS..FMC_HIS.cle,
			EVT.numero,
			EVT.cle
		having
			horodate_validation = min(horodate_validation)
		
	end
	
	
	/*A Recherche des debits associes a chaque evt trouve */
	
	declare pointeur_evt cursor
	for select
		numero,
		cle,
		horodate,
		PR,
		sens,
		autoroute,
		type,
		numero_cause,
		cle_cause,
		type_cause
	from #LISTE_EVTS
		
	open pointeur_evt
	
	
	fetch pointeur_evt into	@vl_numero,
				@vl_cle,
				@vl_horodate,
				@vl_PR,
				@vl_sens,
				@vl_autoroute,
				@vl_type,
				@vl_numero_cause,
				@vl_cle_cause,
				@vl_type_cause
				
				
	while (@@sqlstatus = 0)
	begin
		
		/*B On ne recherche les mesures trafic des evts que si leur sens n'est pas inconnu */
		
		if (@vl_sens != XDC_SENS_INCONNU) 
		begin
			
			select @vl_portion = null
			select @vl_station = null
			select @vl_Q = null
			select @vl_V = null
			select @vl_T_O = null
			
			
			select
				@vl_portion = CFG..RES_POR.numero,
				@vl_station = CFG..RES_INF.station
			from
				CFG..RES_POR, CFG..RES_INF
			where
				/* condition sur la portion */
				CFG..RES_POR.autoroute = @vl_autoroute and
				CFG..RES_POR.PR_debut <= @vl_PR and
				CFG..RES_POR.PR_fin > @vl_PR and
				/* condition sur le reseau d'influence */
				CFG..RES_INF.PR_debut <= @vl_PR and
				CFG..RES_INF.PR_fin > @vl_PR and
				CFG..RES_INF.sens = @vl_sens and
				/* jointure entre RES_POR et RES_INF */
				CFG..RES_INF.portion = CFG..RES_POR.numero
				
				
			select
				@vl_Q = HIS..MES_TRA.Q,
				@vl_V = HIS..MES_TRA.V,
				@vl_T_O = HIS..MES_TRA.T_O
			from
				HIS..MES_TRA
			where
				HIS..MES_TRA.voie = XDC_RADT_CHAUSSEE and
				HIS..MES_TRA.horodate > dateadd(minute, -6, @vl_horodate) and
				HIS..MES_TRA.horodate <= @vl_horodate and
				HIS..MES_TRA.station = @vl_station and
				HIS..MES_TRA.sens = @vl_sens
			
				
		end	/* fin du if @sens = XDC_SENS_INCONNU */
		
		
		/*B recherche du nom de l'autoroute */
		
		select @vl_nom_autoroute = CFG..RES_AUT.nom from CFG..RES_AUT where CFG..RES_AUT.numero = @vl_autoroute
		
		
		/*B insertion des donnees trouvees dans la table #RESULTAT */
		
		insert #RESULTAT 	(numero, 
					cle, 
					type, 
					horodate, 
					PR, 
					sens, 
					autoroute, 
					numero_cause, 
					cle_cause, 
					type_cause, 
					station, 
					Q, 
					V, 
					T_O,
					tunnel)
					
		values (@vl_numero,
			@vl_cle,
			@vl_type,
			@vl_horodate,
			@vl_PR,
			@vl_sens,
			@vl_nom_autoroute,
			@vl_numero_cause,
			@vl_cle_cause,
			@vl_type_cause,
			@vl_station, 
			@vl_Q,
			@vl_V,
			@vl_T_O,
			XDC_FAUX
			)
		
		
		/*B On passe a l'evt suivant */
		
		fetch pointeur_evt into	@vl_numero,
					@vl_cle,
					@vl_horodate,
					@vl_PR,
					@vl_sens,
					@vl_autoroute,
					@vl_type,
					@vl_numero_cause,
					@vl_cle_cause,
					@vl_type_cause

		
	end	/* fin de la recherche des mesures trafic associees a chaque evt */


	close pointeur_evt	
	
			
end 	/* fin du traitement ds le cas ou la condition accidentogene est de type meteo */




/* ========================================================================= */
/*A TRAITEMENT DANS LE CAS OU LA CONDITION ACCIDENTOGENE EST DE TYPE TRAVAUX */
/* ========================================================================= */	

if @va_Type_condition_in = XDC_TYPE_CONDITION_TRAVAUX
begin

	/*A Insertion dans la table des evenements (sauf les bouchons!!!) qui sont la cause */
	/* d'evt travaux.	 */
	
	select
		numero = GEN1.numero,
		cle = GEN1.cle,
		type = CFG..TYP_FMC.nom,
		numero_cause = GEN2.numero,
		cle_cause = GEN2.cle,
		type_cause = GEN2.type
	into #EVTS_TRV
	from
		HIS..FMC_GEN GEN1, HIS..FMC_GEN GEN2, CFG..TYP_FMC
	where
		/* condition sur l'horodate : l'evt doit commencer ds la periode demandee et etre termine */
		GEN1.debut >= @va_Debut_in and
		GEN1.debut <= @va_Fin_in and
		/* condition sur le type de l'evt */
		(GEN1.type = @va_Num_evt_in or
		@va_Num_evt_in is null) and
		GEN1.type != XZAEC_FMC_QueueBouchon and
		GEN1.type != XZAEC_FMC_TeteBouchon and
		GEN1.datex<>XDC_VRAI and
		/* condition sur l'evt cause */
		GEN2.type = XZAEC_FMC_Travaux and
		GEN2.datex<>XDC_VRAI and
		/* jointure entre GEN1 et GEN2 */
		GEN1.cause = GEN2.numero and
		GEN1.cle_cause = GEN2.cle and
		/* jointure entre GEN1 et TYP_FMC */
		GEN1.type = CFG..TYP_FMC.numero

	
	/*A On ajoute la jointure avec FMC_HIS pour les evts autres que les bouchons */
	
	if (@va_Num_evt_in != XZAEC_FMC_TeteBouchon or @va_Num_evt_in != XZAEC_FMC_QueueBouchon)
	begin
	
		insert #LISTE_EVTS (numero, cle, type , horodate, PR, sens, autoroute, numero_cause, cle_cause, type_cause,tunnel)
		select
			ET.numero,
			ET.cle,
			ET.type,
			HIS..FMC_HIS.horodate_validation,
			HIS..FMC_HIS.PR,
			HIS..FMC_HIS.sens,
			HIS..FMC_HIS.autoroute,
			ET.numero_cause,
			ET.cle_cause,
			ET.type_cause,
			XDC_FAUX

		from	#EVTS_TRV ET, HIS..FMC_HIS
		where
			/* jointure entre les 2 */
			ET.numero = HIS..FMC_HIS.numero and
			ET.cle = HIS..FMC_HIS.cle
		group by
			HIS..FMC_HIS.numero,
			HIS..FMC_HIS.cle,
			ET.numero,
			ET.cle
		having
			horodate_validation = max(horodate_validation)
			
			
	
	end
	
		
		
	/*A Si on demande les evts de type bouchon on les ajoute a la table #LISTE_EVTS */
			 
	if (@va_Num_evt_in is null or @va_Num_evt_in = XZAEC_FMC_TeteBouchon or @va_Num_evt_in = XZAEC_FMC_QueueBouchon)
	begin

		/*A On retire tout de #EVTS_TRV pour pouvoir y mettre les bouchons */
		delete #EVTS_TRV
		
		
		/*A On insere les evts de type tete de bouchons */
		
		insert #EVTS_TRV (numero, cle, type, numero_cause, cle_cause, type_cause)
		select
			GEN1.numero,
			GEN1.cle,
			CFG..TYP_FMC.nom,
			GEN2.numero,
			GEN2.cle,
			GEN2.type
		from
			HIS..FMC_GEN GEN1, HIS..FMC_GEN GEN2, CFG..TYP_FMC
		where
			/* condition sur l'horodate : l'evt doit commencer ds la periode demandee et etre termine */
			GEN1.debut >= @va_Debut_in and
			GEN1.debut <= @va_Fin_in and
			/* condition sur le type de l'evt */
			GEN1.type = XZAEC_FMC_TeteBouchon and
			GEN1.datex<>XDC_VRAI and
			/* condition sur l'evt cause */
			GEN2.type = XZAEC_FMC_Travaux and
			GEN2.datex<>XDC_VRAI and
			/* jointure entre GEN1 et GEN2 */
			GEN1.cause = GEN2.numero and
			GEN1.cle_cause = GEN2.cle and
			/* jointure entre GEN1 et TYP_FMC */
			GEN1.type = CFG..TYP_FMC.numero
			
			
		/*A Et on fait la jointure avec FMC_HIS ds #LISTE_EVTS mais on prend la 1ere validation */
		
		insert #LISTE_EVTS (numero, cle, type , horodate, PR, sens, autoroute, numero_cause, cle_cause, type_cause,tunnel)
		select
			ET.numero,
			ET.cle,
			ET.type,
			HIS..FMC_HIS.horodate_validation,
			HIS..FMC_HIS.PR,
			HIS..FMC_HIS.sens,
			HIS..FMC_HIS.autoroute,
			ET.numero_cause,
			ET.cle_cause,
			ET.type_cause,
			XDC_FAUX
		from	#EVTS_TRV ET, HIS..FMC_HIS
		where
			/* jointure entre les 2 */
			ET.numero = HIS..FMC_HIS.numero and
			ET.cle = HIS..FMC_HIS.cle
		group by
			HIS..FMC_HIS.numero,
			HIS..FMC_HIS.cle,
			ET.numero,
			ET.cle
		having
			horodate_validation = min(horodate_validation)
		
	end
	
	
	/*A Recherche des debits associes a chaque evt trouve */
	
	declare pointeur_evt cursor
	for select
		numero,
		cle,
		horodate,
		PR,
		sens,
		autoroute,
		type,
		numero_cause,
		cle_cause,
		type_cause
	from	#LISTE_EVTS
		
	open pointeur_evt
	
	
	fetch pointeur_evt into	@vl_numero,
				@vl_cle,
				@vl_horodate,
				@vl_PR,
				@vl_sens,
				@vl_autoroute,
				@vl_type,
				@vl_numero_cause,
				@vl_cle_cause,
				@vl_type_cause
				
				
	while (@@sqlstatus = 0)
	begin
		
		/*B On ne recherche les mesures trafic des evts que si leur sens n'est pas inconnu */
		
		if (@vl_sens != XDC_SENS_INCONNU) 
		begin
			
			select @vl_portion = null
			select @vl_station = null
			select @vl_Q = null
			select @vl_V = null
			select @vl_T_O = null
			
			
			select
				@vl_portion = CFG..RES_POR.numero,
				@vl_station = CFG..RES_INF.station
			from
				CFG..RES_POR, CFG..RES_INF
			where
				/* condition sur la portion */
				CFG..RES_POR.autoroute = @vl_autoroute and
				CFG..RES_POR.PR_debut <= @vl_PR and
				CFG..RES_POR.PR_fin > @vl_PR and
				/* condition sur le reseau d'influence */
				CFG..RES_INF.PR_debut <= @vl_PR and
				CFG..RES_INF.PR_fin > @vl_PR and
				CFG..RES_INF.sens = @vl_sens and
				/* jointure entre RES_POR et RES_INF */
				CFG..RES_INF.portion = CFG..RES_POR.numero
				
				
			select
				@vl_Q = HIS..MES_TRA.Q,
				@vl_V = HIS..MES_TRA.V,
				@vl_T_O = HIS..MES_TRA.T_O
			from
				HIS..MES_TRA
			where
				HIS..MES_TRA.voie = XDC_RADT_CHAUSSEE and
				HIS..MES_TRA.horodate > dateadd(minute, -6, @vl_horodate) and
				HIS..MES_TRA.horodate <= @vl_horodate and
				HIS..MES_TRA.station = @vl_station and
				HIS..MES_TRA.sens = @vl_sens
			
				
		end	/* fin du if @sens = XDC_SENS_INCONNU */
		
		
		/*B recherche du nom de l'autoroute */
		
		select @vl_nom_autoroute = CFG..RES_AUT.nom from CFG..RES_AUT where CFG..RES_AUT.numero = @vl_autoroute
		
		
		/*B insertion des donnees trouvees dans la table #RESULTAT */
		
		insert #RESULTAT 	(numero, 
					cle, 
					type, 
					horodate, 
					PR, 
					sens, 
					autoroute, 
					numero_cause, 
					cle_cause, 
					type_cause, 
					station, 
					Q, 
					V, 
					T_O,tunnel)
					
		values (@vl_numero,
			@vl_cle,
			@vl_type,
			@vl_horodate,
			@vl_PR,
			@vl_sens,
			@vl_nom_autoroute,
			@vl_numero_cause,
			@vl_cle_cause,
			@vl_type_cause,
			@vl_station, 
			@vl_Q,
			@vl_V,
			@vl_T_O,
			XDC_FAUX)
		
		
		/*B On passe a l'evt suivant */
		
		fetch pointeur_evt into	@vl_numero,
					@vl_cle,
					@vl_horodate,
					@vl_PR,
					@vl_sens,
					@vl_autoroute,
					@vl_type,
					@vl_numero_cause,
					@vl_cle_cause,
					@vl_type_cause
		
	end	/* fin de la recherche des mesures trafic associees a chaque evt */
	
	close pointeur_evt
			
end 	/* fin du traitement ds le cas ou la condition accidentogene est de type travaux */


/* ========================================================================== */
/*A TRAITEMENT DANS LE CAS OU LA CONDITION ACCIDENTOGENE EST DE TYPE CHAUSSEE */
/* ========================================================================== */	

if @va_Type_condition_in = XDC_TYPE_CONDITION_CHAUSSEE
begin


	/* ---------------------------------------------------------------------- */
	/*A Si la condition accidentogene est : BAU < 2m OU NBRE de VOIES = SEUIL */
	/* ---------------------------------------------------------------------- */
	
	
	if (@va_Condition_in = XDC_CONDITION_CHAUSSEE_AVEC_BAU_ETROITE or @va_Condition_in = XDC_CONDITION_CHAUSSEE_AVEC_NB_VOIES)
	begin

		/*B Insertion des evenements (sauf les bouchons)!!! ds la table temporaire */
		/*B Les donnees de la table HIS sont prises a la derniere validation  */
		
		/* !!! Partie de code dupliquee ds la meme proc (ref du code duplique C001) */
		
		select
			numero = HIS..FMC_GEN.numero,
			cle = HIS..FMC_GEN.cle,
			type = CFG..TYP_FMC.nom,
			autoroute = HIS..FMC_HIS.autoroute,
			sens = HIS..FMC_HIS.sens,
			PR = HIS..FMC_HIS.PR,
			horodate = HIS..FMC_HIS.horodate_validation
		into #EVTS_BAU
		from HIS..FMC_HIS, HIS..FMC_GEN, CFG..TYP_FMC
		where
			/* condition sur le type des evts */
			(HIS..FMC_GEN.type = @va_Num_evt_in or @va_Num_evt_in is null) and
			HIS..FMC_GEN.type != XZAEC_FMC_TeteBouchon and
			HIS..FMC_GEN.type != XZAEC_FMC_QueueBouchon and
			/* tous les evts qui ont des PR non nuls */
			HIS..FMC_HIS.PR is not null and
			/* les evts qui sont en sens inconnu ne nous interessent pas */
			HIS..FMC_HIS.sens != XDC_SENS_INCONNU and
			/* condition sur les horodates */
			HIS..FMC_GEN.debut >= @va_Debut_in and
			HIS..FMC_GEN.debut <= @va_Fin_in and
			HIS..FMC_HIS.horodate_validation >= @va_Debut_in and
			HIS..FMC_HIS.horodate_validation <= @va_Fin_in and
			/* jointure entre FMC_GEN et TYP_FMC */
			HIS..FMC_GEN.type = CFG..TYP_FMC.numero and
			HIS..FMC_GEN.datex<>XDC_VRAI and
			/* jointure entre FMC_GEN et FMC_HIS */
			HIS..FMC_HIS.numero=HIS..FMC_GEN.numero and
			HIS..FMC_HIS.cle=HIS..FMC_GEN.cle
		group by
			HIS..FMC_HIS.numero,
			HIS..FMC_HIS.cle,
			HIS..FMC_GEN.numero,
			HIS..FMC_GEN.cle
		having
			horodate_validation = max(horodate_validation) and
			HIS..FMC_GEN.type = CFG..TYP_FMC.numero and
			HIS..FMC_GEN.datex<>XDC_VRAI and
			HIS..FMC_HIS.numero=HIS..FMC_GEN.numero and
			HIS..FMC_HIS.cle=HIS..FMC_GEN.cle
			
		/* !!! Fin de duplication de code (ref du code duplique C001) */


		/*B Si le type d'evt choisi est les bouchons, les inserer ds la table tempo */
		/*B Car pour un evt bouchon on prend le PR de tete du bouchon a la premiere validation */

		/* !!! Partie de code dupliquee ds la meme proc (ref du code duplique C002) */

		if (@va_Num_evt_in is null or @va_Num_evt_in = XZAEC_FMC_TeteBouchon or @va_Num_evt_in = XZAEC_FMC_QueueBouchon)
		begin

			insert #EVTS_BAU (numero, cle, type, autoroute, sens, PR, horodate)
			select
				HIS..FMC_GEN.numero,
				HIS..FMC_GEN.cle,
				CFG..TYP_FMC.nom,
				HIS..FMC_HIS.autoroute,
				HIS..FMC_HIS.sens,
				HIS..FMC_HIS.PR,
				HIS..FMC_HIS.horodate_validation
			from HIS..FMC_HIS, HIS..FMC_GEN, CFG..TYP_FMC
			where
				/* condition sur le type des evts */
				HIS..FMC_GEN.type = XZAEC_FMC_TeteBouchon and
				HIS..FMC_GEN.datex<>XDC_VRAI and
				/* tous les evts qui ont des PR non nuls */
				HIS..FMC_HIS.PR is not null and
				/* les bouchons qui sont en sens inconnu ne nous interessent pas */
				HIS..FMC_HIS.sens != XDC_SENS_INCONNU and
				/* condition sur les horodates */
				HIS..FMC_GEN.debut >= @va_Debut_in and
				HIS..FMC_GEN.debut <= @va_Fin_in and
				HIS..FMC_HIS.horodate_validation >= @va_Debut_in and
				HIS..FMC_HIS.horodate_validation <= @va_Fin_in and
				/* jointure entre FMC_GEN et TYP_FMC */
				HIS..FMC_GEN.type = CFG..TYP_FMC.numero and
				/* jointure entre FMC_GEN et FMC_HIS */
				HIS..FMC_HIS.numero=HIS..FMC_GEN.numero and
				HIS..FMC_HIS.cle=HIS..FMC_GEN.cle
			group by
				HIS..FMC_HIS.numero,
				HIS..FMC_HIS.cle,
				HIS..FMC_GEN.numero,
				HIS..FMC_GEN.cle
			having
				horodate_validation = min(horodate_validation) and
				HIS..FMC_GEN.type = CFG..TYP_FMC.numero and
				HIS..FMC_GEN.datex<>XDC_VRAI and
				HIS..FMC_HIS.numero=HIS..FMC_GEN.numero and
				HIS..FMC_HIS.cle=HIS..FMC_GEN.cle
				
				
		end
		
		/* !!! Fin de duplication de code (ref du code duplique C002) */


		/*B Si la condition est : BAU < 2m */
		
		if (@va_Condition_in = XDC_CONDITION_CHAUSSEE_AVEC_BAU_ETROITE)
		begin

			/*B Parmi les evenements trouves on met ds #LISTE_EVTS que ceux qui ont une BAU etroite */
		
			insert #LISTE_EVTS (numero, cle, type, horodate, PR, sens, autoroute, portion, BAU,tunnel)
			select
				EB.numero,
				EB.cle,
				EB.type,
				EB.horodate,
				EB.PR,
				EB.sens, 
				EB.autoroute,
				CFG..RES_POR.numero,
				CFG..RES_SEG.largeur_BAU,
				XDC_FAUX
			from	#EVTS_BAU EB, CFG..RES_POR, CFG..RES_SEG
			where
				/* condition sur RES_SEG : la BAU doit etre inferieure a 20dcm */
				CFG..RES_SEG.largeur_BAU < 20 and 
				/* jointure entre RES_POR et #EVENEMENT2 */
				CFG..RES_POR.autoroute = EB.autoroute and
				CFG..RES_POR.PR_debut <= EB.PR and
				CFG..RES_POR.PR_fin > EB.PR and
				/* jointure entre RES_SEG et RES_POR */
				CFG..RES_SEG.portion = CFG..RES_POR.numero and
				CFG..RES_SEG.PR_debut <= EB.PR and
				CFG..RES_SEG.PR_fin > EB.PR and
				CFG..RES_SEG.sens = EB.sens
			
		end	/*  fin de traitement pour bau <2m */
		
		
		
		/*B Si la condition est : NBRE DE VOIES EGALES AU SEUIL */
		
		if (@va_Condition_in = XDC_CONDITION_CHAUSSEE_AVEC_NB_VOIES)
		begin

			/*B Parmi les evenements trouves on met ds #LISTE_EVTS que ceux qui ont un nb de voies egal au seuil */
		
			insert #LISTE_EVTS (numero, cle, type, horodate, PR, sens, autoroute, portion, nb_voies,tunnel)
			select
				EB.numero,
				EB.cle,
				EB.type,
				EB.horodate,
				EB.PR,
				EB.sens, 
				EB.autoroute,
				CFG..RES_POR.numero,
				CFG..RES_SEG.nombre_de_voies,
				XDC_FAUX
			from	#EVTS_BAU EB, CFG..RES_POR, CFG..RES_SEG
			where
				/* condition sur RES_SEG : le nbre de voies est egal au seuil */
				CFG..RES_SEG.nombre_de_voies = @va_Nb_voies_in and 
				/* jointure entre RES_POR et #EVENEMENT2 */
				CFG..RES_POR.autoroute = EB.autoroute and
				CFG..RES_POR.PR_debut <= EB.PR and
				CFG..RES_POR.PR_fin > EB.PR and
				/* jointure entre RES_SEG et RES_POR */
				CFG..RES_SEG.portion = CFG..RES_POR.numero and
				CFG..RES_SEG.PR_debut <= EB.PR and
				CFG..RES_SEG.PR_fin > EB.PR and
				CFG..RES_SEG.sens = EB.sens
			
		end	/*  fin de traitement pour nbre de voies egal au seuil */
		

	end	/* fin du traitement si condition accidentogene est BAU etroite ou nbre de voies */
		
	
	
	/* ------------------------------------------------------------------- */
	/*A Si la condition accidentogene est : PENTE > SEUIL OU RAMPE < SEUIL */
	/* ------------------------------------------------------------------- */
	
	
	if (@va_Condition_in = XDC_CONDITION_CHAUSSEE_AVEC_PENTE or @va_Condition_in = XDC_CONDITION_CHAUSSEE_AVEC_RAMPE)
	begin

		/*B Insertion des evenements (sauf les bouchons)!!! ds la table temporaire */
		/*B Les donnees de la table HIS sont prises a la derniere validation  */
		

		select
			numero = HIS..FMC_GEN.numero,
			cle = HIS..FMC_GEN.cle,
			type = CFG..TYP_FMC.nom,
			autoroute = HIS..FMC_HIS.autoroute,
			sens = HIS..FMC_HIS.sens,
			PR = HIS..FMC_HIS.PR,
			horodate = HIS..FMC_HIS.horodate_validation
		into #EVTS_PNT
		from HIS..FMC_HIS, HIS..FMC_GEN, CFG..TYP_FMC
		where
			/* condition sur le type des evts */
			(HIS..FMC_GEN.type = @va_Num_evt_in or @va_Num_evt_in is null) and
			HIS..FMC_GEN.type != XZAEC_FMC_TeteBouchon and
			HIS..FMC_GEN.type != XZAEC_FMC_QueueBouchon and
			HIS..FMC_GEN.datex<>XDC_VRAI and
			/* tous les evts qui ont des PR non nuls */
			HIS..FMC_HIS.PR is not null and
			/* condition sur les horodates */
			HIS..FMC_GEN.debut >= @va_Debut_in and
			HIS..FMC_GEN.debut <= @va_Fin_in and
			HIS..FMC_HIS.horodate_validation >= @va_Debut_in and
			HIS..FMC_HIS.horodate_validation <= @va_Fin_in and
			/* jointure entre FMC_GEN et TYP_FMC */
			HIS..FMC_GEN.type = CFG..TYP_FMC.numero and
			/* jointure entre FMC_GEN et FMC_HIS */
			HIS..FMC_HIS.numero=HIS..FMC_GEN.numero and
			HIS..FMC_HIS.cle=HIS..FMC_GEN.cle
		group by
			HIS..FMC_HIS.numero,
			HIS..FMC_HIS.cle,
			HIS..FMC_GEN.numero,
			HIS..FMC_GEN.cle
		having
			horodate_validation = max(horodate_validation) and
			HIS..FMC_GEN.type = CFG..TYP_FMC.numero and
			HIS..FMC_GEN.datex<>XDC_VRAI and
			HIS..FMC_HIS.numero=HIS..FMC_GEN.numero and
			HIS..FMC_HIS.cle=HIS..FMC_GEN.cle
			


		/*B Si le type d'evt choisi est les bouchons, les inserer ds la table tempo */
		/*B Car pour un evt bouchon on prend le PR de tete du bouchon a la premiere validation */

		if (@va_Num_evt_in is null or @va_Num_evt_in = XZAEC_FMC_TeteBouchon or @va_Num_evt_in = XZAEC_FMC_QueueBouchon)
		begin

			insert #EVTS_PNT (numero, cle, type, autoroute, sens, PR, horodate)
			select
				HIS..FMC_GEN.numero,
				HIS..FMC_GEN.cle,
				CFG..TYP_FMC.nom,
				HIS..FMC_HIS.autoroute,
				HIS..FMC_HIS.sens,
				HIS..FMC_HIS.PR,
				HIS..FMC_HIS.horodate_validation
			from HIS..FMC_HIS, HIS..FMC_GEN, CFG..TYP_FMC
			where
				/* condition sur le type des evts */
				HIS..FMC_GEN.type = XZAEC_FMC_TeteBouchon and
				HIS..FMC_GEN.datex<>XDC_VRAI and
				/* tous les evts qui ont des PR non nuls */
				HIS..FMC_HIS.PR is not null and
				/* condition sur les horodates */
				HIS..FMC_GEN.debut >= @va_Debut_in and
				HIS..FMC_GEN.debut <= @va_Fin_in and
				HIS..FMC_HIS.horodate_validation >= @va_Debut_in and
				HIS..FMC_HIS.horodate_validation <= @va_Fin_in and
				/* jointure entre FMC_GEN et TYP_FMC */
				HIS..FMC_GEN.type = CFG..TYP_FMC.numero and
				/* jointure entre FMC_GEN et FMC_HIS */
				HIS..FMC_HIS.numero=HIS..FMC_GEN.numero and
				HIS..FMC_HIS.cle=HIS..FMC_GEN.cle
			group by
				HIS..FMC_HIS.numero,
				HIS..FMC_HIS.cle,
				HIS..FMC_GEN.numero,
				HIS..FMC_GEN.cle
			having
				horodate_validation = min(horodate_validation) and
				HIS..FMC_GEN.type = CFG..TYP_FMC.numero and
				HIS..FMC_GEN.datex<>XDC_VRAI and
				HIS..FMC_HIS.numero=HIS..FMC_GEN.numero and
				HIS..FMC_HIS.cle=HIS..FMC_GEN.cle
				
				
		end


		/*B Si la condition est : PENTE > SEUIL */
		/*  ----------------------------------- */
		
		if (@va_Condition_in = XDC_CONDITION_CHAUSSEE_AVEC_PENTE)
		begin

			/*B Parmi les evenements trouves on met ds #LISTE_EVTS que ceux qui ont une pente > au seuil */
		
			insert #LISTE_EVTS (numero, cle, type, horodate, PR, sens, autoroute, portion, pente,tunnel)
			select
				EP.numero,
				EP.cle,
				EP.type,
				EP.horodate,
				EP.PR,
				EP.sens, 
				EP.autoroute,
				CFG..RES_POR.numero,
				CFG..RES_PNT.pente,
				XDC_FAUX
			from	#EVTS_PNT EP, CFG..RES_POR, CFG..RES_PNT
			where
				/* condition sur RES_PNT : la pente doit etre superieure au seuil */
				CFG..RES_PNT.pente < 0 and
				abs(CFG..RES_PNT.pente) >= @va_Seuil_pente_in and 
				/* jointure entre RES_POR et #EVENEMENT2 */
				CFG..RES_POR.autoroute = EP.autoroute and
				CFG..RES_POR.PR_debut <= EP.PR and
				CFG..RES_POR.PR_fin > EP.PR and
				/* jointure entre RES_PNT et RES_POR */
				CFG..RES_PNT.portion = CFG..RES_POR.numero and
				CFG..RES_PNT.PR_debut <= EP.PR and
				CFG..RES_PNT.PR_fin > EP.PR
			
		end	/*  fin de traitement pour pente > seuil */
		
		
		
		/*B Si la condition est : RAMPE > SEUIL */
		/* ------------------------------------ */
		
		if (@va_Condition_in = XDC_CONDITION_CHAUSSEE_AVEC_RAMPE)
		begin

			/*B Parmi les evenements trouves on met ds #LISTE_EVTS que ceux qui ont uune rampe > au seuil */
		
			insert #LISTE_EVTS (numero, cle, type, horodate, PR, sens, autoroute, portion, pente,tunnel)
			select
				EP.numero,
				EP.cle,
				EP.type,
				EP.horodate,
				EP.PR,
				EP.sens, 
				EP.autoroute,
				CFG..RES_POR.numero,
				CFG..RES_PNT.pente,
				XDC_FAUX
			from	#EVTS_PNT EP, CFG..RES_POR, CFG..RES_PNT
			where
				/* condition sur RES_PNT : la rampe > au seuil */
				CFG..RES_PNT.pente >= @va_Seuil_pente_in and 
				/* jointure entre RES_POR et #EVENEMENT2 */
				CFG..RES_POR.autoroute = EP.autoroute and
				CFG..RES_POR.PR_debut <= EP.PR and
				CFG..RES_POR.PR_fin > EP.PR and
				/* jointure entre RES_PNT et RES_POR */
				CFG..RES_PNT.portion = CFG..RES_POR.numero and
				CFG..RES_PNT.PR_debut <= EP.PR and
				CFG..RES_PNT.PR_fin > EP.PR 
			
		end	/*  fin de traitement pour rampe > au seuil */
		

	end	/* fin du traitement si condition accidentogene est BAU etroite ou nbre de voies */
		
	
	
	/*B Recherche des debits associes a chaque evt trouve a l'aide d'un curseur */
	/* ------------------------------------------------------------------------ */
	
	/* !!! Partie de code dupliquee ds la meme proc (ref du code C003) */
	
	declare pointeur_evt cursor
	for select
		numero,
		cle,
		horodate,
		PR,
		sens,
		autoroute,
		type,
		portion
	from	#LISTE_EVTS
		
	open pointeur_evt
	
	
	fetch pointeur_evt into	@vl_numero,
				@vl_cle,
				@vl_horodate,
				@vl_PR,
				@vl_sens,
				@vl_autoroute,
				@vl_type,
				@vl_portion
				
				
	while (@@sqlstatus = 0)
	begin
		

		/*B On ne recherche les mesures trafic des evts que si leur sens n'est pas inconnu */
		
		if (@vl_sens != XDC_SENS_INCONNU) 
		begin
			
			select @vl_station = null
			select @vl_Q = null
			select @vl_V = null
			select @vl_T_O = null
			
			
			select
				@vl_station = CFG..RES_INF.station
			from
				CFG..RES_INF
			where
				/* condition sur le reseau d'influence */
				CFG..RES_INF.PR_debut <= @vl_PR and
				CFG..RES_INF.PR_fin > @vl_PR and
				CFG..RES_INF.sens = @vl_sens and
				CFG..RES_INF.portion = @vl_portion
				
				
			select
				@vl_Q = HIS..MES_TRA.Q,
				@vl_V = HIS..MES_TRA.V,
				@vl_T_O = HIS..MES_TRA.T_O
			from
				HIS..MES_TRA
			where
				HIS..MES_TRA.voie = XDC_RADT_CHAUSSEE and
				HIS..MES_TRA.horodate > dateadd(minute, -6, @vl_horodate) and
				HIS..MES_TRA.horodate <= @vl_horodate and
				HIS..MES_TRA.station = @vl_station and
				HIS..MES_TRA.sens = @vl_sens
					
					
		end	/* fin du if @sens = XDC_SENS_INCONNU */		
		
		
		/*B recherche du nom de l'autoroute */
		
		select @vl_nom_autoroute = CFG..RES_AUT.nom from CFG..RES_AUT where CFG..RES_AUT.numero = @vl_autoroute
		
		
		/*B insertion des donnees trouvees dans la table #RESULTAT */
		
		insert #RESULTAT 	(numero, 
					cle, 
					type, 
					horodate, 
					PR, 
					sens, 
					autoroute,
					portion,  
					station, 
					Q, 
					V, 
					T_O,
					tunnel)
					
		values (@vl_numero,
			@vl_cle,
			@vl_type,
			@vl_horodate,
			@vl_PR,
			@vl_sens,
			@vl_nom_autoroute,
			@vl_portion,
			@vl_station, 
			@vl_Q,
			@vl_V,
			@vl_T_O,
			XDC_FAUX)
		
		
		/*B On passe a l'evt suivant */
		
			
		fetch pointeur_evt into	@vl_numero,
					@vl_cle,
					@vl_horodate,
					@vl_PR,
					@vl_sens,
					@vl_autoroute,
					@vl_type,
					@vl_portion
		
	end	/* fin de la recherche des mesures trafic associees a chaque evt */
	
	close pointeur_evt
	
	/* !!! Fin de duplication du code (ref du code dupliquee C003) */	
	
end	/* fin du traitement ds le cas ou la condition accidentogene est de type chaussee */




/* ========================================================================== */
/*A TRAITEMENT DANS LE CAS OU LA CONDITION ACCIDENTOGENE EST DE TYPE OUVRAGE  */
/* ========================================================================== */	

if @va_Type_condition_in = XDC_TYPE_CONDITION_OUVRAGE
begin


	/* -------------------------------------------------------------------------- */
	/*A Si la condition accidentogene est : PRESENCE TUNNEL OU PRESENCE ECHANGEUR OU PRESENCE PEAGE */
	/* -------------------------------------------------------------------------- */
	
	
	if (@va_Condition_in = XDC_CONDITION_OUVRAGE_TUNNEL or @va_Condition_in = XDC_CONDITION_OUVRAGE_ECHANGEUR or @va_Condition_in = XDC_CONDITION_OUVRAGE_PEAGE)
	begin

		/*B Insertion des evenements (sauf les bouchons)!!! ds la table temporaire */
		/*B Les donnees de la table HIS sont prises a la derniere validation  */
		
		/* !!! Partie de code dupliquee ds la meme proc (ref du code duplique C001) */  

		select
			numero = HIS..FMC_GEN.numero,
			cle = HIS..FMC_GEN.cle,
			type = CFG..TYP_FMC.nom,
			autoroute = HIS..FMC_HIS.autoroute,
			sens = HIS..FMC_HIS.sens,
			PR = HIS..FMC_HIS.PR,
			horodate = HIS..FMC_HIS.horodate_validation,
			point_caracteristique = HIS..FMC_HIS.point_caracteristique
		into #EVTS_TUB
		from HIS..FMC_HIS, HIS..FMC_GEN, CFG..TYP_FMC
		where
			/* condition sur le type des evts */
			(HIS..FMC_GEN.type = @va_Num_evt_in or @va_Num_evt_in is null) and
			HIS..FMC_GEN.type != XZAEC_FMC_TeteBouchon and
			HIS..FMC_GEN.type != XZAEC_FMC_QueueBouchon and
			HIS..FMC_GEN.datex<>XDC_VRAI and
			/* tous les evts qui ont des PR non nuls */
			HIS..FMC_HIS.PR is not null and
			/* les evts qui sont en sens inconnu ne nous interessent pas */
			HIS..FMC_HIS.sens != XDC_SENS_INCONNU and
			/* condition sur les horodates */
			HIS..FMC_GEN.debut >= @va_Debut_in and
			HIS..FMC_GEN.debut <= @va_Fin_in and
			HIS..FMC_HIS.horodate_validation >= @va_Debut_in and
			HIS..FMC_HIS.horodate_validation <= @va_Fin_in and
			/* jointure entre FMC_GEN et TYP_FMC */
			HIS..FMC_GEN.type = CFG..TYP_FMC.numero and
			/* jointure entre FMC_GEN et FMC_HIS */
			HIS..FMC_HIS.numero=HIS..FMC_GEN.numero and
			HIS..FMC_HIS.cle=HIS..FMC_GEN.cle
		group by
			HIS..FMC_HIS.numero,
			HIS..FMC_HIS.cle,
			HIS..FMC_GEN.numero,
			HIS..FMC_GEN.cle
		having
			horodate_validation = max(horodate_validation) and
			HIS..FMC_GEN.type = CFG..TYP_FMC.numero and
			HIS..FMC_GEN.datex<>XDC_VRAI and
			HIS..FMC_HIS.numero=HIS..FMC_GEN.numero and
			HIS..FMC_HIS.cle=HIS..FMC_GEN.cle
			
			
		/* !!! Fin de duplication de code (ref du code duplique C001) */

		/*B Si le type d'evt choisi est les bouchons, les inserer ds la table tempo */
		/*B Car pour un evt bouchon on prend le PR de tete du bouchon a la premiere validation */
		
		/* !!! Partie de code dupliquee ds la meme proc (ref du code duplique C002) */

		if (@va_Num_evt_in is null or @va_Num_evt_in = XZAEC_FMC_TeteBouchon or @va_Num_evt_in = XZAEC_FMC_QueueBouchon)
		begin

			insert #EVTS_TUB (numero, cle, type, autoroute, sens, PR, horodate, point_caracteristique)
			select
				HIS..FMC_GEN.numero,
				HIS..FMC_GEN.cle,
				CFG..TYP_FMC.nom,
				HIS..FMC_HIS.autoroute,
				HIS..FMC_HIS.sens,
				HIS..FMC_HIS.PR,
				HIS..FMC_HIS.horodate_validation,
				HIS..FMC_HIS.point_caracteristique
			from HIS..FMC_HIS, HIS..FMC_GEN, CFG..TYP_FMC
			where
				/* condition sur le type des evts */
				HIS..FMC_GEN.type = XZAEC_FMC_TeteBouchon and
				HIS..FMC_GEN.datex<>XDC_VRAI and
				/* tous les evts qui ont des PR non nuls */
				HIS..FMC_HIS.PR is not null and
				/* les bouchons qui sont en sens inconnu ne nous interessent pas */
				HIS..FMC_HIS.sens != XDC_SENS_INCONNU and
				/* condition sur les horodates */
				HIS..FMC_GEN.debut >= @va_Debut_in and
				HIS..FMC_GEN.debut <= @va_Fin_in and
				HIS..FMC_HIS.horodate_validation >= @va_Debut_in and
				HIS..FMC_HIS.horodate_validation <= @va_Fin_in and
				/* jointure entre FMC_GEN et TYP_FMC */
				HIS..FMC_GEN.type = CFG..TYP_FMC.numero and
				/* jointure entre FMC_GEN et FMC_HIS */
				HIS..FMC_HIS.numero=HIS..FMC_GEN.numero and
				HIS..FMC_HIS.cle=HIS..FMC_GEN.cle
			group by
				HIS..FMC_HIS.numero,
				HIS..FMC_HIS.cle,
				HIS..FMC_GEN.numero,
				HIS..FMC_GEN.cle
			having
				horodate_validation = min(horodate_validation) and
				HIS..FMC_GEN.type = CFG..TYP_FMC.numero and
				HIS..FMC_GEN.datex<>XDC_VRAI and
				HIS..FMC_HIS.numero=HIS..FMC_GEN.numero and
				HIS..FMC_HIS.cle=HIS..FMC_GEN.cle
				
				
		end
		
		/* !!! Fin de duplication de code (ref du code duplique C002) */


		/*B Si la condition est : PRESENCE TUNNEL */
		
		if (@va_Condition_in = XDC_CONDITION_OUVRAGE_TUNNEL)
		begin

			/*B Parmi les evenements trouves on met ds #LISTE_EVTS que ceux qui sont sur un segment tunnel */
		
			insert #LISTE_EVTS (numero, cle, type, horodate, PR, sens, autoroute, portion, tunnel)
			select
				ET.numero,
				ET.cle,
				ET.type,
				ET.horodate,
				ET.PR,
				ET.sens, 
				ET.autoroute,
				CFG..RES_POR.numero,
				CFG..RES_SEG.tunnel
			from	#EVTS_TUB ET, CFG..RES_POR, CFG..RES_SEG
			where
				/* condition sur RES_SEG : le segment doit contenir un tunnel */
				CFG..RES_SEG.tunnel = 1 and 
				/* jointure entre RES_POR et #EVENEMENT2 */
				CFG..RES_POR.autoroute = ET.autoroute and
				CFG..RES_POR.PR_debut <= ET.PR and
				CFG..RES_POR.PR_fin > ET.PR and
				/* jointure entre RES_SEG et RES_POR */
				CFG..RES_SEG.portion = CFG..RES_POR.numero and
				CFG..RES_SEG.PR_debut <= ET.PR and
				CFG..RES_SEG.PR_fin > ET.PR and
				CFG..RES_SEG.sens = ET.sens
			
		end	/*  fin de traitement pour presence tunnel */
		

		
		/*B Si la condition est : PRESENCE ECHANGEUR */
		
		if (@va_Condition_in = XDC_CONDITION_OUVRAGE_ECHANGEUR)
		begin

			/*B Parmi les evenements trouves on met ds #LISTE_EVTS que ceux qui sont sur un echangeur */
					
			insert #LISTE_EVTS (numero, cle, type, horodate, PR, sens, autoroute, portion, echangeur,tunnel)
			select
				ET.numero,
				ET.cle,
				ET.type,
				ET.horodate,
				ET.PR,
				ET.sens, 
				ET.autoroute,
				CFG..RES_POR.numero,
				CFG..RES_ECH.numero,
				XDC_FAUX
			from	#EVTS_TUB ET, CFG..RES_POR, CFG..RES_ECH
			where
				ET.point_caracteristique = XDC_POINT_CARACT_ECHANGEUR and
				/* jointure entre RES_POR et #EVENEMENT2 */
				CFG..RES_POR.autoroute = ET.autoroute and
				CFG..RES_POR.PR_debut <= ET.PR and
				CFG..RES_POR.PR_fin > ET.PR and
				/* jointure entre RES_ECH et RES_POR */
				CFG..RES_ECH.portion = CFG..RES_POR.numero
								
							
		end	/*  fin de traitement pour presence echangeur */
	



		/*B Si la condition est : PRESENCE ECHANGEUR */

		if (@va_Condition_in = XDC_CONDITION_OUVRAGE_ECHANGEUR)

		begin

			/*B Parmi les evenements trouves on met ds #LISTE_EVTS que ceux qui sont sur un echangeur */

			insert #LISTE_EVTS (numero, cle, type, horodate, PR, sens, autoroute, portion,tunnel)
			select
				ET.numero,
				ET.cle,
				ET.type,
				ET.horodate,
				ET.PR,
				ET.sens,
				ET.autoroute,
				CFG..RES_POR.numero,
				XDC_FAUX
			from	#EVTS_TUB ET, CFG..RES_POR
			where
				ET.point_caracteristique = XDC_POINT_CARACT_PEAGE and
				/* jointure entre RES_POR et #EVENEMENT2 */
				CFG..RES_POR.autoroute = ET.autoroute and
				CFG..RES_POR.PR_debut <= ET.PR and
				CFG..RES_POR.PR_fin > ET.PR

		end /* fin de traitement pour presence peage */




	end	/* fin du traitement si condition accidentogene est presence tunnel ou presence echangeur */
		
	
	
	/* ---------------------------------------------------------------- */
	/*A Si la condition accidentogene est : presence d'un PI ou d'un PS */
	/* ---------------------------------------------------------------- */
	
	
	if (@va_Condition_in = XDC_CONDITION_OUVRAGE_PI or @va_Condition_in = XDC_CONDITION_OUVRAGE_PS)
	begin

		/*B Insertion des evenements (sauf les bouchons)!!! ds la table temporaire */
		/*B Les donnees de la table HIS sont prises a la derniere validation  */
		

		select
			numero = HIS..FMC_GEN.numero,
			cle = HIS..FMC_GEN.cle,
			type = CFG..TYP_FMC.nom,
			autoroute = HIS..FMC_HIS.autoroute,
			sens = HIS..FMC_HIS.sens,
			PR = HIS..FMC_HIS.PR,
			horodate = HIS..FMC_HIS.horodate_validation
		into #EVTS_PI
		from HIS..FMC_HIS, HIS..FMC_GEN, CFG..TYP_FMC
		where
			/* condition sur le type des evts */
			(HIS..FMC_GEN.type = @va_Num_evt_in or @va_Num_evt_in is null) and
			HIS..FMC_GEN.type != XZAEC_FMC_TeteBouchon and
			HIS..FMC_GEN.type != XZAEC_FMC_QueueBouchon and
			HIS..FMC_GEN.datex<>XDC_VRAI and
			/* tous les evts qui ont des PR non nuls */
			HIS..FMC_HIS.PR is not null and
			/* condition sur les horodates */
			HIS..FMC_GEN.debut >= @va_Debut_in and
			HIS..FMC_GEN.debut <= @va_Fin_in and
			HIS..FMC_HIS.horodate_validation >= @va_Debut_in and
			HIS..FMC_HIS.horodate_validation <= @va_Fin_in and
			/* jointure entre FMC_GEN et TYP_FMC */
			HIS..FMC_GEN.type = CFG..TYP_FMC.numero and
			/* jointure entre FMC_GEN et FMC_HIS */
			HIS..FMC_HIS.numero=HIS..FMC_GEN.numero and
			HIS..FMC_HIS.cle=HIS..FMC_GEN.cle
		group by
			HIS..FMC_HIS.numero,
			HIS..FMC_HIS.cle,
			HIS..FMC_GEN.numero,
			HIS..FMC_GEN.cle
		having
			horodate_validation = max(horodate_validation) and
			HIS..FMC_GEN.type = CFG..TYP_FMC.numero and
			HIS..FMC_GEN.datex<>XDC_VRAI and
			HIS..FMC_HIS.numero=HIS..FMC_GEN.numero and
			HIS..FMC_HIS.cle=HIS..FMC_GEN.cle
			


		/*B Si le type d'evt choisi est les bouchons, les inserer ds la table tempo */
		/*B Car pour un evt bouchon on prend le PR de tete du bouchon a la premiere validation */

		if (@va_Num_evt_in is null or @va_Num_evt_in = XZAEC_FMC_TeteBouchon or @va_Num_evt_in = XZAEC_FMC_QueueBouchon)
		begin

			insert #EVTS_PI (numero, cle, type, autoroute, sens, PR, horodate)
			select
				HIS..FMC_GEN.numero,
				HIS..FMC_GEN.cle,
				CFG..TYP_FMC.nom,
				HIS..FMC_HIS.autoroute,
				HIS..FMC_HIS.sens,
				HIS..FMC_HIS.PR,
				HIS..FMC_HIS.horodate_validation
			from HIS..FMC_HIS, HIS..FMC_GEN, CFG..TYP_FMC
			where
				/* condition sur le type des evts */
				HIS..FMC_GEN.type = XZAEC_FMC_TeteBouchon and
				HIS..FMC_GEN.datex<>XDC_VRAI and
				/* tous les evts qui ont des PR non nuls */
				HIS..FMC_HIS.PR is not null and
				/* condition sur les horodates */
				HIS..FMC_GEN.debut >= @va_Debut_in and
				HIS..FMC_GEN.debut <= @va_Fin_in and
				HIS..FMC_HIS.horodate_validation >= @va_Debut_in and
				HIS..FMC_HIS.horodate_validation <= @va_Fin_in and
				/* jointure entre FMC_GEN et TYP_FMC */
				HIS..FMC_GEN.type = CFG..TYP_FMC.numero and
				/* jointure entre FMC_GEN et FMC_HIS */
				HIS..FMC_HIS.numero=HIS..FMC_GEN.numero and
				HIS..FMC_HIS.cle=HIS..FMC_GEN.cle
			group by
				HIS..FMC_HIS.numero,
				HIS..FMC_HIS.cle,
				HIS..FMC_GEN.numero,
				HIS..FMC_GEN.cle
			having
				horodate_validation = min(horodate_validation) and
				HIS..FMC_GEN.type = CFG..TYP_FMC.numero and
				HIS..FMC_GEN.datex<>XDC_VRAI and
				HIS..FMC_HIS.numero=HIS..FMC_GEN.numero and
				HIS..FMC_HIS.cle=HIS..FMC_GEN.cle
				
				
		end


		/*B Si la condition est : PRESENCE Passage Inferieur */
		/*  ------------------------------------------------ */
		
		if (@va_Condition_in = XDC_CONDITION_OUVRAGE_PI)
		begin

			/*B Parmi les evenements trouves on met ds #LISTE_EVTS que ceux dont le PR du PI = PR de l'evt */
		
			insert #LISTE_EVTS (numero, cle, type, horodate, PR, sens, autoroute, portion, PI,tunnel)
			select
				EP.numero,
				EP.cle,
				EP.type,
				EP.horodate,
				EP.PR,
				EP.sens, 
				EP.autoroute,
				CFG..RES_POR.numero,
				CFG..RES_TRN.numero,
				XDC_FAUX
			from	#EVTS_PI EP, CFG..RES_POR, CFG..RES_TRN
			where
				/* condition sur RES_TRN : le reseau transvresal doit etre de type PI */
				CFG..RES_TRN.type = XDC_RES_PASSINF and 
				/* jointure entre RES_POR et #EVENEMENT2 */
				CFG..RES_POR.autoroute = EP.autoroute and
				CFG..RES_POR.PR_debut <= EP.PR and
				CFG..RES_POR.PR_fin > EP.PR and
				/* jointure entre RES_PNT et RES_POR */
				CFG..RES_TRN.portion = CFG..RES_POR.numero and
				CFG..RES_TRN.PR = EP.PR 
			
		end	/*  fin de traitement pour presence Passage Inferieur */
		
		
		
		/*B Si la condition est : PRESENCE Passage Superieur */
		/* ------------------------------------------------- */
		
		if (@va_Condition_in = XDC_CONDITION_OUVRAGE_PS)
		begin

			/*B Parmi les evenements trouves on met ds #LISTE_EVTS que ceux qui ont leur PR = PR du PS */
		
			insert #LISTE_EVTS (numero, cle, type, horodate, PR, sens, autoroute, portion, PS,tunnel)
			select
				EP.numero,
				EP.cle,
				EP.type,
				EP.horodate,
				EP.PR,
				EP.sens, 
				EP.autoroute,
				CFG..RES_POR.numero,
				CFG..RES_SUP.numero,
				XDC_FAUX
			from	#EVTS_PI EP, CFG..RES_POR, CFG..RES_SUP
			where 
				/* jointure entre RES_POR et #EVENEMENT2 */
				CFG..RES_POR.autoroute = EP.autoroute and
				CFG..RES_POR.PR_debut <= EP.PR and
				CFG..RES_POR.PR_fin > EP.PR and
				/* jointure entre RES_PNT et RES_POR */
				CFG..RES_SUP.portion = CFG..RES_POR.numero and
				CFG..RES_SUP.PR = EP.PR 
			
		end	/*  fin de traitement pour rampe > au seuil */
		

	end	/* fin du traitement si condition accidentogene est BAU etroite ou nbre de voies */
		
	
	
	/*B Recherche des debits associes a chaque evt trouve a l'aide d'un curseur */
	/* ------------------------------------------------------------------------ */

	/* !!! Partie de code dupliquee ds la meme proc (ref du code duplique C003) */
	
	declare pointeur_evt cursor
	for select
		numero,
		cle,
		horodate,
		PR,
		sens,
		autoroute,
		type,
		portion
	from #LISTE_EVTS
		
	open pointeur_evt
	
	
	fetch pointeur_evt into	@vl_numero,
				@vl_cle,
				@vl_horodate,
				@vl_PR,
				@vl_sens,
				@vl_autoroute,
				@vl_type,
				@vl_portion
				
				
	while (@@sqlstatus = 0)
	begin
		

		/*B On ne recherche les mesures trafic des evts que si leur sens n'est pas inconnu */
		
		if (@vl_sens != XDC_SENS_INCONNU) 
		begin
			
			select @vl_station = null
			select @vl_Q = null
			select @vl_V = null
			select @vl_T_O = null
			
			
			select
				@vl_station = CFG..RES_INF.station
			from
				CFG..RES_INF
			where
				/* condition sur le reseau d'influence */
				CFG..RES_INF.PR_debut <= @vl_PR and
				CFG..RES_INF.PR_fin > @vl_PR and
				CFG..RES_INF.sens = @vl_sens and
				CFG..RES_INF.portion = @vl_portion
				
				
			select
				@vl_Q = HIS..MES_TRA.Q,
				@vl_V = HIS..MES_TRA.V,
				@vl_T_O = HIS..MES_TRA.T_O
			from
				HIS..MES_TRA
			where
				HIS..MES_TRA.voie = XDC_RADT_CHAUSSEE and
				HIS..MES_TRA.horodate > dateadd(minute, -6, @vl_horodate) and
				HIS..MES_TRA.horodate <= @vl_horodate and
				HIS..MES_TRA.station = @vl_station and
				HIS..MES_TRA.sens = @vl_sens
					
					
		end	/* fin du if @sens = XDC_SENS_INCONNU */		
		
		
		/*B recherche du nom de l'autoroute */
		
		select @vl_nom_autoroute = CFG..RES_AUT.nom from CFG..RES_AUT where CFG..RES_AUT.numero = @vl_autoroute
		
		
		/*B insertion des donnees trouvees dans la table #RESULTAT */
		
		insert #RESULTAT 	(numero, 
					cle, 
					type, 
					horodate, 
					PR, 
					sens, 
					autoroute,
					portion,  
					station, 
					Q, 
					V, 
					T_O,
					tunnel)
					
		values (@vl_numero,
			@vl_cle,
			@vl_type,
			@vl_horodate,
			@vl_PR,
			@vl_sens,
			@vl_nom_autoroute,
			@vl_portion,
			@vl_station, 
			@vl_Q,
			@vl_V,
			@vl_T_O,
			XDC_FAUX)
		
		
		/*B On passe a l'evt suivant */
		
			
		fetch pointeur_evt into	@vl_numero,
					@vl_cle,
					@vl_horodate,
					@vl_PR,
					@vl_sens,
					@vl_autoroute,
					@vl_type,
					@vl_portion
		
	end	/* fin de la recherche des mesures trafic associees a chaque evt */
	
	close pointeur_evt
	
	/* !!! Fin de duplication du code (ref du code duplique C003) */
	
end	/* fin du traitement ds le cas ou la condition accidentogene est de type chaussee */
	

/*A retourner la liste */
	
select
	type,
	horodate = convert(char(10),horodate,103)+' '+convert(char(8),horodate,8),
	numero,
	PR,
	sens,
	autoroute,
	Q,
	V,
	T_O 
from #RESULTAT


return XDC_OK

go
