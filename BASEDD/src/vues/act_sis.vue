/*X  Fichier : @(#)act_sis.vue	0.1      Release : 0.1        Date : 19/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER act_sis.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue ACT_SIS
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
/*   Vue : ACT_SIS	                                        */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()

/*
*  Fonctionalite de la vue
*  -------------------------
*  Contient les interventions de la patrouille SIS
*
*  Description des colonnes de la vue
*  ------------------------------------
*        ACT_SIS__numero		: numero d'une intervention SIS
*        ACT_SIS__district		: numero du site de l'action
*        ACT_SIS_fmc_liee_numero	: numero de la fiche main courante cause de l'action
*        ACT_SIS_fmc_liee_district	: numero de cle de la fiche main courante cause de l'action
*	 ACT_SIS_nom			: nom de la patrouille
*        ACT_SIS_heure_depart		: date de depart du site
*        ACT_SIS_heure_arrivee		: date d'arrivee sur site
*	 ACT_SIS_heure_fin		: heure de fin de l'intervention
*	 ACT_SIS_balisage		: presence (1) ou absence de balisage (0)
*	 ACT_SIS_bal_pr_debut		: pr du debut de balisage, en metres
*	 ACT_SIS_bal_pr_fin		: pr de fin du balisage, en metres
*	 ACT_SIS_bal_heure_debut	: heure de debut de balisage
*	 ACT_SIS_bal_heure_fin		: heure de fin de balisage
*
*  Remarque
*  --------
*  Le numero d'action est unique par site (district). Pour selectionner une
*  action, il faut preciser son numero et son site.
*
*/


/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'ACT_SIS' and type = 'V')
	begin
	drop view ACT_SIS
	end
go


create view ACT_SIS
(
ACT_SIS__numero,
ACT_SIS__district,
ACT_SIS_fmc_liee_numero,
ACT_SIS_fmc_liee_district,
ACT_SIS_nom,
ACT_SIS_heure_depart,
ACT_SIS_heure_arrivee,
ACT_SIS_heure_fin,
ACT_SIS_balisage,
ACT_SIS_bal_pr_debut,
ACT_SIS_bal_pr_fin,
ACT_SIS_bal_heure_debut,
ACT_SIS_bal_heure_fin
)
as select
GEN1.numero,
GEN1.sit,
GEN1.evenement,
GEN1.cle,
SIS1.nom_patrouille,
GEN1.heure_lancement,
GEN1.heure_succes,
GEN1.heure_fin,
SIS1.pose_balisage,
SIS1.PR_debut,
SIS1.PR_fin,
SIS1.debut_balisage,
SIS1.fin_balisage

from HIS..ACT_GEN GEN1,HIS..ACT_SIS SIS1

where
/* jointure entre ACT_GEN et ACT_SIS */
/* --------------------------------- */
GEN1.numero=SIS1.actio
and GEN1.sit=SIS1.sit

/* ajout des droits a public*/
grant all on ACT_SIS to public

go
