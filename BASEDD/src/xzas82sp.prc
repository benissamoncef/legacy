/*E*/
/*  Fichier : @(#)xzas82.prc	1.2      Release : 1.2      Date : 07/18/03
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas82sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module aweb : Retourne la liste des Q, V, T_O ... pour TDP
* Remarque :
*
------------------------------------------------------
* HISTORIQUE :
*
* Cluchague     20/10/04  : Creation 1.1 et  1.2
* Cluchague     01/12/04  : mise en place de toutes les stations 1.3
* Cluchague     01/12/04  : uniformite de la date selon la plus recente 1.4
-----------------------------------------------------*/
/*X*/
/*-----------------------------------------------------

* Sequence d"appel
* SP    XZAS82
*
* Arguments en entree
*
* Arguments en sortie
*
* aucun :
*
* Service rendu
*
* Code retour
* XDC_OK
* XDC_NOK       :
* XDC_ARG_INV   : parametres d"entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d"utilisation
*
*
* Fonction
-----------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAS82' and type = 'P')

    drop procedure XZAS82

go

create procedure XZAS82 
as

declare
@vl_debut datetime,
@vl_fin   datetime,
@vl_horodate_recente char(12)

   create table #TMP_RADT
	 (
		identifiant  char(20) NULL ,
		horodate     char(50) NULL ,
		sens         int NULL ,
		validite     int NULL ,
                Q            int NULL ,
		QPL          int NULL ,
		V            int NULL ,
		TOC           int NULL
         )

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      recuperation des infos en base                                                  */
/************************************************************************************************************************/
/************************************************************************************************************************/

/* calcul de la tranche de date a traiter */
select @vl_debut = dateadd(minute,-18,getdate())
select @vl_fin =   getdate()

/* insertion dans table temporaire des valeurs pour les stations */
insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=116
and T.sens=1 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=117
and T.sens=2 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=74
and T.sens=1 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=75
and T.sens=2 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=72
and T.sens=1 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=73
and T.sens=2 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=68
and T.sens=1 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=69
and T.sens=2 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=66
and T.sens=1 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=67
and T.sens=2 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=64
and T.sens=1 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=65
and T.sens=2 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=118
and T.sens=1 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=119
and T.sens=2 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=60
and T.sens=1 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=61
and T.sens=2 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=57
and T.sens=1 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=58
and T.sens=2 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=55
and T.sens=2 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=56
and T.sens=1 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=25
and T.sens=1 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=9
and T.sens=1 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=10
and T.sens=2 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=11
and T.sens=1 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=12
and T.sens=2 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=13
and T.sens=1 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=14
and T.sens=2 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=15
and T.sens=1 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=16
and T.sens=2 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=17
and T.sens=1 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=18
and T.sens=2 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=19
and T.sens=1 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=20
and T.sens=2 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=21
and T.sens=1 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=22
and T.sens=2 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=23
and T.sens=1 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=24
and T.sens=2 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=80
and T.sens=1 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=81
and T.sens=2 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=1
and T.sens=1 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=2
and T.sens=2 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=46
and T.sens=1 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=47
and T.sens=1 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=48
and T.sens=2 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=5
and T.sens=1 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=6
and T.sens=2 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=83
and T.sens=1 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=84
and T.sens=2 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=85
and T.sens=1 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=86
and T.sens=2 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=34
and T.sens=1 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=35
and T.sens=2 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=87
and T.sens=1 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

insert #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=88
and T.sens=2 and T.voie=0 and T.horodate between @vl_debut and @vl_fin

/* on cherche l'horodate la plus recente pour masquer les lenteurs de mise en base */
select @vl_horodate_recente=max(horodate) from #TMP_RADT

/*valeurs a retourner*/

select  identifiant,@vl_horodate_recente,sens,validite,Q,QPL,V,TOC  
      from #TMP_RADT group by identifiant,sens having horodate = max(horodate) order by identifiant, sens

drop table #TMP_RADT

return (XDC_OK)

go

