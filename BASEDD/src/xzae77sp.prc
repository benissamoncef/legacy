/*E*/
/*  Fichier : $Id: xzae77sp.prc,v 1.3 2020/11/03 17:01:40 pc2dpdy Exp $        Release : $Revision: 1.3 $        Date : $Date: 2020/11/03 17:01:40 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET MIGRAZUR / PASTRE
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* FICHIER xzae77sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Lecture / ecriture des fiches Main courante
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	18/10/12 : Creation (DEM 1032)  1.1
* JPL	31/10/12 : Verification de la classe seulement et non du type  1.2
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
-----------------------------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU
*
*  Ecrit en base les données de Conditions de Conduite Hivernale
*  attachees à une Fmc de type Meteo existante.
*
* SEQUENCE D'APPEL
*
* SP	XZAE77_Ecrire_Conditions_Conduite
*
* Arguments en entree :
*
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* XDY_Horodate		va_Horodate_in
* XDY_Octet		va_NumZone_in		: rang dans la liste
* XDY_PR		va_PRDebut_in
* XDY_PR		va_PRFin_in
* XDY_Octet		va_cch_VR_in		:     conditions
* XDY_Octet		va_cch_VM2_in		:   de circulation
* XDY_Octet		va_cch_VM1_in		:     sur voies
* XDY_Octet		va_cch_VL_in		:     circulees
* XDY_Octet		va_cch_BAU_in		:
* XDY_Octet		va_cch_VR_I_in		:     conditions
* XDY_Octet		va_cch_VM2_I_in		:   de circulation
* XDY_Octet		va_cch_VM1_I_in		:     sur voies
* XDY_Octet		va_cch_VL_I_in		:      inverses
* XDY_Octet		va_cch_BAU_I_in		:
* XDY_Octet		va_Precipitations_in	: indic. precipitations en cours
* XDY_Eqt		va_PosteOperateur_in
* XDY_Octet		va_Previson_Traiter_Clos_in
*
* Arguments en sortie : Aucun
*
*
* Code retour :
*
* XDC_OK
* XDC_NOK		: evenement inconnu ou de type incorrect pour l'operation
* XDC_ARG_INV		: parametre d'entree obligatoire manquant (Null) ou invalide
* XZAEC_FMC_PAS_ENR	: poste enrichisseur different
* XZAEC_FMC_ETAT_INC	: Fmc en etat inadapte (en prevision)
* XZAEC_FMC_PAS_DONNEES	: les donnees sont inadaptees a celles de la Fmc (type de perturbation)
* <0			: erreur sybase
*
* Conditions d'utilisation :
*
* La date passee en parametre d'entree doit etre ensuite passee
* a la procedure de validation de la fiche main courante (XZAE11)
*
* Fonction :
*
*  Selon l'etat de la Fmc :
*   Prevision	: pas de saisie de conditions avant le debut d'evenement
*   Traiter	: insertion dans la table FMC_MET_CCH
*   Clos	: modification des donnees a la date indiquee.
*
-----------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE77' and type = 'P')
	drop procedure XZAE77
go


create procedure XZAE77
	@va_NumEvenement_in		int		= null,
	@va_CleEvenement_in		tinyint		= null,
	@va_Horodate_in			datetime	= null,
	@va_NumZone_in			tinyint		= null,
	@va_PRDebut_in			T_PR		= null,
	@va_PRFin_in			T_PR		= null,
	@va_cch_VR_in			tinyint		= null,
	@va_cch_VM2_in			tinyint		= null,
	@va_cch_VM1_in			tinyint		= null,
	@va_cch_VL_in			tinyint		= null,
	@va_cch_BAU_in			tinyint		= null,
	@va_cch_VR_I_in			tinyint		= null,
	@va_cch_VM2_I_in		tinyint		= null,
	@va_cch_VM1_I_in		tinyint		= null,
	@va_cch_VL_I_in			tinyint		= null,
	@va_cch_BAU_I_in		tinyint		= null,
	@va_Precipitations_in		tinyint		= null,
	@va_PosteOperateur_in		T_EQUIPEMENT,
	@va_Prevision_Traiter_Clos_in	tinyint		= null,
	@va_NomSiteLocal_in             char(2)         = null
as
	declare	@vl_Site		T_SITE,
		@vl_typeSite		tinyint,
		@vl_SiteFMC		T_SITE,
		@vl_posteFMC		T_EQUIPEMENT,
		@vl_typePerturbation	tinyint


	/*! Transformation des parametres d'entree nuls */
	if @va_NumEvenement_in = 0		select @va_NumEvenement_in = null
	if @va_CleEvenement_in = 0		select @va_CleEvenement_in = null
	if @va_Horodate_in = XDC_DATE_NULLE	select @va_Horodate_in = null
	if @va_PRDebut_in = XDC_PR_INCONNU	select @va_PRDebut_in = null
	if @va_PRFin_in = XDC_PR_INCONNU	select @va_PRFin_in = null
	if @va_Prevision_Traiter_Clos_in = 0	select @va_Prevision_Traiter_Clos_in = null
	if @va_PosteOperateur_in = 0		select @va_PosteOperateur_in = null


	/*A Verifier la validite des parametres d'entree */
	if @va_NumEvenement_in = null or @va_CleEvenement_in = null or
	   @va_Horodate_in = null or
	   @va_PosteOperateur_in = null
		return XDC_ARG_INV

	if @va_Prevision_Traiter_Clos_in not in (XDC_FMC_ETAT_PREV, XDC_FMC_ETAT_TRAI, XDC_FMC_ETAT_CLOS)
		return XDC_ARG_INV

	if @va_NumZone_in <= 0 or
	   @va_PRDebut_in < 0  or  @va_PRFin_in < 0
		return XDC_ARG_INV

	if @va_Precipitations_in not in (XDC_VRAI, XDC_FAUX)
		return XDC_ARG_INV


	/*A Recherche du site local */
        /* MOVIS */
        if @va_NomSiteLocal_in = null
                select @vl_Site = numero, @vl_typeSite = type
                from CFG..RES_DIS
                where serveur = @@servername
        else
                select @vl_Site = numero, @vl_typeSite = type
                from CFG..RES_DIS
                where code = @va_NomSiteLocal_in

	if @vl_typeSite = XDC_TYPEM_PCS
		return XDC_NOK


	/*A Verifier l'existance de l'evenement et le droit d'enrichir */
	select @vl_SiteFMC = sit, @vl_posteFMC = poste_enrichisseur
	from EXP..FMC_GEN G, CFG..TYP_FMC T
	where	G.numero = @va_NumEvenement_in and G.cle = @va_CleEvenement_in and
		T.numero = G.type and
		T.classe = XZAEC_ClasseMeteo

	if @@rowcount = 0
		return XDC_NOK

	if @vl_SiteFMC <> @vl_Site or
	   (@vl_posteFMC <> null  and  @vl_posteFMC <> @va_PosteOperateur_in)
		return XZAEC_FMC_PAS_ENR


	select @vl_SiteFMC = sit, @vl_typePerturbation = type_de_perturbation
	from EXP..FMC_MET M
	where	M.numero = @va_NumEvenement_in and M.cle = @va_CleEvenement_in and
		M.horodate = @va_Horodate_in

	if @@rowcount = 0
		return XDC_NOK

	if @vl_SiteFMC <> @vl_Site  and  @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_CLOS
		return XZAEC_FMC_CLOSE_PAS_ENR

	/* Les conditions de circulation sont specifiques a certains types de perturbation */
	if @vl_typePerturbation not in (
		XZAEC_MET_ChuteNeige,
		XZAEC_MET_NeigeChaus,
		XZAEC_MET_Grele,
		XZAEC_MET_GreleChaus,
		XZAEC_MET_Verglas
	)
		return XZAEC_FMC_PAS_DONNEES


	/* On ne peut saisir les conditions actuelles d'un evenement non debute */
	if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_PREV
		return XZAEC_FMC_ETAT_INC


	if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_TRAI
		/*A insertion d'une fiche a traiter */
		insert EXP..FMC_MET_CCH (numero, cle, horodate_validation,
		                         PR_debut, PR_fin,
		                         cch_VR, cch_VM2, cch_VM1, cch_VL, cch_BAU,
		                         cch_VR_I, cch_VM2_I, cch_VM1_I, cch_VL_I, cch_BAU_I,
		                         precipitations, sit)
		values (@va_NumEvenement_in, @va_CleEvenement_in, @va_Horodate_in,
		        @va_PRDebut_in, @va_PRFin_in,
		        @va_cch_VR_in, @va_cch_VM2_in, @va_cch_VM1_in, @va_cch_VL_in, @va_cch_BAU_in,
		        @va_cch_VR_I_in, @va_cch_VM2_I_in, @va_cch_VM1_I_in, @va_cch_VL_I_in, @va_cch_BAU_I_in,
		        @va_Precipitations_in, @vl_Site)
	else if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_CLOS
	begin
		/*A modification de la fiche pour cloturer */

		/*! si des conditions de conduite ont ete saisies a cette date pour cette Fmc, */
		/*! les supprimer auparavant, c'est a dire avant l'ecriture de la premiere zone */
		if @va_NumZone_in = 1
		begin
			delete	EXP..FMC_MET_CCH
			where	numero = @va_NumEvenement_in and cle = @va_CleEvenement_in and
				horodate_validation = @va_Horodate_in
		end

		insert EXP..FMC_MET_CCH (numero, cle, horodate_validation,
		                            PR_debut, PR_fin,
		                            cch_VR, cch_VM2, cch_VM1, cch_VL, cch_BAU,
		                            cch_VR_I, cch_VM2_I, cch_VM1_I, cch_VL_I, cch_BAU_I,
		                            precipitations, sit)
		values (@va_NumEvenement_in, @va_CleEvenement_in, @va_Horodate_in,
		           @va_PRDebut_in, @va_PRFin_in,
		           @va_cch_VR_in, @va_cch_VM2_in, @va_cch_VM1_in, @va_cch_VL_in, @va_cch_BAU_in,
		           @va_cch_VR_I_in, @va_cch_VM2_I_in, @va_cch_VM1_I_in, @va_cch_VL_I_in, @va_cch_BAU_I_in,
		           @va_Precipitations_in, @vl_Site)
	end

	if @@rowcount != 1
		return XDC_NOK

	return XDC_OK
go
