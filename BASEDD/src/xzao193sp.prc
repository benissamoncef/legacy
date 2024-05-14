/*E*/
/* Fichier : $Id: xzao193sp.prc,v 1.4 2018/09/06 15:16:35 devgfi Exp $        Release : $Revision: 1.4 $        Date : $Date: 2018/09/06 15:16:35 $
--------------------------------------------------------------------------------
* ESCOTA *  PROJET MIGRAZUR
--------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
--------------------------------------------------------------------------------
* MODULE XZAO * FICHIER xzao193.prc
--------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
--------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL		07/10/2010 : Creation  (DEM 948)  1.1
* LCL           11/07/18        : correction syntaxe from  1.3
* JPL		06/09/18 : Correction (inversion) test du site CI  1.4
* GGY	26/09/23 :	Ajout district CA (DEM483)
----------------------------------------------------------------------------- */

/*X-----------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Ajout d'une zone de couverture camera dans la base
*
--------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO193' and type = 'P')
	drop procedure XZAO193
go


create procedure XZAO193
	@va_Basedd_in		char(3),
	@va_Nom_in		char(10),
	@va_Autoroute_in	char(4),
	@va_Sens_in		tinyint,
	@va_PRdebut_in		int,
	@va_PRfin_in		int,
	@va_Pertinence_in	tinyint,
	@va_Resultat_out	int	output
as

/*
*
* SP	XZAO193SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_NomEqt		va_Nom_in
* XDY_Autoroute		va_Autoroute_in
* XDY_Sens		va_Sens_in
* XDY_PR		va_PRdebut_in
* XDY_PR		va_PRfin_in
* XDY_Octet		va_Pertinence_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out
*
* VALEUR RENDUE :
*
* Ajoute une zone de couverture camera en base
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
* Insertion ou modification dans la table EQT_CAM_POS (CAMERA)
*
* MECANISMES :
*
------------------------------------------------------------------------------*/

	/*A
	** Definition des variables globales
	*/

declare
	@vl_Autoroute	tinyint,
	@vl_NumCamera	smallint,
	@vl_NumZone		tinyint,
	@vl_Pertinence	tinyint,
	@vl_Site		char(10)

	select @va_Resultat_out = XDC_ECHEC

	/*A
	** Test des parametres d'entree :
	**   Si l'un d'entre eux est null alors les arguments sont invalides
	*/

	if @va_Basedd_in = null
	or @va_Nom_in = null
	or @va_Autoroute_in = null
	or @va_Sens_in = null
	or @va_PRdebut_in = null
	or @va_PRfin_in = null
	or @va_Pertinence_in = null

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
		** Verification de l'existence de l'autoroute, sinon Echec
		*/

		select	@vl_Autoroute = numero from CFG..RES_AUT
		where	nom = @va_Autoroute_in

		if @vl_Autoroute = null
		begin
			select @va_Resultat_out = XDC_ECHEC
			return XDC_OK
		end


		/*A
		** Verification de l'existence de la camera, sinon Echec
		*/

		select	@vl_NumCamera = numero from CFG..EQT_GEN
		where	type = XDC_EQT_CAM
		and	nom = @va_Nom_in

		if @vl_NumCamera = null
		begin
			select @va_Resultat_out = XDC_ECHEC
			return XDC_OK
		end


		/*A
		** Test de la couverture de la zone par la camera, dans ce sens
		*/

		select	@vl_NumZone = numero from CFG..EQT_CAM_POS
		where	camera = @vl_NumCamera
		and	autoroute = @vl_Autoroute
		and	PR_debut = @va_PRdebut_in
		and	PR_fin = @va_PRfin_in
		and	sens = @va_Sens_in

		/*A
		** S'il n'y a pas de couverture de la zone par la camera
		** alors inserer la camera et la zone dans EQT_CAM_POS
		*/

		if @vl_NumZone = null
		begin
			select @va_Resultat_out = XDC_AJOUT

			select @vl_NumZone = max (numero) from CFG..EQT_CAM_POS
			if @vl_NumZone = null  
				select @vl_NumZone = 0

			insert	CFG..EQT_CAM_POS ( camera, numero, autoroute, PR_debut, PR_fin, sens, pertinence )
			values ( @vl_NumCamera,
				 254,
				 @vl_Autoroute,
				 @va_PRdebut_in,
				 @va_PRfin_in,
				 @va_Sens_in,
				 @va_Pertinence_in )

			if @@rowcount <> 1
				return XDC_NOK
		end

		else

		/*A
		** Si la camera couvre la zone dans ce sens alors
		** modifier la valeur de pertinence dans EQT_CAM_POS
		*/

		begin
			select @va_Resultat_out = XDC_MODIF

			update	CFG..EQT_CAM_POS
			set	pertinence = @va_Pertinence_in
			where	camera = @vl_NumCamera
			and	autoroute = @vl_Autoroute
			and	PR_debut = @va_PRdebut_in
			and	PR_fin = @va_PRfin_in
			and	sens = @va_Sens_in

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
		** Verification de l'existence de l'autoroute, sinon Echec
		*/

		select	@vl_Autoroute = numero from CFT..RES_AUT
		where	nom = @va_Autoroute_in

		if @vl_Autoroute = null
		begin
			select @va_Resultat_out = XDC_ECHEC
			return XDC_OK
		end


		/*A
		** Verification de l'existence de la camera, sinon Echec
		*/

		select	@vl_NumCamera = numero from CFT..EQT_GEN
		where	type = XDC_EQT_CAM
		and	nom = @va_Nom_in

		if @vl_NumCamera = null
		begin
			select @va_Resultat_out = XDC_ECHEC
			return XDC_OK
		end


		/*A
		** Test de la couverture de la zone par la camera, dans ce sens
		*/

		select	@vl_NumZone = numero from CFT..EQT_CAM_POS
		where	camera = @vl_NumCamera
		and	autoroute = @vl_Autoroute
		and	PR_debut = @va_PRdebut_in
		and	PR_fin = @va_PRfin_in
		and	sens = @va_Sens_in

		/*A
		** S'il n'y a pas de couverture de la zone par la camera
		** alors inserer la camera et la zone dans EQT_CAM_POS
		*/

		if @vl_NumZone = null
		begin
			select @va_Resultat_out = XDC_AJOUT

			select @vl_NumZone = max (numero) from CFT..EQT_CAM_POS
			if @vl_NumZone = null  
				select @vl_NumZone = 0

			insert	CFT..EQT_CAM_POS ( camera, numero, autoroute, PR_debut, PR_fin, sens, pertinence )
			values ( @vl_NumCamera,
				 254,
				 @vl_Autoroute,
				 @va_PRdebut_in,
				 @va_PRfin_in,
				 @va_Sens_in,
				 @va_Pertinence_in )

			if @@rowcount <> 1
				return XDC_NOK
		end

		else

		/*A
		** Si la camera couvre la zone dans ce sens alors
		** modifier la valeur de pertinence dans EQT_CAM_POS
		*/

		begin
			select @va_Resultat_out = XDC_MODIF

			update	CFT..EQT_CAM_POS
			set	pertinence = @va_Pertinence_in
			where	camera = @vl_NumCamera
			and	autoroute = @vl_Autoroute
			and	PR_debut = @va_PRdebut_in
			and	PR_fin = @va_PRfin_in
			and	sens = @va_Sens_in

			if @@rowcount <> 1
				return XDC_NOK
		end
	end

#endif

	return XDC_OK

go
