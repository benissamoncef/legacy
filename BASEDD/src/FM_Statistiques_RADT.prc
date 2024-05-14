/*
* Fichier : $Id: FM_Statistiques_RADT.prc,v 1.2 2021/11/08 09:36:57 devgfi Exp $        $Revision: 1.2 $        $Date: 2021/11/08 09:36:57 $
------------------------------------------------------------------------------------------------------------------------
* ESCOTA *  PROJET MIGRAZUR
------------------------------------------------------------------------------------------------------------------------
* SOUS-SYSTEME * OUTILS
------------------------------------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Procédure dont l'objet est d'elaborer des statistiques sur les valeurs des mesures a une date donnee
*  et sur les jours precedents pour un, ou tous les points de mesures des stations RADT du reseau.
*
------------------------------------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	09/09/21 : Creation
* JPL	02/11/21 : Utilisation du nom 'generique' de la base pour validite sur HIS et autres.
----------------------------------------------------------------------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'FM_Statistiques_RADT' and type = 'P')
	drop procedure FM_Statistiques_RADT
go


#ifdef HIST
#define BDD_XPL HIS
#else
#define BDD_XPL EXP
#endif


/* Periode de recueil des donnees */
#define	CM_PERIODE_MESURES	6

#define	CM_NB_MES_HEURE		(60 / CM_PERIODE_MESURES)
#define	CM_NB_MES_JOUR		(24 * CM_NB_MES_HEURE)

#define	CM_DEBIT_MAX_HEURE	2500

#define	CM_DEFAUT_AUCUN		' '
#define	CM_DEFAUT_DEBIT		'D'
#define	CM_DEFAUT_ECART		'E'
#define	CM_DEFAUT_REPETE	'R'
#define	CM_DEFAUT_x10		'0'



create procedure FM_Statistiques_RADT

	@va_Date_Demande	date,
	@va_Station		smallint	= NULL

as
declare
	@vl_AujourDHui		date,
	@vl_Date_Debut		datetime,
	@vl_Jour_J		datetime,
	@vl_Jour_J_1		datetime,

	@vl_station		smallint,
	@vl_sens		tinyint,
	@vl_voie		tinyint,
	@vl_autoroute		tinyint,
	@vl_PR			int,

	@vl_Taux_J_7		int,
	@vl_Taux_J_6		int,
	@vl_Taux_J_5		int,
	@vl_Taux_J_4		int,
	@vl_Taux_J_3		int,
	@vl_Taux_J_2		int,
	@vl_Taux_J_1		int,
	@vl_defaut		char(1),
	@vl_Fmc_Travaux		int,

	@vl_horodate			datetime,
	@vl_moyenne			int,
	@vl_ecart_maxi			int,
	@vl_ecart_moyen			int,
	@vl_ecart_moyen_anterieur	int,

	@vl_heure		char(8)


select @vl_AujourDHui = getdate()


/* Transformation des parametres d'entree nulls */
if @va_Station = 0
	select @va_Station = null


/* Controle des parametres d'entree obligatoires */
if @va_Date_Demande is null  or  @va_Date_Demande = XDC_DATE_NULLE
	return XDC_ARG_INV

if ( @va_Date_Demande < dateadd (year, -10, @vl_AujourDHui) )  or  ( @va_Date_Demande > @vl_AujourDHui )
	return XDC_OBJ_BORNE


/* Initialisations */
select @vl_Date_Debut = dateadd (day, -7, @va_Date_Demande)
select @vl_Jour_J = @va_Date_Demande
select @vl_Jour_J_1 = dateadd (day, -1, @va_Date_Demande)



/*
** Extraire en table temporaire les points de mesure des stations non supprimees
** ou bien de la station specifiee, ordonnes par autoroute, PR, sens et voie
** -----------------------------------------------------------------------------
*/

select	distinct  P.station, P.sens, P.voie,
	          G.nom, G.autoroute, C.PR
into	#POINTS_DE_MESURE
from	CFG..EQT_PNT_MES P, CFG..RES_CHA C, CFG..EQT_GEN G
where	P.voie != 0
  and	P.station = C.station  and  P.sens = C.sens
  and	G.type = XDC_EQT_RAD  and  P.station = G.numero
  and	G.nom like "RDT%"
  and	( P.station = @va_Station  or
	  ( @va_Station is null  and  G.eqt_supprime = XDC_FAUX ) )
order by G.autoroute, C.PR, P.station, P.sens, P.voie

--select * from #POINTS_DE_MESURE


/* Extraire la table des stations et les sens de leurs points de mesure */
select	distinct station, sens, autoroute, PR, fmc_travaux = Null
into	#STATIONS_SENS
from	#POINTS_DE_MESURE

create unique index IDX_STATIONS_SENS on #STATIONS_SENS (station, sens)

--select station, sens, autoroute, PR from #STATIONS_SENS order by station, sens


/* Extraire la table des stations elles memes */
select	distinct station, indisponible = XDC_FAUX
into	#STATIONS_RADT
from	#STATIONS_SENS

create index IDX_STATIONS on #STATIONS_RADT (station)

--select station from #STATIONS_RADT order by station



/* Creer une table des resultats */
create table #STAT_ETAT_STATIONS (
	Station		smallint	not null,
	Sens		tinyint		not null,
	Voie		tinyint		not null,
	J_7		tinyint		not null,
	J_6		tinyint		not null,
	J_5		tinyint		not null,
	J_4		tinyint		not null,
	J_3		tinyint		not null,
	J_2		tinyint		not null,
	J_1		tinyint		not null,
	Etat_Ok 	bit		not null,
	Defaut		char(1)		null
)



/*
** Creer et initialiser une table des donnees d'un point de mesure dans la periode observee
** ----------------------------------------------------------------------------------------
*/

create table #DONNEES_POINT_MESURE (
	horodate	datetime	not null,
	Q		smallint	null,
	validite_Q	bit
)

create index IDX_HORODATE_MESURE on #DONNEES_POINT_MESURE (horodate)


/* Initialiser la table avec les horodates des mesures dans la periode observee */
select @vl_horodate = @vl_Date_Debut

while @vl_horodate < @vl_Jour_J
begin
	insert #DONNEES_POINT_MESURE (horodate, Q, validite_Q)
	values (@vl_horodate, NULL, XDC_RADT_INCONNU)

	select @vl_horodate = dateadd (minute, CM_PERIODE_MESURES, @vl_horodate)
end

--select * from #DONNEES_POINT_MESURE


/* Creer une table des donnees hebdomadaires pour calcul des ecarts */
create table #DONNEES_HEBDO (
	horodate	datetime	not null,
	debit_moyen	smallint	null,
	ecart_moyen	smallint	null,
	ecart_maximal	smallint	null
)


/* Creer une table de valeurs datees temporaires */
create table #VALEURS_DATEES (
	horodate	datetime	not null,
	valeur		int		null
)

create index IDX_VALEURS_DATEES on #VALEURS_DATEES (horodate)

--select @vl_heure = convert(char(8),getdate(),8)    print "Avant calcul statistiques: %1!", @vl_heure    print ""



/*
** Pour chaque point de mesure, elaborer les statistiques :
** --------------------------------------------------------
*/

declare curseur_point_mesure cursor
	for select station, sens, voie, autoroute, PR
	from #POINTS_DE_MESURE

open curseur_point_mesure
fetch curseur_point_mesure
	into @vl_station, @vl_sens, @vl_voie, @vl_autoroute, @vl_PR

while @@sqlstatus = 0
begin
	/* Remettre a zero les donnees du point de mesure, en conservant les horodates de mesure */
	delete	#DONNEES_POINT_MESURE	where horodate < @vl_Date_Debut
	update	#DONNEES_POINT_MESURE	set Q = null, validite_Q = XDC_RADT_INCONNU

	/* Collecter les donnees du point de mesure courant dans la periode observee */
	update	#DONNEES_POINT_MESURE
	set	Q = M.Q, validite_Q = M.validite_Q
	from	BDD_XPL..MES_TRA M
	where	station = @vl_station  and  sens = @vl_sens  and  voie = @vl_voie
	  and	M.horodate = #DONNEES_POINT_MESURE.horodate

	--select * from #DONNEES_POINT_MESURE


	/*
	** Calculer le pourcentage journalier des donnees presentes pour les jours J-7 a J-1
	*/

	select	@vl_Taux_J_7 = (count(*) * 100) / CM_NB_MES_JOUR  from #DONNEES_POINT_MESURE
	 where	horodate >= dateadd (day, -7, @vl_Jour_J)  and  horodate < dateadd (day, -6, @vl_Jour_J)
	   and	Q >= 0  and  validite_Q = XDC_RADT_CORRECT

	select	@vl_Taux_J_6 = (count(*) * 100) / CM_NB_MES_JOUR  from #DONNEES_POINT_MESURE
	 where	horodate >= dateadd (day, -6, @vl_Jour_J)  and  horodate < dateadd (day, -5, @vl_Jour_J)
	   and	Q >= 0  and  validite_Q = XDC_RADT_CORRECT

	select	@vl_Taux_J_5 = (count(*) * 100) / CM_NB_MES_JOUR  from #DONNEES_POINT_MESURE
	 where	horodate >= dateadd (day, -5, @vl_Jour_J)  and  horodate < dateadd (day, -4, @vl_Jour_J)
	   and	Q >= 0  and  validite_Q = XDC_RADT_CORRECT

	select	@vl_Taux_J_4 = (count(*) * 100) / CM_NB_MES_JOUR  from #DONNEES_POINT_MESURE
	 where	horodate >= dateadd (day, -4, @vl_Jour_J)  and  horodate < dateadd (day, -3, @vl_Jour_J)
	   and	Q >= 0  and  validite_Q = XDC_RADT_CORRECT

	select	@vl_Taux_J_3 = (count(*) * 100) / CM_NB_MES_JOUR  from #DONNEES_POINT_MESURE
	 where	horodate >= dateadd (day, -3, @vl_Jour_J)  and  horodate < dateadd (day, -2, @vl_Jour_J)
	   and	Q >= 0  and  validite_Q = XDC_RADT_CORRECT

	select	@vl_Taux_J_2 = (count(*) * 100) / CM_NB_MES_JOUR  from #DONNEES_POINT_MESURE
	 where	horodate >= dateadd (day, -2, @vl_Jour_J)  and  horodate < @vl_Jour_J_1
	   and	Q >= 0  and  validite_Q = XDC_RADT_CORRECT

	select	@vl_Taux_J_1 = (count(*) * 100) / CM_NB_MES_JOUR  from #DONNEES_POINT_MESURE
	 where	horodate >= @vl_Jour_J_1  and  horodate < @vl_Jour_J
	   and	Q >= 0  and  validite_Q = XDC_RADT_CORRECT



	/*
	** Calculer l'indicateur de defaut suspecte sur les donnees de J-1
	*/

	select @vl_defaut = CM_DEFAUT_AUCUN

	/* Chercher un debit superieur a la limite */
	if exists (select Q from #DONNEES_POINT_MESURE D
		    where horodate >= @vl_Jour_J_1  and  horodate < @vl_Jour_J
		      and (Q * CM_NB_MES_HEURE) > CM_DEBIT_MAX_HEURE
		      and validite_Q = XDC_RADT_CORRECT
		  )
	begin
		select @vl_defaut = CM_DEFAUT_DEBIT
	end


	/* Chercher un debit ecarte de la moyenne journaliere trois fois plus */
	/*  que l'ecart moyen sur les memes cinq jours de semaine precedents */
	if @vl_defaut = CM_DEFAUT_AUCUN
	begin
		/* Pour J-1 et les jours de semaine equivalents des 5 semaines precedentes */
		delete #DONNEES_HEBDO
		delete	#VALEURS_DATEES
		select @vl_horodate = dateadd (day, -5 * 7, @vl_Jour_J_1)
		while @vl_horodate <= @vl_Jour_J_1
		begin
			/* collecter les mesures du jour (sans donnees absentes ou invalides) (deja fait a J-1) */
			if @vl_horodate < @vl_Jour_J_1
			begin
				insert #DONNEES_POINT_MESURE (horodate, Q, validite_Q)
				select horodate, Q, validite_Q from BDD_XPL..MES_TRA
				 where station = @vl_station  and  sens = @vl_sens  and  voie = @vl_voie
				   and horodate >= @vl_horodate  and  horodate < dateadd (day, 1, @vl_horodate)
				   and Q >= 0  and  validite_Q = XDC_RADT_CORRECT
			end

			/* calculer la moyenne journaliere des donnees presentes */
			select @vl_moyenne = avg (Q) from #DONNEES_POINT_MESURE
			 where horodate >= @vl_horodate  and  horodate < dateadd (day, 1, @vl_horodate)
			   and Q >= 0  and  validite_Q = XDC_RADT_CORRECT

			/* puis calculer les ecarts a la moyenne des donnees presentes */
			insert	#VALEURS_DATEES (horodate, valeur)
			select	horodate, abs (Q - @vl_moyenne)
			from	#DONNEES_POINT_MESURE
			where	horodate >= @vl_horodate  and  horodate < dateadd (day, 1, @vl_horodate)
			  and	Q >= 0  and  validite_Q = XDC_RADT_CORRECT

			/* puis calculer la moyenne des ecarts a la moyenne */
			select	@vl_ecart_moyen = avg (valeur) from #VALEURS_DATEES
			where	horodate >= @vl_horodate  and  horodate < dateadd (day, 1, @vl_horodate)

			/* calculer l'ecart maximal a J-1 */
			select	@vl_ecart_maxi = Null
			if @vl_horodate = @vl_Jour_J_1
			begin
				select	@vl_ecart_maxi = max (valeur) from #VALEURS_DATEES
				where	horodate >= @vl_horodate  and  horodate < dateadd (day, 1, @vl_horodate)
			end

			insert #DONNEES_HEBDO (horodate, debit_moyen, ecart_moyen, ecart_maximal)
			values (@vl_horodate, @vl_moyenne, @vl_ecart_moyen, @vl_ecart_maxi)

			select @vl_horodate = dateadd (day, 7, @vl_horodate)
		end
		--print "Station / sens / voie : %1!, %2!, %3!", @vl_station, @vl_sens, @vl_voie
		--select * from #DONNEES_HEBDO

		select @vl_ecart_moyen_anterieur = avg (valeur) from #VALEURS_DATEES where horodate < @vl_Jour_J_1
		if @vl_ecart_maxi > 3 * @vl_ecart_moyen_anterieur
		begin
			select @vl_defaut = CM_DEFAUT_ECART
		end
	end


	/* Chercher dix repetitions de valeur de debit */
	if @vl_defaut = CM_DEFAUT_AUCUN
	begin
		if exists (select * from #DONNEES_POINT_MESURE D
		            where horodate >= @vl_Jour_J_1  and  horodate < @vl_Jour_J
		              and Q >= 0  and  validite_Q = XDC_RADT_CORRECT
		              and (select count(*) from #DONNEES_POINT_MESURE
		                    where horodate >= dateadd (minute, -10 * CM_PERIODE_MESURES, D.horodate)
		                      and horodate < D.horodate
		                      and Q = D.Q  and  validite_Q = XDC_RADT_CORRECT) = 10
		          )
		begin
			select @vl_defaut = CM_DEFAUT_REPETE
		end
	end


	/* Chercher des debits horaires constamment multiples de 10 sur la journee */
	if @vl_defaut = CM_DEFAUT_AUCUN
	begin
		delete	#VALEURS_DATEES
		insert	#VALEURS_DATEES (horodate, valeur)
		select	distinct dateadd (hour, datepart (hour, horodate), @vl_Jour_J_1), sum (Q)
		from	#DONNEES_POINT_MESURE
		where	horodate >= @vl_Jour_J_1  and  horodate < @vl_Jour_J
		  and	Q >= 0  and  validite_Q = XDC_RADT_CORRECT
		group by datepart (hour, horodate)
		having	horodate >= @vl_Jour_J_1  and  horodate < @vl_Jour_J

		if @@rowcount > 0
		begin
			if not exists (select 1 from #VALEURS_DATEES where valeur % 10 != 0)
				select @vl_defaut = CM_DEFAUT_x10
		end
	end



	/*
	** Memoriser les valeurs des statistiques pour ce point de mesure
	*/
	insert #STAT_ETAT_STATIONS
	   ( Station, Sens, Voie,
	     J_7, J_6, J_5, J_4, J_3, J_2, J_1,
	     Etat_Ok,
	     Defaut
	   )
	values
	   ( @vl_station, @vl_sens, @vl_voie,
	     @vl_Taux_J_7, @vl_Taux_J_6, @vl_Taux_J_5, @vl_Taux_J_4, @vl_Taux_J_3, @vl_Taux_J_2, @vl_Taux_J_1,
	     (case when @vl_Taux_J_1 >= 50 then XDC_VRAI else XDC_FAUX end),
	     @vl_defaut
	   )

	fetch curseur_point_mesure
		into @vl_station, @vl_sens, @vl_voie, @vl_autoroute, @vl_PR
end

close curseur_point_mesure

--select @vl_heure = convert(char(8),getdate(),8)    print "Avant recherche travaux: %1!", @vl_heure    print ""




/*
** Pour chaque station et sens de mesure, determiner sa presence dans une zone de travaux au jour J-1
** --------------------------------------------------------------------------------------------------
*/

/* Collecter les FMC travaux en cours dans la periode de statistiques */
select	numero, cle
into	#FMC_TRAVAUX
from	BDD_XPL..FMC_GEN
where	type = XZAEC_FMC_Travaux
  and	debut < @vl_Jour_J  and  (fin is null  or  fin > @vl_Jour_J_1)


declare curseur_station_sens cursor
	for select station, sens, autoroute, PR
	from #STATIONS_SENS
	for update

open curseur_station_sens
fetch curseur_station_sens
	into @vl_station, @vl_sens, @vl_autoroute, @vl_PR

while @@sqlstatus = 0
begin
	select @vl_Fmc_Travaux = F.numero
	           from #FMC_TRAVAUX F, BDD_XPL..FMC_HIS H, BDD_XPL..FMC_TRH T
	           where ( F.numero = H.numero  and  F.cle = H.cle )
	             and H.autoroute = @vl_autoroute
		     and ( H.sens = @vl_sens  or
			    ( @vl_sens = XDC_SENS_SORTIE_NORD  and  H.sens = XDC_SENS_NORD )  or
			    ( @vl_sens = XDC_SENS_SORTIE_SUD  and  H.sens = XDC_SENS_SUD )
			 )
	             and ( H.numero = T.numero  and  H.cle = T.cle  and  H.horodate_validation = T.horodate )
		     and ( @vl_PR between H.PR and T.localisation_tete  or  @vl_PR between T.localisation_tete and H.PR )

		     and H.horodate_validation < @vl_Jour_J
		     and H.horodate_validation >= isnull ((select max(horodate_validation) from BDD_XPL..FMC_HIS
							   where numero = F.numero and cle = F.cle
							     and horodate_validation < @vl_Jour_J_1), @vl_Jour_J_1)

	if @@rowcount > 0
	begin
		update #STATIONS_SENS
			set fmc_travaux = @vl_Fmc_Travaux
		where current of curseur_station_sens
	end

	fetch curseur_station_sens
		into @vl_station, @vl_sens, @vl_autoroute, @vl_PR

end

close curseur_station_sens

--select * from #STATIONS_SENS

--select @vl_heure = convert(char(8),getdate(),8)    print "Avant recherche indispo.: %1!", @vl_heure    print ""




/*
** Pour chaque station, determiner toute indisponibilite au jour J-1
** -----------------------------------------------------------------
*/

update	#STATIONS_RADT
   set	indisponible = XDC_VRAI
 where	exists (select 1 from BDD_XPL..EQT_DSP
 	         where type = XDC_EQT_RAD  and  equipement = #STATIONS_RADT.station
 	           and horodate < @vl_Jour_J
 	           and horodate >= isnull ((select max(horodate) from BDD_XPL..EQT_DSP
 	                                    where type = XDC_EQT_RAD  and  equipement = #STATIONS_RADT.station
 	                                      and horodate < @vl_Jour_J_1), @vl_Jour_J_1)
 	           and majeure | critique | HS | inhibe | desactive = XDC_VRAI
 	       )




/* Retourner les donnees statistiques des points de mesure par station */

select	ES.Station,
	Autoroute = A.nom,
	PM.PR,
	ES.Sens,
	ES.Voie,
	J_7, J_6, J_5, J_4, J_3, J_2, J_1,
	Etat = (case when Etat_Ok = XDC_VRAI then "OK" else " " end),
	Defaut,
	Travaux = (case when SS.fmc_travaux <> null then "T" else " " end),
--	Travaux = SS.fmc_travaux,
	Indisponible = (case when SR.indisponible = XDC_VRAI then "I" else " " end)

from	#STAT_ETAT_STATIONS ES, #POINTS_DE_MESURE PM, #STATIONS_SENS SS, #STATIONS_RADT SR,
	CFG..RES_AUT A

where	ES.Station = PM.station  and  ES.Sens = PM.sens  and  ES.Voie = PM.voie
  and	PM.station = SS.station  and  PM.sens = SS.sens
  and	SS.station = SR.station
  and	PM.autoroute = A.numero

order by PM.autoroute, PM.PR, PM.station, PM.sens, PM.voie

return XDC_OK

go
