/*X Fichier fmc_acc.vue 1.0		Date : 08/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER fmc_acc.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue FMC_ACC
*
------------------------------------------------------
* HISTORIQUE :
*
* V-L   08/08/96        : creation
-----------------------------------------------------*/


/* ============================================================ */
/*   Nom de la base   :  MIGRAZUR                               */
/*   Nom de SGBD      :  SYBASE10                               */
/*   Date de creation :  08/08/96                               */
/* ============================================================ */

/* ============================================================ */
/*  VUE : FMC_ACC                                           */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()


/*
*  Fonctionalite de la vue
*  -------------------------
*  Contient les caracteristiques générales des fmc conernant les evenements de type accident
*
*  Description des colonnes de la vue
*  ------------------------------------
*        FMC_ACC__numero		: numero de la fiche main courante
*        FMC_ACC__district		: numero du district de la fiche main courante
*	 FMC_ACC_heure_debut		: no comment
*	 FMC_ACC_heure_fin_prevue	: no comment
*	 FMC_ACC_heure_fin		: no comment
*        FMC_ACC_fmc_cause_numero	: numero de la fiche main courante cause de cette fmc
*        FMC_ACC_fmc_cause_district	: numero du district de la fiche main courante cause de cette fmc
*        FMC_ACC_loc_autoroute		: nom de l'autoroute de la fmc (deniere localisation) lue ds RES_AUT
*	 FMC_ACC_loc_pr			: pr de la fmc en metres (derniere localisation)
*	 FMC_ACC_loc_sens		: sens de la fmc (derniere localisation)
*	 FMC_ACC_origine_creation	: commentaire sur l'origine de la fmc
*	 FMC_ACC_alerte_numero		: numero de l'alerte qui a occasionne la creation de la fmc
*	 FMC_ACC_alerte_site		: code du site de l'alerte
*	 FMC_ACC_alerte_type		: code indiquant le type d'alerte
*	 FMC_ACC_alerte_type_equipement	: code indiquant le type d'equipement generant l'alerte
*	 FMC_ACC_alerte_equipement	: numero de l'equipement generant l'alerte
*	 FMC_ACC_alerte_commentaire	: commentaire sur l'alerte
*	 FMC_ACC_nombre_vl		: nombre de vehicules legers impliques
*	 FMC_ACC_nombre_pl		: nombre de poids lourds impliques
*	 FMC_ACC_nombre_remorques	: nombre de remorques impliques
*	 FMC_ACC_nombre_cars		: nombre de cars impliques
*	 FMC_ACC_nombre_motos		: nombre de motos impliques
*	 FMC_ACC_victimes_legeres	: nombre de blesses legers
*	 FMC_ACC_victimes_graves	: nombre de blesses graves
*	 FMC_ACC_victimes_mortes	: nombre de morts
*	 FMC_ACC_vehicule_ESCOTA	: vehicule ESCOTA implique dans l'accident (1) ou non (0)
*	 FMC_ACC_personnel_ESCOTA	: personnel ESCOTA implique dans l'accident (1) ou non (0)
*	 FMC_ACC_personnalite		: personnalite implique dans l'accident (1) ou non (0)
*	 FMC_ACC_degats_domaine		: oui (1) ou non (0)
*	 FMC_ACC_matieres_dangereuses	: oui (1) ou non (0)
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
if exists (select * from sysobjects where name = "FMC_ACC" and type = 'V')
	begin
	drop view FMC_ACC
	end
go


create view FMC_ACC
(
FMC_ACC__numero,
FMC_ACC__district,
FMC_ACC_heure_debut,
FMC_ACC_heure_fin_prevue,
FMC_ACC_heure_fin,
FMC_ACC_fmc_cause_numero,
FMC_ACC_fmc_cause_district,
FMC_ACC_loc_autoroute,
FMC_ACC_loc_pr,
FMC_ACC_loc_sens,
FMC_ACC_origine_creation,
FMC_ACC_alerte_numero,
FMC_ACC_alerte_site,
FMC_ACC_alerte_type,
FMC_ACC_alerte_type_equipement,
FMC_ACC_alerte_equipement,
FMC_ACC_alerte_commentaire,
FMC_ACC_nombre_vl,
FMC_ACC_nombre_pl,
FMC_ACC_nombre_remorques,
FMC_ACC_nombre_cars,
FMC_ACC_nombre_motos,
FMC_ACC_victimes_legeres,
FMC_ACC_victimes_graves,
FMC_ACC_victimes_mortes,
FMC_ACC_vehicule_ESCOTA,
FMC_ACC_personnel_ESCOTA,
FMC_ACC_personnalite,
FMC_ACC_degats_domaine,
FMC_ACC_matieres_dangereuses
)
as select
GEN1.numero,
GEN1.cle,
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
ACC1.nombre_de_vl,
ACC1.nombre_de_pl,
ACC1.nombre_de_remorques,
ACC1.nombre_de_car,
ACC1.nombre_de_moto,
ACC1.blesses_legers,
ACC1.blesses_graves,
ACC1.morts,
ACC1.vehicule_ESCOTA,
ACC1.personnel_ESCOTA,
ACC1.personnalites,
ACC1.degats_domaine,
ACC1.matieres_dangereuses

from HIS..FMC_GEN GEN1,HIS..FMC_HIS HIS1,HIS..FMC_ACC ACC1,
CFG..RES_AUT RES1,HIS..ALT_EXP ALT1

where
/* realisation de la jointure entre FMC_GEN et FMC_HIS */
/* --------------------------------------------------- */
GEN1.numero=HIS1.numero
and GEN1.cle=HIS1.cle
/* realisation de la jointure entre FMC_GEN et FMC_ACC */
/* --------------------------------------------------- */
and GEN1.numero=ACC1.numero
and GEN1.cle=ACC1.cle
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

grant all on FMC_ACC to public

go

