/*E*/
/*  Fichier : $Id: xzae19sp.prc,v 1.9 2020/11/03 16:29:32 pc2dpdy Exp $      Release : $Revision: 1.9 $        Date : $Date: 2020/11/03 16:29:32 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae19sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	11/10/94	: Creation (modif 364) V1.1
* C.T.	23/09/96	: Ajout possibilite de recuperer les droits du voisin V1.2
*                         (CONF 4)
* JMG	12/11/96	: appel filtre a XZAE;15 (DEM/1205) 1.3
* JMG	26/12/96	: ajout d'un rafraichissement des listes sur les 2 PO (CONF 4) 1.4
* PN	24/06/1997	: correction suite a temps de parcours (dem/1460) 1.5
* JMG	08/01/98	: gestion multi-sites (dem/1536) 1.6
* JMG	12/12/07	: gestion multi-postes DEM717 1.7
* JPL	02/12/08	: Degroupee; appel a XZAE17 renommee (lie a DEM 833) 1.8
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Transmettre les droits d'enrichir des FMC au voisin
* sur le site local
-------------------------------------------------------
* Sequence d'appel
* SP	XZAE19_Transmettre_Tous_Droits_Enrichir_Au_Voisin
* 
* Arguments en entree
* XDY_Machine	va_PosteOperateur_in
* XDY_Machine	va_PosteOperateurDest_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK		: fiche non trouvee
* XDC_ACT_REFUSEE	: action impossible pas de voisin connecte
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* ne fonctionne pas sur pc simplifie 
* 
* Fonction
*
----------------------------------------------------*/

use PRC
go

/*A creation de la table de travail des droits des fmc a traiter */
if exists (select * from sysobjects where name = 'LISTE_DROITS_FMC_A_TRAITER' and type = 'U')
	drop table  LISTE_DROITS_FMC_A_TRAITER
go

create table LISTE_DROITS_FMC_A_TRAITER (
                cle             tinyint not null,
		numero          int not null,
		type            tinyint not null,
		enrichir        tinyint,
		spid		int not null
		)	
go



if exists (select * from sysobjects where name = 'XZAE19' and type = 'P')
	drop procedure XZAE19
go

create procedure XZAE19
	@va_PosteOperateur_in		smallint= null,
	@va_PosteOperateurDest_in	smallint= null,
	@va_NomSiteLocal_in             char(2)         = null
as
	declare @vl_Num int,
		@vl_Cle tinyint,
		@vl_Cause int,
		@vl_CleCause tinyint,
		@vl_Type tinyint,
		@vl_Site T_SITE,
		@vl_AppelInterne tinyint,
		@vl_Resultat int,
		@vl_Status int,
		@vl_Enrichir tinyint,
		@vl_TexteReveil char(100),
		@vl_typeSite tinyint
	
	/*! transfo en paramÉtres nulls bug applix */
	if @va_PosteOperateur_in = 0 select @va_PosteOperateur_in = null
	if @va_PosteOperateurDest_in = 0 select @va_PosteOperateurDest_in = null
	
	/*A controle des arguments obligatoires */
	if @va_PosteOperateur_in = null and @va_PosteOperateurDest_in is null
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

	/* cas de recuperation de droit d'enrichir du voisin */
	/* recherche du poste operateur voisin connecte */
	if @va_PosteOperateur_in = null
	begin
		set rowcount 1

		select @va_PosteOperateur_in = poste_enrichisseur
		from EXP..FMC_GEN, EXP..FMC_TRT
		where	type = XZAEC_FMC_PosteOperateur and fin is null and
			poste_enrichisseur != @va_PosteOperateurDest_in and
			evenement = numero and
			EXP..FMC_TRT.cle = EXP..FMC_GEN.cle and
			EXP..FMC_TRT.sit = @vl_Site

		if @@rowcount = 0 or @va_PosteOperateur_in is null
			return XDC_ACT_REFUSEE

		set rowcount 0

		/* pas de site a valuer */
		select @vl_Site = NULL
	end
	else	select @va_PosteOperateurDest_in = null

	/*A rechercher la liste des fmc traiter par cet operateur */
	delete LISTE_DROITS_FMC_A_TRAITER where spid = @@spid
	select @vl_AppelInterne  = XDC_VRAI
	exec PRC..XZAE17 @va_PosteOperateur_in, @vl_AppelInterne 


	/*A transmettre les droits d'enrichir des fmc a traiter par cet operateur */
	declare FMC_A_ENRICHIR cursor
	for select 
		numero,
		cle,
		type,
		enrichir
	from LISTE_DROITS_FMC_A_TRAITER
	where enrichir != XDC_NON and spid = @@spid and type != XZAEC_FMC_PosteOperateur

	open FMC_A_ENRICHIR
	fetch FMC_A_ENRICHIR into @vl_Num, @vl_Cle, @vl_Type, @vl_Enrichir
	while @@sqlstatus = 0
	begin
		/*B transmettre le droit d'enrichir d'un bouchon */
		if @vl_Type = XZAEC_FMC_QueueBouchon
		begin
			/* transmettre le droit d'enrichir de la tete de bouchon */
			if @vl_Enrichir > 1
			begin
				/* rechercher la tete de bouchon */
				select @vl_Cause = cause, @vl_CleCause = cle_cause
				from EXP..FMC_GEN
				where numero = @vl_Num and cle = @vl_Cle

				exec @vl_Status = PRC..XZAE;15 @vl_Cause, @vl_CleCause, @vl_Site, @va_PosteOperateurDest_in , @vl_Resultat output , XDC_VRAI
			end


			/* transmettre le droit d'enrichir de la queue de bouchon */
			if @vl_Enrichir != 2 and @vl_Status = XDC_OK
				exec @vl_Status = PRC..XZAE;15 @vl_Num, @vl_Cle, @vl_Site, @va_PosteOperateurDest_in , @vl_Resultat output , XDC_VRAI
		end
		else
		/*B transmettre le droit d'enrichir d'une FMC autre que bouchon */
			exec @vl_Status = PRC..XZAE;15 @vl_Num, @vl_Cle, @vl_Site, @va_PosteOperateurDest_in , @vl_Resultat output , XDC_VRAI


		/* envoi d'un reveil pour prevenir le detenteur des droits initiaux qu'il n'a plus les droits d'enrichir */
		/* dans le cas ou il ne l'a pas demande */
		if @vl_Status = XDC_OK and @va_PosteOperateurDest_in is not null
		begin
			select @vl_TexteReveil = XDC_REV_FMC_PAS_ENR
			exec XZAR;03 @vl_Num, @vl_Cle, @va_PosteOperateur_in, @vl_TexteReveil
		end

		fetch FMC_A_ENRICHIR into @vl_Num, @vl_Cle, @vl_Type, @vl_Enrichir
	end
	close FMC_A_ENRICHIR
	
	/*demande de rafraichissement de la liste des FMC a traiter sur les 2 PO concernes*/
	if @va_PosteOperateurDest_in is not null
		exec XZAR;03 0,0,@va_PosteOperateurDest_in,""

	if @va_PosteOperateur_in is not null
		exec XZAR;03 0,0,@va_PosteOperateur_in,""

	/*A vider la table des droits des FMC a traiter */
	delete LISTE_DROITS_FMC_A_TRAITER where spid = @@spid

	return XDC_OK
go
