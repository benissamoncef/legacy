/*X  Fichier : 	$Id: xzao323sp.prc,v 1.1 2018/06/19 10:25:22 pc2dpdy Exp $  Release : $Revision: 1.1 $ Date : $Date: 2018/06/19 10:25:22 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao323sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL	16/05/18	: Creation GABEB 1.1 DEM1283
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Liste des CAP par district
*
* Sequence d'appel
* SP    XZAO323_Liste_CAP_Par_District
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

create procedure XZAO323
	@va_District_in	T_SITE = null
as
	/*A Liste des CAP par district */
	select distinct
		CFG..EQT_GEN.numero,
		CFG..EQT_GEN.nom,
		CFG..EQT_GEN.autoroute,
		CFG..EQT_GEN.PR,
		CFG..EQT_GEN.sens,
		CFG..RES_POR.district
	from CFG..EQT_GEN, CFG..RES_POR
	where type=XDC_EQT_CAP and
		CFG..RES_POR.autoroute=CFG..EQT_GEN.autoroute and
		CFG..RES_POR.PR_debut<=CFG..EQT_GEN.PR and
		CFG..RES_POR.PR_fin>CFG..EQT_GEN.PR and
		CFG..RES_POR.district=@va_District_in

	return XDC_OK
go
