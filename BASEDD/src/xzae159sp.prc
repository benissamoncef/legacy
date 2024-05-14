/*E*/
/*  Fichier : $Id: xzae159sp.prc,v 1.4 2011/09/23 09:27:13 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2011/09/23 09:27:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae159sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Recherche de FMC de type veh_arrete sur un axe dans un rayon
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE  20/01/22: Creation AVA DEM-SAE367
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Recherche de FMC de type veh_arrete sur un axe dans un rayon
------------------------------------------------------- 
* Sequence d'appel
* SP	xzae159_Rechercher_Veh_dans_rayon
* 
* Arguments en entree
* XDY_Autoroute	va_NomAutoroute_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_Entier	va_Rayon_in
* 
* Arguments en sortie
* XDY_Entier    va_NumEvt_out
* XDY_Octet		va_Type_out
* XDY_PR		va_PR_out
* 
* Code retour
* XDC_OK
* XDC_NOK
* XDC_ARG_INV
* <0 erreur sybase 
* 
* Conditions d'utilisation
* Recherche de FMC de type veh_arrete sur un axe dans un rayon
* 
* Fonction
* Select dans la table FMC_VEH
* where  id_ava
-------------------------------------------------------*/

use PRC
go


if exists (select * from sysobjects where name = 'XZAE159' and type = 'P')
	drop procedure XZAE159
go


create procedure XZAE159
	@va_NomAutoroute_in	char(4),
	@va_PR_in	        int = null,
	@va_Sens_in	        T_SENS = null,
	@va_Rayon_in	    int = null
as	

declare @vl_Autoroute tinyint

	/*! transfo en parametres nulls bug applix */
	if @va_NomAutoroute_in = XDC_CHAINE_VIDE select @va_NomAutoroute_in = null
	if @va_PR_in = 0 select @va_PR_in = null
	if @va_Sens_in = 0 select @va_Sens_in = null
	if @va_Rayon_in = 0 select @va_Rayon_in = null

	/* test la presence des parametres obligatoires */
	if @va_NomAutoroute_in = null or @va_PR_in = null
	or @va_Sens_in = null or @va_Rayon_in = null
		return XDC_ARG_INV

	select @vl_Autoroute = numero
	from CFG..RES_AUT
	where nom = @va_NomAutoroute_in

	/*A lecture des informations sur complement vehicule */
    select top 1
        G.numero,
        G.cle,
        G.type,
        H.PR,
		C.nom,
		H.sens
	from EXP..FMC_HIS_DER H, EXP..FMC_GEN G, CFG..RES_AUT C
	where ( G.type = XZAEC_FMC_VehArrete or G.type = XZAEC_FMC_VehPanne ) and H.autoroute = @vl_Autoroute and H.sens = @va_Sens_in
	and H.PR > (@va_PR_in - @va_Rayon_in) and H.PR < (@va_PR_in + @va_Rayon_in)
	and H.numero = G.numero and H.cle = G.cle
	and G.fin is null and G.cloture is null 
	and C.numero = H.autoroute

return XDC_OK
go
