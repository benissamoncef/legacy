/*X  Fichier : @(#)eqt.vue	0.1      Release : 0.1        Date : 20/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER eqt.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue EQT
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
/*   Vue : EQT_BRA	                                        */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()

/*
*  Fonctionalite de la vue
*  -------------------------
*  Vue generique de tous les equipements
*
*  Description des colonnes de la vue
*  ------------------------------------
*        EQT__numero		: numero de l'equipement
*        EQT__numero_type	: numero du type de l'equipement
*	 EQT_type		: nom du type de l'equipement
*	 EQT_nom		: nom de l'equipement
*	 EQT_autoroute		: nom de l'autoroute de l'equipement
*	 EQT_pr			: PR de l'equipement en metres
*	 EQT_sens		: sens de l'equipement sur l'autoroute
*
*  Remarque
*  --------
*  Un numero d'equipement est unique par type d'equipement.
*
*/


/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'EQT' and type = 'V')
	begin
	drop view EQT
	end
go


create view EQT
(
EQT__numero,
EQT__numero_type,
EQT_type,
EQT_nom,
EQT_autoroute,
EQT_pr,
EQT_sens
)
as select
EQT1.numero,
EQT1.type,
DICO1.libelle,
EQT1.nom,
RES1.nom,
EQT1.PR,
EQT1.sens

from CFG..EQT_GEN EQT1,CFG..RES_AUT RES1,VUE..DICO DICO1

where
/* jointure entre EQT_GEN et DICO */
/* ------------------------------ */
EQT1.type=DICO1.code
and 23=DICO1.notyp
/* jointure entre EQT_GEN et RES_AUT */
/* --------------------------------- */
and EQT1.autoroute*=RES1.numero

/* ajout des droits a public*/
grant all on EQT to public

go
