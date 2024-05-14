/*X  Fichier : @(#)eqt_bra.vue	0.1      Release : 0.1        Date : 20/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER eqt_pmv.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue EQT_PMV
*
------------------------------------------------------
* HISTORIQUE :
*
* L.V.	19/08/96	: Creation
-----------------------------------------------------*/


/* ============================================================ */
/*   Nom de la base   :  MIGRAZUR                               */
/*   Nom de SGBD      :  SYBASE10                               */
/*   Date de creation :  20/08/96                               */
/* ============================================================ */

/* ============================================================ */
/*   Vue : EQT_PMV	                                        */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()

/*
*  Fonctionalite de la vue
*  -------------------------
*  Vue des PMV
*
*  Description des colonnes de la vue
*  ------------------------------------
*        EQT_PMV__numero		: numero de l'equipement
*        EQT_PMV__numero_type		: numero du type de PMV
*	 EQT_PMV_type_texte		: nom du type de PMV
*	 EQT_PMV_autoroute		: nom de l'autoroute de l'equipement
*	 EQT_PMV_pr			: PR de l'equipement en metres
*	 EQT_PMV_sens			: sens de l'equipement sur l'autoroute
*
*  Remarque
*  --------
*  Un numero d'equipement est unique par type d'equipement.
*
*/


/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'EQT_PMV' and type = 'V')
	begin
	drop view EQT_PMV
	end
go


create view EQT_PMV
(
EQT_PMV__numero,
EQT_PMV__numero_type,
EQT_PMV_type_texte,
EQT_PMV_autoroute,
EQT_PMV_pr,
EQT_PMV_sens
)
as select
EQT1.numero,
EQT1.type,
TYP1.nom,
RES1.nom,
EQT1.PR,
EQT1.sens

from CFG..EQT_GEN EQT1,CFG..EQT_PMV PMV1,CFG..TYP_PMV TYP1,CFG..RES_AUT RES1

where
/* jointure entre EQT_GEN et EQT_PMV */
/* --------------------------------- */
EQT1.numero=PMV1.numero
and EQT1.type=2
/* jointure entre EQT_GEN et RES_AUT */
/* --------------------------------- */
and EQT1.autoroute*=RES1.numero
/* jointure entre EQT_PMV et TYP_PMV */
/* --------------------------------- */
and TYP1.numero=*PMV1.type

/* ajout des droits a public*/
grant all on EQT_PMV to public

go
