/*X  Fichier : 	$Id: xzao316sp.prc,v 1.1 2007/03/26 15:44:06 gesconf Exp $  Release : $Revision: 1.1 $ Date : $Date: 2007/03/26 15:44:06 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao316sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	09/06/05: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Liste des PMVA par district
*
* Sequence d'appel
* SP    XZAO316_Liste_PMVA_Par_District
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

if exists (select * from sysobjects where name = 'XZAO316' and type = 'P')
	drop procedure XZAO316
go

create procedure XZAO316
	@va_District_in	T_SITE = null
as
	/*A Liste des PMVA par district */
	select distinct
		CFG..EQT_GEN.numero,
		CFG..EQT_GEN.nom,
		CFG..EQT_GEN.autoroute,
		CFG..EQT_GEN.PR,
		CFG..EQT_GEN.sens,
		CFG..RES_POR.district
	from CFG..EQT_GEN, CFG..RES_POR
	where type=XDC_EQT_PMVA and
		CFG..RES_POR.autoroute=CFG..EQT_GEN.autoroute and
		CFG..RES_POR.PR_debut<=CFG..EQT_GEN.PR and
		CFG..RES_POR.PR_fin>CFG..EQT_GEN.PR and
		CFG..RES_POR.district=@va_District_in

	return XDC_OK
go
