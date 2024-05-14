/*E*/
/*  Fichier : $Id: vuem21sp.prc,v 1.5 2010/09/14 10:00:32 gesconf Exp $        Release : $Revision: 1.5 $        Date : $Date: 2010/09/14 10:00:32 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER vuem21_sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* fiche metier dont l'objet est de connaitre les
* conditions accidentogenes par lineaire
*
------------------------------------------------------
* HISTORIQUE :
*
* L.V.	26/02/1997	: Creation V1.0
* L.V.	23/05/1997	: recherche sur tous les evts
*			et non plus sur un seul evt	V1.1
* P.N.	05/02/99	: Modif interg. dem/1724 v1.4
* JPL	05/02/99	: Prise en compte Fmc Veh. Marche arriere, Trait. Chaussees, Ech. Deconseille,
* 			  Chaussee glissante, Basculement pour Travaux, Zone de Stockage 1.5
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	VUEM21_RechercheParLineaire
* 
* Arguments en entree
* XDY_Horodate	va_Debut_in
* XDY_Horodate	va_Fin_in
* tinyint	va_Autoroute_in
* int		va_PR_debut_in
* int		va_PR_fin_in
* int		va_Type_lineaire_in  0->segment; 1->100m; 2->500m; 3->1000m
*					les types de lineaire sont definis dans xdc.h
* int		va_Nb_mini_evts_in
* la liste des evts du choix multiple
*
* Arguments en sortie
* char(5)	autoroute
* tinyint	sens
* int		PR_debut
* int		PR_fin
* int		nb_evts
* int		nb_voies_PR_debut
* int		nb_voies_PR_fin
* int		BAU_PR_debut
* tinyint	BAU_PR_fin
* smallint	pentes_PR_debut
* smallint	pentes_PR_fin
* smallint	rampes_PR_debut
* smallint	rampes_PR_fin
* bit		tunnel
* tinyint	echangeur
* tinyint	aire
* 
* Service rendu
* Recherche les conditions accidentogenes par lineaires
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

create procedure VUEM21
	@va_Debut_in		datetime	= null,
	@va_Fin_in		datetime	= null,
	@va_Autoroute_in	tinyint		= null,
	@va_PR_debut_in		int		= null,
	@va_PR_fin_in		int		= null,
	@va_Type_lineaire_in	int		= null,
	@va_Nb_mini_evts_in	int		= null,

	@va_Tous_les_evts	int             = XDC_FAUX,	
	@va_Accident		int             = XDC_FAUX,
	@va_Delestage		int             = XDC_FAUX,
	@va_Deviation		int             = XDC_FAUX,
	@va_PosteOperateur	int             = XDC_FAUX,
	@va_Meteo		int             = XDC_FAUX,
	@va_Contresens		int		= XDC_FAUX,
	@va_VehArrete		int             = XDC_FAUX,
	@va_VehPanne		int             = XDC_FAUX,
	@va_VehFeu		int             = XDC_FAUX,
	@va_Cirque		int             = XDC_FAUX,
	@va_ConvoiExc		int             = XDC_FAUX,
	@va_ConvoiMil		int             = XDC_FAUX,
	@va_VehLent		int             = XDC_FAUX,
	@va_Manifestation	int             = XDC_FAUX,
	@va_GreveESCOTA		int             = XDC_FAUX,
	@va_Escargot		int             = XDC_FAUX,
	@va_Basculement		int             = XDC_FAUX,
	@va_Travaux		int             = XDC_FAUX,
	@va_IncSsConces		int             = XDC_FAUX,
	@va_QueueBouchon	int             = XDC_FAUX,
	@va_FermetureEch	int             = XDC_FAUX,
	@va_Eboulement		int             = XDC_FAUX,
	@va_Incendie		int             = XDC_FAUX,
	@va_Inondation		int             = XDC_FAUX,
	@va_Obstacle		int             = XDC_FAUX,
	@va_ChausseDeter	int             = XDC_FAUX,
	@va_FlaqueHuile		int             = XDC_FAUX,
	@va_AnimalErrant	int             = XDC_FAUX,
	@va_FrontiereFermee	int             = XDC_FAUX,
	@va_AltercationUsager	int             = XDC_FAUX,
	@va_Agression		int             = XDC_FAUX,
	@va_MalaiseUsager	int             = XDC_FAUX,
	@va_EvtHorsReseau	int             = XDC_FAUX,
	@va_AireFermee		int		= XDC_FAUX,
	@va_Salage		int             = XDC_FAUX,
	@va_HoldUpVol		int             = XDC_FAUX,
	@va_TeteBouchon		int             = XDC_FAUX,
	@va_PietonChaussee	int             = XDC_FAUX,
	@va_Autre		int             = XDC_FAUX,
	@va_VehMarcheArr	int             = XDC_FAUX,
	@va_TraitChaussees	int             = XDC_FAUX,
	@va_EchDeconseille	int             = XDC_FAUX,
	@va_ChausseeGliss	int             = XDC_FAUX,
	@va_BasculTravaux	int             = XDC_FAUX,
	@va_ZoneStockage	int             = XDC_FAUX
as
	declare
		@vl_Autoroute char(5),
		@vl_Sens tinyint,
		@vl_PR_debut int,
		@vl_PR_fin int,
		@vl_Nb_evts int,
		@vl_Nb_voies_PR_debut int,
		@vl_Nb_voies_PR_fin int,
		@vl_BAU_PR_debut tinyint,
		@vl_BAU_PR_fin tinyint,
		@vl_PNT_debut smallint,
		@vl_PNT_fin smallint,
		@vl_Pentes_PR_debut smallint,
		@vl_Pentes_PR_fin smallint,
		@vl_Rampes_PR_debut smallint,
		@vl_Rampes_PR_fin smallint,
		@vl_Tunnel bit,
		@vl_Tunnel_debut tinyint,
		@vl_Tunnel_fin tinyint,
		@vl_Echangeur int,
		@vl_Aire int,
		@vl_intervalle_PR int,
		@vl_PR_courant_debut int,
		@vl_PR_courant_fin int,
		@vl_Nombre_de_voies int,
		@vl_Portion smallint,
		@vl_Largeur_BAU tinyint



/*A controle des parametres d'entree */
if @va_Debut_in is null
	return XDC_ARG_INV
		
if @va_Fin_in is null
	return XDC_ARG_INV
		
if @va_Autoroute_in is null
	return XDC_ARG_INV

if @va_PR_debut_in is null
	return XDC_ARG_INV

if @va_PR_fin_in is null
	return XDC_ARG_INV
		
if @va_Type_lineaire_in is null
	return XDC_ARG_INV
		
if @va_Nb_mini_evts_in is null
	return XDC_ARG_INV
		

/*A Creation de la table resultat */	
create table #ACCIDENTOGENE
			(autoroute char(5) null,
			sens tinyint null,
			PR_debut int not null,
			PR_fin int not null,
			nb_evts int not null,
			nb_voies_PR_debut int null,
			nb_voies_PR_fin int null,
			BAU_PR_debut tinyint null,
			BAU_PR_fin tinyint null,
			pentes_PR_debut smallint null,
			pentes_PR_fin smallint null,
			rampes_PR_debut smallint null,
			rampes_PR_fin smallint null,
			tunnel bit,
			echangeur tinyint null,
			aire tinyint null
			)

/*A Creation de la table temporaire contenant la liste des evts par lineaire */
create table #TABLE_EVT
		(numero int not null, 
		cle int not null, 
		type smallint, 
		autoroute tinyint null, 
		sens tinyint null, 
		PR int null, 
		horodate datetime null)

/*A Creation de la table temporaire contenant la liste des evts du choix multiple */
create table #EVT_SELECT
		(numero int not null)


/* recherche du libelle de l'autoroute */
select @vl_Autoroute = CFG..RES_AUT.nom from CFG..RES_AUT where CFG..RES_AUT.numero = @va_Autoroute_in			
					 

/* Si pas selection de tous les evts */
/* insertion des evts selectionnes dans la table des evts sauf les bouchons */
if @va_Tous_les_evts = XDC_FAUX
begin

	if @va_Accident = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_Accident)

	if @va_Delestage = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_Delestage)

	if @va_Deviation = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_Deviation)

	if @va_Meteo = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_Meteo)

	if @va_Contresens = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_Contresens)

	if @va_VehArrete = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_VehArrete)

	if @va_VehPanne = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_VehPanne)

	if @va_VehFeu = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_VehFeu)

	if @va_Cirque  = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_Cirque)

	if @va_ConvoiExc = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_ConvoiExc)

	if @va_ConvoiMil = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_ConvoiMil)

	if @va_VehLent = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_VehLent)

	if @va_Manifestation = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_Manifestation)

	if @va_GreveESCOTA = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_GreveESCOTA)

	if @va_Escargot = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_Escargot)

	if @va_Basculement = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_Basculement)

	if @va_Travaux = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_Travaux)

	if @va_IncSsConces = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_IncSsConces)

	if @va_FermetureEch = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_FermetureEch)

	if @va_Eboulement = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_Eboulement)

	if @va_Incendie = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_Incendie)

	if @va_Inondation = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_Inondation)

	if @va_Obstacle = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_Obstacle)

	if @va_ChausseDeter = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_ChausseDeter)

	if @va_FlaqueHuile = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_FlaqueHuile)

	if @va_AnimalErrant = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_AnimalErrant)

	if @va_FrontiereFermee = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_FrontiereFermee)

	if @va_AltercationUsager = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_AltercationUsager)

	if @va_Agression = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_Agression)

	if @va_MalaiseUsager = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_MalaiseUsager)

	if @va_EvtHorsReseau = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_EvtHorsReseau)

	if @va_AireFermee = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_AireFermee)

	if @va_Salage = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_Salage)

	if @va_HoldUpVol = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_HoldUpVol)

	if @va_PietonChaussee = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_PietonChaussee)

	if @va_Autre = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_Autre)

	if @va_VehMarcheArr = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_VehMarcheArr)

	if @va_TraitChaussees = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_TraitChaussees)

	if @va_EchDeconseille = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_EchDeconseille)

	if @va_ChausseeGliss = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_ChausseeGliss)

	if @va_BasculTravaux = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_BasculTravaux)

	if @va_ZoneStockage = XDC_VRAI
		insert #EVT_SELECT (numero)
		values (XZAEC_FMC_ZoneStockage)

end


/* ============================================================= */
/*A TRAITEMENT DANS LE CAS OU LE TYPE DE LINEAIRE EST UN SEGMENT */
/* ============================================================= */	

if @va_Type_lineaire_in = XDC_LINEAIRE_SEGMENT
begin

	/*A Creation d'un curseur qui parcourt tous les segments appatenant a la zone demandee */
	
	declare pointeur_segment cursor
	for select
		CFG..RES_SEG.portion,
		CFG..RES_SEG.PR_debut,
		CFG..RES_SEG.PR_fin,
		CFG..RES_SEG.sens,
		CFG..RES_SEG.nombre_de_voies,
		CFG..RES_SEG.largeur_BAU,
		CFG..RES_SEG.tunnel
	from
		CFG..RES_SEG, CFG..RES_POR
	where
		CFG..RES_SEG.PR_debut < @va_PR_fin_in and
		CFG..RES_SEG.PR_fin > @va_PR_debut_in and
		CFG..RES_POR.autoroute = @va_Autoroute_in and
		CFG..RES_POR.PR_debut < @va_PR_fin_in and
		CFG..RES_POR.PR_fin > @va_PR_debut_in and
		CFG..RES_POR.numero = CFG..RES_SEG.portion
		
	open pointeur_segment
	
	fetch pointeur_segment into	@vl_Portion,
					@vl_PR_debut,
					@vl_PR_fin,
					@vl_Sens,
					@vl_Nombre_de_voies,
					@vl_Largeur_BAU,
					@vl_Tunnel
					
	/*A On parcourt les segments un a un */
	
	while (@@sqlstatus = 0)
	begin
		
		/*B Insertion des evenements (sauf les bouchons)!!! ds la table temporaire */
		/*B Un evt autre qu'un bouchon fait partie du segment si le PR de l'evt   */
		/*B a la derniere validation appartient au segment 			   */
			
		insert #TABLE_EVT (numero, cle, type, autoroute, sens, PR, horodate)
		select
			HIS..FMC_GEN.numero,
			HIS..FMC_GEN.cle,
			HIS..FMC_GEN.type,
			HIS..FMC_HIS.autoroute,
			HIS..FMC_HIS.sens,
			HIS..FMC_HIS.PR,
			HIS..FMC_HIS.horodate_validation
		from HIS..FMC_HIS, HIS..FMC_GEN
		where
			/* condition sur le type des evts */
			(HIS..FMC_GEN.type in (select numero from #EVT_SELECT) 
			or @va_Tous_les_evts = XDC_VRAI) and
			HIS..FMC_GEN.type != XZAEC_FMC_TeteBouchon and
			HIS..FMC_GEN.type != XZAEC_FMC_QueueBouchon and
			HIS..FMC_GEN.datex<>XDC_VRAI and
			/* condition sur les horodates */
			HIS..FMC_GEN.debut >= @va_Debut_in and
			HIS..FMC_GEN.debut <= @va_Fin_in and
			/* jointure entre FMC_GEN et FMC_HIS */
			HIS..FMC_HIS.numero=HIS..FMC_GEN.numero and
			HIS..FMC_HIS.cle=HIS..FMC_GEN.cle and
			/* condition sur les PR, le sens et l'autoroute */
			HIS..FMC_HIS.autoroute = @va_Autoroute_in and
			HIS..FMC_HIS.PR >=  @vl_PR_debut and
			HIS..FMC_HIS.PR < @vl_PR_fin and
			HIS..FMC_HIS.sens = @vl_Sens
		group by
			HIS..FMC_HIS.numero, 
			HIS..FMC_HIS.cle, 
			HIS..FMC_GEN.numero, 
			HIS..FMC_GEN.cle
		having 
			horodate_validation = max(horodate_validation) and
			HIS..FMC_HIS.autoroute = @va_Autoroute_in and
			HIS..FMC_HIS.PR >=  @vl_PR_debut and
			HIS..FMC_HIS.PR < @vl_PR_fin and
			HIS..FMC_HIS.sens = @vl_Sens
				
				
		/*B Si la liste des evts contient les bouchons les inserer ds la table tempo */
		/*B Car un evt bouchon fait partie du segment si le PR de tete du bouchon */
		/*B a la premiere validation appartient au segment 			*/
			 
		if (@va_Tous_les_evts = XDC_VRAI or @va_QueueBouchon = XDC_VRAI or @va_TeteBouchon = XDC_VRAI)
		begin

			insert #TABLE_EVT (numero, cle, type, autoroute, sens, PR, horodate)
			select
				HIS..FMC_GEN.numero,
				HIS..FMC_GEN.cle,
				HIS..FMC_GEN.type,
				HIS..FMC_HIS.autoroute,
				HIS..FMC_HIS.sens,
				HIS..FMC_HIS.PR,
				HIS..FMC_HIS.horodate_validation
			from HIS..FMC_HIS, HIS..FMC_GEN
			where
				/* condition sur le type des evts */
				HIS..FMC_GEN.type = XZAEC_FMC_TeteBouchon and
				HIS..FMC_GEN.datex<>XDC_VRAI and
				/* condition sur les horodates */
				HIS..FMC_GEN.debut >= @va_Debut_in and
				HIS..FMC_GEN.debut <= @va_Fin_in and
				/* jointure entre FMC_GEN et FMC_HIS */
				HIS..FMC_HIS.numero=HIS..FMC_GEN.numero and
				HIS..FMC_HIS.cle=HIS..FMC_GEN.cle and
				/* condition sur les PR, le sens et l'autoroute */
				HIS..FMC_HIS.autoroute = @va_Autoroute_in and
				HIS..FMC_HIS.PR >=  @vl_PR_debut and
				HIS..FMC_HIS.PR < @vl_PR_fin and
				HIS..FMC_HIS.sens = @vl_Sens
			group by
				HIS..FMC_HIS.numero, 
				HIS..FMC_HIS.cle, 
				HIS..FMC_GEN.numero, 
				HIS..FMC_GEN.cle
			having 
				horodate_validation = min(horodate_validation) and
				HIS..FMC_HIS.autoroute = @va_Autoroute_in and
				HIS..FMC_HIS.PR >= @vl_PR_debut and
				HIS..FMC_HIS.PR < @vl_PR_fin and
				HIS..FMC_HIS.sens = @vl_Sens
				
		end			


		/*B On compte le nombre d'evt sur le segment et on retire les donnees de la table tempo */
			
		select @vl_Nb_evts = count(*) from #TABLE_EVT
			
		delete #TABLE_EVT
								
			
		/*A Si le nombre d'evts comptabilises est inferieur au seuil : le segment courant ne nous interesse pas !! */
			
		if (@vl_Nb_evts >= @va_Nb_mini_evts_in)
		begin
			
			select @vl_Echangeur = null	
			select @vl_Aire = null
			select @vl_PNT_debut = null
			select @vl_PNT_fin = null
			select @vl_Nb_voies_PR_debut = @vl_Nombre_de_voies
			select @vl_BAU_PR_debut = @vl_Largeur_BAU
			select @vl_Nb_voies_PR_fin = @vl_Nombre_de_voies
			select @vl_BAU_PR_fin = @vl_Largeur_BAU
			select @vl_Pentes_PR_debut = null
			select @vl_Pentes_PR_fin = null
			select @vl_Rampes_PR_debut = null
			select @vl_Rampes_PR_fin = null
			
		
			/*B Recherche des pentes au PR de debut du segment */	
			select 
				@vl_PNT_debut = CFG..RES_PNT.pente
			from CFG..RES_PNT
			where
				CFG..RES_PNT.portion = @vl_Portion and
				CFG..RES_PNT.PR_debut <= @vl_PR_debut and
				CFG..RES_PNT.PR_fin > @vl_PR_debut
					
		
			/*B Recherche d'un echangeur (ici ce sera le dernier) sur le segment */			
			select 
				@vl_Echangeur = CFG..RES_ECH.numero
			from CFG..RES_ECH
			where
				CFG..RES_ECH.portion = @vl_Portion and
				CFG..RES_ECH.PR >= @vl_PR_debut and
				CFG..RES_ECH.PR <= @vl_PR_fin
					
		
			/*B Recherche d'une entree d'air sur le segment */		
			select 
				@vl_Aire = CFG..RES_AIR.numero
			from CFG..RES_AIR
			where
				CFG..RES_AIR.portion = @vl_Portion and
				CFG..RES_AIR.sens = @vl_Sens and
				CFG..RES_AIR.PRentree >= @vl_PR_debut and
				CFG..RES_AIR.PRentree <= @vl_PR_fin
			
			
			/*B Recherche des pentes au PR de fin du segment */
			select
				@vl_PNT_fin = CFG..RES_PNT.pente
			from CFG..RES_PNT
			where
				CFG..RES_PNT.portion = @vl_Portion and
				CFG..RES_PNT.PR_debut < @vl_PR_fin and
				CFG..RES_PNT.PR_fin >= @vl_PR_fin
					
		
			/*B Recherche d'une sortie d'air sur le segment */			
			select
				@vl_Aire = CFG..RES_AIR.numero
			from CFG..RES_AIR
			where
				CFG..RES_AIR.portion = @vl_Portion and
				CFG..RES_AIR.sens = @vl_Sens and
				CFG..RES_AIR.PRsortie >= @vl_PR_debut and
				CFG..RES_AIR.PRsortie <= @vl_PR_fin
				
				
			if (@vl_Echangeur is not null) select @vl_Echangeur = 1 else select @vl_Echangeur = 0
				
			if (@vl_Aire is not null) select @vl_Aire = 1 else select @vl_Aire = 0
				
			if (@vl_PNT_debut > 0) select @vl_Rampes_PR_debut = @vl_PNT_debut
			if (@vl_PNT_fin > 0) select @vl_Rampes_PR_fin = @vl_PNT_fin
			if (@vl_PNT_debut < 0) select @vl_Pentes_PR_debut = abs(@vl_PNT_debut)
			if (@vl_PNT_fin < 0) select @vl_Pentes_PR_fin = abs(@vl_PNT_fin)
				
				
			/*B On insere toutes les donnees trouvees dans la table resultat #ACCIDENTOGENE */
				
			insert #ACCIDENTOGENE 
				(autoroute,
				sens,
				PR_debut,
				PR_fin,
				nb_evts,
				nb_voies_PR_debut,
				nb_voies_PR_fin,
				BAU_PR_debut,
				BAU_PR_fin,
				pentes_PR_debut,
				pentes_PR_fin,
				rampes_PR_debut,
				rampes_PR_fin,
				tunnel,
				echangeur,
				aire)
						
			values (@vl_Autoroute,
				@vl_Sens,
				@vl_PR_debut,
				@vl_PR_fin,
				@vl_Nb_evts,
				@vl_Nb_voies_PR_debut,
				@vl_Nb_voies_PR_fin,
				@vl_BAU_PR_debut,
				@vl_BAU_PR_fin,
				@vl_Pentes_PR_debut,
				@vl_Pentes_PR_fin,
				@vl_Rampes_PR_debut,
				@vl_Rampes_PR_fin,
				@vl_Tunnel,
				@vl_Echangeur,
				@vl_Aire)
		
			
		end	/* fin de recherche des conditions accidentogenes ds le cas ou il y avait assez d'evts */
		
	
	fetch pointeur_segment into	@vl_Portion,
					@vl_PR_debut,
					@vl_PR_fin,
					@vl_Sens,
					@vl_Nombre_de_voies,
					@vl_Largeur_BAU,
					@vl_Tunnel
	
	
	end	/* fin du parcourt de chaque segment */
	
	close pointeur_segment
					
			
end 	/* fin du traitement ds le cas ou le lineaire est un segment */



/* ====================================================================== */	
/*A TRAITEMENT DANS LE CAS OU LE TYPE DE LINEAIRE EST 100m, 500m ou 1000m */
/* ====================================================================== */
	
if @va_Type_lineaire_in = XDC_LINEAIRE_100m or 	@va_Type_lineaire_in = XDC_LINEAIRE_500m or @va_Type_lineaire_in = XDC_LINEAIRE_1000m
begin
	
	/*A Initialisation de l'intervalle entre le PR_debut et le PR_fin de chaque lineaire */
		
	if @va_Type_lineaire_in = XDC_LINEAIRE_100m select @vl_intervalle_PR = 100
	if @va_Type_lineaire_in = XDC_LINEAIRE_500m select @vl_intervalle_PR = 500
	if @va_Type_lineaire_in = XDC_LINEAIRE_1000m select @vl_intervalle_PR = 1000
		
		
	/*A Initialisation du PR courant */
		
	select @vl_PR_courant_debut = @va_PR_debut_in
	select @vl_PR_courant_fin = @vl_PR_courant_debut + @vl_intervalle_PR
		
		
	/*A Parcourt de la zone de recherche lineaire par lineaire */
		
	while (@vl_PR_courant_debut < @va_PR_fin_in)
	begin
			
		/*A Sur un lineaire, on recherche les evts ds le sens SUD puis  ds le sens NORD */
		
		select @vl_Sens = XDC_SENS_SUD
		
		while (@vl_Sens <= XDC_SENS_NORD)
		begin
		
			/*B Insertion des evenements (sauf les bouchons)!!! ds la table temporaire */
			/*B Un evt autre qu'un bouchon fait partie du lineaire si le PR de l'evt   */
			/*B a la derniere validation appartient au lineaire 			   */
			
			insert #TABLE_EVT (numero, cle, type, autoroute, sens, PR, horodate)
			select
				HIS..FMC_GEN.numero,
				HIS..FMC_GEN.cle,
				HIS..FMC_GEN.type,
				HIS..FMC_HIS.autoroute,
				HIS..FMC_HIS.sens,
				HIS..FMC_HIS.PR,
				HIS..FMC_HIS.horodate_validation
			from HIS..FMC_HIS, HIS..FMC_GEN
			where
				/* condition sur le type des evts */
				(HIS..FMC_GEN.type in (select numero from #EVT_SELECT) or @va_Tous_les_evts = XDC_VRAI) and
				HIS..FMC_GEN.type != XZAEC_FMC_TeteBouchon and
				HIS..FMC_GEN.type != XZAEC_FMC_QueueBouchon and
				HIS..FMC_GEN.datex<>XDC_VRAI and
				/* condition sur les horodates */
				HIS..FMC_GEN.debut >= @va_Debut_in and
				HIS..FMC_GEN.debut <= @va_Fin_in and
				/* jointure entre FMC_GEN et FMC_HIS */
				HIS..FMC_HIS.numero=HIS..FMC_GEN.numero and
				HIS..FMC_HIS.cle=HIS..FMC_GEN.cle and
				/* condition sur les PR, le sens et l'autoroute */
				HIS..FMC_HIS.autoroute = @va_Autoroute_in and
				HIS..FMC_HIS.PR >=  @vl_PR_courant_debut and
				HIS..FMC_HIS.PR < @vl_PR_courant_fin and
				HIS..FMC_HIS.sens = @vl_Sens
			group by
				HIS..FMC_HIS.numero, 
				HIS..FMC_HIS.cle, 
				HIS..FMC_GEN.numero, 
				HIS..FMC_GEN.cle
			having 
				horodate_validation = max(horodate_validation) and
				HIS..FMC_HIS.autoroute = @va_Autoroute_in and
				HIS..FMC_HIS.PR >=  @vl_PR_courant_debut and
				HIS..FMC_HIS.PR < @vl_PR_courant_fin and
				HIS..FMC_HIS.sens = @vl_Sens
				
				
			/*B Si la liste des evts contient les bouchons les inserer ds la table tempo */
			/*B Car un evt bouchon fait partie du lineaire si le PR de tete du bouchon */
			/*B a la premiere validation appartient au lineaire 			*/
			 
			if (@va_Tous_les_evts = XDC_VRAI or @va_QueueBouchon = XDC_VRAI or @va_TeteBouchon = XDC_VRAI)
			begin

				insert #TABLE_EVT (numero, cle, type, autoroute, sens, PR, horodate)
				select
					HIS..FMC_GEN.numero,
					HIS..FMC_GEN.cle,
					HIS..FMC_GEN.type,
					HIS..FMC_HIS.autoroute,
					HIS..FMC_HIS.sens,
					HIS..FMC_HIS.PR,
					HIS..FMC_HIS.horodate_validation
				from HIS..FMC_HIS, HIS..FMC_GEN
				where
					/* condition sur le type des evts */
					HIS..FMC_GEN.type = XZAEC_FMC_TeteBouchon and
					HIS..FMC_GEN.datex<>XDC_VRAI and
					/* condition sur les horodates */
					HIS..FMC_GEN.debut >= @va_Debut_in and
					HIS..FMC_GEN.debut <= @va_Fin_in and
					/* jointure entre FMC_GEN et FMC_HIS */
					HIS..FMC_HIS.numero=HIS..FMC_GEN.numero and
					HIS..FMC_HIS.cle=HIS..FMC_GEN.cle and
					/* condition sur les PR, le sens et l'autoroute */
					HIS..FMC_HIS.autoroute = @va_Autoroute_in and
					HIS..FMC_HIS.PR >=  @vl_PR_courant_debut and
					HIS..FMC_HIS.PR < @vl_PR_courant_fin and
					HIS..FMC_HIS.sens = @vl_Sens
				group by
					HIS..FMC_HIS.numero, 
					HIS..FMC_HIS.cle, 
					HIS..FMC_GEN.numero, 
					HIS..FMC_GEN.cle
				having 
					horodate_validation = min(horodate_validation) and
					HIS..FMC_HIS.autoroute = @va_Autoroute_in and
					HIS..FMC_HIS.PR >=  @vl_PR_courant_debut and
					HIS..FMC_HIS.PR < @vl_PR_courant_fin and
					HIS..FMC_HIS.sens = @vl_Sens
				
			end			


			/*B On compte le nombre d'evt sur le lineaire et on retire les donnees de la table tempo */
			
			select @vl_Nb_evts = count(*) from #TABLE_EVT
			
			delete #TABLE_EVT
								
			
			/*B Si le nombre d'evts comptabilises est inferieur au seuil : le lineaire courant ne nous interesse pas !! */
			
			if (@vl_Nb_evts >= @va_Nb_mini_evts_in)
			begin
			
				select @vl_Echangeur = null	
				select @vl_Aire = null
				select @vl_Tunnel_debut = null
				select @vl_Tunnel_fin = null
				select @vl_PNT_debut = null
				select @vl_PNT_fin = null
				select @vl_Nb_voies_PR_debut = null
				select @vl_BAU_PR_debut = null
				select @vl_Nb_voies_PR_fin = null
				select @vl_BAU_PR_fin = null
				select @vl_Pentes_PR_debut = null
				select @vl_Pentes_PR_fin = null
				select @vl_Rampes_PR_debut = null
				select @vl_Rampes_PR_fin = null
				
				/* Recherche du nombre de voies au PR debut, de la BAU au PR debut */
				/* presence tunnel, des pentes au PR debut, les aires au PR entree et les echangeurs */
				select 
					@vl_Nb_voies_PR_debut = CFG..RES_SEG.nombre_de_voies, 
					@vl_BAU_PR_debut = CFG..RES_SEG.largeur_BAU,
					@vl_Tunnel_debut = convert(tinyint, CFG..RES_SEG.tunnel)
				from CFG..RES_SEG, CFG..RES_POR
				where
					@vl_PR_courant_debut >= CFG..RES_SEG.PR_debut and
					@vl_PR_courant_debut < CFG..RES_SEG.PR_fin and
					CFG..RES_SEG.sens = @vl_Sens and
					CFG..RES_POR.autoroute = @va_Autoroute_in and
					@vl_PR_courant_debut >= CFG..RES_POR.PR_debut and
					@vl_PR_courant_debut < CFG..RES_POR.PR_fin and
					CFG..RES_POR.numero = CFG..RES_SEG.portion
					
				
				select 
					@vl_PNT_debut = CFG..RES_PNT.pente
				from CFG..RES_POR, CFG..RES_PNT
				where
					CFG..RES_POR.autoroute = @va_Autoroute_in and
					@vl_PR_courant_debut >= CFG..RES_POR.PR_debut and
					@vl_PR_courant_debut < CFG..RES_POR.PR_fin and
					CFG..RES_POR.numero = CFG..RES_PNT.portion and
					@vl_PR_courant_debut >= CFG..RES_PNT.PR_debut and
					@vl_PR_courant_debut < CFG..RES_PNT.PR_fin
					
					
				select 
					@vl_Echangeur = CFG..RES_ECH.numero
				from CFG..RES_POR, CFG..RES_ECH
				where
					CFG..RES_POR.autoroute = @va_Autoroute_in and
					@vl_PR_courant_debut >= CFG..RES_POR.PR_debut and
					@vl_PR_courant_debut < CFG..RES_POR.PR_fin and
					CFG..RES_POR.numero = CFG..RES_ECH.portion and
					@vl_PR_courant_debut <= CFG..RES_ECH.PR and
					@vl_PR_courant_fin >= CFG..RES_ECH.PR
					
				
				select 
					@vl_Aire = CFG..RES_AIR.numero
				from CFG..RES_POR, CFG..RES_AIR
				where
					CFG..RES_POR.autoroute = @va_Autoroute_in and
					@vl_PR_courant_debut >= CFG..RES_POR.PR_debut and
					@vl_PR_courant_debut < CFG..RES_POR.PR_fin and
					CFG..RES_POR.numero = CFG..RES_AIR.portion and
					CFG..RES_AIR.sens = @vl_Sens and
					@vl_PR_courant_debut <= CFG..RES_AIR.PRentree and
					@vl_PR_courant_fin >= CFG..RES_AIR.PRentree
					
				
				/* Recherche du nombre de voies au PR fin, de la BAU au PR fin */
				/* de la presence tunnel au PR fin, des pentes au PR fin, et les aires au PR sortie */
				select
					@vl_Nb_voies_PR_fin = CFG..RES_SEG.nombre_de_voies, 
					@vl_BAU_PR_fin = CFG..RES_SEG.largeur_BAU,
					@vl_Tunnel_fin = convert(tinyint, CFG..RES_SEG.tunnel)
				from CFG..RES_SEG, CFG..RES_POR
				where
					@vl_PR_courant_fin > CFG..RES_SEG.PR_debut and
					@vl_PR_courant_fin <= CFG..RES_SEG.PR_fin and
					CFG..RES_SEG.sens = @vl_Sens and
					CFG..RES_POR.autoroute = @va_Autoroute_in and
					@vl_PR_courant_fin > CFG..RES_POR.PR_debut and
					@vl_PR_courant_fin <= CFG..RES_POR.PR_fin and
					CFG..RES_POR.numero = CFG..RES_SEG.portion
					
					
				select
					@vl_PNT_fin = CFG..RES_PNT.pente
				from CFG..RES_POR, CFG..RES_PNT
				where
					CFG..RES_POR.autoroute = @va_Autoroute_in and
					@vl_PR_courant_fin > CFG..RES_POR.PR_debut and
					@vl_PR_courant_fin <= CFG..RES_POR.PR_fin and
					CFG..RES_POR.numero = CFG..RES_PNT.portion and
					@vl_PR_courant_fin > CFG..RES_PNT.PR_debut and
					@vl_PR_courant_fin <= CFG..RES_PNT.PR_fin
					
					
				select
					@vl_Aire = CFG..RES_AIR.numero
				from CFG..RES_POR, CFG..RES_AIR
				where
					CFG..RES_POR.autoroute = @va_Autoroute_in and
					@vl_PR_courant_fin > CFG..RES_POR.PR_debut and
					@vl_PR_courant_fin <= CFG..RES_POR.PR_fin and
					CFG..RES_POR.numero = CFG..RES_AIR.portion and
					CFG..RES_AIR.sens = @vl_Sens and
					@vl_PR_courant_debut <= CFG..RES_AIR.PRsortie and
					@vl_PR_courant_fin >= CFG..RES_AIR.PRsortie
				
				
				if (@vl_Tunnel_debut = 1 or @vl_Tunnel_fin = 1) select @vl_Tunnel = 1 else select @vl_Tunnel = 0
				
				if (@vl_Echangeur is not null) select @vl_Echangeur = 1 else select @vl_Echangeur = 0
				
				if (@vl_Aire is not null) select @vl_Aire = 1 else select @vl_Aire = 0
				
				if (@vl_PNT_debut > 0) select @vl_Rampes_PR_debut = @vl_PNT_debut
				if (@vl_PNT_fin > 0) select @vl_Rampes_PR_fin = @vl_PNT_fin
				if (@vl_PNT_debut < 0) select @vl_Pentes_PR_debut = abs(@vl_PNT_debut)
				if (@vl_PNT_fin < 0) select @vl_Pentes_PR_fin = abs(@vl_PNT_fin)
				
				
				/*B On insere toutes les donnees trouvees dans la table resultat #ACCIDENTOGENE */
				
				insert #ACCIDENTOGENE 
						(autoroute,
						sens,
						PR_debut,
						PR_fin,
						nb_evts,
						nb_voies_PR_debut,
						nb_voies_PR_fin,
						BAU_PR_debut,
						BAU_PR_fin,
						pentes_PR_debut,
						pentes_PR_fin,
						rampes_PR_debut,
						rampes_PR_fin,
						tunnel,
						echangeur,
						aire)
						
				values (@vl_Autoroute,
					@vl_Sens,
					@vl_PR_courant_debut,
					@vl_PR_courant_fin,
					@vl_Nb_evts,
					@vl_Nb_voies_PR_debut,
					@vl_Nb_voies_PR_fin,
					@vl_BAU_PR_debut,
					@vl_BAU_PR_fin,
					@vl_Pentes_PR_debut,
					@vl_Pentes_PR_fin,
					@vl_Rampes_PR_debut,
					@vl_Rampes_PR_fin,
					@vl_Tunnel,
					@vl_Echangeur,
					@vl_Aire)
							
				
			end	/* Fin du traitement ds le cas ou le nbre d'evts etait suffisant */
		
			
			
			
			/*A On passe au sens suivant: a savoir le sens NORD */
			
			select @vl_Sens = @vl_Sens + 1
			
		end	/* Fin du traitement pour chacun des deux sens */		
		
			
		/*A On passe au lineaire suivant */
			
		select @vl_PR_courant_debut = @vl_PR_courant_debut + @vl_intervalle_PR
		select @vl_PR_courant_fin = @vl_PR_courant_debut + @vl_intervalle_PR			
			
			
	end	/* fin du parcours de la zone lineaire par lineaire */
		
end	/* fin du traitement pour les lineaires 100m, 500m et 1000m */
	

/*A retourner la liste */
	
select * from #ACCIDENTOGENE


return XDC_OK
go
