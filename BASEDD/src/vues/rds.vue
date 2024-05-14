/*X  Fichier : @(#)rds.vue	0.1      Release : 0.1        Date : 12/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER rds.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue RDS
*
------------------------------------------------------
* HISTORIQUE :
*
* L.V.	12/08/96	: Creation
-----------------------------------------------------*/


/* ============================================================ */
/*   Nom de la base   :  MIGRAZUR                               */
/*   Nom de SGBD      :  SYBASE10                               */
/*   Date de creation :  12/08/96                               */
/* ============================================================ */

/* ============================================================ */
/*   Vue : RDS	                                                */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()

/*
*  Fonctionalite de la vue
*  -------------------------
*  Contient les caracteristiques generales des actions
*
*  Description des colonnes de la vue
*  ------------------------------------
*        numero		: numero point rds
*	 autoroute	: numero autoroute
*	 pr		: pr sur autoroute
*	 nom		: nom du point
*
*/

/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'RDS' and type = 'V')
	begin
	drop view RDS
	end
go


create view RDS
(
numero,
autoroute,
pr,
nom)
as select
NOM.numero,
AUT.numero,
PR.pr_km*1000+PR.pr_m,
NOM.nom

from CFG..NOM_RDS NOM, CFG..PR_RDS PR, CFG..RES_AUT AUT

where
NOM.numero=PR.numero and
PR.autoroute=AUT.nom


/* ajout des droits a public*/
grant all on RDS to public

go
