/*E*/
/*  Fichier : $Id: xzaf02sp.prc,v 1.8 2000/07/21 17:07:47 gesconf Exp $      Release : $Revision: 1.8 $        Date : $Date: 2000/07/21 17:07:47 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzaf02sp.prc
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
* C.T.	31/01/95	: Modif filtrage des actions (V 1.2) 
* C.T.	07/02/95	: Ajout dans la clause le spid pour EVT_FRM (V 1.3) 
* C.T.	25/03/95	: Retourner le nom de l'echangeur pour les objets
*                         sur echangeur (V 1.4)
* C.T.	30/03/95	: Ajout de la zone totale de travaux mobile (V 1.5)
*                         Modif les types d'objet en cas
*                         d'accident 
* C.T.	05/04/95	: Ajout des objets intervention autre et meteo (V 1.6)
* C.T.	29/05/95	: Suppression du traitement du balisage (V 1.7)
* JMG	21/09/99	: autoroute de ACT_RMQ et ACT_FRG est dans la table
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAF02_Animer_Objet_Mobile
* 
* Arguments en entree
* XDY_Horodate  va_Horodate_in
* 
* Arguments en sortie
* 
* Liste retournee :
* XDY_Entier	evenement,
* XDY_Octet 	cle,
* XDY_Mot	portion,
* XDY_Entier	actio,
* XDY_Entier 	cle_action,
* XDY_Octet	type_objet,
* XDY_Octet	sens,
* XDY_Octet	voie,
* XDY_Entier	translation
*
* Service rendu
* Prepare l'animation IHM
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

create procedure  XZAF;02
	@va_Horodate_in datetime = null
as

/*A DÅclaration des variables locales */
declare @vl_Portion 		smallint,
	@vl_PortionPlTete 	smallint,
	@vl_PortionPlQueue 	smallint,
	@vl_Translation 	int,
	@vl_TranslationPlTete 	int,
	@vl_TranslationPlQueue 	int,
	@vl_NumEvt		int,
	@vl_CleEvt		tinyint,
	@vl_NumAction		int,
	@vl_Site		T_SITE,
	@vl_Mobile		bit,
	@vl_Type		smallint,
	@vl_TypeTete		smallint,
	@vl_TypeQueue		smallint,
	@vl_SousType		tinyint,
	@vl_TypeObjet		tinyint,
	@vl_TypeMeteo		tinyint,
	@vl_Autoroute		T_AUTOROUTE,
	@vl_AutoroutePl		T_AUTOROUTE,
	@vl_PR			T_PR,
	@vl_PRsens1		T_PR,
	@vl_PRsens2		T_PR,
	@vl_PRTete		T_PR,
	@vl_PRQueue		T_PR,
	@vl_Sens		T_SENS,
	@vl_SensPl		T_SENS,
	@vl_Voie		tinyint,
	@vl_Echangeur		T_NOM,
	@vl_NomGare		T_NOM,
	@vl_EntSudPeage		bit,
	@vl_TypPtCar		tinyint,
	@vl_PtCar		tinyint,
	@vl_PosPtCar		tinyint,
	@vl_TypeAct		tinyint,
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
	@vl_PresenceRemorque	bit,
	@vl_PresenceCar		bit,
	@vl_PresenceMoto	bit,
	@vl_PresenceVL		bit,
	@vl_PresencePL		bit,
	@vl_autoroute_action	T_AUTOROUTE
		

	create table #SYN_OBJ_POR
	(
		evenement	int		null,
		cle		tinyint		null,
		portion		smallint,
		actio		int		null,
		cle_action	tinyint		null,
		type_objet	tinyint,
		sens		tinyint,
		voie		tinyint,
		translation	int,
		echangeur	char(25)	default XDC_CHAINE_VIDE
	)

	/*A Recherche des evenements en cours a la date precisee */
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
		point_caracteristique,
		numero_point_caracteristique,
		position_sur_aire_ou_echangeur,
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
	where	spid = @@spid and type != XZAEC_FMC_Basculement and
		type != XZAEC_FMC_TeteBouchon and
		type != XZAEC_FMC_QueueBouchon

	open Pointeur_evt
	fetch Pointeur_evt into @vl_NumEvt, @vl_CleEvt, @vl_Type,
				@vl_Autoroute, @vl_PR, @vl_Sens,
				@vl_TypPtCar, @vl_PtCar, @vl_PosPtCar,
				@vl_VR, @vl_VM2, @vl_VM1, @vl_VL,
				@vl_BAU, @vl_VR_I, @vl_VM2_I, @vl_VM1_I, 
				@vl_VL_I, @vl_BAU_I
	while @@sqlstatus = 0
	begin

		select @vl_Echangeur = XDC_CHAINE_VIDE

		/*B Test de la valuation de la localisation */
		if @vl_Autoroute=null or @vl_PR=null or
		   @vl_Sens=null
			return XDC_NOK
	
		/* type de l'objet a animer */
		if @vl_Type = XZAEC_FMC_Travaux
		begin
			/* recherche la date de debut de l'evenement */
			select
				@vl_HoroDebut = debut
			from HIS..FMC_GEN
			where numero = @vl_NumEvt and cle = @vl_CleEvt

			if @@rowcount<>1 return XDC_NOK

			/* recherche le type de travaux */
        		select 
				@vl_Mobile = mobile,
				@vl_AutoroutePl = autoroute_planifiee,
				@vl_PRTete = PR_tete_planifie,
				@vl_PRQueue = PR_queue_planifie,
				@vl_SensPl = sens_planifie
			from HIS..FMC_TRF
			where numero=@vl_NumEvt and cle=@vl_CleEvt

			if @@rowcount<>1 return XDC_NOK

			/* type de l'objet a animer */
			if @vl_HoroDebut < @va_Horodate_in and @vl_Mobile = XDC_VRAI
				select @vl_Type=XZAHC_Travaux_Mob_R
			else if @vl_HoroDebut >= @va_Horodate_in and @vl_Mobile = XDC_VRAI
				select @vl_Type=XZAHC_Travaux_Mob_P
			else if @vl_HoroDebut < @va_Horodate_in and @vl_Mobile = XDC_FAUX
				select @vl_Type=XZAHC_Travaux_Fix_R
			else	select @vl_Type=XZAHC_Travaux_Fix_P

			select @vl_Voie=XZAHC_VoieInconnue

			/* zone totale des travaux mobiles */
			if @vl_Mobile = XDC_VRAI
			begin
				/* type de travaux mobile */
				if @vl_HoroDebut < @va_Horodate_in
					select  @vl_TypeTete = XZAHC_Travaux_Mob_Tete_R,
						@vl_TypeQueue =XZAHC_Travaux_Mob_Queue_R
				else	select	@vl_TypeTete = XZAHC_Travaux_Mob_Tete_P,
						@vl_TypeQueue =XZAHC_Travaux_Mob_Queue_P

				/*B Si l'objet n'est pas sur echangeur, calcule portion, */
				/*B et translation */
				if	@vl_TypPtCar<>XDC_POINT_CARACT_ECHANGEUR or
					@vl_SensPl=XDC_SENS_INCONNU or
					@vl_SensPl=XDC_SENS_NORD or
					@vl_SensPl=XDC_SENS_SUD
				begin
			
					select	@vl_PortionPlTete=numero,
						@vl_TranslationPlTete=10000*(@vl_PRTete-PR_debut)/(PR_fin-PR_debut)
					from CFG..RES_POR
					where	autoroute=@vl_AutoroutePl and
						PR_debut<@vl_PRTete and
						PR_fin>=@vl_PRTete
					
					if @@rowcount<>1
						return XDC_NOK
			
					select	@vl_PortionPlQueue=numero,
						@vl_TranslationPlQueue=10000*(@vl_PRQueue-PR_debut)/(PR_fin-PR_debut)
					from CFG..RES_POR
					where	autoroute=@vl_AutoroutePl and
						PR_debut<@vl_PRQueue and
						PR_fin>=@vl_PRQueue
					
					if @@rowcount<>1
						return XDC_NOK
			
					/* si le sens est connu, le force sur section courante */
			
					if @vl_SensPl=XDC_SENS_SORTIE_NORD or
					   @vl_SensPl=XDC_SENS_ENTREE_NORD
						select @vl_SensPl=XDC_SENS_NORD
					
					else if	@vl_SensPl=XDC_SENS_SORTIE_SUD or
						@vl_SensPl=XDC_SENS_ENTREE_SUD
						select @vl_SensPl=XDC_SENS_SUD
			
				end
				
				/*B Si l'objet est sur echangeur, portion=num ech, et translation=pos sur ech */
				else
				begin
					select	@vl_PortionPlTete = @vl_PtCar,
						@vl_PortionPlQueue = @vl_PtCar,
						@vl_TranslationPlTete=isnull(@vl_PosPtCar,0),
						@vl_TranslationPlQueue=isnull(@vl_PosPtCar,0)
				end	
			end
		end

		/*B Recherche de la voie (la plus È gauche bloquee) */
		/*  pour les evenements autre que travaux           */
		else if	@vl_VR=XDC_VOIE_BLOQUEE or
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

		/*B Si l'objet n'est pas sur echangeur, calcule portion, et translation */
		if	@vl_TypPtCar<>XDC_POINT_CARACT_ECHANGEUR or
			@vl_Sens=XDC_SENS_INCONNU or
			@vl_Sens=XDC_SENS_NORD or
			@vl_Sens=XDC_SENS_SUD
		begin
	
			select	@vl_Portion=numero,
				@vl_Translation=10000*(@vl_PR-PR_debut)/(PR_fin-PR_debut)
			from CFG..RES_POR
			where	autoroute=@vl_Autoroute and
				PR_debut<@vl_PR and
				PR_fin>=@vl_PR
			
			if @@rowcount<>1
				return XDC_NOK
	
			/* si le sens est connu, le force sur section courante */
	
			if @vl_Sens=XDC_SENS_SORTIE_NORD or
			   @vl_Sens=XDC_SENS_ENTREE_NORD
				select @vl_Sens=XDC_SENS_NORD
			
			else if	@vl_Sens=XDC_SENS_SORTIE_SUD or
				@vl_Sens=XDC_SENS_ENTREE_SUD
				select @vl_Sens=XDC_SENS_SUD
	
		end
		
		/*B Si l'objet est sur echangeur, portion=num ech, et translation=pos sur ech */
		else
		begin
			select	@vl_Portion=@vl_PtCar,
				@vl_Translation=isnull(@vl_PosPtCar,0)

			/* rechercher le nom de l'echangeur */
			select @vl_Echangeur = nom
			from CFG..RES_ECH
			where numero = @vl_PtCar

			if @@rowcount = 0
				select @vl_Echangeur = XDC_CHAINE_VIDE
		end	
	
		if @vl_Type = XZAEC_FMC_Accident
		begin
			/* recherche des types de vehicules */
			/* impliques                        */
			select
				@vl_PresenceVL = nombre_de_vl,
				@vl_PresencePL = nombre_de_pl,
				@vl_PresenceRemorque = nombre_de_remorques,
				@vl_PresenceCar = nombre_de_car,
				@vl_PresenceMoto = nombre_de_moto
			from HIS..FMC_ACC
			where numero = @vl_NumEvt and cle = @vl_CleEvt

			if @@rowcount = 1
				select @vl_Type = @vl_PresenceVL * XZAHC_Accident_VL +
					@vl_PresencePL * XZAHC_Accident_PL +
					@vl_PresenceRemorque * XZAHC_Accident_Remorque +
					@vl_PresenceCar * XZAHC_Accident_Car +
					@vl_PresenceMoto * XZAHC_Accident_Moto +
					XZAHC_Accident
			else	select @vl_Type = XZAHC_Accident
		end
		else if @vl_Type =XZAEC_FMC_Meteo
		begin
			/* recherche le type de meteo */
			select
				@vl_TypeMeteo = type_de_perturbation
			from HIS..FMC_MET
			where numero = @vl_NumEvt and cle = @vl_CleEvt

			/* determination du type objet a animer  */
			/* (le type de meteo donne l'offset pour */
			/*  le calcul de l'objet a animer )      */
			select @vl_Type = @vl_TypeMeteo + XZAHC_Meteo
		end

		/*B ajout un mobile */
		insert #SYN_OBJ_POR (
				evenement,
				cle,
				portion,
				actio,
				cle_action,
				type_objet,
				sens,
				voie,
				translation,
				echangeur
				)
		values	(
				@vl_NumEvt,
				@vl_CleEvt,
				@vl_Portion,
				null,
				null,
				@vl_Type,
				@vl_Sens,
				@vl_Voie,
				@vl_Translation,
				@vl_Echangeur
				)

		/*B ajout de la zone de travaux mobiles */
		if (@vl_Type = XZAHC_Travaux_Mob_R or @vl_Type = XZAHC_Travaux_Mob_P) and 
		    @vl_Mobile = XDC_VRAI
		begin
			insert #SYN_OBJ_POR (
					evenement,
					cle,
					portion,
					actio,
					cle_action,
					type_objet,
					sens,
					voie,
					translation,
					echangeur
					)
			values	(
					@vl_NumEvt,
					@vl_CleEvt,
					@vl_PortionPlTete,
					null,
					null,
					@vl_TypeTete,
					@vl_SensPl,
					@vl_Voie,
					@vl_TranslationPlTete,
					@vl_Echangeur
					)

			insert #SYN_OBJ_POR (
					evenement,
					cle,
					portion,
					actio,
					cle_action,
					type_objet,
					sens,
					voie,
					translation,
					echangeur
					)
			values	(
					@vl_NumEvt,
					@vl_CleEvt,
					@vl_PortionPlQueue,
					null,
					null,
					@vl_TypeQueue,
					@vl_SensPl,
					@vl_Voie,
					@vl_TranslationPlQueue,
					@vl_Echangeur
					)
		end

		/* passage a l'evenement suivant */
		fetch Pointeur_evt into @vl_NumEvt, @vl_CleEvt, @vl_Type,
				@vl_Autoroute, @vl_PR, @vl_Sens,
				@vl_TypPtCar, @vl_PtCar, @vl_PosPtCar,
				@vl_VR, @vl_VM2, @vl_VM1, @vl_VL,
				@vl_BAU, @vl_VR_I, @vl_VM2_I, @vl_VM1_I, 
				@vl_VL_I, @vl_BAU_I
	end
	close Pointeur_evt


	/*A Recherche des actions en cours a la date precisee */
	declare Pointeur_act cursor
	for select 
		numero,
		sit,
		type,
		evenement,
		cle
	from ACT_FRM
	where spid = @@spid

	open Pointeur_act
	fetch Pointeur_act into @vl_NumAction, @vl_Site, @vl_Type, @vl_NumEvt, @vl_CleEvt
	while @@sqlstatus = 0
	begin
		/*B Traiter chaque action en cours */

		select @vl_Echangeur = XDC_CHAINE_VIDE

		/* recherche les informations sur l'evenement associe */
		select
			@vl_Autoroute = autoroute,
			@vl_PR = PR,
			@vl_Sens = sens,
			@vl_TypPtCar = point_caracteristique,
			@vl_PtCar = numero_point_caracteristique,
			@vl_PosPtCar = position_sur_aire_ou_echangeur,
			@vl_VR = VR, 
			@vl_VM2 = VM2, 
			@vl_VM1 = VM1, 
			@vl_VL = VL,
			@vl_BAU = BAU
		from EVT_FRM
		where numero = @vl_NumEvt and cle = @vl_CleEvt and
			spid = @@spid

		/* initialisation de la variable de type d'objet */
		select @vl_TypeObjet = null

		if @vl_Type =XDC_ACT_INTERVENTION
		begin
			/* recherche du type de l'intervention */
			select
				@vl_SousType = type
			from HIS..ACT_INT
			where actio = @vl_NumAction and sit = @vl_Site

			/* type de l'objet a animer */
			if @vl_SousType=XDC_ACT_INT_GND
				select  @vl_TypeObjet=XZAHC_Gendarme
			else select  @vl_TypeObjet=XZAHC_Pompier
		end
		else if @vl_Type  = XDC_ACT_PMV_REMORQUE
		begin
			/* recherche du PR de l'action */
			select
				@vl_PR = PR,
				@vl_Autoroute = autoroute
			from HIS..ACT_RMQ
			where actio = @vl_NumAction and sit = @vl_Site

			/* type de l'objet a animer */
			select  @vl_TypeObjet=XZAHC_PMV_Mobile
		end
		else if @vl_Type  = XDC_ACT_PMV_FOURGON
		begin
			/* recherche du PR de l'action */
			select
				@vl_PR = PR,
				@vl_Autoroute = autoroute
			from HIS..ACT_FRG
			where actio = @vl_NumAction and sit = @vl_Site

			/* type de l'objet a animer */
			select  @vl_TypeObjet=XZAHC_PMV_Fourgon 
		end
		else if @vl_Type  = XDC_ACT_PAT_SIS
		begin
			/* type de l'objet a animer */
			select  @vl_TypeObjet=XZAHC_Patrouille
		end
		else if @vl_Type  = XDC_ACT_DEPANNEUR
		begin
			/* type de l'objet a animer */
			select  @vl_TypeObjet=XZAHC_Depanneur
		end
		else if @vl_Type  = XDC_ACT_AFF_GARE
		begin
			/* type de l'objet a animer */
			select  @vl_TypeObjet=XZAHC_Gare

			select  @vl_NomGare=nom_gare
			from	HIS..ACT_GAR
			where	actio=@vl_NumAction and
				sit=@vl_Site
			
			/*A rechercher la localisation de la gare de peage */
			select	
				@vl_PRsens1=CFG..RES_PEA.PRsens1,
				@vl_PRsens2=CFG..RES_PEA.PRsens2
			from	CFG..RES_PEA, CFG..RES_POR
			where	CFG..RES_PEA.nom=@vl_NomGare and CFG..RES_POR.numero = CFG..RES_PEA.portion
				
			if @@rowcount = 1
			begin
				/* dans une gare de peage sur sectioncourante */
		
				select  @vl_Sens = XDC_SENS_INCONNU,
					@vl_TypPtCar=null,
					@vl_PtCar=null,
					@vl_PosPtCar=null
		
				if @vl_PRsens2 is null
					select @vl_PR = @vl_PRsens1
				else if @vl_PRsens1 is null
					select @vl_PR = @vl_PRsens2
				else if @vl_PRsens1 < @vl_PRsens2
					select @vl_PR = @vl_PRsens1 + (@vl_PRsens2 - @vl_PRsens1) / 2
				else select @vl_PR = @vl_PRsens2 + (@vl_PRsens1 - @vl_PRsens2) / 2
			end
			else
			begin
				/* sur un echangeur */
		
				select  
					@vl_TypPtCar = XDC_POINT_CARACT_ECHANGEUR,
					@vl_EntSudPeage = CFG..RES_ECH.peage_entree_sens1,
					@vl_PtCar = CFG..RES_ECH.numero,
					@vl_PosPtCar = XDC_POINT_CARACT_POS_DEF,
					@vl_Sens = XDC_SENS_ENTREE_SUD
				from	CFG..RES_ECH, CFG..RES_POR
				where	CFG..RES_ECH.nom=@vl_NomGare and 
					CFG..RES_POR.numero = CFG..RES_ECH.portion
		
				if @@rowcount = 0
					return XDC_OK
		
				/* si pas de peage sur l'entree sud alors positionner sur entree nord */
				if @vl_EntSudPeage = XDC_NON
					select @vl_Sens = XDC_SENS_ENTREE_NORD
			end
		end

		/*B Si l'objet n'est pas sur echangeur, calcule portion, 
		    et translation */
		if @vl_TypeObjet is not null	
		begin
			if @vl_TypPtCar<>XDC_POINT_CARACT_ECHANGEUR or
			   @vl_Sens=XDC_SENS_INCONNU or
			   @vl_Sens=XDC_SENS_NORD or
			   @vl_Sens=XDC_SENS_SUD
			begin
		
				select	@vl_Portion=numero,
					@vl_Translation=10000*(@vl_PR-PR_debut)/(PR_fin-PR_debut)
				from CFG..RES_POR
				where	autoroute=@vl_Autoroute and
					PR_debut<@vl_PR and
					PR_fin>=@vl_PR
				
				if @@rowcount<>1
					return XDC_NOK
		
				/* si le sens est connu, le force sur section courante */
				if @vl_Sens=XDC_SENS_SORTIE_NORD or
				   @vl_Sens=XDC_SENS_ENTREE_NORD
					select @vl_Sens=XDC_SENS_NORD
				
				else if	@vl_Sens=XDC_SENS_SORTIE_SUD or
					@vl_Sens=XDC_SENS_ENTREE_SUD
					select @vl_Sens=XDC_SENS_SUD
			end
			
			/*B Si l'objet est sur echangeur, portion=num ech, et 
			    translation=pos sur ech */
			else
			begin
				select	@vl_Portion=@vl_PtCar,
					@vl_Translation=isnull(@vl_PosPtCar,0)

				/* rechercher le nom de l'echangeur */
				select @vl_Echangeur = nom
				from CFG..RES_ECH
				where numero = @vl_PtCar

				if @@rowcount = 0
					select @vl_Echangeur = XDC_CHAINE_VIDE
			end
		
		
			/*B il s'agit d'une modification importante 
			    (suppression puis ajout) ou d'un ajout */
			insert #SYN_OBJ_POR (
					evenement,
					cle,
					portion,
					actio,
					cle_action,
					type_objet,
					sens,
					voie,
					translation,
					echangeur
					)
			values	(
					@vl_NumEvt,
					@vl_CleEvt,
					@vl_Portion,
					@vl_NumAction,
					@vl_Site,
					@vl_TypeObjet,
					@vl_Sens,
					XZAHC_VoieInconnue,
					@vl_Translation,
					@vl_Echangeur
					)
		end

		/* passage a l'action suivante */
		fetch Pointeur_act into @vl_NumAction, @vl_Site, @vl_Type,
					@vl_NumEvt, @vl_CleEvt
	end
	close Pointeur_act

	/* retourner la liste des objets mobiles */
	select 
		evenement,
		cle,
		portion,
		actio,
		cle_action,
		type_objet,
		sens,
		voie,
		translation,
		echangeur
	from #SYN_OBJ_POR

	return XDC_OK
go
