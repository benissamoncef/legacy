/*E*/
/*  Fichier : $Id: xzac64sp.prc,v 1.10 2021/03/08 14:25:45 pc2dpdy Exp $      Release : $Revision: 1.10 $        Date : $Date: 2021/03/08 14:25:45 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac64sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	24/04/95	: Creation a partir de xzac53
* C.P.	07/01/98	: Correction gestion multi-sites dans 
*			  les procs (1.2)
* JMG	07/01/98        : Re Correction gestion multi-sites dans
			  les procs (1.3) (dem/1536)
* JMG           02/10/09        : secto DEM887 1/.4
* JMG	10/06/16 : pilotage depuis le CI 1.5
* JMG	17/02/1- : regio  1.6
* LCL	03/07/17 : Modif PRA
* JPL	31/08/17 : Correction appels du CIT vers regions et TASRV  1.8
* LCL	20/11/19 : MOVIS DEM-SAE93 1.9
* GRO	12/10/20 : MOVIS DEM-SAE93 1.91
* CGR	05/01/21 : MOVIS ajout PRA DEM-SAE93 1.91
* LCL	28/09/23 : Adaptations CNA DEM-483	
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Remet l'echangeur a l'etat normal
* 
* Sequence d'appel
* SP	XZAC64_Commande_Fin_Echangeur
* 
* Arguments en entree
* XDY_Eqt	va_NumeroEch_in
* XDY_Entier	va_Site_in
* XDY_Horodate	va_HeureLancementFin_in
* XDY_Booleen	va_Manuelle_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: site incorrect
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* La commande est executee si Manuelle est a XDC_Faux
* 
* Fonction
* Inserer dans la table ACTION la date de fin de l'action.
* Dans le cas de commande automatique :
* 	ACRP64 :	Appeler XZET04 pour com
------------------------------------------------------*/

use PRC
go

create procedure XZAC;64
	@va_NumeroEch_in		T_EQUIPEMENT = null,
	@va_Site_in			T_SITE = null,
	@va_HeureLancementFin_in	datetime = null,
	@va_Manuelle_in			bit,
	@va_NomSiteLocal_in		char(2)	= null,
	@va_distant_in 			int = XDC_NON		
as
	declare	@vl_NumeroAction int, @vl_Status int, @vl_Sequence tinyint,
		@vl_Commande int, @vl_Site tinyint,
		@vl_serveur_pra char(6)

	/*! transfo en param�tres nulls bug applix */
	if @va_NumeroEch_in = 0 select @va_NumeroEch_in = null
	if @va_Site_in = 0 select @va_Site_in = null
	if @va_HeureLancementFin_in = XDC_DATE_NULLE select @va_HeureLancementFin_in = null

	/*A controle des parametres obligatoires */
	if  @va_NumeroEch_in = null or @va_Site_in = null or 
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
		if @vl_serveur_pra = XDC_SQLDP
			exec @vl_Status = SQL_DS.PRC..XZAC;64 @va_NumeroEch_in, @va_Site_in, @va_HeureLancementFin_in, @va_Manuelle_in, @va_NomSiteLocal_in, XDC_OUI 

		else if @vl_serveur_pra = XDC_SQLVC
			exec @vl_Status = SQL_DM.PRC..XZAC;64 @va_NumeroEch_in, @va_Site_in, @va_HeureLancementFin_in, @va_Manuelle_in, @va_NomSiteLocal_in, XDC_OUI 
		
		else if @vl_serveur_pra = XDC_SQLCI
		        exec @vl_Status = SQL_CI.PRC..XZAC;64 @va_NumeroEch_in, @va_Site_in, @va_HeureLancementFin_in, @va_Manuelle_in, @va_NomSiteLocal_in, XDC_OUI
		
		else return XDC_NOK

		return @vl_Status	
	end

	/*A tester si le site est correct */
	if not exists ( select numero from CFG..RES_DIS
                                where serveur = @@servername and type <> XDC_TYPE_PCNIVEAU3 )
		return XDC_NOK


	/*A Rechercher la derniere action en cours pour cet equipement */
	select @vl_NumeroAction = max(numero)
	from EXP..ACT_GEN
	where 	EXP..ACT_GEN.equipement = @va_NumeroEch_in and
		EXP..ACT_GEN.sit = @va_Site_in and
		EXP..ACT_GEN.type = XDC_ACT_ECHANGEUR

	if @@rowcount = 0
		select @vl_NumeroAction = 0

	/*A cas de commande automatique par sequence */
	if @va_Manuelle_in =XDC_NON
	begin
		/*A sequence a utiliser */
		select @vl_Sequence = XDC_SEQ_OUV_ECH
	
		/*A numero de commande a envoyer : - numero action */
		select @vl_Commande = - @vl_NumeroAction

		/*A arreter l'affichage echangeur */

		if @vl_Site = XDC_CI
			exec @vl_Status = TASRV_CI...ACRP08 @va_NumeroEch_in, @vl_Sequence,
				@vl_Commande

		else if @vl_Site = XDC_VC
			exec @vl_Status = TASRV_VC...ACRP08 @va_NumeroEch_in, @vl_Sequence,
				@vl_Commande

		else if @vl_Site = XDC_DP
			exec @vl_Status = TASRV_DP...ACRP08 @va_NumeroEch_in, @vl_Sequence,
				@vl_Commande
	
		else if @vl_Site = XDC_CA
			exec @vl_Status = TASRV_CA...ACRP08 @va_NumeroEch_in, @vl_Sequence,
				@vl_Commande
	
		if @vl_Status != XDC_OK
			return @vl_Status 
	end

	/*A mettre a jour la date de lancement fin */
	if @vl_NumeroAction != 0
		update EXP..ACT_GEN set heure_lancement_fin = @va_HeureLancementFin_in
		where	numero = @vl_NumeroAction and sit = @va_Site_in and
			heure_lancement_fin is null

	return XDC_OK

go
