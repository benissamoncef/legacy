/*E*/
/*  Fichier : $Id: xzas04sp.prc,v 1.10 1999/06/08 09:05:06 gesconf Exp $      Release : $Revision: 1.10 $        Date : $Date: 1999/06/08 09:05:06 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas04sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des mesures de trafic
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	16/11/94	: Creation	(V 1.1)
* C.T.	23/01/95	: Modif le PR retourne : PR de station
*			  au lieu des PR de zone d'influence (V 1.2)
* C.T.	24/01/95	: Suppression du controle de date 
*                         multiple de 6mn (V 1.3)
* C.T.	15/03/95	: Modif la periode de recuperation 
*                         des mesures : 24h (V 1.4)
* C.T.	07/09/95	: Optimisation en temps (V 1.5)
* B.G.	31/10/95	: gestion sens A57 (1.6)
* C.T.	23/02/98	: Modif default suite a passage en version SQL 11.0.3.2 (1.7)
* P.N.	01/07/98	: suppression des station de peage v1.8
* Niepceron	07 Jul 1998	: modif separation EXP/HIS (1696) V1.9
* Niepceron	02 Jun 1999	: correction modif separation EXP/HIS 1.10
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare un graphe trafic 3D pour Q, V, TO 
* 
* Sequence d'appel
* SP	XZAS04_Graphe_3D
* 
* Arguments en entree
* XDY_Autoroute	va_Autoroute_in		: numero d'autoroute
* XDY_PR	va_PRDebut_in		: pr de debut
* XDY_PR	va_PRFin_in		: pr de fin
* XDY_Datetime	va_HorodateDebut_in	: date de debut des mesures
* 
* Arguments en sortie
* 
* Liste 1 retournee :
* XDY_Entier	numero	: reference de la zone d'influence
* XDY_PR	PR	: PR de station
* 
* Liste 2 retournee :
* XDY_Entier	numero		: reference de la zone d'influence
* XDY_Horodate	Horodate	: date de la mesure
* XDY_Mot	Q		: mesure de la quantite de vehicules
* XDY_Booleen	ValiditeQ	: validite de Q
* XDY_Octet	V		: mesure de la vitesse
* XDY_Booleen	ValiditeV	: validite de V
* XDY_Octet	TO		: mesure du taux d'occupation
* XDY_Booleen	ValiditeTO	: validite de TO
* 
* Code retour
* XDC_OK
* XDC_NOK		: probleme d'acces aux mesures
* XDC_ARG_INV		: parametres d'entree null, ou valeur non autorisee ou
*                 	  date non multiple de 6 mn
* 
* Conditions d'utilisation
* Les mesures retournees sont celles de la voie XDC_RADT_CHAUSSEE
* pendant 24 heures a partir de la date de debut donnee
* Retourne la liste des PR des stations de comptage
* dans l'ordre des PR croissant
* puis la liste des mesures avec la date dans l'ordre des dates 
* croissantes et PR croissant
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

create procedure XZAS;04
	@va_Autoroute_in	tinyint		= null,
	@va_PRDebut_in		T_PR		= null,
	@va_PRFin_in		T_PR		= null,
	@va_HorodateDebut_in	datetime	= null
as
	declare  @vl_SensStation T_SENS, @vl_Sens T_SENS, @vl_Sens_inv T_SENS, @vl_Secondes int,
		@vl_Minutes int, @vl_Station smallint,
		@vl_PR T_PR, @vl_Numero int,@vl_boucle tinyint, @vl_boucle_inverse tinyint,
		@vl_DateFin datetime, @vl_Status int

	/*A controle parametres en entree */
	if @va_Autoroute_in = null or @va_PRDebut_in = null or
	   @va_HorodateDebut_in = null or @va_PRFin_in = null or
	   @va_PRDebut_in = @va_PRFin_in
		return XDC_ARG_INV

	/* sens du trafic */
	if @va_PRDebut_in > @va_PRFin_in
		select @vl_Sens_inv = XDC_SENS_NORD
	else	select @vl_Sens_inv = XDC_SENS_SUD

	/* gestion sens inverse A57 */
	if exists (	select * from CFG..RES_AUT_INV
			where	numero=@va_Autoroute_in
			  and	inversee=XDC_PR_INV )
	begin
		if @vl_Sens_inv = XDC_SENS_NORD
			select @vl_Sens = XDC_SENS_SUD
		else
			select @vl_Sens = XDC_SENS_NORD
	end
	else
		select @vl_Sens = @vl_Sens_inv
	
	/*! table de stockage des zones d'influence */
	create table #ZON_PR (numero int default 0, station smallint, PR int, sens tinyint)

	/*A Recherche les stations et leur PR */
	insert #ZON_PR ( numero, station, PR , sens)
	select distinct
		0,
		CFG..RES_CHA.station,
		CFG..RES_CHA.PR,
		CFG..RES_CHA.sens
	from CFG..RES_POR, CFG..RES_INF, CFG..RES_CHA
	where	CFG..RES_POR.autoroute = @va_Autoroute_in and
		CFG..RES_INF.portion = CFG..RES_POR.numero and
		CFG..RES_INF.sens = @vl_Sens and
		CFG..RES_CHA.station = CFG..RES_INF.station and
		CFG..RES_CHA.sens = CFG..RES_INF.sens and
		((@vl_Sens_inv = XDC_SENS_SUD and
		  CFG..RES_CHA.PR >= @va_PRDebut_in and
		  CFG..RES_CHA.PR <= @va_PRFin_in) or
		 (@vl_Sens_inv = XDC_SENS_NORD and
		  CFG..RES_CHA.PR <= @va_PRDebut_in and
		  CFG..RES_CHA.PR >= @va_PRFin_in))
	order by CFG..RES_CHA.PR

	if @@rowcount = 0
		return XDC_OK

	/* numeroter les zones trouvees */
	declare Pointeur_zone cursor
	for select station, PR, sens
	from #ZON_PR

	open Pointeur_zone
	fetch Pointeur_zone into @vl_Station, @vl_PR, @vl_SensStation
	select @vl_Numero = 0
	while (@@sqlstatus = 0)
	begin
                select @vl_boucle=boucle,
			@vl_boucle_inverse=boucle_inverse
		from CFG..EQT_PNT_MES
		where station=@vl_Station and sens=@vl_SensStation

		if ( @vl_boucle <> @vl_boucle_inverse)
		begin
			select @vl_Numero = @vl_Numero + 1

			update #ZON_PR set numero = @vl_Numero
			where station = @vl_Station and PR = @vl_PR
		end
		else
			delete #ZON_PR
			where station = @vl_Station and PR = @vl_PR


		fetch Pointeur_zone into @vl_Station, @vl_PR, @vl_SensStation
	end
	close Pointeur_zone

	/*! calcul la date de fin pour la recherche des mesures */
	select @vl_DateFin = dateadd(hour, 24, @va_HorodateDebut_in)

	create table #MESURES(numero smallint,PR int,horodate datetime,
				Q smallint null,validite_Q bit,
				V tinyint null,validite_V bit,
				T_O tinyint null,validite_TO bit)
#ifdef HIST
	/*A Retourner la liste des zones d'influence sur ce morceau d'autoroute */
	if @vl_Sens_inv = XDC_SENS_SUD
		select numero, PR from #ZON_PR
		order by PR
	else
		select numero, PR from #ZON_PR
		order by PR desc


		   declare pointeur_station cursor
		   for select station, PR, numero
		   from #ZON_PR
		   open pointeur_station 

		   fetch pointeur_station  into @vl_Station, @vl_PR, @vl_Numero
		   while @@sqlstatus = 0
		   begin
			insert #MESURES(numero,PR,horodate,Q,validite_Q,
					V,validite_V,T_O,validite_TO)
			select
				@vl_Numero,
				@vl_PR,
				HIS..MES_TRA.horodate,
				HIS..MES_TRA.Q,
				HIS..MES_TRA.validite_Q,
				HIS..MES_TRA.V,
				HIS..MES_TRA.validite_V,
				HIS..MES_TRA.T_O,
				HIS..MES_TRA.validite_TO
			from HIS..MES_TRA
			where   HIS..MES_TRA.station = @vl_Station and
				HIS..MES_TRA.sens = @vl_Sens and
				HIS..MES_TRA.voie = XDC_RADT_CHAUSSEE and
				HIS..MES_TRA.horodate <= @vl_DateFin and
				HIS..MES_TRA.horodate >= @va_HorodateDebut_in

			fetch pointeur_station  into @vl_Station, @vl_PR, @vl_Numero
		   end
		   close pointeur_station
#else
#ifdef CI
	/*! cas ou la procedure est creee au CI */

	/*A si la date donnee est anterieure d'un jour au moins */
 	/*A rechercher les mesures dans la base HIS sinon dans  */
	/*A la base EXP (au niveau du CI)                       */
	if @va_HorodateDebut_in < (select dateadd(month, -1, getdate ()))
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAS;04
			@va_Autoroute_in	,
			@va_PRDebut_in		,
			@va_PRFin_in		,
			@va_HorodateDebut_in	

        	if @vl_Status = null
                	return XDC_PRC_INC
        	else return @vl_Status
	end
	else
	begin
		/*A Retourner la liste des zones d'influence sur ce morceau d'autoroute */
		if @vl_Sens_inv = XDC_SENS_SUD
			select numero, PR from #ZON_PR
			order by PR
		else
			select numero, PR from #ZON_PR
			order by PR desc

		/*B lecture des mesures de niveau pour ce morceau d'autoroute     */
		   declare pointeur_station cursor
		   for select station, PR, numero
		   from #ZON_PR
		   open pointeur_station 

		   fetch pointeur_station  into @vl_Station, @vl_PR, @vl_Numero
		   while @@sqlstatus = 0
		   begin
			insert #MESURES(numero,PR,horodate,Q,validite_Q,
					V,validite_V,T_O,validite_TO)
			select
				@vl_Numero,
				@vl_PR,
				EXP..MES_TRA.horodate,
				EXP..MES_TRA.Q,
				EXP..MES_TRA.validite_Q,
				EXP..MES_TRA.V,
				EXP..MES_TRA.validite_V,
				EXP..MES_TRA.T_O,
				EXP..MES_TRA.validite_TO
			from EXP..MES_TRA
			where   EXP..MES_TRA.station = @vl_Station and
				EXP..MES_TRA.sens = @vl_Sens and
				EXP..MES_TRA.voie = XDC_RADT_CHAUSSEE and
				EXP..MES_TRA.horodate <= @vl_DateFin and
				EXP..MES_TRA.horodate >= @va_HorodateDebut_in

			fetch pointeur_station  into @vl_Station, @vl_PR, @vl_Numero
		   end
		   close pointeur_station
	end
#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

	   declare pointeur_station cursor
	   for select station, PR, numero
	   from #ZON_PR
	   open pointeur_station 

	   fetch pointeur_station  into @vl_Station, @vl_PR, @vl_Numero
	   while @@sqlstatus = 0
	   begin
		insert #MESURES(numero,PR,horodate,Q,validite_Q,
					V,validite_V,T_O,validite_TO)
		select
			@vl_Numero,
			@vl_PR,
			EXP..MES_TRA.horodate,
			EXP..MES_TRA.Q,
			EXP..MES_TRA.validite_Q,
			EXP..MES_TRA.V,
			EXP..MES_TRA.validite_V,
			EXP..MES_TRA.T_O,
			EXP..MES_TRA.validite_TO
		from EXP..MES_TRA
		where   EXP..MES_TRA.station = @vl_Station and
			EXP..MES_TRA.sens = @vl_Sens and
			EXP..MES_TRA.voie = XDC_RADT_CHAUSSEE and
			EXP..MES_TRA.horodate >= @va_HorodateDebut_in and
			EXP..MES_TRA.horodate <= @vl_DateFin

		fetch pointeur_station  into @vl_Station, @vl_PR, @vl_Numero
	   end
	   close pointeur_station
#endif
#endif

	/*A lecture des mesures de niveau pour ce morceau d'autoroute    */
	/*A ordonnees suivant les PR croissants pour le sens SUD et      */
	/*A decroissant pour le sens NORD et suivant la date des mesures */
	/*A retourner les mesures */
	select
		numero,
		horodate,
		Q,
		validite_Q,
		V,
		validite_V,
		T_O,
		validite_TO
	from #MESURES
	order by horodate, PR

	return XDC_OK
go
