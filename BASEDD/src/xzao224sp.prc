/*E*/
/*Fichier :  $Id: xzao224sp.prc,v 1.2 2008/02/13 13:23:47 devgtie Exp $      Release : $Revision: 1.2 $        Date : $Date: 2008/02/13 13:23:47 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET PASTRE
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* FICHIER xzao224sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture de la configuration du reseau d'autoroutes
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	19/06/07 : Creation 1.1
-----------------------------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU
*
*  Liste des cantons d'un district
*
* -----------------------------------------------------------------------------
* SEQUENCE D'APPEL
*
* SP	XZAO224_Liste_Cantons_Par_District
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
*  Liste des cantons du district de numero indique.
*
*  Remarque : si le numero de district est celui du CI,
*              retourne la liste complete des cantons.
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

if exists (select * from sysobjects where name = 'XZAO224' and type = 'P')
	drop procedure XZAO224
go


create procedure XZAO224
	@va_District_in		T_SITE = null
as

declare	@vl_Site	tinyint

	/*A Bug applix null */
	if @va_District_in = 0 select @va_District_in = null


	/*A Controle des parametres d'entree obligatoires */
	if @va_District_in = null
		return XDC_ARG_INV


	/*A
	** Recherche des cantons dont le PR de debut est situe
	**   sur une portion appartenant au district
	*/

	select	POR.autoroute, CAN.PR_debut, CAN.sens
	from	CFG..RES_CAN CAN, CFG..RES_POR POR
	where	(@va_District_in in (select numero
									from CFG..RES_DIS
									where type = XDC_TYPEM_SD)
				 or POR.district = @va_District_in) and
		POR.numero = CAN.portion
	order by POR.autoroute, CAN.PR_debut, CAN.sens

	return XDC_OK
go
