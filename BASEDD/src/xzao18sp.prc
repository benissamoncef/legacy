/*E*/
/*  Fichier : $Id: xzao18sp.prc,v 1.2 2011/01/21 12:34:24 gesconf Exp $        Release : $Revision: 1.2 $        Date : $Date: 2011/01/21 12:34:24 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET MIGRAZUR PASTRE
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* MODULE XZAO * FICHIER xzao18sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Liste des cameras
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	27/10/10 : Creation (DEM 949)  1.1
* JPL	20/01/11 : Ajout parametres Code et Autoroute, retour localisation (DEM 949)  1.2
-----------------------------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU
*
*  Retourne la liste des informations d'une ou plusieurs cameras.
*
*  La liste est retournee dans l'ordre des numeros de camera croissants.
*
* -----------------------------------------------------------------------------
* SEQUENCE D'APPEL
*
* SP	XZAO18_Liste_Cameras
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in		: Nom de la base ou chercher
* XDY_Mot	va_NumEqtMin_in		: numero de camera minimal
* XDY_Mot	va_NumEqtMax_in		: numero de camera maximal
* char(5)	va_CodeCamera_in	: code de la camera recherchee
* XDY_Octet	va_Autoroute_in		: numero de l'autoroute
*
* PARAMETRES EN SORTIE :
*
* Aucun
*
* VALEUR RENDUE :
*
* Identifiants, localisation et donnees specifiques de chaque camera.
*
* CODE RETOUR :
*
* XDC_OK
* XDC_NOK           : 
* XDC_ARG_INV       : parametre d'entree obligatoire manquant (Null) ou invalide
* <0                : erreur sybase
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
*  Liste les cameras dont le numero est dans l'intervalle defini
*  par les bornes minimale et maximale indiquees.
*
*  Une borne NULLe n'impose aucune limite.
*
-----------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO18' and type = 'P')
	drop procedure XZAO18
go


create procedure XZAO18
	@va_Basedd_in		char(3)		= null,
	@va_NumEqtMin_in	T_EQUIPEMENT	= null,
	@va_NumEqtMax_in	T_EQUIPEMENT	= null,
	@va_CodeCamera_in	char(5)		= null,
	@va_Autoroute_in	T_AUTOROUTE	= null
as

declare
	@vl_Resultat		int

	/*A Transformation des parametres d'entree nulls (bug applix) */
	if @va_Basedd_in = XDC_CHAINE_VIDE	select @va_Basedd_in = null
	if @va_NumEqtMin_in = 0			select @va_NumEqtMin_in = null
	if @va_NumEqtMax_in = 0			select @va_NumEqtMax_in = null
	if @va_CodeCamera_in = XDC_CHAINE_VIDE	select @va_CodeCamera_in = null
	if @va_Autoroute_in = 0			select @va_Autoroute_in = null


	/*A
	** Verifier la validite des parametres d'entree obligatoires
	*/

	if @va_Basedd_in = null
		return XDC_ARG_INV


	/*A
	** Travail dans la base CFG
	*/

	if @va_Basedd_in = XDC_BASE_CFG
	begin

		/*A
		** Recherche de la liste des cameras
		*/

		select
			C.numero,
			C.code_camera,
			G.nom,
			G.autoroute,
			G.PR,
			G.sens,
			C.IP,
			C.protocole,
			C.port,
			C.encoder

		from CFG..EQT_CAM C, CFG..EQT_GEN G

		where (@va_NumEqtMin_in is null  or  C.numero >= @va_NumEqtMin_in)
		  and (@va_NumEqtMax_in is null  or  C.numero <= @va_NumEqtMax_in)

		  and (@va_CodeCamera_in is null  or  C.code_camera = @va_CodeCamera_in)

		  and G.type = XDC_EQT_CAM  and  G.numero = C.numero
		  and (@va_Autoroute_in is null  or  G.autoroute = @va_Autoroute_in)

		order by C.numero
	end

#ifdef CI

	else

	/*A
	** Travail dans la base CFT
	*/

	begin

		/*A
		** Recherche de la liste des cameras
		*/

		select
			C.numero,
			C.code_camera,
			G.nom,
			G.autoroute,
			G.PR,
			G.sens,
			C.IP,
			C.protocole,
			C.port,
			C.encoder

		from CFT..EQT_CAM C, CFT..EQT_GEN G

		where (@va_NumEqtMin_in is null  or  C.numero >= @va_NumEqtMin_in)
		  and (@va_NumEqtMax_in is null  or  C.numero <= @va_NumEqtMax_in)

		  and (@va_CodeCamera_in is null  or  C.code_camera = @va_CodeCamera_in)

		  and G.type = XDC_EQT_CAM  and  G.numero = C.numero
		  and (@va_Autoroute_in is null  or  G.autoroute = @va_Autoroute_in)

		order by C.numero
	end

#endif

	return XDC_OK
go
