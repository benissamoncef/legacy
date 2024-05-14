/*E*/
/* Fichier : xzac610sp.prc
-------------------------------------------------------------------------------------------
* ESCOTA * PROJET MIGRAZUR
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* MODULE TANEU * FICHIER xzac610sp.prc
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Mise au neutre d'un PMV.
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	19/02/13 : Creation (DEM 1061)  1.1
* JPL	27/02/13 : Pas de commande panneau si aucun TdP  1.2
* JPL	14/03/13 : Compte rendu 0 si aucune modification  1.3
* JPL	14/03/13 : Ajout de l'indicateur de terminaison d'action en cours  1.4
* JPL	14/03/13 : Prise en compte des commandes de mise au neutre programmees  1.5
* JPL	19/03/13 : Prise en compte des mise au neutre programmees avec Fmc non debutee  1.6
* JPL	22/03/13 : Identification d'une action programmee par sa priorite  1.7
* PNI	05/06/13 : Suppression des programmations pour FMC closes v1.8
* PNI	08/04/15 : Suppression des programmations pour FMC closes v1.9
* PNI   19/04/16 : Correction des appels dans PRC des XZAC610 sur les serveurs sites v1.10 DEM1159
* JMG	06/03/17 : regionalisation code region  DEM1220 v1.11
* JPL	08/03/17 : Enregistrement des neutres 3points et Temps de Parcours en table dediee (DEM 1196)  1.12
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.12
* LCL	30/06/17 : Modif PRA
* JPL	22/08/17 : Appel en cascade depuis le CIT avant verification de concordance des sites  1.14
* PNI	29/08/17 : Appel au CIT de SQL_DS et SQL_DM sur regions DP et VC  1.15
* JPL	22/08/17 : Prise en compte du mode programme (clignotement / alternat) (DEM 1230)  1.16
* JPL	05/09/17 : Mode DEV: pas de commande equipement; action en succes, ou echec si pb. mineur eqt. (DEM 1231)  1.17
* PNI	06/06/18 : ACT_GEN : distance a 500000 pour permettre le mode automatique de tdrol.x DEM1296 1.18
* JPL	17/12/18 : Prise en compte des mises au neutre discontinues (DEM 1312)  1.19
* JMG	29/12/19 : MOVIS DEM-SAE93 et EQT_FIL DEM1347 1.20
* JMG	08/06/20 : Correctif EQT_FIL DEM1347 1.21
* GRO	12/10/20 : MOVIS DEM-SAE93 1.22
* PNI	07/12/20 : Prise en compte de la date de succes en priorite 1.23
* PNI	18/12/20 : Recuperation de la machine serveur dans tous les cas 1.24
* ABE	06/01/21 : MOVIS ajout PRA DEM-SAE93 1.25 1.26
* PNI   04/02/21 : Correction clignotement picto en cas de liberation 1.27 SAE-66
* JPL	08/12/21 : Prise en compte des actions en cours de lancement; ignorer les actions en echec (SAE_355)  1.28
* JPL	09/12/21 : Prise en compte de la priorite des messages en file d'attente ; priorite des actions creees  1.29
* JPL	08/03/23 : Forcer le neutre selon le parametre s'il y a une action en lancement (SAE_466)  1.30
* JPL	03/04/23 : Horodates de succes et d'echec de l'action creee en mode DEV  1.31
* LCL	23/08/23 : Correction des declarations de variable de T_LIGNE_PMV en char(18) -> crash tasrv (SAE_500) 1.32
* GGY	28/04/23 : Augmentation taille va_TextePicto_in a char(XDC_LGR_TXT_PICTO) et vl_TypePicto_in (XDC_LGR_NOM_PICTO) (DEM-473)
* JPL	13/12/23 : Supprimer les Programmations sur FMC closes pour le PMV avant les traitements (SAE_560)  1.33
* LCL	27/09/23 : Adaptations CNA DEM-483	
-----------------------------------------------------------------------------------------*/


/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU
*
*  Consulte l'etat d'un PMV donne, notamment l'action en cours s'il y en a une.
*  Si le PMV n'est pas utilise pour un evenement, effectue sa mise au neutre.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL
*
* SP	XZAC610_Mise_Au_Neutre_PMV
*
* PARAMETRES EN ENTREE :
*
* XDY_Eqt	va_NumEqt_in			: Numero de l'equipement PMV
* XDY_Site	va_Site_in			: Numero du site
* XDY_Booleen	va_Fin_Action_in		: Vrai pour terminer l'action en cours (IHM), Faux si automatique
* XDY_Ligne_PMV	va_Ligne1_TDP_in		: Texte de message
* XDY_Ligne_PMV	va_Ligne2_TDP_in		:    de type "Temps de Parcours"
* XDY_Ligne_PMV	va_Ligne3_TDP_in		:    a afficher par defaut
* XDY_Mot	va_Destination_in		: Numeros des estinations
* XDY_Mot	va_DestinationAlternat_in	:   correspondantes
* XDY_Mot	va_DestinationNoeud_in		:
*
*
* PARAMETRES EN SORTIE :
*
* XDY_Entier	va_NumeroAction_out	: Action en cours, ou effectuee par la commande
* XDY_Octet	va_Type_Neutre_out	: Type de commande envoye a l'equipement
*
*   0			Aucune modification d'etat demandee
*   XDC_PMV_DEFAUT	Commande de passage au neutre par defaut
*   XDC_PMV_TEXTE	Commande d'affichage de neutre programme
*   XDC_PMV_TDP		Commande d'affichage de Temps de Parcours
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
*  Consulte l'etat du PMV indique, notamment l'action en cours s'il y en a une.
*  Si le PMV est utilise alors
*     terminer l'action en cours SSI. L'INDICATEUR FOURNI EN ENTREE L'AUTORISE,
*     sinon ne rien modifier.
*
*  Si plus aucune action attachee a un evenement n'est en cours alors
*     si un affichage programme existe a l'heure courante alors le commander
*     sinon si un affichage Temps de Parcours est specifie en parametres d'entree
*     alors le commander, sinon mettre le PMV au neutre par defaut.
*
-----------------------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAC610' and type = 'P')
	drop procedure XZAC610
go


create procedure 		XZAC610
	@va_NumEqt_in			T_EQUIPEMENT	= null,
	@va_Site_in			T_SITE		= null,
	@va_Fin_Action_in		tinyint		= XDC_NON,
	@va_Ligne1_TDP_in		char(18)	= null,
	@va_Ligne2_TDP_in		char(18)	= null,
	@va_Ligne3_TDP_in		char(18)	= null,
	@va_Dest_in			smallint	= null,
	@va_DestAlternat_in		smallint	= null,
	@va_DestNoeud_in		smallint	= null,
	@va_NumeroAction_out		int		= null output,
	@va_Type_Neutre_out		tinyint		= null output,
	@va_Attente_EQT_FIL		tinyint		= 0,
	@va_NomSiteLocal_in		char(2)		= null,
	@va_distant_in 			int 		= XDC_NON	
as
	declare	@vl_Site		T_SITE,
		@vl_NomMachine		T_NOM_MACHINE,
		@vl_MachineServeur	T_NOM_MACHINE,
		@vl_PosteServeur	T_EQUIPEMENT,

		/* Derniere action (la plus recente) */
		@vl_NumAction_Der	int,
		@vl_NumFmc_Der		int,
		@vl_CleFmc_Der		T_DISTRICT,
		@vl_Priorite_Der	tinyint,
		@vl_HeureLanc_Der	datetime,
		@vl_HeureSucces_Der	datetime,
		@vl_HeureEchec_Der	datetime,
		@vl_HeureLancFin_Der	datetime,
		@vl_HeureFin_Der	datetime,

		/* Derniere action en succes (etat actuel de l'equipement) */
		@vl_NumAction_Act	int,
		@vl_NumFmc_Act		int,
		@vl_CleFmc_Act		T_DISTRICT,
		@vl_Priorite_Act	tinyint,
		@vl_HeureLanc_Act	datetime,
		@vl_HeureLancFin_Act	datetime,
		@vl_HeureFin_Act	datetime,
		@vl_Texte_Act		varchar(200),
		@vl_Picto_Act		char(XDC_LGR_NOM_PICTO),
		@vl_TextePicto_Act	char(XDC_LGR_TXT_PICTO),

		/* Evenement en file d'attente */
		@vl_NumFmc_Fil		int,
		@vl_CleFmc_Fil		T_DISTRICT,
		@vl_Priorite_Fil	tinyint,
		@vl_Distance_Fil	int,

		/* FMC de rattachement de l'action creee */
		@vl_NumFmc		int,
		@vl_CleFmc		T_DISTRICT,
		@vl_Priorite		tinyint,
		@vl_Distance		int,

		@vl_Commande		int,
		@vl_Ligne1		char(18),
		@vl_Ligne2		char(18),
		@vl_Ligne3		char(18),
		@vl_Alternat1		char(18),
		@vl_Alternat2		char(18),
		@vl_Alternat3		char(18),
		@vl_TypeLigne		tinyint,
		@vl_Clignotement	tinyint,
		@vl_ClignotementPicto	tinyint,
		@vl_Flash_Non		tinyint,
		@vl_Picto		char(XDC_LGR_NOM_PICTO),
		@vl_TextePicto		char(XDC_LGR_TXT_PICTO),
		@vl_LigneDir		char(18),
		@vl_LigneDirAlter	char(18),
		@vl_DepassSeuil		tinyint,

		@vl_NumFmc_Prog		int,
		@vl_CleFmc_Prog		T_DISTRICT,
		@vl_Fin_ProgPrec	datetime,
		@vl_Texte_Prog		varchar(200),
		@vl_Ligne1_Prog		char(18),
		@vl_Ligne2_Prog		char(18),
		@vl_Ligne3_Prog		char(18),
		@vl_Alternat1_Prog	char(18),
		@vl_Alternat2_Prog	char(18),
		@vl_Alternat3_Prog	char(18),
		@vl_Picto_Prog		char(XDC_LGR_NOM_PICTO),
		@vl_TextePicto_Prog	char(XDC_LGR_TXT_PICTO),
		@vl_Mode_Prog		tinyint,

		@vl_Action_Evt_En_Cours		tinyint,
		@vl_Action_En_Lancement		tinyint,
		@vl_FMC_Fil_Non_Prioritaire	tinyint,

		@vl_Maintenant		datetime,
		@vl_HeureSucces		datetime,
		@vl_HeureEchec		datetime,
		@vl_EtatIndispo		int,
		@vl_EtatPbMineur	int,
		@vl_Status		int,
		@vl_fin datetime,
		@vl_numero_plan smallint, @vl_operateur smallint,
		@vl_serveur_pra char(6)

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
	end

	select	@vl_MachineServeur = machine
	from CFG..RES_DIS
	where numero = @va_Site_in

	if @vl_Site is null
		return XDC_NOK


	/* Au CI, executer la commande en cascade sur le secteur pilotant l'equipement */
	if @vl_Site = XDC_CI and @va_distant_in = XDC_NON		
	/* Traitement au district d'exploitation pour Escota, en local sinon */
	begin
		/* Init serveur PRA */
		select @vl_serveur_pra=serveur_pra from CFG..RES_PRA where numero=@va_Site_in

		if @vl_serveur_pra=XDC_SQLVC
			exec @vl_Status = SQL_DM.PRC..XZAC610 @va_NumEqt_in, @va_Site_in, @va_Fin_Action_in,
			                     @va_Ligne1_TDP_in, @va_Ligne2_TDP_in, @va_Ligne3_TDP_in,
			                     @va_Dest_in, @va_DestAlternat_in, @va_DestNoeud_in,
			                     @va_NumeroAction_out output, @va_Type_Neutre_out output, @va_Attente_EQT_FIL, @va_NomSiteLocal_in,  XDC_OUI 
		else if @vl_serveur_pra=XDC_SQLDP
			exec @vl_Status = SQL_DS.PRC..XZAC610 @va_NumEqt_in, @va_Site_in, @va_Fin_Action_in,
			                     @va_Ligne1_TDP_in, @va_Ligne2_TDP_in, @va_Ligne3_TDP_in,
			                     @va_Dest_in, @va_DestAlternat_in, @va_DestNoeud_in,
			                     @va_NumeroAction_out output, @va_Type_Neutre_out output, @va_Attente_EQT_FIL, @va_NomSiteLocal_in, XDC_OUI 

		else if @vl_serveur_pra=XDC_SQLCI
			exec @vl_Status = SQL_CI.PRC..XZAC610 @va_NumEqt_in, @va_Site_in, @va_Fin_Action_in,
			                     @va_Ligne1_TDP_in, @va_Ligne2_TDP_in, @va_Ligne3_TDP_in,
			                     @va_Dest_in, @va_DestAlternat_in, @va_DestNoeud_in,
			                     @va_NumeroAction_out output, @va_Type_Neutre_out output, @va_Attente_EQT_FIL, @va_NomSiteLocal_in, XDC_OUI 
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

	/* controle des parametres d'entree obligatoires */
	if @va_NumEqt_in = null
		return XDC_ARG_INV


	/* Supprimer les Programmations sur FMC closes */
	delete EXP..PRG_PMV from EXP..PRG_PMV P, EXP..FMC_GEN F
	where  P.numero = @va_NumEqt_in
	  and  F.numero=P.evenement and F.cle=P.cle and F.cloture is not null


	/* Resultats par defaut */
	select	@va_NumeroAction_out = 0,
		@va_Type_Neutre_out = 0

	select	@vl_Maintenant = getdate ()
	select	@vl_HeureSucces = null, @vl_HeureEchec = null


	select	@vl_PosteServeur = numero
	from	CFG..EQT_GEN
	where	type = XDC_EQT_MAC  and  nom = @vl_MachineServeur


	/*A Verifier l'existence de l'equipement sur le secteur et determiner sa disponibilite */
	select	@vl_EtatPbMineur = mineure,
		@vl_EtatIndispo = (majeure | critique | HS | inhibe | desactive)
	from	EXP..EQT_DSP
	where	type = XDC_EQT_PMV  and  equipement = @va_NumEqt_in  and
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


	/*A Rechercher le nom de la machine pilotant le PMV */
	begin
		declare	@vl_NumMaitre	T_EQUIPEMENT, @vl_TypeMaitre	tinyint,
			@vl_NumEqt	T_EQUIPEMENT, @vl_TypeEqt	tinyint

		select	@vl_NumEqt = @va_NumEqt_in,
			@vl_TypeEqt = XDC_EQT_PMV

		while @vl_TypeEqt <> XDC_EQT_MAC
		begin
			select	@vl_NumMaitre = maitre,
				@vl_TypeMaitre = type_maitre
			from	CFG..EQT_GEN
			where	numero = @vl_NumEqt and type = @vl_TypeEqt

			if @vl_NumMaitre is null  or  @@rowcount != 1
				return XDC_NOK

			select	@vl_NumEqt = @vl_NumMaitre,
				@vl_TypeEqt = @vl_TypeMaitre
		end

		select @vl_NomMachine = nom
		from CFG..EQT_GEN
		where numero = @vl_NumEqt  and  type = XDC_EQT_MAC
	end



	/*B Definir les parametres par defaut de la commande a l'equipement */

	select	@vl_Commande = 0

	select	@vl_Ligne1 = @va_Ligne1_TDP_in,
		@vl_Ligne2 = @va_Ligne2_TDP_in,
		@vl_Ligne3 = @va_Ligne3_TDP_in

	select	@vl_Alternat1 = XDC_CHAINE_VIDE,
		@vl_Alternat2 = XDC_CHAINE_VIDE,
		@vl_Alternat3 = XDC_CHAINE_VIDE

	select	@vl_Picto = XDC_PICTO_PASPICTO,
		@vl_TextePicto = XDC_CHAINE_VIDE

	select	@vl_Clignotement = XDC_PMV_PASCLIGNO,
		@vl_ClignotementPicto = XDC_PICTO_PASCLIGNO,
		@vl_Flash_Non = XDC_PMV_PASFLASH

	select	@vl_LigneDir = XDC_CHAINE_VIDE,
		@vl_LigneDirAlter = XDC_CHAINE_VIDE

	select	@vl_DepassSeuil = XDC_NON



	/*A
	** Determiner le type de neutre projete selon la demande
	*/

	/*B Type de neutre commande par parametres */
	if @va_Ligne1_TDP_in = XDC_CHAINE_VIDE	select @va_Dest_in = XDC_DTP_AUCUNE
	if @va_Ligne2_TDP_in = XDC_CHAINE_VIDE	select @va_DestAlternat_in = XDC_DTP_AUCUNE
	if @va_Ligne3_TDP_in = XDC_CHAINE_VIDE	select @va_DestNoeud_in = XDC_DTP_AUCUNE

	if ( (@va_Dest_in = XDC_DTP_AUCUNE) and
	     (@va_DestAlternat_in = XDC_DTP_AUCUNE) and
	     (@va_DestNoeud_in = XDC_DTP_AUCUNE) )
		select @va_Type_Neutre_out = XDC_PMV_DEFAUT
	else
		select @va_Type_Neutre_out = XDC_PMV_TDP


	/*B Surcharger par la mise au neutre programmee s'il y en a une */
	select @vl_Texte_Prog = null
	execute XZAT63	@va_NumEqt_in,
			@vl_NumFmc_Prog		output,
			@vl_CleFmc_Prog		output,
			@vl_Ligne1_Prog		output,
			@vl_Ligne2_Prog		output,
			@vl_Ligne3_Prog		output,
			@vl_Alternat1_Prog	output,
			@vl_Alternat2_Prog	output,
			@vl_Alternat3_Prog	output,
			@vl_Picto_Prog		output,
			@vl_TextePicto_Prog	output,
			@vl_Mode_Prog		output

	if @vl_NumFmc_Prog is not null
	begin
		select @vl_Texte_Prog = rtrim (ltrim (@vl_Ligne1_Prog)) + rtrim (ltrim (@vl_Ligne2_Prog))
		                           + rtrim (ltrim (@vl_Ligne3_Prog))
		                           + rtrim (ltrim (@vl_Alternat1_Prog)) + rtrim (ltrim (@vl_Alternat2_Prog))
		                           + rtrim (ltrim (@vl_Alternat3_Prog))

		select @va_Type_Neutre_out = XDC_PMV_TEXTE

		select	@vl_NumFmc = @vl_NumFmc_Prog,
			@vl_CleFmc = @vl_CleFmc_Prog,
			@vl_Priorite = XZACC_PRI_ACT_PRG,
			@vl_Distance = 500000

		select	@va_Dest_in = XDC_DTP_AUCUNE,
			@va_DestAlternat_in = XDC_DTP_AUCUNE,
			@va_DestNoeud_in = XDC_DTP_AUCUNE

		select	@vl_Ligne1 = @vl_Ligne1_Prog,
			@vl_Ligne2 = @vl_Ligne2_Prog,
			@vl_Ligne3 = @vl_Ligne3_Prog

		select	@vl_Picto	= @vl_Picto_Prog,
			@vl_TextePicto	= @vl_TextePicto_Prog

		if @vl_Mode_Prog = XDC_PMV_MODE_CLIGNO
		begin
			select	@vl_Clignotement = XDC_PMV_CLIGNORAPIDE
			--select	@vl_ClignotementPicto = XDC_PICTO_CLIGNO
		end
		else if @vl_Mode_Prog = XDC_PMV_MODE_ALTERNE
		begin
			select	@vl_Alternat1 = @vl_Alternat1_Prog,
				@vl_Alternat2 = @vl_Alternat2_Prog,
				@vl_Alternat3 = @vl_Alternat3_Prog
		end
	end

#if defined (DEV)
	print "PMV %1!: Requis neutre %2!, texte %3!", @va_NumEqt_in, @va_Type_Neutre_out, @vl_Texte_Prog
#endif


	begin tran
		/* Rechercher la derniere action effectuee sur cet equipement */
		select	@vl_NumAction_Der = numero,
			@vl_NumFmc_Der = evenement,
			@vl_CleFmc_Der = cle,
			@vl_Priorite_Der = priorite,
			@vl_HeureLanc_Der = heure_lancement,
			@vl_HeureSucces_Der = heure_succes,
			@vl_HeureEchec_Der = heure_echec,
			@vl_HeureLancFin_Der = heure_lancement_fin,
			@vl_HeureFin_Der = heure_fin
		from	EXP..ACT_GEN
		where	type = XDC_ACT_PMV  and  equipement = @va_NumEqt_in and
			sit = @vl_Site
		having	heure_lancement = max (heure_lancement) and
			type = XDC_ACT_PMV  and  equipement = @va_NumEqt_in and
			sit = @vl_Site

		/* Determiner si la derniere action est en cours DE LANCEMENT */
		if (@vl_NumAction_Der is not null  and
		       @vl_HeureSucces_Der is null  and  @vl_HeureEchec_Der is null  and
		       @vl_HeureLancFin_Der is null  and  @vl_HeureFin_Der is null)
		begin
			select	@vl_Action_En_Lancement = XDC_VRAI
		end

		/* Rechercher la derniere action EN SUCCES, indiquant l'etat de equipement */
		if @vl_HeureSucces_Der is not null
		begin
			select	@vl_NumAction_Act = @vl_NumAction_Der,
				@vl_NumFmc_Act = @vl_NumFmc_Der,
				@vl_CleFmc_Act = @vl_CleFmc_Der,
				@vl_Priorite_Act = @vl_Priorite_Der,
				@vl_HeureLanc_Act = @vl_HeureLanc_Der,
				@vl_HeureLancFin_Act = @vl_HeureLancFin_Der,
				@vl_HeureFin_Act = @vl_HeureFin_Der
		end
		else
		begin
			select	@vl_NumAction_Act = numero,
				@vl_NumFmc_Act = evenement,
				@vl_CleFmc_Act = cle,
				@vl_Priorite_Act = priorite,
				@vl_HeureLanc_Act = heure_lancement,
				@vl_HeureLancFin_Act = heure_lancement_fin,
				@vl_HeureFin_Act = heure_fin
			from	EXP..ACT_GEN
			where	type = XDC_ACT_PMV  and  equipement = @va_NumEqt_in and
				sit = @vl_Site
			having	heure_succes = max (heure_succes) and
				type = XDC_ACT_PMV  and  equipement = @va_NumEqt_in and
				sit = @vl_Site
		end

		/* S'il y a une action en cours alors determiner le texte affiche */
		if (@vl_NumAction_Act is not null)  and  (@vl_HeureLancFin_Act is null  and  @vl_HeureFin_Act is null)
		begin
			select	@vl_Texte_Act = rtrim (ltrim (premiere_ligne)) + rtrim (ltrim (deuxieme_ligne))
			                           + rtrim (ltrim (troisieme_ligne))
			                           + rtrim (ltrim (premiere_ligne_alternat)) + rtrim (ltrim (deuxieme_ligne_alternat))
			                           + rtrim (ltrim (troisieme_ligne_alternat)),
				@vl_Picto_Act = pictogramme,
				@vl_TextePicto_Act = bandeau
			  from	EXP..ACT_PMV
			 where	actio = @vl_NumAction_Act  and  sit = @vl_Site

			/* puis rechercher la FMC dans la table de programmation */
			select	@vl_Fin_ProgPrec = fin
			from	EXP..PRG_PMV
			where	numero = @va_NumEqt_in  and
				(evenement = @vl_NumFmc_Act  and  cle = @vl_CleFmc_Act)  and
				debut <= @vl_HeureLanc_Act
			having	debut = max (debut)  and
				numero = @va_NumEqt_in  and
				(evenement = @vl_NumFmc_Act  and  cle = @vl_CleFmc_Act)

			/* et noter si l'action est attachee a un veritable evenement */
			if not (@vl_Priorite_Act = XZACC_PRI_ACT_PRG  and  @vl_Fin_ProgPrec is not null)
			begin
				select @vl_Action_Evt_En_Cours = XDC_VRAI
			end
		end


		/* S'il y a une action en cours DE LANCEMENT alors pas de neutre automatique */
		if @vl_Action_En_Lancement = XDC_VRAI  and  @va_Fin_Action_in <> XDC_VRAI
		begin
			select	@va_NumeroAction_out = @vl_NumAction_Der,
				@va_Type_Neutre_out = 0
		end

		/* S'il y a une action en cours attachee a un evenement alors pas de neutre automatique */
		if @vl_Action_Evt_En_Cours = XDC_VRAI  and  @va_Fin_Action_in <> XDC_VRAI
		begin
			select	@va_NumeroAction_out = @vl_NumAction_Act,
				@va_Type_Neutre_out = 0
		end


		if @va_Type_Neutre_out = 0
		begin
			commit tran
			return XDC_OK
		end


		if @va_Attente_EQT_FIL = 0
		begin
			/* Surcharger par l'action liee a une FMC en file d'attente la plus prioritaire */
			/* pour cet equipement s'il y en a une (celle ayant la priorite la plus basse) */
			select @vl_NumFmc_Fil=0
			select @vl_FMC_Fil_Non_Prioritaire = XDC_FAUX
			set rowcount 1
			select @vl_NumFmc_Fil = f.evenement,
				@vl_CleFmc_Fil = f.cle,
				@vl_numero_plan = p.numero,
				@vl_Priorite_Fil = m.priorite,
				@vl_Distance_Fil = f.distance
			from EXP..EQT_FIL f, EXP..SAD_PLN p, EXP..SAD_PMV m
			where f.type_equipement = XDC_EQT_PMV
			and f.equipement = @va_NumEqt_in
			and f.evenement = p.evenement and f.cle = p.cle
			and p.numero = m.numero
			and p.numero in (select numero from EXP..SAD_PLN
				where evenement = p.evenement and cle =p.cle having numero = max(numero))
			order by m.priorite , p.numero desc
			if @@rowcount =0
			begin
				set rowcount 0
			end
			else if (@va_Type_Neutre_out = 0  and  @va_Fin_Action_in <> XDC_VRAI  and
			         ( (@vl_Action_En_Lancement = XDC_VRAI  and  @vl_Priorite_Der >= @vl_Priorite_Fil)  or
			           (@vl_Action_Evt_En_Cours = XDC_VRAI  and  @vl_Priorite_Act >= @vl_Priorite_Fil) )
			        )
			begin
				/* Si une action plus prioritaire est lancée ou en cours, il n'est pas decide */
				/* de la remplacer et le mode ne le permet pas alors indiquer cette priorite */
				select @vl_FMC_Fil_Non_Prioritaire = XDC_VRAI
			end
			else
			begin
				select @vl_FMC_Fil_Non_Prioritaire = XDC_FAUX
				set rowcount 0
				select @vl_fin = fin
				from EXP..FMC_GEN
				where numero = @vl_NumFmc_Fil
				and cle = @vl_CleFmc_Fil
				if @@rowcount = 0
				begin
					select null
				end
				else if @vl_fin is not null
				begin
					select null
				end
				else
				begin
					/*recupere la proposition PMV associee*/
					select @vl_Ligne1=premiere_ligne, @vl_Ligne2=deuxieme_ligne,
						@vl_Ligne3=troisieme_ligne, @vl_Alternat1=premier_alternat,
						@vl_Alternat2=deuxieme_alternat,
						@vl_Alternat3=troisieme_alternat, @vl_Picto=picto,
						@vl_TextePicto=bandeau,
						@vl_Clignotement=clignotement,
						@vl_ClignotementPicto=XDC_PMV_PASCLIGNO,
						@vl_LigneDir=XDC_CHAINE_VIDE,  
						@vl_LigneDirAlter = XDC_CHAINE_VIDE,
						@va_Dest_in = destination,
						@va_DestAlternat_in = destination_alternat,
						@va_DestNoeud_in = destination_noeud
						from EXP..SAD_PMV
						where numero = @vl_numero_plan
						and  equipement = @va_NumEqt_in
						and libelle <> "EXTINCTION"
					if @@rowcount <> 0
					begin
						if @vl_Clignotement = 0
							select @vl_Clignotement = XDC_PMV_PASCLIGNO
						if @vl_ClignotementPicto =0
							 select @vl_ClignotementPicto = XDC_PMV_PASCLIGNO
						select @vl_NumFmc = @vl_NumFmc_Fil,
							@vl_CleFmc = @vl_CleFmc_Fil,
							@vl_Priorite = @vl_Priorite_Fil,
							@vl_Distance = @vl_Distance_Fil,
							@va_Type_Neutre_out = XDC_PMV_TEXTE,
							@vl_LigneDir = XDC_CHAINE_VIDE,
							@vl_LigneDirAlter=XDC_CHAINE_VIDE
					end
				end

				if @vl_FMC_Fil_Non_Prioritaire = XDC_FAUX
				begin
					delete EXP..EQT_FIL
					where evenement = @vl_NumFmc_Fil
					and cle = @vl_CleFmc_Fil
					and type_equipement = XDC_EQT_PMV
					and equipement = @va_NumEqt_in
				end
			end
		end


		/* Si le neutre decide est un neutre programme deja affiche alors ne rien faire */
		if (@vl_Priorite_Act = XZACC_PRI_ACT_PRG  and  @vl_Fin_ProgPrec is not null)
		   and (@vl_Priorite = XZACC_PRI_ACT_PRG  and  @vl_NumFmc_Prog is not null)
		   and (upper (@vl_Texte_Act) = upper (@vl_Texte_Prog)  and  @vl_Picto_Act = @vl_Picto_Prog)
		begin
			select	@va_NumeroAction_out = @vl_NumAction_Act,
				@va_Type_Neutre_out = 0
		end



		/* Si un neutre est commande alors lancer la fin des actions en cours */
		if @va_Type_Neutre_out <> 0
		begin
			if @vl_NumAction_Act is not null  and  @vl_HeureFin_Act is null
			begin
				update EXP..ACT_GEN set heure_lancement_fin = isnull (heure_lancement_fin, @vl_Maintenant)
#if defined (DEV)
							, heure_fin = @vl_Maintenant
#endif
				where numero = @vl_NumAction_Act  and  sit = @vl_Site
				update EXP..ACT_PMV set depass_seuil = XDC_FAUX
				where actio = @vl_NumAction_Act  and  sit = @vl_Site
			end

			if ( @vl_NumAction_Act is null  or  @vl_NumAction_Act <> @vl_NumAction_Der )
			   and @vl_HeureFin_Der is null
			begin
				update EXP..ACT_GEN set heure_lancement_fin = isnull (heure_lancement_fin, @vl_Maintenant)
#if defined (DEV)
				                        , heure_fin = @vl_Maintenant
#endif
				where numero = @vl_NumAction_Der  and  sit = @vl_Site
				update EXP..ACT_PMV set depass_seuil = XDC_FAUX
				where actio = @vl_NumAction_Der  and  sit = @vl_Site
			end
		end



		/* Si un message en attente ou un neutre programme est a afficher alors creer une nouvelle actio */
		if @va_Type_Neutre_out = XDC_PMV_TEXTE
		begin
			declare	@vl_MemoCligno		bit,
				@vl_MemoClignoPicto	bit
			select	@vl_MemoCligno = XDC_PMV_PASCLIGNO,
				@vl_MemoClignoPicto = XDC_PMV_PASCLIGNO
			if @vl_Clignotement > 0
				select @vl_MemoCligno = @vl_Clignotement
			if @vl_ClignotementPicto > 0
				select @vl_MemoClignoPicto = @vl_ClignotementPicto

			select @va_NumeroAction_out = isnull(max(numero), 0) + 1 from EXP..ACT_GEN
			where sit = @vl_Site

			select	@vl_Commande = @va_NumeroAction_out

			/*A inserer l'actio PMV */
			insert EXP..ACT_GEN ( numero, sit, evenement, cle, type, equipement,
			                      heure_lancement, heure_succes, heure_echec,
			                      priorite, prioritaire, distance_evt, operateur, sit_origine )
			values ( @va_NumeroAction_out, @vl_Site, @vl_NumFmc, @vl_CleFmc,
			         XDC_ACT_PMV, @va_NumEqt_in,
			         @vl_Maintenant, @vl_HeureSucces, @vl_HeureEchec,
			         @vl_Priorite, XDC_NON, @vl_Distance, @vl_PosteServeur, @va_Site_in )
			select @vl_Maintenant, @vl_HeureSucces, @vl_HeureEchec
			insert EXP..ACT_PMV ( actio, PMV, sit,
						premiere_ligne, deuxieme_ligne, troisieme_ligne,
						premiere_ligne_alternat, deuxieme_ligne_alternat, troisieme_ligne_alternat,
						pictogramme, bandeau,
						clignotement1, clignotement2, clignotement3, clignotement_picto,
						direction_ligne, direction_ligne_alternat,
						destination, destination_alternat, depass_seuil, destination_noeud )
			values ( @va_NumeroAction_out, @va_NumEqt_in, @vl_Site,
				 @vl_Ligne1, @vl_Ligne2, @vl_Ligne3,
                                        @vl_Alternat1, @vl_Alternat2, @vl_Alternat3,
                                        @vl_Picto, @vl_TextePicto,
                                        @vl_MemoCligno, @vl_MemoCligno, @vl_MemoCligno, @vl_MemoClignoPicto,
                                        @vl_LigneDir, @vl_LigneDirAlter,
                                        @va_Dest_in, @va_DestAlternat_in, @vl_DepassSeuil, @va_DestNoeud_in )
		end



#ifndef CI
		/* Mettre a jour la trace des mises au neutre */
		if @va_Type_Neutre_out <> 0
			exec  XZAC841  XDC_EQT_PMV, @va_NumEqt_in, @vl_Maintenant, @va_Type_Neutre_out
#endif


#if defined (DEV)
	print "PMV %1!: effectif neutre %2!, action %3!", @va_NumEqt_in, @va_Type_Neutre_out, @va_NumeroAction_out
#endif


	if @va_Type_Neutre_out = 0
	begin
		commit tran
		return XDC_OK
	end


	/*A
	** Effectuer l'action determinee par la demande et les conditions
	*/

	if (@vl_Commande = 0)  and  (@vl_NumAction_Der <> 0)
		select	@vl_Commande = - @vl_NumAction_Der

	select	@vl_TypeLigne = XDC_PMV_TEXTE
	if @va_Type_Neutre_out = XDC_PMV_DEFAUT
	begin
		select	@vl_TypeLigne = XDC_PMV_DEFAUT
	end
	else if @va_Type_Neutre_out = XDC_PMV_HEURE
	begin
		select	@vl_TypeLigne = XDC_PMV_HEURE
	end

/*#if ! defined (DEV)*/
	/* Envoyer la commande a l'equipement */
	/*! le numero picto est egal au numero pmv */
	if @vl_Site = XDC_VC
		exec @vl_Status = TASRV_VC...ACRP01 @va_NumEqt_in,
					@vl_Ligne1, @vl_TypeLigne, @vl_Clignotement, @vl_Alternat1,
					@vl_Ligne2, @vl_TypeLigne, @vl_Clignotement, @vl_Alternat2,
					@vl_Ligne3, @vl_TypeLigne, @vl_Clignotement, @vl_Alternat3, @vl_Flash_Non,
					@va_NumEqt_in, @vl_TextePicto, @vl_Picto, @vl_ClignotementPicto,
					@vl_Commande, @vl_NomMachine,
					@vl_LigneDir, @vl_LigneDirAlter,
					@va_Dest_in, @va_DestAlternat_in, @vl_DepassSeuil, @va_DestNoeud_in
	else if @vl_Site = XDC_DP
		exec @vl_Status = TASRV_DP...ACRP01 @va_NumEqt_in,
					@vl_Ligne1, @vl_TypeLigne, @vl_Clignotement, @vl_Alternat1,
					@vl_Ligne2, @vl_TypeLigne, @vl_Clignotement, @vl_Alternat2,
					@vl_Ligne3, @vl_TypeLigne, @vl_Clignotement, @vl_Alternat3, @vl_Flash_Non,
					@va_NumEqt_in, @vl_TextePicto, @vl_Picto, @vl_ClignotementPicto,
					@vl_Commande, @vl_NomMachine,
					@vl_LigneDir, @vl_LigneDirAlter,
					@va_Dest_in, @va_DestAlternat_in, @vl_DepassSeuil, @va_DestNoeud_in
	else if @vl_Site = XDC_CA
		exec @vl_Status = TASRV_CA...ACRP01 @va_NumEqt_in,
					@vl_Ligne1, @vl_TypeLigne, @vl_Clignotement, @vl_Alternat1,
					@vl_Ligne2, @vl_TypeLigne, @vl_Clignotement, @vl_Alternat2,
					@vl_Ligne3, @vl_TypeLigne, @vl_Clignotement, @vl_Alternat3, @vl_Flash_Non,
					@va_NumEqt_in, @vl_TextePicto, @vl_Picto, @vl_ClignotementPicto,
					@vl_Commande, @vl_NomMachine,
					@vl_LigneDir, @vl_LigneDirAlter,
					@va_Dest_in, @va_DestAlternat_in, @vl_DepassSeuil, @va_DestNoeud_in
	if @vl_Status != XDC_OK
	begin
		commit tran
		return @vl_Status
	end
/*#endif*/	/* pas en mode DEV */

	commit tran


#if defined (DEV)
	/* Si l'equipement est en alarme mineure alors retourner un cr d'erreur */
	if @vl_EtatPbMineur <> 0
		return XDC_NOK
#endif

	return XDC_OK
go
