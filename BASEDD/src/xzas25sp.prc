/*E*/
/*  Fichier : $Id: xzas25sp.prc,v 1.6 1998/09/22 13:31:42 gaborit Exp $    Release : $Revision: 1.6 $    Date : $Revision: 1.6 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas25sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Analyse 
*
------------------------------------------------------
* HISTORIQUE :
*
* T.F.  06/12/94        : Creation      (V 1.1)
* Guilhou 30/12/94	: bug tests parametres null (V1.2)
* T.F. 2/01/95		: Ajout d'un code de retour comme argument.(V 1.3)
* C.T.  01/03/95        : Recherche infos dans HIS si date de
*                         calcul > 1 mois pour CI (V 1.4)
* B.G.	07/09/95	: correction recette usine (1.5)
* PN	29/06/98	: separartion EXP/HIS (1.6)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Calcul l'indicateur COLOTRA a une heure donnee pour un evenement
* de type "Travaux" donne.
*
* Sequence d'appel
* SP    XZAS25_Ind_COLOTRA
*
* Arguments en entree :
* XDY_Entier    @va_Numero_in		: Numero de FMC
* XDY_Mot	@va_Cle_in              : Cle (numero du district)
* XDY_PR        @va_PR_DebTrav_in    	: PR de debut de travaux 
* XDY_PR        @va_PR_FinTrav_in    	: PR de fin de travaux 
* XDY_Octet	@va_Autoroute_in	: Numero de l'autoroute
* XDY_Sens      @va_Sens_in             : Sens de parcours
* XDY_Datetime	@va_Horodate_in		: Date du calcul de l'indicateur
* XDY_Datetime  @va_Horodate_valid_in   : Date de validation de la FMC
* XDY_Mot       @va_Vit_H_T_in          : Vitesse hors travaux (Km/h)
* XDY_Mot       @va_Vit_BAU_in          : Vitesse BAU neutralise (Km/h)
* XDY_Mot       @va_Vit_Voie_Red_in     : Vitesse voie reduite (Km/h)
* XDY_Mot       @va_Vit_BAS_in          : Vitesse basculement (Km/h)
*
* Arguments en sortie :
* XDY_Entier    @va_Indicateur_out      : Indicateur COLOTRA
* XDY_Octet 	@va_Status_out		: Status retour de la procedure
*
* Liste retournee :
*
* Code retour
* XDC_OK
* XDC_NOK               : probleme d'acces aux donnees 
* XDC_ARG_INV           : parametres d'entree null, ou valeur non autorisee ou
*                         echangeur inexistante
* XDC_PAS_SYN           : pas de resultat.
*
* Conditions d'utilisation
* Tient compte du debit sur l'heure precedent la date de calcul.
*
* Fonction
*
-------------------------------------------------------*/

use PRC
go

create procedure XZAS;25
	@va_Numero_in		int		= null,
	@va_Cle_in		smallint	= null,
        @va_PR_DebTrav_in    	T_PR            = null,
	@va_PR_FinTrav_in    	T_PR            = null,
	@va_Autoroute_in	tinyint		= null,
	@va_Sens_in 		tinyint         = null,
	@va_Horodate_in		datetime	= null,
	@va_Horodate_valid_in	datetime        = null,
	@va_Vit_H_T_in          smallint        = null,
	@va_Vit_BAU_in          smallint        = null,
	@va_Vit_Voie_Red_in     smallint        = null,
	@va_Vit_BAS_in          smallint        = null,
	@va_Indicateur_out	int		= null output,
	@va_Status_out		tinyint		= null output
as
	declare	@vl_Debit		float,	 /* Debit mesure sur une heure*/
		@vl_LgTravaux		float,	 /* Longueur des travaux */
		@vl_Lg_BAU		float,	 /* Lgr BAU neutralisee */
		@vl_Lg_Voie_Red		float,	 /* Lgr voie reduite */
		@vl_Lg_BAS		float,	 /* Lgr de basculement */
		@vl_TempsTheo		float,	 /* Temps theorique */
		@vl_TempsReel		float,	 /* Tps retard de parcours */
		@vl_PerteTemps		float,   /* Perte de temps */
		@vl_VR			tinyint, /* Voie rapide */
		@vl_VR_I                tinyint, /* Voie rapide sens inverse */
		@vl_VM2                 tinyint, /* Voie mediane 2 */
		@vl_VM2_I               tinyint, /* Voie mediane 2 ss inverse*/
		@vl_VM1                 tinyint, /* Voie mediane 1 */
		@vl_VM1_I               tinyint, /* Voie mediane 1 ss inverse*/
		@vl_VL			tinyint, /* Voie lente */
		@vl_VL_I		tinyint, /* Voie lente sens inverse */
		@vl_BAU			tinyint, /* BAU */
		@vl_BAU_I		tinyint, /* BAU sens inverse */
		@vl_NumStation		smallint,/* Num de station de comptage*/
		@vl_PR			T_PR,	 /* PR intermediaire */
		@vl_Status int,
		@vl_RechercheFmcDsHis	bit,
		@vl_RechercheMesDsHis	bit

	/* Initialisation des arguments de sortie */
	select @va_Status_out = XDC_OK
	select @va_Indicateur_out = 0
	select @vl_RechercheFmcDsHis = XDC_NON,
		@vl_RechercheMesDsHis = XDC_NON

	/* Test Argument APPLIX (il n'existe pas d'equipement de type zero) */
	if @va_Vit_H_T_in       = 0     select @va_Vit_H_T_in       	= null
	if @va_Vit_BAU_in       = 0     select @va_Vit_BAU_in       	= null
	if @va_Vit_Voie_Red_in  = 0     select @va_Vit_Voie_Red_in  	= null
	if @va_Vit_BAS_in       = 0     select @va_Vit_BAS_in       	= null
	if @va_Autoroute_in	= 0	select @va_Autoroute_in		= null
	if @va_Numero_in	= 0     select @va_Numero_in		= null
	if @va_Cle_in		= 0	select @va_Cle_in 		= null

	/*A 
	** Controle des parametres en entree :
	*/
	if @va_Numero_in	= null or
	   @va_Cle_in		= null or
	   @va_Autoroute_in	= null or
	   @va_Horodate_in	= null or
	   @va_Horodate_valid_in= null or
	   @va_Vit_H_T_in       = null or
     	   @va_Vit_BAU_in       = null or
	   @va_Vit_Voie_Red_in  = null or
	   @va_Vit_BAS_in       = null
		return XDC_ARG_INV

	/*A
	** Test de coherence (PR de debut < PR de fin)
	*/
	if @va_PR_DebTrav_in >= @va_PR_FinTrav_in
	begin
	 	/* Inversion des parametres */
		select @vl_PR = @va_PR_DebTrav_in
		select @va_PR_DebTrav_in = @va_PR_FinTrav_in
		select @va_PR_FinTrav_in = @vl_PR
	end

	/* Mise a jour de longueur */
	select @vl_Lg_BAS = 0
	select @vl_Lg_Voie_Red = 0
	select @vl_Lg_BAU = 0
	select @vl_Debit = 0

#ifdef CI
	/*A
	** si la date donnee est anterieure d'un mois au moins
	** rechercher les infos dans la base HIS sinon dans la base EXP
	*/
	if (@va_Horodate_valid_in < dateadd(month, -1, getdate ())) or (@va_Horodate_in < dateadd(month, -1, getdate ()))
	begin
		select @vl_RechercheFmcDsHis = XDC_OUI
		select @vl_RechercheMesDsHis = XDC_OUI
		exec @vl_Status = SQL_HIS.PRC..XZAS;25
			@va_Numero_in		,
			@va_Cle_in		,
        		@va_PR_DebTrav_in    	,
			@va_PR_FinTrav_in    	,
			@va_Autoroute_in	,
			@va_Sens_in 		,
			@va_Horodate_in		,
			@va_Horodate_valid_in	,
			@va_Vit_H_T_in          ,
			@va_Vit_BAU_in          ,
			@va_Vit_Voie_Red_in     ,
			@va_Vit_BAS_in          ,
			@va_Indicateur_out	 output,
			@va_Status_out		output

		if @vl_Status = null
                       	return XDC_PRC_INC
                else return @vl_Status
	end
#endif
#ifdef HIST
		select @vl_RechercheFmcDsHis = XDC_OUI
		select @vl_RechercheMesDsHis = XDC_OUI
#endif

	/*A
	** Recherche des informations concernant l'evenement 
	** selectionne.
	*/
	/* Recherche de l'ensemble des informations concernant chacune  */
	/* des voies.							*/
	set rowcount 1
#ifndef HIST
		select	@vl_VR	=	VR,
			@vl_VM2 =	VM2,
			@vl_VM1 =	VM1,
			@vl_VL	=	VL,
			@vl_BAU	=	BAU_I,
			@vl_VR_I  =     VR_I,
			@vl_VM2_I =     VM2_I,
			@vl_VM1_I =     VM1_I,
			@vl_VL_I  =     VL_I,
			@vl_BAU_I =     BAU_I
		from EXP..FMC_HIS
		where numero = @va_Numero_in
		and   cle    = @va_Cle_in
		and   horodate_validation <= @va_Horodate_in
		order by horodate_validation desc
#else
		select	@vl_VR	=	VR,
			@vl_VM2 =	VM2,
			@vl_VM1 =	VM1,
			@vl_VL	=	VL,
			@vl_BAU	=	BAU_I,
			@vl_VR_I  =     VR_I,
			@vl_VM2_I =     VM2_I,
			@vl_VM1_I =     VM1_I,
			@vl_VL_I  =     VL_I,
			@vl_BAU_I =     BAU_I
		from HIS..FMC_HIS
		where numero = @va_Numero_in
		and   cle    = @va_Cle_in
		and   horodate_validation <= @va_Horodate_in
		order by horodate_validation desc
#endif

	if @@rowcount = 0
	begin
		select @va_Status_out = XDC_PAS_SYN
		set rowcount 0
		return XDC_OK
	end

	set rowcount 0

	 /*A
	 ** Calcul de la longueur des travaux (en metre).
	 */
	 select @vl_LgTravaux = abs(@va_PR_FinTrav_in-@va_PR_DebTrav_in)

	/*A
	** Test sur l'implication des travaux sur la circulation
	** (detection de :
	**	- Basculement
	**	- Voies reduites ou voies uniques
	**	- BAU neutralisee).
	** Mise a jour des longueurs de voies affectes par les 
	** travaux en fonction des cas detectes.
	*/
	/* Il y a basculement si ou moins une voie du sens oppose */
	/* est inversee (XDC_VOIE_SENS_INVERSE)			  */
	if  	(@vl_VR_I = XDC_VOIE_SENS_INVERSE) or
		(@vl_VM2_I = XDC_VOIE_SENS_INVERSE) or
		(@vl_VM1_I = XDC_VOIE_SENS_INVERSE) or
		(@vl_VL_I = XDC_VOIE_SENS_INVERSE) or
		(@vl_BAU_I = XDC_VOIE_SENS_INVERSE)  
	begin
	  select @vl_Lg_BAS = @vl_LgTravaux
	end
	/* Les voies voies sont reduites (XDC_VOIE_NEUTRALISEE ou */
	/* XDC_VOIE_BLOQUEE_NEUTRALISEE)			  */
	else if (@vl_VR = XDC_VOIE_NEUTRALISEE) or
		(@vl_VM2 = XDC_VOIE_NEUTRALISEE) or
		(@vl_VM1 = XDC_VOIE_NEUTRALISEE) or
		(@vl_VL= XDC_VOIE_NEUTRALISEE) or
		(@vl_VR = XDC_VOIE_BLOQUEE_NEUTRALISEE) or
		(@vl_VM2 = XDC_VOIE_BLOQUEE_NEUTRALISEE) or
		(@vl_VM1 = XDC_VOIE_BLOQUEE_NEUTRALISEE) or
		(@vl_VL= XDC_VOIE_BLOQUEE_NEUTRALISEE)
	begin
	  select @vl_Lg_Voie_Red = @vl_LgTravaux
	end
	/* La BAU est neutralisee (XDC_VOIE_NEUTRALISEE ou        */
	/* XDC_VOIE_BLOQUEE_NEUTRALISEE)                          */
	else if((@vl_BAU = XDC_VOIE_NEUTRALISEE) or
		(@vl_BAU = XDC_VOIE_BLOQUEE_NEUTRALISEE))
	begin
	  select @vl_Lg_BAU = @vl_LgTravaux
    	end

	/*A
	** Calcul du temps theorique en seconde (Pas de travaux).
	*/
	select @vl_TempsTheo	= 36*(@vl_LgTravaux / @va_Vit_H_T_in)/10

	/*A
	** Calcul du temps reel de parcours en seconde.
	*/
	select @vl_TempsReel	= 36*(
			        @vl_Lg_BAS / @va_Vit_BAS_in +
				@vl_Lg_Voie_Red/ @va_Vit_Voie_Red_in +
				@vl_Lg_BAU/ @va_Vit_BAU_in) /10

	/*A
	** Calcul de la perte de temps en seconde.
	*/
	if @vl_TempsReel=0
		select @vl_PerteTemps = 0 /* travaux sans perturbation */
	else
		select @vl_PerteTemps = @vl_TempsReel - @vl_TempsTheo

	/*A
	** Recherche de la station de comptage la plus proche 
	** situee en amont du PR de fin de travaux.
	*/
	/* Recherche du numero de la station de la ZI contenant le */
	/* PR de fin de travaux.    				   */
	/* Respect de l'organisation du reseau : portion/ZI	   */
	select @vl_NumStation = station
	from CFG..RES_INF
	where portion in (select numero 
			 from CFG..RES_POR
			 where autoroute = @va_Autoroute_in
			 and PR_debut < @va_PR_FinTrav_in
			 and PR_fin >= @va_PR_FinTrav_in)
	and PR_debut <= @va_PR_FinTrav_in
	and PR_fin >= @va_PR_FinTrav_in
	and sens = @va_Sens_in

	/* Pas de station de comptage trouvee */
	if @@rowcount = 0
	begin
		select @va_Status_out = XDC_PAS_SYN
		return XDC_OK
	end

	/*A
	** Recherche du debit sur une heure.
	*/
#ifndef HIST
	begin
		/* Le debit est egale a la moyenne des debits mesures par la */
		/* station de comptage pendant une heure. Ce calcul est effectue */
		/* a partir des donnees 6mn.				   */
	
		/* Calcul du debit sur une heure a partir des donnees 6mn
		(Vehicule/heure) */
		select @vl_Debit = avg(Q)
		from EXP..MES_TRA
		where station 	= @vl_NumStation
		and   sens	= @va_Sens_in
		and   voie	= 0
		and   horodate	<= @va_Horodate_in
		and   horodate  > dateadd(minute,-60, @va_Horodate_in)
	end
#else
	begin
		/* Calcul du debit sur une heure a partir des donnees 6mn
		(Vehicule/heure) */

		select @vl_Debit = avg(Q)
		from HIS..MES_TRA
		where station 	= @vl_NumStation
		and   sens	= @va_Sens_in
		and   voie	= 0
		and   horodate	<= @va_Horodate_in
		and   horodate  > dateadd(minute,-60, @va_Horodate_in)
	end
#endif


	/*A
	** Calcul de l'indicateur COLOTRA pour cet evenement:
	** (debits x longueur des travaux x perte de temps theorique)
	*/
	/* Debit en vehicules/h , la longueur des travaux en m */
	/* et le perte de temps en seconde  le tout divise par */
	/* 3600000 donc resultat en veh*km */
	select @va_Indicateur_out = @vl_Debit * @vl_LgTravaux * @vl_PerteTemps/3600000

	if @va_Indicateur_out = null
		select @va_Status_out = XDC_PAS_SYN

	/*A
	** Fin de la procedure
	*/
 	return XDC_OK
go
