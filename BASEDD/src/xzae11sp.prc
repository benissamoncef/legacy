
/*E*/
/*X Fichier : $Id: xzae11sp.prc,v 1.65 2020/11/27 09:07:02 pc2dpdy Exp $        $Revision: 1.65 $        $Date: 2020/11/27 09:07:02 $
------------------------------------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------------------------------------
* FICHIER xzae11sp.prc
------------------------------------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* valide une FMC
*
------------------------------------------------------------------------------------
* HISTORIQUE :
*
* C.T.     	18/10/94	: Creation 	V 1.1
* B.G.          09/11/94        : modif null pour applix (1.2)
* C.T.          23/11/94        : correction bugs voies mise en base
*                                 non teste (1.4)
* B.G.          04/12/94        : ajout dans la liste a traiter pour le district de l'evenement
*                                 ( cas d'une saisie au CI ) (1.5)
* B.G.          19/12/94        : ARG_INV si type=poste operateur (1.6)
* B.G.          27/01/95        : modif reveil (1.7)
* C.T.          14/02/95        : modif reveil sur fin evt (1.8)
* C.T.          20/02/95        : ajout update de zz (1.9)
*                                 et ajout controle de date de debut
*                                 ou debut prevu
* C.T.		14/03/95	: Modif longueur texte reveil (V 1.10)
* C.T.		14/03/95	: Ajout de la qualification de l'alerte
*                                 quand elle est a confirmer (V 1.11)
* C.T.		24/05/95	: Pas d'update de la cause pour une
*                                 queue de bouchon (V 1.12)
* C.T.		14/06/95	: Correction erreur de compile (v1.13)
* C.T.		14/06/95	: La qualification est forcee a XDC_ALT_VRAIE_EVT (v1.14)
* C.T.		14/06/95	: Correction erreur de compile (v1.15)
* C.T.		12/07/95	: Modif clause d'update sur FMC_HIS dans
*                                 le cas de fmc en prevision et close (v1.16)
*                                 (sit supprimer de la clause)
* C.T.		20/09/95	: Suppression des reveils au poste voisin (V 1.17)
* C.T.		20/11/95	: La qualification est forcee a XDC_ALT_VRAIE_EVT si elle n'est pas deja positionnee (v1.18)
* P.V.		22/08/96	: Modif delai : 24h -> 10h (V1.19) (ANA32)
* JMG+CT	27/08/96	: Modif update FMC close (maj site pour replication) DEM/1172 (V.20)
* CT		03/09/96	: Ajout cas d'une validation sur PC simplifie (V 1.21)
* CT		23/09/96	: Retour XZAEC_FMC_PAS_ENR au lieu de XDC_NOK si fmc pas sur site (V1.22) (CONF 4)
* CT		25/09/96	: Ajout dans l'interface degats domaine (FMC10) (V1.23)
* CT		04/10/96	: Ajout dans l'interface type anterieur (FMC3) (V1.24)
* JMG		24/10/96	: virer le select de debug/trace	1.25
* CT		04/11/96	: Ajout dans l'interface fausse_alerte (FMC18) (V1.26)
* CT		14/11/96	: Ajout dans l'interface BAU etroite (SAD4) (V1.27)
* CT		11/12/96	: Ajout dans l'interface longueur (FMC7) (V1.28)
* JMG		23/12/96	: correction mise a jour de la liste sur poste voisin (DEM/1360) 1.29
* JMG		13/11/97	: plus erreur si local sur echangeur et PR entree != PR sortie (dem/1481) 1.30
* JMG		18/11/97	: modif correction precedente 1.31
* CT		09/12/97	: gestion fmc de type degrade (1531) 1.32
* CT		08/01/98	: ajout param sit_alerte (1532) 1.33
* CT		27/02/98	: ajout commentaire fmc initiale et suppression FMC initiale (1583) 1.34
* CT		17/04/98	: Correction update FMC_HIS dans etat clos : suppresion du sit (1612) 1.35
* PN		29/09/98	: S�paration EXP/HIS 1.38
* PN            10/11/98	: correction appel proc dans HIS 1.39
* PN            08/03/05	: Ajout type fmc � l'appel xzae66 dem439 1.40
* JPL           02/04/07	: Gestion changement classe fmc Accident <-> Vehicule (Dem/637) 1.41
* LCL           09/07/07        : Suppression DY 1.42
* JPL		26/10/07	: Effectue de nouveau le changement de type sans appeler xzae209 (DEM 720) 1.43
* JMG		27/03/08	: correction maj liste sur poste voisin si 3 PO 1.44 DEM768
* JPL		09/04/08	: Conserver la derniere horodate de changement de type (DEM 771) 1.45
* JPL		01/09/08	: Texte de reveil marque urgent pour les Fmc Accident et Contresens (DEM 721) 1.46
* JPL		16/02/09	: Appels a XZAE18 degroupee (lie a DEM 848) 1.47
* JPL		16/02/09	: Tete bouchon: reveil sur la queue, et uniquement si sur autre site (lie a DEM 867) 1.48
* JPL		16/04/09	: Appel a XZAE66 degroupee (lie a DEM 878) 1.49
* JMG           02/10/09        : secto DEM887 1.50
* JMG		14/04/12 : regulation DEM 1016 1.51
* PNI		26/08/16	: scission en 2 du commit tran DEM1179 1.52
* JMG		26/11/16	 : ajout majeur lot 23 1.53
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.55
* JMG	24/05/18 : boucho,n multi auto -> autoriser modif cause queue bouchon 1.56
* JMG	13/06/18 : pas d update alerte si num alerte = -1 (GABEB) 1.57
* JMG	28/08/18 : si date debut nulle en entree etdate prevue nulle, prendre date debut a 
*		  date debut precedente de la FMC (GABEB) 1.58
* PNI   20/03/19 : correctif + reveil NOUEVLLE FMC sur site CI (et non poste par poste) 1.60  DEM1306
* JPL	02/04/19 : Ajout de l'indicateur d'�coulement (DEM 1326)  1.61
* LCL 	11/03/20 : MOVIS DEM-SAE93 1.62
* ABE 	27/04/20 : DATEX 2 DAE-130 + LCL correction MOVIS DEM-SAE93
* ABE	10/09/20 : DATEX 2 DEM-SAE130 Ajout TASRV_VC et DP V1.63
* ABE	24/09/20 : DATEX 2 DEM-SAE130 Revert de la version pr�cedente V1.64
* ABE	27/11/20 : Remise partiel de la version 1.63 pour publication sur ajout FMC au CIT DEM-SAE130 V1.65		
* GGY	09/03/23 : Suppression de l'appel à AARPDATEX2 (deplace dans le trigger fmc_gen_iu_trg)	(DEM-SAE438)			
* LCL	23/03/23 : Ajout traitement alertes Cockpit Bouchons V1.66		
* ABK	26/09/23 : Ajout district CA (DEM-483)
----------------------------------------------------------------------------------*/

/*X*/
/*----------------------------------------------------------------------------------
* Sequence d'appel
* SP	XZAE11_Valider_Fiche_MC
* 
* Arguments en entree
* XDY_Entier	va_NumEvenement_in
* XDY_Octet	va_CleEvenement_in
* XDY_Machine	va_PosteOperateur_in		: numero de la machine de l'operateur
* XDY_Octet	va_Prevision_Traiter_Clos_in	: etat de l'evenement
* XDY_Mot	va_Type_in
* XDY_Horodate	va_DebutPrevu_in
* XDY_Horodate	va_Debut_in
* XDY_Horodate	va_FinPrevu_in
* XDY_Horodate	va_Fin_in
* XDY_Horodate	va_Validation_in
* XDY_Entier	va_NumCause_in
* XDY_Octet	va_CleCause_in
* XDY_Alerte	va_NumAlerte_in
* XDY_Nom	va_Origine_in
* XDY_Eqt	va_PosteEnrich_in		: numero de la machine de l'enrichisseur
* XDY_Booleen	va_ConfirmeSignale_in
* XDY_Autoroute	va_Autoroute_in
* XDY_PR	va_PR_in
* XDY_Sens	va_Sens_in
* XDY_Octet	va_PtCaracteristique_in		: numero du point caracteristique
* XDY_Octet	va_TypePtCaracteristique_in	: type du point caracteristique
* XDY_Octet	va_Position_in			: position sur aire ou echangeur ou lit
* XDY_Voie	va_VR_in
* XDY_Voie	va_VM2_in
* XDY_Voie	va_VM1_in
* XDY_Voie	va_VL_in
* XDY_Voie	va_BAU_in
* XDY_Voie	va_VR_I_in
* XDY_Voie	va_VM2_I_in
* XDY_Voie	va_VM1_I_in
* XDY_Voie	va_VL_I_in
* XDY_Voie	va_BAU_I_in
* XDY_Voie	va_BAU_Etroite_in
* XDY_Voie	va_BAU_I_Etroite_in
* XDY_Octet	va_Degats_in
* XDY_Mot	va_TypeAnterieur_in
* XDY_Octet	va_FausseAlerte_in
* XDY_Entier	va_Longueur_in
* XDY_Entier	va_ComEvInit_in
* XDY_Octet	va_SiteAlerte_in
* XDY_Octet	va_Majeur_in
* XDY_Octet	va_Ecoulement_in
* 
* Arguments en sortie
* 
* Service rendu
* Met en base la fiche correspondante 
* (doit �tre appele apres Ecrire_Fiche_xxx)
*

* Code retour
* XDC_OK
* XDC_NOK		: alerte inconnue, cause de l'evenement inconnue, insertion 
*                         impossible, declencher d'un reveil impossible
* XDC_ARG_INV		: argument d'entree null, ou valeur incorrecte
* XDC_PRC_INC		: procedure stockee n'existe pas en base
* XZAEC_FMC_PAS_ENR	: pas le droit d'enrichir
* XZAEC_FMC_ETAT_INC	: l'evenement n'est pas dans l'etat precise en entree
* XZAEC_FMC_DEB_PRV_INC	: date de debut des travaux en prevision doit etre superieur
*                         a la date courante + 10h
* XZAEC_FMC_VOIE_INC	: la description des voies est incorrecte
* XZAEC_FMC_LOC_INC	: la localisation de l'evenement est incorrecte
* XZAEC_FMC_PAS_RETYPAGE : retypage non autorise
* XZAEC_FMC_INC_RETYPAGE : nouveau type pas de meme classe que l'actuel
* < 0          		: erreur sybase
* 
* Conditions d'utilisation
* 
* Fonction
* Controler si le poste operateur a le droit d'enrichir la fiche MC.
* Dans le cas de fiche "a traiter", tester si le poste 
* d'enrichissement est le poste operateur passe en parametre 
* dans le cas ou le poste d'enrichissement est value.
* Update la table EVENEMENT 
* Suivant le cas de fiche :
* 	"A traiter" :	Insert dans la table EVENEMENT_HISTORIQUE
* 	"Prevision" :	Update ou insert dans la table EVENEMENT_HISTORIQUE
* 	"Clos" :	Update dans la table EVENEMENT_HISTORIQUE
* Envoyer un reveil aux operateurs qui traitent l'evenement avec XZAR03
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE11' and type = 'P')
	drop procedure XZAE11
go


create procedure XZAE11
	@va_NumEvenement_in		int	= null,
	@va_CleEvenement_in		tinyint	= null,
	@va_PosteOperateur_in		smallint = null,
	@va_Prevision_Traiter_Clos_in	tinyint	= null,
	@va_Type_in			smallint= null,
	@va_DebutPrevu_in		datetime = null,
	@va_Debut_in			datetime = null,
	@va_FinPrevu_in			datetime = null,
	@va_Fin_in			datetime = null,
	@va_Validation_in		datetime = null,
	@va_NumCause_in			int	= null,
	@va_CleCause_in			tinyint	= null,
	@va_NumAlerte_in		int	= null,
	@va_Origine_in			T_NOM	= null,
	@va_PosteEnrich_in		smallint = null,
	@va_ConfirmeSignale_in		bit,
	@va_NumAutoroute_in		T_AUTOROUTE = null,
	@va_PR_in			T_PR	= null,
	@va_Sens_in			T_SENS	= null,
	@va_PtCaracteristique_in	tinyint	= null,
	@va_TypePtCaracteristique_in	tinyint = null,
	@va_Position_in			tinyint = null,
	@va_VR_in			tinyint	= null,
	@va_VM2_in			tinyint	= null,
	@va_VM1_in			tinyint	= null,
	@va_VL_in			tinyint	= null,
	@va_BAU_in			tinyint	= null,
	@va_VR_I_in			tinyint	= null,
	@va_VM2_I_in			tinyint	= null,
	@va_VM1_I_in			tinyint	= null,
	@va_VL_I_in			tinyint	= null,
	@va_BAU_I_in			tinyint	= null,
	@va_BAU_Etroite_in		tinyint	= null,
	@va_BAU_I_Etroite_in		tinyint	= null,
	@va_Degats_in			tinyint	= null,
	@va_TypeAnterieur_in		smallint= null,
	@va_FausseAlerte_in		tinyint = null,
	@va_Longueur_in			int	= null,
	@va_ComEvtInit_in		T_NOM	= null,
	@va_SiteAlerte_in		tinyint	= null,
	@va_Majeur_in			tinyint = null,
	@va_Ecoulement_in		tinyint = null,
	@va_NomSiteLocal_in             char(2) = null
as
	declare @vl_PosteEnrich smallint, @vl_Debut datetime, 
		@vl_Cloture datetime, @vl_Site T_SITE, @vl_SiteCI T_SITE,
		@vl_DateCourante datetime, @vl_Status int, 
		@vl_PtCaracteristique tinyint, @vl_TypePtCaracteristique tinyint,
		@vl_Autoroute T_AUTOROUTE, @vl_PR T_PR, @vl_Sens T_SENS,
		@vl_Position tinyint, @vl_VR T_VOIE, @vl_VM2 T_VOIE, 
		@vl_VM1 T_VOIE, @vl_VL T_VOIE, @vl_BAU T_VOIE, 
		@vl_VR_I T_VOIE, @vl_VM2_I T_VOIE, @vl_VM1_I T_VOIE, 
		@vl_VL_I T_VOIE, @vl_BAU_I T_VOIE, @vl_Erreur bit,
		@vl_ResultAjoutReveil bit, @vl_TexteReveil char(100),
		@vl_TypeReveil tinyint, @vl_PosteOperateur smallint,
		@vl_ResultConfig int, @vl_District T_DISTRICT,
		@vl_DistrictRecouvrement T_DISTRICT,
		@vl_Cause int, @vl_CleCause tinyint, @vl_Qualification tinyint, @vl_TypeAlerte tinyint,
		@vl_Type smallint, @vl_Degats tinyint, @vl_HoroChgtType datetime,
		@vl_Classe tinyint, @vl_ClasseAnterieure tinyint,
		@vl_PosteVoisin smallint, @vl_SiteFMC T_SITE,
		@vl_NumQueueBouchon int, @vl_CleQueueBouchon tinyint, @vl_SiteQueueBouchon T_SITE,
		@vl_NumFmcReveillee int, @vl_CleFmcReveillee tinyint,
		@vl_NumAlerte int, @vl_SiteAlerte tinyint,
		@vl_pitlog_id int, @vl_pitlog_reference varchar(50),@vl_pitlog_correlation_id varchar(50)

	/* Transformation des param�tres d'entr�e nulls */
	
	if @va_NumEvenement_in = 0 select @va_NumEvenement_in = null
	if @va_CleEvenement_in = 0 select @va_CleEvenement_in = null
	if @va_PosteOperateur_in = 0 select @va_PosteOperateur_in = null
	if @va_Prevision_Traiter_Clos_in = 0 select @va_Prevision_Traiter_Clos_in = null
	if @va_Type_in = 0 select @va_Type_in = null
	if @va_DebutPrevu_in = XDC_DATE_NULLE select @va_DebutPrevu_in = null
	if @va_Debut_in = XDC_DATE_NULLE select @va_Debut_in = null
	if @va_FinPrevu_in = XDC_DATE_NULLE select @va_FinPrevu_in = null
	if @va_Fin_in = XDC_DATE_NULLE select @va_Fin_in = null
	if @va_Validation_in = XDC_DATE_NULLE select @va_Validation_in = null
	if @va_NumCause_in = 0 select @va_NumCause_in = null
	if @va_CleCause_in = 0 select @va_CleCause_in = null
	if @va_NumAlerte_in = 0 select @va_NumAlerte_in = null
	if @va_SiteAlerte_in = 0 select @va_SiteAlerte_in = null
	if @va_PosteEnrich_in = 0 select @va_PosteEnrich_in = null
	if @va_PtCaracteristique_in = 0 select @va_PtCaracteristique_in = null
	if @va_TypePtCaracteristique_in = 0 select @va_TypePtCaracteristique_in = null
	if @va_Position_in = 0 select @va_Position_in = null
	if @va_TypeAnterieur_in = 0 select @va_TypeAnterieur_in = @va_Type_in
	if @va_FausseAlerte_in = null select @va_FausseAlerte_in = XDC_FAUX

	/* initialisation */
	select @vl_Erreur = XDC_NON

	 /****************************************************************************
	 **                                                                         **
	 **                    Prerequis : Controles de validite                    **
	 **                                                                         **
	 ****************************************************************************/

	/*A
	 * Controle des parametres obligatoires
	 * ------------------------------------
	 */

	if @va_NumEvenement_in = null or @va_CleEvenement_in = null or
	   @va_Validation_in = null or @va_Prevision_Traiter_Clos_in = null or
	/*   (@va_Debut_in = null and @va_DebutPrevu_in = null) or*/
	   (@va_NumCause_in = null and @va_CleCause_in != null) or
	   (@va_NumCause_in != null and @va_CleCause_in = null)
		return XDC_ARG_INV


	/*A
	 * Rechercher le site local
	 * ------------------------
	 */
        /* MOVIS */
        if @va_NomSiteLocal_in = null
                select  @vl_Site = numero, @va_NomSiteLocal_in = code
                from CFG..RES_DIS
                where serveur = @@servername
        else
                select  @vl_Site = numero
                from CFG..RES_DIS
                where code = @va_NomSiteLocal_in

	if @vl_Site is null
		return XDC_NOK


	/*A
	 * Verifier l'existence de l'evenement sur le site local
	 * -----------------------------------------------------
	 */



#ifdef CI
	select 
		@vl_PosteEnrich = poste_enrichisseur,
		@vl_Debut = debut,
		@vl_Cloture = cloture,
		@vl_Cause = cause,
		@vl_CleCause = cle_cause,
		@vl_Degats = degats_domaine,
		@vl_Type = type,
		@vl_HoroChgtType = horodate_chgt_type,
		@vl_NumAlerte = alerte,
		@vl_SiteAlerte = sit_alerte
	from EXP..FMC_GEN
	where numero = @va_NumEvenement_in and cle = @va_CleEvenement_in and
		type = @va_TypeAnterieur_in and sit = @vl_Site

	if @@rowcount != 1
		if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_CLOS
		begin
			exec @vl_Status = SQL_HIS.PRC..XZAE11
				@va_NumEvenement_in		,
				@va_CleEvenement_in		,
				@va_PosteOperateur_in		,
				@va_Prevision_Traiter_Clos_in	,
				@va_Type_in			,
				@va_DebutPrevu_in		,
				@va_Debut_in			,
				@va_FinPrevu_in			,
				@va_Fin_in			,
				@va_Validation_in		,
				@va_NumCause_in			,
				@va_CleCause_in			,
				@va_NumAlerte_in		,
				@va_Origine_in			,
				@va_PosteEnrich_in		,
				@va_ConfirmeSignale_in		,
				@va_NumAutoroute_in		,
				@va_PR_in			,
				@va_Sens_in			,
				@va_PtCaracteristique_in	,
				@va_TypePtCaracteristique_in	,
				@va_Position_in			,
				@va_VR_in			,
				@va_VM2_in			,
				@va_VM1_in			,
				@va_VL_in			,
				@va_BAU_in			,
				@va_VR_I_in			,
				@va_VM2_I_in			,
				@va_VM1_I_in			,
				@va_VL_I_in			,
				@va_BAU_I_in			,
				@va_BAU_Etroite_in		,
				@va_BAU_I_Etroite_in		,
				@va_Degats_in			,
				@va_TypeAnterieur_in		,
				@va_FausseAlerte_in		,
				@va_Longueur_in			,
				@va_ComEvtInit_in		,
				@va_SiteAlerte_in		,
				@va_Majeur_in			,
				@va_Ecoulement_in

			if @vl_Status = null
							return XDC_PRC_INC
					else return @vl_Status
			
		end
		else
		begin
			/* on trace dans PIT_LOG */
			/* Recherche de la reference PIT a partir de la fmc */
			select @vl_pitlog_reference=reference
			from EXP..FMC_REF_EXT
			where numero=@va_NumEvenement_in and 
				  cle=@va_CleEvenement_in
			if @@rowcount>0
			begin
				/* recherche du dernier correlation_id recu pour cette fmc */
				set rowcount 1
				select @vl_pitlog_correlation_id=correlation_id
				from EXP..PIT_LOG
				where reference=@vl_pitlog_reference
				order by horodate desc
				set rowcount 0
				/* recherche de l'id a creer */
				select @vl_pitlog_id=max(id)+1
				from EXP..PIT_LOG
				/* insertion trace */
				insert into EXP..PIT_LOG
				values (@vl_pitlog_id,getdate(),@vl_pitlog_reference,@vl_pitlog_correlation_id,"ERROR",1,"Erreur droits XZAE11 cas 1 :"+
																							" va_NumEvenement_in ["+convert(varchar(6),@va_NumEvenement_in)+"] "+
																							" va_CleEvenement_in ["+convert(varchar(6),@va_CleEvenement_in)+"] "+
																							" va_PosteOperateur_in ["+convert(varchar(6),@va_PosteOperateur_in)+"] "+
																							" va_Prevision_Traiter_Clos_in ["+convert(varchar(6),@va_Prevision_Traiter_Clos_in)+"] "+
																							" va_Type_in ["+convert(varchar(6),@va_Type_in)+"] "+
																							" va_DebutPrevu_in ["+convert(varchar(12),@va_DebutPrevu_in)+"] "+
																							" va_Debut_in ["+convert(varchar(12),@va_Debut_in)+"] "+
																							" va_FinPrevu_in ["+convert(varchar(12),@va_FinPrevu_in	)+"] "+
																							" va_Fin_in ["+convert(varchar(12),@va_Fin_in)+"] "+
																							" va_Validation_in ["+convert(varchar(12),@va_Validation_in)+"] "+
																							" va_NumCause_in ["+convert(varchar(6),@va_NumCause_in)+"] "+
																							" va_CleCause_in ["+convert(varchar(6),@va_CleCause_in)+"] "+
																							" va_NumAlerte_in ["+convert(varchar(6),@va_NumAlerte_in)+"] "+
																							" va_Origine_in ["+@va_Origine_in+"] "+
																							" va_PosteEnrich_in ["+convert(varchar(6),@va_PosteEnrich_in)+"] "+
																							" va_ConfirmeSignale_in ["+convert(varchar(6),@va_ConfirmeSignale_in)+"] "+
																							" va_NumAutoroute_in ["+convert(varchar(6),@va_NumAutoroute_in)+"] "+
																							" va_PR_in ["+convert(varchar(6),@va_PR_in)+"] "+
																							" va_Sens_in ["+convert(varchar(6),@va_Sens_in)+"] "+
																							" va_PtCaracteristique_in ["+convert(varchar(6),@va_PtCaracteristique_in)+"] "+
																							" va_TypePtCaracteristique_in ["+convert(varchar(6),@va_TypePtCaracteristique_in)+"] "+
																							" va_Position_in ["+convert(varchar(6),@va_Position_in)+"] "+
																							" va_VR_in ["+convert(varchar(6),@va_VR_in)+"] "+
																							" va_VM2_in ["+convert(varchar(6),@va_VM2_in)+"] "+
																							" va_VM1_in ["+convert(varchar(6),@va_VM1_in)+"] "+
																							" va_VL_in ["+convert(varchar(6),@va_VL_in)+"] "+
																							" va_BAU_in ["+convert(varchar(6),@va_BAU_in)+"] "+
																							" va_VR_I_in ["+convert(varchar(6),@va_VR_I_in)+"] "+
																							" va_VM2_I_in ["+convert(varchar(6),@va_VM2_I_in)+"] "+
																							" va_VM1_I_in ["+convert(varchar(6),@va_VM1_I_in)+"] "+
																							" va_VL_I_in ["+convert(varchar(6),@va_VL_I_in)+"] "+
																							" va_BAU_I_in ["+convert(varchar(6),@va_BAU_I_in)+"] "+
																							" va_BAU_Etroite_in ["+convert(varchar(6),@va_BAU_Etroite_in)+"] "+
																							" va_BAU_I_Etroite_in ["+convert(varchar(6),@va_BAU_I_Etroite_in)+"] "+
																							" va_Degats_in ["+convert(varchar(6),@va_Degats_in)+"] "+
																							" va_TypeAnterieur_in ["+convert(varchar(6),@va_TypeAnterieur_in)+"] "+
																							" va_FausseAlerte_in ["+convert(varchar(6),@va_FausseAlerte_in)+"] "+
																							" va_Longueur_in ["+convert(varchar(6),@va_Longueur_in)+"] "+
																							" va_ComEvtInit_in ["+convert(varchar(6),@va_ComEvtInit_in)+"] "+
																							" va_SiteAlerte_in ["+convert(varchar(6),@va_SiteAlerte_in)+"] "+
																							" va_Majeur_in ["+convert(varchar(6),@va_Majeur_in)+"] "+
																							" va_Ecoulement_in ["+convert(varchar(6),@va_Ecoulement_in)+"] "+
																							" va_NomSiteLocal_in ["+@va_NomSiteLocal_in+"]"
				)
			end

			return XZAEC_FMC_PAS_ENR
		end

        if @va_Origine_in = "DETECTION BOUCHON"
	        select @va_Debut_in = @vl_Debut
#else
#ifdef HIST
			select 
				@vl_PosteEnrich = poste_enrichisseur,
				@vl_Debut = debut,
				@vl_Cloture = cloture,
				@vl_Cause = cause,
				@vl_CleCause = cle_cause,
				@vl_Degats = degats_domaine,
				@vl_Type = type,
				@vl_HoroChgtType = horodate_chgt_type,
				@vl_NumAlerte = alerte,
		                @vl_SiteAlerte = sit_alerte
			from HIS..FMC_GEN
			where numero = @va_NumEvenement_in and cle = @va_CleEvenement_in and 
				type = @va_TypeAnterieur_in

			if @@rowcount = 0
			begin
				return XDC_NOK
			end
#else
	select 
		@vl_PosteEnrich = poste_enrichisseur,
		@vl_Debut = debut,
		@vl_Cloture = cloture,
		@vl_Cause = cause,
		@vl_CleCause = cle_cause,
		@vl_Degats = degats_domaine,
		@vl_Type = type,
		@vl_HoroChgtType = horodate_chgt_type,
		@vl_NumAlerte = alerte,
	         @vl_SiteAlerte = sit_alerte
		from EXP..FMC_GEN
	where numero = @va_NumEvenement_in and cle = @va_CleEvenement_in and
		type = @va_TypeAnterieur_in and sit = @vl_Site

	if @@rowcount != 1
	begin
		/* on trace dans PIT_LOG */
		/* Recherche de la reference PIT a partir de la fmc */
		select @vl_pitlog_reference=reference
		from EXP..FMC_REF_EXT
		where numero=@va_NumEvenement_in and 
				cle=@va_CleEvenement_in
		if @@rowcount>0
		begin
			/* recherche du dernier correlation_id recu pour cette fmc */
			set rowcount 1
			select @vl_pitlog_correlation_id=correlation_id
			from EXP..PIT_LOG
			where reference=@vl_pitlog_reference
			order by horodate desc
			set rowcount 0
			/* recherche de l'id a creer */
			select @vl_pitlog_id=max(id)+1
			from EXP..PIT_LOG
			/* insertion trace */
			insert into EXP..PIT_LOG
			values (@vl_pitlog_id,getdate(),@vl_pitlog_reference,@vl_pitlog_correlation_id,"ERROR",1,"Erreur droits XZAE11 cas 2 :"+
																						" va_NumEvenement_in ["+convert(varchar(6),@va_NumEvenement_in)+"] "+
																						" va_CleEvenement_in ["+convert(varchar(6),@va_CleEvenement_in)+"] "+
																						" va_PosteOperateur_in ["+convert(varchar(6),@va_PosteOperateur_in)+"] "+
																						" va_Prevision_Traiter_Clos_in ["+convert(varchar(6),@va_Prevision_Traiter_Clos_in)+"] "+
																						" va_Type_in ["+convert(varchar(6),@va_Type_in)+"] "+
																						" va_DebutPrevu_in ["+convert(varchar(12),@va_DebutPrevu_in)+"] "+
																						" va_Debut_in ["+convert(varchar(12),@va_Debut_in)+"] "+
																						" va_FinPrevu_in ["+convert(varchar(12),@va_FinPrevu_in	)+"] "+
																						" va_Fin_in ["+convert(varchar(12),@va_Fin_in)+"] "+
																						" va_Validation_in ["+convert(varchar(12),@va_Validation_in)+"] "+
																						" va_NumCause_in ["+convert(varchar(6),@va_NumCause_in)+"] "+
																						" va_CleCause_in ["+convert(varchar(6),@va_CleCause_in)+"] "+
																						" va_NumAlerte_in ["+convert(varchar(6),@va_NumAlerte_in)+"] "+
																						" va_Origine_in ["+@va_Origine_in+"] "+
																						" va_PosteEnrich_in ["+convert(varchar(6),@va_PosteEnrich_in)+"] "+
																						" va_ConfirmeSignale_in ["+convert(varchar(6),@va_ConfirmeSignale_in)+"] "+
																						" va_NumAutoroute_in ["+convert(varchar(6),@va_NumAutoroute_in)+"] "+
																						" va_PR_in ["+convert(varchar(6),@va_PR_in)+"] "+
																						" va_Sens_in ["+convert(varchar(6),@va_Sens_in)+"] "+
																						" va_PtCaracteristique_in ["+convert(varchar(6),@va_PtCaracteristique_in)+"] "+
																						" va_TypePtCaracteristique_in ["+convert(varchar(6),@va_TypePtCaracteristique_in)+"] "+
																						" va_Position_in ["+convert(varchar(6),@va_Position_in)+"] "+
																						" va_VR_in ["+convert(varchar(6),@va_VR_in)+"] "+
																						" va_VM2_in ["+convert(varchar(6),@va_VM2_in)+"] "+
																						" va_VM1_in ["+convert(varchar(6),@va_VM1_in)+"] "+
																						" va_VL_in ["+convert(varchar(6),@va_VL_in)+"] "+
																						" va_BAU_in ["+convert(varchar(6),@va_BAU_in)+"] "+
																						" va_VR_I_in ["+convert(varchar(6),@va_VR_I_in)+"] "+
																						" va_VM2_I_in ["+convert(varchar(6),@va_VM2_I_in)+"] "+
																						" va_VM1_I_in ["+convert(varchar(6),@va_VM1_I_in)+"] "+
																						" va_VL_I_in ["+convert(varchar(6),@va_VL_I_in)+"] "+
																						" va_BAU_I_in ["+convert(varchar(6),@va_BAU_I_in)+"] "+
																						" va_BAU_Etroite_in ["+convert(varchar(6),@va_BAU_Etroite_in)+"] "+
																						" va_BAU_I_Etroite_in ["+convert(varchar(6),@va_BAU_I_Etroite_in)+"] "+
																						" va_Degats_in ["+convert(varchar(6),@va_Degats_in)+"] "+
																						" va_TypeAnterieur_in ["+convert(varchar(6),@va_TypeAnterieur_in)+"] "+
																						" va_FausseAlerte_in ["+convert(varchar(6),@va_FausseAlerte_in)+"] "+
																						" va_Longueur_in ["+convert(varchar(6),@va_Longueur_in)+"] "+
																						" va_ComEvtInit_in ["+convert(varchar(6),@va_ComEvtInit_in)+"] "+
																						" va_SiteAlerte_in ["+convert(varchar(6),@va_SiteAlerte_in)+"] "+
																						" va_Majeur_in ["+convert(varchar(6),@va_Majeur_in)+"] "+
																						" va_Ecoulement_in ["+convert(varchar(6),@va_Ecoulement_in)+"] "+
																						" va_NomSiteLocal_in ["+@va_NomSiteLocal_in+"]"
			)
		end

		return XZAEC_FMC_PAS_ENR
	end

#endif
#endif

	if @va_Origine_in = "DETECTION BOUCHON"
		select @va_Debut_in = @vl_Debut

	/* Si les degats domaine ne concerne pas ce type de fmc alors il n'est pas value */
	if @vl_Degats is null
		select @va_Degats_in = null

		if @va_NumAlerte_in = -1
		begin
			select @va_NumAlerte_in =  @vl_NumAlerte
			select @va_SiteAlerte_in =  @vl_SiteAlerte
		end
	/*A
	 * Dans le cas d'un retypage, verifier la validite de l'operation
	 * --------------------------------------------------------------
	 */

	if @va_Type_in != @va_TypeAnterieur_in
	begin
		select @vl_HoroChgtType = @va_Validation_in

		/*A le nouveau type ne peut etre 'Tete de bouchon' */
		if @va_Type_in = XZAEC_FMC_TeteBouchon return XDC_ARG_INV

		/*A le nouveau type ne peut etre 'Poste operateur' */
		if @va_Type_in = XZAEC_FMC_PosteOperateur return XDC_ARG_INV

		/*A pas de retypage possible d'une fmc close */
		if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_CLOS
			return XZAEC_FMC_PAS_RETYPAGE

		/*A verifier que le nouveau type existe, determiner sa classe */
		select @vl_Classe = classe
		from CFG..TYP_FMC
		where numero = @va_Type_in  and  supprime = XDC_FAUX

		if @@rowcount != 1
			return XDC_ARG_INV

		/*A verifier la compatibilite des classes des deux types */
		select @vl_ClasseAnterieure = classe
		from CFG..TYP_FMC
		where numero = @va_TypeAnterieur_in

		if @vl_Classe != @vl_ClasseAnterieure and
		   not (@vl_ClasseAnterieure = XZAEC_ClasseAccident and
			   @vl_Classe = XZAEC_ClasseVehicule) and
		   not (@vl_ClasseAnterieure = XZAEC_ClasseVehicule and
			   @vl_Classe = XZAEC_ClasseAccident)
			return XZAEC_FMC_INC_RETYPAGE
	end

	/*A
	 * Controle de l'etat de la fmc et des droits d'enrichissement
	 * -----------------------------------------------------------
	 */


	/*A cas d'un �v�nement en prevision */
	if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_PREV
	begin
		/* controler les parametres d'entree */
		if @va_DebutPrevu_in = null or @va_Debut_in != null or
		   @va_NumAlerte_in != null
			return XDC_ARG_INV

		/*B controler si le poste operateur a le droit d'enrichir */
		if (@va_ComEvtInit_in = null and (
			@va_PosteEnrich_in != null or @vl_PosteEnrich != null))
		begin
			/* on trace dans PIT_LOG */
			/* Recherche de la reference PIT a partir de la fmc */
			select @vl_pitlog_reference=reference
			from EXP..FMC_REF_EXT
			where numero=@va_NumEvenement_in and 
				  cle=@va_CleEvenement_in
			if @@rowcount>0
			begin
				/* recherche du dernier correlation_id recu pour cette fmc */
				set rowcount 1
				select @vl_pitlog_correlation_id=correlation_id
				from EXP..PIT_LOG
				where reference=@vl_pitlog_reference
				order by horodate desc
				set rowcount 0
				/* recherche de l'id a creer */
				select @vl_pitlog_id=max(id)+1
				from EXP..PIT_LOG
				/* insertion trace */
				insert into EXP..PIT_LOG
				values (@vl_pitlog_id,getdate(),@vl_pitlog_reference,@vl_pitlog_correlation_id,"ERROR",1,"Erreur droits XZAE11 cas 3 : <vl_PosteEnrich="+convert(varchar(6),@vl_PosteEnrich)+">"+
																							" va_NumEvenement_in ["+convert(varchar(6),@va_NumEvenement_in)+"] "+
																							" va_CleEvenement_in ["+convert(varchar(6),@va_CleEvenement_in)+"] "+
																							" va_PosteOperateur_in ["+convert(varchar(6),@va_PosteOperateur_in)+"] "+
																							" va_Prevision_Traiter_Clos_in ["+convert(varchar(6),@va_Prevision_Traiter_Clos_in)+"] "+
																							" va_Type_in ["+convert(varchar(6),@va_Type_in)+"] "+
																							" va_DebutPrevu_in ["+convert(varchar(12),@va_DebutPrevu_in)+"] "+
																							" va_Debut_in ["+convert(varchar(12),@va_Debut_in)+"] "+
																							" va_FinPrevu_in ["+convert(varchar(12),@va_FinPrevu_in	)+"] "+
																							" va_Fin_in ["+convert(varchar(12),@va_Fin_in)+"] "+
																							" va_Validation_in ["+convert(varchar(12),@va_Validation_in)+"] "+
																							" va_NumCause_in ["+convert(varchar(6),@va_NumCause_in)+"] "+
																							" va_CleCause_in ["+convert(varchar(6),@va_CleCause_in)+"] "+
																							" va_NumAlerte_in ["+convert(varchar(6),@va_NumAlerte_in)+"] "+
																							" va_Origine_in ["+@va_Origine_in+"] "+
																							" va_PosteEnrich_in ["+convert(varchar(6),@va_PosteEnrich_in)+"] "+
																							" va_ConfirmeSignale_in ["+convert(varchar(6),@va_ConfirmeSignale_in)+"] "+
																							" va_NumAutoroute_in ["+convert(varchar(6),@va_NumAutoroute_in)+"] "+
																							" va_PR_in ["+convert(varchar(6),@va_PR_in)+"] "+
																							" va_Sens_in ["+convert(varchar(6),@va_Sens_in)+"] "+
																							" va_PtCaracteristique_in ["+convert(varchar(6),@va_PtCaracteristique_in)+"] "+
																							" va_TypePtCaracteristique_in ["+convert(varchar(6),@va_TypePtCaracteristique_in)+"] "+
																							" va_Position_in ["+convert(varchar(6),@va_Position_in)+"] "+
																							" va_VR_in ["+convert(varchar(6),@va_VR_in)+"] "+
																							" va_VM2_in ["+convert(varchar(6),@va_VM2_in)+"] "+
																							" va_VM1_in ["+convert(varchar(6),@va_VM1_in)+"] "+
																							" va_VL_in ["+convert(varchar(6),@va_VL_in)+"] "+
																							" va_BAU_in ["+convert(varchar(6),@va_BAU_in)+"] "+
																							" va_VR_I_in ["+convert(varchar(6),@va_VR_I_in)+"] "+
																							" va_VM2_I_in ["+convert(varchar(6),@va_VM2_I_in)+"] "+
																							" va_VM1_I_in ["+convert(varchar(6),@va_VM1_I_in)+"] "+
																							" va_VL_I_in ["+convert(varchar(6),@va_VL_I_in)+"] "+
																							" va_BAU_I_in ["+convert(varchar(6),@va_BAU_I_in)+"] "+
																							" va_BAU_Etroite_in ["+convert(varchar(6),@va_BAU_Etroite_in)+"] "+
																							" va_BAU_I_Etroite_in ["+convert(varchar(6),@va_BAU_I_Etroite_in)+"] "+
																							" va_Degats_in ["+convert(varchar(6),@va_Degats_in)+"] "+
																							" va_TypeAnterieur_in ["+convert(varchar(6),@va_TypeAnterieur_in)+"] "+
																							" va_FausseAlerte_in ["+convert(varchar(6),@va_FausseAlerte_in)+"] "+
																							" va_Longueur_in ["+convert(varchar(6),@va_Longueur_in)+"] "+
																							" va_ComEvtInit_in ["+convert(varchar(6),@va_ComEvtInit_in)+"] "+
																							" va_SiteAlerte_in ["+convert(varchar(6),@va_SiteAlerte_in)+"] "+
																							" va_Majeur_in ["+convert(varchar(6),@va_Majeur_in)+"] "+
																							" va_Ecoulement_in ["+convert(varchar(6),@va_Ecoulement_in)+"] "+
																							" va_NomSiteLocal_in ["+@va_NomSiteLocal_in+"]"
				)
			end
			return XZAEC_FMC_PAS_ENR
		end
#ifndef PC_SIMPL
#ifndef HIST
		/*B verifier que l'evenement est en prevision */
		if (@va_ComEvtInit_in = null)
		begin
		if exists ( 	select * from EXP..FMC_TRT
				where	evenement = @va_NumEvenement_in and 
					cle = @va_CleEvenement_in) or
		   			@vl_Debut != null
			return XZAEC_FMC_ETAT_INC
		end
#endif
#endif

		/* Verifier que la prevision debute au plus tot dans 10h */
		select @vl_DateCourante = getdate()
		if datediff( hour, @vl_DateCourante, @va_DebutPrevu_in) < 10
			return XZAEC_FMC_DEB_PRV_INC
	end

#ifndef PC_SIMPL
#ifndef HIST
	/*A cas d'un �v�nement a traiter */
	else if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_TRAI
	begin
		/* controler les parametres d'entree */
		if (@va_DebutPrevu_in = null and @va_Debut_in = null) or
		   (@va_Fin_in != null and @va_Fin_in < @va_Debut_in)
			return XDC_ARG_INV

		/*B controler si le poste operateur a le droit d'enrichir */
		if @va_PosteEnrich_in != @va_PosteOperateur_in or @va_PosteEnrich_in = null
		begin
			/* on trace dans PIT_LOG */
			/* Recherche de la reference PIT a partir de la fmc */
			select @vl_pitlog_reference=reference
			from EXP..FMC_REF_EXT
			where numero=@va_NumEvenement_in and 
				  cle=@va_CleEvenement_in
			if @@rowcount>0
			begin
				/* recherche du dernier correlation_id recu pour cette fmc */
				set rowcount 1
				select @vl_pitlog_correlation_id=correlation_id
				from EXP..PIT_LOG
				where reference=@vl_pitlog_reference
				order by horodate desc
				set rowcount 0
				/* recherche de l'id a creer */
				select @vl_pitlog_id=max(id)+1
				from EXP..PIT_LOG
				/* insertion trace */
				insert into EXP..PIT_LOG
				values (@vl_pitlog_id,getdate(),@vl_pitlog_reference,@vl_pitlog_correlation_id,"ERROR",1,"Erreur droits XZAE11 cas 4 : "+
																							" va_NumEvenement_in ["+convert(varchar(6),@va_NumEvenement_in)+"] "+
																							" va_CleEvenement_in ["+convert(varchar(6),@va_CleEvenement_in)+"] "+
																							" va_PosteOperateur_in ["+convert(varchar(6),@va_PosteOperateur_in)+"] "+
																							" va_Prevision_Traiter_Clos_in ["+convert(varchar(6),@va_Prevision_Traiter_Clos_in)+"] "+
																							" va_Type_in ["+convert(varchar(6),@va_Type_in)+"] "+
																							" va_DebutPrevu_in ["+convert(varchar(12),@va_DebutPrevu_in)+"] "+
																							" va_Debut_in ["+convert(varchar(12),@va_Debut_in)+"] "+
																							" va_FinPrevu_in ["+convert(varchar(12),@va_FinPrevu_in	)+"] "+
																							" va_Fin_in ["+convert(varchar(12),@va_Fin_in)+"] "+
																							" va_Validation_in ["+convert(varchar(12),@va_Validation_in)+"] "+
																							" va_NumCause_in ["+convert(varchar(6),@va_NumCause_in)+"] "+
																							" va_CleCause_in ["+convert(varchar(6),@va_CleCause_in)+"] "+
																							" va_NumAlerte_in ["+convert(varchar(6),@va_NumAlerte_in)+"] "+
																							" va_Origine_in ["+@va_Origine_in+"] "+
																							" va_PosteEnrich_in ["+convert(varchar(6),@va_PosteEnrich_in)+"] "+
																							" va_ConfirmeSignale_in ["+convert(varchar(6),@va_ConfirmeSignale_in)+"] "+
																							" va_NumAutoroute_in ["+convert(varchar(6),@va_NumAutoroute_in)+"] "+
																							" va_PR_in ["+convert(varchar(6),@va_PR_in)+"] "+
																							" va_Sens_in ["+convert(varchar(6),@va_Sens_in)+"] "+
																							" va_PtCaracteristique_in ["+convert(varchar(6),@va_PtCaracteristique_in)+"] "+
																							" va_TypePtCaracteristique_in ["+convert(varchar(6),@va_TypePtCaracteristique_in)+"] "+
																							" va_Position_in ["+convert(varchar(6),@va_Position_in)+"] "+
																							" va_VR_in ["+convert(varchar(6),@va_VR_in)+"] "+
																							" va_VM2_in ["+convert(varchar(6),@va_VM2_in)+"] "+
																							" va_VM1_in ["+convert(varchar(6),@va_VM1_in)+"] "+
																							" va_VL_in ["+convert(varchar(6),@va_VL_in)+"] "+
																							" va_BAU_in ["+convert(varchar(6),@va_BAU_in)+"] "+
																							" va_VR_I_in ["+convert(varchar(6),@va_VR_I_in)+"] "+
																							" va_VM2_I_in ["+convert(varchar(6),@va_VM2_I_in)+"] "+
																							" va_VM1_I_in ["+convert(varchar(6),@va_VM1_I_in)+"] "+
																							" va_VL_I_in ["+convert(varchar(6),@va_VL_I_in)+"] "+
																							" va_BAU_I_in ["+convert(varchar(6),@va_BAU_I_in)+"] "+
																							" va_BAU_Etroite_in ["+convert(varchar(6),@va_BAU_Etroite_in)+"] "+
																							" va_BAU_I_Etroite_in ["+convert(varchar(6),@va_BAU_I_Etroite_in)+"] "+
																							" va_Degats_in ["+convert(varchar(6),@va_Degats_in)+"] "+
																							" va_TypeAnterieur_in ["+convert(varchar(6),@va_TypeAnterieur_in)+"] "+
																							" va_FausseAlerte_in ["+convert(varchar(6),@va_FausseAlerte_in)+"] "+
																							" va_Longueur_in ["+convert(varchar(6),@va_Longueur_in)+"] "+
																							" va_ComEvtInit_in ["+convert(varchar(6),@va_ComEvtInit_in)+"] "+
																							" va_SiteAlerte_in ["+convert(varchar(6),@va_SiteAlerte_in)+"] "+
																							" va_Majeur_in ["+convert(varchar(6),@va_Majeur_in)+"] "+
																							" va_Ecoulement_in ["+convert(varchar(6),@va_Ecoulement_in)+"] "+
																							" va_NomSiteLocal_in ["+@va_NomSiteLocal_in+"]"
				)
			end
			return XZAEC_FMC_PAS_ENR
		end
		if @vl_PosteEnrich != null 
			if @va_PosteEnrich_in != @vl_PosteEnrich
			begin
				/* on trace dans PIT_LOG */
				/* Recherche de la reference PIT a partir de la fmc */
				select @vl_pitlog_reference=reference
				from EXP..FMC_REF_EXT
				where numero=@va_NumEvenement_in and 
					cle=@va_CleEvenement_in
				if @@rowcount>0
				begin
					/* recherche du dernier correlation_id recu pour cette fmc */
					set rowcount 1
					select @vl_pitlog_correlation_id=correlation_id
					from EXP..PIT_LOG
					where reference=@vl_pitlog_reference
					order by horodate desc
					set rowcount 0
					/* recherche de l'id a creer */
					select @vl_pitlog_id=max(id)+1
					from EXP..PIT_LOG
					/* insertion trace */
					insert into EXP..PIT_LOG
					values (@vl_pitlog_id,getdate(),@vl_pitlog_reference,@vl_pitlog_correlation_id,"ERROR",1,"Erreur droits XZAE11 cas 5 : <vl_PosteEnrich="+convert(varchar(6),@vl_PosteEnrich)+">"+
																								" va_NumEvenement_in ["+convert(varchar(6),@va_NumEvenement_in)+"] "+
																								" va_CleEvenement_in ["+convert(varchar(6),@va_CleEvenement_in)+"] "+
																								" va_PosteOperateur_in ["+convert(varchar(6),@va_PosteOperateur_in)+"] "+
																								" va_Prevision_Traiter_Clos_in ["+convert(varchar(6),@va_Prevision_Traiter_Clos_in)+"] "+
																								" va_Type_in ["+convert(varchar(6),@va_Type_in)+"] "+
																								" va_DebutPrevu_in ["+convert(varchar(12),@va_DebutPrevu_in)+"] "+
																								" va_Debut_in ["+convert(varchar(12),@va_Debut_in)+"] "+
																								" va_FinPrevu_in ["+convert(varchar(12),@va_FinPrevu_in	)+"] "+
																								" va_Fin_in ["+convert(varchar(12),@va_Fin_in)+"] "+
																								" va_Validation_in ["+convert(varchar(12),@va_Validation_in)+"] "+
																								" va_NumCause_in ["+convert(varchar(6),@va_NumCause_in)+"] "+
																								" va_CleCause_in ["+convert(varchar(6),@va_CleCause_in)+"] "+
																								" va_NumAlerte_in ["+convert(varchar(6),@va_NumAlerte_in)+"] "+
																								" va_Origine_in ["+@va_Origine_in+"] "+
																								" va_PosteEnrich_in ["+convert(varchar(6),@va_PosteEnrich_in)+"] "+
																								" va_ConfirmeSignale_in ["+convert(varchar(6),@va_ConfirmeSignale_in)+"] "+
																								" va_NumAutoroute_in ["+convert(varchar(6),@va_NumAutoroute_in)+"] "+
																								" va_PR_in ["+convert(varchar(6),@va_PR_in)+"] "+
																								" va_Sens_in ["+convert(varchar(6),@va_Sens_in)+"] "+
																								" va_PtCaracteristique_in ["+convert(varchar(6),@va_PtCaracteristique_in)+"] "+
																								" va_TypePtCaracteristique_in ["+convert(varchar(6),@va_TypePtCaracteristique_in)+"] "+
																								" va_Position_in ["+convert(varchar(6),@va_Position_in)+"] "+
																								" va_VR_in ["+convert(varchar(6),@va_VR_in)+"] "+
																								" va_VM2_in ["+convert(varchar(6),@va_VM2_in)+"] "+
																								" va_VM1_in ["+convert(varchar(6),@va_VM1_in)+"] "+
																								" va_VL_in ["+convert(varchar(6),@va_VL_in)+"] "+
																								" va_BAU_in ["+convert(varchar(6),@va_BAU_in)+"] "+
																								" va_VR_I_in ["+convert(varchar(6),@va_VR_I_in)+"] "+
																								" va_VM2_I_in ["+convert(varchar(6),@va_VM2_I_in)+"] "+
																								" va_VM1_I_in ["+convert(varchar(6),@va_VM1_I_in)+"] "+
																								" va_VL_I_in ["+convert(varchar(6),@va_VL_I_in)+"] "+
																								" va_BAU_I_in ["+convert(varchar(6),@va_BAU_I_in)+"] "+
																								" va_BAU_Etroite_in ["+convert(varchar(6),@va_BAU_Etroite_in)+"] "+
																								" va_BAU_I_Etroite_in ["+convert(varchar(6),@va_BAU_I_Etroite_in)+"] "+
																								" va_Degats_in ["+convert(varchar(6),@va_Degats_in)+"] "+
																								" va_TypeAnterieur_in ["+convert(varchar(6),@va_TypeAnterieur_in)+"] "+
																								" va_FausseAlerte_in ["+convert(varchar(6),@va_FausseAlerte_in)+"] "+
																								" va_Longueur_in ["+convert(varchar(6),@va_Longueur_in)+"] "+
																								" va_ComEvtInit_in ["+convert(varchar(6),@va_ComEvtInit_in)+"] "+
																								" va_SiteAlerte_in ["+convert(varchar(6),@va_SiteAlerte_in)+"] "+
																								" va_Majeur_in ["+convert(varchar(6),@va_Majeur_in)+"] "+
																								" va_Ecoulement_in ["+convert(varchar(6),@va_Ecoulement_in)+"] "+
																								" va_NomSiteLocal_in ["+@va_NomSiteLocal_in+"]"
					)
				end
				return XZAEC_FMC_PAS_ENR
			end
	end
	/*A cas d'un �v�nement clos */
	else if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_CLOS
	begin
		/* controler les parametres d'entree */
		if @va_Debut_in = null or @va_Fin_in = null
			return XDC_ARG_INV

		/*B verifier que l'evenement est clos */
		if @vl_Cloture = null
			return XZAEC_FMC_ETAT_INC
	end
	else return XDC_ARG_INV
			
	/*A verifier l'existance de la cause de l'evenement */
	if @va_Prevision_Traiter_Clos_in != XDC_FMC_ETAT_CLOS
	begin
		if @va_NumCause_in != null
			if not exists ( select * from EXP..FMC_GEN
					where	numero = @va_NumCause_in and
						cle = @va_CleCause_in)
				return XDC_NOK
	
	
		/*A verifier l'existance de l'alerte de l'evenement */
		if @va_NumAlerte_in != null
		begin
			select @vl_Qualification = qualification, @vl_TypeAlerte = type
			from EXP..ALT_EXP
			where	numero = @va_NumAlerte_in and
				sit = @va_SiteAlerte_in
	
			if @@rowcount != 1
				return XDC_NOK
		end
	end
#endif
#endif

	/*A
	 * Controle de validite de la localisation de la fmc
	 * -------------------------------------------------
	 */

	/* DEM439 : ajout du type FMC � l'appel */
	exec @vl_Status = XZAE66 @va_NumAutoroute_in, @va_PR_in, @va_Sens_in,
				@va_PtCaracteristique_in, @va_TypePtCaracteristique_in,
				@va_Position_in, @vl_Type,@vl_Autoroute output,
				@vl_PR output, @vl_Sens output, @vl_PtCaracteristique output,
				@vl_TypePtCaracteristique output, @vl_Position output,
				@vl_VR output, @vl_VM2 output, @vl_VM1 output,
				@vl_VL output, @vl_BAU output, 
				@vl_VR_I output, @vl_VM2_I output, @vl_VM1_I output,
				@vl_VL_I output, @vl_BAU_I output, 
				@va_BAU_Etroite_in output, @va_BAU_I_Etroite_in output,
				@vl_District output, @vl_DistrictRecouvrement output,
				@vl_ResultConfig output

	if @vl_Status = null
		return XDC_PRC_INC
	else if @vl_Status != XDC_OK
		return XZAEC_FMC_LOC_INC		

	/*si je suis sur un echangeur*/
	if @vl_TypePtCaracteristique = XDC_POINT_CARACT_ECHANGEUR
	begin
	 	if (@vl_ResultConfig != XDC_OK and @vl_ResultConfig != XDC_LOC_PR_MOD and @vl_ResultConfig != XDC_LOC_SENS_MOD ) or
		   @va_Position_in != @vl_Position
			return XZAEC_FMC_LOC_INC
	end
	else
	begin
		/* tester si la localisation donnee en entree etait correcte */
		if @vl_ResultConfig != XDC_OK or @va_NumAutoroute_in != @vl_Autoroute or
	   			@va_PR_in != @vl_PR or @va_Sens_in != @vl_Sens or 
	   			@va_Position_in != @vl_Position
			return XZAEC_FMC_LOC_INC
	end

	if @va_ComEvtInit_in = null
	begin
	if @vl_VR = XDC_VOIE_INEXISTANTE
		if @va_VR_in != XDC_VOIE_INEXISTANTE
			return XZAEC_FMC_VOIE_INC
	if @vl_VM2 = XDC_VOIE_INEXISTANTE
		if @va_VM2_in != XDC_VOIE_INEXISTANTE
			return XZAEC_FMC_VOIE_INC
	if @vl_VM1 = XDC_VOIE_INEXISTANTE
		if @va_VM1_in != XDC_VOIE_INEXISTANTE
			return XZAEC_FMC_VOIE_INC
	if @vl_VL = XDC_VOIE_INEXISTANTE
		if @va_VL_in != XDC_VOIE_INEXISTANTE
			return XZAEC_FMC_VOIE_INC
	if @vl_BAU = XDC_VOIE_INEXISTANTE
		if @va_BAU_in != XDC_VOIE_INEXISTANTE
			return XZAEC_FMC_VOIE_INC
	if @vl_VR_I = XDC_VOIE_INEXISTANTE
		if @va_VR_I_in != XDC_VOIE_INEXISTANTE
			return XZAEC_FMC_VOIE_INC
	if @vl_VM2_I = XDC_VOIE_INEXISTANTE
		if @va_VM2_I_in != XDC_VOIE_INEXISTANTE
			return XZAEC_FMC_VOIE_INC
	if @vl_VM1_I = XDC_VOIE_INEXISTANTE
		if @va_VM1_I_in != XDC_VOIE_INEXISTANTE
			return XZAEC_FMC_VOIE_INC
	if @vl_VL_I = XDC_VOIE_INEXISTANTE
		if @va_VL_I_in != XDC_VOIE_INEXISTANTE
			return XZAEC_FMC_VOIE_INC
	if @vl_BAU_I = XDC_VOIE_INEXISTANTE
		if @va_BAU_I_in != XDC_VOIE_INEXISTANTE
			return XZAEC_FMC_VOIE_INC
	end
	else
	begin
	if @vl_VR = XDC_VOIE_INEXISTANTE
                select @va_VR_in = XDC_VOIE_INEXISTANTE
        if @vl_VM2 = XDC_VOIE_INEXISTANTE
                select @va_VM2_in = XDC_VOIE_INEXISTANTE
        if @vl_VM1 = XDC_VOIE_INEXISTANTE
                select @va_VM1_in = XDC_VOIE_INEXISTANTE
        if @vl_VL = XDC_VOIE_INEXISTANTE
                select @va_VL_in = XDC_VOIE_INEXISTANTE
        if @vl_BAU = XDC_VOIE_INEXISTANTE
                select @va_BAU_in = XDC_VOIE_INEXISTANTE
	if @vl_VR_I = XDC_VOIE_INEXISTANTE
                select @va_VR_I_in = XDC_VOIE_INEXISTANTE
        if @vl_VM2_I = XDC_VOIE_INEXISTANTE
                select @va_VM2_I_in = XDC_VOIE_INEXISTANTE
        if @vl_VM1_I = XDC_VOIE_INEXISTANTE
                select @va_VM1_I_in = XDC_VOIE_INEXISTANTE
        if @vl_VL_I = XDC_VOIE_INEXISTANTE
                select @va_VL_I_in = XDC_VOIE_INEXISTANTE
        if @vl_BAU_I = XDC_VOIE_INEXISTANTE
                select @va_BAU_I_in = XDC_VOIE_INEXISTANTE
	end
	 /****************************************************************************
	 **                                                                         **
	 **                 Etape 1 : Modification de l'evenement                   **
	 **                                                                         **
	 ****************************************************************************/

	begin tran
#ifdef HIST
		/* modifier la qualification de l'alerte si elle est a confirmer */
		/* (dans tous les cas : cause pb */
		if @va_NumAlerte_in != null and 
		   ( @vl_TypeAlerte != XZAAC_ALERTE_AVA_POTENTIEL_DOUBLON and @vl_TypeAlerte != XZAAC_ALERTE_COCKPIT_BOUCHON_POTENTIEL_DOUBLON ) and 
		   not exists ( select * from HIS..ALT_EXP 
						where numero = @va_NumAlerte_in and sit = @va_SiteAlerte_in 
						and qualification = XDC_ALT_VRAIE_EVT)
		begin
			update HIS..ALT_EXP set qualification = XDC_ALT_VRAIE_EVT, evenement = @va_NumEvenement_in, cle = @va_CleEvenement_in
			where numero = @va_NumAlerte_in and sit = @va_SiteAlerte_in
		end
		if @va_NumAlerte_in != null and 
		   ( @vl_TypeAlerte = XZAAC_ALERTE_AVA_POTENTIEL_DOUBLON  or @vl_TypeAlerte = XZAAC_ALERTE_COCKPIT_BOUCHON_POTENTIEL_DOUBLON )
		   and not exists ( select * from HIS..ALT_EXP 
							where numero = @va_NumAlerte_in and sit = @va_SiteAlerte_in 
							and qualification = XDC_ALT_VRAIE_FUSION_FMC_SAE_EXT)
		begin
			update HIS..ALT_EXP set qualification = XDC_ALT_VRAIE_FUSION_FMC_SAE_EXT, evenement = @va_NumEvenement_in, cle = @va_CleEvenement_in
			where numero = @va_NumAlerte_in and sit = @va_SiteAlerte_in
		end
#else
#ifndef PC_SIMPL
		/* modifier la qualification de l'alerte si elle est a confirmer */
		/* (dans tous les cas : cause pb */
		if @va_NumAlerte_in != null and 
		   ( @vl_TypeAlerte != XZAAC_ALERTE_AVA_POTENTIEL_DOUBLON and @vl_TypeAlerte != XZAAC_ALERTE_COCKPIT_BOUCHON_POTENTIEL_DOUBLON ) and 
		   not exists ( select * from EXP..ALT_EXP 
						where numero = @va_NumAlerte_in and sit = @va_SiteAlerte_in 
						and qualification = XDC_ALT_VRAIE_EVT)
		begin
			update EXP..ALT_EXP set qualification = XDC_ALT_VRAIE_EVT, evenement = @va_NumEvenement_in, cle = @va_CleEvenement_in
			where numero = @va_NumAlerte_in and sit = @va_SiteAlerte_in
		end
		if @va_NumAlerte_in != null and 
		   ( @vl_TypeAlerte = XZAAC_ALERTE_AVA_POTENTIEL_DOUBLON  or @vl_TypeAlerte = XZAAC_ALERTE_COCKPIT_BOUCHON_POTENTIEL_DOUBLON )
		   and not exists ( select * from EXP..ALT_EXP 
							where numero = @va_NumAlerte_in and sit = @va_SiteAlerte_in 
							and qualification = XDC_ALT_VRAIE_FUSION_FMC_SAE_EXT)
		begin
			update EXP..ALT_EXP set qualification = XDC_ALT_VRAIE_FUSION_FMC_SAE_EXT, evenement = @va_NumEvenement_in, cle = @va_CleEvenement_in
			where numero = @va_NumAlerte_in and sit = @va_SiteAlerte_in
		end
#endif
#endif
	commit tran

		/* pas de chaine null dans la base : remplacer par " " */
		if @va_Origine_in = null
			select @va_Origine_in = XDC_CHAINE_VIDE

		/*! Pas de modification de la cause d'une queue de bouchon */
		/*! Pas de modification de cause autoriser en mode clos : uniquement possibilite de supprimer */
		/*if (@vl_Type = XZAEC_FMC_QueueBouchon or @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_CLOS) and*/
		if (@va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_CLOS) and
			@vl_Cause is not null and @vl_CleCause is not null
			select	@va_NumCause_in = @vl_Cause, 
				@va_CleCause_in = @vl_CleCause

	begin tran

		/*A modifier la partie generique de l'evenement */
		if	@va_Prevision_Traiter_Clos_in != XDC_FMC_ETAT_CLOS or 
			@vl_Site != (select numero from CFG..RES_DIS where type = XDC_TYPEM_SD) or
			@vl_Cloture < (select dateadd(month, -1, getdate ()))
		begin
#ifdef HIST
			update HIS..FMC_GEN set
				cause		= @va_NumCause_in,
				cle_cause 	= @va_CleCause_in,
				alerte		= @va_NumAlerte_in,
				sit_alerte	= @va_SiteAlerte_in,
				debut		= @va_Debut_in,
				debut_prevu	= @va_DebutPrevu_in,
				fin		= @va_Fin_in,
				fin_prevue	= @va_FinPrevu_in,
				origine_creation = @va_Origine_in,
				cloture		= @vl_Cloture,
				poste_enrichisseur= @va_PosteEnrich_in,
				degats_domaine	= @va_Degats_in,
				type		= @va_Type_in,
				fausse_alerte	= @va_FausseAlerte_in,
				com_fmc_init	= @va_ComEvtInit_in,
				horodate_chgt_type	= @vl_HoroChgtType,
				zz		= zz + 1
			where	numero = @va_NumEvenement_in and
				cle = @va_CleEvenement_in and
				sit = @vl_Site
	
#else
			update EXP..FMC_GEN set
				cause		= @va_NumCause_in,
				cle_cause 	= @va_CleCause_in,
				alerte		= @va_NumAlerte_in,
				sit_alerte	= @va_SiteAlerte_in,
				debut		= @va_Debut_in,
				debut_prevu	= @va_DebutPrevu_in,
				fin		= @va_Fin_in,
				fin_prevue	= @va_FinPrevu_in,
				origine_creation = @va_Origine_in,
				cloture		= @vl_Cloture,
				poste_enrichisseur= @va_PosteEnrich_in,
				degats_domaine	= @va_Degats_in,
				type		= @va_Type_in,
				fausse_alerte	= @va_FausseAlerte_in,
				com_fmc_init	= @va_ComEvtInit_in,
				horodate_chgt_type	= @vl_HoroChgtType,
				zz		= zz + 1
			where	numero = @va_NumEvenement_in and
				cle = @va_CleEvenement_in and
				sit = @vl_Site
	
#endif
			if @@rowcount != 1
				select @vl_Erreur = XDC_OUI
	
			/* recherche si la cause est chang�e */
			/* mettre a jour la date de la cause */
			if @va_NumCause_in != null and @vl_Erreur = XDC_NON and 
			   (@vl_Cause<>@va_NumCause_in or @vl_CleCause <> @va_CleCause_in)
			begin
#ifdef HIST
				update HIS..FMC_GEN set 
					horodate_lien_cause = @va_Validation_in,
					zz		= zz + 1
				where	numero = @va_NumEvenement_in and
					cle = @va_CleEvenement_in and
					sit = @vl_Site
#else
				update EXP..FMC_GEN set 
					horodate_lien_cause = @va_Validation_in,
					zz		= zz + 1
				where	numero = @va_NumEvenement_in and
					cle = @va_CleEvenement_in and
					sit = @vl_Site
#endif
				if @@rowcount != 1
					select @vl_Erreur = XDC_OUI
			end
		
		end
		else if	@va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_CLOS
		begin
#ifdef HIST
			update HIS..FMC_GEN set
				cause		= @va_NumCause_in,
				cle_cause 	= @va_CleCause_in,
				alerte		= @va_NumAlerte_in,
				sit_alerte	= @va_SiteAlerte_in,
				debut		= @va_Debut_in,
				debut_prevu	= @va_DebutPrevu_in,
				fin		= @va_Fin_in,
				fin_prevue	= @va_FinPrevu_in,
				origine_creation = @va_Origine_in,
				cloture		= @vl_Cloture,
				poste_enrichisseur= @va_PosteEnrich_in,
				degats_domaine	= @va_Degats_in,
				type		= @va_Type_in,
				fausse_alerte	= @va_FausseAlerte_in,
				horodate_chgt_type	= @vl_HoroChgtType,
				com_fmc_init	= @va_ComEvtInit_in,
				sit		= @vl_Site,
				zz		= zz + 1
			where	numero = @va_NumEvenement_in and
				cle = @va_CleEvenement_in
		
				if @@rowcount != 1
					select @vl_Erreur = XDC_OUI
		
				/* recherche si la cause est chang�e */
				/* mettre a jour la date de la cause */
				if @va_NumCause_in != null and @vl_Erreur = XDC_NON and 
				   (@vl_Cause<>@va_NumCause_in or @vl_CleCause <> @va_CleCause_in)
				begin
					update HIS..FMC_GEN set 
						horodate_lien_cause = @va_Validation_in,
						zz		= zz + 1
					where	numero = @va_NumEvenement_in and
						cle = @va_CleEvenement_in
	
					if @@rowcount != 1
						select @vl_Erreur = XDC_OUI
				end
#else
			update EXP..FMC_GEN set
				cause		= @va_NumCause_in,
				cle_cause 	= @va_CleCause_in,
				alerte		= @va_NumAlerte_in,
				sit_alerte	= @va_SiteAlerte_in,
				debut		= @va_Debut_in,
				debut_prevu	= @va_DebutPrevu_in,
				fin		= @va_Fin_in,
				fin_prevue	= @va_FinPrevu_in,
				origine_creation = @va_Origine_in,
				cloture		= @vl_Cloture,
				poste_enrichisseur= @va_PosteEnrich_in,
				degats_domaine	= @va_Degats_in,
				type		= @va_Type_in,
				fausse_alerte	= @va_FausseAlerte_in,
				horodate_chgt_type	= @vl_HoroChgtType,
				com_fmc_init	= @va_ComEvtInit_in,
				sit		= @vl_Site,
				zz		= zz + 1
			where	numero = @va_NumEvenement_in and
				cle = @va_CleEvenement_in
#endif
	
			/*!!!!!!!!!!!!!!!!!! Normalement pas de modif sur fmc close purgee !! a verifier */
			/* si oui alors mettre "select @vl_Erreur = XDC_OUI" si @@rowcount = 0 */
			if @@rowcount = 0
			begin
				select @vl_Erreur = XDC_OUI
			end

			/* recherche si la cause est chang�e */
			/* mettre a jour la date de la cause */
			else if @va_NumCause_in != null and @vl_Erreur = XDC_NON and 
			   (@vl_Cause<>@va_NumCause_in or @vl_CleCause <> @va_CleCause_in)
			begin
#ifdef HIST
				update HIS..FMC_GEN set 
					horodate_lien_cause = @va_Validation_in,
					zz		= zz + 1
				where	numero = @va_NumEvenement_in and
					cle = @va_CleEvenement_in and
					sit = @vl_Site
#else
				update EXP..FMC_GEN set 
					horodate_lien_cause = @va_Validation_in,
					zz		= zz + 1
				where	numero = @va_NumEvenement_in and
					cle = @va_CleEvenement_in and
					sit = @vl_Site
#endif
				if @@rowcount != 1
					select @vl_Erreur = XDC_OUI
			end
		end

	commit tran

	 /****************************************************************************
	 **                                                                         **
	 **                   Etape 2 : Ecriture de l'historique                    **
	 **                                                                         **
	 ****************************************************************************/
	begin tran

		/*A modifier ou inserer l'historique de l'�v�nement */
		if @vl_Erreur = XDC_OUI
			select @vl_Erreur = XDC_OUI
		else if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_PREV
		begin
			/* date de cloture pour l'evenement */
			select @vl_Cloture = null


#ifdef HIST
			/*B modifier ou inserer l'historique de l'�v�nement en prevision */
			if not exists (select * from HIS..FMC_HIS
					where	numero = @va_NumEvenement_in and
						cle = @va_CleEvenement_in )
				insert HIS..FMC_HIS ( numero, cle, confirme_ou_signale,
				horodate_validation, autoroute, PR, sens, point_caracteristique, 
				numero_point_caracteristique,position_sur_aire_ou_echangeur,
				VR, VM2, VM1, VL, BAU, VR_I, VM2_I, VM1_I, VL_I, BAU_I, 
				BAU_etroite, BAU_I_etroite, sit, 
				type_anterieur, type_FMC_valide, longueur, majeur, ecoulement)
				values( @va_NumEvenement_in, @va_CleEvenement_in, 
				@va_ConfirmeSignale_in, @va_Validation_in, 
				@va_NumAutoroute_in, @va_PR_in, 
				@va_Sens_in, @va_TypePtCaracteristique_in, @va_PtCaracteristique_in,
				@vl_Position, @va_VR_in, @va_VM2_in, @va_VM1_in, @va_VL_in, 
				@va_BAU_in, @va_VR_I_in, @va_VM2_I_in, @va_VM1_I_in, @va_VL_I_in, 
				@va_BAU_I_in, @va_BAU_Etroite_in,  @va_BAU_I_Etroite_in, @vl_Site, 
				@va_TypeAnterieur_in, @va_Type_in, @va_Longueur_in, @va_Majeur_in, @va_Ecoulement_in)
	
			else	/* modifier les informations de l'evenement en prevision */
				update HIS..FMC_HIS set 
					confirme_ou_signale = @va_ConfirmeSignale_in,
					autoroute = @va_NumAutoroute_in, 
					PR = @va_PR_in, 
					sens = @va_Sens_in, 
					point_caracteristique = @va_TypePtCaracteristique_in, 
					numero_point_caracteristique = @va_PtCaracteristique_in,
					position_sur_aire_ou_echangeur = @vl_Position, 
					type_anterieur = @va_TypeAnterieur_in, 
					type_FMC_valide = @va_Type_in, 
					VR = @va_VR_in, 
					VM2 = @va_VM2_in,
					VM1 = @va_VM1_in, 
					VL = @va_VL_in, 
					BAU = @va_BAU_in, 
					VR_I = @va_VR_I_in, 
					VM2_I = @va_VM2_I_in, 
					VM1_I = @va_VM1_I_in, 
					VL_I = @va_VL_I_in, 
					BAU_I = @va_BAU_I_in,
					BAU_etroite = @va_BAU_Etroite_in,
					BAU_I_etroite = @va_BAU_I_Etroite_in,
					longueur = @va_Longueur_in,
					majeur = @va_Majeur_in,
					ecoulement = @va_Ecoulement_in
				where	horodate_validation = @va_Validation_in and
					numero = @va_NumEvenement_in and
					cle = @va_CleEvenement_in 
#else
			/*B modifier ou inserer l'historique de l'�v�nement en prevision */
			if not exists (select * from EXP..FMC_HIS
					where	numero = @va_NumEvenement_in and
						cle = @va_CleEvenement_in )
				insert EXP..FMC_HIS ( numero, cle, confirme_ou_signale,
				horodate_validation, autoroute, PR, sens, point_caracteristique, 
				numero_point_caracteristique,position_sur_aire_ou_echangeur,
				VR, VM2, VM1, VL, BAU, VR_I, VM2_I, VM1_I, VL_I, BAU_I, 
				BAU_etroite, BAU_I_etroite, sit, 
				type_anterieur, type_FMC_valide, longueur, majeur, ecoulement)
				values( @va_NumEvenement_in, @va_CleEvenement_in, 
				@va_ConfirmeSignale_in, @va_Validation_in, 
				@va_NumAutoroute_in, @va_PR_in, 
				@va_Sens_in, @va_TypePtCaracteristique_in, @va_PtCaracteristique_in,
				@vl_Position, @va_VR_in, @va_VM2_in, @va_VM1_in, @va_VL_in, 
				@va_BAU_in, @va_VR_I_in, @va_VM2_I_in, @va_VM1_I_in, @va_VL_I_in, 
				@va_BAU_I_in, @va_BAU_Etroite_in,  @va_BAU_I_Etroite_in, @vl_Site, 
				@va_TypeAnterieur_in, @va_Type_in, @va_Longueur_in, @va_Majeur_in, @va_Ecoulement_in)
	
			else	/* modifier les informations de l'evenement en prevision */
				update EXP..FMC_HIS set 
					confirme_ou_signale = @va_ConfirmeSignale_in,
					autoroute = @va_NumAutoroute_in, 
					PR = @va_PR_in, 
					sens = @va_Sens_in, 
					point_caracteristique = @va_TypePtCaracteristique_in, 
					numero_point_caracteristique = @va_PtCaracteristique_in,
					position_sur_aire_ou_echangeur = @vl_Position, 
					type_anterieur = @va_TypeAnterieur_in, 
					type_FMC_valide = @va_Type_in, 
					VR = @va_VR_in, 
					VM2 = @va_VM2_in,
					VM1 = @va_VM1_in, 
					VL = @va_VL_in, 
					BAU = @va_BAU_in, 
					VR_I = @va_VR_I_in, 
					VM2_I = @va_VM2_I_in, 
					VM1_I = @va_VM1_I_in, 
					VL_I = @va_VL_I_in, 
					BAU_I = @va_BAU_I_in,
					BAU_etroite = @va_BAU_Etroite_in,
					BAU_I_etroite = @va_BAU_I_Etroite_in,
					longueur = @va_Longueur_in,
					majeur = @va_Majeur_in,
					ecoulement = @va_Ecoulement_in
				where	horodate_validation = @va_Validation_in and
					numero = @va_NumEvenement_in and
					cle = @va_CleEvenement_in 
#endif
		end

#ifndef PC_SIMPL
		else if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_TRAI
		begin
#ifdef HIST
			/* date de cloture pour l'evenement */
			select @vl_Cloture = null

			/*B inserer l'historique d'un �v�nement a traiter */
			insert HIS..FMC_HIS ( numero, cle, confirme_ou_signale,
			horodate_validation, autoroute, PR, sens, point_caracteristique, 
			numero_point_caracteristique,position_sur_aire_ou_echangeur,
			VR, VM2, VM1, VL, BAU, VR_I, VM2_I, VM1_I, VL_I, BAU_I, 
			BAU_etroite, BAU_I_etroite, sit, 
			type_anterieur, type_FMC_valide, longueur, majeur, ecoulement)
			values( @va_NumEvenement_in, @va_CleEvenement_in, 
			@va_ConfirmeSignale_in, @va_Validation_in,
			@va_NumAutoroute_in, @va_PR_in, 
			@va_Sens_in, @va_TypePtCaracteristique_in, @va_PtCaracteristique_in,
			@vl_Position, @va_VR_in, @va_VM2_in, @va_VM1_in, @va_VL_in, 
			@va_BAU_in, @va_VR_I_in, @va_VM2_I_in, @va_VM1_I_in, @va_VL_I_in, 
			@va_BAU_I_in, @va_BAU_Etroite_in, @va_BAU_I_Etroite_in,
			@vl_Site, @va_TypeAnterieur_in, @va_Type_in, @va_Longueur_in, @va_Majeur_in, @va_Ecoulement_in)

		end
		else if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_CLOS
		begin
			select @vl_SiteFMC = sit 
			from HIS..FMC_HIS
			where	horodate_validation = @va_Validation_in and
				numero = @va_NumEvenement_in and
				cle = @va_CleEvenement_in 

			/*A modifier l'historique de l'�v�nement clos */
			update HIS..FMC_HIS set 
				confirme_ou_signale = @va_ConfirmeSignale_in,
				autoroute = @va_NumAutoroute_in, 
				PR = @va_PR_in, 
				sens = @va_Sens_in, 
				point_caracteristique = @va_TypePtCaracteristique_in, 
				numero_point_caracteristique = @va_PtCaracteristique_in,
				position_sur_aire_ou_echangeur = @vl_Position, 
				type_FMC_valide = @va_Type_in, 
				VR = @va_VR_in, 
				VM2 = @va_VM2_in,
				VM1 = @va_VM1_in, 
				VL = @va_VL_in, 
				BAU = @va_BAU_in, 
				VR_I = @va_VR_I_in, 
				VM2_I = @va_VM2_I_in, 
				VM1_I = @va_VM1_I_in, 
				VL_I = @va_VL_I_in, 
				BAU_I = @va_BAU_I_in,
				BAU_etroite = @va_BAU_Etroite_in,
				BAU_I_etroite = @va_BAU_I_Etroite_in,
				longueur = @va_Longueur_in,
				majeur = @va_Majeur_in,
				ecoulement = @va_Ecoulement_in
			where	horodate_validation = @va_Validation_in and
				numero = @va_NumEvenement_in and
				cle = @va_CleEvenement_in 
#else
			/* date de cloture pour l'evenement */
			select @vl_Cloture = null

			/*B inserer l'historique d'un evenement a traiter */
			if not exists (select * from EXP..FMC_HIS
					where	numero = @va_NumEvenement_in and
						cle = @va_CleEvenement_in  and
						horodate_validation = @va_Validation_in)
			insert EXP..FMC_HIS ( numero, cle, confirme_ou_signale,
			horodate_validation, autoroute, PR, sens, point_caracteristique, 
			numero_point_caracteristique,position_sur_aire_ou_echangeur,
			VR, VM2, VM1, VL, BAU, VR_I, VM2_I, VM1_I, VL_I, BAU_I, 
			BAU_etroite, BAU_I_etroite, sit, 
			type_anterieur, type_FMC_valide, longueur, majeur, ecoulement)
			values( @va_NumEvenement_in, @va_CleEvenement_in, 
			@va_ConfirmeSignale_in, @va_Validation_in,
			@va_NumAutoroute_in, @va_PR_in, 
			@va_Sens_in, @va_TypePtCaracteristique_in, @va_PtCaracteristique_in,
			@vl_Position, @va_VR_in, @va_VM2_in, @va_VM1_in, @va_VL_in, 
			@va_BAU_in, @va_VR_I_in, @va_VM2_I_in, @va_VM1_I_in, @va_VL_I_in, 
			@va_BAU_I_in, @va_BAU_Etroite_in, @va_BAU_I_Etroite_in,
			@vl_Site, @va_TypeAnterieur_in, @va_Type_in, @va_Longueur_in, @va_Majeur_in, @va_Ecoulement_in)
			else	/* modifier les informations de l'evenement  */
				update EXP..FMC_HIS set 
					confirme_ou_signale = @va_ConfirmeSignale_in,
					autoroute = @va_NumAutoroute_in, 
					PR = @va_PR_in, 
					sens = @va_Sens_in, 
					point_caracteristique = @va_TypePtCaracteristique_in, 
					numero_point_caracteristique = @va_PtCaracteristique_in,
					position_sur_aire_ou_echangeur = @vl_Position, 
					type_anterieur = @va_TypeAnterieur_in, 
					type_FMC_valide = @va_Type_in, 
					VR = @va_VR_in, 
					VM2 = @va_VM2_in,
					VM1 = @va_VM1_in, 
					VL = @va_VL_in, 
					BAU = @va_BAU_in, 
					VR_I = @va_VR_I_in, 
					VM2_I = @va_VM2_I_in, 
					VM1_I = @va_VM1_I_in, 
					VL_I = @va_VL_I_in, 
					BAU_I = @va_BAU_I_in,
					BAU_etroite = @va_BAU_Etroite_in,
					BAU_I_etroite = @va_BAU_I_Etroite_in,
					longueur = @va_Longueur_in,
					majeur = @va_Majeur_in,
					ecoulement = @va_Ecoulement_in
				where	horodate_validation = @va_Validation_in and
					numero = @va_NumEvenement_in and
					cle = @va_CleEvenement_in 

		end
		else if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_CLOS
		begin
			select @vl_SiteFMC = sit 
			from EXP..FMC_HIS
			where	horodate_validation = @va_Validation_in and
				numero = @va_NumEvenement_in and
				cle = @va_CleEvenement_in 

			/*A modifier l'historique de l'�v�nement clos */
			update EXP..FMC_HIS set 
				confirme_ou_signale = @va_ConfirmeSignale_in,
				autoroute = @va_NumAutoroute_in, 
				PR = @va_PR_in, 
				sens = @va_Sens_in, 
				point_caracteristique = @va_TypePtCaracteristique_in, 
				numero_point_caracteristique = @va_PtCaracteristique_in,
				position_sur_aire_ou_echangeur = @vl_Position, 
				type_FMC_valide = @va_Type_in, 
				VR = @va_VR_in, 
				VM2 = @va_VM2_in,
				VM1 = @va_VM1_in, 
				VL = @va_VL_in, 
				BAU = @va_BAU_in, 
				VR_I = @va_VR_I_in, 
				VM2_I = @va_VM2_I_in, 
				VM1_I = @va_VM1_I_in, 
				VL_I = @va_VL_I_in, 
				BAU_I = @va_BAU_I_in,
				BAU_etroite = @va_BAU_Etroite_in,
				BAU_I_etroite = @va_BAU_I_Etroite_in,
				longueur = @va_Longueur_in,
				majeur = @va_Majeur_in,
				ecoulement = @va_Ecoulement_in
			where	horodate_validation = @va_Validation_in and
				numero = @va_NumEvenement_in and
				cle = @va_CleEvenement_in 
#endif

		end
#endif

		if @@rowcount != 1
			select @vl_Erreur = XDC_OUI
	commit tran

	if @vl_Erreur = XDC_OUI
	begin
		return XDC_NOK
	end



	 /****************************************************************************
	 **                                                                         **
	 **             Etape 3 : Transmission aux autres postes / sites            **
	 **                             Declenchement des reveils                   **
	 **                                                                         **
	 ****************************************************************************/

#ifndef PC_SIMPL
#ifndef HIST
	/*A declenchement des reveils adequates pour un evenement a traiter */
	if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_TRAI
	begin
		/* Pour une tete de bouchon, rechercher le site de traitement de la queue */
		if @va_Type_in = XZAEC_FMC_TeteBouchon
		begin
			select @vl_NumQueueBouchon = numero,
				@vl_CleQueueBouchon = cle,
				@vl_SiteQueueBouchon = sit
			from EXP..FMC_GEN
			where cause = @va_NumEvenement_in
			  and cle_cause = @va_CleEvenement_in
			  and type = XZAEC_FMC_QueueBouchon

			select	@vl_NumFmcReveillee = @vl_NumQueueBouchon,
				@vl_CleFmcReveillee = @vl_CleQueueBouchon
		end
		else
			select	@vl_NumFmcReveillee = @va_NumEvenement_in,
				@vl_CleFmcReveillee = @va_CleEvenement_in


		/* creation de la table contenant les reveils a declencher (un par poste) */

		create table #REVEILS ( poste smallint null, type tinyint null, site tinyint null )


		/*B Sur le site local, l'evenement est deja dans la liste des evenements a traiter */
		/*B Ne pas envoyer de reveil aux postes operateur voisins */
		/*B Reinitialiser simplement la liste des Fmc a traiter des autres postes operateur */
		/*B sauf pour une tete de bouchon si la queue est aussi sur le site (ou va etre creee) */
		if not ( @va_Type_in = XZAEC_FMC_TeteBouchon  and
			 (@vl_NumQueueBouchon is null  or  @vl_SiteQueueBouchon = @vl_Site) )
		begin
			declare pointeur_voisin cursor
			for select poste_enrichisseur
			from EXP..FMC_GEN, CFG..TYP_FMC
			where	EXP..FMC_GEN.sit = @vl_Site and 
				CFG..TYP_FMC.numero = EXP..FMC_GEN.type and
				CFG..TYP_FMC.classe = XZAEC_ClassePosteOper and
				EXP..FMC_GEN.fin is null and 
				EXP..FMC_GEN.poste_enrichisseur is not null and
				EXP..FMC_GEN.poste_enrichisseur != @va_PosteOperateur_in
				
			open pointeur_voisin
			fetch pointeur_voisin into @vl_PosteVoisin
				
			while @@sqlstatus = 0
			begin
				/*B reinitialiser la liste des evenements a traiter pour le poste voisin */
				exec @vl_Status = XZAR;03 0, 0, @vl_PosteVoisin, " ", @vl_Site

				if @vl_Status = null
					return XDC_PRC_INC
				/*else if @vl_Status != XDC_OK
					return @vl_Status*/
				fetch pointeur_voisin into @vl_PosteVoisin
			end
			close pointeur_voisin 
		end


		/*B recherche les sites qui ont cet evenement dans la liste des Fmc a traiter */
		/*B pour les prevenir ulterieurement d'une modification par un reveil         */
		/*B (inutile pour une tete de bouchon sauf si la queue est sur un autre site) */
		if not ( @va_Type_in = XZAEC_FMC_TeteBouchon  and
			 (@vl_NumQueueBouchon is null  or  @vl_SiteQueueBouchon = @vl_Site) )
		begin
			select @vl_TypeReveil = XZAEC_REV_FMC_MOD
			insert #REVEILS ( poste, type,site)
			select distinct
				EXP..FMC_GEN.poste_enrichisseur,
				@vl_TypeReveil,
				null
			from EXP..FMC_GEN, CFG..TYP_FMC, EXP..FMC_TRT
			where	EXP..FMC_TRT.evenement = @va_NumEvenement_in and
				EXP..FMC_TRT.cle = @va_CleEvenement_in and
				EXP..FMC_TRT.sit != @vl_Site and 
				EXP..FMC_GEN.sit = EXP..FMC_TRT.sit and 
				CFG..TYP_FMC.numero = EXP..FMC_GEN.type and
				CFG..TYP_FMC.classe = XZAEC_ClassePosteOper and
				EXP..FMC_GEN.fin is null and 
				EXP..FMC_GEN.poste_enrichisseur is not null
		end


		/*B ajouter l'evenement dans la liste des evts a traiter pour le CI */
		/*! remarque : l'ajout est realise si l'evenement n'est pas deja    */
		/*! dans la liste (et si le site local n'est pas le CI).            */

		/* initialisation variable locale */
		select @vl_SiteCI = numero from CFG..RES_DIS where type = XDC_TYPEM_SD
		if (@vl_Site != @vl_SiteCI)
		begin
			exec @vl_Status = XZAE18 @vl_SiteCI, @va_NumEvenement_in, 
						@va_CleEvenement_in,
						@vl_ResultAjoutReveil output,
						null,
						@va_NomSiteLocal_in
	
			if @vl_Status = null
				return XDC_PRC_INC
			else if @vl_Status != XDC_OK
				return @vl_Status

			/*B reveil de l'evenement au CI si l'evenement est nouveau pour le CI */
			/*B (inutile pour une tete de bouchon: la queue est ou va etre au CI) */
			if (@vl_ResultAjoutReveil = XDC_OUI) and (@va_Type_in != XZAEC_FMC_TeteBouchon)
			begin
				/* reveil en creation */
				select @vl_TypeReveil = XZAEC_REV_FMC_CRE
				insert #REVEILS values (null,@vl_TypeReveil,@vl_SiteCI)
				
				/******************
				insert #REVEILS ( poste, type,site)
				select distinct
					poste_enrichisseur,
					@vl_TypeReveil,
					null
				from EXP..FMC_GEN, CFG..TYP_FMC
				where	EXP..FMC_GEN.sit = XDC_CI and 
					CFG..TYP_FMC.numero = EXP..FMC_GEN.type and
					CFG..TYP_FMC.classe = XZAEC_ClassePosteOper and
					EXP..FMC_GEN.fin is null and 
					EXP..FMC_GEN.poste_enrichisseur is not null
					********************/
			end
		end


		/*B si l'evenement est un contre sens, ajouter     */
		/*B un reveil sur l'evenement si l'evenement est nouveau pour le */
		/*B le site de recouvrement                                      */
		if @vl_Type = XZAEC_FMC_Contresens
		begin
			/* table de stockage */
			create table #DISTRICTS (district tinyint)
		
			insert #DISTRICTS (district) 
			select distinct district_recouvrement from CFG..RES_POR
				where district!=district_recouvrement and district=@vl_District
			
			declare pointeur_district cursor
			for select district
			from #DISTRICTS
			
			open pointeur_district
			fetch pointeur_district into @vl_DistrictRecouvrement
			
			while @@sqlstatus = 0
			begin
				if @vl_DistrictRecouvrement != null 
					and @vl_DistrictRecouvrement != 0 
					and @vl_DistrictRecouvrement != @vl_District
					and (@vl_DistrictRecouvrement = XDC_DP
						or @vl_DistrictRecouvrement = XDC_VC) -- sur migrazur uniquement
				begin
					/*B ajouter l'evenement dans la liste des evts a traiter pour district de recouvrement */
					exec @vl_Status = XZAE18 @vl_DistrictRecouvrement,
								@va_NumEvenement_in, @va_CleEvenement_in,
								@vl_ResultAjoutReveil output,
								null,
								@va_NomSiteLocal_in

					if @vl_Status = null
						return XDC_PRC_INC
					else if @vl_Status != XDC_OK
						return @vl_Status

					/*B reveil de l'evenement pour le site de recouvrement si */
					/*B l'evenement est nouveau                               */
					if @vl_ResultAjoutReveil = XDC_OUI
					begin
						/* reveil pour recouvrement */
						select @vl_TypeReveil = XZAEC_REV_FMC_REC

						insert #REVEILS ( poste, type,site)
						select distinct
							poste_enrichisseur,
							@vl_TypeReveil,
							null
						from EXP..FMC_GEN, CFG..TYP_FMC
						where	EXP..FMC_GEN.sit = @vl_DistrictRecouvrement and 
							CFG..TYP_FMC.numero = EXP..FMC_GEN.type and
							CFG..TYP_FMC.classe = XZAEC_ClassePosteOper and
							EXP..FMC_GEN.fin is null and 
							EXP..FMC_GEN.poste_enrichisseur is not null
					end
				end
				/* On passe au district suivant */
				fetch pointeur_district into @vl_DistrictRecouvrement
			end
			close pointeur_district
		end
		else
		/*B sinon si l'evenement est sur une zone de recouvrement, l'ajouter   */
		/*B dans la liste des evenements a traiter sur le site de recouvrement */
		/*B puis creer un reveil sur l'evenement s'il est nouveau pour le site */
		if @vl_DistrictRecouvrement != null 
			and @vl_DistrictRecouvrement != 0 
			and @vl_DistrictRecouvrement != @vl_District
			and (@vl_DistrictRecouvrement = XDC_DP
				or @vl_DistrictRecouvrement = XDC_VC) -- sur migrazur uniquement
		begin
			/*B ajouter l'evenement dans la liste des evts a traiter pour district de recouvrement */
			exec @vl_Status = XZAE18 @vl_DistrictRecouvrement,
						@va_NumEvenement_in, @va_CleEvenement_in,
						@vl_ResultAjoutReveil output,
						null,
						@va_NomSiteLocal_in

			if @vl_Status = null
				return XDC_PRC_INC
			else if @vl_Status != XDC_OK
				return @vl_Status

			/*B reveil de l'evenement pour le site de recouvrement s'il est nouveau       */
			/*B (inutile pour une tete de bouchon sauf si la queue est sur un autre site) */
			if @vl_ResultAjoutReveil = XDC_OUI  and
				not ( @va_Type_in = XZAEC_FMC_TeteBouchon  and
					(@vl_NumQueueBouchon is null  or  @vl_SiteQueueBouchon = @vl_Site) )
			begin
				/* reveil pour recouvrement */
				select @vl_TypeReveil = XZAEC_REV_FMC_REC

				insert #REVEILS ( poste, type,site)
				select distinct
					poste_enrichisseur,
					@vl_TypeReveil,
					null
				from EXP..FMC_GEN, CFG..TYP_FMC
				where	EXP..FMC_GEN.sit = @vl_DistrictRecouvrement and 
					CFG..TYP_FMC.numero = EXP..FMC_GEN.type and
					CFG..TYP_FMC.classe = XZAEC_ClassePosteOper and
					EXP..FMC_GEN.fin is null and 
					EXP..FMC_GEN.poste_enrichisseur is not null
			end
		end

		/*B si l'evenement est dans un district, ajouter l'evenement */
		/*B dans la liste des evenements a traiter pour ce district  */
		/*B et creer un reveil sur l'evenement s'il est nouveau      */
		/*B (cas d'une validation pour un autre site -au CI-)        */
		if @vl_Site = @vl_SiteCI and @vl_District != null and @vl_District != 0
			and (@vl_District=XDC_VC or  @vl_District=XDC_DP) -- migrazur uniquement
		begin
			/*B ajouter l'evenement dans la liste des evts a traiter pour district */
			exec @vl_Status = XZAE18 @vl_District,
						@va_NumEvenement_in, @va_CleEvenement_in,
						@vl_ResultAjoutReveil output,
						null,
						@va_NomSiteLocal_in

			if @vl_Status = null
				return XDC_PRC_INC
			else if @vl_Status != XDC_OK
				return @vl_Status

			/*B reveil de l'evenement au district s'il est nouveau pour le district */
			/*B (inutile pour une tete de bouchon: geree seule au CI et qui changerait de district ?) */
			if (@vl_ResultAjoutReveil = XDC_OUI)  and  (@va_Type_in != XZAEC_FMC_TeteBouchon)
			begin
				/* reveil pour apparition sur le district */
				select @vl_TypeReveil = XZAEC_REV_FMC_DIS

				insert #REVEILS ( poste, type,site)
				select distinct
					poste_enrichisseur,
					@vl_TypeReveil,
					null
				from EXP..FMC_GEN, CFG..TYP_FMC
				where	EXP..FMC_GEN.sit = @vl_District and 
					CFG..TYP_FMC.numero = EXP..FMC_GEN.type and
					CFG..TYP_FMC.classe = XZAEC_ClassePosteOper and
					EXP..FMC_GEN.fin is null and 
					EXP..FMC_GEN.poste_enrichisseur is not null
			end
		end



		/*B declencher les reveils */
		declare Pointeur_poste cursor
		for select poste, type, site
		from #REVEILS

		open Pointeur_poste
		fetch Pointeur_poste into @vl_PosteOperateur, @vl_TypeReveil,@vl_Site
		while (@@sqlstatus = 0)
		begin
			/* texte du reveil */
			if @vl_TypeReveil = XZAEC_REV_FMC_CRE
				select @vl_TexteReveil = XDC_REV_FMC_CRE
			else if @vl_TypeReveil = XZAEC_REV_FMC_MOD
				select @vl_TexteReveil = XDC_REV_FMC_MOD
			else if @vl_TypeReveil = XZAEC_REV_FMC_FIN
				select @vl_TexteReveil = XDC_REV_FMC_FIN
			else if @vl_TypeReveil = XZAEC_REV_FMC_REC
				select @vl_TexteReveil = XDC_REV_FMC_REC
			else select @vl_TexteReveil = XZAEC_REV_FMC_DIS_NOM

			/* marquer le texte urgent selon l'importance de l'evenement */
			if @va_Type_in in (XZAEC_FMC_Accident, XZAEC_FMC_Contresens)
				select @vl_TexteReveil = XDC_REV_URGENT + @vl_TexteReveil

			/* declencher le reveil */
			exec @vl_Status = XZAR;03 @vl_NumFmcReveillee, @vl_CleFmcReveillee,
						@vl_PosteOperateur, @vl_TexteReveil,@vl_Site
			if @vl_Status = null
				return XDC_PRC_INC
			else if @vl_Status != XDC_OK
				return @vl_Status

			/* machine suivante a prevenir */
			fetch Pointeur_poste into @vl_PosteOperateur, @vl_TypeReveil,@vl_Site
		end

		close Pointeur_poste
	end
#endif
#endif
	return XDC_OK
go
