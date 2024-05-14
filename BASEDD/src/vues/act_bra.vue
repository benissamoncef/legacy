/*X  Fichier : @(#)act_bra.vue	0.1      Release : 0.1        Date : 19/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER act_bra.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue ACT_BRA
*
------------------------------------------------------
* HISTORIQUE :
*
* L.V.	19/08/96	: Creation
-----------------------------------------------------*/


/* ============================================================ */
/*   Nom de la base   :  MIGRAZUR                               */
/*   Nom de SGBD      :  SYBASE10                               */
/*   Date de creation :  19/08/96                               */
/* ============================================================ */

/* ============================================================ */
/*   Vue : ACT_BRA	                                        */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()

/*
*  Fonctionalite de la vue
*  -------------------------
*  Contient les actions BRA
*
*  Description des colonnes de la vue
*  ------------------------------------
*        ACT_BRA__numero		: numero d'une action BRA
*        ACT_BRA__district		: numero du site de l'action
*        ACT_BRA_fmc_liee_numero	: numero de la fiche main courante cause de l'action
*        ACT_BRA_fmc_liee_district	: numero de cle de la fiche main courante cause de l'action
*	 ACT_BRA_nom			: nom du BRA
*	 ACT_BRA_autoroute		: nom de l'autoroute
*	 ACT_BRA_pr			: PR de l'equipement en metres
*	 ACT_BRA_sens			: sens de l'equipement sur l'autoroute    
*        ACT_BRA_heure_lancement	: date de lancement de l'action
*	 ACT_BRA_heure_succes		: heure de succes de l'action
*        ACT_BRA_heure_echec		: date d'echec de l'action
*	 ACT_BRA_heure_lancement_fin	: heure de lancement de la fin de l'action
*	 ACT_BRA_heure_fin		: heure de fin de l'action
*	 ACT_BRA_scenario		: nom du scenario pilote
*	 ACT_BRA_manuel			: pilotage manuel (1) ou automatique (0)
*
*  Remarque
*  --------
*  Le numero d'action est unique par site (district). Pour selectionner une
*  action, il faut preciser son numero et son site.
*
*/


/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'ACT_BRA' and type = 'V')
	begin
	drop view ACT_BRA
	end
go


create view ACT_BRA
(
ACT_BRA__numero,
ACT_BRA__district,
ACT_BRA_fmc_liee_numero,
ACT_BRA_fmc_liee_district,
ACT_BRA_nom,
ACT_BRA_autoroute,
ACT_BRA_pr,
ACT_BRA_sens,
ACT_BRA_heure_lancement,
ACT_BRA_heure_succes,
ACT_BRA_heure_echec,
ACT_BRA_heure_lancement_fin,
ACT_BRA_heure_fin,
ACT_BRA_scenario,
ACT_BRA_manuel
)
as select
GEN1.numero,
GEN1.sit,
GEN1.evenement,
GEN1.cle,
EQT1.nom,
RES1.nom,
EQT1.PR,
EQT1.sens,
GEN1.heure_lancement,
GEN1.heure_succes,
GEN1.heure_echec,
GEN1.heure_lancement_fin,
GEN1.heure_fin,
SCN1.nom,
NAV1.manuelle

from HIS..ACT_GEN GEN1,HIS..ACT_NAV NAV1,CFG..EQT_SCN_NAV SCN1,
CFG..RES_AUT RES1,CFG..EQT_GEN EQT1

where
/* jointure entre ACT_GEN et ACT_NAV */
/* --------------------------------- */
GEN1.numero=NAV1.actio
and GEN1.sit=NAV1.sit
/* jointure entre EQT_GEN et ACT_GEN */
/* --------------------------------- */
and GEN1.equipement=EQT1.numero
and 1=EQT1.type
/* jointure entre RES_AUT et EQT_GEN */
/* --------------------------------- */
and RES1.numero=*EQT1.autoroute
/* jointure entre EQT_SCN_NAV et ACT_NAV */
/* ------------------------------------- */
and SCN1.scenario=NAV1.scenario

/* ajout des droits a public*/
grant all on ACT_BRA to public

go
