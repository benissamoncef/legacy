/*E*/
/*  Fichier : $Id: xzac98sp.prc,v 1.9 2021/03/08 14:13:42 pc2dpdy Exp $     Release : $Revision: 1.9 $        Date : $Date: 2021/03/08 14:13:42 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzac98.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	15/09/05	: Creation
* LCL   09/07/07        : Suppression DY
* JMG   02/10/09        : secto DEM887
* PNI   19/04/16 	: Correction des appels dans PRC des XZAC sur les serveurs sites v1.4 DEM1159
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.5
* LCL	03/07/17	: Modif PRA
* JPL	01/09/17	: En mode DEV, pas d'envoi de commande a l'equipement et action mise en fin (DEM 1231)  1.7
* LCL	20/11/19	: MOVIS DEM-SAE93 1.8
* LPE	12/10/20 	: MOVIS DEM-SAE93 1.8
* ABE	05/01/21	: MOVIS ajout serveur PRA DEM-SAE93 1.9
* LCL 	03/10/23 	: Adaptations CNA DEM-483	
------------------------------------------------------ */

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retour a l'etat normal de la BAF par scenario ou par 
* commande individuelle
* 
* Sequence d'appel
* SP	XZAC98_Commande_Fin_BAF
* 
* Arguments en entree
* XDY_Eqt	va_NumeroBAF_in
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

if exists (select * from sysobjects where name = 'XZAC98' and type = 'P')
	drop procedure XZAC98
go


create procedure XZAC98
	@va_NumeroBAF_in		T_EQUIPEMENT = null,
	@va_Site_in			T_SITE = null,
	@va_HeureLancementFin_in	datetime = null,
	@va_Manuelle_in			bit,
	@va_Module11_in			tinyint	        = null,
	@va_Module12_in			tinyint	        = null,
	@va_Module15_in			tinyint	        = null,
	@va_Module21_in			tinyint	        = null,
	@va_Module22_in			tinyint	        = null,
	@va_Module25_in			tinyint	        = null,
	@va_NumeroAction_out		int		= null	output,
	@va_NomSiteLocal_in		char(2)	= null,
	@va_distant_in 			int 		= XDC_NON
as
	declare	@vl_NumeroAction int, @vl_Status int, @vl_NomMachine char(10),
		@vl_Commande int, @vl_TypeAction tinyint,
		@vl_TypeEqtBoucle tinyint, @vl_NumEqtBoucle T_EQUIPEMENT, 
		@vl_NumMaitre T_EQUIPEMENT, @vl_TypeMaitre tinyint,
		@vl_MachineNonTrouvee bit, @vl_Site tinyint,
		@vl_serveur_pra char(6)	

	/*! transfo en param�tres nulls bug applix */
	if @va_NumeroBAF_in = 0 select @va_NumeroBAF_in = null
	if @va_Site_in = 0 select @va_Site_in = null
	if @va_HeureLancementFin_in = XDC_DATE_NULLE select @va_HeureLancementFin_in = null

	/*A controle des parametres obligatoires */
	if  @va_NumeroBAF_in = null or @va_Site_in = null or 
	    @va_HeureLancementFin_in = null
		return XDC_ARG_INV

	select @vl_TypeAction = XDC_EQT_BAF

        /*A Determiner le site local */
        /* MOVIS */
	if @va_NomSiteLocal_in = null
	        select  @vl_Site = numero
	        from CFG..RES_DIS
	        where serveur = @@servername
	else
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

	if @vl_Site is null
		return XDC_NOK

	/* Init serveur PRA */
	select @vl_serveur_pra = serveur_pra from CFG..RES_PRA where numero=@va_Site_in

	if @vl_Site = XDC_CI and @va_distant_in = XDC_NON
	/* Traitement au district d'exploitation pour Escota, en local sinon */
	begin

		if @vl_serveur_pra=XDC_SQLDP
			exec @vl_Status = SQL_DS.PRC..XZAC98 @va_NumeroBAF_in, @va_Site_in, @va_HeureLancementFin_in, 
				@va_Manuelle_in, @va_Module11_in, @va_Module12_in,
						@va_Module15_in,@va_Module21_in,@va_Module22_in,
						@va_Module25_in, @va_NumeroAction_out output, @va_NomSiteLocal_in, XDC_OUI

		else if @vl_serveur_pra=XDC_SQLVC
			exec @vl_Status = SQL_DM.PRC..XZAC98 @va_NumeroBAF_in, @va_Site_in, @va_HeureLancementFin_in, 
				@va_Manuelle_in, @va_Module11_in, @va_Module12_in,
						@va_Module15_in,@va_Module21_in,@va_Module22_in,
						@va_Module25_in, @va_NumeroAction_out output, @va_NomSiteLocal_in, XDC_OUI

		else if @vl_serveur_pra=XDC_SQLCI
			exec @vl_Status = SQL_CI.PRC..XZAC98 @va_NumeroBAF_in, @va_Site_in, @va_HeureLancementFin_in, 
				@va_Manuelle_in, @va_Module11_in, @va_Module12_in,
						@va_Module15_in,@va_Module21_in,@va_Module22_in,
						@va_Module25_in, @va_NumeroAction_out output, @va_NomSiteLocal_in, XDC_OUI
		else return XDC_NOK

		return @vl_Status
	end

	/*A Verifier la concordance des sites */
	/* MOVIS */
	/*
	if @vl_Site <> @va_Site_in
		return XDC_NOK
	*/


	/*A Rechercher la derniere action en cours pour cet equipement */
	select @vl_NumeroAction = max(numero)
	from EXP..ACT_GEN
	where 	EXP..ACT_GEN.equipement = @va_NumeroBAF_in and
		EXP..ACT_GEN.sit = @va_Site_in and
		EXP..ACT_GEN.type = XDC_ACT_BAF

	if @@rowcount = 0
		select @vl_NumeroAction = 0

	/*A numero d'action a retourner */
	select @va_NumeroAction_out = @vl_NumeroAction

	/*A cas de commande automatique par scenario */
	/*if @va_Manuelle_in =XDC_NON
	begin
	*/
		/*A initialisation des var locales pour la recherche */
		/*A du nom de la machine pilotant la BAF */
		select	@vl_MachineNonTrouvee = XDC_NON,
			@vl_NumEqtBoucle = @va_NumeroBAF_in,
			@vl_TypeEqtBoucle = @vl_TypeAction
	
		/*A recherche le nom de la machine pilotant la BAF */
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

		/*A numero de commande a envoyer : - numero action */
		select @vl_Commande = - @vl_NumeroAction


#if ! defined (DEV)
		/*A arreter la BAF */
       		/* PRA
		if @@servername = XDC_SQLCI
		*/
		if @vl_Site = XDC_CI
			exec @vl_Status = TASRV_CI...ACRP12 @va_NumeroBAF_in, @va_Module11_in, @va_Module12_in,
						@va_Module15_in,@va_Module21_in,@va_Module22_in,@va_Module25_in,
				@vl_Commande, @vl_NomMachine
		/* PRA
		else if  @@servername = XDC_SQLVC
		*/
		else if @vl_Site = XDC_VC
			exec @vl_Status = TASRV_VC...ACRP12 @va_NumeroBAF_in, @va_Module11_in, @va_Module12_in,
						@va_Module15_in,@va_Module21_in,@va_Module22_in,@va_Module25_in,
				@vl_Commande, @vl_NomMachine
		/* PRA
		else if  @@servername = XDC_SQLDP
		*/
		else if @vl_Site = XDC_DP
			exec @vl_Status = TASRV_DP...ACRP12 @va_NumeroBAF_in, @va_Module11_in, @va_Module12_in,
						@va_Module15_in,@va_Module21_in,@va_Module22_in,@va_Module25_in,
				@vl_Commande, @vl_NomMachine
		else if @vl_Site = XDC_CA
			exec @vl_Status = TASRV_CA...ACRP12 @va_NumeroBAF_in, @va_Module11_in, @va_Module12_in,
						@va_Module15_in,@va_Module21_in,@va_Module22_in,@va_Module25_in,
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
