/*E*/
/*  Fichier : $Id: xzae01sp.prc,v 1.18 2013/03/05 14:05:07 gesconf Exp $      Release : $Revision: 1.18 $        Date : $Date: 2013/03/05 14:05:07 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae01sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	30/09/94	: Creation
* C.T.	10/11/94	: Modif null pour applix (1.2)
* C.T.	14/11/94	: recherche des infos ds HIS au CI et
*                         ds EXP aux districts	 (1.3)
* B.G.	20/12/94	: Suppr modif null pour Evt_Operation (1.4)
* C.T.	22/12/94	: Retourner seulement les infos 
*                         pour la localisation initiale (1.5)
*                         non teste
* C.T.	03/01/95	: Ajout dans la date de fin d'evenement
*                         dans les arguments retournes
*                         non teste
* B.G.	20/12/94	: Suppr modif null pour Evt_Operation district (1.8)
* C.T.  29/01/95        : Decoupage du select de recherche evts (1.9)
* C.T.  15/02/95        : Correction clause du select (1.10)
* C.T.  16/02/95        : Correction clause du select pour SATIR (1.11)
*                         (tester tjours sur date de fin si non nulle)
* C.T.  20/02/95        : Correction clause du select pour debut et 
*                         debut_prevu a null(1.12)
* C.T.  05/02/96        : Ajout critere de recherches des fmc sur plusieurs
*                         types de FMC (1.13)
* C.T.  26/03/96        : Ajout critere de recherches des fmc sur plusieurs
*                         types de FMC pour la proc des districts (1.14)
* C.P.	10/06/98	: Separation EXP et HIS (V 1.15)
* PN	23/02/99	: Prise en compte du district 99 pour les FMC "datex" dem/1724 v1.16
* PN	11/03/05	: Prise en cpte de la derniere validation pour la localisation DEM425 v1.17
* PNI	05/03/2013	: Suppression du ; pour compilation plus souple pour FE 1890 v1.18
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne la liste des evenements correspondant aux criteres
* 
* Sequence d'appel
* SP	XZAE01_Liste_Evts
* 
* Arguments en entree
* XDY_Autoroute	va_Autoroute_in
* XDY_PR	va_PR1_in
* XDY_PR	va_PR2_in
* XDY_Sens	va_Sens_in
* XDY_District	va_District_in
* XDY_Horodate	va_HorodateDebut_in
* XDY_Horodate	va_HorodateFin_in
* XDY_Octet	va_Evt_Operation_in
* XDY_Entier	va_Type1_in
* XDY_Entier	va_Typ2_in
* 
* Arguments en sortie
* XDC_Octet	va_Cle_out		-- arguments retournes 
* XDC_Entier	va_Numero_out		-- dans une liste
* XDC_Octet	va_Type_out
* XDC_Autoroute	va_Autoroute_out
* XDC_PR	va_PR_out
* XDC_Sens	va_Sens_out
* XDC_Horodate	va_HorodateDebut_out
* XDC_Horodate	va_HorodateFin_out
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* L'argument evt_operation est obligatoire.
* Lorsqu'une localisation est precisee, seuls les criteres suivants
* au niveau de la localisation sont admis : 
* - district precise, autoroute, PR et sens a null
* - autoroute, PR precises, district a null (le sens est facultatif)
* 
* La procedure stockee au CI recherche les informations dans la base HIS
* tandis qu'au niveau des districts la recherche s'effectue dans la base EXP.
* Le code de creation de la procedure au CI et aux districts doit etre identique
* (toute modif dans la creation de la procedure doit etre effectuee pour le CI et
*  les districts)
* 
* 
* Fonction
* Recherche des evenements verifiant :
* 	localisation sur Autoroute, entre PR1 et PR2, avec le sens Sens
* ET/OU
* 	localisation dans District
* ET/OU
* 	HorodateDebut	<	heure evenement	<	HorodateFin
* ET/OU
* 	Evenement ou Operation ?
* ET/OU
* 	Type = type de l'evenement
----------------------------------------------------------*/

use PRC
go
drop proc XZAE01
go

#ifdef HIST

create procedure XZAE01
	@va_Autoroute_in	T_AUTOROUTE	= null,
	@va_PR1_in		T_PR		= null,
	@va_PR2_in		T_PR		= null,
	@va_Sens_in		T_SENS		= null,
	@va_District_in		T_SITE		= null,
	@va_HorodateDebut_in	datetime	= null,
	@va_HorodateFin_in	datetime	= null,
	@va_Evt_Operation_in	tinyint		= null,
	@va_Type1_in		int		= null,
	@va_Type2_in		int		= null
as
	declare @vl_Numero int, @vl_Cle tinyint, @vl_Type tinyint, 
		@vl_Debut datetime, @vl_Fin datetime, 
		@vl_Autoroute T_AUTOROUTE, @vl_PR T_PR, @vl_Sens T_SENS

	/*! transfo en paramÉtres nulls bug applix */
	if @va_HorodateDebut_in = XDC_DATE_NULLE select @va_HorodateDebut_in = null
	if @va_HorodateFin_in = XDC_DATE_NULLE select @va_HorodateFin_in = null
	if @va_District_in = 0 select @va_District_in = null
	if @va_Autoroute_in = XDC_AUT_INCONNUE select @va_Autoroute_in = null
	if @va_PR2_in = XDC_PR_INCONNU select @va_PR2_in = null
	if @va_PR1_in = XDC_PR_INCONNU select @va_PR1_in = null
	if @va_Sens_in = XDC_SENS_INCONNU select @va_Sens_in = null

	/*A controle les parametres obligatoires */
	if @va_Evt_Operation_in = null or
	   (@va_HorodateDebut_in = null and @va_HorodateFin_in != null) or
	   (@va_HorodateDebut_in != null and @va_HorodateFin_in = null) or
	   (@va_District_in != null and @va_Autoroute_in != null) or
	   ((@va_PR1_in = null or @va_PR2_in = null) and @va_Autoroute_in != null) or
	   ((@va_PR1_in != null or @va_PR2_in != null) and @va_Autoroute_in = null) or
	   (@va_PR1_in != null and @va_PR2_in = null) or
	   (@va_PR1_in = null and @va_PR2_in != null) or
	   (@va_Sens_in != null and @va_Sens_in != XDC_SENS_NORD and 
	    @va_Sens_in != XDC_SENS_SUD)
		return XDC_ARG_INV

	/*A liste des evenements verifiant les criteres de date */
	/*A si le district est 99 alors on prend les FMC datex sinon les FMC escota */
	select
		HIS..FMC_GEN.cle,
		HIS..FMC_GEN.numero,
		HIS..FMC_GEN.type,
		debut = isnull(HIS..FMC_GEN.debut, HIS..FMC_GEN.debut_prevu),
		HIS..FMC_GEN.fin
	into #LISTE_EVT
	from HIS..FMC_GEN
	where	(HIS..FMC_GEN.debut is not null or
		 HIS..FMC_GEN.debut_prevu is not null) and
		 
		((@va_District_in = 99 and datex = XDC_VRAI) or
		 (@va_District_in != 99 and datex != XDC_VRAI) ) and
		
		(@va_HorodateDebut_in is null or

		 ((HIS..FMC_GEN.debut is not null and
		   (@va_HorodateDebut_in <= HIS..FMC_GEN.fin or 
		    HIS..FMC_GEN.fin is null) and
		   @va_HorodateFin_in > HIS..FMC_GEN.debut) or

		  (HIS..FMC_GEN.debut is null and 
		   (HIS..FMC_GEN.fin is null or 
		    @va_HorodateDebut_in < HIS..FMC_GEN.fin) and
		   (HIS..FMC_GEN.debut_prevu is null or
		    ((HIS..FMC_GEN.fin_prevue is null and
		      HIS..FMC_GEN.debut_prevu < @va_HorodateFin_in) or 
		     (@va_HorodateDebut_in <= HIS..FMC_GEN.fin_prevue and
		      @va_HorodateFin_in > HIS..FMC_GEN.debut_prevu))))
		 )
		)
	
	/*A supprimer de la liste des evenements, les evts ne verifiant pas le critere de type operation ou evenement */
	if @va_Evt_Operation_in != XDC_PAS_TYP_FMC
		delete #LISTE_EVT
		from CFG..TYP_FMC
		where	CFG..TYP_FMC.operation = @va_Evt_Operation_in and
			CFG..TYP_FMC.numero = #LISTE_EVT.type

	/*A supprimer de la liste des evenements, les evts ne verifiant pas les criteres de type fmc */
	/*! int sur sybase est sur 30 bits */
	select @vl_Type = XZAEC_FMC_Accident
	if @va_Type1_in is not null or 
		@va_Type1_in = XDC_TOUS_TYP_FMC
	begin
		while @vl_Type < 31
		begin
			if @va_Type1_in % 2 = 0
				delete #LISTE_EVT
				from CFG..TYP_FMC
				where type = @vl_Type
			else select @va_Type1_in = (@va_Type1_in - 1)

			/*B type suivant */
			select @vl_Type = @vl_Type + 1,
				@va_Type1_in = @va_Type1_in / 2
		end
	end
	if @va_Type2_in is not null or
		@va_Type2_in = XDC_TOUS_TYP_FMC
	begin
		while @vl_Type <= XZAEC_FMC_NB_TYP
		begin
			if @va_Type2_in % 2 = 0
				delete #LISTE_EVT
				from CFG..TYP_FMC
				where type = @vl_Type
			else select @va_Type2_in = (@va_Type2_in - 1)

			/*B type suivant */
			select @vl_Type = @vl_Type + 1,
				@va_Type2_in = @va_Type2_in / 2
		end
	end

		   
	/*A Rechercher pour la premiere localisation correspondant aux criteres */
	/*A pour chaque evenement de la liste                                   */
	declare Pointeur_evt cursor
	for select distinct numero, cle, type, debut, fin
	from #LISTE_EVT
	where type != XZAEC_FMC_TeteBouchon
	open Pointeur_evt

	create table #LISTE ( numero int, cle tinyint, type smallint, autoroute tinyint null, 
				PR int null, sens tinyint null, debut datetime,
				fin datetime null)

	fetch Pointeur_evt into @vl_Numero, @vl_Cle, @vl_Type, @vl_Debut, @vl_Fin
	while @@sqlstatus = 0
	begin
		/*A Tester si l'evenement est sur la localisation demandee */
		if exists (	select * from HIS..FMC_HIS, CFG..RES_POR
				where	HIS..FMC_HIS.numero = @vl_Numero and
					HIS..FMC_HIS.cle = @vl_Cle and
					((@va_District_in is null and (@va_Autoroute_in is null or
			   			(HIS..FMC_HIS.autoroute = @va_Autoroute_in and 
			   			HIS..FMC_HIS.PR >= @va_PR1_in and HIS..FMC_HIS.PR <= @va_PR2_in and
			   			(@va_Sens_in is null or (HIS..FMC_HIS.sens & @va_Sens_in =  @va_Sens_in))))) or 
			   		(@va_District_in is not null and (@va_District_in = CFG..RES_POR.district or @va_District_in = 99))) and
					(((HIS..FMC_HIS.autoroute is null or HIS..FMC_HIS.PR is null) and 
			  		@va_District_in is null and @va_Autoroute_in is null) or
			  		(HIS..FMC_HIS.autoroute = CFG..RES_POR.autoroute and
			   		HIS..FMC_HIS.PR >= CFG..RES_POR.PR_debut and 
			   		HIS..FMC_HIS.PR <= CFG..RES_POR.PR_fin )))
		begin
			/*B Rechercher pour la derniere localisation DEM425*/
			select distinct
				@vl_Autoroute = autoroute,
				@vl_PR = PR,
				@vl_Sens = sens
			from HIS..FMC_HIS
			where	numero = @vl_Numero and cle = @vl_Cle and 
				horodate_validation in ( select max(horodate_validation) from HIS..FMC_HIS
							where numero = @vl_Numero and cle = @vl_Cle)

			/* Pas de localisation pour un evenement de type poste operateur */
			if @vl_Type = XZAEC_FMC_PosteOperateur
				select @vl_Autoroute = null, @vl_PR = null, @vl_Sens = null

			insert #LISTE (numero, cle, type, autoroute, PR, sens, debut, fin)
			values(@vl_Numero, @vl_Cle, @vl_Type, @vl_Autoroute, @vl_PR, @vl_Sens, 
				@vl_Debut, @vl_Fin)
		end

		fetch Pointeur_evt into @vl_Numero, @vl_Cle, @vl_Type, @vl_Debut, @vl_Fin
	end
	close Pointeur_evt

	/*A Retourner la liste des evenements concernes */
	select 
		cle,
		numero,
		type,
		autoroute,
		PR,
		sens,
		debut,
		fin
	from #LISTE
	order by debut desc

	return XDC_OK

#else

create procedure XZAE01
	@va_Autoroute_in	T_AUTOROUTE	= null,
	@va_PR1_in		T_PR		= null,
	@va_PR2_in		T_PR		= null,
	@va_Sens_in		T_SENS		= null,
	@va_District_in		T_SITE		= null,
	@va_HorodateDebut_in	datetime	= null,
	@va_HorodateFin_in	datetime	= null,
	@va_Evt_Operation_in	tinyint		= null,
	@va_Type1_in		int		= null,
	@va_Type2_in		int		= null
as
	declare @vl_Numero int, @vl_Cle tinyint, @vl_Type tinyint, 
		@vl_Debut datetime, @vl_Fin datetime, 
		@vl_Autoroute T_AUTOROUTE, @vl_PR T_PR, @vl_Sens T_SENS

	/*! transfo en paramÉtres nulls bug applix */
	if @va_HorodateDebut_in = XDC_DATE_NULLE select @va_HorodateDebut_in = null
	if @va_HorodateFin_in = XDC_DATE_NULLE select @va_HorodateFin_in = null
	if @va_District_in = 0 select @va_District_in = null
	if @va_Autoroute_in = XDC_AUT_INCONNUE select @va_Autoroute_in = null
	if @va_PR2_in = XDC_PR_INCONNU select @va_PR2_in = null
	if @va_PR1_in = XDC_PR_INCONNU select @va_PR1_in = null
	if @va_Sens_in = XDC_SENS_INCONNU select @va_Sens_in = null

	/*A controle les parametres obligatoires */
	if @va_Evt_Operation_in = null or
	   (@va_HorodateDebut_in = null and @va_HorodateFin_in != null) or
	   (@va_HorodateDebut_in != null and @va_HorodateFin_in = null) or
	   (@va_District_in != null and @va_Autoroute_in != null) or
	   ((@va_PR1_in = null or @va_PR2_in = null) and @va_Autoroute_in != null) or
	   ((@va_PR1_in != null or @va_PR2_in != null) and @va_Autoroute_in = null) or
	   (@va_PR1_in != null and @va_PR2_in = null) or
	   (@va_PR1_in = null and @va_PR2_in != null) or
	   (@va_Sens_in != null and @va_Sens_in != XDC_SENS_NORD and @va_Sens_in != XDC_SENS_SUD)
		return XDC_ARG_INV

	/*A liste des evenements verifiants les criteres */
	/*A si le district est 99 alors on prend les FMC datex sinon les FMC escota */
	select
		EXP..FMC_GEN.cle,
		EXP..FMC_GEN.numero,
		EXP..FMC_GEN.type,
		debut = isnull(EXP..FMC_GEN.debut, EXP..FMC_GEN.debut_prevu),
		EXP..FMC_GEN.fin
	into #LISTE_EVT
	from EXP..FMC_GEN
	where	(EXP..FMC_GEN.debut is not null or
		 EXP..FMC_GEN.debut_prevu is not null) and
		 
		((@va_District_in = 99 and datex = XDC_VRAI) or
		 (@va_District_in != 99 and datex != XDC_VRAI) ) and
		 
		(@va_HorodateDebut_in is null or

		 ((EXP..FMC_GEN.debut is not null and
		   (@va_HorodateDebut_in <= EXP..FMC_GEN.fin or 
		    EXP..FMC_GEN.fin is null) and
		   @va_HorodateFin_in > EXP..FMC_GEN.debut) or

		  (EXP..FMC_GEN.debut is null and 
		   (EXP..FMC_GEN.fin is null or 
		    @va_HorodateDebut_in < EXP..FMC_GEN.fin) and
		   (EXP..FMC_GEN.debut_prevu is null or
		    ((EXP..FMC_GEN.fin_prevue is null and
		      EXP..FMC_GEN.debut_prevu < @va_HorodateFin_in) or 
		     (@va_HorodateDebut_in <= EXP..FMC_GEN.fin_prevue and
		      @va_HorodateFin_in > EXP..FMC_GEN.debut_prevu))))
		 )
		)
	 
	/*A supprimer de la liste des evenements, les evts ne verifiant pas le critere de type operation ou evenement */
	if @va_Evt_Operation_in != XDC_PAS_TYP_FMC
		delete #LISTE_EVT
		from CFG..TYP_FMC
		where	CFG..TYP_FMC.operation = @va_Evt_Operation_in and
			CFG..TYP_FMC.numero = #LISTE_EVT.type

	/*A supprimer de la liste des evenements, les evts ne verifiant pas les criteres de type fmc */
	/*! int sur sybase est sur 30 bits */
	select @vl_Type = XZAEC_FMC_Accident
	if @va_Type1_in is not null or 
		@va_Type1_in = XDC_TOUS_TYP_FMC
	begin
		while @vl_Type < 31
		begin
			if @va_Type1_in % 2 = 0
				delete #LISTE_EVT
				from CFG..TYP_FMC
				where type = @vl_Type
			else select @va_Type1_in = (@va_Type1_in - 1)

			/*B type suivant */
			select @vl_Type = @vl_Type + 1,
				@va_Type1_in = @va_Type1_in / 2
		end
	end
	if @va_Type2_in is not null or
		@va_Type2_in = XDC_TOUS_TYP_FMC
	begin
		while @vl_Type <= XZAEC_FMC_NB_TYP
		begin
			if @va_Type2_in % 2 = 0
				delete #LISTE_EVT
				from CFG..TYP_FMC
				where type = @vl_Type
			else select @va_Type2_in = (@va_Type2_in - 1)

			/*B type suivant */
			select @vl_Type = @vl_Type + 1,
				@va_Type2_in = @va_Type2_in / 2
		end
	end

		   
	/*A Rechercher pour la premiere localisation correspondant aux criteres */
	/*A pour chaque evenement de la liste                                   */
	declare Pointeur_evt cursor
	for select distinct numero, cle, type, debut, fin
	from #LISTE_EVT
	where type != XZAEC_FMC_TeteBouchon
	open Pointeur_evt

	create table #LISTE ( numero int, cle tinyint, type smallint, autoroute tinyint null, 
				PR int null, sens tinyint null, debut datetime,
				fin datetime null)

	fetch Pointeur_evt into @vl_Numero, @vl_Cle, @vl_Type, @vl_Debut, @vl_Fin
	while @@sqlstatus = 0
	begin
		/*A Tester si l'evenement est sur la localisation demandee */
		if exists (	select * from EXP..FMC_HIS, CFG..RES_POR
				where	EXP..FMC_HIS.numero = @vl_Numero and
					EXP..FMC_HIS.cle = @vl_Cle and
					((@va_District_in is null and (@va_Autoroute_in is null or
			   			(EXP..FMC_HIS.autoroute = @va_Autoroute_in and 
			   			EXP..FMC_HIS.PR >= @va_PR1_in and EXP..FMC_HIS.PR <= @va_PR2_in and
			   			(@va_Sens_in is null or (EXP..FMC_HIS.sens & @va_Sens_in =  @va_Sens_in))))) or 
			   		(@va_District_in is not null and (@va_District_in = CFG..RES_POR.district or @va_District_in = 99))) and
					(((EXP..FMC_HIS.autoroute is null or EXP..FMC_HIS.PR is null) and 
			  		@va_District_in is null and @va_Autoroute_in is null) or
			  		(EXP..FMC_HIS.autoroute = CFG..RES_POR.autoroute and
			   		EXP..FMC_HIS.PR >= CFG..RES_POR.PR_debut and 
			   		EXP..FMC_HIS.PR <= CFG..RES_POR.PR_fin )))
		begin
			/*B Rechercher pour la derniere localisation DEM425*/
			select distinct
				@vl_Autoroute = autoroute,
				@vl_PR = PR,
				@vl_Sens = sens
			from EXP..FMC_HIS
			where	numero = @vl_Numero and cle = @vl_Cle and
				horodate_validation in ( select max(horodate_validation) from EXP..FMC_HIS
							where numero = @vl_Numero and cle = @vl_Cle)

			/* Pas de localisation pour un evenement de type poste operateur */
			if @vl_Type = XZAEC_FMC_PosteOperateur
				select @vl_Autoroute = null, @vl_PR = null, @vl_Sens = null

			insert #LISTE (numero, cle, type, autoroute, PR, sens, debut, fin)
			values(@vl_Numero, @vl_Cle, @vl_Type, @vl_Autoroute, @vl_PR, @vl_Sens, 
				@vl_Debut, @vl_Fin)
		end

		fetch Pointeur_evt into @vl_Numero, @vl_Cle, @vl_Type, @vl_Debut, @vl_Fin
	end
	close Pointeur_evt

	/*A Retourner la liste des evenements concernes */
	select 
		cle,
		numero,
		type,
		autoroute,
		PR,
		sens,
		debut,
		fin
	from #LISTE
	order by debut desc

	return XDC_OK

#endif

go
