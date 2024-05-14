/*X  Fichier : @(#)eqt_bra.vue	0.1      Release : 0.1        Date : 20/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER inf_seg.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue INF_SEG
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
/*   Vue : INF_SEG	                                        */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()

/*
*  Fonctionalite de la vue
*  -------------------------
*  Vue des segments du reseau
*
*  Description des colonnes de la vue
*  ------------------------------------
*	 INF_SEG_portion		: numero de portion
*	 INF_SEG_autoroute		: nom de l'autoroute
*	 INF_SEG_pr_debut		: PR du debut de segment
*	 INF_SEG_pr_fin			: PR de fin de segment
*	 INF_SEG_sens			: sens du segment
*	 INF_SEG_voies			: nombre de voies du segment
*	 INF_SEG_BAU			: largeur de la BAU sur le segment
*	 INF_SEG_presence_tunnel	: tunnel present (1) ou non (0)
*
*/


/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'INF_SEG' and type = 'V')
	begin
	drop view INF_SEG
	end
go


create view INF_SEG
(
INF_SEG_portion,
INF_SEG_autoroute,
INF_SEG_pr_debut,
/*INF_SEG_pr_fin,*/
INF_SEG_sens,
INF_SEG_voies,
INF_SEG_BAU,
INF_SEG_presence_tunnel
)
as select
SEG1.portion,
AUT1.nom,
SEG1.PR_debut,
/*SEG1.PR_fin,*/
SEG1.sens,
SEG1.nombre_de_voies,
SEG1.largeur_BAU,
SEG1.tunnel

from CFG..RES_AUT AUT1,CFG..RES_SEG SEG1,CFG..RES_POR POR1

where
/* jointure entre RES_POR et RES_AUT */
/* --------------------------------- */
POR1.autoroute=AUT1.numero
/* jointure entre RES_SEG et RES_POR */
/* --------------------------------- */
and SEG1.portion=POR1.numero

/* ajout des droits a public*/
grant all on INF_SEG to public

go
