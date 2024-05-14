/*X  Fichier : @(#)eqt_bra.vue	0.1      Release : 0.1        Date : 20/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER inf_aire.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue INF_AIRE
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
/*   Vue : INF_AIRE	                                        */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()

/*
*  Fonctionalite de la vue
*  -------------------------
*  Vue des aires de repos
*
*  Description des colonnes de la vue
*  ------------------------------------
*        INF_AIRE_nom			: nom de l'aire
*        INF_AIRE_district		: code du district de l'aire
*	 INF_AIRE_autoroute		: nom de l'autoroute de l'aire
*	 INF_AIRE_pr_entree		: PR d'entree de l'aire en metres
*	 INF_AIRE_pr_sortie		: PR de sortie de l'aire en metres
*	 INF_AIRE_sens			: sens de l'aire
*	 INF_AIRE_vl			: capacite en vl
*	 INF_AIRE_pl			: capacite en pl
*
*/


/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'INF_AIRE' and type = 'V')
	begin
	drop view INF_AIRE
	end
go


create view INF_AIRE
(
INF_AIRE_nom,
INF_AIRE_district,
INF_AIRE_autoroute,
INF_AIRE_pr_entree,
INF_AIRE_pr_sortie,
INF_AIRE_sens,
INF_AIRE_vl,
INF_AIRE_pl
)
as select
AIR1.nom,
POR1.district,
RES1.nom,
AIR1.PRentree,
AIR1.PRsortie,
AIR1.sens,
AIR1.capacite_VL,
AIR1.capacite_PL

from CFG..RES_AIR AIR1,CFG..RES_POR POR1,CFG..RES_AUT RES1

where
/* jointure entre RES_AIR et RES_POR */
/* --------------------------------- */
AIR1.portion=POR1.numero
/* jointure entre RES_POR et RES_AUT */
/* --------------------------------- */
and POR1.autoroute=RES1.numero

/* ajout des droits a public*/
grant all on INF_AIRE to public

go
