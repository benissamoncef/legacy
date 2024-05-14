/*E*/
/*  Fichier : $Id: xzae86sp.prc,v 1.6 2010/09/08 12:21:41 gesconf Exp $      Release : $Revision: 1.6 $        Date : $Date: 2010/09/08 12:21:41 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae86sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.     	10/04/95	: Creation	V 1.1 
* C.T.     	29/05/95	: Ajout du cas XDC_VOIE_SENS_NORMAL pour
*                                 la BAU V 1.2
* C.T.     	12/06/95	: erreur XDC v 1.3
* C.T.     	14/06/95	: suppression de la prise en compte des travaux prevus v 1.3
* JPL     	08/06/07	: Prise en compte Fmc Basculement pour Travaux 1.5
* JPL		20/08/10	: Degroupee; prise en compte Fmc Zone de Stockage (DEM 942) 1.6
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourner la configuration des voies pour une localisation
* donn≈e en prenant en compte les modifications dues aux evenements 
* de type travaux ou basculement en cours
* 
* Sequence d'appel
* SP	XZAE86_Configuration_Voies
* 
* Arguments en entree
* XDY_Autoroute	va_NumAutoroute_in
* XDC_PR	va_PR_in
* XDC_Sens	va_Sens_in
* 
* Arguments en sortie
* XDY_Voie	va_VR_out
* XDY_Voie	va_VM2_out
* XDY_Voie	va_VM1_out
* XDY_Voie	va_VL_out
* XDY_Voie	va_BAU_out
* XDY_Voie	va_VR_I_out
* XDY_Voie	va_VM2_I_out
* XDY_Voie	va_VM1_I_out
* XDY_Voie	va_VL_I_out
* XDY_Voie	va_BAU_I_out
* 
* Code retour
* XDC_OK
* XDC_NOK
* XDC_ARG_INV		: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
----------------------------------------------------*/

use PRC
go

drop procedure XZAE86
go


create procedure XZAE86
	@va_NumAutoroute_in	T_AUTOROUTE 	= null,
	@va_PR_in		T_PR	 	= null,
	@va_Sens_in		T_SENS	 	= null,
	@va_VR_out		T_VOIE	 	= null	output,
	@va_VM2_out		T_VOIE	 	= null	output,
	@va_VM1_out		T_VOIE	 	= null	output,
	@va_VL_out		T_VOIE	 	= null	output,
	@va_BAU_out		T_VOIE	 	= null	output,
	@va_VR_I_out		T_VOIE	 	= null	output,
	@va_VM2_I_out		T_VOIE	 	= null	output,
	@va_VM1_I_out		T_VOIE	 	= null	output,
	@va_VL_I_out		T_VOIE	 	= null	output,
	@va_BAU_I_out		T_VOIE	 	= null	output
as
	declare @vl_PREvt T_PR, @vl_PRExtremite T_PR, @vl_Sens T_SENS, @vl_VR T_VOIE, 
		@vl_VM2 T_VOIE, @vl_VM1 T_VOIE, @vl_VL T_VOIE, @vl_BAU T_VOIE,
		@vl_VR_I T_VOIE, @vl_VM2_I T_VOIE,@vl_VM1_I T_VOIE,
		@vl_VL_I T_VOIE, @vl_BAU_I T_VOIE, @vl_Type tinyint,
		@vl_HoroVal datetime, @vl_Evt int, @vl_Cle tinyint


	/* Controle des parametres d'entree */
	if @va_NumAutoroute_in = null or @va_PR_in = null or @va_Sens_in = null or
	   @va_NumAutoroute_in = XDC_AUT_INCONNUE or @va_PR_in = XDC_PR_INCONNU
	   	return XDC_ARG_INV

	/* initialisation de la configuration des voies */
	select @va_VR_I_out = XDC_VOIE_SENS_NORMAL,
		@va_VM2_I_out = XDC_VOIE_SENS_NORMAL,
		@va_VM1_I_out = XDC_VOIE_SENS_NORMAL,
		@va_VL_I_out = XDC_VOIE_SENS_NORMAL,
		@va_BAU_I_out = XDC_VOIE_BAU_NORMALE,
		@va_VR_out = XDC_VOIE_SENS_NORMAL,
		@va_VM2_out = XDC_VOIE_SENS_NORMAL,
		@va_VM1_out = XDC_VOIE_SENS_NORMAL,
		@va_VL_out = XDC_VOIE_SENS_NORMAL,
		@va_BAU_out = XDC_VOIE_BAU_NORMALE

	/*A rechercher les evenements en cours positionn≈s a cette localisation */
	declare Pointeur_evts_en_cours cursor
	for select
		EXP..FMC_HIS.numero,
		EXP..FMC_HIS.cle,
		EXP..FMC_GEN.type,
		EXP..FMC_HIS.horodate_validation,
		EXP..FMC_HIS.PR,
		EXP..FMC_HIS.sens,
		EXP..FMC_HIS.VR,
		EXP..FMC_HIS.VM2,
		EXP..FMC_HIS.VM1,
		EXP..FMC_HIS.VL,
		EXP..FMC_HIS.BAU,
		EXP..FMC_HIS.VR_I,
		EXP..FMC_HIS.VM2_I,
		EXP..FMC_HIS.VM1_I,
		EXP..FMC_HIS.VL_I,
		EXP..FMC_HIS.BAU_I
	from EXP..FMC_GEN, EXP..FMC_HIS
	where	debut is not null and fin is null and  
		type in (XZAEC_FMC_Basculement,
		            XZAEC_FMC_Travaux,
		            XZAEC_FMC_BasculTravaux,
		            XZAEC_FMC_ZoneStockage) and
		EXP..FMC_HIS.numero = EXP..FMC_GEN.numero and
		EXP..FMC_HIS.cle = EXP..FMC_GEN.cle
	group by EXP..FMC_GEN.numero, EXP..FMC_GEN.cle, 
		EXP..FMC_HIS.numero, EXP..FMC_HIS.cle, type, fin
	having	horodate_validation = max(horodate_validation) and 
		autoroute = @va_NumAutoroute_in and
		(sens = XDC_SENS_SUD or sens = XDC_SENS_NORD)

	/*A rechercher la configuration des voies en fonction des evts en cours */
	open Pointeur_evts_en_cours
	fetch Pointeur_evts_en_cours into @vl_Evt, @vl_Cle, @vl_Type, 
			@vl_HoroVal, @vl_PREvt, @vl_Sens,
			@vl_VR, @vl_VM2, @vl_VM1, @vl_VL, @vl_BAU,
			@vl_VR_I, @vl_VM2_I, @vl_VM1_I, @vl_VL_I, @vl_BAU_I
	while @@sqlstatus = 0
	begin

		/* repositionner la configuration des voies en groupant */
		/* l'etat neutralise,bloque avec l'etat neutralise      */
		if @vl_VR = XDC_VOIE_BLOQUEE_NEUTRALISEE
			select @vl_VR = XDC_VOIE_NEUTRALISEE
		if @vl_VM2 = XDC_VOIE_BLOQUEE_NEUTRALISEE
			select @vl_VM2 = XDC_VOIE_NEUTRALISEE
		if @vl_VM1 = XDC_VOIE_BLOQUEE_NEUTRALISEE
			select @vl_VM1 = XDC_VOIE_NEUTRALISEE
		if @vl_VL = XDC_VOIE_BLOQUEE_NEUTRALISEE
			select @vl_VL = XDC_VOIE_NEUTRALISEE
		if @vl_BAU = XDC_VOIE_BLOQUEE_NEUTRALISEE
			select @vl_BAU = XDC_VOIE_NEUTRALISEE
		if @vl_VR_I = XDC_VOIE_BLOQUEE_NEUTRALISEE
			select @vl_VR_I = XDC_VOIE_NEUTRALISEE
		if @vl_VM2_I = XDC_VOIE_BLOQUEE_NEUTRALISEE
			select @vl_VM2_I = XDC_VOIE_NEUTRALISEE
		if @vl_VM1_I = XDC_VOIE_BLOQUEE_NEUTRALISEE
			select @vl_VM1_I = XDC_VOIE_NEUTRALISEE
		if @vl_VL_I = XDC_VOIE_BLOQUEE_NEUTRALISEE
			select @vl_VL_I = XDC_VOIE_NEUTRALISEE
		if @vl_BAU_I = XDC_VOIE_BLOQUEE_NEUTRALISEE
			select @vl_BAU_I = XDC_VOIE_NEUTRALISEE

		/* recherche l'autre extremite de la zone */
		if @vl_Type = XZAEC_FMC_Basculement
			select 
				@vl_PRExtremite = localisation_PR_fin
			from EXP..FMC_BAS
			where numero = @vl_Evt and cle = @vl_Cle
		else 
			select
				@vl_PRExtremite = localisation_tete
			from EXP..FMC_TRH
			where numero = @vl_Evt and cle = @vl_Cle and
				horodate = @vl_HoroVal

		/* prise en compte de la configuration des voies de l'evt */
		if @@rowcount = 1 and
		   (@va_PR_in >= @vl_PRExtremite and @va_PR_in <= @vl_PREvt) or
		   (@va_PR_in <= @vl_PRExtremite and @va_PR_in >= @vl_PREvt)
		begin
			if @vl_VR = XDC_VOIE_SENS_INVERSE  or
		    	   @vl_VR = XDC_VOIE_NEUTRALISEE
			begin
				if @va_Sens_in = @vl_Sens and @va_VR_out != XDC_VOIE_NEUTRALISEE
					select @va_VR_out = @vl_VR
				else if @va_Sens_in != @vl_Sens and @va_VR_I_out != XDC_VOIE_NEUTRALISEE
					select @va_VR_I_out = @vl_VR
			end
			if @vl_VM2 = XDC_VOIE_SENS_INVERSE or
		    	   @vl_VM2 = XDC_VOIE_NEUTRALISEE 
			begin
				if @va_Sens_in = @vl_Sens and @va_VM2_out != XDC_VOIE_NEUTRALISEE
					select @va_VM2_out = @vl_VM2
				else if @va_Sens_in != @vl_Sens and @va_VM2_I_out != XDC_VOIE_NEUTRALISEE
					select @va_VM2_I_out = @vl_VM2
			end
			if @vl_VM1 = XDC_VOIE_SENS_INVERSE  or
		    	   @vl_VM1 = XDC_VOIE_NEUTRALISEE 
			begin
				if @va_Sens_in = @vl_Sens and @va_VM1_out != XDC_VOIE_NEUTRALISEE
					select @va_VM1_out = @vl_VM1
				else if @va_Sens_in != @vl_Sens and @va_VM1_I_out != XDC_VOIE_NEUTRALISEE
					select @va_VM1_I_out = @vl_VM1
			end
			if @vl_VL = XDC_VOIE_SENS_INVERSE  or
		    	   @vl_VL = XDC_VOIE_NEUTRALISEE 
			begin
				if @va_Sens_in = @vl_Sens and @va_VL_out != XDC_VOIE_NEUTRALISEE
					select @va_VL_out = @vl_VL
				else if @va_Sens_in != @vl_Sens and @va_VL_I_out != XDC_VOIE_NEUTRALISEE
					select @va_VL_I_out = @vl_VL
			end
			if @vl_BAU = XDC_VOIE_SENS_INVERSE  or
		    	   @vl_BAU = XDC_VOIE_NEUTRALISEE
			begin
				if @va_Sens_in = @vl_Sens and @va_BAU_out != XDC_VOIE_NEUTRALISEE
					select @va_BAU_out = @vl_BAU
				else if @va_Sens_in != @vl_Sens and @va_BAU_I_out != XDC_VOIE_NEUTRALISEE
					select @va_BAU_I_out = @vl_BAU
			end
			else if @vl_BAU = XDC_VOIE_SENS_NORMAL
			begin
				if @va_Sens_in = @vl_Sens and @va_BAU_out = XDC_VOIE_BAU_NORMALE
					select @va_BAU_out = @vl_BAU
				else if @va_Sens_in != @vl_Sens and @va_BAU_I_out = XDC_VOIE_BAU_NORMALE
					select @va_BAU_I_out = @vl_BAU
			end

			/* configuration des voies inverses de l'evenement */
			if @vl_VR_I = XDC_VOIE_SENS_INVERSE  or
		    	   @vl_VR_I = XDC_VOIE_NEUTRALISEE 
			begin
				if @va_Sens_in = @vl_Sens and @va_VR_I_out != XDC_VOIE_NEUTRALISEE
					select @va_VR_I_out = @vl_VR_I
				else if @va_Sens_in != @vl_Sens and @va_VR_out != XDC_VOIE_NEUTRALISEE
					select @va_VR_out = @vl_VR_I
			end
			if @vl_VM2_I = XDC_VOIE_SENS_INVERSE or
		    	   @vl_VM2_I = XDC_VOIE_NEUTRALISEE
			begin
				if @va_Sens_in = @vl_Sens and @va_VM2_I_out != XDC_VOIE_NEUTRALISEE
					select @va_VM2_I_out = @vl_VM2_I
				else if @va_Sens_in != @vl_Sens and @va_VM2_out != XDC_VOIE_NEUTRALISEE
					select @va_VM2_out = @vl_VM2_I
			end
			if @vl_VM1_I = XDC_VOIE_SENS_INVERSE  or
		    	   @vl_VM1_I = XDC_VOIE_NEUTRALISEE
			begin
				if @va_Sens_in = @vl_Sens and @va_VM1_I_out != XDC_VOIE_NEUTRALISEE
					select @va_VM1_I_out = @vl_VM1_I
				else if @va_Sens_in != @vl_Sens and @va_VM1_out != XDC_VOIE_NEUTRALISEE
					select @va_VM1_out = @vl_VM1_I
			end
			if @vl_VL_I = XDC_VOIE_SENS_INVERSE  or
		    	   @vl_VL_I = XDC_VOIE_NEUTRALISEE 
			begin
				if @va_Sens_in = @vl_Sens and @va_VL_I_out != XDC_VOIE_NEUTRALISEE
					select @va_VL_I_out = @vl_VL_I
				else if @va_Sens_in != @vl_Sens and @va_VL_out != XDC_VOIE_NEUTRALISEE
					select @va_VL_out = @vl_VL_I
			end
			if @vl_BAU_I = XDC_VOIE_SENS_INVERSE  or
		    	   @vl_BAU_I = XDC_VOIE_NEUTRALISEE 
			begin
				if @va_Sens_in = @vl_Sens and @va_BAU_I_out != XDC_VOIE_NEUTRALISEE
					select @va_BAU_I_out = @vl_BAU_I
				else if @va_Sens_in != @vl_Sens and @va_BAU_out != XDC_VOIE_NEUTRALISEE
					select @va_BAU_out = @vl_BAU_I
			end
			else if @vl_BAU_I = XDC_VOIE_SENS_NORMAL
			begin
				if @va_Sens_in = @vl_Sens and @va_BAU_I_out = XDC_VOIE_BAU_NORMALE
					select @va_BAU_I_out = @vl_BAU_I
				else if @va_Sens_in != @vl_Sens and @va_BAU_out = XDC_VOIE_BAU_NORMALE
					select @va_BAU_out = @vl_BAU_I
			end
		end

		/* passage a l'evenement en cours suivant */
		fetch Pointeur_evts_en_cours into @vl_Evt, @vl_Cle, @vl_Type, 
			@vl_HoroVal, @vl_PREvt, @vl_Sens,
			@vl_VR, @vl_VM2, @vl_VM1, @vl_VL, @vl_BAU,
			@vl_VR_I, @vl_VM2_I, @vl_VM1_I, @vl_VL_I, @vl_BAU_I
	end

	close Pointeur_evts_en_cours

	return XDC_OK
go
