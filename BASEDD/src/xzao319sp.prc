/*X  Fichier : 	$Id: xzao319sp.prc,v 1.1 2018/04/19 14:07:45 pc2dpdy Exp $  Release : $Revision: 1.1 $ Date : $Date: 2018/04/19 14:07:45 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao319sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL	02/03/18	: Creation controleurs de feux CTRL_FEUX 1.1 DEM1284
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Liste des CFE par district
*
* Sequence d'appel
* SP    XZAO319_Liste_CFE_Par_District
*
* Arguments en entree
* XDY_Site	va_District_in
*
* Arguments en sortie
*
* Liste retournee :
*
* Code retour
* XDC_OK
* <0 erreur sybase
*
* Conditions d'utilisation
*
* Fonction
---------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO319' and type = 'P')
	drop procedure XZAO319
go

create procedure XZAO319
	@va_District_in	T_SITE = null
as
	/*A Liste des CFE par district */
	select distinct
		CFG..EQT_GEN.numero,
		CFG..EQT_GEN.nom,
		CFG..EQT_GEN.autoroute,
		CFG..EQT_GEN.PR,
		CFG..EQT_GEN.sens,
		CFG..RES_POR.district
	from CFG..EQT_GEN, CFG..RES_POR
	where type=XDC_EQT_CFE and
		CFG..RES_POR.autoroute=CFG..EQT_GEN.autoroute and
		CFG..RES_POR.PR_debut<=CFG..EQT_GEN.PR and
		CFG..RES_POR.PR_fin>CFG..EQT_GEN.PR and
		CFG..RES_POR.district=@va_District_in

	return XDC_OK
go
