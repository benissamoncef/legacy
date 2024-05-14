/*E*/
/*  Fichier : $Id: xzae49sp.prc,v 1.13 2020/06/02 21:23:23 devgfi Exp $      $Revision: 1.13 $        $Date: 2020/06/02 21:23:23 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae49sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	31/08/94	: Creation
* C.T.	12/08/94	: Modif position param entree
*                         (test ne fonctionne plus)
* C.T.  10/11/94	: Modif bugs applix V 1.4
* C.T.	14/11/94	: Modif recherche des infos au CI (V 1.5)
* B.G.	17/01/95	: modif date nulle (V 1.6)
* C.T.	04/07/95	: Pas de calcul de longueur si pas de 
*                         tete de localisation valuee (V 1.7)
* C.T.	10/07/95	: Correction suite pas de tete de 
*                         localisation valuee (V 1.8)
* JMG	11/02/97	: neutrisations de voies (DEM/1394) ANA64 1.9
* C.P.	17/06/98	: Modif separation EXP et HIS (V 1.10)
* VR	05/10/11	: Ajout vitesse v1.2 (DEM/1015)
* JMG	08/03/18	: ajout personnel 1.12
* JPL	06/09/19	: Ajout de l'indicateur de bouchon (mobile) (DEM 1346)  1.13
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs d'une fiche main courante travaux a l'heure precisee
* 
* Sequence d'appel
* SP	XZAE49_Lire_Fiche_Travaux
* 
* Arguments en entree
* XDY_Horodate		va_Horodate_in : par defaut heure courante (non utilisee)
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* 
* Arguments en sortie
* XDY_Booleen		va_Nature_out
* XDY_Octet		va_Type_out
* XDY_Booleen		va_Continu_out
* XDY_Horodate		va_HoroDebC_out
* XDY_Horodate		va_HoroFinC_out
* XDY_Booleen		va_Mobile_out
* XDY_Autoroute		va_Autoroute_out
* XDY_PR		va_PRteteM_out
* XDY_PR		va_PRqueueM_out
* XDY_Sens		va_Sens_out
* XDY_PR		va_PRtete_out
* XDY_Booleen		va_VoieRet_out
* XDY_Entier		va_Longueur_out
* XDY_Entier		va_LongueurM_out
* XDY_Octet             va_VR_out
* XDY_Octet             va_VM2_out
* XDY_Octet             va_VM1_out
* XDY_Octet             va_VL_out
* XDY_Octet             va_BAU_out
* XDY_Octet             va_VR_I_out
* XDY_Octet             va_VM2_I_out
* XDY_Octet             va_VM1_I_out
* XDY_Octet             va_VL_I_out
* XDY_Octet             va_BAU_I_out
* XDY_PR                va_PRdebut_VR_out
* XDY_PR                va_PRfin_VR_out
* XDY_Horodate          va_debut_VR_out
* XDY_Horodate          va_fin_VR_out
* XDY_PR                va_PRdebut_VM2_out
* XDY_PR                va_PRfin_VM2_out
* XDY_Horodate          va_debut_VM2_out
* XDY_Horodate          va_fin_VM2_out
* XDY_PR                va_PRdebut_VM1_out
* XDY_PR                va_PRfin_VM1_out
* XDY_Horodate          va_debut_VM1_out
* XDY_Horodate          va_fin_VM1_out
* XDY_PR                va_PRdebut_VL_out
* XDY_PR                va_PRfin_VL_out
* XDY_Horodate          va_debut_VL_out
* XDY_Horodate          va_fin_VL_out
* XDY_PR                va_PRdebut_BAU_out
* XDY_PR                va_PRfin_BAU_out
* XDY_Horodate          va_debut_BAU_out
* XDY_Horodate          va_fin_BAU_out
* XDY_PR                va_PRdebut_VR_I_out
* XDY_PR                va_PRfin_VR_I_out
* XDY_Horodate          va_debut_VR_I_out
* XDY_Horodate          va_fin_VR_I_out
* XDY_PR                va_PRdebut_VM2_I_out
* XDY_PR                va_PRfin_VM2_I_out
* XDY_Horodate          va_debut_VM2_I_out
* XDY_Horodate          va_fin_VM2_I_out
* XDY_PR                va_PRdebut_VM1_I_out
* XDY_PR                va_PRfin_VM1_I_out
* XDY_Horodate          va_debut_VM1_I_out
* XDY_Horodate          va_fin_VM1_I_out
* XDY_PR                va_PRdebut_VL_I_out
* XDY_PR                va_PRfin_VL_I_out
* XDY_Horodate          va_debut_VL_I_out
* XDY_Horodate          va_fin_VL_I_out
* XDY_PR                va_PRdebut_BAU_I_out
* XDY_PR                va_PRfin_BAU_I_out
* XDY_Horodate          va_debut_BAU_I_out
* XDY_Horodate          va_fin_BAU_I_out
* XDY_Octet             va_vitesse_out
* XDY_Octet             va_personnel_out
* XDY_Octet             va_bouchon_out
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de complement de fiche main courante
* XDC_ARG_INV
* < 0 erreur sybase
* 
* Conditions d'utilisation
* En formation, preciser l'horodate
* 
* La procedure stockee au CI recherche les informations 
* dans la base HIS si la date donnee est anterieure de plus d'un 
* mois tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Le code de creation de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* et du test de la date en entree (< 1 mois)
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
* 
* 
* Fonction
* Select dans la table TRAVAUX et dans la table TRAVAUX_HISTORIQUE
* where numero evenement and horodate
-----------------------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAE49' and type = 'P')
	drop procedure XZAE49
go


create procedure XZAE49
	@va_Horodate_in		datetime = null, -- par defaut heure courante
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null,
	@va_Nature_out		bit output,
	@va_Type_out		tinyint = null 	output,
	@va_Continu_out		bit 			output,
	@va_HoroDebC_out	datetime = null 	output,
	@va_HoroFinC_out	datetime = null 	output,
	@va_Mobile_out		bit 			output,
	@va_Autoroute_out	T_AUTOROUTE = null 	output,
	@va_PRteteM_out		T_PR = null 		output,
	@va_PRqueueM_out	T_PR = null 		output,
	@va_SensM_out		T_SENS = null 		output,
	@va_PRtete_out		T_PR = null 		output,
	@va_VoiesRet_out	bit 			output,
	@va_Longueur_out	int = null  		output,
	@va_LongueurM_out	int = null	  	output,
	@va_VR_out              T_VOIE  = null  output,
	@va_VM2_out             T_VOIE  = null  output,
	@va_VM1_out             T_VOIE  = null  output,
	@va_VL_out              T_VOIE  = null  output,
	@va_BAU_out             T_VOIE  = null  output,
	@va_VR_I_out            T_VOIE  = null  output,
	@va_VM2_I_out           T_VOIE  = null  output,
	@va_VM1_I_out           T_VOIE  = null  output,
	@va_VL_I_out            T_VOIE  = null  output,
	@va_BAU_I_out           T_VOIE  = null  output,
	@va_PRdebut_VR_out      T_PR = null     output,
	@va_PRfin_VR_out        T_PR = null     output,
	@va_debut_VR_out        datetime = null output,
	@va_fin_VR_out          datetime = null output,
	@va_PRdebut_VM2_out     T_PR = null     output,
	@va_PRfin_VM2_out       T_PR = null     output,
	@va_debut_VM2_out       datetime = null output,
	@va_fin_VM2_out         datetime = null output,
	@va_PRdebut_VM1_out     T_PR = null     output,
	@va_PRfin_VM1_out       T_PR = null     output,
	@va_debut_VM1_out       datetime = null output,
	@va_fin_VM1_out         datetime = null output,
	@va_PRdebut_VL_out      T_PR = null     output,
	@va_PRfin_VL_out        T_PR = null     output,
	@va_debut_VL_out        datetime = null output,
	@va_fin_VL_out          datetime = null output,
	@va_PRdebut_BAU_out     T_PR = null     output,
	@va_PRfin_BAU_out       T_PR = null     output,
	@va_debut_BAU_out       datetime = null output,
	@va_fin_BAU_out         datetime = null output,
        @va_PRdebut_VR_I_out    T_PR = null     output,
	@va_PRfin_VR_I_out      T_PR = null     output,
	@va_debut_VR_I_out      datetime = null output,
	@va_fin_VR_I_out        datetime = null output,
	@va_PRdebut_VM2_I_out   T_PR = null     output,
	@va_PRfin_VM2_I_out     T_PR = null     output,
	@va_debut_VM2_I_out     datetime = null output,
	@va_fin_VM2_I_out       datetime = null output,
	@va_PRdebut_VM1_I_out   T_PR = null     output,
	@va_PRfin_VM1_I_out     T_PR = null     output,
	@va_debut_VM1_I_out     datetime = null output,
	@va_fin_VM1_I_out       datetime = null output,
	@va_PRdebut_VL_I_out    T_PR = null     output,
	@va_PRfin_VL_I_out      T_PR = null     output,
	@va_debut_VL_I_out      datetime = null output,
	@va_fin_VL_I_out        datetime = null output,
	@va_PRdebut_BAU_I_out   T_PR = null     output,
	@va_PRfin_BAU_I_out     T_PR = null     output,
	@va_debut_BAU_I_out     datetime = null output,
	@va_fin_BAU_I_out       datetime = null output,
	@va_vitesse_out		tinyint = null 	output,
	@va_personnel_out	tinyint = null  output,
	@va_bouchon_out		tinyint = null  output
as
	declare @vl_Autoroute	T_AUTOROUTE,
		@vl_PRqueue	T_PR,
		@vl_Status	int,
		@vl_Horodate	datetime

	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/* initialisation des variables de sortie (indispensable pour celles de type bit) */
	select	@va_Nature_out		= XDC_NON,
		@va_Type_out		= null,
		@va_Continu_out		= XDC_NON,
		@va_HoroDebC_out	= null,
		@va_HoroFinC_out	= null,
		@va_Mobile_out		= XDC_NON,
		@va_Autoroute_out	= null,
		@va_PRteteM_out		= null,
		@va_PRqueueM_out	= null,
		@va_SensM_out		= null,
		@va_PRtete_out		= null,
		@va_VoiesRet_out	= XDC_NON,
		@va_Longueur_out	= null ,
		@va_LongueurM_out	= null,
		@va_vitesse_out		= null,
		@va_personnel_out	 = null

	/*A test les parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV

	/*A prendre l'heure courante dans le cas ou elle est nulle */
	if @va_Horodate_in = null
		select @va_Horodate_in = XDC_DATE_INFINIE

#ifdef HIST
		/*B recherche la derniere date correspondante a l'heure donnee */
		select @vl_Horodate = max(horodate) from HIS..FMC_TRH
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in and
			horodate <= @va_Horodate_in
	
		if @@rowcount = 0
			return XDC_NOK
	
		/*B lecture des infos sur des travaux */
		select
			@va_Nature_out		= HIS..FMC_TRF.nature,
			@va_Type_out		= HIS..FMC_TRF.type,
			@va_Continu_out		= HIS..FMC_TRF.continu,
			@va_HoroDebC_out	= HIS..FMC_TRF.horodate_debut_planifiee,
			@va_HoroFinC_out	= HIS..FMC_TRF.horodate_fin_planifiee,
			@va_Mobile_out		= HIS..FMC_TRF.mobile,
			@va_Autoroute_out	= HIS..FMC_TRF.autoroute_planifiee,
			@va_PRteteM_out		= HIS..FMC_TRF.PR_tete_planifie,
			@va_PRqueueM_out	= HIS..FMC_TRF.PR_queue_planifie,
			@va_SensM_out		= HIS..FMC_TRF.sens_planifie,
			@va_PRtete_out		= HIS..FMC_TRH.localisation_tete,
			@va_VoiesRet_out	= HIS..FMC_TRH.voies_retrecies,
			@va_VR_out              = VR,
			@va_VM2_out             = VM2,
			@va_VM1_out             = VM1,
			@va_VL_out              = VL,
			@va_BAU_out             = BAU,
			@va_VR_I_out            = VR_I,
			@va_VM2_I_out           = VM2_I,
			@va_VM1_I_out           = VM1_I,
			@va_VL_I_out            = VL_I,
			@va_BAU_I_out           = BAU_I,
			@va_PRdebut_VR_out      = PR_debut_VR,
			@va_PRfin_VR_out        = PR_fin_VR,
			@va_debut_VR_out        = debut_VR,
			@va_fin_VR_out          = fin_VR,
			@va_PRdebut_VM2_out     = PR_debut_VM2,
			@va_PRfin_VM2_out       = PR_fin_VM2,
			@va_debut_VM2_out       = debut_VM2,
			@va_fin_VM2_out         = fin_VM2,
			@va_PRdebut_VM1_out     = PR_debut_VM1,
			@va_PRfin_VM1_out       = PR_fin_VM1,
			@va_debut_VM1_out       = debut_VM1,
			@va_fin_VM1_out         = fin_VM1,
			@va_PRdebut_VL_out      = PR_debut_VL,
			@va_PRfin_VL_out        = PR_fin_VL,
			@va_debut_VL_out        = debut_VL,
			@va_fin_VL_out          = fin_VL,
			@va_PRdebut_BAU_out     = PR_debut_BAU,
			@va_PRfin_BAU_out       = PR_fin_BAU,
			@va_debut_BAU_out       = debut_BAU,
			@va_fin_BAU_out         = fin_BAU,
			@va_PRdebut_VR_I_out    = PR_debut_VR_I,
			@va_PRfin_VR_I_out      = PR_fin_VR_I,
			@va_debut_VR_I_out      = debut_VR_I,
			@va_fin_VR_I_out        = fin_VR_I,
			@va_PRdebut_VM2_I_out   = PR_debut_VM2_I,
			@va_PRfin_VM2_I_out     = PR_fin_VM2_I,
			@va_debut_VM2_I_out     = debut_VM2_I,
			@va_fin_VM2_I_out       = fin_VM2_I,
			@va_PRdebut_VM1_I_out   = PR_debut_VM1_I,
			@va_PRfin_VM1_I_out     = PR_fin_VM1_I,
			@va_debut_VM1_I_out     = debut_VM1_I,
			@va_fin_VM1_I_out       = fin_VM1_I,
			@va_PRdebut_VL_I_out    = PR_debut_VL_I,
			@va_PRfin_VL_I_out      = PR_fin_VL_I,
			@va_debut_VL_I_out      = debut_VL_I,
			@va_fin_VL_I_out        = fin_VL_I,
			@va_PRdebut_BAU_I_out   = PR_debut_BAU_I,
			@va_PRfin_BAU_I_out     = PR_fin_BAU_I,
			@va_debut_BAU_I_out     = debut_BAU_I,
			@va_fin_BAU_I_out       = fin_BAU_I,
			@va_vitesse_out		= HIS..FMC_TRH.vitesse,
			@va_personnel_out	= HIS..FMC_TRH.personnel,
			@va_bouchon_out		= HIS..FMC_TRH.bouchon
		from HIS..FMC_TRF, HIS..FMC_TRH
		where 	HIS..FMC_TRF.numero = @va_NumEvt_in and 
			HIS..FMC_TRF.cle = @va_CleEvt_in and
		 	HIS..FMC_TRH.numero = @va_NumEvt_in and 
			HIS..FMC_TRH.cle = @va_CleEvt_in and
			HIS..FMC_TRH.horodate = @vl_Horodate
	
		if @@rowcount = 0
			return XDC_NOK
	
		/*B recherche du PR de fin de travaux */
		select  @vl_PRqueue = PR, @vl_Autoroute = autoroute 
		from HIS..FMC_HIS
		where	numero = @va_NumEvt_in and 
			cle = @va_CleEvt_in and
			horodate_validation = @vl_Horodate
#else
#ifdef CI
	/*! cas ou la procedure est creee au CI */

	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	/*A la base EXP (au niveau du CI)                       */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAE49
			@va_Horodate_in	, @va_NumEvt_in	, @va_CleEvt_in	, @va_Nature_out output, 
			@va_Type_out output, @va_Continu_out output, @va_HoroDebC_out output,
			@va_HoroFinC_out output, @va_Mobile_out output, @va_Autoroute_out output, 
			@va_PRteteM_out output, @va_PRqueueM_out output, @va_SensM_out output, @va_PRtete_out output,
			@va_VoiesRet_out output, @va_Longueur_out output, @va_LongueurM_out output, 
			@va_VR_out output , @va_VM2_out output, @va_VM1_out output, @va_VL_out output, @va_BAU_out output,
			@va_VR_I_out output, @va_VM2_I_out output, @va_VM1_I_out output, @va_VL_I_out output, 
			@va_BAU_I_out output, @va_PRdebut_VR_out output, @va_PRfin_VR_out output,
			@va_debut_VR_out output, @va_fin_VR_out output, @va_PRdebut_VM2_out output, 
			@va_PRfin_VM2_out output, @va_debut_VM2_out output, @va_fin_VM2_out output, @va_PRdebut_VM1_out output, 
			@va_PRfin_VM1_out output, @va_debut_VM1_out output, @va_fin_VM1_out output, @va_PRdebut_VL_out output, 
			@va_PRfin_VL_out output, @va_debut_VL_out output, @va_fin_VL_out output,
			@va_PRdebut_BAU_out output, @va_PRfin_BAU_out output, @va_debut_BAU_out output, 
			@va_fin_BAU_out output, @va_PRdebut_VR_I_out output, @va_PRfin_VR_I_out output,
			@va_debut_VR_I_out output, @va_fin_VR_I_out output, @va_PRdebut_VM2_I_out output, 
			@va_PRfin_VM2_I_out output, @va_debut_VM2_I_out output, @va_fin_VM2_I_out output, 
			@va_PRdebut_VM1_I_out output, @va_PRfin_VM1_I_out output, @va_debut_VM1_I_out output,
			@va_fin_VM1_I_out output , @va_PRdebut_VL_I_out output, @va_PRfin_VL_I_out output,
			@va_debut_VL_I_out output, @va_fin_VL_I_out output, @va_PRdebut_BAU_I_out output, 
			@va_PRfin_BAU_I_out output, @va_debut_BAU_I_out output, @va_fin_BAU_I_out output,@va_vitesse_out output,
			@va_personnel_out output, @va_bouchon_out output
 		if @vl_Status = null
                       	 return XDC_PRC_INC
                else return @vl_Status
	end
	else
	begin
		/*B recherche la derniere date correspondante a l'heure donnee */
		select @vl_Horodate = max(horodate) from EXP..FMC_TRH
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in and
			horodate <= @va_Horodate_in
	
		if @@rowcount = 0
			return XDC_NOK
	
		/*B lecture des infos sur des travaux */
		select
			@va_Nature_out		= EXP..FMC_TRF.nature,
			@va_Type_out		= EXP..FMC_TRF.type,
			@va_Continu_out		= EXP..FMC_TRF.continu,
			@va_HoroDebC_out	= EXP..FMC_TRF.horodate_debut_planifiee,
			@va_HoroFinC_out	= EXP..FMC_TRF.horodate_fin_planifiee,
			@va_Mobile_out		= EXP..FMC_TRF.mobile,
			@va_Autoroute_out	= EXP..FMC_TRF.autoroute_planifiee,
			@va_PRteteM_out		= EXP..FMC_TRF.PR_tete_planifie,
			@va_PRqueueM_out	= EXP..FMC_TRF.PR_queue_planifie,
			@va_SensM_out		= EXP..FMC_TRF.sens_planifie,
			@va_PRtete_out		= EXP..FMC_TRH.localisation_tete,
			@va_VoiesRet_out	= EXP..FMC_TRH.voies_retrecies,
			@va_VR_out              = VR,
			@va_VM2_out             = VM2,
			@va_VM1_out             = VM1,
			@va_VL_out              = VL,
			@va_BAU_out             = BAU,
			@va_VR_I_out            = VR_I,
			@va_VM2_I_out           = VM2_I,
			@va_VM1_I_out           = VM1_I,
			@va_VL_I_out            = VL_I,
			@va_BAU_I_out           = BAU_I,
			@va_PRdebut_VR_out      = PR_debut_VR,
			@va_PRfin_VR_out        = PR_fin_VR,
			@va_debut_VR_out        = debut_VR,
			@va_fin_VR_out          = fin_VR,
			@va_PRdebut_VM2_out     = PR_debut_VM2,
			@va_PRfin_VM2_out       = PR_fin_VM2,
			@va_debut_VM2_out       = debut_VM2,
			@va_fin_VM2_out         = fin_VM2,
			@va_PRdebut_VM1_out     = PR_debut_VM1,
			@va_PRfin_VM1_out       = PR_fin_VM1,
			@va_debut_VM1_out       = debut_VM1,
			@va_fin_VM1_out         = fin_VM1,
			@va_PRdebut_VL_out      = PR_debut_VL,
			@va_PRfin_VL_out        = PR_fin_VL,
			@va_debut_VL_out        = debut_VL,
			@va_fin_VL_out          = fin_VL,
			@va_PRdebut_BAU_out     = PR_debut_BAU,
			@va_PRfin_BAU_out       = PR_fin_BAU,
			@va_debut_BAU_out       = debut_BAU,
			@va_fin_BAU_out         = fin_BAU,
			@va_PRdebut_VR_I_out    = PR_debut_VR_I,
			@va_PRfin_VR_I_out      = PR_fin_VR_I,
			@va_debut_VR_I_out      = debut_VR_I,
			@va_fin_VR_I_out        = fin_VR_I,
			@va_PRdebut_VM2_I_out   = PR_debut_VM2_I,
			@va_PRfin_VM2_I_out     = PR_fin_VM2_I,
			@va_debut_VM2_I_out     = debut_VM2_I,
			@va_fin_VM2_I_out       = fin_VM2_I,
			@va_PRdebut_VM1_I_out   = PR_debut_VM1_I,
			@va_PRfin_VM1_I_out     = PR_fin_VM1_I,
			@va_debut_VM1_I_out     = debut_VM1_I,
			@va_fin_VM1_I_out       = fin_VM1_I,
			@va_PRdebut_VL_I_out    = PR_debut_VL_I,
			@va_PRfin_VL_I_out      = PR_fin_VL_I,
			@va_debut_VL_I_out      = debut_VL_I,
			@va_fin_VL_I_out        = fin_VL_I,
			@va_PRdebut_BAU_I_out   = PR_debut_BAU_I,
			@va_PRfin_BAU_I_out     = PR_fin_BAU_I,
			@va_debut_BAU_I_out     = debut_BAU_I,
			@va_fin_BAU_I_out       = fin_BAU_I,
			@va_vitesse_out		= EXP..FMC_TRH.vitesse,
			@va_personnel_out	= EXP..FMC_TRH.personnel,
			@va_bouchon_out		= EXP..FMC_TRH.bouchon
		from EXP..FMC_TRF, EXP..FMC_TRH
		where 	EXP..FMC_TRF.numero = @va_NumEvt_in and 
			EXP..FMC_TRF.cle = @va_CleEvt_in and
		 	EXP..FMC_TRH.numero = @va_NumEvt_in and 
			EXP..FMC_TRH.cle = @va_CleEvt_in and
			EXP..FMC_TRH.horodate = @vl_Horodate
	
		if @@rowcount = 0
			return XDC_NOK
	
		/*B recherche du PR de fin de travaux */
		select  @vl_PRqueue = PR, @vl_Autoroute = autoroute 
		from EXP..FMC_HIS
		where	numero = @va_NumEvt_in and 
			cle = @va_CleEvt_in and
			horodate_validation = @vl_Horodate
	end
#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

	/*A recherche la derniere date correspondante a l'heure donnee */
	select @vl_Horodate = max(horodate) from EXP..FMC_TRH
	where	numero = @va_NumEvt_in and cle = @va_CleEvt_in and
		horodate <= @va_Horodate_in

	if @@rowcount = 0
		return XDC_NOK

	/*A lecture des infos sur des travaux */
	select
		@va_Nature_out		= EXP..FMC_TRF.nature,
		@va_Type_out		= EXP..FMC_TRF.type,
		@va_Continu_out		= EXP..FMC_TRF.continu,
		@va_HoroDebC_out	= EXP..FMC_TRF.horodate_debut_planifiee,
		@va_HoroFinC_out	= EXP..FMC_TRF.horodate_fin_planifiee,
		@va_Mobile_out		= EXP..FMC_TRF.mobile,
		@va_Autoroute_out	= EXP..FMC_TRF.autoroute_planifiee,
		@va_PRteteM_out		= EXP..FMC_TRF.PR_tete_planifie,
		@va_PRqueueM_out	= EXP..FMC_TRF.PR_queue_planifie,
		@va_SensM_out		= EXP..FMC_TRF.sens_planifie,
		@va_PRtete_out		= EXP..FMC_TRH.localisation_tete,
		@va_VoiesRet_out	= EXP..FMC_TRH.voies_retrecies,
		@va_VR_out              = VR,
		@va_VM2_out             = VM2,
		@va_VM1_out             = VM1,
		@va_VL_out              = VL,
		@va_BAU_out             = BAU,
		@va_VR_I_out            = VR_I,
		@va_VM2_I_out           = VM2_I,
		@va_VM1_I_out           = VM1_I,
		@va_VL_I_out            = VL_I,
		@va_BAU_I_out           = BAU_I,
		@va_PRdebut_VR_out      = PR_debut_VR,
		@va_PRfin_VR_out        = PR_fin_VR,
		@va_debut_VR_out        = debut_VR,
		@va_fin_VR_out          = fin_VR,
		@va_PRdebut_VM2_out     = PR_debut_VM2,
		@va_PRfin_VM2_out       = PR_fin_VM2,
		@va_debut_VM2_out       = debut_VM2,
		@va_fin_VM2_out         = fin_VM2,
		@va_PRdebut_VM1_out     = PR_debut_VM1,
		@va_PRfin_VM1_out       = PR_fin_VM1,
		@va_debut_VM1_out       = debut_VM1,
		@va_fin_VM1_out         = fin_VM1,
		@va_PRdebut_VL_out      = PR_debut_VL,
		@va_PRfin_VL_out        = PR_fin_VL,
		@va_debut_VL_out        = debut_VL,
		@va_fin_VL_out          = fin_VL,
		@va_PRdebut_BAU_out     = PR_debut_BAU,
		@va_PRfin_BAU_out       = PR_fin_BAU,
		@va_debut_BAU_out       = debut_BAU,
		@va_fin_BAU_out         = fin_BAU,
		@va_PRdebut_VR_I_out    = PR_debut_VR_I,
		@va_PRfin_VR_I_out      = PR_fin_VR_I,
		@va_debut_VR_I_out      = debut_VR_I,
		@va_fin_VR_I_out        = fin_VR_I,
		@va_PRdebut_VM2_I_out   = PR_debut_VM2_I,
		@va_PRfin_VM2_I_out     = PR_fin_VM2_I,
		@va_debut_VM2_I_out     = debut_VM2_I,
		@va_fin_VM2_I_out       = fin_VM2_I,
		@va_PRdebut_VM1_I_out   = PR_debut_VM1_I,
		@va_PRfin_VM1_I_out     = PR_fin_VM1_I,
		@va_debut_VM1_I_out     = debut_VM1_I,
		@va_fin_VM1_I_out       = fin_VM1_I,
		@va_PRdebut_VL_I_out    = PR_debut_VL_I,
		@va_PRfin_VL_I_out      = PR_fin_VL_I,
		@va_debut_VL_I_out      = debut_VL_I,
		@va_fin_VL_I_out        = fin_VL_I,
		@va_PRdebut_BAU_I_out   = PR_debut_BAU_I,
		@va_PRfin_BAU_I_out     = PR_fin_BAU_I,
		@va_debut_BAU_I_out     = debut_BAU_I,
		@va_fin_BAU_I_out       = fin_BAU_I,
		@va_vitesse_out		= EXP..FMC_TRH.vitesse,
		@va_personnel_out	= EXP..FMC_TRH.personnel,
		@va_bouchon_out		= EXP..FMC_TRH.bouchon
	from EXP..FMC_TRF, EXP..FMC_TRH
	where 	EXP..FMC_TRF.numero = @va_NumEvt_in and 
		EXP..FMC_TRF.cle = @va_CleEvt_in and
	 	EXP..FMC_TRH.numero = @va_NumEvt_in and 
		EXP..FMC_TRH.cle = @va_CleEvt_in and
		EXP..FMC_TRH.horodate = @vl_Horodate

	if @@rowcount = 0
		return XDC_NOK

	/*A recherche du PR de fin de travaux */
	select  @vl_PRqueue = PR, @vl_Autoroute = autoroute from EXP..FMC_HIS
	where	EXP..FMC_HIS.numero = @va_NumEvt_in and 
		EXP..FMC_HIS.cle = @va_CleEvt_in and
		EXP..FMC_HIS.horodate_validation = @vl_Horodate
#endif
#endif

	if @@rowcount = 0
		return XDC_NOK

	/*A calcul de la longueur des travaux mobiles     */
	/*! attention : travaux sur une seule autoroute   */
	if @va_Mobile_out = XDC_OUI
	begin
		exec @vl_Status = XZAO;22 	@va_Autoroute_out, 
						@va_PRteteM_out, 
						@va_Autoroute_out, 
						@va_PRqueueM_out, 
				 		@va_LongueurM_out output
	
		/*B test si la procedure s'est correctement deroulee */
		if @vl_Status != XDC_OK
			return @vl_Status	/*! voir si on retourne ce status ou XDC_NOK */
	end

	/*A calcul la longueur des travaux non mobiles */
	if @va_PRtete_out is not null
	begin
		exec @vl_Status = XZAO;22 	@vl_Autoroute,
						@va_PRtete_out, 
						@vl_Autoroute, 
						@vl_PRqueue,
			 			@va_Longueur_out output

		/*A test si la procedure s'est correctement deroulee */
		if @vl_Status = null
			return XDC_PRC_INC
		else if @vl_Status != XDC_OK
			return @vl_Status

	end
	else	select @va_Longueur_out = null


	return XDC_OK
go
