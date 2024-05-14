/*E*/
/*  Fichier : $Id: xzat62sp.prc,v 1.4 2021/02/12 16:43:21 pc2dpdy Exp $        $Revision: 1.4 $        $Date: 2021/02/12 16:43:21 $
-------------------------------------------------------------------------------------------
* ESCOTA * PROJET MIGRAZUR
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* FICHIER xzat62sp.prc
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Supprime la mise au neutre programmee d'un PMV donne.
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	05/03/13 : Creation (DEM 1061)  1.1
* JPL	12/03/13 : Ajout critere de date de debut  1.2
* JPL	12/12/18 : Ajout du type de jour en parametre et critere (DEM 1312)  1.3
* LCL	22/01/22 : MOVIS PRA DEM-SAE93 1.4
* GGY	26/09/23 : Ajout district CA (DEM483)
-----------------------------------------------------------------------------------------*/


/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU
*
*  Supprime la mise au neutre programmee d'un PMV donne dans la table PRG_PMV.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL
*
* SP	XZAT62_Supprimer_Neutre_PMV
*
* PARAMETRES EN ENTREE :
*
* XDY_Eqt	va_NumEqt_in		: Numero de l'equipement PMV
* XDY_Site	va_Site_in		: Numero du site
* XDY_Horodate	va_HorodateDebut_in	: Horodate de debut de la programmation a supprimer
* XDY_Octet	va_Type_Jour		: Type de periode de la programmation a supprimer
* XDY_Octet	va_Forcer_in		: Indicateur forcant la suppression
*
*
* PARAMETRES EN SORTIE :
*
* Aucun
*
* CODE RETOUR :
*
* XDC_OK
* XDC_OBJ_AUCUN     : Pas de programmation pour ce PMV a cette date
* XZAEC_FMC_NON_FIN : FMC attachee non terminee
* XDC_NOK           : PMV inexistant, incoherence de site
* XDC_ARG_INV       : parametre d'entree obligatoire manquant (Null) ou invalide
* <0                : erreur sybase
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
*  Supprime la mise au neutre programmee pour le PMV indique dans la table PRG_PMV.
*  La FMC attachee a cette programmation doit etre terminee et achevee,
*  sauf si l'indicateur 'Forcer' est a VRAI.
*
-----------------------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAT62' and type = 'P')
	drop procedure XZAT62
go


create procedure XZAT62
	@va_NumEqt_in		T_EQUIPEMENT	= null,
	@va_Site_in		T_SITE		= null,
	@va_HorodateDebut_in	datetime	= null,
	@va_Type_Jour		tinyint		= null,
	@va_Forcer_in		tinyint		= null,
	@va_SiteLocal_in	T_SITE		= null
as
	declare	@vl_Site	T_SITE,
		@vl_NumEvt	int,
		@vl_CleEvt	T_DISTRICT,
		@vl_Fin		datetime,
		@vl_Cloture	datetime,
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
	if @va_HorodateDebut_in = XDC_DATE_NULLE
		select @va_HorodateDebut_in = null


	/*A Verifier la validite des parametres d'entree */
	if (@va_NumEqt_in is null) or (@va_Site_in is null) or (@va_HorodateDebut_in is null)
		return XDC_ARG_INV


	/*A Verifier l'existence de l'equipement et la concordance du site */
	if not exists (select nom from CFG..EQT_GEN
	                 where type = XDC_EQT_PMV and
	                 numero = @va_NumEqt_in and
	                 site_gestion = @va_Site_in)
		return XDC_NOK

	if (@vl_Site <> @vl_SiteMaitre and  @va_Site_in <> @vl_Site)
		return XDC_NOK


	/*A Rechercher la programmation de mise au neutre du PMV et la FMC associee */
	select	@vl_NumEvt = evenement,
		@vl_CleEvt = cle
	from	EXP..PRG_PMV
	where	numero = @va_NumEqt_in
	  and	type_jour = @va_Type_Jour
	  and	@va_HorodateDebut_in between debut and fin

	if @@rowcount = 0
		return XDC_OBJ_AUCUN


	/*A Verifier que la FMC associee n'est plus en cours */
	select	@vl_Fin = fin,
		@vl_Cloture = cloture
	from	EXP..FMC_GEN
	where	numero = @vl_NumEvt  and  cle = @vl_CleEvt

	if @@rowcount <> 0
	begin
		if (@vl_Cloture is null)  and  (@va_Forcer_in <> XDC_VRAI)
			return XZAEC_FMC_NON_FIN
	end


	/*A Supprimer la mise au neutre programmee du PMV */
	delete	EXP..PRG_PMV
	where	numero = @va_NumEqt_in
	  and	type_jour = @va_Type_Jour
	  and	@va_HorodateDebut_in between debut and fin

	if @@rowcount = 0
		return XDC_NOK

	return XDC_OK
go
