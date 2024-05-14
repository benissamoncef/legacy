/*X  Fichier : $Id: xzae105sp.prc,v 1.5 2020/11/03 16:12:05 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2020/11/03 16:12:05 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae105sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	03/09/96 : Creation (RADT)
* C.T.	20/09/96 : Suppression des travaux en prevision annules (1.2)
* JMG	07/01/98 : gestion multi-sites (dem/1536) 1.3
* JPL	08/06/07 : Ajout cas des FMCs Basculement pour Travaux (DEM 647) 1.4
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Rechercher les fmc travaux en prevision sur pc simplifie
*
* Sequence d'appel
* SP    XZAE105_Liste_Travaux_En_Prevision
*
* Arguments en entree
*
* Arguments en sortie
*
* XDY_Entier	numero
* XDY_Octet	cle
* XDY_Mot	type
* XDY_Horodate	debut
* XDY_Octet	autoroute
* XDY_PR	PR
* XDY_Sens	sens
*
* Code retour
* XDC_OK
* <0 erreur sybase
*
* Conditions d'utilisation
* au PC simplifie uniquement
*
* Fonction
---------------------------------------------------*/

use PRC
go

#ifdef PC_SIMPL

create procedure XZAE;105
        @va_NomSiteLocal_in             char(2)         = null
as
	declare @vl_Site T_SITE

	/*A Recherche du site */
        /* MOVIS */
        if @va_NomSiteLocal_in = null
                select  @vl_Site = numero
                from CFG..RES_DIS
                where serveur = @@servername
        else
                select  @vl_Site = numero
                from CFG..RES_DIS
                where code = @va_NomSiteLocal_in


	/*A rechercher les fmc travaux en prevision */
	/*! une seule val dans FMC_HIS dans ce cas */
	select
		EXP..FMC_GEN.numero,
		EXP..FMC_GEN.cle,
		type,
		debut_prevu,
		autoroute,
		PR,
		sens
	from EXP..FMC_GEN, EXP..FMC_HIS
	where	(type = XZAEC_FMC_Travaux  or  type = XZAEC_FMC_BasculTravaux) and
		EXP..FMC_GEN.sit = @vl_Site and
		EXP..FMC_GEN.fin is null and
		EXP..FMC_HIS.numero = EXP..FMC_GEN.numero and
		EXP..FMC_HIS.cle = EXP..FMC_GEN.cle


	return XDC_OK
go

#endif
