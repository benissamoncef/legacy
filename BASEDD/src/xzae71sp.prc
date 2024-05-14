/*E*/
/*  Fichier : $Id: xzae71sp.prc,v 1.28 2017/06/20 16:38:18 gesconf Exp $      Release : $Revision: 1.28 $        Date : $Date: 2017/06/20 16:38:18 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae71sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	05/10/94	: Creation		V 1.1
* C.T.	10/11/94	: Modif bugs applix	(V 1.2)
* C.T.	14/11/94	: Modif recherche des infos au CI (V 1.3)
* C.T.  17/01/95	: Cas de criteres districts = 0   (V 1.5)
* C.T.  05/05/95	: Retourner la localisation de l'equipement pour
*                         les actions sur equipements (V 1.6)
* C.T.  02/05/95	: Correction erreur evt lors de FMC valuee (V 1.7)
*                         et remplacement de la cause de la queue de bouchon
*                         par celle de la tete de bouchon
* C.T.  03/09/95	: Correction erreur evt lors de FMC valuee (V 1.8)
* C.T.  02/10/95	: Correction recherche actions de bouchon lors de fmc valuee (V 1.9-1.10)
* C.T.  02/10/95	: Correction insert fmc de fmc valuee (V 1.11)
* B.G.  02/10/95        : Correction pb de compil dans district (HIS..FMC_GEN) (1.12)
* C.T.  10/10/95	: Correction recherche fmc (V 1.13)
* C.T.  31/01/96	: Correction recherche fmc sur tous les districts(V 1.14-1.15)
*                         limite a 30 types pour le critere 1
* C.T.  09/02/96	: Correction recherche fmc si horodate debut non renseignee (V 1.16-1.17)
* C.T.	23/02/98	: Modif default suite a passage en version SQL 11.0.3.2 (1.18)
* P.N.	05/02/99	: modif intergest. dem/1724 v1.20
* P.N.  17/05/99	: correct dem/1749 v1.21
* Hilmarcher 23/02/05   : modif ecriture TAB + # car pb a la compil DEM423 v1.22
* LCL   09/07/07        : suppression DY 1.23
* JMG   10/07/07        : suppression DY 1.24
* JMG           02/10/09        : secto DEM887 1.25
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.27 1.28
* ABK	29/09/2023		: Ajout district CA DEM-483 + suppression des anciens districts
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAE71_Historique_Exploitation
* 
* Arguments en entree
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* XDY_Horodate		va_HoroDebut_in
* XDY_Horodate		va_HoroFin_in
* XDY_Entier		va_Districts_in		: districts selectionnes
* XDY_Entier		va_TypesAction1_in	: types d'actions selectionnes
* XDY_Entier		va_TypesAction2_in	: types d'actions selectionnes
* XDY_Entier		va_TypesEvt1_in		: types d'evenements selectionnes
* XDY_Entier		va_TypesEvt2_in		: types d'evenements selectionnes
* 
* Arguments en sortie
* Liste des actions, Evenements ou operations trouves :
* XDY_Entier		va_Numero_in
* XDY_Octet		va_Cle_in
* XDY_Mot		va_Type_in
* XDY_Entier		va_Cause_in
* XDY_Octet		va_CleCause_in
* XDY_Horodate		va_HoroDebut_in
* XDY_Horodate		va_HoroFin_in
* XDY_Autoroute		va_Autoroute_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_Booleen		va_Categorie_in (XDC_HIS_EXP_EVT ou XDC_HIS_EXP_ACT)
* 
* Service rendu
* Retourne l'historique de l'exploitation
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametres d'entree null
* 
* Conditions d'utilisation
* HoroFin est obligatoire.
* Tous les resultats sont retournes dans une liste 
* Critere de selection de districts : code sur 8 bits (bit a 1 : selection)
* se referer au XDC_HIS_EXP_.. pour la position du bit en fonction du site
* Critere de selection d'actions : code sur 15 bits (se referer a XDC_ACT_...
* pour la position du bit en fonction du type d'action)
* Critere de selection de types d'evenements : code sur 32 bits (pour va_TypesEvt1_in)
* et sur 7 bits (pour va_TypesEvt2_in) la position du bit en fonction du type 
* est donne par XZAEC_FMC_..(type)..  (ex : type accident est code sur le
* bit 0 de va_TypesEvt1_in).
* 
* La procedure stockee au CI recherche les informations 
* dans la base HIS si la date donnee est anterieure de plus d'un 
* mois tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Le code de creation de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
* 
* 
* Fonction
---------------------------------------------------------------*/

use PRC
go
drop proc XZAE71
go
create procedure XZAE71
	@va_NumEvenement_in	int	= null,
	@va_CleEvenement_in	tinyint	= null,
	@va_HoroDebut_in	datetime = null,
	@va_HoroFin_in		datetime = null,
	@va_Districts_in	int = null,
	@va_TypesAction1_in	int = null,
	@va_TypesAction2_in	int = null,
	@va_TypesEvt1_in	int = null,
	@va_TypesEvt2_in	int = null
as
	declare @vl_CritereCI bit, @vl_CritereVC bit,
		@vl_CritereDP bit, @vl_NumEvt int, 
		@vl_CleEvt tinyint, @vl_Type smallint, @vl_Status int,
		@vl_NbTypes2 int, @vl_Cause int, @vl_CleCause tinyint,
		@vl_Cause2 int, @vl_CleCause2 tinyint,
		@vl_Mode tinyint, @vl_DateCourante datetime,
		@vl_SiteMaitre tinyint

	/* initialisation variable locale */
	select @vl_SiteMaitre = numero from CFG..RES_DIS where type = XDC_TYPEM_SD

	/*! transfo en parametres nulls bug applix */
	if @va_NumEvenement_in = 0 select @va_NumEvenement_in = null
	if @va_CleEvenement_in = 0 select @va_CleEvenement_in = null
	if @va_HoroDebut_in = XDC_DATE_NULLE select @va_HoroDebut_in = null
	if @va_HoroFin_in = XDC_DATE_NULLE select @va_HoroFin_in = null

	/*A Controle le parametre obligatoire */
	if @va_HoroFin_in = null or
	   (@va_CleEvenement_in is not null and @va_NumEvenement_in is null) or
	   (@va_CleEvenement_in is null and @va_NumEvenement_in is not null) or
	   @va_Districts_in = 0 or
	   (@va_TypesEvt1_in = 0 and @va_TypesEvt2_in = 0 and 
	    @va_TypesAction1_in = 0 and @va_TypesAction2_in = 0)
		return XDC_ARG_INV

	/*A calcul des districts de localisation */
	if @va_Districts_in = null
		/*A tous les districts sont selectionnes */
		select	@vl_CritereCI = XDC_OUI,
			@vl_CritereVC = XDC_OUI,
			@vl_CritereDP = XDC_OUI
	else
	begin
		if @va_Districts_in & XDC_HIS_EXP_CI = XDC_HIS_EXP_CI
			select @vl_CritereCI = XDC_OUI
		else	select @vl_CritereCI = XDC_NON
		if @va_Districts_in & XDC_HIS_EXP_VC = XDC_HIS_EXP_VC
			select @vl_CritereVC = XDC_OUI
		else	select @vl_CritereVC = XDC_NON
		if @va_Districts_in & XDC_HIS_EXP_DP = XDC_HIS_EXP_DP
			select @vl_CritereDP = XDC_OUI
		else	select @vl_CritereDP = XDC_NON
	end

	/*A creation des tables de travail : liste globale, liste des evenements */
	/*A liste des actions                                                    */
	/*! pour autoroute, PR, sens : impossible d'utiliser types predefinis :  */
	/*! T_AUTOROUTE, T_PR, T_SENS                                            */
	create table #LISTE (	numero		int, 
				cle		tinyint,
				type		smallint,
				cause		int		null,
				cle_cause	tinyint		null,
				debut		datetime	null,
				fin		datetime	null,
				autoroute	tinyint		null,
				PR		int		null,
				sens		tinyint		null,
				categorie	bit  default	XDC_HIS_EXP_EVT)
	create table #LISTE_ACT(numero		int, 
				cle		tinyint,
				type		smallint,
				cause		int,
				cle_cause	tinyint,
				debut		datetime,
				fin		datetime	null,
				autoroute	tinyint		null,
				PR		int		null,
				sens		tinyint		null,
				categorie	bit  default XDC_HIS_EXP_ACT)
	create table #LISTE_EVT(numero		int, 
				cle		tinyint,
				type		smallint,
				cause		int		null,
				cle_cause	tinyint		null,
				debut		datetime,
				fin		datetime	null,
				autoroute	tinyint	null,
				PR		int		null,
				sens		tinyint		null,
				categorie	bit  default XDC_HIS_EXP_EVT)

#ifdef HIST
	/*! cas ou la procedure est creee au CI */

	/*A construire la liste des evenements et  des actions */
	/*A suivant critere sur evenement et sur plage horaire */
	/*A et localisation sur district                       */
	if @va_NumEvenement_in is not null
	begin
		/*A verifier l'existance et recherche informations */
		select 
			@vl_Type = type,
			@vl_Cause = cause,
			@vl_CleCause = cle_cause
		from HIS..FMC_GEN
		where numero = @va_NumEvenement_in and cle = @va_CleEvenement_in and datex=XDC_FAUX

		if @@rowcount != 1
			return XDC_NOK
			
		/*A ajout de l'evenement dans la liste */
		insert #LISTE ( numero , cle , type , cause , cle_cause, categorie)
		values (@va_NumEvenement_in, @va_CleEvenement_in, @vl_Type, 
			@vl_Cause, @vl_CleCause, XDC_HIS_EXP_EVT)

		if @@rowcount != 1
			return XDC_NOK
			
		/*A liste des consequences de l'evenement */
		select	@vl_Mode = XZAEC_PasEffacDonnees,
			@vl_DateCourante = getdate()
		exec @vl_Status = XZAE;68	@va_NumEvenement_in, @va_CleEvenement_in, 
						@vl_DateCourante, @vl_Mode

		if @vl_Status != XDC_OK
			return @vl_Status

		/*A memorisation des consequences momentanement */
		insert #LISTE ( numero , cle , type , cause , cle_cause, categorie)
		select 
			numero , 
			cle , 
			type , 
			numero_pere, 
			cle_pere ,
			XDC_HIS_EXP_EVT
		from LISTE_CONSEQ 
		where spid = @@spid

		/*A effacer les donnees de la table LISTE_CONSEQ (utiliser par XZAE68) */
		delete LISTE_CONSEQ where spid = @@spid

		/*B creation d'un curseur pour la liste des evenements */
		declare Pointeur_evt cursor
		for select numero, cle, type
		from #LISTE
		open Pointeur_evt

		/*B parcours liste des consequences pour rechercher les actions */
		/*B associees et correspondant au critere de date               */
		/*B et a la localisation demandee                               */
		fetch Pointeur_evt into @vl_NumEvt, @vl_CleEvt, @vl_Type
		while (@@sqlstatus = 0)
		begin
			if @vl_Type = XZAEC_FMC_TeteBouchon
				select @vl_NumEvt = numero,
					@vl_CleEvt = cle
				from HIS..FMC_GEN
				where cause = @vl_NumEvt and cle = @vl_CleEvt
				and datex=XDC_FAUX

			/*A ajouter les actions correspondant a ces consequences */
			if @va_TypesAction1_in != 0 or @va_TypesAction2_in != 0
			begin
				insert #LISTE_ACT (numero, cle, cause, cle_cause, 
				debut, fin, type, autoroute, PR, sens, categorie)
				select 
					HIS..ACT_GEN.numero,
					sit,
					evenement,
					cle,
					heure_succes,
					heure_fin,
					HIS..ACT_GEN.type,
					CFG..EQT_GEN.autoroute,
					CFG..EQT_GEN.PR,
					CFG..EQT_GEN.sens,
					XDC_HIS_EXP_ACT
				from HIS..ACT_GEN, CFG..EQT_GEN
				where	evenement = @vl_NumEvt and cle = @vl_CleEvt and
					heure_succes <= @va_HoroFin_in and
					((@va_HoroDebut_in is null and (HIS..ACT_GEN.heure_fin is null or
								HIS..ACT_GEN.heure_fin >= @va_HoroFin_in)) or 
					 @va_HoroDebut_in <= heure_succes) and
					((@vl_CritereCI = XDC_OUI and sit = @vl_SiteMaitre) or
					 (@vl_CritereVC = XDC_OUI and sit = XDC_VC) or
					 (@vl_CritereDP = XDC_OUI and sit = XDC_DP)) and
					CFG..EQT_GEN.type =*  HIS..ACT_GEN.type and
					CFG..EQT_GEN.numero =* equipement
			end

		
			/*B construire la liste des evenements inclus */
			/*B dans la plage horaire  et les districts   */
			/*B selectionnes                              */
			if  @vl_NumEvt = @va_NumEvenement_in and @vl_CleEvt = @va_CleEvenement_in
				select @vl_NumEvt = @va_NumEvenement_in
			else
				insert #LISTE_EVT (numero, cle, debut, fin, type, cause,
				cle_cause, autoroute, PR, sens, categorie )
				select 
					HIS..FMC_GEN.numero,
					HIS..FMC_GEN.cle,
					HIS..FMC_GEN.debut,
					HIS..FMC_GEN.fin,
					HIS..FMC_GEN.type,
					HIS..FMC_GEN.cause,
					HIS..FMC_GEN.cle_cause,
					HIS..FMC_HIS.autoroute,
					HIS..FMC_HIS.PR,
					HIS..FMC_HIS.sens,
					XDC_HIS_EXP_EVT
				from HIS..FMC_GEN, HIS..FMC_HIS, CFG..RES_POR
				where	HIS..FMC_GEN.numero = @vl_NumEvt and 
					HIS..FMC_GEN.cle = @vl_CleEvt and
					HIS..FMC_GEN.datex=XDC_FAUX and
					HIS..FMC_GEN.debut <= @va_HoroFin_in and
					((@va_HoroDebut_in is null and (HIS..FMC_GEN.fin is null or
									HIS..FMC_GEN.fin >= @va_HoroFin_in )) or
					 HIS..FMC_GEN.fin >= @va_HoroDebut_in or HIS..FMC_GEN.fin is null) and
					HIS..FMC_HIS.numero = @vl_NumEvt and
					HIS..FMC_HIS.cle = @vl_CleEvt and
					HIS..FMC_HIS.horodate_validation in 
								(select min(horodate_validation)
								from HIS..FMC_HIS
								where	numero = @vl_NumEvt and
									cle = @vl_CleEvt ) and
					(HIS..FMC_HIS.autoroute = CFG..RES_POR.autoroute and
					 HIS..FMC_HIS.PR >= CFG..RES_POR.PR_debut and
					 HIS..FMC_HIS.PR <= CFG..RES_POR.PR_fin) and
					 ((@vl_CritereCI = XDC_OUI and 
					    CFG..RES_POR.district = @vl_SiteMaitre) or
					   (@vl_CritereVC = XDC_OUI and 
					    CFG..RES_POR.district = XDC_VC) or
					   (@vl_CritereDP = XDC_OUI and 
					    CFG..RES_POR.district = XDC_DP))

			 /*B passage a l'evenement suivant */
			 fetch Pointeur_evt into @vl_NumEvt, @vl_CleEvt, @vl_Type
		end

		close  Pointeur_evt
	end
	else
	begin
		/*A rechercher dans tous les actions et evenements */

		/*A ajouter les actions correspondant a la plage horaire */
		/*A et a la localisation demandee                        */
		if @va_TypesAction1_in != 0 or @va_TypesAction2_in != 0
			insert #LISTE_ACT (numero, cle, cause, cle_cause, 
			debut, fin, type, autoroute, PR, sens, categorie)
			select
				HIS..ACT_GEN.numero,
				sit,
				evenement,
				cle,
				heure_succes,
				heure_fin,
				HIS..ACT_GEN.type,
				CFG..EQT_GEN.autoroute,
				CFG..EQT_GEN.PR,
				CFG..EQT_GEN.sens,
				XDC_HIS_EXP_ACT
			from HIS..ACT_GEN, CFG..EQT_GEN
			where   heure_succes <= @va_HoroFin_in and
				((@va_HoroDebut_in is null and (HIS..ACT_GEN.heure_fin is null or
								HIS..ACT_GEN.heure_fin >= @va_HoroFin_in)) or 
				 @va_HoroDebut_in <= heure_succes) and
				((@vl_CritereCI = XDC_OUI and sit = @vl_SiteMaitre) or
				 (@vl_CritereVC = XDC_OUI and sit = XDC_VC) or
				 (@vl_CritereDP = XDC_OUI and sit = XDC_DP)) and
				CFG..EQT_GEN.type =*  HIS..ACT_GEN.type and
				CFG..EQT_GEN.numero =* equipement

		/*B construire la liste des evenements inclus     */
		/*B dans la plage horaire                         */
		select
			HIS..FMC_GEN.numero,
			HIS..FMC_GEN.cle,
			HIS..FMC_GEN.debut,
			HIS..FMC_GEN.fin,
			HIS..FMC_GEN.type,
			HIS..FMC_GEN.cause,
			HIS..FMC_GEN.cle_cause,
			HIS..FMC_HIS.autoroute,
			HIS..FMC_HIS.PR,
			HIS..FMC_HIS.sens
		into #TEMPO
		from HIS..FMC_GEN, HIS..FMC_HIS, CFG..RES_POR
		where	HIS..FMC_GEN.debut <= @va_HoroFin_in and HIS..FMC_GEN.datex=XDC_FAUX and
			((@va_HoroDebut_in is null and (HIS..FMC_GEN.fin is null or
							HIS..FMC_GEN.fin >= @va_HoroFin_in )) or
			 HIS..FMC_GEN.fin >= @va_HoroDebut_in or HIS..FMC_GEN.fin is null) and
			HIS..FMC_HIS.numero = HIS..FMC_GEN.numero and
			HIS..FMC_HIS.cle = HIS..FMC_GEN.cle
			group by HIS..FMC_GEN.numero, HIS..FMC_GEN.cle, 
				HIS..FMC_HIS.numero, HIS..FMC_HIS.cle
			having HIS..FMC_HIS.horodate_validation = min(HIS..FMC_HIS.horodate_validation) 

		insert #LISTE_EVT (numero, cle, debut, fin, type, cause,
				cle_cause, autoroute, PR, sens, categorie)
		select #TEMPO.numero, #TEMPO.cle, #TEMPO.debut, #TEMPO.fin, #TEMPO.type, #TEMPO.cause, #TEMPO.cle_cause, #TEMPO.autoroute, #TEMPO.PR, #TEMPO.sens, XDC_HIS_EXP_EVT
		from #TEMPO, CFG..RES_POR
		where	#TEMPO.autoroute = CFG..RES_POR.autoroute and #TEMPO.PR >= CFG..RES_POR.PR_debut and #TEMPO.PR <= CFG..RES_POR.PR_fin and
			((@vl_CritereCI = XDC_OUI and CFG..RES_POR.district = @vl_SiteMaitre) or
			 (@vl_CritereVC = XDC_OUI and CFG..RES_POR.district = XDC_VC) or
			 (@vl_CritereDP = XDC_OUI and CFG..RES_POR.district = XDC_DP))
	end
#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

	/*A construire la liste des evenements et  des actions */
	/*A suivant critere sur evenement et sur plage horaire */
	/*A et localisation sur district                       */
	if @va_NumEvenement_in is not null
	begin
		/*A verifier l'existance et recherche informations */
		select 
			@vl_Type = type,
			@vl_Cause = cause,
			@vl_CleCause = cle_cause
		from EXP..FMC_GEN
		where numero = @va_NumEvenement_in and cle = @va_CleEvenement_in and datex=XDC_FAUX

		if @@rowcount != 1
			return XDC_NOK
			
		/*A ajout de l'evenement dans la liste */
		insert #LISTE ( numero , cle , type , cause , cle_cause, categorie)
		values (@va_NumEvenement_in, @va_CleEvenement_in, @vl_Type, 
			@vl_Cause, @vl_CleCause, XDC_HIS_EXP_EVT)

		if @@rowcount != 1
			return XDC_NOK
			
		/*A liste des consequences de l'evenement */
		select	@vl_Mode = XZAEC_PasEffacDonnees,
			@vl_DateCourante = getdate()
		exec @vl_Status = XZAE;68	@va_NumEvenement_in, @va_CleEvenement_in, 
						@vl_DateCourante, @vl_Mode

		if @vl_Status != XDC_OK
			return @vl_Status

		/*A memorisation des consequences momentanement */
		insert #LISTE ( numero , cle , type , cause , cle_cause, categorie)
		select 
			numero , 
			cle , 
			type , 
			numero_pere, 
			cle_pere,
			XDC_HIS_EXP_EVT
		from LISTE_CONSEQ 
		where spid = @@spid

		/*A effacer les donnees de la table LISTE_CONSEQ (utiliser par XZAE68) */
		delete LISTE_CONSEQ where spid = @@spid

		/*B creation d'un curseur pour la liste des evnements */
		declare Pointeur_evt cursor
		for select numero, cle, type
		from #LISTE
		open Pointeur_evt

		/*B parcours liste des consequences pour rechercher les actions */
		/*B associees et correspondant au critere de date               */
		/*B et a la localisation demandee                               */
		fetch Pointeur_evt into @vl_NumEvt, @vl_CleEvt, @vl_Type
		while (@@sqlstatus = 0)
		begin
			if @vl_Type = XZAEC_FMC_TeteBouchon
				select @vl_NumEvt = numero,
					@vl_CleEvt = cle
				from EXP..FMC_GEN
				where cause = @vl_NumEvt and cle = @vl_CleEvt
				and datex=XDC_FAUX

			/*A ajouter les actions correspondant a ces consequences */
			if @va_TypesAction1_in != 0 or @va_TypesAction2_in != 0
			begin
				insert #LISTE_ACT (numero, cle, cause, cle_cause, 
				debut, fin, type, autoroute, PR, sens, categorie)
				select 
					EXP..ACT_GEN.numero,
					sit,
					evenement,
					cle,
					heure_succes,
					heure_fin,
					EXP..ACT_GEN.type,
					CFG..EQT_GEN.autoroute,
					CFG..EQT_GEN.PR,
					CFG..EQT_GEN.sens,
					XDC_HIS_EXP_ACT
				from EXP..ACT_GEN, CFG..EQT_GEN
				where	evenement = @vl_NumEvt and cle = @vl_CleEvt and
					heure_succes <= @va_HoroFin_in and
					((@va_HoroDebut_in is null and (EXP..ACT_GEN.heure_fin is null or
									EXP..ACT_GEN.heure_fin >= @va_HoroFin_in)) or 
					 @va_HoroDebut_in <= heure_succes) and
					((@vl_CritereCI = XDC_OUI and sit = @vl_SiteMaitre) or
					 (@vl_CritereVC = XDC_OUI and sit = XDC_VC) or
					 (@vl_CritereDP = XDC_OUI and sit = XDC_DP)) and
					CFG..EQT_GEN.type =*  EXP..ACT_GEN.type and
					CFG..EQT_GEN.numero =* equipement
			end

		
			/*B construire la liste des evenements inclus */
			/*B dans la plage horaire  et les districts   */
			/*B selectionnes                              */
			if  @vl_NumEvt = @va_NumEvenement_in and @vl_CleEvt = @va_CleEvenement_in
				select @vl_NumEvt = @va_NumEvenement_in
			else
				insert #LISTE_EVT (numero, cle, debut, fin, type, cause,
				cle_cause, autoroute, PR, sens, categorie )
				select 
					EXP..FMC_GEN.numero,
					EXP..FMC_GEN.cle,
					EXP..FMC_GEN.debut,
					EXP..FMC_GEN.fin,
					EXP..FMC_GEN.type,
					EXP..FMC_GEN.cause,
					EXP..FMC_GEN.cle_cause,
					EXP..FMC_HIS.autoroute,
					EXP..FMC_HIS.PR,
					EXP..FMC_HIS.sens,
					XDC_HIS_EXP_EVT
				from EXP..FMC_GEN, EXP..FMC_HIS, CFG..RES_POR
				where	EXP..FMC_GEN.numero = @vl_NumEvt and 
					EXP..FMC_GEN.cle = @vl_CleEvt and
					EXP..FMC_GEN.datex = XDC_FAUX and
					EXP..FMC_GEN.debut <= @va_HoroFin_in and
					((@va_HoroDebut_in is null and (EXP..FMC_GEN.fin is null or
									EXP..FMC_GEN.fin >= @va_HoroFin_in )) or
					 EXP..FMC_GEN.fin >= @va_HoroDebut_in or EXP..FMC_GEN.fin is null) and
					EXP..FMC_HIS.numero = @vl_NumEvt and
					EXP..FMC_HIS.cle = @vl_CleEvt and
					EXP..FMC_HIS.horodate_validation in 
								(select min(horodate_validation)
								from EXP..FMC_HIS
								where	numero = @vl_NumEvt and
									cle = @vl_CleEvt ) and
					(EXP..FMC_HIS.autoroute = CFG..RES_POR.autoroute and
					 EXP..FMC_HIS.PR >= CFG..RES_POR.PR_debut and
					 EXP..FMC_HIS.PR <= CFG..RES_POR.PR_fin) and
					 ((@vl_CritereCI = XDC_OUI and 
					    CFG..RES_POR.district = @vl_SiteMaitre) or
					   (@vl_CritereVC = XDC_OUI and 
					    CFG..RES_POR.district = XDC_VC) or
					   (@vl_CritereDP = XDC_OUI and 
					    CFG..RES_POR.district = XDC_DP)) 

			 /*B passage a l'evenement suivant */
			 fetch Pointeur_evt into @vl_NumEvt, @vl_CleEvt, @vl_Type
		end

		close  Pointeur_evt
	end
	else
	begin
		/*A rechercher dans tous les actions et evenements */

		/*A ajouter les actions correspondant a la plage horaire */
		/*A et a la localisation demandee                        */
		if @va_TypesAction1_in != 0 or @va_TypesAction2_in != 0
			insert #LISTE_ACT (numero, cle, cause, cle_cause, 
			debut, fin, type, autoroute, PR, sens, categorie)
			select
				EXP..ACT_GEN.numero,
				sit,
				evenement,
				cle,
				heure_succes,
				heure_fin,
				EXP..ACT_GEN.type,
				CFG..EQT_GEN.autoroute,
				CFG..EQT_GEN.PR,
				CFG..EQT_GEN.sens,
				XDC_HIS_EXP_ACT
			from EXP..ACT_GEN, CFG..EQT_GEN
			where   heure_succes <= @va_HoroFin_in and
				((@va_HoroDebut_in is null and (EXP..ACT_GEN.heure_fin is null or
								EXP..ACT_GEN.heure_fin >= @va_HoroFin_in)) or 
				 @va_HoroDebut_in <= heure_succes) and
				((@vl_CritereCI = XDC_OUI and sit = @vl_SiteMaitre) or
				 (@vl_CritereVC = XDC_OUI and sit = XDC_VC) or
				 (@vl_CritereDP = XDC_OUI and sit = XDC_DP)) and
				CFG..EQT_GEN.type =*  EXP..ACT_GEN.type and
				CFG..EQT_GEN.numero =* equipement

		/*B construire la liste des evenements inclus     */
		/*B dans la plage horaire                         */
		select
			EXP..FMC_GEN.numero,
			EXP..FMC_GEN.cle,
			EXP..FMC_GEN.debut,
			EXP..FMC_GEN.fin,
			EXP..FMC_GEN.type,
			EXP..FMC_GEN.cause,
			EXP..FMC_GEN.cle_cause,
			EXP..FMC_HIS.autoroute,
			EXP..FMC_HIS.PR,
			EXP..FMC_HIS.sens
		into #TEMPO
		from EXP..FMC_GEN, EXP..FMC_HIS, CFG..RES_POR
		where	EXP..FMC_GEN.debut <= @va_HoroFin_in and EXP..FMC_GEN.datex=XDC_FAUX and
			((@va_HoroDebut_in is null and (EXP..FMC_GEN.fin is null or
							EXP..FMC_GEN.fin >= @va_HoroFin_in )) or
			 EXP..FMC_GEN.fin >= @va_HoroDebut_in  or EXP..FMC_GEN.fin is null) and
			EXP..FMC_HIS.numero = EXP..FMC_GEN.numero and
			EXP..FMC_HIS.cle = EXP..FMC_GEN.cle   
			group by EXP..FMC_GEN.numero, EXP..FMC_GEN.cle, 
				EXP..FMC_HIS.numero, EXP..FMC_HIS.cle
			having EXP..FMC_HIS.horodate_validation = min(EXP..FMC_HIS.horodate_validation)


		insert #LISTE_EVT (numero, cle, debut, fin, type, cause,
				cle_cause, autoroute, PR, sens, categorie)
		select #TEMPO.numero, #TEMPO.cle, #TEMPO.debut, #TEMPO.fin, #TEMPO.type, #TEMPO.cause, #TEMPO.cle_cause, #TEMPO.autoroute, #TEMPO.PR, #TEMPO.sens,
			XDC_HIS_EXP_EVT
		from #TEMPO, CFG..RES_POR
		where	#TEMPO.autoroute = CFG..RES_POR.autoroute and #TEMPO.PR >= CFG..RES_POR.PR_debut and #TEMPO.PR <= CFG..RES_POR.PR_fin and
			((@vl_CritereCI = XDC_OUI and CFG..RES_POR.district = @vl_SiteMaitre) or
			 (@vl_CritereVC = XDC_OUI and CFG..RES_POR.district = XDC_VC) or
			 (@vl_CritereDP = XDC_OUI and CFG..RES_POR.district = XDC_DP))
	end
#endif

	/*A supprimer toutes les actions non desirees */
	if  @va_TypesAction1_in != 0 or  @va_TypesAction2_in != 0
	begin
		/*B initialisation avec le premier type */
		select @vl_Type = XDC_ACT_NAV
	
		/*B supprimer les actions qui ne sont pas du bon type */
		while @vl_Type <= 14
		begin
			if @va_TypesAction1_in % 2 = 0
				/*B type non desire :                             */
				/*B supprimer les actions correspondant a ce type */
				delete #LISTE_ACT 
				where type = @vl_Type
	
				/*B enleve type traite */
			else	select @va_TypesAction1_in = (@va_TypesAction1_in - 1)
	
			/*B type d'action suivant */
			select	@vl_Type = @vl_Type + 1, 
				@va_TypesAction1_in = @va_TypesAction1_in / 2

		end

		delete #LISTE_ACT
		where type = XDC_ACT_PMVDAT

		/*B initialisation avec le premier type */
		select @vl_Type = XDC_ACT_FAC
	
		/*B supprimer les actions qui ne sont pas du bon type */
		while (@vl_Type <= XDC_ACT_FAC + XDC_ACT_NB_TYP - 14)
		begin
			if @va_TypesAction2_in % 2 = 0
				/*B type non desire :                             */
				/*B supprimer les actions correspondant a ce type */
				delete #LISTE_ACT 
				where type = @vl_Type
	
				/*B enleve type traite */
			else	select @va_TypesAction2_in = (@va_TypesAction2_in - 1)
	
			/*B type d'action suivant */
			select	@vl_Type = @vl_Type + 1, 
				@va_TypesAction2_in = @va_TypesAction2_in / 2

		end
	end


	/* initialisation avec le premier type */
	select @vl_Type = XZAEC_FMC_Accident
	
	/*A supprimer les evenements qui ne sont pas du bon type */
	if @va_TypesEvt1_in is not null 
		while @vl_Type < 31
		begin
			if @va_TypesEvt1_in % 2 = 0
				/*B supprimer les actions correspondant a ce type */
				delete #LISTE_EVT
				where type = @vl_Type
		
				/*B enleve type traite */
			else	select @va_TypesEvt1_in = (@va_TypesEvt1_in - 1) 
		
			/*B type suivant */
			select	@vl_Type = @vl_Type + 1,
				@va_TypesEvt1_in = @va_TypesEvt1_in / 2
		end

	if @va_TypesEvt2_in is not null
		while @vl_Type <= XZAEC_FMC_NB_TYP
		begin
			if @va_TypesEvt2_in % 2 = 0
			begin
				/*B supprimer les actions correspondant a ce type */
				delete #LISTE_EVT
				where type = @vl_Type
			end
	
				/*B enleve type traite */
			else	select @va_TypesEvt2_in = (@va_TypesEvt2_in - 1)
		
			/*B type suivant */
			select	@vl_Type = @vl_Type + 1,
				@va_TypesEvt2_in = @va_TypesEvt2_in / 2
		end

	/*A remplacer toutes les causes des queues de bouchon par les causes des tetes de bouchon */
#ifdef HIST
	update #LISTE_EVT 
	set cause = HIS..FMC_GEN.cause, 
		cle_cause = HIS..FMC_GEN.cle_cause
	from HIS..FMC_GEN
	where #LISTE_EVT.type = XZAEC_FMC_QueueBouchon and
		HIS..FMC_GEN.numero = #LISTE_EVT.cause and
		HIS..FMC_GEN.cle = #LISTE_EVT.cle_cause and
		HIS..FMC_GEN.datex = XDC_FAUX
#else
	update #LISTE_EVT 
	set cause = EXP..FMC_GEN.cause, 
		cle_cause = EXP..FMC_GEN.cle_cause
	from EXP..FMC_GEN
	where #LISTE_EVT.type = XZAEC_FMC_QueueBouchon and
		EXP..FMC_GEN.numero = #LISTE_EVT.cause and
		EXP..FMC_GEN.cle = #LISTE_EVT.cle_cause and
		EXP..FMC_GEN.datex = XDC_FAUX
#endif

	/*A regroupement des listes d'actions et d'evenements */
	delete #LISTE

	insert #LISTE(numero, cle, type, cause, cle_cause, debut, fin, 
	autoroute, PR, sens, categorie)
	select
		numero, 
		cle, 
		type, 
		cause, 
		cle_cause, 
		debut, 
		fin, 
		autoroute, 
		PR, 
		sens, 
		categorie  
	from #LISTE_ACT

	insert #LISTE(numero, cle, type, cause, cle_cause, debut, fin, 
	autoroute, PR, sens, categorie)
	select
		numero, 
		cle, 
		type, 
		cause, 
		cle_cause, 
		debut, 
		fin, 
		autoroute, 
		PR, 
		sens, 
		categorie  
	from #LISTE_EVT

	/*A Retourner des parametres ordonnes suivant la date de debut */
	select distinct * from #LISTE order by debut

	return XDC_OK
go
