/*E*/
/* Fichier : $Id: xzac640sp.prc,v 1.7 2021/03/08 14:25:52 pc2dpdy Exp $        Release : $Revision: 1.7 $        Date : $Date: 2021/03/08 14:25:52 $
-------------------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* MODULE TANEU * FICHIER xzac640sp.prc
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Mise au neutre d'un PMV[A] SAGA.
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	29/02/17 : Creation (DEM 1221)  1.1
* JPL	31/08/17 : Recherche du nom du serveur local en table des sites  1.2
* JPL	05/09/17 : Mode DEV: pas de commande equipement; status d'echec si pb. mineur eqt. (DEM 1231)  1.3
* JPL	04/04/18 : Prise en compte des messages de neutres programm�s (DEM 1278)  1.4
* LCL	20/11/19 : MOVIS DEM-SAE93 1.5
* GRO	12/10/20 : MOVIS DEM-SAE93 1.51		
* LCL	19/11/20 : Correction erreur PRA 1.7
* CGR	05/01/21 : MOVIS ajout PRA DEM-SAE93 1.7
* LCL	28/09/23 : Adaptations CNA DEM-483	
-----------------------------------------------------------------------------------------*/


/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU
*
*  Effectue la mise au neutre d'un equipement SAGA.
*  Restreint aux PMV et PMVA, le type SAGA est verifie.
*  Si le panneau fait l'objet d'une action en cours,
*   la mise au neutre n'est commandee que si l'indicateur de forcage le permet.
*
*  RESTRICTION POUR CAUSE D'AFFICHAGE PAR SCENARIO SANS ACTION :
*  En mode automatique periodique (parametre de fin d'action non positionne),
*   un panneau affichant du Texte n'est mis au neutre que si ce texte
*   est celui d'une action de neutre programm� qui n'est plus active.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL
*
* SP	XZAC640_Mise_Au_Neutre_PMVx_SAGA
*
* PARAMETRES EN ENTREE :
*
* XDY_Eqt		va_NumEqt_in		: Numero de l'equipement SAGA
* XDY_Site		va_Site_in		: Numero du site
* XDY_Texte		va_Texte_Affiche_in	: Texte actuellement affiche par le panneau
* XDY_Octet		va_Fin_Action_in	: Vrai pour terminer l'action en cours (IHM), Faux si automatique
* XDY_Ligne_PMV_SAGA	va_Ligne1_TDP_in	: \
* XDY_Ligne_PMV_SAGA	va_Ligne2_TDP_in	:  |      Texte de message
* XDY_Ligne_PMV_SAGA	va_Ligne3_TDP_in	:  | de type "Temps de Parcours"
* XDY_Ligne_PMV_SAGA	va_Ligne4_TDP_in	:  |         a afficher
* XDY_Ligne_PMV_SAGA	va_Ligne5_TDP_in	: /
* XDY_Ligne_PMV_SAGA	va_Alternat1_in		: \
* XDY_Ligne_PMV_SAGA	va_Alternat2_in		:  |      Texte de message
* XDY_Ligne_PMV_SAGA	va_Alternat3_in		:  |         alternatif
* XDY_Ligne_PMV_SAGA	va_Alternat4_in		:  |         a afficher
* XDY_Ligne_PMV_SAGA	va_Alternat5_in		: /
*
*
* PARAMETRES EN SORTIE :
*
* XDY_Entier	va_NumeroAction_out	: Action en cours, ou effectuee par la commande
* XDY_Octet	va_Type_Neutre_out	: Resultat de la commande
*
*   0			Aucune modification d'etat demandee
*   XDC_PMVA_DEFAUT	Commande de passage au neutre par defaut
*   XDC_PMVA_TEXTE	Commande d'affichage de neutre programme
*   XDC_PMVA_TDP	Commande d'affichage de Temps de Parcours
*
* CODE RETOUR :
*
* XDC_OK
* XDC_NOK           : equipement ou machine le pilotant inconnu(e)
* XDC_ARG_INV       : parametre d'entree obligatoire manquant (Null) ou invalide
* XDC_PRC_INC       : procedure appelee inexistante (ACRPxx)
* <0                : erreur sybase
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
*  Consulte l'etat du PMV ou PMVA indique, notamment l'action en cours s'il y en a une.
*
*  Si le panneau n'est pas utilise (aucune action n'est en cours) alors
*     si un affichage Temps de Parcours est specifie en parametres d'entree
*     alors le commander.
*
-----------------------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAC640' and type = 'P')
	drop procedure XZAC640
go


create procedure XZAC640
	@va_NumEqt_in			T_EQUIPEMENT	= null,
	@va_Site_in			T_SITE		= null,
	@va_Texte_Affiche_in		char(100)	= null,
	@va_Fin_Action_in		tinyint		= null,
	@va_Ligne1_TDP_in		char(18)	= null,
	@va_Ligne2_TDP_in		char(18)	= null,
	@va_Ligne3_TDP_in		char(18)	= null,
	@va_Ligne4_TDP_in		char(18)	= null,
	@va_Ligne5_TDP_in		char(18)	= null,
	@va_Alternat1_in		char(18)	= null,
	@va_Alternat2_in		char(18)	= null,
	@va_Alternat3_in		char(18)	= null,
	@va_Alternat4_in		char(18)	= null,
	@va_Alternat5_in		char(18)	= null,
	@va_NumeroAction_out		int		= null output,
	@va_Type_Neutre_out		tinyint		= null output,
	@va_NomSiteLocal_in		char(2)		= null,
	@va_distant_in 			int 		= XDC_NON
as
	declare	@vl_Site		T_SITE,
		@vl_type_saga		tinyint,
		@vl_NomMachine		T_NOM_MACHINE,
		@vl_MachineServeur	T_NOM_MACHINE,
		@vl_PosteServeur	T_EQUIPEMENT,

		@vl_NumAction_Der	int,
		@vl_NumFmc_Der		int,
		@vl_CleFmc_Der		T_DISTRICT,
		@vl_Priorite_Der	tinyint,
		@vl_HeureLanc_Der	datetime,
		@vl_HeureLancFin_Der	datetime,
		@vl_Texte_Action_Der	char(100),

		@vl_NumFmc_Prog		int,
		@vl_CleFmc_Prog		T_DISTRICT,
		@vl_Debut_Prog		datetime,
		@vl_Fin_ProgPrec	datetime,

		@vl_Commande		int,
		@vl_Ligne_3Points	char(18),
		@vl_Ligne1		char(18),
		@vl_Ligne2		char(18),
		@vl_Ligne3		char(18),
		@vl_Ligne4		char(18),
		@vl_Ligne5		char(18),
		@vl_Alternat1		char(18),
		@vl_Alternat2		char(18),
		@vl_Alternat3		char(18),
		@vl_Alternat4		char(18),
		@vl_Alternat5		char(18),
		@vl_TypeLigne		tinyint,
		@vl_Cligno_Non		tinyint,
		@vl_Picto		char(10),
		@vl_Luminosite		smallint,
		@vl_Bandeau		char(10),
		@vl_Barriere		smallint,
		@vl_Scenario		smallint,

		@vl_Ligne1_Prog		char(18),
		@vl_Ligne2_Prog		char(18),
		@vl_Ligne3_Prog		char(18),
		@vl_Ligne4_Prog		char(18),
		@vl_Ligne5_Prog		char(18),
		@vl_Alternat1_Prog	char(18),
		@vl_Alternat2_Prog	char(18),
		@vl_Alternat3_Prog	char(18),
		@vl_Alternat4_Prog	char(18),
		@vl_Alternat5_Prog	char(18),
		@vl_TypePicto_Prog	char(10),
		@vl_TextePicto_Prog	char(10),
		@vl_Mode_Prog		tinyint,

		@vl_Maintenant		datetime,
		@vl_HeureSucces		datetime,
		@vl_HeureEchec		datetime,
		@vl_EtatIndispo		int,
		@vl_EtatPbMineur	int,
		@vl_Status		int,
		@vl_serveur_pra		char(6)


	/* Determiner le site local */
	/* MOVIS */
	if @va_NomSiteLocal_in = null
		select	@vl_Site = numero,
			@vl_MachineServeur = machine
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
		
		select	@vl_MachineServeur = machine
		from CFG..RES_DIS
		where code = @vl_Site
	end

	if @vl_Site is null
		return XDC_NOK

	/* Init serveur PRA */
	select @vl_serveur_pra=serveur_pra from CFG..RES_PRA where numero=@va_Site_in

	/* Au CI, executer la commande en cascade sur le secteur pilotant l'equipement */
	if @vl_Site = XDC_CI and @va_distant_in = XDC_NON		
	/* Traitement au district d'exploitation pour Escota, en local sinon */
	begin
		if @vl_serveur_pra = XDC_SQLVC
			exec @vl_Status = SQL_DM.PRC..XZAC640 @va_NumEqt_in, @va_Site_in,
			                     @va_Texte_Affiche_in, @va_Fin_Action_in,
			                     @va_Ligne1_TDP_in, @va_Ligne2_TDP_in, @va_Ligne3_TDP_in,
			                     @va_Ligne4_TDP_in, @va_Ligne5_TDP_in,
			                     @va_Alternat1_in, @va_Alternat2_in, @va_Alternat3_in,
			                     @va_Alternat4_in, @va_Alternat5_in,
			                     @va_NumeroAction_out output, @va_Type_Neutre_out output, @va_NomSiteLocal_in, XDC_OUI 
		else if @vl_serveur_pra = XDC_SQLDP
			exec @vl_Status = SQL_DS.PRC..XZAC640 @va_NumEqt_in, @va_Site_in,
			                     @va_Texte_Affiche_in, @va_Fin_Action_in,
			                     @va_Ligne1_TDP_in, @va_Ligne2_TDP_in, @va_Ligne3_TDP_in,
			                     @va_Ligne4_TDP_in, @va_Ligne5_TDP_in,
			                     @va_Alternat1_in, @va_Alternat2_in, @va_Alternat3_in,
			                     @va_Alternat4_in, @va_Alternat5_in,
			                     @va_NumeroAction_out output, @va_Type_Neutre_out output, @va_NomSiteLocal_in, XDC_OUI 
		else if @vl_serveur_pra = XDC_SQLCI
			exec @vl_Status = SQL_CI.PRC..XZAC640 @va_NumEqt_in, @va_Site_in,
					     @va_Texte_Affiche_in, @va_Fin_Action_in,
					     @va_Ligne1_TDP_in, @va_Ligne2_TDP_in, @va_Ligne3_TDP_in,
					     @va_Ligne4_TDP_in, @va_Ligne5_TDP_in,
					     @va_Alternat1_in, @va_Alternat2_in, @va_Alternat3_in,
					     @va_Alternat4_in, @va_Alternat5_in,
					     @va_NumeroAction_out output, @va_Type_Neutre_out output, @va_NomSiteLocal_in, XDC_OUI
		else return XDC_NOK

		return @vl_Status
	end

	/* Transformation des parametres d'entree nulls */
	if @va_NumEqt_in = 0	select @va_NumEqt_in = null
	if @va_Ligne1_TDP_in = null
		select @va_Ligne1_TDP_in = XDC_CHAINE_VIDE
	if @va_Ligne2_TDP_in = null
		select @va_Ligne2_TDP_in = XDC_CHAINE_VIDE
	if @va_Ligne3_TDP_in = null
		select @va_Ligne3_TDP_in = XDC_CHAINE_VIDE
	if @va_Ligne4_TDP_in = null
		select @va_Ligne4_TDP_in = XDC_CHAINE_VIDE
	if @va_Ligne5_TDP_in = null
		select @va_Ligne5_TDP_in = XDC_CHAINE_VIDE
	if @va_Alternat1_in = null
		select @va_Alternat1_in = XDC_CHAINE_VIDE
	if @va_Alternat2_in = null
		select @va_Alternat2_in = XDC_CHAINE_VIDE
	if @va_Alternat3_in = null
		select @va_Alternat3_in = XDC_CHAINE_VIDE
	if @va_Alternat4_in = null
		select @va_Alternat4_in = XDC_CHAINE_VIDE
	if @va_Alternat5_in = null
		select @va_Alternat5_in = XDC_CHAINE_VIDE


	/* Resultats par defaut */
	select	@va_NumeroAction_out = 0,
		@va_Type_Neutre_out = 0


	/*A controle des parametres d'entree obligatoires */
	if @va_NumEqt_in = null
		return XDC_ARG_INV


	/*A Verifier la concordance des sites */
	/* MOVIS */
	/*
	if @va_Site_in <> @vl_Site
		return XDC_NOK
	*/


	select	@vl_Maintenant = getdate()
	select	@vl_HeureSucces = null, @vl_HeureEchec = null


	select	@vl_PosteServeur = numero
	from	CFG..EQT_GEN
	where	type = XDC_EQT_MAC  and  nom = @vl_MachineServeur


	/*A Supprimer les programmations sur FMC closes */
	delete	EXP..PRG_PMV_SAGA from EXP..PRG_PMV_SAGA P, EXP..FMC_GEN F
	where	(F.numero = evenement  and  F.cle = P.cle)  and cloture is not null

	delete	EXP..PRG_PMA_SAGA from EXP..PRG_PMA_SAGA P, EXP..FMC_GEN F
	where	(F.numero = evenement  and  F.cle = P.cle)  and cloture is not null


	/*A Verifier l'existence et le type de l'equipement */
	select	@vl_type_saga = type_saga
	from	CFG..EQT_SAGA
	where	numero = @va_NumEqt_in

	if @vl_type_saga not in (
		XDC_SAGA_PMV_1_18,
		XDC_SAGA_PMV_2_18,
		XDC_SAGA_PMV_2_15,
		XDC_SAGA_PMV_3_15,
		XDC_SAGA_PMV_2_18_P,
		XDC_SAGA_PMV_3_15_P,
		XDC_SAGA_PMVA
	)
		return XDC_NOK


	/*A Verifier la presence de l'equipement sur le site et determiner sa disponibilite */
	select	@vl_EtatPbMineur = mineure,
		@vl_EtatIndispo = (majeure | critique | HS | inhibe | desactive)
	from	EXP..EQT_DSP
	where	type = XDC_EQT_SAGA  and
		equipement = @va_NumEqt_in  and
		sit = @vl_Site  and
		dernier = 1

	if @@rowcount = 0
		return XDC_NOK
	if @vl_EtatIndispo <> 0
		return XDC_NOK
#if defined (DEV)
	if @vl_EtatPbMineur <> 0
		select @vl_HeureEchec = @vl_Maintenant
	else
		select @vl_HeureSucces = @vl_Maintenant
#endif


	/*A Rechercher le nom de la machine pilotant l'equipement */
	exec XZAO;05  @va_NumEqt_in, XDC_EQT_SAGA, @vl_NomMachine output
	if @vl_NomMachine = XDC_CHAINE_VIDE
		return XDC_NOK



	/*B Definir les parametres par defaut de la commande a l'equipement */

	select	@vl_Commande = 0

	select	@vl_Ligne1 = @va_Ligne1_TDP_in,
		@vl_Ligne2 = @va_Ligne2_TDP_in,
		@vl_Ligne3 = @va_Ligne3_TDP_in,
		@vl_Ligne4 = @va_Ligne4_TDP_in,
		@vl_Ligne5 = @va_Ligne5_TDP_in

	select	@vl_Alternat1 = @va_Alternat1_in,
		@vl_Alternat2 = @va_Alternat2_in,
		@vl_Alternat3 = @va_Alternat3_in,
		@vl_Alternat4 = @va_Alternat4_in,
		@vl_Alternat5 = @va_Alternat5_in

	select	@vl_Picto	= XDC_PICTO_PASPICTO,
		@vl_Bandeau	= XDC_CHAINE_VIDE

	select	@vl_Cligno_Non = XDC_NON

	select	@vl_Luminosite	= 0

	select	@vl_Barriere	= 0

	select	@vl_Scenario	= 0



	/*A
	** Determiner le type de neutre projete
	*/

	select @va_Type_Neutre_out = XDC_PMV_TDP

	/* Si aucun temps de parcours n'est fourni, preparer le neutre par defaut */
	if left(@vl_Ligne1, 3) + left(@vl_Ligne2, 3) + left(@vl_Ligne3, 3) + left(@vl_Ligne4, 3) + left(@vl_Ligne5, 3) = ""
	begin
		select @va_Type_Neutre_out = XDC_PMV_DEFAUT

		select @vl_Ligne_3Points = "            ..."
		if @vl_type_saga in ( XDC_SAGA_PMV_1_18, XDC_SAGA_PMV_2_18, XDC_SAGA_PMV_2_18_P )
			select @vl_Ligne_3Points = "               ..."

		if @vl_type_saga in ( XDC_SAGA_PMV_1_18 )
			select @vl_Ligne1 = @vl_Ligne_3Points
		else if @vl_type_saga in ( XDC_SAGA_PMV_2_15, XDC_SAGA_PMV_2_18, XDC_SAGA_PMV_2_18_P )
			select @vl_Ligne2 = @vl_Ligne_3Points
		else
			select @vl_Ligne3 = @vl_Ligne_3Points
	end


	/*B Surcharger par la mise au neutre programmee s'il y en a une */
	if @vl_type_saga = XDC_SAGA_PMVA
	begin
		select	@vl_NumFmc_Prog	= P.evenement,
			@vl_CleFmc_Prog	= P.cle,
			@vl_Debut_Prog	= P.debut,
			@vl_Ligne1_Prog		= ligne_1,
			@vl_Ligne2_Prog		= ligne_2,
			@vl_Ligne3_Prog		= ligne_3,
			@vl_Ligne4_Prog		= ligne_4,
			@vl_Ligne5_Prog		= ligne_5,
			@vl_Alternat1_Prog	= ligne_1_alternat,
			@vl_Alternat2_Prog	= ligne_2_alternat,
			@vl_Alternat3_Prog	= ligne_3_alternat,
			@vl_Alternat4_Prog	= ligne_4_alternat,
			@vl_Alternat5_Prog	= ligne_5_alternat,
			@vl_TypePicto_Prog	= XDC_PICTO_PASPICTO,
			@vl_TextePicto_Prog	= XDC_CHAINE_VIDE
		from	EXP..PRG_PMA_SAGA P, EXP..FMC_GEN F
		where	(P.numero = @va_NumEqt_in)  and  (@vl_Maintenant between P.debut and P.fin)
		and	(F.numero = P.evenement)  and  (F.cle = P.cle)
		and	(F.fin is null)
	end
	else
	begin
		select	@vl_NumFmc_Prog	= P.evenement,
			@vl_CleFmc_Prog	= P.cle,
			@vl_Debut_Prog	= P.debut,
			@vl_Ligne1_Prog		= ligne_1,
			@vl_Ligne2_Prog		= ligne_2,
			@vl_Ligne3_Prog		= ligne_3,
			@vl_Ligne4_Prog		= XDC_CHAINE_VIDE,
			@vl_Ligne5_Prog		= XDC_CHAINE_VIDE,
			@vl_Alternat1_Prog	= ligne_1_alternat,
			@vl_Alternat2_Prog	= ligne_2_alternat,
			@vl_Alternat3_Prog	= ligne_3_alternat,
			@vl_Alternat4_Prog	= XDC_CHAINE_VIDE,
			@vl_Alternat5_Prog	= XDC_CHAINE_VIDE,
			@vl_TypePicto_Prog	= pictogramme,
			@vl_TextePicto_Prog	= bandeau
		from	EXP..PRG_PMV_SAGA P, EXP..FMC_GEN F
		where	(P.numero = @va_NumEqt_in)  and  (@vl_Maintenant between P.debut and P.fin)
		and	(F.numero = P.evenement)  and  (F.cle = P.cle)
		and	(F.fin is null)
	end

	if @vl_NumFmc_Prog is not null
	begin
		select @va_Type_Neutre_out = XDC_PMV_TEXTE

		select	@vl_Ligne1 = @vl_Ligne1_Prog,
			@vl_Ligne2 = @vl_Ligne2_Prog,
			@vl_Ligne3 = @vl_Ligne3_Prog,
			@vl_Ligne4 = @vl_Ligne4_Prog,
			@vl_Ligne5 = @vl_Ligne5_Prog

		select	@vl_Alternat1 = @vl_Alternat1_Prog,
			@vl_Alternat2 = @vl_Alternat2_Prog,
			@vl_Alternat3 = @vl_Alternat3_Prog,
			@vl_Alternat4 = @vl_Alternat4_Prog,
			@vl_Alternat5 = @vl_Alternat5_Prog

		select	@vl_Picto	= @vl_TypePicto_Prog,
			@vl_Bandeau	= @vl_TextePicto_Prog
	end

	--print "Panneau %1!: Requis neutre %2!, FMC %3!", @va_NumEqt_in, @va_Type_Neutre_out, @vl_NumFmc_Prog



	begin tran
		/*A Rechercher la derniere action effectuee sur cet equipement */
		select	@vl_NumAction_Der = numero,
			@vl_NumFmc_Der = evenement,
			@vl_CleFmc_Der = cle,
			@vl_Priorite_Der = priorite,
			@vl_HeureLanc_Der = heure_lancement,
			@vl_HeureLancFin_Der = heure_lancement_fin
		from	EXP..ACT_GEN
		where	type = XDC_ACT_SAGA and
			equipement = @va_NumEqt_in and
			sit = @vl_Site
		having	numero = max (numero) and
			type = XDC_ACT_SAGA and
			equipement = @va_NumEqt_in and
			sit = @vl_Site

		/*A S'il n'y a aucune action sur ce panneau alors */
		/*A (si la fin de la derniere action est demandee alors ne pas en tenir compte) */
		if (@vl_NumAction_Der is null)  or  (@vl_HeureLancFin_Der is not null)
		begin
			select	@vl_NumAction_Der = 0,
				@vl_NumFmc_Der = null,
				@vl_CleFmc_Der = null

			/*! RESTRICTION POUR CAUSE D'AFFICHAGE PAR SCENARIO SANS ACTION : !*/
			/*! Ne mettre au neutre que si c'est demande explicitement, ou    !*/
			/*! si le panneau n'affiche aucune information (pas de texte).    !*/
			if @va_Fin_Action_in <> XDC_VRAI  and  @va_Texte_Affiche_in <> XDC_CHAINE_VIDE
				select	@va_NumeroAction_out = 0,
					@va_Type_Neutre_out = 0
		end
		else if (@vl_NumFmc_Der = @vl_NumFmc_Prog)  and  (@vl_CleFmc_Der = @vl_CleFmc_Prog)  and
		        (@vl_HeureLanc_Der >= @vl_Debut_Prog)
		begin
			/*A Sinon si le neutre programme est deja affiche alors ne rien faire */
			select	@va_NumeroAction_out = 0,
				@va_Type_Neutre_out = 0
		end
		else
		begin
			/*A Sinon rechercher la FMC dans la table de programmation */
			if @vl_type_saga = XDC_SAGA_PMVA
				select	@vl_Fin_ProgPrec = fin
				from	EXP..PRG_PMA_SAGA
				where	numero = @va_NumEqt_in  and
					(evenement = @vl_NumFmc_Der  and  cle = @vl_CleFmc_Der)  and
					debut <= @vl_HeureLanc_Der
				having	debut = max (debut)  and
					numero = @va_NumEqt_in  and
					(evenement = @vl_NumFmc_Der  and  cle = @vl_CleFmc_Der)
			else
				select	@vl_Fin_ProgPrec = fin
				from	EXP..PRG_PMV_SAGA
				where	numero = @va_NumEqt_in  and
					(evenement = @vl_NumFmc_Der  and  cle = @vl_CleFmc_Der)  and
					debut <= @vl_HeureLanc_Der
				having	debut = max (debut)  and
					numero = @va_NumEqt_in  and
					(evenement = @vl_NumFmc_Der  and  cle = @vl_CleFmc_Der)

			/*A Si la derniere action est une mise au neutre programmee alors */
			if @vl_Priorite_Der = XZACC_PRI_ACT_PRG
			begin
				/*A Si elle n'est plus active alors terminer l'action */
				if @vl_Maintenant >= @vl_Fin_ProgPrec
				   or (select fin from EXP..FMC_GEN where numero = @vl_NumFmc_Der and
				                                             cle = @vl_CleFmc_Der)  is not null
				   or @va_Fin_Action_in = XDC_VRAI
				begin
					update EXP..ACT_GEN set heure_lancement_fin = @vl_Maintenant,
					                        heure_fin = @vl_Maintenant
					where numero = @vl_NumAction_Der  and  sit = @vl_Site

					select	@va_NumeroAction_out = @vl_NumAction_Der

					/*! RESTRICTION POUR CAUSE D'AFFICHAGE PAR SCENARIO SANS ACTION :       !*/
					/*! Ne mettre au neutre que si l'indicateur en parametre le commande ou !*/
					/*! si le panneau affiche le texte de neutre programme ou autre neutre  !*/
					select	@vl_Texte_Action_Der = upper ( rtrim(ltrim(premiere_ligne)) +
					                                       rtrim(ltrim(deuxieme_ligne)) +
					                                       rtrim(ltrim(troisieme_ligne)) )
					from	EXP..ACT_SAGA
					where	actio = @vl_NumAction_Der  and  sit = @vl_Site

					if @va_Fin_Action_in = XDC_VRAI  or
					   (@va_Texte_Affiche_in = XDC_CHAINE_VIDE  or  rtrim(@va_Texte_Affiche_in) = @vl_Texte_Action_Der)
					begin
						select	@vl_Commande = - @vl_NumAction_Der
					end
					else
					begin
						select	@va_Type_Neutre_out = 0
					end
				end
				else
				begin
					/*A si la mise au neutre programm�e est encore active alors ne rien faire */
					select	@va_NumeroAction_out = @vl_NumAction_Der,
						@va_Type_Neutre_out = 0
				end
			end
			else
			begin
				/* sinon l'action est attachee a un veritable evenement : */
				/* la terminer ssi. l'indicateur en parametre le commande */
				if @va_Fin_Action_in = XDC_VRAI
				begin
					update EXP..ACT_GEN set heure_lancement_fin = @vl_Maintenant,
					                        heure_fin = @vl_Maintenant
					where numero = @vl_NumAction_Der  and  sit = @vl_Site

					select	@vl_Commande = - @vl_NumAction_Der
				end
				else
				begin
					select	@va_NumeroAction_out = @vl_NumAction_Der,
						@va_Type_Neutre_out = 0
				end
			end
		end


		/*A Si un neutre programme est a afficher alors creer une nouvelle actio */
		if @va_Type_Neutre_out = XDC_PMV_TEXTE
		begin
			select @va_NumeroAction_out = isnull(max(numero), 0) + 1 from EXP..ACT_GEN
			where sit = @vl_Site

			select	@vl_Commande = @va_NumeroAction_out

			/*A inserer l'actio PMV[A] SAGA */
			insert EXP..ACT_GEN ( numero, sit, evenement, cle, type, equipement,
			                      heure_lancement, heure_succes, heure_echec,
			                      priorite, prioritaire, distance_evt, operateur, sit_origine )
			values ( @va_NumeroAction_out, @vl_Site, @vl_NumFmc_Prog, @vl_CleFmc_Prog,
			         XDC_ACT_SAGA, @va_NumEqt_in,
			         @vl_Maintenant, @vl_HeureSucces, @vl_HeureEchec,
			         XZACC_PRI_ACT_PRG, XDC_NON, 0, @vl_PosteServeur, @va_Site_in )

			insert EXP..ACT_SAGA ( actio, sit, equipement,
						premiere_ligne, deuxieme_ligne, troisieme_ligne, quatrieme_ligne, cinquieme_ligne,
						premiere_ligne_alternat, deuxieme_ligne_alternat, troisieme_ligne_alternat,
						quatrieme_ligne_alternat, cinquieme_ligne_alternat,
						clignotement,
						picto, luminosite, bandeau,
						barriere, scenario )
			values ( @va_NumeroAction_out, @vl_Site, @va_NumEqt_in,
					@vl_Ligne1, @vl_Ligne2, @vl_Ligne3, @vl_Ligne4, @vl_Ligne5,
					@vl_Alternat1, @vl_Alternat2, @vl_Alternat3, @vl_Alternat4, @vl_Alternat5,
					@vl_Cligno_Non,
					@vl_Picto, @vl_Luminosite, @vl_Bandeau,
					@vl_Barriere, @vl_Scenario )
		end
	commit tran

	--print "Panneau SAGA %1!: mise au neutre %2!, action %3!", @va_NumEqt_in, @va_Type_Neutre_out, @va_NumeroAction_out



	/*A
	** Effectuer l'action determinee par la demande et les conditions
	*/

	if @va_Type_Neutre_out = 0
	begin
		return XDC_OK
	end


	select @vl_TypeLigne = XDC_PMV_TEXTE

#if ! defined (DEV)
	/*A affichage sur l'equipement (le plus tot possible) */
	if @vl_Site = XDC_VC
		exec @vl_Status = TASRV_VC...ACRP111 @va_NumEqt_in,
					@vl_Ligne1, @vl_TypeLigne, @vl_Alternat1,
					@vl_Ligne2, @vl_TypeLigne, @vl_Alternat2,
					@vl_Ligne3, @vl_TypeLigne, @vl_Alternat3,
					@vl_Ligne4, @vl_TypeLigne, @vl_Alternat4,
					@vl_Ligne5, @vl_TypeLigne, @vl_Alternat5,
					@vl_Cligno_Non, @vl_Luminosite, @vl_Picto, @vl_Bandeau,
					@vl_Barriere, @vl_Scenario,
					@vl_Commande, @vl_NomMachine
	else if @vl_Site = XDC_DP
		exec @vl_Status = TASRV_DP...ACRP111 @va_NumEqt_in,
					@vl_Ligne1, @vl_TypeLigne, @vl_Alternat1,
					@vl_Ligne2, @vl_TypeLigne, @vl_Alternat2,
					@vl_Ligne3, @vl_TypeLigne, @vl_Alternat3,
					@vl_Ligne4, @vl_TypeLigne, @vl_Alternat4,
					@vl_Ligne5, @vl_TypeLigne, @vl_Alternat5,
					@vl_Cligno_Non, @vl_Luminosite, @vl_Picto, @vl_Bandeau,
					@vl_Barriere, @vl_Scenario,
					@vl_Commande, @vl_NomMachine
	else if @vl_Site = XDC_CA
		exec @vl_Status = TASRV_CA...ACRP111 @va_NumEqt_in,
					@vl_Ligne1, @vl_TypeLigne, @vl_Alternat1,
					@vl_Ligne2, @vl_TypeLigne, @vl_Alternat2,
					@vl_Ligne3, @vl_TypeLigne, @vl_Alternat3,
					@vl_Ligne4, @vl_TypeLigne, @vl_Alternat4,
					@vl_Ligne5, @vl_TypeLigne, @vl_Alternat5,
					@vl_Cligno_Non, @vl_Luminosite, @vl_Picto, @vl_Bandeau,
					@vl_Barriere, @vl_Scenario,
					@vl_Commande, @vl_NomMachine

	if @vl_Status != XDC_OK
		return @vl_Status
#endif	/* pas en mode DEV */


#if defined (DEV)
	/*A Si l'equipement est en alarme mineure alors retourner un cr d'erreur */
	if @vl_EtatPbMineur <> 0
		return XDC_NOK
#endif

	return XDC_OK
go
