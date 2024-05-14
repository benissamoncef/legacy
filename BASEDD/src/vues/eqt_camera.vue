/*X  Fichier : @(#)eqt_bra.vue	0.1      Release : 0.1        Date : 20/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER eqt_camera.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue EQT_CAMERA
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
/*   Vue : EQT_CAMERA	                                        */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()

/*
*  Fonctionalite de la vue
*  -------------------------
*  Vue des cameras
*
*  Description des colonnes de la vue
*  ------------------------------------
*        EQT_CAMERA__numero		: numero de la camera
*        EQT_CAMERA__numero_type	: numero du type de camera
*	 EQT_CAMERA_autoroute		: nom de l'autoroute de l'equipement
*	 EQT_CAMERA_pr_debut		: PR de debut de l'equipement en metres
*	 EQT_CAMERA_pr_fin		: PR de fin de l'equipement en metres
*	 EQT_CAMERA_sens		: sens de l'equipement sur l'autoroute
*
*  Remarque
*  --------
*  Un numero d'equipement est unique par type d'equipement.
*
*/


/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'EQT_CAMERA' and type = 'V')
	begin
	drop view EQT_CAMERA
	end
go


create view EQT_CAMERA
(
EQT_CAMERA__numero,
EQT_CAMERA__numero_type,
EQT_CAMERA_autoroute,
EQT_CAMERA_pr_debut,
EQT_CAMERA_pr_fin,
EQT_CAMERA_sens
)
as select
CAM1.camera,
EQT1.type,
RES1.nom,
CAM1.PR_debut,
CAM1.PR_fin,
CAM1.sens

from CFG..EQT_GEN EQT1,CFG..EQT_CAM_POS CAM1,CFG..RES_AUT RES1

where
/* jointure entre EQT_GEN et EQT_CAM_POS */
/* ------------------------------------- */
EQT1.numero=CAM1.camera
and 7=EQT1.type
/* jointure entre EQT_CAM_POS et RES_AUT */
/* ------------------------------------- */
and CAM1.autoroute*=RES1.numero
/* condition couverture globale */
/* and CAM1.numero=0 */

/* ajout des droits a public*/
grant all on EQT_CAMERA to public

go
