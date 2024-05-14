/* Fichier : $Id: xzac3031sp.prc
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzac3031.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* GGY	13/11/23	: Creation (DEM483)
------------------------------------------------------ */

/*-----------------------------------------------------
* Service rendu
* Retour a l'etat normal de la PIC par scenario ou par 
* commande individuelle
* 
* Sequence d'appel
* SP	XZAC3031_Commande_Fin_PIC
* 
* Arguments en entree
* XDY_Horodate		va_HorodateFin_in
* XDY_Entier		va_NumEvt_in
* XDY_Octet			va_CleEvt_in
* XDY_Operateur		va_Operateur_in
* XDY_Eqt			va_NumEqt_in
* XDY_Site			va_Site_in
* XDY_Bandeau_PIC	va_Bandeau_in
* XDY_Booleen		va_Flash_in
* XDY_Picto			va_Picto_in
* XDY_Octet			va_Priorite_in
* XDY_Site			va_site_origine_in
* 
* Arguments en sortie
* XDY_Entier		NumeroAction
* 
* Code retour
* XDC_OK
* XDC_NOK		: site incorrect
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
* 	ACRP16
--------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAC3031' and type = 'P')
	drop procedure XZAC3031
go

create procedure XZAC3031
	
	@va_HorodateFin_in      datetime,
	@va_NumEvt_in			int,
	@va_CleEvt_in			tinyint,
	@va_Operateur_in		smallint,
	@va_NumEqt_in			smallint,
	@va_Site_in				tinyint,
	@va_Bandeau_in			char(XDC_LGR_Bandeau_PIC),	
	@va_Flash_in			bit,		
	@va_Picto_in			char(XDC_LGR_Picto_PIC),			
	@va_Priorite_in			tinyint,
	@va_site_origine_in		T_SITE,
	@va_NumeroAction_out		int		= null	output,
	@va_distant_in 			int 		= XDC_NON
	
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
			@vl_bandeau varchar(15),
			@vl_bandeau2 varchar(15),
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

	/* MOVIS execution toujours en local */
	if @va_site_origine_in = null
		select  @vl_Site = numero
			from CFG..RES_DIS
	        	where serveur = @@servername
	else
		if @va_distant_in = XDC_NON
		begin
			select @vl_Site = @va_site_origine_in
		end
		else
		begin
			select @vl_Site = @va_Site_in
		end

	if @vl_Site is null
		return XDC_NOK
	
	/* Init serveur PRA */
	select @vl_serveur_pra=serveur_pra from CFG..RES_PRA where numero=@va_Site_in

	if @vl_Site = XDC_CI and @va_distant_in = XDC_NON	
	/* Traitement sur district Exploit pour Escota, en local sinon */
	begin
		if @vl_serveur_pra = XDC_SQLDP
			exec @vl_Status = SQL_DS.PRC..XZAC3031	@va_HorodateFin_in, @va_NumEvt_in, @va_CleEvt_in,
													@va_Operateur_in, @va_NumEqt_in, @va_Site_in,
													@va_Bandeau_in, @va_Flash_in, @va_Picto_in,
													@va_Priorite_in,
													@va_site_origine_in,
													@va_NumeroAction_out output,
													XDC_OUI
		
		else if @vl_serveur_pra = XDC_SQLVC
			exec @vl_Status = SQL_DM.PRC..XZAC3031	@va_HorodateFin_in, @va_NumEvt_in, @va_CleEvt_in,
													@va_Operateur_in, @va_NumEqt_in, @va_Site_in,
													@va_Bandeau_in, @va_Flash_in, @va_Picto_in,
													@va_Priorite_in,
													@va_site_origine_in,
													@va_NumeroAction_out output,
													XDC_OUI
		else if @vl_serveur_pra = XDC_SQLCI
			exec @vl_Status = SQL_CI.PRC..XZAC303	@va_HorodateFin_in, @va_NumEvt_in, @va_CleEvt_in,
													@va_Operateur_in, @va_NumEqt_in, @va_Site_in,
													@va_Bandeau_in, @va_Flash_in, @va_Picto_in,
													@va_Priorite_in,
													@va_site_origine_in,
													@va_NumeroAction_out output,
													XDC_OUI
		else
			return XDC_NOK
		
		return @vl_Status
	end

	/* Verifier la concordance des sites */
	if @va_Site_in <> @vl_Site
		return XDC_NOK

	/* Rechercher la derniere action en cours pour cet equipement */
	select @vl_NumeroAction = max(numero)
		from EXP..ACT_GEN
			where	EXP..ACT_GEN.sit = @va_Site_in and
					EXP..ACT_GEN.type = XDC_ACT_PIC and
					EXP..ACT_GEN.heure_fin is null and 
					EXP..ACT_GEN.equipement = @va_NumEqt_in

	if @@rowcount = 0
		select @vl_NumeroAction = 0

	/* numero d'action a retourner */
	select @va_NumeroAction_out = @vl_NumeroAction

	begin
		/* initialisation des var locales pour la recherche */
		/* du nom de la machine pilotant la PIC */
		select	@vl_MachineNonTrouvee = XDC_NON,
				@vl_NumEqtBoucle = @va_NumEqt_in,
				@vl_TypeEqtBoucle = XDC_EQT_PIC

		/* recherche le nom de la machine pilotant la PIC */
		while @vl_MachineNonTrouvee = XDC_NON
		begin
			select
				@vl_NumMaitre = GEN.maitre,
				@vl_TypeMaitre = GEN.type_maitre,
				@vl_NomMachine = GEN.nom
					from CFG..EQT_GEN GEN
						where	GEN.numero = @vl_NumEqtBoucle and 
								GEN.type = @vl_TypeEqtBoucle 
	
			if @vl_NumMaitre = null or @@rowcount != 1 
				return XDC_NOK

			/* test si c'est la machine qui pilote */
			if @vl_TypeMaitre = XDC_EQT_MAC
			begin
				select @vl_NomMachine = nom
					from CFG..EQT_GEN
						where numero = @vl_NumMaitre and type = @vl_TypeMaitre

				select @vl_MachineNonTrouvee = XDC_OUI
			end
			else
				select	@vl_NumEqtBoucle = @vl_NumMaitre,
						@vl_TypeEqtBoucle = @vl_TypeMaitre
		end

		if @vl_MachineNonTrouvee = XDC_NON
			return XDC_NOK

		/* PIC Pleine Voie on justifie le texte Bandeau */
		select @vl_bandeau = substring( replicate(" ", ( 9 - char_length( ltrim( rtrim( @va_Bandeau_in ) ) ) ) / 2 ) +  ltrim( rtrim( @va_Bandeau_in ) ) + "          ",1 ,9)

		/* numero de commande a envoyer : - numero action */
		select @vl_Commande = - @vl_NumeroAction

		select @vl_bandeau2 = substring(@vl_bandeau,1,15)


#if ! defined (DEV)
		/*A arreter la PIC */
		if @vl_Site = XDC_CI
			exec @vl_Status = TASRV_CI...ACRP16 @va_NumEqt_in, @vl_bandeau2, @va_Flash_in, @va_Picto_in,
			@vl_Commande, @vl_NomMachine

		else if @vl_Site = XDC_VC
			exec @vl_Status = TASRV_VC...ACRP16 @va_NumEqt_in, @vl_bandeau2,@va_Flash_in, @va_Picto_in,
			@vl_Commande, @vl_NomMachine

		else if @vl_Site = XDC_DP
			exec @vl_Status = TASRV_DP...ACRP16 @va_NumEqt_in, @vl_bandeau2,@va_Flash_in, @va_Picto_in,
			@vl_Commande, @vl_NomMachine

		else if @vl_Site = XDC_CA
			exec @vl_Status = TASRV_CA...ACRP16 @va_NumEqt_in, @vl_bandeau2,@va_Flash_in, @va_Picto_in,
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
			where	numero = @vl_NumeroAction and 
					sit = @va_Site_in and
					heure_lancement_fin is null
					and equipement = @va_NumEqt_in
		end		
	end

	return XDC_OK
go
