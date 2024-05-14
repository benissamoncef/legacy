/*E*/
/*  Fichier : $Id: xzat61sp.prc,v 1.6 2021/03/08 13:15:10 pc2dpdy Exp $        $Revision: 1.6 $        $Date: 2021/03/08 13:15:10 $
-------------------------------------------------------------------------------------------
* ESCOTA * PROJET MIGRAZUR
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* FICHIER xzat61sp.prc
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Ajout d'une mise au neutre programmee pour un PMV donne.
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	05/03/13 : Creation (DEM 1061)  1.1
* JPL	12/03/13 : Ajout critere de date de debut  1.2
* JPL	24/07/17 : Support de l'indicateur de mode clignotant / alternat (DEM 1230)  1.3
* JPL	14/12/18 : Ajout du type de periode de programmation (DEM 1312)  1.4
* JPL	03/04/19 : Autoriser des programmations de meme type dans une periode, dans des tranches horaires distinctes  1.5
* LCL	22/01/21 : MOVIS PRA DEM-SAE93 1.6
* GGY	28/04/23 : Augmentation taille va_Bandeau_in a char(XDC_LGR_TXT_PICTO) et va_Pictogramme_ib (XDC_LGR_NOM_PICTO) (DEM-473)
* GGY	26/09/23 : Ajout district CA (DEM483)
-----------------------------------------------------------------------------------------*/


/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU
*
*  Ajoute une ligne de mise au neutre a la table PRG_PMV
*  pour un PMV donne, s'il n'en existe aucune.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL
*
* SP	XZAT61_Programmer_Neutre_PMV
*
* PARAMETRES EN ENTREE :
*
* XDY_Eqt	va_NumEqt_in		: Numero du PMV
* XDY_District	va_Site_in		: Numero du site
* XDY_Horodate	va_HorodateDebut_in	: Horodate de debut de programmation
* XDY_Horodate	va_HorodateFin_in	: Horodate de fin de programmation
* XDY_Octet	va_Type_Jour_in		: Type de periode (discontinue) de programmation
* XDY_Booleen	va_Discontinu_in	: Indicateur de discontinuite de la periode
* XDY_Entier	va_NumEvt_in		: Numero de la FMC attachee
* XDY_District	va_CleEvt_in		: Cle de la FMC
* XDY_Ligne_PMV	va_Ligne1_in		: \
* XDY_Ligne_PMV	va_Ligne2_in		:  > Message a afficher
* XDY_Ligne_PMV	va_Ligne3_in		: /
* XDY_Ligne_PMV	va_Ligne1_alternat_in	: \
* XDY_Ligne_PMV	va_Ligne2_alternat_in	:  > Message alternat
* XDY_Ligne_PMV	va_Ligne3_alternat_in	: /
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
* XDC_OBJ_EXISTE    : Une programmation existe pour ce PMV dans cette periode
* XDC_NOK           
* XDC_ARG_INV       : parametre d'entree obligatoire manquant (Null) ou invalide
* <0                : erreur sybase
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
*  Recherche une mise au neutre programmee pour le PMV indique dans la table PRG_PMV.
*  S'il n'en existe aucune, ajoute la programmation definie.
*
-----------------------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAT61' and type = 'P')
	drop procedure XZAT61
go


create procedure XZAT61
	@va_NumEqt_in			T_EQUIPEMENT	= null,
	@va_Site_in			T_SITE		= null,
	@va_HorodateDebut_in		datetime	= null,
	@va_HorodateFin_in		datetime	= null,
	@va_Type_Jour_in		tinyint		= null,
	@va_Discontinu_in		bit		,
	@va_NumEvt_in			int		= null,
	@va_CleEvt_in			T_DISTRICT	= null,
	@va_Ligne1_in			T_LIGNE_PMV	= null,
	@va_Ligne2_in			T_LIGNE_PMV	= null,
	@va_Ligne3_in			T_LIGNE_PMV	= null,
	@va_Ligne1_alternat_in		T_LIGNE_PMV	= null,
	@va_Ligne2_alternat_in		T_LIGNE_PMV	= null,
	@va_Ligne3_alternat_in		T_LIGNE_PMV	= null,
	@va_Pictogramme_in		char(XDC_LGR_NOM_PICTO)		= null,
	@va_Bandeau_in			char(XDC_LGR_TXT_PICTO)		= null,
	@va_Mode_Affichage_in		tinyint		= null,
	@va_SiteLocal_in		T_SITE,
	@va_Flash_in			bit
as
	declare	@vl_Site		T_SITE,
		@vl_HeureDebut		smallint,
		@vl_HeureFin		smallint, 
		@vl_SiteMaitre tinyint

	/* Recuperation du site maitre */
	select  @vl_SiteMaitre = numero
		from CFG..RES_DIS
			where type = XDC_TYPEM_SD

	/* Determiner le site local */
	if @va_SiteLocal_in is null
		select	@vl_Site = numero
		from	CFG..RES_DIS
		where	serveur = @@servername
	else
		select @vl_Site = @va_SiteLocal_in

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

	select @vl_HeureDebut = (datepart (hour, @va_HorodateDebut_in) * 60) + datepart (minute, @va_HorodateDebut_in)
	select @vl_HeureFin   = (datepart (hour, @va_HorodateFin_in) * 60) + datepart (minute, @va_HorodateFin_in)


	if @va_Type_Jour_in not in (select code from CFG..LIB_PRE where notyp = XDC_LIB_TYPE_JOUR)
		return XDC_ARG_INV

	/*A En mode discontinu, l'heure de debut doit preceder l'heure de fin */
	if @va_Discontinu_in = XDC_VRAI
	begin
		if @vl_HeureDebut >= @vl_HeureFin
			return XDC_ARG_INV
	end
	else
		/*A En mode continu, la programmation concerne forcement tous les jours */
		select @va_Type_Jour_in = XDC_JOUR_TOUS

	if @va_Mode_Affichage_in not in (XDC_PMV_MODE_NORMAL, XDC_PMV_MODE_CLIGNO, XDC_PMV_MODE_ALTERNE)
		return XDC_ARG_INV


	/*A Verifier l'existence de l'equipement et la concordance du site */
	if not exists (select nom from CFG..EQT_GEN
	                 where type = XDC_EQT_PMV and
	                 numero = @va_NumEqt_in and
	                 site_gestion = @va_Site_in)
		return XDC_NOK

	if (@vl_Site <> @vl_SiteMaitre and  @va_Site_in <> @vl_Site)
		return XDC_NOK


	/*A Verifier l'existence de la FMC */
	if not exists (select numero from EXP..FMC_GEN
	                  where numero = @va_NumEvt_in  and  cle = @va_CleEvt_in)
		return XZAEC_FMC_ETAT_INC


	/*A Verifier qu'aucune programmation de mise au neutre n'existe */
	/*A  pour ce PMV, dans une periode de meme type non distincte   */
	if exists (select numero from EXP..PRG_PMV
		    where numero = @va_NumEqt_in
		      and ( debut <= @va_HorodateFin_in  and  fin >= @va_HorodateDebut_in )
		      and ( (@va_Discontinu_in = XDC_FAUX  and  discontinu = XDC_FAUX)  or
		            ( ( (@va_Discontinu_in = XDC_VRAI  and  discontinu = XDC_VRAI  and  type_jour = @va_Type_Jour_in)  or
		                (@va_Discontinu_in = XDC_FAUX  and  discontinu = XDC_VRAI  and  type_jour = XDC_JOUR_TOUS)  or
		                (@va_Discontinu_in = XDC_VRAI  and  @va_Type_Jour_in = XDC_JOUR_TOUS  and  discontinu = XDC_FAUX)
		              )  and
		              ( (datepart (hour, debut) * 60) + datepart (minute, debut) <= @vl_HeureFin )  and
		              ( (datepart (hour, fin) * 60) + datepart (minute, fin) >= @vl_HeureDebut )
		            )
		          )
		)
		return XDC_OBJ_EXISTE


	/*A Ajouter la mise au neutre programmee definie pour ce PMV */
	insert EXP..PRG_PMV (
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
		clignotement,
		type_jour,
		discontinu,
		flash
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
		@va_Mode_Affichage_in,
		@va_Type_Jour_in,
		@va_Discontinu_in,
		@va_Flash_in
	)

	if @@rowcount <> 1
		return XDC_NOK

	return XDC_OK
go
