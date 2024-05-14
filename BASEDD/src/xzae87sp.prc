/*E*/
/*  Fichier : $Id: xzae87sp.prc,v 1.22 2020/11/03 17:01:52 pc2dpdy Exp $      Release : $Revision: 1.22 $        Date : $Date: 2020/11/03 17:01:52 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER XZAE87sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* lecture et ecriture des fmc
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	03/08/95	: Creation
* C.T.	09/08/95	: Modif commentaire affiche (V 1.2)
* C.T.	25/08/95	: Prendre tous les travaux en cours 
*                         pendant la duree (V 1.3)
* C.T.	22/08/95	: Modif ordre (V 1.4)
* C.T.	22/08/95	: Correction ordre des voies retournees et
*                         commentaires(V 1.4)
* C.T.  17/01/96        : correction longueur du libelle des travaux pour bretelle (1.5)
* C.T.  15/03/96        : Ajout date de planification des travaux discontinus (754) (1.6)
* P.V.  02/09/96	: Filtrage synthese par district (DEM1171) (V1.7)
* C.T.  06/09/96        : Sur PC simplifie, restreindre la liste des travaux concernes
*                         a ceux en prevision (V1.8)
* C.T.  25/11/96        : ajout bau etroite en sortie (SAD4) (1283) (V1.9)
* JMG	11/02/97	: ajout neutralisation de voies (DEM/1394) ANA64 1.10
* JMG	08/01/98	: gestion multi-sites (dem/1536) 1.11
* JMG	22/01/98	: la config des voies est la config finale (dem/1556) 1.13
* C.T.	23/02/98	: Modif default suite a passage en version SQL 11.0.3.2 (1.14)
* P.N	05/02/99	: Modif intergest. dem/1724 1.15
* P.C	18/03/03	: Ajout nouveaux types de travaux (1.16)
* PNI	28/05/09	: Suppression du ; DEM886 v1.17
* JPL	09/02/11	: Lecture type travaux en table des libelles (pour "Tunnels") (DEM 951)  1.18
* VR	06/10/11	: Ajout vitesse et vitesse_opposee 1.19 (DEM/1015)
* NDE	17/07/15	: Ajout evenements dont la cause sont des travaux (DEM1136) 1.20
* JMGa	08/03/18	: ajout personnel 1.21
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAE87_SyntheseTravaux
* 
* Arguments en entree
* XDY_Horodate	va_HoroDeb_in
* XDY_Horodate	va_HoroFin_in
* XDY_Site	va_District_in
* 
* Arguments en sortie
* 
* liste retournee :
* XDY_Horodate	va_HoroFin_in

* Service rendu
* Synthese des travaux en cours ou en prevision a partir de la
* date actuelle
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametres d'entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Si la date de fin n'est pas valuée alors la synthese est sur 7 jours
* 
* Fonction
-----------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE87' and type = 'P')
	drop procedure XZAE87
go

create procedure XZAE87
	@va_HoroDeb_in	datetime	= null,
	@va_HoroFin_in	datetime	= null,
	@va_District_in	tinyint		= null,
	@va_NomSiteLocal_in             char(2)         = null
as
	declare @vl_NumEvt int, @vl_CleEvt tinyint, @vl_Debut datetime, @vl_DebutPrevu datetime, 
		@vl_NumEvtSuivant int, @vl_CleEvtSuivant tinyint,
		@vl_Fin datetime, @vl_FinPrevue datetime,
		@vl_Type tinyint, @vl_Type_FMC tinyint, @vl_Nature bit, @vl_Autoroute T_AUTOROUTE, 
		@vl_PRTete T_PR, @vl_PRQueue T_PR, @vl_Sens T_SENS, @vl_VL T_VOIE,
		@vl_VM1 T_VOIE, @vl_VM2 T_VOIE, @vl_VR T_VOIE, @vl_BAU T_VOIE, @vl_VL_I T_VOIE, 
		@vl_VM1_I T_VOIE, @vl_VM2_I T_VOIE, @vl_VR_I T_VOIE, @vl_BAU_I T_VOIE, 
		@vl_BAU_etroite tinyint, @vl_BAU_I_etroite tinyint, @vl_Mobile bit,
		@vl_PRQueuePlanifie T_PR, @vl_PRTetePlanifie T_PR, @vl_SensPlanifie T_SENS,
		@vl_HoroDebPlanifie datetime, @vl_HoroFinPlanifie datetime, 
		@vl_TypePointCar tinyint, @vl_NumPointCar tinyint,
		@vl_Prevision bit, @vl_DebutAffichee datetime, @vl_FinAffichee datetime, 
		@vl_HoroVal datetime, @vl_Continu bit,
		@vl_Texte varchar(250), @vl_Longueur int, @vl_VoieRetrecie bit,
		@vl_InitLigne varchar(20), @vl_Ligne char(92), @vl_Status int, @vl_SqlTravaux int,
		@vl_DateFin datetime, @vl_Intitule char(68), @vl_NomPointCar T_NOM, @vl_Nombre int,
		@vl_PRmin int, @vl_PRmax int, @vl_indice int, @vl_Site T_SITE,
		@vl_Lib_Type_Travaux varchar(100),
		@vl_Lib_Autres_Travaux varchar(100), @vl_vitesse tinyint,
		@vl_personnel tinyint

	declare @vl_Entree1 bit, @vl_Entree2 bit, @vl_Sortie1 bit, @vl_Sortie2 bit,
		@vl_NumPlan smallint, @vl_NomPlan T_NOM, @vl_NomEchSortie T_NOM,
		@vl_NomEchReinjection T_NOM, @vl_EchReinjection tinyint


	/*A controle des parametres d'entree */
	if @va_HoroFin_in = XDC_DATE_NULLE
		select  @va_HoroFin_in = null

	if @va_HoroFin_in = null
		select @vl_DateFin = dateadd(day,7, @va_HoroDeb_in)
	else	select @vl_DateFin = @va_HoroFin_in

 	/*A Initialisations */
	select @vl_indice=0

	select	@vl_Lib_Autres_Travaux = rtrim (libelle)
	from	CFG..LIB_PRE
	where	notyp = TYP_TRAVAUX and code = XZAEC_TRV_Autre

 
	/*A Recherche du site */
        /* MOVIS */
        if @va_NomSiteLocal_in = null
                select  @vl_Site = numero
                from CFG..RES_DIS
                where serveur = @@servername
        else
                select  @vl_Site = numero
                from CFG..RES_DIS
                where code = @va_NomSiteLocal_in

	if @vl_Site = null
	  return XDC_NOK

	/* tables de travail */
	create table #COMMENTAIRES (	numero smallint, texte char(250) null)
	create table #SYNTHESE_TRV (	ligne char(92), 
					VL tinyint null,	-- default XDC_VOIE_INCONNUE, 
					VM1 tinyint null,	-- default XDC_VOIE_INCONNUE, 
					VM2 tinyint null,	-- default XDC_VOIE_INCONNUE, 
					VR tinyint null,	-- default XDC_VOIE_INCONNUE, 
					BAU tinyint null,	-- default XDC_VOIE_INCONNUE,
					VL_I tinyint null,	-- default XDC_VOIE_INCONNUE, 
					VM1_I tinyint null,	-- default XDC_VOIE_INCONNUE, 
					VM2_I tinyint null,	-- default XDC_VOIE_INCONNUE, 
					VR_I tinyint null,	-- default XDC_VOIE_INCONNUE, 
					BAU_I tinyint null,	-- default XDC_VOIE_INCONNUE,
					BAU_etroite tinyint null,	-- default XDC_FAUX,
					BAU_I_etroite tinyint null,	-- default XDC_FAUX,
					intitule bit default XDC_FAUX,
					date	datetime null
					)

	/* creation d'un curseur pour le parcours des morceaux */
	/* de commentaires */
	declare Pointeur_comment cursor
	for select texte
	from #COMMENTAIRES
	order by numero

	/*A recherche les travaux */
	declare pointeur_trv cursor
	for select		
		EXP..FMC_GEN.numero,
		EXP..FMC_GEN.cle,
		EXP..FMC_GEN.debut,
		EXP..FMC_GEN.debut_prevu,
		EXP..FMC_GEN.fin,
		EXP..FMC_GEN.fin_prevue,
		horodate_validation,
		EXP..FMC_TRF.type,
		EXP..FMC_GEN.type as type_fmc,
		nature,
		autoroute,
		PR,
		sens,
		point_caracteristique,
		numero_point_caracteristique,
		EXP..FMC_TRF.VL,
		EXP..FMC_TRF.VM1,
		EXP..FMC_TRF.VM2,
		EXP..FMC_TRF.VR,
		EXP..FMC_TRF.BAU,
		EXP..FMC_TRF.VL_I,
		EXP..FMC_TRF.VM1_I,
		EXP..FMC_TRF.VM2_I,
		EXP..FMC_TRF.VR_I,
		EXP..FMC_TRF.BAU_I,
		isnull(BAU_etroite, XDC_FAUX),
		isnull(BAU_I_etroite, XDC_FAUX),
		mobile,
		continu,
		PR_queue_planifie,
		PR_tete_planifie,
		sens_planifie,
		horodate_debut_planifiee,
		horodate_fin_planifiee
	from EXP..FMC_GEN 
		inner join EXP..FMC_HIS_DER on
			EXP..FMC_HIS_DER.numero = EXP..FMC_GEN.numero and
			EXP..FMC_HIS_DER.cle = EXP..FMC_GEN.cle
		left join EXP..FMC_TRF on 
			EXP..FMC_TRF.numero = EXP..FMC_GEN.numero and EXP..FMC_TRF.cle = EXP..FMC_GEN.cle
		left join EXP..FMC_GEN as TR on 
			EXP..FMC_GEN.cause = TR.numero and EXP..FMC_GEN.cle_cause = TR.cle
	where 	(EXP..FMC_GEN.type in (XZAEC_FMC_Travaux, XZAEC_FMC_BasculTravaux) or
		 (EXP..FMC_GEN.type in (XZAEC_FMC_BasculTravaux, XZAEC_FMC_FermetureEch, XZAEC_FMC_AireFermee, XZAEC_FMC_Deviation) and
		  TR.type in (XZAEC_FMC_Travaux, XZAEC_FMC_BasculTravaux))) and
		EXP..FMC_GEN.datex=XDC_FAUX and
#ifdef PC_SIMPL
		EXP..FMC_GEN.sit = @vl_Site and
#endif
		EXP..FMC_GEN.fin is null and
		(EXP..FMC_GEN.debut_prevu < @vl_DateFin or
		(EXP..FMC_GEN.debut_prevu is null and EXP..FMC_GEN.debut < @vl_DateFin))
	order by coalesce(TR.debut, EXP..FMC_GEN.debut, "01/01/2500"), coalesce(TR.debut_prevu, EXP..FMC_GEN.debut_prevu), 
		 case when TR.numero is null then 0 else 1 end,		 
		 autoroute, PR, sens

	open pointeur_trv
	fetch pointeur_trv into @vl_NumEvt, @vl_CleEvt, @vl_Debut, @vl_DebutPrevu, @vl_Fin, @vl_FinPrevue,
			@vl_HoroVal, @vl_Type, @vl_Type_FMC, @vl_Nature, @vl_Autoroute, @vl_PRQueue, @vl_Sens, @vl_TypePointCar,
			@vl_NumPointCar, @vl_VL,
			@vl_VM1, @vl_VM2, @vl_VR, @vl_BAU, @vl_VL_I, 
			@vl_VM1_I, @vl_VM2_I, @vl_VR_I, @vl_BAU_I, @vl_BAU_etroite, @vl_BAU_I_etroite,
			@vl_Mobile, @vl_Continu,
			@vl_PRQueuePlanifie, @vl_PRTetePlanifie, @vl_SensPlanifie,
			@vl_HoroDebPlanifie, @vl_HoroFinPlanifie

	select @vl_SqlTravaux = @@sqlstatus
	while (@vl_SqlTravaux = 0)
	begin
	  select @vl_PRmin=min(PR_debut) from CFG..RES_POR POR1 
	  where (POR1.district=@va_District_in or POR1.district_recouvrement=@va_District_in)
			and POR1.autoroute=@vl_Autoroute
			and POR1.PR_debut in (select min(PR_debut) from CFG..RES_POR POR2
					where (POR1.district=POR2.district and 
						POR1.district_recouvrement=POR2.district_recouvrement)
						and POR1.autoroute=POR2.autoroute)
	  select @vl_PRmax=max(PR_fin) from CFG..RES_POR POR1
	  where (POR1.district=@va_District_in or POR1.district_recouvrement=@va_District_in)
			and POR1.autoroute=@vl_Autoroute
			and POR1.PR_fin in (select max(PR_fin) from CFG..RES_POR POR2
					 where (POR1.district=POR2.district and 
						POR1.district_recouvrement=POR2.district_recouvrement)
						and POR1.autoroute=POR2.autoroute)

	  if @vl_Debut is null
		select @vl_Prevision = XDC_VRAI, @vl_DebutAffichee = @vl_DebutPrevu
	  else	select @vl_Prevision = XDC_FAUX, @vl_DebutAffichee = @vl_Debut

	  select @vl_FinAffichee = @vl_FinPrevue

	  /*B rechercher la localisation de tete de travaux */
	  select
		@vl_VoieRetrecie = voies_retrecies,
		@vl_PRTete = localisation_tete,
		@vl_vitesse = vitesse,
		@vl_personnel = personnel
	  from EXP..FMC_TRH
	  where numero =  @vl_NumEvt and cle = @vl_CleEvt and
		horodate = @vl_HoroVal

	  select @vl_Ligne = XZAEC_SYN_VITESSE + convert(varchar(3),@vl_vitesse)
	  insert #SYNTHESE_TRV (ligne, intitule)
	  values(@vl_Ligne, XDC_FAUX)

	  if @vl_personnel  = 1
	  begin
	  	select @vl_Ligne = XZAEC_SYN_PERSONNEL
		insert #SYNTHESE_TRV (ligne, intitule)
		values(@vl_Ligne, XDC_FAUX)
	end

	  if    ((@vl_PRTete>@vl_PRmin and @vl_PRTete<@vl_PRmax)
	      or (@vl_PRQueue>@vl_PRmin and @vl_PRQueue<@vl_PRmax)
	      or (@vl_PRTete<@vl_PRmin and @vl_PRQueue>@vl_PRmax)
	      or (@vl_PRQueue<@vl_PRmin and @vl_PRTete>@vl_PRmax)
	      or (@va_District_in=0))
	  begin
		select @vl_indice=1

		/*B creation de l'intitule pour des travaux mobiles  */
		if @vl_Mobile = XDC_VRAI
		begin
			/*B creation de l'intitule */
			exec @vl_Status = XZAE88 @vl_Prevision, @vl_Type_FMC, @vl_Autoroute, @vl_PRTetePlanifie, 
						@vl_PRQueuePlanifie, @vl_SensPlanifie, 
						@vl_Intitule output

			insert #SYNTHESE_TRV (ligne, VL, VM1, VM2, VR, BAU, VL_I, VM1_I, VM2_I, VR_I, BAU_I, 
						BAU_etroite, BAU_I_etroite, intitule, date)
			values(@vl_Intitule, @vl_VL, @vl_VM1, @vl_VM2, @vl_VR, @vl_BAU, 
				@vl_VL_I, @vl_VM1_I, @vl_VM2_I, @vl_VR_I, @vl_BAU_I, @vl_BAU_etroite, @vl_BAU_I_etroite,
				XDC_VRAI, @vl_DebutAffichee)
				select @vl_Ligne = XZAEC_SYN_TRV_MOB

			select @vl_Ligne = XZAEC_SYN_TRV_MOB

			insert #SYNTHESE_TRV (ligne, intitule)
			values(@vl_Ligne, XDC_FAUX)
		end
		/*B creation de l'intitule pour des travaux fixes */
		else	
		begin
			exec @vl_Status = XZAE88 @vl_Prevision, @vl_Type_FMC, @vl_Autoroute, @vl_PRTete, 
						@vl_PRQueue, @vl_Sens, 
						@vl_Intitule output

			insert #SYNTHESE_TRV (ligne, VL, VM1, VM2, VR, BAU, VL_I, VM1_I, VM2_I, VR_I, BAU_I, 
						BAU_etroite, BAU_I_etroite, intitule, date)
			values(@vl_Intitule, @vl_VL, @vl_VM1, @vl_VM2, @vl_VR, @vl_BAU, 
				@vl_VL_I, @vl_VM1_I, @vl_VM2_I, @vl_VR_I, @vl_BAU_I, @vl_BAU_etroite, @vl_BAU_I_etroite,
				XDC_VRAI, @vl_DebutAffichee)
		end

		/*B ajout du point caracteristique */
		if @vl_TypePointCar is not null and @vl_NumPointCar is not null
		begin
			if @vl_TypePointCar = XDC_POINT_CARACT_AIRE
			begin
				select @vl_NomPointCar = nom
				from CFG..RES_AIR
				where numero = @vl_NumPointCar

				if @@rowcount = 1
					select @vl_Ligne =  XZAEC_SYN_VEH_AIR + @vl_NomPointCar
				else	select @vl_Ligne = null
			end
			else if @vl_TypePointCar = XDC_POINT_CARACT_ECHANGEUR
			begin
				select @vl_NomPointCar = nom
				from CFG..RES_ECH
				where numero = @vl_NumPointCar

				if @@rowcount = 1
					select @vl_Ligne =  XZAEC_SYN_VEH_ECH + @vl_NomPointCar
				else	select @vl_Ligne = null
			end
			else if @vl_TypePointCar = XDC_POINT_CARACT_PEAGE
			begin
				select @vl_NomPointCar = nom
				from CFG..RES_PEA
				where numero = @vl_NumPointCar

				if @@rowcount = 1
					select @vl_Ligne =  XZAEC_SYN_VEH_PEA + @vl_NomPointCar
				else	select @vl_Ligne = null
			end
			else if @vl_TypePointCar = XDC_POINT_CARACT_LIT_ARRET
			begin
				select @vl_NomPointCar = nom
				from CFG..RES_PEA
				where numero = @vl_NumPointCar

				if @@rowcount = 1
					select @vl_Ligne =  XZAEC_SYN_VEH_LIT + @vl_NomPointCar
				else	select @vl_Ligne = null
			end
			else	select @vl_Ligne = null

			if @vl_Ligne is not null
			begin
				insert #SYNTHESE_TRV (ligne, intitule)
				values(@vl_Ligne, XDC_FAUX)
			end

			if @vl_TypePointCar = XDC_POINT_CARACT_ECHANGEUR and @vl_Type_FMC = XZAEC_FMC_FermetureEch
			begin
				select 
					@vl_Entree1 = entree1,
					@vl_Entree2 = entree2,
					@vl_Sortie1 = sortie1,
					@vl_Sortie2 = sortie2
				from EXP..FMC_ECH
				where numero = @vl_NumEvt
				and cle = @vl_CleEvt
				
				if @vl_Entree1 = 1 
					select @vl_Ligne = XZAEC_SYN_ENT_S_FERM
				else 
					select @vl_Ligne = XZAEC_SYN_ENT_S_OUV
				insert #SYNTHESE_TRV (ligne, intitule) values(@vl_Ligne, XDC_FAUX )

				if @vl_Entree2 = 1 
					select @vl_Ligne = XZAEC_SYN_ENT_N_FERM
				else 
					select @vl_Ligne = XZAEC_SYN_ENT_N_OUV
				insert #SYNTHESE_TRV (ligne, intitule) values(@vl_Ligne, XDC_FAUX )

				if @vl_Sortie1 = 1 
					select @vl_Ligne = XZAEC_SYN_SOR_S_FERM
				else 
					select @vl_Ligne = XZAEC_SYN_SOR_S_OUV
				insert #SYNTHESE_TRV (ligne, intitule) values(@vl_Ligne, XDC_FAUX )

				if @vl_Sortie2 = 1 
					select @vl_Ligne = XZAEC_SYN_SOR_N_FERM
				else 
					select @vl_Ligne = XZAEC_SYN_SOR_N_OUV
				insert #SYNTHESE_TRV (ligne, intitule) values(@vl_Ligne, XDC_FAUX )
			end
		end

		if @vl_Type_FMC = XZAEC_FMC_Deviation
		begin
			/* rechercher les informations de deviation */
			select @vl_NumPlan = plan_associe
			from EXP..FMC_DEL
			where   numero = @vl_NumEvt and cle = @vl_CleEvt

			if @vl_NumPlan != null
			begin
				select
					@vl_NomPlan = CFG..PLN_SCR.nom,
					@vl_NomEchSortie = CFG..RES_ECH.nom,
					@vl_EchReinjection = CFG..PLN_SCR.echangeur_reinjection
				from CFG..PLN_SCR, CFG..RES_ECH
				where	CFG..PLN_SCR.numero = @vl_NumPlan and
					CFG..RES_ECH.numero = CFG..PLN_SCR.echangeur_sortie

				if @@rowcount = 1
				begin
					select @vl_NomEchReinjection = nom
					from CFG..RES_ECH
					where	numero = @vl_EchReinjection

					if @@rowcount = 1
					begin	
						/* Ajout des informations de deviation dans la synthese */
						select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_PLN_SCR +
									+ @vl_NomPlan
						insert #SYNTHESE_TRV (ligne, intitule) values(@vl_Ligne, XDC_FAUX )

						select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ECH_SOR +
									+ @vl_NomEchSortie
						insert #SYNTHESE_TRV (ligne, intitule) values(@vl_Ligne, XDC_FAUX )

						select @vl_Ligne = XZAEC_SYN_PAS_DATE + XZAEC_SYN_ECH_INJ +
									+ @vl_NomEchReinjection
						insert #SYNTHESE_TRV (ligne, intitule) values(@vl_Ligne, XDC_FAUX )
					end
				end
			end
		end

		if @vl_Type_FMC in (XZAEC_FMC_Travaux, XZAEC_FMC_BasculTravaux)
		begin
			/*B nature de travaux */
			if @vl_Nature = XZAEC_TRV_Neufs
				select @vl_InitLigne = XZAEC_SYN_TRV_NEU
			else	select @vl_InitLigne = XZAEC_SYN_TRV_ENT

			/*B type de travaux */
			select	@vl_Lib_Type_Travaux = rtrim (libelle)
			from	CFG..LIB_PRE
			where	notyp = TYP_TRAVAUX and code = @vl_Type

			if @@rowcount = 0
				select @vl_Lib_Type_Travaux = @vl_Lib_Autres_Travaux
			select @vl_Ligne = @vl_InitLigne + ' : ' + @vl_Lib_Type_Travaux

			insert #SYNTHESE_TRV (ligne, intitule)
			values(@vl_Ligne, XDC_FAUX)
		end

		/*B ajout heure de debut */
		select  @vl_Ligne = XZAEC_SYN_DEB + convert(char(10), @vl_DebutAffichee, 103) + ' ' +
					convert(char(8), @vl_DebutAffichee, 8)

		insert #SYNTHESE_TRV (ligne, intitule )
		values(@vl_Ligne, XDC_FAUX )

		/*B ajout heure de fin */
		select  @vl_Ligne = XZAEC_SYN_FIN + convert(char(10), @vl_FinAffichee, 103) + ' ' +
					convert(char(8), @vl_FinAffichee, 8)

		insert #SYNTHESE_TRV (ligne, intitule)
		values(@vl_Ligne, XDC_FAUX)

		/*B Voie retrecie */
		if @vl_VoieRetrecie = XDC_VRAI
		begin
			select  @vl_Ligne = XZAEC_SYN_VOI_RET

			insert #SYNTHESE_TRV (ligne, intitule )
			values(@vl_Ligne, XDC_FAUX )
		end

		/*A cas des travaux discontinu */
		if @vl_Continu = XDC_VRAI
		begin
			select  @vl_Ligne = XZAEC_SYN_TRV_DISCONT

			insert #SYNTHESE_TRV (ligne, intitule )
			values(@vl_Ligne, XDC_FAUX)

			/* date de debut de planification a afficher */
			select  @vl_Ligne = XZAEC_SYN_DEB + convert(char(10), @vl_HoroDebPlanifie, 103) + ' ' +
						convert(char(8), @vl_HoroDebPlanifie, 8)

			insert #SYNTHESE_TRV (ligne, intitule )
			values(@vl_Ligne, XDC_FAUX)

			/* date de fin de planification a afficher */
			select  @vl_Ligne = XZAEC_SYN_FIN + convert(char(10), @vl_HoroFinPlanifie, 103) + ' ' +
						convert(char(8), @vl_HoroFinPlanifie, 8)

			insert #SYNTHESE_TRV (ligne, intitule)
			values(@vl_Ligne, XDC_FAUX)
		end
	  end

	  /*B recuperer les travaux suivants */
	  fetch pointeur_trv into @vl_NumEvtSuivant, @vl_CleEvtSuivant, @vl_Debut, @vl_DebutPrevu, @vl_Fin, @vl_FinPrevue,
		@vl_HoroVal, @vl_Type, @vl_Type_FMC, @vl_Nature, @vl_Autoroute, @vl_PRQueue, @vl_Sens, @vl_TypePointCar,
		@vl_NumPointCar, @vl_VL,
		@vl_VM1, @vl_VM2, @vl_VR, @vl_BAU, @vl_VL_I, 
		@vl_VM1_I, @vl_VM2_I, @vl_VR_I, @vl_BAU_I,  @vl_BAU_etroite, @vl_BAU_I_etroite,
		@vl_Mobile, @vl_Continu,
		@vl_PRQueuePlanifie, @vl_PRTetePlanifie, @vl_SensPlanifie,
		@vl_HoroDebPlanifie, @vl_HoroFinPlanifie

	  select @vl_SqlTravaux = @@sqlstatus


	  if @vl_indice=1
	  begin
		select @vl_indice=0

		/*B ajout des commentaires de toutes les validations */
		select @vl_InitLigne = XZAEC_SYN_COM + ': '
		
		insert #COMMENTAIRES
		select numero,texte 
		from EXP..FMC_COM
		where	evenement = @vl_NumEvt and
			cle = @vl_CleEvt 
		order by horodate_validation, numero

		/*B Ajout les commentaires des  validations de la FMC */
		if @@rowcount > 0
		begin
			open Pointeur_comment
			fetch Pointeur_comment into @vl_Texte
			while (@@sqlstatus = 0)
			begin
				/* inserer les morceaux du commentaire en */
				/* tronconnant par texte de 60 caracteres */
				select @vl_Longueur = char_length(@vl_Texte),
					@vl_Ligne = @vl_InitLigne + substring(@vl_Texte, 1, 46)

				insert #SYNTHESE_TRV (ligne, intitule)
				values( @vl_Ligne, XDC_FAUX)

				if @vl_Longueur > 46
				begin
					select @vl_Ligne = substring(@vl_Texte, 47, 60)
					insert #SYNTHESE_TRV (ligne, intitule )
					values( @vl_Ligne, XDC_FAUX)

					if @vl_Longueur > 107
					begin
						select @vl_Ligne = substring(@vl_Texte, 108, 60)
						insert #SYNTHESE_TRV (ligne, intitule )
						values( @vl_Ligne, XDC_FAUX )
					end
					if @vl_Longueur > 168
					begin
						select @vl_Ligne = substring(@vl_Texte, 169, 61)
						insert #SYNTHESE_TRV (ligne, intitule )
						values( @vl_Ligne, XDC_FAUX)
					end
				end

				/* ligne suivante : pas d'entete */
				select @vl_InitLigne  = ""

				/* commentaire suivant */
				fetch Pointeur_comment into @vl_Texte
			end

			close Pointeur_comment

			/* effacer le contenu */
			delete #COMMENTAIRES
		end
	  end

	  /* passage au traitement du suivant */
	  select @vl_NumEvt = @vl_NumEvtSuivant, @vl_CleEvt = @vl_CleEvtSuivant

	end

	select
		ligne,
		date,
		isnull(VR, XDC_VOIE_INCONNUE), 
		isnull(VM2, XDC_VOIE_INCONNUE), 
		isnull(VM1, XDC_VOIE_INCONNUE), 
		isnull(VL, XDC_VOIE_INCONNUE), 
		isnull(BAU, XDC_VOIE_INCONNUE), 
		isnull(VR_I, XDC_VOIE_INCONNUE), 
		isnull(VM2_I, XDC_VOIE_INCONNUE), 
		isnull(VM1_I, XDC_VOIE_INCONNUE), 
		isnull(VL_I, XDC_VOIE_INCONNUE), 
		isnull(BAU_I, XDC_VOIE_INCONNUE), 
		isnull(BAU_etroite, XDC_FAUX),
		isnull(BAU_I_etroite, XDC_FAUX)
	from #SYNTHESE_TRV

	select @vl_Nombre = count(*) from #SYNTHESE_TRV

	if @vl_Nombre = 0
		return XDC_PAS_SYN

	return XDC_OK
go
