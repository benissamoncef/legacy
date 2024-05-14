/*E*/
/*  Fichier : $Id: xzah32sp.prc,v 1.3 1997/12/16 11:48:52 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 1997/12/16 11:48:52 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah32sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.  8/01/97        : Cr≈ation dem/1372
* P.N. 13/11/97		: remplacement de FMC_BAS par FMC_TRF dem/1372
* P.N. 15/12/97		: correction des rabattements dem/1517 
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare l'animation IHM de la presignalisation d'un basculement
* 
* Sequence d'appel
* SP	XZAH32_Animer_Presignalisation
* 
* Arguments en entree
* XDY_Evt		va_NumEvt_in
* XDY_Autoroute		va_Autoroute_in
* XDY_PR		va_PR_deb_in
* XDY_PR		va_PR_fin_in
* XDY_Sens		va_Sens_in
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
* XDC_NOK	: arguments invalides
* 
* Conditions d'utilisation
* appel≈e par XZAH;16
* 
------------------------------------------------------*/

use PRC
go

create procedure  XZAH;32
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null,
	@va_Autoroute_in	T_AUTOROUTE	= null,
	@va_PRdeb_in		T_PR		= null,
	@va_PRfin_in		T_PR		= null,
	@va_Sens_in		T_SENS		= null,
	@va_Oppose		bit,
	@va_numordre		tinyint		= null,
	@va_TypeVoie_in         tinyint		= null
as

/*A declaration des variables */

	declare @vl_voie	tinyint,
		@vl_PR		int,
		@vl_portion	smallint,
		@vl_PRdebP	int, -- d≈but portion
		@vl_PRfinP	int, -- fin portion
		@vl_PRdebS	int, -- d≈but segment
		@vl_PRfinS	int, -- fin segment
		@vl_sens	tinyint, -- sens segment
		@vl_chaussee	tinyint, -- sens commun de la chaussee
		@vl_extension	int,
		@vl_translation int,
		@vl_extensionp	int,
		@vl_translationp int,
		@vl_cpt		tinyint,
		@vl_fin		int,
		@vl_debut	int,
		@vl_finp	int,
		@vl_debutp	int,
		@vl_PR_rab	T_PR,
		@vl_PR_rab_fin	T_PR,
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
		@vl_PR_debut_VR_I T_PR,
		@vl_PR_fin_VR_I	T_PR,
		@vl_debut_VR_I	datetime,
		@vl_fin_VR_I	datetime,
		@vl_PR_debut_BAU_I T_PR,
		@vl_PR_fin_BAU_I	T_PR,
		@vl_debut_BAU_I	datetime,
		@vl_fin_BAU_I	datetime,
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
		@vl_PR_debut_VR T_PR,
		@vl_PR_fin_VR	T_PR,
		@vl_debut_VR	datetime,
		@vl_fin_VR	datetime,
		@vl_PR_debut_BAU T_PR,
		@vl_PR_fin_BAU	T_PR,
		@vl_debut_BAU	datetime,
		@vl_fin_BAU	datetime,
		@vl_DateCourante datetime,
		@vl_voie_opp	tinyint,
		@vl_sens_opp	tinyint,
		@vl_voie1	tinyint,
		@vl_voie2	tinyint,
		@vl_voie3	tinyint,
		@vl_voie4	tinyint,
		@vl_voie5	tinyint,
		@vl_nvoie1	tinyint,
		@vl_nvoie2	tinyint,
		@vl_nvoie3	tinyint,
		@vl_nvoie4	tinyint,
		@vl_nvoie5	tinyint,
		@vl_voiet1	tinyint,
		@vl_voiet2	tinyint,
		@vl_voiet3	tinyint,
		@vl_voiet4	tinyint,
		@vl_voiet5	tinyint,
		@vl_nvoiet1	tinyint,
		@vl_nvoiet2	tinyint,
		@vl_nvoiet3	tinyint,
		@vl_nvoiet4	tinyint,
		@vl_nvoiet5	tinyint,
		@vl_numordre_fin	tinyint,
		@vl_SensAuto	smallint



/*A Test des param…tres d'appel */

/*B	Test des param…tres nuls */

	if	@va_NumEvt_in=null or
		@va_CleEvt_in=null
	return XDC_NOK


/*A Test des param…tres d'appel (suite)*/

/*B	Test des param…tres nuls */

	if	@va_Autoroute_in=null or
		@va_PRdeb_in=null or
		@va_PRfin_in=null or
		@va_Sens_in=null or
		@va_numordre=null or
		@va_TypeVoie_in=null
	return XDC_NOK

	select @vl_DateCourante = getdate ()
	
	select @vl_voie1=XDC_VOIE_INEXISTANTE
	select @vl_voie2=XDC_VOIE_INEXISTANTE
	select @vl_voie3=XDC_VOIE_INEXISTANTE
	select @vl_voie4=XDC_VOIE_INEXISTANTE
	select @vl_voie5=XDC_VOIE_SENS_NORMAL
	select @vl_nvoie1=XDC_VOIE_INEXISTANTE
	select @vl_nvoie2=XDC_VOIE_INEXISTANTE
	select @vl_nvoie3=XDC_VOIE_INEXISTANTE
	select @vl_nvoie4=XDC_VOIE_INEXISTANTE
	select @vl_nvoie5=XDC_VOIE_INEXISTANTE
	select @vl_voiet1=XDC_VOIE_INEXISTANTE
	select @vl_voiet2=XDC_VOIE_INEXISTANTE
	select @vl_voiet3=XDC_VOIE_INEXISTANTE
	select @vl_voiet4=XDC_VOIE_INEXISTANTE
	select @vl_voiet5=XDC_VOIE_INEXISTANTE
	select @vl_nvoiet1=XDC_VOIE_INEXISTANTE
	select @vl_nvoiet2=XDC_VOIE_INEXISTANTE
	select @vl_nvoiet3=XDC_VOIE_INEXISTANTE
	select @vl_nvoiet4=XDC_VOIE_INEXISTANTE
	select @vl_nvoiet5=XDC_VOIE_INEXISTANTE
	
	select @vl_numordre_fin=@va_numordre*10

/*A Si les voies ne sont pas sur section courante : c'est fini */

	if @va_Sens_in<>XDC_SENS_NORD and @va_Sens_in<>XDC_SENS_SUD
	return XDC_OK


/*A on ordonne les PR */	
	if @va_PRdeb_in > @va_PRfin_in
	begin
		select @vl_PR=@va_PRdeb_in
		select @va_PRdeb_in=@va_PRfin_in
		select @va_PRfin_in=@vl_PR
	end
	
	select @vl_SensAuto=inversee from CFG..RES_AUT_INV where numero=@va_Autoroute_in
	
	if ((@va_Sens_in=XDC_SENS_SUD and @vl_SensAuto=1) or 
	    ( @va_Sens_in=XDC_SENS_NORD and @vl_SensAuto=-1)) 
	select @vl_chaussee=XDC_SENS_SUD
	    
	if ((@va_Sens_in=XDC_SENS_NORD and @vl_SensAuto=1) or 
	    ( @va_Sens_in=XDC_SENS_SUD and @vl_SensAuto=-1))
	select @vl_chaussee=XDC_SENS_NORD
	
	if (@va_Sens_in=XDC_SENS_NORD and @vl_SensAuto=1) or 
	   ( @va_Sens_in=XDC_SENS_SUD and @vl_SensAuto=-1)
	begin
	   select @vl_PR_rab=@va_PRfin_in
	   select @vl_PR_rab_fin=@va_PRdeb_in
	end
	else
	begin
	   select @vl_PR_rab=@va_PRdeb_in
	   select @vl_PR_rab_fin=@va_PRfin_in
	end

	if @va_Sens_in=XDC_SENS_NORD
		select @vl_sens_opp=XDC_SENS_SUD
	else
		select @vl_sens_opp=XDC_SENS_NORD
		
/*A Recherche des segments concern≈s */

	declare Segment cursor for
		select	CFG..RES_POR.numero,
			CFG..RES_POR.PR_debut,
			CFG..RES_POR.PR_fin,
			CFG..RES_SEG.PR_debut,
			CFG..RES_SEG.sens,
			CFG..RES_SEG.nombre_de_voies

		from	CFG..RES_POR, CFG..RES_SEG

		where	CFG..RES_POR.numero=CFG..RES_SEG.portion and
			CFG..RES_POR.autoroute=@va_Autoroute_in and
			CFG..RES_POR.PR_fin>=@va_PRdeb_in and
			CFG..RES_POR.PR_debut<@va_PRfin_in and
			CFG..RES_SEG.PR_debut<@va_PRfin_in and
			CFG..RES_SEG.sens=@va_Sens_in 

	open Segment
	
	fetch Segment into	@vl_portion,
				@vl_PRdebP,
				@vl_PRfinP,
				@vl_PRdebS,
				@vl_sens,
				@vl_voie

/*A Pour chaque segment trouv≈ : */

	while (@@sqlstatus = 0)
	begin

/*A	recherche le PR de fin : debut du suivant ou fin de portion */

		select @vl_PRfinS=min(PR_debut)
		from CFG..RES_SEG
		where	portion=@vl_portion and
			sens=@vl_sens and
			PR_debut>@vl_PRdebS

		select @vl_PRfinS=isnull(@vl_PRfinS,@vl_PRfinP)

/*A	v≈rifie que le segment est bien dans la zone */

		if (@vl_PRdebS<@va_PRfin_in) and (@vl_PRfinS>@va_PRdeb_in)
		begin

/*A		calcule les coefficients de translation et extension */

/*B			si le d≈but est apr…s le d≈but du segment : on le prend */

			if @vl_PRdebS<@va_PRdeb_in
			select @vl_PR=@va_PRdeb_in
			else
			select @vl_PR=@vl_PRdebS

/*B			si la fin est avant la fin du segment : on la prend */

			if @vl_PRfinS>@va_PRfin_in
			select @vl_PRfinS=@va_PRfin_in

/*B			Translation */

			select @vl_translation=
				10000
				*((@vl_PR+@vl_PRfinS)/2-@vl_PRdebP)
				/(@vl_PRfinP-@vl_PRdebP)

/*B			Extension */

			select @vl_extension=
				10000
				*(@vl_PRfinS-@vl_PR)
				/(@vl_PRfinP-@vl_PRdebP)

/*A		ins…re les voies neutralis≈es en fonction du sens et du nombre de voies */

			
/*B				on traite en premier la BAU */

				if	@va_TypeVoie_in=XZAHC_BAU
				begin
					select @vl_translationp=translation,
					       @vl_extensionp=extension
					from EXP..SYN_NEU_SEG
					where evenement=@va_NumEvt_in and
					      cle=@va_CleEvt_in and
					      PR=@vl_PRdebS and
					      sens=@vl_sens and
					      ordre=XDC_AJOUT and
					      voie=6-@vl_voie
					
					if @@rowcount=1
					begin
						select @vl_fin=@vl_translation+(@vl_extension/2)
						select @vl_finp=@vl_translationp+(@vl_extensionp/2)
						if  @vl_finp > @vl_fin
						    select @vl_fin=@vl_finp
						select @vl_debut=@vl_translation-(@vl_extension/2)
						select @vl_debutp=@vl_translationp-(@vl_extensionp/2)
						if  @vl_debutp < @vl_debut
						    select @vl_debut=@vl_debutp
						select @vl_translation=@vl_debut+(@vl_fin-@vl_debut)/2
						select @vl_extension=@vl_fin-@vl_debut
						
						delete EXP..SYN_NEU_SEG
						where evenement=@va_NumEvt_in and
					      		cle=@va_CleEvt_in and
					      		PR=@vl_PRdebS and
					      		sens=@vl_sens and
					      		ordre=XDC_AJOUT and
					      		voie=6-@vl_voie
						
					end
				insert	EXP..SYN_NEU_SEG
					(
					evenement,
					cle,
					portion,
					PR,
					sens,
					voie,
					ordre,
					translation,
					extension
					)
					values
					(
					@va_NumEvt_in,
					@va_CleEvt_in,
					@vl_portion,
					@vl_PRdebS,
					@vl_sens,
					6-@vl_voie,
					XDC_AJOUT,
					@vl_translation,
					@vl_extension
					)
				end

				select 	@vl_PR_debut_VM2_I=PR_debut_VM2_I,
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
					@vl_PR_debut_VR_I=PR_debut_VR_I,
					@vl_PR_fin_VR_I=PR_fin_VR_I,
					@vl_debut_VR_I=debut_VR_I,
					@vl_fin_VR_I=fin_VR_I,
					@vl_PR_debut_BAU_I=PR_debut_BAU_I,
					@vl_PR_fin_BAU_I=PR_fin_BAU_I,
					@vl_debut_BAU_I=debut_BAU_I,
					@vl_fin_BAU_I=fin_BAU_I,
					@vl_PR_debut_VM2=PR_debut_VM2,
					@vl_PR_fin_VM2=PR_fin_VM2,
					@vl_debut_VM2=debut_VM2 ,
					@vl_fin_VM2 =fin_VM2 ,
					@vl_PR_debut_VM1  =PR_debut_VM1 ,
					@vl_PR_fin_VM1 =PR_fin_VM1 ,
					@vl_debut_VM1 =debut_VM1 ,
					@vl_fin_VM1 =fin_VM1 ,
					@vl_PR_debut_VL =PR_debut_VL ,
					@vl_PR_fin_VL =PR_fin_VL ,
					@vl_debut_VL =debut_VL ,
					@vl_fin_VL =fin_VL ,
					@vl_PR_debut_VR =PR_debut_VR ,
					@vl_PR_fin_VR =PR_fin_VR ,
					@vl_debut_VR =debut_VR ,
					@vl_fin_VR =fin_VR ,
					@vl_PR_debut_BAU =PR_debut_BAU ,
					@vl_PR_fin_BAU =PR_fin_BAU ,
					@vl_debut_BAU =debut_BAU ,
					@vl_fin_BAU =fin_BAU 
				from EXP..FMC_TRF
				where	numero=@va_NumEvt_in and
					cle=@va_CleEvt_in
							
				select @vl_voie_opp=nombre_de_voies
				from CFG..RES_SEG
				where portion=@vl_portion and
					PR_debut<@va_PRdeb_in and
					sens=@vl_sens_opp
						
				if @vl_voie_opp>=2
				begin
					select @vl_nvoie4=XDC_VOIE_SENS_NORMAL
					select @vl_voie4=XDC_VOIE_SENS_NORMAL
				end
				if @vl_voie_opp>=3
				begin
					select @vl_nvoie3=XDC_VOIE_SENS_NORMAL
					select @vl_voie3=XDC_VOIE_SENS_NORMAL
				end
				if @vl_voie_opp>=4
				begin
					select @vl_nvoie2=XDC_VOIE_SENS_NORMAL
					select @vl_voie2=XDC_VOIE_SENS_NORMAL
				end
							
				if ( ( @vl_PR_debut_VM2_I<=@vl_PR_rab and @vl_PR_fin_VM2_I>=@vl_PR_rab ) or
				     ( @vl_PR_fin_VM2_I<=@vl_PR_rab and @vl_PR_debut_VM2_I>=@vl_PR_rab ) ) and
				     ( @vl_debut_VM2_I != NULL or @vl_debut_VM2_I <= @vl_DateCourante ) and
				     ( @vl_fin_VM2_I = NULL or @vl_fin_VM2_I > @vl_DateCourante )
	   			   select @vl_nvoie4=XDC_VOIE_INEXISTANTE
	   					     
				if ( ( @vl_PR_debut_VM1_I<=@vl_PR_rab and @vl_PR_fin_VM1_I>=@vl_PR_rab ) or
				     ( @vl_PR_fin_VM1_I<=@vl_PR_rab and @vl_PR_debut_VM1_I>=@vl_PR_rab ) ) and
				     ( @vl_debut_VM1_I != NULL or @vl_debut_VM1_I <= @vl_DateCourante ) and
	   			     ( @vl_fin_VM1_I = NULL or @vl_fin_VM1_I > @vl_DateCourante )
	   			begin
				   if @vl_voie_opp>=4
	   			      select @vl_nvoie3=XDC_VOIE_INEXISTANTE
	   			   else
	   			      select @vl_nvoie4=XDC_VOIE_INEXISTANTE
				end
						
				if ( ( @vl_PR_debut_VL_I<=@vl_PR_rab and @vl_PR_fin_VL_I>=@vl_PR_rab ) or
				     ( @vl_PR_fin_VL_I<=@vl_PR_rab and @vl_PR_debut_VL_I>=@vl_PR_rab ) ) and
				     ( @vl_debut_VL_I != NULL or @vl_debut_VL_I <= @vl_DateCourante ) and
	   			     ( @vl_fin_VL_I = NULL or @vl_fin_VL_I > @vl_DateCourante )
				begin
				   if @vl_voie_opp>=4
				   begin
	   			      select @vl_nvoie2=XDC_VOIE_INEXISTANTE
	 			      select @vl_nvoie1=XDC_VOIE_SENS_NORMAL
	   			   end
	   			   else
				   if @vl_voie_opp>=3
				   begin
	   			      select @vl_nvoie3=XDC_VOIE_INEXISTANTE
	  			      select @vl_nvoie2=XDC_VOIE_SENS_NORMAL
	  			   end
	   			   else
	  			   begin
	   			     select @vl_nvoie4=XDC_VOIE_INEXISTANTE
	   			     select @vl_nvoie3=XDC_VOIE_SENS_NORMAL
	   			   end
				end

										
				if @vl_voie=2
				begin
					if ( @vl_PR_debut_VL>@vl_PR_fin_VL ) 
					begin
						select @vl_PR=@vl_PR_debut_VL
						select @vl_PR_debut_VL=@vl_PR_fin_VL
						select @vl_PR_fin_VL=@vl_PR
					end
					if ( @vl_PR_debut_VR>@vl_PR_fin_VR ) 
					begin
						select @vl_PR=@vl_PR_debut_VR
						select @vl_PR_debut_VR=@vl_PR_fin_VR
						select @vl_PR_fin_VR=@vl_PR
					end
	   			     	select @vl_nvoiet2=XDC_VOIE_SENS_NORMAL
	   			     	select @vl_voiet2=XDC_VOIE_SENS_NORMAL
	   			     	select @vl_nvoiet1=XDC_VOIE_SENS_NORMAL
	   			     	select @vl_voiet1=XDC_VOIE_SENS_NORMAL

					if ( (@vl_PR_debut_VL<@vl_PR_rab ) and ( @vl_PR_fin_VL>@vl_PR_rab )  and ( @vl_debut_VL != NULL and @vl_debut_VL <= @vl_DateCourante ) and
	   			     			( @vl_fin_VL = NULL or @vl_fin_VL > @vl_DateCourante ) ) 
	   			     	begin
	   			     	select @vl_voiet2=XDC_VOIE_INEXISTANTE
	   			     	end
	   			     	
					if ( ((( @vl_PR_debut_VL=@vl_PR_rab ) and (@vl_chaussee=XDC_SENS_SUD)) or (( @vl_PR_fin_VL=@vl_PR_rab ) and (@vl_chaussee=XDC_SENS_NORD))) and 
							( @vl_debut_VL != NULL and @vl_debut_VL <= @vl_DateCourante ) and
	   			     			( @vl_fin_VL = NULL or @vl_fin_VL > @vl_DateCourante ) ) or @vl_voiet2=XDC_VOIE_INEXISTANTE
	   			     	begin
	   			     	select @vl_nvoiet2=XDC_VOIE_INEXISTANTE
	   			     	end

					if (( @vl_PR_debut_VR<@vl_PR_rab ) and ( @vl_PR_fin_VR>@vl_PR_rab ) and ( @vl_debut_VR != NULL and @vl_debut_VR <= @vl_DateCourante ) and
	   			     			( @vl_fin_VR = NULL or @vl_fin_VR > @vl_DateCourante ) )
	   			     	begin
	   			     	select @vl_voiet1=XDC_VOIE_INEXISTANTE
	   			     	end
	   			     	
					if (((( @vl_PR_debut_VR=@vl_PR_rab ) and (@vl_chaussee=XDC_SENS_SUD)) or (( @vl_PR_fin_VR=@vl_PR_rab ) and (@vl_chaussee=XDC_SENS_NORD))) and 
							( @vl_debut_VR != NULL and @vl_debut_VR <= @vl_DateCourante ) and
	   			     			( @vl_fin_VR = NULL or @vl_fin_VR > @vl_DateCourante ) ) or @vl_voiet1=XDC_VOIE_INEXISTANTE
	   			     	begin
	   			     	select @vl_nvoiet1=XDC_VOIE_INEXISTANTE
	   			     	end
				end
				if @vl_voie=3
				begin
					if ( @vl_PR_debut_VL>@vl_PR_fin_VL )
					begin
						select @vl_PR=@vl_PR_debut_VL
						select @vl_PR_debut_VL=@vl_PR_fin_VL
						select @vl_PR_fin_VL=@vl_PR
					end
					if ( @vl_PR_debut_VR>@vl_PR_fin_VR )
					begin
						select @vl_PR=@vl_PR_debut_VR
						select @vl_PR_debut_VR=@vl_PR_fin_VR
						select @vl_PR_fin_VR=@vl_PR
					end
					if ( @vl_PR_debut_VM1>@vl_PR_fin_VM1 )
					begin
						select @vl_PR=@vl_PR_debut_VM1
						select @vl_PR_debut_VM1=@vl_PR_fin_VM1
						select @vl_PR_fin_VM1=@vl_PR
					end
	   			     	select @vl_nvoiet1=XDC_VOIE_SENS_NORMAL
	   			     	select @vl_voiet1=XDC_VOIE_SENS_NORMAL
	   			     	select @vl_nvoiet2=XDC_VOIE_SENS_NORMAL
	   			     	select @vl_voiet2=XDC_VOIE_SENS_NORMAL
	   			     	select @vl_nvoiet3=XDC_VOIE_SENS_NORMAL
	   			     	select @vl_voiet3=XDC_VOIE_SENS_NORMAL


					if (( @vl_PR_debut_VL<@vl_PR_rab ) and ( @vl_PR_fin_VL>@vl_PR_rab ) and ( @vl_debut_VL != NULL and @vl_debut_VL <= @vl_DateCourante ) and
	   			     			( @vl_fin_VL = NULL or @vl_fin_VL > @vl_DateCourante ) )
	   			     	begin
	   			     	select @vl_voiet3=XDC_VOIE_INEXISTANTE
	   			     	end
	   			     	
					if (((( @vl_PR_debut_VL=@vl_PR_rab ) and (@vl_chaussee=XDC_SENS_SUD)) or (( @vl_PR_fin_VL=@vl_PR_rab ) and (@vl_chaussee=XDC_SENS_NORD))) and 
							( @vl_debut_VL != NULL and @vl_debut_VL <= @vl_DateCourante ) and
	   			     			( @vl_fin_VL = NULL or @vl_fin_VL > @vl_DateCourante ) ) or @vl_voiet3=XDC_VOIE_INEXISTANTE
	   			     	begin
	   			     	select @vl_nvoiet3=XDC_VOIE_INEXISTANTE
	   			     	end	   			     	

					if (( @vl_PR_debut_VM1<@vl_PR_rab ) and ( @vl_PR_fin_VM1>@vl_PR_rab ) and ( @vl_debut_VM1 != NULL and @vl_debut_VM1 <= @vl_DateCourante ) and
	   			     			( @vl_fin_VM1 = NULL or @vl_fin_VM1 > @vl_DateCourante ) ) 
	   			     	begin
	   			     	select @vl_voiet2=XDC_VOIE_INEXISTANTE
	   			     	end

					if (((( @vl_PR_debut_VM1=@vl_PR_rab ) and (@vl_chaussee=XDC_SENS_SUD)) or (( @vl_PR_fin_VM1=@vl_PR_rab ) and (@vl_chaussee=XDC_SENS_NORD))) and 
							( @vl_debut_VM1 != NULL and @vl_debut_VM1 <= @vl_DateCourante ) and
	   			     			( @vl_fin_VM1 = NULL or @vl_fin_VM1 > @vl_DateCourante ) )or @vl_voiet2=XDC_VOIE_INEXISTANTE
	   			     	begin
	   			     	select @vl_nvoiet2=XDC_VOIE_INEXISTANTE
	   			     	end
	   			     	
					if (( @vl_PR_debut_VR<@vl_PR_rab ) and ( @vl_PR_fin_VR>@vl_PR_rab ) and ( @vl_debut_VR != NULL and @vl_debut_VR <= @vl_DateCourante ) and
	   			     			( @vl_fin_VR = NULL or @vl_fin_VR > @vl_DateCourante ) )
	   			     	begin
	   			     	select @vl_voiet1=XDC_VOIE_INEXISTANTE
	   			     	end
	   			     	
					if (((( @vl_PR_debut_VR=@vl_PR_rab ) and (@vl_chaussee=XDC_SENS_SUD)) or (( @vl_PR_fin_VR=@vl_PR_rab ) and (@vl_chaussee=XDC_SENS_NORD))) and 
							( @vl_debut_VR != NULL and @vl_debut_VR <= @vl_DateCourante ) and
	   			     			( @vl_fin_VR = NULL or @vl_fin_VR > @vl_DateCourante ) ) or @vl_voiet1=XDC_VOIE_INEXISTANTE
	   			     	begin
	   			     	select @vl_nvoiet1=XDC_VOIE_INEXISTANTE
	   			     	end
				end
				if @vl_voie=4
				begin
					if ( @vl_PR_debut_VL>@vl_PR_fin_VL )
					begin
						select @vl_PR=@vl_PR_debut_VL
						select @vl_PR_debut_VL=@vl_PR_fin_VL
						select @vl_PR_fin_VL=@vl_PR
					end
					if ( @vl_PR_debut_VR>@vl_PR_fin_VR )
					begin
						select @vl_PR=@vl_PR_debut_VR
						select @vl_PR_debut_VR=@vl_PR_fin_VR
						select @vl_PR_fin_VR=@vl_PR
					end
					if ( @vl_PR_debut_VM1>@vl_PR_fin_VM1 )
					begin
						select @vl_PR=@vl_PR_debut_VM1
						select @vl_PR_debut_VM1=@vl_PR_fin_VM1
						select @vl_PR_fin_VM1=@vl_PR
					end
					if ( @vl_PR_debut_VM2>@vl_PR_fin_VM2 )
					begin
						select @vl_PR=@vl_PR_debut_VM2
						select @vl_PR_debut_VM2=@vl_PR_fin_VM2
						select @vl_PR_fin_VM2=@vl_PR
					end
	   			     	select @vl_nvoiet1=XDC_VOIE_SENS_NORMAL
	   			     	select @vl_voiet1=XDC_VOIE_SENS_NORMAL
	   			     	select @vl_nvoiet2=XDC_VOIE_SENS_NORMAL
	   			     	select @vl_voiet2=XDC_VOIE_SENS_NORMAL
	   			     	select @vl_nvoiet3=XDC_VOIE_SENS_NORMAL
	   			     	select @vl_voiet3=XDC_VOIE_SENS_NORMAL
	   			     	select @vl_nvoiet4=XDC_VOIE_SENS_NORMAL
	   			     	select @vl_voiet4=XDC_VOIE_SENS_NORMAL

					if (( @vl_PR_debut_VL<@vl_PR_rab ) and ( @vl_PR_fin_VL>@vl_PR_rab ) and ( @vl_debut_VL != NULL and @vl_debut_VL <= @vl_DateCourante ) and
	   			     			( @vl_fin_VL = NULL or @vl_fin_VL > @vl_DateCourante ) )
	   			     	begin
	   			     	select @vl_voiet4=XDC_VOIE_INEXISTANTE
	   			     	end
	   			     	
					if (((( @vl_PR_debut_VL=@vl_PR_rab ) and (@vl_chaussee=XDC_SENS_SUD)) or (( @vl_PR_fin_VL=@vl_PR_rab ) and (@vl_chaussee=XDC_SENS_NORD))) and 
							( @vl_debut_VL != NULL and @vl_debut_VL <= @vl_DateCourante ) and
	   			     			( @vl_fin_VL = NULL or @vl_fin_VL > @vl_DateCourante ) ) or @vl_voiet4=XDC_VOIE_INEXISTANTE
	   			     	begin
	   			     	select @vl_nvoiet4=XDC_VOIE_INEXISTANTE
	   			     	end

					if (( @vl_PR_debut_VM1<@vl_PR_rab ) and ( @vl_PR_fin_VM1>@vl_PR_rab ) and ( @vl_debut_VM1 != NULL and @vl_debut_VM1 <= @vl_DateCourante ) and
	   			     			( @vl_fin_VM1 = NULL or @vl_fin_VM1 > @vl_DateCourante ) )
	   			     	begin
	   			     	select @vl_voiet3=XDC_VOIE_INEXISTANTE
	   			     	end

					if (((( @vl_PR_debut_VM1=@vl_PR_rab ) and (@vl_chaussee=XDC_SENS_SUD)) or (( @vl_PR_fin_VM1=@vl_PR_rab ) and (@vl_chaussee=XDC_SENS_NORD))) and 
							( @vl_debut_VM1 != NULL and @vl_debut_VM1 <= @vl_DateCourante ) and
	   			     			( @vl_fin_VM1 = NULL or @vl_fin_VM1 > @vl_DateCourante ) ) or @vl_voiet3=XDC_VOIE_INEXISTANTE
	   			     	begin
	   			     	select @vl_nvoiet3=XDC_VOIE_INEXISTANTE
	   			     	end
	   			     	
					if (( @vl_PR_debut_VM2<@vl_PR_rab ) and ( @vl_PR_fin_VM2>@vl_PR_rab ) and ( @vl_debut_VM2 != NULL or @vl_debut_VM2 <= @vl_DateCourante ) and
	   			     			( @vl_fin_VM2 = NULL or @vl_fin_VM2 > @vl_DateCourante ) )
	   			     	begin
	   			     	select @vl_voiet2=XDC_VOIE_INEXISTANTE
	   			     	end
	   			     	
					if (((( @vl_PR_debut_VM2=@vl_PR_rab ) and (@vl_chaussee=XDC_SENS_SUD)) or (( @vl_PR_fin_VM2=@vl_PR_rab ) and (@vl_chaussee=XDC_SENS_NORD))) and 
							( @vl_debut_VM2 != NULL or @vl_debut_VM2 <= @vl_DateCourante ) and
	   			     			( @vl_fin_VM2 = NULL or @vl_fin_VM2 > @vl_DateCourante ) ) or @vl_voiet2=XDC_VOIE_INEXISTANTE
	   			     	begin
	   			     	select @vl_nvoiet2=XDC_VOIE_INEXISTANTE
	   			     	end

					if (( @vl_PR_debut_VR<@vl_PR_rab ) and ( @vl_PR_fin_VR>@vl_PR_rab ) and ( @vl_debut_VR != NULL or @vl_debut_VR <= @vl_DateCourante ) and
	   			     			( @vl_fin_VR = NULL or @vl_fin_VR > @vl_DateCourante ) )
	   			     	begin
	   			     	select @vl_voiet1=XDC_VOIE_INEXISTANTE
	   			     	end

					if (((( @vl_PR_debut_VR=@vl_PR_rab ) and (@vl_chaussee=XDC_SENS_SUD)) or (( @vl_PR_fin_VR=@vl_PR_rab ) and (@vl_chaussee=XDC_SENS_NORD))) and 
							( @vl_debut_VR != NULL and @vl_debut_VR <= @vl_DateCourante ) and
	   			     			( @vl_fin_VR = NULL or @vl_fin_VR > @vl_DateCourante ) ) or @vl_voiet1=XDC_VOIE_INEXISTANTE
	   			     	begin
	   			     	select @vl_nvoiet1=XDC_VOIE_INEXISTANTE
	   			     	end
	   			     	
				end
						
/*B				cas 1 voie */

				if @vl_voie=1
				begin
					if	@va_TypeVoie_in=XZAHC_VL
					begin
						
						if @va_Oppose=XDC_VRAI
						   exec PRC..XZAH;33	@va_NumEvt_in,
								@va_CleEvt_in,
								@va_Autoroute_in,
								@vl_PR_rab,
								@va_Sens_in,
								@va_numordre,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE
      						else
      						begin
						   exec PRC..XZAH;33	@va_NumEvt_in,
								@va_CleEvt_in,
								@va_Autoroute_in,
								@vl_PR_rab,
								@va_Sens_in,
								@va_numordre,
      								@vl_voie1,
      								@vl_voie2,
      								@vl_voie3,
      								@vl_voie4,
      								@vl_voie5,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								@vl_nvoie1,
      								@vl_nvoie2,
      								@vl_nvoie3,
      								@vl_nvoie4,
      								XDC_VOIE_SENS_INVERSE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE
						   exec PRC..XZAH;33	@va_NumEvt_in,
								@va_CleEvt_in,
								@va_Autoroute_in,
								@vl_PR_rab_fin,
								@va_Sens_in,
								@vl_numordre_fin,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_SENS_INVERSE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE
      						end
      								
						
						select @vl_translationp=translation,
					       		@vl_extensionp=extension
						from EXP..SYN_NEU_SEG
						where evenement=@va_NumEvt_in and
					      		cle=@va_CleEvt_in and
					      		PR=@vl_PRdebS and
					      		sens=@vl_sens and
					      		ordre=XDC_AJOUT and
					      		voie=6
					
						if @@rowcount=1
						begin
							select @vl_fin=@vl_translation+(@vl_extension/2)
							select @vl_finp=@vl_translationp+(@vl_extensionp/2)
							if  @vl_finp > @vl_fin
							    select @vl_fin=@vl_finp
							select @vl_debut=@vl_translation-(@vl_extension/2)
							select @vl_debutp=@vl_translationp-(@vl_extensionp/2)
							if  @vl_debutp < @vl_debut
							    select @vl_debut=@vl_debutp
							select @vl_translation=@vl_debut+(@vl_fin-@vl_debut)/2
							select @vl_extension=@vl_fin-@vl_debut
						
						delete EXP..SYN_NEU_SEG
						where evenement=@va_NumEvt_in and
					      		cle=@va_CleEvt_in and
					      		PR=@vl_PRdebS and
					      		sens=@vl_sens and
					      		ordre=XDC_AJOUT and
					      		voie=6
						end
						insert	EXP..SYN_NEU_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						ordre,
						translation,
						extension
						)
						values
						(
						@va_NumEvt_in,
						@va_CleEvt_in,
						@vl_portion,
						@vl_PRdebS,
						@vl_sens,
						6,
						XDC_AJOUT,
						@vl_translation,
						@vl_extension
						)
					end
				end
				else

/*B				cas 2 voie */

				if @vl_voie=2
				begin
					if	@va_TypeVoie_in=XZAHC_VR
					begin
						if @va_Oppose=XDC_VRAI
						   exec PRC..XZAH;33	@va_NumEvt_in,
								@va_CleEvt_in,
								@va_Autoroute_in,
								@vl_PR_rab,
								@va_Sens_in,
								@va_numordre,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE
      						else
      						begin
      						  if ((@vl_PR_debut_VL <= @vl_PR_rab and @vl_PR_fin_VL>=@vl_PR_rab ) and 
      						        ( @vl_debut_VL != NULL or @vl_debut_VL <= @vl_DateCourante ) and
	   			     		        ( @vl_fin_VL = NULL or @vl_fin_VL > @vl_DateCourante ) ) 
						  begin
						   exec PRC..XZAH;33	@va_NumEvt_in,
								@va_CleEvt_in,
								@va_Autoroute_in,
								@vl_PR_rab,
								@va_Sens_in,
								@va_numordre,
      								@vl_voie1,
      								@vl_voie2,
      								@vl_voie3,
      								@vl_voie4,
      								@vl_voie5,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_INEXISTANTE,
       								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								@vl_nvoie1,
      								@vl_nvoie2,
      								@vl_nvoie3,
      								@vl_nvoie4,
      								XDC_VOIE_SENS_INVERSE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE
						   exec PRC..XZAH;33	@va_NumEvt_in,
								@va_CleEvt_in,
								@va_Autoroute_in,
								@vl_PR_rab_fin,
								@va_Sens_in,
								@vl_numordre_fin,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_SENS_INVERSE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE
						  end
						  else
						  begin
						   exec PRC..XZAH;33	@va_NumEvt_in,
								@va_CleEvt_in,
								@va_Autoroute_in,
								@vl_PR_rab,
								@va_Sens_in,
								@va_numordre,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								@vl_voiet1,
      								@vl_voiet2,
      								@vl_voiet3,
      								@vl_voiet4,
       								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								@vl_nvoiet1,
      								@vl_nvoiet2,
      								@vl_nvoiet3,
      								@vl_nvoiet4,
       								XDC_VOIE_INEXISTANTE
						  end
      						end
      								
						
						select @vl_translationp=translation,
					       		@vl_extensionp=extension
						from EXP..SYN_NEU_SEG
						where evenement=@va_NumEvt_in and
					      		cle=@va_CleEvt_in and
					      		PR=@vl_PRdebS and
					      		sens=@vl_sens and
					      		ordre=XDC_AJOUT and
					      		voie=6
					
						if @@rowcount=1
						begin
							select @vl_fin=@vl_translation+(@vl_extension/2)
							select @vl_finp=@vl_translationp+(@vl_extensionp/2)
							if  @vl_finp > @vl_fin
							    select @vl_fin=@vl_finp
							select @vl_debut=@vl_translation-(@vl_extension/2)
							select @vl_debutp=@vl_translationp-(@vl_extensionp/2)
							if  @vl_debutp < @vl_debut
							    select @vl_debut=@vl_debutp
							select @vl_translation=@vl_debut+(@vl_fin-@vl_debut)/2
							select @vl_extension=@vl_fin-@vl_debut
							delete EXP..SYN_NEU_SEG
							where evenement=@va_NumEvt_in and
					      			cle=@va_CleEvt_in and
					      			PR=@vl_PRdebS and
					      			sens=@vl_sens and
					      			ordre=XDC_AJOUT and
					      			voie=6
						end
						
						insert	EXP..SYN_NEU_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						ordre,
						translation,
						extension
						)
						values
						(
						@va_NumEvt_in,
						@va_CleEvt_in,
						@vl_portion,
						@vl_PRdebS,
						@vl_sens,
						6,
						XDC_AJOUT,
						@vl_translation,
						@vl_extension
						)
					end
					if	@va_TypeVoie_in=XZAHC_VL
					begin
						if @va_Oppose=XDC_VRAI
						   exec PRC..XZAH;33	@va_NumEvt_in,
								@va_CleEvt_in,
								@va_Autoroute_in,
								@vl_PR_rab,
								@va_Sens_in,
								@va_numordre,
       								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE
      						else
						   exec PRC..XZAH;33	@va_NumEvt_in,
								@va_CleEvt_in,
								@va_Autoroute_in,
								@vl_PR_rab,
								@va_Sens_in,
								@va_numordre,
       								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								@vl_voiet1,
      								@vl_voiet2,
      								@vl_voiet3,
      								@vl_voiet4,
       								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								@vl_nvoiet1,
      								@vl_nvoiet2,
      								@vl_nvoiet3,
      								@vl_nvoiet4,
      								XDC_VOIE_INEXISTANTE
      								
						
						select @vl_translationp=translation,
					       		@vl_extensionp=extension
						from EXP..SYN_NEU_SEG
						where evenement=@va_NumEvt_in and
					      		cle=@va_CleEvt_in and
					      		PR=@vl_PRdebS and
					      		sens=@vl_sens and
					      		ordre=XDC_AJOUT and
					      		voie=5
					
						if @@rowcount=1
						begin
							select @vl_fin=@vl_translation+(@vl_extension/2)
							select @vl_finp=@vl_translationp+(@vl_extensionp/2)
							if  @vl_finp > @vl_fin
							    select @vl_fin=@vl_finp
							select @vl_debut=@vl_translation-(@vl_extension/2)
							select @vl_debutp=@vl_translationp-(@vl_extensionp/2)
							if  @vl_debutp < @vl_debut
							    select @vl_debut=@vl_debutp
							select @vl_translation=@vl_debut+(@vl_fin-@vl_debut)/2
							select @vl_extension=@vl_fin-@vl_debut
							delete EXP..SYN_NEU_SEG
							where evenement=@va_NumEvt_in and
					      			cle=@va_CleEvt_in and
					      			PR=@vl_PRdebS and
					      			sens=@vl_sens and
					      			ordre=XDC_AJOUT and
					      			voie=5
						end
						
						insert	EXP..SYN_NEU_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						ordre,
						translation,
						extension
						)
						values
						(
						@va_NumEvt_in,
						@va_CleEvt_in,
						@vl_portion,
						@vl_PRdebS,
						@vl_sens,
						5,
						XDC_AJOUT,
						@vl_translation,
						@vl_extension
						)
					end
				end
				else

/*B				cas 3 voie */

				if @vl_voie=3
				begin
					if	@va_TypeVoie_in=XZAHC_VR
					begin
						if @va_Oppose=XDC_VRAI
						   exec PRC..XZAH;33	@va_NumEvt_in,
								@va_CleEvt_in,
								@va_Autoroute_in,
								@vl_PR_rab,
								@va_Sens_in,
								@va_numordre,
       								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE
      						else
      						begin
      						  if ( ((@vl_PR_debut_VM1 <= @vl_PR_rab and @vl_PR_fin_VM1>=@vl_PR_rab ) and 
      						        ( @vl_debut_VM1 != NULL or @vl_debut_VM1 <= @vl_DateCourante ) and
	   			     		        ( @vl_fin_VM1 = NULL or @vl_fin_VM1 > @vl_DateCourante ) ) and
	   			     		       ((@vl_PR_debut_VL <= @vl_PR_rab and @vl_PR_fin_VL>=@vl_PR_rab ) and 
      						        ( @vl_debut_VL != NULL or @vl_debut_VL <= @vl_DateCourante ) and
	   			     		        ( @vl_fin_VL = NULL or @vl_fin_VL > @vl_DateCourante ) ) )
 						  begin
						   exec PRC..XZAH;33	@va_NumEvt_in,
								@va_CleEvt_in,
								@va_Autoroute_in,
								@vl_PR_rab,
								@va_Sens_in,
								@va_numordre,
      								@vl_voie1,
      								@vl_voie2,
      								@vl_voie3,
      								@vl_voie4,
      								@vl_voie5,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								@vl_nvoie1,
      								@vl_nvoie2,
      								@vl_nvoie3,
      								@vl_nvoie4,
      								XDC_VOIE_SENS_INVERSE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE
						   exec PRC..XZAH;33	@va_NumEvt_in,
								@va_CleEvt_in,
								@va_Autoroute_in,
								@vl_PR_rab_fin,
								@va_Sens_in,
								@vl_numordre_fin,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_SENS_INVERSE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE
						  end
						  else
						  begin
						   exec PRC..XZAH;33	@va_NumEvt_in,
								@va_CleEvt_in,
								@va_Autoroute_in,
								@vl_PR_rab,
								@va_Sens_in,
								@va_numordre,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								@vl_voiet1,
      								@vl_voiet2,
      								@vl_voiet3,
      								@vl_voiet4,
       								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								@vl_nvoiet1,
      								@vl_nvoiet2,
      								@vl_nvoiet3,
      								@vl_nvoiet4,
       								XDC_VOIE_INEXISTANTE
						  end
      						end
      								
						
						select @vl_translationp=translation,
					       		@vl_extensionp=extension
						from EXP..SYN_NEU_SEG
						where evenement=@va_NumEvt_in and
					      		cle=@va_CleEvt_in and
					      		PR=@vl_PRdebS and
					      		sens=@vl_sens and
					      		ordre=XDC_AJOUT and
					      		voie=6
					
						if @@rowcount=1
						begin
							select @vl_fin=@vl_translation+(@vl_extension/2)
							select @vl_finp=@vl_translationp+(@vl_extensionp/2)
							if  @vl_finp > @vl_fin
							    select @vl_fin=@vl_finp
							select @vl_debut=@vl_translation-(@vl_extension/2)
							select @vl_debutp=@vl_translationp-(@vl_extensionp/2)
							if  @vl_debutp < @vl_debut
							    select @vl_debut=@vl_debutp
							select @vl_translation=@vl_debut+(@vl_fin-@vl_debut)/2
							select @vl_extension=@vl_fin-@vl_debut
							delete EXP..SYN_NEU_SEG
							where evenement=@va_NumEvt_in and
					      			cle=@va_CleEvt_in and
					      			PR=@vl_PRdebS and
					      			sens=@vl_sens and
					      			ordre=XDC_AJOUT and
					      			voie=6
						end
						
						insert	EXP..SYN_NEU_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						ordre,
						translation,
						extension
						)
						values
						(
						@va_NumEvt_in,
						@va_CleEvt_in,
						@vl_portion,
						@vl_PRdebS,
						@vl_sens,
						6,
						XDC_AJOUT,
						@vl_translation,
						@vl_extension
						)
					end
					
					if	@va_TypeVoie_in=XZAHC_VM1
					begin
						if @va_Oppose=XDC_VRAI
						   exec PRC..XZAH;33	@va_NumEvt_in,
								@va_CleEvt_in,
								@va_Autoroute_in,
								@vl_PR_rab,
								@va_Sens_in,
								@va_numordre,
       								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE
      						else
						   exec PRC..XZAH;33	@va_NumEvt_in,
								@va_CleEvt_in,
								@va_Autoroute_in,
								@vl_PR_rab,
								@va_Sens_in,
								@va_numordre,
       								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								@vl_voiet1,
      								@vl_voiet2,
      								@vl_voiet3,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								@vl_nvoiet1,
      								@vl_nvoiet2,
      								@vl_nvoiet3,
      								@vl_nvoiet4,
       								XDC_VOIE_INEXISTANTE
      								
						
						select @vl_translationp=translation,
					       		@vl_extensionp=extension
						from EXP..SYN_NEU_SEG
						where evenement=@va_NumEvt_in and
					      		cle=@va_CleEvt_in and
					      		PR=@vl_PRdebS and
					      		sens=@vl_sens and
					      		ordre=XDC_AJOUT and
					      		voie=5
					
						if @@rowcount=1
						begin
							select @vl_fin=@vl_translation+(@vl_extension/2)
							select @vl_finp=@vl_translationp+(@vl_extensionp/2)
							if  @vl_finp > @vl_fin
							    select @vl_fin=@vl_finp
							select @vl_debut=@vl_translation-(@vl_extension/2)
							select @vl_debutp=@vl_translationp-(@vl_extensionp/2)
							if  @vl_debutp < @vl_debut
							    select @vl_debut=@vl_debutp
							select @vl_translation=@vl_debut+(@vl_fin-@vl_debut)/2
							select @vl_extension=@vl_fin-@vl_debut
							delete EXP..SYN_NEU_SEG
							where evenement=@va_NumEvt_in and
					      			cle=@va_CleEvt_in and
					      			PR=@vl_PRdebS and
					      			sens=@vl_sens and
					      			ordre=XDC_AJOUT and
					      			voie=5
						end
						
						insert	EXP..SYN_NEU_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						ordre,
						translation,
						extension
						)
						values
						(
						@va_NumEvt_in,
						@va_CleEvt_in,
						@vl_portion,
						@vl_PRdebS,
						@vl_sens,
						5,
						XDC_AJOUT,
						@vl_translation,
						@vl_extension
						)
					end
					if	@va_TypeVoie_in=XZAHC_VL
					begin
						if @va_Oppose=XDC_VRAI
						   exec PRC..XZAH;33	@va_NumEvt_in,
								@va_CleEvt_in,
								@va_Autoroute_in,
								@vl_PR_rab,
								@va_Sens_in,
								@va_numordre,
       								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_INEXISTANTE
      						else
						   exec PRC..XZAH;33	@va_NumEvt_in,
								@va_CleEvt_in,
								@va_Autoroute_in,
								@vl_PR_rab,
								@va_Sens_in,
								@va_numordre,
       								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								@vl_voiet1,
      								@vl_voiet2,
      								@vl_voiet3,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								@vl_nvoiet1,
      								@vl_nvoiet2,
      								@vl_nvoiet3,
      								@vl_nvoiet4,
      								XDC_VOIE_INEXISTANTE
      								
						
						select @vl_translationp=translation,
					       		@vl_extensionp=extension
						from EXP..SYN_NEU_SEG
						where evenement=@va_NumEvt_in and
					      		cle=@va_CleEvt_in and
					      		PR=@vl_PRdebS and
					      		sens=@vl_sens and
					      		ordre=XDC_AJOUT and
					      		voie=4
					
						if @@rowcount=1
						begin
							select @vl_fin=@vl_translation+(@vl_extension/2)
							select @vl_finp=@vl_translationp+(@vl_extensionp/2)
							if  @vl_finp > @vl_fin
							    select @vl_fin=@vl_finp
							select @vl_debut=@vl_translation-(@vl_extension/2)
							select @vl_debutp=@vl_translationp-(@vl_extensionp/2)
							if  @vl_debutp < @vl_debut
							    select @vl_debut=@vl_debutp
							select @vl_translation=@vl_debut+(@vl_fin-@vl_debut)/2
							select @vl_extension=@vl_fin-@vl_debut
							delete EXP..SYN_NEU_SEG
							where evenement=@va_NumEvt_in and
					      			cle=@va_CleEvt_in and
					      			PR=@vl_PRdebS and
					      			sens=@vl_sens and
					      			ordre=XDC_AJOUT and
					      			voie=4
						end
						
						insert	EXP..SYN_NEU_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						ordre,
						translation,
						extension
						)
						values
						(
						@va_NumEvt_in,
						@va_CleEvt_in,
						@vl_portion,
						@vl_PRdebS,
						@vl_sens,
						4,
						XDC_AJOUT,
						@vl_translation,
						@vl_extension
						)
					end
				end
				else

/*B				cas 4 voie */

				if @vl_voie=4
				begin
					if	@va_TypeVoie_in=XZAHC_VR
					begin
						if @va_Oppose=XDC_VRAI
						   exec PRC..XZAH;33	@va_NumEvt_in,
								@va_CleEvt_in,
								@va_Autoroute_in,
								@vl_PR_rab,
								@va_Sens_in,
								@va_numordre,
       								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_INEXISTANTE
      						else
      						begin
      						  if ( ((@vl_PR_debut_VM1 <= @vl_PR_rab and @vl_PR_fin_VM1>=@vl_PR_rab ) and 
      						        ( @vl_debut_VM1 != NULL or @vl_debut_VM1 <= @vl_DateCourante ) and
	   			     		        ( @vl_fin_VM1 = NULL or @vl_fin_VM1 > @vl_DateCourante ) ) and
	   			     		       ((@vl_PR_debut_VM2 <= @vl_PR_rab and @vl_PR_fin_VM2>=@vl_PR_rab ) and 
      						        ( @vl_debut_VM2 != NULL or @vl_debut_VM2 <= @vl_DateCourante ) and
	   			     		        ( @vl_fin_VM2 = NULL or @vl_fin_VM2 > @vl_DateCourante ) ) and
	   			     		       ((@vl_PR_debut_VL <= @vl_PR_rab and @vl_PR_fin_VL>=@vl_PR_rab ) and 
      						        ( @vl_debut_VL != NULL or @vl_debut_VL <= @vl_DateCourante ) and
	   			     		        ( @vl_fin_VL = NULL or @vl_fin_VL > @vl_DateCourante ) ) )
						  begin
						   exec PRC..XZAH;33	@va_NumEvt_in,
								@va_CleEvt_in,
								@va_Autoroute_in,
								@vl_PR_rab,
								@va_Sens_in,
								@va_numordre,
      								@vl_voie1,
      								@vl_voie2,
      								@vl_voie3,
      								@vl_voie4,
      								@vl_voie5,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								@vl_nvoie1,
      								@vl_nvoie2,
      								@vl_nvoie3,
      								@vl_nvoie4,
      								XDC_VOIE_SENS_INVERSE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE
						   exec PRC..XZAH;33	@va_NumEvt_in,
								@va_CleEvt_in,
								@va_Autoroute_in,
								@vl_PR_rab_fin,
								@va_Sens_in,
								@vl_numordre_fin,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_SENS_INVERSE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_INEXISTANTE
						  end
						  else
						  begin
						   exec PRC..XZAH;33	@va_NumEvt_in,
								@va_CleEvt_in,
								@va_Autoroute_in,
								@vl_PR_rab,
								@va_Sens_in,
								@va_numordre,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								@vl_voiet1,
      								@vl_voiet2,
      								@vl_voiet3,
      								@vl_voiet4,
       								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								@vl_nvoiet1,
      								@vl_nvoiet2,
      								@vl_nvoiet3,
      								@vl_nvoiet4,
       								XDC_VOIE_INEXISTANTE

						  end
      						end
      								
						
						select @vl_translationp=translation,
					       		@vl_extensionp=extension
						from EXP..SYN_NEU_SEG
						where evenement=@va_NumEvt_in and
					      		cle=@va_CleEvt_in and
					      		PR=@vl_PRdebS and
					      		sens=@vl_sens and
					      		ordre=XDC_AJOUT and
					      		voie=6
					
						if @@rowcount=1
						begin
							select @vl_fin=@vl_translation+(@vl_extension/2)
							select @vl_finp=@vl_translationp+(@vl_extensionp/2)
							if  @vl_finp > @vl_fin
							    select @vl_fin=@vl_finp
							select @vl_debut=@vl_translation-(@vl_extension/2)
							select @vl_debutp=@vl_translationp-(@vl_extensionp/2)
							if  @vl_debutp < @vl_debut
							    select @vl_debut=@vl_debutp
							select @vl_translation=@vl_debut+(@vl_fin-@vl_debut)/2
							select @vl_extension=@vl_fin-@vl_debut
							delete EXP..SYN_NEU_SEG
							where evenement=@va_NumEvt_in and
					      			cle=@va_CleEvt_in and
					      			PR=@vl_PRdebS and
					      			sens=@vl_sens and
					      			ordre=XDC_AJOUT and
					      			voie=6
						end
						
						insert	EXP..SYN_NEU_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						ordre,
						translation,
						extension
						)
						values
						(
						@va_NumEvt_in,
						@va_CleEvt_in,
						@vl_portion,
						@vl_PRdebS,
						@vl_sens,
						6,
						XDC_AJOUT,
						@vl_translation,
						@vl_extension
						)
					end
					
					if	@va_TypeVoie_in=XZAHC_VM2
					begin
						if @va_Oppose=XDC_VRAI
						   exec PRC..XZAH;33	@va_NumEvt_in,
								@va_CleEvt_in,
								@va_Autoroute_in,
								@vl_PR_rab,
								@va_Sens_in,
								@va_numordre,
       								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_INEXISTANTE
      						else
						   exec PRC..XZAH;33	@va_NumEvt_in,
								@va_CleEvt_in,
								@va_Autoroute_in,
								@vl_PR_rab,
								@va_Sens_in,
								@va_numordre,
       								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								@vl_voiet1,
      								@vl_voiet2,
      								@vl_voiet3,
      								@vl_voiet4,
       								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								@vl_nvoiet1,
      								@vl_nvoiet2,
      								@vl_nvoiet3,
      								@vl_nvoiet4,
       								XDC_VOIE_INEXISTANTE
      								
						
						select @vl_translationp=translation,
					       		@vl_extensionp=extension
						from EXP..SYN_NEU_SEG
						where evenement=@va_NumEvt_in and
					      		cle=@va_CleEvt_in and
					      		PR=@vl_PRdebS and
					      		sens=@vl_sens and
					      		ordre=XDC_AJOUT and
					      		voie=5
					
						if @@rowcount=1
						begin
							select @vl_fin=@vl_translation+(@vl_extension/2)
							select @vl_finp=@vl_translationp+(@vl_extensionp/2)
							if  @vl_finp > @vl_fin
							    select @vl_fin=@vl_finp
							select @vl_debut=@vl_translation-(@vl_extension/2)
							select @vl_debutp=@vl_translationp-(@vl_extensionp/2)
							if  @vl_debutp < @vl_debut
							    select @vl_debut=@vl_debutp
							select @vl_translation=@vl_debut+(@vl_fin-@vl_debut)/2
							select @vl_extension=@vl_fin-@vl_debut
							delete EXP..SYN_NEU_SEG
							where evenement=@va_NumEvt_in and
					      			cle=@va_CleEvt_in and
					      			PR=@vl_PRdebS and
					      			sens=@vl_sens and
					      			ordre=XDC_AJOUT and
					      			voie=5
						end
						
						insert	EXP..SYN_NEU_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						ordre,
						translation,
						extension
						)
						values
						(
						@va_NumEvt_in,
						@va_CleEvt_in,
						@vl_portion,
						@vl_PRdebS,
						@vl_sens,
						5,
						XDC_AJOUT,
						@vl_translation,
						@vl_extension
						)
					end
					
					if	@va_TypeVoie_in=XZAHC_VM1
					begin
						if @va_Oppose=XDC_VRAI
						   exec PRC..XZAH;33	@va_NumEvt_in,
								@va_CleEvt_in,
								@va_Autoroute_in,
								@vl_PR_rab,
								@va_Sens_in,
								@va_numordre,
       								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_INEXISTANTE
      						else
						   exec PRC..XZAH;33	@va_NumEvt_in,
								@va_CleEvt_in,
								@va_Autoroute_in,
								@vl_PR_rab,
								@va_Sens_in,
								@va_numordre,
       								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								@vl_voiet1,
      								@vl_voiet2,
      								@vl_voiet3,
      								@vl_voiet4,
       								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								@vl_nvoiet1,
      								@vl_nvoiet2,
      								@vl_nvoiet3,
      								@vl_nvoiet4,
       								XDC_VOIE_INEXISTANTE
      								
						
						select @vl_translationp=translation,
					       		@vl_extensionp=extension
						from EXP..SYN_NEU_SEG
						where evenement=@va_NumEvt_in and
					      		cle=@va_CleEvt_in and
					      		PR=@vl_PRdebS and
					      		sens=@vl_sens and
					      		ordre=XDC_AJOUT and
					      		voie=4
					
						if @@rowcount=1
						begin
							select @vl_fin=@vl_translation+(@vl_extension/2)
							select @vl_finp=@vl_translationp+(@vl_extensionp/2)
							if  @vl_finp > @vl_fin
							    select @vl_fin=@vl_finp
							select @vl_debut=@vl_translation-(@vl_extension/2)
							select @vl_debutp=@vl_translationp-(@vl_extensionp/2)
							if  @vl_debutp < @vl_debut
							    select @vl_debut=@vl_debutp
							select @vl_translation=@vl_debut+(@vl_fin-@vl_debut)/2
							select @vl_extension=@vl_fin-@vl_debut
							delete EXP..SYN_NEU_SEG
							where evenement=@va_NumEvt_in and
					      			cle=@va_CleEvt_in and
					      			PR=@vl_PRdebS and
					      			sens=@vl_sens and
					      			ordre=XDC_AJOUT and
					      			voie=4
						end
						
						insert	EXP..SYN_NEU_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						ordre,
						translation,
						extension
						)
						values
						(
						@va_NumEvt_in,
						@va_CleEvt_in,
						@vl_portion,
						@vl_PRdebS,
						@vl_sens,
						4,
						XDC_AJOUT,
						@vl_translation,
						@vl_extension
						)
					end
					if	@va_TypeVoie_in=XZAHC_VL
					begin
						if @va_Oppose=XDC_VRAI
						   exec PRC..XZAH;33	@va_NumEvt_in,
								@va_CleEvt_in,
								@va_Autoroute_in,
								@vl_PR_rab,
								@va_Sens_in,
								@va_numordre,
       								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_SENS_NORMAL,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_SENS_NORMAL
      						else
						   exec PRC..XZAH;33	@va_NumEvt_in,
								@va_CleEvt_in,
								@va_Autoroute_in,
								@vl_PR_rab,
								@va_Sens_in,
								@va_numordre,
       								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								@vl_voiet1,
      								@vl_voiet2,
      								@vl_voiet3,
      								@vl_voiet4,
       								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								XDC_VOIE_INEXISTANTE,
      								@vl_nvoiet1,
      								@vl_nvoiet2,
      								@vl_nvoiet3,
      								@vl_nvoiet4,
       								XDC_VOIE_INEXISTANTE
      								
						
						select @vl_translationp=translation,
					       		@vl_extensionp=extension
						from EXP..SYN_NEU_SEG
						where evenement=@va_NumEvt_in and
					      		cle=@va_CleEvt_in and
					      		PR=@vl_PRdebS and
					      		sens=@vl_sens and
					      		ordre=XDC_AJOUT and
					      		voie=3
					
						if @@rowcount=1
						begin
							select @vl_fin=@vl_translation+(@vl_extension/2)
							select @vl_finp=@vl_translationp+(@vl_extensionp/2)
							if  @vl_finp > @vl_fin
							    select @vl_fin=@vl_finp
							select @vl_debut=@vl_translation-(@vl_extension/2)
							select @vl_debutp=@vl_translationp-(@vl_extensionp/2)
							if  @vl_debutp < @vl_debut
							    select @vl_debut=@vl_debutp
							select @vl_translation=@vl_debut+(@vl_fin-@vl_debut)/2
							select @vl_extension=@vl_fin-@vl_debut
							delete EXP..SYN_NEU_SEG
							where evenement=@va_NumEvt_in and
					      			cle=@va_CleEvt_in and
					      			PR=@vl_PRdebS and
					      			sens=@vl_sens and
					      			ordre=XDC_AJOUT and
					      			voie=3
						end

					insert	EXP..SYN_NEU_SEG
						(
						evenement,
						cle,
						portion,
						PR,
						sens,
						voie,
						ordre,
						translation,
						extension
						)
						values
						(
						@va_NumEvt_in,
						@va_CleEvt_in,
						@vl_portion,
						@vl_PRdebS,
						@vl_sens,
						3,
						XDC_AJOUT,
						@vl_translation,
						@vl_extension
						)
					end
				end
		end

/*A	passe au segment suivant */

		fetch Segment into	@vl_portion,
					@vl_PRdebP,
					@vl_PRfinP,
					@vl_PRdebS,
					@vl_sens,
					@vl_voie
		
	end


	return XDC_OK
go

