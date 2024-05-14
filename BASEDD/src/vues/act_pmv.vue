/*X  Fichier : @(#)act_pmv.vue	0.1      Release : 0.1        Date : 19/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER act_pmv.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue ACT_PMV
*
------------------------------------------------------
* HISTORIQUE :
*
* L.V.	19/08/96	: Creation
* L.V.	11/09/96	: Correction ajout d'une liaison entre EQT_PMV et TYP_PMV
-----------------------------------------------------*/


/* ============================================================ */
/*   Nom de la base   :  MIGRAZUR                               */
/*   Nom de SGBD      :  SYBASE10                               */
/*   Date de creation :  19/08/96                               */
/* ============================================================ */

/* ============================================================ */
/*   Vue : ACT_PMV	                                        */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()

/*
*  Fonctionalite de la vue
*  -------------------------
*  Contient les affichages PMV
*
*  Description des colonnes de la vue
*  ------------------------------------
*        ACT_PMV__numero		: numero d'une action PMV
*        ACT_PMV__district		: numero du site de l'action PMV
*        ACT_PMV_fmc_liee_numero	: numero de la fiche main courante cause de l'action
*        ACT_PMV_fmc_liee_district	: numero de cle de la fiche main courante cause de l'action
*	 ACT_PMV_autoroute		: nom de l'autoroute
*	 ACT_PMV_pr			: PR de l'equipement en metres
*	 ACT_PMV_sens			: sens de l'equipement sur l'autoroute
*	 ACT_PMV_type_texte		: nom du type de PMV    
*        ACT_PMV_heure_lancement	: date de lancement de l'action
*	 ACT_PMV_heure_succes		: heure de succes de l'action
*        ACT_PMV_heure_echec		: date d'echec de l'action
*	 ACT_PMV_heure_lancement_fin	: heure de lancement de la fin de l'action
*	 ACT_PMV_heure_fin		: heure de fin de l'action
*	 ACT_PMV_ligne1			: premiere ligne
*	 ACT_PMV_ligne2			: deuxieme ligne
*	 ACT_PMV_ligne3			: troisieme ligne
*	 ACT_PMV_alternat1		: premiere ligne d'alternat
*	 ACT_PMV_alternat2		: deuxieme ligne d'alternat
*	 ACT_PMV_alternat3		: troisieme ligne d'alternat
*	 ACT_PMV_picto			: code du pictogramme
*	 ACT_PMV_bandeau		: texte du bandeau du pictogramme
*	 ACT_PMV_nom			: nom du PMV
*
*  Remarque
*  --------
*  Le numero d'action est unique par site (district). Pour selectionner une
*  action, il faut preciser son numero et son site.
*
*/


/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'ACT_PMV' and type = 'V')
	begin
	drop view ACT_PMV
	end
go


create view ACT_PMV
(
ACT_PMV__numero,
ACT_PMV__district,
ACT_PMV_fmc_liee_numero,
ACT_PMV_fmc_liee_district,
ACT_PMV_autoroute,
ACT_PMV_pr,
ACT_PMV_sens,
ACT_PMV_type_texte,
ACT_PMV_heure_lancement,
ACT_PMV_heure_succes,
ACT_PMV_heure_echec,
ACT_PMV_heure_lancement_fin,
ACT_PMV_heure_fin,
ACT_PMV_ligne1,
ACT_PMV_ligne2,
ACT_PMV_ligne3,
ACT_PMV_alternat1,
ACT_PMV_alternat2,
ACT_PMV_alternat3,
ACT_PMV_picto,
ACT_PMV_bandeau,
ACT_PMV_nom
)
as select
GEN1.numero,
GEN1.sit,
GEN1.evenement,
GEN1.cle,
RES1.nom,
EQT1.PR,
EQT1.sens,
TYP1.nom,
GEN1.heure_lancement,
GEN1.heure_succes,
GEN1.heure_echec,
GEN1.heure_lancement_fin,
GEN1.heure_fin,
PMV1.premiere_ligne,
PMV1.deuxieme_ligne,
PMV1.troisieme_ligne,
PMV1.premiere_ligne_alternat,
PMV1.deuxieme_ligne_alternat,
PMV1.troisieme_ligne_alternat,
PMV1.pictogramme,
PMV1.bandeau,
EQT1.nom

from HIS..ACT_GEN GEN1,HIS..ACT_PMV PMV1,CFG..EQT_GEN EQT1,
CFG..RES_AUT RES1,CFG..TYP_PMV TYP1,CFG..EQT_PMV EPM1

where
/* jointure entre ACT_GEN et ACT_PMV */
/* --------------------------------- */
GEN1.numero=PMV1.actio
and GEN1.sit=PMV1.sit
/* jointure entre EQT_GEN et ACT_GEN */
/* --------------------------------- */
and GEN1.equipement=EQT1.numero
and 2=EQT1.type
/* jointure entre RES_AUT et EQT_GEN */
/* --------------------------------- */
and RES1.numero=*EQT1.autoroute
/* jointure entre EQT_GEN et EQT_PMV */
/* --------------------------------- */
and GEN1.numero=EPM1.numero
/* jointure entre TYP_PMV et EQT_PMV */
/* --------------------------------- */
and EPM1.numero*=TYP1.numero

/* ajout des droits a public*/
grant all on ACT_PMV to public

go
