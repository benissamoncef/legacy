/*X  Fichier : @(#)eqt_bra.vue	0.1      Release : 0.1        Date : 20/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER inf_passinf.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue INF_PASSINF
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
/*   Vue : INF_PASSINF	                                        */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()

/*
*  Fonctionalite de la vue
*  -------------------------
*  Vue des reseaux transversaux (sauf passages inf)
*
*  Description des colonnes de la vue
*  ------------------------------------
*	 INF_PASSINF_nom			: nom du reseau transversal
*        INF_PASSINF_district		: code du district
*	 INF_PASSINF_autoroute		: nom de l'autoroute
*	 INF_PASSINF_pr			: PR du reseau transversal
*	 INF_PASSINF_type		: nom du type de reseau		
*
*/


/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'INF_PASSINF' and type = 'V')
	begin
	drop view INF_PASSINF
	end
go


create view INF_PASSINF
(
INF_PASSINF_nom,
INF_PASSINF_district,
INF_PASSINF_autoroute,
INF_PASSINF_pr
)
as select
TRN1.nom,
POR1.district,
AUT1.nom,
TRN1.PR

from CFG..RES_POR POR1,CFG..RES_AUT AUT1,CFG..RES_TRN TRN1

where
/* jointure entre RES_TRN et RES_POR */
/* --------------------------------- */
TRN1.portion=POR1.numero
/* jointure entre RES_POR et RES_AUT */
/* --------------------------------- */
and POR1.autoroute=AUT1.numero
/* condition de selection des passages inferieurs */
/* ---------------------------------------------- */
and TRN1.type=8

/* ajout des droits a public*/
grant all on INF_PASSINF to public

go
