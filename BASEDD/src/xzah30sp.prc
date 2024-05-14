/*E*/
/*  Fichier : $Id: xzah30sp.prc,v 1.19 2017/03/02 17:56:44 gesconf Exp $      Release : $Revision: 1.19 $        Date : $Date: 2017/03/02 17:56:44 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah30sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 
* Animation des synoptiques
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.  20/02/95        : Creation
* C.T.  30/03/95        : Modif les types d'objet en cas
*                         d'accident (V 1.2)
* C.T.  02/04/95        : Ajout la modification des actions
*                         liees a l'evenement (V 1.3)
* C.T.  05/04/95        : Ajout de l'objet meteo (V 1.4)
* C.T.  02/10/96        : retypage de fmc (FMC3) (V 1.5)
* CT	23/12/96	: longueur dans operation escargot (FMC7) (DEM/1369) 1.6
* JMG	21/05/97	: appel a XZAH;16 uniquement si basculement pas consequence de travaux (DEM/1394) 1.7
* JMG	04/11/97	: insertion de la fmc dans FMC_ADA au CI (dem/strada) 1.8
* PN	06/01/98	: appel a XZAH;16 si FMC finie pour tous les basculement (DEM/1394) 1.9
* PN    19/10/98	: suppression satir dem/1700 1.10
* PN    24/11/98	: intergestionnaire dem/1724 1.11
* PN	04/05/99	: ajout icone manquantes v1.12
* CHI   16/03/05	: appel a XZAH34 pour gestion FMC bouchon par segment v1.13
* JPL	11/06/07	: Prise en compte Fmc Basculement pour Travaux (DEM 647) 1.14
* JPL	06/07/07	: Cas Basculement pour Travaux = cas travaux et non Basculement + travaux 1.15
* JMG	03/12/08	: ajout validation dans FMC_ACC 1.16 1.17 DEM/852
* JPL	24/08/10	: Prise en compte Fmc Zone de stockage (DEM 942); Utilise FMC_HIS_DER pour perfos 1.18
* PNI	2/3/2017	: PR=1 si PR=0 afin de pouvoir trouver une portion pour l'animation FI4030 1.19 DEM1219
* PNI	8/1/2024	: Purge des evt cloturés en fin de proc SAE-566
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare l'animation IHM
*
* Code retour
* 
* n�ant
*
* Conditions d'utilisation
* base EXP
---------------------------------------------------*/

use PRC
go

create procedure XZAH;30
as

/*A D�claration des variables locales */

	declare @vl_NumEvt 	int,
		@vl_CleEvt 	tinyint,
		@vl_NumCause	int,
		@vl_CleCause	tinyint,
		@vl_Autoroute 	T_AUTOROUTE,
		@vl_PR		T_PR,
		@vl_Sens	T_SENS,
		@vl_TypPtCar	tinyint,
		@vl_PtCar	tinyint,
		@vl_PosPtCar	tinyint,
		@vl_Type	smallint,
		@vl_Type_FMC	smallint,
		@vl_Type_Cause	smallint,
		@vl_TypeAnterieur	smallint,
		@vl_TypeMeteo	tinyint,
		@vl_Horodeb	datetime,
		@vl_Horofin	datetime,
		@vl_Horoval	datetime,
		@vl_VR		tinyint,
		@vl_VM2		tinyint,
		@vl_VM1		tinyint,
		@vl_VL		tinyint,
		@vl_BAU		tinyint,
		@vl_VR_I	tinyint,
		@vl_VM2_I	tinyint,
		@vl_VM1_I	tinyint,
		@vl_VL_I	tinyint,
		@vl_BAU_I	tinyint,
		@vl_Voie	tinyint,
		@vl_Longueur	int,
		@vl_NbValidation int,
		@vl_NbValidationActuel int,
		@vl_PresenceRemorque	bit,
		@vl_PresenceCar		bit,
		@vl_PresenceMoto	bit,
		@vl_PresenceVL		bit,
		@vl_PresencePL		bit
		
		
while (select count(*) from EXP..FMC_SYN) > 0
begin
/*A r�cup�ration la premiere valeur ins�r�e */
	set rowcount 1

	select @vl_NumEvt = evenement, @vl_CleEvt = cle 
	from EXP..FMC_SYN

/*A memoriser le nombre de validations faite pour cet evenement */
	select @vl_NbValidation = count(*)
	from EXP..FMC_SYN
	where evenement = @vl_NumEvt and cle = @vl_CleEvt

	set rowcount 0

/*A rechercher l'historique de cet evenement */
	select	@vl_TypeAnterieur=type_anterieur,
		@vl_Horoval=horodate_validation,
		@vl_Autoroute=autoroute,
		@vl_PR=PR,
		@vl_Sens=sens,
		@vl_TypPtCar=point_caracteristique,
		@vl_PtCar=numero_point_caracteristique,
		@vl_PosPtCar=position_sur_aire_ou_echangeur,
		@vl_VR=VR,
		@vl_VM2=VM2,
		@vl_VM1=VM1,
		@vl_VL=VL,
		@vl_BAU=BAU,
		@vl_VR_I=VR_I,
		@vl_VM2_I=VM2_I,
		@vl_VM1_I=VM1_I,
		@vl_VL_I=VL_I,
		@vl_BAU_I=BAU_I,
		@vl_Longueur=longueur
		
	from EXP..FMC_HIS_DER
	where	numero = @vl_NumEvt and cle = @vl_CleEvt

	if (@vl_PR=0)
	select @vl_PR=1

/*A r�cup�ration des valeurs de FMC_GEN */
	
	select	@vl_Type=type,
		@vl_Horodeb=debut,
		@vl_Horofin=fin,
		@vl_NumCause=cause,
		@vl_CleCause=cle_cause
	from EXP..FMC_GEN
	where	@vl_NumEvt=numero and
		@vl_CleEvt=cle

/*mets de cote le type de la fmc*/
	select @vl_Type_FMC =   @vl_Type

/*A si il s'agit d'une fin, on v�rifie que l'objet est supprim� du synoptique */

	if @vl_Horofin<>null
	begin

/*B	cas PosteOperateur,
	TeteBouchon : on ne fait rien ! */

		if 	@vl_Type=XZAEC_FMC_PosteOperateur or
			@vl_Type=XZAEC_FMC_TeteBouchon
		begin
			select @vl_CleEvt=@vl_CleEvt
		end
		else

/*B	cas Basculement */

		if @vl_Type=XZAEC_FMC_Basculement
		begin
				exec XZAH;16	@va_NumEvt_in=@vl_NumEvt,
						@va_CleEvt_in=@vl_CleEvt,
						@va_Suppr_in=XDC_VRAI -- suppression=vrai
		end
		else

/*B	cas Travaux */

		if @vl_Type in (XZAEC_FMC_Travaux, XZAEC_FMC_ZoneStockage)
		begin
			exec XZAH;15	@va_NumEvt_in=@vl_NumEvt,
						@va_CleEvt_in=@vl_CleEvt,
						@va_Suppr_in=XDC_VRAI -- suppression=vrai
		end
		else

/*B	cas Basculement pour Travaux */

		if @vl_Type=XZAEC_FMC_BasculTravaux
		begin
			exec XZAH;15	@va_NumEvt_in=@vl_NumEvt,
						@va_CleEvt_in=@vl_CleEvt,
						@va_Suppr_in=XDC_VRAI -- suppression=vrai

			exec XZAH;16	@va_NumEvt_in=@vl_NumEvt,
						@va_CleEvt_in=@vl_CleEvt,
						@va_Suppr_in=XDC_VRAI -- suppression=vrai
		end
		else

/*B	cas QueueBouchon */

		if @vl_Type=XZAEC_FMC_QueueBouchon
			begin
			exec XZAH;14	@va_NumEvt_in=@vl_NumEvt,
						@va_CleEvt_in=@vl_CleEvt,
						@va_TeteQueue_in=XZAHC_Queue,
						@va_Suppr_in=XDC_VRAI -- suppression=vrai
		
			exec XZAH34 		@vl_NumEvt,
						@vl_CleEvt,
						XZAHC_Queue,
						@vl_Autoroute,
						@vl_PR,
						@vl_Sens,
						@vl_VR,
						@vl_VM2,
						@vl_VM1,
						@vl_VL,
						@vl_BAU,
						@vl_VR_I,
						@vl_VM2_I,
						@vl_VM1_I,
						@vl_VL_I,
						@vl_BAU_I,
						XDC_VRAI -- suppression=vrai
			end	
		else

/*B	cas Accident, Contresens, VehArrete, VehPanne, VehFeu, Cirque, ConvoiExc,
	ConvoiMil, VehLent, Manifestation, GreveESCOTA, Escargot, IncSsConces,
	FermetureEch, Eboulement, Incendie, Inondation, Obstacle, ChausseDeter,
	FlaqueHuile, AnimalErrant, FrontiereFermee, AltercationUsager, Agression,
	MalaiseUsager, AireFermee, Salage, HoldUpVol, PietonChaussee, Autre, meteo */

		begin
		
/*B		Suppression objet mobile */

			exec XZAH;12	@va_NumEvt_in=@vl_NumEvt,
						@va_CleEvt_in=@vl_CleEvt,
						@va_Suppr_in=XDC_VRAI	

/*B		Suppression zone escargot */
			if @vl_Type=XZAEC_FMC_Escargot
				exec XZAH;31	@va_NumEvt_in=@vl_NumEvt,
						@va_CleEvt_in=@vl_CleEvt,
						@va_Suppr_in=XDC_VRAI -- suppression=vrai

/*B		Suppression rabattement */

			exec XZAH;13	@va_NumEvt_in=@vl_NumEvt,
						@va_CleEvt_in=@vl_CleEvt,
						@va_Suppr_in=XDC_VRAI	
		end
	end

/*A si il ne s'agit pas d'une fin et si le d�but est renseign� ou s'il s'agit
    de travaux pr�vus et � traiter, on anime l'objet */

	else
	if	@vl_Horodeb<>null or
		@vl_Type in (XZAEC_FMC_Travaux, XZAEC_FMC_BasculTravaux)
	begin

/*B	cas  PosteOperateur, 
	: on ne fait rien ! */

		if 	@vl_Type=XZAEC_FMC_PosteOperateur 
		begin
			select @vl_CleEvt=@vl_CleEvt
		end
		else

/*B	cas Basculement */

		if @vl_Type=XZAEC_FMC_Basculement
		begin
                        if @vl_NumCause!=NULL
			begin
				select @vl_Type_Cause = type 
				from EXP..FMC_GEN
				where numero=@vl_NumCause
				and cle=@vl_CleCause
			end

			if @vl_NumCause=NULL or
			   @vl_Type_Cause != XZAEC_FMC_Travaux
			begin
				exec XZAH;16	@vl_NumEvt,
						@vl_CleEvt,
						@vl_Autoroute,
						@vl_PR,
						@vl_Sens,
						@vl_VR,
						@vl_VM2,
						@vl_VM1,
						@vl_VL,
						@vl_BAU,
						@vl_VR_I,
						@vl_VM2_I,
						@vl_VM1_I,
						@vl_VL_I,
						@vl_BAU_I,
						XDC_FAUX -- suppression=faux
			end
		end
		else

/*B	cas Travaux */

		if @vl_Type in (XZAEC_FMC_Travaux, XZAEC_FMC_ZoneStockage, XZAEC_FMC_BasculTravaux)
			exec XZAH;15	@vl_NumEvt,
						@vl_CleEvt,
						@vl_Type_FMC,
						@vl_Autoroute,
						@vl_PR,
						@vl_Sens,
						@vl_TypPtCar,
						@vl_PtCar,
						@vl_PosPtCar,
						@vl_Horoval,
						@vl_Horodeb,
						@vl_VR,
						@vl_VM2,
						@vl_VM1,
						@vl_VL,
						@vl_BAU,
						@vl_VR_I,
						@vl_VM2_I,
						@vl_VM1_I,
						@vl_VL_I,
						@vl_BAU_I,
						XDC_FAUX -- suppression=faux

		else

/*B	cas TeteBouchon */

		if @vl_Type=XZAEC_FMC_TeteBouchon
			begin
			exec XZAH;14	@vl_NumEvt,
						@vl_CleEvt,
						XZAHC_Tete,
						@vl_Autoroute,
						@vl_PR,
						@vl_Sens,
						XDC_FAUX -- suppression=faux
			
			exec XZAH34 	@vl_NumEvt,
						@vl_CleEvt,
						XZAHC_Tete,
						@vl_Autoroute,
						@vl_PR,
						@vl_Sens,
						@vl_VR,
						@vl_VM2,
						@vl_VM1,
						@vl_VL,
						@vl_BAU,
						@vl_VR_I,
						@vl_VM2_I,
						@vl_VM1_I,
						@vl_VL_I,
						@vl_BAU_I,
						XDC_FAUX -- suppression=faux
		end	
		else

/*B	cas QueueBouchon */

		if @vl_Type=XZAEC_FMC_QueueBouchon
		begin
			exec XZAH;14	@vl_NumEvt,
						@vl_CleEvt,
						XZAHC_Queue,
						@vl_Autoroute,
						@vl_PR,
						@vl_Sens,
						XDC_FAUX -- suppression=faux
			
			exec XZAH34 	@vl_NumEvt,
						@vl_CleEvt,
						XZAHC_Queue,
						@vl_Autoroute,
						@vl_PR,
						@vl_Sens,
						@vl_VR,
						@vl_VM2,
						@vl_VM1,
						@vl_VL,
						@vl_BAU,
						@vl_VR_I,
						@vl_VM2_I,
						@vl_VM1_I,
						@vl_VL_I,
						@vl_BAU_I,
						XDC_FAUX -- suppression=faux
		end
		else

/*B	cas Accident, Contresens, VehArrete, VehPanne, VehFeu, Cirque, ConvoiExc,
	ConvoiMil, VehLent, Manifestation, GreveESCOTA, Escargot, IncSsConces,
	FermetureEch, Eboulement, Incendie, Inondation, Obstacle, ChausseDeter,
	FlaqueHuile, AnimalErrant, FrontiereFermee, AltercationUsager, Agression,
	MalaiseUsager, AireFermee, Salage, HoldUpVol, PietonChaussee, Autre, meteo */

		begin
		
						
/*B		Animation rabattement */
					
			exec XZAH;13	@vl_NumEvt,
						@vl_CleEvt,
						@vl_Autoroute,
						@vl_PR,
						@vl_Sens,
						XZAHC_Ponctuel,
						@vl_VR,
						@vl_VM2,
						@vl_VM1,
						@vl_VL,
						@vl_BAU,
						@vl_VR_I,
						@vl_VM2_I,
						@vl_VM1_I,
						@vl_VL_I,
						@vl_BAU_I,
						XDC_FAUX -- suppression=faux

/*B		Animation objet mobile */

/*B		Recherche de la voie (la plus � gauche bloquee) */

			if	@vl_VR=XDC_VOIE_BLOQUEE or
				@vl_VR=XDC_VOIE_BLOQUEE_NEUTRALISEE
			select	@vl_Voie=6
			else
			if	@vl_VM2=XDC_VOIE_BLOQUEE or
				@vl_VM2=XDC_VOIE_BLOQUEE_NEUTRALISEE
			select	@vl_Voie=5
			else
			if	@vl_VM1=XDC_VOIE_BLOQUEE or
				@vl_VM1=XDC_VOIE_BLOQUEE_NEUTRALISEE
			begin
				if @vl_VM2=XDC_VOIE_INEXISTANTE
				select @vl_Voie=5
				else
				select @vl_Voie=4
			end
			else
			if	@vl_VL=XDC_VOIE_BLOQUEE or
				@vl_VL=XDC_VOIE_BLOQUEE_NEUTRALISEE
			begin
				if @vl_VR=XDC_VOIE_INEXISTANTE
				select @vl_Voie=6
				else
				if @vl_VM1=XDC_VOIE_INEXISTANTE
				select @vl_Voie=5
				else
				if @vl_VM2=XDC_VOIE_INEXISTANTE
				select @vl_Voie=4
				else
				select @vl_Voie=3
			end
			else
			if	@vl_BAU=XDC_VOIE_BLOQUEE or
				@vl_BAU=XDC_VOIE_BLOQUEE_NEUTRALISEE
			begin
				if @vl_VR=XDC_VOIE_INEXISTANTE
				select @vl_Voie=5
				else
				if @vl_VM1=XDC_VOIE_INEXISTANTE
				select @vl_Voie=4
				else
				if @vl_VM2=XDC_VOIE_INEXISTANTE
				select @vl_Voie=3
				else
				select @vl_Voie=2
			end
			else
			select @vl_Voie=XZAHC_VoieInconnue

			if @vl_Type = XZAEC_FMC_Accident
			begin
				/* recherche des types de vehicules */
				/* impliques                        */
				select
					@vl_PresenceVL = isnull(nombre_de_vl,0),
					@vl_PresencePL = isnull(nombre_de_pl,0),
					@vl_PresenceRemorque = isnull(nombre_de_remorques,0),
					@vl_PresenceCar = isnull(nombre_de_car,0),
					@vl_PresenceMoto = isnull(nombre_de_moto,0)
				from EXP..FMC_ACC
				where numero = @vl_NumEvt and cle = @vl_CleEvt and zz >1
				group by numero,cle
				having validation=max(validation)

				select @vl_Type = @vl_PresenceVL * XZAHC_Accident_VL +
						@vl_PresencePL * XZAHC_Accident_PL +
						@vl_PresenceRemorque * XZAHC_Accident_Remorque +
						@vl_PresenceCar * XZAHC_Accident_Car +
						@vl_PresenceMoto * XZAHC_Accident_Moto +
						XZAHC_Accident
			end
			else if @vl_Type =XZAEC_FMC_Meteo
			begin
				/* recherche le type de meteo */
				select
					@vl_TypeMeteo = type_de_perturbation
				from EXP..FMC_MET
				where numero = @vl_NumEvt and cle = @vl_CleEvt

				/* determination du type objet a animer  */
				/* (le type de meteo donne l'offset pour */
				/*  le calcul de l'objet a animer )      */
				select @vl_Type = @vl_TypeMeteo + XZAHC_Meteo
			end

			/* cas de retypage de fmc : supprimer d'abord l'objet */
			if @vl_Type != @vl_TypeAnterieur
				exec XZAH;12	@va_NumEvt_in=@vl_NumEvt,
						@va_CleEvt_in=@vl_CleEvt,
						@va_Suppr_in=XDC_VRAI	

			/* cas de retypage de fmc : supprimer la zone escargot */
			if @vl_TypeAnterieur=XZAEC_FMC_Escargot
				exec XZAH;31	@va_NumEvt_in=@vl_NumEvt,
						@va_CleEvt_in=@vl_CleEvt,
						@va_Suppr_in=XDC_VRAI -- suppression=vrai

			exec XZAH;12	@vl_NumEvt,
					@vl_CleEvt,
					null, -- action
					null, -- cle action
					@vl_Type,
					@vl_Autoroute,
					@vl_PR,
					@vl_Sens,
					@vl_Voie,
					@vl_TypPtCar,
					@vl_PtCar,
					@vl_PosPtCar,
					XDC_FAUX -- suppression=faux

			/* animation de la zone escargot */
			if @vl_Type=XZAEC_FMC_Escargot
				exec XZAH;31	@vl_NumEvt,
						@vl_CleEvt,
						@vl_Longueur,
						@vl_Autoroute,
						@vl_PR,
						@vl_Sens,
						XDC_FAUX -- suppression=faux
		end
	end

	/*A animation des voies radt et dai */
	exec XZAH;20 @vl_NumEvt,
		@vl_CleEvt,
		@vl_Horoval,
		@vl_Autoroute,
		@vl_PR,
		@vl_Sens,
		@vl_VL,
		@vl_VM1,
		@vl_VM2,
		@vl_VR,
		@vl_BAU,
		@vl_VL_I,
		@vl_VM1_I,
		@vl_VM2_I,
		@vl_VR_I,
		@vl_BAU_I,
		XDC_CI -- ne sert a rien !!

	
#ifdef CI
	
	/*verifie qu'il existe une action "export intergestionnaire"
	 associee a la fmc pour mettre son numero et sa cle dans FMC_ADA */
	if exists ( select * from EXP..ACT_GEN
		    where type = XDC_ACT_EXP 
		    and   evenement = @vl_NumEvt
		    and   cle = @vl_CleEvt
		    and   heure_lancement <> null 
		    and   heure_fin = null)
	begin
		/*mise de la fmc dans la liste de strada*/
		if not exists (	select * from EXP..FMC_ADA
			where evenement = @vl_NumEvt
			and   cle = @vl_CleEvt)
		begin
			insert EXP..FMC_ADA (evenement, cle, type)
			values (@vl_NumEvt,@vl_CleEvt, @vl_Type_FMC)
		end
	end
#endif	

/*A rechercher le nombre de validations faite pour cet evenement maintenant */
	select @vl_NbValidationActuel = count(*)
	from EXP..FMC_SYN
	where evenement = @vl_NumEvt and cle = @vl_CleEvt

/*A Supprimer les indicateurs de modification de synoptique pour cet evenement */
	if @vl_NbValidationActuel = @vl_NbValidation
		delete EXP..FMC_SYN
		where evenement = @vl_NumEvt and cle = @vl_CleEvt

/*A Modification des actions liees a l'evenement */
	exec PRC..XZAH;29	@vl_NumEvt,
			@vl_CleEvt,
			@vl_Horofin,
			@vl_Autoroute,
			@vl_PR,
			@vl_Sens,
			@vl_TypPtCar,
			@vl_PtCar,
			@vl_PosPtCar,
			@vl_VL,
			@vl_VM1,
			@vl_VM2,
			@vl_VR,
			@vl_BAU
end

/* SAE-566 : purge des tables pour evt cloturés */
update EXP..SYN_OBJ_POR set ordre=XDC_SUPPR where evenement not in (select numero from EXP..FMC_GEN where cloture is null) 
update EXP..SYN_OBJ_SEG set ordre=XDC_SUPPR where evenement not in (select numero from EXP..FMC_GEN where cloture is null) 
update EXP..SYN_BAS_SEG set ordre=XDC_SUPPR where evenement_bou not in (select numero from EXP..FMC_GEN where cloture is null) 
update EXP..SYN_BOU_POR set ordre=XDC_SUPPR where evenement not in (select numero from EXP..FMC_GEN where cloture is null) 
update EXP..SYN_BOU_SEG set ordre=XDC_SUPPR where evenement not in (select numero from EXP..FMC_GEN where cloture is null) 
update EXP..SYN_INV_SEG set ordre=XDC_SUPPR where evenement not in (select numero from EXP..FMC_GEN where cloture is null) 
update EXP..SYN_NEU_SEG set ordre=XDC_SUPPR where evenement not in (select numero from EXP..FMC_GEN where cloture is null) 
update EXP..SYN_RAB_SEG set ordre=XDC_SUPPR where evenement not in (select numero from EXP..FMC_GEN where cloture is null) 

return XDC_OK
go
