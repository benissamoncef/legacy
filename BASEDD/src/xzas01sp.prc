/*E*/
/*  Fichier : @(#)xzas01sp.prc	1.4      Release : 1.4        Date : 10/11/95
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas01sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des mesures de trafic
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	16/11/94	: Creation	(V 1.1)
* C.T.	27/01/95	: Suppression des insertions 
*                         des XDC_RADT_INCONNU	(V 1.3)
* B.G. 11/10/95		: autorisation des valeurs nulles dans la table tempo
			  pour éviter le message d'erreur 'tentative insersion de null' (1.4)
* C.P.	23/06/98	: Modif separation EXP et HIS (V 1.5)
 -----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare un nuage de points Q, V, TO, Horodate 
* 
* Sequence d'appel
* SP	XZAS01_Points_QVTO
* 
* Arguments en entree
* XDY_Mot	va_NumStation_in	: numero de station
* XDY_Sens	va_Sens_in		: XDC_SENS_NORD ou XDC_SENS_SUD
* XDY_Datetime	va_HorodateDebut_in	: date de debut des mesures
* XDY_Datetime	va_HorodateFin_in	: date de fin des mesures
* 
* Arguments en sortie
* 
* Liste retournee :
* XDY_Horodate	horodate 	: date de la mesure
* XDY_Octet	V 		: vitesse
* XDY_Octet	validite_V 	: validite a correcte ou incorrecte ou inconnue
* XDY_Octet	T_O 		: taux d'occupation
* XDY_Octet	validite_TO 	: validite a correcte ou incorrecte ou inconnue
* XDY_Octet	Q 		: quantite de vehicules
* XDY_Octet	validite_Q	: validite a correcte ou incorrecte ou inconnue
* 
* Code retour
* XDC_OK
* XDC_NOK		: probleme d'acces aux mesures
* XDC_ARG_INV		: parametres d'entree null, ou valeur non autorisee ou
*                 	  station inexistante
* XZASC_RADT_PAS_MES	: pas de mesure
* 
* Conditions d'utilisation
* La date de depart des mesures est egale a la date donnee ou
* superieure de 5mm 59s maximum
* Si pas d'enregistrement a une date, alors pas de mesures retournees
* a cette date (si une mesure est null en base alors elle est retournee nulle)
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

create procedure XZAS;01
	@va_NumStation_in	smallint	= null,
	@va_Sens_in		T_SENS		= null,
	@va_HorodateDebut_in	datetime	= null,
	@va_HorodateFin_in	datetime	= null
as
	declare @vl_Q smallint, @vl_Validite_Q tinyint,
		@vl_V tinyint, @vl_Validite_V tinyint,
		@vl_TO tinyint, @vl_Validite_TO tinyint,
		@vl_DateDebut datetime, @vl_DateFin datetime,
		@vl_Status int
		
	/*A controle parametres en entree */
	if @va_NumStation_in = null or @va_Sens_in = null or
	   @va_HorodateDebut_in = null or @va_HorodateFin_in = null or
	   (@va_Sens_in != XDC_SENS_NORD and @va_Sens_in != XDC_SENS_SUD) or
	   (@va_HorodateDebut_in > @va_HorodateFin_in)
		return XDC_ARG_INV

	/*A verifier l'existance de la station */
	if not exists ( select * from CFG..RES_CHA 
			where	station = @va_NumStation_in and 
				sens = @va_Sens_in)
		return XDC_ARG_INV

	/* creation d'une table de stockage de la courbe */
	create table #CURV_MES (horodate datetime, V tinyint null, validite_V tinyint null,
	T_O tinyint null, validite_TO tinyint null, Q smallint null, validite_Q tinyint null)

#ifdef HIST
		/*B recherche la date de mesure mise en base egale ou sup a */
		/*B la date de debut specifiee                              */
		select @vl_DateDebut = min(horodate)
		from HIS..MES_TRA
		where   station = @va_NumStation_in and
			sens = @va_Sens_in and
			voie = XDC_RADT_CHAUSSEE and
			horodate >= @va_HorodateDebut_in and
			horodate <= @va_HorodateFin_in

		if @@rowcount = 0
			return XZASC_RADT_PAS_MES

		/*B Lecture des mesures dans l'intervalle de temps */
		select @vl_DateFin = @vl_DateDebut
		while @vl_DateFin <= @va_HorodateFin_in
		begin
			/* lecture d'une mesure a une date precisee */
			select
				@vl_Q 		= Q,
				@vl_Validite_Q	= validite_Q,
				@vl_V 		= V,
				@vl_Validite_V	= validite_V,
				@vl_TO 		= T_O,
				@vl_Validite_TO	= validite_TO
			from HIS..MES_TRA
			where   station = @va_NumStation_in and
				sens = @va_Sens_in and
				voie = XDC_RADT_CHAUSSEE and
				horodate = @vl_DateFin

			/* memoriser la valeur de la mesure         */
			/* si pas de valeur : ajouter une valeur 0, */
			/* validite inconnue                        */
			if @@rowcount > 0
				insert #CURV_MES (horodate , V , validite_V ,
				T_O , validite_TO , Q , validite_Q )
				values ( @vl_DateFin, @vl_V, @vl_Validite_V,
				@vl_TO, @vl_Validite_TO, @vl_Q, @vl_Validite_Q)
	
			/* incrementer la date de 6 minutes */
			select @vl_DateFin = dateadd(minute, 6, @vl_DateFin)
	
		end

#else
#ifdef CI
	/*! cas ou la procedure est creee au CI */

	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	if @va_HorodateDebut_in < (select dateadd(month, -1, getdate ()))
	begin	
		exec @vl_Status = SQL_HIS.PRC..XZAS;01
			@va_NumStation_in, 	@va_Sens_in,
			@va_HorodateDebut_in, 	@va_HorodateFin_in
            	if @vl_Status = null
                  	return XDC_PRC_INC
              	else return @vl_Status
	end
	else
	begin
		/*B recherche la date de mesure mise en base egale ou sup a */
		/*B la date de debut specifiee                              */
		select @vl_DateDebut = min(horodate)
		from EXP..MES_TRA
		where   station = @va_NumStation_in and
			sens = @va_Sens_in and
			voie = XDC_RADT_CHAUSSEE and
			horodate >= @va_HorodateDebut_in and
			horodate <= @va_HorodateFin_in

		if @@rowcount = 0
			return XZASC_RADT_PAS_MES

		/*B Lecture des mesures dans l'intervalle de temps */
		select @vl_DateFin = @vl_DateDebut
		while @vl_DateFin <= @va_HorodateFin_in
		begin
			/* lecture d'une mesure a une date precisee */
			select
				@vl_Q 		= Q,
				@vl_Validite_Q	= validite_Q,
				@vl_V 		= V,
				@vl_Validite_V	= validite_V,
				@vl_TO 		= T_O,
				@vl_Validite_TO	= validite_TO
			from EXP..MES_TRA
			where   station = @va_NumStation_in and
				sens = @va_Sens_in and
				voie = XDC_RADT_CHAUSSEE and
				horodate = @vl_DateFin

			/* memoriser la valeur de la mesure */
			/* si une de valeur                 */
			if @@rowcount > 0
				insert #CURV_MES (horodate , V , validite_V ,
				T_O , validite_TO , Q , validite_Q )
				values ( @vl_DateFin, @vl_V, @vl_Validite_V,
				@vl_TO, @vl_Validite_TO, @vl_Q, @vl_Validite_Q)
	
			/* incrementer la date de 6 minutes */
			select @vl_DateFin = dateadd(minute, 6, @vl_DateFin)
	
		end
	end
#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

	/*A recherche la date de mesure mise en base egale ou sup a */
	/*A la date de debut specifiee                              */
	select @vl_DateDebut = min(horodate)
	from EXP..MES_TRA
	where   station = @va_NumStation_in and
		sens = @va_Sens_in and
		voie = XDC_RADT_CHAUSSEE and
		horodate >= @va_HorodateDebut_in and
		horodate <= @va_HorodateFin_in

	if @@rowcount = 0
		return XZASC_RADT_PAS_MES

	/*A Lecture des mesures dans l'intervalle de temps */
	select @vl_DateFin = @vl_DateDebut
	while @vl_DateFin <= @va_HorodateFin_in
	begin
		/* lecture d'une mesure a une date precisee */
		select
			@vl_Q 		= Q,
			@vl_Validite_Q	= validite_Q,
			@vl_V 		= V,
			@vl_Validite_V	= validite_V,
			@vl_TO 		= T_O,
			@vl_Validite_TO	= validite_TO
		from EXP..MES_TRA
		where   station = @va_NumStation_in and
			sens = @va_Sens_in and
			voie = XDC_RADT_CHAUSSEE and
			horodate = @vl_DateFin

		/* memoriser la valeur de la mesure         */
		/* si pas de valeur : ajouter une valeur 0, */
		/* validite inconnue */
		if @@rowcount > 0
			insert #CURV_MES (horodate , V , validite_V ,
			T_O , validite_TO , Q , validite_Q )
			values ( @vl_DateFin, @vl_V, @vl_Validite_V,
			@vl_TO, @vl_Validite_TO, @vl_Q, @vl_Validite_Q)

		/* incrementer la date de 6 minutes */
		select @vl_DateFin = dateadd(minute, 6, @vl_DateFin)

	end
#endif
#endif

	/*A Retourner la courbe */
	select * from #CURV_MES
	order by horodate

	return XDC_OK
go
