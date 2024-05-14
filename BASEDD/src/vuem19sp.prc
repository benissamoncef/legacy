/*E*/
/* Fichier : $Id: vuem19sp.prc,v 1.11 2016/09/13 13:34:21 devgfi Exp $        Release : $Revision: 1.11 $        Date : $Date: 2016/09/13 13:34:21 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER vuem19sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* vues metiers
*
------------------------------------------------------
* HISTORIQUE :
*
* B.G.	01/09/95	: Creation
* C.T.	26/09/95	: Regroupement des zones d'influence par station (V 1.2)
* C.T.	08/12/95	: Optimisation (V 1.3)
* C.T.	03/05/96	: rechercher des mesures de niveau dans EXP (V 1.4)
* C.T.	21/06/96	: Ajout en entree parametre indiquant si la recherche est 
*                         sur les mesures inferieures ou sup au seuil fourni
* V.L	23/10/96	: Selection des stations distinctes (DEM 916) V1.5
* JMG	01/12/97	: pas de donnees extrapolees 1.8
* P.N.  09/0798		: seperation EXP/HIS (dem/1696) 1.10
* JPL	13/09/16	: Type de seuil change en entier (tinyint)  1.11
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	VUEM19_MesuresDepassantSeuil
* 
* Arguments en entree
* XDY_Horodate	va_Debut_in : date de debut de recherche
* XDY_Horodate	va_Fin_in : date de fin de recherche
* XDY_Octet	va_Autoroute_in
* XDY_PR	va_PR_in
* XDY_Sens	va_Sens_in
* XDY_Entier	va_Seuil_in
* XDY_Octet	va_Type_in
* 
* Arguments en sortie
*
* liste retournee
* char()	va_Station
* XDY_Entier	va_PRDebutInf
* XDY_Entier	va_PRFinInf
* XDY_Entier	va_Mesure
* 
* Service rendu
* retourne les TO ou vitesse ou debit superieur ou inferieur a un seuil
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

if exists (select 1 from sysobjects where name = 'VUEM19' and type = 'P')
	drop procedure VUEM19
go


create procedure VUEM19
	@va_Debut_in		datetime	= null,
	@va_Fin_in		datetime	= null,
	@va_Autoroute_in	T_AUTOROUTE	= null,
	@va_PRDebut_in		T_PR		= null,
	@va_PRFin_in		T_PR		= null,
	@va_Sens_in		T_SENS		= null,
	@va_Seuil_in		smallint	= null,
	@va_Type_in		tinyint		= null,
	@va_SeuilSuperieur_in	tinyint
as
	declare @vl_Seuil tinyint, @vl_Station smallint, @vl_Nom char(10), @vl_PRdeb T_PR, @vl_PRfin T_PR,
		@vl_Status int
#ifdef HIST
	if @va_Type_in != XDC_RADT_Q and  @va_Type_in != XDC_RADT_V and @va_Type_in != XDC_RADT_TO and @va_Type_in != XDC_RADT_PL
	begin
	exec @vl_Status = SQL_CI.PRC..VUEM19
	@va_Debut_in		,
	@va_Fin_in		,
	@va_Autoroute_in	,
	@va_PRDebut_in		,
	@va_PRFin_in		,
	@va_Sens_in		,
	@va_Seuil_in		,
	@va_Type_in		,
	@va_SeuilSuperieur_in	

        if @vl_Status = null
                return XDC_PRC_INC
        else return @vl_Status
	end
#else
	if @va_Type_in = XDC_RADT_Q or  @va_Type_in = XDC_RADT_V or @va_Type_in = XDC_RADT_TO or @va_Type_in = XDC_RADT_PL
	begin
	exec @vl_Status = SQL_HIS.PRC..VUEM19
	@va_Debut_in		,
	@va_Fin_in		,
	@va_Autoroute_in	,
	@va_PRDebut_in		,
	@va_PRFin_in		,
	@va_Sens_in		,
	@va_Seuil_in		,
	@va_Type_in		,
	@va_SeuilSuperieur_in	

        if @vl_Status = null
                return XDC_PRC_INC
        else return @vl_Status
	end
#endif
	if @va_Type_in != XDC_RADT_Q
		select @vl_Seuil = convert(tinyint,@va_Seuil_in)

	create table #MESURES (horodate datetime, nom char(10), PR_debut int, PR_fin int, mesure int)

	/*A rechercher les stations */
	declare pointeur_station cursor
	for select distinct
		CFG..EQT_GEN.numero, 
		CFG..EQT_GEN.nom, 
		min(CFG..RES_INF.PR_debut), 
		max(CFG..RES_INF.PR_fin)
	from CFG..RES_INF, CFG..RES_POR, CFG..EQT_GEN
	where	CFG..RES_POR.autoroute = @va_Autoroute_in and
		CFG..RES_INF.portion = CFG..RES_POR.numero and
		CFG..RES_INF.sens = @va_Sens_in and
		CFG..RES_INF.PR_debut < @va_PRFin_in and
		CFG..RES_INF.PR_fin >= @va_PRDebut_in and
		CFG..EQT_GEN.numero = CFG..RES_INF.station and
		CFG..EQT_GEN.type = XDC_EQT_RAD
	group by CFG..EQT_GEN.numero, CFG..RES_INF.station, CFG..EQT_GEN.type
	open pointeur_station

	/*A rechercher les mesures */
	fetch pointeur_station into @vl_Station, @vl_Nom, @vl_PRdeb, @vl_PRfin
	while (@@sqlstatus = 0)
	begin
#ifdef HIST
	    if @va_SeuilSuperieur_in = XDC_VRAI
	    begin
		if @va_Type_in = XDC_RADT_V
			insert #MESURES
			select
				horodate,
				@vl_Nom,
				@vl_PRdeb,
				@vl_PRfin, 
				V
			from HIS..MES_TRA
			where	HIS..MES_TRA.station = @vl_Station and
				HIS..MES_TRA.voie = XDC_RADT_CHAUSSEE and
				HIS..MES_TRA.sens = @va_Sens_in and
				horodate >= @va_Debut_in and
				horodate < @va_Fin_in and
				(V > @vl_Seuil) and
				validite_V != XDC_RADT_RECONST
		else if @va_Type_in = XDC_RADT_Q
			insert #MESURES
			select
				horodate,
				@vl_Nom,
				@vl_PRdeb,
				@vl_PRfin, 
				Q
			from HIS..MES_TRA
			where	HIS..MES_TRA.station = @vl_Station and
				HIS..MES_TRA.voie = XDC_RADT_CHAUSSEE and
				HIS..MES_TRA.sens = @va_Sens_in and
				horodate >= @va_Debut_in and
				horodate < @va_Fin_in and
				(Q > @va_Seuil_in) and
				validite_Q != XDC_RADT_RECONST
		else if @va_Type_in = XDC_RADT_TO
			insert #MESURES
			select
				horodate,
				@vl_Nom,
				@vl_PRdeb,
				@vl_PRfin, 
				T_O
			from HIS..MES_TRA
			where	HIS..MES_TRA.station = @vl_Station and
				HIS..MES_TRA.voie = XDC_RADT_CHAUSSEE and
				HIS..MES_TRA.sens = @va_Sens_in and
				horodate >= @va_Debut_in and
				horodate < @va_Fin_in and
				(T_O > @vl_Seuil) and
				validite_TO != XDC_RADT_RECONST
		else if @va_Type_in = XDC_RADT_PL
			insert #MESURES
			select
				horodate,
				@vl_Nom,
				@vl_PRdeb,
				@vl_PRfin, 
				PL
			from HIS..MES_TRA
			where	HIS..MES_TRA.station = @vl_Station and
				HIS..MES_TRA.voie = XDC_RADT_CHAUSSEE and
				HIS..MES_TRA.sens = @va_Sens_in and
				horodate >= @va_Debut_in and
				horodate < @va_Fin_in and
				(PL > @vl_Seuil) and
				validite_PL != XDC_RADT_RECONST
	    end
	    else
	    begin
		if @va_Type_in = XDC_RADT_V
			insert #MESURES
			select
				horodate,
				@vl_Nom,
				@vl_PRdeb,
				@vl_PRfin, 
				V
			from HIS..MES_TRA
			where	HIS..MES_TRA.station = @vl_Station and
				HIS..MES_TRA.voie = XDC_RADT_CHAUSSEE and
				HIS..MES_TRA.sens = @va_Sens_in and
				horodate >= @va_Debut_in and
				horodate < @va_Fin_in and
				(V < @vl_Seuil) and
				validite_V !=  XDC_RADT_RECONST
		else if @va_Type_in = XDC_RADT_Q
			insert #MESURES
			select
				horodate,
				@vl_Nom,
				@vl_PRdeb,
				@vl_PRfin, 
				Q
			from HIS..MES_TRA
			where	HIS..MES_TRA.station = @vl_Station and
				HIS..MES_TRA.voie = XDC_RADT_CHAUSSEE and
				HIS..MES_TRA.sens = @va_Sens_in and
				horodate >= @va_Debut_in and
				horodate < @va_Fin_in and
				(Q < @va_Seuil_in) and
				validite_Q != XDC_RADT_RECONST
		else if @va_Type_in = XDC_RADT_TO
			insert #MESURES
			select
				horodate,
				@vl_Nom,
				@vl_PRdeb,
				@vl_PRfin, 
				T_O
			from HIS..MES_TRA
			where	HIS..MES_TRA.station = @vl_Station and
				HIS..MES_TRA.voie = XDC_RADT_CHAUSSEE and
				HIS..MES_TRA.sens = @va_Sens_in and
				horodate >= @va_Debut_in and
				horodate < @va_Fin_in and
				(T_O < @vl_Seuil) and
				validite_TO != XDC_RADT_RECONST
		else if @va_Type_in = XDC_RADT_PL
			insert #MESURES
			select
				horodate,
				@vl_Nom,
				@vl_PRdeb,
				@vl_PRfin, 
				PL
			from HIS..MES_TRA
			where	HIS..MES_TRA.station = @vl_Station and
				HIS..MES_TRA.voie = XDC_RADT_CHAUSSEE and
				HIS..MES_TRA.sens = @va_Sens_in and
				horodate >= @va_Debut_in and
				horodate < @va_Fin_in and
				(PL < @vl_Seuil) and
				validite_PL != XDC_RADT_RECONST
	    end
#else
	    if @va_SeuilSuperieur_in = XDC_VRAI
	    begin
			insert #MESURES
			select
				horodate,
				@vl_Nom,
				@vl_PRdeb,
				@vl_PRfin, 
				niveau
			from EXP..MES_NIV
			where	station = @vl_Station and
				horodate >= @va_Debut_in and
				horodate < @va_Fin_in and
				(niveau > @vl_Seuil)
	    end
	    else
	    begin
			insert #MESURES
			select
				horodate,
				@vl_Nom,
				@vl_PRdeb,
				@vl_PRfin, 
				niveau
			from EXP..MES_NIV
			where	station = @vl_Station and
				horodate >= @va_Debut_in and
				horodate < @va_Fin_in and
				(niveau < @vl_Seuil)
	    end
#endif
	    /* station suivante */
	    fetch pointeur_station into @vl_Station, @vl_Nom, @vl_PRdeb, @vl_PRfin
	end
	close pointeur_station

	/*A retourner les mesures */
	select distinct
		convert(char(10),horodate,103)+' '+convert(char(8),horodate,8),
		nom,
		min(PR_debut),
		max(PR_fin),
		mesure
	from #MESURES
	group by nom
	order by PR_debut, horodate, mesure

	return XDC_OK
go
