/*E*/
/*  Fichier : $Id: xzac16sp.prc,v 1.16 2021/03/08 14:37:17 pc2dpdy Exp $      Release : $Revision: 1.16 $        Date : $Date: 2021/03/08 14:37:17 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac16sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	07/09/94	: Creation
* C.T.	13/09/94	: Modif nom param entree 
*                         (HoroFin au lieu de HoroLancFin)
* C.T.	28/09/94	: Modif NomIntervention en " " si null
* B.G.  17/10/94        : Suppression des appels xzah
* C.T.	10/11/94	: Modif bugs applix (V 1.5)
* C.T.	09/12/94	: Modif clause existance action (V 1.6)
*                         non teste
* JMG	28/11/96	: ajout competence et annulation_intervention (gestion des astreintes) (DEM/1306) 1.7
* C.P.	07/01/98	: Correction gestion multi-sites dans 
*			  les procs (1.8)
* JMG	03/02/98	: ajout sit_origine (dem/1605) 1.9
* PNI	18/03/09	: Exec de la proc sur le sit de l'action DEM871 1.10
* JMG   02/10/09        : secto DEM887 1.11
* PNI 	02/02/15	: Creation de l'action au sein d'une transaction  1.12
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.13
* LCL   25/03/20   	: MOVIS Ajout site local DEM-SAE93
* LPE	12/10/20 	: MOVIS DEM-SAE93 1.15
* ABE	06/01/21   	: MOVIS ajout PRA DEM-SAE93 1.16
* LCL	27/09/23 : Adaptations CNA DEM-483	
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAC16_Ecrire_Autre_Intervention
* 
* Arguments en entrEe
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* XDY_Entier		va_NumeroAction_in
* XDY_Octet		va_Site_in
* XDY_Nom		va_NomIntervention_in
* XDY_Octet		va_TypeIntervention_in
* XY_Horodate		va_HoroLancement_in
* XY_Horodate		va_HoroFin_in
* XDY_Operateur		va_Operateur_in
* XY_Horodate		va_HoroSucces_in
* XDY_Entier		va_Competence_in
* XY_Horodate           va_HoroAnnulation_in
* XDY_Octet             va_Annulation_in

* Arguments en sortie
* XDY_Entier		va_NumeroAction_out
* 
* Service rendu
* Inserer ou modifier une intervention de type pompiers, gendarmes, ...
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio ou evnement non trouvee ou insertion impossible ou site incorrect
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Si le numero d'actio et le site sont non values en entree 
* alors l'intervention d'un depanneur est ajoutee sinon elle est modifiee
* 
* Fonction
* Verifier l'existence de l'evenement
* Inserer dans les tables ACTION, INTERVENTION
* Appeler XZAH12_Animer_Objet_Mobile
------------------------------------------------------*/

use PRC
go
create procedure XZAC;16
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null,
	@va_NumeroAction_in	int		= null,
	@va_Site_in		T_SITE		= null,
	@va_NomIntervention_in	T_NOM		= null,
	@va_TypeIntervention_in	tinyint		= null,
	@va_HoroLancement_in	datetime	= null,
	@va_HoroFin_in		datetime	= null,
	@va_Operateur_in	T_OPERATEUR	= null,
	@va_HoroSucces_in	datetime	= null,
	@va_HoroAnnulation_in	datetime	= null,
	@va_Annulation_in	tinyint		= null,
	@va_Competence_in       smallint        = null,
	@va_NumeroAction_out	int		= null	output,
	@va_NomSiteLocal_in	char(2)		= null,
	@va_distant_in 		int 		= XDC_NON
as
	declare @vl_Site tinyint,@vl_Autoroute T_AUTOROUTE, @vl_Sens T_SENS, @vl_PR T_PR, @vl_Status int, 
	@vl_serveur_pra char(6)

/*A recherche le site */
	/* MOVIS */
	if @va_NomSiteLocal_in = null
	        select @vl_Site = numero 
        	from CFG..RES_DIS
	        where serveur = @@servername
	else
		if @va_distant_in = XDC_NON
		begin
			select @vl_Site = numero
	        	from CFG..RES_DIS
			where code = @va_NomSiteLocal_in
		end
		else
		begin
			select @vl_Site = @va_Site_in
		end

	/*A On est au CI et on pilote un autre district */
	if @va_Site_in != null and @va_Site_in != 0 and @vl_Site != @va_Site_in and (@vl_Site = XDC_CI or @va_Site_in =  XDC_CI) and @va_distant_in = XDC_NON
	begin

	    /* Init serveur PRA */
	    select @vl_serveur_pra=serveur_pra from CFG..RES_PRA where numero=@va_Site_in

		/*if @va_Site_in = XDC_CI*/
		if @vl_serveur_pra=XDC_SQLCI
			exec @vl_Status = SQL_CI.PRC..XZAC;16 @va_NumEvt_in,
						@va_CleEvt_in,
						@va_NumeroAction_in,
						@va_Site_in,
						@va_NomIntervention_in,
						@va_TypeIntervention_in,
						@va_HoroLancement_in,
						@va_HoroFin_in,
						@va_Operateur_in,
						@va_HoroSucces_in,
						@va_HoroAnnulation_in,
						@va_Annulation_in,
						@va_Competence_in,
						@va_NumeroAction_out output,
						@va_NomSiteLocal_in,
						XDC_OUI
		/*else if @va_Site_in = XDC_VC*/
		else if @vl_serveur_pra=XDC_SQLVC
			exec @vl_Status = SQL_DM.PRC..XZAC;16 @va_NumEvt_in,
						@va_CleEvt_in,
						@va_NumeroAction_in,
						@va_Site_in,
						@va_NomIntervention_in,
						@va_TypeIntervention_in,
						@va_HoroLancement_in,
						@va_HoroFin_in,
						@va_Operateur_in,
						@va_HoroSucces_in,
						@va_HoroAnnulation_in,
						@va_Annulation_in,
						@va_Competence_in,
						@va_NumeroAction_out output,
						@va_NomSiteLocal_in,
						XDC_OUI
		/*else if @va_Site_in = XDC_DP*/
		else if @vl_serveur_pra=XDC_SQLDP
			exec @vl_Status = SQL_DS.PRC..XZAC;16 @va_NumEvt_in,
						@va_CleEvt_in,
						@va_NumeroAction_in,
						@va_Site_in,
						@va_NomIntervention_in,
						@va_TypeIntervention_in,
						@va_HoroLancement_in,
						@va_HoroFin_in,
						@va_Operateur_in,
						@va_HoroSucces_in,
						@va_HoroAnnulation_in,
						@va_Annulation_in,
						@va_Competence_in,
						@va_NumeroAction_out output,
						@va_NomSiteLocal_in,
						XDC_OUI
		else return XDC_NOK

		return @vl_Status
	end
	/* pas de traitement distant sur autres sites, tout s'effectue en local */

	/*! transfo en param�tres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_TypeIntervention_in = 0 select @va_TypeIntervention_in = XDC_ACT_INT_AUTRE
	if @va_HoroLancement_in = XDC_DATE_NULLE select @va_HoroLancement_in = null
	if @va_HoroFin_in = XDC_DATE_NULLE select @va_HoroFin_in = null
	if @va_HoroSucces_in = XDC_DATE_NULLE select @va_HoroSucces_in = null
	if @va_NumeroAction_in = 0 select @va_NumeroAction_in = null
	if @va_Site_in = 0 select @va_Site_in = null
	if @va_Operateur_in = 0 select @va_Operateur_in = null
	if @va_HoroAnnulation_in = XDC_DATE_NULLE select @va_HoroAnnulation_in = null
	if @va_Competence_in = 0 select @va_Competence_in = null

	/*A controle des parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null or @va_Operateur_in = null or
	   @va_HoroLancement_in = null
		return XDC_ARG_INV

	/*A verifier l'existance de l'evenement */
	if not exists ( select * from EXP..FMC_GEN 
			where numero = @va_NumEvt_in and cle = @va_CleEvt_in)
		return XDC_NOK

	/*A positionner a " " si chaine null */
	if @va_NomIntervention_in = null
		select @va_NomIntervention_in = XDC_CHAINE_VIDE

	/*A recherche de l'autoroute, sens et PR : derniere horodate de validation */
	select 
		@vl_Autoroute = autoroute,
		@vl_Sens = sens,
		@vl_PR = PR
	from EXP..FMC_HIS
	where   numero = @va_NumEvt_in and cle = @va_CleEvt_in and 
		horodate_validation in ( select max(horodate_validation) 
					from EXP..FMC_HIS
					where numero = @va_NumEvt_in and cle = @va_CleEvt_in )

	if @@rowcount != 1
		return XDC_NOK

	if @va_NumeroAction_in = null
	begin
		/*A recherche le site */
		/* MOVIS */
		if @va_NomSiteLocal_in = null
			select @va_Site_in = numero 	
        		from CFG..RES_DIS
        		where serveur = @@servername
		else
			select @va_Site_in = numero 	
        		from CFG..RES_DIS
			where code = @va_NomSiteLocal_in

        	if @va_Site_in is null
                	return XDC_NOK

begin tran
		/*A recherche du numero d'actio suivant */
		select @va_NumeroAction_out = isnull(max(numero), 0) + 1 
		from EXP..ACT_GEN
		where sit = @va_Site_in

		/*A inserer une actio de type PMV sur fourgon */
		insert EXP..ACT_GEN (numero, evenement, cle, type, heure_lancement,
			heure_succes, heure_echec, prioritaire, heure_lancement_fin,
			heure_fin, operateur, sit, distance_evt, competence, sit_origine)
		values( @va_NumeroAction_out, @va_NumEvt_in, @va_CleEvt_in, 
			XDC_ACT_INTERVENTION, @va_HoroLancement_in,
			@va_HoroSucces_in, null, XDC_NON, null,
			isnull(@va_HoroFin_in,@va_HoroAnnulation_in), @va_Operateur_in, @va_Site_in, null, @va_Competence_in, @va_Site_in)
commit tran
		/*A inserer le complement de l'actio */
		if exists (select * from EXP..ACT_GEN
				where numero = @va_NumeroAction_out and sit = @va_Site_in)
			insert EXP..ACT_INT (actio, nom, type, sit, annulation_intervention)
			values (@va_NumeroAction_out, @va_NomIntervention_in, 
				@va_TypeIntervention_in, @va_Site_in, @va_Annulation_in)
		else	return XDC_NOK
	end
	else
	begin
		/*A tester si le site est correct */
		/* MOVIS */
		if @va_NomSiteLocal_in = null
		begin
			if not exists ( select numero from CFG..RES_DIS
                                where serveur = @@servername and type <> XDC_TYPE_PCNIVEAU3 )
			return XDC_NOK
		end
		else
		begin
			if not exists ( select numero from CFG..RES_DIS
                                where code = @va_NomSiteLocal_in and type <> XDC_TYPE_PCNIVEAU3 )
			return XDC_NOK
		end

		/*A tester si l'actio existe */
		if not exists (select * from EXP..ACT_GEN, EXP..ACT_INT
				where	EXP..ACT_GEN.numero = @va_NumeroAction_in and 
					EXP..ACT_GEN.type = XDC_ACT_INTERVENTION and
					EXP..ACT_GEN.sit = @va_Site_in and
					EXP..ACT_GEN.evenement = @va_NumEvt_in and
					EXP..ACT_GEN.cle = @va_CleEvt_in and
					EXP..ACT_INT.actio = @va_NumeroAction_in and 
					EXP..ACT_INT.sit = @va_Site_in)
			return XDC_NOK

		/*A modification d'une actio */
		update EXP..ACT_GEN 
			set heure_fin =  isnull(@va_HoroFin_in,@va_HoroAnnulation_in), 
			heure_succes = @va_HoroSucces_in,
			heure_lancement = @va_HoroLancement_in,
			operateur = @va_Operateur_in
		where numero = @va_NumeroAction_in and sit = @va_Site_in

		/*A modification du complement d'une actio */
		update EXP..ACT_INT set nom = @va_NomIntervention_in, type = @va_TypeIntervention_in, annulation_intervention = @va_Annulation_in
		where actio = @va_NumeroAction_in and sit = @va_Site_in

		/*A numero d'actio */
		select @va_NumeroAction_out = @va_NumeroAction_in
	end


	return XDC_OK
go
