/*E*/
/*  Fichier : $Id: xzaz01sp.prc,v 1.17 2020/11/03 17:06:25 pc2dpdy Exp $      Release : $Revision: 1.17 $        Date : $Date: 2020/11/03 17:06:25 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzaz01sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture pour satir
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	17/01/95	: Creation
* C.T.	19/01/95	: Correction erreur de type dans 
*                         appel a XZAE;11 (V 1.2)
*			  autoriser les commentaires null
* C.T.	28/02/95	: Modif appel a XZAE08 ( V 1.3)
* C.T.  07/03/95        : Modif interface d'appel (V 1.4)
* C.T.  09/03/95        : Correction horodate_validation 
*                         utilise pour modif travaux (V 1.5)
* B.G.	17/05/95	: Modif pour prendre en compte des voies en trop ou en moins sur SATIR (V 1.6)
* P.V.  09/08/96	: Maj suite modif xzae08 (DEM76) (V1.7)
* P.V.  22/08/96	: Modif delai FMC prevision 24h->10h (V1.8) (ANA32)	
* C.T.  11/10/96	: Modif Appel XZAE;11 pour FMC3 (V1.9) 
* C.T.  11/10/96	: Suppression des millisecondes de la date de validation (DEM 1236) (V1.10) 
* C.T.  14/11/96	: Modif interface XZAE;11 et XZAE;66 pour bau etroite (SAD4) (V1.11)
* P.NI	14/03/05	: Ajout d'un arg � XZAE66 pour DEM439 1.13
* JPL	06/04/07	: Changement de nom de la procedure XZAE11 appelee (1.14)
* JPL	11/06/07	: Changement de nom procedures XZAE08 et XZAE61 appelees (lie a DEM 647) 1.15
* JPL	16/04/09	: Appel a XZAE66 degroupee (lie a DEM 878) 1.16
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Creer une fiche travaux et la valider
* -----------------------------------------------------
* Sequence d'appel
* SP	XZAZ01_Ecrire_Travaux_Satir
* 
* Arguments en entree
* XDY_Entier    va_NumEvt_in
* XDY_Octet     va_CleEvt_in
* XDY_Horodate	va_DebutPrevu_in 
* XDY_Horodate	va_Debut_in 
* XDY_Horodate	va_FinPrevue_in 
* XDY_Autoroute	va_Autoroute_out
* XDY_PR	va_PR_in
* XDY_Sens	va_Sens_in
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
* XDY_Commentaire	va_Texte_in
* XDY_Booleen   va_Nature_in
* XDY_Octet	va_Type_in
* XDY_Booleen   va_Continu_in
* XDY_Horodate	va_HoroDebC_in
* XDY_Horodate	va_HoroFinC_in
* XDY_Booleen   va_Mobile_in
* XDY_PR	va_PRqueueM_in
* XDY_PR	va_PRteteM_in
* XDY_Sens	va_SensM_in
* XDY_PR	va_PRtete_in
* XDY_Booleen   va_VoiesRet_in
* 
* Arguments en sortie
*
* XDY_Entier    va_NumEvt_out
* XDY_Octet     va_CleEvt_out
* 
* Code retour
* XDC_OK
* XDC_NOK		: fiche non trouvee
* XZAZC_DEBUT_INC	: debut prevue est inferieure a la date
*                         actuelle + 10h.
* XZAZC_LOC_INC		: localisation incorrecte
* XDC_ARG_INV		: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* La date de debut prevue doit etre superieure a la date
* actuelle + 10h. 
* La date de debut doit etre nulle (ou valoir 1970 pour les 
* clients en C)
* 
* Fonction
* Creation d'une fiche travaux, insertion du complement
* de la fiche et validation de la fiche
----------------------------------------------------*/

use PRC
go

create procedure XZAZ;01
	@va_NumEvt_in           int             = null,
	@va_CleEvt_in           tinyint         = null,
	@va_DebutPrevu_in	datetime 	= null,
	@va_Debut_in		datetime 	= null,
	@va_FinPrevue_in	datetime 	= null,
	@va_Autoroute_in	tinyint	 	= null,
	@va_PR_in		T_PR	 	= null,
	@va_Sens_in		tinyint	 	= null,
	@va_VR_in		tinyint	 	= null,
	@va_VM2_in		tinyint	 	= null,
	@va_VM1_in		tinyint	 	= null,
	@va_VL_in		tinyint	 	= null,
	@va_BAU_in		tinyint	 	= null,
	@va_VR_I_in		tinyint	 	= null,
	@va_VM2_I_in		tinyint	 	= null,
	@va_VM1_I_in		tinyint	 	= null,
	@va_VL_I_in		tinyint	 	= null,
	@va_BAU_I_in		tinyint	 	= null,
	@va_Texte_in		char(250) 	= null,
	@va_Nature_in		bit	 	,
	@va_Type_in		tinyint	 	= null,	
	@va_Continu_in		bit	 	,
	@va_HoroDebC_in		datetime 	= null,
	@va_HoroFinC_in		datetime 	= null,
	@va_Mobile_in		bit	 	,
	@va_PRqueueM_in		T_PR	 	= null,
	@va_PRteteM_in		T_PR	 	= null,
	@va_SensM_in		T_SENS	 	= null,
	@va_PRtete_in		T_PR	 	= null,
	@va_VoiesRet_in		bit,
	@va_NumEvt_out          int             output,
	@va_CleEvt_out          tinyint         output,
        @va_NomSiteLocal_in             char(2)         = null

as
	declare @vl_PtCaracteristique tinyint, @vl_TypePtCaracteristique tinyint,
		@vl_Autoroute T_AUTOROUTE, @vl_PR T_PR, @vl_Sens T_SENS,
		@vl_Position tinyint, @vl_VR T_VOIE, @vl_VM2 T_VOIE, 
		@vl_VM1 T_VOIE, @vl_VL T_VOIE, @vl_BAU T_VOIE, 
		@vl_VR_I T_VOIE, @vl_VM2_I T_VOIE, @vl_VM1_I T_VOIE, 
		@vl_VL_I T_VOIE, @vl_BAU_I T_VOIE, @vl_ResultConfig int,
		@vl_District tinyint, @vl_DistrictRecouvrement tinyint,
		@vl_AutorouteM T_AUTOROUTE, @vl_Delta int,
		@vl_DateCourante datetime, @vl_Status int, @vl_Modif int, @vl_Origine char(25),
		@vl_BAU_etroite tinyint, @vl_BAU_I_etroite tinyint, @vl_Site tinyint

	/*A controle des arguments obligatoires */
	/*! cas d'un client : la date nulle vaut 1970 */
	if @va_DebutPrevu_in = null or (@va_Debut_in is not null and
	    @va_Debut_in > "1970")
		return XDC_ARG_INV

	/*A test de la date de debut prevu */
	select @vl_DateCourante = getdate ()
	select @vl_DateCourante = dateadd(millisecond, -datepart(millisecond,@vl_DateCourante),@vl_DateCourante)
	select @vl_Delta = datediff( hour, @vl_DateCourante, @va_DebutPrevu_in)
	if @vl_Delta < 10
		return XZAZC_DEBUT_INC

	select @va_Debut_in = null

	/*A tester la validite de la configuration des voies donnee en entree */
	exec @vl_Status = XZAE66 @va_Autoroute_in, @va_PR_in, @va_Sens_in,
				null, null, null, 0, @vl_Autoroute output,
				@vl_PR output, @vl_Sens output, @vl_PtCaracteristique output,
				@vl_TypePtCaracteristique output, @vl_Position output,
				@vl_VR output, @vl_VM2 output, @vl_VM1 output,
				@vl_VL output, @vl_BAU output, 
				@vl_VR_I output, @vl_VM2_I output, @vl_VM1_I output,
				@vl_VL_I output, @vl_BAU_I output, 
				@vl_BAU_etroite output, @vl_BAU_I_etroite output, 
				@vl_District output, @vl_DistrictRecouvrement output,
				@vl_ResultConfig output

	if @vl_Status = null
		return XDC_PRC_INC
	else if @vl_Status != XDC_OK
		return XZAZC_LOC_INC		

	/* tester si la localisation donnee en entree etait correcte */
	select @vl_Modif=XDC_NON

	if @vl_ResultConfig != XDC_OK or @va_Autoroute_in != @vl_Autoroute or
	   @va_PR_in != @vl_PR or @va_Sens_in != @vl_Sens
		return XZAZC_LOC_INC
	/* cas ou satir a plus de voies que le sae : on supprime */
	if @vl_VR = XDC_VOIE_INEXISTANTE and @va_VR_in != XDC_VOIE_INEXISTANTE
			select @va_VR_in= XDC_VOIE_INEXISTANTE, @vl_Modif=XDC_OUI	
	if @vl_VM2 = XDC_VOIE_INEXISTANTE and @va_VM2_in != XDC_VOIE_INEXISTANTE
	                select @va_VM2_in= XDC_VOIE_INEXISTANTE, @vl_Modif=XDC_OUI      
	if @vl_VM1 = XDC_VOIE_INEXISTANTE and @va_VM1_in != XDC_VOIE_INEXISTANTE
			select @va_VM1_in= XDC_VOIE_INEXISTANTE, @vl_Modif=XDC_OUI
	if @vl_VL = XDC_VOIE_INEXISTANTE and @va_VL_in != XDC_VOIE_INEXISTANTE
			select @va_VL_in= XDC_VOIE_INEXISTANTE, @vl_Modif=XDC_OUI
	if @vl_BAU = XDC_VOIE_INEXISTANTE and @va_BAU_in != XDC_VOIE_INEXISTANTE
			select @va_BAU_in= XDC_VOIE_INEXISTANTE, @vl_Modif=XDC_OUI
	if @vl_VR_I = XDC_VOIE_INEXISTANTE and @va_VR_I_in != XDC_VOIE_INEXISTANTE
			select @va_VR_I_in= XDC_VOIE_INEXISTANTE, @vl_Modif=XDC_OUI
	if @vl_VM2_I = XDC_VOIE_INEXISTANTE and @va_VM2_I_in != XDC_VOIE_INEXISTANTE
			select @va_VM2_I_in= XDC_VOIE_INEXISTANTE, @vl_Modif=XDC_OUI
	if @vl_VM1_I = XDC_VOIE_INEXISTANTE and @va_VM1_I_in != XDC_VOIE_INEXISTANTE
			select @va_VM1_I_in= XDC_VOIE_INEXISTANTE, @vl_Modif=XDC_OUI
	if @vl_VL_I = XDC_VOIE_INEXISTANTE and @va_VL_I_in != XDC_VOIE_INEXISTANTE
			select @va_VL_I_in= XDC_VOIE_INEXISTANTE, @vl_Modif=XDC_OUI
	if @vl_BAU_I = XDC_VOIE_INEXISTANTE and @va_BAU_I_in != XDC_VOIE_INEXISTANTE
			select @va_BAU_I_in= XDC_VOIE_INEXISTANTE, @vl_Modif=XDC_OUI

	/* cas ou satir a moins de voies que le sae : on reconstitue */
	if @vl_VR != XDC_VOIE_INEXISTANTE and @va_VR_in = XDC_VOIE_INEXISTANTE
			select @va_VR_in= XDC_VOIE_SENS_NORMAL, @vl_Modif=XDC_OUI	
	if @vl_VM2 != XDC_VOIE_INEXISTANTE and @va_VM2_in = XDC_VOIE_INEXISTANTE
	                select @va_VM2_in= XDC_VOIE_SENS_NORMAL, @vl_Modif=XDC_OUI      
	if @vl_VM1 != XDC_VOIE_INEXISTANTE and @va_VM1_in = XDC_VOIE_INEXISTANTE
			select @va_VM1_in= XDC_VOIE_SENS_NORMAL, @vl_Modif=XDC_OUI
	if @vl_VL != XDC_VOIE_INEXISTANTE and @va_VL_in = XDC_VOIE_INEXISTANTE
			select @va_VL_in= XDC_VOIE_SENS_NORMAL, @vl_Modif=XDC_OUI
	if @vl_BAU != XDC_VOIE_INEXISTANTE and @va_BAU_in = XDC_VOIE_INEXISTANTE
			select @va_BAU_in= XDC_VOIE_BAU_NORMALE, @vl_Modif=XDC_OUI
	if @vl_VR_I != XDC_VOIE_INEXISTANTE and @va_VR_I_in = XDC_VOIE_INEXISTANTE
			select @va_VR_I_in= XDC_VOIE_SENS_NORMAL, @vl_Modif=XDC_OUI
	if @vl_VM2_I != XDC_VOIE_INEXISTANTE and @va_VM2_I_in = XDC_VOIE_INEXISTANTE
			select @va_VM2_I_in= XDC_VOIE_SENS_NORMAL, @vl_Modif=XDC_OUI
	if @vl_VM1_I != XDC_VOIE_INEXISTANTE and @va_VM1_I_in = XDC_VOIE_INEXISTANTE
			select @va_VM1_I_in= XDC_VOIE_SENS_NORMAL, @vl_Modif=XDC_OUI
	if @vl_VL_I != XDC_VOIE_INEXISTANTE and @va_VL_I_in = XDC_VOIE_INEXISTANTE
			select @va_VL_I_in= XDC_VOIE_SENS_NORMAL, @vl_Modif=XDC_OUI
	if @vl_BAU_I != XDC_VOIE_INEXISTANTE and @va_BAU_I_in = XDC_VOIE_INEXISTANTE
			select @va_BAU_I_in= XDC_VOIE_BAU_NORMALE, @vl_Modif=XDC_OUI

	/* si on a reconstitue des voies, on positionne origine a "SATIR MODIFIE" */
	select @vl_Origine=XZAZC_FMC_SATIR
	if @vl_Modif=XDC_OUI select @vl_Origine=XZAZC_FMC_SATIR_MODIF
	begin tran
		if @va_NumEvt_in = 0 and @va_CleEvt_in = 0
		begin
			/*A creation d'une fiche travaux en prevision */
			exec @vl_Status = XZAE08 XZAEC_FMC_Travaux, null, XDC_NON,
					@va_Debut_in, @va_DebutPrevu_in,null,
					@va_NumEvt_out output, @va_CleEvt_out output
	
			if @vl_Status = null
				select @vl_Status = XDC_PRC_INC
			if @vl_Status != XDC_OK
			begin
				return @vl_Status
			end
		end
		else
		begin
			/*A Tester l'existance de l'evenement */
			/*A pour une modification d'evenement */
			if not exists ( select * from EXP..FMC_GEN
					where numero = @va_NumEvt_in and cle = @va_CleEvt_in)
			begin
				select @vl_Status = XDC_NOK
				return @vl_Status
			end
			else	select	@va_NumEvt_out = @va_NumEvt_in,
					@va_CleEvt_out = @va_CleEvt_in

			/* Verification du site */
			select  @vl_Site = numero
				from CFG..RES_DIS
				where serveur = @@servername and type = XDC_TYPEM_SD

			if @@rowcount = 1			/*A recherche de la date de validation */
				select @vl_DateCourante = horodate_validation
				from EXP..FMC_HIS
				where	numero = @va_NumEvt_out and cle = @va_CleEvt_out and
					sit = @vl_Site


		end

		/* informations a inserer concernant des travaux mobiles */
		if @va_Mobile_in = XDC_OUI
			select @vl_AutorouteM = @va_Autoroute_in
		else select	@vl_AutorouteM = null,
				@va_PRteteM_in = null,
				@va_SensM_in = null,
				@va_PRqueueM_in = null

		/*A Ecriture du complement de la fiche travaux */
		/*! le poste operateur est mis a 1 juste pour l'appel de la procedure */
		exec @vl_Status = XZAE61 @va_NumEvt_out, @va_CleEvt_out, @va_Nature_in,
					@va_Type_in, @va_Continu_in, null, null,
					@va_Mobile_in, @vl_AutorouteM, @va_PRqueueM_in,
					@va_PRteteM_in, @va_SensM_in, @va_PRtete_in,
					@va_VoiesRet_in, @vl_DateCourante,
					1, XDC_FMC_ETAT_PREV,@va_NomSiteLocal_in
		if @vl_Status = null
		begin
			select @vl_Status = XDC_PRC_INC
			return @vl_Status
		end
		else if @vl_Status != XDC_OK
		begin
			rollback tran
			return @vl_Status
		end
	
		/*A Ecriture du commentaire de la fiche travaux */
		/*! le poste operateur est mis a 1 juste pour l'appel de la procedure */
		if @va_Texte_in is not null
			exec @vl_Status = XZAE;67 @va_NumEvt_out, @va_CleEvt_out, null,
						@vl_DateCourante, null,
						@va_Texte_in, 1
	
		if @vl_Status = null
		begin
			select @vl_Status = XDC_PRC_INC
			return @vl_Status
		end
		if @vl_Status != XDC_OK
		begin
			rollback tran
			return @vl_Status
		end
	
		/*A Validation de la fiche travaux */
		exec @vl_Status = XZAE11 @va_NumEvt_out, @va_CleEvt_out, null,
					XDC_FMC_ETAT_PREV, XZAEC_FMC_Travaux,
					@va_DebutPrevu_in, @va_Debut_in,
					@va_FinPrevue_in, null, @vl_DateCourante,
					null, null, null, @vl_Origine, 
					null, XZAEC_FMC_CONFIRMEE,
					@va_Autoroute_in, @va_PR_in, @va_Sens_in, null,
					null, null, @va_VR_in, @va_VM2_in, @va_VM1_in,
					@va_VL_in, @va_BAU_in, @va_VR_I_in,
					@va_VM2_I_in, @va_VM1_I_in, @va_VL_I_in, @va_BAU_I_in,
					@vl_BAU_etroite, @vl_BAU_I_etroite,
					XDC_FAUX, XZAEC_FMC_Travaux, XDC_FAUX, @va_NomSiteLocal_in
	
	
		if @vl_Status = null
			select @vl_Status = XDC_PRC_INC
		if @vl_Status != XDC_OK
			rollback tran
	commit tran

	return @vl_Status
go
