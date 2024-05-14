/*X  Fichier : @(#)eqt_bra.vue	0.1      Release : 0.1        Date : 20/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER inf_pente.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue INF_PENTE
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
/*   Vue : INF_PENTE	                                        */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()

/*
*  Fonctionalite de la vue
*  -------------------------
*  Vue des pentes et rames
*
*  Description des colonnes de la vue
*  ------------------------------------
*        INF_PENTE_nom			: nom de la pente ou rame
*        INF_PENTE_district		: code du district de la pente
*	 INF_PENTE_autoroute		: nom de l'autoroute de la pente
*	 INF_PENTE_pr_debut		: PR du debut de pente
*	 INF_PENTE_pr_fin		: PR de fin de pente
*	 INF_PENTE_inclinaison		: pourcentage de la rampe ou de la pente
*
*/


/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'INF_PENTE' and type = 'V')
	begin
	drop view INF_PENTE
	end
go


create view INF_PENTE
(
INF_PENTE_nom,
INF_PENTE_district,
INF_PENTE_autoroute,
INF_PENTE_pr_debut,
INF_PENTE_pr_fin,
INF_PENTE_inclinaison
)
as select
PNT1.nom,
POR1.district,
AUT1.nom,
PNT1.PR_debut,
PNT1.PR_fin,
PNT1.pente

from CFG..RES_PNT PNT1,CFG..RES_POR POR1,CFG..RES_AUT AUT1

where
/* jointure entre RES_POR et RES_AUT */
/* --------------------------------- */
POR1.autoroute=AUT1.numero
/* jointure entre RES_POR et RES_PNT */
/* --------------------------------- */
and PNT1.portion=POR1.numero

/* ajout des droits a public*/
grant all on INF_PENTE to public

go
