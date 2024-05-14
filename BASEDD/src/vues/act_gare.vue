/*X  Fichier : @(#)act_gare.vue	0.1      Release : 0.1        Date : 19/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER act_gare.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue ACT_GARE
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
/*   Vue : ACT_GARE	                                        */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()

/*
*  Fonctionalite de la vue
*  -------------------------
*  Contient les affichages en gare
*
*  Description des colonnes de la vue
*  ------------------------------------
*        ACT_GARE__numero		: numero d'une action affichage en gare
*        ACT_GARE__district		: numero du site de l'action
*        ACT_GARE_fmc_liee_numero	: numero de la fiche main courante cause de l'action
*        ACT_GARE_fmc_liee_district	: numero de cle de la fiche main courante cause de l'action
*	 ACT_GARE_nom			: nom de la gare
*	 ACT_GARE_message		: texte du message affiche sur le fourgon
*	 ACT_GARE_heure_debut		: heure de debut d'affichage
*	 ACT_GARE_heure_fin		: heure de fin d'affichage
*
*  Remarque
*  --------
*  Le numero d'action est unique par site (district). Pour selectionner une
*  action, il faut preciser son numero et son site.
*
*/


/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'ACT_GARE' and type = 'V')
	begin
	drop view ACT_GARE
	end
go


create view ACT_GARE
(
ACT_GARE__numero,
ACT_GARE__district,
ACT_GARE_fmc_liee_numero,
ACT_GARE_fmc_liee_district,
ACT_GARE_nom,
ACT_GARE_message,
ACT_GARE_heure_debut,
ACT_GARE_heure_fin
)
as select
GEN1.numero,
GEN1.sit,
GEN1.evenement,
GEN1.cle,
GAR1.nom_gare,
GAR1.affichette,
GAR1.horodate_debut,
GAR1.horodate_fin

from HIS..ACT_GEN GEN1,HIS..ACT_GAR GAR1

where
/* jointure entre ACT_GEN et ACT_GAR */
/* --------------------------------- */
GEN1.numero=GAR1.actio
and GEN1.sit=GAR1.sit

/* ajout des droits a public*/
grant all on ACT_GARE to public

go
