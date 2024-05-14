/*E*/
/* Fichier : $Id: xzac802sp.prc,v 1.7 2021/03/08 14:19:21 pc2dpdy Exp $      Release : $Revision: 1.7 $        Date : $Date: 2021/03/08 14:19:21 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac802.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	03/01/12	creation (DEM/1016)
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.2
* LCL	03/07/17	: modif PRA
* JPL	31/08/17	: Restauration appel en cascade du CIT sur VC  1.4
* JPL	05/09/17 : Mode DEV: pas de commande equipement; action en succes, ou echec avec alerte si pb. mineur eqt. (DEM 1231)  1.5
* LCL	20/11/19	: MOVIS DEM-SAE93 1.6
* GRO	12/10/2020      : MOVIS DEM-SAE93 1.61
* CGR	05/01/21	: MOVIS ajout PRA DEM-SAE93 1.7
* LCL	27/09/23 : Adaptations CNA DEM-483	
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Action d'affichage PRV.
* 
* Sequence d'appel
* SP	XZAC802_Commande_PRV
* 
* Arguments en entree
* XDY_Horodate	va_Horodate_in
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* XDY_Operateur	va_Operateur_in
* XDY_Eqt	va_NumEqt_in
* XDY_Site	va_Site_in
* XDY_Bandeau	va_Bandeau_in
* XDY_VitesseRegul	Vitesse_in
* XDY_Octet	va_Flash_in
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
* (Priorite sera mis automatiquement a null)
* 
* Fonction
* Verifier l'existence de l'evenement
* Verifier l'existence d'un equipement non indisponible 
* dans le district ou l'actio est executee.
* 
* Inserer dans la table ACTION et la table E_PMV.
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
* ACRP802 :	Appeler XZEP802 pour afficher sur l'equipement PMV
* 		Appeler XZEP02 pour afficher le picto
* 
-----------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAC802' and type = 'P')
	drop procedure XZAC802
go

create procedure XZAC802
	@va_Horodate_in         datetime,
	@va_NumEvt_in			int,
	@va_CleEvt_in			tinyint,
	@va_Operateur_in		smallint,
	@va_NumEqt_in			smallint,
	@va_Site_in				tinyint,
	@va_Bandeau_in			varchar(10),
	@va_Vitesse_in			char(6),
	@va_Priorite_in			tinyint,
	@va_site_origine_in		T_SITE,
	@va_DistanceEvt_in		int,
	@va_NumeroAction_out		int = null	output,
	@va_distant_in 			int = XDC_NON
as
	declare @vl_Site tinyint, @vl_NomSite char(2), 
	 	@vl_NumActionPrecedente int,
		@vl_NumEvtActionPrecedente int, 
		@vl_CleEvtActionPrecedente tinyint,
		@vl_HeureSucces datetime,
		@vl_HeureEchec datetime,
		@vl_EtatIndispo int,
		@vl_EtatPbMineur int,
		@vl_NomMachine char(10), 
		@vl_Status int, 
		@vl_Texte char(100), 
		@vl_NomEqt char(10), 
		@vl_PMVTexte tinyint, 
		@vl_NumMaitre T_EQUIPEMENT, 
		@vl_TypeMaitre tinyint,
		@vl_TypeEqt tinyint, 
		@vl_NumEqt T_EQUIPEMENT, 
		@vl_MachineNonTrouvee bit,
		@vl_Autoroute T_AUTOROUTE, 
		@vl_PR T_PR,
		@vl_Sens T_SENS, 
		@vl_Intitule varchar(50),
		@vl_TypePRV tinyint,
		@vl_BandeauAffiche varchar(10),
		@vl_serveur_pra char(6)

 
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
	begin
		if @vl_serveur_pra = XDC_SQLDP
			exec @vl_Status = SQL_DS.PRC..XZAC802 @va_Horodate_in, @va_NumEvt_in, @va_CleEvt_in,
						@va_Operateur_in, @va_NumEqt_in, @va_Site_in,
						@va_Bandeau_in,@va_Vitesse_in,
						@va_Priorite_in,
						@va_site_origine_in,
						@va_DistanceEvt_in,
						@va_NumeroAction_out output, 
						XDC_OUI 
		else if @vl_serveur_pra = XDC_SQLVC
			exec @vl_Status = SQL_DM.PRC..XZAC802 @va_Horodate_in, @va_NumEvt_in, @va_CleEvt_in,
						@va_Operateur_in, @va_NumEqt_in, @va_Site_in,
						@va_Bandeau_in,@va_Vitesse_in,
						@va_Priorite_in,
						@va_site_origine_in,
						@va_DistanceEvt_in,
						@va_NumeroAction_out output, 
						XDC_OUI 
		else if @vl_serveur_pra = XDC_SQLCI
			exec @vl_Status = SQL_CI.PRC..XZAC802 @va_Horodate_in, @va_NumEvt_in, @va_CleEvt_in,
						@va_Operateur_in, @va_NumEqt_in, @va_Site_in,
						@va_Bandeau_in,@va_Vitesse_in,
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
	if @va_NumEqt_in = 0 select @va_NumEqt_in = null
	if @va_DistanceEvt_in = XDC_DISTANCE_INCONNUE 
		select @va_DistanceEvt_in = null /*, @va_Priorite_in = null*/
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A controle des parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null or @va_NumEqt_in = null or
	   @va_Operateur_in = null 
		return XDC_ARG_INV

	/*A toutes les chaines nulles sont positionnees a " " */
	/*! pour eviter pb pour recuperer donnees             */
	if @va_Bandeau_in = null 
		select @va_Bandeau_in	= XDC_CHAINE_VIDE
	if @va_Vitesse_in = null 
		select @va_Vitesse_in	= XDC_CHAINE_VIDE

	select @vl_HeureSucces = null, @vl_HeureEchec = null


#ifndef PC_SIMPL
	/*A verifier l'existence de l'evenement */
	if not exists (	select * from EXP..FMC_GEN
			where numero = @va_NumEvt_in and cle = @va_CleEvt_in)
		return XDC_NOK
#endif


	/*A Verifier l'existence de l'equipement sur le site et determiner sa disponibilite */
	select	@vl_EtatPbMineur = mineure,
		@vl_EtatIndispo = (majeure | critique | HS | inhibe | desactive)
	from	EXP..EQT_DSP
	where	type = XDC_EQT_PRV  and  equipement = @va_NumEqt_in  and
		sit = @vl_Site  and
		dernier = 1

	if @@rowcount = 0
		return XDC_NOK
	if @vl_EtatIndispo <> 0
		return XDC_NOK
#if defined (DEV)
	if @vl_EtatPbMineur <> 0
		select @vl_HeureEchec = @va_Horodate_in
	else
		select @vl_HeureSucces = @va_Horodate_in
#endif


	/*A Si PRV Pleine Voie on justifie le texte Bandeau */
	select @vl_TypePRV=type
	from CFG..EQT_PRV
	where numero = @va_NumEqt_in
	if @vl_TypePRV = XDC_PRV_PLEINE_VOIE
		select @vl_BandeauAffiche = substring( replicate(" ", ( 10 - char_length( ltrim( rtrim( @va_Bandeau_in ) ) ) ) / 2 ) +  ltrim( rtrim( @va_Bandeau_in ) ) + "          ",1 ,10)
	else
		select @vl_BandeauAffiche = @va_Bandeau_in


	/*A calcul le numero d'actio a inserer */
	select @va_NumeroAction_out = isnull(max(numero), 0) + 1 from EXP..ACT_GEN
	where sit = @vl_Site

	/*A inserer l'actio PMV */
	begin tran
		insert EXP..ACT_GEN ( numero, sit, evenement, cle, type, equipement,
		heure_lancement, heure_succes, heure_echec,
		priorite, prioritaire, distance_evt, operateur, sit_origine)
		values (@va_NumeroAction_out, @vl_Site, @va_NumEvt_in, @va_CleEvt_in,
		XDC_ACT_PRV,  @va_NumEqt_in,
		@va_Horodate_in, @vl_HeureSucces, @vl_HeureEchec,
		@va_Priorite_in, XDC_NON, @va_DistanceEvt_in, @va_Operateur_in, @va_site_origine_in)

		insert EXP..ACT_PRV (actio, PRV, sit, vitesse, bandeau)
		values ( @va_NumeroAction_out,  @va_NumEqt_in, @vl_Site, 
		@va_Vitesse_in, @vl_BandeauAffiche)

	commit tran


	/*A initialisation des var locales pour la recherche */
	/*A du nom de la machine pilotant le PRV */
	select	@vl_MachineNonTrouvee = XDC_NON,
		@vl_NumEqt = @va_NumEqt_in,
		@vl_TypeEqt = XDC_EQT_PRV

	/*A recherche le nom de la machine pilotant le PRV */
	while @vl_MachineNonTrouvee = XDC_NON
	begin
		select
			@vl_NumMaitre = maitre,
			@vl_TypeMaitre = type_maitre
		from CFG..EQT_GEN
		where numero = @vl_NumEqt and type = @vl_TypeEqt

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
		else	select	@vl_NumEqt = @vl_NumMaitre,
				@vl_TypeEqt = @vl_TypeMaitre
	end

	if @vl_MachineNonTrouvee = XDC_NON
		return XDC_NOK


	/*A initialisation des types d'affichage */
	select @vl_PMVTexte = XDC_PRV_TEXTE

#if ! defined (DEV)
	/*A affichage sur l'equipement (le plus tot possible) */

	if @vl_Site = XDC_CI
		exec @vl_Status = TASRV_CI...ACRP15 @va_NumEqt_in, @va_Bandeau_in,@va_Vitesse_in,
		@va_NumeroAction_out, @vl_NomMachine

	else if @vl_Site = XDC_VC
		exec @vl_Status = TASRV_VC...ACRP15 @va_NumEqt_in, @va_Bandeau_in,@va_Vitesse_in,
		@va_NumeroAction_out, @vl_NomMachine

	else if @vl_Site = XDC_DP
		exec @vl_Status = TASRV_DP...ACRP15 @va_NumEqt_in, @va_Bandeau_in, @va_Vitesse_in,
		@va_NumeroAction_out, @vl_NomMachine

	else if @vl_Site = XDC_CA
		exec @vl_Status = TASRV_CA...ACRP15 @va_NumEqt_in, @va_Bandeau_in, @va_Vitesse_in,
		@va_NumeroAction_out, @vl_NomMachine

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
	where	EXP..ACT_GEN.equipement = @va_NumEqt_in and 
		EXP..ACT_GEN.type = XDC_ACT_PRV and
		EXP..ACT_GEN.sit = @vl_Site and 
		EXP..ACT_GEN.numero != @va_NumeroAction_out and
		CFG..EQT_GEN.numero = @va_NumEqt_in and 
		CFG..EQT_GEN.type = XDC_EQT_PRV and
		EXP..ACT_GEN.heure_lancement_fin is null and heure_lancement in
		(select max(heure_lancement) from EXP..ACT_GEN
		 where	equipement = @va_NumEqt_in and type = XDC_ACT_PRV and
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
			exec XZAC;60 XDC_EQT_PRV, @vl_Autoroute, @vl_PR, @vl_Sens, @vl_Intitule output

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


#if defined (DEV)
	/*A Si l'equipement est en alarme mineure alors emettre une alerte */
	if @vl_EtatPbMineur <> 0
	begin
		exec PRC..XZAA;01 @va_Horodate_in, XDC_EQT_PRV, @va_NumEqt_in, XZAAC_AFF_DIF, "Affichage PRV"
		return XDC_NOK
	end
#endif

	return XDC_OK
go
