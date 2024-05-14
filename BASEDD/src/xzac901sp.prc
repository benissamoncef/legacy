/*E*/
/* Fichier : $Id: xzac901sp.prc,v 1.3 2021/03/08 14:14:01 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2021/03/08 14:14:01 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzac901.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL	02/03/18	: Creation controleurs de feux CTRL_FEUX 1.1 DEM1284
* LCL	20/11/19	: MOVIS DEM-SAE93 1.2
* GRO	12/10/2020      : MOVIS DEM-SAE93 1.3
* ABE	05/01/21   	: MOVIS ajout serveur PRA DEM-SAE93 1.4
* LCL 	03/10/23 	: Adaptations CNA DEM-483	
------------------------------------------------------ */

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retour a l'etat normal de la CFE par scenario ou par 
* commande individuelle
* 
* Sequence d'appel
* SP	XZAC901_Commande_Fin_CFE
* 
* Arguments en entree
* XDY_Horodate	va_HorodateFin_in
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* XDY_Operateur	va_Operateur_in
* XDY_Eqt	va_NumEqt_in
* XDY_Site	va_Site_in
* XDY_Octet	va_PlanDeFeux_in
* XDY_Octet	va_Priorite_in
* XDY_Site	va_site_origine_in
* 
* Arguments en sortie
* XDY_Entier		NumeroAction
* 
* Code retour
* XDC_OK
* XDC_NOK	: site incorrect
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* L'execution de la commande de retour a l'etat normal 
* est executee si Manuelle est positionne a XDC_Faux
* 
* Fonction
* Inserer dans la table ACTION la date de fin de l'action.
* 
* Dans le cas de commande automatique par scenario :
* 	ACRP15 
--------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAC901' and type = 'P')
	drop procedure XZAC901
go

create procedure XZAC901
	
	@va_HorodateFin_in      datetime,
	@va_NumEvt_in			int,
	@va_CleEvt_in			tinyint,
	@va_Operateur_in		smallint,
	@va_NumEqt_in			smallint,
	@va_Site_in			tinyint,
	@va_PlanDeFeux_in		tinyint,
	@va_LibellePlanDeFeux_in		char(150),
	@va_Priorite_in			tinyint,
	@va_site_origine_in		T_SITE,
	@va_NumeroAction_out		int		= null	output,
	@va_distant_in 			int = XDC_NON		
	
as

	declare	@vl_NumeroAction int, 
	    @vl_Status int, 
	    @vl_NomMachine char(10),
		@vl_Commande int, @vl_TypeAction tinyint,
		@vl_NumEqtBoucle T_EQUIPEMENT, 
		@vl_TypeEqtBoucle tinyint,
		@vl_NumMaitre T_EQUIPEMENT, 
		@vl_TypeMaitre tinyint,
		@vl_MachineNonTrouvee bit,
		@vl_Site tinyint,
		@vl_serveur_pra char(6)	

	/*! transfo en parametres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_Operateur_in = 0 select @va_Operateur_in = null
	if @va_NumEqt_in = 0 select @va_NumEqt_in = null
	if @va_Site_in = 0 select @va_Site_in = null
	if @va_HorodateFin_in = XDC_DATE_NULLE select @va_HorodateFin_in = null

	/*A controle des parametres obligatoires */
	if @va_HorodateFin_in = null
		return XDC_ARG_INV

        /*A Determiner le site local */
        /* MOVIS */
	if @va_site_origine_in = null
	        select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername
	else
        begin
                if @va_distant_in = XDC_NON
                begin
                    select @vl_Site = @va_site_origine_in
                end
                else
                begin
                    select @vl_Site = @va_Site_in
                end
        end

	if @vl_Site is null
		return XDC_NOK

	/* Init serveur PRA */
	select @vl_serveur_pra=serveur_pra from CFG..RES_PRA where numero=@va_Site_in

	/* MOVIS execution toujous en local
	if @vl_Site = XDC_CI and @va_distant_in = XDC_NON		
	begin
		if @vl_serveur_pra=XDC_SQLDP
			exec @vl_Status = SQL_DS.PRC..XZAC901 @va_HorodateFin_in, @va_NumEvt_in, @va_CleEvt_in,
										@va_Operateur_in, @va_NumEqt_in, @va_Site_in,
										@va_PlanDeFeux_in,
										@va_LibellePlanDeFeux_in,
										@va_Priorite_in,
										@va_site_origine_in,
										@va_NumeroAction_out output, 
										XDC_OUI 
		
		else if @vl_serveur_pra=XDC_SQLVC
			exec @vl_Status = SQL_DM.PRC..XZAC901 @va_HorodateFin_in, @va_NumEvt_in, @va_CleEvt_in,
										@va_Operateur_in, @va_NumEqt_in, @va_Site_in,
										@va_PlanDeFeux_in,
										@va_LibellePlanDeFeux_in,
										@va_Priorite_in,
										@va_site_origine_in,
										@va_NumeroAction_out output, 
										XDC_OUI 
		
		else if @vl_serveur_pra=XDC_SQLCI
			exec @vl_Status = SQL_CI.PRC..XZAC901 @va_HorodateFin_in, @va_NumEvt_in, @va_CleEvt_in,
										@va_Operateur_in, @va_NumEqt_in, @va_Site_in,
										@va_PlanDeFeux_in,
										@va_LibellePlanDeFeux_in,
										@va_Priorite_in,
										@va_site_origine_in,
										@va_NumeroAction_out output, 
										XDC_OUI 
		else return XDC_NOK
		
		return @vl_Status
	end
	*/


	/* Verifier la concordance des sites */
	if @va_Site_in <> @vl_Site
		return XDC_NOK


	/*A Rechercher la derniere action en cours pour cet equipement */
	select @vl_NumeroAction = max(numero)
	from EXP..ACT_GEN
	where
		EXP..ACT_GEN.sit = @va_Site_in and
		EXP..ACT_GEN.type = XDC_ACT_CFE
		and EXP..ACT_GEN.heure_fin is null 
		and EXP..ACT_GEN.equipement = @va_NumEqt_in

	if @@rowcount = 0
		select @vl_NumeroAction = 0

	/*A numero d'action a retourner */
	select @va_NumeroAction_out = @vl_NumeroAction

	begin
		/*A initialisation des var locales pour la recherche */
		/*A du nom de la machine pilotant la CFE */
		select	@vl_MachineNonTrouvee = XDC_NON,
			@vl_NumEqtBoucle = @va_NumEqt_in,
			@vl_TypeEqtBoucle = XDC_EQT_CFE

		/*A recherche le nom de la machine pilotant la CFE */

		while @vl_MachineNonTrouvee = XDC_NON
		begin
			select
				@vl_NumMaitre = GEN.maitre,
				@vl_TypeMaitre = GEN.type_maitre,
				@vl_NomMachine = GEN.nom
			from CFG..EQT_GEN GEN
			where GEN.numero = @vl_NumEqtBoucle 
				and GEN.type = @vl_TypeEqtBoucle 
	
			if @vl_NumMaitre = null or @@rowcount != 1 
				return XDC_NOK

			/*A test si c'est la machine qui pilote */
			if @vl_TypeMaitre = XDC_EQT_MAC
			begin
				select @vl_NomMachine = nom
				from CFG..EQT_GEN
				where numero = @vl_NumMaitre and type = @vl_TypeMaitre

				select @vl_MachineNonTrouvee = XDC_OUI
			end
			else	select	@vl_NumEqtBoucle = @vl_NumMaitre,
					@vl_TypeEqtBoucle = @vl_TypeMaitre

		end
		

		if @vl_MachineNonTrouvee = XDC_NON
			return XDC_NOK


		/*A numero de commande a envoyer : - numero action */
		select @vl_Commande = - @vl_NumeroAction

#if ! defined (DEV)
		/*A arreter la CFE */
		if @vl_Site = XDC_CI
			exec @vl_Status = TASRV_CI...ACRP60 @va_NumEqt_in, @va_PlanDeFeux_in,
			@vl_Commande, @vl_NomMachine
		else if @vl_Site = XDC_VC
			exec @vl_Status = TASRV_VC...ACRP60 @va_NumEqt_in, @va_PlanDeFeux_in,
			@vl_Commande, @vl_NomMachine
		else if @vl_Site = XDC_DP
			exec @vl_Status = TASRV_DP...ACRP60 @va_NumEqt_in, @va_PlanDeFeux_in,
			@vl_Commande, @vl_NomMachine
		else if @vl_Site = XDC_CA
			exec @vl_Status = TASRV_CA...ACRP60 @va_NumEqt_in, @va_PlanDeFeux_in,
			@vl_Commande, @vl_NomMachine

		if @vl_Status != XDC_OK
			return @vl_Status
#endif	/* pas en mode DEV */

			
		/*A mettre a jour la date de lancement fin s'il existe une action a terminee */
		
		if @vl_NumeroAction != 0
		begin
			update EXP..ACT_GEN set heure_lancement_fin = getdate()
			/*! mettre a jour la date de fin, que ce soit en mode DEV ou non */
			                      , heure_fin = getdate()
			where	numero = @vl_NumeroAction and sit = @va_Site_in and
				heure_lancement_fin is null
				and equipement = @va_NumEqt_in
		end		
	end


	return XDC_OK

go
