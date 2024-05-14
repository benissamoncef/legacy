/*E*/
/*  Fichier : @(#)xzae208sp.prc	1.8      Release : 1.8        Date : 04/27/98
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae208sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* CHI	18/07/2005: Creation v1.1 DEM487
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
*
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Mettre en base une fiche de classe nature (de l'obstacle) relative a un 
* evenement existant
* 
* Sequence d'appel
* SP	XZAE208_Ecrire_nature_obstacle
* 
* Arguments en entree
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* XDY_Horodate		va_Horodate_in
* char(100)             va_contenu_in
* XDY_Eqt		va_PosteOperateur_in
* XDY_Octet		va_Previson_Traiter_Clos_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve, insert ou update impossible,
*                 poste enrichisseur different
* XDC_ARG_INV	: parametres d'entree null
* 
* Conditions d'utilisation
* la date passee en parametre d'entree doit etre ensuite passee
* a la procedure de validation de la fiche main courante (XZAE11)
* 
* Fonction
* Selon la valeur de Prevision_Traiter_Clos :
* 	Prevision :	Inserer ou modifier dans la table FMC_NAT
* 	Traiter :	Inserer dans la table FMC_NAT
* 	Clos :		Modifier dans la table FMC_NAT
--------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE208' and type = 'P')
        drop procedure XZAE208
go

create procedure XZAE208
	@va_NumEvenement_in	int	= null,
	@va_CleEvenement_in	tinyint	= null,
	@va_Horodate_in		datetime = null,
	@va_nature_in           char(100)=null,
	@va_code_nature_in	tinyint = null,
	@va_PosteOperateur_in	T_EQUIPEMENT,
	@va_Prevision_Traiter_Clos_in tinyint = null,
	@va_NomSiteLocal_in             char(2)         = null
as
	declare @vl_Site T_SITE, @vl_typeSite tinyint, @vl_SiteFMC T_SITE

	/*! transfo en paramétres nulls bug applix */
	if @va_CleEvenement_in = 0 select @va_CleEvenement_in = null
	if @va_NumEvenement_in = 0 select @va_NumEvenement_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null
	if @va_Prevision_Traiter_Clos_in = 0 select @va_Prevision_Traiter_Clos_in = null
	if @va_PosteOperateur_in = 0 select @va_PosteOperateur_in = null

	/*A controle parametres en entree */
	if @va_NumEvenement_in  = null or @va_CleEvenement_in = null or
	   @va_Horodate_in = null or
	   @va_nature_in = null or
	   @va_code_nature_in = null or
	   @va_PosteOperateur_in = null or @va_Prevision_Traiter_Clos_in = null
		return XDC_ARG_INV

	/*A recherche du site local */
        /* MOVIS */
        if @va_NomSiteLocal_in = null
		select @vl_Site = numero, @vl_typeSite = type
                from CFG..RES_DIS
                where serveur = @@servername
        else
		select @vl_Site = numero, @vl_typeSite = type
                from CFG..RES_DIS
                where code = @va_NomSiteLocal_in

	if @vl_typeSite = XDC_TYPEM_PCS
		return XDC_NOK

	/*A verifier l'existance de l'evenement et la concordance */
	/*A du poste enrichisseur                                 */
	if @va_Prevision_Traiter_Clos_in != XDC_FMC_ETAT_CLOS and
	   not exists ( select * from EXP..FMC_GEN 
			where	numero = @va_NumEvenement_in and 
				cle = @va_CleEvenement_in and
				( poste_enrichisseur = @va_PosteOperateur_in or
				  poste_enrichisseur = null ) and
				sit = @vl_Site)
		return XZAEC_FMC_PAS_ENR

	if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_CLOS and
	   not exists(	select sit
			from EXP..FMC_NAT
			where	numero = @va_NumEvenement_in and
				cle = @va_CleEvenement_in and
				horodate = @va_Horodate_in and
				sit = @vl_Site)
		return XZAEC_FMC_CLOSE_PAS_ENR


	if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_PREV
	begin
		/*A fiche en prevision a inserer si non existante sinon */
		/*A modifier les valeurs                                */
		if exists ( select * from EXP..FMC_NAT
				where 	numero = @va_NumEvenement_in and
					cle = @va_CleEvenement_in and
					sit = @vl_Site)
			update EXP..FMC_NAT set
				horodate = @va_Horodate_in,
				nature = @va_nature_in,
				code = @va_code_nature_in
			where	numero = @va_NumEvenement_in and
				cle = @va_CleEvenement_in and
				sit = @vl_Site
		else
			insert EXP..FMC_NAT (numero, cle, horodate,
			nature, code, sit)
			values (@va_NumEvenement_in, @va_CleEvenement_in,
			@va_Horodate_in, @va_nature_in,@va_code_nature_in, 
			@vl_Site)
	end
	else if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_TRAI
		/*A insertion d'une fiche a traiter */
		insert EXP..FMC_NAT (numero, cle, horodate,
		nature,code, sit)
		values (@va_NumEvenement_in, @va_CleEvenement_in,
		@va_Horodate_in, @va_nature_in, @va_code_nature_in,
		@vl_Site)
	else if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_CLOS
	begin

		/*A modification de la fiche pour cloturer */
		update EXP..FMC_NAT set
			nature = @va_nature_in,
			code = @va_code_nature_in
		where	numero = @va_NumEvenement_in and
			cle = @va_CleEvenement_in and
			sit = @vl_Site and
			horodate = @va_Horodate_in
	end
	else return XDC_ARG_INV

	if @@rowcount != 1
		return XDC_NOK

	return XDC_OK

go
