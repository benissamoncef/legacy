/*E*/
/*  Fichier : $Id: xzae64sp.prc,v 1.14 2020/11/03 16:54:56 pc2dpdy Exp $      Release : $Revision: 1.14 $        Date : $Date: 2020/11/03 16:54:56 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae64sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	03/10/94	: Creation	(V 1.1)
* C.T.	24/10/94	: Modif	de l'appel a XZAE;18
*                         et test avec XZAE;18 reelle
*                         modif de la construction de la
*			  liste des evts a traiter
*			                (V 1.2)
* C.T.	10/11/94	: Modif bugs applix (V 1.3)
* B.G.	16/12/94	: Modif prise de poste (1.5)
* C.T.	09/02/95        : Modif update : zz value (V 1.6)
* C.T.	24/02/95        : Modif update : zz value (V 1.7)
* C.T.	01/02/96        : Ajout rafraichissement liste a traiter (V1.8)
* C.T.	03/09/96        : Ajout cas d'un PC simpl (V1.8)
*                         et passage a 10h au lieu de 24h
* C.T.	30/10/96        : Ajout type_FMC_valide dans FMC_HIS (V1.10)
* JMG	30/05/97	: supprime la precedente fmc operateur de EQT_FIL (tdp) 1.11
* JMG	08/01/98	: gestion multi-sites (dem/1536) 1.12
* JPL	12/01/2009	: Degroupee; appel a XZAE18 renommee (lie a DEM 848) 1.13
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Modifier une fiche de poste operateur relative a un evenement existant
* 
* Sequence d'appel
* SP	XZAE64_Ecrire_Fiche_Poste_Operateur
* 
* Arguments en entree
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* XDY_Nom		va_Operateur_in
* XDY_Eqt		va_PosteOperateur_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve, update impossible,
*                 poste enrichisseur different
* XDC_ARG_INV	: parametres d'entree null
* 
* Conditions d'utilisation
* 
* Fonction
* Modifier dans la table POSTE_OPERATEUR
* S'il s'agit de la creation de la FMC, reconstruire la liste 
* des evenements a traiter pour les 24 prochaines heures (XZAE18)
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE64' and type = 'P')
	drop procedure XZAE64
go


create procedure XZAE64
	@va_NumEvenement_in	int	= null,
	@va_CleEvenement_in	tinyint	= null,
	@va_Operateur_in	T_NOM	= null,
	@va_PosteOperateur_in	T_EQUIPEMENT = null,
	@va_NomSiteLocal_in             char(2)         = null
as
	declare @vl_Site T_SITE,  @vl_Erreur bit,
		@vl_OperateurAInserer T_OPERATEUR, @vl_Status int,
		@vl_NumEvtPrev int, @vl_CleEvtPrev tinyint,
		@vl_DateCourantePlus10h datetime

	/*! transfo en param…tres nulls bug applix */
	if @va_NumEvenement_in = 0 select @va_NumEvenement_in = null
	if @va_CleEvenement_in = 0 select @va_CleEvenement_in = null
	if @va_PosteOperateur_in = 0 select @va_PosteOperateur_in = null

	/*A controle parametres en entree */
	if @va_NumEvenement_in  = null or @va_CleEvenement_in = null or
	   @va_PosteOperateur_in = null or @va_Operateur_in = null
		return XDC_ARG_INV

	/*A recherche du site local */
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

	/*A verifier l'existance de l'evenement et la concordance */
	/*A du poste enrichisseur                                 */
	if not exists ( select * from EXP..FMC_GEN 
			where	numero = @va_NumEvenement_in and 
				cle = @va_CleEvenement_in and
				poste_enrichisseur = @va_PosteOperateur_in and
				sit = @vl_Site)
		return XDC_NOK


	/*A recherche le numero de l'operateur */
	select @vl_OperateurAInserer = numero from CFG..OPR_NOM
	where nom = @va_Operateur_in

	if @@rowcount != 1
		return XDC_NOK

	begin tran
		/*A modification de la fiche operateur */
		update EXP..FMC_OPR set 
			operateur = @vl_OperateurAInserer
		where	numero = @va_NumEvenement_in and 
			cle = @va_CleEvenement_in and
			sit = @vl_Site
	
		if @@rowcount != 1
			select @vl_Erreur = XDC_OUI
		else
			/*A modification de la fiche generique */
			update EXP..FMC_GEN set 
				debut=getdate(), 
				zz = zz + 1
			where	numero = @va_NumEvenement_in and 
				cle = @va_CleEvenement_in and
				sit = @vl_Site

		if @@rowcount != 1
			select @vl_Erreur = XDC_OUI
	commit tran

	if @vl_Erreur = XDC_OUI
		return XDC_NOK

	/*A creation de la fiche historique (validation) */
	
	insert EXP..FMC_HIS (
		numero,
		cle,
		horodate_validation,
		confirme_ou_signale,
		autoroute,
		PR,
		sens,
		VR,
		VM2,
		VM1,
		VL,
		BAU,
		VR_I,
		VM2_I,
		VM1_I,
		VL_I,
		BAU_I,
		sit,
		type_anterieur,
		type_FMC_valide)
	values (
		@va_NumEvenement_in,
		@va_CleEvenement_in,
		getdate(),
		XDC_OUI,
		null,
		null,
		XDC_SENS_INCONNU,
		XDC_VOIE_INEXISTANTE,
		XDC_VOIE_INEXISTANTE,
		XDC_VOIE_INEXISTANTE,
		XDC_VOIE_INEXISTANTE,
		XDC_VOIE_INEXISTANTE,
		XDC_VOIE_INEXISTANTE,
		XDC_VOIE_INEXISTANTE,
		XDC_VOIE_INEXISTANTE,
		XDC_VOIE_INEXISTANTE,
		XDC_VOIE_INEXISTANTE,
		@vl_Site,
		XZAEC_FMC_PosteOperateur,
		XZAEC_FMC_PosteOperateur
		)
#ifndef PC_SIMPL
	/*A suppression de la fiche poste operateur precedente de la liste a traiter s'il y a lieu */
	
	delete EXP..FMC_TRT
	from EXP..FMC_GEN
	where EXP..FMC_GEN.type=XZAEC_FMC_PosteOperateur
	  and EXP..FMC_GEN.poste_enrichisseur=@va_PosteOperateur_in
	  and EXP..FMC_GEN.numero<>@va_NumEvenement_in
	  and EXP..FMC_GEN.fin=null
	  and EXP..FMC_GEN.cle=EXP..FMC_TRT.cle
	  and EXP..FMC_GEN.numero=EXP..FMC_TRT.evenement

	delete EXP..EQT_FIL
	from EXP..FMC_GEN
	where EXP..FMC_GEN.type=XZAEC_FMC_PosteOperateur
	  and EXP..FMC_GEN.poste_enrichisseur=@va_PosteOperateur_in
	  and EXP..FMC_GEN.numero<>@va_NumEvenement_in
	  and EXP..FMC_GEN.fin=null
	  and EXP..FMC_GEN.cle=EXP..EQT_FIL.cle
	  and EXP..FMC_GEN.numero=EXP..EQT_FIL.evenement
#endif
	/*A cloture de la fiche poste operateur precedente, s'il y a lieu */
	
	update EXP..FMC_GEN set 
		fin=getdate(), 
		cloture=getdate(),
		zz = zz + 1
	where type=XZAEC_FMC_PosteOperateur
	  and poste_enrichisseur=@va_PosteOperateur_in
	  and numero<>@va_NumEvenement_in
	  and fin=null

#ifndef PC_SIMPL
	/*A construire la liste des evenements a la creation de la */
	/*A FMC                                                    */
	begin
		/*B ajouter l'evenement operateur a la liste des evenements */
		/*B a traiter                                               */
		exec @vl_Status = XZAE18 @vl_Site, @va_NumEvenement_in,
					  @va_CleEvenement_in
		if @vl_Status = null
			return XDC_PRC_INC
		else if @vl_Status != XDC_OK
			return @vl_Status

		/* date + 10h */
		select @vl_DateCourantePlus10h = getdate() 
		select @vl_DateCourantePlus10h = dateadd(hour, 10,@vl_DateCourantePlus10h) 

		/*B recherche tous les evenements pour les futurs */
		/*B 24h sur le site local                         */
		select
			numero,
			cle
		into #EVT_PRV
		from EXP..FMC_GEN
		where	sit = @vl_Site and 
			debut_prevu <= @vl_DateCourantePlus10h and
			fin = null

		/*B ajouter des evenements en prevision pour les futurs 10h */ 
		/*B dans la liste des evenements a traiter                  */
		declare Pointeur_evts cursor
		for select numero, cle
		from #EVT_PRV

		open Pointeur_evts
		fetch Pointeur_evts into @vl_NumEvtPrev, @vl_CleEvtPrev
		while (@@sqlstatus = 0)
		begin
			/* ajout de l'evenement en prevision dans la liste */
			/* (il est ajoute s'il n'est pas deja a traiter)   */
			exec @vl_Status = XZAE18 @vl_Site, @vl_NumEvtPrev,
				  		@vl_CleEvtPrev
			if @vl_Status = null
				return XDC_PRC_INC
			else if @vl_Status != XDC_OK
				return @vl_Status

			/* passage a l'evenement en prevision suivant */
			fetch Pointeur_evts into @vl_NumEvtPrev, @vl_CleEvtPrev
		end

		close Pointeur_evts

		/*B Reinitialiser la liste des evenements a traiter pour le site local */
		exec XZAR;03 0, 0, null, "", @vl_Site
	end
#endif

	return XDC_OK

go
