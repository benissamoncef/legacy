/*E*/
/*  Fichier : $Id: xzac50sp.prc,v 1.18 2020/10/13 17:21:17 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2020/10/13 17:21:17 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac50sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	21/09/94	: Creation
* C.T.	22/09/94	: Modif numero de commande 
*                         pour ACRP01 est negatif
* C.T.	23/09/94	: Modification du traitement de
*                         recherche du nom de la machine
*                         pilotant (V 1.3)
* C.T.	10/11/94	: Modif bugs applix (V 1.4)
* C.T.	05/12/94	: Correction erreur nom maitre V 1.5
*                         non teste
* B.G.	30/01/95	: Modification recherche dispo (1.6)
* B.G.  12/04/95        : Modification recherche dispo (1.7)
* B.G.  12/04/95        : Modification symbole picto envoye (1.8)
* B.G.  20/09/1995	: Ajout d'un argument Type neutre qui est transmis a equext (neutre, temperature, heure)
* C.T.	03/09/95	: ajout test pour DC et DS (V1.10)
* C.P.	07/01/98	: Correction gestion multi-sites dans 
*			  les procs (1.11)
* JMG	07/01/98        :  Re Correction gestion multi-sites dans
*			  es procs (1.12) (dem/1535)
* LCL   09/07/07        : Suppression DY
* JMG           02/10/09        : secto DEM887
*  JMG           03/03/17 : regio DEM1220
* LCL	30/06/17	: Modif PRA
* JMG	08/08/18 : ligne 18 car suppression SAGA DEM1306 1.17
* LCL	19/11/18	: MOVIS DEM-SAE93 1.18
* GGY	28/04/23	: Augmentation du champ  va_TextePicto_in (XDC_LGR_TXT_PICTO) et va_TypePicto_in (XDC_LGR_NOM_PICTO) (DEM-473)
* GGY	28/04/23	: Augmentation du champ  va_TextePicto_in (XDC_LGR_TXT_PICTO) et va_TypePicto_in (XDC_LGR_NOM_PICTO) (DEM-473)
* LCL	27/09/23 : Adaptations CNA DEM-483	
----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Terminer l'action d'affichage PMV+picto.
* 
* Sequence d'appel
* SP	XZAC50_Commande_Fin_PMV
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
* ACRP50 :	Appeler XZEP01 pour arreter l'affichage sur l'equipement PMV
* 		Appeler XZEP02 pour arreter l'affichage du picto
-----------------------------------------------------*/

use PRC
go

create procedure XZAC;50
	@va_NumeroAction_in		int = null,
	@va_Site_in			T_SITE = null,
	@va_HeureLancementFin_in	datetime = null,
	@va_TypeNeutre_in		tinyint = null,
	@va_NomSiteLocal_in		char(2)	= null
as
	declare	@vl_PMVTexte tinyint, @vl_PMVPasFlash tinyint, @vl_Clignotement tinyint,
		@vl_Texte char(18), @vl_TextePicto char(XDC_LGR_TXT_PICTO), @vl_TypePicto char(XDC_LGR_NOM_PICTO),
		@vl_NumEqt T_EQUIPEMENT, @vl_Status int, @vl_NomMachine char(10),
		@vl_TypeEqtBoucle tinyint, @vl_NumEqtBoucle T_EQUIPEMENT, 
		@vl_ClignotementPicto tinyint,
		@vl_NumMaitre T_EQUIPEMENT, @vl_TypeMaitre tinyint,
		@vl_MachineNonTrouvee bit, @vl_Commande int, @vl_Site tinyint

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
		select @vl_Site = numero 
		from CFG..RES_DIS
        	where serveur = @@servername
	else
		select @vl_Site = numero 
		from CFG..RES_DIS
		where code = @va_NomSiteLocal_in

	if @@rowcount = 0
		return XDC_NOK

	/*A Verifier la non indisponibilite de l'equipement */
	select @vl_NumEqt = EXP..ACT_GEN.equipement 
	from EXP..EQT_DSP, EXP..ACT_GEN
	where 	EXP..ACT_GEN.numero = @va_NumeroAction_in and
		EXP..ACT_GEN.sit = @va_Site_in and
		EXP..ACT_GEN.type = XDC_ACT_PMV and
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

	/*A initialisation des types et texte d'affichage (si type neutre non precise alors XDC_PMV_DEFAUT */
	select	@vl_PMVTexte = isnull(@va_TypeNeutre_in,XDC_PMV_DEFAUT), 
		@vl_PMVPasFlash = XDC_PMV_PASFLASH,
		@vl_Clignotement = XDC_PMV_PASCLIGNO,
		@vl_ClignotementPicto = XDC_PICTO_PASCLIGNO,
		@vl_Texte = null,
		@vl_TextePicto = null,
		@vl_TypePicto = XDC_PICTO_PASPICTO

	/*A numero de commande a envoyer : - numero action */
	select @vl_Commande = - @va_NumeroAction_in

	/*A arreter l'affichage sur PMV */
	/*! le numero picto est egal au numero pmv            */
	/* PRA
	if @@servername = XDC_SQLCI
	*/
	if @vl_Site = XDC_CI
		exec @vl_Status = TASRV_CI...ACRP01 @vl_NumEqt, @vl_Texte, @vl_PMVTexte, 
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_PMVPasFlash, @vl_NumEqt,
		@vl_TextePicto, @vl_TypePicto, @vl_ClignotementPicto, 
		@vl_Commande, @vl_NomMachine
	/* PRA
	else if  @@servername = XDC_SQLVC
	*/
	else if @vl_Site = XDC_VC
		exec @vl_Status = TASRV_VC...ACRP01 @vl_NumEqt, @vl_Texte, @vl_PMVTexte, 
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_PMVPasFlash, @vl_NumEqt,
		@vl_TextePicto, @vl_TypePicto, @vl_ClignotementPicto, 
		@vl_Commande, @vl_NomMachine
	/* PRA
	else if  @@servername = XDC_SQLDP
	*/
	else if @vl_Site = XDC_DP
		exec @vl_Status = TASRV_DP...ACRP01 @vl_NumEqt, @vl_Texte, @vl_PMVTexte, 
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_PMVPasFlash, @vl_NumEqt,
		@vl_TextePicto, @vl_TypePicto, @vl_ClignotementPicto, 
		@vl_Commande, @vl_NomMachine

	else if @vl_Site = XDC_CA
		exec @vl_Status = TASRV_CA...ACRP01 @vl_NumEqt, @vl_Texte, @vl_PMVTexte, 
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_Texte, @vl_PMVTexte,
		@vl_Clignotement, @vl_Texte, @vl_PMVPasFlash, @vl_NumEqt,
		@vl_TextePicto, @vl_TypePicto, @vl_ClignotementPicto, 
		@vl_Commande, @vl_NomMachine

	if @vl_Status != XDC_OK
		return @vl_Status

	return XDC_OK

go
