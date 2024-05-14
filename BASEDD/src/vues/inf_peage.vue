/*X  Fichier : @(#)eqt_bra.vue	0.1      Release : 0.1        Date : 20/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER inf_peage.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue INF_PEAGE
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
/*   Vue : INF_PEAGE	                                        */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()

/*
*  Fonctionalite de la vue
*  -------------------------
*  Vue des peages
*
*  Description des colonnes de la vue
*  ------------------------------------
*        INF_PEAGE_nom			: nom du peage
*        INF_PEAGE_district		: code du district du peage
*	 INF_PEAGE_autoroute		: nom de l'autoroute du peage
*	 INF_PEAGE_sens1_pr		: PR de la barriere dans le sens 1
*	 INF_PEAGE_sens1_longueur	: longueur de la plateforme dans le sens 1
*	 INF_PEAGE_sens2_pr		: PR de la barriere dans le sens 2
*	 INF_PEAGE_sens2_longueur	: longueur de la plateforme dans le sens 2
*
*/


/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'INF_PEAGE' and type = 'V')
	begin
	drop view INF_PEAGE
	end
go


create view INF_PEAGE
(
INF_PEAGE_nom,
INF_PEAGE_district,
INF_PEAGE_autoroute,
INF_PEAGE_sens1_pr,
INF_PEAGE_sens1_longueur,
INF_PEAGE_sens2_pr,
INF_PEAGE_sens2_longueur
)
as select
PEA1.nom,
POR1.district,
AUT1.nom,
PEA1.PRsens1,
PEA1.longueur_sens1,
PEA1.PRsens2,
PEA1.longueur_sens2

from CFG..RES_PEA PEA1,CFG..RES_POR POR1,CFG..RES_AUT AUT1

where
/* jointure entre RES_POR et RES_AUT */
/* --------------------------------- */
POR1.autoroute=AUT1.numero
/* jointure entre RES_POR et RES_PEA */
/* --------------------------------- */
and PEA1.portion=POR1.numero

/* ajout des droits a public*/
grant all on INF_PEAGE to public

go
