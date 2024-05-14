/*X  Fichier : @(#)eqt_bra.vue	0.1      Release : 0.1        Date : 20/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER inf_portail.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue INF_PORTAIL
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
/*   Vue : INF_PORTAIL	                                        */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()

/*
*  Fonctionalite de la vue
*  -------------------------
*  Vue des interruptions de terre plein central
*
*  Description des colonnes de la vue
*  ------------------------------------
*        INF_PORTAIL_district		: code du district
*	 INF_PORTAIL_autoroute		: nom de l'autoroute
*	 INF_PORTAIL_pr			: PR du portail
*	 INF_PORTAIL_sens		: sens d'implantation
*
*/


/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'INF_PORTAIL' and type = 'V')
	begin
	drop view INF_PORTAIL
	end
go


create view INF_PORTAIL
(
INF_PORTAIL_district,
INF_PORTAIL_autoroute,
INF_PORTAIL_pr,
INF_PORTAIL_sens
)
as select
POR1.district,
AUT1.nom,
PTL1.PR,
PTL1.sens

from CFG..RES_POR POR1,CFG..RES_AUT AUT1,CFG..RES_PTL PTL1

where
/* jointure entre RES_PTL et RES_POR */
/* --------------------------------- */
PTL1.portion=POR1.numero
/* jointure entre RES_POR et RES_AUT */
/* --------------------------------- */
and POR1.autoroute=AUT1.numero

/* ajout des droits a public*/
grant all on INF_PORTAIL to public

go
