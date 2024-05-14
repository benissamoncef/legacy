/*
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac1001.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	16/09/94	: Creation
* C.T.	23/09/94	: Modification du traitement de
*                         recherche du nom de la machine
*                         pilotant
* C.T.	28/09/94	: Modification interface ajout flash
*                         et mise a " " des chaines nulles
* C.T.	10/11/94	: Modification bugs applix (V 1.4)
* C.T.	16/11/94	: Modification du declenchement du reveil
*                         (appel ARRP03 au lieu XZAR;03)
*                         non teste (V 1.5)
* C.T.	05/12/94	: Correction erreur nom maitre V 1.6
*                         non teste
* C.T.	14/02/95	: Correction du controle sur la 
*                         disponiblite V 1.7
* C.T.	14/03/95	: Modif longueur texte reveil (V 1.9)
* C.T.	28/03/95	: Modif texte reveil (V 1.10)
* P.V.  14/08/96	: Ajout enreg 4eme ligne PMV (V1.11) (ANA30)
* C.T.	06/09/96	: Piltage a distance pour le CI (RADTV1.12)
* P.N.  28/02/97	: Ajout des destibation et du flag reveil TDP (dem/1398) V1.13
* C.P.	07/01/98	: Correction gestion multi-sites dans les procs (1.15)
* JMG	07/01/98        : Re Correction gestion multi-sites dans les procs (1.16) (dem/1536)
* JMG	02/03/98	: ajout sit_origine (dem/1605) 1.17
* PNI	26/01/07	: Un va_TextePicto_in ne provoque plus d'erreur 1.18 DEM584 SAPIENS
* LCL   09/07/07        : Suppression DY 1.19
* PNI	22/04/08	: Suppression du test eqt disponible DEM706 1.20
* JPL	26/11/08	: Ajout du pilotage du secteur DM au CI (DEM 846) 1.21
* JMG           02/10/09        : secto DEM887 1.22
* VR	07/11/11	: Ajout DestinationNoeud (DEM/1014)
* JPL	06/03/13	: Reveil sur VD; Ordre instructions - IDEM PRECEDENTE  1.24
* PNI	28/08/13	: suppression verif existance FMC pour SAPIENS au CI (delais de propagation FMC via repli) DEM 1074 1.25
* JMG	06/03/17	: regionalisation code region  DEM1220 v1.26
* JPL	08/03/17	: Enregistrement des neutres 3points et Temps de Parcours en table dediee (DEM 1196)  1.27
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.27
* LCL 	29/06/17	: modif PRA
* JPL	05/09/17 : Mode DEV: pas de commande equipement; action en succes, ou echec avec alerte si pb. mineur eqt. (DEM 1231)  1.29
* JMG	08/08/18 : ligne 18 car suppression SAGA DEM1306 1.30
* JMG	03/12/19 : EQT_FIL DEM1347 1.31
* LCL	26/03/20 : MOVIS DEM-SAE93 1.32
* GGY	28/04/23 : Augmentation taille va_TextePicto_in a char(XDC_LGR_TXT_PICTO) et va_TypePicto_in (XDC_LGR_NOM_PICTO) (DEM-473)
* LCL	28/09/23 	: Adaptations CNA DEM-483	
* JPL	20/10/23 : Pas de FMC d'Essai ou Exercice en attente dans EQT_FIL (SAE_525)  1.33
-----------------------------------------------------*/

/*-----------------------------------------------------
* Service rendu
* Action d'affichage PMV+picto.
* 
* Sequence d'appel
* SP	XZAC1001_Commande_PMV
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
* XDY_LignePMV	va_Alternat1_in
* XDY_LignePMV	va_Alternat2_in
* XDY_LignePMV	va_Alternat3_in
* XDY_Octet	va_Clignotement1_in
* XDY_Octet	va_Clignotement2_in
* XDY_Octet	va_Clignotement3_in
* XDY_Octet	va_Flash_in
* char[XDC_LGR_NOM_PICTO]	va_TypePicto_in
* char[XDC_LGR_TXT_PICTO]	va_TextePicto_in
* XDY_Octet	va_ClignotementPicto_in
* XDY_Octet	va_Priorite_in
* XDY_Entier	va_DistanceEvt_in
* XDY_LignePMV	va_LigneDir_in
* XDY_LignePMV	va_AlternatDir_in
* XDY_Mot	va_Destination_in
* XDY_Mot       va_DestinationAlterna_in
* XDY_Octet	va_depass_seuil_in
* XDY_Site	va_site_origine_in
* XDY_Mot	va_DestinationNoeud_in
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
* ACRP01 :	Appeler XZEP01 pour afficher sur l'equipement PMV
* 		Appeler XZEP02 pour afficher le picto
* 
-----------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAC1001' and type = 'P')
	drop procedure XZAC1001
go

create procedure XZAC1001
	@va_Horodate_in			datetime	= null,
	@va_NumEvt_in			int		= null,
	@va_CleEvt_in			tinyint		= null,
	@va_Operateur_in		T_OPERATEUR	= null,
	@va_NumEqt_in			T_EQUIPEMENT	= null,
	@va_Site_in			T_SITE		= null,
	@va_Ligne1_in			char(18)	= null,
	@va_Ligne2_in			char(18)	= null,
	@va_Ligne3_in			char(18)	= null,
	@va_Alternat1_in		char(18)	= null,
	@va_Alternat2_in		char(18)	= null,
	@va_Alternat3_in		char(18)	= null,
	@va_Clignotement1_in		tinyint		= null,
	@va_Clignotement2_in		tinyint		= null,
	@va_Clignotement3_in		tinyint		= null,
	@va_Flash_in			tinyint		= null,
	@va_TypePicto_in		char(XDC_LGR_NOM_PICTO)		= null,
	@va_TextePicto_in		char(XDC_LGR_TXT_PICTO)		= null,
	@va_ClignotementPicto_in	tinyint		= null,
	@va_Priorite_in			tinyint		= null,
	@va_DistanceEvt_in		int		= null,
	@va_LigneDir_in			char(18)	= null,
	@va_AlternatDir_in		char(18)	= null,
	@va_Destination_in		smallint	= null,
	@va_DestinationAlternat_in       smallint        = null,
	@va_DepassSeuil_in		tinyint		= null,
	@va_site_origine_in		T_SITE		= null,
	@va_NumeroAction_out		int		= null	output,
	@va_DestinationNoeud_in smallint	= null
as
	declare @vl_Site tinyint, @vl_NomSite char(2), @vl_NumActionPrecedente int,
		@vl_NumEvtActionPrecedente int, @vl_CleEvtActionPrecedente tinyint,
		@vl_HeureLancFinActionPrec datetime,
		@vl_HeureSucces datetime, @vl_HeureEchec datetime,
		@vl_EtatIndispo int, @vl_EtatPbMineur int,
		@vl_NomMachine char(10), @vl_Status int, @vl_Clignotement1 bit,
		@vl_Clignotement2 bit, @vl_Clignotement3 bit,
		@vl_ClignotementPicto bit, @vl_Texte char(100), 
		@vl_NomEqt char(10), @vl_PMVTexte tinyint, 
		@vl_NumMaitre T_EQUIPEMENT, @vl_TypeMaitre tinyint,
		@vl_TypeEqt tinyint, @vl_NumEqt T_EQUIPEMENT, @vl_MachineNonTrouvee bit,
		@vl_PR T_PR, @vl_Sens T_SENS, @vl_Autoroute T_AUTOROUTE, 
		@vl_Intitule varchar(50)
		 

	/*! transfo en parametres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_Operateur_in = 0 select @va_Operateur_in = null
	if @va_NumEqt_in = 0 select @va_NumEqt_in = null
	if @va_Clignotement1_in = 0 select @va_Clignotement1_in = null
	if @va_Clignotement2_in = 0 select @va_Clignotement2_in = null
	if @va_Clignotement3_in = 0 select @va_Clignotement3_in = null
	if @va_Flash_in = 0 select @va_Flash_in = null
	if @va_ClignotementPicto_in = 0 select @va_ClignotementPicto_in = null
	if @va_DistanceEvt_in = XDC_DISTANCE_INCONNUE 
		select @va_DistanceEvt_in = null, @va_Priorite_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A controle des parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null or @va_NumEqt_in = null or
	   @va_Operateur_in = null or @va_TypePicto_in = null or
	   @va_Ligne1_in = null or @va_Ligne2_in = null or @va_Ligne3_in = null or
	   @va_Clignotement1_in = null or @va_Clignotement2_in = null or 
	   @va_Clignotement3_in = null
		return XDC_ARG_INV

	/*A toutes les chaines nulles sont positionnees a " " */
	/*! pour eviter pb pour recuperer donnees             */
	if @va_Alternat1_in = null 
		select @va_Alternat1_in	= XDC_CHAINE_VIDE
	if @va_Alternat2_in = null 
		select @va_Alternat2_in	= XDC_CHAINE_VIDE
	if @va_Alternat3_in = null 
		select @va_Alternat3_in	= XDC_CHAINE_VIDE
	if @va_TextePicto_in = null 
		select @va_TextePicto_in = XDC_CHAINE_VIDE

	/*A initialisation des indicateurs de clignotement */
	select	@vl_Clignotement1 = XDC_NON,
		@vl_Clignotement2 = XDC_NON,
		@vl_Clignotement3 = XDC_NON,
		@vl_ClignotementPicto = XDC_NON

	if @va_Clignotement1_in != XDC_PMV_PASCLIGNO
		select @vl_Clignotement1 = XDC_OUI
	if @va_Clignotement2_in != XDC_PMV_PASCLIGNO
		select @vl_Clignotement2 = XDC_OUI
	if @va_Clignotement3_in != XDC_PMV_PASCLIGNO
		select @vl_Clignotement3 = XDC_OUI
	if @va_ClignotementPicto_in != XDC_PICTO_PASCLIGNO
		select @vl_ClignotementPicto = XDC_OUI

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
	where	type = XDC_EQT_PMV  and  equipement = @va_NumEqt_in  and
		sit = @vl_Site  and
		dernier = 1
#if defined (DEV)
	if @@rowcount = 0
		return XDC_NOK
	if @vl_EtatIndispo <> 0
		return XDC_NOK
	if @vl_EtatPbMineur <> 0
		select @vl_HeureEchec = @va_Horodate_in
	else
		select @vl_HeureSucces = @va_Horodate_in
#else
/*DEM706*/
	if @@rowcount = 0
		return XDC_OK
	if @vl_EtatIndispo <> 0
		return XDC_OK
#endif


	/*A recherche le nom de la machine pilotant le PMV */
	select	@vl_MachineNonTrouvee = XDC_NON,
		@vl_NumEqt = @va_NumEqt_in,
		@vl_TypeEqt = XDC_EQT_PMV

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


	/*A test si le PMV a un flash si le flash a ete demande */
	if @va_Flash_in = XDC_PMV_FLASH
	begin
		/*B recherche si le PMV a un flash */
		if not exists (select * from CFG..EQT_PMV, CFG..TYP_PMV
				where	CFG..EQT_PMV.numero = @vl_NumEqt and 
					CFG..TYP_PMV.numero = CFG..EQT_PMV.type and
					CFG..TYP_PMV.flash = XDC_OUI)
			return XDC_NOK
	end


	begin tran

		/*A calculer le numero d'actio a inserer */
		select @va_NumeroAction_out = isnull(max(numero), 0) + 1 from EXP..ACT_GEN
		where sit = @vl_Site

		/*A inserer l'actio PMV */
		insert EXP..ACT_GEN ( numero, sit, evenement, cle, type, equipement,
		heure_lancement, heure_succes, heure_echec,
		priorite, prioritaire, distance_evt, operateur, sit_origine)
		values (@va_NumeroAction_out, @vl_Site, @va_NumEvt_in, @va_CleEvt_in, XDC_ACT_PMV, @va_NumEqt_in,
		@va_Horodate_in, @vl_HeureSucces, @vl_HeureEchec,
		@va_Priorite_in, XDC_NON, @va_DistanceEvt_in, @va_Operateur_in, @va_site_origine_in)

		insert EXP..ACT_PMV (actio, PMV, sit, premiere_ligne, deuxieme_ligne,
		troisieme_ligne, premiere_ligne_alternat, deuxieme_ligne_alternat,
		troisieme_ligne_alternat, pictogramme, bandeau, clignotement1,
		clignotement2, clignotement3, clignotement_picto,direction_ligne,
		direction_ligne_alternat,destination,destination_alternat,depass_seuil,destination_noeud,flash)
		values ( @va_NumeroAction_out,  @va_NumEqt_in, @vl_Site, 
		@va_Ligne1_in, @va_Ligne2_in, @va_Ligne3_in, 
		@va_Alternat1_in, @va_Alternat2_in, @va_Alternat3_in,
		@va_TypePicto_in, @va_TextePicto_in, @vl_Clignotement1,
		@vl_Clignotement2, @vl_Clignotement3, @vl_ClignotementPicto,
		@va_LigneDir_in,@va_AlternatDir_in,@va_Destination_in,@va_DestinationAlternat_in,@va_DepassSeuil_in, @va_DestinationNoeud_in, @va_Flash_in)

#ifndef CI
		/* Mettre a jour la trace des mises au neutre */
		exec XZAC841  XDC_EQT_PMV, @va_NumEqt_in, @va_Horodate_in, XDC_PMV_TEXTE
#endif

		/* on vient de piloter le PMV pour cette FMC*/
		/* donc on le supprime de EQT_FIL*/
		delete EXP..EQT_FIL
		where evenement = @va_NumEvt_in
		and cle = @va_CleEvt_in
		and type_equipement = XDC_EQT_PMV
		and equipement = @va_NumEqt_in

	commit tran


	/*A initialisation des types d'affichage */
	select @vl_PMVTexte = XDC_PMV_TEXTE

/*#if ! defined (DEV) */
	/*A affichage sur l'equipement (le plus tot possible) */
	/*! le numero picto est egal au numero pmv            */
	/* PRA
	if @@servername = XDC_SQLCI
	*/
	if @vl_Site = XDC_CI
		exec @vl_Status = TASRV_CI...ACRP01 @va_NumEqt_in, @va_Ligne1_in, @vl_PMVTexte,
		@va_Clignotement1_in, @va_Alternat1_in, @va_Ligne2_in, @vl_PMVTexte,
		@va_Clignotement2_in, @va_Alternat2_in, @va_Ligne3_in, @vl_PMVTexte,
		@va_Clignotement3_in, @va_Alternat3_in, @va_Flash_in, @va_NumEqt_in,
		@va_TextePicto_in, @va_TypePicto_in, @va_ClignotementPicto_in, 
		@va_NumeroAction_out, @vl_NomMachine,@va_LigneDir_in,@va_AlternatDir_in,
		@va_Destination_in, @va_DestinationAlternat_in, @va_DepassSeuil_in, @va_DestinationNoeud_in
	else /* PRA
	     if  @@servername = XDC_SQLVC
	     */
	    if @vl_Site = XDC_VC
			exec @vl_Status = TASRV_VC...ACRP01 @va_NumEqt_in, @va_Ligne1_in, @vl_PMVTexte,
			@va_Clignotement1_in, @va_Alternat1_in, @va_Ligne2_in, @vl_PMVTexte,
			@va_Clignotement2_in, @va_Alternat2_in, @va_Ligne3_in, @vl_PMVTexte,
			@va_Clignotement3_in, @va_Alternat3_in, @va_Flash_in, @va_NumEqt_in,
			@va_TextePicto_in, @va_TypePicto_in, @va_ClignotementPicto_in,
			@va_NumeroAction_out, @vl_NomMachine,@va_LigneDir_in,@va_AlternatDir_in,
			@va_Destination_in, @va_DestinationAlternat_in, @va_DepassSeuil_in, @va_DestinationNoeud_in
	else /* PRA
	     if  @@servername = XDC_SQLDP
	     */
	    if @vl_Site = XDC_DP
			exec @vl_Status = TASRV_DP...ACRP01 @va_NumEqt_in, @va_Ligne1_in, @vl_PMVTexte,
			@va_Clignotement1_in, @va_Alternat1_in, @va_Ligne2_in, @vl_PMVTexte,
			@va_Clignotement2_in, @va_Alternat2_in, @va_Ligne3_in, @vl_PMVTexte,
			@va_Clignotement3_in, @va_Alternat3_in, @va_Flash_in, @va_NumEqt_in,
			@va_TextePicto_in, @va_TypePicto_in, @va_ClignotementPicto_in,
			@va_NumeroAction_out, @vl_NomMachine,@va_LigneDir_in,@va_AlternatDir_in,
			@va_Destination_in, @va_DestinationAlternat_in, @va_DepassSeuil_in, @va_DestinationNoeud_in
	else
		if @vl_Site = XDC_CA
			exec @vl_Status = TASRV_CA...ACRP01 @va_NumEqt_in, @va_Ligne1_in, @vl_PMVTexte,
			@va_Clignotement1_in, @va_Alternat1_in, @va_Ligne2_in, @vl_PMVTexte,
			@va_Clignotement2_in, @va_Alternat2_in, @va_Ligne3_in, @vl_PMVTexte,
			@va_Clignotement3_in, @va_Alternat3_in, @va_Flash_in, @va_NumEqt_in,
			@va_TextePicto_in, @va_TypePicto_in, @va_ClignotementPicto_in,
			@va_NumeroAction_out, @vl_NomMachine,@va_LigneDir_in,@va_AlternatDir_in,
			@va_Destination_in, @va_DestinationAlternat_in, @va_DepassSeuil_in, @va_DestinationNoeud_in

	if @vl_Status != XDC_OK
		return @vl_Status
/*#endif */ /* pas en mode DEV */


	/*A Rechercher l'actio precedente sur cet equipement non terminee */
	select 
		@vl_NumActionPrecedente = EXP..ACT_GEN.numero,
		@vl_NumEvtActionPrecedente = EXP..ACT_GEN.evenement,
		@vl_CleEvtActionPrecedente = EXP..ACT_GEN.cle,
		@vl_HeureLancFinActionPrec = EXP..ACT_GEN.heure_lancement_fin
	from EXP..ACT_GEN
	where	EXP..ACT_GEN.equipement = @va_NumEqt_in and 
		EXP..ACT_GEN.type = XDC_ACT_PMV and
		EXP..ACT_GEN.sit = @vl_Site and 
		EXP..ACT_GEN.numero != @va_NumeroAction_out and
		EXP..ACT_GEN.heure_lancement_fin is null and heure_lancement in
		(select max(heure_lancement) from EXP..ACT_GEN
		 where	equipement = @va_NumEqt_in and type = XDC_ACT_PMV and
			numero != @va_NumeroAction_out)

	if @@rowcount <> 1
	begin
		select @vl_NumActionPrecedente = null
	end


	/* Si l'actio precedente n'est pas terminee alors */
	if (@vl_NumActionPrecedente is not null) and (@vl_HeureLancFinActionPrec is null)
	begin
		/*A Positionner l'heure de lancement de fin                     */
		update EXP..ACT_GEN set heure_lancement_fin = @va_Horodate_in
#if defined (DEV)
		                        , heure_fin = @va_Horodate_in
#endif
		where  numero = @vl_NumActionPrecedente and sit = @vl_Site

		update EXP..ACT_PMV set depass_seuil=XDC_FAUX
		where  actio = @vl_NumActionPrecedente and sit = @vl_Site

	
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
				insert into EXP..EQT_FIL (evenement, cle, type_equipement,
							equipement, priorite, distance, sit)
				values (@vl_NumEvtActionPrecedente,
						@vl_CleEvtActionPrecedente,
						XDC_EQT_PMV,
						@va_NumEqt_in,
						0,0, @vl_Site)
			end

			/* puis reveiller l'evenement lie a l'actio precedente */
			/*! on considere qu'aucun probleme de reveil puisse arriver */
			select	@vl_Autoroute = CFG..EQT_GEN.autoroute,
				@vl_PR = CFG..EQT_GEN.PR,
				@vl_Sens = CFG..EQT_GEN.sens
			from	CFG..EQT_GEN
			where	type = XDC_EQT_PMV and
				numero = @va_NumEqt_in

			/* nommage de l'equipement */
			exec XZAC;60 XDC_EQT_PMV, @vl_Autoroute, @vl_PR, @vl_Sens, @vl_Intitule output

			/* texte du reveil */
			select @vl_Texte = XDC_REV_VOL_EQT + @vl_Intitule + XDC_REV_VOL_EQT_SUITE,
				@vl_NomMachine = null

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
		exec PRC..XZAA;01 @va_Horodate_in, XDC_EQT_PMV, @va_NumEqt_in, XZAAC_AFF_DIF, "Affichage PMV"
		return XDC_NOK
	end
#endif

	return XDC_OK
go
