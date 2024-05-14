/*E*/
/*  Fichier : $Id: xzae212sp.prc,v 1.3 2020/11/03 16:37:12 pc2dpdy Exp $        $Revision: 1.3 $        $Date: 2020/11/03 16:37:12 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET PASTRE
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* FICHIER xzae212sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Lecture / ecriture des fiches Main courante
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	30/08/12 : Creation (DEM 1037)  1.1
* JPL	05/09/19 : Ecriture du code de nature de l'incendie (DEM 1352)  1.2
* LCL   22/04/20 : MOVIS Ajout site local pilotage DEM-SAE93
-----------------------------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU
*
*  Ecriture des données spécifiques d'une fiche Main courante
*  de classe Incendie, relative à un événement existant.
*
* -----------------------------------------------------------------------------
* SEQUENCE D'APPEL
*
* SP	XZAE212_Ecrire_Fiche_Incendie
*
* Arguments en entree :
*
* XDY_Entier	va_NumEvt_in		: numero de l'evenement
* XDY_Octet	va_CleEvt_in		: cle de l'evenement
* XDY_Horodate	va_Validation_in	: horodate de validation de la la Fmc
* XDY_Octet	va_feu_maitrise_in	: indicateur de maitrise de l'incendie
* XDY_Octet	va_Code_Nature_in	: code de nature de l'incendie
* XDY_Eqt	va_PosteOperateur_in	: numero de la machine de l'operateur
* XDY_Octet	va_Prevision_Traiter_Clos_in	: etat de la Fmc
*
* Arguments en sortie :
*
*
* Code retour :
*
* XDC_OK
* XDC_NOK           : evenement inconnu, erreur a l'insertion ou la modification
* XZAEC_FMC_PAS_ENR : evenement non modifiable par cet operateur
* XDC_ARG_INV       : parametre d'entree obligatoire manquant (Null) ou invalide
* <0                : erreur sybase
*
* Conditions d'utilisation :
*  La date passee en parametre d'entree doit etre ensuite passee
*  a la procedure de validation de la fiche main courante (XZAE11)
*
*
* Fonction :
*
*  Ecrire la valeur de l'indicateur de maitrise de l'incendie.
*
*  . En mode A TRAITER :
*       Si l'indicateur vaut Faux, l'horodate de maitrise doit etre annulee ;
*       S'il vaut Vrai, elle est mise a la date de validation SI ELLE ETAIT NULLE
*                      (c'est a dire que l'indicateur DEVIENT Vrai).
*
*  . En mode CLOS :
*       Les données spécifiques n'etant pas historisées, et la date de maitrise
*       n'étant pas saisie, il est impossible de la calculer.
*       Pour permettre de la modifier a posteriori,
*          on l'annule si l'indicateur vaut Faux comme en temps réel,
*          et on la fixe à la date de validation s'il DEVIENT Vrai,
*          COMME EN TEMPS REEL, obligeant à l'ANNULER pour le définir.
*
-----------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE212' and type = 'P')
	drop procedure XZAE212
go


create procedure XZAE212
	@va_NumEvenement_in		int		= null,
	@va_CleEvenement_in		tinyint		= null,
	@va_Validation_in		datetime	= null,
	@va_feu_maitrise_in		tinyint		= null,
	@va_Code_Nature_in		tinyint		= null,
	@va_PosteOperateur_in		T_OPERATEUR	= null,
	@va_Prevision_Traiter_Clos_in	tinyint		= null,
	@va_NomSiteLocal_in             char(2)         = null
as

declare
	@vl_SiteLocal		T_SITE,
	@vl_typeSite		tinyint,
	@vl_feu_maitrise_prec	tinyint,
	@vl_date_maitrise_prec	datetime,
	@vl_date_maitrise_nouv	datetime


	/*A Transformation des parametres d'entree nulls (bug applix) */
	if @va_NumEvenement_in = 0		select @va_NumEvenement_in = null
	if @va_CleEvenement_in = 0		select @va_CleEvenement_in = null
	if @va_Validation_in = XDC_DATE_NULLE	select @va_Validation_in = null
	if @va_PosteOperateur_in = 0		select @va_PosteOperateur_in = null


	/*A Controle de validite des parametres d'entree obligatoires */
	if @va_NumEvenement_in is null or @va_CleEvenement_in is null  or
	   @va_Validation_in is null  or
	   @va_PosteOperateur_in is null  or
	   @va_feu_maitrise_in not in (XDC_VRAI, XDC_FAUX)  or
	   @va_Code_Nature_in not in (select code from CFG..LIB_PRE where notyp = XDC_LIB_TYPE_NATURE_INCENDIE)  or
	   @va_Prevision_Traiter_Clos_in not in (XDC_FMC_ETAT_PREV, XDC_FMC_ETAT_TRAI, XDC_FMC_ETAT_CLOS)
		return XDC_ARG_INV


	/*A Rechercher le site local */
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


	/*A Verifier l'existence de l'evenement et la concordance du poste enrichisseur */
	if not exists ( select * from EXP..FMC_GEN, CFG..TYP_FMC
			where	EXP..FMC_GEN.numero = @va_NumEvenement_in  and
				EXP..FMC_GEN.cle = @va_CleEvenement_in  and
				CFG..TYP_FMC.numero = EXP..FMC_GEN.type  and
				CFG..TYP_FMC.classe = XZAEC_ClasseIncendie  and
				( poste_enrichisseur = @va_PosteOperateur_in or
				  poste_enrichisseur = null )  and
				EXP..FMC_GEN.sit = @vl_SiteLocal )
		return XZAEC_FMC_PAS_ENR

	select	@vl_feu_maitrise_prec	= feu_maitrise,
		@vl_date_maitrise_prec	= horodate_maitrise
	from EXP..FMC_INC
	where	numero = @va_NumEvenement_in and cle = @va_CleEvenement_in  and
		sit = @vl_SiteLocal

	if @@rowcount != 1
		if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_CLOS
			return XZAEC_FMC_CLOSE_PAS_ENR
		else
			return XZAEC_FMC_PAS_ENR


	/* en prevision, l'incendie ne peut etre maitrise */
	if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_PREV
	begin
		if @va_feu_maitrise_in = XDC_VRAI
			return XDC_NOK
	end


	/*A Par defaut la date de maitrise est la precedente si le feu est maitrise, nulle sinon */
	select @vl_date_maitrise_nouv = null
	if (@va_feu_maitrise_in = XDC_VRAI)
	begin
		/* si l'incendie DEVIENT maitrise, la date est celle de validation */
		if (@vl_feu_maitrise_prec != XDC_VRAI)
			select @vl_date_maitrise_nouv = @va_Validation_in
		else
			select @vl_date_maitrise_nouv = @vl_date_maitrise_prec
	end


	update EXP..FMC_INC set
		feu_maitrise = @va_feu_maitrise_in,
		horodate_maitrise = @vl_date_maitrise_nouv,
		code_nature = @va_Code_Nature_in,
		zz = zz + 1
	where	numero = @va_NumEvenement_in and
		cle = @va_CleEvenement_in and
		sit = @vl_SiteLocal

	if @@rowcount != 1
		return XDC_NOK

	return XDC_OK
go
