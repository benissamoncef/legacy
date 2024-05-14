/*
* Fichier : $Id: FM_Statistiques_Hebdo_RADT.prc,v 1.2 2021/11/08 09:37:08 devgfi Exp $        $Revision: 1.2 $        $Date: 2021/11/08 09:37:08 $
------------------------------------------------------------------------------------------------------------------------
* ESCOTA *  PROJET MIGRAZUR
------------------------------------------------------------------------------------------------------------------------
* SOUS-SYSTEME * OUTILS
------------------------------------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Procédure dont l'objet est d'elaborer des statistiques hebdomadaires sur les valeurs des mesures a une date donnee
*  et sur les jours precedents pour les points de mesures d'une, ou toutes les stations RADT du reseau.
*
------------------------------------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	09/09/21 : Creation
* JPL	02/11/21 : Utilisation du nom 'generique' de la base pour validite sur HIS et autres.
----------------------------------------------------------------------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'FM_Statistiques_Hebdo_RADT' and type = 'P')
	drop procedure FM_Statistiques_Hebdo_RADT
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


create procedure FM_Statistiques_Hebdo_RADT

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
	@vl_Taux_Moyen		int,
	@vl_Nb_Jours_Defaut	tinyint,

	@vl_Nb_Points_Mesure	int,
	@vl_Nb_Taux_0		int,
	@vl_Nb_Taux_0_25	int,
	@vl_Nb_Taux_25_50	int,
	@vl_Nb_Taux_50_75	int,
	@vl_Nb_Taux_75_100	int,
	@vl_Nb_Taux_100		int,

	@vl_horodate		datetime,
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
	Taux_Moyen	tinyint		not null,
	Jours_Defaut	tinyint		not null
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
	** Calculer le pourcentage moyen sur les 7 jours precedents
	*/

	select	@vl_Taux_Moyen = (count(*) * 100) / (7 * CM_NB_MES_JOUR)  from #DONNEES_POINT_MESURE
	 where	Q >= 0  and  validite_Q = XDC_RADT_CORRECT
	   --and	horodate >= dateadd (day, -7, @vl_Jour_J)  and  horodate < @vl_Jour_J


	/*
	** Calculer le nombre de jours consecutifs precedents ayant un pourcentage inferieur a 50 %
	*/

	select @vl_Nb_Jours_Defaut = 7
	if @vl_Taux_J_7 >= 50	select @vl_Nb_Jours_Defaut = 6
	if @vl_Taux_J_6 >= 50	select @vl_Nb_Jours_Defaut = 5
	if @vl_Taux_J_5 >= 50	select @vl_Nb_Jours_Defaut = 4
	if @vl_Taux_J_4 >= 50	select @vl_Nb_Jours_Defaut = 3
	if @vl_Taux_J_3 >= 50	select @vl_Nb_Jours_Defaut = 2
	if @vl_Taux_J_2 >= 50	select @vl_Nb_Jours_Defaut = 1
	if @vl_Taux_J_1 >= 50	select @vl_Nb_Jours_Defaut = 0


	/*
	** Memoriser les valeurs des statistiques pour ce point de mesure
	*/

	insert #STAT_ETAT_STATIONS
	   ( Station, Sens, Voie,
	     J_7, J_6, J_5, J_4, J_3, J_2, J_1,
	     Taux_Moyen, Jours_Defaut
	   )
	values
	   ( @vl_station, @vl_sens, @vl_voie,
	     @vl_Taux_J_7, @vl_Taux_J_6, @vl_Taux_J_5, @vl_Taux_J_4, @vl_Taux_J_3, @vl_Taux_J_2, @vl_Taux_J_1,
	     @vl_Taux_Moyen, @vl_Nb_Jours_Defaut
	   )

	fetch curseur_point_mesure
		into @vl_station, @vl_sens, @vl_voie, @vl_autoroute, @vl_PR
end

close curseur_point_mesure



/* Calculer le pourcentage de points de mesure ayant un pourcentage de donnees dans chaque plage de valeurs */

select @vl_Nb_Points_Mesure = count(*)  from #POINTS_DE_MESURE

select	@vl_Nb_Taux_0 = count(*)	from #STAT_ETAT_STATIONS
 where	(J_7 = 0)  or  (J_6 = 0)  or  (J_5 = 0)  or  (J_4 = 0)  or  (J_3 = 0)  or  (J_2 = 0)  or  (J_1 = 0)

select	@vl_Nb_Taux_0_25 = count(*)	from #STAT_ETAT_STATIONS
 where	(J_7 > 0  and  J_7 < 25)  or  (J_6 > 0  and  J_6 < 25)  or  (J_5 > 0  and  J_5 < 25)
    or	(J_4 > 0  and  J_4 < 25)  or  (J_3 > 0  and  J_3 < 25)
    or	(J_2 > 0  and  J_2 < 25)  or  (J_1 > 0  and  J_1 < 25)

select	@vl_Nb_Taux_25_50 = count(*)	from #STAT_ETAT_STATIONS
 where	(J_7 >= 25  and  J_7 < 50)  or  (J_6 >= 25  and  J_6 < 50)  or  (J_5 >= 25  and  J_5 < 50)
    or	(J_4 >= 25  and  J_4 < 50)  or  (J_3 >= 25  and  J_3 < 50)
    or	(J_2 >= 25  and  J_2 < 50)  or  (J_1 >= 25  and  J_1 < 50)

select	@vl_Nb_Taux_50_75 = count(*)	from #STAT_ETAT_STATIONS
 where	(J_7 >= 50  and  J_7 < 75)  or  (J_6 >= 50  and  J_6 < 75)  or  (J_5 >= 50  and  J_5 < 75)
    or	(J_4 >= 50  and  J_4 < 75)  or  (J_3 >= 50  and  J_3 < 75)
    or	(J_2 >= 50  and  J_2 < 75)  or  (J_1 >= 50  and  J_1 < 75)

select	@vl_Nb_Taux_75_100 = count(*)	from #STAT_ETAT_STATIONS
 where	(J_7 >= 75  and  J_7 < 100)  or  (J_6 >= 75  and  J_6 < 100)  or  (J_5 >= 75  and  J_5 < 100)
    or	(J_4 >= 75  and  J_4 < 100)  or  (J_3 >= 75  and  J_3 < 100)
    or	(J_2 >= 75  and  J_2 < 100)  or  (J_1 >= 75  and  J_1 < 100)

select	@vl_Nb_Taux_100 = count(*)	from #STAT_ETAT_STATIONS
 where	(J_7 = 100)  or  (J_6 = 100)  or  (J_5 = 100)  or  (J_4 = 100)
    or	(J_3 = 100)  or  (J_2 = 100)  or  (J_1 = 100)



/* Retourner les donnees agregees des taux de points de mesure par plage de valeurs */
/* puis les statistiques detaillees de chaque point de mesure par station */

select	"0 %" = ( @vl_Nb_Taux_0 * 100 ) / @vl_Nb_Points_Mesure,
	"0 - 25 %" = ( @vl_Nb_Taux_0_25 * 100 ) / @vl_Nb_Points_Mesure,
	"25 - 50 %" = ( @vl_Nb_Taux_25_50 * 100 ) / @vl_Nb_Points_Mesure,
	"50 - 75 %" = ( @vl_Nb_Taux_50_75 * 100 ) / @vl_Nb_Points_Mesure,
	"75 - 100 %" = ( @vl_Nb_Taux_75_100 * 100 ) / @vl_Nb_Points_Mesure,
	"100 %" = ( @vl_Nb_Taux_100 * 100 ) / @vl_Nb_Points_Mesure


select	ES.Station,
	Autoroute = A.nom,
	PM.PR,
	ES.Sens,
	ES.Voie,
	"J - 7" = J_7, "J - 6" = J_6, "J - 5" = J_5, "J - 4" = J_4, "J - 3" = J_3, "J - 2" = J_2, "J - 1" = J_1,
	"% Moyen" = Taux_Moyen,
	"Nb. Jours < 50%" = Jours_Defaut

from	#STAT_ETAT_STATIONS ES, #POINTS_DE_MESURE PM,
	CFG..RES_AUT A

where	ES.Station = PM.station  and  ES.Sens = PM.sens  and  ES.Voie = PM.voie
  and	PM.autoroute = A.numero

order by PM.autoroute, PM.PR, PM.station, PM.sens, PM.voie

return XDC_OK

go
