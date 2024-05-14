/* Fichier : $Id: xzao318sp.prc,v 1.1 2012/07/04 11:29:26 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 11:29:26 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao318sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	03/01/12	creation (DEM/1016)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Liste des PRV par district
*
* Sequence d'appel
* SP    XZAO318_Liste_PRV_Par_District
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

if exists (select * from sysobjects where name = 'XZAO318' and type = 'P')
	drop procedure XZAO318
go

create procedure XZAO318
	@va_District_in	T_SITE = null
as
	/*A Liste des PRV par district */
	select distinct
		CFG..EQT_GEN.numero,
		CFG..EQT_GEN.nom,
		CFG..EQT_GEN.autoroute,
		CFG..EQT_GEN.PR,
		CFG..EQT_GEN.sens,
		CFG..RES_POR.district
	from CFG..EQT_GEN, CFG..RES_POR
	where type=XDC_EQT_PRV and
		CFG..RES_POR.autoroute=CFG..EQT_GEN.autoroute and
		CFG..RES_POR.PR_debut<=CFG..EQT_GEN.PR and
		CFG..RES_POR.PR_fin>CFG..EQT_GEN.PR and
		CFG..RES_POR.district=@va_District_in

	return XDC_OK
go
