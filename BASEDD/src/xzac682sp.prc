/*E*/
/* Fichier : $Id: xzac682sp.prc,v 1.7 2021/03/08 14:20:09 pc2dpdy Exp $        Release : $Revision: 1.7 $        Date : $Date: 2021/03/08 14:20:09 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac682.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR/MG	20/08/12		: Ajout DTP PMVA (DEM/1014 PMA)
* JMG	06/03/17 : regionalisation code region DEM1220 1.2
* JPL	08/03/17 : Enregistrement des neutres 3points et Temps de Parcours en table dediee (DEM 1196)  1.3
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.3
* LCL	03/07/17	: Modif PRA
* JPL	05/09/17 : Mode DEV: pas de commande equipement; status d'echec si pb. mineur eqt. (DEM 1231)  1.5
* LCL	20/11/19	: MOVIS DEM-SAE93 1.6
* GRO	12/10/2020      : MOVIS DEM-SAE93 1.61
* CGR	05/01/21	: MOVIS ajout PRA DEM-SAE93 1.7
* LCL	28/09/23 : Adaptations CNA DEM-483	
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Action d'affichage PMVA.
* 
* Sequence d'appel
* SP	XZAC682_Commande_PMVA_TDP
* 
* Arguments en entree
* XDY_Horodate	va_Horodate_in
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* XDY_Operateur	va_Operateur_in
* XDY_Eqt	va_NumEqt_in
* XDY_Site	va_Site_in
* XDY_LignePMV	va_Ligne1_in
* XDY_LignePMV	va_Ligne2_in
* XDY_LignePMV	va_Ligne3_in
* XDY_LignePMV	va_Ligne4_in
* XDY_LignePMV	va_Ligne5_in
* XDY_LignePMV	va_Alternat1_in
* XDY_LignePMV	va_Alternat2_in
* XDY_LignePMV	va_Alternat3_in
* XDY_LignePMV	va_Alternat4_in
* XDY_LignePMV	va_Alternat5_in
* XDY_Octet	va_Clignotement1_in
* XDY_Octet	va_Clignotement2_in
* XDY_Octet	va_Clignotement3_in
* XDY_Octet	va_Clignotement4_in
* XDY_Octet	va_Clignotement5_in
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
* ACRP82 :	Appeler XZEP82 pour afficher sur l'equipement PMV
* 		Appeler XZEP02 pour afficher le picto
* 
-----------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAC682' and type = 'P')
	drop procedure XZAC682
go

create procedure XZAC682
	@va_Horodate_in			datetime	= null,
	@va_NumEvt_in			int		= null,
	@va_CleEvt_in			tinyint		= null,
	@va_Operateur_in		smallint	= null,
	@va_NumEqt_in			smallint	= null,
	@va_Site_in			tinyint		= null,
	@va_Ligne1_in			char(15)	= null,
	@va_Ligne2_in			char(15)	= null,
	@va_Ligne3_in			char(15)	= null,
	@va_Ligne4_in			char(15)	= null,
	@va_Ligne5_in			char(15)	= null,
	@va_Alternat1_in		char(15)	= null,
	@va_Alternat2_in		char(15)	= null,
	@va_Alternat3_in		char(15)	= null,
	@va_Alternat4_in		char(15)	= null,
	@va_Alternat5_in		char(15)	= null,
	@va_Clignotement1_in		tinyint		= null,
	@va_Clignotement2_in		tinyint		= null,
	@va_Clignotement3_in		tinyint		= null,
	@va_Clignotement4_in		tinyint		= null,
	@va_Clignotement5_in		tinyint		= null,
	@va_Flash_in			tinyint		= null,
	@va_Priorite_in			tinyint		= null,
	@va_DistanceEvt_in		int		= null,
	@va_DepassSeuil_in		tinyint		= null,
	@va_site_origine_in		T_SITE		= null,
	@va_Dest_un_proche_in		smallint	= null,
	@va_Dest_un_lointaine_in	smallint	= null,
	@va_Dest_deux_proche_in		smallint	= null,	
	@va_Dest_deux_lointaine_in	smallint	= null,
	@va_distant_in 			int 		= XDC_NON
as
	declare @vl_Site tinyint, @vl_NomSite char(2), @vl_NumActionPrecedente int,
		@vl_NumEvtActionPrecedente int, @vl_CleEvtActionPrecedente tinyint,
		@vl_EtatIndispo int, @vl_EtatPbMineur int,
		@vl_NomMachine char(10), @vl_Status int, @vl_Clignotement1 bit,
		@vl_Clignotement2 bit, @vl_Clignotement3 bit,@vl_Clignotement4 bit,@vl_Clignotement5 bit,
		@vl_Texte char(100), 
		@vl_NomEqt char(10), @vl_PMVTexte tinyint, 
		@vl_NumMaitre T_EQUIPEMENT, @vl_TypeMaitre tinyint,
		@vl_TypeEqt tinyint, @vl_NumEqt T_EQUIPEMENT, @vl_MachineNonTrouvee bit,
		@vl_PR T_PR, @vl_Sens T_SENS, @vl_Autoroute T_AUTOROUTE, 
		@vl_Intitule varchar(50),@vl_NumeroAction int, @vl_Commande int,
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
	/* Traitement au district d'exploitation pour Escota, en local sinon */
	begin
		if @vl_serveur_pra = XDC_SQLDP
			exec @vl_Status = SQL_DS.PRC..XZAC682 @va_Horodate_in, @va_NumEvt_in, @va_CleEvt_in,
						@va_Operateur_in, @va_NumEqt_in, @va_Site_in,
						@va_Ligne1_in, @va_Ligne2_in, @va_Ligne3_in,
						@va_Ligne4_in,@va_Ligne5_in,
						@va_Alternat1_in, @va_Alternat2_in, @va_Alternat3_in,
						@va_Alternat4_in,@va_Alternat5_in,
						@va_Clignotement1_in, @va_Clignotement2_in, @va_Clignotement3_in,
						@va_Clignotement4_in,@va_Clignotement5_in,
						@va_Flash_in, @va_Priorite_in,
						@va_DistanceEvt_in,
						@va_DepassSeuil_in,
						@va_site_origine_in,
						@va_Dest_un_proche_in, @va_Dest_un_lointaine_in, @va_Dest_deux_proche_in, @va_Dest_deux_lointaine_in, XDC_OUI 
		else if @vl_serveur_pra = XDC_SQLVC
			exec @vl_Status = SQL_DM.PRC..XZAC682 @va_Horodate_in, @va_NumEvt_in, @va_CleEvt_in,
						@va_Operateur_in, @va_NumEqt_in, @va_Site_in,
						@va_Ligne1_in, @va_Ligne2_in, @va_Ligne3_in,
						@va_Ligne4_in,@va_Ligne5_in,
						@va_Alternat1_in, @va_Alternat2_in, @va_Alternat3_in,
						@va_Alternat4_in,@va_Alternat5_in,
						@va_Clignotement1_in, @va_Clignotement2_in, @va_Clignotement3_in,
						@va_Clignotement4_in,@va_Clignotement5_in,
						@va_Flash_in, @va_Priorite_in,
						@va_DistanceEvt_in,
						@va_DepassSeuil_in,
						@va_site_origine_in,
						@va_Dest_un_proche_in, @va_Dest_un_lointaine_in, @va_Dest_deux_proche_in, @va_Dest_deux_lointaine_in, XDC_OUI 
		else if @vl_serveur_pra = XDC_SQLCI
			exec @vl_Status = SQL_CI.PRC..XZAC682 @va_Horodate_in, @va_NumEvt_in, @va_CleEvt_in,
						@va_Operateur_in, @va_NumEqt_in, @va_Site_in,
						@va_Ligne1_in, @va_Ligne2_in, @va_Ligne3_in,
						@va_Ligne4_in,@va_Ligne5_in,
						@va_Alternat1_in, @va_Alternat2_in, @va_Alternat3_in,
						@va_Alternat4_in,@va_Alternat5_in,
						@va_Clignotement1_in, @va_Clignotement2_in, @va_Clignotement3_in,
						@va_Clignotement4_in,@va_Clignotement5_in,
						@va_Flash_in, @va_Priorite_in,
						@va_DistanceEvt_in,
						@va_DepassSeuil_in,
						@va_site_origine_in,
						@va_Dest_un_proche_in, @va_Dest_un_lointaine_in, @va_Dest_deux_proche_in, @va_Dest_deux_lointaine_in, XDC_OUI
		else return XDC_NOK

		return @vl_Status
	end

	/*! transfo en parametres nulls bug applix */
	if @va_Operateur_in = 0 select @va_Operateur_in = null
	if @va_NumEqt_in = 0 select @va_NumEqt_in = null
	if @va_Clignotement1_in = 0 select @va_Clignotement1_in = null
	if @va_Clignotement2_in = 0 select @va_Clignotement2_in = null
	if @va_Clignotement3_in = 0 select @va_Clignotement3_in = null
	if @va_Clignotement4_in = 0 select @va_Clignotement4_in = null
	if @va_Clignotement5_in = 0 select @va_Clignotement5_in = null
	if @va_Flash_in = 0 select @va_Flash_in = null
	if @va_DistanceEvt_in = XDC_DISTANCE_INCONNUE 
		select @va_DistanceEvt_in = null, @va_Priorite_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A controle des parametres obligatoires */
	if @va_Operateur_in = null or
	   @va_Ligne1_in = null or @va_Ligne2_in = null or @va_Ligne3_in = null or 
	   @va_Ligne4_in = null or @va_Ligne5_in = null or
	   @va_Clignotement1_in = null or @va_Clignotement2_in = null or 
	   @va_Clignotement3_in = null or @va_Clignotement4_in = null or
	   @va_Clignotement5_in = null
		return XDC_ARG_INV

	/*A toutes les chaines nulles sont positionnees a " " */
	/*! pour eviter pb pour recuperer donnees             */
	if @va_Alternat1_in = null 
		select @va_Alternat1_in	= XDC_CHAINE_VIDE
	if @va_Alternat2_in = null 
		select @va_Alternat2_in	= XDC_CHAINE_VIDE
	if @va_Alternat3_in = null 
		select @va_Alternat3_in	= XDC_CHAINE_VIDE
	if @va_Alternat4_in = null 
		select @va_Alternat4_in	= XDC_CHAINE_VIDE
	if @va_Alternat5_in = null 
		select @va_Alternat5_in	= XDC_CHAINE_VIDE

	/*A initialisation des indicateurs de clignotement */
	select	@vl_Clignotement1 = XDC_NON,
		@vl_Clignotement2 = XDC_NON,
		@vl_Clignotement3 = XDC_NON,
		@vl_Clignotement4 = XDC_NON,
		@vl_Clignotement5= XDC_NON


	/*A Verifier l'existence de l'equipement sur le site et determiner sa disponibilite */
	select	@vl_EtatPbMineur = mineure,
		@vl_EtatIndispo = (majeure | critique | HS | inhibe | desactive)
	from	EXP..EQT_DSP
	where	type = XDC_EQT_PMVA  and  equipement = @va_NumEqt_in  and
		sit = @vl_Site  and
		dernier = 1

	if @@rowcount = 0
		return XDC_NOK
	if @vl_EtatIndispo <> 0
		return XDC_NOK


	/*A indicateurs de clignotement pour la base de donnee */
	if @va_Clignotement1_in != XDC_PMVA_PASCLIGNO
		select @vl_Clignotement1 = XDC_OUI
	if @va_Clignotement2_in != XDC_PMVA_PASCLIGNO
		select @vl_Clignotement2 = XDC_OUI
	if @va_Clignotement3_in != XDC_PMVA_PASCLIGNO
		select @vl_Clignotement3 = XDC_OUI
	if @va_Clignotement4_in != XDC_PMVA_PASCLIGNO
		select @vl_Clignotement4 = XDC_OUI
	if @va_Clignotement5_in != XDC_PMVA_PASCLIGNO
		select @vl_Clignotement5 = XDC_OUI

	/*A Rechercher la derniere action en cours sur cet equipement */
	select @vl_NumeroAction = max(numero)
	from EXP..ACT_GEN
	where   equipement = @va_NumEqt_in and
		sit = @va_Site_in and type = XDC_ACT_PMVA

	if @@rowcount = 0
		select @vl_NumeroAction = 0

	/*A initialisation des var locales pour la recherche */
	/*A du nom de la machine pilotant le PMVA */
	select	@vl_MachineNonTrouvee = XDC_NON,
		@vl_NumEqt = @va_NumEqt_in,
		@vl_TypeEqt = XDC_EQT_PMVA

	/*A recherche le nom de la machine pilotant le PMVA */
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
	select @vl_PMVTexte = XDC_PMVA_TEXTE

	select @vl_Commande = - @vl_NumeroAction

#if ! defined (DEV)
	/*A affichage sur l'equipement (le plus tot possible) */

	if @vl_Site = XDC_CI
		exec @vl_Status = TASRV_CI...ACRP11 @va_NumEqt_in, @va_Ligne1_in, @vl_PMVTexte,
		@va_Clignotement1_in, @va_Alternat1_in, @va_Ligne2_in, @vl_PMVTexte,
		@va_Clignotement2_in, @va_Alternat2_in, @va_Ligne3_in, @vl_PMVTexte,
		@va_Clignotement3_in, @va_Alternat3_in, @va_Ligne4_in, @vl_PMVTexte,
		@va_Clignotement4_in, @va_Alternat4_in, @va_Ligne5_in, @vl_PMVTexte,
		@va_Clignotement5_in, @va_Alternat5_in,@va_Flash_in,  
		@va_DepassSeuil_in,
		@va_Dest_un_proche_in, @va_Dest_un_lointaine_in, @va_Dest_deux_proche_in, @va_Dest_deux_lointaine_in,
		@vl_Commande, @vl_NomMachine


	else if @vl_Site = XDC_VC
		exec @vl_Status = TASRV_VC...ACRP11 @va_NumEqt_in, @va_Ligne1_in, @vl_PMVTexte,
		@va_Clignotement1_in, @va_Alternat1_in, @va_Ligne2_in, @vl_PMVTexte,
		@va_Clignotement2_in, @va_Alternat2_in, @va_Ligne3_in, @vl_PMVTexte,
		@va_Clignotement3_in, @va_Alternat3_in, @va_Ligne4_in, @vl_PMVTexte,
		@va_Clignotement4_in, @va_Alternat4_in, @va_Ligne5_in, @vl_PMVTexte,
		@va_Clignotement5_in, @va_Alternat5_in,@va_Flash_in,
		@va_DepassSeuil_in,
		@va_Dest_un_proche_in, @va_Dest_un_lointaine_in, @va_Dest_deux_proche_in, @va_Dest_deux_lointaine_in,
		@vl_Commande, @vl_NomMachine

	else if @vl_Site = XDC_DP
		exec @vl_Status = TASRV_DP...ACRP11 @va_NumEqt_in, @va_Ligne1_in, @vl_PMVTexte,
		@va_Clignotement1_in, @va_Alternat1_in, @va_Ligne2_in, @vl_PMVTexte,
		@va_Clignotement2_in, @va_Alternat2_in, @va_Ligne3_in, @vl_PMVTexte,
		@va_Clignotement3_in, @va_Alternat3_in, @va_Ligne4_in, @vl_PMVTexte,
		@va_Clignotement4_in, @va_Alternat4_in, @va_Ligne5_in, @vl_PMVTexte,
		@va_Clignotement5_in, @va_Alternat5_in,@va_Flash_in, 	
		@va_DepassSeuil_in,
		@va_Dest_un_proche_in, @va_Dest_un_lointaine_in, @va_Dest_deux_proche_in, @va_Dest_deux_lointaine_in,
		@vl_Commande, @vl_NomMachine

	else if @vl_Site = XDC_CA
		exec @vl_Status = TASRV_CA...ACRP11 @va_NumEqt_in, @va_Ligne1_in, @vl_PMVTexte,
		@va_Clignotement1_in, @va_Alternat1_in, @va_Ligne2_in, @vl_PMVTexte,
		@va_Clignotement2_in, @va_Alternat2_in, @va_Ligne3_in, @vl_PMVTexte,
		@va_Clignotement3_in, @va_Alternat3_in, @va_Ligne4_in, @vl_PMVTexte,
		@va_Clignotement4_in, @va_Alternat4_in, @va_Ligne5_in, @vl_PMVTexte,
		@va_Clignotement5_in, @va_Alternat5_in,@va_Flash_in, 	
		@va_DepassSeuil_in,
		@va_Dest_un_proche_in, @va_Dest_un_lointaine_in, @va_Dest_deux_proche_in, @va_Dest_deux_lointaine_in,
		@vl_Commande, @vl_NomMachine

	if @vl_Status != XDC_OK
		return @vl_Status
#endif	/* pas en mode DEV */


	begin tran

	       /*A mettre a jour la date de lancement fin s'il existe une action a terminee */
		if @vl_NumeroAction != 0
		begin
			update EXP..ACT_GEN set heure_lancement_fin = @va_Horodate_in
			where   numero = @vl_NumeroAction and sit = @va_Site_in and
				heure_lancement_fin is null

			update EXP..ACT_PMA set depass_seuil = XDC_FAUX
			where actio=@vl_NumeroAction and sit = @va_Site_in
		end

#ifndef CI
		/* Mettre a jour la trace des mises au neutre */
		exec XZAC841  XDC_EQT_PMVA, @va_NumEqt_in, @va_Horodate_in, XDC_PMVA_TDP
#endif

	commit tran


#if defined (DEV)
	/*A Si l'equipement est en alarme mineure alors retourner un cr d'erreur */
	if @vl_EtatPbMineur <> 0
		return XDC_NOK
#endif

	return XDC_OK
go
