/*X Fichier fmc_opr.vue 1.0		Date : 16/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER fmc_opr.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue FMC_OPR
*
------------------------------------------------------
* HISTORIQUE :
*
* V-L   16/08/96        : creation
-----------------------------------------------------*/


/* ============================================================ */
/*   Nom de la base   :  MIGRAZUR                               */
/*   Nom de SGBD      :  SYBASE10                               */
/*   Date de creation :  01/08/96                               */
/* ============================================================ */

/* ============================================================ */
/*  VUE : FMC_OPR                                               */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()


/*
*  Fonctionalite de la vue
*  -------------------------
*  Vue des fiches main courantes operateurs
*
*  Description des colonnes de la vue
*  ------------------------------------
*        FMC_OPR__numero		: numero de la fiche main courante
*        FMC_OPR__district		: numero du district de la fiche main courante
*	 FMC_OPR_heure_debut		: no comment
*	 FMC_heure_fin			: no comment
*	 FMC_OPR_numero_operateur	: numero du poste operateur
*	 FMC_OPR_nom_operateur		: nom de l'operateur
*	 FMC_OPR_site			: numero du site de la fmc
*
*  Remarque
*  --------
*  Pour sélectionner une fmc, il faut préciser son numéro et sa clé car seul le couple
*  (numéro, clé) est unique sur tous les sites.
*
*/


/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'FMC_OPR' and type = 'V')
	begin
	drop view FMC_OPR
	end
go


create view FMC_OPR
(
FMC_OPR__numero,
FMC_OPR__district,
FMC_OPR_heure_debut,
FMC_OPR_heure_fin,
FMC_OPR_numero_operateur,
FMC_OPR_nom_operateur,
FMC_OPR_site
)
as select
GEN1.numero,
GEN1.cle,
GEN1.debut,
GEN1.fin,
OPR1.operateur,
NOM1.nom,
OPR1.sit

from HIS..FMC_GEN GEN1,HIS..FMC_OPR OPR1,CFG..OPR_NOM NOM1

where
/* realisation de la jointure entre FMC_GEN et FMC_OPR */
/* --------------------------------------------------- */
GEN1.numero=OPR1.numero
and GEN1.cle=OPR1.cle
/* realisation de la jointure entre FMC_OPR et OPR_NOM */
/* --------------------------------------------------- */
and OPR1.operateur*=NOM1.numero

grant all on FMC_OPR to public

go
