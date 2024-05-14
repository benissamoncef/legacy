/*X  Fichier : @(#)act_tfm.vue	0.1      Release : 0.1        Date : 19/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER act_tfm.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue ACT_TFM
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
/*   Vue : ACT_TFM	                                        */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()

/*
*  Fonctionalite de la vue
*  -------------------------
*  Vue des messages Traffic FM
*
*  Description des colonnes de la vue
*  ------------------------------------
*        ACT_TFM__numero		: numero d'une action diffuser message Traffic FM
*        ACT_TFM__district		: numero du site de l'action
*        ACT_TFM_fmc_liee_numero	: numero de la fiche main courante cause de l'action
*        ACT_TFM_fmc_liee_district	: numero de cle de la fiche main courante cause de l'action
*	 ACT_TFM_heure_envoi		: heure d'envoi du message par l'operateur du CI
*	 ACT_TFM_huere_fin		: heure de debut de diffusion
*	 ACT_TFM_message		: morceau de texte du message
*	 ACT_TFM_ordre			: ordre des morceaux de message
*	 ACT_TFM_nombre_diffusions	: nombre de diffusions du message
*
*  Remarque
*  --------
*  Le numero d'action est unique par site (district). Pour selectionner une
*  action, il faut preciser son numero et son site.
*
*/


/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'ACT_TFM' and type = 'V')
	begin
	drop view ACT_TFM
	end
go


create view ACT_TFM
(
ACT_TFM__numero,
ACT_TFM__district,
ACT_TFM_fmc_liee_numero,
ACT_TFM_fmc_liee_district,
ACT_TFM_heure_envoi,
ACT_TFM_heure_fin,
ACT_TFM_message,
ACT_TFM_ordre,
ACT_TFM_nombre_diffusions
)
as select
GEN1.numero,
GEN1.sit,
GEN1.evenement,
GEN1.cle,
GEN1.heure_succes,
GEN1.heure_fin,
TXT1.texte,
(100*TXT1.actio)+TXT1.morceau,
TFM1.nombre_diffusions

from HIS..ACT_GEN GEN1,HIS..ACT_TXT_TFM TXT1,HIS..ACT_TFM TFM1

where
/* jointure entre ACT_GEN et ACT_TFM */
/* --------------------------------- */
GEN1.numero=TFM1.actio
and GEN1.sit=TFM1.sit
/* jointure entre ACT_GEN et ACT_TXT_TFM */
/* ------------------------------------- */
and GEN1.numero=TXT1.actio
and GEN1.sit=TXT1.sit

/* ajout des droits a public*/
grant all on ACT_TFM to public

go
