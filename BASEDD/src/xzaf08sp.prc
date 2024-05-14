/*E*/
/*  Fichier : $Id: xzaf08sp.prc,v 1.1 1995/01/17 16:30:57 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/17 16:30:57 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzaf08sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm pour la formation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	10/01/95	: Creation 
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAF08_Animer_Voies_RADT_Inversees
* 
* Arguments en entree
* 
* Arguments en sortie
* 
* Liste retournee :
* XDY_Entier	evenement,
* XDY_Octet 	cle,
* XDY_Mot	portion,
* XDY_Entier	PR,
* XDY_Octet	sens,
* XDY_Octet	voie,
* XDY_Entier	translation
* XDY_Entier	extenxion
* XDY_Mot	station
* XDY_Sens	sens_station
*
* Service rendu
* Prepare l'animation IHM pour la formation
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

create procedure  XZAF;08
as

/*A DÅclaration des variables locales */
declare @vl_NumEvt		int,
	@vl_CleEvt		tinyint,
	@vl_Type		smallint,
	@vl_Autoroute		T_AUTOROUTE,
	@vl_PRdeb		T_PR,
	@vl_PRfin		T_PR,
	@vl_Sens		T_SENS,
	@vl_Horodate		datetime,
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
		

	create table #SYN_INV_SEG
	(
		evenement	int,
		cle		tinyint,
		portion		smallint,
		PR		int,
		sens		tinyint,
		voie		tinyint,
		translation	int,
		extension	int,
		station		smallint,
		sens_station	tinyint
	)

	/*A Recherche des evenements en cours a la date precisee */
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
		type = XZAEC_FMC_Basculement and
		(sens = XDC_SENS_NORD or sens = XDC_SENS_SUD)

	open Pointeur_evt
	fetch Pointeur_evt into @vl_NumEvt, @vl_CleEvt, @vl_Type,
				@vl_Autoroute, @vl_PRdeb, @vl_Sens,
				@vl_VR, @vl_VM2, @vl_VM1, @vl_VL,
				@vl_BAU, @vl_VR_I, @vl_VM2_I, @vl_VM1_I, 
				@vl_VL_I, @vl_BAU_I
	while @@sqlstatus = 0
	begin
		/* recherche le PR de fin */
       		select @vl_PRfin=localisation_PR_fin
		from HIS..FMC_BAS
		where numero=@vl_NumEvt and cle=@vl_CleEvt

		if @@rowcount<>1 return XDC_NOK

		/* calcul des voies radt inversees */
		exec @vl_Status = XZAF;09 @vl_NumEvt, @vl_CleEvt, 
				@vl_Autoroute, @vl_PRdeb, @vl_PRfin, @vl_Sens,
				@vl_VR, @vl_VM2, @vl_VM1, @vl_VL,
				@vl_BAU, @vl_VR_I, @vl_VM2_I, @vl_VM1_I, 
				@vl_VL_I, @vl_BAU_I

		if @vl_Status = null
			return XDC_PRC_INC
		else if @vl_Status != XDC_OK
			return XDC_NOK

		/* passage a l'evenement suivant */
		fetch Pointeur_evt into @vl_NumEvt, @vl_CleEvt, @vl_Type,
				@vl_Autoroute, @vl_PRdeb, @vl_Sens,
				@vl_VR, @vl_VM2, @vl_VM1, @vl_VL,
				@vl_BAU, @vl_VR_I, @vl_VM2_I, @vl_VM1_I, 
				@vl_VL_I, @vl_BAU_I
	end
	close Pointeur_evt

	/* retourner la liste des voies neutalisees */
	select 
		evenement,
		cle,
		portion,
		PR,
		sens,
		voie,
		translation,
		extension,
		station,
		sens_station
	from #SYN_INV_SEG

	return XDC_OK
go
