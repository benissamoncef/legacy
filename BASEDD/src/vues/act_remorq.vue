/*X  Fichier : @(#)act_remorq.vue	0.1      Release : 0.1        Date : 19/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER act_remorq.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue ACT_REMORQ
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
/*   Vue : ACT_REMORQ	                                        */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()

/*
*  Fonctionalite de la vue
*  -------------------------
*  Contient les affichages PMV sur remorque
*
*  Description des colonnes de la vue
*  ------------------------------------
*        ACT_REMORQ__numero		: numero de l'action affichage sur remorque
*        ACT_REMORQ__district		: numero du site de l'action
*        ACT_REMORQ_fmc_liee_numero	: numero de la fiche main courante cause de l'action
*        ACT_REMORQ_fmc_liee_district	: numero de cle de la fiche main courante cause de l'action
*	 ACT_REMORQ_pr			: PR de la remorque
*	 ACT_REMORQ_sens		: sens de la remorque
*	 ACT_REMORQ_message		: texte du message affiche sur le fourgon
*	 ACT_REMORQ_heure_debut		: heure de debut d'affichage
*	 ACT_REMORQ_heure_fin		: heure de fin d'affichage
*
*  Remarque
*  --------
*  Le numero d'action est unique par site (district). Pour selectionner une
*  action, il faut preciser son numero et son site.
*
*/


/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'ACT_REMORQ' and type = 'V')
	begin
	drop view ACT_REMORQ
	end
go


create view ACT_REMORQ
(
ACT_REMORQ__numero,
ACT_REMORQ__district,
ACT_REMORQ_fmc_liee_numero,
ACT_REMORQ_fmc_liee_district,
ACT_REMORQ_pr,
ACT_REMORQ_sens,
ACT_REMORQ_message,
ACT_REMORQ_heure_debut,
ACT_REMORQ_heure_fin
)
as select
GEN1.numero,
GEN1.sit,
GEN1.evenement,
GEN1.cle,
RMQ1.PR,
RMQ1.sens,
RMQ1.message,
GEN1.heure_succes,
GEN1.heure_fin

from HIS..ACT_GEN GEN1,HIS..ACT_RMQ RMQ1

where
/* jointure entre ACT_GEN et ACT_RMQ */
/* --------------------------------- */
GEN1.numero=RMQ1.actio
and GEN1.sit=RMQ1.sit

/* ajout des droits a public*/
grant all on ACT_REMORQ to public

go
