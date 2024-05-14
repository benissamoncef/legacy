/*E*/
/* Fichier : $Id: xzac61sp.prc,v 1.24 2021/03/08 14:26:55 pc2dpdy Exp $        Release : $Revision: 1.24 $        Date : $Date: 2021/03/08 14:26:55 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac61sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	24/04/95	: Creation a partir de xzac50
* F.V.  22/09/1995	: Ajout d'un argument Type neutre qui est transmis a equext (neutre, temperature, heure)
* F.V.  22/09/1995	: Ajout d'un argument Type neutre qui est transmis a equext (neutre, temperature, heure)
* C.T.	03/09/95	: ajout test pour DC et DS (V1.4)
* P.V.  03/09/96	: MAJ ACRP01 (V1.5)
* P.V.  12/09/96	: MAJ ACRP01 (V1.6)
* C.T.  09/09/96	: Pour PC simplifie envoi au sql server du pc simplifie (RADT V1.7)
* P.N.  28/02/1997	: Ajout destination et depass de seuilet maj de depass_seuil a xdc_faux	(dem/1398) V1.8
* C.P.	07/01/98	: Correction gestion multi-sites dans 
*			  les procs (1.9)
* JMG	07/01/98        : Re Correction gestion multi-sites dans les procs (1.10) (dem/1536)
* LCL   09/07/07        : Suppression DY
* JPL	27/11/08	: Ajout du pilotage du secteur CA au CI (DEM 846) 1.12
* JMG           02/10/09        : secto DEM887 1.13
* JMG	08/02/12 : tdp etendu dem/1014 1.14
* PNI           19/04/16 : Correction des appels dans PRC des XZAC sur les serveurs sites v1.15 DEM1159
* JMG	06/03/17	: regionalisation code region  DEM1220 v1.16
* JPL	08/03/17	: Enregistrement des neutres 3points et Temps de Parcours en table dediee (DEM 1196)  1.17
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.17
* LCL	30/06/17	: Modif PRA
* JPL	01/09/17	: En mode DEV, pas d'envoi de commande a l'equipement et action mise en fin (DEM 1231)  1.19
* JMG	08/08/18 	: ligne passe a 18 car. suppression SAGA DEM1306 1.20
* LCL	20/11/19	: MOVIS DEM-SAE93 1.21
* JMG	14/12/19	: EQT_FIL DEM1347 1.22
* LCL	15/07/20	: MOVIS DEM-SAE93 correction select dans RES_DIS 1.23
* GRO	12/10/2020      : MOVIS DEM-SAE93 1.24
* CGR	05/01/21	: MOVIS ajout PRA DEM-SAE93 1.24
* GGY	28/04/23	: Augmentation du champ  va_TextePicto_in (XDC_LGR_TXT_PICTO) et va_TypePicto_in (XDC_LGR_NOM_PICTO) (DEM-473)
* LCL	27/09/23 : Adaptations CNA DEM-483	
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Terminer l'action d'affichage PMV+picto.
* 
* Sequence d'appel
* SP	XZAC61_Commande_Fin_PMV
* 
* Arguments en entree
* XDY_Eqt	va_NumeroPMV_in
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
* Inserer dans la table ACTION la date de fin lance de l'actio.
* ACRP61 :	Appeler XZEP01 pour arreter l'affichage sur l'equipement PMV
* 		Appeler XZEP02 pour arreter l'affichage du picto
-----------------------------------------------------*/

use PRC
go

create procedure XZAC;61
	@va_NumeroPMV_in		T_EQUIPEMENT = null,
	@va_Site_in			T_SITE = null,
	@va_HeureLancementFin_in	datetime = null,
	@va_TypeNeutre_in		tinyint = null,
	@va_NomSiteLocal_in		char(2)	= null,
	@va_distant_in 			int = XDC_NON		
as
	declare	@vl_PMVTexte tinyint, @vl_PMVPasFlash tinyint, @vl_Clignotement tinyint,
		@vl_Texte char(18), @vl_TextePicto char(XDC_LGR_TXT_PICTO), @vl_TypePicto char(XDC_LGR_NOM_PICTO),
		@vl_NumeroAction int, @vl_Status int, @vl_NomMachine char(10),
		@vl_TypeEqtBoucle tinyint, @vl_NumEqtBoucle T_EQUIPEMENT, 
		@vl_ClignotementPicto tinyint,
		@vl_NumMaitre T_EQUIPEMENT, @vl_TypeMaitre tinyint,
		@vl_MachineNonTrouvee bit, @vl_Commande int,
	        @vl_Dest smallint, @vl_DestAlternat smallint, @vl_DepSeuil tinyint, @vl_DestNoeud smallint,
		@vl_Site tinyint,
		@vl_numero_evt int, @vl_cle_evt tinyint,
		@vl_fin datetime,
		@vl_numero_plan smallint,
		@vl_premiere_ligne char(18),
        @vl_deuxieme_ligne char(18),
        @vl_troisieme_ligne char(18),
        @vl_quatrieme_ligne char(18),
        @vl_cinquieme_ligne char(18),
        @vl_premier_alternat char(18),
        @vl_deuxieme_alternat char(18),
        @vl_troisieme_alternat char(18),
        @vl_quatrieme_alternat char(18),
        @vl_cinquieme_alternat char(18),
        @vl_picto char(XDC_LGR_NOM_PICTO),
        @vl_flash tinyint,
        @vl_libelle T_NOM,
        @vl_remarque char(50),
        @vl_explication char(250),
        @vl_destination smallint,
        @vl_destination_alternat smallint,
        @vl_destination_noeud smallint,
        @vl_clignotement int, @vl_action int,
	@vl_ordre smallint,
	@vl_priorite smallint,
	@vl_bandeau char(10),
	@vl_operateur smallint,
	@vl_serveur_pra char(6)


	/*! transfo en param�tres nulls bug applix */
	if @va_NumeroPMV_in = 0 select @va_NumeroPMV_in = null
	if @va_Site_in = 0 select @va_Site_in = null
	if @va_HeureLancementFin_in = XDC_DATE_NULLE select @va_HeureLancementFin_in = null

	/*A controle des parametres obligatoires */
	if  @va_NumeroPMV_in = null or @va_Site_in = null or 
	    @va_HeureLancementFin_in = null
		return XDC_ARG_INV

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
	select @vl_serveur_pra=serveur_pra from CFG..RES_PRA where numero=@va_Site_in
		
	if @vl_Site = XDC_CI and @va_distant_in = XDC_NON		
	/* Traitement au district d'exploitation pour Escota, en local sinon */
	begin
		if @vl_serveur_pra = XDC_SQLVC
			exec @vl_Status = SQL_DM.PRC..XZAC;61 @va_NumeroPMV_in, @va_Site_in, @va_HeureLancementFin_in, @va_TypeNeutre_in, @va_NomSiteLocal_in, XDC_OUI 
		else if @vl_serveur_pra = XDC_SQLDP
			exec @vl_Status = SQL_DS.PRC..XZAC;61 @va_NumeroPMV_in, @va_Site_in, @va_HeureLancementFin_in, @va_TypeNeutre_in, @va_NomSiteLocal_in, XDC_OUI 
		else if @vl_serveur_pra = XDC_SQLCI
                        exec @vl_Status = SQL_CI.PRC..XZAC;61 @va_NumeroPMV_in, @va_Site_in, @va_HeureLancementFin_in, @va_TypeNeutre_in, @va_NomSiteLocal_in, XDC_OUI
		else return XDC_NOK

		return @vl_Status
	end

	/*A Rechercher la derniere action en cours sur cet equipement */
	select @vl_NumeroAction = max(numero)
	from EXP..ACT_GEN
	where 	equipement = @va_NumeroPMV_in and
		sit = @va_Site_in and type = XDC_ACT_PMV 

	if @@rowcount = 0
		select @vl_NumeroAction = 0

	/*A initialisation des var locales pour la recherche */
	/*A du nom de la machine pilotant le PMV */
	select	@vl_MachineNonTrouvee = XDC_NON,
		@vl_NumEqtBoucle = @va_NumeroPMV_in,
		@vl_TypeEqtBoucle = XDC_EQT_PMV

	/*A recherche le nom de la machine pilotant le PMV */
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
	select	@vl_PMVTexte = isnull(@va_TypeNeutre_in,XDC_PMV_DEFAUT), 
		@vl_PMVPasFlash = XDC_PMV_PASFLASH,
		@vl_Clignotement = XDC_PMV_PASCLIGNO,
		@vl_ClignotementPicto = XDC_PICTO_PASCLIGNO,
		@vl_Texte = null,
		@vl_TextePicto = null,
		@vl_TypePicto = XDC_PICTO_PASPICTO,
		@vl_Dest = null,
		@vl_DestAlternat = null,
		@vl_DepSeuil = XDC_FAUX,
		@vl_DestNoeud = null

	/*A numero de commande a envoyer : - numero action */
	select @vl_Commande = - @vl_NumeroAction

/*#if ! defined (DEV)*/
	/*A arreter l'affichage sur PMV */
	/*! le numero picto est egal au numero pmv            */

	if @vl_Site = XDC_CI
		exec @vl_Status = TASRV_CI...ACRP01 @va_NumeroPMV_in, @vl_Texte, @vl_PMVTexte, 
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_PMVPasFlash, @va_NumeroPMV_in,
		@vl_TextePicto, @vl_TypePicto, @vl_ClignotementPicto, 
		@vl_Commande, @vl_NomMachine, @vl_Texte, @vl_Texte,@vl_Dest,@vl_DestAlternat,@vl_DepSeuil, @vl_DestNoeud

	else if @vl_Site = XDC_VC
		exec @vl_Status = TASRV_VC...ACRP01 @va_NumeroPMV_in, @vl_Texte, @vl_PMVTexte, 
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_PMVPasFlash, @va_NumeroPMV_in,
		@vl_TextePicto, @vl_TypePicto, @vl_ClignotementPicto, 
		@vl_Commande, @vl_NomMachine, @vl_Texte, @vl_Texte,@vl_Dest,@vl_DestAlternat,@vl_DepSeuil, @vl_DestNoeud

	else if @vl_Site = XDC_DP
		exec @vl_Status = TASRV_DP...ACRP01 @va_NumeroPMV_in, @vl_Texte, @vl_PMVTexte, 
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_PMVPasFlash, @va_NumeroPMV_in,
		@vl_TextePicto, @vl_TypePicto, @vl_ClignotementPicto, 
		@vl_Commande, @vl_NomMachine, @vl_Texte, @vl_Texte,@vl_Dest,@vl_DestAlternat,@vl_DepSeuil, @vl_DestNoeud

	else if @vl_Site = XDC_CA
		exec @vl_Status = TASRV_CA...ACRP01 @va_NumeroPMV_in, @vl_Texte, @vl_PMVTexte, 
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_PMVPasFlash, @va_NumeroPMV_in,
		@vl_TextePicto, @vl_TypePicto, @vl_ClignotementPicto, 
		@vl_Commande, @vl_NomMachine, @vl_Texte, @vl_Texte,@vl_Dest,@vl_DestAlternat,@vl_DepSeuil, @vl_DestNoeud

	if @vl_Status != XDC_OK
		return @vl_Status
/*#endif*/	/* pas en mode DEV */


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
					
			update EXP..ACT_PMV set depass_seuil = XDC_FAUX
			where actio=@vl_NumeroAction and sit = @va_Site_in
		end

#ifndef CI
		/* Mettre a jour la trace des mises au neutre */
		exec XZAC841  XDC_EQT_PMV, @va_NumeroPMV_in, @va_HeureLancementFin_in, @va_TypeNeutre_in
#endif

	commit tran


	/* recherche si FMC dans EQT_FIL*/
	/* pour cet equipement*/
	/* avec la priorite la plus basse donc la plus prioritaire*/
	select @vl_numero_evt=0
	set rowcount 1
                select @vl_numero_evt = f.evenement,
                        @vl_cle_evt = f.cle,
                        @vl_numero_plan = p.numero
                from EXP..EQT_FIL f, EXP..SAD_PLN p, EXP..SAD_PMV m
                where f.type_equipement = XDC_EQT_PMV
                and f.equipement = @va_NumeroPMV_in
                and f.evenement = p.evenement and f.cle = p.cle
                and p.numero = m.numero
                order by m.priorite , p.numero desc

	if @@rowcount =0
		return XDC_OK

	set rowcount 0

	/*on verifie evt est en cours*/
	select vl_fin = fin
	from EXP..FMC_GEN
	where numero = @vl_numero_evt
	and cle = @vl_cle_evt

	if  @@rowcount = 0
		return XDC_OK

	if @vl_fin is not null
	begin
		return XDC_OK
	end

	/*recupere la proposition PMV associee*/
	    select @vl_ordre = ordre, @vl_priorite = priorite,
        @vl_premiere_ligne = premiere_ligne,
        @vl_deuxieme_ligne = deuxieme_ligne,
        @vl_troisieme_ligne = troisieme_ligne,
        @vl_premier_alternat = premier_alternat,
        @vl_deuxieme_alternat = deuxieme_alternat,
        @vl_troisieme_alternat = troisieme_alternat,
        @vl_picto = picto, @vl_bandeau = bandeau,
        @vl_libelle = libelle,
        @vl_destination = destination,
        @vl_destination_alternat = destination_alternat,
        @vl_destination_noeud = destination_noeud,
        @vl_clignotement = clignotement,
        @vl_explication = explication
    from EXP..SAD_PMV
    where numero = @vl_numero_plan
    and equipement =@va_NumeroPMV_in
    and libelle <> "EXTINCTION"
		
	if @@rowcount = 0
		return XDC_OK

	if @vl_Site = XDC_VC
		select @vl_operateur =15
	else if @vl_Site = XDC_CI
		select @vl_operateur = 13
	else if @vl_Site = XDC_DP
		select @vl_operateur = 58
       if @vl_Clignotement = 0
          select @vl_Clignotement = XDC_PMV_PASCLIGNO

	exec PRC..XZAC;01
			@va_HeureLancementFin_in,
			@vl_numero_evt, @vl_cle_evt,
			@vl_operateur,
			@va_NumeroPMV_in, @va_Site_in,
			@vl_premiere_ligne, @vl_deuxieme_ligne, 
			@vl_troisieme_ligne, @vl_premier_alternat, @vl_deuxieme_alternat ,
			@vl_troisieme_alternat, @vl_clignotement, @vl_clignotement, 
			@vl_clignotement, XDC_PMV_PAS_FLASH,
			@vl_picto, @vl_libelle , @vl_clignotement, 
			 @vl_priorite, 
			XDC_DISTANCE_INCONNUE,
			"", "",
			@vl_destination, @vl_destination_alternat,
			0,
			@va_Site_in, @vl_action output,
			@vl_destination_noeud,
			@va_distant_in

	return XDC_OK

go
