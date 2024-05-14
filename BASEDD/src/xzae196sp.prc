/*E*/
/* Fichier : $Id: xzae196sp.prc,v 1.2 2020/11/27 09:23:00 pc2dpdy Exp $        Release : $Revision: 1.2 $        Date : $Date: 2020/11/27 09:23:00 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae196sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module MI2 : Retourne le code SIREDO et la version pour une station et sens
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE     30/07/20  : Creation DEM-SAE130 1.1
* ABE     15/10/20  : Ajout select colonne version DEM-SAE130 1.2
*
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
*
* Sequence d"appel
* SP    XZAE196
*
* Arguments en entree
* va_numero_station T_EQUIPEMENT
* va_sens T_SENS
*
* Arguments en sortie
* CFG..RES_DIT.nom_pt_mesure
* CFG..RES_DIT.version_radt
*
* aucun :
*
* Service rendu
*    Retourne le code siredo d'une station et du sens.
*
* Code retour
* XDC_OK
* XDC_NOK       :
* XDC_ARG_INV   : parametres d"entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d"utilisation
*
* Fonction
*
-----------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE196' and type = 'P')
	drop procedure XZAE196
go


create procedure XZAE196
	@va_numero_station_in 	T_EQUIPEMENT,
	@va_sens_in 		T_SENS,
	@va_code_siredo_out 	char = null output
as

declare
@vl_maintenant datetime,
@vl_date_debut datetime,
@vl_date_fin   datetime

/*A
** Test des parametres d'entree :
**   Si un des parametres d'entree est egal a null alors XDC_ARG_INV
*/
if @va_numero_station_in = 0 select @va_numero_station_in = null
if @va_sens_in = 0 select @va_sens_in = null

if @va_numero_station_in = null
or @va_sens_in = null
	return ( XDC_ARG_INV )

select	rtrim (nom_pt_mesure),version_radt
from	CFG..RES_DIT
where	CFG..RES_DIT.numero_radt = @va_numero_station_in
  and	CFG..RES_DIT.sens_radt = @va_sens_in

return (XDC_OK)

go
