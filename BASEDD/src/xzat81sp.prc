/*
-------------------------------------------------------------------------------------------
* ESCOTA * PROJET MIGRAZUR
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* FICHIER xzat81sp.prc
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Enregistre l'etat (Mode de fonctionnement, Plan de Feux actif)
*   d'un Controleur de Feux donne.
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	11/05/23 : Creation (SAE-440) 
-----------------------------------------------------------------------------------------*/


/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU
*
*  Memorise l'etat commande a un Controleur de Feux, c'est a dire le mode
*   de fonctionnement et le Plan de Feux.
*  Dans la version initiale, les etats successifs ne sont pas historises.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL
*
* SP	XZAT81_Ecrire_Etat_CFE
*
* PARAMETRES EN ENTREE :
*
* XDY_Eqt	va_NumEqt_in		: Numero de l'equipement CFE
* XDY_Horodate	va_Horodate_in		: Horodate de determination de l'etat courant
* XDY_Octet	va_Fonctionnement_in	: Etat de fonctionnement (Eteint, Clignotant, Tricolore ...)
* XDY_Octet	va_Plan_Feux_in		: Numero du Plan de Feux commande au Controleur
*
*
* PARAMETRES EN SORTIE :
*
* Aucun
*
* CODE RETOUR :
*
* XDC_OK
* XDC_NOK           : Equipement CFE inexistant, incoherence de site
* XDC_ARG_INV       : parametre d'entree obligatoire manquant (Null) ou invalide
* <0                : erreur sybase
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
*  Si les donnees sont valides, enregistre l'etat de fonctionnement et le Plan de Feux
*  commandes a l'equipement.
*  Dans tous les cas, termine toute notification d'etat active de l'equipement.
*
-----------------------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAT81' and type = 'P')
	drop procedure XZAT81
go


create procedure XZAT81
	@va_NumEqt_in		T_EQUIPEMENT	= null,
	@va_Horodate_in		datetime	= null,
	@va_Fonctionnement_in	char(1)		= null,
	@va_Plan_Feux_in	tinyint		= null,
	@va_NomSiteLocal_in	char(2)		= null
as
	declare	@vl_Site		T_SITE,
		@vl_NomEqt		T_NOM,
		@vl_Fonctionnement	char(1),
		@vl_Mode		int


	/* Determiner le site local */
	if @va_NomSiteLocal_in = null
		select	@vl_Site = numero
		from	CFG..RES_DIS
		where	serveur = @@servername
	else
	begin
		select	@vl_Site = numero
		from	CFG..RES_DIS
		where	code = @va_NomSiteLocal_in
	end

	if @vl_Site is null
		return XDC_NOK


	/* Verifier la validite des parametres d'entree */
	if @va_Horodate_in = XDC_DATE_NULLE
		select @va_Horodate_in = getdate ()

	select @vl_Fonctionnement = @va_Fonctionnement_in
	if @vl_Fonctionnement <> XDC_CFE_FCT_NUL
	begin
		/* Ramener un fonctionnement de test (minuscule) en fonctionnement normal (majuscule) */
		select @vl_Fonctionnement = upper (@va_Fonctionnement_in)
		if @vl_Fonctionnement not in (XDC_CFE_FCT_ETEINT, XDC_CFE_FCT_CLIGNO, XDC_CFE_FCT_INIT)  and
		   @vl_Fonctionnement not in ('D', 'F', 'K', 'G', 'I')
			return XDC_ARG_INV
	end
	select @vl_Mode = ascii (@va_Fonctionnement_in)

	if (@va_Plan_Feux_in <> XDC_CFE_PLN_NUL)  and
	    @va_Plan_Feux_in not in (select plan_de_feux from CFG..DEF_PDF where numero = @va_NumEqt_in  and  valide = XDC_VRAI)
		return XDC_ARG_INV


	/* Verifier la presence de l'equipement sur le site */
	select @vl_NomEqt= nom from CFG..EQT_GEN where type = XDC_EQT_CFE  and  numero = @va_NumEqt_in  and
	                                               site_gestion = @vl_Site  and
	                                               eqt_supprime = XDC_FAUX
	if @@rowcount = 0
		return XDC_NOK


	begin tran

		/* Si le nouvel etat est different de l'etat en cours alors */
		if not exists (select 1 from EXP..ETA_EQT where type = XDC_EQT_CFE  and  numero = @va_NumEqt_in  and
		                                                heure_fin is null  and
		                                                mode = @vl_Mode  and  etat = @va_Plan_Feux_in)
		begin
			/* Terminer tout etat en cours memorise */
			delete EXP..ETA_EQT where type = XDC_EQT_CFE  and  numero = @va_NumEqt_in

			/* Pour les equipements monitores, memoriser le nouvel etat en cours s'il est valide */
			if @vl_NomEqt like "%LB"
			begin
				if @vl_Fonctionnement <> XDC_CFE_FCT_NUL
				begin
					insert EXP..ETA_EQT (type, numero, mode, etat, heure_debut, heure_fin)
					values (XDC_EQT_CFE, @va_NumEqt_in, @vl_Mode, @va_Plan_Feux_in, @va_Horodate_in, NULL)
				end
			end
		end

	commit tran

	return XDC_OK
go
