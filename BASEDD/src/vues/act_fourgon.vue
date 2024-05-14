/*X  Fichier : @(#)act_fourgon.vue	0.1      Release : 0.1        Date : 19/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER act_fourgon.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue ACT_FOURGON
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
/*   Vue : ACT_FOURGON	                                        */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()

/*
*  Fonctionalite de la vue
*  -------------------------
*  Contient les affichages pMV sur fourgon
*
*  Description des colonnes de la vue
*  ------------------------------------
*        ACT_FOURGON__numero		: numero de l'action affichage sur fourgon
*        ACT_FOURGON__district		: numero du site de l'action
*        ACT_FOURGON_fmc_liee_numero	: numero de la fiche main courante cause de l'action
*        ACT_FOURGON_fmc_liee_district	: numero de cle de la fiche main courante cause de l'action
*	 ACT_FOURGON_pr			: PR du fourgon
*	 ACT_FOURGON_sens		: sens du fourgon
*	 ACT_FOURGON_message		: texte du message affiche sur le fourgon
*	 ACT_FOURGON_heure_debut	: heure de debut d'affichage
*	 ACT_FOURGON_heure_fin		: heure de fin d'affichage
*
*  Remarque
*  --------
*  Le numero d'action est unique par site (district). Pour selectionner une
*  action, il faut preciser son numero et son site.
*
*/


/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'ACT_FOURGON' and type = 'V')
	begin
	drop view ACT_FOURGON
	end
go


create view ACT_FOURGON
(
ACT_FOURGON__numero,
ACT_FOURGON__district,
ACT_FOURGON_fmc_liee_numero,
ACT_FOURGON_fmc_liee_district,
ACT_FOURGON_pr,
ACT_FOURGON_sens,
ACT_FOURGON_message,
ACT_FOURGON_heure_debut,
ACT_FOURGON_heure_fin
)
as select
GEN1.numero,
GEN1.sit,
GEN1.evenement,
GEN1.cle,
FRG1.PR,
FRG1.sens,
FRG1.message,
GEN1.heure_succes,
GEN1.heure_fin

from HIS..ACT_GEN GEN1,HIS..ACT_FRG FRG1

where
/* jointure entre ACT_GEN et ACT_FRG */
/* --------------------------------- */
GEN1.numero=FRG1.actio
and GEN1.sit=FRG1.sit

/* ajout des droits a public*/
grant all on ACT_FOURGON to public

go
