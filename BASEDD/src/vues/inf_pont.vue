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
* creation de la vue INF_PONT
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
/*   Vue : INF_PONT	                                        */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()

/*
*  Fonctionalite de la vue
*  -------------------------
*  Vue des passages superieurs
*
*  Description des colonnes de la vue
*  ------------------------------------
*	 INF_PONT_nom			: nom du passage superieur
*        INF_PONT_district		: code du district
*	 INF_PONT_autoroute		: nom de l'autoroute
*	 INF_PONT_pr			: PR du portail
*	 INF_PONT_gabarit_sens1		: gabarit du passage superieur dans le sens 1, en cm
*	 INF_PONT_gabarit_sens2		: gabarit du passage superieur dans le sens 2, en cm
*
*/


/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'INF_PONT' and type = 'V')
	begin
	drop view INF_PONT
	end
go


create view INF_PONT
(
INF_PONT_nom,
INF_PONT_district,
INF_PONT_autoroute,
INF_PONT_pr,
INF_PONT_gabarit_sens1,
INF_PONT_gabarit_sens2
)
as select
SUP1.nom,
POR1.district,
AUT1.nom,
SUP1.PR,
SUP1.gabarit_sens1,
SUP1.gabarit_sens2

from CFG..RES_POR POR1,CFG..RES_AUT AUT1,CFG..RES_SUP SUP1

where
/* jointure entre RES_SUP et RES_POR */
/* --------------------------------- */
SUP1.portion=POR1.numero
/* jointure entre RES_POR et RES_AUT */
/* --------------------------------- */
and POR1.autoroute=AUT1.numero

/* ajout des droits a public*/
grant all on INF_PONT to public

go
