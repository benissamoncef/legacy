/*E*/
/*  Fichier : $Id: xzae75sp.prc,v 1.2 1998/04/02 16:24:59 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1998/04/02 16:24:59 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae75sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.     	26/02/98	: Creation 	V 1.1
* C.T.     	02/04/98	: Correction pour le 
*                                 test de fmc init non autorisee 	V 1.2
* ABK   	26/09/23 	: Ajout district CA (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Modifier la FMC initiale au CI pour une FMC de type degrade
* 
* Sequence d'appel
* XZAE75_Modif_FMC_Initiale
* 
* Arguments en entree
* XDY_Entier	va_NumEvt_in		: queue de bouchon
* XDY_Octet	va_CleEvt_in
* XDY_Machine	va_PosteOperateur_in	: numero de machine
* XDY_Entier	va_NumFMCInit_in		: queue de bouchon
* XDY_Octet	va_CleFMCInit_in
* 
* Arguments en sortie
* XDY_Entier	va_Resultat_out : XDC_OK si achevenement de l'evt est ok
* 
* Code retour
* XDC_OK
* XDC_NOK		: evenement inexistant, suppression de la liste des evts
*                         a traiter impossible, site local inconnu,
*                         positionner la date de cloture impossible
* XDC_ARG_INV		: argument d'entree null, ou valeur incorrecte
* XDC_PRC_INC		: procedure stockee n'existe pas en base
* 
* Conditions d'utilisation
* au CI uniquement.
* va_Resultat_out vaut :
*   XDC_OK
*   XZAEC_FMC_INIT_INC	: FMC initiale inexistante
*   XZAEC_FMC_INIT_DEG	: toutes les actions relatives a l'evenement ne sont 
*   	      		    terminees
*   XZAEC_FMC_INIT_INCOMP: FMC initiale de type incomptable
*   XZAEC_FMC_INIT_PAS_ENR  : pas le droit d'enrichir la FMC initiale
* 
* Fonction
-------------------------------------------------------------*/

use PRC
go

drop proc XZAE75
go

create procedure XZAE75
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null,
	@va_PosteOperateur_in	T_EQUIPEMENT = null,
	@va_NumFMCInit_in	int = null,
	@va_CleFMCInit_in	tinyint = null
as
declare @vl_Type smallint, @vl_DegradeInit tinyint, @vl_NumFMCInitOrig int, 
	@vl_TypeFMCInit smallint, @vl_SiteFmcInit T_SITE,  @vl_Cause int,
	@vl_PosteEnrichisseurInit T_EQUIPEMENT, @vl_PosteEnrich T_EQUIPEMENT, @vl_Site T_SITE,
	@vl_SiteMaitre T_SITE

	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_NumFMCInit_in = 0 select @va_NumFMCInit_in = null
	if @va_CleFMCInit_in = 0 select @va_CleFMCInit_in = null
	if @va_PosteOperateur_in = 0 select @va_PosteOperateur_in = null

	/*A controle des parametres d'entree */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV

	/* initialisation variable locale */
	select @vl_SiteMaitre = numero from CFG..RES_DIS where type = XDC_TYPEM_SD

	/*A verifier l'existence de la fmc degrade */
	select
		@vl_Type = type,
		@vl_Cause = cause,
		@vl_PosteEnrich = poste_enrichisseur,
		@vl_Site = sit
	from EXP..FMC_GEN
	where numero = @va_NumEvt_in and cle = @va_CleEvt_in and degrade = XDC_OUI

	if @@rowcount = 0
		return XDC_NOK

	if (@vl_PosteEnrich is not null and @vl_PosteEnrich != @va_PosteOperateur_in) or
		(@vl_Site != @vl_SiteMaitre)
	begin
		return XZAEC_FMC_PAS_ENR
	end

	/*A tester la validite de la fmc initiale */
	if @va_NumFMCInit_in is not null
	begin
		if @va_CleFMCInit_in != @va_CleEvt_in and @va_CleFMCInit_in != @vl_SiteMaitre and 
			@va_CleEvt_in != @vl_vl_SiteMaitreSiteCI
			return XZAEC_FMC_INIT_NON_AUTORISEE

		/* rechercher la fmc init */
		select
			@vl_DegradeInit = degrade,
			@vl_NumFMCInitOrig = num_fmc_init,
			@vl_TypeFMCInit = type,
			@vl_SiteFmcInit = sit,
			@vl_PosteEnrichisseurInit = poste_enrichisseur
		from EXP..FMC_GEN
		where numero = @va_NumFMCInit_in and cle = @va_CleFMCInit_in
	
		/* fmc init inexistante */
		if @@rowcount = 0
		begin
			return XZAEC_FMC_INIT_INC
		end
		/* fmc init degrade */
		else if @vl_DegradeInit = XDC_OUI and @vl_NumFMCInitOrig is not null
		begin
			return XZAEC_FMC_INIT_DEG
		end
		else if not exists (	select * from CFG..TYP_FMC T1, CFG..TYP_FMC T2 
					where T1.numero = @vl_TypeFMCInit and
					T2.classe = T1.classe and T2.numero = @vl_Type)
		begin
			return XZAEC_FMC_INIT_INCOMP
		end
		else if (@vl_PosteEnrichisseurInit is not null and
	    			@va_PosteOperateur_in != @vl_PosteEnrichisseurInit) or
	   			@vl_SiteFmcInit != @vl_SiteMaitre
		begin
			return XZAEC_FMC_INIT_PAS_ENR
		end
	end

	/*A modif fmc initiale */
	update EXP..FMC_GEN set 
		num_fmc_init = @va_NumFMCInit_in, 
		cle_fmc_init = @va_CleFMCInit_in, 
		zz = zz + 1
	where numero = @va_NumEvt_in and cle = @va_CleEvt_in 

	if @vl_Type = XZAEC_FMC_QueueBouchon
		update EXP..FMC_GEN set 
			num_fmc_init = @va_NumFMCInit_in, 
			cle_fmc_init = @va_CleFMCInit_in, 
			zz = zz + 1
		where numero = @vl_Cause and cle = @va_CleEvt_in 

	return XDC_OK
go
