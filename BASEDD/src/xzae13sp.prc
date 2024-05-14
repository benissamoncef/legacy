/*E*/
/*  Fichier : $Id: xzae13sp.prc,v 1.17 2020/11/03 16:13:21 pc2dpdy Exp $      Release : $Revision: 1.17 $        Date : $Date: 2020/11/03 16:13:21 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae13sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.     	06/04/95	: Creation 	V 1.1
* C.T.     	18/12/97	: ajout test pour fmc degrade (1531) V 1.2
* JMG		07/01/97	: gestion PC niveau 2 (dem/1536) 1.3
* C.T.		15/01/98	: Correction la gestion de l'achevement d'une fmc degrade  (1531) (1.4-1.5)
* C.T.		26/02/98	: Ajout de l'achevement d'un bouchon trait� sur un PC niveau 2 et 3 par le CI (1580) 1.6
* C.T.		04/03/98	: Deplacement du transfert des infos dans le cas de fmc degrade (1583) 1.7
* C.T.		25/04/98	: Suppression la gestion de l'achevement d'une fmc degrade  (1638) (1.8)
* P.N.		12/01/99	: Modif pour intergestionnaire (1724) V1.9
* F.Lizot  	27/06/2005 	:  Correction SAGA DEM473 1.10
* P.N.		10/09/2007	: Ajout cas pour DP DEM641 1.11
* JPL		12/01/2009	: Degroupee; appel a XZAE15 renommee (lie a DEM 848) 1.12
* JMG		02/10/09	: secto DEM887 1.13
* JMG		15/05/16	: ajout SAGA 1.14
*  JMG           03/03/17 : regio DEM1220
* LCL		03/07/17	: Modif PRA
* LCL   	22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
* LCL		16/05/23	: Correctif MAGISTRA_RQM-791 : cloture FMC bouchon sur deux sites différents
* LCL		24/08/23	: Correction achevement FMC ASF/ESCOTA au CI
* LCL		18/09/23	: Correction achevement FMC ASF/ESCOTA au CI
* ABK		26/09/23 	: Ajout district CA (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Acheve un bouchon c-a-d les fiches main courante de type 
* tete et queue de bouchon (l'enleve de la liste 
* des fiches a traiter pour le poste operateur).
* 
* Sequence d'appel
* XZAE13_Achever_Bouchon
* 
* Arguments en entree
* XDY_Entier	va_NumEvt_in		: queue de bouchon
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

if exists (select * from sysobjects where name = 'XZAE13' and type = 'P')
	drop procedure XZAE13
go


create procedure XZAE13
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null,
	@va_PosteOperateur_in	T_EQUIPEMENT = null,
	@va_Resultat_out	int = null	output,
	@va_NomSiteLocal_in             char(2)         = null
as
	declare @vl_Site T_SITE, @vl_Classe tinyint,
		@vl_SiteMaitre T_SITE, @vl_SiteFmc T_SITE, @vl_Fin datetime,
		@vl_FinCause datetime, @vl_PosteEnrichisseur T_EQUIPEMENT, 
		@vl_Cause int, @vl_CleCause tinyint,
		@vl_TexteReveil char(100), @vl_Status int,
		@vl_Resultat int, @vl_NomMachine char(10),
		@vl_CauseTete int, @vl_CleCauseTete tinyint,
		@vl_DegradeCause tinyint, @vl_CauseNumFMCInit int, @vl_CauseCleFMCInit tinyint,
		@vl_NumEvt int, @vl_CleEvt tinyint, @vl_NomSite char(2),
		@vl_NumFMCInit int, @vl_CleFMCInit tinyint, @vl_DegradeInit tinyint,
		@vl_NumFMCInitOrig int, @vl_Degrade tinyint, @vl_TypeFMCInit smallint,
		@vl_SiteFmcInit T_SITE, @vl_PosteEnrichisseurInit T_EQUIPEMENT, @vl_Datex tinyint,
		@vl_date datetime, @vl_ComFMCInit char(100),
		@vl_SiteTete T_SITE, @vl_NomSiteTete char(2),
		@vl_SiteQueue T_SITE, @vl_NomSiteQueue char(2)

	/*! transfo en param�tres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_PosteOperateur_in = 0 select @va_PosteOperateur_in = null

	/*A controle des parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
	   	return XDC_ARG_INV

	/* recherche du site local */
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

	/*A verifier l'existance de l'evenement */
	select 
		@vl_Datex		= EXP..FMC_GEN.datex,
		@vl_Fin			= EXP..FMC_GEN.fin,
		@vl_PosteEnrichisseur	= EXP..FMC_GEN.poste_enrichisseur,
		@vl_Cause		= EXP..FMC_GEN.cause,
		@vl_CleCause		= EXP..FMC_GEN.cle_cause,
		@vl_SiteFmc		= EXP..FMC_GEN.sit,
		@vl_Classe             	= CFG..TYP_FMC.classe,
		@vl_Degrade             = EXP..FMC_GEN.degrade,
		@vl_NumFMCInit          = EXP..FMC_GEN.num_fmc_init,
		@vl_CleFMCInit          = EXP..FMC_GEN.cle_fmc_init,
		@vl_ComFMCInit          = EXP..FMC_GEN.com_fmc_init
	from EXP..FMC_GEN, CFG..TYP_FMC
	where	EXP..FMC_GEN.numero = @va_NumEvt_in and 
		EXP..FMC_GEN.cle = @va_CleEvt_in and
		CFG..TYP_FMC.numero = EXP..FMC_GEN.type and
		CFG..TYP_FMC.classe = XZAEC_ClasseBouchon

	if @@rowcount != 1
		return XDC_NOK

	/*A verifier que toutes les actions relatives a l'evenement */
	/*A sont terminees  sur le site local                       */
	/*! (la tete et de la queue ont le meme site de creation :  */
	/*!  vl_CleCause = vl_Cause)                                */
	/*B finir les action pmv datex s'il en existe */
	update EXP..ACT_GEN set heure_fin=getdate()
	where evenement = @va_NumEvt_in and
		cle = @vl_CleCause and
		heure_fin is null and 
		type = XDC_ACT_PMVDAT

	if exists (	select * from EXP..ACT_GEN
			where	(evenement = @va_NumEvt_in or
				 evenement = @vl_Cause) and
				cle = @va_CleEvt_in and
				heure_fin is null and
				sit=@vl_Site)
	begin
		select @va_Resultat_out = XZAEC_FMC_ACT_NON_FIN
		return XDC_OK
	end
	
/* Verifier le cas des fmc degrade et init *
	if @vl_Site = XDC_CI
	begin
		* Verifier que la fmc degrade n'est pas liee a une fmc init degrade *
		if @vl_Degrade = XDC_OUI
		begin
			if @vl_NumFMCInit is not null
			begin
				* rechercher la fmc init *
				select
					@vl_DegradeInit = degrade,
					@vl_NumFMCInitOrig = num_fmc_init,
					@vl_TypeFMCInit = type,
					@vl_SiteFmcInit = sit,
					@vl_PosteEnrichisseurInit = poste_enrichisseur
				from EXP..FMC_GEN
				where numero = @vl_NumFMCInit and cle = @vl_CleFMCInit

				* fmc init inexistante *
				if @@rowcount = 0
				begin
					select @va_Resultat_out = XZAEC_FMC_INIT_INC
					return XDC_OK
				end
				* fmc init degrade *
				else if @vl_DegradeInit = XDC_OUI and @vl_NumFMCInitOrig is not null
				begin
					select @va_Resultat_out = XZAEC_FMC_INIT_DEG
					return XDC_OK
				end
				else if not exists (select * from CFG..TYP_FMC where numero = @vl_TypeFMCInit and
							classe = @vl_Classe)
				begin
					select @va_Resultat_out = XZAEC_FMC_INIT_INCOMP
					return XDC_OK
				end
				else if (@vl_ComFMCInit !=null) and
				        ( (@vl_PosteEnrichisseurInit is not null and
		    			@va_PosteOperateur_in != @vl_PosteEnrichisseurInit) or
		   			@vl_SiteFmcInit != @vl_Site)
				begin
					select @va_Resultat_out = XZAEC_FMC_INIT_PAS_ENR
					return XDC_OK
				end
			end
		end
		* Verifier que la fmc n'est une fmc init d'une fmc degrade non achevee *
		else
		begin
			if exists (	select numero from EXP..FMC_GEN 
					where num_fmc_init = @va_NumEvt_in and 
					cle = @va_CleEvt_in and cloture is null )
			begin
				select @va_Resultat_out = XZAEC_FMC_DEG_ENCOURS
				return XDC_OK
			end
		end
	end
*/

	/* nettoyage de la file d'attente */
	
	delete EXP..EQT_FIL
	where (evenement=@va_NumEvt_in or
		evenement = @vl_Cause)
	  	and cle=@va_CleEvt_in

	/* initialisation variable locale */
	select @vl_SiteMaitre = numero from CFG..RES_DIS where type = XDC_TYPEM_SD

#ifdef CI
	/*A cas 1 : site local est le CI */

	/*A verifier que l'evenement n'est pas a traiter sur un district */
	if exists (	select * from EXP..FMC_TRT
			where	(evenement =  @va_NumEvt_in or
				 evenement = @vl_Cause) and 
				cle = @va_CleEvt_in and
				sit != @vl_SiteMaitre and 
				sit in ( select numero from CFG..RES_DIS where type <> XDC_TYPE_PCNIVEAU3 and type <> XDC_TYPE_PCNIVEAU2 and numero != XDC_ASF))
	begin
		select @va_Resultat_out = XZAEC_FMC_A_TRT
		return XDC_OK
	end

	/*A recherche la date de fin de l'evenement tete de bouchon */
	select 
		@vl_FinCause = fin
	from EXP..FMC_GEN
	where	EXP..FMC_GEN.numero = @vl_Cause and 
		EXP..FMC_GEN.cle = @vl_CleCause

	if @@rowcount != 1
		return XDC_NOK

	/*A verifier que la tete et la queue sont finies */
	if @vl_Fin is null 
	begin
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
	
	if @vl_FinCause is null
	begin
		if @vl_Datex = XDC_VRAI
		begin
			update EXP..FMC_GEN
			set zz=zz+1,fin=getdate()
			where	numero = @vl_Cause and 
				cle = @vl_CleCause
		end
		else
		begin
			select @va_Resultat_out = XZAEC_FMC_NON_FIN
			return XDC_OK
		end
	end

/* si le bouchon n'est pas de type degrade avec lien et a une cause de type degrade *
	* liee a une fmc init alors positionner la cause a la fmc init *
	if @vl_NumFMCInit is null
	begin
		select 
			@vl_CauseTete = cause,
			@vl_CleCauseTete = cle_cause
		from EXP..FMC_GEN
		where numero = @vl_Cause and cle = @va_CleEvt_in

		*si une cause existe *
		if @vl_CauseTete is not null and @vl_CleCauseTete is not null
		begin
			* rechercher si la cause est de type degrade *
			select
				@vl_DegradeCause = degrade,
				@vl_CauseNumFMCInit = num_fmc_init,
				@vl_CauseCleFMCInit = cle_fmc_init
			from EXP..FMC_GEN
			where numero = @vl_CauseTete  and cle = @vl_CleCauseTete

			if @vl_DegradeCause = XDC_OUI and @vl_CauseNumFMCInit is not null
				update EXP..FMC_GEN set cause = @vl_CauseNumFMCInit, cle_cause = @vl_CauseCleFMCInit, zz = zz+1
				where numero = @vl_Cause and cle = @va_CleEvt_in
		end
	end


	*A transferer la fmc si elle est de type degrade avec une fmc initiale *
	*A avant la date de cloture en cas de pb au transfert : sinon fmc disparait de la liste sans transfert*
	if @vl_Degrade = XDC_OUI and @vl_NumFMCInit is not null
	begin
		exec @vl_Status = XZAE12  @va_NumEvt_in, @va_CleEvt_in

		if @vl_Status != XDC_OK
			 return XDC_NOK
	end
*/

	/*A supprimer l'evenement de la liste a traiter */
	delete EXP..FMC_TRT
	where	(evenement = @va_NumEvt_in or evenement = @vl_Cause) and 
		cle = @va_CleEvt_in and sit = @vl_SiteMaitre

	/*A positionner l'heure de cloture a la date courante */
	update EXP..FMC_GEN set cloture = getdate(),
				zz = zz + 1
	where (numero = @va_NumEvt_in or numero = @vl_Cause) and 
		cle = @va_CleEvt_in

	if @@rowcount = 0
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

#else
	/*A cas 2 : site local est un district */

	/*A Si l'operateur a le droit d'enrichir alors   */
	/*A transmettre les evenements au CI dans le cas */
	/*A ou le site local est le site primaire pour   */
	/*A les evenements tete et queue                 */
	if (@vl_Site = @vl_SiteFmc) or ( @vl_Site=XDC_DP and @vl_SiteFmc=XDC_ASF)
	begin
		if @vl_PosteEnrichisseur = null or
		   @va_PosteOperateur_in = @vl_PosteEnrichisseur 
		   or @vl_ComFMCInit != null
		begin
			/* transmission des evenements au CI */
			select @vl_SiteQueue = sit
			from EXP..FMC_GEN
			where EXP..FMC_GEN.numero = @va_NumEvt_in and
				  EXP..FMC_GEN.cle = @va_CleEvt_in

			select @vl_NomSiteQueue = code
			from CFG..RES_DIS
			where numero = @vl_SiteQueue

			exec @vl_Status = XZAE15 @va_NumEvt_in, 
						@va_CleEvt_in,
						@vl_SiteMaitre, null, 
						@vl_Resultat output
			if @vl_Status = null
				return XDC_PRC_INC
			else if @vl_Status != XDC_OK
				return @vl_Status
			else if @vl_Resultat != XDC_OK
			begin
				select @va_Resultat_out = @vl_Resultat
				return XDC_OK
			end

			select @vl_SiteTete = sit
			from EXP..FMC_GEN
			where EXP..FMC_GEN.numero = @vl_Cause and
				  EXP..FMC_GEN.cle = @vl_CleCause

			select @vl_NomSiteTete = code
			from CFG..RES_DIS
			where numero = @vl_SiteTete

			exec @vl_Status = XZAE15 @vl_Cause, 
						@vl_CleCause,
						@vl_SiteMaitre, null, 
						@vl_Resultat output,
						null, null,
						@vl_NomSiteTete
			if @vl_Status != XDC_OK
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

	/*A le supprimer de la liste des evenements a traiter */
	/*A pour le site local                                */
	delete EXP..FMC_TRT
	where	(evenement = @va_NumEvt_in or evenement = @vl_Cause) and 
		cle = @va_CleEvt_in and sit = @vl_Site

	if @@rowcount = 0
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
#endif

	if @vl_Status = null
		return XDC_PRC_INC
	else if @vl_Status != XDC_OK
		return XDC_REV_PB_DCL

	select @va_Resultat_out = XDC_OK

	return XDC_OK
go
