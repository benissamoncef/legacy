/*E*/
/*  Fichier : $Id: xzao651sp.prc,v 1.1 2016/12/08 16:18:17 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2016/12/08 16:18:17 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao651sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL	14/11/16	: Creation (LOT23)	(V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* retourne la liste des viaducs
* 
* Sequence d'appel
* SP	XZAO651_Liste_Viaduc
* 
* Arguments en entree
* 
* Arguments en sortie
* 
* XDY_Octet		va_Numero_out		-- arguments
* XDY_Nom		va_Nom_out		-- retournes
* XDY_Octet		va_Autoroute_out	-- dans une liste
* XDY_PR		va_PRDebut_out
* XDY_PR		va_PRFin_out
* XDY_Sens		va_Sens_out
* XDY_Site   		va_Site_out
* 
* Code retour
* XDC_OK
* <0 erreur sybase 
* 
* Conditions d'utilisation
* Toutes les informations sont retournees dans une liste
* 
* Fonction
* Select from RES_VIA
---------------------------------------------------*/

/* #include "xzao651sp.h" */

use PRC
go

if exists (select * from sysobjects where name = 'XZAO651' and type = 'P')
	drop procedure XZAO651
go


create procedure XZAO651
as
	/*A rechercher tous les viaducs */
	select
		CFG..RES_VIA.numero,
		CFG..RES_VIA.nom,
		CFG..RES_AUT.nom,
		CFG..RES_VIA.PRdebut,
		CFG..RES_VIA.PRfin,
		CFG..RES_VIA.sens,
		CFG..RES_VIA.site_gestion
	from CFG..RES_VIA, CFG..RES_AUT
	where CFG..RES_AUT.numero = CFG..RES_VIA.autoroute
	order by upper(CFG..RES_VIA.nom)
	
	return XDC_OK
go
