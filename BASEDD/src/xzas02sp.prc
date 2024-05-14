/*E*/
/*  Fichier : $Id: xzas02sp.prc,v 1.10 1998/09/22 13:30:52 gaborit Exp $      Release : $Revision: 1.10 $        Date : $Date: 1998/09/22 13:30:52 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas02sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des mesures de trafic
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	16/11/94	: Creation	(V 1.1)
* C.T.	13/01/95	: Modif autre des select retournes	(V 1.2)
* C.T.	10/08/95	: Correction selection station en sens nord (V 1.4)
* B.G.	15/09/95	: optimisation (1.5)
* B.G.	31/10/95	: gestion sens A57 (1.6)
* C.T.	03/05/96	: Recherche des mesures de niveaux dans EXP (1.7)
* P.N.  23/09/96	: Ajout de la plage en parametre (ana3) v1.8
* C.T.	23/02/98	: Modif default suite a passage en version SQL 11.0.3.2 (1.9)
* P.N	02/07/98	: separation EXP/HIS (1.10)
* GGY	26/09/23	: Ajout district CA (DEM483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare un graphe trafic INRETS 
* 
* Sequence d'appel
* SP	XZAS02_Graphe_Trafic
* 
* Arguments en entree
* XDY_Autoroute	va_Autoroute_in		: numero d'autoroute
* XDY_PR	va_PRDebut_in		
* XDY_PR	va_PRFin_in		
* XDY_Datetime	va_HorodateDebut_in	: date de debut des mesures multiple de 6mn
* XDY_Octet	va_Plage_in	: duree de la plage (sans pavets!)
* Arguments en sortie
*
* Liste 1 retournee :
* XDY_Entier	numero	: reference de la zone d'influence
* XDY_PR	PRDebut	: PR de debut de la zone
* XDY_PR	PRFin	: PR de fin de la zone
* 
* Liste 2 retournee :
* XDY_Entier	numero	: reference de la zone d'influence
* XDY_Horodate	Horodate: date de la mesure
* XDY_Octet	Niveau	: niveau de trafic
* 
* Code retour
* XDC_OK
* XDC_ARG_INV		: parametres d'entree null, ou valeur non autorisee ou
*                 	  date non multiple de 6 mn
* 
* Conditions d'utilisation
* Les mesures retournees sont datees a partir de la date de debut donnee
* et pendant 6 heures
* Retourne la liste des PR debut, PR fin des zones d'influence
* dans l'ordre des PR de debut (croissant si sens sud sinon decroissant)
* puis la liste des mesures avec la date dans l'ordre des PR de debut
* (croissant si sens sud sinon decroissant) et des dates croissantes
*
* La procedure stockee au CI recherche les informations 
* dans la base HIS si la date donnee est anterieure de plus d'un 
* mois tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Le code de recherche des infos de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* et du test de la date en entree (< 1 mois)
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
* 
*
* Fonction
* selectionner dans la table MESURE_NIVEAU
-------------------------------------------------------*/

use PRC
go

create procedure XZAS;02
	@va_Autoroute_in	tinyint		= null,
	@va_PRDebut_in		T_PR		= null,
	@va_PRFin_in		T_PR		= null,
	@va_Horodate_in	datetime	= null,
	@va_Plage_in		tinyint = 6
as
	declare @vl_Sens T_SENS,
		@vl_Sens_inv T_SENS,
		@vl_Station smallint,
		@vl_PRDebut T_PR, @vl_PRFin T_PR, @vl_Numero int,
		@vl_DateFin datetime,
		@vl_Status int, 
		@vl_Site tinyint	

	/*A controle parametres en entree */
	if @va_Autoroute_in = null or @va_PRDebut_in = null or
	   @va_Horodate_in = null or @va_PRFin_in = null or
	   @va_PRDebut_in = @va_PRFin_in
		return XDC_ARG_INV

#ifdef HIST
	/* Verification du site */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

	if @vl_Site = XDC_CI
		exec @vl_Status = SQL_CI.PRC..XZAS;02
			@va_Autoroute_in,
			@va_PRDebut_in	,
			@va_PRFin_in	,
			@va_Horodate_in,
			@va_Plage_in

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAS;02
			@va_Autoroute_in,
			@va_PRDebut_in	,
			@va_PRFin_in	,
			@va_Horodate_in,
			@va_Plage_in

	if @vl_Status = null
		return XDC_PRC_INC
	else
		return @vl_Status

#else
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
	create table #ZON_PR (numero int default 0, 
	station smallint, PR_debut int, PR_fin int)

	create table #ZON_PR2 (numero int default 0, 
	station smallint, PR_debut int, PR_fin int)

	/*A Recherche les stations et leur zone d'influence */
	insert #ZON_PR2 ( numero, station, PR_debut, PR_fin )
	select distinct
		0,
		CFG..RES_INF.station,
		CFG..RES_INF.PR_debut,
		CFG..RES_INF.PR_fin
	from CFG..RES_INF, CFG..RES_POR
	where CFG..RES_POR.autoroute = @va_Autoroute_in and
		CFG..RES_INF.portion = CFG..RES_POR.numero and
		CFG..RES_INF.sens = @vl_Sens and
		((@vl_Sens_inv = XDC_SENS_SUD and
		  CFG..RES_INF.PR_fin >= @va_PRDebut_in and
		  CFG..RES_INF.PR_debut <= @va_PRFin_in) or
		 (@vl_Sens_inv = XDC_SENS_NORD and
		  CFG..RES_INF.PR_fin >= @va_PRFin_in and
		  CFG..RES_INF.PR_debut <= @va_PRDebut_in))

	if @@rowcount = 0
		return XDC_OK

	/* fusionne les zones adjacentes de la meme station */
	insert #ZON_PR ( numero, station, PR_debut, PR_fin )
	select distinct
		0,
		station,
		min(PR_debut),
		max(PR_fin)
	from #ZON_PR2
	group by station

	delete #ZON_PR2

	insert #ZON_PR2
	select * from #ZON_PR

	/* numeroter les zones trouvees */
	declare Pointeur_zone cursor
	for select station, PR_debut, PR_fin
	from #ZON_PR2 order by PR_debut

	open Pointeur_zone
	fetch Pointeur_zone into @vl_Station, @vl_PRDebut, @vl_PRFin
	select @vl_Numero = 0
	while (@@sqlstatus = 0)
	begin
		select @vl_Numero = @vl_Numero + 1

		update #ZON_PR set numero = @vl_Numero
		where station = @vl_Station and
			PR_debut = @vl_PRDebut and PR_fin = @vl_PRFin

		fetch Pointeur_zone into @vl_Station, @vl_PRDebut, @vl_PRFin
	end
	close Pointeur_zone

	/* creation d'une table tempo contenant les resultats */
	create table #RESULT ( numero	int,
				horodate datetime,
				niveau	tinyint )

	select @vl_DateFin = dateadd(hour, @va_Plage_in , @va_Horodate_in)

	declare STATION cursor
	for select numero,station
	from #ZON_PR

	/*! cas ou la procedure est creee sur le serveur d'un district */

		open STATION
		fetch STATION into @vl_Numero,@vl_Station

		while (@@sqlstatus = 0)
		begin
			insert #RESULT (numero,horodate,niveau)
			select @vl_Numero,horodate,niveau
			from EXP..MES_NIV
			where station=@vl_Station
			  and sens=@vl_Sens
			  and horodate >= @va_Horodate_in
			  and horodate <= @vl_DateFin

			fetch STATION into @vl_Numero,@vl_Station
		end

	/*A Retourner les niveaux trouves */
	if @vl_Sens_inv = XDC_SENS_SUD
		select numero,horodate,niveau
		from #RESULT
		order by horodate,numero
	else
		select numero,horodate,niveau
		from #RESULT
		order by horodate asc,numero desc

	/*A Retourner la liste des zone d'influence sur ce morceau d'autoroute */
	select numero, PR_debut, PR_fin from #ZON_PR
	order by numero

	return XDC_OK
#endif
go
