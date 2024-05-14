/*E*/
/*  Fichier : $Id: xzae96sp.prc,v 1.8 2017/03/09 16:28:30 pc2dpdy Exp $     Release : $Revision: 1.8 $      Date : $Date: 2017/03/09 16:28:30 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae96sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* P.V.  01/08/96	: Creation (DEM76)
* JMG	04/12/96	: plus de numero automatique de fiche vehicule (DEM/76) 1.2-1.3
* P.N.  17/03/1997	: ajout du forcage du sit au site de creation s'il est nul (dem/1407) 1.4
* LCL   09/07/07        : Suppression DY 1.5
* JMG           02/10/09        : secto DEM887 1.6
* JMG		22/09/11	 : appel client 1.7 DEM 1000
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.8
* ABK   26/09/23 	: Ajout district CA (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Modifier une fiche de vehicule relative a un evenement existant
* 
* Sequence d'appel
* SP	XZAE96_Ecrire_Fiche_Vehicule_Site
* 
* Arguments en entree
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* XDY_Octet		va_Type_in
* XDY_Octet		va_Passagers_in
* XDY_Booleen		va_Bebe_in
* XDY_Booleen		va_PersAgee_in
* XDY_Booleen		va_Animaux_in
* XDY_Booleen		va_VehEscota_in
* XDY_Octet		va_Chargement_in
* char(6)		va_CodeMat_in
* XDY_Entier		va_Depannage_in
* XDY_Site		va_SiteDepannage_in
* CHAR(100)		va_Texte_in
* XDY_Mot		va_NumeroVehicule_in
* XDY_Eqt		va_PosteOperateur_in
* char(25)              va_marque_in
* char(25)              va_type_in
* char(25)              va_couleur_in
* char(25)              va_immatriculation_in
* XDY_Mot               va_code_pays_in
* char(25)              va_pays_in
* char(25)              va_nom_in
* char(25)              va_ville_in
* char(75)              va_adresse_in
* char(4)               va_code_danger_in
* XDY_Mot               va_code_nature_panne_in
* char(25)              va_nature_panne_in
* char(25)              va_positionnement_in
* XDY_Octet             va_vehicule_roulant_in
* char(25)              va_enlev_dem_par_in
* XDY_Entier            va_numero_fiche_appel_in
* XDY_Octet             va_pres_mat_dangereuse_in
* XDY_Octet             va_fax_envoye_in
* XDY_Mot               va_code_marque_in
* XDY_Mot               va_code_couleur_in
* XDY_Mot               va_code_enlev_demande_par_in
* XDY_Entier            va_sit_in
* XDY_Octet             va_site_creation_in
* XDY_Mot		va_nb_bebes_in
* XDY_Mot		va_nb_agees_in
* XDY_Mot		va_flag_association_in
* XDY_Entier		va_id_ava_in
* XDY_Booleen		va_dangerosite_in

*
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve, update impossible,
*                 poste enrichisseur different
* XDC_ARG_INV	: parametres d'entree null
* 
* Conditions d'utilisation
* va_NumeroVehicule_in doit etre positionne a 0 pour une FMC
* de type vehicule
* pour un accident, positionner a null pour inserer un 
* vehicule dans la liste des vehicules impliques dans 
* l'accident ou preciser le numero du vehicule dont on
* desire modifier les valeurs
*
* Suite a un ajout de colonnes dans FMC_VEH les champs vehicule_roulant,
* presence_matiere_dangereuse et fax_envoye n'ont pu etre codes en booleens.
* Des controles sont donc necessaires dans le code appelant cette procedure pour que les
* valeurs de ces champs soient 0 ou 1 uniquement.
*
* Fonction
* Modifier dans la table VEHICULE
---------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE96' and type = 'P')
	drop procedure XZAE96
go

create procedure XZAE96
	@va_NumEvenement_in	int	= null,
	@va_CleEvenement_in	tinyint	= null,
	@va_Type_in		tinyint = null,
	@va_Passagers_in	tinyint = null,
	@va_Bebe_in		bit ,
	@va_PersAgee_in		bit ,
	@va_Animaux_in		bit ,
	@va_VehEscota_in	bit ,
	@va_Chargement_in	tinyint = null,
	@va_CodeMat_in		char(6) = null,
	@va_Depannage_in	int = null,
	@va_SiteDepannage_in	T_SITE = null,
	@va_Texte_in		char(100) = null,
	@va_NumeroVehicule_in	smallint = null,
	@va_PosteOperateur_in	T_EQUIPEMENT	= null,
	@va_marque_in           char(25) = null,
	@va_type_in             char(25) = null,
	@va_couleur_in          char(25) = null,
	@va_immatriculation_in  char(25) = null,
	@va_code_pays_in        smallint = null,
	@va_pays_in             char(25) = null,
	@va_nom_in              char(25) = null,
	@va_ville_in            char(25) = null,
	@va_adresse_in          char(75) = null,
	@va_code_danger_in      char(4)  = null,
	@va_code_nature_panne_in  smallint = null,
	@va_nature_panne_in     char(25) = null,
	@va_positionnement_in   char(25) = null,
	@va_vehicule_roulant_in  tinyint = null,
	@va_enlev_dem_par_in    char(25) = null,
	@va_numero_fiche_appel_in    int = null,
	@va_pres_mat_dangereuse_in  tinyint = null,
	@va_fax_envoye_in       tinyint = null,
	@va_code_marque_in      smallint = null,
	@va_code_couleur_in     smallint = null,
	@va_code_enlev_dem_par_in smallint = null,
	@va_sit_in 		int = null,
	@va_site_creation_in    tinyint = null,
	@va_nb_bebes_in	       smallint = null,
	@va_nb_agees_in        smallint = null,
	@va_flag_association_in tinyint = null,
	@va_id_ava_in			char(50) = null,
	@va_dangerosite_in		int = null


as
	declare @vl_NumVehicule smallint, @vl_NumFicheAppel int, @vl_code_appel int, @vl_date_raz datetime

	/*! transfo en param�tres nulls bug applix */
	if @va_NumEvenement_in = 0 select @va_NumEvenement_in = null
	if @va_CleEvenement_in = 0 select @va_CleEvenement_in = null
	if @va_Type_in = 0 select @va_Type_in = null
	if @va_sit_in = 0 select @va_sit_in = null
	if @va_Depannage_in = 0 select @va_Depannage_in = null
	if @va_SiteDepannage_in = 0 select @va_SiteDepannage_in = null
	if @va_PosteOperateur_in = 0 select @va_PosteOperateur_in = null

	/* Suppression Nom et Adresse */
	select @va_nom_in = null
	select @va_adresse_in = null


	/*A controle parametres en entree */
	if @va_NumEvenement_in  = null or @va_CleEvenement_in = null or
	   @va_PosteOperateur_in = null 
		return XDC_ARG_INV

	/*A controle les chaines null */
	if @va_CodeMat_in = null
		select @va_CodeMat_in = XDC_CHAINE_VIDE
	if @va_Texte_in = null
		select @va_Texte_in = XDC_CHAINE_VIDE


	/*si la fiche doit etre associee a l'action depanneur*/
	select @va_flag_association_in
	if @va_flag_association_in = XDC_VRAI
	begin
		select @va_site_creation_in
		/*A recherche du code d'appel suivant le site */
		if @va_site_creation_in = XDC_CI
			select @vl_code_appel = XDC_APPEL_CI
		else if @va_site_creation_in = XDC_VC
			select @vl_code_appel = XDC_APPEL_VC
		else if @va_site_creation_in = XDC_DP
			select @vl_code_appel = XDC_APPEL_DP
		else if @va_site_creation_in = XDC_CA
			select @vl_code_appel = XDC_APPEL_CA
		select @vl_code_appel

		/*A SI le site est null alors on le force au site de creation */
		if @va_sit_in = null
			select @va_sit_in = @va_site_creation_in

		/*A teste si le compteur doit etre reinitialise */
		select @vl_date_raz = date_raz
		from EXP..LST_CPT
		where code=@vl_code_appel
		if (getdate() >= @vl_date_raz)
		begin
		  	update EXP..LST_CPT set
				   	valeur=0,
				   	date_raz=dateadd(year, 1,@vl_date_raz)
		  	where code=@vl_code_appel
		end
	
		/*A recherche du numero de fiche d'appel */
		select @vl_NumFicheAppel=valeur+1
		from EXP..LST_CPT
		where code=@vl_code_appel

		/*A mise a jour du numero de fiche d'appel */
		update EXP..LST_CPT set valeur=@vl_NumFicheAppel where code=@vl_code_appel
	end
	else
		select @vl_NumFicheAppel=@va_numero_fiche_appel_in

	select @vl_NumFicheAppel

	/*A mise a jour du numero de fiche d'appel */
	if @va_NumeroVehicule_in != null and 
	   @va_NumeroVehicule_in != XZAEC_VEH_INCONNU
	begin
		/*A verifier l'existance de l'evenement et la concordance */
		/*A du poste enrichisseur                                 */
		if not exists ( select * from EXP..FMC_GEN , CFG..TYP_FMC
				where	EXP..FMC_GEN.numero = @va_NumEvenement_in and 
					EXP..FMC_GEN.cle = @va_CleEvenement_in and
					CFG..TYP_FMC.numero = EXP..FMC_GEN.type and
					(CFG..TYP_FMC.classe = XZAEC_ClasseVehicule or
					CFG..TYP_FMC.classe = XZAEC_ClasseAccident))
			return XDC_NOK

		
		/*A modification de la fiche vehicule */
		update EXP..FMC_VEH set 
			depannage = @va_Depannage_in,
			site_depannage = @va_SiteDepannage_in,
			type_de_vehicule = @va_Type_in,
			nombre_passagers = @va_Passagers_in,
			bebe = @va_Bebe_in,
			personne_agee = @va_PersAgee_in,
			animaux_vivants = @va_Animaux_in,
			vehicule_ESCOTA = @va_VehEscota_in,
			volume_de_chargement = @va_Chargement_in,
			matieres_dangereuses = @va_CodeMat_in,
			commentaire = @va_Texte_in,
			zz = zz + 1,
			marque = @va_marque_in,
			type = @va_type_in,
			couleur = @va_couleur_in,
			immatriculation = @va_immatriculation_in,
			code_pays = @va_code_pays_in,
			pays = @va_pays_in,
			nom = @va_nom_in,
			ville = @va_ville_in,
			adresse = @va_adresse_in,
			code_danger = @va_code_danger_in,
			code_nature_panne = @va_code_nature_panne_in,
			nature_panne = @va_nature_panne_in,
			positionnement = @va_positionnement_in,
			vehicule_roulant = @va_vehicule_roulant_in,
			enlevement_demande_par = @va_enlev_dem_par_in,
			numero_fiche_appel = @vl_NumFicheAppel,
			presence_matiere_dangereuse = @va_pres_mat_dangereuse_in,
			fax_envoye = @va_fax_envoye_in,
			code_marque = @va_code_marque_in,
			code_couleur = @va_code_couleur_in,
			code_enlev_demande_par = @va_code_enlev_dem_par_in,
			sit = @va_sit_in,
			site_creation =  @va_site_creation_in,
			nb_bebes = @va_nb_bebes_in,
			nb_agees = @va_nb_agees_in,
			id_ava = @va_id_ava_in,
			dangerosite = @va_dangerosite_in
		where	numero = @va_NumEvenement_in and 
			cle = @va_CleEvenement_in and
			numero_vehicule = @va_NumeroVehicule_in
		
	end
	else
	begin
		/*A verifier l'existance de l'evenement et la concordance */
		/*A du poste enrichisseur                                 */
		if not exists ( select * from EXP..FMC_GEN, CFG..TYP_FMC
				where	EXP..FMC_GEN.numero = @va_NumEvenement_in and 
					EXP..FMC_GEN.cle = @va_CleEvenement_in and
					CFG..TYP_FMC.numero = EXP..FMC_GEN.type and
					CFG..TYP_FMC.classe = XZAEC_ClasseAccident)
			return XDC_NOK

		/*A recherche du numero de vehicule */
		select @vl_NumVehicule = isnull(max(numero_vehicule), 0) + 1 
		from EXP..FMC_VEH
		where	numero = @va_NumEvenement_in and
			cle = @va_CleEvenement_in

		/*A insertion de la fiche vehicule  dans le cas d'un accident */
		insert EXP..FMC_VEH (numero, cle, numero_vehicule, type_de_vehicule,
		nombre_passagers, bebe, personne_agee, animaux_vivants, 
		vehicule_ESCOTA, volume_de_chargement, matieres_dangereuses, 
		commentaire, sit, zz, marque, type, couleur, immatriculation, code_pays, 
		pays, nom, ville, adresse, code_danger, code_nature_panne, nature_panne, 
		positionnement, vehicule_roulant, enlevement_demande_par, numero_fiche_appel,
		presence_matiere_dangereuse, fax_envoye, code_marque, code_couleur, 
		code_enlev_demande_par,depannage,site_depannage,site_creation,nb_bebes,
		nb_agees,id_ava,dangerosite)
		values (@va_NumEvenement_in,  @va_CleEvenement_in, @vl_NumVehicule, 
		@va_Type_in, @va_Passagers_in, @va_Bebe_in, @va_PersAgee_in, 
		@va_Animaux_in, @va_VehEscota_in, @va_Chargement_in,  
		@va_CodeMat_in, @va_Texte_in, @va_sit_in, 1, 
		@va_marque_in, @va_type_in, @va_couleur_in,
		@va_immatriculation_in, @va_code_pays_in, 
		@va_pays_in, @va_nom_in, @va_ville_in, 
		@va_adresse_in, @va_code_danger_in, 
		@va_code_nature_panne_in, @va_nature_panne_in, 
		@va_positionnement_in, @va_vehicule_roulant_in, @va_enlev_dem_par_in, 
		@vl_NumFicheAppel, @va_pres_mat_dangereuse_in, @va_fax_envoye_in,
		@va_code_marque_in, @va_code_couleur_in, 
		@va_code_enlev_dem_par_in,@va_Depannage_in,
		@va_SiteDepannage_in,@va_site_creation_in,
		@va_nb_bebes_in, @va_nb_agees_in,		
		@va_id_ava_in,
		@va_dangerosite_in)

	end

	if @@rowcount != 1
		return XDC_NOK

	return XDC_OK
go