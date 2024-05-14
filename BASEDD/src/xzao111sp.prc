/*E*/
/* Fichier : $Id: xzao111sp.prc,v 1.1 2015/08/12 23:10:29 devgfi Exp $        Release : $Revision: 1.1 $        Date : $Date: 2015/08/12 23:10:29 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET PASTRE
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* FICHIER xzao111sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Lecture / ecriture des objets statiques
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	12/08/15 : Creation (pour DEM 1138)  1.1
-----------------------------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU
*
*  Retourne une liste des machines, filtree sur leur site ou leur nom.
*
* -----------------------------------------------------------------------------
* SEQUENCE D'APPEL
*
* SP	XZAO111_Liste_Machines
*
* Arguments en entree :
*
* XDY_Basedd		va_Basedd_in	 : base de configuration
* XDY_District		va_Site_in	 : numero du site des machines recherchees
* XDY_NomMachine	va_NomMachine_in : Motif de nom de machine (optionnel)
*
* Arguments en sortie :
*
*
* Code retour :
*
* XDC_OK
* XDC_NOK           : 
* XDC_ARG_INV       : parametre d'entree obligatoire manquant (Null) ou invalide
* <0                : erreur sybase
*
* Conditions d'utilisation :
* Toujours preciser la base
*
*
* Fonction :
*
*  Select dans la base indiquee les tables EQT_GEN et EQT_MAC filtrees
*  where site et nom machine inclus par le filtre.
*
-----------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO111' and type = 'P')
	drop procedure XZAO111
go


create procedure XZAO111
	@va_Basedd_in		char(3)		= null,
	@va_Site_in		T_SITE		= null,
	@va_NomMachine_in	T_NOM_MACHINE	= null
as

	/*A Transformation des parametres d'entree nulls (bug applix) */
	if @va_Site_in = 0	select @va_Site_in = null


	/*A Controle de validite des parametres d'entree obligatoires */
	if rtrim (@va_Basedd_in) is null
		return XDC_ARG_INV


	if @va_Basedd_in = XDC_BASE_CFG
	begin

		/*A
		**  Travail dans la base CFG
		*/

		/*A Recherche des machines correspondant aux criteres */
		select	E.numero, E.nom,
			M.district
		from	CFG..EQT_GEN E,
			CFG..EQT_MAC M
		where	E.type = XDC_EQT_MAC
		  and	E.numero = M.numero
		  and	( @va_Site_in is null  or  M.district = @va_Site_in )
		  and	E.nom like rtrim (@va_NomMachine_in) ++ "%"
		order by E.nom

		return XDC_OK
	end

#ifdef CI

	else if @va_Basedd_in = XDC_BASE_CFT
	begin

		/*A
		**  Travail dans la base CFT
		*/

		/*A Recherche des machines correspondant aux criteres */
		select	E.numero,
			E.nom,
			M.district
		from	CFT..EQT_GEN E,
			CFT..EQT_MAC M
		where	E.type = XDC_EQT_MAC
		  and	E.numero = M.numero
		  and	( @va_Site_in is null  or  M.district = @va_Site_in )
		  and	E.nom like rtrim (@va_NomMachine_in) ++ "%"
		order by E.nom

		return XDC_OK
	end

#endif

	return XDC_NOK
go
