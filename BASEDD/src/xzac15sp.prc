/*E*/
/*  Fichier : $Id: xzac15sp.prc,v 1.19 2021/03/08 14:37:31 pc2dpdy Exp $      Release : $Revision: 1.19 $        Date : $Date: 2021/03/08 14:37:31 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac15sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	07/09/94	: Creation
* C.T.	13/09/94	: Modif param entree et insertion ds table
* C.T.	28/09/94	: Modif NomDepanneur et resultat en " " si null
* B.G.  17/10/94        : Suppression des appels xzah
* C.T.	10/11/94	: Modif bugs applix (V 1.5)
* C.T.	09/12/94	: Modif clause existance action (V 1.6)
*                         non teste
* P.V.  11/07/96	: Ajout colonnes (V1.6) (DEM76)
* P.N.  13/09/96	: Correction des affectation horodate_xxx -> heure_xxx (V1.9)
* JMG	04/12/96	: ajout site_responsable (DEM/1256) 1.10
* C.P.	07/01/98	: Correction gestion multi-sites dans 
*			  les procs (1.11)
* JMG	03/02/98	: ajout sit_origine (dem/1605) 1.12
* PNI	18/03/09	: Exec de la proc sur le sit de l'action DEM871 1.13
* JMG           02/10/09        : secto DEM887 1/14
* PNI	02/02/15	: Creation de l'action au sein d'une transaction  1.15
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.16
* LCL 	30/06/17	: Modif PRA
* LCL   25/03/20   	: MOVIS Ajout site local DEM-SAE93
* LPE	12/10/20 	: MOVIS DEM-SAE93 1.18
* ABE	06/01/21	: MOVIS ajout PRA DEM-SAE93 1.19
* LCL	27/09/23 : Adaptations CNA DEM-483	
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Ajouter ou modifier l'intervention d'un depanneur
* 
* Sequence d'appel
* SP	XZAC15_Ecrire_Intervention_Depanneur
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* XDY_Entier		va_NumAction_in
* XDY_Octet		va_Site_in
* XDY_Nom		va_NomDepanneur_in
* XDY_Nom		va_Resultat_in
* XY_Horodate		va_HoroAppel_in
* XY_Horodate		va_HoroArrivee_in
* XDY_Operateur		va_Operateur_in
* XY_Horodate		va_HoroDepart_in
* XY_Horodate		va_HoroAnnul_in
* XDY_Octet             va_renfort_in
* XDY_Octet             va_patrouille_presente_in
* XDY_Octet             va_garagiste_contact_pc_in
* XDY_Octet             va_annul_intervention_in
* XDY_Entier            va_code_type_astreinte_in
* XDY_Entier            va_interlocuteur_in
* XDY_Octet		va_site_responsable_in
*
* Arguments en sortie
* XDY_Entier		va_NumAction_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio ou evenement non trouvee ou insertion impossible ou site incorrect
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Si le numero d'actio et le site sont non values en entree 
* alors l'intervention d'un depanneur est ajoutee sinon elle est modifiee
*
* Suite a un ajout de colonnes dans ACT_DEP, les champs renfort, patrouille_presente,
* garagiste_doit_contacter_pc, annulation_intervention n'ont pu etre codes en booleens.
* Des controles sont donc necessaires dans le code appelant cette procedure pour que les
* valeurs de ces champs soient 0 ou 1 uniquement.
*
* Fonction
* Verifier l'existence de l'evenement
* Inserer dans les tables ACTION, INTERVENTION_DEPANNEUR
* Appeler XZAH12_Animer_Objet_Mobile
---------------------------------------------------------*/

use PRC
go

create procedure XZAC;15
	@va_NumEvt_in			   int		= null,
	@va_CleEvt_in			   tinyint	= null,
	@va_NumAction_in		   int		= null,
	@va_Site_in			   T_SITE	= null,
	@va_NomDepanneur_in		   T_NOM	= null,
	@va_Resultat_in			   T_NOM	= null,
	@va_HoroAppel_in		   datetime	= null,
	@va_HoroArrivee_in		   datetime	= null,
	@va_Operateur_in		   T_OPERATEUR	= null,
	@va_HoroDepart_in		   datetime	= null,
	@va_HoroAnnul_in		   datetime	= null,
	@va_renfort_in			   tinyint	= null,
	@va_patrouille_presente_in 	   tinyint	= null,
	@va_garagiste_contact_pc_in 	   tinyint 	= null,
	@va_annul_intervention_in 	   tinyint	= null,
	@va_code_type_astreinte_in	   int		= null,
	@va_interlocuteur_in		   int		= null,
	@va_site_responsable_in		   T_SITE	= null,
	@va_NumAction_out		   int		= null	output,
	@va_NomSiteLocal_in		   char(2)	= null,
	@va_distant_in 			   int 		= XDC_NON
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
		if @vl_serveur_pra = XDC_SQLCI
			exec @vl_Status = SQL_CI.PRC..XZAC;15 @va_NumEvt_in,
						@va_CleEvt_in,
						@va_NumAction_in,
						@va_Site_in,
						@va_NomDepanneur_in,
						@va_Resultat_in,
						@va_HoroAppel_in,
						@va_HoroArrivee_in,
						@va_Operateur_in,
						@va_HoroDepart_in,
						@va_HoroAnnul_in,
						@va_renfort_in,
						@va_patrouille_presente_in,
						@va_garagiste_contact_pc_in,
						@va_annul_intervention_in,
						@va_code_type_astreinte_in,
						@va_interlocuteur_in,
						@va_site_responsable_in,
						@va_NumAction_out output,
						@va_NomSiteLocal_in,
						XDC_OUI
		/*else if @va_Site_in = XDC_VC*/
		else if @vl_serveur_pra = XDC_SQLVC
			exec @vl_Status = SQL_DM.PRC..XZAC;15 @va_NumEvt_in,
						@va_CleEvt_in,
						@va_NumAction_in,
						@va_Site_in,
						@va_NomDepanneur_in,
						@va_Resultat_in,
						@va_HoroAppel_in,
						@va_HoroArrivee_in,
						@va_Operateur_in,
						@va_HoroDepart_in,
						@va_HoroAnnul_in,
						@va_renfort_in,
						@va_patrouille_presente_in,
						@va_garagiste_contact_pc_in,
						@va_annul_intervention_in,
						@va_code_type_astreinte_in,
						@va_interlocuteur_in,
						@va_site_responsable_in,
						@va_NumAction_out output,
						@va_NomSiteLocal_in,
						XDC_OUI
		/*else if @va_Site_in = XDC_DP*/
		else if @vl_serveur_pra = XDC_SQLDP
			exec @vl_Status = SQL_DS.PRC..XZAC;15 @va_NumEvt_in,
						@va_CleEvt_in,
						@va_NumAction_in,
						@va_Site_in,
						@va_NomDepanneur_in,
						@va_Resultat_in,
						@va_HoroAppel_in,
						@va_HoroArrivee_in,
						@va_Operateur_in,
						@va_HoroDepart_in,
						@va_HoroAnnul_in,
						@va_renfort_in,
						@va_patrouille_presente_in,
						@va_garagiste_contact_pc_in,
						@va_annul_intervention_in,
						@va_code_type_astreinte_in,
						@va_interlocuteur_in,
						@va_site_responsable_in,
						@va_NumAction_out output,
						@va_NomSiteLocal_in,
						XDC_OUI
		else return XDC_NOK

		return @vl_Status
	end
	/* pas de traitement distant sur autres sites, tout s'effectue en local */

	/*! transfo en param�tres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_HoroAppel_in = XDC_DATE_NULLE select @va_HoroAppel_in = null
	if @va_HoroArrivee_in = XDC_DATE_NULLE select @va_HoroArrivee_in = null
	if @va_HoroDepart_in = XDC_DATE_NULLE select @va_HoroDepart_in = null
	if @va_HoroAnnul_in = XDC_DATE_NULLE select @va_HoroAnnul_in = null
	if @va_NumAction_in = 0 select @va_NumAction_in = null
	if @va_Site_in = 0 select @va_Site_in = null
	if @va_Operateur_in = 0 select @va_Operateur_in = null
	if @va_code_type_astreinte_in = 0 select @va_code_type_astreinte_in = null
	if @va_interlocuteur_in = 0 select @va_interlocuteur_in = null
	if @va_site_responsable_in = 0 select @va_site_responsable_in = null

	/*A controle des parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null or @va_Operateur_in = null or
	   @va_HoroAppel_in = null
		return XDC_ARG_INV

	/*A verifier l'existance de l'evenement */
	if not exists ( select * from EXP..FMC_GEN 
			where numero = @va_NumEvt_in and cle = @va_CleEvt_in)
		return XDC_NOK

	/*A positionner a " " si chaine null */
	if @va_NomDepanneur_in = null
		select @va_NomDepanneur_in = XDC_CHAINE_VIDE
	if @va_Resultat_in = null
		select @va_Resultat_in = XDC_CHAINE_VIDE

	/*A recherche de l'autoroute, sens et PR : derniere horodate de validation */
	select 
		@vl_Autoroute = autoroute,
		@vl_Sens = sens,
		@vl_PR = PR
	from EXP..FMC_HIS
	where   numero = @va_NumEvt_in and cle = @va_CleEvt_in and 
		horodate_validation in ( select max(horodate_validation) from EXP..FMC_HIS
					where numero = @va_NumEvt_in and cle = @va_CleEvt_in )

	if @@rowcount != 1
		return XDC_NOK

	if @va_NumAction_in = null
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
		select @va_NumAction_out = isnull(max(numero), 0) + 1 from EXP..ACT_GEN
		where sit = @va_Site_in

		/*A inserer une actio de type PMV sur fourgon */
		insert EXP..ACT_GEN (numero, evenement, cle, type, heure_lancement,
			heure_succes, heure_echec, prioritaire, heure_lancement_fin,
			heure_fin, operateur, sit, distance_evt, sit_origine)
		values( @va_NumAction_out, @va_NumEvt_in, @va_CleEvt_in, 
			XDC_ACT_DEPANNEUR, @va_HoroAppel_in,
			@va_HoroArrivee_in, null , XDC_NON, null,
			isnull(@va_HoroDepart_in,@va_HoroAnnul_in), @va_Operateur_in, @va_Site_in, null, @va_Site_in)
commit tran
		/*A inserer le complement de l'actio */
		if exists (select * from EXP..ACT_GEN
				where numero = @va_NumAction_out and sit = @va_Site_in)
			insert EXP..ACT_DEP (actio, nom, resultat, sit, renfort, patrouille_presente,
				garagiste_doit_contacter_pc, annulation_intervention, code_type_astreinte,
				interlocuteur,site_responsable)
			values (@va_NumAction_out, @va_NomDepanneur_in, @va_Resultat_in, @va_Site_in,
				@va_renfort_in, @va_patrouille_presente_in, @va_garagiste_contact_pc_in,
				@va_annul_intervention_in, @va_code_type_astreinte_in, 
				@va_interlocuteur_in, @va_site_responsable_in)
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
		if not exists (select * from EXP..ACT_GEN, EXP..ACT_DEP
				where	EXP..ACT_GEN.numero = @va_NumAction_in and 
					EXP..ACT_GEN.type = XDC_ACT_DEPANNEUR and
					EXP..ACT_GEN.sit = @va_Site_in and
					EXP..ACT_GEN.evenement = @va_NumEvt_in and
					EXP..ACT_GEN.cle = @va_CleEvt_in and
					EXP..ACT_DEP.actio = @va_NumAction_in and 
					EXP..ACT_DEP.sit = @va_Site_in)
			return XDC_NOK

		/*A modification d'une actio */
		update EXP..ACT_GEN 
			set heure_fin =  isnull(@va_HoroDepart_in,@va_HoroAnnul_in), 
			heure_lancement = @va_HoroAppel_in,
			heure_succes = @va_HoroArrivee_in,
			operateur = @va_Operateur_in,
			heure_echec = @va_HoroAnnul_in
		where numero = @va_NumAction_in and sit = @va_Site_in

		/*A modification du complement d'une actio */
		update EXP..ACT_DEP set 
					nom = @va_NomDepanneur_in,
					resultat = @va_Resultat_in,
					renfort = @va_renfort_in,
					patrouille_presente = @va_patrouille_presente_in,
					garagiste_doit_contacter_pc = @va_garagiste_contact_pc_in,
					annulation_intervention = @va_annul_intervention_in,
					code_type_astreinte = @va_code_type_astreinte_in,
					interlocuteur = @va_interlocuteur_in,
					site_responsable = @va_site_responsable_in
		where actio = @va_NumAction_in and sit = @va_Site_in

		/*A numero d'actio */
		select @va_NumAction_out = @va_NumAction_in
	end


	return XDC_OK
go
