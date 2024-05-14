						/*E*/
/* Fichier : $Id: xzac906sp.prc,v 1.9 2021/06/10 08:51:53 pc2dpdy Exp $      Release : $Revision: 1.9 $        Date : $Date: 2021/06/10 08:51:53 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac906sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* CGR	12/04/21	: Creation PAU SONO 1.1 DEM-SAE244
* ABE	08/06/21	: Fix Autoroute en str pour acrp80, fix num_action en négatif pour acrp80 DEM-SAE244 1.9
* LCL	27/09/23 : Adaptations CNA DEM-483	
------------------------------------------------------ */

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retour a l'etat normal du RAU SONO par scenario ou par 
* 				commande individuelle
* 
* Sequence d'appel
* SP	XZAC906_Commande_Fin_SONO
* 
* Arguments en entree
* XDY_Horodate	va_HorodateFin_in
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* XDY_Operateur	va_Operateur_in
* XDY_Site	va_Site_in
* XDY_Entier	va_PAU_erreur_in
* XDY_Entier	va_PAU_pilotes_in
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

if exists (select * from sysobjects where name = 'XZAC906' and type = 'P')
	drop procedure XZAC906
go

create procedure XZAC906
	
	@va_HorodateFin_in      datetime,
	@va_NumEvt_in		int,
	@va_CleEvt_in		tinyint,												
	@va_Site_in		tinyint,
	@va_PAU_erreur_out	int output,
	@va_PAU_pilotes_out	int output,
	@va_Priorite_in		tinyint,
	@va_site_origine_in	T_SITE,
	@va_distant_in 		int 			= XDC_NON	
	
as

	declare	@vl_NumeroAction int, 
	    	@vl_Status int, 
	    	@vl_NomMachine char(10),
		@vl_TypeAction tinyint,
		@vl_NumEqtBoucle T_EQUIPEMENT, 
		@vl_TypeEqtBoucle tinyint,
		@vl_NumMaitre T_EQUIPEMENT, 
		@vl_TypeMaitre tinyint,
		@vl_MachineNonTrouvee bit,
		@vl_Site tinyint,
		@vl_serveur_pra char(6),												
		@vl_Sens_in tinyint,
		@vl_PR_debut_in int,				
		@vl_PR_fin_in int,
		@vl_Message varchar(30),								
		@vl_Autoroute_str char(4),
		@vl_Autoroute_tinyint tinyint,
		@vl_Sens tinyint,
		@vl_PR_debut int,
		@vl_PR_fin int,
		@vl_NumEqt int,
		@vl_Commande int

	select @vl_Message="REPOS"

	/*! transfo en parametres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
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

	/* MOVIS execution toujous en local */
	if @vl_Site = XDC_CI and @va_distant_in = XDC_NON		
	/* Traitement au district d'exploitation pour Escota, en local sinon */
	begin
		if @vl_serveur_pra=XDC_SQLDP		
			exec @vl_Status = SQL_DS.PRC..XZAC906 @va_HorodateFin_in,
								@va_NumEvt_in,
								@va_CleEvt_in,
								@va_Site_in,
								@va_PAU_erreur_out output,
								@va_PAU_pilotes_out output,
								@va_Priorite_in,
								@va_site_origine_in,
								XDC_OUI 
		
		else if @vl_serveur_pra=XDC_SQLVC
			exec @vl_Status = SQL_DM.PRC..XZAC906 @va_HorodateFin_in,
								@va_NumEvt_in,
								@va_CleEvt_in,
								@va_Site_in,
								@va_PAU_erreur_out output,
								@va_PAU_pilotes_out output,
								@va_Priorite_in,
								@va_site_origine_in,
								XDC_OUI 
		
		else if @vl_serveur_pra=XDC_SQLCI
			exec @vl_Status = SQL_CI.PRC..XZAC906 @va_HorodateFin_in,
								@va_NumEvt_in,
								@va_CleEvt_in,
								@va_Site_in,
								@va_PAU_erreur_out output,
								@va_PAU_pilotes_out output,
								@va_Priorite_in,
								@va_site_origine_in,
								XDC_OUI  
		else return XDC_NOK
		
		return @vl_Status
	end

	/*A Rechercher la derniere action en cours pour cet equipement */
	select @vl_NumeroAction = GEN.numero,
		@vl_NumEqt = GEN.equipement
	from EXP..ACT_GEN GEN											
	where
		GEN.sit = @va_Site_in and
		GEN.type = XDC_ACT_PAU		
		and GEN.heure_fin is null 
	order by heure_lancement desc


	if @@rowcount = 0
		select @vl_NumeroAction = 0

	/*A Rechercher les info de l'action PAU pour transmettre a tacli */
	select  @vl_Autoroute_tinyint = PAU.autoroute,
		@vl_Sens = PAU.sens,
		@vl_PR_debut = PAU.PR_debut,
		@vl_PR_fin = PAU.PR_fin,
		@va_PAU_erreur_out = PAU.PAUErreur,
		@va_PAU_pilotes_out = PAU.PAUPilotes
	from EXP..ACT_PAU PAU
	where PAU.actio = @vl_NumeroAction

	begin
		/*A initialisation des var locales pour la recherche */
		/*A du nom de la machine pilotant la PAU */
		select	@vl_MachineNonTrouvee = XDC_NON,
			@vl_NumEqtBoucle = @vl_NumEqt,
			@vl_TypeEqtBoucle = XDC_EQT_PAU

		/*A recherche le nom de la machine pilotant la PAU */

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

		/* Trouver le nom d'autoroute */
		select 	@vl_Autoroute_str = nom
		from CFG..RES_AUT
		where numero   = @vl_Autoroute_tinyint

		/*A numero de commande a envoyer : - numero action */
		select @vl_Commande = - @vl_NumeroAction
												
#if ! defined (DEV)
		/*A arreter la PAU */
		if @vl_Site = XDC_CI								
			exec @vl_Status = TASRV_CI...ACRP80 @vl_Message, @vl_Autoroute_str, @vl_Sens, @vl_PR_debut, @vl_PR_fin, @vl_Commande, @vl_NomMachine
		else if @vl_Site = XDC_VC
			exec @vl_Status = TASRV_VC...ACRP80  @vl_Message, @vl_Autoroute_str, @vl_Sens, @vl_PR_debut, @vl_PR_fin, @vl_Commande, @vl_NomMachine
		else if @vl_Site = XDC_DP
			exec @vl_Status = TASRV_DP...ACRP80  @vl_Message, @vl_Autoroute_str, @vl_Sens, @vl_PR_debut, @vl_PR_fin, @vl_Commande, @vl_NomMachine
		else if @vl_Site = XDC_CA
			exec @vl_Status = TASRV_CA...ACRP80  @vl_Message, @vl_Autoroute_str, @vl_Sens, @vl_PR_debut, @vl_PR_fin, @vl_Commande, @vl_NomMachine

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
				and equipement = @vl_NumEqt
		end		
	end


	return XDC_OK

go
