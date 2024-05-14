/*E*/
/*  Fichier : $Id: xzac03sp.prc,v 1.26 2021/03/08 14:38:48 pc2dpdy Exp $      Release : $Revision: 1.26 $        Date : $Date: 2021/03/08 14:38:48 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac03.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	19/09/94	: Creation
* C.T.	23/09/94	: Modification du traitement de
*                         recherche du nom de la machine
*                         pilotant (V 1.3)
* C.T.	10/11/94	: Modification bugs applix (V 1.4)
* C.T.	16/11/94	: Modification du declenchement du reveil
*                         (appel ARRP03 au lieu XZAR;03)
*                         non teste (V 1.5)
* C.T.	05/12/94	: Correction erreur nom maitre V 1.6
*                         non teste
* C.T.	09/02/95	: Correction envoi de commande si en
*                         mode automatique V 1.7
* C.T.  09/02/95        : autorisation si indispo mineure v1.8
* C.T.	14/03/95	: Modif longueur texte reveil (V 1.9)
* C.T.	14/03/95	: Modif texte reveil (V 1.10)
* C.T.	03/09/95	: ajout test pour DC et DS (V1.11)
* C.P.	07/01/98	: Correction gestion multi-sites dans les procs (1.12)
* JMG	07/01/98	: Re-Correction gestion multi-sites dans les procs (1.13)
* C.T.	19/01/98	: ajout gestion FAC et FAU (1538) V1.14-1.15
* JMG	02/03/98	: ajout sit_origine (dem/1605) 1.16
* LCL   09/07/07        : Suppression DY
* PNI	22/04/08	: Suppression du test eqt disponible DEM706 1.12
* JMG           02/10/09        : secto DEM887	1.13
* PNI	28/08/13	: suppression verif existance FMC pour SAPIENS au CI (delais de propagation FMC via repli) DEM 1074 1.20
* JMG	27/06/16	: gestion pilotage depuis le CI 1.21 
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.22
* LCL 	29/06/17	: modif PRA 1.23
* JPL	05/09/17 : Mode DEV: pas de commande equipement; action en succes, ou echec avec alerte si pb. mineur eqt. (DEM 1231)  1.24
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
* LPE	12/10/20 	: MOVIS DEM-SAE93 1.25
* ABE	05/01/21	: MOVIS ajout serveur PRA DEM-SAE93 1.26							
* LCL	06/07/22	: Correction utilisation parametre va_Manuelle_in DEM-SAE392
* LCL	27/09/23 : Adaptations CNA DEM-483	
* LCL	12/02/24	: Remise en place test va_Manuelle. Si va_Manuelle=vrai c'est XZEN/xtitre.x et pas tasrv qui exécute la commande DEM-SAE573
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Action de deployement de NAV.par scenario
* 
* Sequence d'appel
* SP	XZAC03_Commande_NAV
* 
* Arguments en entree
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* XDY_Operateur	va_Operateur_in
* XDY_Eqt	va_NumEqt_in
* XDY_Horodate	va_HeureLancement_in
* XDY_Booleen	va_Manuelle_in
* XDY_Octet	va_Priorite_in
* XDY_Entier	va_DistanceEvt_in
* char(4)	va_Scenario_in
* XDY_Site	va_Site_in
* XDY_Site	va_site_origine_in
*
* Arguments en sortie
* XDY_Entier		NumeroAction
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve, equipement indisponible, 
*                 insertion imposible, nom de l'equipement pilotant la nav
*                 non trouve
* XDC_REV_PB_DCL: probleme pour declencher le reveil
* XDC_PRC_INC	: ARRP03 inexistante
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* L'execution de l'actio est realisee dans le cas de 
* commande automatique par scenario
* Si l'actio a ete proposee par le plan d'actions, 
* positionner Priorite et DistanceEvt, sinon, les mettre a 0
* Si l'operateur pilote equipement par equipement, 
* positionner Manuelle a XDC_Vrai (l'operateur doit 
* quand meme renseigner le scenario auquel il compte aboutir)
* 
* Fonction
* Verifier l'existence de l'evenement
* Verifier l'existence de l'equipement non indisponible 
* dans le district ou l'actio est executee.
* 
* Inserer dans la table ACTION et la table E_NAV.
* Traitement sur  l'actio precedente sur l'equipement a commander :
* 	Chercher l'actio precedente :
* 	SELECT numero FROM ACTION 
* 	WHERE equipement, type, heure_lancement_fin = null, 
* 		heure_lancement = all
* 		(SELECT max(heure_lancement) FROM ACTION 
* 		WHERE equipement, type)
* 	
* 	Positionner heure_lancement_fin a HeureLancement 
*	pour l'actio precedente
* 	Si l'actio precedente trouvee est liee a 
* 	un autre evenement alors l'autre evenement doit etre 
* 	reveille pour vol (appeler XZAR03).
* 
* Dans le cas de commande automatique par scenario :
* 	ACRP03 :	Appeler XZEN03 pour commander l'equipement
------------------------------------------------------*/

use PRC
go

create procedure XZAC;03
	@va_NumEvt_in			int		= null,
	@va_CleEvt_in			tinyint		= null,
	@va_Operateur_in		T_OPERATEUR	= null,
	@va_NumEqt_in			T_EQUIPEMENT	= null,
	@va_HeureLancement_in		datetime	= null,
	@va_Manuelle_in			bit		      ,
	@va_Priorite_in			tinyint		= null,
	@va_DistanceEvt_in		int		= null,
	@va_Scenario_in			char(4)		= null,
	@va_Site_in			T_SITE		= null,
	@va_site_origine_in		T_SITE          = null,
	@va_NumeroAction_out		int		= null	output,
	@va_distant_in 			int 		= XDC_NON	
as
	declare @vl_Site tinyint, @vl_NomSite char(2), @vl_NumActionPrecedente int,
		@vl_NumEvtActionPrecedente int, @vl_CleEvtActionPrecedente tinyint,
		@vl_HeureSucces datetime, @vl_HeureEchec datetime,
		@vl_EtatIndispo int, @vl_EtatPbMineur int,
		@vl_NomMachine char(10), @vl_Status int, 
		@vl_Texte char(100), @vl_NomEqt char(10),
		@vl_NumMaitre T_EQUIPEMENT, @vl_TypeMaitre tinyint,
		@vl_TypeEqt tinyint, @vl_NumEqt T_EQUIPEMENT, @vl_MachineNonTrouvee bit,
		@vl_PR T_PR, @vl_Sens T_SENS, @vl_Autoroute T_AUTOROUTE, 
		@vl_Intitule varchar(50), @vl_TypeAction tinyint,
		@vl_serveur_pra char(6)								

	/*! transfo en param�tres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_Operateur_in = 0 select @va_Operateur_in = null
	if @va_NumEqt_in = 0 select @va_NumEqt_in = null
	if @va_DistanceEvt_in = XDC_DISTANCE_INCONNUE 
		select @va_DistanceEvt_in = null, @va_Priorite_in = null
	if @va_HeureLancement_in = XDC_DATE_NULLE select @va_HeureLancement_in = null


	/*A controle des parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null or @va_NumEqt_in = null or
	   @va_Operateur_in = null or @va_HeureLancement_in = null or @va_Scenario_in = null
		return XDC_ARG_INV

	select @vl_HeureSucces = null, @vl_HeureEchec = null


	/*A Recherche du site local*/
	/* MOVIS */
	if @va_site_origine_in = null
		select @vl_Site = numero 
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

	/*A On est au CI et on pilote un autre district */
	if @vl_Site != @va_Site_in and @vl_Site = XDC_CI and @va_distant_in = XDC_NON				
	begin
		if @vl_serveur_pra = XDC_SQLVC
			exec @vl_Status = SQL_DM.PRC..XZAC;03 @va_NumEvt_in, @va_CleEvt_in,
						@va_Operateur_in, @va_NumEqt_in, 
						@va_HeureLancement_in, @va_Manuelle_in, @va_Priorite_in,
						@va_DistanceEvt_in, @va_Scenario_in, @va_Site_in, 
						@va_site_origine_in,
						@va_NumeroAction_out output,
						XDC_OUI
		else if @vl_serveur_pra = XDC_SQLDP
			exec @vl_Status = SQL_DS.PRC..XZAC;03 @va_NumEvt_in, @va_CleEvt_in,
						@va_Operateur_in, @va_NumEqt_in, 
						@va_HeureLancement_in, @va_Manuelle_in, @va_Priorite_in,
						@va_DistanceEvt_in, @va_Scenario_in, @va_Site_in, 
						@va_site_origine_in,
						@va_NumeroAction_out output,
						XDC_OUI
		else if @vl_serveur_pra = XDC_SQLCI
			exec @vl_Status = SQL_CI.PRC..XZAC;03 @va_NumEvt_in, @va_CleEvt_in,
						@va_Operateur_in, @va_NumEqt_in, 
						@va_HeureLancement_in, @va_Manuelle_in, @va_Priorite_in,
						@va_DistanceEvt_in, @va_Scenario_in, @va_Site_in, 
						@va_site_origine_in,
						@va_NumeroAction_out output,
						XDC_OUI
		else return XDC_NOK

		return @vl_Status
	end
	/* pas de traitement distant sur autres sites, tout s'effectue en local */


#ifndef PC_SIMPL
	/*A verifier l'existence de l'evenement */
/*DEM1074 : suppression de la verif pour SAPIENS au CI (delais de propagation FMC via repli)
	if not exists (	select * from EXP..FMC_GEN
			where numero = @va_NumEvt_in and cle = @va_CleEvt_in)
		return XDC_NOK*/
#endif


	/*A Verifier l'existence de l'equipement sur le site et determiner sa disponibilite */
	select	@vl_EtatPbMineur = mineure,
		@vl_EtatIndispo = (majeure | critique | HS | inhibe | desactive)
	from	EXP..EQT_DSP
	where	type in ( XDC_EQT_NAV, XDC_EQT_FAC, XDC_EQT_FAU )  and  equipement = @va_NumEqt_in  and
		sit = @vl_Site  and
		dernier = 1
#if defined (DEV)
	if @@rowcount = 0
		return XDC_NOK
	if @vl_EtatIndispo <> 0
		return XDC_NOK
	if @vl_EtatPbMineur <> 0
		select @vl_HeureEchec = @va_HeureLancement_in
	else
		select @vl_HeureSucces = @va_HeureLancement_in
#else
/*DEM706	if @@rowcount = 0
		return XDC_OK
	if @vl_EtatIndispo <> 0
		return XDC_OK*/
#endif


	select @vl_TypeAction = XDC_EQT_NAV

	/*A calcul le numero d'actio a inserer */
	select @va_NumeroAction_out = isnull(max(numero), 0) + 1 from EXP..ACT_GEN
	where sit = @vl_Site

	/*A inserer l'actio NAV */
	begin tran
		insert EXP..ACT_GEN ( numero, sit, evenement, cle, type, equipement,
		heure_lancement, heure_succes, heure_echec,
		priorite, prioritaire, distance_evt, operateur, sit_origine)
		values (@va_NumeroAction_out, @vl_Site, @va_NumEvt_in, @va_CleEvt_in, @vl_TypeAction, @va_NumEqt_in,
		@va_HeureLancement_in, @vl_HeureSucces, @vl_HeureEchec,
		@va_Priorite_in, XDC_NON, @va_DistanceEvt_in, @va_Operateur_in, @va_site_origine_in)

		insert EXP..ACT_NAV (actio, NAV, sit, scenario, manuelle)
		values ( @va_NumeroAction_out,  @va_NumEqt_in, @vl_Site, 
		@va_Scenario_in, @va_Manuelle_in)

	commit tran


	/*A initialisation des var locales pour la recherche */
	/*A du nom de la machine pilotant le PMV */
	select	@vl_MachineNonTrouvee = XDC_NON,
		@vl_NumEqt = @va_NumEqt_in,				
		@vl_TypeEqt = @vl_TypeAction

	/*A recherche le nom de la machine pilotant la NAV */
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


#if ! defined (DEV)
	/*A commander l'equipement (le plus tot possible) */
	if @va_Manuelle_in = XDC_NON
	begin 
       		/* PRA
		if @@servername = XDC_SQLCI
		*/
		if @vl_Site = XDC_CI
			exec @vl_Status = TASRV_CI...ACRP03 @va_NumEqt_in, @va_Scenario_in,
			@va_NumeroAction_out, @vl_NomMachine
		else /* PRA
		     if  @@servername = XDC_SQLVC
		     */
		     if @vl_Site = XDC_VC
			exec @vl_Status = TASRV_VC...ACRP03 @va_NumEqt_in, @va_Scenario_in,
			@va_NumeroAction_out, @vl_NomMachine
		else /* PRA
		     if  @@servername = XDC_SQLDP
		     */
		     if @vl_Site = XDC_DP
			exec @vl_Status = TASRV_DP...ACRP03 @va_NumEqt_in, @va_Scenario_in,
			@va_NumeroAction_out, @vl_NomMachine
		else /* PRA
		     if  @@servername = XDC_SQLCA
		     */
		     if @vl_Site = XDC_CA
			exec @vl_Status = TASRV_CA...ACRP03 @va_NumEqt_in, @va_Scenario_in,
			@va_NumeroAction_out, @vl_NomMachine

		if @vl_Status != XDC_OK
			return @vl_Status
	end

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
		EXP..ACT_GEN.type = @vl_TypeAction and
		EXP..ACT_GEN.sit = @vl_Site and 
		EXP..ACT_GEN.numero != @va_NumeroAction_out and
		CFG..EQT_GEN.numero = EXP..ACT_GEN.equipement and 
		CFG..EQT_GEN.type = EXP..ACT_GEN.type and
		EXP..ACT_GEN.heure_lancement_fin is null and heure_lancement in
		(select max(heure_lancement) from EXP..ACT_GEN
		 where	equipement = @va_NumEqt_in and type = @vl_TypeAction and
			numero != @va_NumeroAction_out)

	if @@rowcount = 1
	begin
		/*A Positionner l'heure de lancement de fin pour l'action precedente */
		update EXP..ACT_GEN set heure_lancement_fin = @va_HeureLancement_in
#if defined (DEV)
		                        , heure_fin = @va_HeureLancement_in
#endif
		where  numero = @vl_NumActionPrecedente and sit = @vl_Site
	
		/*A Reveiller si l'evenement liee a l'action precedente est different */
		if @vl_NumEvtActionPrecedente != @va_NumEvt_in or 
		   @vl_CleEvtActionPrecedente != @va_CleEvt_in
		begin
			/* nommage de l'equipement */
			exec XZAC;60 @vl_TypeAction, @vl_Autoroute, @vl_PR, @vl_Sens, @vl_Intitule output

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
			else if @vl_Site = XDC_DP
				exec @vl_Status = TASRV_DP...ARRP03 
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
select @va_HeureLancement_in, XDC_EQT_NAV, @va_NumEqt_in, 4, "Action NAV"

		exec PRC..XZAA;01 @va_HeureLancement_in, XDC_EQT_NAV, @va_NumEqt_in, XZAAC_AFF_DIF, "Action NAV"
		return XDC_NOK
	end
#endif
	return XDC_OK
go
