/*E*/
/*Fichier : $Id: xzap50sp.prc,v 1.2 2013/09/30 11:14:26 devgtie Exp $      Release : $Revision: 1.2 $        Date :  $Date: 2013/09/30 11:14:26 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAP * FICHIER xzap50sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Plan d'Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	02/12/08	: creation
* ABE   26/03/13	: gestion du retour a la normale avec ajout de sc�narios pour le domaine exploitation
* ABE   12/08/22    : Ajout consultation de la table EQT_SQR_GTC_SIG pour proposer le retour a la normal specifique dans le PA DEM-SAE406
* GGY	26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/

use PRC
go

/*X------------------------------------------------------
* SERVICE RENDU : 
*
* gere la fin d un evenement pour le pilotage GTC
* sur la ZI du tunnel 
* si pas evt en cours : proposition de remise au neutre
* si evt en cours : afficher un reveil pour la FMC la plus recente
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

if exists (select * from sysobjects where name = 'XZAP50' and type = 'P')
	drop procedure XZAP50
go

create procedure XZAP50

	@NumPA_In		smallint,
	@Ordre_In		smallint,
	@District_In		tinyint,
	@Tube_In		smallint,
	@Type_In		tinyint
	

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		NumPA_In
* XDY_Mot 		Ordre_In
* XDY_District		District_In
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION :
*
* FONCTION :
*
* Ajoute dans la table SAD_VEN
* cree
*
* MECANISMES :
*
------------------------------------------------------*/


as 

declare @vl_autoroute_tube	tinyint,
	@vl_sens_tube	tinyint,
	@vl_pr_tube	int,
	@vl_pr_influence	int,
	@vl_min int,
	@vl_max int,
	@vl_scenario tinyint,
	@vl_numero int,
	@vl_cle int,
	@vl_enrichisseur int,
	@vl_Site tinyint,
	@vl_Status int,
	@vl_lib varchar(30),
	@vl_scenario_terrain tinyint,
	@vl_evt int,
	@vl_numAct int, 
	@vl_SiteMaitre tinyint

	/* Verification du site */
	select  @vl_SiteMaitre = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

   /*A 
   ** Test des parametres d'entree :
   **    Si un des parametres d'entree est egal a null alors XDC_NOK
   */

   if      @NumPA_In = null 
   	or @Ordre_In = null 
   	or @District_In = null
	or @Tube_In = null 
	or @Type_In = null
      
      return XDC_NOK
   else   
      begin
   
      /*recupere autoroute, PR, sens du tube*/
      select @vl_autoroute_tube=autoroute,
		@vl_sens_tube=sens,
		@vl_pr_tube=PR
	from CFG..EQT_GEN
	where type=XDC_EQT_TUB 
	and numero=@Tube_In

	if @@rowcount=0
	begin
		return XDC_ARG_INV
	end
     
	/*recupere PR influence du tube*/
	select @vl_pr_influence=PR_Influence
	from CFG..EQT_TUB
	where numero=@Tube_In

	if @@rowcount=0
	begin
		return XDC_ARG_INV
	end

	if @vl_pr_influence>@vl_pr_tube
	begin
		select @vl_min=@vl_pr_tube
		select @vl_max=@vl_pr_influence
	end
	else
	begin
		select @vl_min=@vl_pr_influence
		select @vl_max=@vl_pr_tube
	end

	/*recherches les FMC en cours dans la zone d'influence*/
	select g.numero as numero,g.cle as cle,g.debut as debut,g.sit as sit, g.poste_enrichisseur as poste_enrichisseur into  #TEMPO_FMC
	from EXP..FMC_HIS_DER h, EXP..FMC_GEN g
	where h.autoroute=@vl_autoroute_tube
	and h.sens = @vl_sens_tube
	and h.numero=g.numero
	and h.cle=g.cle
	and g.fin is null
	and g.debut is not null
	and h.PR >= @vl_min
	and h.PR <= @vl_max
	order by g.debut desc

	if @@rowcount=0
	begin
		select @vl_scenario=XDC_TUB_ETEINT
		if @Type_In=XDC_EQT_TUB
			select @vl_scenario=XDC_TUB_ETEINT, @vl_lib = "EXTINCTION SIGNAL"
		else if @Type_In=XDC_ACT_GTC_SIG
			begin
				/*
				* Recherche du scenario en cours sur le terrain
				*/
				select @vl_evt = evenement from EXP..SAD_PLN where numero = @NumPA_In
				select @vl_numAct = numero from EXP..ACT_GEN where evenement = @vl_evt and type = XDC_ACT_GTC_SIG and heure_fin is null and equipement = @Tube_In
				select @vl_scenario_terrain = sequence from EXP..ACT_GTC_SIG where actio = @vl_numAct and equipement = @Tube_In
				/*
				* Test pour savoir si le scenario sur le terrain possede un scenario de retour a la normal definit dans EQT_SQR_GTC_SIG
				*/
				if exists (select * from CFG..EQT_SQR_GTC_SIG where numero_equipement = @Tube_In and scenario_terrain = @vl_scenario_terrain)
					select @vl_scenario = scenario_retour_normal from CFG..EQT_SQR_GTC_SIG where numero_equipement = @Tube_In and scenario_terrain = @vl_scenario_terrain
				else
					select @vl_scenario=XDC_SIG_SEQ_RETOUR_NORMAL, @vl_lib = "EXTINCTION SIGNAL"  
			end

			
		else if @Type_In=XDC_ACT_GTC_ECL
			select @vl_scenario=XDC_ECL_SEQ_RETOUR_NORMAL, @vl_lib = "EXTINCTION ECL"
		else if @Type_In=XDC_ACT_GTC_VEN
			select @vl_scenario=XDC_VEN_SEQ_RETOUR_NORMAL, @vl_lib = "EXTINCTION VENTIL"
		else if @Type_In=XDC_ACT_GTC_EXP
			select @vl_scenario=XDC_EXP_SEQ_RETOUR_NORMAL, @vl_lib = "EXTINCTION EXPLOI"

		/*remise au neutre suivant le type d action*/
		exec PRC..XZAP;04       @NumPA_In,
					@Ordre_In,
					@District_In,
					"CE TUNNEL N'EST PLUS UTILE POUR L'EVENEMENT",
					0,
					@Tube_In,
					"CE TUNNEL N'EST PLUS UTILE POUR L'EVENEMENT",
					@vl_lib,	
					@vl_scenario,
					XDC_VRAI,
					@Type_In
	end
	else
	begin
		/*generer un reveil sur la FMC la plus recente*/
		/*au CI et sur le site de l action*/
		select @vl_Site=numero
		from CFG..RES_DIS 
		where serveur=@@servername
		if @vl_Site = @vl_SiteMaitre or @vl_Site=@District_In 
		begin
			set rowcount 1

			select @vl_numero=numero ,@vl_cle=cle,@vl_enrichisseur=poste_enrichisseur, @vl_Site=sit
			from #TEMPO_FMC

			exec @vl_Status = XZAR;03 @vl_numero,@vl_cle,@vl_enrichisseur,"PLAN D ACTIONS A CALCULER"
			if @vl_Status != XDC_OK
				return @vl_Status
		end
	end

      end

	return XDC_OK
go
