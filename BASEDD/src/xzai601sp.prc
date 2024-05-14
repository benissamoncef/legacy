/*E*/
/* Fichier : $Id: xzai601sp.prc,v 1.1 2018/09/05 09:04:06 devgfi Exp $        $Revision: 1.1 $        $Date: 2018/09/05 09:04:06 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* FICHIER xzai601sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Tracage des ouvertures et fermeture des FMC.
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	30/08/18 : Creation (DEM 1300)  1.1
-----------------------------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------------------------------
* Service rendu
*
* Enregistre dans la table dediee les horodates d'ouverture et fermeture
* d'une FMC avec le site et le poste de l'opération.
*
* -----------------------------------------------------------------------------
* Sequence d'appel
* SP	XZAI601_Tracer_Ouverture_FMC
*
* Arguments en entree
* XDY_Entier		va_NumFmc_in
* XDY_District		va_CleFmc_in
* XDY_Horodate		va_Heure_Ouverture_in
* XDY_Horodate		va_Heure_Fermeture_in
* XDY_Eqt		va_Poste_in
*
* Arguments en sortie
*  Aucun
*
* Code retour
* XDC_OK
* XDC_NOK
* <0 erreur sybase
*
* Conditions d'utilisation
*
-----------------------------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAI601' and type = 'P')
	drop procedure XZAI601
go


create procedure XZAI601
	@va_NumFmc_in		int,
	@va_CleFmc_in		tinyint,
	@va_Heure_Ouverture_in	datetime,
	@va_Heure_Fermeture_in	datetime,
	@va_Poste_in		T_EQUIPEMENT
as
	declare	@vl_SiteLocal	T_SITE

	/*A Transformation des parametres d'entree nulls */
	if @va_Heure_Ouverture_in = XDC_DATE_NULLE	select @va_Heure_Ouverture_in = null
	if @va_Heure_Fermeture_in = XDC_DATE_NULLE	select @va_Heure_Fermeture_in = null


	/*A Controle des parametres d'entree obligatoire */
	if @va_Heure_Ouverture_in = null
		return XDC_ARG_INV


	/*A Rechercher le site local */
	select @vl_SiteLocal = numero
	from CFG..RES_DIS
	where serveur = @@servername

	if @vl_SiteLocal is null
		return XDC_NOK


#ifndef HIST
	if not exists (select 1 from EXP..FMC_GEN where numero = @va_NumFmc_in  and  cle = @va_CleFmc_in)
		return XDC_ARG_INV

	if not exists (select 1 from CFG..EQT_GEN where type = XDC_EQT_MAC  and  numero = @va_Poste_in)
		return XDC_ARG_INV


	/*A Mémoriser la consultation de la fiche sur le poste */
	insert	EXP..FMC_TRA (numero_fmc, cle_fmc, heure_ouverture, heure_fermeture, sit, poste)
	values	(@va_NumFmc_in, @va_CleFmc_in, @va_Heure_Ouverture_in, @va_Heure_Fermeture_in, @vl_SiteLocal, @va_Poste_in)
#endif

	if @@rowcount <> 1
		return XDC_NOK

	return XDC_OK
go
