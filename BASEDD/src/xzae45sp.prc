/*E*/
/*  Fichier : $Id: xzae45sp.prc,v 1.10 2020/09/14 07:10:07 pc2dpdy Exp $      Release : $Revision: 1.10 $        Date : $Date: 2020/09/14 07:10:07 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae45sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	31/08/94	: Creation
* C.T.	12/08/94	: Modif table FMC_VEH (numero_vehicule 
*                         remplace evenement) et 
*                         position param entree
* C.T.	04/10/94	: Ajout table FMC_VEH (site_depannage)
*                         at dans l'interface
*                         remplace va_Enleve_out par 
*                         va_Depannage_out	(V 1.4)
* C.T.  10/11/94	: Modif bugs applix 	(V 1.5)
* C.T.	14/11/94	: Modif recherche des infos au CI (V 1.6)
* P.V.  15/07/96	: Ajout colonnes dans FMC_VEH (DEM76) (V1.7)
* P.V.  05/09/96	: FMC vehicule => suppr test num_veh=0 (V1.8)
* C.P.	17/06/98	: Modif separation EXP et HIS (V 1.6)
* ABE	10/09/20	: DEM-SAE130 Datex2 Récuperation du code matiere avec le curseur (pb récuperation par asql_ctparam_char_output)	V2.0
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs d'une fiche main courante vehicule
* non implique dans un accident
------------------------------------------------------- 
* Sequence d'appel
* SP	XZAE45_Lire_Fiche_Vehicule
* 
* Arguments en entree
* XDY_Horodate		va_Horodate_in : par defaut heure courante(non utilisee)
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* 
* Arguments en sortie
* XDY_Octet		va_Type_out
* XDY_Octet		va_Passagers_out
* XDY_Booleen		va_Bebe_out
* XDY_Booleen		va_PersAgee_out
* XDY_Booleen		va_Animaux_out
* XDY_Booleen		va_VehEscota_out
* XDY_Octet		va_Chargement_out
* char(6)		va_CodeMat_out
* XDY_Entier		va_Depannage_out
* XDY_Site		va_SiteDepannage_out
* char(100)		va_Texte_out
* XDY_Mot		va_numveh_out
* char(25)		va_marque_out
* char(25)		va_type_out
* char(25)		va_couleur_out
* char(25)		va_immatriculation_out
* XDY_Mot		va_code_pays_out
* char(25)		va_pays_out
* char(25)		va_nom_out
* char(25)		va_ville_out
* char(75)		va_adresse_out
* char(4)		va_code_danger_out
* XDY_Mot		va_code_nature_panne_out
* char(25)		va_nature_panne_out
* char(25)		va_positionnement_out
* XDY_Octet		va_vehicule_roulant_out
* char(25)		va_enlev_dem_par_out
* XDY_Entier		va_numero_fiche_appel_out
* XDY_Octet		va_pres_mat_dangereuse_out
* XDY_Octet		va_fax_envoye_out
* XDY_Mot		va_code_marque_out
* XDY_Mot		va_code_couleur_out
* XDY_Mot		va_code_enlev_demande_par_out
* XDY_Octet		va_site_creation_out
* XDY_Mot		va_nb_bebes_out
* XDY_Mot		va_nb_agees_out
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de complement de fiche main courante
* XDC_ARG_INV
* <0 erreur sybase 
* 
* Conditions d'utilisation
* En formation, preciser l'horodate
* 
* La procedure stockee au CI recherche les informations 
* dans la base HIS si la date donnee est anterieure de plus d'un 
* mois tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Le code de creation de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* et du test de la date en entree (< 1 mois)
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
*
* Suite a un ajout de colonnes dans FMC_VEH, les champs vehicule_roulant,
* presence_matiere_dangereuse et fax_envoye n'ont pu etre codes en booleens.
* Des controles sont donc necessaires dans le code appelant cette procedure pour que les
* valeurs de ces champs soient 0 ou 1 uniquement.
* 
* Fonction
* Select dans la table VEHICULE
* where numero evenement and horodate
-------------------------------------------------------*/

use PRC
go

create procedure 		XZAE;45
	@va_Horodate_in		datetime = null, -- par defaut heure courante
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null,
	@va_Type_out		tinyint = null	output,
	@va_Passagers_ou	tinyint = null	output,
	@va_Bebe_out		bit = XDC_NON	output,
	@va_PersAgee_out	bit = XDC_NON	output,
	@va_Animaux_out		bit = XDC_NON	output,
	@va_TypeVehEscota_out	bit = XDC_NON	output,
	@va_Chargement_out	tinyint = null	output,
	@va_CodeMat_out		char(6) = null	output,
	@va_Depannage_out	int = null	output,
	@va_SiteDepannage_out	T_SITE = null	output,
	@va_Texte_out		char(100) = null output,
	@va_numveh_out 		smallint = null output,
	@va_marque_out		char(25) = null output,
	@va_type_out		char(25) = null output,
	@va_couleur_out		char(25) = null output,
	@va_immatriculation_out char(25) = null output,
	@va_code_pays_out	smallint = null output,
	@va_pays_out		char(25) = null output,
	@va_nom_out		char(25) = null output,
	@va_ville_out		char(25) = null output,
	@va_adresse_out		char(75) = null output,
	@va_code_danger_out	char(4)  = null output,
	@va_code_nature_panne_out smallint = null output,
	@va_nature_panne_out	char(25) = null output,
	@va_positionnement_out	char(25) = null output,
	@va_vehicule_roulant_out tinyint = null output,
	@va_enlev_dem_par_out	char(25) = null output,
	@va_numero_fiche_appel_out   int = null output,
	@va_pres_mat_dangereuse_out tinyint = null output,
	@va_fax_envoye_out	tinyint = null output,
	@va_code_marque_out     smallint = null output,
	@va_code_couleur_out    smallint = null output,
	@va_code_enlev_dem_par_out       smallint = null output,
	@va_site_creation_out       tinyint = null output,
	@va_nb_bebes_out           smallint = null output,
	@va_nb_agees_out           smallint = null output
as
	declare @vl_Status int
	
	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/* test la presence des parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV

	/*A lecture des informations sur complement vehicule non */
	/*A implique dans un accident                            */

#ifdef HIST
	select 
		@va_Type_out		= type_de_vehicule,
		@va_Passagers_ou	= nombre_passagers,
		@va_Bebe_out		= bebe,
		@va_PersAgee_out	= personne_agee,
		@va_Animaux_out		= animaux_vivants,
		@va_TypeVehEscota_out	= vehicule_ESCOTA,
		@va_Chargement_out	= volume_de_chargement,
		@va_CodeMat_out		= matieres_dangereuses,
		@va_Depannage_out	= depannage,
		@va_SiteDepannage_out	= site_depannage,
		@va_Texte_out		= commentaire,
		@va_numveh_out		= numero_vehicule,
		@va_marque_out		= marque,
		@va_type_out		= HIS..FMC_VEH.type,
		@va_couleur_out		= couleur,
		@va_immatriculation_out = immatriculation,
		@va_code_pays_out	= code_pays,
		@va_pays_out		= pays,
		@va_nom_out		= HIS..FMC_VEH.nom,
		@va_ville_out		= ville,
		@va_adresse_out		= adresse,
		@va_code_danger_out	= code_danger,
		@va_code_nature_panne_out = code_nature_panne,
		@va_nature_panne_out	= nature_panne,
		@va_positionnement_out	= positionnement,
		@va_vehicule_roulant_out = vehicule_roulant,
		@va_enlev_dem_par_out	= enlevement_demande_par,
		@va_numero_fiche_appel_out = numero_fiche_appel,
		@va_pres_mat_dangereuse_out = presence_matiere_dangereuse,
		@va_fax_envoye_out	= fax_envoye,
		@va_code_marque_out	= code_marque,
		@va_code_couleur_out	= code_couleur,
		@va_code_enlev_dem_par_out = code_enlev_demande_par,
		@va_site_creation_out	= site_creation,
		@va_nb_bebes_out	= nb_bebes,
		@va_nb_agees_out	= nb_agees
	from HIS..FMC_VEH, HIS..FMC_GEN, CFG..TYP_FMC
	where	HIS..FMC_GEN.numero = @va_NumEvt_in and HIS..FMC_GEN.cle = @va_CleEvt_in and 
		CFG..TYP_FMC.numero = HIS..FMC_GEN.type and CFG..TYP_FMC.classe = XZAEC_ClasseVehicule and
		HIS..FMC_VEH.numero = HIS..FMC_GEN.numero and HIS..FMC_VEH.cle = HIS..FMC_GEN.cle

#else
#ifdef CI
	/*! cas ou la procedure est creee au CI */

	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	/*A la base EXP (au niveau du CI)                       */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
		begin
			exec @vl_Status = SQL_HIS.PRC..XZAE;45
				@va_Horodate_in	, @va_NumEvt_in	, @va_CleEvt_in	, @va_Type_out output, @va_Passagers_ou, @va_Bebe_out output,
				@va_PersAgee_out output, @va_Animaux_out output, @va_TypeVehEscota_out output, @va_Chargement_out output, @va_CodeMat_out output,  @va_Depannage_out output,
				@va_SiteDepannage_out output, @va_Texte_out output, @va_numveh_out output , @va_marque_out output, @va_type_out output, @va_couleur_out output, @va_immatriculation_out output,
				@va_code_pays_out output, @va_pays_out output, @va_nom_out output, @va_ville_out output, @va_adresse_out output, @va_code_danger_out output, @va_code_nature_panne_out output,
				@va_nature_panne_out output, @va_positionnement_out output, @va_vehicule_roulant_out output, @va_enlev_dem_par_out output, @va_numero_fiche_appel_out output, 
				@va_pres_mat_dangereuse_out output, @va_fax_envoye_out output, @va_code_marque_out output, @va_code_couleur_out output , @va_code_enlev_dem_par_out output,
				@va_site_creation_out output , @va_nb_bebes_out output, @va_nb_agees_out output
 			if @vl_Status = null
                       	 	return XDC_PRC_INC
                	else return @vl_Status
		end
	else
		select 
			@va_Type_out		= type_de_vehicule,
			@va_Passagers_ou	= nombre_passagers,
			@va_Bebe_out		= bebe,
			@va_PersAgee_out	= personne_agee,
			@va_Animaux_out		= animaux_vivants,
			@va_TypeVehEscota_out	= vehicule_ESCOTA,
			@va_Chargement_out	= volume_de_chargement,
			@va_CodeMat_out		= matieres_dangereuses,
			@va_Depannage_out	= depannage,
			@va_SiteDepannage_out	= site_depannage,
			@va_Texte_out		= commentaire,
			@va_numveh_out		= numero_vehicule,
			@va_marque_out          = marque,
			@va_type_out            = EXP..FMC_VEH.type,
			@va_couleur_out         = couleur,
			@va_immatriculation_out = immatriculation,
			@va_code_pays_out       = code_pays,
			@va_pays_out            = pays,
			@va_nom_out             = EXP..FMC_VEH.nom,
			@va_ville_out           = ville,
			@va_adresse_out         = adresse,
			@va_code_danger_out     = code_danger,
			@va_code_nature_panne_out = code_nature_panne,
			@va_nature_panne_out    = nature_panne,
			@va_positionnement_out  = positionnement,
			@va_vehicule_roulant_out = vehicule_roulant,
			@va_enlev_dem_par_out   = enlevement_demande_par,
			@va_numero_fiche_appel_out = numero_fiche_appel,
			@va_pres_mat_dangereuse_out = presence_matiere_dangereuse,
			@va_fax_envoye_out      = fax_envoye,
			@va_code_marque_out     = code_marque,
			@va_code_couleur_out    = code_couleur,
			@va_code_enlev_dem_par_out = code_enlev_demande_par,
			@va_site_creation_out   = site_creation,
			@va_nb_bebes_out	= nb_bebes,
			@va_nb_agees_out	= nb_agees
		from EXP..FMC_VEH, EXP..FMC_GEN, CFG..TYP_FMC
		where	EXP..FMC_GEN.numero = @va_NumEvt_in and EXP..FMC_GEN.cle = @va_CleEvt_in and 
			CFG..TYP_FMC.numero = EXP..FMC_GEN.type and CFG..TYP_FMC.classe = XZAEC_ClasseVehicule and
			EXP..FMC_VEH.numero = EXP..FMC_GEN.numero and EXP..FMC_VEH.cle = EXP..FMC_GEN.cle

		select 
			matieres_dangereuses			
		from EXP..FMC_VEH, EXP..FMC_GEN, CFG..TYP_FMC
		where	EXP..FMC_GEN.numero = @va_NumEvt_in and EXP..FMC_GEN.cle = @va_CleEvt_in and 
			CFG..TYP_FMC.numero = EXP..FMC_GEN.type and CFG..TYP_FMC.classe = XZAEC_ClasseVehicule and
			EXP..FMC_VEH.numero = EXP..FMC_GEN.numero and EXP..FMC_VEH.cle = EXP..FMC_GEN.cle

#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

	select 
		@va_Type_out		= type_de_vehicule,
		@va_Passagers_ou	= nombre_passagers,
		@va_Bebe_out		= bebe,
		@va_PersAgee_out	= personne_agee,
		@va_Animaux_out		= animaux_vivants,
		@va_TypeVehEscota_out	= vehicule_ESCOTA,
		@va_Chargement_out	= volume_de_chargement,
		@va_CodeMat_out		= matieres_dangereuses,
		@va_Depannage_out	= depannage,
		@va_SiteDepannage_out	= site_depannage,
		@va_Texte_out		= commentaire,
		@va_numveh_out		= numero_vehicule,
		@va_marque_out          = marque,
		@va_type_out            = EXP..FMC_VEH.type,
		@va_couleur_out         = couleur,
		@va_immatriculation_out = immatriculation,
		@va_code_pays_out       = code_pays,
		@va_pays_out            = pays,
		@va_nom_out             = EXP..FMC_VEH.nom,
		@va_ville_out           = ville,
		@va_adresse_out         = adresse,
		@va_code_danger_out     = code_danger,
		@va_code_nature_panne_out = code_nature_panne,
		@va_nature_panne_out    = nature_panne,
		@va_positionnement_out  = positionnement,
		@va_vehicule_roulant_out = vehicule_roulant,
		@va_enlev_dem_par_out   = enlevement_demande_par,
		@va_numero_fiche_appel_out = numero_fiche_appel,
		@va_pres_mat_dangereuse_out = presence_matiere_dangereuse,
		@va_fax_envoye_out      = fax_envoye,
		@va_code_marque_out     = code_marque,
		@va_code_couleur_out    = code_couleur,
		@va_code_enlev_dem_par_out = code_enlev_demande_par,
		@va_site_creation_out   = site_creation,
		@va_nb_bebes_out	= nb_bebes,
		@va_nb_agees_out	= nb_agees
		from EXP..FMC_VEH, EXP..FMC_GEN, CFG..TYP_FMC
	where	EXP..FMC_GEN.numero = @va_NumEvt_in and EXP..FMC_GEN.cle = @va_CleEvt_in and 
		CFG..TYP_FMC.numero = EXP..FMC_GEN.type and CFG..TYP_FMC.classe = XZAEC_ClasseVehicule and
		EXP..FMC_VEH.numero = EXP..FMC_GEN.numero and EXP..FMC_VEH.cle = EXP..FMC_GEN.cle
		
	select 
		matieres_dangereuses
		from EXP..FMC_VEH, EXP..FMC_GEN, CFG..TYP_FMC
	where	EXP..FMC_GEN.numero = @va_NumEvt_in and EXP..FMC_GEN.cle = @va_CleEvt_in and 
		CFG..TYP_FMC.numero = EXP..FMC_GEN.type and CFG..TYP_FMC.classe = XZAEC_ClasseVehicule and
		EXP..FMC_VEH.numero = EXP..FMC_GEN.numero and EXP..FMC_VEH.cle = EXP..FMC_GEN.cle
#endif
#endif

	/*A test si complement vehicule a ete trouve */
	if @@rowcount = 0
		return XDC_NOK

	return XDC_OK
go
