/*X Fichier fmc_com.vue 1.0		Date : 16/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER fmc_com.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue FMC_COM
*
------------------------------------------------------
* HISTORIQUE :
*
* V-L   01/08/96        : creation
-----------------------------------------------------*/


/* ============================================================ */
/*   Nom de la base   :  MIGRAZUR                               */
/*   Nom de SGBD      :  SYBASE10                               */
/*   Date de creation :  16/08/96                               */
/* ============================================================ */

/* ============================================================ */
/*  VUE : FMC_COM                                               */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()


/*
*  Fonctionalite de la vue
*  -------------------------
*  Vue des commentaires dans les fiches main courantes 
*
*  Description des colonnes de la vue
*  ------------------------------------
*        FMC_COM__numero		: numero de la fiche main courante
*        FMC_COM__district		: numero du district de la fiche main courante
*        FMC_COM_type			: nom du type de la fmc lu dans TYP_FMC.nom
*	 FMC_COM_heure_debut_prevu	: no comment
*	 FMC_COM_heure_debut		: no comment
*	 FMC_COM_heure_fin_prevue	: no comment
*	 FMC_COM_heure_fin		: no comment
*        FMC_COM_fmc_cause_numero	: numero de la fiche main courante cause de cette fmc
*        FMC_COM_fmc_cause_district	: numero du district de la fiche main courante cause de cette fmc
*        FMC_COM_loc_autoroute		: nom de l'autoroute de la fmc (deniere localisation) lue ds RES_AUT
*	 FMC_COM_loc_pr			: pr de la fmc en metres (derniere localisation)
*	 FMC_COM_loc_sens		: sens de la fmc (derniere localisation)
*	 FMC_COM_origine_creation	: commentaire sur l'origine de la fmc
*	 FMC_COM_alerte_numero		: numero de l'alerte qui a occasionne la creation de la fmc
*	 FMC_COM_alerte_site		: code du site de l'alerte
*	 FMC_COM_alerte_type		: code indiquant le type d'alerte
*	 FMC_COM_alerte_type_equipement	: code indiquant le type d'equipement generant l'alerte
*	 FMC_COM_alerte_equipement	: numero de l'equipement generant l'alerte
*	 FMC_COM_alerte_commentaire	: commentaire sur l'alerte
*	 FMC_COM_numero_commentaire	: position du texte de commentaire pour une fmc donnee
*	 FMC_COM_texte_commentaire	: portion de texte en commentaire d'une fmc donnee
*
*  Remarque
*  --------
*  Pour sélectionner une fmc, il faut préciser son numéro et sa clé car seul le couple
*  (numéro, clé) est unique sur tous les sites.
*  Un equipement generant une alerte est clairement identifie par son numero et le numero
*  du type d'equipement. En effet le numero d'equipement est unique par type d'equipement.
*
*/


/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'FMC_COM' and type = 'V')
	begin
	drop view FMC_COM
	end
go


create view FMC_COM
(
FMC_COM__numero,
FMC_COM__district,
FMC_COM_type,
FMC_COM_heure_debut_prevu,
FMC_COM_heure_debut,
FMC_COM_heure_fin_prevue,
FMC_COM_heure_fin,
FMC_COM_fmc_cause_numero,
FMC_COM_fmc_cause_district,
FMC_COM_loc_autoroute,
FMC_COM_loc_pr,
FMC_COM_loc_sens,
FMC_COM_origine_creation,
FMC_COM_alerte_numero,
FMC_COM_alerte_site,
FMC_COM_alerte_type,
FMC_COM_alerte_type_equipement,
FMC_COM_alerte_equipement,
FMC_COM_alerte_commentaire,
FMC_COM_numero_commentaire,
FMC_COM_texte_commentaire
)
as select
GEN1.numero,
GEN1.cle,
TYP1.nom,
GEN1.debut_prevu,
GEN1.debut,
GEN1.fin_prevue,
GEN1.fin,
GEN1.cause,
GEN1.cle_cause,
RES1.nom,
HIS1.PR,
HIS1.sens,
GEN1.origine_creation,
GEN1.alerte,
GEN1.sit,
ALT1.type,
ALT1.type_equipement,
ALT1.equipement,
ALT1.texte,
COM1.numero,
COM1.texte

from HIS..FMC_GEN GEN1,HIS..FMC_HIS HIS1,CFG..TYP_FMC TYP1,
CFG..RES_AUT RES1,HIS..ALT_EXP ALT1,HIS..FMC_COM COM1

where
/* realisation de la jointure entre FMC_GEN et FMC_COM */
/* --------------------------------------------------- */
GEN1.numero=COM1.evenement
and GEN1.cle=COM1.cle
/* realisation de la jointure entre FMC_GEN et FMC_HIS */
/* --------------------------------------------------- */
and GEN1.numero=HIS1.numero
and GEN1.cle=HIS1.cle
/* realisation de la jointure entre FMC_GEN et TYP_FMC */
/* --------------------------------------------------- */
and GEN1.type=TYP1.numero
/* realisation de la jointure entre FMC_HIS et RES_AUT */
/* --------------------------------------------------- */
and HIS1.autoroute=RES1.numero
/* realisation de la jointure externe entre FMC_GEN et ALT_EXP */
/* On veut avoir toutes les fmc meme celles qui n'ont pas d'alerte */
/* --------------------------------------------------------------- */
and GEN1.alerte*=ALT1.numero
and GEN1.cle*=ALT1.sit
/* selection de l'historique pour la derniere validation */
/* ----------------------------------------------------- */
and HIS1.horodate_validation in
(
select max(HIS2.horodate_validation)
from HIS..FMC_HIS HIS2
where
HIS2.numero=HIS1.numero
and HIS2.cle=HIS1.cle
group by HIS2.numero,HIS2.cle
)

grant all on FMC_COM to public

go
