/*X  Fichier : @(#)eqt_bra.vue	0.1      Release : 0.1        Date : 20/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER inf_ech.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue INF_ECH
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
/*   Vue : INF_ECH	                                        */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()

/*
*  Fonctionalite de la vue
*  -------------------------
*  Vue des echangeurs
*
*  Description des colonnes de la vue
*  ------------------------------------
*        INF_ECH_nom			: nom de l'echangeur
*	 INF_ECH_numero			: numero de sortie
*        INF_ECH_district		: code du district de l'echangeur
*	 INF_ECH_autoroute		: nom de l'autoroute de l'echangeur
*	 INF_ECH_pr			: PR de l'echangeur en metres
*	 INF_ECH_entree_1_voies		: nbre de voies en entree dans le sens 1
*	 INF_ECH_entree_1_peage		: presence d'un peage en entree dans le sens 1
*	 INF_ECH_entree_1_pr		: PR de l'entree dans le sens 1
*	 INF_ECH_entree_2_voies		: nbre de voies en entree dans le sens 2
*	 INF_ECH_entree_2_peage		: presence d'un peage en entree dans le sens 2
*	 INF_ECH_entree_2_pr		: PR de l'entree dans le sens 2
*	 INF_ECH_sortie_1_voies		: nbre de voies en sortie dans le sens 1
*	 INF_ECH_sortie_1_peage		: presence d'un peage en sortie dans le sens 1
*	 INF_ECH_sortie_1_pr		: PR de la sortie dans le sens 1
*	 INF_ECH_sortie_2_voies		: nbre de voies en sortie dans le sens 2
*	 INF_ECH_sortie_2_peage		: presence d'un peage en sortie dans le sens 2
*	 INF_ECH_sortie_2_pr		: PR de la sortie dans le sens 2
*
*/


/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'INF_ECH' and type = 'V')
	begin
	drop view INF_ECH
	end
go


create view INF_ECH
(
INF_ECH_nom,
INF_ECH_numero,
INF_ECH_district,
INF_ECH_autoroute,
INF_ECH_pr,
INF_ECH_entree_1_voies,
INF_ECH_entree_1_peage,
INF_ECH_entree_1_pr,
INF_ECH_entree_2_voies,
INF_ECH_entree_2_peage,
INF_ECH_entree_2_pr,
INF_ECH_sortie_1_voies,
INF_ECH_sortie_1_peage,
INF_ECH_sortie_1_pr,
INF_ECH_sortie_2_voies,
INF_ECH_sortie_2_peage,
INF_ECH_sortie_2_pr
)
as select
ECH1.nom,
isnull(NUMECH1.sortie,ECH1.numero),
POR1.district,
AUT1.nom,
ECH1.PR,
ECH1.voies_entree_sens1,
ECH1.peage_entree_sens1,
ECH1.PR_entree_sens1,
ECH1.voies_entree_sens2,
ECH1.peage_entree_sens2,
ECH1.PR_entree_sens2,
ECH1.voies_sortie_sens1,
ECH1.peage_sortie_sens1,
ECH1.PR_sortie_sens1,
ECH1.voies_sortie_sens2,
ECH1.peage_sortie_sens2,
ECH1.PR_sortie_sens2

from CFG..RES_ECH ECH1,CFG..RES_POR POR1,CFG..RES_AUT AUT1,CFG..RES_NUM_ECH NUMECH1

where
/* jointure externe entre RES_ECH et RES_NUM_ECH */
/* --------------------------------------------- */
ECH1.numero*=NUMECH1.numero
/* jointure entre RES_POR et RES_AUT */
/* --------------------------------- */
and POR1.autoroute=AUT1.numero
/* jointure entre RES_POR et RES_ECH */
/* --------------------------------- */
and ECH1.portion=POR1.numero

/* ajout des droits a public*/
grant all on INF_ECH to public

go
