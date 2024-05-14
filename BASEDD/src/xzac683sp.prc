/*E*/
/* Fichier : $Id: xzac683sp.prc,v 1.6 2021/03/08 14:19:34 pc2dpdy Exp $        Release : $Revision: 1.6 $        Date : $Date: 2021/03/08 14:19:34 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac683.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR/MG	20/08/12	: Ajout DTP PMVA (DEM/1014 PMA)
* JMG	17/03/17	: regio
* LCL	03/07/17	: Modif PRA
* LCL   25/03/20	: MOVIS Ajout site local DEM-SAE93
* GRO	12/10/2020      : MOVIS DEM-SAE93
* CGR	05/01/21	: MOVIS ajout PRA DEM-SAE93 1.6
* LCL	28/09/23 : Adaptations CNA DEM-483	
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Action d'affichage PMVA.
* 
* Sequence d'appel
* SP	XZAC683_Commande_PMVA
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

if exists (select * from sysobjects where name = 'XZAC683' and type = 'P')
	drop procedure XZAC683
go

create procedure XZAC683
	@va_Horodate_in			datetime	= null,
	@va_NumEvt_in			int		= null,
	@va_CleEvt_in			tinyint		= null,
	@va_Operateur_in		smallint	= null,
	@va_NumEqt_in			smallint	= null,
	@va_Site_in			tinyint		= null,
	@va_Ligne1_in			char(18)	= null,
	@va_Ligne2_in			char(18)	= null,
	@va_Ligne3_in			char(18)	= null,
	@va_Ligne4_in			char(18)	= null,
	@va_Ligne5_in			char(18)	= null,
	@va_Alternat1_in		char(18)	= null,
	@va_Alternat2_in		char(18)	= null,
	@va_Alternat3_in		char(18)	= null,
	@va_Alternat4_in		char(18)	= null,
	@va_Alternat5_in		char(18)	= null,
	@va_Clignotement_in		tinyint		= null,
	@va_Picto_in                    char(10)        = null,
	@va_Luminosite_in               smallint        = null,
	@va_Bandeau_in                  char(10)        = null,
	@va_Barriere_in                 smallint        = null,
	@va_Scenario_in                 smallint        = null,
	@va_Priorite_in			tinyint		= null,
	@va_DistanceEvt_in		int		= null,
	@va_site_origine_in		T_SITE		= null,
	@va_distant_in 			int 		= XDC_NON
as
	declare @vl_Site tinyint, @vl_NomSite char(2), @vl_NumActionPrecedente int,
		@vl_NumEvtActionPrecedente int, @vl_CleEvtActionPrecedente tinyint,
		@vl_NomMachine char(10), @vl_Status int, @vl_Clignotement tinyint,
		@vl_Texte char(100), @vl_trouve int,
		@vl_NomEqt char(10), @vl_PMVTexte tinyint, 
		@vl_NumMaitre T_EQUIPEMENT, @vl_TypeMaitre tinyint,
		@vl_TypeEqt tinyint, @vl_NumEqt T_EQUIPEMENT, @vl_MachineNonTrouvee bit,
		@vl_PR T_PR, @vl_Sens T_SENS, @vl_Autoroute T_AUTOROUTE, 
		@vl_Intitule varchar(50),@vl_NumeroAction int, @vl_Commande int,
		@vl_type_saga tinyint,
		@vl_serveur_pra char(6)
		 
	/* recherche le site local */
        select  @vl_Site = @va_site_origine_in
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
		if @vl_serveur_pra = XDC_SQLVC
			exec @vl_Status = SQL_DM.PRC..XZAC683 @va_Horodate_in, @va_NumEvt_in, @va_CleEvt_in,
						@va_Operateur_in, @va_NumEqt_in, @va_Site_in,
						@va_Ligne1_in, @va_Ligne2_in, @va_Ligne3_in,
						@va_Ligne4_in,@va_Ligne5_in,
						@va_Alternat1_in, @va_Alternat2_in, @va_Alternat3_in,
						@va_Alternat4_in,@va_Alternat5_in,
						@va_Clignotement_in,@va_Picto_in, @va_Luminosite_in,
						@va_Bandeau_in, @va_Barriere_in, @va_Scenario_in,
						@va_Priorite_in,
						@va_DistanceEvt_in,
						@va_site_origine_in, 
						XDC_OUI 
		else if @vl_serveur_pra = XDC_SQLDP
			exec @vl_Status = SQL_DS.PRC..XZAC683 @va_Horodate_in, @va_NumEvt_in, @va_CleEvt_in,
						@va_Operateur_in, @va_NumEqt_in, @va_Site_in,
						@va_Ligne1_in, @va_Ligne2_in, @va_Ligne3_in,
						@va_Ligne4_in,@va_Ligne5_in,
						@va_Alternat1_in, @va_Alternat2_in, @va_Alternat3_in,
						@va_Alternat4_in,@va_Alternat5_in,
						@va_Clignotement_in, @va_Picto_in, @va_Luminosite_in,
						@va_Bandeau_in, @va_Barriere_in, @va_Scenario_in,
						@va_Priorite_in,
						@va_DistanceEvt_in,
						@va_site_origine_in, 
						XDC_OUI 
		else if @vl_serveur_pra = XDC_SQLCI
			exec @vl_Status = SQL_CI.PRC..XZAC683 @va_Horodate_in, @va_NumEvt_in, @va_CleEvt_in,
						@va_Operateur_in, @va_NumEqt_in, @va_Site_in,
						@va_Ligne1_in, @va_Ligne2_in, @va_Ligne3_in,
						@va_Ligne4_in,@va_Ligne5_in,
						@va_Alternat1_in, @va_Alternat2_in, @va_Alternat3_in,
						@va_Alternat4_in,@va_Alternat5_in,
						@va_Clignotement_in, @va_Picto_in, @va_Luminosite_in,
						@va_Bandeau_in, @va_Barriere_in, @va_Scenario_in,
						@va_Priorite_in,
						@va_DistanceEvt_in,
						@va_site_origine_in,
						XDC_OUI
		return @vl_Status
	end

	/*! transfo en parametres nulls bug applix */
	if @va_Operateur_in = 0 select @va_Operateur_in = null
	if @va_NumEqt_in = 0 select @va_NumEqt_in = null
	if @va_Clignotement_in = 0 select @va_Clignotement_in = null
	if @va_DistanceEvt_in = XDC_DISTANCE_INCONNUE 
		select @va_DistanceEvt_in = null, @va_Priorite_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A controle des parametres obligatoires */
	if @va_Operateur_in = null 
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
	select	@vl_Clignotement = XDC_NON

	/*A verifier la non indisponibilite de l'equipement et */
	/*A son existence sur ce district                      */
	if not exists ( select equipement from EXP..EQT_DSP
			where	equipement = @va_NumEqt_in and type = XDC_EQT_SAGA and
				dernier = 1 and
				majeure = XDC_NON and 
				critique = XDC_NON and HS = XDC_NON and 
				inhibe = XDC_NON and desactive = XDC_NON and 
				sit = @vl_Site)
		return XDC_NOK

	select @vl_type_saga = type_saga
	from CFG..EQT_SAGA
	where numero = @va_NumEqt_in

	/*A indicateurs de clignotement pour la base de donnee */
	if @va_Clignotement_in != XDC_PMVA_PASCLIGNO
		select @vl_Clignotement = XDC_OUI

	
	/*terminer les actions scenario pour lesquelles*/
	/*mon scenario est le scenatrio de fin qui est dans luminosite*/
	select @vl_trouve=0

	/*A Rechercher la derniere action en cours sur cet equipement */
	select @vl_NumeroAction = max(numero)
	from EXP..ACT_GEN
	where   equipement = @va_NumEqt_in and
		sit = @va_Site_in and type = XDC_ACT_SAGA
		and heure_lancement_fin is null
	if @@rowcount = 0
		select @vl_NumeroAction = 0
	if @va_Scenario_in != 0
	begin
		update EXP..ACT_GEN
		set heure_lancement_fin = @va_Horodate_in, heure_fin = @va_Horodate_in
		where sit = @va_Site_in and type = XDC_ACT_SAGA
		and equipement = @va_NumEqt_in and
		heure_lancement_fin is null and
		numero in (select GEN.numero from EXP..ACT_GEN GEN, EXP..ACT_SAGA SAG
		where GEN.sit = @va_Site_in and GEN.type = XDC_ACT_SAGA
		and GEN.equipement = @va_NumEqt_in and
		GEN.heure_lancement_fin is null
		and GEN.numero = SAG.actio and
		SAG.sit = @va_Site_in and
		SAG.luminosite = @va_Scenario_in)
	
		if @@rowcount > 0
		select @vl_trouve= 1
	end
	else
	begin
       /*A mettre a jour la date de lancement fin s'il existe une action a terminee */
        if @vl_NumeroAction != 0
        begin
                update EXP..ACT_GEN set heure_lancement_fin = @va_Horodate_in, heure_fin = @va_Horodate_in
                where   numero = @vl_NumeroAction and sit = @va_Site_in and
                        heure_lancement_fin is null

        end
	end

	/*creer la nouvelle action qui est forcement en lancement_fin*/
	if @va_Scenario_in != 0 and @vl_trouve =0
        begin 
		begin tran
                select @vl_NumeroAction = isnull(max(numero), 0) + 1 from EXP..ACT_GEN
                where sit = @vl_Site

               insert EXP..ACT_GEN ( numero, sit, evenement, cle, type, equipement,
                heure_lancement, heure_lancement_fin,priorite, prioritaire, distance_evt, operateur, sit_origine)
                values (@vl_NumeroAction, @vl_Site, @va_NumEvt_in, @va_CleEvt_in,
                XDC_ACT_SAGA,  @va_NumEqt_in, @va_Horodate_in,@va_Horodate_in, @va_Priorite_in,
                XDC_NON, @va_DistanceEvt_in, @va_Operateur_in, @va_site_origine_in)
		commit tran

                insert EXP..ACT_SAGA (actio, equipement, sit, premiere_ligne, deuxieme_ligne,
                troisieme_ligne, quatrieme_ligne, cinquieme_ligne, premiere_ligne_alternat, deuxieme_ligne_alternat,
                troisieme_ligne_alternat, quatrieme_ligne_alternat, cinquieme_ligne_alternat, clignotement,
                picto, luminosite, bandeau,barriere,scenario )
                values ( @vl_NumeroAction,  @va_NumEqt_in, @vl_Site,
                @va_Ligne1_in, @va_Ligne2_in, @va_Ligne3_in, @va_Ligne4_in, @va_Ligne5_in,
                @va_Alternat1_in, @va_Alternat2_in, @va_Alternat3_in,@va_Alternat4_in,@va_Alternat5_in,
                @vl_Clignotement,@va_Picto_in, @va_Luminosite_in, @va_Bandeau_in, @va_Barriere_in,
                @va_Scenario_in)
	end 
	
	if @vl_type_saga = XDC_SAGA_META_EQT and @va_Scenario_in = 0
	  return XDC_OK

	/*A initialisation des var locales pour la recherche */
	/*A du nom de la machine pilotant le PMVA */
	select	@vl_MachineNonTrouvee = XDC_NON,
		@vl_NumEqt = @va_NumEqt_in,
		@vl_TypeEqt = XDC_EQT_SAGA

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
	
	/*A affichage sur l'equipement (le plus tot possible) */

	if @vl_Site = XDC_CI
		exec @vl_Status = TASRV_CI...ACRP111 @va_NumEqt_in, @va_Ligne1_in, @vl_PMVTexte,
		@va_Alternat1_in, @va_Ligne2_in, @vl_PMVTexte,
		@va_Alternat2_in, @va_Ligne3_in, @vl_PMVTexte,
		@va_Alternat3_in, @va_Ligne4_in, @vl_PMVTexte,
		@va_Alternat4_in, @va_Ligne5_in, @vl_PMVTexte,
		@va_Alternat5_in,@vl_Clignotement, @va_Luminosite_in, @va_Picto_in, @va_Bandeau_in,
		@va_Barriere_in, @va_Scenario_in,
		@vl_Commande, @vl_NomMachine


	else if @vl_Site = XDC_VC
		exec @vl_Status = TASRV_DM...ACRP111 @va_NumEqt_in, @va_Ligne1_in, @vl_PMVTexte,
		@va_Alternat1_in, @va_Ligne2_in, @vl_PMVTexte,
		@va_Alternat2_in, @va_Ligne3_in, @vl_PMVTexte,
		@va_Alternat3_in, @va_Ligne4_in, @vl_PMVTexte,
		@va_Alternat4_in, @va_Ligne5_in, @vl_PMVTexte,
		@va_Alternat5_in, @vl_Clignotement, @va_Luminosite_in, @va_Picto_in, @va_Bandeau_in,
		@va_Barriere_in, @va_Scenario_in,
		@vl_Commande, @vl_NomMachine

	else if @vl_Site = XDC_DP
		exec @vl_Status = TASRV_DP...ACRP111 @va_NumEqt_in, @va_Ligne1_in, @vl_PMVTexte,
		@va_Alternat1_in, @va_Ligne2_in, @vl_PMVTexte,
		@va_Alternat2_in, @va_Ligne3_in, @vl_PMVTexte,
		@va_Alternat3_in, @va_Ligne4_in, @vl_PMVTexte,
		@va_Alternat4_in, @va_Ligne5_in, @vl_PMVTexte,
		@va_Alternat5_in, @vl_Clignotement, @va_Luminosite_in, @va_Picto_in, @va_Bandeau_in,
		@va_Barriere_in, @va_Scenario_in,
		@vl_Commande, @vl_NomMachine

	else if @vl_Site = XDC_CA
		exec @vl_Status = TASRV_CA...ACRP111 @va_NumEqt_in, @va_Ligne1_in, @vl_PMVTexte,
		@va_Alternat1_in, @va_Ligne2_in, @vl_PMVTexte,
		@va_Alternat2_in, @va_Ligne3_in, @vl_PMVTexte,
		@va_Alternat3_in, @va_Ligne4_in, @vl_PMVTexte,
		@va_Alternat4_in, @va_Ligne5_in, @vl_PMVTexte,
		@va_Alternat5_in, @vl_Clignotement, @va_Luminosite_in, @va_Picto_in, @va_Bandeau_in,
		@va_Barriere_in, @va_Scenario_in,
		@vl_Commande, @vl_NomMachine

	if @vl_Status != XDC_OK
		return @vl_Status

	return XDC_OK
go
