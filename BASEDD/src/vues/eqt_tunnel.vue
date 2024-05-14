/*X  Fichier : @(#)eqt_bra.vue	0.1      Release : 0.1        Date : 20/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER eqt_tunnel.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue EQT_TUNNEL
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
/*   Vue : EQT_TUNNEL	                                        */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()

/*
*  Fonctionalite de la vue
*  -------------------------
*  Vue des tunnels de NICE
*
*  Description des colonnes de la vue
*  ------------------------------------
*        EQT_TUNNEL__numero			: numero de l'equipement
*        EQT_TUNNEL__numero_type		: numero du type de tunnel
*	 EQT_TUNNEL_autoroute			: nom de l'autoroute de l'equipement
*	 EQT_TUNNEL_pr				: PR de l'equipement en metres
*	 EQT_TUNNELpr_influence			: PR de la zone d'influence
*	 EQT_TUNNEL_sens			: sens de l'equipement sur l'autoroute
*
*  Remarque
*  --------
*  Un numero d'equipement est unique par type d'equipement.
*
*/


/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'EQT_TUNNEL' and type = 'V')
	begin
	drop view EQT_TUNNEL
	end
go


create view EQT_TUNNEL
(
EQT_TUNNEL__numero,
EQT_TUNNEL__numero_type,
EQT_TUNNEL_autoroute,
EQT_TUNNEL_pr,
EQT_TUNNEL_pr_influence,
EQT_TUNNEL_sens
)
as select
EQT1.numero,
EQT1.type,
RES1.nom,
EQT1.PR,
TUB1.PR_Influence,
EQT1.sens

from CFG..EQT_GEN EQT1,CFG..EQT_TUB TUB1,CFG..RES_AUT RES1

where
/* jointure entre EQT_GEN et EQT_TUB */
/* --------------------------------- */
EQT1.numero=TUB1.numero
and EQT1.type=3
/* jointure entre EQT_GEN et RES_AUT */
/* --------------------------------- */
and EQT1.autoroute*=RES1.numero

/* ajout des droits a public*/
grant all on EQT_TUNNEL to public

go
