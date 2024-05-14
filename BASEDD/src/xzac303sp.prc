/*E*/
/* Fichier : $Id: xzac303sp.prc,v 1.8 2021/03/08 14:36:15 pc2dpdy Exp $      Release : $Revision: 1.8 $        Date : $Date: 2021/03/08 14:36:15 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzac303.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	05/03/12	: Creation (DEM/1016)
* JMG	11/11/12	: ajout numero action out
* PNI           19/04/16 : Correction des appels dans PRC des XZAC sur les serveurs sites v1.3 DEM1159
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.4
* LCL	xx/xx/17	: Modif PRA
* JPL	01/09/17	: En mode DEV, pas d'envoi de commande a l'equipement et action mise en fin (DEM 1231)  1.6
* LCL	19/11/19	: MOVIS DEM-SAE93 1.7
* LPE	12/10/20 	: MOVIS DEM-SAE93 1.7
* CGR	05/01/21	: MOVIS ajout PRA DEM-SAE93 1.8
* LCL	28/09/23 : Adaptations CNA DEM-483	
------------------------------------------------------ */

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retour a l'etat normal de la PRV par scenario ou par 
* commande individuelle
* 
* Sequence d'appel
* SP	XZAC303_Commande_Fin_PRV
* 
* Arguments en entree
* XDY_Horodate	va_HorodateFin_in
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* XDY_Operateur	va_Operateur_in
* XDY_Eqt	va_NumEqt_in
* XDY_Site	va_Site_in
* XDY_Bandeau	va_Bandeau_in
* XDY_VitesseRegul	Vitesse_in
* XDY_Octet	va_Flash_in
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

if exists (select * from sysobjects where name = 'XZAC303' and type = 'P')
	drop procedure XZAC303
go

create procedure XZAC303
	
	@va_HorodateFin_in      datetime,
	@va_NumEvt_in			int,
	@va_CleEvt_in			tinyint,
	@va_Operateur_in		smallint,
	@va_NumEqt_in			smallint,
	@va_Site_in			tinyint,
	@va_Bandeau_in			char(10),
	@va_Vitesse_in			char(6),
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
		@vl_TypePRV tinyint,
		@vl_MachineNonTrouvee bit,
		@vl_bandeau varchar(10),
		@vl_bandeau2 varchar(10),
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
			exec @vl_Status = SQL_DS.PRC..XZAC303 @va_HorodateFin_in, @va_NumEvt_in, @va_CleEvt_in,
										@va_Operateur_in, @va_NumEqt_in, @va_Site_in,
										@va_Bandeau_in,@va_Vitesse_in,
										@va_Priorite_in,
										@va_site_origine_in,
										@va_NumeroAction_out output,
										XDC_OUI
		
		else if @vl_serveur_pra = XDC_SQLVC
			exec @vl_Status = SQL_DM.PRC..XZAC303 @va_HorodateFin_in, @va_NumEvt_in, @va_CleEvt_in,
										@va_Operateur_in, @va_NumEqt_in, @va_Site_in,
										@va_Bandeau_in,@va_Vitesse_in,
										@va_Priorite_in,
										@va_site_origine_in,
										@va_NumeroAction_out output,
										XDC_OUI
		else if @vl_serveur_pra = XDC_SQLCI
                        exec @vl_Status = SQL_CI.PRC..XZAC303 @va_HorodateFin_in, @va_NumEvt_in, @va_CleEvt_in,
                                                                                @va_Operateur_in, @va_NumEqt_in, @va_Site_in,
                                                                                @va_Bandeau_in,@va_Vitesse_in,
                                                                                @va_Priorite_in,
                                                                                @va_site_origine_in,
                                                                                @va_NumeroAction_out output,
                                                                                XDC_OUI
		
		else return XDC_NOK
		
		return @vl_Status
	end

	/* Verifier la concordance des sites */
	if @va_Site_in <> @vl_Site
		return XDC_NOK


	/*A Rechercher la derniere action en cours pour cet equipement */
	select @vl_NumeroAction = max(numero)
	from EXP..ACT_GEN
	where
		EXP..ACT_GEN.sit = @va_Site_in and
		EXP..ACT_GEN.type = XDC_ACT_PRV
		and EXP..ACT_GEN.heure_fin is null 
		and EXP..ACT_GEN.equipement = @va_NumEqt_in

	if @@rowcount = 0
		select @vl_NumeroAction = 0

	/*A numero d'action a retourner */
	select @va_NumeroAction_out = @vl_NumeroAction

	begin
		/*A initialisation des var locales pour la recherche */
		/*A du nom de la machine pilotant la PRV */
		select	@vl_MachineNonTrouvee = XDC_NON,
			@vl_NumEqtBoucle = @va_NumEqt_in,
			@vl_TypeEqtBoucle = XDC_EQT_PRV,
			@vl_TypePRV = XDC_PRV_PLEINE_VOIE

		select @vl_TypePRV = PRV.type
		from CFG..EQT_PRV PRV
		where numero = @vl_NumEqtBoucle

		/*A recherche le nom de la machine pilotant la PRV */

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


		/*A Si PRV Pleine Voie on justifie le texte Bandeau */
		if @vl_TypePRV = XDC_PRV_PLEINE_VOIE
			select @vl_bandeau = substring( replicate(" ", ( 9 - char_length( ltrim( rtrim( @va_Bandeau_in ) ) ) ) / 2 ) +  ltrim( rtrim( @va_Bandeau_in ) ) + "          ",1 ,9)
		else
			select @vl_bandeau = @va_Bandeau_in

		/*A numero de commande a envoyer : - numero action */
		select @vl_Commande = - @vl_NumeroAction

		select @vl_bandeau2 = substring(@vl_bandeau,1,10)


#if ! defined (DEV)
		/*A arreter la PRV */
		if @vl_Site = XDC_CI
			exec @vl_Status = TASRV_CI...ACRP15 @va_NumEqt_in, @vl_bandeau2,@va_Vitesse_in,
			@vl_Commande, @vl_NomMachine

		else if @vl_Site = XDC_VC
			exec @vl_Status = TASRV_VC...ACRP15 @va_NumEqt_in, @vl_bandeau2,@va_Vitesse_in,
			@vl_Commande, @vl_NomMachine

		else if @vl_Site = XDC_DP
			exec @vl_Status = TASRV_DP...ACRP15 @va_NumEqt_in, @vl_bandeau2,@va_Vitesse_in,
			@vl_Commande, @vl_NomMachine

		else if @vl_Site = XDC_CA
			exec @vl_Status = TASRV_CA...ACRP15 @va_NumEqt_in, @vl_bandeau2,@va_Vitesse_in,
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
