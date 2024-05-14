/*E*/
/*Fichier : $Id: xzao807sp.prc,v 1.9 2020/11/03 17:04:25 pc2dpdy Exp $      Release : $Revision: 1.9 $        Date : $Date: 2020/11/03 17:04:25 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao807sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* GGY	11/10/23 : Recuperation proc depuis SDCI2D (DEM484)
------------------------------------------------------*/

/* fichiers inclus */
/* #include "xzao807sp.h" */
/* static char *version = "@(#)xzao78.c	1.1 23/18/94 : xzao807sp" ; */
/* declaration de fonctions internes */
/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des PMV pour une machine
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
use PRC
go


if exists (select * from sysobjects where name = 'XZAO807' and type = 'P')
	drop procedure XZAO807
go

create procedure XZAO807
   @va_NomMachine_in            char(10)
   as

/*
*
* SP    XZAO807SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier    va_Basedd_in
* XDY_Machine   va_NomMachine_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Retourne la liste des CFE connectes
*
* CODE RETOUR :
*
* 0     XDC_OK
* 1     XDC_NOK
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* MECANISMES :
*
* ------------------------------------------------------
* HISTORIQUE :
*
* C.T.  22 Jan 1998     : Creation
* ------------------------------------------------------ */

declare

	/* Declaration des variables locales */
	@vl_Machine          smallint

	if rtrim (@va_NomMachine_in) = NULL select @va_NomMachine_in=NULL

	if ( @va_NomMachine_in != NULL )
	begin
		/* Recherche du numero de la machine argument */
		select @vl_Machine = numero from CFG..EQT_GEN
			where nom  = @va_NomMachine_in
				and type = XDC_EQT_MAC

		/* Recherche de la liste des CFE associes a un serveur */
        select	CFE.numero, GSRV.nom, rtrim(CFE.adresse_ip), CFE.port, GCFE.site_gestion
            from	CFG..EQT_GEN GMAC,
					CFG..EQT_GEN GSRV,
					CFG..EQT_GEN GCFE,
					CFG..EQT_CFE CFE

				where	GMAC.numero			= @vl_Machine
					and GMAC.type			= XDC_EQT_MAC
					and GSRV.type			= XDC_EQT_SRV
					and GSRV.maitre			= GMAC.numero
					and GSRV.type_maitre	= XDC_EQT_MAC
					and GCFE.type			= XDC_EQT_CFE
					and GCFE.maitre			= GSRV.numero
					and GCFE.type_maitre	= XDC_EQT_SRV
					and CFE.numero			= GCFE.numero
						order by CFE.numero
	end
	else
	begin
		/* Recherche de la liste des CFE associes a plusieurs serveurs */
		select distinct CFE.numero,
						GSRV.nom,
						rtrim(CFE.adresse_ip),
						CFE.port,
						GCFE.site_gestion
			from	CFG..EQT_GEN GMAC,
					CFG..EQT_GEN GSRV,
					CFG..EQT_GEN GCFE,
					CFG..EQT_CFE CFE

				where	GMAC.type			= XDC_EQT_MAC
					and GSRV.type			= XDC_EQT_SRV
					and GSRV.maitre			= GMAC.numero
					and GSRV.type_maitre	= XDC_EQT_MAC
					and GCFE.type			= XDC_EQT_CFE
					and GCFE.maitre			= GSRV.numero
					and GCFE.type_maitre	= XDC_EQT_SRV
					and CFE.numero			= GCFE.numero
						order by CFE.numero
	end

return 0

go
