/*X  Fichier : @(#)act_depan.vue	0.1      Release : 0.1        Date : 19/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER act_depan.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue ACT_DEPAN
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
/*   Vue : ACT_DEPAN	                                        */
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
*        ACT_DEPAN__numero		: numero d'une intervention depanneur
*        ACT_DEPAN__district		: numero du site de l'action
*        ACT_DEPAN_fmc_liee_numero	: numero de la fiche main courante cause de l'action
*        ACT_DEPAN_fmc_liee_district	: numero de cle de la fiche main courante cause de l'action
*	 ACT_DEPAN_nom			: nom du depanneur
*	 ACT_DEPAN_resultat		: commentaire indiquant le resultat du depannage    
*        ACT_DEPAN_heure_depart		: date de depart du site
*	 ACT_DEPAN_heure_appel		: heure d'appel du depanneur
*        ACT_DEPAN_heure_arrivee	: date d'arrivee sur site
*	 ACT_DEPAN_heure_annulation	: heure d'annulation du depannage
*	 ACT_DEPAN_renfort		: renfort necessaire (1) ou non (0)
*	 ACT_DEPAN_presence_patrouille	: presence (1) ou non (0) d'une patrouille
*	 ACT_DEPAN_contact_pc		: le garagiste doit il contacter le pc oui (1) ou non (0)
*
*  Remarque
*  --------
*  Le numero d'action est unique par site (district). Pour selectionner une
*  action, il faut preciser son numero et son site.
*
*/


/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'ACT_DEPAN' and type = 'V')
	begin
	drop view ACT_DEPAN
	end
go


create view ACT_DEPAN
(
ACT_DEPAN__numero,
ACT_DEPAN__district,
ACT_DEPAN_fmc_liee_numero,
ACT_DEPAN_fmc_liee_district,
ACT_DEPAN_nom,
ACT_DEPAN_resultat,
ACT_DEPAN_heure_depart,
ACT_DEPAN_heure_appel,
ACT_DEPAN_heure_arrivee,
ACT_DEPAN_heure_annulation,
ACT_DEPAN_renfort,
ACT_DEPAN_presence_patrouille,
ACT_DEPAN_contact_pc
)
as select
GEN1.numero,
GEN1.sit,
GEN1.evenement,
GEN1.cle,
DEP1.nom,
DEP1.resultat,
GEN1.heure_succes,
GEN1.heure_lancement,
GEN1.heure_lancement_fin,
GEN1.heure_echec,
DEP1.renfort,
DEP1.patrouille_presente,
DEP1.garagiste_doit_contacter_pc

from HIS..ACT_GEN GEN1,HIS..ACT_DEP DEP1

where
/* jointure entre ACT_GEN et ACT_DEP */
/* --------------------------------- */
GEN1.numero=DEP1.actio
and GEN1.sit=DEP1.sit

/* ajout des droits a public*/
grant all on ACT_DEPAN to public

go
