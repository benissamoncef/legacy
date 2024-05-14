/*
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac82.prc
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
* JMG           02/10/09        : secto DEM887
* VR	25/05/12		: Ajout DTP PMVA (DEM/1014 PMA)
* PNI	28/08/13	: suppression verif existance FMC pour SAPIENS au CI (delais de propagation FMC via repli) DEM 1074 1.5
* PNI	08/04/15	: d�placement du calcul du no d'action dans la transaction DEM1117 1.6
* JMG	06/03/17	: regionalisation code region  DEM1220 v1.7
* JPL	08/03/17	: Enregistrement des neutres 3points et Temps de Parcours en table dediee (DEM 1196)  1.8
* LCL	03/07/17	: Modif PRA
* JPL	05/09/17 	: Mode DEV: pas de commande equipement; action en succes, ou echec avec alerte si pb. mineur eqt. (DEM 1231)  1.10
* JMG	19/11/19 	: EQT_FIL DEM1347 1.11
* LCL	11/03/20 	: MOVIS DEM-SAE93 1.12
* GRO	12/10/2020      : MOVIS DEM-SAE93 1.13											
* JMG	02/11/20 	: cle double EQT_FIL 1.14
* LCL/RGR 14/12/20	: Correction distance -1 => priorit� � 0
* ABE	05/01/21 	: MOVIS ajout serveur PRA DEM-SAE93 1.15
* ABE	07/02/22 	: Fix gestion Flash PRA DEM-SAE311
* LCL	27/09/23 : Adaptations CNA DEM-483	
* JPL	20/10/23	: Pas de FMC d'Essai ou Exercice en attente dans EQT_FIL (SAE_525) 
-----------------------------------------------------*/

/*-----------------------------------------------------
* Service rendu
* Action d'affichage PMVA.
* 
* Sequence d'appel
* SP	XZAC82_Commande_PMVA
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

if exists (select 1 from sysobjects where name = 'XZAC82' and type = 'P')
	drop procedure XZAC82
go

create procedure XZAC82
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
	@va_NumeroAction_out		int		= null	output,
	@va_distant_in 			int 		= XDC_NON
as
	declare @vl_Site tinyint, @vl_NomSite char(2), @vl_NumActionPrecedente int,
		@vl_NumEvtActionPrecedente int, @vl_CleEvtActionPrecedente tinyint,
		@vl_HeureSucces datetime, @vl_HeureEchec datetime,
		@vl_EtatIndispo int, @vl_EtatPbMineur int,
		@vl_NomMachine char(10), @vl_Status int, @vl_Clignotement1 bit,
		@vl_Clignotement2 bit, @vl_Clignotement3 bit,@vl_Clignotement4 bit,@vl_Clignotement5 bit, @vl_Flash bit,
		@vl_Texte char(100), 
		@vl_NomEqt char(10), @vl_PMVTexte tinyint, 
		@vl_NumMaitre T_EQUIPEMENT, @vl_TypeMaitre tinyint,
		@vl_TypeEqt tinyint, @vl_NumEqt T_EQUIPEMENT, @vl_MachineNonTrouvee bit,
		@vl_PR T_PR, @vl_Sens T_SENS, @vl_Autoroute T_AUTOROUTE, 
		@vl_Intitule varchar(50),
		@vl_serveur_pra char(6)	
		 
	/* recherche le site local */
	/* MOVIS */
	if @va_site_origine_in = null
	        select @vl_Site = numero 
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
	/* MOVIS execution toujours en local */
	if @vl_Site != @va_Site_in and @vl_Site = XDC_CI and @va_distant_in = XDC_NON		
	begin
		if @vl_serveur_pra=XDC_SQLDP
			exec @vl_Status = SQL_DS.PRC..XZAC82 @va_Horodate_in, @va_NumEvt_in, @va_CleEvt_in,
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
						@va_Dest_un_proche_in, @va_Dest_un_lointaine_in, @va_Dest_deux_proche_in, @va_Dest_deux_lointaine_in,
						@va_NumeroAction_out output, 
						XDC_OUI 
		else if @vl_serveur_pra=XDC_SQLVC
			exec @vl_Status = SQL_DM.PRC..XZAC82 @va_Horodate_in, @va_NumEvt_in, @va_CleEvt_in,
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
						@va_Dest_un_proche_in, @va_Dest_un_lointaine_in, @va_Dest_deux_proche_in, @va_Dest_deux_lointaine_in,
						@va_NumeroAction_out output, 
						XDC_OUI 
		else if @vl_serveur_pra=XDC_SQLCI
			exec @vl_Status = SQL_CI.PRC..XZAC82 @va_Horodate_in, @va_NumEvt_in, @va_CleEvt_in,
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
						@va_Dest_un_proche_in, @va_Dest_un_lointaine_in, @va_Dest_deux_proche_in, @va_Dest_deux_lointaine_in,
						@va_NumeroAction_out output, 
						XDC_OUI 
		else return XDC_NOK


		return @vl_Status
	end

	/*! transfo en param�tres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
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
	if @va_NumEvt_in = null or @va_CleEvt_in = null or @va_NumEqt_in = null or
	   @va_Operateur_in = null or
	   @va_Ligne1_in = null or @va_Ligne2_in = null or @va_Ligne3_in = null or
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

	if @va_DistanceEvt_in = null
		select @va_DistanceEvt_in = 0

	/*A initialisation des indicateurs de clignotement */
	select	@vl_Clignotement1 = XDC_NON,
		@vl_Clignotement2 = XDC_NON,
		@vl_Clignotement3 = XDC_NON,
		@vl_Clignotement4 = XDC_NON,
		@vl_Clignotement5= XDC_NON,
		@vl_Flash= XDC_NON

	select	@vl_HeureSucces = null, @vl_HeureEchec = null


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
	where	type = XDC_EQT_PMVA  and  equipement = @va_NumEqt_in  and
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
	if @va_Flash_in !=  XDC_PMVA_PASCLIGNO
		select @vl_Flash = XDC_OUI

	/*A inserer l'actio PMV */
	begin tran
		/*A calcul le numero d'actio a inserer */
		select @va_NumeroAction_out = isnull(max(numero), 0) + 1 from EXP..ACT_GEN
		where sit = @vl_Site

		insert EXP..ACT_GEN ( numero, sit, evenement, cle, type, equipement,
		heure_lancement, heure_succes, heure_echec,
		priorite, prioritaire, distance_evt, operateur, sit_origine)
		values (@va_NumeroAction_out, @vl_Site, @va_NumEvt_in, @va_CleEvt_in,
		XDC_ACT_PMVA,  @va_NumEqt_in,
		@va_Horodate_in, @vl_HeureSucces, @vl_HeureEchec,
		@va_Priorite_in, XDC_NON, @va_DistanceEvt_in, @va_Operateur_in, @va_site_origine_in)

		insert EXP..ACT_PMA (actio, PMVA, sit, premiere_ligne, deuxieme_ligne,
		troisieme_ligne, quatrieme_ligne, cinquieme_ligne, premiere_ligne_alternat, deuxieme_ligne_alternat,
		troisieme_ligne_alternat, quatrieme_ligne_alternat, cinquieme_ligne_alternat, clignotement1,
		clignotement2, clignotement3, clignotement4,clignotement5,depass_seuil, flash,
		dest_un_proche, dest_un_lointaine, dest_deux_proche, dest_deux_lointaine)
		values ( @va_NumeroAction_out,  @va_NumEqt_in, @vl_Site, 
		@va_Ligne1_in, @va_Ligne2_in, @va_Ligne3_in, @va_Ligne4_in, @va_Ligne5_in, 
		@va_Alternat1_in, @va_Alternat2_in, @va_Alternat3_in,@va_Alternat4_in,@va_Alternat5_in,
		@vl_Clignotement1,
		@vl_Clignotement2, @vl_Clignotement3, @vl_Clignotement4,@vl_Clignotement5,
		@va_DepassSeuil_in, @vl_Flash,
		@va_Dest_un_proche_in, @va_Dest_un_lointaine_in, @va_Dest_deux_proche_in, @va_Dest_deux_lointaine_in)

#ifndef CI
		/* Mettre a jour la trace des mises au neutre */
		exec XZAC841  XDC_EQT_PMVA, @va_NumEqt_in, @va_Horodate_in, XDC_PMVA_TEXTE
#endif

		/* on vient de piloter le PMVA pour cette FMC*/
		/* donc on le supprime de EQT_FIL*/
		delete EXP..EQT_FIL
		where evenement = @va_NumEvt_in
		and cle = @va_CleEvt_in
		and type_equipement = XDC_EQT_PMVA
		and equipement = @va_NumEqt_in
	commit tran


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

#if ! defined (DEV)
	/*A affichage sur l'equipement (le plus tot possible) */
	/* PRA
	if @@servername = XDC_SQLCI
	*/
	if @vl_Site = XDC_CI
		exec @vl_Status = TASRV_CI...ACRP11 @va_NumEqt_in, @va_Ligne1_in, @vl_PMVTexte,
		@va_Clignotement1_in, @va_Alternat1_in, @va_Ligne2_in, @vl_PMVTexte,
		@va_Clignotement2_in, @va_Alternat2_in, @va_Ligne3_in, @vl_PMVTexte,
		@va_Clignotement3_in, @va_Alternat3_in, @va_Ligne4_in, @vl_PMVTexte,
		@va_Clignotement4_in, @va_Alternat4_in, @va_Ligne5_in, @vl_PMVTexte,
		@va_Clignotement5_in, @va_Alternat5_in,@va_Flash_in,  
		@va_DepassSeuil_in,
		@va_Dest_un_proche_in, @va_Dest_un_lointaine_in, @va_Dest_deux_proche_in, @va_Dest_deux_lointaine_in,
		@va_NumeroAction_out, @vl_NomMachine

	/* PRA
	else if  @@servername = XDC_SQLVC
	*/
	else if @vl_Site = XDC_VC
		exec @vl_Status = TASRV_VC...ACRP11 @va_NumEqt_in, @va_Ligne1_in, @vl_PMVTexte,
		@va_Clignotement1_in, @va_Alternat1_in, @va_Ligne2_in, @vl_PMVTexte,
		@va_Clignotement2_in, @va_Alternat2_in, @va_Ligne3_in, @vl_PMVTexte,
		@va_Clignotement3_in, @va_Alternat3_in, @va_Ligne4_in, @vl_PMVTexte,
		@va_Clignotement4_in, @va_Alternat4_in, @va_Ligne5_in, @vl_PMVTexte,
		@va_Clignotement5_in, @va_Alternat5_in,@va_Flash_in,
		@va_DepassSeuil_in,
		@va_Dest_un_proche_in, @va_Dest_un_lointaine_in, @va_Dest_deux_proche_in, @va_Dest_deux_lointaine_in,
		@va_NumeroAction_out, @vl_NomMachine
	/* PRA
	else if  @@servername = XDC_SQLDP
	*/
	else if @vl_Site = XDC_DP
		exec @vl_Status = TASRV_DP...ACRP11 @va_NumEqt_in, @va_Ligne1_in, @vl_PMVTexte,
		@va_Clignotement1_in, @va_Alternat1_in, @va_Ligne2_in, @vl_PMVTexte,
		@va_Clignotement2_in, @va_Alternat2_in, @va_Ligne3_in, @vl_PMVTexte,
		@va_Clignotement3_in, @va_Alternat3_in, @va_Ligne4_in, @vl_PMVTexte,
		@va_Clignotement4_in, @va_Alternat4_in, @va_Ligne5_in, @vl_PMVTexte,
		@va_Clignotement5_in, @va_Alternat5_in,@va_Flash_in, 	
		@va_DepassSeuil_in,
		@va_Dest_un_proche_in, @va_Dest_un_lointaine_in, @va_Dest_deux_proche_in, @va_Dest_deux_lointaine_in,
		@va_NumeroAction_out, @vl_NomMachine
	/* PRA
	else if  @@servername = XDC_SQLCA
	*/
	else if @vl_Site = XDC_CA
		exec @vl_Status = TASRV_CA...ACRP11 @va_NumEqt_in, @va_Ligne1_in, @vl_PMVTexte,
		@va_Clignotement1_in, @va_Alternat1_in, @va_Ligne2_in, @vl_PMVTexte,
		@va_Clignotement2_in, @va_Alternat2_in, @va_Ligne3_in, @vl_PMVTexte,
		@va_Clignotement3_in, @va_Alternat3_in, @va_Ligne4_in, @vl_PMVTexte,
		@va_Clignotement4_in, @va_Alternat4_in, @va_Ligne5_in, @vl_PMVTexte,
		@va_Clignotement5_in, @va_Alternat5_in,@va_Flash_in, 	
		@va_DepassSeuil_in,
		@va_Dest_un_proche_in, @va_Dest_un_lointaine_in, @va_Dest_deux_proche_in, @va_Dest_deux_lointaine_in,
		@va_NumeroAction_out, @vl_NomMachine

	if @vl_Status != XDC_OK
		return @vl_Status
#endif	/* pas en mode DEV */


	/*A Rechercher l'actio precedente sur cet equipement non terminee */
	select 
		@vl_NumActionPrecedente = EXP..ACT_GEN.numero,
		@vl_NumEvtActionPrecedente = EXP..ACT_GEN.evenement,
		@vl_CleEvtActionPrecedente = EXP..ACT_GEN.cle,
		@vl_Autoroute = CFG..EQT_GEN.autoroute,
		@vl_PR = CFG..EQT_GEN.PR,
		@vl_Sens = CFG..EQT_GEN.sens
	from EXP..ACT_GEN, CFG..EQT_GEN
	where	EXP..ACT_GEN.equipement = @va_NumEqt_in and 
		EXP..ACT_GEN.type = XDC_ACT_PMVA and
		EXP..ACT_GEN.sit = @vl_Site and 
		EXP..ACT_GEN.numero != @va_NumeroAction_out and
		CFG..EQT_GEN.numero = @va_NumEqt_in and 
		CFG..EQT_GEN.type = XDC_EQT_PMVA and
		EXP..ACT_GEN.heure_lancement_fin is null and heure_lancement in
		(select max(heure_lancement) from EXP..ACT_GEN
		 where	equipement = @va_NumEqt_in and type = XDC_ACT_PMVA and
			numero != @va_NumeroAction_out)

	if @@rowcount = 1
	begin
		/*A Positionner l'heure de lancement de fin                     */
		update EXP..ACT_GEN set heure_lancement_fin = @va_Horodate_in
#if defined (DEV)
		                        , heure_fin = @va_Horodate_in
#endif
		where  numero = @vl_NumActionPrecedente and sit = @vl_Site


		/* Si l'evenement lie a l'actio precedente est different alors */
		if @vl_NumEvtActionPrecedente != @va_NumEvt_in or 
		   @vl_CleEvtActionPrecedente != @va_CleEvt_in
		begin
			select 1 from EXP..FMC_GEN
			where numero = @vl_NumEvtActionPrecedente  and  cle = @vl_CleEvtActionPrecedente
			and origine_creation not like '%ESSAI%'
			and origine_creation not like '%EXERCICE%'
			and origine_creation not like '%SUPPRIMER%'

			if @@rowcount = 1
			begin
				/* mettre l ancien evt dans EQT_FIL, en le supprimant s'il y etait deja */
				delete EXP..EQT_FIL
				where evenement = @vl_NumEvtActionPrecedente
				and cle = @vl_CleEvtActionPrecedente
				and type_equipement = XDC_EQT_PMVA
				and equipement = @va_NumEqt_in
				and sit = @vl_Site

				/* quand distance -1 (liaison fiche op�rateur) */
				if @va_Priorite_in = null
					select @va_Priorite_in = 0

				insert into EXP..EQT_FIL (evenement, cle, type_equipement, equipement,
								priorite, distance, sit)
				values (@vl_NumEvtActionPrecedente,
					@vl_CleEvtActionPrecedente,
					XDC_EQT_PMVA, @va_NumEqt_in,
					@va_Priorite_in, @va_DistanceEvt_in, @vl_Site)
			end

			/* puis reveiller l'evenement lie a l'actio precedente */
			/*! on considere qu'aucun probleme de reveil puisse arriver */

			/* nommage de l'equipement */
			exec XZAC;60 XDC_EQT_PMVA, @vl_Autoroute, @vl_PR, @vl_Sens, @vl_Intitule output

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
		exec PRC..XZAA;01 @va_Horodate_in, XDC_EQT_PMVA, @va_NumEqt_in, XZAAC_AFF_DIF, "Affichage PMVA"
		return XDC_NOK
	end
#endif

	return XDC_OK
go
