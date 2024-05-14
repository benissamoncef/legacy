/*X  Fichier : @(#)eqt_bra.vue	0.1      Release : 0.1        Date : 20/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER inf_vitesse.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue INF_VITESSE
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
/*   Vue : INF_VITESSE	                                        */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()

/*
*  Fonctionalite de la vue
*  -------------------------
*  Vue des prescriptions de vitesse
*
*  Description des colonnes de la vue
*  ------------------------------------
*        INF_VITESSE_district		: code du district
*	 INF_VITESSE_autoroute		: nom de l'autoroute
*	 INF_VITESSE_pr			: PR du panneau de limitation
*	 INF_VITESSE_sens		: sens du panneau
*	 INF_VITESSE_prescription	: valeur de la vitesse prescrite
*
*/


/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'INF_VITESSE' and type = 'V')
	begin
	drop view INF_VITESSE
	end
go


create view INF_VITESSE
(
INF_VITESSE_district,
INF_VITESSE_autoroute,
INF_VITESSE_pr,
INF_VITESSE_sens,
INF_VITESSE_prescription
)
as select
POR1.district,
AUT1.nom,
POL1.PR,
POL1.sens,
POL1.vitesse

from CFG..RES_POL POL1,CFG..RES_POR POR1,CFG..RES_AUT AUT1

where
/* jointure entre RES_POR et RES_AUT */
/* --------------------------------- */
POR1.autoroute=AUT1.numero
/* jointure entre RES_POR et RES_POL */
/* --------------------------------- */
and POL1.portion=POR1.numero

/* ajout des droits a public*/
grant all on INF_VITESSE to public

go
