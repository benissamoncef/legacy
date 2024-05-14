/*E*/
/*  Fichier : $Id: xzae53sp.prc,v 1.12 2020/11/03 16:42:17 pc2dpdy Exp $      Release : $Revision: 1.12 $        Date : $Date: 2020/11/03 16:42:17 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae53sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	04/10/94	: Creation	(V 1.1)
* C.T.	10/11/94	: Modif bugs applix (V 1.2) 
* B.G.	21/12/94	: Modif test enrichisseur (1.4) 
* C.T.	09/02/95        : Modif update : zz value (V 1.5)
* C.T.	23/09/96        : Retour de XZAEC_FMC_PAS_ENR si pas les droits d'enrichir (V1.6) (CONF 4)
* C.T.	25/09/96        : Plus de valuation du champ degats_domaine devenu obsolete (FMC10) (V1.7)
* JMG	01/08/98	: gestion multi-sites (dem/1536) 1.8
* JMG	03/12/08	: ajout validation dans FMC_ACC 1.9 DEM/852
* JPL	09/11/11	: Degroupee; ajout champs de conditions accident dans FMC_ACC (DEM 1006)  1.10
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAE53_Ecrire_Fiche_Accident
* 
* Arguments en entree
* XDY_Entier	va_NumEvenement_in
* XDY_Octet	va_CleEvenement_in
* XDY_Octet	va_VL_in
* XDY_Octet	va_PL_in
* XDY_Octet	va_Remorque_in
* XDY_Octet	va_Car_in
* XDY_Octet	va_Moto_in
* XDY_Octet	va_BlesseLeg_in
* XDY_Octet	va_BlesseGra_in
* XDY_Octet	va_Mort_in
* XDY_Booleen	va_VehEscota_in
* XDY_Booleen	va_MatDang_in
* XDY_Booleen	va_HomEscota_in
* XDY_Booleen	va_Personalite_in
* XDY_Booleen	va_DegatsDom_in			Pour compatibilite
* XDY_Eqt	va_PosteOperateur_in
* XDY_Horodate	va_Validation_in
* XDY_Mot	va_Cond_Meteo_in
* XDY_Mot	va_Cond_Visibilite_in
* XDY_Mot	va_Cond_Etat_Chau_in
* XDY_Mot	va_Cond_Chantier_in
*
* Arguments en sortie
* 
* Service rendu
* Modifier une fiche d'accident relative a un evenement existant
* 
* Code retour
* XDC_OK
* XDC_NOK	    : evenement non trouve, update impossible,
* XZAEC_FMC_PAS_ENR : poste enrichisseur different
* XDC_ARG_INV	    : parametres d'entree null
* 
* Conditions d'utilisation
* 
* Fonction
* Modifier dans la table ACCIDENT
---------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE53' and type = 'P')
	drop procedure XZAE53
go


create procedure XZAE53
	@va_NumEvenement_in	int	= null,
	@va_CleEvenement_in	tinyint	= null,
	@va_VL_in		tinyint = null,
	@va_PL_in		tinyint = null,
	@va_Remorque_in		tinyint = null,
	@va_Car_in		tinyint = null,
	@va_Moto_in		tinyint = null,
	@va_BlesseLeg_in	tinyint = null,
	@va_BlesseGra_in	tinyint = null,
	@va_Mort_in		tinyint = null,
	@va_VehEscota_in	bit ,
	@va_MatDang_in		bit ,
	@va_HomEscota_in	bit ,
	@va_Personalite_in	bit ,
	@va_DegatsDom_in	bit ,			-- obsolete, pour compatibilite
	@va_PosteOperateur_in	T_EQUIPEMENT,
	@va_Validation_in	datetime,
	@va_Cond_Meteo_in	smallint,
	@va_Cond_Visibilite_in	smallint,
	@va_Cond_Etat_Chau_in	smallint,
	@va_Cond_Chantier_in	smallint,
	@va_NomSiteLocal_in             char(2)         = null
as
	declare @vl_SiteLocal	T_SITE,
		@vl_Site	T_SITE,
		@vl_typeSite	tinyint,
		@vl_Poste	smallint,
		@vl_zz		tinyint,
		 @vl_com_fmc_init char(50)

	/*! transfo en param…tres nulls bug applix */
	if @va_NumEvenement_in = 0 select @va_NumEvenement_in = null
	if @va_CleEvenement_in = 0 select @va_CleEvenement_in = null
	if @va_PosteOperateur_in = 0 select @va_PosteOperateur_in = null

	/*A controle parametres en entree */
	if @va_NumEvenement_in  = null or @va_CleEvenement_in = null or
	   @va_PosteOperateur_in = null
		return XDC_ARG_INV

	/*A recherche du site local */
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

	/*A verifier l'existance de l'evenement et la concordance */
	/*A du poste enrichisseur                                 */
	select @vl_Site = sit,
		@vl_Poste = poste_enrichisseur,
		@vl_com_fmc_init = com_fmc_init
	from EXP..FMC_GEN 
	where	numero = @va_NumEvenement_in and 
		cle = @va_CleEvenement_in
	if @@rowcount = 0
		return XDC_NOK
	if @vl_com_fmc_init <>NULL and 
		(@vl_Site <> @vl_SiteLocal or
		(@vl_Poste <> null and
		 @vl_Poste <> @va_PosteOperateur_in))
		return XZAEC_FMC_PAS_ENR

	/*recupere le dernier zz*/
	select @vl_zz = max(zz) from EXP..FMC_ACC
	where   numero = @va_NumEvenement_in and
		cle = @va_CleEvenement_in and
		sit = @vl_SiteLocal

	if @@rowcount != 1
		select @vl_zz=0

	/*A modification de la fiche accident */
	insert into EXP..FMC_ACC (
		numero, cle, sit,
		nombre_de_vl, nombre_de_pl,
		nombre_de_remorques, nombre_de_car, nombre_de_moto,
		vehicule_ESCOTA, matieres_dangereuses,
		blesses_legers, blesses_graves, morts,
		personnel_ESCOTA, personnalites, degats_domaine,
		zz, validation,
		cond_meteo, cond_visibilite, cond_etat_chaus, cond_chantier
	)
	values (
		@va_NumEvenement_in, @va_CleEvenement_in, @vl_Site,
		@va_VL_in, @va_PL_in,
		@va_Remorque_in, @va_Car_in, @va_Moto_in,
		@va_VehEscota_in, @va_MatDang_in,
		@va_BlesseLeg_in, @va_BlesseGra_in, @va_Mort_in,
		@va_HomEscota_in, @va_Personalite_in, @va_DegatsDom_in,
		@vl_zz + 1, @va_Validation_in,
		@va_Cond_Meteo_in, @va_Cond_Visibilite_in,
		@va_Cond_Etat_Chau_in, @va_Cond_Chantier_in
	)

	if @@rowcount != 1
		return XDC_NOK

	return XDC_OK

go
