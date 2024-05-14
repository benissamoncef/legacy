/*E*/
/*  Fichier : $Id: xzae54sp.prc,v 1.18 2020/12/15 09:15:57 pc2dpdy Exp $        Release : $Revision: 1.18 $        Date : $Date: 2020/12/15 09:15:57 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae54sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	04/10/94	: Creation	(V 1.1)
* C.T.	10/11/94	: Modif bugs applix (V 1.2) 
* C.T.	24/11/94	: Correction test existance evenement
*                         non teste (V 1.4 V 1.5)
* B.G.	21/12/94	: Modif test enrichisseur (1.6) 
* C.T.	09/02/95        : Modif creation : zz value (V 1.7)
* P.V.  15/07/96	: Ajout colonnes a FMC_VEH (DEM76)
* JMG	04/12/96	: plus de numero automatique de fiche d'appel (DEM/76) 1.9
* JMG	01/08/98	: gestion multi-sites (dem/1536) 1.10
* PN	25/05/98	: Mise a jour uniquement des champs du tampon de  la FMC (dem/1423) 1.11
* JPL	25/02/09	: Degroupee; ajout du champ 'feu maitrise' (DEM 845) 1.12
* JPL	18/08/09	: Ecriture champ 'presence matiere dangereuse' selon le code matiere (pour DEM 549) 1.13
* JMg	25/11/10 : ajout horodate maitrise 1.14
* LCL	18/07/11	: ajout rappel client 1.15 DEM 1000
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
* LCL	16/11/20	: MOVIS correction site origine DEM-SAE93
* LCL	14/11/20	: MOVIS Correction sit dans FMC_VEH en PRA DEM-SAE93
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Modifier une fiche de vehicule relative a un evenement existant
* 
* Sequence d'appel
* SP	XZAE54_Ecrire_Fiche_Vehicule
* 
* Arguments en entree
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* XDY_Octet		va_Type_in
* XDY_Octet		va_Passagers_in
* XDY_Booleen		va_Animaux_in
* XDY_Booleen		va_VehEscota_in
* XDY_Octet		va_Chargement_in
* char(6)		va_CodeMat_in
* CHAR(100)		va_Texte_in
* XDY_Mot		va_NumeroVehicule_in
* XDY_Octet             va_site_creation_in
* XDY_Mot		va_nb_bebes_in
* XDY_Mot		va_nb_agees_in
* XDY_Octet             va_feu_maitrise_in
* tinyint		va_type_rappel_in
* char(20)		va_complement_rappel_in
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
* Suite a un ajout de colonnes dans FMC_VEH, les champs vehicule_roulant,
* presence_matiere_dangereuse et fax_envoye n'ont pu etre codes en booleens.
* Des controles sont donc necessaires dans le code appelant cette procedure pour que les
* valeurs de ces champs soient 0 ou 1 uniquement.
*
* Fonction
* Modifier dans la table VEHICULE
---------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE54' and type = 'P')
	drop procedure XZAE54
go


create procedure XZAE54
	@va_NumEvenement_in	int	= null,
	@va_CleEvenement_in	tinyint	= null,
	@va_Type_in		tinyint = null,
	@va_Passagers_in	tinyint = null,
	@va_Animaux_in		bit ,
	@va_VehEscota_in	bit ,
	@va_Chargement_in	tinyint = null,
	@va_CodeMat_in		char(6) = null,
	@va_Texte_in		char(100) = null,
        @va_NumeroVehicule_in   smallint = null,
	@va_site_creation_in       tinyint = null,
	@va_nb_bebes_in         smallint = null,
	@va_nb_agees_in         smallint = null,
	@va_feu_maitrise_in     tinyint = null,
	@va_type_rappel_in		tinyint = null,
	@va_complement_rappel_in	char(20) = null
as
	declare @vl_Site T_SITE, @vl_NumVehicule smallint, @vl_NumFicheAppel smallint, @vl_code_appel int, @vl_date_raz datetime,
		@vl_typeSite tinyint,
		@vl_presence_mat_dang tinyint,
		@vl_horodate datetime,
		@vl_horodate_rappel datetime,
		@vl_type_rappel tinyint

	/*! transfo en param�tres nulls bug applix */
	if @va_NumEvenement_in = 0 select @va_NumEvenement_in = null
	if @va_CleEvenement_in = 0 select @va_CleEvenement_in = null
	if @va_Type_in = 0 select @va_Type_in = null

	/*A controle parametres en entree */
	if @va_NumEvenement_in  = null or @va_CleEvenement_in = null 
		return XDC_ARG_INV

	/*A controle les chaines null */
	if @va_CodeMat_in = null
		select @va_CodeMat_in = XDC_CHAINE_VIDE
	if @va_Texte_in = null
		select @va_Texte_in = XDC_CHAINE_VIDE


	/*A recherche du site local */
	/*
	select @vl_Site = numero, @vl_typeSite = type
	from CFG..RES_DIS
	where numero = @va_site_creation_in
	*/
	select @vl_Site = sit 
	from EXP..FMC_GEN
	where	numero = @va_NumEvenement_in and 
		cle = @va_CleEvenement_in 
	/*
	where serveur = @@servername
	*/


	/* MOVIS */
	if @vl_Site = null
		select @vl_Site=@va_CleEvenement_in

	if @vl_typeSite = XDC_TYPEM_PCS
		return XDC_NOK


	/*A calcul des donnees dependant des arguments */
	if @va_CodeMat_in = XDC_CHAINE_VIDE
		select @vl_presence_mat_dang = XDC_FAUX
	else
		select @vl_presence_mat_dang = XDC_VRAI

	select @vl_horodate=null

	if @va_feu_maitrise_in = XDC_VRAI
	begin
		select @vl_horodate = horodate_maitrise
		from EXP..FMC_VEH
		where numero = @va_NumEvenement_in and cle = @va_CleEvenement_in
		and numero_vehicule = @va_NumeroVehicule_in

		if @vl_horodate = null
			select @vl_horodate = getdate()
	end
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

		/*A recherche horodate rappel */
		select @vl_type_rappel = type_rappel,
			   @vl_horodate_rappel = horodate_rappel
		from EXP..FMC_VEH
		where	numero = @va_NumEvenement_in and 
			cle = @va_CleEvenement_in and
			numero_vehicule = @va_NumeroVehicule_in
			
		if @vl_type_rappel != @va_type_rappel_in
			select @vl_horodate_rappel = getdate()

		/*A modification de la fiche vehicule */
		update EXP..FMC_VEH set 
			type_de_vehicule = @va_Type_in,
			nombre_passagers = @va_Passagers_in,
			animaux_vivants = @va_Animaux_in,
			vehicule_ESCOTA = @va_VehEscota_in,
			volume_de_chargement = @va_Chargement_in,
			matieres_dangereuses = @va_CodeMat_in,
			commentaire = @va_Texte_in,
			zz = zz + 1,
			sit = @vl_Site,
			presence_matiere_dangereuse = @vl_presence_mat_dang,
			site_creation = @va_site_creation_in,
			nb_bebes = @va_nb_bebes_in,
			nb_agees = @va_nb_agees_in,
			feu_maitrise = @va_feu_maitrise_in,
			horodate_maitrise = @vl_horodate,
			type_rappel = @va_type_rappel_in,
			complement_rappel = @va_complement_rappel_in,
			horodate_rappel = @vl_horodate_rappel
		where	numero = @va_NumEvenement_in and 
			cle = @va_CleEvenement_in and
			numero_vehicule = @va_NumeroVehicule_in
		
	end

	if @@rowcount != 1
		return XDC_NOK

	return XDC_OK
go
