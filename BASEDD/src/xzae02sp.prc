/*E*/
/* Fichier : $Id: xzae02sp.prc,v 1.94 2019/10/01 19:30:40 devgfi Exp $        $Revision: 1.94 $        $Date: 2019/10/01 19:30:40 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae02sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* retourne la synthese d'un evt ou operation
*
------------------------------------------------------
* HISTORIQUE :
*
* B.GABORIT	28/09/94	: Creation d'un bouchon
* C.T.     	10/10/94	: Creation 	V 1.1
* C.T.     	10/11/94	: Creation 	V 1.2
* C.T.		14/11/94	: Recherche des infos ds HIS au CI et
*              	           	  ds EXP aux districts	 ( V 1.3)
* C.T.		14/02/95	: Remplacer le code district par
*                                 le numero et modifier le format
*                                 des dates ( V 1.4)
* C.T.		15/02/95	: Erreur compile ( V 1.6)
* C.T.		15/02/95	: Erreur compile ( V 1.7)
* C.T.		08/03/95	: Correction du type bouchon 
*                                 pour affichage ( V 1.8)
* C.T.		10/03/95	: Report de la correction 
*                                 du type bouchon dans le district
*                                 ( V 1.9)
* C.T.     	17/02/95	: Modif longueur ligne 1.10 
* C.T.     	07/04/95	: Retourner la config des voies 
*                                 pour chaque validation (1.11)
* C.T.     	26/04/95	: Correction erreur cursor pointeur_histo
*                                 pour district (1.12)
* C.T.     	05/06/95	: Correction erreur actions pour bouchon (1.13)
* C.T.     	21/07/95	: Correction travaux mobiles (1.14)
*                                 Ajout des consequences
*                                 Suppression des fins d'action pour appel 
*                                 tel bip fax tfm
* C.T.     	08/08/95	: Correction de compile (1.15)
* C.T.     	05/09/95	: Correction  sens des bouchons (1.17)
* C.T.     	06/09/95	: Correction  erreur de compile (1.18)
* C.T.     	07/09/95	: Correction  consequences (1.19)
* C.T.     	18/09/95	: Correction  fmc fermeture echangeur au CI (1.20)
*                                 et commentaire avant premiere validation
* C.T.+BG	02/10/95	: Delete de LISTE_CONSEQ avant calcul (1.21)
* BG		02/10/95        : Correction bug compil en district (1.22)
* C.T.     	03/10/95	: Modif interface d'appel a xzae82 pour inverser les sens sur A57 (1.23)
*                                 ligne plus longue de 8 characteres
* C.T.     	10/10/95	: Correction nom type fmc des consequences (1.24)
* C.T.     	18/10/95	: Correction commentaires sur bouchon (1.25)
* C.T.     	17/11/95	: Correction commentaires, modif libelle validation fmc, 
*                                 position consequences et commentaires (1.26)
* C.T.     	05/03/96	: Correction appel XZAE52 (inversion parametres de sortie) (1044) (1.27)
* C.T.     	25/09/96	: Ajout degats domaine (FMC10) (1.30)
* C.T.     	02/10/96	: Modif intitule des validations avec ajout type precedent 
*                                 si changement (FMC3) champs hauteur, larg., poids, contenu 
*                                 pour convoi except. ou mil (FMC6) (1.31)
* C.T.     	15/10/96	: Correction hauteur vehicule lent (FMC6)(1.32)
* P.N.		29/10/96	: Correction dans district type_anterieur peut etre null (1.33-34)
* C.T.     	07/11/96	: Ajout en tete fausse alerte si la fmc est une fausse alerte (FMC18)(1.35)
* C.T.     	25/11/96	: Ajout bau etroite en sortie (SAD4) (1283)(1.36)
* JMG		27/12/96	: tableau des neutralisations de voies IHM5 (DEM/1372) (DEM/1394) 1.37
* JMG		22/05/97	: correction erreur compil au district		1.38
* C.T.		08/01/98	: modif recherche de l'alerte avec sit_alerte (1532) 1.39
* C.T.		29/01/98	: Ajout fmc initiale (1531) 1.40
* P.N.		06/02/98	: Correction pour fonctionnement sur ancien travaux (1552) 1.41
* C.T.		23/02/98	: Modif default suite a passage en version SQL 11.0.3.2 1.42
* JMG		03/03/98	: ajout sit_origine dans ACT_GEN (dem/1605) 1.43-1.44
* C.P.		10/06/98	: Separation EXP et HIS (V 1.45)
* P.N.		12/01/99	: Modif intergestionnaire (V 1.46)
* JMG		09/02/00	: libelles plus court pour les neutralisations de voies (1.47)
* JMG		27/04/00	: traitement cas alerte plus dans la base (SAE 89) 1.48
* JMG		01/09/00	: traitement erreur PR 1e validation (SAE 130) 1.50
* PCL		18/03/03	: nouveaux type de travaux 1.51
* Hilmarcher    02/01/04        : modif pour plus de troncature des actions venant de EXP (DEM 313, FI 37) v1.52
				 + corections divers bugs
* P.N.		28/02/05	: Ajout du bouchon "En accordeon" v1.53 DEM428
* CHI		13/04/05	: Rajout sous type "grele sur chaussee" et "chaussee mouillee" v1.54 DEM456
* CHI           19/07/05        : Rajout classe nature DEM487 v1.55
* JPL		18/04/07	: Ajout d'un argument aux appels XZAE80 et XZAE83 (DEM 638) 1.56
* JPL		15/05/07	: Prise en compte FMCs Traitement chaussees et Chaussee glissante (DEM 644,646) 1.57
* JPL		15/05/07	: Prise en compte FMC Echangeur deconseille (DEM 645) 1.58
* JPL		05/06/07	: Commentaires 1.59
* JPL		07/06/07	: Pas de visibilite pour Traitement Chaussees et Chaussee glissante (DEM 644,646) 1.60
* JPL		12/06/07	: Prise en compte Fmc Basculement pour Travaux (DEM 647) 1.61
* JPL		10/07/07	:  ... et dans un district aussi ... (DEM 647) 1.62
* JPL		10/07/07	: Modification des libelles de config des voies pour Basculement pour Travaux 1.63
* JMG		18/09/08	: ajout horodate validation dans FMC_ACC DEM/852 1.64
* JPL		18/05/09	: Sur HIS, recherche des commentaires saisis apres la date de fin (DEM 883) 1.65
* JPL		18/05/09	: Differences EXP/HIS: vitesse et contenu vehicules lents, gravite Datex ...) 1.66
* JPL		19/05/09	: Retour de codes d'erreur differents selon le cas d'erreur 1.67
* JPL		10/07/09	: Trace des actions avant leur ajout par xzae82 1.68
* PNI		05/07/10	: Ajout du renvoi en arg output de la date de cloture de l'evt DEM940 1.69
* JPL		02/09/10	: Prise en compte Fmc Zone de stockage dans la classe Travaux (DEM 942) 1.70
* JPL		09/02/11	: Prise en compte du type de travaux "Tunnels" (DEM 951)  1.71
* JMG		25/11/10	: ajout hotodate maitrise FMC_VEH 1.72
* LCL		20/07/11	: ajout champs rappel client 1.74
* JPL		22/09/11	: Localisation des tetes de bouchons sur points caracteristiques (DEM 994)  1.75
* JPL		22/09/11	: Utilisation de xzae83 a la place de xzae80, obsolete  1.76
* LCL		02/12/11	: Ajout vitesse et vitesse opposee (DEM 1015)  1.77
* LCL		02/12/11	: correction vitesse et vitesse opposee (DEM 1015)  1.78
* JPL		08/02/12	: Indiquer le type initial de l'evenement a la creation  1.79
* VR		08/03/12	: Ajout de la FMC Regulation dans la synthese (DEM/1016)  1.81
* JPL		07/09/12	: Mention 'Incendie maitrise' avec la date pour Incendie de vegetation (DEM 1037)  1.82
* JPL		21/11/12	: Gestion des actions Operateur d'envoi des conditions de conduite (DEM 1032)  1.83
* JPL		06/12/12	: Classe Travaux : vitesse sur HIS comme sur secteurs  1.84
* JPL		07/12/12	: Traitement cas d'absence de l'alerte sur secteurs  1.85
* JPL		07/12/12	: Nommage de la base des donnees d'exploitation  1.86
* JPL		07/12/12	: Reunification EXP et HIS  1.87
* JPL		10/12/12	: Fmc Meteo: appel xzae81 a la creation pour complement avec conditions de conduite  1.88
* JMG		26/11/16	: ajout majeur lot 23 1.89 1.90
* JMG		08/03/18	: ajout personnel FMC travaux 1.91
* JPL		04/04/19	: Mention de la presence d'ecoulement pour les Fmc Accident et Vehicules (DEM 1326)  1.92
* JPL		11/09/19	: FMC Incendie generalisee : mention de la nature de l'incendie (DEM 1352)  1.93
* JPL		01/10/19	: Basculement pour travaux : indiquer la presence d'un bouchon mobile (DEM 1346)  1.94
* ABE		18/11/21	: Ajout colonne categories des actions pour les filtres de la synthese Magistra DEM-SAE311 1.95
*ABE		25/08/22	: Ajout localisation tunnel niche / interniche et Issuer de secour DEM-SAE403
* PNI		10/11/2022	: Correction répétition du type de véhicule dans la synthèse FMC AVA DEM-SAE-433
* LCL		03/05/23	: Correction problemes action pas presente a cause des milliemes de secondes dans l'horodate de validation - COCKPIT
* LCL		17/05/23	: Adaptation suite correction precedente SAE-482
* GGY		30/08/23	: Passage du picto a 10 caracteres (DEM-473)
* GGY		22/02/24	: Ajout de la localisation en batiment (DEM-473)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne la synthese d'une fiche main courante 
* sous la forme d'une liste de lignes
* 
* Sequence d'appel
* SP	XZAE02_Synthese_FMC
* 
* Arguments en entree
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
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

if exists (select 1 from sysobjects where name = 'XZAE02' and type = 'P')
	drop procedure XZAE02
go


#ifdef HIST
	#define BDD_XPL HIS
#else
	#define BDD_XPL EXP
#endif



create procedure XZAE02
	@va_NumEvt_in	int	= null,
	@va_CleEvt_in	tinyint	= null,
	@va_Etat_out	tinyint	= null output,	-- XDC_FMC_ETAT_xxx
	@va_Cloture_out datetime = null output

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
		@vl_NumEvt int, @vl_CleEvt tinyint, @vl_OrdreSuivant tinyint,
		@vl_nb int, @vl_DateRecherche datetime, @vl_Date datetime,
		@vl_Eqt T_EQUIPEMENT, @vl_IndicateurCreation bit, @vl_AutoroutePourAction T_AUTOROUTE,
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
		@vl_type_rappel tinyint, @vl_complement_rappel varchar(20), @vl_horodate_rappel datetime,
		@vl_index tinyint, @vl_Longueur int,
		@vl_Intitule char(60), @vl_Localisation char(60),
		@vl_Intitule2 char(60), @vl_Localisation2 char(60),
		@vl_Texte varchar(250), @vl_Texte_ligne varchar(250),
		@vl_InitLigne char(20), @vl_Ligne char(100),
		@vl_Libelle varchar(40),@vl_vitesse_opposee tinyint,
		@vl_vitesse tinyint, @vl_scenario smallint, @vl_zone smallint,
		@vl_contexte smallint, @vl_transitoire char(200),
		@vl_mesure smallint, @vl_indicateur_horaire smallint,
		@vl_horodate datetime, @vl_majeur tinyint,
		@vl_personnel tinyint, @vl_AutorouteTete T_AUTOROUTE, @vl_ecoulement tinyint,
		@vl_code_nature tinyint, @vl_nature_inc varchar(100), @vl_PresenceBouchon tinyint,
		@vl_status_courant bit, @vl_categorie_courante tinyint,
		@vl_libelle_localisation_tunnel char(30), @vl_libelle_localisation_tunnel_tet char(30),
		@vl_libelle_localisation_tunnel_tmp char(30)

	/*! transfo en parametres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null

	/*A Controle des parametres d'entree */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV

	 /*A reinitialisation de la table LISTE_CONSEQ */
	 delete LISTE_CONSEQ
	 where spid=@@spid

	/*A Verifier l'existence de la fiche main courante */
	select
		@vl_Datex	= BDD_XPL..FMC_GEN.datex,
		@vl_Cloture	= BDD_XPL..FMC_GEN.cloture,
		@vl_Alerte	= BDD_XPL..FMC_GEN.alerte,
		@vl_SiteAlerte	= BDD_XPL..FMC_GEN.sit_alerte,
		@vl_TypeFmc	= CFG..TYP_FMC.nom,
		@vl_NumTypeFmc	= BDD_XPL..FMC_GEN.type,
		@vl_OrigCreation = BDD_XPL..FMC_GEN.origine_creation,
		@vl_Debut	= BDD_XPL..FMC_GEN.debut,
		@vl_DebutPrevu	= BDD_XPL..FMC_GEN.debut_prevu,
		@vl_Fin		= BDD_XPL..FMC_GEN.fin,
		@vl_FinPrevu	= BDD_XPL..FMC_GEN.fin_prevue,
		@vl_Cause	= BDD_XPL..FMC_GEN.cause,
		@vl_CleCause	= BDD_XPL..FMC_GEN.cle_cause,
		@vl_HoroCause	= BDD_XPL..FMC_GEN.horodate_lien_cause,
		@vl_FausseAlerte 	= fausse_alerte,
		@vl_NumFmcInit 	= num_fmc_init,
		@vl_CleFmcInit 	= cle_fmc_init,
		@vl_Degats 	= degats_domaine
	from BDD_XPL..FMC_GEN, CFG..TYP_FMC
	where	BDD_XPL..FMC_GEN.numero = @va_NumEvt_in and 
		BDD_XPL..FMC_GEN.cle = @va_CleEvt_in and
		CFG..TYP_FMC.numero = BDD_XPL..FMC_GEN.type

	if @@rowcount != 1
		return XDC_NOK


	/*A Recherche l'etat de la fiche main courante */
	select @va_Etat_out = XDC_FMC_ETAT_CLOS

/* ***************** dans HIST on decide que la fmc est close **********************/
#ifndef HIST
	if @vl_Cloture = null
	begin
		if exists ( select * from BDD_XPL..FMC_TRT
		             where evenement = @va_NumEvt_in and cle = @va_CleEvt_in)
			select @va_Etat_out = XDC_FMC_ETAT_TRAI
		else	select @va_Etat_out = XDC_FMC_ETAT_PREV
	end
#endif


	/* positionner var internes */
	select @vl_NumEvtQueue = null, @vl_CleEvtQueue = null

	/*A Creation du tableau de stockage de la synthese */
	create table #SYNTHESE
		(
		ligne	char(120) not null,
		heure	datetime null,
		VR	tinyint	null ,			-- default	XDC_VOIE_INCONNUE,
		VM2	tinyint	null ,			-- default	XDC_VOIE_INCONNUE,
		VM1	tinyint	null ,			-- default	XDC_VOIE_INCONNUE,
		VL	tinyint	null ,			-- default	XDC_VOIE_INCONNUE,
		BAU	tinyint	null ,			-- default	XDC_VOIE_INCONNUE,
		VR_I	tinyint	null ,			-- default	XDC_VOIE_INCONNUE,
		VM2_I	tinyint	null ,			-- default	XDC_VOIE_INCONNUE,
		VM1_I	tinyint	null ,			-- default	XDC_VOIE_INCONNUE,
		VL_I	tinyint	null ,			-- default	XDC_VOIE_INCONNUE,
		BAU_I	tinyint	null ,			-- default	XDC_VOIE_INCONNUE,
		BAU_etroite	tinyint	null ,			-- default	XDC_FAUX,
		BAU_I_etroite	tinyint	null ,			-- default	XDC_FAUX,
		picto	char(XDC_LGR_NOM_PICTO) null,
		categorie tinyint null
		)
	create table #SYNTHESE_CONSEQ
		(
		ligne	char(120) not null,
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
		picto	char(XDC_LGR_NOM_PICTO) null,
		categorie tinyint null
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
    		sit                   tinyint        		null	,     
			email				  varchar(255)			null
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
		critere_ordre		tinyint default 0,
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
		majeur tinyint default XDC_FAUX,
		ecoulement		tinyint default XDC_FAUX
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
		critere_ordre		tinyint default 0,
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
		majeur tinyint default XDC_FAUX,
		ecoulement		tinyint default XDC_FAUX
		)

	if @vl_FausseAlerte = XDC_VRAI
	begin
		/* fausse alerte */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_FAUSSE_ALERTE
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne,  null, XDC_CAT_ACT_MODIFICATION)

		/* ligne vide */
		insert #SYNTHESE (ligne, heure, categorie)
		values(XZAEC_SYN_PAS_DATE,  null, XDC_CAT_ACT_MODIFICATION)
	end

	if @vl_Datex = XDC_VRAI
	begin
		/* fausse alerte */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ORIGINE_DATEX
		insert #SYNTHESE (ligne, heure, categorie)
		values(@vl_Ligne,  null, XDC_CAT_ACT_MODIFICATION)

		/* ligne vide */
		insert #SYNTHESE (ligne, heure, categorie)
		values(XZAEC_SYN_PAS_DATE,  null, XDC_CAT_ACT_MODIFICATION)
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

--	    select dans ALT_LOC pour voir si on a une ligne pour @vl_alerte et @vl_SiteAlerte
        if exists (select * from BDD_XPL..ALT_LOC
        where BDD_XPL..ALT_LOC.numero = @vl_Alerte and
              BDD_XPL..ALT_LOC.sit = @vl_SiteAlerte)

        begin
            /*B Creation de l'intitule de l'alerte avec localisation de ALT_LOC */
            select
                @vl_Ligne = convert(char(10),BDD_XPL..ALT_EXP.horodate, 103) +
                ' ' + convert(char(8), BDD_XPL..ALT_EXP.horodate, 8) +
                ' ' + XZAEC_SYN_ALT + CFG..TYP_ALT.nom +
                ' ' + CFG..EQT_GEN.nom ,
                @vl_Texte = BDD_XPL..ALT_EXP.texte,
                @vl_Autoroute = BDD_XPL..ALT_LOC.autoroute,
                @vl_PR = BDD_XPL..ALT_LOC.PR,
                @vl_Sens = BDD_XPL..ALT_LOC.sens
            from BDD_XPL..ALT_EXP, CFG..TYP_ALT, CFG..EQT_GEN, BDD_XPL..ALT_LOC
            where	BDD_XPL..ALT_EXP.numero = @vl_Alerte and
                BDD_XPL..ALT_EXP.sit = @vl_SiteAlerte and
                CFG..TYP_ALT.numero = BDD_XPL..ALT_EXP.type and
                BDD_XPL..ALT_LOC.numero = @vl_Alerte and
                BDD_XPL..ALT_LOC.sit = @vl_SiteAlerte and
                CFG..EQT_GEN.numero = BDD_XPL..ALT_EXP.equipement and
                CFG..EQT_GEN.type = BDD_XPL..ALT_EXP.type_equipement
        end

        else
        begin
            /*B Creation de l'intitule de l'alerte avec localisation de EQT_GEN */
            select
                @vl_Ligne = convert(char(10),BDD_XPL..ALT_EXP.horodate, 103) +
                ' ' + convert(char(8), BDD_XPL..ALT_EXP.horodate, 8) +
                ' ' + XZAEC_SYN_ALT + CFG..TYP_ALT.nom +
                ' ' + CFG..EQT_GEN.nom ,
                @vl_Texte = BDD_XPL..ALT_EXP.texte,
                @vl_Autoroute = CFG..EQT_GEN.autoroute,
                @vl_PR = CFG..EQT_GEN.PR,
                @vl_Sens = CFG..EQT_GEN.sens
            from BDD_XPL..ALT_EXP, CFG..TYP_ALT, CFG..EQT_GEN
            where	BDD_XPL..ALT_EXP.numero = @vl_Alerte and
                BDD_XPL..ALT_EXP.sit = @vl_SiteAlerte and
                CFG..TYP_ALT.numero = BDD_XPL..ALT_EXP.type and
                CFG..EQT_GEN.numero = BDD_XPL..ALT_EXP.equipement and
                CFG..EQT_GEN.type = BDD_XPL..ALT_EXP.type_equipement
        end

		if @@rowcount = 1
		begin
			/*B Ajout de l'intitule de l'alerte dans la synthese */
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne,  null, XDC_CAT_ACT_MODIFICATION)

			/* mise en forme de la localisation de l'alerte */
			exec XZAE83 null, null, null, null, @vl_Autoroute, @vl_PR, @vl_Sens,
				@vl_Intitule2 output, @vl_Localisation2 output

			/* recherche si la localisation est dans un tunnel, ajoute le libelle de la niche/IS proche */
			exec PRC..XZAO508 @vl_Autoroute,@vl_PR,@vl_Sens,@vl_libelle_localisation_tunnel_tmp output

			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ALT_LOC +
				rtrim(@vl_Localisation2) + " " + @vl_libelle_localisation_tunnel_tmp

			/*B Ajout des informations de l'alerte pour la synthese */
			insert #SYNTHESE(ligne, heure, categorie)
			values ( @vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)

			/*B Ajout des commentaires de l'alerte pour la synthese */
			if @vl_Texte != XDC_CHAINE_VIDE
			begin
				/* tronconner le texte en deux morceaux */
				select	@vl_Longueur = char_length(@vl_Texte),
					@vl_Ligne = XZAEC_SYN_PAS_DATE + 
						XZAEC_SYN_ALT_COM +
						substring(@vl_Texte, 1, 65)
	
				/* insertion de la premiere ligne de commentaire */
				insert #SYNTHESE(ligne, heure, categorie)
				values ( @vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
	
				if @vl_Longueur > 60
				begin
					select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
						   substring(@vl_Texte, 61, 39)
	
					/* insertion de la deuxieme ligne de commentaire */
					insert #SYNTHESE(ligne, heure, categorie)
					values ( @vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
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
		BAU_etroite, BAU_I_etroite, critere_ordre, majeur, ecoulement)
	select 
		BDD_XPL..FMC_HIS.numero,
		BDD_XPL..FMC_HIS.cle,
		CFG..TYP_FMC.classe,
		BDD_XPL..FMC_HIS.type_FMC_valide,
		CFG..TYP_FMC.nom,
		BDD_XPL..FMC_HIS.type_anterieur,
		BDD_XPL..FMC_HIS.autoroute,
		BDD_XPL..FMC_HIS.PR,
		BDD_XPL..FMC_HIS.sens,
		BDD_XPL..FMC_HIS.confirme_ou_signale,
		BDD_XPL..FMC_HIS.horodate_validation,
		BDD_XPL..FMC_HIS.point_caracteristique,
		BDD_XPL..FMC_HIS.numero_point_caracteristique,
		isnull(BDD_XPL..FMC_HIS.VR, XDC_VOIE_INCONNUE),
		isnull(BDD_XPL..FMC_HIS.VM2, XDC_VOIE_INCONNUE),
		isnull(BDD_XPL..FMC_HIS.VM1, XDC_VOIE_INCONNUE),
		isnull(BDD_XPL..FMC_HIS.VL, XDC_VOIE_INCONNUE),
		isnull(BDD_XPL..FMC_HIS.BAU, XDC_VOIE_INCONNUE),
		isnull(BDD_XPL..FMC_HIS.VR_I, XDC_VOIE_INCONNUE),
		isnull(BDD_XPL..FMC_HIS.VM2_I, XDC_VOIE_INCONNUE),
		isnull(BDD_XPL..FMC_HIS.VM1_I, XDC_VOIE_INCONNUE),
		isnull(BDD_XPL..FMC_HIS.VL_I, XDC_VOIE_INCONNUE),
		isnull(BDD_XPL..FMC_HIS.BAU_I, XDC_VOIE_INCONNUE),
		isnull(BDD_XPL..FMC_HIS.BAU_etroite, XDC_FAUX),
		isnull(BDD_XPL..FMC_HIS.BAU_I_etroite, XDC_FAUX),
		0,
		isnull(BDD_XPL..FMC_HIS.majeur,XDC_FAUX),
		isnull(BDD_XPL..FMC_HIS.ecoulement, XDC_FAUX)
	from BDD_XPL..FMC_HIS, CFG..TYP_FMC
	where	BDD_XPL..FMC_HIS.numero = @va_NumEvt_in and 
		BDD_XPL..FMC_HIS.cle = @va_CleEvt_in and
		CFG..TYP_FMC.numero = BDD_XPL..FMC_HIS.type_FMC_valide
		order by BDD_XPL..FMC_HIS.horodate_validation

	/* l'evenement n'a jamais ete valide : pas de synthese */
	if @@rowcount = 0
		return XDC_PAS_SYN


	/*A Recherche la description de la creation de la FMC */
	select 
		@vl_HoroValidation = horodate_validation,
		@vl_NumTypeFmc = type,
		@vl_TypeFmc = nom_type,
		@vl_ClasseFmc = classe,
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
		@vl_BAU_I_etroite = BAU_I_etroite,
		@vl_majeur = majeur,
		@vl_ecoulement = ecoulement
	from #HISTORIQUE_FMC
	having horodate_validation = min(horodate_validation)

	/* recherche si la localisation est dans un tunnel, ajoute le libelle de la niche/IS proche */
	exec PRC..XZAO508 @vl_Autoroute,@vl_PR,@vl_Sens,@vl_libelle_localisation_tunnel output

	/* set du status à la création pour choix catégorie MODIFICATION ou ETAPE pour les blocs suivant */
	select @vl_status_courant = @vl_ConfirmeSignale
	select @vl_categorie_courante = XDC_CAT_ACT_MODIFICATION

	/* mise a jour de l'autoroute pour la localisation des actions */
	select @vl_AutoroutePourAction = @vl_Autoroute

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

	insert #SYNTHESE(ligne, heure, categorie)
	values ( @vl_Ligne, @vl_HoroValidation, XDC_CAT_ACT_MODIFICATION)

	select @vl_Ligne = XZAEC_SYN_PAS_DATE + rtrim(@vl_Localisation) + " " + @vl_libelle_localisation_tunnel
	insert #SYNTHESE
	values ( @vl_Ligne, null, @vl_VR, @vl_VM2, @vl_VM1,
		@vl_VL, @vl_BAU, @vl_VR_I, @vl_VM2_I, @vl_VM1_I,@vl_VL_I, @vl_BAU_I, 
		@vl_BAU_etroite, @vl_BAU_I_etroite, null, XDC_CAT_ACT_MODIFICATION)

	if @vl_OrigCreation != XDC_CHAINE_VIDE
	begin
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ORI_CRE + 
				 @vl_OrigCreation
		insert #SYNTHESE(ligne, heure, categorie)
		values ( @vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
	end

	if @vl_Debut != null
	begin
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_DEB + 
					convert(char(10), @vl_Debut, 103) + ' ' + 
					convert(char(8), @vl_Debut, 8) 
		insert #SYNTHESE(ligne, heure, categorie)
		values ( @vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
	end

	if @vl_Fin != null
	begin
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_FIN + 
					convert(char(10), @vl_Fin, 103) + ' ' + 
					convert(char(8), @vl_Fin, 8) 
		insert #SYNTHESE(ligne, heure, categorie)
		values ( @vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
	end

	if @vl_DebutPrevu != null
	begin
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_DEB_PREV + 
					convert(char(10), @vl_DebutPrevu, 103) + ' ' + 
					convert(char(8), @vl_DebutPrevu, 8) 
		insert #SYNTHESE(ligne, heure, categorie)
		values ( @vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
	end

	if @vl_FinPrevu != null
	begin
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_FIN_PREV + 
					convert(char(10), @vl_FinPrevu, 103) + ' ' + 
					convert(char(8), @vl_FinPrevu, 8) 
		insert #SYNTHESE(ligne, heure, categorie)
		values ( @vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
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
		else if @vl_PointCaract = XDC_POINT_CARACT_BAT
		begin
			select @vl_Libelle = XZAEC_SYN_VEH_BAT

			select @vl_NomPointCaract = nom
			from CFG..RES_BAT								
			where	numero = @vl_NumPointCaract
		end
		else return XZAEC_FMC_LOC_INC

		if @@rowcount != 1
		return XZAEC_FMC_LOC_INC

		select @vl_Ligne = XZAEC_SYN_PAS_DATE + @vl_Libelle + @vl_NomPointCaract

		insert #SYNTHESE(ligne, heure, categorie)
		values ( @vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)

	end		

	if @vl_ConfirmeSignale = XZAEC_FMC_SIGNALEE
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_SIGNALEE
	else	select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_CONFIRMEE
	insert #SYNTHESE(ligne, heure, categorie)
	values ( @vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)

	if @vl_NumFmcInit is not null
	begin
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
				XZAEC_SYN_FMC_DEGRADE + convert(varchar(10), @vl_NumFmcInit) + ' ' + convert(char(2), @vl_CleFmcInit)
		insert #SYNTHESE(ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
	end

	if @vl_Degats = XDC_OUI
	begin
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
				XZAEC_SYN_DEG_DOM
		insert #SYNTHESE(ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
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
		from BDD_XPL..FMC_ACC
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in and zz > 1
		group by numero,cle
		having validation=min(validation)

		if @vl_NbVehLeg != null and @vl_NbVehLeg != 0
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_VL + 
					convert(varchar(8),@vl_NbVehLeg)
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end

		if @vl_NbPL != null and @vl_NbPL != 0
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_PL + 
					convert(varchar(8),@vl_NbPL)
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end

		if @vl_NbMoto != null and @vl_NbMoto != 0
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_MOTO + 
					convert(varchar(8), @vl_NbMoto)
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end

		if @vl_NbCar != null and @vl_NbCar != 0
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_CAR + 
					convert(varchar(8), @vl_NbCar)
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end

		if @vl_NbRemorques != null and @vl_NbRemorques != 0
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_REMORQ + 
					convert(varchar(8), @vl_NbRemorques)
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end

		if @vl_BlessesLeg != null and @vl_BlessesLeg != 0 
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_BLE_LEG + 
					convert(varchar(8), @vl_BlessesLeg)
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end

		if @vl_BlessesGra != null and @vl_BlessesGra != 0
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_BLE_GRA + 
					convert(varchar(8), @vl_BlessesGra)
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end

		if @vl_Morts != null and @vl_Morts != 0
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_MOR + 
					convert(varchar(8), @vl_Morts)
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end

		if @vl_MatDang = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_PRE_MAT_DNG
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end

		if @vl_VehESCOTA = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_VEH_ESC 
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end

		if @vl_PersESCOTA = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_PER_ESC
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end

		if @vl_Personalite = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_PERS 
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end


		if @vl_ecoulement = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_ECOUL
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end

	end

	
/************************************************************************************************************************/
/**                                               Cas d'une Regulation                                                 **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClasseRegulation
	begin
				select
					@vl_scenario = scenario,
					@vl_contexte = contexte,
					@vl_transitoire = transitoire,
					@vl_zone = zone,
					@vl_mesure = mesure,
					@vl_indicateur_horaire = indicateur_horaire,
					@vl_horodate=horodate
						from BDD_XPL..FMC_REG
						where	numero = @va_NumEvt_in and cle = @va_CleEvt_in
						and zone != null and scenario != -1
						and horodate in (select max(horodate) from BDD_XPL..FMC_REG
								where   numero = @va_NumEvt_in and cle = @va_CleEvt_in)

					select @vl_Intitule = nom
					from CFG..ZON_REG
					where numero = @vl_zone
					select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
							XZAEC_SYN_ZON + ' ' + @vl_Intitule

					insert #SYNTHESE(ligne, heure, categorie)
					values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
	end

/************************************************************************************************************************/
/**                                    Cas d'un evenement de classe Delestage                                          **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClasseDelestage
	begin
		/* rechercher les informations du delestage */
		select @vl_NumPlan = plan_associe
		from BDD_XPL..FMC_DEL
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
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
	
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ECH_SOR +
						+ @vl_NomEchSortie
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
	
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ECH_INJ +
						+ @vl_NomEchReinjection
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
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
			@vl_HorodateMaitrise = horodate_maitrise,
			@vl_type_rappel = type_rappel,
			@vl_complement_rappel = complement_rappel,
			@vl_horodate_rappel = horodate_rappel
		from BDD_XPL..FMC_VEH
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

			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end
		if @vl_FeuMaitrise = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_VEH_FEU_MAITRISE +
					convert(char(10),@vl_HorodateMaitrise,103) + ' ' +
					convert(char(8),@vl_HorodateMaitrise,8)
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end
		if @vl_VehESCOTA = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_VEH_ESC 
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end
		if @vl_CodeMatDang != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_MAT_DNG + @vl_CodeMatDang
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end
		if @vl_ecoulement = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
					XZAEC_SYN_ECOUL
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end
		if @vl_NumTypeFmc = XZAEC_FMC_VehArrete or
		   @vl_NumTypeFmc = XZAEC_FMC_VehPanne or
		   @vl_NumTypeFmc = XZAEC_FMC_VehFeu
		begin
			if @vl_type_rappel = XZAEC_TYP_RAP_NUM_CLIENT
				select @vl_Ligne = XZAEC_TYP_RAP_LIB_GENE + XZAEC_TYP_RAP_LIB_NUM_CLIENT + ' ' +
								   substring(@vl_complement_rappel,1,4) + ' (' +
								   convert(char(10), @vl_horodate_rappel, 103) + ' ' + 
								   convert(char(8), @vl_horodate_rappel, 8)  +')'
			else if @vl_type_rappel = XZAEC_TYP_RAP_AUTRE
				select @vl_Ligne = XZAEC_TYP_RAP_LIB_GENE + XZAEC_TYP_RAP_LIB_AUTRE + ' ' +
								   @vl_complement_rappel + ' (' +
								   convert(char(10), @vl_horodate_rappel, 103) + ' ' + 
								   convert(char(8), @vl_horodate_rappel, 8)  +')'
			else if @vl_type_rappel = XZAEC_TYP_RAP_REFUS_COORD
				select @vl_Ligne = XZAEC_TYP_RAP_LIB_GENE + XZAEC_TYP_RAP_LIB_REFUS_COORD + ' (' +
								   convert(char(10), @vl_horodate_rappel, 103) + ' ' + 
								   convert(char(8), @vl_horodate_rappel, 8)  +')'
			else if @vl_type_rappel = XZAEC_TYP_RAP_INJOIGNABLE
				select @vl_Ligne = XZAEC_TYP_RAP_LIB_GENE + XZAEC_TYP_RAP_LIB_INJOIGNABLE + ' (' +
								   convert(char(10), @vl_horodate_rappel, 103) + ' ' + 
								   convert(char(8), @vl_horodate_rappel, 8)  +')'
			else if @vl_type_rappel = XZAEC_TYP_RAP_NUM_ETRANGER
				select @vl_Ligne = XZAEC_TYP_RAP_LIB_GENE + XZAEC_TYP_RAP_LIB_NUM_ETRANGER + ' (' +
								   convert(char(10), @vl_horodate_rappel, 103) + ' ' + 
								   convert(char(8), @vl_horodate_rappel, 8)  +')'
			else if @vl_type_rappel = XZAEC_TYP_RAP_DEPAN_SEUL
				select @vl_Ligne = XZAEC_TYP_RAP_LIB_GENE + XZAEC_TYP_RAP_LIB_DEPAN_SEUL + ' (' +
								   convert(char(10), @vl_horodate_rappel, 103) + ' ' + 
								   convert(char(8), @vl_horodate_rappel, 8)  +')'
			else
				select @vl_type_rappel=null

			if @vl_type_rappel != null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + @vl_Ligne
				insert #SYNTHESE(ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
			end
		end		   
		if @vl_Commentaire != XDC_CHAINE_VIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_COM + 
					substring(@vl_Commentaire, 1, 48),
				@vl_Longueur = char_length(@vl_Commentaire)
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)

			if @vl_Longueur > 48
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					substring(@vl_Commentaire, 49, 51)
				insert #SYNTHESE(ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
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
		from BDD_XPL..FMC_VLN
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in
		having horodate = min(horodate)

		/* Ajout des informations dans la synthese */
		if @vl_LongVeh != null
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_LON_VEH + 
					convert(varchar(8), @vl_LongVeh)
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end

		if @vl_Vitesse != null
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_VIT_VEH + 
					convert(varchar(8), @vl_Vitesse)
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end

		if @vl_NumTypeFmc = XZAEC_FMC_ConvoiExc or @vl_NumTypeFmc = XZAEC_FMC_ConvoiMil
		begin
			if @vl_Hauteur != null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_HAU_VEH + 
						convert(varchar(8), @vl_Hauteur)
				insert #SYNTHESE(ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
			end
	
			if @vl_Largeur != null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_LRG_VEH + 
						convert(varchar(8), @vl_Largeur)
				insert #SYNTHESE(ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
			end
	
			if @vl_Poids != null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_POI_VEH + 
						convert(varchar(8), @vl_Poids)
				insert #SYNTHESE(ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
			end

			if @vl_Contenu != null and @vl_Contenu != XDC_CHAINE_VIDE
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_CNT_VEH + 
						convert(varchar(25), @vl_Contenu)
				insert #SYNTHESE(ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
			end
	
		end
	end

/************************************************************************************************************************/
/**                                         Cas d'un evenement de classe Meteo                                         **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClasseMeteo
	begin
		exec @vl_Status = XZAE81 @va_NumEvt_in, @va_CleEvt_in,
		                         @vl_ClasseFmc, @vl_NumTypeFmc, 
		                         @vl_HoroValidation,
		                         @vl_Autoroute, @vl_PR, @vl_Sens
		if @vl_Status != XDC_OK
			return @vl_Status

	end

/************************************************************************************************************************/
/**                                         Cas d'un evenement de classe Nature                                        **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClasseNature
	begin
		select @vl_nature_obstacle = nature
		from BDD_XPL..FMC_NAT
		where   numero = @va_NumEvt_in and cle = @va_CleEvt_in
		having horodate = min(horodate)
		select @vl_Ligne = XZAEC_SYN_PAS_DATE +
				XZAEC_SYN_NATURE + @vl_nature_obstacle
		insert #SYNTHESE(ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
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
				insert #SYNTHESE(ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
			end
			if @vl_EntreeS = XDC_OUI
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ENT_S_DECONS
				insert #SYNTHESE(ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
			end
			if @vl_SortieN = XDC_OUI
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_SOR_N_DECONS
				insert #SYNTHESE(ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
			end
			if @vl_SortieS = XDC_OUI
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_SOR_S_DECONS
				insert #SYNTHESE(ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
			end
		end
		else
		begin
			if @vl_EntreeN = XDC_OUI
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
						   XZAEC_SYN_ENT_N_FERM
			else	select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_ENT_N_OUV
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)

			if @vl_EntreeS = XDC_OUI
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
						   XZAEC_SYN_ENT_S_FERM
			else	select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_ENT_S_OUV
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)

			if @vl_SortieN = XDC_OUI
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
						   XZAEC_SYN_SOR_N_FERM
			else	select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_SOR_N_OUV
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)

			if @vl_SortieS = XDC_OUI
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
						   XZAEC_SYN_SOR_S_FERM
			else	select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_SOR_S_OUV
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
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
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end

		if @vl_Penurie = XDC_OUI
		begin
			/* recherche le nom du carburant manquant */
			select @vl_NomCarburant = nom from CFG..TYP_CRB
			where numero = @vl_Carburant

			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_PEN_CRB + @vl_NomCarburant
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end

		if @vl_CncFerme = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					   XZAEC_SYN_CNC_FERM
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
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
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end
		if @vl_Violence = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_VIO_MNF 
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end
		if @vl_Perception = XZAEC_FMC_PAS_PER
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_PAS_PER_PEA
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
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
					rtrim(@vl_Localisation) + " " + @vl_libelle_localisation_tunnel
		insert #SYNTHESE(ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)

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
			@vl_HoroValidation = BDD_XPL..FMC_HIS.horodate_validation,
			@vl_PR		= BDD_XPL..FMC_HIS.PR,
			@vl_Sens	= BDD_XPL..FMC_HIS.sens
		from BDD_XPL..FMC_HIS
		where	BDD_XPL..FMC_HIS.numero = @vl_NumEvtQueue and 
			BDD_XPL..FMC_HIS.cle = @vl_CleEvtQueue
		group by BDD_XPL..FMC_HIS.numero, BDD_XPL..FMC_HIS.cle
		having BDD_XPL..FMC_HIS.horodate_validation = 
				min(BDD_XPL..FMC_HIS.horodate_validation)

		if @@rowcount != 1
			return XZAEC_FMC_ERR_VAL

		/* le type pour un bouchon est change pour l'affichage */
		/* (libelle plus explicite)                            */
		select @vl_TypeFmc = XZAEC_SYN_FMC_QueueBouchon


		/* mise en forme de l'intitule de la queue de bouchon */
		exec XZAE83	null, null, null, null,
				@vl_Autoroute, @vl_PR, @vl_Sens, 
				@vl_Intitule2 output, @vl_Localisation2 output

		/* recherche si la localisation est dans un tunnel, ajoute le libelle de la niche/IS proche */
		exec PRC..XZAO508 @vl_Autoroute, @vl_PR, @vl_Sens, @vl_libelle_localisation_tunnel_tmp output

		/* ajout de l'intitule de la queue de bouchon */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_QUE +
					rtrim(@vl_Localisation2) + " " + @vl_libelle_localisation_tunnel_tmp
		insert #SYNTHESE(ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)

		/* ajout de la longueur du bouchon */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_LON +
					convert(varchar(8), @vl_Longueur/1000.0)

		insert #SYNTHESE(ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)

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

		insert #SYNTHESE(ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)

		/* Ajouter dans l'historique de la fmc celui de la */
		/* queue de bouchon ordonne suivant la date        */
		/* sans l'enregistrement correspondant a la creation*/
		/* de la fmc de queue de bouchon                    */
		select @vl_OrdreSuivant = 1
		insert #TEMPO_HISTORIQUE_FMC
			(numero, cle, classe, type, nom_type, type_anterieur, autoroute,
			PR, sens, confirme_ou_signale, horodate_validation,
			point_caracteristique, numero_point_caracteristique,
			critere_ordre, VR, VM2, VM1, VL, BAU,
			VR_I, VM2_I, VM1_I, VL_I, BAU_I, BAU_etroite, BAU_I_etroite, majeur)
		select 
			BDD_XPL..FMC_HIS.numero,
			BDD_XPL..FMC_HIS.cle,
			CFG..TYP_FMC.classe,
			BDD_XPL..FMC_HIS.type_FMC_valide,
			CFG..TYP_FMC.nom,
			BDD_XPL..FMC_HIS.type_anterieur,
			BDD_XPL..FMC_HIS.autoroute,
			BDD_XPL..FMC_HIS.PR,
			BDD_XPL..FMC_HIS.sens,
			BDD_XPL..FMC_HIS.confirme_ou_signale,
			BDD_XPL..FMC_HIS.horodate_validation,
			BDD_XPL..FMC_HIS.point_caracteristique,
			BDD_XPL..FMC_HIS.numero_point_caracteristique,
			@vl_OrdreSuivant,
			isnull(BDD_XPL..FMC_HIS.VR, XDC_VOIE_INCONNUE),
			isnull(BDD_XPL..FMC_HIS.VM2, XDC_VOIE_INCONNUE),
			isnull(BDD_XPL..FMC_HIS.VM1, XDC_VOIE_INCONNUE),
			isnull(BDD_XPL..FMC_HIS.VL, XDC_VOIE_INCONNUE),
			isnull(BDD_XPL..FMC_HIS.BAU, XDC_VOIE_INCONNUE),
			isnull(BDD_XPL..FMC_HIS.VR_I, XDC_VOIE_INCONNUE),
			isnull(BDD_XPL..FMC_HIS.VM2_I, XDC_VOIE_INCONNUE),
			isnull(BDD_XPL..FMC_HIS.VM1_I, XDC_VOIE_INCONNUE),
			isnull(BDD_XPL..FMC_HIS.VL_I, XDC_VOIE_INCONNUE),
			isnull(BDD_XPL..FMC_HIS.BAU_I, XDC_VOIE_INCONNUE),
			isnull(BDD_XPL..FMC_HIS.BAU_etroite, XDC_FAUX),
			isnull(BDD_XPL..FMC_HIS.BAU_I_etroite, XDC_FAUX),
			isnull(BDD_XPL..FMC_HIS.majeur, XDC_FAUX)
		from BDD_XPL..FMC_HIS, CFG..TYP_FMC
		where	BDD_XPL..FMC_HIS.numero = @vl_NumEvtQueue and
		 	BDD_XPL..FMC_HIS.cle = @vl_CleEvtQueue and
			CFG..TYP_FMC.numero = BDD_XPL..FMC_HIS.type_FMC_valide and
			BDD_XPL..FMC_HIS.horodate_validation != @vl_HoroValidation
			order by BDD_XPL..FMC_HIS.horodate_validation

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
		from BDD_XPL..FMC_GEN
		where numero = @vl_NumEvtTete and cle = @vl_CleEvtTete

		/* Creation de l'intitule de la creation de FMC de tete */
		select 
			@vl_HoroValidation	= BDD_XPL..FMC_HIS.horodate_validation,
			@vl_PR			= BDD_XPL..FMC_HIS.PR,
			@vl_Sens		= BDD_XPL..FMC_HIS.sens,
			@vl_PointCarTete	= BDD_XPL..FMC_HIS.point_caracteristique,
			@vl_NumPointCarTete	= BDD_XPL..FMC_HIS.numero_point_caracteristique,
			@vl_AutorouteTete	= BDD_XPL..FMC_HIS.autoroute
		from BDD_XPL..FMC_HIS
		where	BDD_XPL..FMC_HIS.numero = @vl_NumEvtTete and 
			BDD_XPL..FMC_HIS.cle = @vl_CleEvtTete 
		group by BDD_XPL..FMC_HIS.numero, BDD_XPL..FMC_HIS.cle
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
			@vl_AutorouteTete, @vl_PR, @vl_Sens, 
			@vl_Intitule2 output, @vl_Localisation2 output

			/* recherche si la localisation est dans un tunnel, ajoute le libelle de la niche/IS proche */
			exec PRC..XZAO508 @vl_AutorouteTete, @vl_PR, @vl_Sens, @vl_libelle_localisation_tunnel_tet output

			if @vl_libelle_localisation_tunnel_tet is not null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_TET +
								 rtrim(@vl_Localisation2) + " " + @vl_libelle_localisation_tunnel_tet
			end
			else
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_TET + 
									@vl_Localisation2
			end
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
			else if @vl_PointCarTete = XDC_POINT_CARACT_BAT
			begin
				select @vl_Libelle = XZAEC_SYN_VEH_BAT

				select @vl_NomPointCarTete = nom
				from CFG..RES_BAT								
				where	numero = @vl_NumPointCarTete
			end
			else return XZAEC_FMC_LOC_INC

			if @@rowcount != 1
			return XZAEC_FMC_LOC_INC

			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_TET +
			                      @vl_Libelle + @vl_NomPointCarTete
		end

		/* ajout de l'intitule de la tete de bouchon */
		insert #SYNTHESE(ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)

		/* ajout de l'intitule de la queue de bouchon */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_QUE +
					 rtrim(@vl_Localisation) + " " + @vl_libelle_localisation_tunnel
		insert #SYNTHESE(ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)

		/* ajout de la longueur du bouchon */
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_LON +
					convert(varchar(8), @vl_Longueur/1000.0)

		insert #SYNTHESE(ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)

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

		insert #SYNTHESE(ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)

		/* Ajouter dans l'historique de la fmc celui de la */
		/* tete de bouchon ordonne suivant la date         */
		/* sans l'enregistrement correspondant a la creation*/
		/* de la fmc de tete de bouchon                     */
		select @vl_OrdreSuivant = 0
		insert #TEMPO_HISTORIQUE_FMC
			(numero, cle, classe, type, nom_type, type_anterieur, autoroute,
			PR, sens, confirme_ou_signale, horodate_validation,
			point_caracteristique, numero_point_caracteristique,
			critere_ordre, VR, VM2, VM1, VL, BAU,
			VR_I, VM2_I, VM1_I, VL_I, BAU_I, BAU_etroite, BAU_I_etroite, majeur)
		select 
			BDD_XPL..FMC_HIS.numero,
			BDD_XPL..FMC_HIS.cle,
			CFG..TYP_FMC.classe,
			BDD_XPL..FMC_HIS.type_FMC_valide,
			CFG..TYP_FMC.nom,
			BDD_XPL..FMC_HIS.type_anterieur,
			BDD_XPL..FMC_HIS.autoroute,
			BDD_XPL..FMC_HIS.PR,
			BDD_XPL..FMC_HIS.sens,
			@vl_ConfirmeSignale,
			BDD_XPL..FMC_HIS.horodate_validation,
			BDD_XPL..FMC_HIS.point_caracteristique,
			BDD_XPL..FMC_HIS.numero_point_caracteristique,
			@vl_OrdreSuivant,
			isnull(BDD_XPL..FMC_HIS.VR, XDC_VOIE_INCONNUE),
			isnull(BDD_XPL..FMC_HIS.VM2, XDC_VOIE_INCONNUE),
			isnull(BDD_XPL..FMC_HIS.VM1, XDC_VOIE_INCONNUE),
			isnull(BDD_XPL..FMC_HIS.VL, XDC_VOIE_INCONNUE),
			isnull(BDD_XPL..FMC_HIS.BAU, XDC_VOIE_INCONNUE),
			isnull(BDD_XPL..FMC_HIS.VR_I, XDC_VOIE_INCONNUE),
			isnull(BDD_XPL..FMC_HIS.VM2_I, XDC_VOIE_INCONNUE),
			isnull(BDD_XPL..FMC_HIS.VM1_I, XDC_VOIE_INCONNUE),
			isnull(BDD_XPL..FMC_HIS.VL_I, XDC_VOIE_INCONNUE),
			isnull(BDD_XPL..FMC_HIS.BAU_I, XDC_VOIE_INCONNUE),
			isnull(BDD_XPL..FMC_HIS.BAU_etroite, XDC_FAUX),
			isnull(BDD_XPL..FMC_HIS.BAU_I_etroite, XDC_FAUX),
			isnull(BDD_XPL..FMC_HIS.majeur, XDC_FAUX)
		from BDD_XPL..FMC_HIS, CFG..TYP_FMC
		where	BDD_XPL..FMC_HIS.numero = @vl_NumEvtTete and 
			BDD_XPL..FMC_HIS.cle = @vl_CleEvtTete and
			CFG..TYP_FMC.numero = BDD_XPL..FMC_HIS.type_FMC_valide and
			BDD_XPL..FMC_HIS.horodate_validation != @vl_HoroValidation
			order by BDD_XPL..FMC_HIS.horodate_validation


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
			@vl_NatureTrv	= BDD_XPL..FMC_TRF.nature,
			@vl_TypeTrv	= BDD_XPL..FMC_TRF.type,
			@vl_Continu	= BDD_XPL..FMC_TRF.continu,
			@vl_DebutPlanifie = BDD_XPL..FMC_TRF.horodate_debut_planifiee,
			@vl_FinPlanifie = BDD_XPL..FMC_TRF.horodate_fin_planifiee,
			@vl_Mobile	= BDD_XPL..FMC_TRF.mobile,
			@vl_AutorouteM	= BDD_XPL..FMC_TRF.autoroute_planifiee,
			@vl_PRQueueM	= BDD_XPL..FMC_TRF.PR_queue_planifie,
			@vl_PRTeteM	= BDD_XPL..FMC_TRF.PR_tete_planifie,
			@vl_SensM	= BDD_XPL..FMC_TRF.sens_planifie,
			@vl_PR		= BDD_XPL..FMC_TRH.localisation_tete,
			@vl_VoiesRet	= BDD_XPL..FMC_TRH.voies_retrecies,
			@vl_PresenceBouchon	= BDD_XPL..FMC_TRH.bouchon,
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
			@vl_vitesse = BDD_XPL..FMC_TRH.vitesse,
			@vl_personnel = BDD_XPL..FMC_TRH.personnel
		from BDD_XPL..FMC_TRF, BDD_XPL..FMC_TRH
		where	BDD_XPL..FMC_TRF.numero = @va_NumEvt_in and 
			BDD_XPL..FMC_TRF.cle = @va_CleEvt_in and
			BDD_XPL..FMC_TRH.numero = @va_NumEvt_in and 
			BDD_XPL..FMC_TRH.cle = @va_CleEvt_in  and
			BDD_XPL..FMC_TRH.horodate = @vl_HoroValidation

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

				else 	return XZAEC_FMC_ERR_DONNEES

				insert #SYNTHESE(ligne, heure, categorie)
				values (@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
			end

			if @vl_NatureTrv = XZAEC_FMC_TRV_NEU
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_TRV_NEU
			else	select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
						   XZAEC_SYN_TRV_ENT

			insert #SYNTHESE(ligne, heure, categorie)
			values (@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)

			if @vl_Mobile = XDC_OUI
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_TRV_MOB
				insert #SYNTHESE(ligne, heure, categorie)
				values (@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
			end

			if @vl_DebutPlanifie != null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_DEB_PLA + 
						   convert(char(10), @vl_DebutPlanifie, 103) + ' ' +
						   convert(char(8), @vl_DebutPlanifie, 8)

				insert #SYNTHESE(ligne, heure, categorie)
				values (@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
			end

			if @vl_FinPlanifie != null
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_FIN_PLA + 
						   convert(char(10), @vl_FinPlanifie, 103) + ' ' +
						   convert(char(8), @vl_FinPlanifie, 8)

				insert #SYNTHESE(ligne, heure, categorie)
				values (@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
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

				/* recherche si la localisation est dans un tunnel, ajoute le libelle de la niche/IS proche */
				exec PRC..XZAO508 @vl_AutorouteM, @vl_PRTeteM, @vl_SensM, @vl_libelle_localisation_tunnel_tmp output

				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_TET_PLA + rtrim(@vl_Localisation2) + " " + @vl_libelle_localisation_tunnel_tmp
				insert #SYNTHESE(ligne, heure, categorie)
				values (@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)

				/* mise en forme de la localisation de la tete */
				/* des travaux planifies */
				exec XZAE83	null, null, null, null,
						@vl_AutorouteM, @vl_PRQueueM, @vl_SensM, 
						@vl_Intitule2 output, @vl_Localisation2 output

				/* recherche si la localisation est dans un tunnel, ajoute le libelle de la niche/IS proche */
				exec PRC..XZAO508 @vl_AutorouteM, @vl_PRQueueM, @vl_SensM, @vl_libelle_localisation_tunnel_tmp output

				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						   XZAEC_SYN_QUE_PLA + rtrim(@vl_Localisation2) + " " + @vl_libelle_localisation_tunnel_tmp
				insert #SYNTHESE(ligne, heure, categorie)
				values (@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
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
			insert #SYNTHESE(ligne, heure, categorie)
			values (@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end

		if @vl_NumTypeFmc = XZAEC_FMC_ZoneStockage
		begin
			if @vl_VoiesRet = XDC_OUI
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_STK_ENCOURS
			else
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_STK_PREPAR
			insert #SYNTHESE(ligne, heure, categorie)
			values (@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end
		else
		if @vl_VoiesRet = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_VOI_RET 
			insert #SYNTHESE(ligne, heure, categorie)
			values (@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end

		/*configuration finale des voies*/
		if (@vl_VR <> null ) and (@vl_VM2 <> null ) and (@vl_VM1 <> null ) and (@vl_VL <> null ) and (@vl_BAU <> null ) and (@vl_VR_I <> null ) and (@vl_VM2_I <> null ) and (@vl_VM1_I <> null ) and (@vl_VL_I <> null ) and (@vl_BAU_I <> null ) 
		begin
			if @vl_ClasseFmc = XZAEC_ClasseBasculTrav
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_CFG_VOIES_BAS_TRV
			else
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_CFG_VOIES_BAS
			insert #SYNTHESE
			values ( @vl_Ligne, null, @vl_VR, @vl_VM2, @vl_VM1,
			@vl_VL, @vl_BAU, @vl_VR_I, @vl_VM2_I, @vl_VM1_I,@vl_VL_I, @vl_BAU_I, 
			XDC_FAUX, XDC_FAUX, null, XDC_CAT_ACT_MODIFICATION)
	
			select @vl_Ligne=XZAEC_SYN_PAS_DATE
			insert #SYNTHESE (ligne,heure, categorie)
			values (@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
	
			select @vl_Ligne=XZAEC_SYN_PAS_DATE + convert(char(10),XZAEC_SYN_VOIE_BAS) 
					+ XZAEC_SYN_CAR_SEPAR +
					convert (char(8), XZAEC_SYN_PRD_BAS) + XZAEC_SYN_CAR_SEPAR+
					convert (char(8),XZAEC_SYN_PRF_BAS) + XZAEC_SYN_CAR_SEPAR +
					convert (char(16),XZAEC_SYN_HD_BAS) +XZAEC_SYN_CAR_SEPAR+
					convert (char(16),XZAEC_SYN_HF_BAS)
			insert #SYNTHESE (ligne,heure, categorie)
			values (@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
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
			insert #SYNTHESE (ligne,heure, categorie)
			values (@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
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
			insert #SYNTHESE (ligne,heure, categorie)
			values (@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
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
			insert #SYNTHESE (ligne,heure, categorie)
			values (@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
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
			insert #SYNTHESE (ligne,heure, categorie)
			values (@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
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
			insert #SYNTHESE (ligne,heure, categorie)
			values (@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
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
			insert #SYNTHESE (ligne,heure, categorie)
			values (@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
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
			insert #SYNTHESE (ligne,heure, categorie)
			values (@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
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
			insert #SYNTHESE (ligne,heure, categorie)
			values (@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
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
			insert #SYNTHESE (ligne,heure, categorie)
			values (@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
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
			insert #SYNTHESE (ligne,heure, categorie)
			values (@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end


		/* ajouter les configurations de voies du Basculement pour travaux */
		if @vl_ClasseFmc = XZAEC_ClasseBasculTrav
		begin
			select @vl_Ligne=XZAEC_SYN_PAS_DATE
			insert #SYNTHESE (ligne,heure, categorie)
			values (@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)

			select @vl_VR=VR, @vl_VM2=VM2, @vl_VM1=VM1, @vl_VL=VL, @vl_BAU=BAU,
			       @vl_VR_I=VR_I, @vl_VM2_I=VM2_I, @vl_VM1_I=VM1_I, @vl_VL_I=VL_I, @vl_BAU_I=BAU_I,
				@vl_vitesse = vitesse, @vl_vitesse_opposee = vitesse_opposee
			from BDD_XPL..FMC_BAS
			where numero = @va_NumEvt_in and cle = @va_CleEvt_in

			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_CFG_VOIES_BAS
			insert #SYNTHESE
			values ( @vl_Ligne, null, @vl_VR, @vl_VM2, @vl_VM1,
			@vl_VL, @vl_BAU, @vl_VR_I, @vl_VM2_I, @vl_VM1_I,@vl_VL_I, @vl_BAU_I, 
			XDC_FAUX, XDC_FAUX, null, XDC_CAT_ACT_MODIFICATION)
		end
		else
		if @vl_ClasseFmc = XZAEC_ClasseTravaux
		begin
			select @vl_vitesse= vitesse,
					@vl_personnel = personnel
			from  BDD_XPL..FMC_TRH
			where   numero = @va_NumEvt_in and cle = @va_CleEvt_in
			and horodate=@vl_HoroValidation

			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_VITESSE + convert(varchar(3),@vl_vitesse)
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)

			if @vl_personnel != 0
			begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_PERSONNEL
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
			end
		end

		/* Pour un Basculement pour travaux, indiquer la presence d'un bouchon mobile */
		if @vl_NumTypeFmc = XZAEC_FMC_BasculTravaux
		begin
			if @vl_PresenceBouchon = XDC_VRAI
			begin
				select @vl_Ligne=XZAEC_SYN_PAS_DATE
				insert #SYNTHESE (ligne,heure, categorie)
				values (@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)

				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BOU_MOBILE
				insert #SYNTHESE(ligne, heure, categorie)
				values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
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
		from BDD_XPL..FMC_BAS
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in

		if @vl_PR != null
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					XZAEC_SYN_FIN_BAS +
					convert(char(7),@vl_PR/1000.0)
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end

		/*configuration finale des voies*/
		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_CFG_VOIES_BAS
		insert #SYNTHESE
		values ( @vl_Ligne, null, @vl_VR, @vl_VM2, @vl_VM1,
		@vl_VL, @vl_BAU, @vl_VR_I, @vl_VM2_I, @vl_VM1_I,@vl_VL_I, @vl_BAU_I, 
		XDC_FAUX, XDC_FAUX, null, XDC_CAT_ACT_MODIFICATION)
	end

/************************************************************************************************************************/
/**                                            Cas d'un evenement de Operateur                                         **/
/************************************************************************************************************************/
	else if @vl_ClasseFmc = XZAEC_ClassePosteOper
	begin
		/* recherche les informations sur le poste operateur */
		select @vl_Operateur = operateur
		from BDD_XPL..FMC_OPR
		where 	numero = @va_NumEvt_in and cle = @va_CleEvt_in 

		/* ajout l'information sur le poste operateur dans la synthese */
		if @vl_Operateur != null
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_OPR +
						nom
			from CFG..OPR_NOM
			where numero = @vl_Operateur

			if @@rowcount != 1
				return XZAEC_FMC_PAS_DONNEES

			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
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
		from BDD_XPL..FMC_INC
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in

		select	@vl_nature_inc = libelle
		from CFG..LIB_PRE
		where	notyp = XDC_LIB_TYPE_NATURE_INCENDIE  and  code = @vl_code_nature

		select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_NATURE + @vl_nature_inc
		insert #SYNTHESE(ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)

		if @vl_FeuMaitrise = XDC_OUI
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_INC_MAITRISE +
					convert(char(10),@vl_HorodateMaitrise,103) + ' ' +
					convert(char(8),@vl_HorodateMaitrise,8)
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end
	end

/************************************************************************************************************************/
/**                                               Cas general                                                          **/
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
		from BDD_XPL..FMC_DAT
		where numero= @va_NumEvt_in and cle = @va_CleEvt_in
		
		if @vl_gravite=0
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_GRAVITE + XZAEC_SYN_GRAVITE0
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end
		else if @vl_gravite=1
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_GRAVITE + XZAEC_SYN_GRAVITE1
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end
		else if @vl_gravite=2
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_GRAVITE + XZAEC_SYN_GRAVITE2
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end
		else if @vl_gravite=3
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_GRAVITE + XZAEC_SYN_GRAVITE3
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end
		
		if @vl_trafic=XDC_RADT_FLUIDE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_TRAFIC + XZAEC_SYN_TRAFIC_FLUIDE
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end
		else if @vl_trafic=XDC_RADT_PRESAT
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_TRAFIC + XZAEC_SYN_TRAFIC_PRESAT
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end
		else if @vl_trafic=XDC_RADT_SATURE
		begin
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_TRAFIC + XZAEC_SYN_TRAFIC_SATURE
			insert #SYNTHESE(ligne, heure, categorie)
			values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
		end
		
		if @vl_bloquant=XDC_VRAI
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_BLOQUANT
		else
			select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_NONBLOQUANT
		
		insert #SYNTHESE(ligne, heure, categorie)
		values(@vl_Ligne, null, XDC_CAT_ACT_MODIFICATION)
	
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
		VR, VM2, VM1, VL, BAU, VR_I, VM2_I, VM1_I, VL_I, BAU_I, BAU_etroite, BAU_I_etroite,
		majeur, ecoulement
	from #HISTORIQUE_FMC

	open Pointeur_histo
	fetch Pointeur_histo into @vl_NumEvt, @vl_CleEvt,
				@vl_ClasseFmc, @vl_TypeFmc, 
				@vl_NumTypeFmc, @vl_TypeAnterieur, 
				@vl_Autoroute, @vl_PR, @vl_Sens,
				@vl_ConfirmeSignale, @vl_HoroValidation,
				@vl_VR, @vl_VM2, @vl_VM1, @vl_VL, @vl_BAU, 
				@vl_VR_I, @vl_VM2_I, @vl_VM1_I,
				@vl_VL_I, @vl_BAU_I, @vl_BAU_etroite, @vl_BAU_I_etroite,
				@vl_majeur, @vl_ecoulement
	select @vl_SqlStatusValidation = @@sqlstatus

	while (@vl_SqlStatusValidation = 0)
	begin
		if @vl_IndicateurCreation != XDC_OUI
		begin

			/* Determination de la categorie des actions pour la synthèse en fonction du status de la création */
			if @vl_status_courant = @vl_ConfirmeSignale
				select @vl_categorie_courante = XDC_CAT_ACT_MODIFICATION
			else 
				select @vl_categorie_courante = XDC_CAT_ACT_ETAPE
			select @vl_status_courant = @vl_ConfirmeSignale

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
	
			/* mise a jour de l'autoroute pour la localisation des actions */
			select @vl_AutoroutePourAction = @vl_Autoroute

			/*B Ajout de l'intitule d'une validation de FMC */
			/*B dans la synthese */
			insert #SYNTHESE(ligne, heure, categorie)
			values ( @vl_Ligne, @vl_HoroValidation, @vl_categorie_courante)

			/* recherche si la localisation est dans un tunnel, ajoute le libelle de la niche/IS proche */
			exec PRC..XZAO508 @vl_Autoroute,@vl_PR,@vl_Sens,@vl_libelle_localisation_tunnel_tmp output

			select @vl_Ligne = XZAEC_SYN_PAS_DATE + rtrim(@vl_Localisation) + " " + @vl_libelle_localisation_tunnel_tmp
			insert #SYNTHESE 
			values ( @vl_Ligne, null,
				@vl_VR, @vl_VM2, @vl_VM1,
				@vl_VL, @vl_BAU, @vl_VR_I, @vl_VM2_I, 
				@vl_VM1_I,@vl_VL_I, @vl_BAU_I, 
				@vl_BAU_etroite, @vl_BAU_I_etroite, null, @vl_categorie_courante)
	
			if @vl_ConfirmeSignale = XZAEC_FMC_CONFIRMEE
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
						XZAEC_SYN_CONFIRMEE
			else 	select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						XZAEC_SYN_SIGNALEE
			insert #SYNTHESE(ligne, heure, categorie)
			values ( @vl_Ligne, null, @vl_categorie_courante)
	
			if @vl_majeur = XDC_VRAI
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_MAJEUR
				insert #SYNTHESE(ligne, heure, categorie)
				values ( @vl_Ligne, null, @vl_categorie_courante)
			end

			/*B Ajout du complement de la validation de */
			/*B FMC dans la synthese                    */
			exec @vl_Status = XZAE81 @vl_NumEvt, @vl_CleEvt, 
						@vl_ClasseFmc, @vl_NumTypeFmc, 
						@vl_HoroValidation,
						@vl_Autoroute, @vl_PR, @vl_Sens
	
			if @vl_Status != XDC_OK
				return XZAEC_FMC_ERR_DON_VAL


			/*historisation des victimes*/
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
				from BDD_XPL..FMC_ACC
				where	numero = @va_NumEvt_in and cle = @va_CleEvt_in
				and validation=@vl_HoroValidation

				if @@rowcount != 0
				begin

				if @vl_NbVehLeg != null and @vl_NbVehLeg != 0
				begin
					select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
							XZAEC_SYN_VL + 
							convert(varchar(8),@vl_NbVehLeg)
					insert #SYNTHESE(ligne, heure, categorie)
					values(@vl_Ligne, null, @vl_categorie_courante)
				end

				if @vl_NbPL != null and @vl_NbPL != 0
				begin
					select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
							XZAEC_SYN_PL + 
							convert(varchar(8),@vl_NbPL)
					insert #SYNTHESE(ligne, heure, categorie)
					values(@vl_Ligne, null, @vl_categorie_courante)
				end

				if @vl_NbMoto != null and @vl_NbMoto != 0
				begin
					select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
							XZAEC_SYN_MOTO + 
							convert(varchar(8), @vl_NbMoto)
					insert #SYNTHESE(ligne, heure, categorie)
					values(@vl_Ligne, null, @vl_categorie_courante)
				end

				if @vl_NbCar != null and @vl_NbCar != 0
				begin
					select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
							XZAEC_SYN_CAR + 
							convert(varchar(8), @vl_NbCar)
					insert #SYNTHESE(ligne, heure, categorie)
					values(@vl_Ligne, null, @vl_categorie_courante)
				end

				if @vl_NbRemorques != null and @vl_NbRemorques != 0
				begin
					select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
							XZAEC_SYN_REMORQ + 
							convert(varchar(8), @vl_NbRemorques)
					insert #SYNTHESE(ligne, heure, categorie)
					values(@vl_Ligne, null, @vl_categorie_courante)
				end

				if @vl_BlessesLeg != null and @vl_BlessesLeg != 0 
				begin
					select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
							XZAEC_SYN_BLE_LEG + 
							convert(varchar(8), @vl_BlessesLeg)
					insert #SYNTHESE(ligne, heure, categorie)
					values(@vl_Ligne, null, @vl_categorie_courante)
				end

				if @vl_BlessesGra != null and @vl_BlessesGra != 0
				begin
					select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
							XZAEC_SYN_BLE_GRA + 
							convert(varchar(8), @vl_BlessesGra)
					insert #SYNTHESE(ligne, heure, categorie)
					values(@vl_Ligne, null, @vl_categorie_courante)
				end

				if @vl_Morts != null and @vl_Morts != 0
				begin
					select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
							XZAEC_SYN_MOR + 
							convert(varchar(8), @vl_Morts)
					insert #SYNTHESE(ligne, heure, categorie)
					values(@vl_Ligne, null, @vl_categorie_courante)
				end

				if @vl_MatDang = XDC_OUI
				begin
					select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
							XZAEC_SYN_PRE_MAT_DNG
					insert #SYNTHESE(ligne, heure, categorie)
					values(@vl_Ligne, null, @vl_categorie_courante)
				end

				if @vl_VehESCOTA = XDC_OUI
				begin
					select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
							XZAEC_SYN_VEH_ESC 
					insert #SYNTHESE(ligne, heure, categorie)
					values(@vl_Ligne, null, @vl_categorie_courante)
				end

				if @vl_PersESCOTA = XDC_OUI
				begin
					select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
							XZAEC_SYN_PER_ESC
					insert #SYNTHESE(ligne, heure, categorie)
					values(@vl_Ligne, null, @vl_categorie_courante)
				end

				if @vl_Personalite = XDC_OUI
				begin
					select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
							XZAEC_SYN_PERS 
					insert #SYNTHESE(ligne, heure, categorie)
					values(@vl_Ligne, null, @vl_categorie_courante)
				end

				if @vl_ecoulement = XDC_OUI
				begin
					select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
							XZAEC_SYN_ECOUL
					insert #SYNTHESE(ligne, heure, categorie)
					values(@vl_Ligne, null, @vl_categorie_courante)
				end

				end
			end
			else if @vl_ClasseFmc = XZAEC_ClasseVehicule
			begin
				if @vl_ecoulement = XDC_OUI
				begin
					select @vl_Ligne = XZAEC_SYN_PAS_DATE + 
							XZAEC_SYN_ECOUL
					insert #SYNTHESE(ligne, heure, categorie)
					values(@vl_Ligne, null, @vl_categorie_courante)
				end
			end
			else if @vl_ClasseFmc = XZAEC_ClasseTravaux
			begin
				select @vl_vitesse= vitesse,
					@vl_personnel = personnel
				from  BDD_XPL..FMC_TRH
				where   numero = @va_NumEvt_in and cle = @va_CleEvt_in
				and horodate=@vl_HoroValidation
				if @@rowcount>0
				begin
					select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_VITESSE + convert(varchar(3),@vl_vitesse)
					insert #SYNTHESE(ligne, heure, categorie)
					values(@vl_Ligne, null, @vl_categorie_courante)

					if @vl_personnel > 0
					begin
						select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_PERSONNEL
						insert #SYNTHESE(ligne, heure, categorie)
						values(@vl_Ligne, null, @vl_categorie_courante)
					end
				end
			end
			else if @vl_ClasseFmc = XZAEC_ClasseRegulation
			begin
				select
                                        @vl_scenario = scenario,
                                        @vl_contexte = contexte,
                                        @vl_transitoire = transitoire,
                                        @vl_zone = zone,
                                        @vl_mesure = mesure,
                                        @vl_indicateur_horaire = indicateur_horaire,
                                        @vl_horodate=horodate
                                                from BDD_XPL..FMC_REG
                                                where   numero = @va_NumEvt_in and cle = @va_CleEvt_in
                                                and zone != null and scenario != -1
                                                and horodate =  @vl_HoroValidation

                                if @@rowcount > 0
                                begin
                                if @vl_scenario != null and @vl_scenario != 0
                                begin
                                        select @vl_Intitule = libelle from CFG..REG_SCE
                                        where numero = @vl_scenario

                                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                                XZAEC_SYN_SCEN + ' ' + @vl_Intitule
                                        insert #SYNTHESE(ligne, heure, categorie)
                                        values(@vl_Ligne, null, @vl_categorie_courante)
                                end

                                if @vl_contexte != null and @vl_contexte != 0
                                begin
                                        select @vl_Intitule = libelle
                                        from CFG..LIB_PRE
                                        where notyp = XDC_LIB_TYPE_CONTEXTE_REGUL
                                        and code = @vl_contexte
                                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                                        XZAEC_SYN_CON + ' ' + @vl_Intitule
                                        insert #SYNTHESE(ligne, heure, categorie)
                                        values(@vl_Ligne, null, @vl_categorie_courante)
                                end

                                if @vl_transitoire != null
                                begin
                                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                                XZAEC_SYN_TRA + @vl_transitoire
                                        insert #SYNTHESE(ligne, heure, categorie)
                                        values(@vl_Ligne, null, @vl_categorie_courante)
                                end

                                if @vl_mesure != null and @vl_mesure != 0
                                begin
                                        select @vl_Intitule = libelle
                                        from CFG..LIB_PRE
                                        where notyp = XDC_LIB_TYPE_MESURE_REGUL
                                        and code = @vl_mesure
                                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                                XZAEC_SYN_MES +  ' ' + @vl_Intitule
                                        insert #SYNTHESE(ligne, heure, categorie)
                                        values(@vl_Ligne, null, @vl_categorie_courante)
                                end

                                if @vl_indicateur_horaire = 1
                                begin
                                        select @vl_Ligne = XZAEC_SYN_PAS_DATE +
                                                XZAEC_SYN_IND_HOR
                                        insert #SYNTHESE(ligne, heure, categorie)
                                        values(@vl_Ligne, null, @vl_categorie_courante)
                                end
				end
			end
		end

		/* memoriser la date de validation venant d'etre traitee */
		select @vl_HoroValPrec = @vl_HoroValidation
		/* Probleme des milliemes de secondes : on les retire */
		/* on perd des actions quand elles sont lancees dans la même seconde que la validation du PA */
		/* @HoroValPrec contient les milliemes de secondes */
		/* heure_suscces dans ACT_GEN ne contient pas les milliemes */
		/* donc les lignes sont exclues a cause de la clause */
		/* where heure_succes >= @vl_HoroValPrec par exemple, lors de l'insert dans #ACTIONS ci-dessous */ 
		/* Suite mise en prod COCKPIT */
		select @vl_HoroValPrec = dateadd(millisecond, -datepart(millisecond, @vl_HoroValPrec),@vl_HoroValPrec)

		/* lecture de la validation suivante */
		fetch Pointeur_histo into @vl_NumEvt, @vl_CleEvt,
				@vl_ClasseFmc, @vl_TypeFmc, @vl_NumTypeFmc, @vl_TypeAnterieur,
				@vl_Autoroute, @vl_PR, @vl_Sens, 
				@vl_ConfirmeSignale, @vl_HoroValidation,
				@vl_VR, @vl_VM2, @vl_VM1, @vl_VL, @vl_BAU, 
				@vl_VR_I, @vl_VM2_I, @vl_VM1_I,
				@vl_VL_I, @vl_BAU_I, @vl_BAU_etroite, @vl_BAU_I_etroite,
				@vl_majeur, @vl_ecoulement

		/* memorisation de l'etat du curseur de validation */
		select @vl_SqlStatusValidation = @@sqlstatus
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
					from BDD_XPL..FMC_GEN, BDD_XPL..FMC_HIS, CFG..TYP_FMC
					where BDD_XPL..FMC_GEN.numero = @vl_NumEvtConseq and BDD_XPL..FMC_GEN.cle = @vl_CleEvtConseq and
						 CFG..TYP_FMC.numero =  BDD_XPL..FMC_GEN.type and
						 BDD_XPL..FMC_HIS.numero = BDD_XPL..FMC_GEN.numero and BDD_XPL..FMC_HIS.cle = BDD_XPL..FMC_GEN.cle and
						 BDD_XPL..FMC_HIS.horodate_validation in (	select min(horodate_validation) 
											from BDD_XPL..FMC_HIS
											where numero = @vl_NumEvtConseq and 
												cle = @vl_CleEvtConseq)
		
					/* presenter le numero de la queue de bouchon a la place de */
					/* celui de la tete et son type                             */
					if @vl_NumTypeFmcConseq = XZAEC_FMC_TeteBouchon 
					begin
						select @vl_NumEvtConseq = numero from BDD_XPL..FMC_GEN
						where cause = @vl_NumEvtConseq and cle_cause = @vl_CleEvtConseq

						select @vl_TypeFmcConseq = nom from CFG..TYP_FMC
						where numero = XZAEC_FMC_QueueBouchon
					end
		
					/* Creation de l'intitule d'une validation de FMC */
					exec XZAE83	@vl_NumEvtConseq, @vl_CleEvtConseq,
							@vl_NumTypeFmcConseq, @vl_TypeFmcConseq,
							@vl_Autoroute_conseq, @vl_PR_conseq, @vl_Sens_conseq, 
							@vl_Intitule output, @vl_Localisation output

					/* recherche si la localisation est dans un tunnel, ajoute le libelle de la niche/IS proche */
					exec PRC..XZAO508 @vl_Autoroute_conseq,@vl_PR_conseq,@vl_Sens_conseq,@vl_libelle_localisation_tunnel_tmp output

					select @vl_Ligne =  @vl_InitLigne + convert(varchar(60), @vl_Intitule) +
					                    ' ' + rtrim(@vl_Localisation) + " " + @vl_libelle_localisation_tunnel_tmp
			
					insert #SYNTHESE_CONSEQ (ligne, heure, VR, VM2, VM1, VL, BAU,
								VR_I, VM2_I, VM1_I, VL_I, BAU_I, BAU_etroite, BAU_I_etroite, categorie)
					values ( @vl_Ligne, @vl_Date, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE,
						XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE,
						XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_FAUX, XDC_FAUX, @vl_categorie_courante)
		
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
				select @vl_Ligne = XZAEC_SYN_CONSEQ + XZAEC_SYN_AUCUNE_CONSEQ
		
				insert #SYNTHESE_CONSEQ (ligne, heure, VR, VM2, VM1, VL, BAU,
							VR_I, VM2_I, VM1_I, VL_I, BAU_I, BAU_etroite, BAU_I_etroite, categorie)
				values ( @vl_Ligne, @vl_Debut, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE,
					XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE,
					XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_FAUX, XDC_FAUX, @vl_categorie_courante)
			end
			

			/*B Ajout la cause de la FMC */
			if @vl_Cause != null and @vl_CleCause != null
			begin
				/* recherche les informations sur la cause */
				select
					@vl_Autoroute_cause = BDD_XPL..FMC_HIS.autoroute,
					@vl_PR_cause = BDD_XPL..FMC_HIS.PR,
					@vl_Sens_cause = BDD_XPL..FMC_HIS.sens,
					@vl_NumTypeCause = CFG..TYP_FMC.numero,
					@vl_TypeCause = CFG..TYP_FMC.nom
				from 	BDD_XPL..FMC_GEN, BDD_XPL..FMC_HIS, CFG..TYP_FMC
				where	BDD_XPL..FMC_GEN.numero = @vl_Cause and 
					BDD_XPL..FMC_GEN.cle = @vl_CleCause and
					BDD_XPL..FMC_HIS.numero = @vl_Cause and
					BDD_XPL..FMC_HIS.cle = @vl_CleCause and
					CFG..TYP_FMC.numero = BDD_XPL..FMC_GEN.type 
					group by BDD_XPL..FMC_HIS.numero, BDD_XPL..FMC_HIS.cle,
					BDD_XPL..FMC_GEN.numero, BDD_XPL..FMC_GEN.cle,
					CFG..TYP_FMC.numero
					having BDD_XPL..FMC_HIS.horodate_validation =
						min(BDD_XPL..FMC_HIS.horodate_validation)
	
				if @@rowcount = 1
				begin
					/* localisation de la cause */
					exec XZAE83	@vl_Cause, @vl_CleCause, @vl_NumTypeCause, @vl_TypeCause,
							@vl_Autoroute_cause, @vl_PR_cause, @vl_Sens_cause, 
							@vl_Intitule output, @vl_Localisation output
		
					/* recherche si la localisation est dans un tunnel, ajoute le libelle de la niche/IS proche */
					exec PRC..XZAO508 @vl_Autoroute_cause,@vl_PR_cause,@vl_Sens_cause,@vl_libelle_localisation_tunnel_tmp output

					/* insertion de la cause */
					select @vl_Ligne =  XZAEC_SYN_CAUSE + convert(varchar(60), @vl_Intitule) +
					                    ' ' + rtrim(@vl_Localisation) + " " + @vl_libelle_localisation_tunnel_tmp
		
					insert #SYNTHESE_CONSEQ (ligne, heure, VR, VM2, VM1, VL, BAU,
								VR_I, VM2_I, VM1_I, VL_I, BAU_I, BAU_etroite, BAU_I_etroite, categorie)
					values ( @vl_Ligne, @vl_Debut, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE,
						XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE,
						XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_VOIE_INCONNUE, XDC_FAUX, XDC_FAUX, @vl_categorie_courante)
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
			select * from BDD_XPL..FMC_COM
			where	evenement = @vl_NumEvt and
				cle = @vl_CleEvt and
				(@vl_SqlStatusValidation = 2 or 
			 	horodate_validation < @vl_HoroValidation)
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
			select * from BDD_XPL..FMC_COM
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
		from BDD_XPL..ACT_GEN
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
		from BDD_XPL..ACT_GEN
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
		from BDD_XPL..ACT_GEN
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
		from BDD_XPL..ACT_GEN
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
				print "Action : %1!/%2! (etat %3!)", @vl_NumAction, @vl_Site, @vl_TypeHeure
				exec @vl_Status = XZAE82 @vl_NumAction, @vl_Site, @vl_SiteOrigine,
						@vl_TypeAction, @vl_Heure, 
						@vl_TypeHeure, @vl_Eqt, @vl_AutoroutePourAction

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
				select @vl_Ligne = convert(char(10), @vl_HoroComm, 103) + ' ' +
					convert(char(8), @vl_HoroComm, 8) + ' ' +
					XZAEC_SYN_COM 
			else select @vl_Ligne = convert(char(10), @vl_HoroComm, 103) + ' ' +
					convert(char(8), @vl_HoroComm, 8) + ' ' +
					XZAEC_SYN_COM_VAL + 
					convert(char(10), @vl_HoroCommFin, 103) + ' ' +
					convert(char(8), @vl_HoroCommFin, 8)

			/* inserer l'entete du commentaire */
			insert #SYNTHESE(ligne, heure, categorie)
			values( @vl_Ligne, @vl_HoroComm, XDC_CAT_ACT_COMMENTAIRE)
			
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
				insert #SYNTHESE(ligne, heure, categorie)
				 values( @vl_Ligne, null, XDC_CAT_ACT_COMMENTAIRE)

				if @vl_Longueur > 60
				begin
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						substring(@vl_Texte, 61, 60)
					insert #SYNTHESE(ligne, heure, categorie)
					 values( @vl_Ligne, null, XDC_CAT_ACT_COMMENTAIRE)

					if @vl_Longueur > 120
					begin
						select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							substring(@vl_Texte, 121, 60)
						insert #SYNTHESE(ligne, heure, categorie)
						values( @vl_Ligne, null, XDC_CAT_ACT_COMMENTAIRE)
					end
					if @vl_Longueur > 180
					begin
						select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							substring(@vl_Texte, 181, 60)
						insert #SYNTHESE(ligne, heure, categorie)
						values( @vl_Ligne, null, XDC_CAT_ACT_COMMENTAIRE)
					end
					if @vl_Longueur > 240
					begin
						select @vl_Ligne = XZAEC_SYN_PAS_DATE +
							substring(@vl_Texte, 241, 10)
						insert #SYNTHESE(ligne, heure, categorie)
						values( @vl_Ligne, null, XDC_CAT_ACT_COMMENTAIRE)
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
			insert #SYNTHESE(ligne, heure, categorie)
			values( @vl_Ligne, null, XDC_CAT_ACT_COMMENTAIRE)

			if @vl_Longueur > 60
			begin
				select @vl_Ligne = XZAEC_SYN_PAS_DATE +
					substring(@vl_Texte, 61, 60)
				insert #SYNTHESE(ligne, heure, categorie)
				values( @vl_Ligne, null, XDC_CAT_ACT_COMMENTAIRE)

				if @vl_Longueur > 120
				begin
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						substring(@vl_Texte, 121, 60)
					insert #SYNTHESE(ligne, heure, categorie)
					values( @vl_Ligne, null, XDC_CAT_ACT_COMMENTAIRE)
				end
				if @vl_Longueur > 180
				begin
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						substring(@vl_Texte, 181, 60)
					insert #SYNTHESE(ligne, heure, categorie)
					values( @vl_Ligne, null, XDC_CAT_ACT_COMMENTAIRE)
				end
				if @vl_Longueur > 240
				begin
					select @vl_Ligne = XZAEC_SYN_PAS_DATE +
						substring(@vl_Texte, 241, 10)
					insert #SYNTHESE(ligne, heure, categorie)
					values( @vl_Ligne, null, XDC_CAT_ACT_COMMENTAIRE)
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
		insert #SYNTHESE
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
		select @vl_Ligne = convert(char(10), @vl_Fin, 103) +
					' ' + convert(char(8), @vl_Fin, 8) + ' ' +
					XZAEC_SYN_FINIE
		insert #SYNTHESE(ligne, heure, categorie)
		values( @vl_Ligne, @vl_Fin, XDC_CAT_ACT_ETAPE)
	end

	/*A Retourner la synthese */
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
		categorie
	from #SYNTHESE
	
        select @va_Cloture_out=@vl_Cloture

	return XDC_OK
go	
