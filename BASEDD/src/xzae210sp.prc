/*E*/
/*  Fichier : $Id: xzae210sp.prc,v 1.8 2020/11/03 16:36:45 pc2dpdy Exp $        Release : $Revision: 1.8 $        Date : $Date: 2020/11/03 16:36:45 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET PASTRE
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* FICHIER xzae210sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Lecture / ecriture des fiches Main courante
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	19/12/08 : Creation 1.1
* JPL	20/01/09 : Controle poste operateur; synchronisation par transaction 1.2
* JPL	21/01/09 : Gestion tetes de bouchons; message aux postes du site 1.3
* JPL	05/02/09 : Correction condition de reservation inutile 1.4
* JPL	06/02/09 : Verrouiller la table FMC_GEN 1.5
* JPL	18/03/09 : Ajout directive 'use PRC' 1.6
* JPL	10/09/09 : Eviter retour non OK (et trace d'erreur) si aucune liberation (bouchon) 1.7
* LCL   22/04/20 : MOVIS Ajout site local pilotage DEM-SAE93
-----------------------------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU
*
*  Effectue la reservation ou la liberation d'une fiche Main courante
*  pour un poste operateur donne.
*
* -----------------------------------------------------------------------------
* SEQUENCE D'APPEL
*
* SP	XZAE210_Reserver_FMC
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_NumEvt_in		: numero de l'evenement
* XDY_Octet	va_CleEvt_in		: cle de l'evenement
* XDY_Machine	va_PosteOperateur_in	: numero de la machine de l'operateur
* XDY_Booleen	va_Reserver_in		: vrai pour reserver, faux pour liberer
*
* PARAMETRES EN SORTIE :
*
* XDY_Booleen	va_Modifie_out		: Vrai ssi. l'operation a modifie les donnees
*
* VALEUR RENDUE :
*
* Aucune
*
* CODE RETOUR :
*
* XDC_OK
* XDC_NOK           : evenement inconnu ou de type incorrect pour l'operation
* XZAEC_FMC_PAS_ENR : evenement non modifiable par cet operateur
* XDC_ARG_INV       : parametre d'entree obligatoire manquant (Null) ou invalide
* <0                : erreur sybase
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
*  Reservation :
*    Si l'evenement n'a ete reserve par aucun autre poste operateur,
*    le marquer pour modification par ce poste ;
*
*  Liberation :
*    Si l'evenement a ete reserve par ce poste operateur,
*    oter la marque de reservation par ce poste.
*
-----------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE210' and type = 'P')
	drop procedure XZAE210
go


create procedure XZAE210
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null,
	@va_PosteOperateur_in	T_OPERATEUR	= null,
	@va_Reserver_in		bit		= XDC_FAUX,
	@va_Modifie_out		bit		= XDC_NON	output,
	@va_NomSiteLocal_in             char(2)         = null
as

declare
	@vl_SiteLocal		T_SITE,
	@vl_TypeFMC		smallint,
	@vl_SiteFMC		T_SITE,
	@vl_PosteEnrich		T_OPERATEUR,
	@vl_NumCause		int,
	@vl_NumTete		int,
	@vl_SiteTete		T_SITE,
	@vl_PosteEnrichTete	T_OPERATEUR,
	@vl_Texte		varchar(100),
	@vl_Resultat		int

	/*A Valeur par defaut du parametre resultat */
	select @va_Modifie_out = XDC_NON


	/*A Transformation des parametres d'entree nulls (bug applix) */
	if @va_NumEvt_in = 0		select @va_NumEvt_in = null
	if @va_CleEvt_in = 0		select @va_CleEvt_in = null
	if @va_PosteOperateur_in = 0	select @va_PosteOperateur_in = null


	/*A
	** Verifier la validite des parametres d'entree obligatoires
	*/

	if @va_NumEvt_in is null or @va_CleEvt_in is null or
	   @va_PosteOperateur_in is null
		return XDC_ARG_INV

	if not exists (select nom from CFG..EQT_GEN
	                where type = XDC_EQT_MAC and numero = @va_PosteOperateur_in)
		return XDC_ARG_INV


	/*A
	** Rechercher le site local
	*/
        /* MOVIS */
        if @va_NomSiteLocal_in = null
                select  @vl_SiteLocal = numero
                from CFG..RES_DIS
                where serveur = @@servername
        else
                select  @vl_SiteLocal = numero
                from CFG..RES_DIS
                where code = @va_NomSiteLocal_in

	if @vl_SiteLocal is null
		return XDC_NOK


	--select @vl_Texte = "AVANT TRANSACTION date = " + convert (char(8), getdate(), 8)
	--print @vl_Texte

	/* Prevenir deux modifications simultanees */
	begin tran Tran_Reservation_Fmc


	/*A
	** Verifier l'existence de l'evenement
	** et son appartenance au site local
	*/

	select @vl_TypeFMC = type,
		@vl_SiteFMC = sit,
		@vl_NumCause = cause,
		@vl_PosteEnrich = poste_enrichisseur
	  from EXP..FMC_GEN holdlock
	 where numero = @va_NumEvt_in and cle = @va_CleEvt_in

	if @@rowcount = 0

		select @vl_Resultat = XDC_NOK

	else if @vl_TypeFMC = XZAEC_FMC_PosteOperateur

		/* L'operation ne peut etre effectuee sur les FMC de type 'Poste Operateur' */
		select @vl_Resultat = XDC_NOK

	else
	begin
		--select @vl_Texte = "DEBUT TRANSACTION date = " + convert(char(8),getdate(),8) + ", poste=" + convert(char(8),@vl_PosteEnrich)
		--print @vl_Texte

		/* Pour un bouchon rechercher les donnees de la tete (nulles si elle n'est pas trouvee) */
		if @vl_TypeFMC = XZAEC_FMC_QueueBouchon
		begin
			select @vl_NumTete = @vl_NumCause,
				@vl_SiteTete = sit,
				@vl_PosteEnrichTete = poste_enrichisseur
			  from EXP..FMC_GEN
			 where numero = @vl_NumCause and cle = @va_CleEvt_in
			   and type = XZAEC_FMC_TeteBouchon
		end

		if (@vl_SiteFMC <> @vl_SiteLocal) and (@vl_SiteTete <> @vl_SiteLocal)

			/* L'operation n'est autorisee que sur les FMC du site local */
			select @vl_Resultat = XZAEC_FMC_PAS_ENR

		else
		if @va_Reserver_in = XDC_VRAI
		begin
			/*A
			** Reserver la FMC et/ou la tete pour un bouchon si elle ne le sont pas deja
			*/

			/* si la FMC OU la tete du bouchon a ete reservee par un autre poste alors non autorise */
			if (@vl_SiteFMC = @vl_SiteLocal  and
			       (@vl_PosteEnrich is not null  and  @vl_PosteEnrich <> @va_PosteOperateur_in)) or
			   (@vl_SiteTete = @vl_SiteLocal  and
			       (@vl_PosteEnrichTete is not null  and  @vl_PosteEnrichTete <> @va_PosteOperateur_in))

				select @vl_Resultat = XZAEC_FMC_PAS_ENR

			else if not (@vl_SiteFMC = @vl_SiteLocal  and  @vl_PosteEnrich is null) and
			        not (@vl_SiteTete = @vl_SiteLocal  and  @vl_PosteEnrichTete is null)

				/* si aucune des FMC n'est sur le site local et libre */
				/* (celle(s) du site sont deja TOUTES DEUX reservees par ce poste) alors rien a faire */
				/* (si une sur deux: tete/queue bouchon nouvelle sur site ou libre, a reserver) */
				select @vl_Resultat = XDC_OK

			else
			begin
				--select @vl_Texte = "UPDATE date = " + convert(char(8),getdate(),8)
				--print @vl_Texte

				/* attribuer la ou les FMC du site local non reservees a ce poste operateur */
				update EXP..FMC_GEN set
					poste_enrichisseur = @va_PosteOperateur_in,
					zz = zz + 1
				where numero in (@va_NumEvt_in, @vl_NumTete)
				  and cle = @va_CleEvt_in
				  and sit = @vl_SiteLocal
				  and poste_enrichisseur is null

				if @@rowcount > 0
				begin
					select @va_Modifie_out = XDC_OUI
					select @vl_Resultat = XDC_OK
				end
				else
					select @vl_Resultat = XDC_NOK
			end
		end
		else
		begin
			/*A
			** Restituer la FMC et/ou la tete pour un bouchon si elles sont reservees par ce poste
			*/

			if (@vl_SiteFMC = @vl_SiteLocal  and  @vl_PosteEnrich = @va_PosteOperateur_in) or
			   (@vl_SiteTete = @vl_SiteLocal  and  @vl_PosteEnrichTete = @va_PosteOperateur_in)
			begin
				/* restituer toute fiche reservee par ce poste, dans toute situation */
				update EXP..FMC_GEN set
					poste_enrichisseur = null,
					zz = zz + 1
				where numero in (@va_NumEvt_in, @vl_NumTete)
				  and cle = @va_CleEvt_in
				  and sit = @vl_SiteLocal
				  and poste_enrichisseur = @va_PosteOperateur_in

				if @@rowcount > 0
				begin
					select @va_Modifie_out = XDC_OUI
					select @vl_Resultat = XDC_OK

					/* si l'une des fiches (du SITE LOCAL) reste reservee par un autre poste */
					/* alors l'indiquer (PAS DE MODIFICATION a signaler aux autres postes) */
					if (@vl_SiteFMC = @vl_SiteLocal  and
					       (@vl_PosteEnrich is not null and @vl_PosteEnrich <> @va_PosteOperateur_in)) or
					   (@vl_SiteTete = @vl_SiteLocal  and
					       (@vl_PosteEnrichTete is not null and @vl_PosteEnrichTete <> @va_PosteOperateur_in))
						select @vl_Resultat = XZAEC_FMC_PAS_ENR
				end
				else
					select @vl_Resultat = XDC_NOK
			end
			else
				select @vl_Resultat = XDC_OK
		end
	end


	--select "AVANT FIN TRANSACTION poste=" = poste_enrichisseur from EXP..FMC_GEN where numero = @va_NumEvt_in and cle = @va_CleEvt_in

	if @vl_Resultat = XDC_NOK
		rollback tran Tran_Reservation_Fmc
	else
		commit tran Tran_Reservation_Fmc

	--select "APRES FIN TRANSACTION poste=" = poste_enrichisseur from EXP..FMC_GEN where numero = @va_NumEvt_in and cle = @va_CleEvt_in


	/* si TOUTES LES FICHES du site sont reservees ou restituees, le signaler aux autres postes du site */
	if @va_Modifie_out = XDC_OUI  and  @vl_Resultat = XDC_OK
	begin
		if @va_Reserver_in = XDC_VRAI
			select @vl_Texte = XDC_REV_MSG + XDC_REV_FMC_RESERVEE + XDC_REV_TXT_SEP +
					      convert (varchar(10), @va_PosteOperateur_in)
		else
			select @vl_Texte = XDC_REV_MSG + XDC_REV_FMC_RESERVEE + XDC_REV_TXT_SEP + "0"
		exec XZAR;03 @va_NumEvt_in, @va_CleEvt_in, null, @vl_Texte, @vl_SiteLocal
	end

	return @vl_Resultat
go
