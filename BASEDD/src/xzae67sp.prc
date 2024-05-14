/*E*/
/*  Fichier : $Id: xzae67sp.prc,v 1.8 2020/11/03 17:00:58 pc2dpdy Exp $      Release : $Revision: 1.8 $        Date : $Date: 2020/11/03 17:00:58 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae67sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	03/10/94	: Creation	(V 1.1)
* C.T.	10/11/94	: Modif bugs applix (V 1.2)
* B.G.	20/12/94	: suppression test enrichir (1.4)
* JMG	05/09/96	: ajout PC simplifies (RADT) (1.5)
* NDE   21/07/15	: Reveil au CIT sur ajout/modif (1.7, DEM1137)
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
* ABK   26/09/23 	: Ajout district CA (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Inserer un commentaire relatif a un evenement existant
* 
* Sequence d'appel
* SP	XZAE67_Ecrire_Fiche_Commentaire
* 
* Arguments en entree
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* XDY_Entier		va_Numero_in
* XDY_Horodate		va_Horodate_in
* XDY_Horodate		va_HoroPerime_in
* XDY_Commentaire	va_Texte_in
* XDY_Eqt		va_PosteOperateur_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve, insertion ou update impossible,
*                 poste enrichisseur different, va_Horodate_in null
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Si le parametre va_Numero_in est null alors insertion sinon modification
* Le parametre va_Horodate_in doit etre non null dans le cas d'une
* insertion d'un nouveau commentaire
* 
* Fonction
* Verifier droit d'enrichir
* Inserer ou modifier dans la table COMMENTAIRES
----------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE67' and type = 'P')
drop procedure XZAE67
go

create procedure XZAE67
	@va_NumEvenement_in	int	= null,
	@va_CleEvenement_in	tinyint	= null,
	@va_Numero_in		int	= null,
	@va_Horodate_in		datetime = null,
	@va_HoroPerime_in	datetime = null,
	@va_Texte_in		T_TEXTE = null,
	@va_PosteOperateur_in	smallint = null,
	@va_NomSiteLocal_in             char(2)         = null
as
	declare @vl_Site T_SITE,
			@vl_SiteMaitre T_SITE,
		@vl_HoroPerime datetime,
		@vl_TexteReveil char(100),
		@vl_PosteEnrichisseur smallint,
		@vl_Status int

	/*! transfo en param�tres nulls bug applix */
	if @va_NumEvenement_in = 0 select @va_NumEvenement_in = null
	if @va_CleEvenement_in = 0 select @va_CleEvenement_in = null
	if @va_Numero_in = 0 select @va_Numero_in = null
	if @va_PosteOperateur_in = 0 select @va_PosteOperateur_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null
	if @va_HoroPerime_in = XDC_DATE_NULLE select @va_HoroPerime_in = null

	/*A controle les parametres obligatoires */
	if @va_NumEvenement_in = null or @va_CleEvenement_in = null or
	   @va_PosteOperateur_in = null or @va_Texte_in = null
		return XDC_ARG_INV

	/*A recherche le site en local */
        /* MOVIS */
        if @va_NomSiteLocal_in = null
                select  @vl_Site = numero
                from CFG..RES_DIS
                where serveur = @@servername
        else
                select  @vl_Site = numero
                from CFG..RES_DIS
                where code = @va_NomSiteLocal_in

	if @vl_Site = null
	  return XDC_NOK

	/*A verifier l'existance de l'evenement  */
	if not exists ( select * from EXP..FMC_GEN 
			where numero = @va_NumEvenement_in and 
				cle = @va_CleEvenement_in)
		return XDC_NOK

	if @va_Numero_in = null
	begin
		/*A controle les parametres obligatoires dans ce cas */
		if @va_Horodate_in = null 
			return XDC_ARG_INV

		/*A recherche le numero du morceau de commentaire */
		select @va_Numero_in = isnull(max(numero), 0) + 1 from EXP..FMC_COM
		where	evenement = @va_NumEvenement_in and
			cle = @va_CleEvenement_in and
			sit = @vl_Site

		/*A insertion d'un commentaire */
		insert EXP..FMC_COM (evenement, cle, numero, horodate_validation,
		horodate_fin, texte, sit)
		values (@va_NumEvenement_in, @va_CleEvenement_in, @va_Numero_in, @va_Horodate_in,
		@va_HoroPerime_in, @va_Texte_in, @vl_Site)
	end
	else
	begin
		/* verifier l'existance du commentaire sur le site */
		select @vl_HoroPerime = horodate_fin from EXP..FMC_COM
		where	numero = @va_Numero_in and
			evenement = @va_NumEvenement_in and
			cle = @va_CleEvenement_in and
			sit = @vl_Site

		if @@rowcount != 1
			return XDC_NOK

		/*A modification d'un commentaire */
		if @vl_HoroPerime is null
		begin
			update EXP..FMC_COM set horodate_fin = @va_HoroPerime_in,
				texte = @va_Texte_in
			where	numero = @va_Numero_in and
				evenement = @va_NumEvenement_in and
				cle = @va_CleEvenement_in and
				sit = @vl_Site
		end
	end

	if @@rowcount != 1
		return XDC_NOK

	/* initialisation variable locale */
	select @vl_SiteMaitre = numero from CFG..RES_DIS where type = XDC_TYPEM_SD

	/*A envoi d'un reveil aux operateur du CI sur ajout ou 
	    modification ssi. la modification a ete faite a 
	    partir d'un site autre que le CI */
	if @vl_Site != @vl_SiteMaitre
	begin
		select @vl_TexteReveil = XDC_REV_FMC_MOD + ' (COMMENTAIRE)'

		declare pointeur_operateur_ci cursor for
		select distinct
			poste_enrichisseur
		from
			EXP..FMC_GEN, CFG..TYP_FMC
		where
			EXP..FMC_GEN.sit = @vl_SiteMaitre and
			CFG..TYP_FMC.numero = EXP..FMC_GEN.type and
			CFG..TYP_FMC.classe = XZAEC_ClassePosteOper and
			EXP..FMC_GEN.fin is null and
			EXP..FMC_GEN.poste_enrichisseur is not null

		open pointeur_operateur_ci
		fetch pointeur_operateur_ci into @vl_PosteEnrichisseur 

		while @@sqlstatus = 0
		begin
			exec @vl_Status = XZAR;03 @va_NumEvenement_in, @va_CleEvenement_in,
						@vl_PosteEnrichisseur, @vl_TexteReveil
/*
			if @vl_Status = null
				return XDC_PRC_INC
			else if @vl_Status != XDC_OK
				return @vl_Status

*/
			fetch pointeur_operateur_ci into @vl_PosteEnrichisseur 
		end
		close pointeur_operateur_ci
	end

	return XDC_OK
go
