/*E*/
/*  Fichier : $Id: xzat71sp.prc,v 1.2 2018/04/05 10:16:25 devgfi Exp $        $Revision: 1.2 $        $Date: 2018/04/05 10:16:25 $
-------------------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* FICHIER xzat71sp.prc
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Ajout d'une mise au neutre programmee pour un PMV SAGA donne.
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	26/03/18 : Creation (DEM 1278)  1.1
* JPL	30/03/18 : Longueur des lignes de texte a 18 caracteres  1.2
-----------------------------------------------------------------------------------------*/


/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU
*
*  Ajoute une ligne de mise au neutre a la table PRG_PMV_SAGA
*  pour un PMV SAGA donne, s'il n'en existe aucune.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL
*
* SP	XZAT71_Programmer_Neutre_PMV_SAGA
*
* PARAMETRES EN ENTREE :
*
* XDY_Eqt	va_NumEqt_in		: Numero du PMV SAGA
* XDY_District	va_Site_in		: Numero du site
* XDY_Horodate	va_HorodateDebut_in	: Horodate de debut de programmation
* XDY_Horodate	va_HorodateFin_in	: Horodate de fin de programmation
* XDY_Entier	va_NumEvt_in		: Numero de la FMC attachee
* XDY_District	va_CleEvt_in		: Cle de la FMC
* char(18)	va_Ligne1_in		: \
* char(18)	va_Ligne2_in		:  > Message a afficher
* char(18)	va_Ligne3_in		: /
* char(18)	va_Ligne1_alternat_in	: \
* char(18)	va_Ligne2_alternat_in	:  > Message alternat
* char(18)	va_Ligne3_alternat_in	: /
* XDY_Picto	va_Pictogramme_in	: Code du pictogramme a afficher
* XDY_TxtPicto	va_Bandeau_in		: Texte du bandeau
* XDY_Octet	va_Mode_Affichage_in	: Mode : Normal, Clignotant ou Alterne
*
*
* PARAMETRES EN SORTIE :
*
* Aucun
*
* CODE RETOUR :
*
* XDC_OK
* XZAEC_FMC_ETAT_INC : la FMC n'existe pas ou n'est pas dans l'etat requis
* XDC_OBJ_EXISTE    : Une programmation existe pour ce PMV SAGA dans cette periode
* XDC_NOK           
* XDC_ARG_INV       : parametre d'entree obligatoire manquant (Null) ou invalide
* <0                : erreur sybase
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
*  Recherche une mise au neutre programmee pour le PMV SAGA indique dans la table PRG_PMV_SAGA.
*  S'il n'en existe aucune, ajoute la programmation definie.
*
-----------------------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAT71' and type = 'P')
	drop procedure XZAT71
go


create procedure XZAT71
	@va_NumEqt_in			T_EQUIPEMENT	= null,
	@va_Site_in			T_SITE		= null,
	@va_HorodateDebut_in		datetime	= null,
	@va_HorodateFin_in		datetime	= null,
	@va_NumEvt_in			int		= null,
	@va_CleEvt_in			T_DISTRICT	= null,
	@va_Ligne1_in			char(18)	= null,
	@va_Ligne2_in			char(18)	= null,
	@va_Ligne3_in			char(18)	= null,
	@va_Ligne1_alternat_in		char(18)	= null,
	@va_Ligne2_alternat_in		char(18)	= null,
	@va_Ligne3_alternat_in		char(18)	= null,
	@va_Pictogramme_in		char(4)		= null,
	@va_Bandeau_in			char(6)		= null,
	@va_Mode_Affichage_in		tinyint		= null
as
	declare	@vl_Site		T_SITE,
			@vl_SiteMaitre	tinyint

	/* Recuperation du site maitre */
	select  @vl_SiteMaitre = numero
		from CFG..RES_DIS
			where type = XDC_TYPEM_SD

	/* Determiner le site local */
	select	@vl_Site = numero
	from	CFG..RES_DIS
	where	serveur = @@servername

	if @vl_Site is null
		return XDC_NOK


	/* Transformation des parametres d'entree nulls */
	if @va_NumEqt_in = 0	select @va_NumEqt_in = null
	if @va_Site_in = 0	select @va_Site_in = null
	if @va_NumEvt_in = 0	select @va_NumEvt_in = null
	if @va_CleEvt_in = 0	select @va_CleEvt_in = null

	if @va_HorodateDebut_in = XDC_DATE_NULLE	select @va_HorodateDebut_in = null
	if @va_HorodateFin_in = XDC_DATE_NULLE		select @va_HorodateFin_in = null

	if @va_Ligne1_in = null
		select @va_Ligne1_in = XDC_CHAINE_VIDE
	if @va_Ligne2_in = null
		select @va_Ligne2_in = XDC_CHAINE_VIDE
	if @va_Ligne3_in = null
		select @va_Ligne3_in = XDC_CHAINE_VIDE

	if @va_Ligne1_alternat_in = null
		select @va_Ligne1_alternat_in = XDC_CHAINE_VIDE
	if @va_Ligne2_alternat_in = null
		select @va_Ligne2_alternat_in = XDC_CHAINE_VIDE
	if @va_Ligne3_alternat_in = null
		select @va_Ligne3_alternat_in = XDC_CHAINE_VIDE

	if (@va_Pictogramme_in = null)  or  (@va_Pictogramme_in = XDC_CHAINE_VIDE)
		select @va_Pictogramme_in = XDC_PICTO_PASPICTO


	/*A Verifier la validite des parametres d'entree */
	if ( (@va_NumEqt_in is null)  or  (@va_Site_in is null)  or
	     (@va_NumEvt_in is null)  or  (@va_CleEvt_in is null)  or
	     (@va_HorodateDebut_in is null)  or  (@va_HorodateFin_in is null)
	   )
		return XDC_ARG_INV

	if ( (@va_HorodateDebut_in >= @va_HorodateFin_in)  or
	     (getdate() >= @va_HorodateFin_in)
	   )
		return XDC_ARG_INV

	if @va_Mode_Affichage_in not in (XDC_PMV_MODE_NORMAL, XDC_PMV_MODE_CLIGNO, XDC_PMV_MODE_ALTERNE)
		return XDC_ARG_INV


	/*A Verifier l'existence de l'equipement et la concordance du site */
	if not exists (select nom from CFG..EQT_GEN
	                 where type = XDC_EQT_SAGA and
	                 numero = @va_NumEqt_in and
	                 site_gestion = @va_Site_in)
		return XDC_NOK

	if (@vl_Site <> @vl_SiteMaitre and @va_Site_in <> @vl_Site)
		return XDC_NOK


	/*A Verifier l'existence de la FMC */
	if not exists (select numero from EXP..FMC_GEN
	                  where numero = @va_NumEvt_in  and  cle = @va_CleEvt_in)
		return XZAEC_FMC_ETAT_INC


	/*A Verifier qu'aucune programmation de mise au neutre n'existe pour ce PMV */
	if exists (select numero from EXP..PRG_PMV_SAGA
	            where numero = @va_NumEqt_in
	              and ( debut <= @va_HorodateFin_in  and  fin >= @va_HorodateDebut_in )
		)
		return XDC_OBJ_EXISTE


	/*A Ajouter la mise au neutre programmee definie pour ce PMV */
	insert EXP..PRG_PMV_SAGA (
		numero,
		debut,
		fin,
		evenement,
		cle,
		ligne_1,
		ligne_2,
		ligne_3,
		ligne_1_alternat,
		ligne_2_alternat,
		ligne_3_alternat,
		pictogramme,
		bandeau,
		clignotement
	)
	values (
		@va_NumEqt_in,
		@va_HorodateDebut_in,
		@va_HorodateFin_in,
		@va_NumEvt_in,
		@va_CleEvt_in,
		@va_Ligne1_in,
		@va_Ligne2_in,
		@va_Ligne3_in,
		@va_Ligne1_alternat_in,
		@va_Ligne2_alternat_in,
		@va_Ligne3_alternat_in,
		@va_Pictogramme_in,
		@va_Bandeau_in,
		@va_Mode_Affichage_in
	)

	if @@rowcount <> 1
		return XDC_NOK

	return XDC_OK
go
