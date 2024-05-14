/*E*/
/*  Fichier : @(#)xzas26sp.prc	1.7   Release : 1.7       Date : 03/11/98 
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas26sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Analyse 
*
------------------------------------------------------
* HISTORIQUE :
*
* T.F.  06/12/94        : Creation      (V 1.1)
* C.T.  01/03/95        : Recherche infos dans HIS si date de
*                         validation > 1 mois pour CI (V 1.2)
* P.N.  03/01/97	: Ajout des interdistance par configuration de voie (dem/1285) v1.4
* P.N.  03/01/97        : Correction sur le sens (dem/1285) v1.6
* C.T.  10/03/98        : Correction erreur de compil v1.7
* P.N.  29/06/98        : separation EXP/HIS v1.8
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Calcul de l'interdistance de travaux 
*
* Sequence d'appel
* SP    XZAS26_Interdistance_Travaux
*
* Arguments en entree :
* XDY_Entier    @va_Numero1_in		: Numero de FMC TRAV1
* XDY_Mot	@va_Cle1_in             : Cle (numero du district) TRAV1
* XDY_PR        @va_PR_FinTrav1_in    	: PR de fin de travaux TRAV1
* XDY_Octet	@va_Autoroute1_in	: Numero de l'autoroute TRAV1
* XDY_Sens      @va_Sens1_in            : Sens de parcours TRAV1
* XDY_Datetime  @va_Horodate_valid1_in  : Date de validation TRAV1
* XDY_Entier    @va_Numero2_in          : Numero de FMC TRAV2
* XDY_Mot       @va_Cle2_in             : Cle (numero du district) TRAV2
* XDY_PR        @va_PR_FinTrav2_in      : PR de fin de travaux TRAV2
* XDY_Octet     @va_Autoroute2_in       : Numero de l'autoroute TRAV2
* XDY_Sens      @va_Sens2_in            : Sens de parcours TRAV2
* XDY_Datetime  @va_Horodate_valid2_in  : Date de validation TRAV2
* XDY_Mot       @va_Dis_ZU_in		: Distance pour laquelle les travaux
*					  compte pour un seul (Km)
*
* Arguments en sortie :
* XDY_Entier    @va_Indicateur_out      : Indicateur resultat
*					  XDC_OK si l'interdistance est 
*						 respectee
*                                         XDC_NOK si l'interdistance 
*                                                 n'est pas respectee
* XDY_Entier    @va_IndicateurZU_out	: Indicateur Zone Urbaine
*                                         XDC_OK si les deux travaux comptent
*                                                pour un seul
*                                         XDC_NOK si les deux travaux sont
*						 espace de plus de va_Dis_ZU_in
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
* Les travaux sont consecutifs
*
* Fonction
* Verification de la regle d'interdistance DSCR entre deux travaux.
* La verification est appliquee aussi en zone urbaine (plusieurs travaux
* compris dans une zone de 6Km comptent pour un seul).
*
-------------------------------------------------------*/

use PRC
go

create procedure XZAS;26
	@va_Numero1_in		int		= null,
	@va_Cle1_in		smallint	= null,
	@va_PR_FinTrav1_in    	T_PR            = null,
	@va_Autoroute1_in	tinyint		= null,
	@va_Sens1_in 		tinyint         = null,
	@va_Horodate_valid1_in	datetime        = null,
	@va_Numero2_in          int             = null,
	@va_Cle2_in             smallint        = null,
	@va_PR_FinTrav2_in      T_PR            = null,
	@va_Autoroute2_in       tinyint         = null,
	@va_Sens2_in            tinyint         = null,
	@va_Horodate_valid2_in  datetime        = null,
	@va_Dis_baubau_in	int             = null,
	@va_Dis_1_bau1voie_in	int             = null,

	@va_Dis_2_bau1voie_in	int		= null,
	@va_Dis_2_baubasc	int             = null,
	@va_Dis_2_voievoie      int             = null,
	@va_Dis_2_voiebasc      int             = null,
	@va_Dis_2_bascbasc	int             = null,

	@va_Dis_3_bauvoie      int             = null,
	@va_Dis_3_bau2voies     int             = null,
	@va_Dis_3_baubasc       int             = null,
	@va_Dis_3_voievoie      int             = null,
	@va_Dis_3_voie2voies     int             = null,
	@va_Dis_3_voiebasc	int             = null,
	@va_Dis_3_2voies2voies	int             = null,
	@va_Dis_3_2voiesbasc	int             = null,
	@va_Dis_3_bascbasc	int             = null,

	@va_Dis_4_bauvoie	int             = null,
	@va_Dis_4_bau2voies	int             = null,
	@va_Dis_4_bau3voies	int             = null,
	@va_Dis_4_baubasc	int             = null,
	@va_Dis_4_voievoie	int             = null,
	@va_Dis_4_voie2voies	int             = null,
	@va_Dis_4_voie3voies    int             = null,
	@va_Dis_4_voiebasc	int             = null,
	@va_Dis_4_2voies2voies	int             = null,
	@va_Dis_4_2voies3voies	int             = null,
	@va_Dis_4_2voiesbasc	int             = null,
	@va_Dis_4_3voies2voies	int             = null,
	@va_Dis_4_3voiesbasc	int             = null,
	@va_Dis_4_bascbasc	int             = null,

        @va_Dis_ZU_in           int             = null,

	@va_Indicateur_out	tinyint		= null output,
	@va_IndicateurZU_out      tinyint         = null output
as
	declare	@vl_LgInterTravaux	float,	 /* Distance entre les Travaux*/
		@vl_LgExtrTravaux	float,	 /* Distance entre les extremite Travaux*/
		@vl_PR_DebTrav1      	T_PR,    /* Debut de travaux TRAV1 */
		@vl_PR_DebTrav2      	T_PR,	 /* Debut de travaux TRAV2 */
		@vl_VR_1			tinyint, /* Voie rapide */
		@vl_VR_I_1                tinyint, /* Voie rapide sens inverse */
		@vl_VM2_1                tinyint, /* Voie mediane 2 */
		@vl_VM2_I_1               tinyint, /* Voie mediane 2 ss inverse*/
		@vl_VM1_1                 tinyint, /* Voie mediane 1 */
		@vl_VM1_I_1               tinyint, /* Voie mediane 1 ss inverse*/
		@vl_VL_1			tinyint, /* Voie lente */
		@vl_VL_I_1		tinyint, /* Voie lente sens inverse */
		@vl_BAU_1			tinyint, /* BAU */
		@vl_BAU_I_1		tinyint, /* BAU sens inverse */
		@vl_VR_2			tinyint, /* Voie rapide */
		@vl_VR_I_2                tinyint, /* Voie rapide sens inverse */
		@vl_VM2_2                 tinyint, /* Voie mediane 2 */
		@vl_VM2_I_2               tinyint, /* Voie mediane 2 ss inverse*/
		@vl_VM1_2                 tinyint, /* Voie mediane 1 */
		@vl_VM1_I_2               tinyint, /* Voie mediane 1 ss inverse*/
		@vl_VL_2			tinyint, /* Voie lente */
		@vl_VL_I_2		tinyint, /* Voie lente sens inverse */
		@vl_BAU_2			tinyint, /* BAU */
		@vl_BAU_I_2		tinyint, /* BAU sens inverse */
		@vl_BAU_etroite_1	tinyint, /* BAU etroite */
		@vl_BAU_I_etr_1	tinyint, /* BAU etroite sens inverse */
		@vl_BAU_etroite_2	tinyint, /* BAU etroite */
		@vl_BAU_I_etr_2	tinyint, /* BAU etroite sens inverse */
		@vl_NumStation		smallint,/* Num de station de comptage*/
		@vl_CodeRet		    int, /* Code retour fonction */
		@vl_CodeRet1		    int, /* Code retour fonction */
		@vl_PR			T_PR,	 /* variable intermediaire */
		@vl_PR_aval		T_PR, /* pr des travaux en aval */
		@vl_tempo			tinyint,	 /* variable intermediaire */
		@vl_Portion			smallint,	 /* numero de portion */
		@vl_nb_voies_neut1	tinyint, /* nb de voies neutralisee trav1 */
		@vl_nb_voies_neut2	tinyint, /* nb de voies neutralisee trav2 */
		@vl_bau_neut1		tinyint, /* indicateur de bau neutralisee */
		@vl_bau_neut2		tinyint, /* indicateur de bau neutralisee */
		@vl_Distance		int, /* interdistance a respecter */
		@vl_sens_aut		smallint,
		@vl_Status int,
		@vl_nb_voies		tinyint  /* Nombre de voies */

	/* Initialisation des arguments */
	select @va_Indicateur_out = XDC_OK 
	select @va_IndicateurZU_out = XDC_OK

	/* Test Argument APPLIX (il n'existe pas d'equipement de type zero) */
	if @va_Autoroute1_in	= 0	select @va_Autoroute1_in	= null
	if @va_Numero1_in	= 0     select @va_Numero1_in		= null
	if @va_Cle1_in		= 0	select @va_Cle1_in 		= null
	if @va_Autoroute2_in    = 0     select @va_Autoroute2_in        = null
	if @va_Numero2_in       = 0     select @va_Numero2_in           = null
	if @va_Cle2_in          = 0     select @va_Cle2_in              = null
	if @va_Dis_ZU_in	= 0     select @va_Dis_ZU_in		= null

	/*A 
	** Controle des parametres en entree :
	*/
	if @va_PR_FinTrav1_in 	= null or
	   @va_Numero1_in	= null or
	   @va_Cle1_in		= null or
	   @va_Autoroute1_in	= null or
	   @va_Sens1_in		= null or
	   @va_Horodate_valid1_in= null or
	   @va_PR_FinTrav2_in   = null or
   	   @va_Numero2_in       = null or
	   @va_Cle2_in          = null or
	   @va_Autoroute2_in    = null or
	   @va_Sens2_in         = null or
	   @va_Horodate_valid1_in= null or
	   @va_Dis_ZU_in	= null
		return XDC_ARG_INV

	/*A
	** Test de coherence (Travaux influent dans le meme sens)
	*/
	if @va_Sens1_in != @va_Sens2_in
		return XDC_NOK

	/*A
	** Recherche du sens de l'autoroute */
	select @vl_sens_aut=inversee from CFG..RES_AUT_INV where numero=@va_Autoroute1_in
	
	
	/*A
	** Recherche des informations concernant l'evenement "Travaux"
	** selectionne.
	*/
	/* Recherche de l'ensemble des informations concernant chacune  */
	/* des voies pour le premier "Travaux".				*/

#ifdef CI
	/* Recherche de l'ensemble des informations concernant chacune  */
	/* dans HIS si pas trouvees                                     */
	if @va_Horodate_valid1_in < (select dateadd(month, -1, getdate ())) or
		@va_Horodate_valid2_in < (select dateadd(month, -1, getdate ())) 
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAS;26
	@va_Numero1_in,
	@va_Cle1_in	,
	@va_PR_FinTrav1_in    ,
	@va_Autoroute1_in	,
	@va_Sens1_in 		,
	@va_Horodate_valid1_in	,
	@va_Numero2_in          ,
	@va_Cle2_in             ,
	@va_PR_FinTrav2_in      ,
	@va_Autoroute2_in       ,
	@va_Sens2_in            ,
	@va_Horodate_valid2_in  ,
	@va_Dis_baubau_in	,
	@va_Dis_1_bau1voie_in	,

	@va_Dis_2_bau1voie_in	,
	@va_Dis_2_baubasc	,
	@va_Dis_2_voievoie      ,
	@va_Dis_2_voiebasc      ,
	@va_Dis_2_bascbasc	,

	@va_Dis_3_bauvoie      ,
	@va_Dis_3_bau2voies     ,
	@va_Dis_3_baubasc       ,
	@va_Dis_3_voievoie      ,
	@va_Dis_3_voie2voies    ,
	@va_Dis_3_voiebasc	,
	@va_Dis_3_2voies2voies	,
	@va_Dis_3_2voiesbasc	,
	@va_Dis_3_bascbasc	,

	@va_Dis_4_bauvoie	,
	@va_Dis_4_bau2voies	,
	@va_Dis_4_bau3voies	,
	@va_Dis_4_baubasc	,
	@va_Dis_4_voievoie	,
	@va_Dis_4_voie2voies	,
	@va_Dis_4_voie3voies    ,
	@va_Dis_4_voiebasc	,
	@va_Dis_4_2voies2voies	,
	@va_Dis_4_2voies3voies	,
	@va_Dis_4_2voiesbasc	,
	@va_Dis_4_3voies2voies	,
	@va_Dis_4_3voiesbasc	,
	@va_Dis_4_bascbasc	,

        @va_Dis_ZU_in           ,

	@va_Indicateur_out	 output,
	@va_IndicateurZU_out     output

		if @vl_Status = null
                       	return XDC_PRC_INC
                else return @vl_Status
		
	end
	else
		select	@vl_VR_1	=	VR,
			@vl_VM2_1 =	VM2,
			@vl_VM1_1 =	VM1,
			@vl_VL_1	=	VL,
			@vl_BAU_1	=	BAU,
			@vl_VR_I_1  =     VR_I,
			@vl_VM2_I_1 =     VM2_I,
			@vl_VM1_I_1 =     VM1_I,
			@vl_VL_I_1  =     VL_I,
			@vl_BAU_I_1 =     BAU_I,
			@vl_BAU_etroite_1 =     BAU_etroite,
			@vl_BAU_I_etr_1 =     BAU_I_etroite,
			@vl_PR_DebTrav1 = PR
		from EXP..FMC_HIS
		where numero = @va_Numero1_in
		and   cle    = @va_Cle1_in
		and   horodate_validation = @va_Horodate_valid1_in
#else 
#ifdef HIST
		select	@vl_VR_1	=	VR,
			@vl_VM2_1 =	VM2,
			@vl_VM1_1 =	VM1,
			@vl_VL_1	=	VL,
			@vl_BAU_1	=	BAU,
			@vl_VR_I_1  =     VR_I,
			@vl_VM2_I_1 =     VM2_I,
			@vl_VM1_I_1 =     VM1_I,
			@vl_VL_I_1  =     VL_I,
			@vl_BAU_I_1 =     BAU_I,
			@vl_BAU_etroite_1 =     BAU_etroite,
			@vl_BAU_I_etr_1 =     BAU_I_etroite,
			@vl_PR_DebTrav1 = PR
		from HIS..FMC_HIS
		where numero = @va_Numero1_in
		and   cle    = @va_Cle1_in
		and   horodate_validation = @va_Horodate_valid1_in
#else
	select	@vl_VR_1	=	VR,
		@vl_VM2_1 =	VM2,
		@vl_VM1_1 =	VM1,
		@vl_VL_1	=	VL,
		@vl_BAU_1	=	BAU,
		@vl_VR_I_1  =     VR_I,
		@vl_VM2_I_1 =     VM2_I,
		@vl_VM1_I_1 =     VM1_I,
		@vl_VL_I_1  =     VL_I,
		@vl_BAU_I_1 =     BAU_I,
		@vl_BAU_etroite_1 =     BAU_etroite,
		@vl_BAU_I_etr_1 =     BAU_I_etroite,
		@vl_PR_DebTrav1 = PR
	from EXP..FMC_HIS
	where numero = @va_Numero1_in
	and   cle    = @va_Cle1_in
	and   horodate_validation = @va_Horodate_valid1_in
#endif
#endif

	/* Si les informations ne sont pas trouvees */
	if @@rowcount = 0
		return XDC_PAS_SYN
			

	/* Recherche de l'ensemble des informations concernant chacune  */
	/* des voies pour le deuxieme "Travaux".                        */
#ifdef HIST
		select  @vl_VR_2  =       VR,
			@vl_VM2_2 =       VM2,
			@vl_VM1_2 =       VM1,
			@vl_VL_2  =       VL,
			@vl_BAU_2 =       BAU,
			@vl_VR_I_2  =     VR_I,
			@vl_VM2_I_2 =     VM2_I,
			@vl_VM1_I_2 =     VM1_I,
			@vl_VL_I_2  =     VL_I,
			@vl_BAU_I_2 =     BAU_I,
			@vl_BAU_etroite_2 =     BAU_etroite,
			@vl_BAU_I_etr_2 =     BAU_I_etroite,
			@vl_PR_DebTrav2 = PR
		from HIS..FMC_HIS
		where numero = @va_Numero2_in
		and   cle    = @va_Cle2_in
		and   horodate_validation = @va_Horodate_valid2_in
#else
	select  @vl_VR_2  =       VR,
		@vl_VM2_2 =       VM2,
		@vl_VM1_2 =       VM1,
		@vl_VL_2  =       VL,
		@vl_BAU_2 =       BAU,
		@vl_VR_I_2  =     VR_I,
		@vl_VM2_I_2 =     VM2_I,
		@vl_VM1_I_2 =     VM1_I,
		@vl_VL_I_2  =     VL_I,
		@vl_BAU_I_2 =     BAU_I,
		@vl_BAU_etroite_2 =     BAU_etroite,
		@vl_BAU_I_etr_2 =     BAU_I_etroite,
		@vl_PR_DebTrav2 = PR
	from EXP..FMC_HIS
	where numero = @va_Numero2_in
	and   cle    = @va_Cle2_in
	and   horodate_validation = @va_Horodate_valid2_in
#endif

	/* Si les informations les sont pas trouvees */
	if @@rowcount = 0
		return XDC_PAS_SYN


	/*A Mise en place des PR debut et fin fonction du sens */
	if @vl_PR_DebTrav2>@va_PR_FinTrav2_in
	begin
		select @vl_PR=@va_PR_FinTrav2_in
		select @va_PR_FinTrav2_in=@vl_PR_DebTrav2
		select @vl_PR_DebTrav2=@vl_PR
	end
	if @vl_PR_DebTrav1>@va_PR_FinTrav1_in
	begin
		select @vl_PR=@va_PR_FinTrav1_in
		select @va_PR_FinTrav1_in=@vl_PR_DebTrav1
		select @vl_PR_DebTrav1=@vl_PR
	end
	
	
	/* Recherche du nb de voies des travaux aval */
	if ((@va_Sens1_in = XDC_SENS_SUD) and (@vl_sens_aut = 1)) OR ( (@va_Sens1_in = XDC_SENS_NORD) and (@vl_sens_aut = -1 ))
	begin
		if @va_PR_FinTrav1_in>@va_PR_FinTrav2_in
			select @vl_PR_aval = @va_PR_FinTrav1_in
		else
			select @vl_PR_aval = @va_PR_FinTrav2_in
	end
	else
	begin
		if @vl_PR_DebTrav1>@vl_PR_DebTrav2
			select @vl_PR_aval = @vl_PR_DebTrav2
		else
			select @vl_PR_aval = @vl_PR_DebTrav1
	end

	select	@vl_Portion=numero
	from CFG..RES_POR
	where	autoroute = @va_Autoroute1_in and
		@vl_PR_aval > PR_debut and @vl_PR_aval <= PR_fin 

	if @@rowcount = 0
		return XDC_NOK
	if ((@va_Sens1_in = XDC_SENS_SUD) and (@vl_sens_aut = 1)) OR ( (@va_Sens1_in = XDC_SENS_NORD) and (@vl_sens_aut = -1 ))
	begin
		set rowcount 1
		select
			@vl_nb_voies = nombre_de_voies
		from CFG..RES_SEG
		where	portion = @vl_Portion and PR_debut <= @vl_PR_aval and
			sens = @va_Sens1_in
		order by PR_debut desc

		set rowcount 0
	end
	else
	begin
		set rowcount 1
		select
			@vl_nb_voies = nombre_de_voies
		from CFG..RES_SEG
		where	portion = @vl_Portion and PR_debut <= @vl_PR_aval and
			sens = @va_Sens1_in
                order by PR_debut desc

		set rowcount 0
	end
	
	/*A
	** Calcul de la distance separant les travaux.
	*/
	/* SI TRAV1 est avant TRAV2 */
	if @vl_PR_DebTrav2 > @va_PR_FinTrav1_in
	begin
		execute @vl_CodeRet = PRC..XZAO;22 @va_Autoroute1_in,
				           	   @va_PR_FinTrav1_in,
					           @va_Autoroute2_in,
					           @vl_PR_DebTrav2,
					           @vl_LgInterTravaux out
		execute @vl_CodeRet1= PRC..XZAO;22 @va_Autoroute1_in,
						   @va_PR_FinTrav2_in,
					           @va_Autoroute2_in,
					           @vl_PR_DebTrav1,
					           @vl_LgExtrTravaux out
	end
	/* SI TRAV2 est avant TRAV1 */
	else
	if @vl_PR_DebTrav1 > @va_PR_FinTrav2_in
	begin
		execute @vl_CodeRet = PRC..XZAO;22 @va_Autoroute2_in,
						   @va_PR_FinTrav2_in,
						   @va_Autoroute1_in,
						   @vl_PR_DebTrav1,
						   @vl_LgInterTravaux out
		execute @vl_CodeRet1= PRC..XZAO;22 @va_Autoroute2_in,
						   @va_PR_FinTrav1_in,
					           @va_Autoroute1_in,
					           @vl_PR_DebTrav2,
					           @vl_LgExtrTravaux out
	end
	/* SI les travaux se chevauchent */
	else
	begin
		select @vl_LgInterTravaux=0
		/* on prend les extremites */
		if @vl_PR_DebTrav1<@vl_PR_DebTrav2
			select @vl_PR_DebTrav2=@vl_PR_DebTrav1
		if @va_PR_FinTrav1_in>@va_PR_FinTrav2_in
			select @va_PR_FinTrav2_in=@va_PR_FinTrav1_in
			
		execute @vl_CodeRet = PRC..XZAO;22 @va_Autoroute2_in,
					   @vl_PR_DebTrav2,
					   @va_Autoroute2_in,
					   @va_PR_FinTrav2_in,
					   @vl_LgExtrTravaux out
		select @vl_CodeRet1=@vl_CodeRet
	end

	/* Distance entre les deux travaux n'est par calculee */
    	if @vl_CodeRet != XDC_OK
		return @vl_CodeRet
    	if @vl_CodeRet1 != XDC_OK
		return @vl_CodeRet1
	
	if @vl_LgInterTravaux = null
    		return XDC_PAS_SYN

	/*A
	** Preparation des indicateurs en fonction des informations
	** recueuilies.
	*/

	select @vl_nb_voies_neut1=0
	select @vl_nb_voies_neut2=0
	select @vl_bau_neut1=XDC_FAUX
	select @vl_bau_neut2=XDC_FAUX
	
	/* Comptage du nb de voie neutralisees */
	if  (@vl_VR_1 = XDC_VOIE_NEUTRALISEE) or
	    (@vl_VR_1 = XDC_VOIE_BLOQUEE_NEUTRALISEE) 
	    select @vl_nb_voies_neut1= @vl_nb_voies_neut1+1
	if  (@vl_VM1_1 = XDC_VOIE_NEUTRALISEE) or
	    (@vl_VM1_1 = XDC_VOIE_BLOQUEE_NEUTRALISEE) 
	    select @vl_nb_voies_neut1= @vl_nb_voies_neut1+1
	if  (@vl_VM2_1 = XDC_VOIE_NEUTRALISEE) or
	    (@vl_VM2_1 = XDC_VOIE_BLOQUEE_NEUTRALISEE) 
	    select @vl_nb_voies_neut1= @vl_nb_voies_neut1+1
	if  (@vl_VL_1 = XDC_VOIE_NEUTRALISEE) or
	    (@vl_VL_1 = XDC_VOIE_BLOQUEE_NEUTRALISEE) 
	    select @vl_nb_voies_neut1= @vl_nb_voies_neut1+1
	    
	if  (@vl_BAU_etroite_1 = XDC_VOIE_NEUTRALISEE) or
	    (@vl_BAU_etroite_1 = XDC_VOIE_BLOQUEE_NEUTRALISEE) or
	    (@vl_BAU_1 = XDC_VOIE_NEUTRALISEE) or
	    (@vl_BAU_1 = XDC_VOIE_BLOQUEE_NEUTRALISEE)
	    select @vl_bau_neut1=XDC_VRAI
	
	if  (@vl_VR_2 = XDC_VOIE_NEUTRALISEE) or
	    (@vl_VR_2 = XDC_VOIE_BLOQUEE_NEUTRALISEE) 
	    select @vl_nb_voies_neut2= @vl_nb_voies_neut2+1
	if  (@vl_VM1_2 = XDC_VOIE_NEUTRALISEE) or
	    (@vl_VM1_2 = XDC_VOIE_BLOQUEE_NEUTRALISEE) 
	    select @vl_nb_voies_neut2= @vl_nb_voies_neut2+1
	if  (@vl_VM2_2 = XDC_VOIE_NEUTRALISEE) or
	    (@vl_VM2_2 = XDC_VOIE_BLOQUEE_NEUTRALISEE) 
	    select @vl_nb_voies_neut2= @vl_nb_voies_neut2+1
	if  (@vl_VL_2 = XDC_VOIE_NEUTRALISEE) or
	    (@vl_VL_2 = XDC_VOIE_BLOQUEE_NEUTRALISEE) 
	    select @vl_nb_voies_neut2= @vl_nb_voies_neut2+1
	    
	if  (@vl_BAU_etroite_2 = XDC_VOIE_NEUTRALISEE) or
	    (@vl_BAU_etroite_2 = XDC_VOIE_BLOQUEE_NEUTRALISEE) or
	    (@vl_BAU_2 = XDC_VOIE_NEUTRALISEE) or
	    (@vl_BAU_2 = XDC_VOIE_BLOQUEE_NEUTRALISEE)
	    select @vl_bau_neut2=XDC_VRAI
	
	/*A
	** Test sur l'existance d'un basculement , utile si le 
	** nb de voies est differents entre trav1 et trav2
	*/
	/* Il y a basculement si ou moins une voie du sens oppose */
	/* est inversee (XDC_VOIE_SENS_INVERSE)			  */
	if ( 	(@vl_VR_I_1 = XDC_VOIE_SENS_INVERSE) or
		(@vl_VM2_I_1 = XDC_VOIE_SENS_INVERSE) or
		(@vl_VM1_I_1 = XDC_VOIE_SENS_INVERSE) or
		(@vl_VL_I_1 = XDC_VOIE_SENS_INVERSE) or
		(@vl_BAU_I_1 = XDC_VOIE_SENS_INVERSE)or
		(@vl_BAU_I_etr_1 = XDC_VOIE_SENS_INVERSE) ) 
	begin
		select @vl_nb_voies_neut1 =  @vl_nb_voies
	end
	if ( 	(@vl_VR_I_2 = XDC_VOIE_SENS_INVERSE) or
		(@vl_VM2_I_2 = XDC_VOIE_SENS_INVERSE) or
		(@vl_VM1_I_2 = XDC_VOIE_SENS_INVERSE) or
		(@vl_VL_I_2 = XDC_VOIE_SENS_INVERSE) or
		(@vl_BAU_I_2 = XDC_VOIE_SENS_INVERSE)or
		(@vl_BAU_I_etr_2 = XDC_VOIE_SENS_INVERSE) ) 
	begin
		select @vl_nb_voies_neut2 =  @vl_nb_voies
	end

	
	if ( @vl_nb_voies_neut2=0 ) or (  @vl_nb_voies_neut2<@vl_nb_voies_neut1 )
	begin
		select @vl_tempo=@vl_nb_voies_neut2
		select @vl_nb_voies_neut2=@vl_nb_voies_neut1
		select @vl_nb_voies_neut1=@vl_tempo
		select @vl_tempo=@vl_bau_neut2
		select @vl_bau_neut2=@vl_bau_neut1
		select @vl_bau_neut1=@vl_tempo
	end
	

	if @vl_bau_neut1=XDC_FAUX and @vl_nb_voies_neut1=0
		return XDC_OK
	if @vl_bau_neut2=XDC_FAUX and @vl_nb_voies_neut2=0
		return XDC_OK
		
	if (@vl_nb_voies_neut2=0 and @vl_nb_voies_neut1=0)
	begin
		select @vl_Distance=@va_Dis_baubau_in
	end
	else
	begin
	if @vl_nb_voies = 1
	begin
		if @vl_nb_voies_neut2=1
		begin
			select @vl_Distance=@va_Dis_1_bau1voie_in
		end
		else
		begin
			select @vl_Distance=@va_Dis_baubau_in
		end
	end
	else
	begin
	if @vl_nb_voies = 2
	begin
		if @vl_nb_voies_neut1=0
		begin
			if @vl_nb_voies_neut2=1
			begin
				select @vl_Distance=@va_Dis_2_bau1voie_in
			end
			else
			begin
				select @vl_Distance=@va_Dis_2_baubasc
			end
		
		end
		else
		begin
			if @vl_nb_voies_neut1=1
			begin
				if @vl_nb_voies_neut2=1
				begin
					select @vl_Distance=@va_Dis_2_voievoie
				end
				else
				begin
					select @vl_Distance=@va_Dis_2_voiebasc
				end
			end
			else
			begin
				select @vl_Distance=@va_Dis_2_bascbasc
			end
		end
	end
	else
	begin
	if @vl_nb_voies = 3
	begin
		if @vl_nb_voies_neut1=0
		begin
			if @vl_nb_voies_neut2=1
			begin
				select @vl_Distance=@va_Dis_3_bauvoie
			end
			else
			begin
			if @vl_nb_voies_neut2=2
			begin
				select @vl_Distance=@va_Dis_3_bau2voies
			end
			else
			begin
				select @vl_Distance=@va_Dis_3_baubasc
			end
			end
		
		end
		else
		begin
			if @vl_nb_voies_neut1=1
			begin
				if @vl_nb_voies_neut2=1
				begin
					select @vl_Distance=@va_Dis_3_voievoie
				end
				else
				begin
				if @vl_nb_voies_neut2=2
				begin
					select @vl_Distance=@va_Dis_3_voie2voies
				end
				else
				begin
					select @vl_Distance=@va_Dis_3_voiebasc
				end
				end
			end
			else
			begin
			if @vl_nb_voies_neut1=2
			begin
				if @vl_nb_voies_neut2=2
				begin
					select @vl_Distance=@va_Dis_3_2voies2voies
				end
				else
				begin
					select @vl_Distance=@va_Dis_3_2voiesbasc
				end
			end
			else
			begin
				select @vl_Distance=@va_Dis_3_bascbasc
			end
			end
		end
	
	end
	else
	begin
	if @vl_nb_voies = 4
	begin
		if @vl_nb_voies_neut1=0
		begin
			if @vl_nb_voies_neut2=1
			begin
				select @vl_Distance=@va_Dis_4_bauvoie
			end
			else
			begin
			if @vl_nb_voies_neut2=2
			begin
				select @vl_Distance=@va_Dis_4_bau2voies
			end
			else
			begin
			if @vl_nb_voies_neut2=3
			begin
				select @vl_Distance=@va_Dis_4_bau3voies
			end
			else
			begin
				select @vl_Distance=@va_Dis_4_baubasc
			end
			end
			end
		
		end
		else
		begin
			if @vl_nb_voies_neut1=1
			begin
				if @vl_nb_voies_neut2=1
				begin
					select @vl_Distance=@va_Dis_4_voievoie
				end
				else
				begin
				if @vl_nb_voies_neut2=2
				begin
					select @vl_Distance=@va_Dis_4_voie2voies
				end
				else
				begin
				if @vl_nb_voies_neut2=3
				begin
					select @vl_Distance=@va_Dis_4_voie3voies
				end
				else
				begin
					select @vl_Distance=@va_Dis_4_voiebasc
				end
				end
				end
			end
			else
			begin
			if @vl_nb_voies_neut1=2
			begin
				if @vl_nb_voies_neut2=2
				begin
					select @vl_Distance=@va_Dis_4_2voies2voies
				end
				else
				begin
				if @vl_nb_voies_neut2=3
				begin
					select @vl_Distance=@va_Dis_4_2voies3voies
				end
				else
				begin
					select @vl_Distance=@va_Dis_4_2voiesbasc
				end
				end
			end
			else
			begin
			if @vl_nb_voies_neut1=3
			begin
				if @vl_nb_voies_neut2=3
				begin
					select @vl_Distance=@va_Dis_4_3voies2voies
				end
				else
				begin
					select @vl_Distance=@va_Dis_4_3voiesbasc
				end
			end
			else
			begin
				select @vl_Distance=@va_Dis_4_bascbasc
			end
			end
			end
		end
	
	end
	else
	begin
 		return XDC_OK
	end
	end
	end
	end
	end


	if (1000 * @vl_Distance)  > @vl_LgInterTravaux
		select @va_Indicateur_out = XDC_NOK
	if (1000 * @va_Dis_ZU_in) < @vl_LgExtrTravaux
		/* Les deux travaux sont dissocies en ZU */
		select @va_IndicateurZU_out = XDC_NOK
	/*A
	** Fin de la procedure
	*/
 	return XDC_OK
go
