/*E*/
/* Fichier : $Id: xzac905sp.prc,v 1.10 2021/06/10 09:22:23 pc2dpdy Exp $      Release : $Revision: 1.10 $        Date : $Date: 2021/06/10 09:22:23 $
-----------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac905sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	06/04/21 	: Creation PAU SONO 1.1 DEM-SAE244
* ABE	08/06/21	: Fix sens typer pour acrp80 DEM-SAE244 1.9
* LCL	27/09/23 : Adaptations CNA DEM-483	
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Action d'affichage PAU.
* 
* Sequence d'appel
* SP	XZAC905_Commande_PAU
* 
* Arguments en entree
* XDY_Horodate	va_Horodate_in
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* XDY_Operateur	va_Operateur_in
* XDY_Site	va_Site_in
* XDY_MessageSono va_Message_in
* XDY_Texte	va_Autoroute_in
* XDY_Entier	va_Sens_in
* XDY_Entier	va_PR_debut_in
* XDY_Entier	va_PR_fin_in
* XDY_Entier	va_PAU_erreur_in
* XDY_Entier	va_PAU_pilotes_in
* XDY_Octet	va_Priorite_in
* XDY_Entier	va_DistanceEvt_in
* XDY_Site	va_site_origine_in
*
* Arguments en sortie
* XDY_Entier	va_NumeroAction_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve
* XDC_ARG_INV	: parametres d'entree null
* XDC_REV_PB_DCL: probleme pour declencher le reveil
* XDC_PRC_INC	: ARRP03 inexistante
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Si l'action a ete proposee par le plan d'actions, positionner 
* Priorite et DistanceEvt, sinon, mettre DistanceEvt a XDC_DISTANCE_INCONNUE
* (Priorite sera mise automatiquement a null)
* 
* Fonction
* Verifier l'existence de l'evenement
* Verifier l'existence d'un equipement non indisponible 
* dans le district ou l'action est executee./A
* 
* Inserer dans la table ACTION et la table E_PAU.
* 
* Chercher l'action precedente sur l'equipement a commander :
* 	SELECT numero FROM ACTION 
* 	WHERE equipement, type, heure_lancement_fin = null, 
* 		heure_lancement = all
* 		(SELECT max(heure_lancement) FROM ACTION WHERE equipement, type)
* 	
* 	Positionner heure_lancement_fin a Horodate pour l'action precedente
* 	Si l'action precedente trouvee est liee a un autre 
* 	evenement alors l'autre evenement doit 	etre reveille 
* 	pour l'avertir du vol (appeler XZAR03).
* 
* ACRP905 :	Appeler XZEP09 pour piloter l'equipement SONO
* 
-----------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAC905' and type = 'P')
	drop procedure XZAC905
go

create procedure XZAC905
	@va_Horodate_in         datetime,
	@va_NumEvt_in		int,
	@va_CleEvt_in		tinyint,
	@va_Operateur_in	smallint,
	@va_Site_in		tinyint,
	@va_Message_in		char(30),								
	@va_Autoroute_in	char(4),												
	@va_Sens_in		tinyint,
	@va_PR_debut_in		int,
	@va_PR_fin_in		int,
	@va_PAU_erreur_in	int,
	@va_PAU_pilotes_in	int,
	@va_Priorite_in		tinyint,
	@va_site_origine_in	T_SITE,
	@va_DistanceEvt_in	int,
	@va_NumeroAction_out	int			= null	output,
	@va_distant_in 		int 			= XDC_NON
as
	declare @vl_Site tinyint,
		@vl_NomSite char(2), 
	 	@vl_NumActionPrecedente int,
		@vl_NumEvtActionPrecedente int, 
		@vl_CleEvtActionPrecedente tinyint,
		@vl_HeureSucces datetime,
		@vl_HeureEchec datetime,
		@vl_EtatPbMineur int,
		@vl_NomMachine char(10),
		@vl_Status int, 
		@vl_Texte char(100), 
		@vl_NomEqt char(10), 
		@vl_NumMaitre T_EQUIPEMENT, 
		@vl_TypeMaitre tinyint,
		@vl_TypeEqt tinyint, 
		@vl_NumEqt T_EQUIPEMENT, 
		@vl_NumEqtTemp T_EQUIPEMENT, 
		@vl_MachineNonTrouvee bit,
		@vl_Autoroute T_AUTOROUTE, 
		@vl_PR T_PR,
		@vl_Sens T_SENS, 
		@vl_Intitule varchar(50),
		@vl_BandeauAffiche	varchar(10),
		@vl_serveur_pra char(6),
		@vl_A57 tinyint,
		@vl_varMessage varchar(30),
		@vl_varAutoroute varchar(4),
		@vl_varNomMachine varchar(10)

	/* récupération valeur A57*/
	select @vl_A57 = numero from CFG..RES_AUT where nom="A57" 

	/* recherche le site local */
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

	/*A On est au CI et on pilote un autre district */
	if @vl_Site != @va_Site_in and @vl_Site = XDC_CI and @va_distant_in = XDC_NON		
	/* Traitement au district d'exploitation pour Escota, en local sinon */
	begin
		if @vl_serveur_pra=XDC_SQLDP	
			exec @vl_Status = SQL_DS.PRC..XZAC905 @va_Horodate_in,
						@va_NumEvt_in,
						@va_CleEvt_in,
						@va_Operateur_in,
						@va_Site_in,
						@va_Message_in,
						@va_Autoroute_in,
						@va_Sens_in,
						@va_PR_debut_in,
						@va_PR_fin_in,
						@va_PAU_erreur_in,
						@va_PAU_pilotes_in,
						@va_Priorite_in,
						@va_site_origine_in,
						@va_DistanceEvt_in,
						@va_NumeroAction_out output, 
						XDC_OUI 
		else if @vl_serveur_pra=XDC_SQLVC
			exec @vl_Status = SQL_DM.PRC..XZAC905 @va_Horodate_in,
						@va_NumEvt_in,
						@va_CleEvt_in,
						@va_Operateur_in,
						@va_Site_in,
						@va_Message_in,
						@va_Autoroute_in,
						@va_Sens_in,
						@va_PR_debut_in,
						@va_PR_fin_in,
						@va_PAU_erreur_in,
						@va_PAU_pilotes_in,
						@va_Priorite_in,
						@va_site_origine_in,
						@va_DistanceEvt_in,
						@va_NumeroAction_out output, 
						XDC_OUI  
		else if @vl_serveur_pra=XDC_SQLCI	
			exec @vl_Status = SQL_CI.PRC..XZAC905 @va_Horodate_in,
						@va_NumEvt_in,
						@va_CleEvt_in,
						@va_Operateur_in,
						@va_Site_in,
						@va_Message_in,
						@va_Autoroute_in,
						@va_Sens_in,
						@va_PR_debut_in,
						@va_PR_fin_in,
						@va_PAU_erreur_in,
						@va_PAU_pilotes_in,
						@va_Priorite_in,
						@va_site_origine_in,
						@va_DistanceEvt_in,
						@va_NumeroAction_out output, 
						XDC_OUI 
		else return XDC_NOK

		return @vl_Status
	end

	/*! transfo en parametres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_Operateur_in = 0 select @va_Operateur_in = null
	if @va_DistanceEvt_in = XDC_DISTANCE_INCONNUE 
		select @va_DistanceEvt_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null
								
	/*A controle des parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null or 
	   @va_Operateur_in = null 
		return XDC_ARG_INV

	select @vl_HeureSucces = null, @vl_HeureEchec = null

#ifndef PC_SIMPL
	/*A verifier l'existence de l'evenement */
	if not exists (	select * from EXP..FMC_GEN
			where numero = @va_NumEvt_in and cle = @va_CleEvt_in)
		return XDC_NOK
#endif										
												
	/* Trouver le numéro PAU le plus proche de la zone avec la xzao907 */
	exec @vl_Status = PRC..XZAO907 @va_Site_in, 
					@va_Autoroute_in,
					@va_Sens_in,
					@va_PR_debut_in,
					@va_PR_fin_in,
					@va_site_origine_in,
					@vl_NumEqt output,
					@va_distant_in

	/* Si status != OK ou pas d'équipement retourné */
	if @vl_Status != 0 or @vl_NumEqt = 0												
		return XDC_NOK 
								
	select	@vl_EtatPbMineur = mineure
	from	EXP..EQT_DSP
	where	type = XDC_EQT_PAU  and  equipement =  @vl_NumEqt  and
		sit = @vl_Site  and
		dernier = 1

#if defined (DEV)
	if @vl_EtatPbMineur <> 0							
		select @vl_HeureEchec = @va_Horodate_in
	else
		select @vl_HeureSucces = @va_Horodate_in
#endif
	
	/* Trouver le numéro d'autoroute */
	select 	@vl_Autoroute = numero
	from CFG..RES_AUT
	where nom   = @va_Autoroute_in
																			
	/*A calcul le numero d'actio a inserer */
	select @va_NumeroAction_out = isnull(max(numero), 0) + 1 from EXP..ACT_GEN
	where sit = @vl_Site

	/*A inserer l'actio PAU SONO */
	begin tran
		insert EXP..ACT_GEN ( numero, sit, evenement, cle, type, equipement,
		heure_lancement, heure_succes, heure_echec,
		priorite, prioritaire, distance_evt, operateur, sit_origine)
		values (@va_NumeroAction_out, @vl_Site, @va_NumEvt_in, @va_CleEvt_in,
		XDC_ACT_PAU,   @vl_NumEqt,
		@va_Horodate_in, @vl_HeureSucces, @vl_HeureEchec,
		@va_Priorite_in, XDC_NON, @va_DistanceEvt_in, @va_Operateur_in, @va_site_origine_in)

												
	insert EXP..ACT_PAU (actio, PAU, message, autoroute, sens, PR_debut,PR_fin, PAUErreur, PAUPilotes, sit)
		values ( @va_NumeroAction_out, @vl_NumEqt,@va_Message_in, @vl_Autoroute, @va_Sens_in, @va_PR_debut_in, @va_PR_fin_in, @va_PAU_erreur_in, @va_PAU_pilotes_in, @vl_Site)

	commit tran


	/*A initialisation des var locales pour la recherche */
	/*A du nom de la machine pilotant le PAU */
	select	@vl_MachineNonTrouvee = XDC_NON,
		@vl_NumEqtTemp = @vl_NumEqt,
		@vl_TypeEqt = XDC_EQT_PAU

	/*A recherche le nom de la machine pilotant le PAU */
	while @vl_MachineNonTrouvee = XDC_NON
	begin
		select
			@vl_NumMaitre = maitre,
			@vl_TypeMaitre = type_maitre
		from CFG..EQT_GEN
		where numero = @vl_NumEqtTemp and type = @vl_TypeEqt

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
		else	select	@vl_NumEqtTemp = @vl_NumMaitre,								
				@vl_TypeEqt = @vl_TypeMaitre
	end												

	if @vl_MachineNonTrouvee = XDC_NON
		return XDC_NOK

select @vl_varMessage = rtrim(@va_Message_in)
select @vl_varAutoroute = rtrim(@va_Autoroute_in)
select @vl_varNomMachine = rtrim(@vl_NomMachine)

select  @vl_NumEqt, @vl_varMessage, @vl_varAutoroute, @va_Sens_in, @va_PR_debut_in, @va_PR_fin_in, @va_NumeroAction_out, @vl_varNomMachine
#if ! defined (DEV)
	/*A affichage sur l'equipement (le plus tot possible) */
	if @vl_Site = XDC_CI
		exec @vl_Status = TASRV_CI...ACRP80  @vl_varMessage, @vl_varAutoroute, @va_Sens_in, @va_PR_debut_in, @va_PR_fin_in, @va_NumeroAction_out, @vl_varNomMachine
	else if @vl_Site = XDC_VC
		exec @vl_Status = TASRV_VC...ACRP80  @vl_varMessage, @vl_varAutoroute, @va_Sens_in, @va_PR_debut_in, @va_PR_fin_in, @va_NumeroAction_out, @vl_varNomMachine
	else if @vl_Site = XDC_DP
		exec @vl_Status = TASRV_DP...ACRP80  @vl_varMessage, @vl_varAutoroute, @va_Sens_in, @va_PR_debut_in, @va_PR_fin_in, @va_NumeroAction_out, @vl_varNomMachine
	else if @vl_Site = XDC_CA
		exec @vl_Status = TASRV_CA...ACRP80  @vl_varMessage, @vl_varAutoroute, @va_Sens_in, @va_PR_debut_in, @va_PR_fin_in, @va_NumeroAction_out, @vl_varNomMachine
					
	if @vl_Status != XDC_OK
		return @vl_Status
#endif	/* pas en mode DEV */


	/*A Rechercher l'action precedente sur cet equipement non terminee */	
	select 
		@vl_NumActionPrecedente = EXP..ACT_GEN.numero,
		@vl_NumEvtActionPrecedente = EXP..ACT_GEN.evenement,
		@vl_CleEvtActionPrecedente = EXP..ACT_GEN.cle,
		@vl_Autoroute = CFG..EQT_GEN.autoroute,
		@vl_PR = CFG..EQT_GEN.PR,
		@vl_Sens = CFG..EQT_GEN.sens
	from EXP..ACT_GEN, CFG..EQT_GEN
	where	EXP..ACT_GEN.equipement =  @vl_NumEqt and 
		EXP..ACT_GEN.type = XDC_ACT_PAU and
		EXP..ACT_GEN.sit = @vl_Site and 
		EXP..ACT_GEN.numero != @va_NumeroAction_out and
		CFG..EQT_GEN.numero =  @vl_NumEqt and 
		CFG..EQT_GEN.type = XDC_EQT_PAU and
		EXP..ACT_GEN.heure_lancement_fin is null and heure_lancement in
		(select max(heure_lancement) from EXP..ACT_GEN
		 where	equipement =  @vl_NumEqt and type = XDC_ACT_PAU and
			numero != @va_NumeroAction_out)

	if @@rowcount = 1
	begin

		/*A Positionner l'heure de lancement de fin                     */
		update EXP..ACT_GEN set heure_lancement_fin = @va_Horodate_in, heure_fin = getdate()
		where  numero = @vl_NumActionPrecedente and sit = @vl_Site

		/*A Reveiller si l'evenement liee a l'actio precedente est different */
		/*! on considere qu'aucun probleme de reveil puisse arriver           */
		if @vl_NumEvtActionPrecedente != @va_NumEvt_in or 
		   @vl_CleEvtActionPrecedente != @va_CleEvt_in
		begin
			/* nommage de l'equipement */
			exec 		XZAC;60 XDC_EQT_PAU, @vl_Autoroute, @vl_PR, @vl_Sens, @vl_Intitule output

			/* texte du reveil */
			select @vl_Texte = XDC_REV_VOL_EQT + @vl_Intitule + XDC_REV_VOL_EQT_SUITE,
				@vl_NomMachine = null

			/* recherche du nom du site */
			select  @vl_NomSite = code
			from CFG..RES_DIS
			where numero = @vl_Site

			if @@rowcount != 1
				return XDC_NOK

			/* declencher le reveil pour le site local */
			if  @vl_Site = XDC_CI
				exec @vl_Status = TASRV_CI...ARRP03 
						@vl_NumEvtActionPrecedente, 
						@vl_CleEvtActionPrecedente,
						@vl_NomMachine,
						@vl_NomSite, @vl_Texte
			else if @vl_Site = XDC_VC
				exec @vl_Status = TASRV_VC...ARRP03 
						@vl_NumEvtActionPrecedente, 
						@vl_CleEvtActionPrecedente,
						@vl_NomMachine,
						@vl_NomSite, @vl_Texte
			else if @vl_Site = XDC_CA
				exec @vl_Status = TASRV_CA...ARRP03
						@vl_NumEvtActionPrecedente,
						@vl_CleEvtActionPrecedente,
						@vl_NomMachine,
						@vl_NomSite, @vl_Texte
			else if @vl_Site = XDC_DP
				exec @vl_Status = TASRV_DP...ARRP03
						@vl_NumEvtActionPrecedente,
						@vl_CleEvtActionPrecedente,
						@vl_NomMachine,
						@vl_NomSite, @vl_Texte
			else return XDC_NOK

			if @vl_Status = null
				return XDC_PRC_INC
			else if @vl_Status != XDC_OK
				return XDC_REV_PB_DCL
		end
	end
	else
	begin
		/* termine les autres évenements qui ne sont pas terminé */
		update EXP..ACT_GEN set heure_fin = getdate()
		where  numero < @va_NumeroAction_out and sit = @vl_Site and type = XDC_ACT_PAU and heure_fin is null
	end


#if defined (DEV)
	/*A Si l'equipement est en alarme mineure alors emettre une alerte */
	if @vl_EtatPbMineur <> 0
	begin
		exec PRC..XZAA;01 @va_Horodate_in, XDC_EQT_PAU,  @vl_NumEqt, XZAAC_AFF_DIF, "Affichage PAU", @vl_NomSite
		return XDC_NOK
	end
#endif


	return XDC_OK
go
