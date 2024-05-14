/*E*/
/*  Fichier : $Id: xzae17sp.prc,v 1.37 2020/11/03 16:14:46 pc2dpdy Exp $        $Revision: 1.37 $        $Date: 2020/11/03 16:14:46 $
-------------------------------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* FICHIER xzae17sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* B.G.	14/10/94	: Creation
* B.G.  19/10/94	: Ajout 'enrichir=1 si
			  posteenrichisseur=null'(1.2)
* C.T.	10/11/94	: Modif bugs applix V 1.4
* B.G.	01/12/94	: Modif bouchon V 1.6
* B.G.	01/12/94	: Modif pb having V 1.7
* C.T.	11/03/96	: Ajout parametre d'appel interne de proc V1.8
*                         (modif 364)
* C.T.	29/11/96	: Modif ordre de la liste retournee (680) V1.9
*                         et ajout des dates de debut, fin, debut prevu, fin prevue dans la liste
* C.T.	09/12/96	: Correction ordre de la liste retournee (680) V1.10
* JMG	07/05/97	: ajouter les fmc operateurs des sites DN et DM au CI (temps de parcours) 1.11
* JMG	08/01/98	: gestion multi-sites (dem/1536) 1.12
* PN	24/02/99	: modif interges dem/1724 1.13 1.14
* JMG	21/03/00	: FMC operateurs dans un nouveau bloc  1.15
* Hilmarcher 16/02/05   : Recherche de la localisation a partir de la derniere validation de la FMC DEM425 v1.17
* Niepceron 25/02/05   : Nouveau tri FMC (second critere = District courant enrichisseur/non enrichisseur) DEM426 v1.18
* Niepceron 11/03/05   :Ajout sit ds la #LISTE_ORDONNEE DEM426 v1.19
* ???	22/06/07	: Prise en compte site DP 1.20
* JPL	27/11/08	: Degroupee; Donnees FMC_GEN extraites ensemble; utiliser FMC_HIS_DER; Identique precedente 1.21
* JPL	01/12/08	: Ne plus retourner de Fmc tete de bouchon quand la queue n'est pas dans FMC_GEN (DEM 849) 1.22
* JPL	02/12/08	: Tri des fmc en sous-groupes plus fins (DEM 833) 1.23
* JPL	17/12/08	: Tri different des groupes d'operations en cours et FMC en prevision (DEM 833) 1.24
* JPL	23/12/08	: Tetes de bouchon prises en compte quand la queue n'est pas sur le site (DEM 849) 1.25
* JPL	23/01/09	: Retourne le droit d'enrichir du site et non du poste + le poste enrichisseur (DEM 848) 1.26
* JPL	12/03/09	: Si tete et queue de bouchon separees: retourne site et poste de l'extremite locale 1.27
* JPL	11/05/09	: Restauration 'distinct' pour compenser les eventuels doublons de FMC_TRT 1.28
* JPL	29/06/09	: Fmc 'Veh. Marche Arriere' dans les sous-groupes 'Accident', 'Contresens' ... (DEM 891) 1.29
* JMG           02/10/09        : secto DEM887 1.30
* NDE   20/07/15	: Nouveau parametre pour FMC deux colonnes au CI (DEM 1138) 1.31
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.32
* JPL	27/02/17	: Fmc 'Pieton' en sous-groupes 3 des evts. en cours; FMC 'Autre' en deuxieme liste (DEM 1197)  1.33
* JPL	25/03/19	: Selection des Fmc selon directives en configuration (DEM 1325)  1.34
* JPL	11/09/19	: Sélection de configuration du site seul en table LST_FMC_MAC evitant les doublons (DEM 1325)  1.35
* JPL	13/09/19	: Restauration du moyen de traiter les FMC clôturées  1.36
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
-----------------------------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------------------------------
* Service rendu
*
* Retourne la liste des fiches main courante à traiter
-------------------------------------------------------------------------------
* Sequence d'appel
* SP	XZAE17_Liste_Evt_Traiter
*
* Arguments en entree
* XDY_Eqt	va_Machine_in
* XDY_Operateur	va_Operateur_in
* XDY_Octet	va_ListeFMC_in
*
* Arguments en sortie
* XDY_Octet	va_CleEvt_out
* XDY_Entier	va_NumEvt_out
* XDY_Octet	va_Type_out
* XDY_Autoroute	va_Autoroute_out
* XDY_PR	va_PR_out
* XDY_Sens	va_Sens_out
* XDY_Horodate	va_Horodate_out
* XDY_Booleen	va_Enrichir_out
* XDY_Booleen	va_Reveil_out
* XDY_Horodate	va_HoroDebutPrevu_out
* XDY_Horodate	va_HoroFinPrevue_out
* XDY_Horodate	va_HoroDebut_out
* XDY_Horodate	va_HoroFin_out
* XDY_Booleen	va_Operation_out
* XDY_Octet	va_Datex_out
* XDY_Octet	va_sit_out
*
* Code retour
* XDC_OK
* XDC_NOK	: fiche non trouvee
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
*
-----------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE17' and type = 'P')
	drop procedure XZAE17
go

/*A Table de travail des données des Fmc a traiter */
--if exists (select * from sysobjects where name = 'LISTE_FMC_A_TRAITER' and type = 'U')
--	drop table  LISTE_FMC_A_TRAITER
--go
--
--create table LISTE_FMC_A_TRAITER (
--	spid			int		not null,
--	liste			int,
--	position		int,
--	espace			tinyint,

--	cle			tinyint,
--	numero			int,
--	type			tinyint,
--	operation		tinyint,
--	sit			tinyint,
--	poste_enrichisseur	smallint	null,
--	enrichir		tinyint,

--	autoroute		tinyint		null,
--	PR			int		null,
--	sens			tinyint		null,

--	horodate		datetime,
--	debut_prevu		datetime	null,
--	fin_prevue		datetime	null,
--	debut			datetime	null,
--	fin			datetime	null,

--	ordre			int
--)
--go


create procedure XZAE17
	@va_Machine_in		smallint	= null,
	@va_Operateur_in	smallint	= null,
	@va_ListeFMC_in		tinyint		= null,
	@va_NomSiteLocal_in     char(2)         = null
as
	declare	@vl_SiteLocal		T_SITE,
		@vl_typeSite		tinyint,
		@vl_Maintenant		datetime


	/* Transformation des parametres d'entree nulls */
	if @va_Machine_in = 0		select @va_Machine_in = null
	if @va_Operateur_in = 0		select @va_Operateur_in = null
	if @va_ListeFMC_in = 0		select @va_ListeFMC_in = null


	/*A controle des arguments obligatoires */
--	if (@va_Machine_in = null  or  @va_Operateur_in = null)
--		return XDC_ARG_INV


	/*A recherche du site local */
        /* MOVIS */
        if @va_NomSiteLocal_in = null
		select @vl_SiteLocal = numero, @vl_typeSite = type
                from CFG..RES_DIS
                where serveur = @@servername
        else
		select @vl_SiteLocal = numero, @vl_typeSite = type
                from CFG..RES_DIS
                where code = @va_NomSiteLocal_in


	if @vl_typeSite = XDC_TYPEM_PCS
		return XDC_NOK


	/*A
	** Déterminer la configuration attachée à l'opérateur,
	**   à défaut à la machine, à défaut au site
	** ---------------------------------------------------
	*/

	select	liste, colonne  into #LISTE_FMC_CONFIG
	from	CFG..LST_FMC_MAC  where operateur is not null  and  operateur = @va_Operateur_in

	if @@rowcount = 0
	begin
		insert	#LISTE_FMC_CONFIG
		select	liste, colonne	from CFG..LST_FMC_MAC
		where	machine is not null  and  machine = @va_Machine_in

		if @@rowcount = 0
		begin
			insert	#LISTE_FMC_CONFIG
			select	liste, colonne	from CFG..LST_FMC_MAC
			where	sit is not null  and  sit = @vl_SiteLocal
			  and	machine is null  and  operateur is null
			if @@rowcount = 0
				return XDC_NOK
		end
	end

--select * from #LISTE_FMC_CONFIG



	/*A
	** Rechercher les FMC à traiter correspondant aux critères de la configuration
	** ---------------------------------------------------------------------------
	*/

	create table #LISTE_FMC_A_TRAITER
	(
		liste			int,
		position		int,
		espace			tinyint,

		cle			tinyint,
		numero			int,
		type			tinyint,
		operation		tinyint,
		sit			tinyint,
		poste_enrichisseur	smallint	null,
		enrichir		tinyint,

		autoroute		tinyint		null,
		PR			int		null,
		sens			tinyint		null,

		horodate		datetime,
		debut_prevu		datetime	null,
		fin_prevue		datetime	null,
		debut			datetime	null,
		fin			datetime	null,

		ordre			int
	)


	begin tran

	/*DEM 426 : ajout champ sit */
	insert #LISTE_FMC_A_TRAITER
	select --distinct
		CFG.liste,
		COL.position,
		COL.espace,

		GEN.cle,
		GEN.numero,
		GEN.type,
		TYP.operation,
		GEN.sit,
		GEN.poste_enrichisseur,
		enrichir = case when GEN.sit = @vl_SiteLocal then XZAEC_ENR_FMC else 0 end,

		DER.autoroute,
		DER.PR,
		DER.sens,

		horodate = isnull (GEN.debut, GEN.debut_prevu),
		GEN.debut_prevu,
		GEN.fin_prevue,
		GEN.debut,
		GEN.fin,

		ordre = case when COL.ordre = 0 then (-1) else 1 end
		--,delta_date = 0

	from	EXP..FMC_GEN GEN, EXP..FMC_TRT TRT,
		CFG..TYP_FMC TYP, #LISTE_FMC_CONFIG CFG, CFG..LST_FMC_COL COL, CFG..LST_FMC_BLC BLC,
		EXP..FMC_HIS_DER DER

	where	GEN.type != XZAEC_FMC_TeteBouchon
--	  and	GEN.cloture is null

	  and	( GEN.numero = TRT.evenement  and  GEN.cle = TRT.cle)

	  and	( TRT.sit = @vl_SiteLocal
#ifdef CI
		/* au CI, ajouter les Fmc operateurs des autres sites */
		     or ( GEN.type = XZAEC_FMC_PosteOperateur  and  TRT.sit in (XDC_VC, XDC_DP) )
#endif
		)

	  and	GEN.type = TYP.numero

	  and	(
			( @va_ListeFMC_in is null  or  @va_ListeFMC_in = CFG.liste )  and

			( COL.numero = CFG.colonne )  and
			( BLC.numero = COL.bloc )  and

			( BLC.operation is null  or  TYP.operation = BLC.operation )  and

			( BLC.sit is null  or
			  ( BLC.sit = XDC_LOCAL      and  GEN.sit = @vl_SiteLocal )  or
			  ( BLC.sit = XDC_NON_LOCAL  and  GEN.sit != @vl_SiteLocal )
			)  and

			( BLC.poste is null  or
			  ( BLC.poste = XDC_LOCAL      and  GEN.poste_enrichisseur = @va_Machine_in )  or
			  ( BLC.poste = XDC_NON_LOCAL  and  GEN.poste_enrichisseur != @va_Machine_in )
			)  and

			( BLC.etat is null  or
			  ( BLC.etat = XDC_FMC_ETA_TERMINE  and GEN.fin is not null )  or
			  ( BLC.etat = XDC_FMC_ETA_ENCOURS  and GEN.debut is not null and GEN.fin is null )  or
			  ( BLC.etat = XDC_FMC_ETA_PREVU  and GEN.debut is null and GEN.fin is null )
			)  and

			( BLC.types is null  or
			  ( BLC.types > 0  and
			       GEN.type in (select type from CFG..TYP_FMC_GRP where numero = BLC.types) )  or
			  ( BLC.types < 0  and
			       -- Critère déja appliqué  GEN.type != XZAEC_FMC_TeteBouchon  and
			       GEN.type not in (select type from CFG..TYP_FMC_GRP where numero = (-BLC.types)) )
			)  and

			( BLC.origine = XDC_CHAINE_VIDE  or
			  ( BLC.origine != XDC_CHAINE_VIDE  and
			    ( substring (BLC.origine, 1, 1) != XDC_CAR_NON  and
			         GEN.origine_creation like rtrim (BLC.origine) )  or
			    ( substring (BLC.origine, 1, 1) = XDC_CAR_NON  and
			         GEN.origine_creation not like rtrim (stuff (BLC.origine, 1, 1, null)) )
			  )
			)
		)

	  and	( GEN.numero = DER.numero  and  GEN.cle = DER.cle )



	/*A
	** Mettre à jour les droits d'enrichissement de la tête des bouchons
	** -----------------------------------------------------------------
	*/

	update	#LISTE_FMC_A_TRAITER

	set	enrichir = enrichir | XZAEC_ENR_TETEBOU

	from	#LISTE_FMC_A_TRAITER LFT,  EXP..FMC_GEN GEN1,  EXP..FMC_GEN GEN2

	where	LFT.type = XZAEC_FMC_QueueBouchon

	  and	(GEN1.numero = LFT.numero  and  GEN1.cle = LFT.cle)
	  and	(GEN2.numero = GEN1.cause  and  GEN2.cle = GEN1.cle_cause)

	  and	GEN2.type = XZAEC_FMC_TeteBouchon
	  and	GEN2.sit = @vl_SiteLocal


	commit tran


	/*A
	** Retourner les Fmc dans l'ordre des listes, positions et ordre spécifié (horodate ...)
	** -------------------------------------------------------------------------------------
	*/

	select @vl_Maintenant = getdate()

--	update #LISTE_FMC_A_TRAITER
--	set delta_date = datediff (minute, @vl_Maintenant, horodate)

	select * from #LISTE_FMC_A_TRAITER
	order by liste, position, ordre * datediff (minute, @vl_Maintenant, horodate)

	return XDC_OK
go
