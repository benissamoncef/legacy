/*E*/
/*  Fichier : $Id: xzas06sp.prc,v 1.4 1998/09/22 13:30:56 gaborit Exp $      Release : $Revision: 1.4 $        Date : $Date: 1998/09/22 13:30:56 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas06sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des mesures de trafic
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	27/06/94	: Creation	(V 1.1)
* C.T.	15/11/94	: Modif recherche infos sur CI	(V 1.2)
* B.G.	11/01/95	: Modif liste retournee	(V 1.3)
* C.P.	23/06/98	: Modif separation EXP et HIS (V 1.4)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare une courbe f(t) pour les mesures trafic 6 minutes
* sur une periode de 6 heures
* 
* Sequence d'appel
* SP	XZAS06_Ajout_Niveau_Service
* 
* Arguments en entree
* XDY_Mot	va_Station_in		: numero de station
* XDY_Sens	va_Sens_in
* XDY_Octet	va_Voie_in
* XDY_Datetime	va_HorodateDebut_in	: date de debut des mesures
* XDY_Octet	va_TypeDeDonnees_in	: type de courbe
* 
* Arguments en sortie
* XDY_Datetime	va_HorodateDebut_out	: date de debut de la courbe
*
* XDY_Mot	va_Valeur_out		: valeur de la mesure	-- donnees retournees
* XDY_Octet	va_Validite_out		: validite de la mesure	-- dans une liste
* 
* Code retour
* XDC_OK
* XDC_NOK		: probleme d'acces aux mesures
* XDC_ARG_INV		: parametres d'entree null, ou valeur non autorisee ou
*                 	  station inexistante
* XZASC_RADT_PAS_MES	: pas de mesures
* 
* Conditions d'utilisation
* Type de courbe (va_TypeDeDonnees_in) retourne :
* - XDC_RADT_V	: la vitesse en fonction du temps
* - XDC_RADT_Q	: la quantite de vehicules en fonction du temps
* - XDC_RADT_PL	: la quantite de poids lourds en fonction du temps
* - XDC_RADT_TO	: le taux d'occupation en fonction du temps
* La date de depart des mesures est egale a la date donnee ou
* superieure de 5mm 59s maximum ou egale la derniere date inseree moins 6 heures
* (si la date donnee est superieure a la derniere date inseree moins 6 heures)
* La valeur XDC_RADT_MES_INCONNU et la validite XDC_RADT_INCONNU sont
* retournees dans la liste des mesures si pas d'enregistrement a une date
* Les mesures au nombre de 60 sont retournees dans une liste
*
* La procedure stockee au CI recherche les informations 
* dans la base HIS si la date donnee est anterieure de plus d'un 
* mois tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Le code de recherche des infos de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* et du test de la date en entree (< 1 jour)
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
* 
*
* Fonction
* selectionner dans la table MESURE_TRAFIC_6_MINUTES
-------------------------------------------------------*/

use PRC
go

create procedure XZAS;06
	@va_Station_in		smallint	= null,
	@va_Sens_in		T_SENS		= null,
	@va_Voie_in		tinyint		= null,
	@va_HorodateDebut_in	datetime	= null,
	@va_TypeDeDonnees_in	tinyint		= null,
	@va_HorodateDebut_out	datetime	= null output
as
	declare @vl_Valeur smallint, @vl_Validite tinyint,
		@vl_DerniereDate datetime, @vl_NbMesures tinyint, @vl_Status int

	/*A controle parametres en entree */
	if @va_Station_in = null or @va_Sens_in = null or
	   @va_HorodateDebut_in = null or @va_Voie_in = null or
	   (@va_TypeDeDonnees_in != XDC_RADT_V and 
	    @va_TypeDeDonnees_in != XDC_RADT_Q and
	    @va_TypeDeDonnees_in != XDC_RADT_TO and
	    @va_TypeDeDonnees_in != XDC_RADT_PL)
		return XDC_ARG_INV

	/*A verifier l'existance de la station */
	if not exists ( select * from CFG..RES_CHA 
			where	station = @va_Station_in and 
				sens = @va_Sens_in)
		return XDC_ARG_INV

	/* creation d'une table de stockage de la courbe */
	create table #CURV_MES (numero tinyint, valeur smallint null, validite tinyint)

#ifdef HIST
	
		/*B recherche la derniere date de mesure mise en base */
		select @vl_DerniereDate = max(horodate) 
		from HIS..MES_TRA
		where   station = @va_Station_in and
			sens = @va_Sens_in and
			voie = @va_Voie_in

		if @@rowcount = 0 or @vl_DerniereDate = null
			return XZASC_RADT_PAS_MES

		/*B calcul de la date de debut de la courbe */
		select @va_HorodateDebut_out = dateadd(minute,6,dateadd(hour, -6, @vl_DerniereDate))
		if @va_HorodateDebut_in <= @va_HorodateDebut_out
		begin
			select @va_HorodateDebut_out = min(horodate)
			from HIS..MES_TRA
			where   station = @va_Station_in and
				sens = @va_Sens_in and
				voie = @va_Voie_in and
				horodate >= @va_HorodateDebut_in
	
			if @@rowcount != 1
				return XDC_NOK
		end
	
		/*B Lecture des mesures des 6 dernieres heures */
		select @vl_NbMesures = 0
		while @vl_NbMesures < 60
		begin
			/* lecture d'une mesure a une date precisee et de type demande */
			if @va_TypeDeDonnees_in = XDC_RADT_V
				select
					@vl_Valeur 	= V,
					@vl_Validite	= validite_V
				from HIS..MES_TRA
				where   station = @va_Station_in and
					sens = @va_Sens_in and
					voie = @va_Voie_in and
					horodate = dateadd(minute, @vl_NbMesures*6, 
								   @va_HorodateDebut_out)
			else if @va_TypeDeDonnees_in = XDC_RADT_Q
				select
					@vl_Valeur 	= Q,
					@vl_Validite	= validite_Q
				from HIS..MES_TRA
				where   station = @va_Station_in and
					sens = @va_Sens_in and
					voie = @va_Voie_in and
					horodate = dateadd(minute, @vl_NbMesures*6, 
							@va_HorodateDebut_out)
			else if @va_TypeDeDonnees_in = XDC_RADT_TO
				select
					@vl_Valeur 	= T_O,
					@vl_Validite	= validite_TO
				from HIS..MES_TRA
				where   station = @va_Station_in and
					sens = @va_Sens_in and
					voie = @va_Voie_in and
					horodate = dateadd(minute, @vl_NbMesures*6, 
							@va_HorodateDebut_out)
			else 
				select
					@vl_Valeur 	= PL,
					@vl_Validite	= validite_PL
				from HIS..MES_TRA
				where   station = @va_Station_in and
					sens = @va_Sens_in and
					voie = @va_Voie_in and
					horodate = dateadd(minute, @vl_NbMesures*6, 
							@va_HorodateDebut_out)
	
			/* memoriser la valeur de la mesure                           */
			/* si pas de valeur : ajouter une valeur 0, validite inconnue */
			if @@rowcount = 0
				insert #CURV_MES (numero, valeur, validite)
				values ( @vl_NbMesures, XDC_RADT_MES_INCONNU, XDC_RADT_INCONNU)
			else	insert #CURV_MES (numero, valeur, validite)
				values ( @vl_NbMesures, @vl_Valeur, @vl_Validite)
	
			/* numero de la mesure lue */
			select @vl_NbMesures = @vl_NbMesures + 1
	
		end
#else	
#ifdef CI
	/*! cas ou la procedure est creee au CI */

	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	if @va_HorodateDebut_in < (select dateadd(month, -1, getdate ()))
	begin	
		exec @vl_Status = SQL_HIS.PRC..XZAS;06
				@va_Station_in, 	@va_Sens_in, @va_Voie_in,
				@va_HorodateDebut_in, 	@va_TypeDeDonnees_in,
				@va_HorodateDebut_out	output
                        if @vl_Status = null
                                return XDC_PRC_INC
                        else return @vl_Status
	end
	else
	begin
		/*B recherche la derniere date de mesure mise en base */
		select @vl_DerniereDate = max(horodate) 
		from EXP..MES_TRA
		where   station = @va_Station_in and
			sens = @va_Sens_in and
			voie = @va_Voie_in

		if @@rowcount = 0 or @vl_DerniereDate = null
			return XZASC_RADT_PAS_MES

		/*B calcul de la date de debut de la courbe */
		select @va_HorodateDebut_out = dateadd(minute,6,dateadd(hour, -6, @vl_DerniereDate))
		if @va_HorodateDebut_in <= @va_HorodateDebut_out
		begin
			select @va_HorodateDebut_out = min(horodate)
			from EXP..MES_TRA
			where   station = @va_Station_in and
				sens = @va_Sens_in and
				voie = @va_Voie_in and
				horodate >= @va_HorodateDebut_in
	
			if @@rowcount != 1
				return XDC_NOK
		end
	
		/*B Lecture des mesures des 6 dernieres heures */
		select @vl_NbMesures = 0
		while @vl_NbMesures < 60
		begin
			/* lecture d'une mesure a une date precisee et de type demande */
			if @va_TypeDeDonnees_in = XDC_RADT_V
				select
					@vl_Valeur 	= V,
					@vl_Validite	= validite_V
				from EXP..MES_TRA
				where   station = @va_Station_in and
					sens = @va_Sens_in and
					voie = @va_Voie_in and
					horodate = dateadd(minute, @vl_NbMesures*6, 
								   @va_HorodateDebut_out)
			else if @va_TypeDeDonnees_in = XDC_RADT_Q
				select
					@vl_Valeur 	= Q,
					@vl_Validite	= validite_Q
				from EXP..MES_TRA
				where   station = @va_Station_in and
					sens = @va_Sens_in and
					voie = @va_Voie_in and
					horodate = dateadd(minute, @vl_NbMesures*6, 
							@va_HorodateDebut_out)
			else if @va_TypeDeDonnees_in = XDC_RADT_TO
				select
					@vl_Valeur 	= T_O,
					@vl_Validite	= validite_TO
				from EXP..MES_TRA
				where   station = @va_Station_in and
					sens = @va_Sens_in and
					voie = @va_Voie_in and
					horodate = dateadd(minute, @vl_NbMesures*6, 
							@va_HorodateDebut_out)
			else 
				select
					@vl_Valeur 	= PL,
					@vl_Validite	= validite_PL
				from EXP..MES_TRA
				where   station = @va_Station_in and
					sens = @va_Sens_in and
					voie = @va_Voie_in and
					horodate = dateadd(minute, @vl_NbMesures*6, 
							@va_HorodateDebut_out)
	
			/* memoriser la valeur de la mesure                           */
			/* si pas de valeur : ajouter une valeur 0, validite inconnue */
			if @@rowcount = 0
				insert #CURV_MES (numero, valeur, validite)
				values ( @vl_NbMesures, XDC_RADT_MES_INCONNU, XDC_RADT_INCONNU)
			else	insert #CURV_MES (numero, valeur, validite)
				values ( @vl_NbMesures, @vl_Valeur, @vl_Validite)
	
			/* numero de la mesure lue */
			select @vl_NbMesures = @vl_NbMesures + 1
	
		end
	end
#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

	/*A recherche la derniere date de mesure mise en base */
	select @vl_DerniereDate = max(horodate) 
	from EXP..MES_TRA
	where   station = @va_Station_in and
		sens = @va_Sens_in and
		voie = @va_Voie_in

	if @@rowcount = 0 or @vl_DerniereDate = null
		return XZASC_RADT_PAS_MES

	/*A calcul de la date de debut de la courbe */
	select @va_HorodateDebut_out = dateadd(minute,6,dateadd(hour, -6, @vl_DerniereDate))
	if @va_HorodateDebut_in <= @va_HorodateDebut_out
	begin
		select @va_HorodateDebut_out = min(horodate)
		from EXP..MES_TRA
		where   station = @va_Station_in and
			sens = @va_Sens_in and
			voie = @va_Voie_in and
			horodate >= @va_HorodateDebut_in

		if @@rowcount != 1
			return XDC_NOK
	end

	/*A Lecture des mesures des 6 dernieres heures */
	select @vl_NbMesures = 0
	while @vl_NbMesures < 60
	begin
		/* lecture d'une mesure a une date precisee et de type demande */
		if @va_TypeDeDonnees_in = XDC_RADT_V
			select
				@vl_Valeur 	= V,
				@vl_Validite	= validite_V
			from EXP..MES_TRA
			where   station = @va_Station_in and
				sens = @va_Sens_in and
				voie = @va_Voie_in and
				horodate = dateadd(minute, @vl_NbMesures*6, 
							   @va_HorodateDebut_out)
		else if @va_TypeDeDonnees_in = XDC_RADT_Q
			select
				@vl_Valeur 	= Q,
				@vl_Validite	= validite_Q
			from EXP..MES_TRA
			where   station = @va_Station_in and
				sens = @va_Sens_in and
				voie = @va_Voie_in and
				horodate = dateadd(minute, @vl_NbMesures*6, 
						@va_HorodateDebut_out)
		else if @va_TypeDeDonnees_in = XDC_RADT_TO
			select
				@vl_Valeur 	= T_O,
				@vl_Validite	= validite_TO
			from EXP..MES_TRA
			where   station = @va_Station_in and
				sens = @va_Sens_in and
				voie = @va_Voie_in and
				horodate = dateadd(minute, @vl_NbMesures*6, 
						@va_HorodateDebut_out)
		else 
			select
				@vl_Valeur 	= PL,
				@vl_Validite	= validite_PL
			from EXP..MES_TRA
			where   station = @va_Station_in and
				sens = @va_Sens_in and
				voie = @va_Voie_in and
				horodate = dateadd(minute, @vl_NbMesures*6, 
						@va_HorodateDebut_out)

		/* memoriser la valeur de la mesure                             */
		/* si pas de valeur : ajouter une valeur 0 et validite inconnue */
		if @@rowcount = 0
			insert #CURV_MES (numero, valeur, validite)
			values ( @vl_NbMesures, XDC_RADT_MES_INCONNU, XDC_RADT_INCONNU)
		else	insert #CURV_MES (numero, valeur, validite)
			values ( @vl_NbMesures, @vl_Valeur, @vl_Validite)

		/* numero de la mesure lue */
		select @vl_NbMesures = @vl_NbMesures + 1

	end
#endif
#endif

	/*A Retourner la courbe */
	select valeur,validite from #CURV_MES
	order by numero

	return XDC_OK
go
