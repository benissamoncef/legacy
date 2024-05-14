/*E*/
/*  Fichier : $Id: xzah16sp.prc,v 1.4 1996/12/23 14:56:09 gaborit Exp $      Release : $Revision: 1.4 $        Date : $Date: 1996/12/23 14:56:09 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah16sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm
*
------------------------------------------------------
* HISTORIQUE :
*
* B.G.  28/10/94        : Cr≈ation
* C.T.  08/12/94        : Ajout les bouchons sur voie inverse (V 1.2)
* B.G.  22/12/94        : modif (1.3)
* C.T.  11/12/96        : Modif appel XZAH;19 (FMC7) (1.4)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare l'animation IHM (basculement)
* 
* Sequence d'appel
* SP	XZAH16_Animer_Basculement
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
* XDC_NOK	: argument invalide
* 
* Conditions d'utilisation
* 
* appel≈ sur trigger par fmc_his_iu (fmc_his.trg)
------------------------------------------------------*/

use PRC
go

create procedure  XZAH;16
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null,
	@va_Autoroute_in	T_AUTOROUTE	= null,
	@va_PR_in		T_PR		= null,
	@va_Sens_in		T_SENS		= null,
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

	declare	@vl_PRTete	int

/*A Test des param…tres d'appel */

/*B	Test des param…tres nuls */

	if	@va_NumEvt_in=null or
		@va_CleEvt_in=null
	return XDC_NOK

/*A S'il s'agit d'une suppression, supprime les rabattements,
    les voies neutralis≈es, les voies invers≈es RADT, et les
    bouchons invers≈s */

	if @va_Suppr_in=XDC_VRAI
	begin
		exec PRC..XZAH;13	@va_NumEvt_in=@va_NumEvt_in,
					@va_CleEvt_in=@va_CleEvt_in,
					@va_Suppr_in=XDC_VRAI	

		exec PRC..XZAH;17	@va_NumEvt_in=@va_NumEvt_in,
					@va_CleEvt_in=@va_CleEvt_in,
					@va_Suppr_in=XDC_VRAI	

		exec PRC..XZAH;18	@va_NumEvt_in=@va_NumEvt_in,
					@va_CleEvt_in=@va_CleEvt_in,
					@va_Suppr_in=XDC_VRAI	

		exec PRC..XZAH;19	@va_NumEvt_in=@va_NumEvt_in, 
					@va_CleEvt_in=@va_CleEvt_in, 
					@va_TypeFMC_in=XZAEC_FMC_Basculement, 
					@va_Suppr_in=XDC_VRAI

		return XDC_OK
	end

/*A Test des param…tres d'appel (suite)*/

/*B	Test des param…tres nuls */

	if	@va_Autoroute_in=null or
		@va_PR_in=null or
		@va_Sens_in=null or
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

/*A lecture du compl≈ment basculement */

	select @vl_PRTete=localisation_PR_fin
	from EXP..FMC_BAS
	where	numero=@va_NumEvt_in and
		cle=@va_CleEvt_in

	if @@rowcount<>1 return XDC_NOK

/*A animation des voies neutralis≈es */

	exec PRC..XZAH;17	@va_NumEvt_in,
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
				XDC_FAUX -- suppression=faux
						

/*A puis animation du symbole de rabattement d≈but */

	exec PRC..XZAH;13	@va_NumEvt_in,
				@va_CleEvt_in,
				@va_Autoroute_in,
				@va_PR_in,
				@va_Sens_in,
				XZAHC_Debut, -- symbole au d≈but basculement
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
				XDC_FAUX -- suppression=faux

/*A puis animation du symbole de rabattement fin */

	exec PRC..XZAH;13	@va_NumEvt_in,
				@va_CleEvt_in,
				@va_Autoroute_in,
				@vl_PRTete,
				@va_Sens_in,
				XZAHC_Fin, -- symbole » la fin basculement
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
				XDC_FAUX -- suppression=faux

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
				XDC_FAUX -- suppression=faux

/*A puis bouchon invers≈ */

	exec PRC..XZAH;19	@va_NumEvt_in, 
				@va_CleEvt_in, 
				XZAEC_FMC_Basculement, 
				null,
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
				XDC_FAUX -- suppression=faux


	return XDC_OK
go
