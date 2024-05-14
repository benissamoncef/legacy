/*E*/
/*  Fichier : 	$Id: xzac83sp.prc,v 1.9 2021/03/08 14:18:46 pc2dpdy Exp $ Release : $Revision: 1.9 $ Date : $Date: 2021/03/08 14:18:46 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac83sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* jmg	10/06/05	creation
* LCL   09/07/07        Suppression DY
* JMG   02/10/09        : secto DEM887 :
* JMG	10/06/16 	: pilotage depuis le CI 
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.5
* LCL 	03/07/17	: modif PRA
* JPL	26/09/17 	: Detail de syntaxe  1.7
* LCL	20/11/19 	: MOVIS DEM-SAE93 1.8
* ABE	05/01/21 	: MOVIS ajout serveur PRA DEM-SAE93 1.9
* LCL 	03/10/23 	: Adaptations CNA DEM-483	
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Terminer l'action d'affichage PMVA.
* 
* Sequence d'appel
* SP	XZAC83_Commande_Fin_PMVA
* 
* Arguments en entree
* XDY_Entier	va_NumeroAction_in
* XDY_Entier	va_Site_in
* XDY_Horodate	va_HeureLancementFin_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee ou site incorrect
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Pour revenir l'affichage par defaut sur PMV
* 
* Fonction
* Verifier la non indisponibilite de l'equipement.
* Inserer dans la table ACTION la date de fin lance de l'actio.
* ACRP83 :	Appeler XZEP01 pour arreter l'affichage sur l'equipement PMV
* 		Appeler XZEP02 pour arreter l'affichage du picto
-----------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAC83' and type = 'P')
	drop procedure XZAC83
go


create procedure XZAC83
	@va_NumeroAction_in		int = null,
	@va_Site_in			T_SITE = null,
	@va_HeureLancementFin_in	datetime = null,
	@va_TypeNeutre_in		tinyint = null,
	@va_NomSiteLocal_in		char(2)	= null,
	@va_distant_in 			int = XDC_NON
as
	declare	@vl_PMVTexte tinyint, @vl_PMVPasFlash tinyint, @vl_Clignotement tinyint,
		@vl_Texte char(15),
		@vl_NumEqt T_EQUIPEMENT, @vl_Status int, @vl_NomMachine char(10),
		@vl_TypeEqtBoucle tinyint, @vl_NumEqtBoucle T_EQUIPEMENT, 
		@vl_NumMaitre T_EQUIPEMENT, @vl_TypeMaitre tinyint,
		@vl_MachineNonTrouvee bit, @vl_Commande int, @vl_Site tinyint,
		@vl_serveur_pra char(6)	

	/*! transfo en param�tres nulls bug applix */
	if @va_NumeroAction_in = 0 select @va_NumeroAction_in = null
	if @va_Site_in = 0 select @va_Site_in = null
	if @va_HeureLancementFin_in = XDC_DATE_NULLE select @va_HeureLancementFin_in = null

	/*A controle des parametres obligatoires */
	if  @va_NumeroAction_in = null or @va_Site_in = null or 
	    @va_HeureLancementFin_in = null
		return XDC_ARG_INV

	/*A tester si le site est correct */
	/* MOVIS */
	if @va_NomSiteLocal_in = null
	begin
		if not exists ( select numero from CFG..RES_DIS
	                                where serveur = @@servername )
		return XDC_NOK
	end
	else
	begin
		if not exists ( select numero from CFG..RES_DIS
	                                where code = @va_NomSiteLocal_in )
		return XDC_NOK
	end
	
        /* MOVIS */
	if @va_NomSiteLocal_in = null
	        select  @vl_Site = numero
	        from CFG..RES_DIS
	        where serveur = @@servername
	else
		if @va_distant_in = XDC_NON
			select @vl_Site=@va_Site_in
		else
	        	select  @vl_Site = numero
	        	from CFG..RES_DIS
			where code = @va_NomSiteLocal_in


	/* Init serveur PRA */
	select @vl_serveur_pra=serveur_pra from CFG..RES_PRA where numero=@va_Site_in

	if @vl_Site = XDC_CI
	/* Traitement au district d'exploitation pour Escota, en local sinon */
	begin
		if @vl_serveur_pra=XDC_SQLDP
			exec @vl_Status = SQL_DS...XZAC83 va_NumeroAction_in, @va_Site_in, @va_HeureLancementFin_in, @va_TypeNeutre_in, @va_NomSiteLocal_in, XDC_OUI
		else if @vl_serveur_pra=XDC_SQLVC
			exec @vl_Status = SQL_DM...XZAC83 va_NumeroAction_in, @va_Site_in, @va_HeureLancementFin_in, @va_TypeNeutre_in, @va_NomSiteLocal_in, XDC_OUI
		else if @vl_serveur_pra=XDC_SQLCI
			exec @vl_Status = SQL_CI...XZAC83 va_NumeroAction_in, @va_Site_in, @va_HeureLancementFin_in, @va_TypeNeutre_in, @va_NomSiteLocal_in, XDC_OUI
		return @vl_Status
	end

	/*A Verifier la non indisponibilite de l'equipement */
	select @vl_NumEqt = EXP..ACT_GEN.equipement 
	from EXP..EQT_DSP, EXP..ACT_GEN
	where 	EXP..ACT_GEN.numero = @va_NumeroAction_in and
		EXP..ACT_GEN.sit = @va_Site_in and
		EXP..ACT_GEN.type = XDC_ACT_PMVA and
		EXP..EQT_DSP.equipement = EXP..ACT_GEN.equipement and
		EXP..EQT_DSP.type = EXP..ACT_GEN.type and
		EXP..EQT_DSP.majeure = XDC_NON and
		EXP..EQT_DSP.HS = XDC_NON and
		EXP..EQT_DSP.desactive = XDC_NON and
		EXP..EQT_DSP.critique = XDC_NON and
		EXP..EQT_DSP.dernier=XDC_OUI

	if @@rowcount = 0
		return XDC_NOK

	/*A mettre a jour la date de lancement fin */
	update EXP..ACT_GEN set heure_lancement_fin = @va_HeureLancementFin_in
	where	numero = @va_NumeroAction_in and sit = @va_Site_in and
		heure_lancement_fin is null

	if @@rowcount > 1
		return XDC_NOK

	/*A initialisation des var locales pour la recherche */
	/*A du nom de la machine pilotant le PMV */
	select	@vl_MachineNonTrouvee = XDC_NON,
		@vl_NumEqtBoucle = @vl_NumEqt,
		@vl_TypeEqtBoucle = XDC_EQT_PMVA

	/*A recherche le nom de la machine pilotant le PMVA */
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

	/*A initialisation des types et texte d'affichage (si type neutre non precise alors XDC_PMV_DEFAUT */
	select	@vl_PMVTexte = isnull(@va_TypeNeutre_in,XDC_PMVA_DEFAUT), 
		@vl_PMVPasFlash = XDC_PMVA_PASFLASH,
		@vl_Clignotement = XDC_PMVA_PASCLIGNO,
		@vl_Texte = null
		
	/*A numero de commande a envoyer : - numero action */
	select @vl_Commande = - @va_NumeroAction_in

	/*A arreter l'affichage sur PMVA */
	/*! le numero picto est egal au numero pmv            */
	/* PRA
	if @@servername = XDC_SQLCI
	*/
	if @vl_Site = XDC_CI
		exec @vl_Status = TASRV_CI...ACRP11 @vl_NumEqt, @vl_Texte, @vl_PMVTexte, 
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_PMVPasFlash, @vl_NumEqt, 
		@vl_Commande, @vl_NomMachine
	/* PRA
	else if  @@servername = XDC_SQLVC
	*/
	else if @vl_Site = XDC_VC
		exec @vl_Status = TASRV_VC...ACRP11 @vl_NumEqt, @vl_Texte, @vl_PMVTexte, 
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_PMVPasFlash, @vl_NumEqt, 
		@vl_Commande, @vl_NomMachine
	/* PRA
	else if  @@servername = XDC_SQLDP
	*/
	else if @vl_Site = XDC_DP
		exec @vl_Status = TASRV_DP...ACRP11 @vl_NumEqt, @vl_Texte, @vl_PMVTexte, 
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_PMVPasFlash, @vl_NumEqt, 
		@vl_Commande, @vl_NomMachine
	else if @vl_Site = XDC_CA
		exec @vl_Status = TASRV_CA...ACRP11 @vl_NumEqt, @vl_Texte, @vl_PMVTexte, 
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_PMVPasFlash, @vl_NumEqt, 
		@vl_Commande, @vl_NomMachine

	if @vl_Status != XDC_OK
		return @vl_Status

	return XDC_OK

go
