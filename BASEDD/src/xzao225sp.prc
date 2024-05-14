/*E*/
/*Fichier :  $Id: xzao225sp.prc,v 1.2 2008/02/13 13:23:47 devgtie Exp $      Release : $Revision: 1.2 $        Date : $Date: 2008/02/13 13:23:47 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET PASTRE
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* FICHIER xzao225sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Lecture et ecriture de la configuration du reseau d'autoroutes
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	20/06/07 : Creation 1.1
-----------------------------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU
*
*  Liste des cantons d'un Troncon, d'une portion ou de toutes les autoroutes
*
* -----------------------------------------------------------------------------
* SEQUENCE D'APPEL
*
* SP	XZAO225_Liste_Cantons_Par_Portion
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in			
* XDY_Nom	va_NomTroncon_in
* XDY_Mot	va_NumeroPortion_in
*
* PARAMETRES EN SORTIE :
*
* Aucun
*
* VALEUR RENDUE :
*
*  Liste des cantons d'un Troncon ou d'une portion
*    ou de l'ensemble des autoroutes
*
* CODE RETOUR :
*
* XDC_OK
* XDC_NOK     : parametre(s) incorrect(s)
* XDC_ARG_INV : parametre d'entree obligatoire Null
* <0          : erreur sybase
*
* CONDITIONS D'UTILISATION :
*
* Si NomTroncon et NumeroPortion sont nuls alors retourne
*  la liste des cantons de l'ensemble des autoroutes ;
*
* Si NomTroncon n'est pas nul alors retourne
*  la liste des cantons du troncon ;
*
* Si NumeroPortion n'est pas nul alors retourne
*  la liste des cantons de la portion.
*
* FONCTION :
*
* Retourne  XZAOT_Canton
*
-----------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO225' and type = 'P')
	drop procedure XZAO225
go


create procedure XZAO225
	@va_Basedd_in		char(3),
	@va_NomTroncon_in	char(25),
	@va_NumeroPortion_in	smallint
as

declare
	@vl_Autoroute	T_AUTOROUTE,
	@vl_PRdebut	T_PR,
	@vl_PRfin	T_PR


	/*A Bug applix null */
	if rtrim ( @va_Basedd_in ) = null	select @va_Basedd_in = null
	if rtrim ( @va_NomTroncon_in ) = null	select @va_NomTroncon_in = null
	if @va_NumeroPortion_in = 0	 	select @va_NumeroPortion_in = null


	/*A Controle des parametres d'entree obligatoires */
	if @va_Basedd_in = null
		return XDC_ARG_INV


	/*A
	**  Travail dans la base CFG
	*/   

	if @va_Basedd_in = XDC_BASE_CFG
	begin
		/*A
		** Si NomTroncon et NumeroPortion sont nuls
		** Recherche des Cantons de l'ensemble des Autotoutes
		*/

		if @va_NomTroncon_in = null and @va_NumeroPortion_in = null
			select POR.autoroute, CAN.PR_debut, CAN.sens
			  from CFG..RES_CAN CAN, CFG..RES_POR POR
			 where CAN.portion = POR.numero
			 order by POR.autoroute, CAN.PR_debut, CAN.sens

		/*A
		** Si NumeroPortion n'est pas nul
		** Recherche des Cantons de la Portion
		*/

		else if @va_NomTroncon_in = null and @va_NumeroPortion_in <> null
			select POR.autoroute, CAN.PR_debut, CAN.sens
			  from CFG..RES_CAN CAN, CFG..RES_POR POR
			 where CAN.portion = @va_NumeroPortion_in
			   and CAN.portion = POR.numero
			 order by POR.autoroute, CAN.PR_debut, CAN.sens

		/*A
		** Si NomTroncon n'est pas nul
		** Recherche des Cantons du Troncon
		*/

		else if @va_NomTroncon_in <> null and @va_NumeroPortion_in = null
		begin
			select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin
			from CFG..RES_TRC
			where nom = @va_NomTroncon_in

			select POR.autoroute, CAN.PR_debut, CAN.sens
			  from CFG..RES_CAN CAN, CFG..RES_POR POR
			 where POR.autoroute = @vl_Autoroute
			   and POR.PR_debut <= @vl_PRfin
			   and POR.PR_fin   >= @vl_PRdebut
			   and CAN.portion   = POR.numero
			order by POR.autoroute, CAN.PR_debut, CAN.sens
		end

		else
			/*B parametres incorrects */
			return XDC_NOK
	end

#ifdef CI

	/*A
	**  Travail dans la base CFT
	*/   

	else if @va_Basedd_in = XDC_BASE_CFT
	begin
		/*A
		** Si NomTroncon et NumeroPortion sont nuls
		** Recherche des Cantons de l'ensemble des Autotoutes
		*/

		if @va_NomTroncon_in = null and @va_NumeroPortion_in = null
			select POR.autoroute, CAN.PR_debut, CAN.sens
			  from CFT..RES_CAN CAN, CFT..RES_POR POR
			 where CAN.portion = POR.numero
			 order by POR.autoroute, CAN.PR_debut, CAN.sens

		/*A
		** Si NumeroPortion n'est pas nul
		** Recherche des Cantons de la Portion
		*/

		else if @va_NomTroncon_in = null and @va_NumeroPortion_in <> null
			select POR.autoroute, CAN.PR_debut, CAN.sens
			  from CFT..RES_CAN CAN, CFT..RES_POR POR
			 where CAN.portion = @va_NumeroPortion_in
			   and CAN.portion = POR.numero
			 order by POR.autoroute, CAN.PR_debut, CAN.sens

		/*A
		** Si NomTroncon n'est pas nul
		** Recherche des Cantons du Troncon
		*/

		else if @va_NomTroncon_in <> null and @va_NumeroPortion_in = null
		begin
			select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin
			from CFT..RES_TRC
			where nom = @va_NomTroncon_in

			select POR.autoroute, CAN.PR_debut, CAN.sens
			  from CFT..RES_CAN CAN, CFT..RES_POR POR
			 where POR.autoroute = @vl_Autoroute
			   and POR.PR_debut <= @vl_PRfin
			   and POR.PR_fin   >= @vl_PRdebut
			   and CAN.portion   = POR.numero
			order by POR.autoroute, CAN.PR_debut, CAN.sens
		end

		else
			/*B parametres incorrects */
			return XDC_NOK
	end
#endif         

	else
		/*B nom base errone */
		return XDC_NOK

	return XDC_OK
go
