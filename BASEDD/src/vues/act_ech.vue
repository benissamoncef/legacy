/*X  Fichier : @(#)act_ech.vue	0.1      Release : 0.1        Date : 19/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER act_ech.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue ACT_ECH
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
/*   Vue : ACT_ECH	                                        */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()

/*
*  Fonctionalite de la vue
*  -------------------------
*  Contient les actions signalisation echangeur
*
*  Description des colonnes de la vue
*  ------------------------------------
*        ACT_ECH__numero		: numero d'une action signalisation echangeur
*        ACT_ECH__district		: numero du site de l'action
*        ACT_ECH_fmc_liee_numero	: numero de la fiche main courante cause de l'action
*        ACT_ECH_fmc_liee_district	: numero de cle de la fiche main courante cause de l'action
*	 ACT_ECH_nom			: nom de l'echangeur
*	 ACT_ECH_autoroute		: nom de l'autoroute
*	 ACT_ECH_pr			: PR de l'equipement en metres
*	 ACT_ECH_sens			: sens de l'equipement sur l'autoroute    
*        ACT_ECH_heure_lancement	: date de lancement de l'action
*	 ACT_ECH_heure_succes		: heure de succes de l'action
*        ACT_ECH_heure_echec		: date d'echec de l'action
*	 ACT_ECH_heure_lancement_fin	: heure de lancement de la fin de l'action
*	 ACT_ECH_heure_fin		: heure de fin de l'action
*	 ACT_ECH_scenario		: nom du scenario pilote
*	 ACT_ECH_manuel			: pilotage manuel (1) ou automatique (0)
*
*  Remarque
*  --------
*  Le numero d'action est unique par site (district). Pour selectionner une
*  action, il faut preciser son numero et son site.
*
*/


/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'ACT_ECH' and type = 'V')
	begin
	drop view ACT_ECH
	end
go


create view ACT_ECH
(
ACT_ECH__numero,
ACT_ECH__district,
ACT_ECH_fmc_liee_numero,
ACT_ECH_fmc_liee_district,
ACT_ECH_nom,
ACT_ECH_autoroute,
ACT_ECH_pr,
ACT_ECH_sens,
ACT_ECH_heure_lancement,
ACT_ECH_heure_succes,
ACT_ECH_heure_echec,
ACT_ECH_heure_lancement_fin,
ACT_ECH_heure_fin,
ACT_ECH_scenario,
ACT_ECH_manuel
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
SEQ1.nom,
ECH1.manuelle

from HIS..ACT_GEN GEN1,HIS..ACT_ECH ECH1,CFG..EQT_SEQ_ECH SEQ1,
CFG..RES_AUT RES1,CFG..EQT_GEN EQT1

where
/* jointure entre ACT_GEN et ACT_ECH */
/* --------------------------------- */
GEN1.numero=ECH1.actio
and GEN1.sit=ECH1.sit
/* jointure entre EQT_GEN et ACT_GEN */
/* --------------------------------- */
and GEN1.equipement=EQT1.numero
and 11=EQT1.type
/* jointure entre RES_AUT et EQT_GEN */
/* --------------------------------- */
and RES1.numero=*EQT1.autoroute
/* jointure entre EQT_SEQ_ECH et ACT_ECH */
/* ------------------------------------- */
and SEQ1.numero=*ECH1.sequence

/* ajout des droits a public*/
grant all on ACT_ECH to public

go
