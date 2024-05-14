/*E*/
/*Fichier :  $Id: xzao220sp.prc,v 1.2 2008/02/13 13:23:46 devgtie Exp $      Release : $Revision: 1.2 $        Date : $Date: 2008/02/13 13:23:46 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET PASTRE
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* FICHIER xzao220sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture de la configuration du reseau d'autoroutes
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	19/06/07 : Creation (DEM 666) 1.1
-----------------------------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU
*
*  Liste des ITPC d'un district
*
* -----------------------------------------------------------------------------
* SEQUENCE D'APPEL
*
* SP	XZAO220_Liste_ITPC_Par_District
*
* PARAMETRES EN ENTREE :
*
* XDY_District		va_District_in
*
* PARAMETRES EN SORTIE :
*
* Aucun
*
* VALEUR RENDUE :
*
*  Liste des ITPC du district de numero indique.
*
*  Remarque : si le numero de district est celui du CI,
*              retourne la liste complete des ITPC.
*
* CODE RETOUR :
*
* XDC_OK
* XDC_ARG_INV : parametre d'entree Null
* <0 erreur sybase
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
-----------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO220' and type = 'P')
	drop procedure XZAO220
go


create procedure XZAO220
	@va_District_in		T_SITE = null
as

declare	@vl_Site	tinyint

	/*A Bug applix null */
	if @va_District_in = 0 select @va_District_in = null


	/*A Controle des parametres d'entree obligatoires */
	if @va_District_in = null
		return XDC_ARG_INV


	/*A
	** Recherche des ITPC situes sur une portion
	**   appartenant au district
	*/

	select	POR.autoroute, ITP.PR
	from	CFG..RES_ITP ITP, CFG..RES_POR POR
	where	(@va_District_in in (select numero
									from CFG..RES_DIS
									where type = XDC_TYPEM_SD) 
				or POR.district = @va_District_in) and
		POR.numero = ITP.portion
	order by POR.autoroute, ITP.PR

	return XDC_OK
go
