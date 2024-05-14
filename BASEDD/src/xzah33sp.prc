/*E*/
/*  Fichier : $Id: xzah33sp.prc,v 1.1 1997/05/21 11:10:11 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1997/05/21 11:10:11 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah33sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.	07/09/94	: Creation dem/1372
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare l'animation IHM (symbole de rabattement) pour presignalisation
* 
* Sequence d'appel
* SP	XZAH33_Animer_rab_presignalisation
* 
* Arguments en entree
* XDY_Evt		va_NumEvt_in
* XDY_Autoroute		va_Autoroute_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_Octet		va_DebutFin_in
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
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: argument invalide
* <0 erreur sybase (voir Transact SQL p 33-15)
* 
* Conditions d'utilisation
* appel≈ par XZAH;32
* 
------------------------------------------------------*/

use PRC
go

create procedure  XZAH;33
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null,
	@va_Autoroute_in	T_AUTOROUTE	= null,
	@va_PR_in		T_PR		= null,
	@va_Sens_in		T_SENS		= null,
	@va_numordre		tinyint		= null,
	@va_vvoie1		tinyint		= null,
	@va_vvoie2		tinyint		= null,
	@va_vvoie3		tinyint		= null,
	@va_vvoie4		tinyint		= null,
	@va_vvoie5		tinyint		= null,
	@va_vvoie6		tinyint		= null,
	@va_vvoie7		tinyint		= null,
	@va_vvoie8		tinyint		= null,
	@va_vvoie9		tinyint		= null,
	@va_vvoie10		tinyint		= null,
	@va_nvoie1		tinyint		= null,
	@va_nvoie2		tinyint		= null,
	@va_nvoie3		tinyint		= null,
	@va_nvoie4		tinyint		= null,
	@va_nvoie5		tinyint		= null,
	@va_nvoie6		tinyint		= null,
	@va_nvoie7		tinyint		= null,
	@va_nvoie8		tinyint		= null,
	@va_nvoie9		tinyint		= null,
	@va_nvoie10		tinyint		= null
	
as

/*A D≈claration des variables locales */

	declare @vl_Portion smallint,
		@vl_Translation int,
		@vl_NombreSud tinyint,
		@vl_NombreNord tinyint,
		@vl_Nombre int,
		@vl_nvoie1 tinyint ,
		@vl_vvoie1 tinyint ,
		@vl_nvoie2 tinyint ,
		@vl_vvoie2 tinyint ,
		@vl_nvoie3 tinyint ,
		@vl_vvoie3 tinyint ,
		@vl_nvoie4 tinyint ,
		@vl_vvoie4 tinyint ,
		@vl_nvoie5 tinyint ,
		@vl_vvoie5 tinyint ,
		@vl_nvoie6 tinyint ,
		@vl_vvoie6 tinyint ,
		@vl_nvoie7 tinyint ,
		@vl_vvoie7 tinyint ,
		@vl_nvoie8 tinyint ,
		@vl_vvoie8 tinyint ,
		@vl_nvoie9 tinyint ,
		@vl_vvoie9 tinyint ,
		@vl_nvoie10 tinyint ,
		@vl_vvoie10 tinyint ,
		@vl_autoinv tinyint ,
		@vl_voie tinyint
		
/*A Test des param…tres d'appel */

/*B	Test des param…tres nuls */

	if	@va_NumEvt_in=null or
		@va_CleEvt_in=null
	return XDC_NOK


/*A Test des param…tres d'appel (suite)*/

/*B	Test des param…tres nuls */

	if	@va_PR_in=null or
		@va_Sens_in=null or
		@va_nvoie1=null or
		@va_nvoie2=null or
		@va_nvoie3=null or
		@va_nvoie4=null or
		@va_nvoie5=null or
		@va_vvoie1=null or
		@va_vvoie2=null or
		@va_vvoie3=null or
		@va_vvoie4=null or
		@va_vvoie5=null or
		@va_nvoie6=null or
		@va_vvoie6=null or
		@va_nvoie7=null or
		@va_vvoie7=null or
		@va_nvoie8=null or
		@va_vvoie8=null or
		@va_nvoie9=null or
		@va_vvoie9=null or
		@va_nvoie10=null or
		@va_vvoie10=null
	return XDC_NOK

/*A Si le sens n'est pas sur section courante -> suppression */

	if	@va_Sens_in<>XDC_SENS_NORD and
		@va_Sens_in<>XDC_SENS_SUD
	begin
		update EXP..SYN_RAB_SEG
		
		set ordre=XDC_SUPPR
		
		where	evenement=@va_NumEvt_in and
			cle=@va_CleEvt_in
	return XDC_OK
	end

/*A Calcule portion, et translation */

	select	@vl_Portion=numero,
		@vl_Translation=10000*(@va_PR_in-PR_debut)/(PR_fin-PR_debut)
		
	from CFG..RES_POR
	
	where	autoroute=@va_Autoroute_in and
		PR_debut<@va_PR_in and
		PR_fin>=@va_PR_in
		
	if @@rowcount<>1
	return XDC_NOK

/*A Initialise les variables voies */

	select	@vl_nvoie1 = XDC_VOIE_INEXISTANTE,
		@vl_vvoie1 = XDC_VOIE_INEXISTANTE,
		@vl_nvoie2 = XDC_VOIE_INEXISTANTE,
		@vl_vvoie2 = XDC_VOIE_INEXISTANTE,
		@vl_nvoie3 = XDC_VOIE_INEXISTANTE,
		@vl_vvoie3 = XDC_VOIE_INEXISTANTE,
		@vl_nvoie4 = XDC_VOIE_INEXISTANTE,
		@vl_vvoie4 = XDC_VOIE_INEXISTANTE,
		@vl_nvoie5 = XDC_VOIE_INEXISTANTE,
		@vl_vvoie5 = XDC_VOIE_INEXISTANTE,
		@vl_nvoie6 = XDC_VOIE_INEXISTANTE,
		@vl_vvoie6 = XDC_VOIE_INEXISTANTE,
		@vl_nvoie7 = XDC_VOIE_INEXISTANTE,
		@vl_vvoie7 = XDC_VOIE_INEXISTANTE,
		@vl_nvoie8 = XDC_VOIE_INEXISTANTE,
		@vl_vvoie8 = XDC_VOIE_INEXISTANTE,
		@vl_nvoie9 = XDC_VOIE_INEXISTANTE,
		@vl_vvoie9 = XDC_VOIE_INEXISTANTE,
		@vl_nvoie10 = XDC_VOIE_INEXISTANTE,
		@vl_vvoie10 = XDC_VOIE_INEXISTANTE




/*A Affectation des voies d'apres le sens */

	if @va_Sens_in = XDC_SENS_SUD
	begin
		select	@vl_nvoie1	= @va_nvoie1
		select	@vl_nvoie2	= @va_nvoie2
		select	@vl_nvoie3	= @va_nvoie3
		select	@vl_nvoie4	= @va_nvoie4
		select	@vl_nvoie5	= @va_nvoie5
		select	@vl_nvoie6	= @va_nvoie6
		select	@vl_nvoie7	= @va_nvoie7
		select	@vl_nvoie8	= @va_nvoie8
		select	@vl_nvoie9	= @va_nvoie9
		select	@vl_nvoie10	= @va_nvoie10
		
		select	@vl_vvoie1	= @va_vvoie1
		select	@vl_vvoie2	= @va_vvoie2
		select	@vl_vvoie3	= @va_vvoie3
		select	@vl_vvoie4	= @va_vvoie4
		select	@vl_vvoie5	= @va_vvoie5
		select	@vl_vvoie6	= @va_vvoie6
		select	@vl_vvoie7	= @va_vvoie7
		select	@vl_vvoie8	= @va_vvoie8
		select	@vl_vvoie9	= @va_vvoie9
		select	@vl_vvoie10	= @va_vvoie10
	end
	else
	begin
		select	@vl_nvoie1	= @va_vvoie10
		select	@vl_nvoie2	= @va_vvoie9
		select	@vl_nvoie3	= @va_vvoie8
		select	@vl_nvoie4	= @va_vvoie7
		select	@vl_nvoie5	= @va_vvoie6
		select	@vl_nvoie6	= @va_vvoie5
		select	@vl_nvoie7	= @va_vvoie4
		select	@vl_nvoie8	= @va_vvoie3
		select	@vl_nvoie9	= @va_vvoie2
		select	@vl_nvoie10	= @va_vvoie1
		
		select	@vl_vvoie1	= @va_nvoie10
		select	@vl_vvoie2	= @va_nvoie9
		select	@vl_vvoie3	= @va_nvoie8
		select	@vl_vvoie4	= @va_nvoie7
		select	@vl_vvoie5	= @va_nvoie6
		select	@vl_vvoie6	= @va_nvoie5
		select	@vl_vvoie7	= @va_nvoie4
		select	@vl_vvoie8	= @va_nvoie3
		select	@vl_vvoie9	= @va_nvoie2
		select	@vl_vvoie10	= @va_nvoie1
	end    

/*A Toutes les voies sont positionn≈es en variable locale-> mise en table */

/*A Si l'autoroute est inversee (A57) echange des voies (1<-->10,2<-->9, etc) */

	if exists (	select * from CFG..RES_AUT_INV
			where	numero=@va_Autoroute_in
			  and	inversee=XDC_PR_INV )
	begin
		select	@vl_voie	= @vl_nvoie1
		select	@vl_nvoie1	= @vl_nvoie10
		select	@vl_nvoie10	= @vl_voie
		
		select	@vl_voie	= @vl_nvoie2
		select	@vl_nvoie2	= @vl_nvoie9
		select	@vl_nvoie9	= @vl_voie
		
		select	@vl_voie	= @vl_nvoie3
		select	@vl_nvoie3	= @vl_nvoie8
		select	@vl_nvoie8	= @vl_voie
		
		select	@vl_voie	= @vl_nvoie4
		select	@vl_nvoie4	= @vl_nvoie7
		select	@vl_nvoie7	= @vl_voie
		
		select	@vl_voie	= @vl_nvoie5
		select	@vl_nvoie5	= @vl_nvoie6
		select	@vl_nvoie6	= @vl_voie
		
		select	@vl_voie	= @vl_vvoie1
		select	@vl_vvoie1	= @vl_vvoie10
		select	@vl_vvoie10	= @vl_voie
		
		select	@vl_voie	= @vl_vvoie2
		select	@vl_vvoie2	= @vl_vvoie9
		select	@vl_vvoie9	= @vl_voie
		
		select	@vl_voie	= @vl_vvoie3
		select	@vl_vvoie3	= @vl_vvoie8
		select	@vl_vvoie8	= @vl_voie
		
		select	@vl_voie	= @vl_vvoie4
		select	@vl_vvoie4	= @vl_vvoie7
		select	@vl_vvoie7	= @vl_voie
		
		select	@vl_voie	= @vl_vvoie5
		select	@vl_vvoie5	= @vl_vvoie6
		select	@vl_vvoie6	= @vl_voie
		
		
		select	@vl_voie	= @vl_vvoie1
		select	@vl_vvoie1	= @vl_nvoie1
		select	@vl_nvoie1	= @vl_voie
		
		select	@vl_voie	= @vl_vvoie2
		select	@vl_vvoie2	= @vl_nvoie2
		select	@vl_nvoie2	= @vl_voie
		
		select	@vl_voie	= @vl_vvoie2
		select	@vl_vvoie2	= @vl_nvoie2
		select	@vl_nvoie2	= @vl_voie
		
		select	@vl_voie	= @vl_vvoie3
		select	@vl_vvoie3	= @vl_nvoie3
		select	@vl_nvoie3	= @vl_voie
		
		select	@vl_voie	= @vl_vvoie4
		select	@vl_vvoie4	= @vl_nvoie4
		select	@vl_nvoie4	= @vl_voie
		
		select	@vl_voie	= @vl_vvoie5
		select	@vl_vvoie5	= @vl_nvoie5
		select	@vl_nvoie5	= @vl_voie
		
		select	@vl_voie	= @vl_vvoie6
		select	@vl_vvoie6	= @vl_nvoie6
		select	@vl_nvoie6	= @vl_voie
		
		select	@vl_voie	= @vl_vvoie7
		select	@vl_vvoie7	= @vl_nvoie7
		select	@vl_nvoie7	= @vl_voie
		
		select	@vl_voie	= @vl_vvoie8
		select	@vl_vvoie8	= @vl_nvoie8
		select	@vl_nvoie8	= @vl_voie
		
		select	@vl_voie	= @vl_vvoie9
		select	@vl_vvoie9	= @vl_nvoie9
		select	@vl_nvoie9	= @vl_voie
		
		select	@vl_voie	= @vl_vvoie10
		select	@vl_vvoie10	= @vl_nvoie10
		select	@vl_nvoie10	= @vl_voie
		
		/*! si on se trouve sur l'A57, on renvoie translation entre -10001 et -1 */
		
		select @vl_Translation=@vl_Translation-10001
	end

/*A Verifie s'il s'agit d'une cr≈ation ou d'une modification */

	select @vl_Nombre=count(*)
	
	from EXP..SYN_RAB_SEG holdlock
	
	where	evenement=@va_NumEvt_in and
		cle=@va_CleEvt_in and
		numordre=@va_numordre and
		portion=@vl_Portion

/*B	il s'agit d'une modification */

	if @vl_Nombre=1
	begin
		update EXP..SYN_RAB_SEG
		set	translation=@vl_Translation,
			nvoie1=@vl_nvoie1,
			vvoie1=@vl_vvoie1,
			nvoie2=@vl_nvoie2,
			vvoie2=@vl_vvoie2,
			nvoie3=@vl_nvoie3,
			vvoie3=@vl_vvoie3,
			nvoie4=@vl_nvoie4,
			vvoie4=@vl_vvoie4,
			nvoie5=@vl_nvoie5,
			vvoie5=@vl_vvoie5,
			nvoie6=@vl_nvoie6,
			vvoie6=@vl_vvoie6,
			nvoie7=@vl_nvoie7,
			vvoie7=@vl_vvoie7,
			nvoie8=@vl_nvoie8,
			vvoie8=@vl_vvoie8,
			nvoie9=@vl_nvoie9,
			vvoie9=@vl_vvoie9,
			nvoie10=@vl_nvoie10,
			vvoie10=@vl_vvoie10
		where	evenement=@va_NumEvt_in and
			cle=@va_CleEvt_in and
			debut_fin=0 and
			numordre=@va_numordre and
			portion=@vl_Portion

		update EXP..SYN_RAB_SEG
		set	ordre=XDC_MODIF
		where	evenement=@va_NumEvt_in and
			cle=@va_CleEvt_in and
			debut_fin=0 and
			numordre=@va_numordre and
			portion=@vl_Portion and
			ordre<>XDC_AJOUT
	end

/*B	il s'agit d'une modification importante (suppression puis ajout)
	ou d'un ajout */
	
	else
	begin
		update EXP..SYN_RAB_SEG
		set	ordre=XDC_SUPPR
		where	evenement=@va_NumEvt_in and
			cle=@va_CleEvt_in and
			numordre=@va_numordre 

		insert EXP..SYN_RAB_SEG (
			evenement,
			cle,
			debut_fin,
			portion,
			ordre,
			translation,
			nvoie1,
			vvoie1,
			nvoie2,
			vvoie2,
			nvoie3,
			vvoie3,
			nvoie4,
			vvoie4,
			nvoie5,
			vvoie5,
			nvoie6,
			vvoie6,
			nvoie7,
			vvoie7,
			nvoie8,
			vvoie8,
			nvoie9,
			vvoie9,
			nvoie10,
			vvoie10,
			numordre)
		values	(
			@va_NumEvt_in,
			@va_CleEvt_in,
			0,
			@vl_Portion,
			XDC_AJOUT,
			@vl_Translation,
			@vl_nvoie1,
			@vl_vvoie1,
			@vl_nvoie2,
			@vl_vvoie2,
			@vl_nvoie3,
			@vl_vvoie3,
			@vl_nvoie4,
			@vl_vvoie4,
			@vl_nvoie5,
			@vl_vvoie5,
			@vl_nvoie6,
			@vl_vvoie6,
			@vl_nvoie7,
			@vl_vvoie7,
			@vl_nvoie8,
			@vl_vvoie8,
			@vl_nvoie9,
			@vl_vvoie9,
			@vl_nvoie10,
			@vl_vvoie10,
			@va_numordre
			)
	end
	return XDC_OK
go
