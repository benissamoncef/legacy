/*E*/
/*  Fichier : $Id: xzae204sp.prc,v 1.30 2019/10/01 19:28:19 devgfi Exp $        Release : $Revision: 1.30 $        Date : $Date: 2019/10/01 19:28:19 $
------------------------------------------------------
* STERIA *  PROJET PASTRE
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae204sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* retourne la syntese des syntheses
*
------------------------------------------------------
* HISTORIQUE :
*
* Hilmarcher 09/03/05: Creation
* CHI	25/04/05: modif pour ajout grele sur la chaussee DEM456 v1.2
* CHI	20/07/05: ajout cas nature de l(obstacle DEM487 v1.3
* JPL	18/04/07 : Ajout d'un argument aux appels XZAE80 et XZAE83 (DEM 638) 1.4
* JPL	15/05/07 : Prise en compte FMCs Traitement chaussees et Chaussee glissante (DEM 644,646) 1.5
* JPL	15/05/07 : Prise en compte FMC Echangeur deconseille (DEM 645) 1.6
* JPL	06/06/07 : Ajout commentaires marques de sections 1.7
* JPL	07/06/07 : Pas de visibilite pour Traitement chaussees et Chaussee glissante (DEM 644,646) 1.8
* JPL	12/06/07 : Prise en compte Fmc Basculement pour Travaux (DEM 647) 1.9 + 1.10 (05/01/08)
* JPL	11/08/08 : Classe Echangeur: distinction FMCs par NUMERO de type (DEM 797)  1.11
* JPL	11/08/08 : Basculement pour Travaux: idem sur district qu'au CI; libelle voies (DEM 798)  1.12
* JMG	03/12/08 : ajout validation dans FMC_ACC 1.13 DEM/852
* JPL	02/06/09 : Gestion complete requetes PAR POSTE; Purge resultats precedents au premier appel 1.14
* JPL	04/06/09 : Gestion ordre validations tete et queue de bouchon a la meme date; identique precedente 1.15
* JPL	19/05/09 : Retour de codes d'erreur differents selon le cas d'erreur 1.16
* JPL	10/07/09 : Trace des actions avant leur ajout par xzae206 1.17
* JPL	31/08/09 : Isolation differences EXP / HIS ; identique a la precedente 1.18
* JPL	01/09/09 : Diff. EXP/HIS: type anterieur, premiers commentaires, vitesse et contenu vehicules lents 1.19
* JPL	01/09/09 : Transmission localisation complete (de la queue pour un bouchon) a xzae206 pour les actions (DEM 519) 1.20
* JPL	26/08/10 : Prise en compte Fmc Zone de stockage dans la classe Travaux (DEM 942) 1.21
* JPL	09/02/11 : Prise en compte du type de travaux "Tunnels" (DEM 951)  1.22
* JPL	22/09/11 : Localisation des tetes de bouchons sur points caracteristiques (DEM 994)  1.23
* JPL	23/09/11 : Utilisation de xzae83 a la place de xzae80, obsolete  1.24
* LCL 	02/12/11 : Ajout vitesse et vitesse opposee (DEM 1015)  1.25
* JPL	07/09/12 : Mention 'Incendie maitrise' avec la date pour Incendie de vegetation (DEM 1037)  1.26
* JPL	14/11/12 : Action envoi conditions de conduite: pas de fin (avec succes ou echec comme appels/fax)  (DEM 1032)  1.27
* JMG	08/03/18:  ajout personnel travaux 1.28
* JPL	11/09/19 : FMC Incendie généralisée : mention de la nature de l'incendie (DEM 1352)  1.29
* JPL	01/10/19 : Basculement pour travaux : indiquer la présence d'un bouchon mobile (DEM 1346)  1.30
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
* 1_ si premier appel ou appel unique :
* -------------------------------------
*	- vide la table FMC_LIS des relatives au poste indique
*
* 2_ dans tous les cas d'appel :
* ------------------------------
*	- cree la synthese d'une fiche main courante sous la forme d'une liste de lignes 
* 	- insere le resultat dans FMC_LIS avec le numero de poste
*
* 3_ si dernier appel ou appel unique :
* -------------------------------------
*	- trie et retourne le contenu de la table FMC_LIS avec conditions sur NumPoste
*	- vide la table FMC_LIS des donnees relatives a ce poste
* 
* Sequence d'appel
* SP	XZAE204_synthese_synthese
* 
* Arguments en entree
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* XDY_Octet 	va_TypeAppel_in:
* XDY_Octet     va_NumPoste_in
* 
* Arguments en sortie
* XDY_Octet	va_Etat_out  : a traiter, prevision ou 
* 			       clos (XDC_FMC_ETAT_xxx)
* char(92)	va_ligne_out : colonne 1 du select
* XDY_Horodate	va_heure_out : colonne 2 du select 
* 			       (null si la ligne n'est pas selectionnable)
* 
* Code retour
* XDC_OK	
* XDC_NOK	evenement pas trouve
* XDC_ARG_INV	
* < 0          : erreur sybase
* 
* Conditions d'utilisation
* Les parametres de sortie va_ligne_out, va_heure_out
* sont retournes dans une liste
* 
* La procedure stockee au CI recherche les informations 
* dans la base HIS tandis qu'au niveau des districts la 
* recherche s'effectue dans la base EXP.
* Le code de creation de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
* 
* 
* Fonction
* Select dans les tables EVENEMENT, EVENEMENT_HISTORIQUE, 
* ALERTE, ACTION, etc
* a preciser fonction de l'IHM
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE204' and type = 'P')
	drop procedure XZAE204
go


#ifdef HIST

/*A 
 ** CREATION DE LA PRODECURE STOCKEE POUR LE CI
 */

create procedure XZAE204
	@va_NumEvt_in		int	= null,
	@va_CleEvt_in		tinyint	= null,
	@va_TypeAppel_in	tinyint = null,
	@va_NumPoste_in 	smallint= null,
	@va_Etat_out		tinyint	= null output	-- XDC_FMC_ETAT_xxx

as
	declare @vl_Cloture datetime, @vl_Alerte int, @vl_SiteAlerte T_SITE,
		@vl_PR T_PR, @vl_Sens T_SENS, @vl_Autoroute T_AUTOROUTE,
		@vl_NomAutoroute char(4), @vl_ConfirmeSignale bit,
		@vl_PR_cause T_PR,@vl_Sens_cause T_SENS, @vl_Autoroute_cause T_AUTOROUTE,
		@vl_PR_conseq T_PR, @vl_Sens_conseq T_SENS, @vl_Autoroute_conseq T_AUTOROUTE,
		@vl_VL T_VOIE, @vl_VM1 T_VOIE,
		@vl_VM2 T_VOIE, @vl_VR T_VOIE, @vl_BAU T_VOIE,
		@vl_VL_I T_VOIE, @vl_VM1_I T_VOIE,
		@vl_VM2_I T_VOIE, @vl_VR_I T_VOIE, @vl_BAU_I T_VOIE,
		@vl_BAU_etroite tinyint, @vl_BAU_I_etroite tinyint,
		@vl_OrigCreation char(25),
		@vl_Debut datetime, @vl_DebutPrevu datetime,
		@vl_Fin datetime, @vl_FinPrevu datetime,
		@vl_NumEvtConseq int, @vl_CleEvtConseq tinyint, 
		@vl_TypeFmc T_NOM, @vl_NumTypeFmc smallint, @vl_ClasseFmc tinyint,
		@vl_TypeCause T_NOM, @vl_NumTypeCause smallint, @vl_HoroCause datetime,
		@vl_TypeFmcConseq T_NOM, @vl_NumTypeFmcConseq smallint,
		@vl_TypeAnterieur smallint, @vl_NomTypeAnterieur T_NOM,
		@vl_HoroValidation datetime, @vl_HoroValPrec datetime,
		@vl_Heure datetime, @vl_TypeHeure tinyint, @vl_NumAction int,
		@vl_Cause int, @vl_CleCause tinyint,
		@vl_PointCaract tinyint, @vl_NumPointCaract tinyint, @vl_NomPointCaract T_NOM,
		@vl_PointCarTete tinyint, @vl_NumPointCarTete tinyint, @vl_NomPointCarTete T_NOM,
		@vl_NumeroTexte smallint, @vl_HoroCommFin datetime,
		@vl_HoroComm datetime, @vl_TypeAction tinyint,
		@vl_SqlStatusValidation int, @vl_Site T_SITE,
		@vl_NbVehLeg tinyint, @vl_NbPL tinyint, @vl_NbMoto tinyint,
		@vl_NbCar tinyint, @vl_NbRemorques tinyint,
		@vl_BlessesLeg tinyint, @vl_BlessesGra tinyint,
		@vl_Morts tinyint, @vl_MatDang bit, @vl_VehESCOTA bit,
		@vl_PersESCOTA bit, @vl_Personalite bit, @vl_Degats tinyint,
		@vl_NumPlan smallint, @vl_NomPlan T_NOM, @vl_NomEchSortie T_NOM,
		@vl_NomEchReinjection T_NOM, @vl_EchReinjection tinyint,
		@vl_TypeVeh tinyint, @vl_CodeMatDang char(6), 
		@vl_Commentaire char(100), @vl_LongVeh smallint,
		@vl_Contenu char(25), @vl_Hauteur smallint,
		@vl_Largeur smallint, @vl_Poids smallint,
		@vl_Vitesse tinyint, @vl_TypePerturb tinyint,
		@vl_nature_obstacle char(100),
		@vl_Visibilite smallint, @vl_EntreeN bit, @vl_EntreeS bit,
		@vl_SortieN bit, @vl_SortieS bit,
		@vl_NomCnc T_NOM, @vl_CncFerme bit, @vl_Status int,
		@vl_Penurie bit, @vl_Carburant tinyint, @vl_Nombre smallint,
		@vl_Perception bit, @vl_Violence bit, @vl_Operateur T_OPERATEUR,
		@vl_NomCarburant char(15), @vl_AutorouteM T_AUTOROUTE,
		@vl_PRTeteM T_PR, @vl_PRQueueM T_PR, @vl_SensM T_SENS, 
		@vl_Mobile bit, @vl_VoiesRet bit, @vl_DebutPlanifie datetime,
		@vl_FinPlanifie datetime, @vl_Continu bit, @vl_NatureTrv bit,
		@vl_TypeTrv tinyint, @vl_TypeBouchon tinyint, 
		@vl_NumEvtUtilise int, @vl_CleEvtUtilise tinyint,
		@vl_NumEvtTete int, @vl_CleEvtTete tinyint,
		@vl_NumEvtQueue int, @vl_CleEvtQueue tinyint,
		@vl_NumEvt int, @vl_CleEvt tinyint,
		@vl_nb int, @vl_DateRecherche datetime, @vl_Date datetime,
		@vl_Eqt T_EQUIPEMENT, @vl_IndicateurCreation bit,
		@vl_Autoroute_Prec T_AUTOROUTE, @vl_PR_Prec T_PR, @vl_Sens_Prec T_SENS,
		@vl_FausseAlerte tinyint,
		@vl_PRdebut_VR T_PR,@vl_PRfin_VR T_PR,@vl_debut_VR datetime,@vl_fin_VR datetime,
		@vl_PRdebut_VM2 T_PR,@vl_PRfin_VM2 T_PR,@vl_debut_VM2 datetime,@vl_fin_VM2 datetime,
		@vl_PRdebut_VM1 T_PR,@vl_PRfin_VM1 T_PR,@vl_debut_VM1 datetime,@vl_fin_VM1 datetime,
		@vl_PRdebut_VL T_PR,@vl_PRfin_VL T_PR,@vl_debut_VL datetime,@vl_fin_VL datetime,
		@vl_PRdebut_BAU T_PR,@vl_PRfin_BAU T_PR,@vl_debut_BAU datetime,@vl_fin_BAU datetime,
		@vl_PRdebut_VR_I T_PR,@vl_PRfin_VR_I T_PR, @vl_debut_VR_I datetime,@vl_fin_VR_I datetime,
		@vl_PRdebut_VM2_I T_PR,@vl_PRfin_VM2_I T_PR, @vl_debut_VM2_I datetime,@vl_fin_VM2_I datetime,
		@vl_PRdebut_VM1_I T_PR,@vl_PRfin_VM1_I T_PR, @vl_debut_VM1_I datetime,@vl_fin_VM1_I datetime,
		@vl_PRdebut_VL_I T_PR,@vl_PRfin_VL_I T_PR, @vl_debut_VL_I datetime,@vl_fin_VL_I datetime,
		@vl_PRdebut_BAU_I T_PR,@vl_PRfin_BAU_I T_PR, @vl_debut_BAU_I datetime,@vl_fin_BAU_I datetime,
		@vl_NumFmcInit int, @vl_CleFmcInit tinyint, @vl_SiteOrigine T_SITE, @vl_Datex tinyint,
		@vl_gravite tinyint, @vl_trafic tinyint, @vl_bloquant bit,
		@vl_FeuMaitrise tinyint, @vl_HorodateMaitrise datetime,
		@vl_numero int, @vl_cle int, @vl_ligne char(150), @vl_heure datetime,
		@vl_picto char(4), @vl_poste smallint, @vl_horo datetime, @vl_compteur smallint, @vl_debut_ligne char(1),
		@vl_index tinyint, @vl_Longueur int,
		@vl_Intitule char(60), @vl_Localisation  char(60), 
		@vl_Intitule2 char(60), @vl_Localisation2  char(60), 
		@vl_Texte varchar(250), @vl_Texte_ligne varchar(250),
		@vl_InitLigne char(20), @vl_Ligne char(150),
		@vl_Libelle varchar(20),@vl_vitesse tinyint, @vl_vitesse_opposee tinyint,
		@vl_personnel tinyint, @vl_code_nature tinyint, @vl_nature_inc varchar(100),
		@vl_PresenceBouchon tinyint

	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_TypeAppel_in = 0 select @va_TypeAppel_in = null
	if @va_NumPoste_in = 0 select @va_NumPoste_in = null

	/*A Controle des parametres d'entree */
	if @va_NumEvt_in = null or @va_CleEvt_in = null or @va_TypeAppel_in = null 
		return XDC_ARG_INV

	/*A Lors d'un premier appel, supprimer tous restes d'un appel precedent */
	if (@va_TypeAppel_in = XDC_FCT_APL_PREMIER) or (@va_TypeAppel_in = XDC_FCT_APL_UNIQUE)
	begin
		delete HIS..FMC_LIS
		where poste = @va_NumPoste_in
	end

	/*A réinitialisation de la table LISTE_CONSEQ */
	delete LISTE_CONSEQ
	where spid=@@spid

	/*A Verifier l'existence de la fiche main courante */
	select
		@vl_Datex	= HIS..FMC_GEN.datex,
		@vl_Cloture	= HIS..FMC_GEN.cloture,
		@vl_Alerte	= HIS..FMC_GEN.alerte,
		@vl_SiteAlerte	= HIS..FMC_GEN.sit_alerte,
		@vl_ClasseFmc	= CFG..TYP_FMC.classe,
		@vl_TypeFmc	= CFG..TYP_FMC.nom,
		@vl_NumTypeFmc	= HIS..FMC_GEN.type,
		@vl_OrigCreation = HIS..FMC_GEN.origine_creation,
		@vl_Debut	= HIS..FMC_GEN.debut,
		@vl_DebutPrevu	= HIS..FMC_GEN.debut_prevu,
		@vl_Fin		= HIS..FMC_GEN.fin,
		@vl_FinPrevu	= HIS..FMC_GEN.fin_prevue,
		@vl_Cause	= HIS..FMC_GEN.cause,
		@vl_CleCause	= HIS..FMC_GEN.cle_cause,
		@vl_HoroCause	= HIS..FMC_GEN.horodate_lien_cause,
		@vl_FausseAlerte	= fausse_alerte,
		@vl_NumFmcInit	= num_fmc_init,
		@vl_CleFmcInit	= cle_fmc_init,
		@vl_Degats	= degats_domaine
	from HIS..FMC_GEN, CFG..TYP_FMC
	where	HIS..FMC_GEN.numero = @va_NumEvt_in and 
		HIS..FMC_GEN.cle = @va_CleEvt_in and
		CFG..TYP_FMC.numero = HIS..FMC_GEN.type

	if @@rowcount != 1
		return XDC_NOK


/* ***************** dans HIST on decide que la fmc est close **********************/

			select @va_Etat_out = XDC_FMC_ETAT_CLOS


	/* positionner var internes */
	select @vl_NumEvtQueue = null, @vl_CleEvtQueue = null

	/*A Creation du tableau de stockage de la synthese */
	
	create table #SYNTHESE_CONSEQ
		(
		numero  int not null,
		cle	int not null,
		ligne	char(150) not null,
		heure	datetime null,
		VR	tinyint	default	XDC_VOIE_INCONNUE,
		VM2	tinyint	default	XDC_VOIE_INCONNUE,
		VM1	tinyint	default	XDC_VOIE_INCONNUE,
		VL	tinyint	default	XDC_VOIE_INCONNUE,
		BAU	tinyint	default	XDC_VOIE_INCONNUE,
		VR_I	tinyint	default	XDC_VOIE_INCONNUE,
		VM2_I	tinyint	default	XDC_VOIE_INCONNUE,
		VM1_I	tinyint	default	XDC_VOIE_INCONNUE,
		VL_I	tinyint	default	XDC_VOIE_INCONNUE,
		BAU_I	tinyint	default	XDC_VOIE_INCONNUE,
		BAU_etroite	tinyint	default	XDC_FAUX,
		BAU_I_etroite	tinyint	default	XDC_FAUX,
		picto	char(4) null,
		poste	smallint not null
		)
	
	create table #SYNTH
		(
		numero  int not null,
		cle	int not null,
		ligne	char(150) not null,
		heure	datetime null,
		VR	tinyint	default	XDC_VOIE_INCONNUE,
		VM2	tinyint	default	XDC_VOIE_INCONNUE,
		VM1	tinyint	default	XDC_VOIE_INCONNUE,
		VL	tinyint	default	XDC_VOIE_INCONNUE,
		BAU	tinyint	default	XDC_VOIE_INCONNUE,
		VR_I	tinyint	default	XDC_VOIE_INCONNUE,
		VM2_I	tinyint	default	XDC_VOIE_INCONNUE,
		VM1_I	tinyint	default	XDC_VOIE_INCONNUE,
		VL_I	tinyint	default	XDC_VOIE_INCONNUE,
		BAU_I	tinyint	default	XDC_VOIE_INCONNUE,
		BAU_etroite	tinyint	default	XDC_FAUX,
		BAU_I_etroite	tinyint	default	XDC_FAUX,
		picto	char(4) null,
		poste	smallint not null,
		compteur smallint not null
		)
	
	create table #ACTIONS 
		( 
		numero		int, 
		sit		tinyint, 
		sit_origine	tinyint, 
		type		tinyint, 
		heure		datetime, 
		type_heure	tinyint default XZAEC_ACT_SUC,	/* heure de succes par defaut */
		equipement	smallint null
		) 
	create table #COMMENTAIRES
		(
		evenement             int                   not null,
		cle                   tinyint               not null,
		numero                smallint              not null,
		horodate_validation   datetime              not null,
		horodate_fin          datetime              null    ,
		texte                 char(250)             null    ,
		sit                   tinyint                null     
		)
	create table #HISTORIQUE_FMC
		(
		numero			int,
		cle			tinyint,
		classe			tinyint,
		type			smallint,
		nom_type		char(25),
		type_anterieur		smallint null,
		autoroute		tinyint	null,
		PR			int	null,
		sens			tinyint	null,
		point_caracteristique	tinyint null,
		numero_point_caracteristique tinyint null,
		confirme_ou_signale	bit,
		horodate_validation	datetime,
		critere_ordre		tinyint default 1,
		VR	tinyint	default	XDC_VOIE_INCONNUE,
		VM2	tinyint	default	XDC_VOIE_INCONNUE,
		VM1	tinyint	default	XDC_VOIE_INCONNUE,
		VL	tinyint	default	XDC_VOIE_INCONNUE,
		BAU	tinyint	default	XDC_VOIE_INCONNUE,
		VR_I	tinyint	default	XDC_VOIE_INCONNUE,
		VM2_I	tinyint	default	XDC_VOIE_INCONNUE,
		VM1_I	tinyint	default	XDC_VOIE_INCONNUE,
		VL_I	tinyint	default	XDC_VOIE_INCONNUE,
		BAU_I	tinyint	default	XDC_VOIE_INCONNUE,
		BAU_etroite	tinyint	default	XDC_FAUX,
		BAU_I_etroite	tinyint	default	XDC_FAUX
		)
	create table #TEMPO_HISTORIQUE_FMC
		(
		numero			int,
		cle			tinyint,
		classe			tinyint,
		type			smallint,
		nom_type		char(25),
		type_anterieur		smallint null,
		autoroute		tinyint	null,
		PR			int	null,
		sens			tinyint	null,
		point_caracteristique	tinyint null,
		numero_point_caracteristique tinyint null,
		confirme_ou_signale	bit,
		horodate_validation	datetime,
		critere_ordre		tinyint default 1,
		VR	tinyint	default	XDC_VOIE_INCONNUE,
		VM2	tinyint	default	XDC_VOIE_INCONNUE,
		VM1	tinyint	default	XDC_VOIE_INCONNUE,
		VL	tinyint	default	XDC_VOIE_INCONNUE,
		BAU	tinyint	default	XDC_VOIE_INCONNUE,
		VR_I	tinyint	default	XDC_VOIE_INCONNUE,
		VM2_I	tinyint	default	XDC_VOIE_INCONNUE,
		VM1_I	tinyint	default	XDC_VOIE_INCONNUE,
		VL_I	tinyint	default	XDC_VOIE_INCONNUE,
		BAU_I	tinyint	default	XDC_VOIE_INCONNUE,
		BAU_etroite	tinyint	default	XDC_FAUX,
		BAU_I_etroite	tinyint	default	XDC_FAUX
		)

	if @vl_FausseAlerte = XDC_VRAI
	begin
		/* fausse alerte */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_FAUSSE_ALERTE
		insert HIS..FMC_LIS (numero, cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,  null,@va_NumPoste_in)

		/* ligne vide */
		insert HIS..FMC_LIS(numero, cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,XZAEC_SYN_PAS_DATE,  null,@va_NumPoste_in)
	end

	if @vl_Datex = XDC_VRAI
	begin
		/* fausse alerte */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ORIGINE_DATEX
		insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,  null,@va_NumPoste_in)

		/* ligne vide */
		insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,XZAEC_SYN_PAS_DATE,  null,@va_NumPoste_in)
	end

	/* creation d'un curseur pour le parcours des morceaux */
	/* de commentaires */
	declare Pointeur_comment cursor
	for select horodate_validation, horodate_fin, texte, numero
	from #COMMENTAIRES

	/* creation d'un curseur pour le parcours de la liste des */
	/* actions de la fmc */
	declare Pointeur_action cursor
	for select numero, 
		sit, 
		sit_origine,
		type, 
		heure, 
		type_heure, 
		equipement
	from #ACTIONS
	order by heure, type_heure

	/*A Recherche les informations sur l'alerte */
	if @vl_Alerte != null
	begin
		/*B creation de l'intitule de l'alerte */
		select @vl_Ligne = XDC_CHAINE_VIDE
		select
			@vl_Ligne = convert(char(10),HIS..ALT_EXP.horodate, 103) + 
			' ' + convert(char(8), HIS..ALT_EXP.horodate, 8) +
			' ' + XZAEC_SYN_ALT + CFG..TYP_ALT.nom +
			' ' + CFG..EQT_GEN.nom ,
			@vl_Texte = HIS..ALT_EXP.texte,
			@vl_Autoroute = CFG..EQT_GEN.autoroute,
			@vl_PR = CFG..EQT_GEN.PR,
			@vl_Sens = CFG..EQT_GEN.sens
		from HIS..ALT_EXP, CFG..TYP_ALT, CFG..EQT_GEN
		where	HIS..ALT_EXP.numero = @vl_Alerte and
			HIS..ALT_EXP.sit = @vl_SiteAlerte and
			CFG..TYP_ALT.numero = HIS..ALT_EXP.type and
			CFG..EQT_GEN.numero = HIS..ALT_EXP.equipement and
			CFG..EQT_GEN.type = HIS..ALT_EXP.type_equipement

		/*B Ajout de l'intitule de l'alerte dans la synthese */
		if @vl_Ligne != XDC_CHAINE_VIDE
		begin
			insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,  null,@va_NumPoste_in)

			/* mise en forme de la localisation de l'alerte */
			exec XZAE83 null, null, null, null, @vl_Autoroute, @vl_PR, @vl_Sens,
					@vl_Intitule2 output, @vl_Localisation2 output
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ALT_LOC +
						@vl_Localisation2

			/*B Ajout des informations de l'alerte pour la synthese */
			insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
			values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

			/*B Ajout des commentaires de l'alerte pour la synthese */
			if @vl_Texte != XDC_CHAINE_VIDE
			begin
				/* tronconner le texte en deux morceaux */
				select	@vl_Longueur = char_length(@vl_Texte),
					@vl_Ligne = XZAEC_SYN_PAS_DATE + 
							XZAEC_SYN_ALT_COM + 
							substring(@vl_Texte, 1, 65)
	
				/* insertion de la premiere ligne de commentaire */
				insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
				values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
	
				if @vl_Longueur > 60
				begin
					select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
							   substring(@vl_Texte, 61, 39)
	
					/* insertion de la deuxieme ligne de commentaire */
					insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
					values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
				end
			end
		end
	end


/************************************************************************************************************************/
/************************************************************************************************************************/
/**                                       Recherche de l'historique complet de la FMC                                  **/
/************************************************************************************************************************/
/************************************************************************************************************************/

	insert #HISTORIQUE_FMC
		(numero, cle, classe, type, nom_type, type_anterieur, autoroute,
		PR, sens, confirme_ou_signale, horodate_validation,
		point_caracteristique, numero_point_caracteristique,
		VR, VM2, VM1, VL, BAU, VR_I, VM2_I, VM1_I, VL_I, BAU_I,
		BAU_etroite, BAU_I_etroite, critere_ordre)
	select 
		HIS..FMC_HIS.numero,
		HIS..FMC_HIS.cle,
		CFG..TYP_FMC.classe,
		HIS..FMC_HIS.type_FMC_valide,
		CFG..TYP_FMC.nom,
		HIS..FMC_HIS.type_anterieur,
		HIS..FMC_HIS.autoroute,
		HIS..FMC_HIS.PR,
		HIS..FMC_HIS.sens,
		HIS..FMC_HIS.confirme_ou_signale,
		HIS..FMC_HIS.horodate_validation,
		HIS..FMC_HIS.point_caracteristique,
		HIS..FMC_HIS.numero_point_caracteristique,
		isnull(HIS..FMC_HIS.VR, XDC_VOIE_INCONNUE),
		isnull(HIS..FMC_HIS.VM2, XDC_VOIE_INCONNUE),
		isnull(HIS..FMC_HIS.VM1, XDC_VOIE_INCONNUE),
		isnull(HIS..FMC_HIS.VL, XDC_VOIE_INCONNUE),
		isnull(HIS..FMC_HIS.BAU, XDC_VOIE_INCONNUE),
		isnull(HIS..FMC_HIS.VR_I, XDC_VOIE_INCONNUE),
		isnull(HIS..FMC_HIS.VM2_I, XDC_VOIE_INCONNUE),
		isnull(HIS..FMC_HIS.VM1_I, XDC_VOIE_INCONNUE),
		isnull(HIS..FMC_HIS.VL_I, XDC_VOIE_INCONNUE),
		isnull(HIS..FMC_HIS.BAU_I, XDC_VOIE_INCONNUE),
		isnull(HIS..FMC_HIS.BAU_etroite, XDC_FAUX),
		isnull(HIS..FMC_HIS.BAU_I_etroite, XDC_FAUX),
		1
	from HIS..FMC_HIS, CFG..TYP_FMC
	where	HIS..FMC_HIS.numero = @va_NumEvt_in and 
		HIS..FMC_HIS.cle = @va_CleEvt_in and
		CFG..TYP_FMC.numero = HIS..FMC_HIS.type_FMC_valide
		order by HIS..FMC_HIS.horodate_validation

	/* l'evenement n'a jamais ete valide : pas de synthese */
	if @@rowcount = 0
		return XDC_PAS_SYN


	/*A Recherche la description de la creation de la FMC */
	select 
		@vl_HoroValidation = horodate_validation,
		@vl_NumTypeFmc = type,
		@vl_Autoroute = autoroute,
		@vl_PR = PR,
		@vl_Sens = sens,
		@vl_ConfirmeSignale = confirme_ou_signale,
		@vl_PointCaract = point_caracteristique,
		@vl_NumPointCaract = numero_point_caracteristique,
		@vl_VR = VR,
		@vl_VM2 = VM2,
		@vl_VM1 = VM1,
		@vl_VL = VL,
		@vl_BAU = BAU,
		@vl_VR_I = VR_I,
		@vl_VM2_I = VM2_I,
		@vl_VM1_I = VM1_I,
		@vl_VL_I = VL_I,
		@vl_BAU_I = BAU_I,
		@vl_BAU_etroite = BAU_etroite,
		@vl_BAU_I_etroite = BAU_I_etroite
	from #HISTORIQUE_FMC
	having horodate_validation = min(horodate_validation)

	/* positionner un indicateur sur de la creation de l'historique */
	select @vl_IndicateurCreation = XDC_OUI

	/* le type pour un bouchon est change pour l'affichage */
	/* (libelle plus explicite)                            */
	if @vl_NumTypeFmc = XZAEC_FMC_TeteBouchon
		select @vl_TypeFmc = XZAEC_SYN_FMC_TeteBouchon
	else if @vl_NumTypeFmc = XZAEC_FMC_QueueBouchon
		select @vl_TypeFmc = XZAEC_SYN_FMC_QueueBouchon

	/*A Ajout de description de la creation de la FMC */
	exec XZAE83	@va_NumEvt_in, @va_CleEvt_in, @vl_NumTypeFmc, @vl_TypeFmc,
			@vl_Autoroute, @vl_PR, @vl_Sens,
			@vl_Intitule output, @vl_Localisation output
	select @vl_Ligne = convert(char(10), @vl_HoroValidation, 103) + ' ' +
			convert(char(8), @vl_HoroValidation, 8) + ' ' +
			convert(varchar(60), @vl_Intitule) + ' - '  + XZAEC_SYN_CREATION

	insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
	values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, @vl_HoroValidation,@va_NumPoste_in)

	select @vl_Ligne = XZAEC_SYN_PAS_DATE + @vl_Localisation
	insert HIS..FMC_LIS 
	values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null, @vl_VR, @vl_VM2, @vl_VM1,
		@vl_VL, @vl_BAU, @vl_VR_I, @vl_VM2_I, @vl_VM1_I,@vl_VL_I, @vl_BAU_I, 
		@vl_BAU_etroite, @vl_BAU_I_etroite, null,@va_NumPoste_in)

	if @vl_OrigCreation != XDC_CHAINE_VIDE
	begin
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ORI_CRE + 
				 @vl_OrigCreation
		insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
		values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
	end

	if @vl_Debut != null
	begin
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_DEB + 
					convert(char(10), @vl_Debut, 103) + ' ' + 
					convert(char(8), @vl_Debut, 8) 
		insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
		values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
	end

	if @vl_Fin != null
	begin
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_FIN + 
					convert(char(10), @vl_Fin, 103) + ' ' + 
					convert(char(8), @vl_Fin, 8) 
		insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
		values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
	end

	if @vl_DebutPrevu != null
	begin
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_DEB_PREV + 
					convert(char(10), @vl_DebutPrevu, 103) + ' ' + 
					convert(char(8), @vl_DebutPrevu, 8) 
		insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
		values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
	end

	if @vl_FinPrevu != null
	begin
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_FIN_PREV + 
					convert(char(10), @vl_FinPrevu, 103) + ' ' + 
					convert(char(8), @vl_FinPrevu, 8) 
		insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
		values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
	end

	if @vl_PointCaract != null and @vl_NumPointCaract != null
	begin
		if @vl_PointCaract = XDC_POINT_CARACT_AIRE
		begin
			select @vl_Libelle = XZAEC_SYN_VEH_AIR

			select @vl_NomPointCaract = nom
			from CFG..RES_AIR
			where numero = @vl_NumPointCaract
		end
		else if @vl_PointCaract = XDC_POINT_CARACT_ECHANGEUR
		begin
			select @vl_Libelle = XZAEC_SYN_VEH_ECH

			select @vl_NomPointCaract = nom
			from CFG..RES_ECH
			where numero = @vl_NumPointCaract
		end
		else if @vl_PointCaract = XDC_POINT_CARACT_PEAGE
		begin
			select @vl_Libelle = XZAEC_SYN_VEH_PEA

			select @vl_NomPointCaract = nom
			from CFG..RES_PEA
			where numero = @vl_NumPointCaract
		end
		else if @vl_PointCaract = XDC_POINT_CARACT_LIT_ARRET
		begin
			select @vl_Libelle = XZAEC_SYN_VEH_LIT

			select @vl_NomPointCaract = nom
			from CFG..EQT_GEN
			where	numero = @vl_NumPointCaract and 
				type = XDC_CAPT_LIT_ARRET
		end
		else return XZAEC_FMC_LOC_INC

		if @@rowcount != 1
			return XZAEC_FMC_LOC_INC

		select @vl_Ligne = XZAEC_SYN_PAS_DATE + @vl_Libelle + @vl_NomPointCaract

		insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
		values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
	end

	if @vl_ConfirmeSignale = XZAEC_FMC_SIGNALEE
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_SIGNALEE
	else	select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_CONFIRMEE
	insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
	values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

	if @vl_NumFmcInit is not null
	begin
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
				XZAEC_SYN_FMC_DEGRADE + convert(varchar(10), @vl_NumFmcInit) + ' ' + convert(char(2), @vl_CleFmcInit)
		insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
	end

	if @vl_Degats = XDC_OUI
	begin
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
				XZAEC_SYN_DEG_DOM
		insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
	end


/************************************************************************************************************************/
/************************************************************************************************************************/
/**                                            Ajout du complement de la FMC                                           **/
/************************************************************************************************************************/
/************************************************************************************************************************/

/************************************************************************************************************************/
/**                                               Cas d'un Accident                                                    **/
/************************************************************************************************************************/
	if @vl_ClasseFmc = XZAEC_ClasseAccident
	begin
		select
			@vl_NbVehLeg = nombre_de_vl,
			@vl_NbPL = nombre_de_pl,
			@vl_NbMoto = nombre_de_moto,
			@vl_NbCar = nombre_de_car,
			@vl_NbRemorques = nombre_de_remorques,
			@vl_BlessesLeg = blesses_legers,
			@vl_BlessesGra = blesses_graves,
			@vl_Morts = morts,
			@vl_MatDang = matieres_dangereuses,
			@vl_VehESCOTA = vehicule_ESCOTA,
			@vl_PersESCOTA = personnel_ESCOTA,
			@vl_Personalite = personnalites
		from HIS..FMC_ACC
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in
		having validation = max(validation)

		if @vl_NbVehLeg != null and @vl_NbVehLeg != 0
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_VL + 
					convert(varchar(8),@vl_NbVehLeg)
			insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_NbPL != null and @vl_NbPL != 0
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_PL + 
					convert(varchar(8),@vl_NbPL)
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_NbMoto != null and @vl_NbMoto != 0
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_MOTO + 
					convert(varchar(8), @vl_NbMoto)
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_NbCar != null and @vl_NbCar != 0
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_CAR + 
					convert(varchar(8), @vl_NbCar)
			insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_NbRemorques != null and @vl_NbRemorques != 0
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_REMORQ + 
					convert(varchar(8), @vl_NbRemorques)
			insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_BlessesLeg != null and @vl_BlessesLeg != 0 
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_BLE_LEG + 
					convert(varchar(8), @vl_BlessesLeg)
			insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_BlessesGra != null and @vl_BlessesGra != 0
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_BLE_GRA + 
					convert(varchar(8), @vl_BlessesGra)
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_Morts != null and @vl_Morts != 0
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_MOR + 
					convert(varchar(8), @vl_Morts)
			insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_MatDang = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_PRE_MAT_DNG
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in, @vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_VehESCOTA = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_VEH_ESC 
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_PersESCOTA = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_PER_ESC
			insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_Personalite = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_PERS 
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

	end

/************************************************************************************************************************/
/**                                    Cas d'un evenement de classe Delestage                                          **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClasseDelestage
	begin
		/* rechercher les informations du delestage */
		select @vl_NumPlan = plan_associe
		from HIS..FMC_DEL
		where   numero = @va_NumEvt_in and cle = @va_CleEvt_in

		if @vl_NumPlan != null
		begin
			select
				@vl_NomPlan = CFG..PLN_SCR.nom,
				@vl_NomEchSortie = CFG..RES_ECH.nom,
				@vl_EchReinjection = CFG..PLN_SCR.echangeur_reinjection
			from CFG..PLN_SCR, CFG..RES_ECH
			where	CFG..PLN_SCR.numero = @vl_NumPlan and
				CFG..RES_ECH.numero = CFG..PLN_SCR.echangeur_sortie
	
			if @@rowcount != 1
				return XZAEC_FMC_PAS_DONNEES
	
			select @vl_NomEchReinjection = nom
			from CFG..RES_ECH
			where	numero = @vl_EchReinjection
	
			if @@rowcount != 1
				return XZAEC_FMC_PAS_DONNEES
	
			/* Ajout des informations du delestage dans la synthese */
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_PLN_SCR +
						+ @vl_NomPlan
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
	
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ECH_SOR +
						+ @vl_NomEchSortie
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
	
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ECH_INJ +
						+ @vl_NomEchReinjection
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
	end

/************************************************************************************************************************/
/**                                    Cas d'un evenement de classe Vehicule                                           **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClasseVehicule
	begin
		/* recherche les informations sur un vehicule */
		select
			@vl_TypeVeh = type_de_vehicule,
			@vl_VehESCOTA = vehicule_ESCOTA,
			@vl_CodeMatDang = matieres_dangereuses,
			@vl_Commentaire = commentaire,
			@vl_FeuMaitrise = feu_maitrise,
			@vl_HorodateMaitrise = horodate_maitrise
		from HIS..FMC_VEH
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in

		if @vl_TypeVeh != null
		begin
			if @vl_TypeVeh = XZAEC_VEH_PL
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_VEH_PL
			else if @vl_TypeVeh = XZAEC_VEH_VL   
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_VEH_VL
			else if @vl_TypeVeh = XZAEC_VEH_Car   
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_VEH_CAR
			else if @vl_TypeVeh = XZAEC_VEH_Moto   
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_VEH_MOT
			else if @vl_TypeVeh =  XZAEC_VEH_Remorque 
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_VEH_REM
			else return XZAEC_FMC_ERR_DONNEES

			insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
		if @vl_FeuMaitrise = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_VEH_FEU_MAITRISE +
					convert(char(10),@vl_HorodateMaitrise,103) + ' ' +
					convert(char(8),@vl_HorodateMaitrise,8)
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
		if @vl_VehESCOTA = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_VEH_ESC 
			insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
		if @vl_CodeMatDang != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_MAT_DNG + @vl_CodeMatDang
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
		if @vl_Commentaire != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_COM + 
					substring(@vl_Commentaire, 1, 48),
				@vl_Longueur = char_length(@vl_Commentaire)
			insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

			if @vl_Longueur > 48
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					substring(@vl_Commentaire, 49, 51)
				insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
				values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
			end
		end
	end

/************************************************************************************************************************/
/**                                    Cas d'un evenement de classe Vehicule lent                                      **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClasseVehLent
	begin
		/* recherche les informations sur un vehicule lent */
		select
			@vl_LongVeh = longueur,
			@vl_Vitesse = vitesse,
			@vl_Hauteur = hauteur,
			@vl_Largeur = largeur,
			@vl_Poids = poids,
			@vl_Contenu = contenu
		from HIS..FMC_VLN
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in
		having horodate = min(horodate)

		/* Ajout des informations dans la synthese */
		if @vl_LongVeh != null
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_LON_VEH + 
					convert(varchar(8), @vl_LongVeh)
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_Vitesse != null
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_VIT_VEH + 
					convert(varchar(8), @vl_Vitesse)
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_NumTypeFmc = XZAEC_FMC_ConvoiExc or @vl_NumTypeFmc = XZAEC_FMC_ConvoiMil
		begin
			if @vl_Hauteur != null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_HAU_VEH + 
						convert(varchar(8), @vl_Hauteur)
				insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
				values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
			end
	
			if @vl_Largeur != null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_LRG_VEH + 
						convert(varchar(8), @vl_Largeur)
				insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
				values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
			end
	
			if @vl_Poids != null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_POI_VEH + 
						convert(varchar(8), @vl_Poids)
				insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
				values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
			end

			if @vl_Contenu != null and @vl_Contenu != XDC_CHAINE_VIDE
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_CNT_VEH + 
						convert(varchar(25), @vl_Contenu)
				insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
				values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
			end
	
		end
	end

/************************************************************************************************************************/
/**                                         Cas d'un evenement de classe Meteo                                         **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClasseMeteo
	begin
		/* recherche des informations sur la meteo */
		select
			@vl_PR = PR,
			@vl_Visibilite = visibilite,
			@vl_TypePerturb = type_de_perturbation
		from HIS..FMC_MET
		where   numero = @va_NumEvt_in and cle = @va_CleEvt_in
		having horodate = min(horodate)

		/* Ajout des informations dans la synthese */
		if @vl_TypePerturb != null
		begin
			if @vl_TypePerturb = XZAEC_MET_ChuteNeige
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
						   XZAEC_SYN_CHUT_NEI
			else if @vl_TypePerturb = XZAEC_MET_NeigeChaus
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_NEI_CHA
			else if @vl_TypePerturb = XZAEC_MET_Pluie
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_PLU
			else if @vl_TypePerturb = XZAEC_MET_VentFort
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_VEN
			else if @vl_TypePerturb = XZAEC_MET_Brouillard
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_BROUIL
			else if @vl_TypePerturb = XZAEC_MET_BrouillardGiv
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_BRO_GIV
			else if @vl_TypePerturb = XZAEC_MET_Grele
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_GRELE 
			else if @vl_TypePerturb = XZAEC_MET_GreleChaus
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						  XZAEC_SYN_GREL_CH
			else if @vl_TypePerturb = XZAEC_MET_Verglas
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_VERG
			else if @vl_TypePerturb = XZAEC_MET_Autre
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_AUT
			/* cas d'un evenement Chaussee Glissante */
			else if @vl_TypePerturb = XZAEC_CH_GLIS_Neige
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_NEI_CHA
			else if @vl_TypePerturb = XZAEC_CH_GLIS_Verglas
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_VERG
			else if @vl_TypePerturb = XZAEC_CH_GLIS_Autre
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_AUT
			else if @vl_TypePerturb = XZAEC_CH_GLIS_Grele
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_GREL_CH
			else if @vl_TypePerturb = XZAEC_CH_GLIS_Huile
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_CHS_HUILE
			else if @vl_TypePerturb = XZAEC_CH_GLIS_Carburant
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_CHS_CARBURANT
			else if @vl_TypePerturb = XZAEC_CH_GLIS_Gasoil
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_CHS_GASOIL
			/* cas d'un evenement Traitement Chaussees */
			else if @vl_TypePerturb = XZAEC_TRT_CH_Salage
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_TRT_SALAGE
			else if @vl_TypePerturb = XZAEC_TRT_CH_Deneigement
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_TRT_DENEIG
			else return XZAEC_FMC_ERR_DONNEES

			insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_PR != null
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_FIN_MET + 
					convert(char(7), @vl_PR/1000.0) 
			
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_NumTypeFmc = XZAEC_FMC_Meteo
		begin
			if @vl_Visibilite != null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_VIS_MET + 
						convert(varchar(8), @vl_Visibilite/1000.0)
				insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
				values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
			end
		end

	end

/************************************************************************************************************************/
/**                                         Cas d'un evenement de classe Nature                                        **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClasseNature
	begin
		/* recherche des informations sur la nature de l'obstacle */
		select
			@vl_nature_obstacle = nature
		from HIS..FMC_NAT
		where   numero = @va_NumEvt_in and cle = @va_CleEvt_in
		having horodate = min(horodate)
		select @vl_Ligne = XZAEC_SYN_PAS_DATE +
				XZAEC_SYN_NATURE + @vl_nature_obstacle
		insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
	end

/************************************************************************************************************************/
/**                                               Cas d'un evenement Echangeur                                         **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClasseEchangeur
	begin
		/* recherche les informations sur la fermeture d'un echangeur */
		exec @vl_Status = XZAE;52 @vl_HoroValidation, @va_NumEvt_in, @va_CleEvt_in,
					@vl_EntreeS output, @vl_EntreeN output,
					@vl_SortieS output, @vl_SortieN output
		if @vl_Status != XDC_OK
			return XZAEC_FMC_PAS_DONNEES

		/* Ajout des informations dans la synthese */
		if @vl_NumTypeFmc = XZAEC_FMC_EchDeconseille
		begin
			if @vl_EntreeN = XDC_OUI
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ENT_N_DECONS
				insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
				values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
			end
			if @vl_EntreeS = XDC_OUI
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ENT_S_DECONS
				insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
				values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
			end
			if @vl_SortieN = XDC_OUI
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_SOR_N_DECONS
				insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
				values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
			end
			if @vl_SortieS = XDC_OUI
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_SOR_S_DECONS
				insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
				values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
			end
		end
		else
		begin
			if @vl_EntreeN = XDC_OUI
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
						   XZAEC_SYN_ENT_N_FERM
			else	select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_ENT_N_OUV
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

			if @vl_EntreeS = XDC_OUI
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
						   XZAEC_SYN_ENT_S_FERM
			else	select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_ENT_S_OUV
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

			if @vl_SortieN = XDC_OUI
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
						   XZAEC_SYN_SOR_N_FERM
			else	select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_SOR_N_OUV
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

			if @vl_SortieS = XDC_OUI
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
						   XZAEC_SYN_SOR_S_FERM
			else	select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_SOR_S_OUV
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
	end

/************************************************************************************************************************/
/**                                       Cas d'un Incident sous-concessionnaire                                       **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClasseSousConcess
	begin
		/* recherche des informations du concessionnaire */
		exec @vl_Status = XZAE;50 @va_NumEvt_in, @va_CleEvt_in, @vl_HoroValidation,
					@vl_NomCnc output, @vl_CncFerme output,
					@vl_Penurie output, @vl_Carburant output
		if @vl_Status != XDC_OK
			return XZAEC_FMC_PAS_DONNEES

		if @vl_NomCnc != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_CNC + @vl_NomCnc
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_Penurie = XDC_OUI
		begin
			/* recherche le nom du carburant manquant */
			select @vl_NomCarburant = nom from CFG..TYP_CRB
			where numero = @vl_Carburant

			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_PEN_CRB + @vl_NomCarburant
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_CncFerme = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_CNC_FERM
			insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

	end

/************************************************************************************************************************/
/**                                     Cas d'un evenement de classe Manifestation                                     **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClasseManif
	begin
		/* recherche les informations sur la manifestation */
		exec @vl_Status = XZAE;47 @vl_HoroValidation, @va_NumEvt_in, @va_CleEvt_in,
					@vl_Nombre output, 
					@vl_Perception output,
					@vl_Violence output
		if @vl_Status != XDC_OK
			return XZAEC_FMC_PAS_DONNEES

		/* Ajout des informations dans la synthese */
		if @vl_Nombre != null
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_NBR_MNF + 
					convert(varchar(8), @vl_Nombre)
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
		if @vl_Violence = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_VIO_MNF 
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
		if @vl_Perception = XZAEC_FMC_PAS_PER
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_PAS_PER_PEA
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

	end

/************************************************************************************************************************/
/**                                               Cas d'un Bouchon (tete)                                              **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClasseGenerique and 
		@vl_NumTypeFmc = XZAEC_FMC_TeteBouchon
	begin
		/* ajout de l'intitule de la tete de bouchon */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_TET + 
					' ' + @vl_Localisation
		insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

		/* recherche les informations sur le bouchon */
		exec @vl_Status = XZAE;51 @vl_HoroValidation, @va_NumEvt_in,
					@va_CleEvt_in, @vl_PR output,
					@vl_TypeBouchon output, 
					@vl_Longueur output,
					@vl_NumEvtQueue output, 
					@vl_CleEvtQueue output
		if @vl_Status != XDC_OK
			return XZAEC_FMC_PAS_DONNEES


		/* Creation de l'intitule d'une creation de FMC de queue */
		/* de bouchon                                            */
		select 
			@vl_HoroValidation = HIS..FMC_HIS.horodate_validation,
			@vl_Autoroute	= HIS..FMC_HIS.autoroute,
			@vl_PR		= HIS..FMC_HIS.PR,
			@vl_Sens	= HIS..FMC_HIS.sens
		from HIS..FMC_HIS
		where	HIS..FMC_HIS.numero = @vl_NumEvtQueue and 
			HIS..FMC_HIS.cle = @vl_CleEvtQueue
		group by HIS..FMC_HIS.numero, HIS..FMC_HIS.cle
		having HIS..FMC_HIS.horodate_validation = 
				min(HIS..FMC_HIS.horodate_validation)

		if @@rowcount != 1
			return XZAEC_FMC_ERR_VAL

		/* memoriser la localisation initiale de la queue pour les actions */
		select @vl_Autoroute_Prec = @vl_Autoroute, @vl_PR_Prec = @vl_PR, @vl_Sens_Prec = @vl_Sens

		/* le type pour un bouchon est change pour l'affichage */
		/* (libelle plus explicite)                            */
		select @vl_TypeFmc = XZAEC_SYN_FMC_QueueBouchon

		/* mise en forme de l'intitule de la queue de bouchon */
		exec XZAE83	null, null, null, null,
				@vl_Autoroute, @vl_PR, @vl_Sens, 
				@vl_Intitule2 output, @vl_Localisation2 output

		/* ajout de l'intitule de la queue de bouchon */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_QUE + 
					@vl_Localisation2
		insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

		/* ajout de la longueur du bouchon */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_LON +
					convert(varchar(8), @vl_Longueur/1000.0)

		insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

		/* ajout du type du bouchon */
		if @vl_TypeBouchon = XZAEC_BOU_Dense
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					   XZAEC_SYN_BOU_DNS
		else if @vl_TypeBouchon = XZAEC_BOU_Ralenti
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					   XZAEC_SYN_BOU_RAL
		else if @vl_TypeBouchon = XZAEC_BOU_Accordeon
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					   XZAEC_SYN_BOU_ACD
		else if @vl_TypeBouchon = XZAEC_BOU_Bouche
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					   XZAEC_SYN_BOU_BOU
		else if @vl_TypeBouchon = XZAEC_BOU_Bloque
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					   XZAEC_SYN_BOU_BLQ
		else return XZAEC_FMC_ERR_DONNEES

		insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

		/* Ajouter dans l'historique de la fmc celui de la */
		/* queue de bouchon ordonne suivant la date        */
		/* sans l'enregistrement correspondant a la creation*/
		/* de la fmc de queue de bouchon                    */
		insert #TEMPO_HISTORIQUE_FMC
			(numero, cle, classe, type, nom_type, type_anterieur, autoroute,
			PR, sens, confirme_ou_signale, horodate_validation,
			point_caracteristique, numero_point_caracteristique,
			critere_ordre, VR, VM2, VM1, VL, BAU, VR_I, 
			VM2_I, VM1_I, VL_I, BAU_I, BAU_etroite, BAU_I_etroite)
		select 
			HIS..FMC_HIS.numero,
			HIS..FMC_HIS.cle,
			CFG..TYP_FMC.classe,
			HIS..FMC_HIS.type_FMC_valide,
			CFG..TYP_FMC.nom,
			HIS..FMC_HIS.type_anterieur,
			HIS..FMC_HIS.autoroute,
			HIS..FMC_HIS.PR,
			HIS..FMC_HIS.sens,
			HIS..FMC_HIS.confirme_ou_signale,
			HIS..FMC_HIS.horodate_validation,
			HIS..FMC_HIS.point_caracteristique,
			HIS..FMC_HIS.numero_point_caracteristique,
			2,
			isnull(HIS..FMC_HIS.VR, XDC_VOIE_INCONNUE),
			isnull(HIS..FMC_HIS.VM2, XDC_VOIE_INCONNUE),
			isnull(HIS..FMC_HIS.VM1, XDC_VOIE_INCONNUE),
			isnull(HIS..FMC_HIS.VL, XDC_VOIE_INCONNUE),
			isnull(HIS..FMC_HIS.BAU, XDC_VOIE_INCONNUE),
			isnull(HIS..FMC_HIS.VR_I, XDC_VOIE_INCONNUE),
			isnull(HIS..FMC_HIS.VM2_I, XDC_VOIE_INCONNUE),
			isnull(HIS..FMC_HIS.VM1_I, XDC_VOIE_INCONNUE),
			isnull(HIS..FMC_HIS.VL_I, XDC_VOIE_INCONNUE),
			isnull(HIS..FMC_HIS.BAU_I, XDC_VOIE_INCONNUE),
			isnull(HIS..FMC_HIS.BAU_etroite, XDC_FAUX),
			isnull(HIS..FMC_HIS.BAU_I_etroite, XDC_FAUX)
		from HIS..FMC_HIS, CFG..TYP_FMC
		where	HIS..FMC_HIS.numero = @vl_NumEvtQueue and
			HIS..FMC_HIS.cle = @vl_CleEvtQueue and
			CFG..TYP_FMC.numero = HIS..FMC_HIS.type_FMC_valide and
			HIS..FMC_HIS.horodate_validation != @vl_HoroValidation
			order by HIS..FMC_HIS.horodate_validation

		/* reordonner l'historique complet suivant la date */
		/* et les histo des fmc tete de bouchon avant      */
		/* ceux des fmc queue de bouchon pour une date     */
		/* identique                                       */
		insert #TEMPO_HISTORIQUE_FMC
		select * from #HISTORIQUE_FMC

		delete #HISTORIQUE_FMC
		insert #HISTORIQUE_FMC
		select * from #TEMPO_HISTORIQUE_FMC
		order by horodate_validation, critere_ordre

	end

/************************************************************************************************************************/
/**                                               Cas d'un Bouchon (queue)                                             **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClasseBouchon
	begin
		/* recherche les informations sur le bouchon */
		exec @vl_Status = XZAE;51 @vl_HoroValidation, @va_NumEvt_in,
					@va_CleEvt_in, @vl_PR output,
					@vl_TypeBouchon output, 
					@vl_Longueur output,
					@vl_NumEvtTete output, 
					@vl_CleEvtTete output

		if @vl_Status != XDC_OK
			return XZAEC_FMC_PAS_DONNEES

		/* Rechercher la cause reelle du bouchon */
		select
			@vl_Cause	= cause,
			@vl_CleCause	= cle_cause
		from HIS..FMC_GEN
		where numero = @vl_NumEvtTete and cle = @vl_CleEvtTete

		/* Creation de l'intitule de la creation de FMC de tete */
		select 
			@vl_HoroValidation	= HIS..FMC_HIS.horodate_validation,
			@vl_Autoroute		= HIS..FMC_HIS.autoroute,
			@vl_PR			= HIS..FMC_HIS.PR,
			@vl_Sens		= HIS..FMC_HIS.sens,
			@vl_PointCarTete	= HIS..FMC_HIS.point_caracteristique,
			@vl_NumPointCarTete	= HIS..FMC_HIS.numero_point_caracteristique
		from HIS..FMC_HIS
		where	HIS..FMC_HIS.numero = @vl_NumEvtTete and 
			HIS..FMC_HIS.cle = @vl_CleEvtTete 
		group by HIS..FMC_HIS.numero, HIS..FMC_HIS.cle
		having horodate_validation = min(horodate_validation)

		if @@rowcount != 1
			return XZAEC_FMC_ERR_VAL

		/* le type pour un bouchon est change pour l'affichage */
		/* (libelle plus explicite)                            */
		select @vl_TypeFmc = XZAEC_SYN_FMC_TeteBouchon

		/* determination de la localisation de la tete de bouchon */
		if @vl_PointCarTete = null  or  @vl_NumPointCarTete = null
		begin
			exec XZAE83	null, null, null, null,
					@vl_Autoroute, @vl_PR, @vl_Sens, 
					@vl_Intitule2 output, @vl_Localisation2 output

			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_TET + 
			                      @vl_Localisation2
		end
		else
		begin
			if @vl_PointCarTete = XDC_POINT_CARACT_AIRE
			begin
				select @vl_Libelle = XZAEC_SYN_VEH_AIR

				select @vl_NomPointCarTete = nom
				from CFG..RES_AIR
				where numero = @vl_NumPointCarTete
			end
			else if @vl_PointCarTete = XDC_POINT_CARACT_ECHANGEUR
			begin
				select @vl_Libelle = XZAEC_SYN_VEH_ECH

				select @vl_NomPointCarTete = nom
				from CFG..RES_ECH
				where numero = @vl_NumPointCarTete
			end
			else if @vl_PointCarTete = XDC_POINT_CARACT_PEAGE
			begin
				select @vl_Libelle = XZAEC_SYN_VEH_PEA

				select @vl_NomPointCarTete = nom
				from CFG..RES_PEA
				where numero = @vl_NumPointCarTete
			end
			else if @vl_PointCarTete = XDC_POINT_CARACT_LIT_ARRET
			begin
				select @vl_Libelle = XZAEC_SYN_VEH_LIT

				select @vl_NomPointCarTete = nom
				from CFG..EQT_GEN
				where	numero = @vl_NumPointCarTete and 
					type = XDC_CAPT_LIT_ARRET
			end
			else return XZAEC_FMC_LOC_INC

			if @@rowcount != 1
				return XZAEC_FMC_LOC_INC

			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_TET + 
			                      @vl_Libelle + @vl_NomPointCarTete
		end

		/* ajout de l'intitule de la tete de bouchon */
		insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

		/* ajout de l'intitule de la queue de bouchon */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_QUE + 
					' ' + @vl_Localisation
		insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

		/* ajout de la longueur du bouchon */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_LON +
					convert(varchar(8), @vl_Longueur/1000.0)

		insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

		/* ajout du type du bouchon */
		if @vl_TypeBouchon = XZAEC_BOU_Dense
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					   XZAEC_SYN_BOU_DNS
		else if @vl_TypeBouchon = XZAEC_BOU_Accordeon
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					   XZAEC_SYN_BOU_ACD
		else if @vl_TypeBouchon = XZAEC_BOU_Ralenti
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					   XZAEC_SYN_BOU_RAL
		else if @vl_TypeBouchon = XZAEC_BOU_Bouche
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					   XZAEC_SYN_BOU_BOU
		else if @vl_TypeBouchon = XZAEC_BOU_Bloque
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					   XZAEC_SYN_BOU_BLQ
		else return XZAEC_FMC_ERR_DONNEES

		insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

		/* Ajouter dans l'historique de la fmc celui de la */
		/* tete de bouchon ordonne suivant la date         */
		/* sans l'enregistrement correspondant a la creation*/
		/* de la fmc de tete de bouchon                     */
		insert #TEMPO_HISTORIQUE_FMC
			(numero, cle, classe, type, nom_type, type_anterieur, autoroute,
			PR, sens, confirme_ou_signale, horodate_validation,
			point_caracteristique, numero_point_caracteristique,
			critere_ordre, VR, VM2, VM1, VL, BAU,
			VR_I, VM2_I, VM1_I, VL_I, BAU_I, BAU_etroite, BAU_I_etroite)
		select 
			HIS..FMC_HIS.numero,
			HIS..FMC_HIS.cle,
			CFG..TYP_FMC.classe,
			HIS..FMC_HIS.type_FMC_valide,
			CFG..TYP_FMC.nom,
			HIS..FMC_HIS.type_anterieur,
			HIS..FMC_HIS.autoroute,
			HIS..FMC_HIS.PR,
			HIS..FMC_HIS.sens,
			@vl_ConfirmeSignale,
			HIS..FMC_HIS.horodate_validation,
			HIS..FMC_HIS.point_caracteristique,
			HIS..FMC_HIS.numero_point_caracteristique,
			2,
			isnull(HIS..FMC_HIS.VR, XDC_VOIE_INCONNUE),
			isnull(HIS..FMC_HIS.VM2, XDC_VOIE_INCONNUE),
			isnull(HIS..FMC_HIS.VM1, XDC_VOIE_INCONNUE),
			isnull(HIS..FMC_HIS.VL, XDC_VOIE_INCONNUE),
			isnull(HIS..FMC_HIS.BAU, XDC_VOIE_INCONNUE),
			isnull(HIS..FMC_HIS.VR_I, XDC_VOIE_INCONNUE),
			isnull(HIS..FMC_HIS.VM2_I, XDC_VOIE_INCONNUE),
			isnull(HIS..FMC_HIS.VM1_I, XDC_VOIE_INCONNUE),
			isnull(HIS..FMC_HIS.VL_I, XDC_VOIE_INCONNUE),
			isnull(HIS..FMC_HIS.BAU_I, XDC_VOIE_INCONNUE),
			isnull(HIS..FMC_HIS.BAU_etroite, XDC_FAUX),
			isnull(HIS..FMC_HIS.BAU_I_etroite, XDC_FAUX)
		from HIS..FMC_HIS, CFG..TYP_FMC
		where	HIS..FMC_HIS.numero = @vl_NumEvtTete and 
			HIS..FMC_HIS.cle = @vl_CleEvtTete and
			CFG..TYP_FMC.numero = HIS..FMC_HIS.type_FMC_valide and
			HIS..FMC_HIS.horodate_validation != @vl_HoroValidation
			order by HIS..FMC_HIS.horodate_validation

		/* reordonner l'historique complet suivant la date */
		/* et les histo des fmc tete de bouchon avant      */
		/* ceux des fmc queue de bouchon pour une date     */
		/* identique                                       */
		insert #TEMPO_HISTORIQUE_FMC
		select * from #HISTORIQUE_FMC

		delete #HISTORIQUE_FMC
		insert #HISTORIQUE_FMC
		select * from #TEMPO_HISTORIQUE_FMC
		order by horodate_validation, critere_ordre

		/* positionner des var internes */
		select @vl_NumEvtQueue = @va_NumEvt_in, @vl_CleEvtQueue = @va_CleEvt_in
	end

/************************************************************************************************************************/
/**                                               Cas de Travaux                                                       **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc in (XZAEC_ClasseTravaux, XZAEC_ClasseBasculTrav)
	begin
		/* recherche les informations sur les travaux */
		select
			@vl_NatureTrv	= HIS..FMC_TRF.nature,
			@vl_TypeTrv	= HIS..FMC_TRF.type,
			@vl_Continu	= HIS..FMC_TRF.continu,
			@vl_DebutPlanifie = HIS..FMC_TRF.horodate_debut_planifiee,
			@vl_FinPlanifie = HIS..FMC_TRF.horodate_fin_planifiee,
			@vl_Mobile	= HIS..FMC_TRF.mobile,
			@vl_AutorouteM	= HIS..FMC_TRF.autoroute_planifiee,
			@vl_PRQueueM	= HIS..FMC_TRF.PR_queue_planifie,
			@vl_PRTeteM	= HIS..FMC_TRF.PR_tete_planifie,
			@vl_SensM	= HIS..FMC_TRF.sens_planifie,
			@vl_PR		= HIS..FMC_TRH.localisation_tete,
			@vl_VoiesRet	= HIS..FMC_TRH.voies_retrecies,
			@vl_PresenceBouchon = HIS..FMC_TRH.bouchon,
			@vl_VR=VR,
			@vl_VM2=VM2,
			@vl_VM1=VM1,
			@vl_VL=VL,
			@vl_BAU=BAU,
			@vl_VR_I=VR_I,
			@vl_VM2_I=VM2_I,
			@vl_VM1_I=VM1_I,
			@vl_VL_I=VL_I,
			@vl_BAU_I=BAU_I,
			@vl_PRdebut_VR=PR_debut_VR,
			@vl_PRfin_VR=PR_fin_VR,
			@vl_debut_VR=debut_VR,
			@vl_fin_VR=fin_VR,
			@vl_PRdebut_VM2=PR_debut_VM2,
			@vl_PRfin_VM2=PR_fin_VM2,
			@vl_debut_VM2=debut_VM2,
			@vl_fin_VM2=fin_VM2,
			@vl_PRdebut_VM1=PR_debut_VM1,
			@vl_PRfin_VM1=PR_fin_VM1,
			@vl_debut_VM1=debut_VM1,
			@vl_fin_VM1=fin_VM1,
			@vl_PRdebut_VL=PR_debut_VL,
			@vl_PRfin_VL=PR_fin_VL,
			@vl_debut_VL=debut_VL,
			@vl_fin_VL=fin_VL,
			@vl_PRdebut_BAU=PR_debut_BAU,
			@vl_PRfin_BAU=PR_fin_BAU,
			@vl_debut_BAU=debut_BAU,
			@vl_fin_BAU=fin_BAU,
			@vl_PRdebut_VR_I=PR_debut_VR_I,
			@vl_PRfin_VR_I=PR_fin_VR_I,
			@vl_debut_VR_I=debut_VR_I,
			@vl_fin_VR_I=fin_VR_I,
			@vl_PRdebut_VM2_I=PR_debut_VM2_I,
			@vl_PRfin_VM2_I=PR_fin_VM2_I,
			@vl_debut_VM2_I=debut_VM2_I,
			@vl_fin_VM2_I=fin_VM2_I,
			@vl_PRdebut_VM1_I=PR_debut_VM1_I,
			@vl_PRfin_VM1_I=PR_fin_VM1_I,
			@vl_debut_VM1_I=debut_VM1_I,
			@vl_fin_VM1_I=fin_VM1_I,
			@vl_PRdebut_VL_I=PR_debut_VL_I,
			@vl_PRfin_VL_I=PR_fin_VL_I,
			@vl_debut_VL_I=debut_VL_I,
			@vl_fin_VL_I=fin_VL_I,
			@vl_PRdebut_BAU_I=PR_debut_BAU_I,
			@vl_PRfin_BAU_I=PR_fin_BAU_I,
			@vl_debut_BAU_I=debut_BAU_I,
			@vl_fin_BAU_I=fin_BAU_I,
			@vl_vitesse = HIS..FMC_TRH.vitesse,
			@vl_personnel = HIS..FMC_TRH.personnel
		from HIS..FMC_TRF, HIS..FMC_TRH
		where	HIS..FMC_TRF.numero = @va_NumEvt_in and 
			HIS..FMC_TRF.cle = @va_CleEvt_in and
			HIS..FMC_TRH.numero = @va_NumEvt_in and 
			HIS..FMC_TRH.cle = @va_CleEvt_in  and
			HIS..FMC_TRH.horodate = @vl_HoroValidation

		if @@rowcount != 1
			return XZAEC_FMC_PAS_DONNEES

		if @vl_NumTypeFmc != XZAEC_FMC_ZoneStockage
		begin
			/* Ajout des informations sur les travaux dans la synthese */
			if @vl_TypeTrv != null
			begin
				if @vl_TypeTrv = XZAEC_TRV_Glissieres
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_GLI
				else if @vl_TypeTrv = XZAEC_TRV_Vegetation
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_VEG
				else if @vl_TypeTrv = XZAEC_TRV_OuvHydro
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_HYD
				else if @vl_TypeTrv = XZAEC_TRV_OuvArt
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_ART
				else if @vl_TypeTrv = XZAEC_TRV_Terrass
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_TER
				else if @vl_TypeTrv = XZAEC_TRV_SignalVert
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_SGV
				else if @vl_TypeTrv = XZAEC_TRV_SignalHori
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_SGH
				else if @vl_TypeTrv = XZAEC_TRV_Eclairage
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_ECL
				else if @vl_TypeTrv = XZAEC_TRV_CableRAU
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_RAU
				else if @vl_TypeTrv = XZAEC_TRV_LavBalTun
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_LBT
				else if @vl_TypeTrv = XZAEC_TRV_LavBalAutre
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_LBA
				else if @vl_TypeTrv = XZAEC_TRV_MaintTun
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_MTN
				else if @vl_TypeTrv = XZAEC_TRV_MaintAutre
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_MAU
				else if @vl_TypeTrv = XZAEC_TRV_Elargiss
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_ELA
				else if @vl_TypeTrv = XZAEC_TRV_MurAntibruit
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_MUR
				else if @vl_TypeTrv = XZAEC_TRV_ConfGliss
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_CGL
				else if @vl_TypeTrv = XZAEC_TRV_Autre
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_AUT
				else if @vl_TypeTrv = XZAEC_TRV_Enrobes
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_ENR
				else if @vl_TypeTrv = XZAEC_TRV_MaintVideo
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_MTV
				else if @vl_TypeTrv = XZAEC_TRV_PAU
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_PAU
				else if @vl_TypeTrv = XZAEC_TRV_Cloture
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_CLO
				else if @vl_TypeTrv = XZAEC_TRV_FO
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_FON
				else if @vl_TypeTrv = XZAEC_TRV_Accident
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_ACC
				else if @vl_TypeTrv = XZAEC_TRV_Stockage
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_STO
				else if @vl_TypeTrv = XZAEC_TRV_Tunnels
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_TUN

				else	return XZAEC_FMC_ERR_DONNEES

				insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end

			if @vl_NatureTrv = XZAEC_FMC_TRV_NEU
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_TRV_NEU
			else	select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
						   XZAEC_SYN_TRV_ENT

			insert HIS..FMC_LIS(numero,cle,ligne, heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

			if @vl_Mobile = XDC_OUI
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_TRV_MOB
				insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end

			if @vl_DebutPlanifie != null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_DEB_PLA + 
						   convert(char(10), @vl_DebutPlanifie, 103) + ' ' +
						   convert(char(8), @vl_DebutPlanifie, 8)

				insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end

			if @vl_FinPlanifie != null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_FIN_PLA +  
						   convert(char(10), @vl_FinPlanifie, 103) + ' ' +
						   convert(char(8), @vl_FinPlanifie, 8)

				insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end

			/* localisation des travaux planifies */
			if @vl_AutorouteM != null and @vl_Mobile = XDC_OUI and
			   @vl_PRTeteM is not null
			begin
				/* mise en forme de la localisation de la tete */
				/* des travaux planifies */
				exec XZAE83	null, null, null, null,
						@vl_AutorouteM, @vl_PRTeteM, @vl_SensM, 
						@vl_Intitule2 output, @vl_Localisation2 output

				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_TET_PLA + @vl_Localisation2
				insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

				/* mise en forme de la localisation de la tete */
				/* des travaux planifies */
				exec XZAE83	null, null, null, null,
						@vl_AutorouteM, @vl_PRQueueM, @vl_SensM, 
						@vl_Intitule2 output, @vl_Localisation2 output

				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_QUE_PLA + @vl_Localisation2
				insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end
		end


		if @vl_PR != null
		begin
			if @vl_NumTypeFmc = XZAEC_FMC_ZoneStockage
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_STK_PRT + 
						   convert(char(7), @vl_PR/1000.0)
			else
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_PR_TET + 
						   convert(char(7), @vl_PR/1000.0)
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

		if @vl_NumTypeFmc = XZAEC_FMC_ZoneStockage
		begin
			if @vl_VoiesRet = XDC_OUI
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_STK_ENCOURS
			else
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_STK_PREPAR
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		else
		if @vl_VoiesRet = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_VOI_RET 
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

		/*configuration finale des voies*/
		if (@vl_VR <> null ) and (@vl_VM2 <> null ) and (@vl_VM1 <> null ) and (@vl_VL <> null ) and (@vl_BAU <> null ) and (@vl_VR_I <> null ) and (@vl_VM2_I <> null ) and (@vl_VM1_I <> null ) and (@vl_VL_I <> null ) and (@vl_BAU_I <> null ) 
		begin
			if @vl_ClasseFmc = XZAEC_ClasseBasculTrav
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_CFG_VOIES_BAS_TRV
			else
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_CFG_VOIES_BAS
			insert HIS..FMC_LIS
			values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null, @vl_VR, @vl_VM2, @vl_VM1,
			@vl_VL, @vl_BAU, @vl_VR_I, @vl_VM2_I, @vl_VM1_I,@vl_VL_I, @vl_BAU_I, 
			XDC_FAUX, XDC_FAUX, null,@va_NumPoste_in)
	
			select @vl_Ligne=XZAEC_SYN_PAS_DATE
			insert HIS..FMC_LIS (numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
	
			select @vl_Ligne=XZAEC_SYN_PAS_DATE + convert(char(10),XZAEC_SYN_VOIE_BAS) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert (char(8), XZAEC_SYN_PRD_BAS) + XZAEC_SYN_CAR_SEPAR+
					convert (char(8),XZAEC_SYN_PRF_BAS) + XZAEC_SYN_CAR_SEPAR +
					convert (char(16),XZAEC_SYN_HD_BAS) +XZAEC_SYN_CAR_SEPAR+
					convert (char(16),XZAEC_SYN_HF_BAS)
			insert HIS..FMC_LIS (numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end 

		/*tableau des neutralisations de voies*/
		if @vl_PRdebut_VR<>null
		begin
			select @vl_Contenu=libelle 
			from CFG..LIB_PRE
			where notyp = XDC_LIB_TYPE_VOIE and code = XDC_CODE_VOIE_VR
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + convert(char(10),@vl_Contenu) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRdebut_VR/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR+
					convert(char(8),@vl_PRfin_VR/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert (char(16),convert(char(10), @vl_debut_VR, 103) + ' ' + 
					convert(char(8), @vl_debut_VR, 8))
					+ XZAEC_SYN_CAR_SEPAR +
					convert (char(16),convert(char(10), @vl_fin_VR, 103) + ' ' +
					convert(char(8), @vl_fin_VR, 8))
			insert HIS..FMC_LIS (numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end 

		if @vl_PRdebut_VM2<>null
		begin
			select @vl_Contenu=libelle 
			from CFG..LIB_PRE
			where notyp = XDC_LIB_TYPE_VOIE and code = XDC_CODE_VOIE_VM2
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + convert(char(10),@vl_Contenu) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRdebut_VM2/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRfin_VM2/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert (char(16),convert(char(10), @vl_debut_VM2, 103) + ' ' + 
					convert(char(8), @vl_debut_VM2, 8)) 
					+ XZAEC_SYN_CAR_SEPAR+
					convert (char(16),convert(char(10), @vl_fin_VM2, 103) + ' ' +
					convert(char(8), @vl_fin_VM2, 8))
			insert HIS..FMC_LIS (numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end 
		
		if @vl_PRdebut_VM1<>null
		begin
			select @vl_Contenu=libelle 
			from CFG..LIB_PRE
			where notyp = XDC_LIB_TYPE_VOIE and code = XDC_CODE_VOIE_VM1
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + convert(char(10),@vl_Contenu) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRdebut_VM1/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRfin_VM1/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert (char(16),convert(char(10), @vl_debut_VM1, 103) + ' ' + 
					convert(char(8), @vl_debut_VM1, 8))
					+ XZAEC_SYN_CAR_SEPAR +
					convert (char(16),convert(char(10), @vl_fin_VM1, 103) + ' ' +
					convert(char(8), @vl_fin_VM1, 8))
			insert HIS..FMC_LIS (numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end 

		if @vl_PRdebut_VL<>null
		begin
			select @vl_Contenu=libelle 
			from CFG..LIB_PRE
			where notyp = XDC_LIB_TYPE_VOIE and code = XDC_CODE_VOIE_VL
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + convert(char(10),@vl_Contenu) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRdebut_VL/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRfin_VL/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert (char(16), convert(char(10), @vl_debut_VL, 103) + ' ' + 
							convert(char(8), @vl_debut_VL, 8)) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert (char(16), convert(char(10), @vl_fin_VL, 103) + ' ' +
							convert(char(8), @vl_fin_VL, 8))
			insert HIS..FMC_LIS (numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end 

		if @vl_PRdebut_BAU<>null
		begin
			select @vl_Contenu=libelle 
			from CFG..LIB_PRE
			where notyp = XDC_LIB_TYPE_VOIE and code = XDC_CODE_VOIE_BAU
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + convert(char(10),@vl_Contenu) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRdebut_BAU/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRfin_BAU/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert (char(16), convert(char(10), @vl_debut_BAU, 103) + ' ' + 
					convert(char(8), @vl_debut_BAU, 8)) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert (char(16),convert(char(10), @vl_fin_BAU, 103) + ' ' +
					convert(char(8), @vl_fin_VR, 8))
			insert HIS..FMC_LIS (numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end 

		if @vl_PRdebut_VR_I<>null
		begin
			select @vl_Contenu=libelle 
			from CFG..LIB_PRE
			where notyp = XDC_LIB_TYPE_VOIE and code = XDC_CODE_VOIE_VR_I
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + convert(char(10),@vl_Contenu) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRdebut_VR_I/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRfin_VR_I/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert (char(16),convert(char(10), @vl_debut_VR_I, 103) + ' ' + 
					convert(char(8), @vl_debut_VR_I, 8))
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(16),convert(char(10), @vl_fin_VR_I, 103) + ' ' +
					convert(char(8), @vl_fin_VR_I, 8))
			insert HIS..FMC_LIS (numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end 

		if @vl_PRdebut_VM2_I<>null
		begin
			select @vl_Contenu=libelle 
			from CFG..LIB_PRE
			where notyp = XDC_LIB_TYPE_VOIE and code = XDC_CODE_VOIE_VM2_I
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + convert(char(10),@vl_Contenu) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRdebut_VM2_I/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRfin_VM2_I/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert (char(16),convert(char(10), @vl_debut_VM2_I, 103) + ' ' + 
					convert(char(8), @vl_debut_VM2_I, 8))
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(16),convert(char(10), @vl_fin_VM2_I, 103) + ' ' +
					convert(char(8), @vl_fin_VM2_I, 8))
			insert HIS..FMC_LIS (numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end 

		if @vl_PRdebut_VM1_I<>null
		begin
			select @vl_Contenu=libelle 
			from CFG..LIB_PRE
			where notyp = XDC_LIB_TYPE_VOIE and code = XDC_CODE_VOIE_VM1_I
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + convert(char(10),@vl_Contenu) 
					+ XZAEC_SYN_CAR_SEPAR+
					convert(char(8),@vl_PRdebut_VM1_I/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRfin_VM1_I/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(16),convert(char(10), @vl_debut_VM1_I, 103) + ' ' + 
					convert(char(8), @vl_debut_VM1_I, 8))
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(16),convert(char(10), @vl_fin_VM1_I, 103) + ' ' +
					convert(char(8), @vl_fin_VM1_I, 8))
			insert HIS..FMC_LIS (numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end 

		if @vl_PRdebut_VL_I<>null
		begin
			select @vl_Contenu=libelle 
			from CFG..LIB_PRE
			where notyp = XDC_LIB_TYPE_VOIE and code = XDC_CODE_VOIE_VL_I
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + convert(char(10),@vl_Contenu) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRdebut_VL_I/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRfin_VL_I/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(16),convert(char(10), @vl_debut_VL_I, 103) + ' ' + 
					convert(char(8), @vl_debut_VL_I, 8))
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(16),convert(char(10), @vl_fin_VL_I, 103) + ' ' +
					convert(char(8), @vl_fin_VL_I, 8))
			insert HIS..FMC_LIS (numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end 

		if @vl_PRdebut_BAU_I<>null
		begin
			select @vl_Contenu=libelle 
			from CFG..LIB_PRE
			where notyp = XDC_LIB_TYPE_VOIE and code = XDC_CODE_VOIE_BAU_I
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + convert(char(10),@vl_Contenu) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRdebut_BAU_I/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRfin_BAU_I/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(16),convert(char(10), @vl_debut_BAU_I, 103) + ' ' + 
					convert(char(8), @vl_debut_BAU_I, 8))
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(16),convert(char(10), @vl_fin_BAU_I, 103) + ' ' +
					convert(char(8), @vl_fin_BAU_I, 8))
			insert HIS..FMC_LIS (numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end


		/* ajouter les configurations de voies du Basculement pour travaux */
		if @vl_ClasseFmc = XZAEC_ClasseBasculTrav
		begin
			select @vl_Ligne=XZAEC_SYN_PAS_DATE
			insert HIS..FMC_LIS (numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in, @vl_Ligne, null, @va_NumPoste_in)

			select @vl_VR=VR, @vl_VM2=VM2, @vl_VM1=VM1, @vl_VL=VL, @vl_BAU=BAU,
			       @vl_VR_I=VR_I, @vl_VM2_I=VM2_I, @vl_VM1_I=VM1_I, @vl_VL_I=VL_I, @vl_BAU_I=BAU_I
			from HIS..FMC_BAS
			where numero = @va_NumEvt_in and cle = @va_CleEvt_in

			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_CFG_VOIES_BAS
			insert HIS..FMC_LIS
			values ( @va_NumEvt_in, @va_CleEvt_in, @vl_Ligne, null,
			         @vl_VR, @vl_VM2, @vl_VM1, @vl_VL, @vl_BAU,
			         @vl_VR_I, @vl_VM2_I, @vl_VM1_I,@vl_VL_I, @vl_BAU_I,
			         XDC_FAUX, XDC_FAUX, null, @va_NumPoste_in)
		end

		/* Pour un Basculement pour travaux, indiquer la présence d'un bouchon mobile */
		if @vl_NumTypeFmc = XZAEC_FMC_BasculTravaux
		begin
			if @vl_PresenceBouchon = XDC_VRAI
			begin
				select @vl_Ligne=XZAEC_SYN_PAS_DATE
				insert HIS..FMC_LIS (numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in, @vl_Ligne, null, @va_NumPoste_in)

				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_MOBILE
				insert HIS..FMC_LIS (numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in, @vl_Ligne, null, @va_NumPoste_in)
			end
		end
	end

/************************************************************************************************************************/
/**                                               Cas d'un Basculement                                                 **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClasseBasculement
	begin
		/* recherche les informations sur le basculement */
		select
			@vl_VR=VR,
			@vl_VM2=VM2,
			@vl_VM1=VM1,
			@vl_VL=VL,
			@vl_BAU=BAU,
			@vl_VR_I=VR_I,
			@vl_VM2_I=VM2_I,
			@vl_VM1_I=VM1_I,
			@vl_VL_I=VL_I,
			@vl_BAU_I=BAU_I,
			@vl_PR = localisation_PR_fin,
			@vl_vitesse = vitesse,
			@vl_vitesse_opposee = vitesse_opposee
		from HIS..FMC_BAS
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in

		if @vl_PR != null
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_FIN_BAS +
					convert(char(7),@vl_PR/1000.0)
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

		/*configuration finale des voies*/
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_CFG_VOIES_BAS
		insert HIS..FMC_LIS
		values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null, @vl_VR, @vl_VM2, @vl_VM1,
		@vl_VL, @vl_BAU, @vl_VR_I, @vl_VM2_I, @vl_VM1_I,@vl_VL_I, @vl_BAU_I, 
		XDC_FAUX, XDC_FAUX, null,@va_NumPoste_in)
	end

/************************************************************************************************************************/
/**                                            Cas d'un evenement de Operateur                                         **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClassePosteOper
	begin
		/* recherche les informations sur le poste operateur */
		select @vl_Operateur = operateur
		from HIS..FMC_OPR
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in 

		/* ajout l'information sur le poste operateur dans la synthese */
		if @vl_Operateur != null
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_OPR +
						nom
			from CFG..OPR_NOM
			where numero = @vl_Operateur

			if @@rowcount != 1
				return XZAEC_FMC_PAS_DONNEES

			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

	end

/************************************************************************************************************************/
/**                                    Cas d'un evenement de classe Incendie                                           **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClasseIncendie
	begin
		/* recherche les informations sur l'incendie */
		select	@vl_code_nature = code_nature,
			@vl_FeuMaitrise = feu_maitrise,
			@vl_HorodateMaitrise = horodate_maitrise
		from HIS..FMC_INC
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in

		select	@vl_nature_inc = libelle
		from CFG..LIB_PRE
		where	notyp = XDC_LIB_TYPE_NATURE_INCENDIE  and  code = @vl_code_nature

		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_NATURE + @vl_nature_inc
		insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

		if @vl_FeuMaitrise = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_INC_MAITRISE +
					convert(char(10),@vl_HorodateMaitrise,103) + ' ' +
					convert(char(8),@vl_HorodateMaitrise,8)
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
	end

/************************************************************************************************************************/
/**                                               Cas général                                                          **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc != XZAEC_ClasseGenerique
		return XZAEC_FMC_ERR_DONNEES

	/* Ajout des champs datex */
	if @vl_Datex=XDC_VRAI
	begin
		select 
			@vl_gravite=gravite,
			@vl_trafic=trafic,
			@vl_bloquant=bloquant
		from HIS..FMC_DAT
		where numero= @va_NumEvt_in and cle = @va_CleEvt_in
		
		if @vl_gravite=0
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_GRAVITE + XZAEC_SYN_GRAVITE0
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
		else if @vl_gravite=1
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_GRAVITE + XZAEC_SYN_GRAVITE1
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
		else if @vl_gravite=2
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_GRAVITE + XZAEC_SYN_GRAVITE2
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
		else if @vl_gravite=3
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_GRAVITE + XZAEC_SYN_GRAVITE3
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
		
		if @vl_trafic=XDC_RADT_FLUIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_TRAFIC + XZAEC_SYN_TRAFIC_FLUIDE
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
		else if @vl_trafic=XDC_RADT_PRESAT
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_TRAFIC + XZAEC_SYN_TRAFIC_PRESAT
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
		else if @vl_trafic=XDC_RADT_SATURE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_TRAFIC + XZAEC_SYN_TRAFIC_SATURE
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
		
		if @vl_bloquant=XDC_VRAI
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BLOQUANT
		else
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_NONBLOQUANT
		
		insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
	
	end


/************************************************************************************************************************/
/************************************************************************************************************************/
/**                                   Descriptions des validations successives de la FMC                               **/
/************************************************************************************************************************/
/************************************************************************************************************************/
	/*A Recherche les descriptions des validations successives de la FMC */
	declare Pointeur_histo cursor
	for select numero, cle, classe, nom_type, type, type_anterieur,
		autoroute, PR, sens, confirme_ou_signale, horodate_validation,
		VR, VM2, VM1, VL, BAU, VR_I, VM2_I, VM1_I, VL_I, BAU_I, BAU_etroite, BAU_I_etroite
	from #HISTORIQUE_FMC

	open Pointeur_histo
	fetch Pointeur_histo into @vl_NumEvt, @vl_CleEvt, @vl_ClasseFmc,
				@vl_TypeFmc, @vl_NumTypeFmc, @vl_TypeAnterieur, 
				@vl_Autoroute, @vl_PR, @vl_Sens,
				@vl_ConfirmeSignale, @vl_HoroValidation,
				@vl_VR, @vl_VM2, @vl_VM1, @vl_VL, @vl_BAU, 
				@vl_VR_I, @vl_VM2_I, @vl_VM1_I,
				@vl_VL_I, @vl_BAU_I, @vl_BAU_etroite, @vl_BAU_I_etroite
	select @vl_SqlStatusValidation = @@sqlstatus

	while (@vl_SqlStatusValidation = 0)
	begin
		if @vl_IndicateurCreation != XDC_OUI
		begin
			/* le type pour un bouchon est change pour l'affichage */
			/* (libelle plus explicite)                            */
			if @vl_NumTypeFmc = XZAEC_FMC_TeteBouchon
				select @vl_TypeFmc = XZAEC_SYN_FMC_TeteBouchon
			else if @vl_NumTypeFmc = XZAEC_FMC_QueueBouchon
				select @vl_TypeFmc = XZAEC_SYN_FMC_QueueBouchon

			/* Creation de l'intitule d'une validation de FMC */
			exec XZAE83	@vl_NumEvt, @vl_CleEvt, @vl_NumTypeFmc, @vl_TypeFmc, 
					@vl_Autoroute, @vl_PR, @vl_Sens, 
					@vl_Intitule output, @vl_Localisation output
			select @vl_Ligne = convert(char(10), @vl_HoroValidation, 103) +
					' ' + convert(char(8), @vl_HoroValidation, 8) +
					' ' + convert(varchar(60),@vl_Intitule) + ' - ' + XZAEC_SYN_VAL 

			/* s'il y a eu retypage a cette validation, indiquer l'ancien type */
			if @vl_TypeAnterieur != @vl_NumTypeFmc
			begin
				select @vl_Ligne =  @vl_Ligne + ' ' + nom
				from CFG..TYP_FMC
				where numero = @vl_TypeAnterieur
			end

			/*B Ajout de l'intitule d'une validation de FMC */
			/*B dans la synthese */
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, @vl_HoroValidation,@va_NumPoste_in)
	
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + @vl_Localisation
			insert HIS..FMC_LIS 
			values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,
				@vl_VR, @vl_VM2, @vl_VM1, @vl_VL, @vl_BAU,
				@vl_VR_I, @vl_VM2_I, @vl_VM1_I,@vl_VL_I, @vl_BAU_I,
				@vl_BAU_etroite, @vl_BAU_I_etroite, null,@va_NumPoste_in)
	
			if @vl_ConfirmeSignale = XZAEC_FMC_CONFIRMEE
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
						XZAEC_SYN_CONFIRMEE
			else	select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_SIGNALEE
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
	
			/*B Ajout du complement de la validation de */
			/*B FMC dans la synthese                    */
			exec @vl_Status = XZAE205 @vl_NumEvt, @vl_CleEvt, 
						@vl_ClasseFmc, @vl_NumTypeFmc, 
						@vl_HoroValidation,
						@vl_Autoroute, @vl_PR, @vl_Sens,@va_NumPoste_in
	
			if @vl_Status != XDC_OK
				return XZAEC_FMC_ERR_DON_VAL
		end

		/* memoriser les donnees de la validation venant d'etre traitee   */
		/* pour les bouchons seule la localisation de la queue est retenue */
		select @vl_HoroValPrec = @vl_HoroValidation
		if @vl_NumTypeFmc <> XZAEC_FMC_TeteBouchon
			select @vl_Autoroute_Prec = @vl_Autoroute, @vl_PR_Prec = @vl_PR, @vl_Sens_Prec = @vl_Sens

		/* lecture de la validation suivante */
		fetch Pointeur_histo into @vl_NumEvt, @vl_CleEvt, @vl_ClasseFmc,
				@vl_TypeFmc, @vl_NumTypeFmc, @vl_TypeAnterieur,
				@vl_Autoroute, @vl_PR, @vl_Sens, 
				@vl_ConfirmeSignale, @vl_HoroValidation,
				@vl_VR, @vl_VM2, @vl_VM1, @vl_VL, @vl_BAU, 
				@vl_VR_I, @vl_VM2_I, @vl_VM1_I,
				@vl_VL_I, @vl_BAU_I, @vl_BAU_etroite, @vl_BAU_I_etroite

		/* memorisation de l'etat du curseur de validation */
		select @vl_SqlStatusValidation = @@sqlstatus

		/* trace pour DEBUG SEULEMENT (a mettre en commentaire) */
--		if @vl_SqlStatusValidation <> 2
--			print "Validation FMC %1! a %2!, Loc=%3!-%4!-%5! / Precedente a %6!, Loc=%7!-%8!-%9!",
--				@vl_NumEvt, @vl_HoroValidation, @vl_Autoroute, @vl_Sens, @vl_PR,
--				@vl_HoroValPrec, @vl_Autoroute_Prec, @vl_Sens_Prec, @vl_PR_Prec

		/*A Rechercher les consequences de cette fmc */
		if @vl_IndicateurCreation = XDC_OUI
		begin
			if @vl_Cloture is not null
				select @vl_DateRecherche = @vl_Cloture
			else	select @vl_DateRecherche = getdate()
			exec @vl_Status = XZAE;70 @va_NumEvt_in, @va_CleEvt_in, @vl_DateRecherche, XZAEC_MemoriserConseq
		
			select @vl_nb = count(*) from LISTE_CONSEQ
			where spid = @@spid
		
			if @vl_Status = XDC_OK and @vl_nb > 0 
			begin
				select @vl_InitLigne = XZAEC_SYN_CONSEQ, @vl_Date = @vl_Debut
		
				declare Pointeur_conseq cursor
				for select numero, cle, type from LISTE_CONSEQ
				where spid = @@spid
		
				open Pointeur_conseq 
				fetch Pointeur_conseq  into  @vl_NumEvtConseq, @vl_CleEvtConseq, @vl_NumTypeFmcConseq
				while (@@sqlstatus = 0)
				begin
					/* recherche d'une localisation de la consequence */
					select 
						 @vl_Autoroute_conseq = autoroute, 
						 @vl_PR_conseq = PR, 
						 @vl_Sens_conseq = sens, 
						 @vl_TypeFmcConseq = nom
					from HIS..FMC_GEN, HIS..FMC_HIS, CFG..TYP_FMC
					where HIS..FMC_GEN.numero = @vl_NumEvtConseq and HIS..FMC_GEN.cle = @vl_CleEvtConseq and
						 CFG..TYP_FMC.numero =  HIS..FMC_GEN.type and
						 HIS..FMC_HIS.numero = HIS..FMC_GEN.numero and HIS..FMC_HIS.cle = HIS..FMC_GEN.cle and
						 HIS..FMC_HIS.horodate_validation in (	select min(horodate_validation) 
											from HIS..FMC_HIS
											where numero = @vl_NumEvtConseq and 
												cle = @vl_CleEvtConseq)
		
					/* presenter le numero de la queue de bouchon a la place de */
					/* celui de la tete et son type                             */
					if @vl_NumTypeFmcConseq = XZAEC_FMC_TeteBouchon 
					begin
						select @vl_NumEvtConseq = numero from HIS..FMC_GEN
						where cause = @vl_NumEvtConseq and cle_cause = @vl_CleEvtConseq

						select @vl_TypeFmcConseq = nom from CFG..TYP_FMC
						where numero = XZAEC_FMC_QueueBouchon
					end
		
					/* Creation de l'intitule d'une validation de FMC */
					exec XZAE83	@vl_NumEvtConseq, @vl_CleEvtConseq,
							@vl_NumTypeFmcConseq, @vl_TypeFmcConseq,
							@vl_Autoroute_conseq, @vl_PR_conseq, @vl_Sens_conseq, 
							@vl_Intitule output, @vl_Localisation output

					select @vl_Ligne =  @vl_InitLigne + 
					'N' + convert(char(6),@va_NumEvt_in) + ' - ' + convert(char(2), @va_CleEvt_in)+ ' : ' +
					rtrim(@vl_Intitule) + ' ' + @vl_Localisation
			
					insert #SYNTHESE_CONSEQ (numero,cle,ligne, heure, VR, VM2, VM1, VL, BAU,
								VR_I, VM2_I, VM1_I, VL_I, BAU_I, BAU_etroite, BAU_I_etroite,poste)
					values ( @va_NumEvt_in,@va_CleEvt_in,
						@vl_Ligne, @vl_Date, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE,
						XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE,
						XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_FAUX, XDC_FAUX,
						@va_NumPoste_in)
		
					select @vl_InitLigne = XZAEC_SYN_PAS_DATE, @vl_Date = null
					fetch Pointeur_conseq  into  @vl_NumEvtConseq, 
								@vl_CleEvtConseq, @vl_NumTypeFmcConseq
				end
				close Pointeur_conseq
		
				delete LISTE_CONSEQ
				where spid = @@spid

			end
			else
			begin
				select @vl_Ligne = XZAEC_SYN_CONSEQ + 
				'N' + convert(char(6),@va_NumEvt_in) + ' - ' + convert(char(2), @va_CleEvt_in)+ ' : ' +
				 XZAEC_SYN_AUCUNE_CONSEQ
		
				insert #SYNTHESE_CONSEQ (numero,cle,ligne, heure, VR, VM2, VM1, VL, BAU,
							VR_I, VM2_I, VM1_I, VL_I, BAU_I, BAU_etroite, BAU_I_etroite,poste)
				values ( @va_NumEvt_in,@va_CleEvt_in,
					@vl_Ligne, @vl_Debut, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE,
					XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE,
					XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_FAUX, XDC_FAUX,
					@va_NumPoste_in)
			end
			

			/*B Ajout la cause de la FMC */
			if @vl_Cause != null and @vl_CleCause != null
			begin
				/* recherche les informations sur la cause */
				select
					@vl_Autoroute_cause = HIS..FMC_HIS.autoroute,
					@vl_PR_cause = HIS..FMC_HIS.PR,
					@vl_Sens_cause = HIS..FMC_HIS.sens,
					@vl_NumTypeCause = CFG..TYP_FMC.numero,
					@vl_TypeCause = CFG..TYP_FMC.nom
				from	HIS..FMC_GEN, HIS..FMC_HIS, CFG..TYP_FMC
				where	HIS..FMC_GEN.numero = @vl_Cause and 
					HIS..FMC_GEN.cle = @vl_CleCause and
					HIS..FMC_HIS.numero = @vl_Cause and
					HIS..FMC_HIS.cle = @vl_CleCause and
					CFG..TYP_FMC.numero = HIS..FMC_GEN.type 
					group by HIS..FMC_HIS.numero, HIS..FMC_HIS.cle,
					HIS..FMC_GEN.numero, HIS..FMC_GEN.cle,
					CFG..TYP_FMC.numero
					having HIS..FMC_HIS.horodate_validation =
						min(HIS..FMC_HIS.horodate_validation)
	
				if @@rowcount = 1
				begin
					/* localisation de la cause */
					exec XZAE83	@vl_Cause, @vl_CleCause, @vl_NumTypeCause, @vl_TypeCause,
							@vl_Autoroute_cause, @vl_PR_cause, @vl_Sens_cause, 
							@vl_Intitule output, @vl_Localisation output
		
					/* insertion de la cause */
					select @vl_Ligne = XZAEC_SYN_CAUSE + 
					'N' + convert(char(6),@va_NumEvt_in) + ' - ' + convert(char(2), @va_CleEvt_in)
					+ ' : ' + rtrim(@vl_Intitule) 
					+ ' ' + @vl_Localisation
		
					insert #SYNTHESE_CONSEQ (numero, cle, ligne, heure, VR, VM2, VM1, VL, BAU,
								VR_I, VM2_I, VM1_I, VL_I, BAU_I,
								BAU_etroite, BAU_I_etroite, poste)
					values ( @va_NumEvt_in, @va_CleEvt_in,
						@vl_Ligne, @vl_Debut, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE,
						XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE,
						XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_FAUX, XDC_FAUX,
						@va_NumPoste_in)
				end
			end

			/* memoriser le fait que la creation a ete ajoutee */
			select @vl_IndicateurCreation = XDC_NON

			/* si c'est un bouchon alors on recherche les actions par rapport a la queue */
			if @vl_NumEvtQueue is not null
			begin
				select @vl_NumEvtUtilise = @vl_NumEvt, @vl_CleEvtUtilise = @vl_CleEvt
				select @vl_NumEvt = @vl_NumEvtQueue, @vl_CleEvt = @vl_CleEvtQueue
			end

			/*B Recherche les commentaires avant la validation suivante de la FMC */
			/*! attention les commentaires peuevent etre inseres avant la premiere */
			/*! validation                                                         */
			insert #COMMENTAIRES
			select * from HIS..FMC_COM
			where	evenement = @vl_NumEvt and
				cle = @vl_CleEvt and
				horodate_validation < @vl_HoroValidation
				order by horodate_validation, numero
		end
		else
		begin
			/* si c'est un bouchon alors on recherche les actions par rapport a la queue */
			if @vl_NumEvtQueue is not null
			begin
				select @vl_NumEvtUtilise = @vl_NumEvt, @vl_CleEvtUtilise = @vl_CleEvt
				select @vl_NumEvt = @vl_NumEvtQueue, @vl_CleEvt = @vl_CleEvtQueue
			end

			/*B Recherche les commentaires inseres entre la validation precedente et */
			/*B la presente validation de la FMC */
			insert #COMMENTAIRES
			select * from HIS..FMC_COM
			where	evenement = @vl_NumEvt and
				cle = @vl_CleEvt and
				horodate_validation >= @vl_HoroValPrec and
				(@vl_SqlStatusValidation = 2 or 
				horodate_validation < @vl_HoroValidation)
				order by horodate_validation, numero
		end

		/*B recherche les actions relatives a la validation de fmc */
		select @vl_TypeHeure = XZAEC_ACT_ECHEC
		insert #ACTIONS ( numero, sit, sit_origine, type, heure, type_heure, 
					equipement)
		select
			numero,
			sit,
			sit_origine,
			type,
			heure_lancement,
			@vl_TypeHeure,
			equipement
		from HIS..ACT_GEN
		where	evenement = @vl_NumEvt and cle = @vl_CleEvt and
			type = XDC_ACT_TFM and
			heure_succes is null and
			heure_echec is null and
			heure_lancement >= @vl_HoroValPrec and
			(@vl_SqlStatusValidation = 2 or 
			 heure_lancement < @vl_HoroValidation)
			order by heure_lancement, numero

		select @vl_TypeHeure = XZAEC_ACT_SUC
		insert #ACTIONS ( numero, sit, sit_origine, type, heure, type_heure, 
					equipement)
		select
			numero,
			sit,
			sit_origine,
			type,
			heure_succes,
			@vl_TypeHeure,
			equipement
		from HIS..ACT_GEN
		where	evenement = @vl_NumEvt and cle = @vl_CleEvt and
			heure_succes >= @vl_HoroValPrec and
			(@vl_SqlStatusValidation = 2 or 
			 heure_succes < @vl_HoroValidation)
			order by heure_succes, numero

		select @vl_TypeHeure = XZAEC_ACT_ECHEC
		insert #ACTIONS ( numero, sit, sit_origine, type, heure, type_heure, 
					equipement)
		select
			numero,
			sit,
			sit_origine,
			type,
			heure_echec,
			@vl_TypeHeure,
			equipement
		from HIS..ACT_GEN
		where	evenement = @vl_NumEvt and cle = @vl_CleEvt and
			heure_echec >= @vl_HoroValPrec and
			(@vl_SqlStatusValidation = 2 or 
			 heure_echec < @vl_HoroValidation)
			order by heure_echec, numero

		select @vl_TypeHeure = XZAEC_ACT_FIN
		insert #ACTIONS ( numero, sit, sit_origine, type, heure, type_heure, 
					equipement)
		select
			numero,
			sit,
			sit_origine,
			type,
			heure_fin,
			@vl_TypeHeure,
			equipement
		from HIS..ACT_GEN
		where	evenement = @vl_NumEvt and cle = @vl_CleEvt and
			type not in ( XDC_ACT_TFM,
					XDC_ACT_TEL,
					XDC_ACT_BIP,
					XDC_ACT_FAX,
					XDC_ACT_CCH ) and
			heure_fin >= @vl_HoroValPrec and
			(@vl_SqlStatusValidation = 2 or 
			 heure_fin < @vl_HoroValidation)
			order by heure_fin, numero


		/*B Ajout les actions de la validation de la FMC */
		if (select count(*) from #ACTIONS) > 0
		begin
			open Pointeur_action

			fetch Pointeur_action into @vl_NumAction, @vl_Site, @vl_SiteOrigine,
					@vl_TypeAction, @vl_Heure, @vl_TypeHeure,
					@vl_Eqt
			while (@@sqlstatus = 0)
			begin
				/* recherche les informations relatives a une action */
				/* trace pour DIAGNOSTIC (a conserver en operationnel) */
				print "Action %1!/%2! a %3! : etat %4!", @vl_NumAction, @vl_Site, @vl_Heure, @vl_TypeHeure
				exec @vl_Status = XZAE206 @va_NumEvt_in,@va_CleEvt_in, @vl_TypeFmc,
						@vl_NumAction, @vl_Site, @vl_SiteOrigine,
						@vl_TypeAction, @vl_Heure, @vl_TypeHeure, @vl_Eqt,
						@vl_Autoroute_Prec, @vl_PR_Prec, @vl_Sens_Prec, @va_NumPoste_in

				if @vl_Status != XDC_OK
					return XZAEC_FMC_ERR_ACTION

				/* action suivante */
				fetch Pointeur_action into @vl_NumAction, 
					@vl_Site, @vl_SiteOrigine, @vl_TypeAction, 
					@vl_Heure, @vl_TypeHeure, @vl_Eqt
			end

			/* effacer le contenu */
			delete #ACTIONS

			close Pointeur_action
		end

		/*B Ajout les commentaires de la validation de la FMC */
		open Pointeur_comment
		fetch Pointeur_comment into @vl_HoroComm, 
				@vl_HoroCommFin, @vl_Texte, 
				@vl_NumeroTexte
		while (@@sqlstatus = 0)
		begin
			/* mettre la date lors du commentaire */
			if @vl_HoroCommFin = null
				/* Hilmarcher ajout de la date + NumEvt + NumCle + " - COMMENTAIRE" */
			begin
				select @vl_Ligne = convert(char(10), @vl_HoroComm, 103) + ' ' +
					convert(char(8), @vl_HoroComm, 8) + ' ' +
					'N' + convert(char(6),@va_NumEvt_in) + ' - ' +
					convert(char(2), @va_CleEvt_in) + ' ' +  convert(varchar(60),@vl_TypeFmc) + 
					' - COMMENTAIRE'
				insert  HIS..FMC_LIS (numero,cle,ligne, heure,poste)
				values( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, @vl_HoroComm,@va_NumPoste_in)

				select @vl_Ligne = convert(char(10), @vl_HoroComm, 103) + ' ' +
					convert(char(8), @vl_HoroComm, 8) + ' ' +
					XZAEC_SYN_COM  
			end
			else
				select @vl_Ligne = convert(char(10), @vl_HoroComm, 103) + ' ' +
					convert(char(8), @vl_HoroComm, 8) + ' ' +
					XZAEC_SYN_COM_VAL +
					convert(char(10), @vl_HoroCommFin, 103) + ' ' +
					convert(char(8), @vl_HoroCommFin, 8)

			/* inserer l'entete du commentaire */
			insert  HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, @vl_HoroComm,@va_NumPoste_in)
			
			/* On coupe la ligne tous les "|" */
			select @vl_Texte_ligne=@vl_Texte
			select @vl_index=patindex("%|%",@vl_Texte_ligne)
			while ( @vl_index!=0)
			begin
				select @vl_Texte = substring(@vl_Texte_ligne,1,@vl_index-1)
				select @vl_Texte_ligne = substring(@vl_Texte_ligne,@vl_index+1,char_length(@vl_Texte_ligne))
				
				/* inserer les morceaux du commentaire en */
				/* tronconnant par texte de 60 caracteres */
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
						substring(@vl_Texte, 1, 60),
					@vl_Longueur = char_length(@vl_Texte)
				insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
				 values( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

				if @vl_Longueur > 60
				begin
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						substring(@vl_Texte, 61, 60)
					insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
					 values( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

					if @vl_Longueur > 120
					begin
						select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							substring(@vl_Texte, 121, 60)
						insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
						values( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
					end
					if @vl_Longueur > 180
					begin
						select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							substring(@vl_Texte, 181, 60)
						insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
						values( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
					end
					if @vl_Longueur > 240
					begin
						select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							substring(@vl_Texte, 241, 10)
						insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
						values( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
					end
				end
				
				select @vl_index=patindex("%|%",@vl_Texte_ligne)
			end
			select @vl_Texte = @vl_Texte_ligne
			
			/* inserer les morceaux du commentaire en */
			/* tronconnant par texte de 60 caracteres */
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					substring(@vl_Texte, 1, 60),
				@vl_Longueur = char_length(@vl_Texte)
			insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
			values( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

			if @vl_Longueur > 60
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					substring(@vl_Texte, 61, 60)
				insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
				values( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

				if @vl_Longueur > 120
				begin
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						substring(@vl_Texte, 121, 60)
					insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
					values( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
				end
				if @vl_Longueur > 180
				begin
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						substring(@vl_Texte, 181, 60)
					insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
					values( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
				end
				if @vl_Longueur > 240
				begin
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						substring(@vl_Texte, 241, 10)
					insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
					values( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
				end
			end

			/* commentaire suivant */
			fetch Pointeur_comment into @vl_HoroComm, 
					@vl_HoroCommFin, @vl_Texte,
					@vl_NumeroTexte
		end

		close Pointeur_comment

		/* effacer le contenu */
		delete #COMMENTAIRES

		/* inserer les consequences dans la synthese */
		insert HIS..FMC_LIS (numero, cle, ligne, heure,
					VR, VM2, VM1, VL, BAU, VR_I, VM2_I, VM1_I, VL_I, BAU_I,
					BAU_etroite, BAU_I_etroite, picto, poste)
		select * from #SYNTHESE_CONSEQ
		delete #SYNTHESE_CONSEQ

		/* repositionner l'evenement a la valeur reelle de la validation */
		if @vl_NumEvtQueue is not null
		begin
			select @vl_NumEvt = @vl_NumEvtUtilise, @vl_CleEvt = @vl_CleEvtUtilise
		end
	end

	/* fermeture du curseur situe sur l'historique de validation */
	close Pointeur_histo

	/* fin de la synthese de l'evenement */
	if @vl_Fin is not null
	begin
		select @vl_Fin = max(horodate_validation) from HIS..FMC_HIS where 
					HIS..FMC_HIS.numero = @va_NumEvt_in and HIS..FMC_HIS.cle = @va_CleEvt_in
		
		select @vl_Ligne = convert(char(10), @vl_Fin, 103) +
					' ' + convert(char(8), @vl_Fin, 8) + ' ' +
					XZAEC_SYN_FINIE + ' N' + convert(char(6),@va_NumEvt_in) +
					' - ' + convert(char(2), @va_CleEvt_in)  
		insert HIS..FMC_LIS (numero,cle,ligne, heure,poste)
		values( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, @vl_Fin,@va_NumPoste_in)
	end

	/* Si dernier appel de la proc alors on retourne la synthese des synthese */
	if (@va_TypeAppel_in = XDC_FCT_APL_DERNIER) or (@va_TypeAppel_in = XDC_FCT_APL_UNIQUE)
	begin
		select @vl_horo = null
		select @vl_compteur = 0	

		declare Pointeur_horodate cursor
		for select numero,cle, ligne, heure, 
				VR, VM2, VM1, VL, BAU, VR_I, VM2_I, VM1_I, VL_I, 
				BAU_I, BAU_etroite, BAU_I_etroite, picto, poste
		from HIS..FMC_LIS
		where poste = @va_NumPoste_in		

		open Pointeur_horodate
		fetch Pointeur_horodate into @vl_numero, @vl_cle,@vl_ligne , @vl_heure,
						@vl_VR, @vl_VM2, @vl_VM1, @vl_VL, @vl_BAU,
						@vl_VR_I, @vl_VM2_I, @vl_VM1_I, @vl_VL_I,
						@vl_BAU_I, @vl_BAU_etroite, @vl_BAU_I_etroite, @vl_picto, @vl_poste
		while @@sqlstatus = 0
		begin
			select @vl_compteur = @vl_compteur + 1
			if @vl_heure != null 
				select @vl_horo = @vl_heure
			begin
				select @vl_debut_ligne = substring(@vl_ligne,1,1)
				if @vl_debut_ligne <> "." and  @vl_debut_ligne <> "C"
				select @vl_horo = convert (datetime,substring(@vl_ligne,1,19))
			end	

			insert #SYNTH values(@vl_numero, @vl_cle, @vl_ligne, @vl_horo,
						isnull(@vl_VR,XDC_VOIE_INCONNUE), 
						isnull(@vl_VM2,XDC_VOIE_INCONNUE),
						isnull(@vl_VM1, XDC_VOIE_INCONNUE),
						isnull(@vl_VL, XDC_VOIE_INCONNUE),
						isnull(@vl_BAU, XDC_VOIE_INCONNUE),
						isnull(@vl_VR_I, XDC_VOIE_INCONNUE),
						isnull(@vl_VM2_I,XDC_VOIE_INCONNUE),
						isnull(@vl_VM1_I, XDC_VOIE_INCONNUE),
						isnull(@vl_VL_I, XDC_VOIE_INCONNUE),
						isnull(@vl_BAU_I,  XDC_VOIE_INCONNUE),
						isnull(@vl_BAU_etroite, XDC_FAUX),
						isnull(@vl_BAU_I_etroite, XDC_FAUX),
						@vl_picto, @vl_poste, @vl_compteur)

			fetch Pointeur_horodate into @vl_numero, @vl_cle, @vl_ligne, @vl_heure,
							@vl_VR, @vl_VM2, @vl_VM1, @vl_VL, @vl_BAU,
							@vl_VR_I, @vl_VM2_I, @vl_VM1_I, @vl_VL_I,
							@vl_BAU_I, @vl_BAU_etroite, @vl_BAU_I_etroite, @vl_picto, @vl_poste
		end	

		close Pointeur_horodate
		
		select
			ligne,
			heure,
			isnull(VR, XDC_VOIE_INCONNUE),
			isnull(VM2, XDC_VOIE_INCONNUE),
			isnull(VM1, XDC_VOIE_INCONNUE),
			isnull(VL, XDC_VOIE_INCONNUE),
			isnull(BAU, XDC_VOIE_INCONNUE),
			isnull(VR_I, XDC_VOIE_INCONNUE),
			isnull(VM2_I, XDC_VOIE_INCONNUE),
			isnull(VM1_I, XDC_VOIE_INCONNUE),
			isnull(VL_I, XDC_VOIE_INCONNUE),
			isnull(BAU_I, XDC_VOIE_INCONNUE),
			isnull(BAU_etroite, XDC_FAUX),
			isnull(BAU_I_etroite, XDC_FAUX),
			picto,
			poste
		from #SYNTH
		order by heure asc, compteur asc

		delete HIS..FMC_LIS
		where poste = @va_NumPoste_in
	end

	return XDC_OK
go	


#else

/*A 
 ** CREATION DE LA PRODECURE STOCKEE POUR UN DISTRICT
 */

create procedure XZAE204
	@va_NumEvt_in		int	= null,
	@va_CleEvt_in		tinyint	= null,
	@va_TypeAppel_in	tinyint = null,
	@va_NumPoste_in 	smallint= null,
	@va_Etat_out		tinyint	= null output	-- XDC_FMC_ETAT_xxx

as
	declare @vl_Cloture datetime, @vl_Alerte int, @vl_SiteAlerte T_SITE,
		@vl_PR T_PR, @vl_Sens T_SENS, @vl_Autoroute T_AUTOROUTE,
		@vl_NomAutoroute char(4), @vl_ConfirmeSignale bit,
		@vl_PR_cause T_PR,@vl_Sens_cause T_SENS, @vl_Autoroute_cause T_AUTOROUTE,
		@vl_PR_conseq T_PR, @vl_Sens_conseq T_SENS, @vl_Autoroute_conseq T_AUTOROUTE,
		@vl_VL T_VOIE, @vl_VM1 T_VOIE,
		@vl_VM2 T_VOIE, @vl_VR T_VOIE, @vl_BAU T_VOIE,
		@vl_VL_I T_VOIE, @vl_VM1_I T_VOIE,
		@vl_VM2_I T_VOIE, @vl_VR_I T_VOIE, @vl_BAU_I T_VOIE,
		@vl_BAU_etroite tinyint, @vl_BAU_I_etroite tinyint,
		@vl_OrigCreation char(25),
		@vl_Debut datetime, @vl_DebutPrevu datetime,
		@vl_Fin datetime, @vl_FinPrevu datetime,
		@vl_NumEvtConseq int, @vl_CleEvtConseq tinyint, 
		@vl_TypeFmc T_NOM, @vl_NumTypeFmc smallint, @vl_ClasseFmc tinyint,
		@vl_TypeCause T_NOM, @vl_NumTypeCause smallint, @vl_HoroCause datetime,
		@vl_TypeFmcConseq T_NOM, @vl_NumTypeFmcConseq smallint,
		@vl_TypeAnterieur smallint, @vl_NomTypeAnterieur T_NOM,
		@vl_HoroValidation datetime, @vl_HoroValPrec datetime,
		@vl_Heure datetime, @vl_TypeHeure tinyint, @vl_NumAction int,
		@vl_Cause int, @vl_CleCause tinyint,
		@vl_PointCaract tinyint, @vl_NumPointCaract tinyint, @vl_NomPointCaract T_NOM,
		@vl_PointCarTete tinyint, @vl_NumPointCarTete tinyint, @vl_NomPointCarTete T_NOM,
		@vl_NumeroTexte smallint, @vl_HoroCommFin datetime,
		@vl_HoroComm datetime, @vl_TypeAction tinyint,
		@vl_SqlStatusValidation int, @vl_Site T_SITE,
		@vl_NbVehLeg tinyint, @vl_NbPL tinyint, @vl_NbMoto tinyint,
		@vl_NbCar tinyint, @vl_NbRemorques tinyint,
		@vl_BlessesLeg tinyint, @vl_BlessesGra tinyint,
		@vl_Morts tinyint, @vl_MatDang bit, @vl_VehESCOTA bit,
		@vl_PersESCOTA bit, @vl_Personalite bit, @vl_Degats tinyint,
		@vl_NumPlan smallint, @vl_NomPlan T_NOM, @vl_NomEchSortie T_NOM,
		@vl_NomEchReinjection T_NOM, @vl_EchReinjection tinyint,
		@vl_TypeVeh tinyint, @vl_CodeMatDang char(6), 
		@vl_Commentaire char(100), @vl_LongVeh smallint,
		@vl_Contenu char(25), @vl_Hauteur smallint,
		@vl_Largeur smallint, @vl_Poids smallint,
		@vl_Vitesse tinyint, @vl_TypePerturb tinyint,
		@vl_nature_obstacle char(100),
		@vl_Visibilite smallint, @vl_EntreeN bit, @vl_EntreeS bit,
		@vl_SortieN bit, @vl_SortieS bit,
		@vl_NomCnc T_NOM, @vl_CncFerme bit, @vl_Status int,
		@vl_Penurie bit, @vl_Carburant tinyint, @vl_Nombre smallint,
		@vl_Perception bit, @vl_Violence bit, @vl_Operateur T_OPERATEUR,
		@vl_NomCarburant char(15), @vl_AutorouteM T_AUTOROUTE,
		@vl_PRTeteM T_PR, @vl_PRQueueM T_PR, @vl_SensM T_SENS, 
		@vl_Mobile bit, @vl_VoiesRet bit, @vl_DebutPlanifie datetime,
		@vl_FinPlanifie datetime, @vl_Continu bit, @vl_NatureTrv bit,
		@vl_TypeTrv tinyint, @vl_TypeBouchon tinyint, 
		@vl_NumEvtUtilise int, @vl_CleEvtUtilise tinyint,
		@vl_NumEvtTete int, @vl_CleEvtTete tinyint,
		@vl_NumEvtQueue int, @vl_CleEvtQueue tinyint,
		@vl_NumEvt int, @vl_CleEvt tinyint,
		@vl_nb int, @vl_DateRecherche datetime, @vl_Date datetime,
		@vl_Eqt T_EQUIPEMENT, @vl_IndicateurCreation bit,
		@vl_Autoroute_Prec T_AUTOROUTE, @vl_PR_Prec T_PR, @vl_Sens_Prec T_SENS,
		@vl_FausseAlerte tinyint,
		@vl_PRdebut_VR T_PR,@vl_PRfin_VR T_PR,@vl_debut_VR datetime,@vl_fin_VR datetime,
		@vl_PRdebut_VM2 T_PR,@vl_PRfin_VM2 T_PR,@vl_debut_VM2 datetime,@vl_fin_VM2 datetime,
		@vl_PRdebut_VM1 T_PR,@vl_PRfin_VM1 T_PR,@vl_debut_VM1 datetime,@vl_fin_VM1 datetime,
		@vl_PRdebut_VL T_PR,@vl_PRfin_VL T_PR,@vl_debut_VL datetime,@vl_fin_VL datetime,
		@vl_PRdebut_BAU T_PR,@vl_PRfin_BAU T_PR,@vl_debut_BAU datetime,@vl_fin_BAU datetime,
		@vl_PRdebut_VR_I T_PR,@vl_PRfin_VR_I T_PR, @vl_debut_VR_I datetime,@vl_fin_VR_I datetime,
		@vl_PRdebut_VM2_I T_PR,@vl_PRfin_VM2_I T_PR, @vl_debut_VM2_I datetime,@vl_fin_VM2_I datetime,
		@vl_PRdebut_VM1_I T_PR,@vl_PRfin_VM1_I T_PR, @vl_debut_VM1_I datetime,@vl_fin_VM1_I datetime,
		@vl_PRdebut_VL_I T_PR,@vl_PRfin_VL_I T_PR, @vl_debut_VL_I datetime,@vl_fin_VL_I datetime,
		@vl_PRdebut_BAU_I T_PR,@vl_PRfin_BAU_I T_PR, @vl_debut_BAU_I datetime,@vl_fin_BAU_I datetime,
		@vl_NumFmcInit int, @vl_CleFmcInit tinyint, @vl_SiteOrigine T_SITE, @vl_Datex tinyint,
		@vl_gravite tinyint, @vl_trafic tinyint, @vl_bloquant bit,
		@vl_FeuMaitrise tinyint, @vl_HorodateMaitrise datetime,
		@vl_numero int, @vl_cle int, @vl_ligne char(150), @vl_heure datetime,
		@vl_picto char(4), @vl_poste smallint, @vl_horo datetime, @vl_compteur smallint, @vl_debut_ligne char(1),
		@vl_index tinyint, @vl_Longueur int,
		@vl_Intitule char(60), @vl_Localisation  char(60), 
		@vl_Intitule2 char(60), @vl_Localisation2  char(60), 
		@vl_Texte varchar(250), @vl_Texte_ligne varchar(250),
		@vl_InitLigne char(20), @vl_Ligne char(150),
		@vl_Libelle varchar(20),@vl_vitesse tinyint, @vl_vitesse_opposee tinyint,
		@vl_personnel tinyint, @vl_code_nature tinyint, @vl_nature_inc varchar(100),
		@vl_PresenceBouchon tinyint

	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_TypeAppel_in = 0 select @va_TypeAppel_in = null
	if @va_NumPoste_in = 0 select @va_NumPoste_in = null

	/*A Controle des parametres d'entree */
	if @va_NumEvt_in = null or @va_CleEvt_in = null or @va_TypeAppel_in = null 
		return XDC_ARG_INV

	/*A Lors d'un premier appel, supprimer tous restes d'un appel precedent */
	if (@va_TypeAppel_in = XDC_FCT_APL_PREMIER) or (@va_TypeAppel_in = XDC_FCT_APL_UNIQUE)
	begin
		delete EXP..FMC_LIS
		where poste = @va_NumPoste_in
	end

	/*A réinitialisation de la table LISTE_CONSEQ */
	delete LISTE_CONSEQ
	where spid=@@spid

	/*A Verifier l'existence de la fiche main courante */
	select
		@vl_Datex	= EXP..FMC_GEN.datex,
		@vl_Cloture	= EXP..FMC_GEN.cloture,
		@vl_Alerte	= EXP..FMC_GEN.alerte,
		@vl_SiteAlerte	= EXP..FMC_GEN.sit_alerte,
		@vl_ClasseFmc	= CFG..TYP_FMC.classe,
		@vl_TypeFmc	= CFG..TYP_FMC.nom,
		@vl_NumTypeFmc	= EXP..FMC_GEN.type,
		@vl_OrigCreation = EXP..FMC_GEN.origine_creation,
		@vl_Debut	= EXP..FMC_GEN.debut,
		@vl_DebutPrevu	= EXP..FMC_GEN.debut_prevu,
		@vl_Fin		= EXP..FMC_GEN.fin,
		@vl_FinPrevu	= EXP..FMC_GEN.fin_prevue,
		@vl_Cause	= EXP..FMC_GEN.cause,
		@vl_CleCause	= EXP..FMC_GEN.cle_cause,
		@vl_HoroCause	= EXP..FMC_GEN.horodate_lien_cause,
		@vl_FausseAlerte	= fausse_alerte,
		@vl_NumFmcInit	= num_fmc_init,
		@vl_CleFmcInit	= cle_fmc_init,
		@vl_Degats	= degats_domaine
	from EXP..FMC_GEN, CFG..TYP_FMC
	where	EXP..FMC_GEN.numero = @va_NumEvt_in and 
		EXP..FMC_GEN.cle = @va_CleEvt_in and
		CFG..TYP_FMC.numero = EXP..FMC_GEN.type

	if @@rowcount != 1
		return XDC_NOK


	/*A Recherche l'etat de la fiche main courante */
	if @vl_Cloture != null
		select @va_Etat_out = XDC_FMC_ETAT_CLOS
	else if exists ( select * from EXP..FMC_TRT
			 where	evenement = @va_NumEvt_in and
				cle = @va_CleEvt_in)
		select @va_Etat_out = XDC_FMC_ETAT_TRAI
	else	select @va_Etat_out = XDC_FMC_ETAT_PREV


	/* positionner var internes */
	select @vl_NumEvtQueue = null, @vl_CleEvtQueue = null

	/*A Creation du tableau de stockage de la synthese */
	
	create table #SYNTHESE_CONSEQ
		(
		numero  int not null,
		cle	int not null,
		ligne	char(150) not null,
		heure	datetime null,
		VR	tinyint	default	XDC_VOIE_INCONNUE,
		VM2	tinyint	default	XDC_VOIE_INCONNUE,
		VM1	tinyint	default	XDC_VOIE_INCONNUE,
		VL	tinyint	default	XDC_VOIE_INCONNUE,
		BAU	tinyint	default	XDC_VOIE_INCONNUE,
		VR_I	tinyint	default	XDC_VOIE_INCONNUE,
		VM2_I	tinyint	default	XDC_VOIE_INCONNUE,
		VM1_I	tinyint	default	XDC_VOIE_INCONNUE,
		VL_I	tinyint	default	XDC_VOIE_INCONNUE,
		BAU_I	tinyint	default	XDC_VOIE_INCONNUE,
		BAU_etroite	tinyint	default	XDC_FAUX,
		BAU_I_etroite	tinyint	default	XDC_FAUX,
		picto	char(4) null,
		poste	smallint not null
		)
	
	create table #SYNTH
		(
		numero  int not null,
		cle	int not null,
		ligne	char(150) not null,
		heure	datetime null,
		VR	tinyint	default	XDC_VOIE_INCONNUE,
		VM2	tinyint	default	XDC_VOIE_INCONNUE,
		VM1	tinyint	default	XDC_VOIE_INCONNUE,
		VL	tinyint	default	XDC_VOIE_INCONNUE,
		BAU	tinyint	default	XDC_VOIE_INCONNUE,
		VR_I	tinyint	default	XDC_VOIE_INCONNUE,
		VM2_I	tinyint	default	XDC_VOIE_INCONNUE,
		VM1_I	tinyint	default	XDC_VOIE_INCONNUE,
		VL_I	tinyint	default	XDC_VOIE_INCONNUE,
		BAU_I	tinyint	default	XDC_VOIE_INCONNUE,
		BAU_etroite	tinyint	default	XDC_FAUX,
		BAU_I_etroite	tinyint	default	XDC_FAUX,
		picto	char(4) null,
		poste	smallint not null,
		compteur smallint not null
		)
	
	create table #ACTIONS 
		( 
		numero		int, 
		sit		tinyint, 
		sit_origine	tinyint, 
		type		tinyint, 
		heure		datetime, 
		type_heure	tinyint default XZAEC_ACT_SUC,	/* heure de succes par defaut */
		equipement	smallint null
		) 
	create table #COMMENTAIRES
		(
		evenement             int                   not null,
		cle                   tinyint               not null,
		numero                smallint              not null,
		horodate_validation   datetime              not null,
		horodate_fin          datetime              null    ,
		texte                 char(250)             null    ,
		sit                   tinyint                null     
		)
	create table #HISTORIQUE_FMC
		(
		numero			int,
		cle			tinyint,
		classe			tinyint,
		type			smallint,
		nom_type		char(25),
		type_anterieur		smallint null,
		autoroute		tinyint	null,
		PR			int	null,
		sens			tinyint	null,
		point_caracteristique	tinyint null,
		numero_point_caracteristique tinyint null,
		confirme_ou_signale	bit,
		horodate_validation	datetime,
		critere_ordre		tinyint default 1,
		VR	tinyint	default	XDC_VOIE_INCONNUE,
		VM2	tinyint	default	XDC_VOIE_INCONNUE,
		VM1	tinyint	default	XDC_VOIE_INCONNUE,
		VL	tinyint	default	XDC_VOIE_INCONNUE,
		BAU	tinyint	default	XDC_VOIE_INCONNUE,
		VR_I	tinyint	default	XDC_VOIE_INCONNUE,
		VM2_I	tinyint	default	XDC_VOIE_INCONNUE,
		VM1_I	tinyint	default	XDC_VOIE_INCONNUE,
		VL_I	tinyint	default	XDC_VOIE_INCONNUE,
		BAU_I	tinyint	default	XDC_VOIE_INCONNUE,
		BAU_etroite	tinyint	default	XDC_FAUX,
		BAU_I_etroite	tinyint	default	XDC_FAUX
		)
	create table #TEMPO_HISTORIQUE_FMC
		(
		numero			int,
		cle			tinyint,
		classe			tinyint,
		type			smallint,
		nom_type		char(25),
		type_anterieur		smallint null,
		autoroute		tinyint	null,
		PR			int	null,
		sens			tinyint	null,
		point_caracteristique	tinyint null,
		numero_point_caracteristique tinyint null,
		confirme_ou_signale	bit,
		horodate_validation	datetime,
		critere_ordre		tinyint default 1,
		VR	tinyint	default	XDC_VOIE_INCONNUE,
		VM2	tinyint	default	XDC_VOIE_INCONNUE,
		VM1	tinyint	default	XDC_VOIE_INCONNUE,
		VL	tinyint	default	XDC_VOIE_INCONNUE,
		BAU	tinyint	default	XDC_VOIE_INCONNUE,
		VR_I	tinyint	default	XDC_VOIE_INCONNUE,
		VM2_I	tinyint	default	XDC_VOIE_INCONNUE,
		VM1_I	tinyint	default	XDC_VOIE_INCONNUE,
		VL_I	tinyint	default	XDC_VOIE_INCONNUE,
		BAU_I	tinyint	default	XDC_VOIE_INCONNUE,
		BAU_etroite	tinyint	default	XDC_FAUX,
		BAU_I_etroite	tinyint	default	XDC_FAUX
		)

	if @vl_FausseAlerte = XDC_VRAI
	begin
		/* fausse alerte */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_FAUSSE_ALERTE
		insert EXP..FMC_LIS (numero, cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,  null,@va_NumPoste_in)

		/* ligne vide */
		insert EXP..FMC_LIS(numero, cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,XZAEC_SYN_PAS_DATE,  null,@va_NumPoste_in)
	end

	if @vl_Datex = XDC_VRAI
	begin
		/* fausse alerte */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ORIGINE_DATEX
		insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,  null,@va_NumPoste_in)

		/* ligne vide */
		insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,XZAEC_SYN_PAS_DATE,  null,@va_NumPoste_in)
	end

	/* creation d'un curseur pour le parcours des morceaux */
	/* de commentaires */
	declare Pointeur_comment cursor
	for select horodate_validation, horodate_fin, texte, numero
	from #COMMENTAIRES

	/* creation d'un curseur pour le parcours de la liste des */
	/* actions de la fmc */
	declare Pointeur_action cursor
	for select numero, 
		sit, 
		sit_origine,
		type, 
		heure, 
		type_heure, 
		equipement
	from #ACTIONS
	order by heure, type_heure

	/*A Recherche les informations sur l'alerte */
	if @vl_Alerte != null
	begin
		/*B creation de l'intitule de l'alerte */
		select @vl_Ligne = XDC_CHAINE_VIDE
		select
			@vl_Ligne = convert(char(10),EXP..ALT_EXP.horodate, 103) + 
			' ' + convert(char(8), EXP..ALT_EXP.horodate, 8) +
			' ' + XZAEC_SYN_ALT + CFG..TYP_ALT.nom +
			' ' + CFG..EQT_GEN.nom ,
			@vl_Texte = EXP..ALT_EXP.texte,
			@vl_Autoroute = CFG..EQT_GEN.autoroute,
			@vl_PR = CFG..EQT_GEN.PR,
			@vl_Sens = CFG..EQT_GEN.sens
		from EXP..ALT_EXP, CFG..TYP_ALT, CFG..EQT_GEN
		where	EXP..ALT_EXP.numero = @vl_Alerte and
			EXP..ALT_EXP.sit = @vl_SiteAlerte and
			CFG..TYP_ALT.numero = EXP..ALT_EXP.type and
			CFG..EQT_GEN.numero = EXP..ALT_EXP.equipement and
			CFG..EQT_GEN.type = EXP..ALT_EXP.type_equipement

		/*B Ajout de l'intitule de l'alerte dans la synthese */
		if @vl_Ligne != XDC_CHAINE_VIDE
		begin
			insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,  null,@va_NumPoste_in)

			/* mise en forme de la localisation de l'alerte */
			exec XZAE83 null, null, null, null, @vl_Autoroute, @vl_PR, @vl_Sens,
					@vl_Intitule2 output, @vl_Localisation2 output
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ALT_LOC +
						@vl_Localisation2

			/*B Ajout des informations de l'alerte pour la synthese */
			insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
			values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

			/*B Ajout des commentaires de l'alerte pour la synthese */
			if @vl_Texte != XDC_CHAINE_VIDE
			begin
				/* tronconner le texte en deux morceaux */
				select	@vl_Longueur = char_length(@vl_Texte),
					@vl_Ligne = XZAEC_SYN_PAS_DATE + 
							XZAEC_SYN_ALT_COM + 
							substring(@vl_Texte, 1, 65)
	
				/* insertion de la premiere ligne de commentaire */
				insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
				values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
	
				if @vl_Longueur > 60
				begin
					select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
							   substring(@vl_Texte, 61, 39)
	
					/* insertion de la deuxieme ligne de commentaire */
					insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
					values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
				end
			end
		end
	end


/************************************************************************************************************************/
/************************************************************************************************************************/
/**                                       Recherche de l'historique complet de la FMC                                  **/
/************************************************************************************************************************/
/************************************************************************************************************************/

	insert #HISTORIQUE_FMC
		(numero, cle, classe, type, nom_type, type_anterieur, autoroute,
		PR, sens, confirme_ou_signale, horodate_validation,
		point_caracteristique, numero_point_caracteristique,
		VR, VM2, VM1, VL, BAU, VR_I, VM2_I, VM1_I, VL_I, BAU_I,
		BAU_etroite, BAU_I_etroite, critere_ordre)
	select 
		EXP..FMC_HIS.numero,
		EXP..FMC_HIS.cle,
		CFG..TYP_FMC.classe,
		EXP..FMC_HIS.type_FMC_valide,
		CFG..TYP_FMC.nom,
		EXP..FMC_HIS.type_anterieur,
		EXP..FMC_HIS.autoroute,
		EXP..FMC_HIS.PR,
		EXP..FMC_HIS.sens,
		EXP..FMC_HIS.confirme_ou_signale,
		EXP..FMC_HIS.horodate_validation,
		EXP..FMC_HIS.point_caracteristique,
		EXP..FMC_HIS.numero_point_caracteristique,
		isnull(EXP..FMC_HIS.VR, XDC_VOIE_INCONNUE),
		isnull(EXP..FMC_HIS.VM2, XDC_VOIE_INCONNUE),
		isnull(EXP..FMC_HIS.VM1, XDC_VOIE_INCONNUE),
		isnull(EXP..FMC_HIS.VL, XDC_VOIE_INCONNUE),
		isnull(EXP..FMC_HIS.BAU, XDC_VOIE_INCONNUE),
		isnull(EXP..FMC_HIS.VR_I, XDC_VOIE_INCONNUE),
		isnull(EXP..FMC_HIS.VM2_I, XDC_VOIE_INCONNUE),
		isnull(EXP..FMC_HIS.VM1_I, XDC_VOIE_INCONNUE),
		isnull(EXP..FMC_HIS.VL_I, XDC_VOIE_INCONNUE),
		isnull(EXP..FMC_HIS.BAU_I, XDC_VOIE_INCONNUE),
		isnull(EXP..FMC_HIS.BAU_etroite, XDC_FAUX),
		isnull(EXP..FMC_HIS.BAU_I_etroite, XDC_FAUX),
		1
	from EXP..FMC_HIS, CFG..TYP_FMC
	where	EXP..FMC_HIS.numero = @va_NumEvt_in and 
		EXP..FMC_HIS.cle = @va_CleEvt_in and
		CFG..TYP_FMC.numero = EXP..FMC_HIS.type_FMC_valide
		order by EXP..FMC_HIS.horodate_validation

	/* l'evenement n'a jamais ete valide : pas de synthese */
	if @@rowcount = 0
		return XDC_PAS_SYN


	/*A Recherche la description de la creation de la FMC */
	select 
		@vl_HoroValidation = horodate_validation,
		@vl_NumTypeFmc = type,
		@vl_Autoroute = autoroute,
		@vl_PR = PR,
		@vl_Sens = sens,
		@vl_ConfirmeSignale = confirme_ou_signale,
		@vl_PointCaract = point_caracteristique,
		@vl_NumPointCaract = numero_point_caracteristique,
		@vl_VR = VR,
		@vl_VM2 = VM2,
		@vl_VM1 = VM1,
		@vl_VL = VL,
		@vl_BAU = BAU,
		@vl_VR_I = VR_I,
		@vl_VM2_I = VM2_I,
		@vl_VM1_I = VM1_I,
		@vl_VL_I = VL_I,
		@vl_BAU_I = BAU_I,
		@vl_BAU_etroite = BAU_etroite,
		@vl_BAU_I_etroite = BAU_I_etroite
	from #HISTORIQUE_FMC
	having horodate_validation = min(horodate_validation)

	/* positionner un indicateur sur de la creation de l'historique */
	select @vl_IndicateurCreation = XDC_OUI

	/* le type pour un bouchon est change pour l'affichage */
	/* (libelle plus explicite)                            */
	if @vl_NumTypeFmc = XZAEC_FMC_TeteBouchon
		select @vl_TypeFmc = XZAEC_SYN_FMC_TeteBouchon
	else if @vl_NumTypeFmc = XZAEC_FMC_QueueBouchon
		select @vl_TypeFmc = XZAEC_SYN_FMC_QueueBouchon

	/*A Ajout de description de la creation de la FMC */
	exec XZAE83	@va_NumEvt_in, @va_CleEvt_in, @vl_NumTypeFmc, @vl_TypeFmc,
			@vl_Autoroute, @vl_PR, @vl_Sens,
			@vl_Intitule output, @vl_Localisation output
	select @vl_Ligne = convert(char(10), @vl_HoroValidation, 103) + ' ' +
			convert(char(8), @vl_HoroValidation, 8) + ' ' +
			convert(varchar(60), @vl_Intitule) + ' - '  + XZAEC_SYN_CREATION

	insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
	values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, @vl_HoroValidation,@va_NumPoste_in)

	select @vl_Ligne = XZAEC_SYN_PAS_DATE + @vl_Localisation
	insert EXP..FMC_LIS 
	values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null, @vl_VR, @vl_VM2, @vl_VM1,
		@vl_VL, @vl_BAU, @vl_VR_I, @vl_VM2_I, @vl_VM1_I,@vl_VL_I, @vl_BAU_I, 
		@vl_BAU_etroite, @vl_BAU_I_etroite, null,@va_NumPoste_in)

	if @vl_OrigCreation != XDC_CHAINE_VIDE
	begin
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ORI_CRE + 
				 @vl_OrigCreation
		insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
		values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
	end

	if @vl_Debut != null
	begin
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_DEB + 
					convert(char(10), @vl_Debut, 103) + ' ' + 
					convert(char(8), @vl_Debut, 8) 
		insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
		values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
	end

	if @vl_Fin != null
	begin
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_FIN + 
					convert(char(10), @vl_Fin, 103) + ' ' + 
					convert(char(8), @vl_Fin, 8) 
		insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
		values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
	end

	if @vl_DebutPrevu != null
	begin
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_DEB_PREV + 
					convert(char(10), @vl_DebutPrevu, 103) + ' ' + 
					convert(char(8), @vl_DebutPrevu, 8) 
		insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
		values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
	end

	if @vl_FinPrevu != null
	begin
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_FIN_PREV + 
					convert(char(10), @vl_FinPrevu, 103) + ' ' + 
					convert(char(8), @vl_FinPrevu, 8) 
		insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
		values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
	end

	if @vl_PointCaract != null and @vl_NumPointCaract != null
	begin
		if @vl_PointCaract = XDC_POINT_CARACT_AIRE
		begin
			select @vl_Libelle = XZAEC_SYN_VEH_AIR

			select @vl_NomPointCaract = nom
			from CFG..RES_AIR
			where numero = @vl_NumPointCaract
		end
		else if @vl_PointCaract = XDC_POINT_CARACT_ECHANGEUR
		begin
			select @vl_Libelle = XZAEC_SYN_VEH_ECH

			select @vl_NomPointCaract = nom
			from CFG..RES_ECH
			where numero = @vl_NumPointCaract
		end
		else if @vl_PointCaract = XDC_POINT_CARACT_PEAGE
		begin
			select @vl_Libelle = XZAEC_SYN_VEH_PEA

			select @vl_NomPointCaract = nom
			from CFG..RES_PEA
			where numero = @vl_NumPointCaract
		end
		else if @vl_PointCaract = XDC_POINT_CARACT_LIT_ARRET
		begin
			select @vl_Libelle = XZAEC_SYN_VEH_LIT

			select @vl_NomPointCaract = nom
			from CFG..EQT_GEN
			where	numero = @vl_NumPointCaract and 
				type = XDC_CAPT_LIT_ARRET
		end
		else return XZAEC_FMC_LOC_INC

		if @@rowcount != 1
			return XZAEC_FMC_LOC_INC

		select @vl_Ligne = XZAEC_SYN_PAS_DATE + @vl_Libelle + @vl_NomPointCaract

		insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
		values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
	end

	if @vl_ConfirmeSignale = XZAEC_FMC_SIGNALEE
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_SIGNALEE
	else	select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_CONFIRMEE
	insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
	values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

	if @vl_NumFmcInit is not null
	begin
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
				XZAEC_SYN_FMC_DEGRADE + convert(varchar(10), @vl_NumFmcInit) + ' ' + convert(char(2), @vl_CleFmcInit)
		insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
	end

	if @vl_Degats = XDC_OUI
	begin
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
				XZAEC_SYN_DEG_DOM
		insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
	end


/************************************************************************************************************************/
/************************************************************************************************************************/
/**                                            Ajout du complement de la FMC                                           **/
/************************************************************************************************************************/
/************************************************************************************************************************/

/************************************************************************************************************************/
/**                                               Cas d'un Accident                                                    **/
/************************************************************************************************************************/
	if @vl_ClasseFmc = XZAEC_ClasseAccident
	begin
		select
			@vl_NbVehLeg = nombre_de_vl,
			@vl_NbPL = nombre_de_pl,
			@vl_NbMoto = nombre_de_moto,
			@vl_NbCar = nombre_de_car,
			@vl_NbRemorques = nombre_de_remorques,
			@vl_BlessesLeg = blesses_legers,
			@vl_BlessesGra = blesses_graves,
			@vl_Morts = morts,
			@vl_MatDang = matieres_dangereuses,
			@vl_VehESCOTA = vehicule_ESCOTA,
			@vl_PersESCOTA = personnel_ESCOTA,
			@vl_Personalite = personnalites
		from EXP..FMC_ACC
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in
		having validation = max(validation)

		if @vl_NbVehLeg != null and @vl_NbVehLeg != 0
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_VL + 
					convert(varchar(8),@vl_NbVehLeg)
			insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_NbPL != null and @vl_NbPL != 0
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_PL + 
					convert(varchar(8),@vl_NbPL)
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_NbMoto != null and @vl_NbMoto != 0
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_MOTO + 
					convert(varchar(8), @vl_NbMoto)
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_NbCar != null and @vl_NbCar != 0
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_CAR + 
					convert(varchar(8), @vl_NbCar)
			insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_NbRemorques != null and @vl_NbRemorques != 0
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_REMORQ + 
					convert(varchar(8), @vl_NbRemorques)
			insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_BlessesLeg != null and @vl_BlessesLeg != 0 
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_BLE_LEG + 
					convert(varchar(8), @vl_BlessesLeg)
			insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_BlessesGra != null and @vl_BlessesGra != 0
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_BLE_GRA + 
					convert(varchar(8), @vl_BlessesGra)
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_Morts != null and @vl_Morts != 0
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_MOR + 
					convert(varchar(8), @vl_Morts)
			insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_MatDang = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_PRE_MAT_DNG
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in, @vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_VehESCOTA = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_VEH_ESC 
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_PersESCOTA = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_PER_ESC
			insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_Personalite = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_PERS 
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

	end

/************************************************************************************************************************/
/**                                    Cas d'un evenement de classe Delestage                                          **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClasseDelestage
	begin
		/* rechercher les informations du delestage */
		select @vl_NumPlan = plan_associe
		from EXP..FMC_DEL
		where   numero = @va_NumEvt_in and cle = @va_CleEvt_in

		if @vl_NumPlan != null
		begin
			select
				@vl_NomPlan = CFG..PLN_SCR.nom,
				@vl_NomEchSortie = CFG..RES_ECH.nom,
				@vl_EchReinjection = CFG..PLN_SCR.echangeur_reinjection
			from CFG..PLN_SCR, CFG..RES_ECH
			where	CFG..PLN_SCR.numero = @vl_NumPlan and
				CFG..RES_ECH.numero = CFG..PLN_SCR.echangeur_sortie
	
			if @@rowcount != 1
				return XZAEC_FMC_PAS_DONNEES
	
			select @vl_NomEchReinjection = nom
			from CFG..RES_ECH
			where	numero = @vl_EchReinjection
	
			if @@rowcount != 1
				return XZAEC_FMC_PAS_DONNEES
	
			/* Ajout des informations du delestage dans la synthese */
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_PLN_SCR +
						+ @vl_NomPlan
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
	
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ECH_SOR +
						+ @vl_NomEchSortie
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
	
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ECH_INJ +
						+ @vl_NomEchReinjection
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
	end

/************************************************************************************************************************/
/**                                    Cas d'un evenement de classe Vehicule                                           **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClasseVehicule
	begin
		/* recherche les informations sur un vehicule */
		select
			@vl_TypeVeh = type_de_vehicule,
			@vl_VehESCOTA = vehicule_ESCOTA,
			@vl_CodeMatDang = matieres_dangereuses,
			@vl_Commentaire = commentaire,
			@vl_FeuMaitrise = feu_maitrise,
			@vl_HorodateMaitrise = horodate_maitrise
		from EXP..FMC_VEH
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in

		if @vl_TypeVeh != null
		begin
			if @vl_TypeVeh = XZAEC_VEH_PL
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_VEH_PL
			else if @vl_TypeVeh = XZAEC_VEH_VL   
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_VEH_VL
			else if @vl_TypeVeh = XZAEC_VEH_Car   
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_VEH_CAR
			else if @vl_TypeVeh = XZAEC_VEH_Moto   
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_VEH_MOT
			else if @vl_TypeVeh =  XZAEC_VEH_Remorque 
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_VEH_REM
			else return XZAEC_FMC_ERR_DONNEES

			insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
		if @vl_FeuMaitrise = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_VEH_FEU_MAITRISE +
					convert(char(10),@vl_HorodateMaitrise,103) + ' ' +
					convert(char(8),@vl_HorodateMaitrise,8)
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
		if @vl_VehESCOTA = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_VEH_ESC 
			insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
		if @vl_CodeMatDang != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_MAT_DNG + @vl_CodeMatDang
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
		if @vl_Commentaire != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_COM + 
					substring(@vl_Commentaire, 1, 48),
				@vl_Longueur = char_length(@vl_Commentaire)
			insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

			if @vl_Longueur > 48
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					substring(@vl_Commentaire, 49, 51)
				insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
				values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
			end
		end
	end

/************************************************************************************************************************/
/**                                    Cas d'un evenement de classe Vehicule lent                                      **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClasseVehLent
	begin
		/* recherche les informations sur un vehicule lent */
		select
			@vl_LongVeh = longueur,
			@vl_Vitesse = vitesse,
			@vl_Hauteur = hauteur,
			@vl_Largeur = largeur,
			@vl_Poids = poids,
			@vl_Contenu = contenu
		from EXP..FMC_VLN
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in
		having horodate = min(horodate)

		/* Ajout des informations dans la synthese */
		if @vl_LongVeh != null
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_LON_VEH + 
					convert(varchar(8), @vl_LongVeh)
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_Vitesse != null
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_VIT_VEH + 
					convert(varchar(8), @vl_Vitesse)
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_NumTypeFmc = XZAEC_FMC_ConvoiExc or @vl_NumTypeFmc = XZAEC_FMC_ConvoiMil
		begin
			if @vl_Hauteur != null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_HAU_VEH + 
						convert(varchar(8), @vl_Hauteur)
				insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
				values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
			end
	
			if @vl_Largeur != null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_LRG_VEH + 
						convert(varchar(8), @vl_Largeur)
				insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
				values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
			end
	
			if @vl_Poids != null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_POI_VEH + 
						convert(varchar(8), @vl_Poids)
				insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
				values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
			end

			if @vl_Contenu != null and @vl_Contenu != XDC_CHAINE_VIDE
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_CNT_VEH + 
						convert(varchar(25), @vl_Contenu)
				insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
				values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
			end
	
		end
	end

/************************************************************************************************************************/
/**                                         Cas d'un evenement de classe Meteo                                         **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClasseMeteo
	begin
		/* recherche des informations sur la meteo */
		select
			@vl_PR = PR,
			@vl_Visibilite = visibilite,
			@vl_TypePerturb = type_de_perturbation
		from EXP..FMC_MET
		where   numero = @va_NumEvt_in and cle = @va_CleEvt_in
		having horodate = min(horodate)

		/* Ajout des informations dans la synthese */
		if @vl_TypePerturb != null
		begin
			if @vl_TypePerturb = XZAEC_MET_ChuteNeige
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
						   XZAEC_SYN_CHUT_NEI
			else if @vl_TypePerturb = XZAEC_MET_NeigeChaus
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_NEI_CHA
			else if @vl_TypePerturb = XZAEC_MET_Pluie
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_PLU
			else if @vl_TypePerturb = XZAEC_MET_VentFort
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_VEN
			else if @vl_TypePerturb = XZAEC_MET_Brouillard
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_BROUIL
			else if @vl_TypePerturb = XZAEC_MET_BrouillardGiv
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_BRO_GIV
			else if @vl_TypePerturb = XZAEC_MET_Grele
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_GRELE 
			else if @vl_TypePerturb = XZAEC_MET_GreleChaus
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						  XZAEC_SYN_GREL_CH
			else if @vl_TypePerturb = XZAEC_MET_Verglas
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_VERG
			else if @vl_TypePerturb = XZAEC_MET_Autre
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_AUT
			/* cas d'un evenement Chaussee Glissante */
			else if @vl_TypePerturb = XZAEC_CH_GLIS_Neige
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_NEI_CHA
			else if @vl_TypePerturb = XZAEC_CH_GLIS_Verglas
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_VERG
			else if @vl_TypePerturb = XZAEC_CH_GLIS_Autre
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_AUT
			else if @vl_TypePerturb = XZAEC_CH_GLIS_Grele
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_GREL_CH
			else if @vl_TypePerturb = XZAEC_CH_GLIS_Huile
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_CHS_HUILE
			else if @vl_TypePerturb = XZAEC_CH_GLIS_Carburant
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_CHS_CARBURANT
			else if @vl_TypePerturb = XZAEC_CH_GLIS_Gasoil
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_CHS_GASOIL
			/* cas d'un evenement Traitement Chaussees */
			else if @vl_TypePerturb = XZAEC_TRT_CH_Salage
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_TRT_SALAGE
			else if @vl_TypePerturb = XZAEC_TRT_CH_Deneigement
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_TRT_DENEIG
			else return XZAEC_FMC_ERR_DONNEES

			insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_PR != null
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_FIN_MET + 
					convert(char(7), @vl_PR/1000.0) 
			
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_NumTypeFmc = XZAEC_FMC_Meteo
		begin
			if @vl_Visibilite != null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_VIS_MET + 
						convert(varchar(8), @vl_Visibilite/1000.0)
				insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
				values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
			end
		end

	end

/************************************************************************************************************************/
/**                                         Cas d'un evenement de classe Nature                                        **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClasseNature
	begin
		/* recherche des informations sur la nature de l'obstacle */
		select
			@vl_nature_obstacle = nature
		from EXP..FMC_NAT
		where   numero = @va_NumEvt_in and cle = @va_CleEvt_in
		having horodate = min(horodate)
		select @vl_Ligne = XZAEC_SYN_PAS_DATE +
				XZAEC_SYN_NATURE + @vl_nature_obstacle
		insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
	end

/************************************************************************************************************************/
/**                                               Cas d'un evenement Echangeur                                         **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClasseEchangeur
	begin
		/* recherche les informations sur la fermeture d'un echangeur */
		exec @vl_Status = XZAE;52 @vl_HoroValidation, @va_NumEvt_in, @va_CleEvt_in,
					@vl_EntreeS output, @vl_EntreeN output,
					@vl_SortieS output, @vl_SortieN output
		if @vl_Status != XDC_OK
			return XZAEC_FMC_PAS_DONNEES

		/* Ajout des informations dans la synthese */
		if @vl_NumTypeFmc = XZAEC_FMC_EchDeconseille
		begin
			if @vl_EntreeN = XDC_OUI
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ENT_N_DECONS
				insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
				values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
			end
			if @vl_EntreeS = XDC_OUI
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ENT_S_DECONS
				insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
				values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
			end
			if @vl_SortieN = XDC_OUI
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_SOR_N_DECONS
				insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
				values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
			end
			if @vl_SortieS = XDC_OUI
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_SOR_S_DECONS
				insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
				values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
			end
		end
		else
		begin
			if @vl_EntreeN = XDC_OUI
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
						   XZAEC_SYN_ENT_N_FERM
			else	select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_ENT_N_OUV
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

			if @vl_EntreeS = XDC_OUI
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
						   XZAEC_SYN_ENT_S_FERM
			else	select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_ENT_S_OUV
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

			if @vl_SortieN = XDC_OUI
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
						   XZAEC_SYN_SOR_N_FERM
			else	select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_SOR_N_OUV
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

			if @vl_SortieS = XDC_OUI
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
						   XZAEC_SYN_SOR_S_FERM
			else	select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_SOR_S_OUV
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
	end

/************************************************************************************************************************/
/**                                       Cas d'un Incident sous-concessionnaire                                       **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClasseSousConcess
	begin
		/* recherche des informations du concessionnaire */
		exec @vl_Status = XZAE;50 @va_NumEvt_in, @va_CleEvt_in, @vl_HoroValidation,
					@vl_NomCnc output, @vl_CncFerme output,
					@vl_Penurie output, @vl_Carburant output
		if @vl_Status != XDC_OK
			return XZAEC_FMC_PAS_DONNEES

		if @vl_NomCnc != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_CNC + @vl_NomCnc
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_Penurie = XDC_OUI
		begin
			/* recherche le nom du carburant manquant */
			select @vl_NomCarburant = nom from CFG..TYP_CRB
			where numero = @vl_Carburant

			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_PEN_CRB + @vl_NomCarburant
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

		if @vl_CncFerme = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_CNC_FERM
			insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

	end

/************************************************************************************************************************/
/**                                     Cas d'un evenement de classe Manifestation                                     **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClasseManif
	begin
		/* recherche les informations sur la manifestation */
		exec @vl_Status = XZAE;47 @vl_HoroValidation, @va_NumEvt_in, @va_CleEvt_in,
					@vl_Nombre output, 
					@vl_Perception output,
					@vl_Violence output
		if @vl_Status != XDC_OK
			return XZAEC_FMC_PAS_DONNEES

		/* Ajout des informations dans la synthese */
		if @vl_Nombre != null
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_NBR_MNF + 
					convert(varchar(8), @vl_Nombre)
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
		if @vl_Violence = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_VIO_MNF 
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
		if @vl_Perception = XZAEC_FMC_PAS_PER
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_PAS_PER_PEA
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

	end

/************************************************************************************************************************/
/**                                               Cas d'un Bouchon (tete)                                              **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClasseGenerique and 
		@vl_NumTypeFmc = XZAEC_FMC_TeteBouchon
	begin
		/* ajout de l'intitule de la tete de bouchon */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_TET + 
					' ' + @vl_Localisation
		insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

		/* recherche les informations sur le bouchon */
		exec @vl_Status = XZAE;51 @vl_HoroValidation, @va_NumEvt_in,
					@va_CleEvt_in, @vl_PR output,
					@vl_TypeBouchon output, 
					@vl_Longueur output,
					@vl_NumEvtQueue output, 
					@vl_CleEvtQueue output
		if @vl_Status != XDC_OK
			return XZAEC_FMC_PAS_DONNEES


		/* Creation de l'intitule d'une creation de FMC de queue */
		/* de bouchon                                            */
		select 
			@vl_HoroValidation = EXP..FMC_HIS.horodate_validation,
			@vl_Autoroute	= EXP..FMC_HIS.autoroute,
			@vl_PR		= EXP..FMC_HIS.PR,
			@vl_Sens	= EXP..FMC_HIS.sens
		from EXP..FMC_HIS
		where	EXP..FMC_HIS.numero = @vl_NumEvtQueue and 
			EXP..FMC_HIS.cle = @vl_CleEvtQueue
		group by EXP..FMC_HIS.numero, EXP..FMC_HIS.cle
		having EXP..FMC_HIS.horodate_validation = 
				min(EXP..FMC_HIS.horodate_validation)

		if @@rowcount != 1
			return XZAEC_FMC_ERR_VAL

		/* memoriser la localisation initiale de la queue pour les actions */
		select @vl_Autoroute_Prec = @vl_Autoroute, @vl_PR_Prec = @vl_PR, @vl_Sens_Prec = @vl_Sens

		/* le type pour un bouchon est change pour l'affichage */
		/* (libelle plus explicite)                            */
		select @vl_TypeFmc = XZAEC_SYN_FMC_QueueBouchon

		/* mise en forme de l'intitule de la queue de bouchon */
		exec XZAE83	null, null, null, null,
				@vl_Autoroute, @vl_PR, @vl_Sens, 
				@vl_Intitule2 output, @vl_Localisation2 output

		/* ajout de l'intitule de la queue de bouchon */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_QUE + 
					@vl_Localisation2
		insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

		/* ajout de la longueur du bouchon */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_LON +
					convert(varchar(8), @vl_Longueur/1000.0)

		insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

		/* ajout du type du bouchon */
		if @vl_TypeBouchon = XZAEC_BOU_Dense
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					   XZAEC_SYN_BOU_DNS
		else if @vl_TypeBouchon = XZAEC_BOU_Ralenti
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					   XZAEC_SYN_BOU_RAL
		else if @vl_TypeBouchon = XZAEC_BOU_Accordeon
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					   XZAEC_SYN_BOU_ACD
		else if @vl_TypeBouchon = XZAEC_BOU_Bouche
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					   XZAEC_SYN_BOU_BOU
		else if @vl_TypeBouchon = XZAEC_BOU_Bloque
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					   XZAEC_SYN_BOU_BLQ
		else return XZAEC_FMC_ERR_DONNEES

		insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

		/* Ajouter dans l'historique de la fmc celui de la */
		/* queue de bouchon ordonne suivant la date        */
		/* sans l'enregistrement correspondant a la creation*/
		/* de la fmc de queue de bouchon                    */
		insert #TEMPO_HISTORIQUE_FMC
			(numero, cle, classe, type, nom_type, type_anterieur, autoroute,
			PR, sens, confirme_ou_signale, horodate_validation,
			point_caracteristique, numero_point_caracteristique,
			critere_ordre, VR, VM2, VM1, VL, BAU, VR_I, 
			VM2_I, VM1_I, VL_I, BAU_I, BAU_etroite, BAU_I_etroite)
		select 
			EXP..FMC_HIS.numero,
			EXP..FMC_HIS.cle,
			CFG..TYP_FMC.classe,
			EXP..FMC_HIS.type_FMC_valide,
			CFG..TYP_FMC.nom,
			EXP..FMC_HIS.type_anterieur,
			EXP..FMC_HIS.autoroute,
			EXP..FMC_HIS.PR,
			EXP..FMC_HIS.sens,
			EXP..FMC_HIS.confirme_ou_signale,
			EXP..FMC_HIS.horodate_validation,
			EXP..FMC_HIS.point_caracteristique,
			EXP..FMC_HIS.numero_point_caracteristique,
			2,
			isnull(EXP..FMC_HIS.VR, XDC_VOIE_INCONNUE),
			isnull(EXP..FMC_HIS.VM2, XDC_VOIE_INCONNUE),
			isnull(EXP..FMC_HIS.VM1, XDC_VOIE_INCONNUE),
			isnull(EXP..FMC_HIS.VL, XDC_VOIE_INCONNUE),
			isnull(EXP..FMC_HIS.BAU, XDC_VOIE_INCONNUE),
			isnull(EXP..FMC_HIS.VR_I, XDC_VOIE_INCONNUE),
			isnull(EXP..FMC_HIS.VM2_I, XDC_VOIE_INCONNUE),
			isnull(EXP..FMC_HIS.VM1_I, XDC_VOIE_INCONNUE),
			isnull(EXP..FMC_HIS.VL_I, XDC_VOIE_INCONNUE),
			isnull(EXP..FMC_HIS.BAU_I, XDC_VOIE_INCONNUE),
			isnull(EXP..FMC_HIS.BAU_etroite, XDC_FAUX),
			isnull(EXP..FMC_HIS.BAU_I_etroite, XDC_FAUX)
		from EXP..FMC_HIS, CFG..TYP_FMC
		where	EXP..FMC_HIS.numero = @vl_NumEvtQueue and
			EXP..FMC_HIS.cle = @vl_CleEvtQueue and
			CFG..TYP_FMC.numero = EXP..FMC_HIS.type_FMC_valide and
			EXP..FMC_HIS.horodate_validation != @vl_HoroValidation
			order by EXP..FMC_HIS.horodate_validation

		/* reordonner l'historique complet suivant la date */
		/* et les histo des fmc tete de bouchon avant      */
		/* ceux des fmc queue de bouchon pour une date     */
		/* identique                                       */
		insert #TEMPO_HISTORIQUE_FMC
		select * from #HISTORIQUE_FMC

		delete #HISTORIQUE_FMC
		insert #HISTORIQUE_FMC
		select * from #TEMPO_HISTORIQUE_FMC
		order by horodate_validation, critere_ordre

	end

/************************************************************************************************************************/
/**                                               Cas d'un Bouchon (queue)                                             **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClasseBouchon
	begin
		/* recherche les informations sur le bouchon */
		exec @vl_Status = XZAE;51 @vl_HoroValidation, @va_NumEvt_in,
					@va_CleEvt_in, @vl_PR output,
					@vl_TypeBouchon output, 
					@vl_Longueur output,
					@vl_NumEvtTete output, 
					@vl_CleEvtTete output

		if @vl_Status != XDC_OK
			return XZAEC_FMC_PAS_DONNEES

		/* Rechercher la cause reelle du bouchon */
		select
			@vl_Cause	= cause,
			@vl_CleCause	= cle_cause
		from EXP..FMC_GEN
		where numero = @vl_NumEvtTete and cle = @vl_CleEvtTete

		/* Creation de l'intitule de la creation de FMC de tete */
		select 
			@vl_HoroValidation	= EXP..FMC_HIS.horodate_validation,
			@vl_Autoroute		= EXP..FMC_HIS.autoroute,
			@vl_PR			= EXP..FMC_HIS.PR,
			@vl_Sens		= EXP..FMC_HIS.sens,
			@vl_PointCarTete	= EXP..FMC_HIS.point_caracteristique,
			@vl_NumPointCarTete	= EXP..FMC_HIS.numero_point_caracteristique
		from EXP..FMC_HIS
		where	EXP..FMC_HIS.numero = @vl_NumEvtTete and 
			EXP..FMC_HIS.cle = @vl_CleEvtTete 
		group by EXP..FMC_HIS.numero, EXP..FMC_HIS.cle
		having horodate_validation = min(horodate_validation)

		if @@rowcount != 1
			return XZAEC_FMC_ERR_VAL

		/* le type pour un bouchon est change pour l'affichage */
		/* (libelle plus explicite)                            */
		select @vl_TypeFmc = XZAEC_SYN_FMC_TeteBouchon

		/* determination de la localisation de la tete de bouchon */
		if @vl_PointCarTete = null  or  @vl_NumPointCarTete = null
		begin
			exec XZAE83	null, null, null, null,
					@vl_Autoroute, @vl_PR, @vl_Sens, 
					@vl_Intitule2 output, @vl_Localisation2 output

			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_TET + 
			                      @vl_Localisation2
		end
		else
		begin
			if @vl_PointCarTete = XDC_POINT_CARACT_AIRE
			begin
				select @vl_Libelle = XZAEC_SYN_VEH_AIR

				select @vl_NomPointCarTete = nom
				from CFG..RES_AIR
				where numero = @vl_NumPointCarTete
			end
			else if @vl_PointCarTete = XDC_POINT_CARACT_ECHANGEUR
			begin
				select @vl_Libelle = XZAEC_SYN_VEH_ECH

				select @vl_NomPointCarTete = nom
				from CFG..RES_ECH
				where numero = @vl_NumPointCarTete
			end
			else if @vl_PointCarTete = XDC_POINT_CARACT_PEAGE
			begin
				select @vl_Libelle = XZAEC_SYN_VEH_PEA

				select @vl_NomPointCarTete = nom
				from CFG..RES_PEA
				where numero = @vl_NumPointCarTete
			end
			else if @vl_PointCarTete = XDC_POINT_CARACT_LIT_ARRET
			begin
				select @vl_Libelle = XZAEC_SYN_VEH_LIT

				select @vl_NomPointCarTete = nom
				from CFG..EQT_GEN
				where	numero = @vl_NumPointCarTete and 
					type = XDC_CAPT_LIT_ARRET
			end
			else return XZAEC_FMC_LOC_INC

			if @@rowcount != 1
				return XZAEC_FMC_LOC_INC

			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_TET + 
			                      @vl_Libelle + @vl_NomPointCarTete
		end

		/* ajout de l'intitule de la tete de bouchon */
		insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

		/* ajout de l'intitule de la queue de bouchon */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_QUE + 
					' ' + @vl_Localisation
		insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

		/* ajout de la longueur du bouchon */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_LON +
					convert(varchar(8), @vl_Longueur/1000.0)

		insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

		/* ajout du type du bouchon */
		if @vl_TypeBouchon = XZAEC_BOU_Dense
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					   XZAEC_SYN_BOU_DNS
		else if @vl_TypeBouchon = XZAEC_BOU_Accordeon
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					   XZAEC_SYN_BOU_ACD
		else if @vl_TypeBouchon = XZAEC_BOU_Ralenti
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					   XZAEC_SYN_BOU_RAL
		else if @vl_TypeBouchon = XZAEC_BOU_Bouche
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					   XZAEC_SYN_BOU_BOU
		else if @vl_TypeBouchon = XZAEC_BOU_Bloque
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					   XZAEC_SYN_BOU_BLQ
		else return XZAEC_FMC_ERR_DONNEES

		insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)

		/* Ajouter dans l'historique de la fmc celui de la */
		/* tete de bouchon ordonne suivant la date         */
		/* sans l'enregistrement correspondant a la creation*/
		/* de la fmc de tete de bouchon                     */
		insert #TEMPO_HISTORIQUE_FMC
			(numero, cle, classe, type, nom_type, type_anterieur, autoroute,
			PR, sens, confirme_ou_signale, horodate_validation,
			point_caracteristique, numero_point_caracteristique,
			critere_ordre, VR, VM2, VM1, VL, BAU,
			VR_I, VM2_I, VM1_I, VL_I, BAU_I, BAU_etroite, BAU_I_etroite)
		select 
			EXP..FMC_HIS.numero,
			EXP..FMC_HIS.cle,
			CFG..TYP_FMC.classe,
			EXP..FMC_HIS.type_FMC_valide,
			CFG..TYP_FMC.nom,
			EXP..FMC_HIS.type_anterieur,
			EXP..FMC_HIS.autoroute,
			EXP..FMC_HIS.PR,
			EXP..FMC_HIS.sens,
			@vl_ConfirmeSignale,
			EXP..FMC_HIS.horodate_validation,
			EXP..FMC_HIS.point_caracteristique,
			EXP..FMC_HIS.numero_point_caracteristique,
			2,
			isnull(EXP..FMC_HIS.VR, XDC_VOIE_INCONNUE),
			isnull(EXP..FMC_HIS.VM2, XDC_VOIE_INCONNUE),
			isnull(EXP..FMC_HIS.VM1, XDC_VOIE_INCONNUE),
			isnull(EXP..FMC_HIS.VL, XDC_VOIE_INCONNUE),
			isnull(EXP..FMC_HIS.BAU, XDC_VOIE_INCONNUE),
			isnull(EXP..FMC_HIS.VR_I, XDC_VOIE_INCONNUE),
			isnull(EXP..FMC_HIS.VM2_I, XDC_VOIE_INCONNUE),
			isnull(EXP..FMC_HIS.VM1_I, XDC_VOIE_INCONNUE),
			isnull(EXP..FMC_HIS.VL_I, XDC_VOIE_INCONNUE),
			isnull(EXP..FMC_HIS.BAU_I, XDC_VOIE_INCONNUE),
			isnull(EXP..FMC_HIS.BAU_etroite, XDC_FAUX),
			isnull(EXP..FMC_HIS.BAU_I_etroite, XDC_FAUX)
		from EXP..FMC_HIS, CFG..TYP_FMC
		where	EXP..FMC_HIS.numero = @vl_NumEvtTete and 
			EXP..FMC_HIS.cle = @vl_CleEvtTete and
			CFG..TYP_FMC.numero = EXP..FMC_HIS.type_FMC_valide and
			EXP..FMC_HIS.horodate_validation != @vl_HoroValidation
			order by EXP..FMC_HIS.horodate_validation

		/* reordonner l'historique complet suivant la date */
		/* et les histo des fmc tete de bouchon avant      */
		/* ceux des fmc queue de bouchon pour une date     */
		/* identique                                       */
		insert #TEMPO_HISTORIQUE_FMC
		select * from #HISTORIQUE_FMC

		delete #HISTORIQUE_FMC
		insert #HISTORIQUE_FMC
		select * from #TEMPO_HISTORIQUE_FMC
		order by horodate_validation, critere_ordre

		/* positionner des var internes */
		select @vl_NumEvtQueue = @va_NumEvt_in, @vl_CleEvtQueue = @va_CleEvt_in
	end

/************************************************************************************************************************/
/**                                               Cas de Travaux                                                       **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc in (XZAEC_ClasseTravaux, XZAEC_ClasseBasculTrav)
	begin
		/* recherche les informations sur les travaux */
		select
			@vl_NatureTrv	= EXP..FMC_TRF.nature,
			@vl_TypeTrv	= EXP..FMC_TRF.type,
			@vl_Continu	= EXP..FMC_TRF.continu,
			@vl_DebutPlanifie = EXP..FMC_TRF.horodate_debut_planifiee,
			@vl_FinPlanifie = EXP..FMC_TRF.horodate_fin_planifiee,
			@vl_Mobile	= EXP..FMC_TRF.mobile,
			@vl_AutorouteM	= EXP..FMC_TRF.autoroute_planifiee,
			@vl_PRQueueM	= EXP..FMC_TRF.PR_queue_planifie,
			@vl_PRTeteM	= EXP..FMC_TRF.PR_tete_planifie,
			@vl_SensM	= EXP..FMC_TRF.sens_planifie,
			@vl_PR		= EXP..FMC_TRH.localisation_tete,
			@vl_VoiesRet	= EXP..FMC_TRH.voies_retrecies,
			@vl_PresenceBouchon	= EXP..FMC_TRH.bouchon,
			@vl_VR=VR,
			@vl_VM2=VM2,
			@vl_VM1=VM1,
			@vl_VL=VL,
			@vl_BAU=BAU,
			@vl_VR_I=VR_I,
			@vl_VM2_I=VM2_I,
			@vl_VM1_I=VM1_I,
			@vl_VL_I=VL_I,
			@vl_BAU_I=BAU_I,
			@vl_PRdebut_VR=PR_debut_VR,
			@vl_PRfin_VR=PR_fin_VR,
			@vl_debut_VR=debut_VR,
			@vl_fin_VR=fin_VR,
			@vl_PRdebut_VM2=PR_debut_VM2,
			@vl_PRfin_VM2=PR_fin_VM2,
			@vl_debut_VM2=debut_VM2,
			@vl_fin_VM2=fin_VM2,
			@vl_PRdebut_VM1=PR_debut_VM1,
			@vl_PRfin_VM1=PR_fin_VM1,
			@vl_debut_VM1=debut_VM1,
			@vl_fin_VM1=fin_VM1,
			@vl_PRdebut_VL=PR_debut_VL,
			@vl_PRfin_VL=PR_fin_VL,
			@vl_debut_VL=debut_VL,
			@vl_fin_VL=fin_VL,
			@vl_PRdebut_BAU=PR_debut_BAU,
			@vl_PRfin_BAU=PR_fin_BAU,
			@vl_debut_BAU=debut_BAU,
			@vl_fin_BAU=fin_BAU,
			@vl_PRdebut_VR_I=PR_debut_VR_I,
			@vl_PRfin_VR_I=PR_fin_VR_I,
			@vl_debut_VR_I=debut_VR_I,
			@vl_fin_VR_I=fin_VR_I,
			@vl_PRdebut_VM2_I=PR_debut_VM2_I,
			@vl_PRfin_VM2_I=PR_fin_VM2_I,
			@vl_debut_VM2_I=debut_VM2_I,
			@vl_fin_VM2_I=fin_VM2_I,
			@vl_PRdebut_VM1_I=PR_debut_VM1_I,
			@vl_PRfin_VM1_I=PR_fin_VM1_I,
			@vl_debut_VM1_I=debut_VM1_I,
			@vl_fin_VM1_I=fin_VM1_I,
			@vl_PRdebut_VL_I=PR_debut_VL_I,
			@vl_PRfin_VL_I=PR_fin_VL_I,
			@vl_debut_VL_I=debut_VL_I,
			@vl_fin_VL_I=fin_VL_I,
			@vl_PRdebut_BAU_I=PR_debut_BAU_I,
			@vl_PRfin_BAU_I=PR_fin_BAU_I,
			@vl_debut_BAU_I=debut_BAU_I,
			@vl_fin_BAU_I=fin_BAU_I,
			@vl_vitesse=EXP..FMC_TRH.vitesse,
			@vl_personnel=EXP..FMC_TRH.vitesse
		from EXP..FMC_TRF, EXP..FMC_TRH
		where	EXP..FMC_TRF.numero = @va_NumEvt_in and 
			EXP..FMC_TRF.cle = @va_CleEvt_in and
			EXP..FMC_TRH.numero = @va_NumEvt_in and 
			EXP..FMC_TRH.cle = @va_CleEvt_in  and
			EXP..FMC_TRH.horodate = @vl_HoroValidation

		if @@rowcount != 1
			return XZAEC_FMC_PAS_DONNEES

		if @vl_NumTypeFmc != XZAEC_FMC_ZoneStockage
		begin
			/* Ajout des informations sur les travaux dans la synthese */
			if @vl_TypeTrv != null
			begin
				if @vl_TypeTrv = XZAEC_TRV_Glissieres
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_GLI
				else if @vl_TypeTrv = XZAEC_TRV_Vegetation
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_VEG
				else if @vl_TypeTrv = XZAEC_TRV_OuvHydro
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_HYD
				else if @vl_TypeTrv = XZAEC_TRV_OuvArt
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_ART
				else if @vl_TypeTrv = XZAEC_TRV_Terrass
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_TER
				else if @vl_TypeTrv = XZAEC_TRV_SignalVert
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_SGV
				else if @vl_TypeTrv = XZAEC_TRV_SignalHori
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_SGH
				else if @vl_TypeTrv = XZAEC_TRV_Eclairage
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_ECL
				else if @vl_TypeTrv = XZAEC_TRV_CableRAU
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_RAU
				else if @vl_TypeTrv = XZAEC_TRV_LavBalTun
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_LBT
				else if @vl_TypeTrv = XZAEC_TRV_LavBalAutre
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_LBA
				else if @vl_TypeTrv = XZAEC_TRV_MaintTun
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_MTN
				else if @vl_TypeTrv = XZAEC_TRV_MaintAutre
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_MAU
				else if @vl_TypeTrv = XZAEC_TRV_Elargiss
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_ELA
				else if @vl_TypeTrv = XZAEC_TRV_MurAntibruit
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_MUR
				else if @vl_TypeTrv = XZAEC_TRV_ConfGliss
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_CGL
				else if @vl_TypeTrv = XZAEC_TRV_Autre
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_AUT
				else if @vl_TypeTrv = XZAEC_TRV_Enrobes
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_ENR
				else if @vl_TypeTrv = XZAEC_TRV_MaintVideo
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_MTV
				else if @vl_TypeTrv = XZAEC_TRV_PAU
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_PAU
				else if @vl_TypeTrv = XZAEC_TRV_Cloture
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_CLO
				else if @vl_TypeTrv = XZAEC_TRV_FO
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_FON
				else if @vl_TypeTrv = XZAEC_TRV_Accident
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_ACC
				else if @vl_TypeTrv = XZAEC_TRV_Stockage
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_STO
				else if @vl_TypeTrv = XZAEC_TRV_Tunnels
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							   XZAEC_SYN_TRV_TUN

				else	return XZAEC_FMC_ERR_DONNEES

				insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end

			if @vl_NatureTrv = XZAEC_FMC_TRV_NEU
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_TRV_NEU
			else	select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
						   XZAEC_SYN_TRV_ENT

			insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

			if @vl_personnel = 1
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_PERSONNEL
				insert EXP..FMC_LIS(numero,cle,ligne, heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end	
			if @vl_Mobile = XDC_OUI
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_TRV_MOB
				insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end

			if @vl_DebutPlanifie != null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_DEB_PLA + 
						   convert(char(10), @vl_DebutPlanifie, 103) + ' ' +
						   convert(char(8), @vl_DebutPlanifie, 8)

				insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end

			if @vl_FinPlanifie != null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_FIN_PLA +  
						   convert(char(10), @vl_FinPlanifie, 103) + ' ' +
						   convert(char(8), @vl_FinPlanifie, 8)

				insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end

			/* localisation des travaux planifies */


			if @vl_AutorouteM != null and @vl_Mobile = XDC_OUI and
			   @vl_PRTeteM is not null
			begin
				/* mise en forme de la localisation de la tete */
				/* des travaux planifies */
				exec XZAE83	null, null, null, null,
						@vl_AutorouteM, @vl_PRTeteM, @vl_SensM, 
						@vl_Intitule2 output, @vl_Localisation2 output

				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_TET_PLA + @vl_Localisation2
				insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

				/* mise en forme de la localisation de la tete */
				/* des travaux planifies */
				exec XZAE83	null, null, null, null,
						@vl_AutorouteM, @vl_PRQueueM, @vl_SensM, 
						@vl_Intitule2 output, @vl_Localisation2 output

				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_QUE_PLA + @vl_Localisation2
				insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
			end
		end


		if @vl_PR != null
		begin
			if @vl_NumTypeFmc = XZAEC_FMC_ZoneStockage
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_STK_PRT + 
						   convert(char(7), @vl_PR/1000.0)
			else
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_PR_TET + 
						   convert(char(7), @vl_PR/1000.0)
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

		if @vl_NumTypeFmc = XZAEC_FMC_ZoneStockage
		begin
			if @vl_VoiesRet = XDC_OUI
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_STK_ENCOURS
			else
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_STK_PREPAR
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
		else
		if @vl_VoiesRet = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_VOI_RET 
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

		/*configuration finale des voies*/
		if (@vl_VR <> null ) and (@vl_VM2 <> null ) and (@vl_VM1 <> null ) and (@vl_VL <> null ) and (@vl_BAU <> null ) and (@vl_VR_I <> null ) and (@vl_VM2_I <> null ) and (@vl_VM1_I <> null ) and (@vl_VL_I <> null ) and (@vl_BAU_I <> null ) 
		begin
			if @vl_ClasseFmc = XZAEC_ClasseBasculTrav
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_CFG_VOIES_BAS_TRV
			else
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_CFG_VOIES_BAS
			insert EXP..FMC_LIS
			values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null, @vl_VR, @vl_VM2, @vl_VM1,
			@vl_VL, @vl_BAU, @vl_VR_I, @vl_VM2_I, @vl_VM1_I,@vl_VL_I, @vl_BAU_I, 
			XDC_FAUX, XDC_FAUX, null,@va_NumPoste_in)
	
			select @vl_Ligne=XZAEC_SYN_PAS_DATE
			insert EXP..FMC_LIS (numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
	
			select @vl_Ligne=XZAEC_SYN_PAS_DATE + convert(char(10),XZAEC_SYN_VOIE_BAS) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert (char(8), XZAEC_SYN_PRD_BAS) + XZAEC_SYN_CAR_SEPAR+
					convert (char(8),XZAEC_SYN_PRF_BAS) + XZAEC_SYN_CAR_SEPAR +
					convert (char(16),XZAEC_SYN_HD_BAS) +XZAEC_SYN_CAR_SEPAR+
					convert (char(16),XZAEC_SYN_HF_BAS)
			insert EXP..FMC_LIS (numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end 

		/*tableau des neutralisations de voies*/
		if @vl_PRdebut_VR<>null
		begin
			select @vl_Contenu=libelle 
			from CFG..LIB_PRE
			where notyp = XDC_LIB_TYPE_VOIE and code = XDC_CODE_VOIE_VR
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + convert(char(10),@vl_Contenu) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRdebut_VR/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR+
					convert(char(8),@vl_PRfin_VR/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert (char(16),convert(char(10), @vl_debut_VR, 103) + ' ' + 
					convert(char(8), @vl_debut_VR, 8))
					+ XZAEC_SYN_CAR_SEPAR +
					convert (char(16),convert(char(10), @vl_fin_VR, 103) + ' ' +
					convert(char(8), @vl_fin_VR, 8))
			insert EXP..FMC_LIS (numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end 

		if @vl_PRdebut_VM2<>null
		begin
			select @vl_Contenu=libelle 
			from CFG..LIB_PRE
			where notyp = XDC_LIB_TYPE_VOIE and code = XDC_CODE_VOIE_VM2
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + convert(char(10),@vl_Contenu) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRdebut_VM2/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRfin_VM2/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert (char(16),convert(char(10), @vl_debut_VM2, 103) + ' ' + 
					convert(char(8), @vl_debut_VM2, 8)) 
					+ XZAEC_SYN_CAR_SEPAR+
					convert (char(16),convert(char(10), @vl_fin_VM2, 103) + ' ' +
					convert(char(8), @vl_fin_VM2, 8))
			insert EXP..FMC_LIS (numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end 
		
		if @vl_PRdebut_VM1<>null
		begin
			select @vl_Contenu=libelle 
			from CFG..LIB_PRE
			where notyp = XDC_LIB_TYPE_VOIE and code = XDC_CODE_VOIE_VM1
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + convert(char(10),@vl_Contenu) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRdebut_VM1/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRfin_VM1/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert (char(16),convert(char(10), @vl_debut_VM1, 103) + ' ' + 
					convert(char(8), @vl_debut_VM1, 8))
					+ XZAEC_SYN_CAR_SEPAR +
					convert (char(16),convert(char(10), @vl_fin_VM1, 103) + ' ' +
					convert(char(8), @vl_fin_VM1, 8))
			insert EXP..FMC_LIS (numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end 

		if @vl_PRdebut_VL<>null
		begin
			select @vl_Contenu=libelle 
			from CFG..LIB_PRE
			where notyp = XDC_LIB_TYPE_VOIE and code = XDC_CODE_VOIE_VL
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + convert(char(10),@vl_Contenu) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRdebut_VL/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRfin_VL/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert (char(16), convert(char(10), @vl_debut_VL, 103) + ' ' + 
							convert(char(8), @vl_debut_VL, 8)) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert (char(16), convert(char(10), @vl_fin_VL, 103) + ' ' +
							convert(char(8), @vl_fin_VL, 8))
			insert EXP..FMC_LIS (numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end 

		if @vl_PRdebut_BAU<>null
		begin
			select @vl_Contenu=libelle 
			from CFG..LIB_PRE
			where notyp = XDC_LIB_TYPE_VOIE and code = XDC_CODE_VOIE_BAU
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + convert(char(10),@vl_Contenu) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRdebut_BAU/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRfin_BAU/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert (char(16), convert(char(10), @vl_debut_BAU, 103) + ' ' + 
					convert(char(8), @vl_debut_BAU, 8)) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert (char(16),convert(char(10), @vl_fin_BAU, 103) + ' ' +
					convert(char(8), @vl_fin_VR, 8))
			insert EXP..FMC_LIS (numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end 

		if @vl_PRdebut_VR_I<>null
		begin
			select @vl_Contenu=libelle 
			from CFG..LIB_PRE
			where notyp = XDC_LIB_TYPE_VOIE and code = XDC_CODE_VOIE_VR_I
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + convert(char(10),@vl_Contenu) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRdebut_VR_I/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRfin_VR_I/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert (char(16),convert(char(10), @vl_debut_VR_I, 103) + ' ' + 
					convert(char(8), @vl_debut_VR_I, 8))
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(16),convert(char(10), @vl_fin_VR_I, 103) + ' ' +
					convert(char(8), @vl_fin_VR_I, 8))
			insert EXP..FMC_LIS (numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end 

		if @vl_PRdebut_VM2_I<>null
		begin
			select @vl_Contenu=libelle 
			from CFG..LIB_PRE
			where notyp = XDC_LIB_TYPE_VOIE and code = XDC_CODE_VOIE_VM2_I
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + convert(char(10),@vl_Contenu) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRdebut_VM2_I/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRfin_VM2_I/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert (char(16),convert(char(10), @vl_debut_VM2_I, 103) + ' ' + 
					convert(char(8), @vl_debut_VM2_I, 8))
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(16),convert(char(10), @vl_fin_VM2_I, 103) + ' ' +
					convert(char(8), @vl_fin_VM2_I, 8))
			insert EXP..FMC_LIS (numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end 

		if @vl_PRdebut_VM1_I<>null
		begin
			select @vl_Contenu=libelle 
			from CFG..LIB_PRE
			where notyp = XDC_LIB_TYPE_VOIE and code = XDC_CODE_VOIE_VM1_I
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + convert(char(10),@vl_Contenu) 
					+ XZAEC_SYN_CAR_SEPAR+
					convert(char(8),@vl_PRdebut_VM1_I/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRfin_VM1_I/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(16),convert(char(10), @vl_debut_VM1_I, 103) + ' ' + 
					convert(char(8), @vl_debut_VM1_I, 8))
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(16),convert(char(10), @vl_fin_VM1_I, 103) + ' ' +
					convert(char(8), @vl_fin_VM1_I, 8))
			insert EXP..FMC_LIS (numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end 

		if @vl_PRdebut_VL_I<>null
		begin
			select @vl_Contenu=libelle 
			from CFG..LIB_PRE
			where notyp = XDC_LIB_TYPE_VOIE and code = XDC_CODE_VOIE_VL_I
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + convert(char(10),@vl_Contenu) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRdebut_VL_I/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRfin_VL_I/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(16),convert(char(10), @vl_debut_VL_I, 103) + ' ' + 
					convert(char(8), @vl_debut_VL_I, 8))
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(16),convert(char(10), @vl_fin_VL_I, 103) + ' ' +
					convert(char(8), @vl_fin_VL_I, 8))
			insert EXP..FMC_LIS (numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end 

		if @vl_PRdebut_BAU_I<>null
		begin
			select @vl_Contenu=libelle 
			from CFG..LIB_PRE
			where notyp = XDC_LIB_TYPE_VOIE and code = XDC_CODE_VOIE_BAU_I
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + convert(char(10),@vl_Contenu) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRdebut_BAU_I/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(8),@vl_PRfin_BAU_I/1000.0) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(16),convert(char(10), @vl_debut_BAU_I, 103) + ' ' + 
					convert(char(8), @vl_debut_BAU_I, 8))
					+ XZAEC_SYN_CAR_SEPAR +
					convert(char(16),convert(char(10), @vl_fin_BAU_I, 103) + ' ' +
					convert(char(8), @vl_fin_BAU_I, 8))
			insert EXP..FMC_LIS (numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end


		/* ajouter les configurations de voies du Basculement pour travaux */
		if @vl_ClasseFmc = XZAEC_ClasseBasculTrav
		begin
			select @vl_Ligne=XZAEC_SYN_PAS_DATE
			insert EXP..FMC_LIS (numero,cle,ligne,heure,poste)
			values (@va_NumEvt_in, @va_CleEvt_in, @vl_Ligne, null, @va_NumPoste_in)

			select @vl_VR=VR, @vl_VM2=VM2, @vl_VM1=VM1, @vl_VL=VL, @vl_BAU=BAU,
			       @vl_VR_I=VR_I, @vl_VM2_I=VM2_I, @vl_VM1_I=VM1_I, @vl_VL_I=VL_I, @vl_BAU_I=BAU_I
			from EXP..FMC_BAS
			where numero = @va_NumEvt_in and cle = @va_CleEvt_in

			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_CFG_VOIES_BAS
			insert EXP..FMC_LIS
			values ( @va_NumEvt_in, @va_CleEvt_in, @vl_Ligne, null,
			         @vl_VR, @vl_VM2, @vl_VM1, @vl_VL, @vl_BAU,
			         @vl_VR_I, @vl_VM2_I, @vl_VM1_I,@vl_VL_I, @vl_BAU_I,
			         XDC_FAUX, XDC_FAUX, null, @va_NumPoste_in)
		end

		/* Pour un Basculement pour travaux, indiquer la présence d'un bouchon mobile */
		if @vl_NumTypeFmc = XZAEC_FMC_BasculTravaux
		begin
			if @vl_PresenceBouchon = XDC_VRAI
			begin
				select @vl_Ligne=XZAEC_SYN_PAS_DATE
				insert EXP..FMC_LIS (numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in, @vl_Ligne, null, @va_NumPoste_in)

				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_MOBILE
				insert EXP..FMC_LIS (numero,cle,ligne,heure,poste)
				values (@va_NumEvt_in, @va_CleEvt_in, @vl_Ligne, null, @va_NumPoste_in)
			end
		end
	end

/************************************************************************************************************************/
/**                                               Cas d'un Basculement                                                 **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClasseBasculement
	begin
		/* recherche les informations sur le basculement */
		select
			@vl_VR=VR,
			@vl_VM2=VM2,
			@vl_VM1=VM1,
			@vl_VL=VL,
			@vl_BAU=BAU,
			@vl_VR_I=VR_I,
			@vl_VM2_I=VM2_I,
			@vl_VM1_I=VM1_I,
			@vl_VL_I=VL_I,
			@vl_BAU_I=BAU_I,
			@vl_PR = localisation_PR_fin,
			@vl_vitesse=vitesse,
			@vl_vitesse_opposee=vitesse_opposee
		from EXP..FMC_BAS
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in

		if @vl_PR != null
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_FIN_BAS +
					convert(char(7),@vl_PR/1000.0)
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end

		/*configuration finale des voies*/
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_CFG_VOIES_BAS
		insert EXP..FMC_LIS
		values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null, @vl_VR, @vl_VM2, @vl_VM1,
		@vl_VL, @vl_BAU, @vl_VR_I, @vl_VM2_I, @vl_VM1_I,@vl_VL_I, @vl_BAU_I, 
		XDC_FAUX, XDC_FAUX, null,@va_NumPoste_in)
	end

/************************************************************************************************************************/
/**                                            Cas d'un evenement de Operateur                                         **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClassePosteOper
	begin
		/* recherche les informations sur le poste operateur */
		select @vl_Operateur = operateur
		from EXP..FMC_OPR
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in 

		/* ajout l'information sur le poste operateur dans la synthese */
		if @vl_Operateur != null
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_OPR +
						nom
			from CFG..OPR_NOM
			where numero = @vl_Operateur

			if @@rowcount != 1
				return XZAEC_FMC_PAS_DONNEES

			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end

	end

/************************************************************************************************************************/
/**                                    Cas d'un evenement de classe Incendie                                           **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClasseIncendie
	begin
		/* recherche les informations sur l'incendie */
		select	@vl_code_nature = code_nature,
			@vl_FeuMaitrise = feu_maitrise,
			@vl_HorodateMaitrise = horodate_maitrise
		from EXP..FMC_INC
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in

		select	@vl_nature_inc = libelle
		from CFG..LIB_PRE
		where	notyp = XDC_LIB_TYPE_NATURE_INCENDIE  and  code = @vl_code_nature

		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_NATURE + @vl_nature_inc
		insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

		if @vl_FeuMaitrise = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_INC_MAITRISE +
					convert(char(10),@vl_HorodateMaitrise,103) + ' ' +
					convert(char(8),@vl_HorodateMaitrise,8)
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
		end
	end

/************************************************************************************************************************/
/**                                               Cas général                                                          **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc != XZAEC_ClasseGenerique
		return XZAEC_FMC_ERR_DONNEES

	/* Ajout des champs datex */
	if @vl_Datex=XDC_VRAI
	begin
		select 
			@vl_gravite=gravite,
			@vl_trafic=trafic,
			@vl_bloquant=bloquant
		from EXP..FMC_DAT
		where numero= @va_NumEvt_in and cle = @va_CleEvt_in
		
		if @vl_gravite=0
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_GRAVITE + XZAEC_SYN_GRAVITE0
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
		else if @vl_gravite=1
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_GRAVITE + XZAEC_SYN_GRAVITE1
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
		else if @vl_gravite=2
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_GRAVITE + XZAEC_SYN_GRAVITE2
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
		else if @vl_gravite=3
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_GRAVITE + XZAEC_SYN_GRAVITE3
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
		
		if @vl_trafic=XDC_RADT_FLUIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_TRAFIC + XZAEC_SYN_TRAFIC_FLUIDE
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
		else if @vl_trafic=XDC_RADT_PRESAT
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_TRAFIC + XZAEC_SYN_TRAFIC_PRESAT
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
		else if @vl_trafic=XDC_RADT_SATURE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_TRAFIC + XZAEC_SYN_TRAFIC_SATURE
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
		end
		
		if @vl_bloquant=XDC_VRAI
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BLOQUANT
		else
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_NONBLOQUANT
		
		insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_Ligne,null,@va_NumPoste_in)
	
	end


/************************************************************************************************************************/
/************************************************************************************************************************/
/**                                   Descriptions des validations successives de la FMC                               **/
/************************************************************************************************************************/
/************************************************************************************************************************/
	/*A Recherche les descriptions des validations successives de la FMC */
	declare Pointeur_histo cursor
	for select numero, cle, classe, nom_type, type, type_anterieur,
		autoroute, PR, sens, confirme_ou_signale, horodate_validation,
		VR, VM2, VM1, VL, BAU, VR_I, VM2_I, VM1_I, VL_I, BAU_I, BAU_etroite, BAU_I_etroite
	from #HISTORIQUE_FMC

	open Pointeur_histo
	fetch Pointeur_histo into @vl_NumEvt, @vl_CleEvt, @vl_ClasseFmc,
				@vl_TypeFmc, @vl_NumTypeFmc, @vl_TypeAnterieur, 
				@vl_Autoroute, @vl_PR, @vl_Sens,
				@vl_ConfirmeSignale, @vl_HoroValidation,
				@vl_VR, @vl_VM2, @vl_VM1, @vl_VL, @vl_BAU, 
				@vl_VR_I, @vl_VM2_I, @vl_VM1_I,
				@vl_VL_I, @vl_BAU_I, @vl_BAU_etroite, @vl_BAU_I_etroite
	select @vl_SqlStatusValidation = @@sqlstatus

	while (@vl_SqlStatusValidation = 0)
	begin
		if @vl_IndicateurCreation != XDC_OUI
		begin
			/* le type pour un bouchon est change pour l'affichage */
			/* (libelle plus explicite)                            */
			if @vl_NumTypeFmc = XZAEC_FMC_TeteBouchon
				select @vl_TypeFmc = XZAEC_SYN_FMC_TeteBouchon
			else if @vl_NumTypeFmc = XZAEC_FMC_QueueBouchon
				select @vl_TypeFmc = XZAEC_SYN_FMC_QueueBouchon

			/* Creation de l'intitule d'une validation de FMC */
			exec XZAE83	@vl_NumEvt, @vl_CleEvt, @vl_NumTypeFmc, @vl_TypeFmc, 
					@vl_Autoroute, @vl_PR, @vl_Sens, 
					@vl_Intitule output, @vl_Localisation output
			select @vl_Ligne = convert(char(10), @vl_HoroValidation, 103) +
					' ' + convert(char(8), @vl_HoroValidation, 8) +
					' ' + convert(varchar(60),@vl_Intitule) + ' - ' + XZAEC_SYN_VAL 

			/* s'il y a eu retypage a cette validation, indiquer l'ancien type */
			if @vl_TypeAnterieur != @vl_NumTypeFmc
			begin
				select @vl_Ligne =  @vl_Ligne + ' ' + nom
				from CFG..TYP_FMC
				where numero = @vl_TypeAnterieur
			end

			/*B Ajout de l'intitule d'une validation de FMC */
			/*B dans la synthese */
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, @vl_HoroValidation,@va_NumPoste_in)
	
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + @vl_Localisation
			insert EXP..FMC_LIS 
			values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,
				@vl_VR, @vl_VM2, @vl_VM1, @vl_VL, @vl_BAU,
				@vl_VR_I, @vl_VM2_I, @vl_VM1_I,@vl_VL_I, @vl_BAU_I,
				@vl_BAU_etroite, @vl_BAU_I_etroite, null,@va_NumPoste_in)
	
			if @vl_ConfirmeSignale = XZAEC_FMC_CONFIRMEE
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
						XZAEC_SYN_CONFIRMEE
			else	select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_SIGNALEE
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values ( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
	
			/*B Ajout du complement de la validation de */
			/*B FMC dans la synthese                    */
			exec @vl_Status = XZAE205 @vl_NumEvt, @vl_CleEvt, 
						@vl_ClasseFmc, @vl_NumTypeFmc, 
						@vl_HoroValidation,
						@vl_Autoroute, @vl_PR, @vl_Sens,@va_NumPoste_in
	
			if @vl_Status != XDC_OK
				return XZAEC_FMC_ERR_DON_VAL
		end

		/* memoriser les donnees de la validation venant d'etre traitee   */
		/* pour les bouchons seule la localisation de la queue est retenue */
		select @vl_HoroValPrec = @vl_HoroValidation
		if @vl_NumTypeFmc <> XZAEC_FMC_TeteBouchon
			select @vl_Autoroute_Prec = @vl_Autoroute, @vl_PR_Prec = @vl_PR, @vl_Sens_Prec = @vl_Sens

		/* lecture de la validation suivante */
		fetch Pointeur_histo into @vl_NumEvt, @vl_CleEvt, @vl_ClasseFmc,
				@vl_TypeFmc, @vl_NumTypeFmc, @vl_TypeAnterieur,
				@vl_Autoroute, @vl_PR, @vl_Sens, 
				@vl_ConfirmeSignale, @vl_HoroValidation,
				@vl_VR, @vl_VM2, @vl_VM1, @vl_VL, @vl_BAU, 
				@vl_VR_I, @vl_VM2_I, @vl_VM1_I,
				@vl_VL_I, @vl_BAU_I, @vl_BAU_etroite, @vl_BAU_I_etroite

		/* memorisation de l'etat du curseur de validation */
		select @vl_SqlStatusValidation = @@sqlstatus

		/* trace pour DEBUG SEULEMENT (a mettre en commentaire) */
--		if @vl_SqlStatusValidation <> 2
--			print "Validation FMC %1! a %2!, Loc=%3!-%4!-%5! / Precedente a %6!, Loc=%7!-%8!-%9!",
--				@vl_NumEvt, @vl_HoroValidation, @vl_Autoroute, @vl_Sens, @vl_PR,
--				@vl_HoroValPrec, @vl_Autoroute_Prec, @vl_Sens_Prec, @vl_PR_Prec

		/*A Rechercher les consequences de cette fmc */
		if @vl_IndicateurCreation = XDC_OUI
		begin
			if @vl_Cloture is not null
				select @vl_DateRecherche = @vl_Cloture
			else	select @vl_DateRecherche = getdate()
			exec @vl_Status = XZAE;70 @va_NumEvt_in, @va_CleEvt_in, @vl_DateRecherche, XZAEC_MemoriserConseq
		
			select @vl_nb = count(*) from LISTE_CONSEQ
			where spid = @@spid
		
			if @vl_Status = XDC_OK and @vl_nb > 0 
			begin
				select @vl_InitLigne = XZAEC_SYN_CONSEQ, @vl_Date = @vl_Debut
		
				declare Pointeur_conseq cursor
				for select numero, cle, type from LISTE_CONSEQ
				where spid = @@spid
		
				open Pointeur_conseq 
				fetch Pointeur_conseq  into  @vl_NumEvtConseq, @vl_CleEvtConseq, @vl_NumTypeFmcConseq
				while (@@sqlstatus = 0)
				begin
					/* recherche d'une localisation de la consequence */
					select 
						 @vl_Autoroute_conseq = autoroute, 
						 @vl_PR_conseq = PR, 
						 @vl_Sens_conseq = sens, 
						 @vl_TypeFmcConseq = nom
					from EXP..FMC_GEN, EXP..FMC_HIS, CFG..TYP_FMC
					where EXP..FMC_GEN.numero = @vl_NumEvtConseq and EXP..FMC_GEN.cle = @vl_CleEvtConseq and
						 CFG..TYP_FMC.numero =  EXP..FMC_GEN.type and
						 EXP..FMC_HIS.numero = EXP..FMC_GEN.numero and EXP..FMC_HIS.cle = EXP..FMC_GEN.cle and
						 EXP..FMC_HIS.horodate_validation in (	select min(horodate_validation) 
											from EXP..FMC_HIS
											where numero = @vl_NumEvtConseq and 
												cle = @vl_CleEvtConseq)
		
					/* presenter le numero de la queue de bouchon a la place de */
					/* celui de la tete et son type                             */
					if @vl_NumTypeFmcConseq = XZAEC_FMC_TeteBouchon 
					begin
						select @vl_NumEvtConseq = numero from EXP..FMC_GEN
						where cause = @vl_NumEvtConseq and cle_cause = @vl_CleEvtConseq

						select @vl_TypeFmcConseq = nom from CFG..TYP_FMC
						where numero = XZAEC_FMC_QueueBouchon
					end
		
					/* Creation de l'intitule d'une validation de FMC */
					exec XZAE83	@vl_NumEvtConseq, @vl_CleEvtConseq,
							@vl_NumTypeFmcConseq, @vl_TypeFmcConseq,
							@vl_Autoroute_conseq, @vl_PR_conseq, @vl_Sens_conseq, 
							@vl_Intitule output, @vl_Localisation output

					select @vl_Ligne =  @vl_InitLigne + 
					'N' + convert(char(6),@va_NumEvt_in) + ' - ' + convert(char(2), @va_CleEvt_in)+ ' : ' +
					rtrim(@vl_Intitule) + ' ' + @vl_Localisation
			
					insert #SYNTHESE_CONSEQ (numero,cle,ligne, heure, VR, VM2, VM1, VL, BAU,
								VR_I, VM2_I, VM1_I, VL_I, BAU_I, BAU_etroite, BAU_I_etroite,poste)
					values ( @va_NumEvt_in,@va_CleEvt_in,
						@vl_Ligne, @vl_Date, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE,
						XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE,
						XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_FAUX, XDC_FAUX,
						@va_NumPoste_in)
		
					select @vl_InitLigne = XZAEC_SYN_PAS_DATE, @vl_Date = null
					fetch Pointeur_conseq  into  @vl_NumEvtConseq, 
								@vl_CleEvtConseq, @vl_NumTypeFmcConseq
				end
				close Pointeur_conseq
		
				delete LISTE_CONSEQ
				where spid = @@spid

			end
			else
			begin
				select @vl_Ligne = XZAEC_SYN_CONSEQ + 
				'N' + convert(char(6),@va_NumEvt_in) + ' - ' + convert(char(2), @va_CleEvt_in)+ ' : ' +
				 XZAEC_SYN_AUCUNE_CONSEQ
		
				insert #SYNTHESE_CONSEQ (numero,cle,ligne, heure, VR, VM2, VM1, VL, BAU,
							VR_I, VM2_I, VM1_I, VL_I, BAU_I, BAU_etroite, BAU_I_etroite,poste)
				values ( @va_NumEvt_in,@va_CleEvt_in,
					@vl_Ligne, @vl_Debut, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE,
					XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE,
					XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_FAUX, XDC_FAUX,
					@va_NumPoste_in)
			end
			

			/*B Ajout la cause de la FMC */
			if @vl_Cause != null and @vl_CleCause != null
			begin
				/* recherche les informations sur la cause */
				select
					@vl_Autoroute_cause = EXP..FMC_HIS.autoroute,
					@vl_PR_cause = EXP..FMC_HIS.PR,
					@vl_Sens_cause = EXP..FMC_HIS.sens,
					@vl_NumTypeCause = CFG..TYP_FMC.numero,
					@vl_TypeCause = CFG..TYP_FMC.nom
				from	EXP..FMC_GEN, EXP..FMC_HIS, CFG..TYP_FMC
				where	EXP..FMC_GEN.numero = @vl_Cause and 
					EXP..FMC_GEN.cle = @vl_CleCause and
					EXP..FMC_HIS.numero = @vl_Cause and
					EXP..FMC_HIS.cle = @vl_CleCause and
					CFG..TYP_FMC.numero = EXP..FMC_GEN.type 
					group by EXP..FMC_HIS.numero, EXP..FMC_HIS.cle,
					EXP..FMC_GEN.numero, EXP..FMC_GEN.cle,
					CFG..TYP_FMC.numero
					having EXP..FMC_HIS.horodate_validation =
						min(EXP..FMC_HIS.horodate_validation)
	
				if @@rowcount = 1
				begin
					/* localisation de la cause */
					exec XZAE83	@vl_Cause, @vl_CleCause, @vl_NumTypeCause, @vl_TypeCause,
							@vl_Autoroute_cause, @vl_PR_cause, @vl_Sens_cause, 
							@vl_Intitule output, @vl_Localisation output
		
					/* insertion de la cause */
					select @vl_Ligne = XZAEC_SYN_CAUSE + 
					'N' + convert(char(6),@va_NumEvt_in) + ' - ' + convert(char(2), @va_CleEvt_in)
					+ ' : ' + rtrim(@vl_Intitule) 
					+ ' ' + @vl_Localisation
		
					insert #SYNTHESE_CONSEQ (numero, cle, ligne, heure, VR, VM2, VM1, VL, BAU,
								VR_I, VM2_I, VM1_I, VL_I, BAU_I,
								BAU_etroite, BAU_I_etroite, poste)
					values ( @va_NumEvt_in, @va_CleEvt_in,
						@vl_Ligne, @vl_Debut, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE,
						XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE,
						XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_FAUX, XDC_FAUX,
						@va_NumPoste_in)
				end
			end

			/* memoriser le fait que la creation a ete ajoutee */
			select @vl_IndicateurCreation = XDC_NON

			/* si c'est un bouchon alors on recherche les actions par rapport a la queue */
			if @vl_NumEvtQueue is not null
			begin
				select @vl_NumEvtUtilise = @vl_NumEvt, @vl_CleEvtUtilise = @vl_CleEvt
				select @vl_NumEvt = @vl_NumEvtQueue, @vl_CleEvt = @vl_CleEvtQueue
			end

			/*B Recherche les commentaires avant la validation suivante de la FMC */
			/*! attention les commentaires peuevent etre inseres avant la premiere */
			/*! validation                                                         */
			insert #COMMENTAIRES
			select * from EXP..FMC_COM
			where	evenement = @vl_NumEvt and
				cle = @vl_CleEvt and
				horodate_validation < @vl_HoroValidation
				order by horodate_validation, numero
		end
		else
		begin
			/* si c'est un bouchon alors on recherche les actions par rapport a la queue */
			if @vl_NumEvtQueue is not null
			begin
				select @vl_NumEvtUtilise = @vl_NumEvt, @vl_CleEvtUtilise = @vl_CleEvt
				select @vl_NumEvt = @vl_NumEvtQueue, @vl_CleEvt = @vl_CleEvtQueue
			end

			/*B Recherche les commentaires inseres entre la validation precedente et */
			/*B la presente validation de la FMC */
			insert #COMMENTAIRES
			select * from EXP..FMC_COM
			where	evenement = @vl_NumEvt and
				cle = @vl_CleEvt and
				horodate_validation >= @vl_HoroValPrec and
				(@vl_SqlStatusValidation = 2 or 
				horodate_validation < @vl_HoroValidation)
				order by horodate_validation, numero
		end

		/*B recherche les actions relatives a la validation de fmc */
		select @vl_TypeHeure = XZAEC_ACT_ECHEC
		insert #ACTIONS ( numero, sit, sit_origine, type, heure, type_heure, 
					equipement)
		select
			numero,
			sit,
			sit_origine,
			type,
			heure_lancement,
			@vl_TypeHeure,
			equipement
		from EXP..ACT_GEN
		where	evenement = @vl_NumEvt and cle = @vl_CleEvt and
			type = XDC_ACT_TFM and
			heure_succes is null and
			heure_echec is null and
			heure_lancement >= @vl_HoroValPrec and
			(@vl_SqlStatusValidation = 2 or 
			 heure_lancement < @vl_HoroValidation)
			order by heure_lancement, numero

		select @vl_TypeHeure = XZAEC_ACT_SUC
		insert #ACTIONS ( numero, sit, sit_origine, type, heure, type_heure, 
					equipement)
		select
			numero,
			sit,
			sit_origine,
			type,
			heure_succes,
			@vl_TypeHeure,
			equipement
		from EXP..ACT_GEN
		where	evenement = @vl_NumEvt and cle = @vl_CleEvt and
			heure_succes >= @vl_HoroValPrec and
			(@vl_SqlStatusValidation = 2 or 
			 heure_succes < @vl_HoroValidation)
			order by heure_succes, numero

		select @vl_TypeHeure = XZAEC_ACT_ECHEC
		insert #ACTIONS ( numero, sit, sit_origine, type, heure, type_heure, 
					equipement)
		select
			numero,
			sit,
			sit_origine,
			type,
			heure_echec,
			@vl_TypeHeure,
			equipement
		from EXP..ACT_GEN
		where	evenement = @vl_NumEvt and cle = @vl_CleEvt and
			heure_echec >= @vl_HoroValPrec and
			(@vl_SqlStatusValidation = 2 or 
			 heure_echec < @vl_HoroValidation)
			order by heure_echec, numero

		select @vl_TypeHeure = XZAEC_ACT_FIN
		insert #ACTIONS ( numero, sit, sit_origine, type, heure, type_heure, 
					equipement)
		select
			numero,
			sit,
			sit_origine,
			type,
			heure_fin,
			@vl_TypeHeure,
			equipement
		from EXP..ACT_GEN
		where	evenement = @vl_NumEvt and cle = @vl_CleEvt and
			type not in ( XDC_ACT_TFM,
					XDC_ACT_TEL,
					XDC_ACT_BIP,
					XDC_ACT_FAX,
					XDC_ACT_CCH ) and
			heure_fin >= @vl_HoroValPrec and
			(@vl_SqlStatusValidation = 2 or 
			 heure_fin < @vl_HoroValidation)
			order by heure_fin, numero


		/*B Ajout les actions de la validation de la FMC */
		if (select count(*) from #ACTIONS) > 0
		begin
			open Pointeur_action

			fetch Pointeur_action into @vl_NumAction, @vl_Site, @vl_SiteOrigine,
					@vl_TypeAction, @vl_Heure, @vl_TypeHeure,
					@vl_Eqt
			while (@@sqlstatus = 0)
			begin
				/* recherche les informations relatives a une action */
				/* trace pour DIAGNOSTIC (a conserver en operationnel) */
				print "Action %1!/%2! a %3! : etat %4!", @vl_NumAction, @vl_Site, @vl_Heure, @vl_TypeHeure
				exec @vl_Status = XZAE206 @va_NumEvt_in,@va_CleEvt_in, @vl_TypeFmc,
						@vl_NumAction, @vl_Site, @vl_SiteOrigine,
						@vl_TypeAction, @vl_Heure, @vl_TypeHeure, @vl_Eqt,
						@vl_Autoroute_Prec, @vl_PR_Prec, @vl_Sens_Prec, @va_NumPoste_in

				if @vl_Status != XDC_OK
					return XZAEC_FMC_ERR_ACTION

				/* action suivante */
				fetch Pointeur_action into @vl_NumAction, 
					@vl_Site, @vl_SiteOrigine, @vl_TypeAction, 
					@vl_Heure, @vl_TypeHeure, @vl_Eqt
			end

			/* effacer le contenu */
			delete #ACTIONS

			close Pointeur_action
		end

		/*B Ajout les commentaires de la validation de la FMC */
		open Pointeur_comment
		fetch Pointeur_comment into @vl_HoroComm, 
				@vl_HoroCommFin, @vl_Texte, 
				@vl_NumeroTexte
		while (@@sqlstatus = 0)
		begin
			/* mettre la date lors du commentaire */
			if @vl_HoroCommFin = null
				/* Hilmarcher ajout de la date + NumEvt + NumCle + " - COMMENTAIRE" */
			begin
				select @vl_Ligne = convert(char(10), @vl_HoroComm, 103) + ' ' +
					convert(char(8), @vl_HoroComm, 8) + ' ' +
					'N' + convert(char(6),@va_NumEvt_in) + ' - ' +
					convert(char(2), @va_CleEvt_in) + ' ' +  convert(varchar(60),@vl_TypeFmc) + 
					' - COMMENTAIRE'
				insert  EXP..FMC_LIS (numero,cle,ligne, heure,poste)
				values( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, @vl_HoroComm,@va_NumPoste_in)

				select @vl_Ligne = convert(char(10), @vl_HoroComm, 103) + ' ' +
					convert(char(8), @vl_HoroComm, 8) + ' ' +
					XZAEC_SYN_COM  
			end
			else
				select @vl_Ligne = convert(char(10), @vl_HoroComm, 103) + ' ' +
					convert(char(8), @vl_HoroComm, 8) + ' ' +
					XZAEC_SYN_COM_VAL +
					convert(char(10), @vl_HoroCommFin, 103) + ' ' +
					convert(char(8), @vl_HoroCommFin, 8)

			/* inserer l'entete du commentaire */
			insert  EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, @vl_HoroComm,@va_NumPoste_in)
			
			/* On coupe la ligne tous les "|" */
			select @vl_Texte_ligne=@vl_Texte
			select @vl_index=patindex("%|%",@vl_Texte_ligne)
			while ( @vl_index!=0)
			begin
				select @vl_Texte = substring(@vl_Texte_ligne,1,@vl_index-1)
				select @vl_Texte_ligne = substring(@vl_Texte_ligne,@vl_index+1,char_length(@vl_Texte_ligne))
				
				/* inserer les morceaux du commentaire en */
				/* tronconnant par texte de 60 caracteres */
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
						substring(@vl_Texte, 1, 60),
					@vl_Longueur = char_length(@vl_Texte)
				insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
				 values( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

				if @vl_Longueur > 60
				begin
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						substring(@vl_Texte, 61, 60)
					insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
					 values( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

					if @vl_Longueur > 120
					begin
						select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							substring(@vl_Texte, 121, 60)
						insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
						values( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
					end
					if @vl_Longueur > 180
					begin
						select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							substring(@vl_Texte, 181, 60)
						insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
						values( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
					end
					if @vl_Longueur > 240
					begin
						select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							substring(@vl_Texte, 241, 10)
						insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
						values( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
					end
				end
				
				select @vl_index=patindex("%|%",@vl_Texte_ligne)
			end
			select @vl_Texte = @vl_Texte_ligne
			
			/* inserer les morceaux du commentaire en */
			/* tronconnant par texte de 60 caracteres */
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					substring(@vl_Texte, 1, 60),
				@vl_Longueur = char_length(@vl_Texte)
			insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
			values( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

			if @vl_Longueur > 60
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					substring(@vl_Texte, 61, 60)
				insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
				values( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)

				if @vl_Longueur > 120
				begin
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						substring(@vl_Texte, 121, 60)
					insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
					values( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
				end
				if @vl_Longueur > 180
				begin
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						substring(@vl_Texte, 181, 60)
					insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
					values( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
				end
				if @vl_Longueur > 240
				begin
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						substring(@vl_Texte, 241, 10)
					insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
					values( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, null,@va_NumPoste_in)
				end
			end

			/* commentaire suivant */
			fetch Pointeur_comment into @vl_HoroComm, 
					@vl_HoroCommFin, @vl_Texte,
					@vl_NumeroTexte
		end

		close Pointeur_comment

		/* effacer le contenu */
		delete #COMMENTAIRES

		/* inserer les consequences dans la synthese */
		insert EXP..FMC_LIS (numero, cle, ligne, heure,
					VR, VM2, VM1, VL, BAU, VR_I, VM2_I, VM1_I, VL_I, BAU_I,
					BAU_etroite, BAU_I_etroite, picto, poste)
		select * from #SYNTHESE_CONSEQ
		delete #SYNTHESE_CONSEQ

		/* repositionner l'evenement a la valeur reelle de la validation */
		if @vl_NumEvtQueue is not null
		begin
			select @vl_NumEvt = @vl_NumEvtUtilise, @vl_CleEvt = @vl_CleEvtUtilise
		end
	end

	/* fermeture du curseur situe sur l'historique de validation */
	close Pointeur_histo

	/* fin de la synthese de l'evenement */
	if @vl_Fin is not null
	begin
		select @vl_Fin = max(horodate_validation) from EXP..FMC_HIS where 
					EXP..FMC_HIS.numero = @va_NumEvt_in and EXP..FMC_HIS.cle = @va_CleEvt_in
		
		select @vl_Ligne = convert(char(10), @vl_Fin, 103) +
					' ' + convert(char(8), @vl_Fin, 8) + ' ' +
					XZAEC_SYN_FINIE + ' N' + convert(char(6),@va_NumEvt_in) +
					' - ' + convert(char(2), @va_CleEvt_in)  
		insert EXP..FMC_LIS (numero,cle,ligne, heure,poste)
		values( @va_NumEvt_in, @va_CleEvt_in,@vl_Ligne, @vl_Fin,@va_NumPoste_in)
	end

	/* Si dernier appel de la proc alors on retourne la synthese des synthese */
	if (@va_TypeAppel_in = XDC_FCT_APL_DERNIER) or (@va_TypeAppel_in = XDC_FCT_APL_UNIQUE)
	begin
		select @vl_horo = null
		select @vl_compteur = 0	

		declare Pointeur_horodate cursor
		for select numero,cle, ligne, heure, 
				VR, VM2, VM1, VL, BAU, VR_I, VM2_I, VM1_I, VL_I, 
				BAU_I, BAU_etroite, BAU_I_etroite, picto, poste
		from EXP..FMC_LIS
		where poste = @va_NumPoste_in		

		open Pointeur_horodate
		fetch Pointeur_horodate into @vl_numero, @vl_cle,@vl_ligne , @vl_heure,
						@vl_VR, @vl_VM2, @vl_VM1, @vl_VL, @vl_BAU,
						@vl_VR_I, @vl_VM2_I, @vl_VM1_I, @vl_VL_I,
						@vl_BAU_I, @vl_BAU_etroite, @vl_BAU_I_etroite, @vl_picto, @vl_poste
		while @@sqlstatus = 0
		begin
			select @vl_compteur = @vl_compteur + 1
			if @vl_heure != null 
				select @vl_horo = @vl_heure
			begin
				select @vl_debut_ligne = substring(@vl_ligne,1,1)
				if @vl_debut_ligne <> "." and  @vl_debut_ligne <> "C"
				select @vl_horo = convert (datetime,substring(@vl_ligne,1,19))
			end	

			insert #SYNTH values(@vl_numero, @vl_cle, @vl_ligne, @vl_horo,
						isnull(@vl_VR,XDC_VOIE_INCONNUE), 
						isnull(@vl_VM2,XDC_VOIE_INCONNUE),
						isnull(@vl_VM1, XDC_VOIE_INCONNUE),
						isnull(@vl_VL, XDC_VOIE_INCONNUE),
						isnull(@vl_BAU, XDC_VOIE_INCONNUE),
						isnull(@vl_VR_I, XDC_VOIE_INCONNUE),
						isnull(@vl_VM2_I,XDC_VOIE_INCONNUE),
						isnull(@vl_VM1_I, XDC_VOIE_INCONNUE),
						isnull(@vl_VL_I, XDC_VOIE_INCONNUE),
						isnull(@vl_BAU_I,  XDC_VOIE_INCONNUE),
						isnull(@vl_BAU_etroite, XDC_FAUX),
						isnull(@vl_BAU_I_etroite, XDC_FAUX),
						@vl_picto, @vl_poste, @vl_compteur)

			fetch Pointeur_horodate into @vl_numero, @vl_cle, @vl_ligne, @vl_heure,
							@vl_VR, @vl_VM2, @vl_VM1, @vl_VL, @vl_BAU,
							@vl_VR_I, @vl_VM2_I, @vl_VM1_I, @vl_VL_I,
							@vl_BAU_I, @vl_BAU_etroite, @vl_BAU_I_etroite, @vl_picto, @vl_poste
		end	

		close Pointeur_horodate
		
		select
			ligne,
			heure,
			isnull(VR, XDC_VOIE_INCONNUE),
			isnull(VM2, XDC_VOIE_INCONNUE),
			isnull(VM1, XDC_VOIE_INCONNUE),
			isnull(VL, XDC_VOIE_INCONNUE),
			isnull(BAU, XDC_VOIE_INCONNUE),
			isnull(VR_I, XDC_VOIE_INCONNUE),
			isnull(VM2_I, XDC_VOIE_INCONNUE),
			isnull(VM1_I, XDC_VOIE_INCONNUE),
			isnull(VL_I, XDC_VOIE_INCONNUE),
			isnull(BAU_I, XDC_VOIE_INCONNUE),
			isnull(BAU_etroite, XDC_FAUX),
			isnull(BAU_I_etroite, XDC_FAUX),
			picto,
			poste
		from #SYNTH
		order by heure asc, compteur asc

		delete EXP..FMC_LIS
		where poste = @va_NumPoste_in
	end

	return XDC_OK
#endif

go	
