/*E*/
/*  Fichier : $Id: vuem01sp.prc,v 1.29 2021/01/07 07:22:19 gesconf Exp $        Release : $Revision: 1.29 $        Date : $Date: 2021/01/07 07:22:19 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER vuem01sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* vues metiers
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	18/07/95	: Creation
* C.T.	23/08/95	: Ajout des commentaires et des 
*                         types de points caracteristiques (V 1.2)
* C.T.	06/09/95	: Correction commentaires (V 1.3)
* C.T.	28/09/95	: Ajout numero fmc  (V 1.4)
* C.T.	29/09/95	: Suppression des voies neutralisees bloquees pour le comptage  (V 1.5)
* C.T.	29/09/95	: Modification des recherches de fmc (V 1.6)
* C.T.	03/10/95	: Correction fin non valuee (V 1.7)
* C.T.	12/12/95	: Correction limites de concession (V 1.8,1.9)
* C.T.	24/06/96	: Supprimer les basculements lies aux travaux 
*                         recherche le nb max de voies neutralisees ou bloquees 
*                         neutralisees, remplace PR de queue final par PR de tete initiale,
*                         prise en compte des PR fantomes pour la long de bouchon (V1.10)
* V.L.	30/10/96	: Ajout de l'origine de l'alerte (V1.11)
* V.L.	06/11/96	: Correction longueur bouchons (V1.12)
* V.L.	07/11/96	: Ajout Fausses alertes (FMC 18 DEM 1282) (V1.13)
* JMG	20/12/96	: optimisation 1.15
* C.T.	24/02/98	: correction erreur de compilation 1.16
* P.N.	05/02/99	: Modif interg. dem/1724 v1.18
* P.N.	26/03/99	: Modif interg. dem/1724 v1.19
* JMG	03/12/08	: ajout validation dans FMC_ACC 1.20 1.21 DEM/
* PCL	17/02/11	: bug origine alerte sans tenir compte du site 1.22 MANTIS 511
* JPL	22/08/12	: Simple renommage de constantes  1.23
* PNI	01/10/2014	: Ajout des lignes pour les bouchons et des nouvelles colonnes v1.24 DEM1092
* JPL	08/07/2016	: Support regions pour criteres geographiques; compatible precedente (DEM 1171)  1.25
* JMG   09/03/17 	: REGIONALISATION - DEM1220 1.27
* PNI   08/12/20        : Ajout position sur bretelle 1.28
* PNI   06/01/20        : Ajout nature 1.29
* ABE	19/11/22	: Suppression des FMC hors reseau escota de la liste DEM-SAE412
* LCL	28/09/23 	: Adaptations CNA DEM-483	
* PNI	10/02/24	: Pas de mise à jour du PR du bouchon avec celui du bouchon conséquence  SAE-571
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	VUEM01_Detail_exploitation
* 
* Arguments en entree
* XDY_Horodate	va_Debut_in
* XDY_Site	va_District_in
* 
* Arguments en sortie
* 
* premiere liste retournee
*	debut
*	fin
*	duree,
*	autoroute,
*	PR,
*	sens,
*	nom_type,
*	longueur,
*	duree_bouchon,
*	type_bouchon,
*	nombre_de_vl,
*	nombre_de_pl,
*	nombre_de_car,
*	nombre_de_moto,
*	nombre_de_remorques,
*	morts,
*	blesses_graves,
*	blesses_legers,
*	fausse_alerte,
*	voies_neutralisees,
*	origine,
*	point_caracteristique,
*	type_meteo,
*	numero,
*	cle
* 
* deuxieme liste retournee : liste des commentaires par fmc
*	numero,
*	cle
*	texte
*
* Service rendu
* Detail des actions d'exploitation/evenement d'un mois pour un district
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
-----------------------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'VUEM01b' and type = 'P')
	drop procedure VUEM01b
go


create procedure VUEM01b
	@va_Debut_in	datetime	= null,
	@va_District_in	T_SITE		= null
as
	declare @vl_Fin datetime, @vl_Type smallint, @vl_TypeBouchon tinyint,
		@vl_NumEvt int, @vl_CleEvt tinyint, @vl_Cause int,
		@vl_CleCause tinyint, @vl_CauseBouch int, @vl_CleCauseBouch tinyint, 
		@vl_Sens T_SENS, @vl_NomSens varchar(100),@vl_nature varchar(100),
		@vl_Longueur int, @vl_fausse_alerte tinyint, @vl_Autoroute T_NOM, @vl_PR T_PR,
		@vl_MaxPR int, @vl_MinPR int, @vl_TypeVeh tinyint, @vl_Duree int,
		@vl_nombre_de_vl tinyint, @vl_nombre_de_pl tinyint, @vl_nombre_de_car tinyint,
		@vl_nombre_de_moto tinyint, @vl_nombre_de_remorques tinyint, @vl_morts tinyint,
		@vl_blesses_graves tinyint, @vl_blesses_legers tinyint, @vl_Densite char(15),
		@vl_Site T_SITE, @vl_NumAutoroute T_AUTOROUTE, @vl_Total tinyint, @vl_TypeCauseBouch tinyint,
		@vl_Texte char(250), @vl_PointCar tinyint,  @vl_PositionCar tinyint, @vl_DateCourante datetime, @vl_DateButee datetime,
		@vl_TypePointCar varchar(100), @vl_Position varchar(100), @vl_Suppression bit, @vl_auto char(5), @vl_cpt int,@vl_VR tinyint,
				@vl_VM1 tinyint,
				@vl_VM2 tinyint,
				@vl_VL tinyint,
				@vl_VR_I tinyint,
				@vl_VM1_I tinyint,
				@vl_VM2_I tinyint,
				@vl_VL_I tinyint,
				@vl_BAU_I tinyint,
				@vl_BAU tinyint
				
		

	/*A controle des parametres d'entree */
	if @va_Debut_in is null
		return XDC_ARG_INV

	select @vl_Fin = dateadd(month, 1, @va_Debut_in), @vl_DateCourante = getdate()


	if @vl_DateCourante >  @vl_Fin
		select  @vl_DateButee = @vl_Fin
	else	select  @vl_DateButee = getdate()


	/* creer et renseigner une table temporaires des sites (regions, secteurs, districts) */
	create table #LISTE_SITES ( numero tinyint null )

	insert #LISTE_SITES ( numero )  values ( @va_District_in )



	/* table de stockage des evenements */
/*	create table #LISTE_FMC (numero int, cle tinyint, debut datetime, fin datetime null,
		duree int null , autoroute char(4) default XDC_CHAINE_VIDE, PR int  null, 
		sens tinyint null, nom_sens varchar(100) null, point_caracteristique varchar(100) null, type tinyint, 
		nom_type char(25), fausse_alerte tinyint null,
		nombre_de_vl tinyint default 0, nombre_de_pl tinyint default 0, 
		nombre_de_car tinyint default 0, nombre_de_moto tinyint default 0, 
		nombre_de_remorques tinyint default 0,
		morts tinyint default 0, blesses_legers tinyint default 0, 
		blesses_graves tinyint default 0,
		cause int null, cle_cause tinyint null, 
		voies_neutralisees tinyint default 0,
		longueur int null, type_bouchon char(15) default XDC_CHAINE_VIDE, 
		duree_bouchon int null, origine char(25) null, commentaires char(250) null, 
		type_meteo tinyint null, alerte int null)*/

	/* table de calcul du nombre max de voies neutralisees ou bloquees neutralisees */
/*	create table #VOIES_NEUTRALISEES (VR tinyint null, VM1 tinyint null, VM2 tinyint null, VL tinyint null, 
					VR_I tinyint null, VM1_I tinyint null, VM2_I tinyint null, VL_I tinyint null, total tinyint null)*/

	/*A table des PRs des bouchons */
	create table #BOU_PR (numero int, cle tinyint, horodate datetime null, PR int null, cause int null, cle_cause tinyint null)

	create table #GRA_BOU ( horodate datetime , PR_debut int , PR_fin int, longueur int null)

	/*A rechercher les fmc du mois */
	select @vl_Texte=null
	select @vl_TypePointCar=null
	select @vl_Position=null
	select @vl_auto = null
	select 
		numero = HIS..FMC_GEN.numero,
		cle = HIS..FMC_GEN.cle,
		debut=debut,
		fin=fin,
		duree=null,
		autoroute=@vl_auto,
		PR = null,
		sens=null,
		nom_sens=XDC_CHAINE_VIDE,
		point_caracteristique=@vl_TypePointCar,
		position=@vl_Position,
		type=HIS..FMC_GEN.type,
		nom_type=CFG..TYP_FMC.nom,
		fausse_alerte=null,
		nombre_de_vl=0,
		nombre_de_pl=0,
		nombre_de_car=0,
		nombre_de_moto=0,
		nombre_de_remorques=0,
		morts=0,
		blesses_legers=0,
		blesses_graves=0,
		cause=cause,
		cle_cause=cle_cause,
		voies_neutralisees=0,
		longueur=null,
		type_bouchon="                          ",
		duree_bouchon=null,
		origine=origine_creation,
		commentaires= @vl_Texte,
		type_meteo=null,
		alerte=alerte,
		site_alerte=sit_alerte,
		BAU=0,
		VL=0,
		VM1=0,
		VM2=0,
		VR=0,
		VR_I=0,
		VM2_I=0,
		VM1_I=0,
		VL_I=0,
		BAU_I=0

	into #LISTE_FMC
	from HIS..FMC_GEN, CFG..TYP_FMC
	where	type != XZAEC_FMC_TeteBouchon and
		type != XZAEC_FMC_PosteOperateur and
		type != XZAEC_FMC_Travaux and
		((datex<>XDC_VRAI and @va_District_in <> 99) or (datex=XDC_VRAI and @va_District_in =99)) and 
		debut >= @va_Debut_in and debut < @vl_Fin and
		CFG..TYP_FMC.numero = HIS..FMC_GEN.type
	order by HIS..FMC_GEN.numero

	/* suppression des FMC sur A8 cote ASF */
	delete  #LISTE_FMC from  #LISTE_FMC L where 
	L.numero in ( 
		select his.numero
		from HIS..FMC_HIS his
		left join HIS..FMC_GEN gen on his.numero = gen.numero and his.cle = gen.cle								
		where his.autoroute = 1 and his.PR < 17900 and his.type_FMC_valide = 20 and
		gen.cause in (select his.numero from HIS..FMC_HIS his where his.autoroute = 1 and his.PR < 17900 and his.type_FMC_valide = 37)
		and gen.cle_cause in (select his.cle from HIS..FMC_HIS his where his.autoroute = 1 and his.PR < 17900 and his.type_FMC_valide = 37)
	)

	/*A Ajout de l'origine de l'alerte : PAU, DAI ou ... */
	update #LISTE_FMC set origine = libelle
	from #LISTE_FMC,CFG..LIB_PRE,HIS..ALT_EXP
	where alerte is not null and
		CFG..LIB_PRE.notyp = TYP_EQT and
		HIS..ALT_EXP.numero = #LISTE_FMC.alerte and
		HIS..ALT_EXP.sit = #LISTE_FMC.site_alerte and
		CFG..LIB_PRE.code = HIS..ALT_EXP.type_equipement

	select numero,
		cle,
		type,
		cause,
		cle_cause
	into #COPIE
	from #LISTE_FMC

	declare pointeur cursor
	for select 
		numero,
		cle,
		type,
		cause,
		cle_cause
	from #COPIE

	open pointeur
	fetch pointeur into @vl_NumEvt, @vl_CleEvt, @vl_Type, 
		@vl_Cause, @vl_CleCause

	while @@sqlstatus = 0
	begin

	   /*B rechercher la derniere position */
	   select
		@vl_NumAutoroute = HIS..FMC_HIS.autoroute,
		@vl_Autoroute = CFG..RES_AUT.nom,
		@vl_PR = HIS..FMC_HIS.PR,
		@vl_Sens = HIS..FMC_HIS.sens,
		@vl_PointCar = HIS..FMC_HIS.point_caracteristique,
		@vl_PositionCar = HIS..FMC_HIS.position_sur_aire_ou_echangeur,
		@vl_fausse_alerte = fausse_alerte
	   from HIS..FMC_HIS, CFG..RES_AUT, HIS..FMC_GEN
	   where HIS..FMC_HIS.numero = @vl_NumEvt and HIS..FMC_HIS.cle = @vl_CleEvt and
		HIS..FMC_GEN.numero = HIS..FMC_HIS.numero and HIS..FMC_GEN.cle = HIS..FMC_HIS.cle and
		CFG..RES_AUT.numero = HIS..FMC_HIS.autoroute and
		horodate_validation in ( select max(horodate_validation)
					from HIS..FMC_HIS
					where numero = @vl_NumEvt and cle = @vl_CleEvt)

	  if @@rowcount = 0
		select @vl_Suppression = XDC_VRAI
	  else	select @vl_Suppression = XDC_FAUX
	
	  select @vl_Position=null

	  /*B si bouchon alors recherche le PR initial de la tete de bouchon */
	  if @vl_Type = XZAEC_FMC_QueueBouchon
		select 
			@vl_PR = PR
		from HIS..FMC_HIS
		where numero = @vl_Cause and cle = @vl_CleCause
		group by numero, cle
		having numero = @vl_Cause and cle = @vl_CleCause and
			horodate_validation = min(horodate_validation)

		/* Limitation des PR au reseau ESCOTA pour l'A8*/
		if (@vl_Autoroute = "A8" and @vl_PR < 17900)
			select @vl_PR = 17900

	   /* recherche du district */
	   select
		@vl_Site = district
	   from CFG..RES_POR, CFG..RES_DIS
	   where autoroute = @vl_NumAutoroute and
		PR_debut <= @vl_PR and
		PR_fin > @vl_PR and
		CFG..RES_DIS.numero = district

	   /* fmc en limite de concession */
	   if @@rowcount = 0
		select @vl_Site = district
	   	from CFG..RES_POR, CFG..RES_DIS
	   	where autoroute = @vl_NumAutoroute and
			PR_fin = @vl_PR and
			CFG..RES_DIS.numero = district
		
	   if ( @vl_Site in (select numero from #LISTE_SITES) or @va_District_in = 99 ) and @vl_Suppression = XDC_FAUX
	   begin
		select @vl_Duree = isnull(datediff(minute, debut, fin), datediff(minute, debut, @vl_DateButee))
		from #LISTE_FMC
		where numero = @vl_NumEvt and cle = @vl_CleEvt

		if @vl_Sens = XDC_SENS_INCONNU
			select @vl_Sens = XDC_SENS_INCONNU
		else if @vl_Sens % 2 = XDC_SENS_SUD
			select @vl_Sens = XDC_SENS_SUD
		else
			select @vl_Sens = XDC_SENS_NORD

		select
			@vl_NomSens = libelle
		from CFG..LIB_PRE
		where notyp = XDC_LIB_SENS_LOC and code = @vl_Sens

		if @vl_PointCar is not null
		begin	
			select @vl_TypePointCar = libelle
			from CFG..LIB_PRE
			where notyp = XDC_LIB_TYPE_POINT_CAR and code = @vl_PointCar

			select @vl_Position = libelle
			from CFG..LIB_PRE
			where notyp = XDC_LIB_TYPE_POS_AIR_ECH and code = @vl_PositionCar
			if @vl_Position =null
				select @vl_Position = "Non definie"
		end
		else	select @vl_TypePointCar = null

		update #LISTE_FMC set
			duree = @vl_Duree,
			autoroute = @vl_Autoroute,
			fausse_alerte = @vl_fausse_alerte,
			PR = @vl_PR,
			sens = @vl_Sens,
			nom_sens = @vl_NomSens,
			point_caracteristique = @vl_TypePointCar,
			position = @vl_Position
		where numero = @vl_NumEvt and cle = @vl_CleEvt 

		/*B pour une obstacle rechercher son type */
		if @vl_Type = XZAEC_FMC_Obstacle
		begin
			select @vl_nature=XDC_CHAINE_VIDE

			select @vl_nature=nature
			from HIS..FMC_NAT
			where 
				numero = @vl_NumEvt and
				cle = @vl_CleEvt  
			group by numero,cle
			having 
				numero = @vl_NumEvt and
				cle = @vl_CleEvt  and horodate=max(horodate)

			update #LISTE_FMC set type_bouchon=@vl_nature
			where #LISTE_FMC.numero = @vl_NumEvt and #LISTE_FMC.cle = @vl_CleEvt 
		end

		/*B pour une perturbation rechercher son type */
		if @vl_Type = XZAEC_FMC_Meteo
		begin
			update #LISTE_FMC set type_meteo = type_de_perturbation
			from HIS..FMC_MET
			where #LISTE_FMC.numero = @vl_NumEvt and #LISTE_FMC.cle = @vl_CleEvt and
				HIS..FMC_MET.numero = #LISTE_FMC.numero and
				HIS..FMC_MET.cle = #LISTE_FMC.cle
		end

		/*B pour un accident rechercher les infos de l'accident */
		if @vl_Type = XZAEC_FMC_Accident
		begin
/*select numero,cle,validation from HIS..FMC_ACC
			where numero = @vl_NumEvt and
				cle = @vl_CleEvt
			group by numero,cle
			having validation=max(validation)*/
			select @vl_nombre_de_vl = isnull(HIS..FMC_ACC.nombre_de_vl,0),
				@vl_nombre_de_pl = isnull(HIS..FMC_ACC.nombre_de_pl,0),
				@vl_nombre_de_remorques = isnull(HIS..FMC_ACC.nombre_de_remorques,0),
				@vl_nombre_de_car = isnull(HIS..FMC_ACC.nombre_de_car,0),
				@vl_nombre_de_moto = isnull(HIS..FMC_ACC.nombre_de_moto,0),
				@vl_morts = isnull(HIS..FMC_ACC.morts,0),
				@vl_blesses_legers = isnull(HIS..FMC_ACC.blesses_legers,0),
				@vl_blesses_graves = isnull(HIS..FMC_ACC.blesses_graves,0)
			from HIS..FMC_ACC
			where numero = @vl_NumEvt and
				cle = @vl_CleEvt
			group by numero,cle
			having validation=max(validation)

			update #LISTE_FMC set
				nombre_de_vl = @vl_nombre_de_vl,
				nombre_de_pl = @vl_nombre_de_pl,
				nombre_de_remorques = @vl_nombre_de_remorques,
				nombre_de_car = @vl_nombre_de_car,
				nombre_de_moto = @vl_nombre_de_moto,
				morts = @vl_morts,
				blesses_legers = @vl_blesses_legers,
				blesses_graves = @vl_blesses_graves
			where #LISTE_FMC.numero = @vl_NumEvt and #LISTE_FMC.cle = @vl_CleEvt 

/*			select @vl_DateValidation=validation from HIS..FMC_ACC 
			where HIS..FMC_ACC.numero = @vl_NumEvt and HIS..FMC_ACC.cle = @vl_CleEvt
			having HIS..FMC_ACC.numero = @vl_NumEvt and HIS..FMC_ACC.cle = @vl_CleEvt and validation=max(validation)

			update #LISTE_FMC set
				nombre_de_vl = HIS..FMC_ACC.nombre_de_vl,
				nombre_de_pl = HIS..FMC_ACC.nombre_de_pl,
				nombre_de_remorques = HIS..FMC_ACC.nombre_de_remorques,
				nombre_de_car = HIS..FMC_ACC.nombre_de_car,
				nombre_de_moto = HIS..FMC_ACC.nombre_de_moto,
				morts = HIS..FMC_ACC.morts,
				blesses_legers = HIS..FMC_ACC.blesses_legers,
				blesses_graves = HIS..FMC_ACC.blesses_graves
			from HIS..FMC_ACC
			where #LISTE_FMC.numero = @vl_NumEvt and  #LISTE_FMC.cle = @vl_CleEvt and
				HIS..FMC_ACC.numero = #LISTE_FMC.numero and
				HIS..FMC_ACC.cle = #LISTE_FMC.cle and HIS..FMC_ACC.validation=@vl_DateValidation
		*/
	
		end

		/*B pour un vehicule rechercher les infos du vehicule */
		if @vl_Type = XZAEC_FMC_Contresens or 
		   @vl_Type = XZAEC_FMC_VehArrete or
		   @vl_Type = XZAEC_FMC_VehPanne or
		   @vl_Type = XZAEC_FMC_VehFeu
		begin
			select
				@vl_TypeVeh = type_de_vehicule
			from HIS..FMC_VEH
			where numero = @vl_NumEvt and cle = @vl_CleEvt

			if @vl_TypeVeh = XZAEC_VEH_PL
				update #LISTE_FMC set nombre_de_pl = 1
				where numero = @vl_NumEvt and cle = @vl_CleEvt
			else if @vl_TypeVeh = XZAEC_VEH_VL
				update #LISTE_FMC set nombre_de_vl = 1
				where numero = @vl_NumEvt and cle = @vl_CleEvt
			else if @vl_TypeVeh = XZAEC_VEH_Car
				update #LISTE_FMC set nombre_de_car = 1
				where numero = @vl_NumEvt and cle = @vl_CleEvt
			else if @vl_TypeVeh = XZAEC_VEH_Moto
				update #LISTE_FMC set nombre_de_moto = 1
				where numero = @vl_NumEvt and cle = @vl_CleEvt
			else if @vl_TypeVeh = XZAEC_VEH_Remorque
				update #LISTE_FMC set nombre_de_remorques = 1
				where numero = @vl_NumEvt and cle = @vl_CleEvt
		end

		/*B supprimer les basculements dus a des travaux */
		if @vl_Type = XZAEC_FMC_Basculement
		begin
			if exists (select numero from HIS..FMC_GEN
				where numero = @vl_Cause and cle = @vl_CleCause and
					type = XZAEC_FMC_Travaux and datex<>XDC_VRAI)
				delete #LISTE_FMC 
				where numero = @vl_NumEvt and cle = @vl_CleEvt 
		end

		if @vl_Type = XZAEC_FMC_QueueBouchon
		begin
			/*B recherche des PR min et max */
			insert #BOU_PR (numero, cle, horodate, PR)
			select
				HIS..FMC_HIS.numero,
				HIS..FMC_HIS.cle,
				HIS..FMC_HIS.horodate_validation,
				HIS..FMC_HIS.PR
			from HIS..FMC_GEN, HIS..FMC_HIS
			where	HIS..FMC_GEN.numero = @vl_Cause and
				HIS..FMC_GEN.cle = @vl_CleCause and
				HIS..FMC_GEN.datex<>XDC_VRAI and
				HIS..FMC_HIS.numero = HIS..FMC_GEN.numero and
				HIS..FMC_HIS.cle = HIS..FMC_GEN.cle

			insert #BOU_PR (numero, cle, horodate, PR, cause, cle_cause)
			select
				HIS..FMC_HIS.numero,
				HIS..FMC_HIS.cle,
				HIS..FMC_HIS.horodate_validation,
				HIS..FMC_HIS.PR,
				HIS..FMC_GEN.cause,
				HIS..FMC_GEN.cle_cause
			from HIS..FMC_GEN, HIS..FMC_HIS
			where   HIS..FMC_GEN.numero = @vl_NumEvt and
				HIS..FMC_GEN.cle = @vl_CleEvt and
				HIS..FMC_GEN.datex<>XDC_VRAI and
				HIS..FMC_HIS.numero = HIS..FMC_GEN.numero and
				HIS..FMC_HIS.cle = HIS..FMC_GEN.cle

			insert #GRA_BOU (horodate, PR_debut, PR_fin)
			select distinct
				Q.horodate,
				T.PR,
				Q.PR
				from #BOU_PR T, #BOU_PR Q
			where   T.numero = Q.cause and
				T.cle = Q.cle_cause and
				T.horodate <= Q.horodate
			group by T.numero,T.cle, Q.numero,Q.cle,Q.horodate
			having  T.numero = Q.cause and
				T.cle = Q.cle_cause and
				T.horodate <= Q.horodate and
				T.horodate = max(T.horodate)

			insert #GRA_BOU (horodate, PR_debut, PR_fin)
			select distinct
				T.horodate,
				T.PR,
				Q.PR
			from #BOU_PR T, #BOU_PR Q
			where   Q.cause = T.numero and
				Q.cle_cause = T.cle and
				Q.horodate <= T.horodate
			group by
				Q.numero,Q.cle,
				T.numero,T.cle,T.horodate
			having  Q.cause = T.numero and
				Q.cle_cause = T.cle and
				Q.horodate <= T.horodate and
				Q.horodate = max(Q.horodate)


			update #GRA_BOU set longueur = abs(PR_fin - PR_debut)

			select @vl_MinPR = 0, @vl_MaxPR = 0, @vl_Longueur = 0

			if @vl_Sens = XDC_SENS_SUD
			select @vl_MinPR = PR_debut, @vl_MaxPR = PR_fin
			from #GRA_BOU
			having longueur = max(longueur)

			if @vl_Sens = XDC_SENS_NORD
			select @vl_MaxPR = PR_debut, @vl_MinPR = PR_fin
			from #GRA_BOU
			having longueur = max(longueur)

			delete #BOU_PR
			delete #GRA_BOU
			

			/*B calcul de la longueur de bouchon en prenant en compte les PR fantomes */
			exec XZAO;22 @vl_NumAutoroute, @vl_MinPR, @vl_NumAutoroute, @vl_MaxPR ,@vl_Longueur output

			/*B cause du bouchon */
			select @vl_CauseBouch=cause, @vl_CleCauseBouch=cle_cause, @vl_TypeCauseBouch=type
			from HIS..FMC_GEN
			where	HIS..FMC_GEN.numero = @vl_Cause and 
				HIS..FMC_GEN.cle = @vl_CleCause and
				HIS..FMC_GEN.datex<>XDC_VRAI

			/*B niveau final du bouchon */
			select @vl_TypeBouchon = max(type)
			from HIS..FMC_BOU
			where HIS..FMC_BOU.numero = @vl_NumEvt and HIS..FMC_BOU.cle = @vl_CleEvt
			group by numero, cle
			having numero = @vl_NumEvt and cle = @vl_CleEvt and
				horodate = max(horodate)
				
			/*B rechercher le libelle associe au niveau final du bouchon */
			select @vl_Densite = libelle
			from CFG..LIB_PRE
			where notyp = XDC_LIB_TYPE_BOUCHON and code = @vl_TypeBouchon

			if @@rowcount = 0
				select @vl_Densite = XDC_CHAINE_VIDE

			select @vl_Cause=@vl_CauseBouch, @vl_CleCause=@vl_CleCauseBouch

			/*B si le bouchon est du a un autre incident alors renseigner     */
			/*B les champs de la cause pour ce bouchon sinon faire apparaitre */
			/*B le bouchon comme les autres incidents dans la liste           */
			if @vl_CauseBouch is not null and @vl_TypeCauseBouch != XZAEC_FMC_Travaux
			begin
				/*B si le bouchon est la premiere consequence alors le faire apparaitre sans cause */
				/*B sinon le faire apparaitre comme un evenement sans cause */
				if exists (select * from #LISTE_FMC
					   where numero = @vl_CauseBouch and cle = @vl_CleCauseBouch and 
						duree_bouchon is null)
				begin
/*DEM 1092					delete #LISTE_FMC
					where numero = @vl_NumEvt and cle = @vl_CleEvt
	*/
					if @vl_TypeCauseBouch != XZAEC_FMC_TeteBouchon
					begin
						update #LISTE_FMC set longueur = @vl_Longueur,
							PR = @vl_PR,
							duree_bouchon = @vl_Duree,
							type_bouchon = @vl_Densite
						where numero = @vl_CauseBouch and cle= @vl_CleCauseBouch
					end
				end
/*DEM1092				else*/
					update #LISTE_FMC set longueur = @vl_Longueur,
						PR = @vl_PR,
						duree = @vl_Duree,
						nom_type = XZAEC_FMC_LIB_BOU,
						type_bouchon = @vl_Densite
					where numero = @vl_NumEvt and cle= @vl_CleEvt
			end
			else
			begin
				update #LISTE_FMC set longueur = @vl_Longueur,
					PR = @vl_PR,
					duree = @vl_Duree,
					nom_type = XZAEC_FMC_LIB_BOU,
					type_bouchon = @vl_Densite
				where numero = @vl_NumEvt and cle= @vl_CleEvt
			end
			select 	@vl_Duree=0
		end
		else
		begin
			/*B recherche des voies neutralisees ou bloquees neutralisees */
			select 
				BAU,BAU_I,VR,
				VM1,
				VM2,
				VL,
				VR_I,
				VM1_I,
				VM2_I,
				VL_I,
				total=0
			into #VOIES_NEUTRALISEES
			from HIS..FMC_HIS
			where numero = @vl_NumEvt and cle= @vl_CleEvt and
				(BAU in ( XDC_VOIE_NEUTRALISEE , XDC_VOIE_BLOQUEE_NEUTRALISEE ,XDC_VOIE_BLOQUEE, XDC_VOIE_SENS_INVERSE) or
				VR in ( XDC_VOIE_NEUTRALISEE ,XDC_VOIE_BLOQUEE_NEUTRALISEE ,XDC_VOIE_BLOQUEE, XDC_VOIE_SENS_INVERSE) or
				VL in ( XDC_VOIE_NEUTRALISEE , XDC_VOIE_BLOQUEE_NEUTRALISEE , XDC_VOIE_BLOQUEE, XDC_VOIE_SENS_INVERSE) or
				VM1 in ( XDC_VOIE_NEUTRALISEE , XDC_VOIE_BLOQUEE_NEUTRALISEE , XDC_VOIE_BLOQUEE, XDC_VOIE_SENS_INVERSE) or
				VM2 in ( XDC_VOIE_NEUTRALISEE , XDC_VOIE_BLOQUEE_NEUTRALISEE , XDC_VOIE_BLOQUEE, XDC_VOIE_SENS_INVERSE) or
				BAU_I in (XDC_VOIE_NEUTRALISEE ,XDC_VOIE_BLOQUEE_NEUTRALISEE ,XDC_VOIE_BLOQUEE, XDC_VOIE_SENS_INVERSE) or
				VR_I in( XDC_VOIE_NEUTRALISEE , XDC_VOIE_BLOQUEE_NEUTRALISEE ,XDC_VOIE_BLOQUEE, XDC_VOIE_SENS_INVERSE) or
				VL_I in ( XDC_VOIE_NEUTRALISEE , XDC_VOIE_BLOQUEE_NEUTRALISEE ,XDC_VOIE_BLOQUEE, XDC_VOIE_SENS_INVERSE) or
				VM1_I in ( XDC_VOIE_NEUTRALISEE , XDC_VOIE_BLOQUEE_NEUTRALISEE , XDC_VOIE_BLOQUEE, XDC_VOIE_SENS_INVERSE) or
				VM2_I in ( XDC_VOIE_NEUTRALISEE , XDC_VOIE_BLOQUEE_NEUTRALISEE , XDC_VOIE_BLOQUEE, XDC_VOIE_SENS_INVERSE) )

			/*B calcul le nombre max de voies neutralisees ou bloquees neutralisees */
			/*! sans utiliser de curseur */
			if @@rowcount > 0
			begin	
				update #VOIES_NEUTRALISEES set BAU = 0
				where BAU not in ( XDC_VOIE_NEUTRALISEE , XDC_VOIE_BLOQUEE_NEUTRALISEE ,XDC_VOIE_BLOQUEE, XDC_VOIE_SENS_INVERSE) 
				update #VOIES_NEUTRALISEES set BAU_I = 0
				where BAU_I not in ( XDC_VOIE_NEUTRALISEE , XDC_VOIE_BLOQUEE_NEUTRALISEE ,XDC_VOIE_BLOQUEE, XDC_VOIE_SENS_INVERSE)
				update #VOIES_NEUTRALISEES set VR = 0
				where VR not in ( XDC_VOIE_NEUTRALISEE , XDC_VOIE_BLOQUEE_NEUTRALISEE ,XDC_VOIE_BLOQUEE, XDC_VOIE_SENS_INVERSE) 
				update #VOIES_NEUTRALISEES set VL = 0
				where VL  not in ( XDC_VOIE_NEUTRALISEE , XDC_VOIE_BLOQUEE_NEUTRALISEE ,XDC_VOIE_BLOQUEE, XDC_VOIE_SENS_INVERSE)
				update #VOIES_NEUTRALISEES set VM1 = 0
				where VM1 not in ( XDC_VOIE_NEUTRALISEE , XDC_VOIE_BLOQUEE_NEUTRALISEE ,XDC_VOIE_BLOQUEE, XDC_VOIE_SENS_INVERSE) 
				update #VOIES_NEUTRALISEES set VM2 = 0
				where VM2 not in ( XDC_VOIE_NEUTRALISEE , XDC_VOIE_BLOQUEE_NEUTRALISEE ,XDC_VOIE_BLOQUEE, XDC_VOIE_SENS_INVERSE) 
				update #VOIES_NEUTRALISEES set VR_I = 0
				where VR_I not in ( XDC_VOIE_NEUTRALISEE , XDC_VOIE_BLOQUEE_NEUTRALISEE ,XDC_VOIE_BLOQUEE, XDC_VOIE_SENS_INVERSE) 
				update #VOIES_NEUTRALISEES set VL_I = 0
				where VL_I not in ( XDC_VOIE_NEUTRALISEE , XDC_VOIE_BLOQUEE_NEUTRALISEE ,XDC_VOIE_BLOQUEE, XDC_VOIE_SENS_INVERSE) 
				update #VOIES_NEUTRALISEES set VM1_I = 0
				where VM1_I not in ( XDC_VOIE_NEUTRALISEE , XDC_VOIE_BLOQUEE_NEUTRALISEE ,XDC_VOIE_BLOQUEE, XDC_VOIE_SENS_INVERSE)
				update #VOIES_NEUTRALISEES set VM2_I = 0
				where VM2_I not in ( XDC_VOIE_NEUTRALISEE , XDC_VOIE_BLOQUEE_NEUTRALISEE ,XDC_VOIE_BLOQUEE, XDC_VOIE_SENS_INVERSE)
	
				if exists (select * from #VOIES_NEUTRALISEES
					where VR > 0 and VL > 0 and VM1 > 0 and VM2 > 0 and
					VR_I  > 0 and VL_I > 0 and VM1_I > 0 and VM2_I> 0)
					update #LISTE_FMC set voies_neutralisees = 8
					where numero = @vl_NumEvt and cle= @vl_CleEvt
				else 
				begin
					update #VOIES_NEUTRALISEES set total = BAU+BAU_I+VR+VL+VM1+VM2+VR_I+VL_I+VM1_I+VM2_I 

					select @vl_Total = max(total)
					from #VOIES_NEUTRALISEES

					if @vl_Total >= 7 * XDC_VOIE_NEUTRALISEE
						update #LISTE_FMC set voies_neutralisees = 7
						where numero = @vl_NumEvt and cle= @vl_CleEvt
					else if @vl_Total >= 6 * XDC_VOIE_NEUTRALISEE
						update #LISTE_FMC set voies_neutralisees = 6
						where numero = @vl_NumEvt and cle= @vl_CleEvt
					else if @vl_Total >= 5 * XDC_VOIE_NEUTRALISEE
						update #LISTE_FMC set voies_neutralisees = 5
						where numero = @vl_NumEvt and cle= @vl_CleEvt
					else if @vl_Total >= 4 * XDC_VOIE_NEUTRALISEE
						update #LISTE_FMC set voies_neutralisees = 4
						where numero = @vl_NumEvt and cle= @vl_CleEvt
					else if @vl_Total >= 3 * XDC_VOIE_NEUTRALISEE
						update #LISTE_FMC set voies_neutralisees = 3
						where numero = @vl_NumEvt and cle= @vl_CleEvt
					else if @vl_Total >= 2 * XDC_VOIE_NEUTRALISEE
						update #LISTE_FMC set voies_neutralisees = 2
						where numero = @vl_NumEvt and cle= @vl_CleEvt
					else
						update #LISTE_FMC set voies_neutralisees = 1
						where numero = @vl_NumEvt and cle= @vl_CleEvt
				end
			end
			/*A Recherche des configuration de voies les pires */
			select @vl_cpt=0
			select @vl_cpt = count(*) from  #VOIES_NEUTRALISEES
			if @vl_cpt > 0 
			begin
/*select @vl_cpt,"=================="
select  @vl_NumEvt,* from #VOIES_NEUTRALISEES
select  @vl_NumEvt,* from #VOIES_NEUTRALISEES having total = max(total)*/
			  select 
				@vl_BAU=BAU,
				@vl_BAU_I=BAU_I,
				@vl_VR=VR,
				@vl_VM1=VM1,
				@vl_VM2=VM2,
				@vl_VL=VL,
				@vl_VR_I=VR_I,
				@vl_VM1_I=VM1_I,
				@vl_VM2_I=VM2_I,
				@vl_VL_I=VL_I from #VOIES_NEUTRALISEES having total = max(total)
			  update  #LISTE_FMC set 
				BAU=@vl_BAU,
				VL=@vl_VL,
				VM1=@vl_VM1,
				VM2=@vl_VM2,
				VR=@vl_VR,
				VR_I=@vl_VR_I,
				VM2_I=@vl_VM2_I,
				VM1_I=@vl_VM1_I,
				VL_I=@vl_VL_I
			  where numero = @vl_NumEvt and cle= @vl_CleEvt

			end	
			
			drop table #VOIES_NEUTRALISEES	
		end
	   end
	   else
		delete #LISTE_FMC 
		where numero = @vl_NumEvt and cle = @vl_CleEvt 



	   /*B passage a l'evenement suivant */
	   fetch pointeur into @vl_NumEvt, @vl_CleEvt, @vl_Type, 
			@vl_Cause, @vl_CleCause
	end

	

	/*A retourner la liste des fmc d'un mois */
	select
		convert(char(10),debut,103)+' '+convert(char(8),debut,8),
		convert(char(10),fin,103)+' '+convert(char(8),fin,8),
		duree,
		autoroute,
		PR,
		nom_sens,
		nom_type,
		isnull(longueur,0),
		duree_bouchon,
		type_bouchon,
		nombre_de_vl,
		nombre_de_pl,
		nombre_de_car,
		nombre_de_moto,
		nombre_de_remorques,
		morts,
		blesses_graves,
		blesses_legers,
		isnull(fausse_alerte,XDC_FAUX),
		voies_neutralisees,
		origine,
		point_caracteristique,
		type_meteo,
		numero,
		cle,
		BAU,VL,VM1,VM2,VR,VR_I,VM2_I,VM1_I,VL_I,BAU_I,
		position
	from #LISTE_FMC
	order by debut, autoroute, PR, sens

	/*A retourner les commentaires des fmc */
	select
		evenement,
		HIS..FMC_COM.cle,
		texte
	from #LISTE_FMC,HIS..FMC_COM
	where HIS..FMC_COM.evenement = #LISTE_FMC.numero and
		HIS..FMC_COM.cle = #LISTE_FMC.cle
	order by evenement, HIS..FMC_COM.cle, HIS..FMC_COM.numero

	return XDC_OK
go
