/*E*/
/*  Fichier : $Id: xzae14sp.prc,v 1.29 2020/12/18 09:34:12 pc2dpdy Exp $        Release : $Revision: 1.29 $        Date : $Date: 2020/12/18 09:34:12 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae14sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.     	24/10/94	: Creation 	V 1.1
* C.T.     	24/10/94	: Modif commentaire entete
*                                 V 1.2
* C.T.     	10/11/94	: Modif bugs applix V 1.3
*                                 non teste
* C.T.          16/11/94        : Correction test servername
*                                 non teste  V 1.5
* B.G.          19/12/94        : modif action finies pour le site (1.6)
* B.G.          19/01/95        : modif nettoyage file d'attente eqt (1.7)
* C.T.          21/02/95        : Ajout modiff zz V 1.8
* C.T.		14/03/95	: Modif longueur texte reveil (V 1.9)
* C.T.		03/09/96	: Restreindre l'achevenement d'une fmc
*                                 operateur ou travaux sur un PC simplifie (V1.10)
*                                 suppression du controle des actions en cours dans ce cas
* C.T.		30/10/96	: Correction erreur de compilation (V1.11)
* P.N.		24/12/96	: Correction de l'appel xzae15 (1.12)
* C.T.		16/12/97	: Ajout la gestion de l'achevement d'une fmc degrade  (1531) (1.13)
* JMG		07/01/98	: gestion PC niveau 2 (dem/1536) 1.14
* C.T.		15/01/98	: Correction la gestion de l'achevement d'une fmc degrade  (1531) (1.15)
* JMG		19/01/98	: achevement FMC PC niveau 3 au CI 1.16
* C.T.		28/01/98	: Correction la gestion de l'achevement d'une fmc degrade  (1531) (1.17)
* JMG		26/02/98	: achevement FMC PC niveau 2 au CI (dem/1580)
* C.T.		04/03/98	: Deplacement du transfert d'info par XZAE12 en cas de pb (1583) 1.18
* C.T.		25/04/98	: Suppression la gestion de l'achevement d'une fmc degrade  (1638) (1.19)
* P..		12/01/99	: Modif pour FMC DATEX  (1724) (1.20)
* P.N.		22/06/07	: Ajout cas pour DP (DEM641) 1.21
* JPL		12/01/2009	: Degroupee; appel a XZAE15 renommee (lie a DEM 848) 1.22
* JMG		02/10/09	: secto DEM88 1.23.
* JMG		15/05/16	: SAGA 1.24
* JMG		20/03/17	: regio 1.25
* LCL		03/07/17	: Modif PRA
* LCL   	22/04/20	: MOVIS Ajout site local pilotage DEM-SAE93
* LCL		16/11/20	: Tests PRA correction achevement FMC
* LCL		17/12/20	: Correction achevement FMC au CI en mode PRA DEM-SAE93 1.29
* LCL		24/08/23	: Correction achevement FMC ASF/ESCOTA au CI 1.30
* ABK		26/09/23 	: Ajout district CA (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Acheve la fiche main courante (l'enleve de la liste 
* des fiches a traiter pour le poste operateur).
* 
* Sequence d'appel
* XZAE14_Achever_Fiche_MC
* 
* Arguments en entree
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* XDY_Machine	va_PosteOperateur_in	: numero de machine
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
* XDC_REV_NON_DCL	: l'evenement a ete acheve mais pb de reveil
* 
* Conditions d'utilisation
* va_Resultat_out vaut :
*   XDC_OK
*   XZAEC_FMC_PAS_ENR	  : l'evenement ne peut pas etre modifie par cet operateur
*   XZAEC_FMC_ACT_NON_FIN : toutes les actions relatives a l'evenement ne sont 
*   	      		    terminees
*   XZAEC_FMC_A_TRT	  : evenement est a traiter dans un district
*   XZAEC_FMC_NON_FIN	  : evenement non termine
* 
* Fonction
* 	-verifier que toutes les actions du site liees 
* 	 a la FMC sont finies, sinon refuser
* 	-au district, si l'operateur a le droit d'enrichir, 
* 	 transmettre ce droit au CI
* 	-au CI, verifier que l'evenement est fini
* 	-supprimer la FMC de la liste des evenements a traiter
-------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE14' and type = 'P')
	drop procedure XZAE14
go


create procedure XZAE14
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null,
	@va_PosteOperateur_in	T_EQUIPEMENT = null,
	@va_Resultat_out	int = null	output,
	@va_NomSiteLocal_in             char(2)         = null
as
	declare @vl_Site T_SITE, @vl_Classe tinyint, @vl_Degrade tinyint,
		@vl_SiteMaitre T_SITE, @vl_SiteFmc T_SITE, @vl_Fin datetime,
		@vl_PosteEnrichisseur T_EQUIPEMENT, 
		@vl_TexteReveil char(100), @vl_Status int,
		@vl_Resultat int, @vl_NomMachine char(10),
		@vl_NumEvt int, @vl_CleEvt tinyint, @vl_NomSite char(2),
		@vl_NumFMCInit int, @vl_CleFMCInit tinyint, @vl_DegradeInit tinyint,
		@vl_NumFMCInitOrig int, @vl_TypeFMCInit smallint,
		@vl_Cause int, @vl_CleCause tinyint,
		@vl_DegradeCause tinyint, @vl_CauseNumFMCInit int, @vl_CauseCleFMCInit tinyint,
		@vl_SiteFmcInit T_SITE, @vl_PosteEnrichisseurInit T_EQUIPEMENT, @vl_Datex tinyint,
		@vl_date datetime,@vl_ComFMCInit char(100)

	/*! transfo en parametres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_PosteOperateur_in = 0 select @va_PosteOperateur_in = null

	/*A controle des parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
	   	return XDC_ARG_INV

	/* recherche du site local */
	/* MOVIS */
	if @va_NomSiteLocal_in = null
			select  @vl_Site = numero, @vl_NomSite = code
			from CFG..RES_DIS
			where serveur = @@servername
	else
			select  @vl_Site = numero, @vl_NomSite = code
			from CFG..RES_DIS
			where code = @va_NomSiteLocal_in

	if @vl_Site is null
		return XDC_NOK


#ifndef PC_SIMPL
	/*A verifier que toutes les actions relatives a l'evenement */
	/*A sont terminees  sur le site local                       */
	/*B finir les action pmv datex s'il en existe */
	update EXP..ACT_GEN set heure_fin=getdate()
	where evenement = @va_NumEvt_in and
		cle = @va_CleEvt_in and
		heure_fin is null and 
		type = XDC_ACT_PMVDAT
	
	if exists (	select * from EXP..ACT_GEN
			where	evenement = @va_NumEvt_in and
				cle = @va_CleEvt_in and
				heure_fin is null and
				sit=@vl_Site and type <> XDC_ACT_PMVDAT)
	begin
		select @va_Resultat_out = XZAEC_FMC_ACT_NON_FIN
		return XDC_OK
	end
	
	/* nettoyage de la file d'attente */
	
	delete EXP..EQT_FIL
	where evenement=@va_NumEvt_in
	  and cle=@va_CleEvt_in
#endif
	/* initialisation variable locale */
	select @vl_SiteMaitre = numero from CFG..RES_DIS where type = XDC_TYPEM_SD

	/*A verifier l'existance de l'evenement sur le site local */
	select 
		@vl_Datex		= EXP..FMC_GEN.datex,
		@vl_Fin			= EXP..FMC_GEN.fin,
		@vl_PosteEnrichisseur	= EXP..FMC_GEN.poste_enrichisseur,
		@vl_Classe		= CFG..TYP_FMC.classe,
		@vl_SiteFmc		= EXP..FMC_GEN.sit,
		@vl_Degrade		= EXP..FMC_GEN.degrade,
		@vl_NumFMCInit		= EXP..FMC_GEN.num_fmc_init,
		@vl_CleFMCInit		= EXP..FMC_GEN.cle_fmc_init,
		@vl_Cause		= EXP..FMC_GEN.cause,
		@vl_CleCause		= EXP..FMC_GEN.cle_cause,
		@vl_ComFMCInit		= EXP..FMC_GEN.com_fmc_init
	from EXP..FMC_GEN, CFG..TYP_FMC
	where	EXP..FMC_GEN.numero = @va_NumEvt_in and 
		EXP..FMC_GEN.cle = @va_CleEvt_in and
		CFG..TYP_FMC.numero = EXP..FMC_GEN.type

	if @@rowcount != 1
	begin
		return XDC_NOK
	end


	/*A Si l'evenement est de type operateur et l'operateur a */
	/*A le droit d'enrichir alors le supprimer de la          */
	/*A liste des evenements a traiter sur tous les sites     */
	if @vl_Classe = XZAEC_ClassePosteOper
		if (@vl_PosteEnrichisseur = null or
		    @va_PosteOperateur_in = @vl_PosteEnrichisseur) and
		   @vl_SiteFmc = @vl_Site
		begin
			/*B supprimer l'evenement de la liste a traiter */
			/*B sur tous les sites                          */
			delete EXP..FMC_TRT
			where evenement = @va_NumEvt_in and cle = @va_CleEvt_in

			if @@rowcount = 0
				return XDC_NOK

			/* positionner l'heure de fin et cloture a la date courante */
			/* sur tous les sites                                */
			update EXP..FMC_GEN set fin=getdate(),
			                        cloture = getdate(),
						zz = zz + 1
			where numero = @va_NumEvt_in and cle = @va_CleEvt_in

			/*B declencher un reveil sur le site local */
			select @vl_TexteReveil = XDC_CHAINE_VIDE,
				@vl_NomMachine = null,
				@vl_NumEvt = 0,
				@vl_CleEvt = 0
			/* PRA
			if @@servername = XDC_SQLCI
			*/
			if @vl_Site = XDC_CI
				exec @vl_Status = TASRV_CI...ARRP03 @vl_NumEvt, 
						@vl_CleEvt, @vl_NomMachine,
						@vl_NomSite, @vl_TexteReveil
			/* PRA
			else if @@servername = XDC_SQLVC
			*/
			else if @vl_Site = XDC_VC
				exec @vl_Status = TASRV_VC...ARRP03 @vl_NumEvt, 
						@vl_CleEvt, @vl_NomMachine,
						@vl_NomSite, @vl_TexteReveil
			/* PRA
			else if @@servername = XDC_SQLDP
			*/
			else if @vl_Site = XDC_DP
				exec @vl_Status = TASRV_DP...ARRP03 @vl_NumEvt,
						@vl_CleEvt, @vl_NomMachine,
						@vl_NomSite, @vl_TexteReveil
			/* PRA
			else if @@servername = XDC_SQLCA
			*/
			else if @vl_Site = XDC_CA
				exec @vl_Status = TASRV_CA...ARRP03 @vl_NumEvt,
						@vl_CleEvt, @vl_NomMachine,
						@vl_NomSite, @vl_TexteReveil
		end
		else 
		begin
			select @va_Resultat_out = XZAEC_FMC_PAS_ENR
			return XDC_OK
		end

	else if @vl_Site = @vl_SiteMaitre
	begin
		/*A cas 1 : site local est le CI */

		/*B verifier que l'evenement n'est pas a traiter sur un district */
		if exists (	select * from EXP..FMC_TRT
				where	evenement =  @va_NumEvt_in and 
					cle = @va_CleEvt_in and
					sit != @vl_SiteMaitre and
					sit in (select numero from CFG..RES_DIS where type != XDC_TYPE_PCNIVEAU3 and type != XDC_TYPE_PCNIVEAU2 and numero != XDC_ASF)
			  )
		begin
			select @va_Resultat_out = XZAEC_FMC_A_TRT
			return XDC_OK
		end
	
		/*B verifier que l'evenement est fini */
		if @vl_Fin = null
		begin	/*B Si c'est une FMC DATEX alors on force sa date de fin sinon erreur*/
			if @vl_Datex = XDC_VRAI
			begin
				update EXP..FMC_GEN
				set zz=zz+1,fin=getdate()
				where	numero = @va_NumEvt_in and 
					cle = @va_CleEvt_in 

				/* Mise a jour de FMC_HIS pour mise a jour synoptique */
				select @vl_date=horodate_validation
				from EXP..FMC_HIS
				where numero = @va_NumEvt_in and
					cle = @va_CleEvt_in
				having numero = @va_NumEvt_in and
					cle = @va_CleEvt_in and
					horodate_validation=max(horodate_validation)
					
				update EXP..FMC_HIS set numero = @va_NumEvt_in
				where numero = @va_NumEvt_in and
					cle = @va_CleEvt_in and
					horodate_validation=@vl_date

			end
			else
			begin
				select @va_Resultat_out = XZAEC_FMC_NON_FIN
				return XDC_OK
			end
		end
	
		/*B supprimer l'evenement de la liste a traiter */
		delete EXP..FMC_TRT
		where	evenement = @va_NumEvt_in and cle = @va_CleEvt_in and
			sit = @vl_SiteMaitre

		/*B positionner l'heure de cloture a la date courante */
		update EXP..FMC_GEN set cloture = getdate(),
					zz = zz + 1
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in
	
		if @@rowcount != 1
			return XDC_NOK

		/*A declencher un reveil sur le site local */
		select	@vl_TexteReveil = XDC_CHAINE_VIDE,
			@vl_NomMachine = null,
			@vl_NumEvt = 0,
			@vl_CleEvt = 0
		
		if @vl_Site = XDC_CI
		exec @vl_Status = TASRV_CI...ARRP03 @vl_NumEvt, @vl_CleEvt, 
					@vl_NomMachine,
					@vl_NomSite, @vl_TexteReveil
		else
		if @vl_Site = XDC_CA
				exec @vl_Status = TASRV_CA...ARRP03 @vl_NumEvt, @vl_CleEvt, 
					@vl_NomMachine,
					@vl_NomSite, @vl_TexteReveil
	end
	else 
	begin
		/*A cas 2 : site local est un district */

		/*B Si l'operateur a le droit d'enrichir alors */
		/*B transmettre l'evenement au CI dans le cas  */
		/*B ou le site local est le site primaire pour */
		/*B l'evenement                                */
		if @vl_Site = @vl_SiteFmc
		begin
			if @vl_PosteEnrichisseur = null or
			   @va_PosteOperateur_in = @vl_PosteEnrichisseur  or
			  (@vl_Fin!=null and @vl_ComFMCInit !=null) 
			begin
				/* transmission de l'evenement au CI */
				exec @vl_Status = XZAE15 @va_NumEvt_in, 
							@va_CleEvt_in,
							@vl_SiteMaitre, 
							null, 
							@vl_Resultat output,
							XDC_FAUX,
							XDC_FAUX,
							@va_NomSiteLocal_in

				if @vl_Status = null
					return XDC_PRC_INC
				else if @vl_Status != XDC_OK
					return @vl_Status
				else if @vl_Resultat != XDC_OK
				begin
					select @va_Resultat_out = @vl_Resultat
					return XDC_OK
				end

			end
			else
			begin
				/* l'operateur n'a pas le droit d'achever la fmc */
				select @va_Resultat_out = XZAEC_FMC_PAS_ENR
				return XDC_OK
			end
		end

		/*B le supprimer de la liste des evenements a traiter */
		/*B pour le site local                                */
		delete EXP..FMC_TRT
		where	evenement = @va_NumEvt_in and 
			cle = @va_CleEvt_in and
			sit = @vl_Site
	
		if @@rowcount != 1
			return XDC_NOK

		/*A declencher un reveil sur le site local */
		select @vl_TexteReveil = XDC_CHAINE_VIDE,
			@vl_NomMachine = null,
			@vl_NumEvt = 0,
			@vl_CleEvt = 0
		/* PRA
		if @@servername = XDC_SQLVC
		*/
		if @vl_Site = XDC_VC
			exec @vl_Status = TASRV_VC...ARRP03 @vl_NumEvt, 
					@vl_CleEvt, @vl_NomMachine,
					@vl_NomSite, @vl_TexteReveil
		/* PRA
		else if @@servername = XDC_SQLDP
		*/
		else if @vl_Site = XDC_DP
			exec @vl_Status = TASRV_DP...ARRP03 @vl_NumEvt, 
					@vl_CleEvt, @vl_NomMachine,
					@vl_NomSite, @vl_TexteReveil
		else if @vl_Site = XDC_CA
			exec @vl_Status = TASRV_CA...ARRP03 @vl_NumEvt, 
					@vl_CleEvt, @vl_NomMachine,
					@vl_NomSite, @vl_TexteReveil
	end

	if @vl_Status = null
		return XDC_PRC_INC
	else if @vl_Status != XDC_OK
		return XDC_REV_PB_DCL

	select @va_Resultat_out = XDC_OK

	return XDC_OK
go
