/*X  Fichier : $Id: xzao315sp.prc,v 1.1 1995/10/02 16:00:29 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/10/02 16:00:29 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao315sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	07/06/95	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Liste des PMV par district
*
* Sequence d'appel
* SP    XZAO315_Liste_PMV_Par_District
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

create procedure XZAO315
	@va_District_in	T_SITE = null
as
	/*A Liste des PMV par district */
	select distinct
		CFG..EQT_GEN.numero,
		CFG..EQT_GEN.nom,
		CFG..EQT_GEN.autoroute,
		CFG..EQT_GEN.PR,
		CFG..EQT_GEN.sens,
		CFG..RES_POR.district
	from CFG..EQT_GEN, CFG..RES_POR
	where type=XDC_EQT_PMV and
		CFG..RES_POR.autoroute=CFG..EQT_GEN.autoroute and
		CFG..RES_POR.PR_debut<=CFG..EQT_GEN.PR and
		CFG..RES_POR.PR_fin>CFG..EQT_GEN.PR and
		CFG..RES_POR.district=@va_District_in

	return XDC_OK
go
