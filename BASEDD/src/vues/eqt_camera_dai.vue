/*X  Fichier : @(#)eqt_bra.vue	0.1      Release : 0.1        Date : 20/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER eqt_camera_dai.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue EQT_CAMERA_DAI
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
/*   Vue : EQT_CAMERA_DAI                                       */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()

/*
*  Fonctionalite de la vue
*  -------------------------
*  Vue des cameras DAI
*
*  Description des colonnes de la vue
*  ------------------------------------
*        EQT_CAMERA_DAI__numero		: numero de la camera
*        EQT_CAMERA_DAI__numero_type	: numero du type de camera
*	 EQT_CAMERA_DAI_autoroute	: nom de l'autoroute de l'equipement
*	 EQT_CAMERA_DAI_pr_debut	: PR de debut de l'equipement en metres
*	 EQT_CAMERA_DAI_pr_fin		: PR de fin de l'equipement en metres
*	 EQT_CAMERA_DAI_sens		: sens de l'equipement sur l'autoroute
*
*  Remarque
*  --------
*  Un numero d'equipement est unique par type d'equipement.
*
*/


/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'EQT_CAMERA_DAI' and type = 'V')
	begin
	drop view EQT_CAMERA_DAI
	end
go


create view EQT_CAMERA_DAI
(
EQT_CAMERA_DAI__numero,
EQT_CAMERA_DAI__numero_type,
EQT_CAMERA_DAI_autoroute,
EQT_CAMERA_DAI_pr_debut,
EQT_CAMERA_DAI_pr_fin,
EQT_CAMERA_DAI_sens
)
as select
ANA1.numero,
EQT1.type,
RES1.nom,
EQT1.PR,
ANA1.PR_fin,
EQT1.sens

from CFG..EQT_GEN EQT1,CFG..EQT_ANA ANA1,CFG..RES_AUT RES1

where
/* jointure entre EQT_GEN et EQT_ANA */
/* --------------------------------- */
EQT1.numero=ANA1.numero
and 27=EQT1.type
/* jointure entre EQT_GEN et RES_AUT */
/* ------------------------------------- */
and EQT1.autoroute*=RES1.numero

/* ajout des droits a public*/
grant all on EQT_CAMERA_DAI to public

go
