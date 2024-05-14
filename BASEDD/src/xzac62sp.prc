/*E*/
/*  Fichier : $Id: xzac62sp.prc,v 1.17 2021/03/08 14:26:50 pc2dpdy Exp $      Release : $Revision: 1.17 $        Date : $Date: 2021/03/08 14:26:50 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac62sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	24/04/95	: Creation a partir de xzac51
* C.T.	03/09/96	: ajout test pour DC et DS (V1.2)
* D.M	03/11/96	: ajout retourne numero d'acton (DEM/1228) (V1.3)
* C.P.	07/01/98	: Correction gestion multi-sites dans 
*			  les procs (1.4)
* JMG	07/01/98        : Re Correction gestion multi-sites dans les procs (1.5) (dem/1536)
* C.T.	20/01/98	: ajout commande fin FAU et FAC (1538) (V1.6)
* PNI	02/01/06	: ajout de la clause type in (XDC_EQT_NAV, XDC_EQT_FAC, XDC_EQT_FAU) dans la recherche de l'action (v1.9) DEM546
* LCL   09/07/07        : Suppression DY
* JMG           02/10/09        : secto DEM887
* PNI           19/04/16 : Correction des appels dans PRC des XZAC sur les serveurs sites v1.12 DEM1159
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.13
* LCL	03/07/17	! Modif PRA
* JPL	01/09/17	: En mode DEV, pas d'envoi de commande a l'equipement et action mise en fin (DEM 1231)  1.15
* LCL	20/11/19	: MOVIS DEM-SAE93 1.16
* GRO	12/10/2020      : MOVIS DEM-SAE93 1.17
* CGR	05/01/21	: MOVIS ajout PRA DEM-SAE93 1.17
* LCL	27/09/23 : Adaptations CNA DEM-483	
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retour a l'etat normal de la NAV par scenario ou par 
* commande individuelle
* 
* Sequence d'appel
* SP	XZAC62_Commande_Fin_NAV
* 
* Arguments en entree
* XDY_Eqt	va_NumeroNAV_in
* XDY_Entier	va_Site_in
* XDY_Horodate	va_HeureLancementFin_in
* XDY_Booleen	va_Manuelle_in
* char(4)	va_Scenario_in
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
* 	ACRP03 :	Appeler XZEN03 pour commander l'equipement
--------------------------------------------------------*/

use PRC
go

create procedure XZAC;62
	@va_NumeroNAV_in		T_EQUIPEMENT = null,
	@va_Site_in			T_SITE = null,
	@va_HeureLancementFin_in	datetime = null,
	@va_Manuelle_in			bit,
	@va_Scenario_in			char(4)		= null,
	@va_NumeroAction_out		int		= null	output,
	@va_NomSiteLocal_in		char(2)	= null,
	@va_distant_in 			int = XDC_NON		
as
	declare	@vl_NumeroAction int, @vl_Status int, @vl_NomMachine char(10),
		@vl_Scenario char(4), @vl_Commande int, @vl_TypeAction tinyint,
		@vl_TypeEqtBoucle tinyint, @vl_NumEqtBoucle T_EQUIPEMENT, 
		@vl_NumMaitre T_EQUIPEMENT, @vl_TypeMaitre tinyint,
		@vl_MachineNonTrouvee bit, @vl_Site tinyint,
		@vl_serveur_pra char(6)

	/*! transfo en param�tres nulls bug applix */
	if @va_NumeroNAV_in = 0 select @va_NumeroNAV_in = null
	if @va_Site_in = 0 select @va_Site_in = null
	if @va_HeureLancementFin_in = XDC_DATE_NULLE select @va_HeureLancementFin_in = null

	/*A controle des parametres obligatoires */
	if  @va_NumeroNAV_in = null or @va_Site_in = null or 
	    @va_HeureLancementFin_in = null
		return XDC_ARG_INV

	/*A Determiner le site local */
	/* MOVIS */
	if @va_NomSiteLocal_in = null
		select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername
	else
	begin
                if @va_distant_in = XDC_NON
                begin
			select  @vl_Site = numero
			from CFG..RES_DIS
			where code = @va_NomSiteLocal_in
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

	if @vl_Site = XDC_CI and @va_distant_in = XDC_NON		
	/* Traitement au district d'exploitation pour Escota, en local sinon */
	begin
		if @vl_serveur_pra = XDC_SQLDP
			exec @vl_Status = SQL_DS.PRC..XZAC;62 @va_NumeroNAV_in, @va_Site_in, @va_HeureLancementFin_in, 
				@va_Manuelle_in, @va_Scenario_in, @va_NumeroAction_out output, @va_NomSiteLocal_in, XDC_OUI 
		else if @vl_serveur_pra = XDC_SQLVC
			exec @vl_Status = SQL_DM.PRC..XZAC;62 @va_NumeroNAV_in, @va_Site_in, @va_HeureLancementFin_in, 
				@va_Manuelle_in, @va_Scenario_in, @va_NumeroAction_out output, @va_NomSiteLocal_in, XDC_OUI 
		else if @vl_serveur_pra = XDC_SQLCI
                        exec @vl_Status = SQL_CI.PRC..XZAC;62 @va_NumeroNAV_in, @va_Site_in, @va_HeureLancementFin_in,
                                @va_Manuelle_in, @va_Scenario_in, @va_NumeroAction_out output, @va_NomSiteLocal_in, XDC_OUI
		else return XDC_NOK

		return @vl_Status
	end

	/* Verifier la concordance des sites */
	/* MOVIS */
	/*
	if @va_Site_in <> @vl_Site
		return XDC_NOK
	*/


	/*A Rechercher le type de l'action : BRA, FAC ou FAU */
	select @vl_TypeAction = type
	from EXP..ACT_GEN
	where 	EXP..ACT_GEN.equipement = @va_NumeroNAV_in and
		EXP..ACT_GEN.sit = @va_Site_in and type in (XDC_EQT_NAV, XDC_EQT_FAC, XDC_EQT_FAU)

	/*A Rechercher la derniere action en cours pour cet equipement */
	select @vl_NumeroAction = max(numero)
	from EXP..ACT_GEN
	where 	EXP..ACT_GEN.equipement = @va_NumeroNAV_in and
		EXP..ACT_GEN.sit = @va_Site_in and
		EXP..ACT_GEN.type = @vl_TypeAction

	if @@rowcount = 0
		select @vl_NumeroAction = 0

	/*A numero d'action a retourner */
	select @va_NumeroAction_out = @vl_NumeroAction

	/*A cas de commande automatique par scenario */
/*	if @va_Manuelle_in =XDC_NON
	begin
*/		/*A initialisation des var locales pour la recherche */
		/*A du nom de la machine pilotant la NAV */
		select	@vl_MachineNonTrouvee = XDC_NON,
			@vl_NumEqtBoucle = @va_NumeroNAV_in,
			@vl_TypeEqtBoucle = @vl_TypeAction
	
		/*A recherche le nom de la machine pilotant la NAV */
		while @vl_MachineNonTrouvee = XDC_NON
		begin
			select
				@vl_NumMaitre = maitre,
				@vl_TypeMaitre = type_maitre,
				@vl_NomMachine = nom
			from CFG..EQT_GEN
			where numero = @vl_NumEqtBoucle and type = @vl_TypeEqtBoucle
	
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

		/*A scenario a utiliser */
		select @vl_Scenario = @va_Scenario_in
	
		/*A numero de commande a envoyer : - numero action */
		select @vl_Commande = - @vl_NumeroAction

#if ! defined (DEV)
		/*A arreter la NAV */

		if @vl_Site = XDC_CI
			exec @vl_Status = TASRV_CI...ACRP03 @va_NumeroNAV_in, @vl_Scenario,
				@vl_Commande, @vl_NomMachine

		else if @vl_Site = XDC_VC
			exec @vl_Status = TASRV_VC...ACRP03 @va_NumeroNAV_in, @vl_Scenario,
				@vl_Commande, @vl_NomMachine

		else if @vl_Site = XDC_DP
			exec @vl_Status = TASRV_DP...ACRP03 @va_NumeroNAV_in, @vl_Scenario,
				@vl_Commande, @vl_NomMachine
	
		else if @vl_Site = XDC_CA
			exec @vl_Status = TASRV_CA...ACRP03 @va_NumeroNAV_in, @vl_Scenario,
				@vl_Commande, @vl_NomMachine
	
		if @vl_Status != XDC_OK
			return @vl_Status
#endif	/* pas en mode DEV */
/*	end
*/

	/*A mettre a jour la date de lancement fin */
	if @vl_NumeroAction != 0
	begin
		update EXP..ACT_GEN set heure_lancement_fin = @va_HeureLancementFin_in
#if defined (DEV)
		                      , heure_fin = @va_HeureLancementFin_in
#endif
		where	numero = @vl_NumeroAction and sit = @va_Site_in and
			heure_lancement_fin is null
	end


	return XDC_OK

go
