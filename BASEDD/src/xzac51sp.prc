/*E*/
/*  Fichier : $Id: xzac51sp.prc,v 1.17 2020/10/13 17:21:24 pc2dpdy Exp $      Release : $Revision: 1.17 $        Date : $Date: 2020/10/13 17:21:24 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac51sp.prc
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
*                         pour ACRP03 est negatif
* C.T.	23/09/94	: Modification du traitement de
*                         recherche du nom de la machine
*                         pilotant (V 1.3)
* C.T.	10/11/94	: Modif bugs applix (V 1.4)
*                         non teste
* C.T.	05/12/94	: Correction erreur nom maitre V 1.5
*                         non teste
* C.T.	28/03/95	: Modif nom scenario de liberation V 1.6
* B.G.	12/04/95	: ajout condition dernier dans EQT_DSP (1.7)
* B.G.  12/04/95        : ajout condition dernier dans EQT_DSP (1.8)
* C.T.	03/09/95	: ajout test pour DC et DS (V1.9)
* C.P.	07/01/98	: Correction gestion multi-sites dans 
*			  les procs (1.10)
* JMG	07/01/98        : Re Correction gestion multi-sites dans
*                         les procs (1.11) (dem/1536)
* C.T.	20/01/98	: Ajout commande action FAU et FAC (1538) (V1.12)
*                         
* Fonction : NON UTILISE (garder pour memoire)
* LCL   09/07/07        : Suppression DY
* JMG           02/10/09        : secto DEM887
*  JMG           03/03/17 : regio DEM1220
* LCL	30/06/17	: Modif PRA 
* LCL	19/11/19	: MOVIS DEM-SAE93 1.17
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retour a l'etat normal de la NAV par scenario ou par 
* commande individuelle
* 
* Sequence d'appel
* SP	XZAC51_Commande_Fin_NAV
* 
* Arguments en entree
* XDY_Entier	va_NumeroAction_in
* XDY_Entier	va_Site_in
* XDY_Horodate	va_HeureLancementFin_in
* XDY_Booleen	va_Manuelle_in
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
* L'execution de la commande de retour a l'etat normal 
* est executee si Manuelle est positionne a XDC_Faux
* 
* Fonction
* Verifier la non indisponibilite de l'equipement.
* Inserer dans la table ACTION la date de fin de l'actio.
* 
* Dans le cas de commande automatique par scenario :
* 	ACRP03 :	Appeler XZEN03 pour commander l'equipement
--------------------------------------------------------*/

use PRC
go

create procedure XZAC;51
	@va_NumeroAction_in		int = null,
	@va_Site_in			T_SITE = null,
	@va_HeureLancementFin_in	datetime = null,
	@va_Manuelle_in			bit,
	@va_NomSiteLocal_in		char(2)	= null
as
	declare	@vl_NumEqt T_EQUIPEMENT, @vl_Status int, @vl_NomMachine char(10),
		@vl_Scenario char(4), @vl_Commande int, @vl_TypeAction tinyint,
		@vl_TypeEqtBoucle tinyint, @vl_NumEqtBoucle T_EQUIPEMENT, 
		@vl_NumMaitre T_EQUIPEMENT, @vl_TypeMaitre tinyint,
		@vl_MachineNonTrouvee bit, @vl_Site tinyint

	/*! transfo en param�tres nulls bug applix */
	if @va_NumeroAction_in = 0 select @va_NumeroAction_in = null
	if @va_Site_in = 0 select @va_Site_in = null 
	if @va_HeureLancementFin_in = XDC_DATE_NULLE select @va_HeureLancementFin_in = null

	/*A controle des parametres obligatoires */
	if  @va_NumeroAction_in = null or @va_Site_in = null or 
	    @va_HeureLancementFin_in = null
		return XDC_ARG_INV

	/*A tester si le site est correct */
	if @va_NomSiteLocal_in = null
	begin
		if not exists ( select numero from CFG..RES_DIS
                        where serveur = @@servername and numero = @va_Site_in)
		return XDC_NOK
	end
	else
	begin
		if not exists ( select numero from CFG..RES_DIS
                        where numero = @va_NomSiteLocal_in)
		return XDC_NOK
	end

	/* MOVIS */
	if @va_NomSiteLocal_in = null
		select  @vl_Site = numero
		from CFG..RES_DIS
        	where serveur = @@servername
	else
		select  @vl_Site = numero
		from CFG..RES_DIS
		where code = @va_NomSiteLocal_in

	/*A Rechercher le type de l'action : BRA, FAC ou FAU */
	select @vl_TypeAction = type
	from EXP..ACT_GEN
	where 	EXP..ACT_GEN.equipement = @va_NumeroAction_in and
		EXP..ACT_GEN.sit = @va_Site_in

	if @@rowcount != 1
		return XDC_NOK

	/*A Verifier la non indisponibilite de l'equipement */
	select @vl_NumEqt = EXP..ACT_GEN.equipement 
	from EXP..EQT_DSP, EXP..ACT_GEN
	where 	EXP..ACT_GEN.numero = @va_NumeroAction_in and
		EXP..ACT_GEN.sit = @va_Site_in and
		EXP..ACT_GEN.type = @vl_TypeAction and
		EXP..EQT_DSP.equipement = EXP..ACT_GEN.equipement and
		EXP..EQT_DSP.type = @vl_TypeAction and
		EXP..EQT_DSP.majeure = XDC_NON and
		EXP..EQT_DSP.HS = XDC_NON and
		EXP..EQT_DSP.desactive = XDC_NON and
		EXP..EQT_DSP.critique = XDC_NON and
		EXP..EQT_DSP.dernier = XDC_OUI

	if @@rowcount != 1
		return XDC_NOK

	/*A mettre a jour la date de lancement fin */
	update EXP..ACT_GEN set heure_lancement_fin = @va_HeureLancementFin_in
	where	numero = @va_NumeroAction_in and sit = @va_Site_in and
		heure_lancement_fin is null

	if @@rowcount > 1
		return XDC_NOK

	/*A cas de commande automatique par scenario */
	if @va_Manuelle_in =XDC_NON
	begin
		/*A initialisation des var locales pour la recherche */
		/*A du nom de la machine pilotant la NAV */
		select	@vl_MachineNonTrouvee = XDC_NON,
			@vl_NumEqtBoucle = @vl_NumEqt,
			@vl_TypeEqtBoucle = XDC_EQT_NAV
	
		/*A recherche le nom de la machine pilotant la NAV */
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

		/*A scenario a utiliser */
		select @vl_Scenario = XDC_SCN_LIB_NAV
	
		/*A numero de commande a envoyer : - numero action */
		select @vl_Commande = - @va_NumeroAction_in

		/*A arreter la NAV */
       		/* PRA 
		if @@servername = XDC_SQLCI
		*/
		if @vl_Site = XDC_CI
			exec @vl_Status = TASRV_CI...ACRP03 @vl_NumEqt, @vl_Scenario,
				@vl_Commande, @vl_NomMachine
		else if  @vl_Site = XDC_VC
			exec @vl_Status = TASRV_VC...ACRP03 @vl_NumEqt, @vl_Scenario,
				@vl_Commande, @vl_NomMachine
		else if  @vl_Site = XDC_DP
			exec @vl_Status = TASRV_DP...ACRP03 @vl_NumEqt, @vl_Scenario,
				@vl_Commande, @vl_NomMachine
		else if  @vl_Site = XDC_CA
			exec @vl_Status = TASRV_CA...ACRP03 @vl_NumEqt, @vl_Scenario,
				@vl_Commande, @vl_NomMachine
		if @vl_Status != XDC_OK
			return @vl_Status
	end

	return XDC_OK

go
