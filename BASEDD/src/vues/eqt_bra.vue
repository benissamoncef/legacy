/*X  Fichier : @(#)eqt_bra.vue	0.1      Release : 0.1        Date : 20/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER eqt_bra.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue EQT_BRA
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
/*   Vue : EQT_BRA	                                        */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()

/*
*  Fonctionalite de la vue
*  -------------------------
*  Vue des biseaux de rabattement automatique
*
*  Description des colonnes de la vue
*  ------------------------------------
*        EQT_BRA__numero		: numero de l'equipement
*        EQT_BRA__numero_type		: numero du type de l'equipement
*	 EQT_BRA_type			: nom du type de l'equipement
*	 EQT_BRA_autoroute		: nom de l'autoroute de l'equipement
*	 EQT_BRA_pr			: PR de l'equipement en metres
*	 EQT_BRA_pr_balise		: PR de la balise en metres
*	 EQT_BRA_sens			: sens de l'equipement sur l'autoroute
*
*  Remarque
*  --------
*  Un numero d'equipement est unique par type d'equipement.
*
*/


/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'EQT_BRA' and type = 'V')
	begin
	drop view EQT_BRA
	end
go


create view EQT_BRA
(
EQT_BRA__numero,
EQT_BRA__numero_type,
EQT_BRA_type,
EQT_BRA_autoroute,
EQT_BRA_pr,
EQT_BRA_pr_balise,
EQT_sens
)
as select
EQT1.numero,
EQT1.type,
TYP1.nom,
RES1.nom,
EQT1.PR,
NAV1.PR_influence,
EQT1.sens

from CFG..EQT_GEN EQT1,CFG..RES_AUT RES1,CFG..TYP_NAV TYP1,CFG..EQT_NAV NAV1

where
/* jointure entre EQT_GEN et EQT_NAV */
/* --------------------------------- */
EQT1.numero=NAV1.numero
and EQT1.type=1
/* jointure entre EQT_NAV et TYP_NAV */
/* --------------------------------- */
and NAV1.type=TYP1.numero
/* jointure entre EQT_GEN et RES_AUT */
/* --------------------------------- */
and EQT1.autoroute*=RES1.numero

/* ajout des droits a public*/
grant all on EQT_BRA to public

go
