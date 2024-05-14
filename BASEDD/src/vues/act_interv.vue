/*X  Fichier : @(#)act_interv.vue	0.1      Release : 0.1        Date : 19/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER act_interv.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue ACT_INTERV
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
/*   Vue : ACT_INTERV	                                        */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()

/*
*  Fonctionalite de la vue
*  -------------------------
*  Contient les interventions depanneurs
*
*  Description des colonnes de la vue
*  ------------------------------------
*        ACT_INTERV__numero		: numero d'une intervention depanneur
*        ACT_INTERV__district		: numero du site de l'action
*        ACT_INTERV_fmc_liee_numero	: numero de la fiche main courante cause de l'action
*        ACT_INTERV_fmc_liee_district	: numero de cle de la fiche main courante cause de l'action
*	 ACT_INTERV_type		: libelle du type d'intervention(pompier,etc..)
*	 ACT_INTERV_nom			: nom de l'intervenant    
*        ACT_INTERV_heure_depart	: date de depart du site
*        ACT_INTERV_heure_arrivee	: date d'arrivee sur site
*	 ACT_INTERV_heure_fin		: heure de fin de l'intervention
*
*  Remarque
*  --------
*  Le numero d'action est unique par site (district). Pour selectionner une
*  action, il faut preciser son numero et son site.
*
*/


/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'ACT_INTERV' and type = 'V')
	begin
	drop view ACT_INTERV
	end
go


create view ACT_INTERV
(
ACT_INTERV__numero,
ACT_INTERV__district,
ACT_INTERV_fmc_liee_numero,
ACT_INTERV_fmc_liee_district,
ACT_INTERV_type,
ACT_INTERV_nom,
ACT_INTERV_heure_depart,
ACT_INTERV_heure_arrivee,
ACT_INTERV_heure_fin
)
as select
GEN1.numero,
GEN1.sit,
GEN1.evenement,
GEN1.cle,
DICO1.libelle,
INT1.nom,
GEN1.heure_lancement,
GEN1.heure_succes,
GEN1.heure_fin

from HIS..ACT_GEN GEN1,HIS..ACT_INT INT1,VUE..DICO DICO1

where
/* jointure entre ACT_GEN et ACT_INT */
/* --------------------------------- */
GEN1.numero=INT1.actio
and GEN1.sit=INT1.sit
/* jointure entre ACT_INT et DICO */
/* ------------------------------ */
and DICO1.code=INT1.type
and 11=DICO1.notyp

/* ajout des droits a public*/
grant all on ACT_INTERV to public

go
