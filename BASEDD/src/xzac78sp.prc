/*E*/
/*  Fichier : $Id: xzac78sp.prc,v 1.6 2020/10/13 17:32:49 pc2dpdy Exp $        Release : $Revision: 1.6 $        Date : $Date: 2020/10/13 17:32:49 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET MIGRAZUR / PASTRE
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* FICHIER xzac78sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Lecture / ecriture des actions
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	25/10/12 : Creation (DEM 1032)  1.1
* JPL	30/10/12 : Transformation des dates NULLes en entree  1.2
* JPL	31/10/12 : Mise a jour de la date de fin des actions precedentes pour la Fmc  1.3
* JPL	07/11/12 : Mise a jour de la date de fin a la date de succes ou echec des le debut  1.4
* PNI	02/02/15 : Creation de l'action au sein d'une transaction  1.5
* LCL   25/03/20   : MOVIS Ajout site local DEM-SAE93
-----------------------------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU
*
*  Action d'envoi des donnees de Conditions de Conduite Hivernale.
*
* SEQUENCE D'APPEL
*
* SP	XZAC78_Action_Envoi_CCH
*
* Arguments en entree :
*  XDY_Entier           va_NumEvt_in
*  XDY_Octet            va_CleEvt_in
*  XDY_Horodate         va_HoroLancement_in
*  XDY_Horodate         va_HoroSucces_in
*  XDY_Horodate         va_HoroEchec_in
*  XDY_Operateur        va_Operateur_in
*  char(200)            va_Commentaire_in
*
* Arguments en sortie :
*  XDY_Entier           va_NumAction_out
*
* Code retour :
*  XDC_OK
*  XDC_NOK            : evenement inexistant; creation de l'action erronee
*  XDC_ARG_INV        : parametre d'entree obligatoire manquant (Null) ou invalide
*  <0                 : erreur sybase
*
* Conditions d'utilisation :
*  La procedure stockee n'existe qu'au CI.
*
* Fonction :
*  Verifier l'existence de l'evenement.
*  Inserer dans les tables ACT_GEN et ACT_CCH avec le site du serveur.
*
-----------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAC78' and type = 'P')
	drop procedure XZAC78
go


#ifdef CI

create procedure XZAC78
	@va_NumEvt_in			int		= null,
	@va_CleEvt_in			tinyint		= null,
	@va_HoroLancement_in		datetime	= null,
	@va_HoroSucces_in		datetime	= null,
	@va_HoroEchec_in		datetime	= null,
	@va_Operateur_in		T_OPERATEUR	= null,
	@va_Commentaire_in		char(200)	= null,
	@va_NumAction_out		int		= null	output,
	@va_NomSiteLocal_in		char(2)	= null
as
	declare	@vl_Site		T_SITE,
		@vl_HoroFin		datetime


	/*A Transformation des parametres d'entree nuls */
	if @va_NumEvt_in = 0	select @va_NumEvt_in	= null
	if @va_CleEvt_in = 0	select @va_CleEvt_in	= null
	if @va_Operateur_in = 0	select @va_Operateur_in	= null
	if @va_HoroLancement_in = XDC_DATE_NULLE select @va_HoroLancement_in = null
	if @va_HoroSucces_in = XDC_DATE_NULLE select @va_HoroSucces_in = null
	if @va_HoroEchec_in = XDC_DATE_NULLE select @va_HoroEchec_in = null

	/*A Verifier la validite des parametres d'entree */
	if @va_NumEvt_in = null or @va_CleEvt_in = null  or
	   @va_Operateur_in = null
		return XDC_ARG_INV


	/*A Recherche du site local */
	/* MOVIS */
	if @va_NomSiteLocal_in = null
		select @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername
	else
		select @vl_Site = numero
		from CFG..RES_DIS
		where code = @va_NomSiteLocal_in

	if @vl_Site is null
		return XDC_NOK


	/*A verifier l'existance de l'evenement */
	if not exists (select * from EXP..FMC_GEN
	                  where numero = @va_NumEvt_in and cle = @va_CleEvt_in  and
	                        type = XZAEC_FMC_PosteOperateur)
		return XDC_NOK
begin tran
	/*A recherche du numero d'actio suivant */
	select @va_NumAction_out = isnull (max(numero), 0) + 1 from EXP..ACT_GEN
	where sit = @vl_Site


	/*A la valeur de la date de fin est celle de succes ou d'echec */
	select @vl_HoroFin = isnull (@va_HoroSucces_in, @va_HoroEchec_in)

	/*A creer une actio */
	insert EXP..ACT_GEN (
		numero, sit, sit_origine,
		evenement, cle,
		type, equipement,
		heure_lancement, heure_succes, heure_echec, heure_lancement_fin, heure_fin,
		priorite, distance_evt, prioritaire,
		operateur, competence
	) values (
		@va_NumAction_out, @vl_Site, @vl_Site,
		@va_NumEvt_in, @va_CleEvt_in,
		XDC_ACT_CCH, null,
		@va_HoroLancement_in, @va_HoroSucces_in, @va_HoroEchec_in, null, @vl_HoroFin,
		null, null, XDC_NON,
		@va_Operateur_in, null
	)
	if @@rowcount <> 1
	begin
		commit tran
		return XDC_NOK
	end

commit tran
	/*A inserer le complement de l'actio (le commentaire general) */
	/*A sur un troncon nul, uniquement s'il contient du texte     */
	if rtrim (@va_Commentaire_in) <> null
	begin
		insert EXP..ACT_CCH (actio, sit, troncon, cch, commentaire)
		values (@va_NumAction_out, @vl_Site, 0, XZAEC_CCH_NR, @va_Commentaire_in)
	end

	return XDC_OK

#endif

go
