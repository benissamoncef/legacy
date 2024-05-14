/*E*/
/*  Fichier : $Id: xzah15sp.prc,v 1.10 2017/04/02 15:49:15 pc2dpdy Exp $      Release : $Revision: 1.10 $        Date : $Date: 2017/04/02 15:49:15 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah15sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm
*
* HISTORIQUE :
*
* B.G.  18/10/94        : Cr≈ation
* B.G.  27/03/95        : Ajout de la zone totale de travaux mobile (V 1.2)
* C.T.  09/09/96        : Ajout de la suppression de l'objet en prevision lors
*                         du passage des travaux prevus ‡ en cours (PADT V 1.3)
* P.N.  14/01/97        : Plus de neutralisation en voies inverses pour les rabattements (dem/1372) v1.4
* P.N. 15/12/97		: correction des rabattements et neutralisation de voies dem/1517 
* JPL	10/09/10	: Prise en compte Fmc Zone de stockage (DEM 942) 1.8
* VR	12/03/12	: Ajout vitesse dans la diffusion (DEM/1016)
# PNI	09/03/17	: correction des arg d'appel de XZAH12
*-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare l'animation IHM (travaux)
* 
* Sequence d'appel
* SP	XZAH15_Animer_Travaux
* 
* Arguments en entree
* XDY_Evt		va_NumEvt_in
* XDY_Autoroute		va_Autoroute_in
* XDY_Entier		va_TypeEvt_in
* XDY_PR		va_PR_deb_in
* XDY_PR		va_PR_fin_in
* XDY_Sens		va_Sens_in
* XDY_Octet		va_TypPtCar_in
* XDY_Octet		va_PtCar_in
* XDY_Octet		va_PosPtCar_in
* XDY_Horodate		va_Horoval_in
* XDY_Horodate		va_Horodeb_in
* XDY_Octet		va_VR_in
* XDY_Octet		va_VM2_in
* XDY_Octet		va_VM1_in
* XDY_Octet		va_VL_in
* XDY_Octet		va_BAU_in
* XDY_Octet		va_VR_I_in
* XDY_Octet		va_VM2_I_in
* XDY_Octet		va_VM1_I_in
* XDY_Octet		va_VL_I_in
* XDY_Octet		va_BAU_I_in
* XDY_Octet		va_Suppr_in
* 
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: argument invalide
* 
* Conditions d'utilisation
* 
* appel≈ sur trigger par fmc_his_iu (fmc_his.trg)
*------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAH15' and type = 'P')
	drop procedure XZAH15
go


create procedure  XZAH;15
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null,
	@va_TypeEvt_in		smallint	= null,
	@va_Autoroute_in	T_AUTOROUTE	= null,
	@va_PR_in		T_PR		= null,
	@va_Sens_in		T_SENS		= null,
	@va_TypPtCar_in		tinyint		= null,
	@va_PtCar_in		tinyint		= null,
	@va_PosPtCar_in		tinyint		= null,
	@va_Horoval_in		datetime	= null,
	@va_Horodeb_in		datetime	= null,
	@va_VR_in               tinyint		= null,
	@va_VM2_in              tinyint		= null,
	@va_VM1_in              tinyint		= null,
	@va_VL_in               tinyint		= null,
	@va_BAU_in              tinyint		= null,
	@va_VR_I_in             tinyint		= null,
	@va_VM2_I_in            tinyint		= null,
	@va_VM1_I_in            tinyint		= null,
	@va_VL_I_in             tinyint		= null,
	@va_BAU_I_in            tinyint		= null,
	@va_Suppr_in		bit
as

/*A declaration des variables */

	declare	@vl_Mobile	bit,
		@vl_PRTete	T_PR,
		@vl_PRQueue	T_PR,
		@vl_Type	tinyint,
		@vl_Type_Tmp	tinyint,
		@vl_TypeQueue	tinyint,
		@vl_TypeTete	tinyint,
		@vl_Sens_Opp	tinyint,
		@vl_PR_debut_VR T_PR,
		@vl_PR_fin_VR	T_PR,
		@vl_debut_VR	datetime,
		@vl_fin_VR	datetime,
		@vl_PR_debut_VM2 T_PR,
		@vl_PR_fin_VM2	T_PR,
		@vl_debut_VM2	datetime,
		@vl_fin_VM2	datetime,
		@vl_PR_debut_VM1 T_PR,
		@vl_PR_fin_VM1	T_PR,
		@vl_debut_VM1	datetime,
		@vl_fin_VM1	datetime,
		@vl_PR_debut_VL T_PR,
		@vl_PR_fin_VL	T_PR,
		@vl_debut_VL	datetime,
		@vl_fin_VL	datetime,
		@vl_PR_debut_BAU T_PR,
		@vl_PR_fin_BAU	T_PR,
		@vl_debut_BAU	datetime,
		@vl_fin_BAU	datetime,
		@vl_PR_debut_VR_I T_PR,
		@vl_PR_fin_VR_I	T_PR,
		@vl_debut_VR_I	datetime,
		@vl_fin_VR_I	datetime,
		@vl_PR_debut_VM2_I T_PR,
		@vl_PR_fin_VM2_I	T_PR,
		@vl_debut_VM2_I	datetime,
		@vl_fin_VM2_I	datetime,
		@vl_PR_debut_VM1_I T_PR,
		@vl_PR_fin_VM1_I	T_PR,
		@vl_debut_VM1_I	datetime,
		@vl_fin_VM1_I	datetime,
		@vl_PR_debut_VL_I T_PR,
		@vl_PR_fin_VL_I	T_PR,
		@vl_debut_VL_I	datetime,
		@vl_fin_VL_I	datetime,
		@vl_PR_debut_BAU_I T_PR,
		@vl_PR_fin_BAU_I	T_PR,
		@vl_debut_BAU_I	datetime,
		@vl_fin_BAU_I	datetime,
		@vl_numordre	tinyint,
		@vl_DateCourante datetime,
		@vl_Vitesse		tinyint

/*A Test des param…tres d'appel */

/*B	Test des param…tres nuls */

	if	@va_NumEvt_in=null or
		@va_CleEvt_in=null
	return XDC_NOK

	select @vl_numordre=1

/*A S'il s'agit d'une suppression, supprime le rabattement, le symbole et 
    les voies neutralis≈es */

	if @va_Suppr_in=XDC_VRAI
	begin
		exec PRC..XZAH;13	@va_NumEvt_in=@va_NumEvt_in,
					@va_CleEvt_in=@va_CleEvt_in,
					@va_Suppr_in=XDC_VRAI	

		exec PRC..XZAH;12	@va_NumEvt_in=@va_NumEvt_in,
					@va_CleEvt_in=@va_CleEvt_in,
					@va_Suppr_in=XDC_VRAI	
		
		exec PRC..XZAH;17	@va_NumEvt_in=@va_NumEvt_in,
					@va_CleEvt_in=@va_CleEvt_in,
					@va_Suppr_in=XDC_VRAI	

		exec PRC..XZAH;18	@va_NumEvt_in=@va_NumEvt_in,
					@va_CleEvt_in=@va_CleEvt_in,
					@va_Suppr_in=XDC_VRAI	
		return XDC_OK
	end

/*A Test des param…tres d'appel (suite)*/

/*B	Test des param…tres nuls */

	if	@va_TypeEvt_in=null or
		@va_Autoroute_in=null or
		@va_PR_in=null or
		@va_Sens_in=null or
		@va_Horoval_in=null or
		@va_VR_in=null or 
		@va_VM2_in=null or
		@va_VM1_in=null or
		@va_VL_in=null or
		@va_BAU_in=null or
		@va_VR_I_in=null or
		@va_VM2_I_in=null or
		@va_VM1_I_in=null or
		@va_VL_I_in=null or
		@va_BAU_I_in=null
	return XDC_NOK

/*A lecture du compl≈ment travaux */

	select @vl_Mobile=mobile
	from EXP..FMC_TRF
	where numero=@va_NumEvt_in and cle=@va_CleEvt_in

	if @@rowcount<>1 return XDC_NOK

	select @vl_PRTete=localisation_tete
	from EXP..FMC_TRH
	where	numero=@va_NumEvt_in and
		cle=@va_CleEvt_in and
		horodate=@va_Horoval_in

	if @@rowcount<>1 return XDC_NOK

/*A	choix du type (mobile ou fixe, reel ou prevu) pour les travaux, */
/*A	pas de conversion du type pour les autres types de la classe    */

	if @va_TypeEvt_in = XZAEC_FMC_ZoneStockage
		select @vl_Type=@va_TypeEvt_in
	else
	begin
		if @va_Horodeb_in<>null and @vl_Mobile=XDC_VRAI
			select @vl_Type=XZAHC_Travaux_Mob_R
		if @va_Horodeb_in=null and @vl_Mobile=XDC_VRAI
			select @vl_Type=XZAHC_Travaux_Mob_P
		if @va_Horodeb_in<>null and @vl_Mobile=XDC_FAUX
			select @vl_Type=XZAHC_Travaux_Fix_R
		if @va_Horodeb_in=null and @vl_Mobile=XDC_FAUX
			select @vl_Type=XZAHC_Travaux_Fix_P
	end

/*A suppression de l'objet travaux prevu lors du passage a en cours */
	if (	@vl_Type=XZAHC_Travaux_Mob_R or 
		@vl_Type=XZAHC_Travaux_Fix_R ) and
	   exists (	select * from EXP..SYN_OBJ_POR 
			where evenement = @va_NumEvt_in and cle = @va_CleEvt_in and
			(type_objet = XZAHC_Travaux_Mob_P or type_objet = XZAHC_Travaux_Fix_P))
	begin
		exec PRC..XZAH;12	@va_NumEvt_in=@va_NumEvt_in,
					@va_CleEvt_in=@va_CleEvt_in,
					@va_Suppr_in=XDC_VRAI	
	end

/*A si les travaux ont commenc≈ */

	if @va_Horodeb_in<>null
	begin

/*B	animation des voies neutralis≈es */
	
	if	@va_VR_I_in=XDC_VOIE_BLOQUEE_NEUTRALISEE or
		@va_VR_I_in=XDC_VOIE_NEUTRALISEE
	select	@va_VR_I_in=XDC_VOIE_INEXISTANTE
	if	@va_VL_I_in=XDC_VOIE_BLOQUEE_NEUTRALISEE or
		@va_VL_I_in=XDC_VOIE_NEUTRALISEE
	select	@va_VL_I_in=XDC_VOIE_INEXISTANTE
	if	@va_VM1_I_in=XDC_VOIE_BLOQUEE_NEUTRALISEE or
		@va_VM1_I_in=XDC_VOIE_NEUTRALISEE
	select	@va_VM1_I_in=XDC_VOIE_INEXISTANTE
	if	@va_VM2_I_in=XDC_VOIE_BLOQUEE_NEUTRALISEE or
		@va_VM2_I_in=XDC_VOIE_NEUTRALISEE
	select	@va_VM2_I_in=XDC_VOIE_INEXISTANTE
	if	@va_BAU_I_in=XDC_VOIE_BLOQUEE_NEUTRALISEE or
		@va_BAU_I_in=XDC_VOIE_NEUTRALISEE
	select	@va_BAU_I_in=XDC_VOIE_INEXISTANTE

	

/*A lecture du compl≈ment presignalisation */

	select 
		@vl_PR_debut_VR=PR_debut_VR,
		@vl_PR_fin_VR=PR_fin_VR,
		@vl_debut_VR=debut_VR,
		@vl_fin_VR=fin_VR,
		@vl_PR_debut_VM2=PR_debut_VM2,
		@vl_PR_fin_VM2=PR_fin_VM2,
		@vl_debut_VM2=debut_VM2,
		@vl_fin_VM2=fin_VM2,
		@vl_PR_debut_VM1=PR_debut_VM1,
		@vl_PR_fin_VM1=PR_fin_VM1,
		@vl_debut_VM1=debut_VM1,
		@vl_fin_VM1=fin_VM1,
		@vl_PR_debut_VL=PR_debut_VL,
		@vl_PR_fin_VL=PR_fin_VL,
		@vl_debut_VL=debut_VL,
		@vl_fin_VL=fin_VL,
		@vl_PR_debut_BAU=PR_debut_BAU,
		@vl_PR_fin_BAU=PR_fin_BAU,
		@vl_debut_BAU=debut_BAU,
		@vl_fin_BAU=fin_BAU,
		@vl_PR_debut_VR_I=PR_debut_VR_I,
		@vl_PR_fin_VR_I=PR_fin_VR_I,
		@vl_debut_VR_I=debut_VR_I,
		@vl_fin_VR_I=fin_VR_I,
		@vl_PR_debut_VM2_I=PR_debut_VM2_I,
		@vl_PR_fin_VM2_I=PR_fin_VM2_I,
		@vl_debut_VM2_I=debut_VM2_I,
		@vl_fin_VM2_I=fin_VM2_I,
		@vl_PR_debut_VM1_I =PR_debut_VM1_I,
		@vl_PR_fin_VM1_I=PR_fin_VM1_I,
		@vl_debut_VM1_I=debut_VM1_I,
		@vl_fin_VM1_I=fin_VM1_I,
		@vl_PR_debut_VL_I=PR_debut_VL_I,
		@vl_PR_fin_VL_I=PR_fin_VL_I,
		@vl_debut_VL_I=debut_VL_I,
		@vl_fin_VL_I=fin_VL_I,
		@vl_PR_debut_BAU_I=PR_debut_BAU_I,
		@vl_PR_fin_BAU_I=PR_fin_BAU_I,
		@vl_debut_BAU_I=debut_BAU_I,
		@vl_fin_BAU_I=fin_BAU_I
	from EXP..FMC_TRF
	where	numero=@va_NumEvt_in and
		cle=@va_CleEvt_in

	select @vl_PRTete=localisation_tete
	from EXP..FMC_TRH
	where	numero=@va_NumEvt_in and
		cle=@va_CleEvt_in
	group by horodate
	having horodate=max(horodate)

/*		exec PRC..XZAH;17	@va_NumEvt_in,
					@va_CleEvt_in,
					@va_Autoroute_in,
					@va_PR_in,
					@vl_PRTete,
					@va_Sens_in,
					@va_VR_in,
					@va_VM2_in,
					@va_VM1_in,
					@va_VL_in,
					@va_BAU_in,
					@va_VR_I_in,
					@va_VM2_I_in,
					@va_VM1_I_in,
					@va_VL_I_in,
					@va_BAU_I_in,
					XDC_FAUX
						
*/


	if @va_Sens_in = XDC_SENS_SUD
		select @vl_Sens_Opp = XDC_SENS_NORD
	else
		select @vl_Sens_Opp = XDC_SENS_SUD
		

	select @vl_DateCourante = getdate ()
	update EXP..SYN_NEU_SEG
	set ordre=XDC_SUPPR
	where	evenement=@va_NumEvt_in and
		cle=@va_CleEvt_in
	update EXP..SYN_RAB_SEG
	set ordre=XDC_SUPPR
	where	evenement=@va_NumEvt_in and
		cle=@va_CleEvt_in
	update EXP..SYN_INV_SEG
	set ordre=XDC_SUPPR
	where	evenement=@va_NumEvt_in and
		cle=@va_CleEvt_in
	update EXP..SYN_BAS_SEG
	set ordre=XDC_SUPPR
	where	evenement_basc=@va_NumEvt_in and
		cle_basc=@va_CleEvt_in

	if ( @vl_debut_VR != NULL or @vl_debut_VR <= @vl_DateCourante ) and
	   ( @vl_fin_VR = NULL or @vl_fin_VR > @vl_DateCourante )
		exec PRC..XZAH;32	@va_NumEvt_in,
					@va_CleEvt_in,
					@va_Autoroute_in,
					@vl_PR_debut_VR,
					@vl_PR_fin_VR,
					@va_Sens_in,
					XDC_FAUX,
					@vl_numordre,
					XZAHC_VR
	   
	select @vl_numordre=@vl_numordre+1
	
	if ( @vl_debut_VM2 != NULL or @vl_debut_VM2 <= @vl_DateCourante ) and
	   ( @vl_fin_VM2 = NULL or @vl_fin_VM2 > @vl_DateCourante )
		exec PRC..XZAH;32	@va_NumEvt_in,
					@va_CleEvt_in,
					@va_Autoroute_in,
					@vl_PR_debut_VM2,
					@vl_PR_fin_VM2,
					@va_Sens_in,
					XDC_FAUX,
					@vl_numordre,
					XZAHC_VM2
	   
	select @vl_numordre=@vl_numordre+1
	
	   
	if ( @vl_debut_VM1 != NULL or @vl_debut_VM1 <= @vl_DateCourante ) and
	   ( @vl_fin_VM1 = NULL or @vl_fin_VM1 > @vl_DateCourante )
		exec PRC..XZAH;32	@va_NumEvt_in,
					@va_CleEvt_in,
					@va_Autoroute_in,
					@vl_PR_debut_VM1,
					@vl_PR_fin_VM1,
					@va_Sens_in,
					XDC_FAUX,
					@vl_numordre,
					XZAHC_VM1
	   
	select @vl_numordre=@vl_numordre+1
	
	   
	if ( @vl_debut_VL != NULL or @vl_debut_VL <= @vl_DateCourante ) and
	   ( @vl_fin_VL = NULL or @vl_fin_VL > @vl_DateCourante )
		exec PRC..XZAH;32	@va_NumEvt_in,
					@va_CleEvt_in,
					@va_Autoroute_in,
					@vl_PR_debut_VL,
					@vl_PR_fin_VL,
					@va_Sens_in,
					XDC_FAUX,
					@vl_numordre,
					XZAHC_VL
	   
	select @vl_numordre=@vl_numordre+1
	
	   
	if ( @vl_debut_BAU != NULL or @vl_debut_BAU <= @vl_DateCourante ) and
	   ( @vl_fin_BAU = NULL or @vl_fin_BAU > @vl_DateCourante )
		exec PRC..XZAH;32	@va_NumEvt_in,
					@va_CleEvt_in,
					@va_Autoroute_in,
					@vl_PR_debut_BAU,
					@vl_PR_fin_BAU,
					@va_Sens_in,
					XDC_FAUX,
					@vl_numordre,
					XZAHC_BAU
	   
	select @vl_numordre=@vl_numordre+1
	
	if ( @vl_debut_VR_I != NULL or @vl_debut_VR_I <= @vl_DateCourante ) and
	   ( @vl_fin_VR_I = NULL or @vl_fin_VR_I > @vl_DateCourante )
		exec PRC..XZAH;32	@va_NumEvt_in,
					@va_CleEvt_in,
					@va_Autoroute_in,
					@vl_PR_debut_VR_I,
					@vl_PR_fin_VR_I,
					@vl_Sens_Opp,
					XDC_VRAI,
					@vl_numordre,
					XZAHC_VR
	   
	select @vl_numordre=@vl_numordre+1
	
	if ( @vl_debut_VM2_I != NULL or @vl_debut_VM2_I <= @vl_DateCourante ) and
	   ( @vl_fin_VM2_I = NULL or @vl_fin_VM2_I > @vl_DateCourante )
		exec PRC..XZAH;32	@va_NumEvt_in,
					@va_CleEvt_in,
					@va_Autoroute_in,
					@vl_PR_debut_VM2_I,
					@vl_PR_fin_VM2_I,
					@vl_Sens_Opp,
					XDC_VRAI,
					@vl_numordre,
					XZAHC_VM2
	   
	select @vl_numordre=@vl_numordre+1
	
	   
	if ( @vl_debut_VM1_I != NULL or @vl_debut_VM1_I <= @vl_DateCourante ) and
	   ( @vl_fin_VM1_I = NULL or @vl_fin_VM1_I > @vl_DateCourante )
		exec PRC..XZAH;32	@va_NumEvt_in,
					@va_CleEvt_in,
					@va_Autoroute_in,
					@vl_PR_debut_VM1_I,
					@vl_PR_fin_VM1_I,
					@vl_Sens_Opp,
					XDC_VRAI,
					@vl_numordre,
					XZAHC_VM1
	   
	select @vl_numordre=@vl_numordre+1
	
	   
	if ( @vl_debut_VL_I != NULL or @vl_debut_VL_I <= @vl_DateCourante ) and
	   ( @vl_fin_VL_I = NULL or @vl_fin_VL_I > @vl_DateCourante )
		exec PRC..XZAH;32	@va_NumEvt_in,
					@va_CleEvt_in,
					@va_Autoroute_in,
					@vl_PR_debut_VL_I,
					@vl_PR_fin_VL_I,
					@vl_Sens_Opp,
					XDC_VRAI,
					@vl_numordre,
					XZAHC_VL
	   
	   
	select @vl_numordre=@vl_numordre+1
	
	if ( @vl_debut_BAU_I != NULL or @vl_debut_BAU_I <= @vl_DateCourante ) and
	   ( @vl_fin_BAU_I = NULL or @vl_fin_BAU_I > @vl_DateCourante )
		exec PRC..XZAH;32	@va_NumEvt_in,
					@va_CleEvt_in,
					@va_Autoroute_in,
					@vl_PR_debut_BAU_I,
					@vl_PR_fin_BAU_I,
					@vl_Sens_Opp,
					XDC_VRAI,
					@vl_numordre,
					XZAHC_BAU
	   
/*B	puis animation du symbole de rabattement */
/*
		exec PRC..XZAH;13	@va_NumEvt_in,
					@va_CleEvt_in,
					@va_Autoroute_in,
					@va_PR_in,
					@va_Sens_in,
					XZAHC_Ponctuel,
					@va_VR_in,
					@va_VM2_in,
					@va_VM1_in,
					@va_VL_in,
					@va_BAU_in,
					@va_VR_I_in,
					@va_VM2_I_in,
					@va_VM1_I_in,
					@va_VL_I_in,
					@va_BAU_I_in,
					XDC_FAUX
*/

/*A puis voies RADT invers≈es */
	exec PRC..XZAH;18	@va_NumEvt_in,
				@va_CleEvt_in,
				@va_Autoroute_in,
				@va_PR_in,
				@vl_PRTete,
				@va_Sens_in,
				@va_VR_in,
				@va_VM2_in,
				@va_VM1_in,
				@va_VL_in,
				@va_BAU_in,
				@va_VR_I_in,
				@va_VM2_I_in,
				@va_VM1_I_in,
				@va_VL_I_in,
				@va_BAU_I_in,
				XDC_FAUX 

/*A puis bouchon invers≈ */

	select @vl_Type_Tmp=null
	exec PRC..XZAH;19	@va_NumEvt_in, 
				@va_CleEvt_in, 
				XZAEC_FMC_Basculement, 
				@vl_Type_Tmp,
				@va_Autoroute_in, 
				@va_PR_in, 
				@vl_PRTete, 
				@va_Sens_in,
				@va_VR_in,
				@va_VM2_in,
				@va_VM1_in,
				@va_VL_in,
				@va_BAU_in,
				@va_VR_I_in,
				@va_VM2_I_in,
				@va_VM1_I_in,
				@va_VL_I_in,
				@va_BAU_I_in,
				XDC_FAUX

	end

/*A dans tous les cas, positionnement des symboles travaux */
	
	exec PRC..XZAH;12	@va_NumEvt_in,
				@va_CleEvt_in,
				/*vl_Vitesse,*/
				null,
				null,
				@vl_Type,
				@va_Autoroute_in,
				@va_PR_in,
				@va_Sens_in,
				XZAHC_VoieInconnue,
				@va_TypPtCar_in,
				@va_PtCar_in,
				@va_PosPtCar_in,
				XDC_FAUX

/*B	zone totale des travaux mobiles */
	if @vl_Mobile=XDC_VRAI
	begin
		/* type de travaux mobile */
		if @va_Horodeb_in<>null
			select	@vl_TypeTete = XZAHC_Travaux_Mob_Tete_R,
				@vl_TypeQueue =XZAHC_Travaux_Mob_Queue_R
		else
			select	@vl_TypeTete = XZAHC_Travaux_Mob_Tete_P,
				@vl_TypeQueue =XZAHC_Travaux_Mob_Queue_P

		/* localisation de la zone totale de travaux mobile */
		select
			@va_Autoroute_in = autoroute_planifiee,
			@vl_PRTete = PR_tete_planifie,
			@vl_PRQueue = PR_queue_planifie,
			@va_Sens_in = sens_planifie
		from EXP..FMC_TRF
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in

		if @@rowcount = 0
			return XDC_OK

		/* ajout de l'objet de tete de travaux mobile */
		exec PRC..XZAH;12	@va_NumEvt_in,
					@va_CleEvt_in,
					/*vl_Vitesse,*/
					null,
					null,
					@vl_TypeTete,
					@va_Autoroute_in,
					@vl_PRTete,
					@va_Sens_in,
					XZAHC_VoieInconnue,
					@va_TypPtCar_in,
					@va_PtCar_in,
					@va_PosPtCar_in,
					XDC_FAUX

		/* ajout de l'objet de queue de travaux mobile */
		exec PRC..XZAH;12	@va_NumEvt_in,
					@va_CleEvt_in,
					/*vl_Vitesse,*/
					null,
					null,
					@vl_TypeQueue,
					@va_Autoroute_in,
					@vl_PRQueue,
					@va_Sens_in,
					XZAHC_VoieInconnue,
					@va_TypPtCar_in,
					@va_PtCar_in,
					@va_PosPtCar_in,
					XDC_FAUX
	end

	return XDC_OK
go
