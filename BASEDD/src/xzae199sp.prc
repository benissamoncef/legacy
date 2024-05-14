/*E*/
/*  Fichier : $Id: xzae199sp.prc,v 1.5 2021/03/08 14:12:39 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2021/03/08 14:12:39 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae199sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Creer une FMC a la suite d'une alerte cockpit
*
---------------------------------------------------------------
* HISTORIQUE :
* LCL	18/09/19	: Creation COCKPIT LOT27 DEM1351 V1.1
* ABE	23/06/2020	: Cockpit LOT27 DEM1351 V1.2 Correction suite essai prod
* ABE	29/06/2020	: Cockpit LOT27 DEM1351 V1.3 Ajout r�cup�ration de l'�tat des voies XZAE66
*			: MOVIS DEM-SAE93 ajout site local
* ABE	06/01/21   	: MOVIS ajout PRA DEM-SAE93 1.4
* ABK   26/09/23 	: Ajout district CA (DEM-483)
--------------------------------------------------------------*/

/*X*/
/*-------------------------------------------------------
* Sequence d'appel
* SP	XZAE199_Creer_FMC_Cockpit
* 
* Arguments en entree
* XDY_Horodate	 va_Horodate_in
* XDY_TypeEqt	 va_TypeEqt_in
* XDY_Eqt 	 va_NumEqt_in
* XDY_TypeAlerte va_TypeFMC_in
* XDY_Machine	 va_poste_in
* int		 va_Site_in
* T_STR		 va_Texte_in
* T_STR		 va_IdCockpit_in
* T_STR		 va_Autoroute_in
* int		 va_PR_deb_in
* int		 va_PR_fin_in
* XDY_Octet	 va_Sens_in
* int		 va_Probabilite_in
*
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK		: fiche non trouvee
* XDC_ARG_INV		: argument d'entree  null
* XZAEC_FMC_PAS_RETYPAGE : retypage non autorise (16)
* < 0			: erreur sybase 
* 
* Conditions d'utilisation
* ne fonctionne pas sur pc simplifie 
* 
* Fonction
*
----------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE199' and type = 'P')
	drop procedure XZAE199
go

create procedure XZAE199
	@va_Horodate_in      datetime = null,
	@va_TypeEqt_in       tinyint = null,
	@va_NumEqt_in        smallint = null,
	@va_TypeFMC_in       int,
	@va_Poste_in	     int,
	@va_Site_in	     smallint = null,
	@va_Texte_in         char(100),
	@va_IdCockpit_in     char(10),
	@va_Autoroute_in     char(10),
	@va_PR_deb_in	     int,
	@va_PR_fin_in        int,
	@va_Sens_in          tinyint = null,
	@va_Probabilite_in   int

as

declare
  @vl_SiteAlerte		tinyint,
  @vl_Numero			int,		
  @vl_Serveur			tinyint,
  @vl_Autoroute			tinyint,
  @vl_Status			int,		
  @vl_NumEvt_out		int,
  @vl_NumEvtTete_out		int,
  @vl_CleEvt_out		tinyint,
  @vl_CleEvtTete_out		tinyint,
  @vl_NumFicheAppel_out		int,
  @vl_NumFicheAppelTete_out	int,
  @vl_HorodateValidation	datetime,
  @vl_nom_site_local		char(2),
  @vl_VR			T_VOIE,
  @vl_VM2			T_VOIE,
  @vl_VM1			T_VOIE,
  @vl_VL			T_VOIE,
  @vl_BAU			T_VOIE,
  @vl_VR_I			T_VOIE,
  @vl_VM2_I			T_VOIE,
  @vl_VM1_I			T_VOIE,
  @vl_VL_I			T_VOIE,
  @vl_BAU_I			T_VOIE,
  @vl_Autoroute_out 		tinyint,
  @vl_PR_out			T_PR,
  @vl_Sens_out			T_SENS,
  @vl_PtCaracteristique_out 	tinyint,
  @vl_TypePtCaracteristique_out tinyint,
  @vl_Position_out tinyint,
  @vl_BAU_Etroite_out		tinyint,
  @vl_BAU_I_Etroite_out		tinyint,
  @vl_District_out		tinyint,
  @vl_DistrictRecouvr_out	tinyint,
  @vl_Resultat_out		int,
  @vl_serveur_pra 		char(6)

  /*A
  ** Test des parametres d'entree :
  **   Si un des parametres d'entree est egal a null alors XDC_ARG_INV
  */
  if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null
  if @va_TypeEqt_in = 0 select @va_TypeEqt_in = null
  if @va_NumEqt_in = 0 select @va_NumEqt_in = null
  if @va_TypeFMC_in = 0 select @va_TypeFMC_in = null
  if @va_Site_in = 0 select @va_Site_in = null
  if @va_Poste_in = 0 select @va_Poste_in = null
  /*if @va_Texte_in = 0 select @va_Texte_in = null*/
  /*if @va_IdCockpit_in = 0 select @va_IdCockpit_in = null*/
  /*if @va_Autoroute_in = 0 select @va_Autoroute_in = null*/
  /*if @va_PR_deb_in = 0 select @va_PR_deb_in = null*/
  /*if @va_PR_fin_in = 0 select @va_PR_fin_in = null*/
  /*if @va_Sens_in = 0 select @va_Sens_in = null*/
  /*if @va_Probabilite_in =0 select @va_Probabilite_in= null*/

  if @va_Horodate_in = null
  or @va_TypeEqt_in = null
  or @va_NumEqt_in = null
  or @va_TypeFMC_in = null
  or @va_Site_in = null
  or @va_Poste_in = null
  or @va_Texte_in = null
  or @va_IdCockpit_in = null
  or @va_Autoroute_in = null
  or @va_PR_deb_in = null
  or @va_PR_fin_in = null
  or @va_Sens_in = null
	return ( XDC_ARG_INV )

	/* Init serveur PRA */
	select @vl_serveur_pra=serveur_pra from CFG..RES_PRA where numero=@va_Site_in

	/*A
	** Si FMC existe d�j�
	*/
	
	begin

	select "Debut"

	select * 
	  from  EXP..FMC_GEN
	  	inner join EXP..FMC_TRT on 
	  		EXP..FMC_GEN.numero = EXP..FMC_TRT.evenement
	  where EXP..FMC_GEN.origine_creation like @va_IdCockpit_in
	  and EXP..FMC_GEN.fin = null

	if @@rowcount != 0
		return XZAEC_FMC_PAS_RETYPAGE
		
		
	/*A
	** Si L'�quipement et le type d'alerte existent
	*/

	select 	@vl_Autoroute = numero
	from CFG..RES_AUT
	where nom   = @va_Autoroute_in

        /* alors creer l alerte*/
	
        begin tran
        select @vl_Numero = max ( numero ) from EXP..ALT_EXP where sit=@va_Site_in

        if @vl_Numero = null
             select @vl_Numero = 0

        select @vl_Numero = @vl_Numero + 1

	select @vl_Numero

         insert into EXP..ALT_EXP
         values ( @vl_Numero, @va_TypeEqt_in, @va_NumEqt_in, @va_TypeFMC_in, @va_Horodate_in, @va_Texte_in, XDC_VRAI, null, @va_Site_in, null )
	commit tran
	/*if @va_Site_in = XDC_CI*/
	if @vl_serveur_pra=XDC_SQLCI
		exec @vl_Status = SQL_CI.PRC..XZAE08	@va_TypeFMC_in,
					@va_Poste_in,
					XDC_OUI,
					@va_Horodate_in,
					null,
					@va_Site_in,
					XDC_NON,
					@vl_NumEvt_out		output,
					@vl_CleEvt_out		output,
					@vl_NumFicheAppel_out	output,
					@vl_Numero,
					@va_Site_in
	/*else if @va_Site_in = XDC_DP*/
	else if @vl_serveur_pra=XDC_SQLDP
		exec @vl_Status = SQL_DS.PRC..XZAE08	@va_TypeFMC_in,
					@va_Poste_in,
					XDC_OUI,
					@va_Horodate_in,
					null,
					@va_Site_in,
					XDC_NON,
					@vl_NumEvt_out		output,
					@vl_CleEvt_out		output,
					@vl_NumFicheAppel_out	output,
					@vl_Numero,
					@va_Site_in
	/*else if @va_Site_in = XDC_VC*/
	else if @vl_serveur_pra=XDC_SQLVC
		exec @vl_Status = SQL_DM.PRC..XZAE08	@va_TypeFMC_in,
					@va_Poste_in,
					XDC_OUI,
					@va_Horodate_in,
					null,
					@va_Site_in,
					XDC_NON,
					@vl_NumEvt_out		output,
					@vl_CleEvt_out		output,
					@vl_NumFicheAppel_out	output,
					@vl_Numero,
					@va_Site_in
	/*else if @va_Site_in = XDC_CA*/
	else if @vl_serveur_pra=XDC_SQLCA
		exec @vl_Status = SQL_CA.PRC..XZAE08	@va_TypeFMC_in,
					@va_Poste_in,
					XDC_OUI,
					@va_Horodate_in,
					null,
					@va_Site_in,
					XDC_NON,
					@vl_NumEvt_out		output,
					@vl_CleEvt_out		output,
					@vl_NumFicheAppel_out	output,
					@vl_Numero,
					@va_Site_in

	if @vl_Status != XDC_OK
	begin
		select "XZAE08 err"
		return @vl_Status
	end
	if @va_TypeFMC_in = XZAEC_FMC_QueueBouchon
	begin
		/*if @va_Site_in = XDC_CI*/
		if @vl_serveur_pra=XDC_SQLCI
		exec @vl_Status = SQL_CI.PRC..XZAE08	XZAEC_FMC_TeteBouchon,
						@va_Poste_in,
						XDC_OUI,
						@va_Horodate_in,
						null,
						@va_Site_in,
						XDC_NON,
						@vl_NumEvtTete_out		output,
						@vl_CleEvtTete_out		output,
						@vl_NumFicheAppelTete_out	output,
						@vl_Numero, 
						@va_Site_in
	        /*else if @va_Site_in = XDC_DP*/
		else if @vl_serveur_pra=XDC_SQLDP
		exec @vl_Status = SQL_DS.PRC..XZAE08	XZAEC_FMC_TeteBouchon,
						@va_Poste_in,
						XDC_OUI,
						@va_Horodate_in,
						null,
						@va_Site_in,
						XDC_NON,
						@vl_NumEvtTete_out		output,
						@vl_CleEvtTete_out		output,
						@vl_NumFicheAppelTete_out	output,
						@vl_Numero, 
						@va_Site_in
	        /*else if @va_Site_in = XDC_VC*/
		else if @vl_serveur_pra=XDC_SQLVC
		exec @vl_Status = SQL_DM.PRC..XZAE08	XZAEC_FMC_TeteBouchon,
						@va_Poste_in,
						XDC_OUI,
						@va_Horodate_in,
						null,
						@va_Site_in,
						XDC_NON,
						@vl_NumEvtTete_out		output,
						@vl_CleEvtTete_out		output,
						@vl_NumFicheAppelTete_out	output,
						@vl_Numero, 
						@va_Site_in
			/*else if @va_Site_in = XDC_CA*/
		else if @vl_serveur_pra=XDC_SQLCA
		exec @vl_Status = SQL_CA.PRC..XZAE08	XZAEC_FMC_TeteBouchon,
						@va_Poste_in,
						XDC_OUI,
						@va_Horodate_in,
						null,
						@va_Site_in,
						XDC_NON,
						@vl_NumEvtTete_out		output,
						@vl_CleEvtTete_out		output,
						@vl_NumFicheAppelTete_out	output,
						@vl_Numero, 
						@va_Site_in

		if @vl_Status != XDC_OK
		begin
			select "XZAE08 bouchon err"
        		return @vl_Status
		end	
	end
	if @va_TypeFMC_in = XZAEC_FMC_QueueBouchon
	begin
		/*if @va_Site_in = XDC_CI*/
		if @vl_serveur_pra=XDC_SQLCI							
		exec @vl_Status = SQL_CI.PRC..XZAE;60	@vl_NumEvt_out,
						@vl_CleEvt_out,
						XZAEC_BOU_Ralenti,
						@va_Horodate_in,
						@va_Poste_in,
						XDC_FMC_ETAT_TRAI
	        /*else if @va_Site_in = XDC_DP*/
		else if @vl_serveur_pra=XDC_SQLDP
		exec @vl_Status = SQL_DS.PRC..XZAE;60	@vl_NumEvt_out,
						@vl_CleEvt_out,
						XZAEC_BOU_Ralenti,
						@va_Horodate_in,
						@va_Poste_in,
						XDC_FMC_ETAT_TRAI
	        /*else if @va_Site_in = XDC_VC*/
		else if @vl_serveur_pra=XDC_SQLVC
		exec @vl_Status = SQL_DM.PRC..XZAE;60	@vl_NumEvt_out,
						@vl_CleEvt_out,
						XZAEC_BOU_Ralenti,
						@va_Horodate_in,
						@va_Poste_in,
						XDC_FMC_ETAT_TRAI
	        /*else if @va_Site_in = XDC_CA*/
		else if @vl_serveur_pra=XDC_SQLCA
		exec @vl_Status = SQL_CA.PRC..XZAE;60	@vl_NumEvt_out,
						@vl_CleEvt_out,
						XZAEC_BOU_Ralenti,
						@va_Horodate_in,
						@va_Poste_in,
						XDC_FMC_ETAT_TRAI

		if @vl_Status != XDC_OK
		begin
			select "XZAE60 err"
			return @vl_Status
		end
	end

	else if @va_TypeFMC_in = XZAEC_FMC_Accident
	begin
		/*if @va_Site_in = XDC_CI*/
		if @vl_serveur_pra=XDC_SQLCI
		exec @vl_Status = SQL_CI.PRC..XZAE53	@vl_NumEvt_out,
						@vl_CleEvt_out,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						@va_Poste_in,
						@va_Horodate_in,
						0,
						0,
						0,
						0
	        /*else if @va_Site_in = XDC_DP*/
		else if @vl_serveur_pra=XDC_SQLDP
		exec @vl_Status = SQL_DS.PRC..XZAE53	@vl_NumEvt_out,
						@vl_CleEvt_out,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						@va_Poste_in,
						@va_Horodate_in,
						0,
						0,
						0,
						0
	        /*else if @va_Site_in = XDC_VC*/
		else if @vl_serveur_pra=XDC_SQLVC
		exec @vl_Status = SQL_DM.PRC..XZAE53	@vl_NumEvt_out,
						@vl_CleEvt_out,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						@va_Poste_in,
						@va_Horodate_in,
						0,
						0,
						0,
						0
			/*else if @va_Site_in = XDC_CA*/
		else if @vl_serveur_pra=XDC_SQLCA
		exec @vl_Status = SQL_CA.PRC..XZAE53	@vl_NumEvt_out,
						@vl_CleEvt_out,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						@va_Poste_in,
						@va_Horodate_in,
						0,
						0,
						0,
						0

		if @vl_Status != XDC_OK
		begin
			select "XZAE53 err"
			return @vl_Status
		end
	end
	else if	@va_TypeFMC_in = XZAEC_FMC_Obstacle
	begin
		/*if @va_Site_in = XDC_CI*/
		if @vl_serveur_pra=XDC_SQLCI
		exec @vl_Status = SQL_CI.PRC..XZAE208	@vl_NumEvt_out,
						@vl_CleEvt_out,
						@va_Horodate_in,
						"",
						0,
						@va_Poste_in,
						XDC_FMC_ETAT_TRAI
        	/*else if @va_Site_in = XDC_DP*/
		else if @vl_serveur_pra=XDC_SQLDP
		exec @vl_Status = SQL_DS.PRC..XZAE208	@vl_NumEvt_out,
						@vl_CleEvt_out,
						@va_Horodate_in,
						"",
						0,
						@va_Poste_in,
						XDC_FMC_ETAT_TRAI
        	/*else if @va_Site_in = XDC_VC*/
		else if @vl_serveur_pra=XDC_SQLVC
		exec @vl_Status = SQL_DM.PRC..XZAE208	@vl_NumEvt_out,
						@vl_CleEvt_out,
						@va_Horodate_in,
						"",
						0,
						@va_Poste_in,
						XDC_FMC_ETAT_TRAI
			/*else if @va_Site_in = XDC_CA*/
		else if @vl_serveur_pra=XDC_SQLCA
		exec @vl_Status = SQL_CA.PRC..XZAE208	@vl_NumEvt_out,
						@vl_CleEvt_out,
						@va_Horodate_in,
						"",
						0,
						@va_Poste_in,
						XDC_FMC_ETAT_TRAI

		if @vl_Status != XDC_OK
		begin
			select "XZAE208 err"
			return @vl_Status
		end
	end

	/* Get etat des voies */
	
	exec @vl_Status = XZAE66 @vl_Autoroute,
				@va_PR_deb_in,
				@va_Sens_in,
				null,
				null,
				null,
				@va_TypeFMC_in,
				@vl_Autoroute_out output,
				@vl_PR_out output,
				@vl_Sens_out output,
				@vl_PtCaracteristique_out output,
				@vl_TypePtCaracteristique_out output,
				@vl_Position_out output,
				@vl_VR output,
				@vl_VM2 output,
				@vl_VM1 output,
				@vl_VL output,
				@vl_BAU output,
				@vl_VR_I output,
				@vl_VM2_I output,
				@vl_VM1_I output,
				@vl_VL_I output,
				@vl_BAU_I output,
				@vl_BAU_Etroite_out output,
				@vl_BAU_I_Etroite_out output,
				@vl_District_out output,
				@vl_DistrictRecouvr_out output,
				@vl_Resultat_out output
	if @vl_Status != XDC_OK
	begin
		select "XZAE66 err"
		return @vl_Status
	end

	if @va_Site_in = XDC_CI
		select @vl_nom_site_local = XDC_NOM_SITE_CI
	else if @va_Site_in = XDC_DP
		select @vl_nom_site_local = XDC_NOM_SITE_DP
	else if @va_Site_in = XDC_VC
		select @vl_nom_site_local = XDC_NOM_SITE_VC
	else if @va_Site_in = XDC_CA
		select @vl_nom_site_local = XDC_NOM_SITE_CA

	/*if @va_Site_in = XDC_CI*/
	if @vl_serveur_pra=XDC_SQLCI
		exec @vl_Status = SQL_CI.PRC..XZAE11	@vl_NumEvt_out,
					@vl_CleEvt_out,
					@va_Poste_in,
					XDC_FMC_ETAT_TRAI,
					@va_TypeFMC_in,
					null,
					@va_Horodate_in,
					null,
					null,
					@va_Horodate_in,
					@vl_NumEvtTete_out,
					@vl_CleEvtTete_out,
					null,
					@va_IdCockpit_in,
					@va_Poste_in,
				 	@va_Probabilite_in,
					@vl_Autoroute,
					@va_PR_deb_in,
					@va_Sens_in,
					null,
					null,
					null,
					@vl_VR,
					@vl_VM2,
					@vl_VM1,
					@vl_VL,
					@vl_BAU,
					@vl_VR_I,
					@vl_VM2_I,
					@vl_VM1_I,
					@vl_VL_I,
					@vl_BAU_I,
					4,
					4,
					null,
					0,
					null,
					null,
					null,
					null,
					null,
					null,
					@vl_nom_site_local
	/*else if @va_Site_in = XDC_DP*/
	else if @vl_serveur_pra=XDC_SQLDP
		exec @vl_Status = SQL_DS.PRC..XZAE11	@vl_NumEvt_out,
					@vl_CleEvt_out,
					@va_Poste_in,
					XDC_FMC_ETAT_TRAI,
					@va_TypeFMC_in,
					null,
					@va_Horodate_in,
					null,
					null,
					@va_Horodate_in,
					@vl_NumEvtTete_out,
					@vl_CleEvtTete_out,
					null,
					@va_IdCockpit_in,
					@va_Poste_in,
				 	@va_Probabilite_in,
					@vl_Autoroute,
					@va_PR_deb_in,
					@va_Sens_in,
					null,
					null,
					null,
					@vl_VR,
					@vl_VM2,
					@vl_VM1,
					@vl_VL,
					@vl_BAU,
					@vl_VR_I,
					@vl_VM2_I,
					@vl_VM1_I,
					@vl_VL_I,
					@vl_BAU_I,
					4,
					4,
					null,
					0,
					null,
					null,
					null,
					null,
					null,
					null,
					@vl_nom_site_local
	/*else if @va_Site_in = XDC_VC*/
	else if @vl_serveur_pra=XDC_SQLVC
		exec @vl_Status = SQL_DM.PRC..XZAE11	@vl_NumEvt_out,
					@vl_CleEvt_out,
					@va_Poste_in,
					XDC_FMC_ETAT_TRAI,
					@va_TypeFMC_in,
					null,
					@va_Horodate_in,
					null,
					null,
					@va_Horodate_in,
					@vl_NumEvtTete_out,
					@vl_CleEvtTete_out,
					null,
					@va_IdCockpit_in,
					@va_Poste_in,
				 	@va_Probabilite_in,
					@vl_Autoroute,
					@va_PR_deb_in,
					@va_Sens_in,
					null,
					null,
					null,
					@vl_VR,
					@vl_VM2,
					@vl_VM1,
					@vl_VL,
					@vl_BAU,
					@vl_VR_I,
					@vl_VM2_I,
					@vl_VM1_I,
					@vl_VL_I,
					@vl_BAU_I,
					4,
					4,
					null,
					0,
					null,
					null,
					null,
					null,
					null,
					null,
					@vl_nom_site_local
	/*else if @va_Site_in = XDC_CA*/
	else if @vl_serveur_pra=XDC_SQLCA
		exec @vl_Status = SQL_CA.PRC..XZAE11	@vl_NumEvt_out,
					@vl_CleEvt_out,
					@va_Poste_in,
					XDC_FMC_ETAT_TRAI,
					@va_TypeFMC_in,
					null,
					@va_Horodate_in,
					null,
					null,
					@va_Horodate_in,
					@vl_NumEvtTete_out,
					@vl_CleEvtTete_out,
					null,
					@va_IdCockpit_in,
					@va_Poste_in,
				 	@va_Probabilite_in,
					@vl_Autoroute,
					@va_PR_deb_in,
					@va_Sens_in,
					null,
					null,
					null,
					@vl_VR,
					@vl_VM2,
					@vl_VM1,
					@vl_VL,
					@vl_BAU,
					@vl_VR_I,
					@vl_VM2_I,
					@vl_VM1_I,
					@vl_VL_I,
					@vl_BAU_I,
					4,
					4,
					null,
					0,
					null,
					null,
					null,
					null,
					null,
					null,
					@vl_nom_site_local
	

		if @vl_Status != XDC_OK
		begin
			select "XZAE11 err"
			return @vl_Status
		end
	if @va_TypeFMC_in = XZAEC_FMC_QueueBouchon
	begin
        /*if @va_Site_in = XDC_CI*/
	if @vl_serveur_pra=XDC_SQLCI
		exec @vl_Status = SQL_CI.PRC..XZAE11        @vl_NumEvtTete_out,
	                                @vl_CleEvtTete_out,
					@va_Poste_in,
					XDC_FMC_ETAT_TRAI,
					XZAEC_FMC_TeteBouchon,
					null,
					@va_Horodate_in,
					null,
					null,
					@va_Horodate_in,
					null,
					null,
					null,
					@va_IdCockpit_in,
					@va_Poste_in,
					@va_Probabilite_in,
					@vl_Autoroute,
					@va_PR_fin_in,
					@va_Sens_in,
					null,
					null,
					null,
					@vl_VR,
					@vl_VM2,
					@vl_VM1,
					@vl_VL,
					@vl_BAU,
					@vl_VR_I,
					@vl_VM2_I,
					@vl_VM1_I,
					@vl_VL_I,
					@vl_BAU_I,
					4,
					4,
					null,
					0,
					null,
					null,
					null,
					null,		
					null,
					null,
					@vl_nom_site_local
	/*else if @va_Site_in = XDC_DP*/
	else if @vl_serveur_pra=XDC_SQLDP
		exec @vl_Status = SQL_DS.PRC..XZAE11        @vl_NumEvtTete_out,
	                                @vl_CleEvtTete_out,
					@va_Poste_in,
					XDC_FMC_ETAT_TRAI,
					XZAEC_FMC_TeteBouchon,
					null,
					@va_Horodate_in,
					null,
					null,
					@va_Horodate_in,
					null,
					null,
					null,
					@va_IdCockpit_in,
					@va_Poste_in,
					@va_Probabilite_in,
					@vl_Autoroute,
					@va_PR_fin_in,
					@va_Sens_in,
					null,
					null,
					null,
					@vl_VR,
					@vl_VM2,
					@vl_VM1,
					@vl_VL,
					@vl_BAU,
					@vl_VR_I,
					@vl_VM2_I,
					@vl_VM1_I,
					@vl_VL_I,
					@vl_BAU_I,
					4,
					4,
					null,
					0,
					null,
					null,
					null,
					null,		
					null,
					null,
					@vl_nom_site_local
	/*else if @va_Site_in = XDC_VC*/
	else if @vl_serveur_pra=XDC_SQLVC								
		exec @vl_Status = SQL_DM.PRC..XZAE11        @vl_NumEvtTete_out,
	                                @vl_CleEvtTete_out,
					@va_Poste_in,
					XDC_FMC_ETAT_TRAI,
					XZAEC_FMC_TeteBouchon,
					null,
					@va_Horodate_in,
					null,
					null,
					@va_Horodate_in,
					null,
					null,
					null,
					@va_IdCockpit_in,
					@va_Poste_in,
					@va_Probabilite_in,
					@vl_Autoroute,
					@va_PR_fin_in,
					@va_Sens_in,
					null,
					null,
					null,
					@vl_VR,
					@vl_VM2,
					@vl_VM1,
					@vl_VL,
					@vl_BAU,
					@vl_VR_I,
					@vl_VM2_I,
					@vl_VM1_I,
					@vl_VL_I,
					@vl_BAU_I,
					4,
					4,
					null,
					0,
					null,
					null,
					null,
					null,		
					null,
					null,
					@vl_nom_site_local
		/*else if @va_Site_in = XDC_CA*/
	else if @vl_serveur_pra=XDC_SQLCA								
		exec @vl_Status = SQL_CA.PRC..XZAE11        @vl_NumEvtTete_out,
	                                @vl_CleEvtTete_out,
					@va_Poste_in,
					XDC_FMC_ETAT_TRAI,
					XZAEC_FMC_TeteBouchon,
					null,
					@va_Horodate_in,
					null,
					null,
					@va_Horodate_in,
					null,
					null,
					null,
					@va_IdCockpit_in,
					@va_Poste_in,
					@va_Probabilite_in,
					@vl_Autoroute,
					@va_PR_fin_in,
					@va_Sens_in,
					null,
					null,
					null,
					@vl_VR,
					@vl_VM2,
					@vl_VM1,
					@vl_VL,
					@vl_BAU,
					@vl_VR_I,
					@vl_VM2_I,
					@vl_VM1_I,
					@vl_VL_I,
					@vl_BAU_I,
					4,
					4,
					null,
					0,
					null,
					null,
					null,
					null,		
					null,
					null,
					@vl_nom_site_local
	
		if @vl_Status != XDC_OK
		begin
			select "XZAE11 bouchon err"
			return @vl_Status						
		end
	end
  end

	/*if @va_Site_in = XDC_CI*/
	if @vl_serveur_pra=XDC_SQLCI
	begin
		/* Lib�ration FMC */
		exec @vl_Status = SQL_CI.PRC..XZAE210	@vl_NumEvt_out,
					@vl_CleEvt_out,
					@va_Poste_in,
					0
		/* R�veil */
		exec @vl_Status = SQL_CI.PRC..XZAR;03 @vl_NumEvt_out,@vl_CleEvt_out,null,"NOUVELLE FMC",@vl_CleEvt_out
	end
	/*else if @va_Site_in = XDC_DP*/
	else if @vl_serveur_pra=XDC_SQLDP
	begin
		/* Lib�ration FMC */
		exec @vl_Status = SQL_DS.PRC..XZAE210	@vl_NumEvt_out,
					@vl_CleEvt_out,
					@va_Poste_in,
					0
		/* R�veil */
		exec @vl_Status = SQL_DS.PRC..XZAR;03 @vl_NumEvt_out,@vl_CleEvt_out,null,"NOUVELLE FMC",@vl_CleEvt_out
	end
	/*else if @va_Site_in = XDC_VC*/
	else if @vl_serveur_pra=XDC_SQLVC
	begin
		/* Lib�ration FMC */
		exec @vl_Status = SQL_DM.PRC..XZAE210	@vl_NumEvt_out,
					@vl_CleEvt_out,
					@va_Poste_in,
					0
		/* R�veil */
		exec @vl_Status = SQL_DM.PRC..XZAR;03 @vl_NumEvt_out,@vl_CleEvt_out,null,"NOUVELLE FMC",@vl_CleEvt_out
	end
	/*else if @va_Site_in = XDC_CA*/
	else if @vl_serveur_pra=XDC_SQLCA
	begin
		/* Lib�ration FMC */
		exec @vl_Status = SQL_CA.PRC..XZAE210	@vl_NumEvt_out,
					@vl_CleEvt_out,
					@va_Poste_in,
					0
		/* R�veil */
		exec @vl_Status = SQL_CA.PRC..XZAR;03 @vl_NumEvt_out,@vl_CleEvt_out,null,"NOUVELLE FMC",@vl_CleEvt_out
	end



			return XDC_OK
go

