/*E*/
/*  Fichier : $Id: xzae156sp.prc,v 1.3 2020/11/03 16:14:01 pc2dpdy Exp $     Release : $Revision: 1.3 $        Date : $Date: 2020/11/03 16:14:01 $
-------------------------------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* FICHIER xzae156sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Info sur un evt pour la liste des fmc a traiter
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* Guilhou	08/02/00	creation
* JPL		11/12/08	Enrichissement tete bouchon; plus d'infos (idem XZAE17)
* LCL   	22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
-----------------------------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE156_Lire_FMC
*
* Arguments en entree
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve, utilisation interdite
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
-----------------------------------------------------------------------------*/

use PRC
go


if exists (select * from sysobjects where name = 'XZAE156' and type = 'P')
	drop procedure XZAE156
go


create procedure XZAE156
	@va_NumEvt_in 		int=null,
	@va_CleEvt_in		tinyint,
	@va_PosteOperateur_in   smallint= null,
	@va_NomSiteLocal_in     char(2)         = null
	
as

	declare @vl_SiteLocal		T_SITE,
		@vl_typeSite		tinyint,
		@vl_Num_Cause		int,
		@vl_Cle_Cause		tinyint,
		@vl_Type		tinyint,
		@vl_HoroEvt		datetime,
		@vl_HoroDebutPrevu	datetime,
		@vl_HoroFinPrevue	datetime,
		@vl_HoroDebut		datetime,
		@vl_HoroFin		datetime,
		@vl_SiteFMC		T_SITE,
		@vl_PosteEnrichisseur	T_OPERATEUR,
		@vl_Autoroute		T_AUTOROUTE,
		@vl_PR			T_PR,
		@vl_Sens		T_SENS,
		@vl_Enrichir		tinyint

	/*! transfo en paramÉtres nulls bug applix */
        if @va_NumEvt_in = 0 select @va_NumEvt_in = null
        if @va_CleEvt_in = 0 select @va_CleEvt_in = null
        if @va_PosteOperateur_in = 0 select @va_PosteOperateur_in = null


	/*A controle des parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null or
	   @va_PosteOperateur_in = null
		return XDC_ARG_INV


	/* recherche du site local */
        /* MOVIS */
        if @va_NomSiteLocal_in = null
        	select @vl_SiteLocal = numero, @vl_typeSite = type
                from CFG..RES_DIS
                where serveur = @@servername
        else
        	select @vl_SiteLocal = numero, @vl_typeSite = type
                from CFG..RES_DIS
                where code = @va_NomSiteLocal_in

	if @vl_typeSite = XDC_TYPEM_PCS
		return XDC_NOK


	/*A rechercher les informations de l'evenement */
	select 	@vl_Num_Cause=cause,
		@vl_Cle_Cause=cle_cause,
		@vl_Type=type,
		@vl_HoroEvt=isnull(debut,debut_prevu),
		@vl_HoroDebutPrevu=debut_prevu,
		@vl_HoroFinPrevue=fin_prevue,
		@vl_HoroDebut=debut,
		@vl_HoroFin=fin,
		@vl_SiteFMC=EXP..FMC_GEN.sit,
		@vl_PosteEnrichisseur=EXP..FMC_GEN.poste_enrichisseur,
		@vl_Autoroute=autoroute,
		@vl_PR=PR,
		@vl_Sens=sens
	from EXP..FMC_GEN, EXP..FMC_HIS_DER
	where EXP..FMC_GEN.numero=@va_NumEvt_in 
	and   EXP..FMC_GEN.cle=@va_CleEvt_in
	and   EXP..FMC_GEN.numero=EXP..FMC_HIS_DER.numero
	and   EXP..FMC_GEN.cle=EXP..FMC_HIS_DER.cle

	if @@rowcount = 0
		return XDC_NOK


	/*A calculer les droits d'enrichissement */
	select @vl_Enrichir=XZAEC_ENR_AUCUN

	if ( @vl_PosteEnrichisseur = @va_PosteOperateur_in ) or
		( @vl_PosteEnrichisseur = null and @vl_SiteFMC = @vl_SiteLocal )
		select @vl_Enrichir = XZAEC_ENR_FMC

	/*B ajouter aux bouchons les droits d'enrichissement de la tete */
	if @vl_Type = XZAEC_FMC_QueueBouchon
	begin
		if exists (select numero
				from EXP..FMC_GEN
				where numero=@vl_Num_Cause and
				      cle=@vl_Cle_Cause and
				      type=XZAEC_FMC_TeteBouchon and
				      ( poste_enrichisseur = @va_PosteOperateur_in or
					( poste_enrichisseur = null and sit = @vl_SiteLocal ) )
			)
			select @vl_Enrichir = @vl_Enrichir | XZAEC_ENR_TETEBOU
	end


	select	TYPE = @vl_Type,
		AUTOROUTE = @vl_Autoroute,
		PR = @vl_PR,
		SENS = @vl_Sens,
		DATE = @vl_HoroEvt,
		DEBUT_PREVU = @vl_HoroDebutPrevu,
		FIN_PREVUE = @vl_HoroFinPrevue,
		DEBUT = @vl_HoroDebut,
		FIN = @vl_HoroFin,
		ENRICHIR = @vl_Enrichir

	return XDC_OK
go
