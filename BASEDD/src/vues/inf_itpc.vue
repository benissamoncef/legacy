/*X  Fichier : @(#)eqt_bra.vue	0.1      Release : 0.1        Date : 20/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER inf_itpc.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue INF_ITPC
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
/*   Vue : INF_ITPC	                                        */
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
*        INF_ITPC_district		: code du district de l'ITPC
*	 INF_ITPC_autoroute		: nom de l'autoroute de l'ITPC
*	 INF_ITPC_pr			: PR de l'ITPC
*
*/


/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'INF_ITPC' and type = 'V')
	begin
	drop view INF_ITPC
	end
go


create view INF_ITPC
(
INF_ITPC_district,
INF_ITPC_autoroute,
INF_ITPC_pr
)
as select
POR1.district,
AUT1.nom,
ITP1.PR

from CFG..RES_POR POR1,CFG..RES_AUT AUT1,CFG..RES_ITP ITP1

where
/* jointure entre RES_ITP et RES_POR */
/* --------------------------------- */
ITP1.portion=POR1.numero
/* jointure entre RES_POR et RES_AUT */
/* --------------------------------- */
and POR1.autoroute=AUT1.numero

/* ajout des droits a public*/
grant all on INF_ITPC to public

go
