/*E*/
/*  Fichier : $Id: xzae98sp.prc,v 1.11 2021/03/08 14:08:08 pc2dpdy Exp $        $Revision: 1.11 $        $Date: 2021/03/08 14:08:08 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae98sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* P.V.  28/08/96: Creation
* C.T.  18/09/96: Correction : si le site local n'est pas le CI 
*                 alors declencher la proc au CI (1.2-1.3)
* PN	29/06/98: separation EXP/HIS 1.4
* PN    24/09/98: separation EXP/HIS:correction 1.5
* JPL	20/03/19: Syntaxe  1.7 1.8
* PN	05/06/19 : Correctif sur test CI 1.9
* LCL   22/04/20   : MOVIS Ajout site local pilotage DEM-SAE93
* ABE	06/01/21   : MOVIS ajout PRA DEM-SAE93 1.11
* ABK   26/09/23 	: Ajout district CA (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Efface le vehicule desire
------------------------------------------------------- 
* Sequence d'appel
* SP	XZAE98_Supprime_Vehicule
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* XDY_Mot		va_NumVeh_in
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de complement de fiche main courante
* XDC_ARG_INV
* <0 erreur sybase 
* 
* 
* Fonction
* Suppression du vehicule desire dans EXP et HIS
* 
* Remarque : un probleme de replication sur HIS peut
* entrainer une impossibilite d'effectuer cette action
* sans erreur
-----------------------------------------------------*/

use PRC
go
drop proc XZAE98
go

create procedure XZAE98
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null,
	@va_NumVeh_in		smallint = null,
	@va_NomSiteLocal_in             char(2)         = null
as
	declare @vl_Status int, @vl_Site tinyint,@vl_SiteMaitre T_SITE,
	@vl_serveur_pra char(6)

	/*! transfo en param�tres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null

	/* test la presence des parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null or @va_NumVeh_in = null
		return XDC_ARG_INV

	/* initialisation variable locale */
	select @vl_SiteMaitre = numero from CFG..RES_DIS where type = XDC_TYPEM_SD

#ifndef HIST
	/* l'effacement de la fiche vehicule s'effectue toujours au CI */

        /* MOVIS */
        if @va_NomSiteLocal_in = null
                select  @vl_Site = numero
                from CFG..RES_DIS
                where serveur = @@servername
        else
                select  @vl_Site = numero
                from CFG..RES_DIS
                where code = @va_NomSiteLocal_in
select @vl_Site


	if @vl_Site != @vl_SiteMaitre
	begin

		/* Init serveur PRA */
		select @vl_serveur_pra=serveur_pra from CFG..RES_PRA where numero=@vl_SiteMaitre

		if  @vl_serveur_pra=XDC_SQLDP
		exec @vl_Status = SQL_DS.PRC..XZAE98 @va_NumEvt_in, @va_CleEvt_in, @va_NumVeh_in

		/* redirection de l'actio sur le CA */
		else if @vl_serveur_pra=XDC_SQLCA
		exec @vl_Status = SQL_CA.PRC..XZAE98 @va_NumEvt_in, @va_CleEvt_in, @va_NumVeh_in

		/* redirection de l'actio sur le CI */
		else
		exec @vl_Status = SQL_CI.PRC..XZAE98 @va_NumEvt_in, @va_CleEvt_in, @va_NumVeh_in

		return @vl_Status
	end
#ifdef CI
	else
	begin

		select * from EXP..FMC_VEH 
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in and numero_vehicule = @va_NumVeh_in
		
		if @@rowcount != 1
			return XDC_NOK
	
	
		/* effacement des donnees */
		delete EXP..FMC_VEH
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in and numero_vehicule = @va_NumVeh_in
		/* redirection de l'actio sur le HIST */
		exec @vl_Status = SQL_HIS.PRC..XZAE98 @va_NumEvt_in, @va_CleEvt_in, @va_NumVeh_in
		return @vl_Status
	
	end
#endif
#else
		select * from HIS..FMC_VEH 
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in and numero_vehicule = @va_NumVeh_in
	
		if @@rowcount != 1
			return XDC_NOK
		delete HIS..FMC_VEH
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in and numero_vehicule = @va_NumVeh_in
#endif

	return XDC_OK
go
