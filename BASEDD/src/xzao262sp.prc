/*E*/
/* Fichier : $Id: xzao262sp.prc,v 1.2 2020/10/21 12:25:00 pc2dpdy Exp $        Release : $Revision: 1.2 $        Date : $Date: 2020/10/21 12:25:00 $
--------------------------------------------------------------------------------
* ESCOTA *  PROJET MIGRAZUR
--------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
--------------------------------------------------------------------------------
* MODULE XZAO * FICHIER xzao262.prc
--------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
--------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL		04/01/17 : Creation  (DEM 1266)  1.1
* PNI		21/10/20 : Ajout colonne version_radt SAE-182
* GGY		26/09/23 : Ajout district CA (DEM483)
* GGY		17/10/23 : Correction si CFG vide (DEM483)
----------------------------------------------------------------------------- */

/*X-----------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Ajout d'un point de mesure DIT et du point RADT correspondant dans la base
*
--------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO262' and type = 'P')
	drop procedure XZAO262
go


create procedure XZAO262
	@va_Basedd_in		char(3),
	@va_Nom_DIT_in		char(10),
	@va_Station_RADT_in	char(10),
	@va_Sens_RADT_in	T_SENS,
	@va_Resultat_out	int	output
as

/*
*
* SP	XZAO262SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_NomEqt		va_Nom_DIT_in		Nom du point de mesure DIT
* XDY_NomEqt		va_Station_RADT_in	Nom de la station RADT representant ce point de mesure
* XDY_Sens		va_Sens_RADT_in		Sens des mesures RADT representant les donnees a fournir a MI2
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out
*
* VALEUR RENDUE :
*
* Ajoute un point de mesure DIT en base
*
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
*
* CONDITIONS D'UTILISATION :
*
* Config OFF LINE
*
* FONCTION :
*
* Insertion ou modification dans la table RES_DIT
*
* MECANISMES :
*
------------------------------------------------------------------------------*/

declare
	@vl_Num_RADT	T_EQUIPEMENT,
	@vl_Site		char(10)


	select @va_Resultat_out = XDC_ECHEC

	/* Transformation des parametres d'entree nulls */
	if @va_Nom_DIT_in = XDC_CHAINE_VIDE		select @va_Nom_DIT_in = null
	if @va_Station_RADT_in = XDC_CHAINE_VIDE	select @va_Station_RADT_in = null


	/*A Verifier la validite des parametres d'entree */
	if (@va_Basedd_in <> XDC_BASE_CFG  and  @va_Basedd_in <> XDC_BASE_CFT)
		return XDC_ARG_INV

	if (@va_Nom_DIT_in is null  or  @va_Station_RADT_in is null)
		return XDC_ARG_INV

	if @va_Sens_RADT_in not in (
		XDC_SENS_NORD,
		XDC_SENS_SUD
	   )
		return XDC_ARG_INV


		/* Verification que le site est bien celui du CI ou CA */
		select  @vl_Site = @@servername
	   
		/* Si le site est CI ou CA */
		if @vl_Site <> XDC_SQLCI and @vl_Site <> XDC_SQLCA
			return XDC_OK


	/*A
	**  Travail dans la base CFG
	*/

	if @va_Basedd_in = XDC_BASE_CFG
	begin

		/*A
		** Verification de l'existence de l'equipement RADT
		*/

		select	@vl_Num_RADT = numero from CFG..EQT_GEN
		where	type = XDC_EQT_RAD  and  nom = @va_Station_RADT_in

		if @vl_Num_RADT is null
		begin
			select @va_Resultat_out = XDC_ECHEC
			return XDC_NOK
		end


		/*A
		** Verification du recueil de mesures par cette station dans ce sens
		*/

		if not exists (select 1 from CFG..RES_CHA where station = @vl_Num_RADT and sens = @va_Sens_RADT_in)
		begin
			select @va_Resultat_out = XDC_ECHEC
			return XDC_NOK
		end


		/*A
		** S'il n'y a pas de point de mesure DIT de ce nom
		** alors inserer ce point de mesure et les donnees RADT associees
		*/

		if not exists (select 1 from CFG..RES_DIT where nom_pt_mesure = @va_Nom_DIT_in)
		begin
			select @va_Resultat_out = XDC_AJOUT

			insert	CFG..RES_DIT ( nom_pt_mesure, numero_radt, sens_radt,version_radt )
			values ( @va_Nom_DIT_in,
				 @vl_Num_RADT,
				 @va_Sens_RADT_in,"1.0"	)

			if @@rowcount <> 1
				return XDC_NOK
		end

		else

		/*A
		** Si le point de mesure DIT existe alors
		** modifier les donnees RADT associees
		*/

		begin
			select @va_Resultat_out = XDC_MODIF

			update	CFG..RES_DIT
			set	numero_radt = @vl_Num_RADT,
				sens_radt = @va_Sens_RADT_in
			where	nom_pt_mesure = @va_Nom_DIT_in

			if @@rowcount <> 1
				return XDC_NOK
		end
	end

#ifdef CI

	else

	/*A
	**  Travail dans la base CFT
	*/

	begin

		/*A
		** Verification de l'existence de l'equipement RADT
		*/

		select	@vl_Num_RADT = numero from CFT..EQT_GEN
		where	type = XDC_EQT_RAD  and  nom = @va_Station_RADT_in

		if @vl_Num_RADT is null
		begin
			select @va_Resultat_out = XDC_ECHEC
			return XDC_NOK
		end


		/*A
		** Verification du recueil de mesures par cette station dans ce sens
		*/

		if not exists (select 1 from CFT..RES_CHA where station = @vl_Num_RADT and sens = @va_Sens_RADT_in)
		begin
			select @va_Resultat_out = XDC_ECHEC
			return XDC_NOK
		end


		/*A
		** S'il n'y a pas de point de mesure DIT de ce nom
		** alors inserer ce point de mesure et les donnees RADT associees
		*/

		if not exists (select 1 from CFT..RES_DIT where nom_pt_mesure = @va_Nom_DIT_in)
		begin
			select @va_Resultat_out = XDC_AJOUT

			insert	CFT..RES_DIT ( nom_pt_mesure, numero_radt, sens_radt ,version_radt)
			values ( @va_Nom_DIT_in,
				 @vl_Num_RADT,
				 @va_Sens_RADT_in ,"1.0")

			if @@rowcount <> 1
				return XDC_NOK
		end

		else

		/*A
		** Si le point de mesure DIT existe alors
		** modifier les donnees RADT associees
		*/

		begin
			select @va_Resultat_out = XDC_MODIF

			update	CFT..RES_DIT
			set	numero_radt = @vl_Num_RADT,
				sens_radt = @va_Sens_RADT_in
			where	nom_pt_mesure = @va_Nom_DIT_in

			if @@rowcount <> 1
				return XDC_NOK
		end
	end

#endif

	return XDC_OK

go
