/*E*/
/* Fichier : $Id: xzac84sp.prc,v 1.11 2021/03/08 14:16:37 pc2dpdy Exp $        Release : $Revision: 1.11 $        Date : $Date: 2021/03/08 14:16:37 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac84sp.prc
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
* JMG           02/10/09        : secto DEM887 1.3
* VR	30/05/2012	: AjouT TDP PMVA (DEM/1014 PMA)
* PNI           19/04/16 : Correction des appels dans PRC des XZAC sur les serveurs sites v1.5
 DEM1159
* JMG	06/03/17	: regionalisation code region  DEM1220 v1.6
* JPL	08/03/17 : Enregistrement des neutres 3points et Temps de Parcours en table dediee (DEM 1196)  1.7
* LCL	03/07/17	: Modif PRA
* JPL	01/09/17 : En mode DEV, pas d'envoi de commande a l'equipement et action mise en fin (DEM 1231)  1.9
* LCL	20/11/19	: MOVIS DEM-SAE93 1.10
* GRO	12/10/2020      : MOVIS DEM-SAE93 1.11	
* ABE	05/01/21   	: MOVIS ajout serveur PRA DEM-SAE93 1.12			
* LCL 	03/10/23 	: Adaptations CNA DEM-483	
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Terminer l'action d'affichage PMVA.
* 
* Sequence d'appel
* SP	XZAC84_Commande_Fin_PMVA
* 
* Arguments en entree
* XDY_Eqt	va_NumeroPMA_in
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
* Pour revenir l'affichage par defaut sur PMVA
* 
* Fonction
* Inserer dans la table ACTION la date de fin lance de l'actio.
* ACRP84 :	Appeler XZEP01 pour arreter l'affichage sur l'equipement PMVA
* 		Appeler XZEP02 pour arreter l'affichage du picto
-----------------------------------------------------*/

use PRC
go
if exists (select * from sysobjects where name = 'XZAC84' and type = 'P')
	drop procedure XZAC84
go

create procedure XZAC84
	@va_NumeroPMA_in		T_EQUIPEMENT = null,
	@va_Site_in			T_SITE = null,
	@va_HeureLancementFin_in	datetime = null,
	@va_TypeNeutre_in		tinyint = null,
	@va_NomSiteLocal_in		char(2)	= null,
	@va_distant_in 			int = XDC_NON		
as
	declare	@vl_PMATexte tinyint, @vl_PMAPasFlash tinyint, @vl_Clignotement tinyint,
		@vl_Texte char(15), 
		@vl_NumeroAction int, @vl_Status int, @vl_NomMachine char(10),
		@vl_TypeEqtBoucle tinyint, @vl_NumEqtBoucle T_EQUIPEMENT, 
		@vl_NumMaitre T_EQUIPEMENT, @vl_TypeMaitre tinyint,
		@vl_MachineNonTrouvee bit, @vl_Commande int,
	    @vl_Dest_Un_Proche smallint, @vl_Dest_Un_Lointaine smallint, @vl_Dest_Deux_Proche smallint, @vl_Dest_Deux_Lointaine smallint, @vl_DepSeuil tinyint,
		@vl_Site tinyint,
		@vl_serveur_pra char(6)	

	/*! transfo en parametres nulls bug applix */
	if @va_NumeroPMA_in = 0 select @va_NumeroPMA_in = null
	if @va_Site_in = 0 select @va_Site_in = null
	if @va_HeureLancementFin_in = XDC_DATE_NULLE select @va_HeureLancementFin_in = null

	select @vl_Dest_Un_Proche=0
	select @vl_Dest_Un_Lointaine=0
	select @vl_Dest_Deux_Proche=0
	select @vl_Dest_Deux_Lointaine=0
	select @vl_DepSeuil=0
	/*A controle des parametres obligatoires */
	if  @va_NumeroPMA_in = null or @va_Site_in = null or 
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
		if @vl_serveur_pra=XDC_SQLDP
			exec @vl_Status = SQL_DS.PRC..XZAC84 @va_NumeroPMA_in, @va_Site_in, @va_HeureLancementFin_in, @va_TypeNeutre_in, @va_NomSiteLocal_in, XDC_OUI 

		else if @vl_serveur_pra=XDC_SQLVC
			exec @vl_Status = SQL_DM.PRC..XZAC84 @va_NumeroPMA_in, @va_Site_in, @va_HeureLancementFin_in, @va_TypeNeutre_in, @va_NomSiteLocal_in, XDC_OUI 

		else if @vl_serveur_pra=XDC_SQLCI
			exec @vl_Status = SQL_CI.PRC..XZAC84 @va_NumeroPMA_in, @va_Site_in, @va_HeureLancementFin_in, @va_TypeNeutre_in, @va_NomSiteLocal_in, XDC_OUI 
		else return XDC_NOK

		return @vl_Status
	end

	/*A tester si le site est correct */
	if not exists ( select numero from CFG..RES_DIS
                                where numero = @va_Site_in )
/*                                where serveur = @@servername )
*/
		return XDC_NOK


	/*A Rechercher la derniere action en cours sur cet equipement */
	select @vl_NumeroAction = max(numero)
	from EXP..ACT_GEN
	where 	equipement = @va_NumeroPMA_in and
		sit = @va_Site_in and type = XDC_ACT_PMVA 

	if @@rowcount = 0
		select @vl_NumeroAction = 0

	/*A initialisation des var locales pour la recherche */
	/*A du nom de la machine pilotant le PMVA */
	select	@vl_MachineNonTrouvee = XDC_NON,
		@vl_NumEqtBoucle = @va_NumeroPMA_in,
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

	/*A initialisation des types et texte d'affichage */
	select	@vl_PMATexte = isnull(@va_TypeNeutre_in,XDC_PMVA_DEFAUT), 
		@vl_PMAPasFlash = XDC_PMVA_PASFLASH,
		@vl_Clignotement = XDC_PMVA_PASCLIGNO,
		@vl_Texte = null

	if @vl_PMATexte = 0
		select @vl_PMATexte = XDC_PMVA_DEFAUT
	/*A numero de commande a envoyer : - numero action */
	select @vl_Commande = - @vl_NumeroAction


#if ! defined (DEV)
	/*A arreter l'affichage sur PMVA */
	/*! le numero picto est egal au numero pmva            */
	/* PRA
	if @@servername = XDC_SQLCI
	*/
	if @vl_Site = XDC_CI
		exec @vl_Status = TASRV_CI...ACRP11 @va_NumeroPMA_in, @vl_Texte, @vl_PMATexte, 
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMATexte,
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMATexte,
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMATexte, 
		@vl_Clignotement, @vl_Texte,@vl_Texte, @vl_PMATexte, 
		@vl_Clignotement, @vl_Texte,@vl_PMAPasFlash, 
		@vl_DepSeuil,
		@vl_Dest_Un_Proche, @vl_Dest_Un_Lointaine,
		@vl_Dest_Deux_Proche, @vl_Dest_Deux_Lointaine,
		@vl_Commande, @vl_NomMachine
	/* PRA
	else if  @@servername = XDC_SQLVC
	*/
	else if @vl_Site = XDC_VC
		exec @vl_Status = TASRV_VC...ACRP11 @va_NumeroPMA_in, @vl_Texte, @vl_PMATexte, 
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMATexte,
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMATexte,
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMATexte, 
		@vl_Clignotement, @vl_Texte,@vl_Texte, @vl_PMATexte, 
		@vl_Clignotement, @vl_Texte,@vl_PMAPasFlash, 
		@vl_DepSeuil,
		@vl_Dest_Un_Proche, @vl_Dest_Un_Lointaine,
		@vl_Dest_Deux_Proche, @vl_Dest_Deux_Lointaine,
		@vl_Commande, @vl_NomMachine
	/* PRA
	else if  @@servername = XDC_SQLDP
	*/
	else if @vl_Site = XDC_DP
		exec @vl_Status = TASRV_DP...ACRP11 @va_NumeroPMA_in, @vl_Texte, @vl_PMATexte, 
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMATexte,
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMATexte,
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMATexte, 
		@vl_Clignotement, @vl_Texte,@vl_Texte, @vl_PMATexte, 
		@vl_Clignotement, @vl_Texte,@vl_PMAPasFlash, 
		@vl_DepSeuil,
		@vl_Dest_Un_Proche, @vl_Dest_Un_Lointaine,
		@vl_Dest_Deux_Proche, @vl_Dest_Deux_Lointaine,
		@vl_Commande, @vl_NomMachine
	else if @vl_Site = XDC_CA
		exec @vl_Status = TASRV_CA...ACRP11 @va_NumeroPMA_in, @vl_Texte, @vl_PMATexte, 
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMATexte,
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMATexte,
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMATexte, 
		@vl_Clignotement, @vl_Texte,@vl_Texte, @vl_PMATexte, 
		@vl_Clignotement, @vl_Texte,@vl_PMAPasFlash, 
		@vl_DepSeuil,
		@vl_Dest_Un_Proche, @vl_Dest_Un_Lointaine,
		@vl_Dest_Deux_Proche, @vl_Dest_Deux_Lointaine,
		@vl_Commande, @vl_NomMachine

	if @vl_Status != XDC_OK
		return @vl_Status
#endif	/* pas en mode DEV */


	begin tran

		/*A mettre a jour la date de lancement fin s'il existe une action a terminee */
		if @vl_NumeroAction != 0
		begin
			update EXP..ACT_GEN set heure_lancement_fin = @va_HeureLancementFin_in
#if defined (DEV)
			                      , heure_fin = @va_HeureLancementFin_in
#endif
			where	numero = @vl_NumeroAction and sit = @va_Site_in and
				heure_lancement_fin is null
		end

#ifndef CI
		/* Mettre a jour la trace des mises au neutre */
		exec XZAC841  XDC_EQT_PMVA, @va_NumeroPMA_in, @va_HeureLancementFin_in, @va_TypeNeutre_in
#endif

	commit tran

	return XDC_OK

go
