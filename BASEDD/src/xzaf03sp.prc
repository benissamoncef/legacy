/*E*/
/*  Fichier : $Id: xzaf03sp.prc,v 1.1 1995/01/17 16:31:30 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/17 16:31:30 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzaf03sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm pour la formation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	11/01/95	: Creation 
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAF03_Animer_Neut_Voies
* 
* Arguments en entree
* 
* Arguments en sortie
* 
* Liste retournee :
* XDY_Entier	evenement,
* XDY_Octet 	cle,
* XDY_Octet 	debut_fin,
* XDY_Mot	portion,
* XDY_Entier	translation
* XDY_Voies	nvoie1,
* XDY_Voies	vvoie1,
* XDY_Voies	nvoie2,
* XDY_Voies	vvoie2,
* XDY_Voies	nvoie3,
* XDY_Voies	vvoie3,
* XDY_Voies	nvoie4,
* XDY_Voies	vvoie4,
* XDY_Voies	nvoie5,
* XDY_Voies	vvoie5,
* XDY_Voies	nvoie6,
* XDY_Voies	vvoie6,
* XDY_Voies	nvoie7,
* XDY_Voies	vvoie7,
* XDY_Voies	nvoie8,
* XDY_Voies	vvoie8,
* XDY_Voies	nvoie9,
* XDY_Voies	vvoie9,
* XDY_Voies	nvoie10,
* XDY_Voies	vvoie10
*
* Service rendu
* Retourne la liste des voies neutralisees pour
* l'animation IHM pour la formation
*
* Code retour
* XDC_OK
* XDC_NOK	
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
*
---------------------------------------------------*/

use PRC
go

create procedure  XZAF;03
as
/*A DÅclaration des variables locales */
declare @vl_NumEvt		int,
	@vl_CleEvt		tinyint,
	@vl_Type		smallint,
	@vl_Autoroute		T_AUTOROUTE,
	@vl_PR			T_PR,
	@vl_PRTete		T_PR,
	@vl_Sens		T_SENS,
	@vl_DebutFin 		tinyint,
	@vl_DebutFinTete	tinyint,
	@vl_HoroDebut		datetime,
	@vl_VR			T_VOIE,
	@vl_VM2			T_VOIE,
	@vl_VM1			T_VOIE,
	@vl_VL			T_VOIE,
	@vl_BAU			T_VOIE,
	@vl_VR_I		T_VOIE,
	@vl_VM2_I		T_VOIE,
	@vl_VM1_I		T_VOIE,
	@vl_VL_I		T_VOIE,
	@vl_BAU_I		T_VOIE,
	@vl_Status		int

	create table #SYN_RAB_SEG
	(
		evenement		int,
		cle			tinyint,
		debut_fin		tinyint,
		portion			smallint,
		translation		int,
		nvoie1			tinyint,
		vvoie1			tinyint,
		nvoie2			tinyint,
		vvoie2			tinyint,
		nvoie3			tinyint,
		vvoie3			tinyint,
		nvoie4			tinyint,
		vvoie4			tinyint,
		nvoie5			tinyint,
		vvoie5			tinyint,
		nvoie6			tinyint,
		vvoie6			tinyint,
		nvoie7			tinyint,
		vvoie7			tinyint,
		nvoie8			tinyint,
		vvoie8			tinyint,
		nvoie9			tinyint,
		vvoie9			tinyint,
		nvoie10			tinyint,
		vvoie10			tinyint
	)

	/*A Recherche des evenements en cours a la date precisee */
	/*A sur la section courante */
	/*  cas Accident, Contresens, VehArrete, VehPanne, VehFeu, 
	    Cirque, ConvoiExc, ConvoiMil, VehLent, Manifestation,
	    GreveESCOTA, Escargot, IncSsConces, FermetureEch,
	    Eboulement, Incendie, Inondation, Obstacle,
	    ChausseDeter, FlaqueHuile, AnimalErrant, FrontiereFermee,
	    AltercationUsager, Agression,MalaiseUsager, AireFermee,
	    Salage, HoldUpVol, PietonChaussee, Autre */
	declare Pointeur_evt cursor
	for select 
		numero,
		cle,
		type,
		autoroute,
		PR,
		sens,
		VR,
		VM2,
		VM1,
		VL,
		BAU,
		VR_I,
		VM2_I,
		VM1_I,
		VL_I,
		BAU_I
	from EVT_FRM
	where	spid = @@spid and 
		type != XZAEC_FMC_QueueBouchon and
		type != XZAEC_FMC_TeteBouchon and
		(sens = XDC_SENS_SUD or sens = XDC_SENS_NORD)

	open Pointeur_evt
	fetch Pointeur_evt into @vl_NumEvt, @vl_CleEvt, @vl_Type,
			@vl_Autoroute, @vl_PR, @vl_Sens, @vl_VR,
			@vl_VM2, @vl_VM1, @vl_VL, @vl_BAU,
			@vl_VR_I, @vl_VM2_I, @vl_VM1_I, @vl_VL_I, @vl_BAU_I
	while @@sqlstatus = 0
	begin
		/*B Test des informations nulles */
		if	@vl_Autoroute=null or
			@vl_PR=null or
			@vl_Sens=null or
			@vl_VR=null or 
			@vl_VM2=null or
			@vl_VM1=null or
			@vl_VL=null or
			@vl_BAU=null or
			@vl_VR_I=null or
			@vl_VM2_I=null or
			@vl_VM1_I=null or
			@vl_VL_I=null or
			@vl_BAU_I=null
		return XDC_NOK
	
		/* */
		if @vl_Type = XZAEC_FMC_Basculement
		begin
			/* recherche le PR de tete */
			select
				@vl_PRTete = localisation_PR_fin
			from HIS..FMC_BAS
			where numero = @vl_NumEvt and cle = @vl_CleEvt

			select	@vl_DebutFin = XZAHC_Debut,
				@vl_DebutFinTete = XZAHC_Fin
 
			/*B calcul les voies neutralisees pour l'evenement pour rabattement */
			exec @vl_Status = XZAF;04 @vl_NumEvt, @vl_CleEvt, @vl_Autoroute, 
					@vl_PRTete, @vl_Sens, @vl_DebutFinTete, @vl_VR,
					@vl_VM2, @vl_VM1, @vl_VL, @vl_BAU,
					@vl_VR_I, @vl_VM2_I, @vl_VM1_I, @vl_VL_I, @vl_BAU_I
		end
		else 
			select	@vl_DebutFin = XZAHC_Ponctuel

		/*B calcul les voies neutralisees pour l'evenement pour rabattement */
		exec @vl_Status = XZAF;04 @vl_NumEvt, @vl_CleEvt, @vl_Autoroute, 
					@vl_PR, @vl_Sens, @vl_DebutFin, @vl_VR,
					@vl_VM2, @vl_VM1, @vl_VL, @vl_BAU,
					@vl_VR_I, @vl_VM2_I, @vl_VM1_I, @vl_VL_I, @vl_BAU_I

		if @vl_Status = null
			return XDC_PRC_INC
		else if @vl_Status != XDC_OK
			return XDC_NOK

		/* passage a l'evenement suivant */
		fetch Pointeur_evt into @vl_NumEvt, @vl_CleEvt, @vl_Type,
			@vl_Autoroute, @vl_PR, @vl_Sens, @vl_VR,
			@vl_VM2, @vl_VM1, @vl_VL, @vl_BAU,
			@vl_VR_I, @vl_VM2_I, @vl_VM1_I, @vl_VL_I, @vl_BAU_I
	end
	close Pointeur_evt

	/* retourner la liste des voies neutralisees */
	select 
		evenement,
		cle,
		debut_fin,
		portion,
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
		vvoie10
	from #SYN_RAB_SEG

	return XDC_OK
go
